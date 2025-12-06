#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"



class PMSensitiveDetector : public G4VSensitiveDetector
{
public:
    PMSensitiveDetector(G4String name, G4double detectorMass, G4int detectorID);
    virtual ~PMSensitiveDetector();

    virtual void Initialize(G4HCofThisEvent* hce) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void EndOfEvent(G4HCofThisEvent* hce) override;

private:
    G4String fDetectorName;
    G4int    fDetectorID;
    G4double fDetectorMass;

    //per-event quantities
    G4double fTotalEnergyDeposited;
    G4double fFirstHitTime;

    //efficiency counters
    G4int fIncidentGammas;
    G4int fInteractingGammas;

    //photon bookkeeping for Tc·Tt and DE
    G4double fGeneratedPhotons;  // number of optical photons estimated
    G4int    fEscapedPhotons;    // photons escaping volume (detected/transmitted)
};

#endif

