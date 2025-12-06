#include "detector.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4UnitsTable.hh"

PMSensitiveDetector::PMSensitiveDetector(G4String name, G4double detectorMass, G4int detectorID)
    : G4VSensitiveDetector(name),
      fTotalEnergyDeposited(0.),
      fFirstHitTime(-1.),
      fDetectorMass(detectorMass),
      fDetectorID(detectorID),
      fIncidentGammas(0),
      fInteractingGammas(0),
      fGeneratedPhotons(0),
      fEscapedPhotons(0)
{
    fDetectorName = name;
}

PMSensitiveDetector::~PMSensitiveDetector() {}

void PMSensitiveDetector::Initialize(G4HCofThisEvent *)
{
    fTotalEnergyDeposited = 0.;
    fFirstHitTime = -1.;
    fIncidentGammas = 0;
    fInteractingGammas = 0;
    fGeneratedPhotons = 0;
    fEscapedPhotons = 0;
}

G4bool PMSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    auto analysisManager = G4AnalysisManager::Instance();
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    auto track = aStep->GetTrack();
    auto particleName = track->GetDefinition()->GetParticleName();

    //count incident gammas entering PSF volume
    if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary && particleName == "gamma"){
        
        fIncidentGammas++;
        G4cout << "Gamma entered PSF at z="
               << aStep->GetPreStepPoint()->GetPosition().z()/mm << " mm"
               << G4endl;
    }

    //energy deposition (gamma OR secondaries)
    G4double energyDeposited = aStep->GetTotalEnergyDeposit();
    if (energyDeposited > 0.) {
        fTotalEnergyDeposited += energyDeposited;

        //mark gamma as interacted if energy is deposited
        if (fInteractingGammas == 0 && fIncidentGammas > 0) {
            fInteractingGammas = 1;
        }

        if (fFirstHitTime < 0.) {
            fFirstHitTime = aStep->GetPreStepPoint()->GetGlobalTime();
        }

        //step-level ntuple
        G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
        G4double globalTime = aStep->GetPreStepPoint()->GetGlobalTime();

        analysisManager->FillNtupleIColumn(0, 0, eventID);
        analysisManager->FillNtupleIColumn(0, 1, fDetectorID);
        analysisManager->FillNtupleDColumn(0, 2, energyDeposited / keV);
        analysisManager->FillNtupleDColumn(0, 3, pos.x() / mm);
        analysisManager->FillNtupleDColumn(0, 4, pos.y() / mm);
        analysisManager->FillNtupleDColumn(0, 5, pos.z() / mm);
        analysisManager->FillNtupleDColumn(0, 6, globalTime / ns);
        analysisManager->AddNtupleRow(0);
    }

    //optical photon bookkeeping
    if (particleName == "opticalphoton") {
        //count all optical photons created
        if (track->GetCurrentStepNumber() == 1) {
            fGeneratedPhotons++;
        }

        //  Photon escapes fiber end (at +Z face)
        if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
            if (aStep->GetPostStepPoint()->GetPosition().z() > 0.5*100*mm - 1*um) {
                fEscapedPhotons++;
                track->SetTrackStatus(fStopAndKill); //kill after detection
            }
        }
    }

    return true;
}


void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    auto analysisManager = G4AnalysisManager::Instance();

    //dose
    G4double dose;
    
    if (fDetectorMass > 0.0) {
        dose = (fTotalEnergyDeposited / joule) / (fDetectorMass / kg);
    }
    else {
        dose = 0.0;
    }

    //absorption efficiency Ta
    G4double absorptionEff;

    if (fIncidentGammas > 0) {
        absorptionEff = (double)fInteractingGammas / (double)fIncidentGammas;
    }
    else{
        absorptionEff = 0.0;
    }


    //transmission efficiency Tc*Tt
    G4double transmissionEff;
    
    if (fGeneratedPhotons > 0) {
        transmissionEff = (double)fEscapedPhotons / (double)fGeneratedPhotons;
    }
    else {
        transmissionEff = 0.0;
    }


    //overall detection efficiency DE = Ta * TcTt
    G4double detectionEff = absorptionEff * transmissionEff;

    //event-level ntuple
    analysisManager->FillNtupleIColumn(1, 0, fDetectorID);
    analysisManager->FillNtupleDColumn(1, 1, fTotalEnergyDeposited / keV);
    analysisManager->FillNtupleDColumn(1, 2, fFirstHitTime / ns);
    analysisManager->FillNtupleDColumn(1, 3, dose / gray);
    analysisManager->FillNtupleDColumn(1, 4, absorptionEff);
    analysisManager->FillNtupleDColumn(1, 5, fGeneratedPhotons);
    analysisManager->FillNtupleDColumn(1, 6, fEscapedPhotons);
    analysisManager->FillNtupleDColumn(1, 7, transmissionEff);
    analysisManager->FillNtupleDColumn(1, 8, detectionEff);
    analysisManager->AddNtupleRow(1);

    if (fDetectorID == 0) {
        G4cout << "[" << fDetectorName << "] "
               << "Edep = " << fTotalEnergyDeposited/keV << " keV, "
               << "AbsEff = " << absorptionEff << ", "
               << "Gen = " << fGeneratedPhotons << ", "
               << "Esc = " << fEscapedPhotons << ", "
               << "TransEff = " << transmissionEff << ", "
               << "DE = " << detectionEff
               << G4endl;
    }
}
