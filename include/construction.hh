#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh" 

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4ProductionCuts.hh"
#include "G4Region.hh"

#include "G4SDManager.hh"
#include "detector.hh"

class PMDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    PMDetectorConstruction();
    virtual ~PMDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

private:
    G4LogicalVolume *logicDetector;  // plastic scintillator
    G4LogicalVolume* logicClad;

    virtual void ConstructSDandField();
};

#endif
