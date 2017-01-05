// ComptonEventAction.cc

#include "ComptonEventAction.hh"
#include "ComptonCalorimeterSD.hh"
#include "ComptonCalorHit.hh"
#include "ComptonAnalysis.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>


ComptonEventAction::ComptonEventAction()
  : G4UserEventAction(),
    fHPGeHCID(-1),
    fTarget1HCID(-1),
    fTarget2HCID(-1)
{}


ComptonEventAction::~ComptonEventAction()
{}

ComptonCalorHitsCollection* ComptonEventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
    ComptonCalorHitsCollection* hitsCollection = static_cast<ComptonCalorHitsCollection*>
      (event->GetHCofThisEvent()->GetHC(hcID));
    
    if ( ! hitsCollection ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hitsCollection ID " << hcID;
        G4Exception("ComptonEventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
    }
    
    return hitsCollection;
}

void ComptonEventAction::PrintEventStatistics(G4double HPGeEdep, G4double HPGeTrackLength,
					      G4double target1Edep, G4double target1TrackLength,
					      G4double target2Edep, G4double target2TrackLength) const
{
    G4cout
        << "HPGe:   total energy: "
        << std::setw(7) << G4BestUnit(HPGeEdep, "Energy")
        << "\ttotal track length: "
        << std::setw(7) << G4BestUnit(HPGeTrackLength, "Length")
        << G4endl
        << "Target1: total energy: "
        << std::setw(7) << G4BestUnit(target1Edep, "Energy")
        << "\ttotal track length: "
        << std::setw(7) << G4BestUnit(target1TrackLength, "Length")
        << G4endl
        << "Target2: total energy: "
        << std::setw(7) << G4BestUnit(target2Edep, "Energy")
        << "\ttotal track length: "
        << std::setw(7) << G4BestUnit(target2TrackLength, "Length")
        << G4endl;
}

void ComptonEventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

void ComptonEventAction::EndOfEventAction(const G4Event* event)
{
    // Get hits collections IDs (only once)
    if ( fHPGeHCID == -1 ) {
        fHPGeHCID = G4SDManager::GetSDMpointer()->GetCollectionID("HPGeHits");
        fTarget1HCID = G4SDManager::GetSDMpointer()->GetCollectionID("Scintillator1Hits");
	fTarget2HCID = G4SDManager::GetSDMpointer()->GetCollectionID("Scintillator2Hits");
    }
    
    // Get hits collections
    ComptonCalorHitsCollection* HPGeHC = GetHitsCollection(fHPGeHCID, event);
    ComptonCalorHitsCollection* target1HC = GetHitsCollection(fTarget1HCID, event);
    ComptonCalorHitsCollection* target2HC = GetHitsCollection(fTarget2HCID, event);
    
    // Get hit with total values
    ComptonCalorHit* HPGeHit = (*HPGeHC)[HPGeHC->entries()-1];
    ComptonCalorHit* target1Hit = (*target1HC)[target1HC->entries()-1];
    ComptonCalorHit* target2Hit = (*target2HC)[target2HC->entries()-1];
    
// Print per event (modulo n)
    //
    G4int eventID = event->GetEventID();
    G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
        G4cout << "---> End of event: " << eventID << G4endl;
        
        PrintEventStatistics(
                             HPGeHit->GetEdep(), HPGeHit->GetTrackLength(),
                             target1Hit->GetEdep(), target1Hit->GetTrackLength(),
			     target2Hit->GetEdep(), target2Hit->GetTrackLength());
    }
    
    // Fill histograms, ntuple 
    // get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // fill histograms
    // if-statements get rid of the no-hit events
    if ( HPGeHit->GetEdep() > 0. )
        analysisManager->FillH1(1, HPGeHit->GetEdep());
    if ( target1Hit->GetEdep() > 0. )
        analysisManager->FillH1(2, target1Hit->GetEdep());
    if ( target2Hit->GetEdep() > 0. )
        analysisManager->FillH1(3, target2Hit->GetEdep());
    if ( (target1Hit->GetEdep() > 0.) || (target2Hit->GetEdep() > 0.) )
        analysisManager->FillH1(4, target1Hit->GetEdep() + target2Hit->GetEdep());
    if ( HPGeHit->GetTrackLength() > 0. )
        analysisManager->FillH1(5, HPGeHit->GetTrackLength());
    if ( target1Hit->GetTrackLength() > 0. )
        analysisManager->FillH1(6, target1Hit->GetTrackLength());
    if ( target2Hit->GetTrackLength() > 0. )
        analysisManager->FillH1(7, target2Hit->GetTrackLength());
   
    // 2D histogram for coincidence analysis that considers non-zero-energy events only
    // different numbering than H1
    if ( (HPGeHit->GetEdep() > 0.) && (target2Hit->GetEdep() > 0.) )
         analysisManager->FillH2(1, HPGeHit->GetEdep(), target2Hit->GetEdep());
    if ( (HPGeHit->GetEdep() > 0.) && ((target1Hit->GetEdep() > 0.) || (target2Hit->GetEdep() > 0.)) )
         analysisManager->FillH2(2, HPGeHit->GetEdep(), target1Hit->GetEdep() + target2Hit->GetEdep());
        
    // fill ntuple
    analysisManager->FillNtupleDColumn(0, HPGeHit->GetEdep());
    analysisManager->FillNtupleDColumn(1, target1Hit->GetEdep());
    analysisManager->FillNtupleDColumn(2, target2Hit->GetEdep());
    analysisManager->FillNtupleDColumn(3, HPGeHit->GetTrackLength());
    analysisManager->FillNtupleDColumn(4, target1Hit->GetTrackLength());
    analysisManager->FillNtupleDColumn(5, target2Hit->GetTrackLength());
    analysisManager->AddNtupleRow();  
}  
