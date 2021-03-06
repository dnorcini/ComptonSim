// ComptonCalorHit.hh

#ifndef ComptonCalorHit_h
#define ComptonCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

// Calorimeter hit class.
// It defines data members to store the the energy deposit and track lengths
// of charged particles in a selected volume:
// - fEdep, fTrackLength

class ComptonCalorHit : public G4VHit
{
public:
  ComptonCalorHit();
  ComptonCalorHit(const ComptonCalorHit&);
  virtual ~ComptonCalorHit();

  // operators
  const ComptonCalorHit& operator=(const ComptonCalorHit&);
  G4int operator==(const ComptonCalorHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  // methods from base class
  virtual void Draw() {}
  virtual void Print();

  // methods for data handling
  void Add(G4double de, G4double dl);

  G4double GetEdep() const;
  G4double GetTrackLength() const;
      
private:
  G4double fEdep;        // energy deposit in SD
  G4double fTrackLength; // track length in SD
};

typedef G4THitsCollection<ComptonCalorHit> ComptonCalorHitsCollection;

extern G4ThreadLocal G4Allocator<ComptonCalorHit>* ComptonCalorHitAllocator;

inline void* ComptonCalorHit::operator new(size_t)
{
  if(!ComptonCalorHitAllocator)
    ComptonCalorHitAllocator = new G4Allocator<ComptonCalorHit>;
  void *hit;
  hit = (void *) ComptonCalorHitAllocator->MallocSingle();
  return hit;
}

inline void ComptonCalorHit::operator delete(void *hit)
{
  if(!ComptonCalorHitAllocator)
    ComptonCalorHitAllocator = new G4Allocator<ComptonCalorHit>;
  ComptonCalorHitAllocator->FreeSingle((ComptonCalorHit*) hit);
}

inline void ComptonCalorHit::Add(G4double de, G4double dl) {
  fEdep += de; 
  fTrackLength += dl;
}

inline G4double ComptonCalorHit::GetEdep() const { 
  return fEdep; 
}

inline G4double ComptonCalorHit::GetTrackLength() const { 
  return fTrackLength; 
}

#endif
