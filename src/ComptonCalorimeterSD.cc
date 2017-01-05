// ComptonCalorimeterSD.cc

#include "ComptonCalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

ComptonCalorimeterSD::ComptonCalorimeterSD
(const G4String& name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(0)
{
  collectionName.insert(hitsCollectionName);
}

ComptonCalorimeterSD::~ComptonCalorimeterSD()
{
}


void ComptonCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  // create hits collection
    fHitsCollection = new ComptonCalorHitsCollection(SensitiveDetectorName, collectionName[0]);
    
    // add this collection in hce
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection );
    
    // create hits
    fHitsCollection->insert(new ComptonCalorHit());
}


G4bool ComptonCalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
    
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }
    
  if ( edep==0. && stepLength == 0. ) return false;
    
  // get hit for total accounting
    ComptonCalorHit* hitTotal = (*fHitsCollection)[fHitsCollection->entries()-1];
    
    // add values
    hitTotal->Add(edep, stepLength);
    
    return true;
}

void ComptonCalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) {
    G4int nofHits = fHitsCollection->entries();
        G4cout
	  << G4endl
	  << "-------->Hits Collection: in this event they are " << nofHits
	  << " hits in the tracker chambers: " << G4endl;
        for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}
