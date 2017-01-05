// ComptonActionInitialization.hh

#ifndef ComptonActionInitialization_h
#define ComptonActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class ComptonActionInitialization : public G4VUserActionInitialization
{
  public:
    ComptonActionInitialization();
    virtual ~ComptonActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif

    
