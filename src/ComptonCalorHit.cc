// ComptonCalorHit.cc

#include "ComptonCalorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <iomanip>

G4ThreadLocal G4Allocator<ComptonCalorHit>* ComptonCalorHitAllocator = 0;

ComptonCalorHit::ComptonCalorHit()
  : G4VHit(),
    fEdep(0.),
    fTrackLength(0.)
{}


ComptonCalorHit::~ComptonCalorHit() {}


ComptonCalorHit::ComptonCalorHit(const ComptonCalorHit& right)
  : G4VHit()
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
}


const ComptonCalorHit& ComptonCalorHit::operator=(const ComptonCalorHit& right)
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
   
  return *this;
}


G4int ComptonCalorHit::operator==(const ComptonCalorHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}


void ComptonCalorHit::Print()
{
    G4cout
      << "Deposited energy: "
      << std::setw(7) << G4BestUnit(fEdep,"Energy")
      << " Track length: "
      << std::setw(7) << G4BestUnit( fTrackLength,"Track Length")
      << G4endl;
}
