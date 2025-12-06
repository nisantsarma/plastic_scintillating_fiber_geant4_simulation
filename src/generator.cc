#include "generator.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"

PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    //define gamma particle
    G4ParticleDefinition* particle = G4Gamma::GammaDefinition();
    fParticleGun->SetParticleDefinition(particle);

    //placing particle gun at at z = -8.0 cm
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -8.0*cm));

    //direction of gamma
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

    //default photon energy
    fParticleGun->SetParticleEnergy(100.*keV);
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
