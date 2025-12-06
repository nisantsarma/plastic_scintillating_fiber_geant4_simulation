#include "run.hh"

PMRunAction::PMRunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();

    //histograms
    analysisManager->CreateH1("Edep", "Deposited energy in Scintillator per event",
                              100, 0., 2000. * keV);

    //ntuple 0: step-level info
    analysisManager->CreateNtuple("StepData", "Step-level hits");
    analysisManager->CreateNtupleIColumn("eventID");       // col 0
    analysisManager->CreateNtupleIColumn("detectorID");    // col 1
    analysisManager->CreateNtupleDColumn("Edep_keV");      // col 2
    analysisManager->CreateNtupleDColumn("x_mm");          // col 3
    analysisManager->CreateNtupleDColumn("y_mm");          // col 4
    analysisManager->CreateNtupleDColumn("z_mm");          // col 5
    analysisManager->CreateNtupleDColumn("time_ns");       // col 6
    analysisManager->FinishNtuple(); // ntuple 0

    //ntuple 1: event-level info
    analysisManager->CreateNtuple("EventSummary", "Event-level totals");
    analysisManager->CreateNtupleIColumn("detectorID");          // col 0
    analysisManager->CreateNtupleDColumn("totalE_keV");          // col 1
    analysisManager->CreateNtupleDColumn("firstHitTime_ns");     // col 2
    analysisManager->CreateNtupleDColumn("dose_Gy");             // col 3
    analysisManager->CreateNtupleDColumn("absorptionEff");       // col 4 (Ta)
    analysisManager->CreateNtupleDColumn("generatedPhotons");    // col 5
    analysisManager->CreateNtupleDColumn("escapedPhotons");      // col 6
    analysisManager->CreateNtupleDColumn("transmissionEff");     // col 7 (Tc·Tt)
    analysisManager->CreateNtupleDColumn("detectionEff");        // col 8 (DE)
    analysisManager->FinishNtuple(); // ntuple 1
}

PMRunAction::~PMRunAction() {}

void PMRunAction::BeginOfRunAction(const G4Run *run)
{
    auto analysisManager = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    G4String fileName = "output" + strRunID.str() + ".root";
    analysisManager->OpenFile(fileName);

    G4cout << " Run " << runID << " started. Output file: " << fileName << G4endl;
}

void PMRunAction::EndOfRunAction(const G4Run *run)
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();

    G4int runID = run->GetRunID();
    G4cout << " Run " << runID << " finished and data saved " << G4endl;
}
