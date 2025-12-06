#include "physics.hh"

PMPhysicsList::PMPhysicsList()
{
    //electromagnetic physics
    RegisterPhysics(new G4EmStandardPhysics());

    //decay physics
    RegisterPhysics(new G4DecayPhysics());

    //radioactive decay physics
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    //optical physics
    RegisterPhysics(new G4OpticalPhysics());
}

PMPhysicsList::~PMPhysicsList()
{
}

void PMPhysicsList::SetCuts()
{
    //use default cut values (1 mm for gamma, e-, e+)
    SetCutsWithDefault();
}
