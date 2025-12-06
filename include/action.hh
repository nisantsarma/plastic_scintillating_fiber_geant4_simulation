#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "run.hh"

class PMActionInitialization : public G4VUserActionInitialization
{
public:
    PMActionInitialization();
    ~PMActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif