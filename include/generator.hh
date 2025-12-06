#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "G4Gamma.hh"
#include "G4Event.hh"          
#include "G4PrimaryVertex.hh"  
#include "G4PrimaryParticle.hh"

class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PMPrimaryGenerator();
    ~PMPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *);

private:
    G4ParticleGun *fParticleGun;
};

#endif
