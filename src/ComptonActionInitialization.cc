// ComptonActionInitialization.cc

#include "ComptonActionInitialization.hh"
#include "ComptonPrimaryGeneratorAction.hh"
#include "ComptonRunAction.hh"
#include "ComptonEventAction.hh"

ComptonActionInitialization::ComptonActionInitialization()
: G4VUserActionInitialization()
{}

ComptonActionInitialization::~ComptonActionInitialization()
{}

void ComptonActionInitialization::BuildForMaster() const
{
    SetUserAction(new ComptonRunAction);
}

void ComptonActionInitialization::Build() const
{
    SetUserAction(new ComptonPrimaryGeneratorAction);
    SetUserAction(new ComptonRunAction);
    SetUserAction(new ComptonEventAction);
}
