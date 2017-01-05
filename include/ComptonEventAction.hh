// ComptonEventAction.hh

#ifndef ComptonEventAction_h
#define ComptonEventAction_h 1

#include "ComptonCalorHit.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

// Event action class.
// In EndOfEventAction(), it prints the accumulated quantities of the energy 
// deposit and track lengths in target and HPGe stored in hits collections.

class ComptonEventAction : public G4UserEventAction
{
public:
  ComptonEventAction();
  virtual ~ComptonEventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
    
private:
  ComptonCalorHitsCollection* GetHitsCollection(G4int hcID,const G4Event* event) const;
  void PrintEventStatistics(G4double HPGeEdep, G4double HPGeTrackLength,
			    G4double target1Edep, G4double target1TrackLength,
			    G4double target2Edep, G4double target2TrackLength) const;
  
  G4int  fHPGeHCID;
  G4int  fTarget1HCID;
  G4int  fTarget2HCID;
};
                     
#endif

    
