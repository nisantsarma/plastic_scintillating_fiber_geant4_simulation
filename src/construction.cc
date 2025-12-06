#include "construction.hh"

PMDetectorConstruction::PMDetectorConstruction() {}
PMDetectorConstruction::~PMDetectorConstruction() {}

G4VPhysicalVolume* PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;
    G4NistManager* nist = G4NistManager::Instance();

    //materials
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* psMat = nist->FindOrBuildMaterial("G4_POLYSTYRENE"); // PS core
    G4Material* cladMat = nist->FindOrBuildMaterial("G4_PLEXIGLASS"); // PMMA cladding

    //optical properties for PSF core
    const G4int nEntries = 2;
    //~492 nm emission(blue-green region) (~2.5 eV)
    G4double photonEnergy[nEntries]  = {2.4*eV, 2.6*eV}; 

    G4double scintSpectrum[nEntries] = {1.0, 1.0}; // flat
    G4double rindexPS[nEntries]      = {1.60, 1.60};
    G4double absLengthPS[nEntries]   = {3.5*m, 3.5*m}; // long attenuation length

    G4MaterialPropertiesTable* psMPT = new G4MaterialPropertiesTable();
    psMPT->AddConstProperty("SCINTILLATIONYIELD", 8000./MeV);
    psMPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
    psMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.7*ns);
    psMPT->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
    psMPT->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintSpectrum, nEntries);
    psMPT->AddProperty("RINDEX", photonEnergy, rindexPS, nEntries);
    psMPT->AddProperty("ABSLENGTH", photonEnergy, absLengthPS, nEntries);
    psMat->SetMaterialPropertiesTable(psMPT);

    //cladding (PMMA)
    G4double rindexClad[nEntries] = {1.49, 1.49};
    G4MaterialPropertiesTable* cladMPT = new G4MaterialPropertiesTable();
    cladMPT->AddProperty("RINDEX", photonEnergy, rindexClad, nEntries);
    cladMat->SetMaterialPropertiesTable(cladMPT);

    //world volume
    G4Box* solidWorld = new G4Box("solidWorld", 10*cm, 10*cm, 10*cm);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume* physWorld =new G4PVPlacement(0, {}, logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    //fiber geometry
    G4double fiberLength = 100*mm;     // 10 cm
    G4double fiberRadius = 0.5*mm;     // 1 mm diameter
    G4double cladThickness = 0.03*mm;  // 30 micro m

    //core
    G4Tubs* solidCore = new G4Tubs("solidCore", 0, fiberRadius, 0.5*fiberLength, 0, 360*deg);
    logicDetector = new G4LogicalVolume(solidCore, psMat, "logicCore");
    new G4PVPlacement(0, {0,0,0}, logicDetector, "physCore",logicWorld, false, 0, checkOverlaps);

    //cladding
    G4Tubs* solidClad = new G4Tubs("solidClad", fiberRadius, fiberRadius+cladThickness,0.5*fiberLength, 0, 360*deg);
    G4LogicalVolume* logicClad = new G4LogicalVolume(solidClad, cladMat, "logicClad");
    new G4PVPlacement(0, {0,0,0}, logicClad, "physClad", logicWorld, false, 0, checkOverlaps);

    // visualisation

    //fibre core (yellow, solid)
    auto visCore = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 1.0));  // bright yellow
    visCore->SetForceSolid(true);
    logicDetector->SetVisAttributes(visCore);

    //cladding (blue, semi-transparent)
    auto visClad = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.3));  // transparent blue
    visClad->SetForceSolid(true);
    logicClad->SetVisAttributes(visClad);

    //world volume (invisible, to avoid blocking view)
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdm = G4SDManager::GetSDMpointer();

    //sensitive detector for scintillator
    G4double detMass = logicDetector->GetMass();
    auto psSD = new PMSensitiveDetector("PlasticSD", detMass, 0);
    sdm->AddNewDetector(psSD);
    logicDetector->SetSensitiveDetector(psSD);

    //production cuts
    G4Region* detRegion = new G4Region("DetectorRegion");
    logicDetector->SetRegion(detRegion);
    detRegion->AddRootLogicalVolume(logicDetector);

    G4ProductionCuts* cuts = new G4ProductionCuts();
    cuts->SetProductionCut(0.1*mm, "gamma"); // paper used ~0.1 mm
    cuts->SetProductionCut(0.1*mm, "e-");
    cuts->SetProductionCut(0.1*mm, "e+");
    detRegion->SetProductionCuts(cuts);
}
