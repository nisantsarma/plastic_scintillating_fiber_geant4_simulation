#include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "physics.hh"
#include "construction.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    G4RunManager* runManager = new G4RunManager();

    runManager->SetUserInitialization(new PMPhysicsList());

    runManager->SetUserInitialization(new PMDetectorConstruction());

    runManager->SetUserInitialization(new PMActionInitialization());

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc == 1)
    {
        //interactive mode
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else
    {
        //batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;
    delete runManager;

    return 0;
}
