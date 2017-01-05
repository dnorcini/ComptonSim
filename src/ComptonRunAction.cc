// ComptonRunAction.cc

#include "ComptonRunAction.hh"
#include "ComptonAnalysis.hh"
#include "ComptonDetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <ctime>
#include <stdlib.h>
#include <string>

ComptonRunAction::ComptonRunAction()
: G4UserRunAction()
{
    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);
    
    // Create analysis manager
    // The choice of analysis technology is done via selectin of a namespace
    // in ComptonAnalysis.hh
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;
    
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFirstHistoId(1);
    
    // Creating histograms
    analysisManager->CreateH1("hist1","Edep in HPGe", 100, 0., 1.00*MeV);    
    analysisManager->CreateH1("hist2","Edep in scintillator1", 100, 0., 1.00*MeV);  
    analysisManager->CreateH1("hist3","Edep in scintillator2", 100, 0., 1.00*MeV);
    analysisManager->CreateH1("hist4","Edep in scintillator1+2", 100, 0., 1.00*MeV);
    analysisManager->CreateH1("hist5","trackL in HPGe", 100, 0., 2*mm);     
    analysisManager->CreateH1("hist6","trackL in scintillator1", 100, 0., 6*cm);   
    analysisManager->CreateH1("hist7","trackL in scintillator2", 100, 0., 2*cm);
    analysisManager->CreateH2("hist8","HPGe vs scintillator2", 100, 0., 1.00*MeV, 100, 0., 1.00*MeV);
    analysisManager->CreateH2("hist9","HPGe vs scintillator1+2", 100, 0., 1.00*MeV, 100, 0., 1.00*MeV);
    
    // Getting NAMES from hists and adding labels
    G4int name;
    name = analysisManager->GetH1Id("hist1");
    analysisManager->SetH1XAxisTitle(name, "Energy (MeV)");
    analysisManager->SetH1YAxisTitle(name, "Counts");
    name = analysisManager->GetH1Id("hist2");
    analysisManager->SetH1XAxisTitle(name, "Energy (MeV)");
    analysisManager->SetH1YAxisTitle(name, "Counts");
    name = analysisManager->GetH1Id("hist3");
    analysisManager->SetH1XAxisTitle(name, "Energy (MeV)");
    analysisManager->SetH1YAxisTitle(name, "Counts");
    name = analysisManager->GetH1Id("hist4");
    analysisManager->SetH1XAxisTitle(name, "Scintillator1+2 Energy (MeV)");
    analysisManager->SetH1YAxisTitle(name, "Counts");
    name = analysisManager->GetH1Id("hist5");
    analysisManager->SetH1XAxisTitle(name, "Track length (mm)");
    analysisManager->SetH1YAxisTitle(name, "Counts");
    name = analysisManager->GetH1Id("hist6");
    analysisManager->SetH1XAxisTitle(name, "Track length (mm)");
    analysisManager->SetH1YAxisTitle(name, "Counts");
    name = analysisManager->GetH1Id("hist7");
    analysisManager->SetH1XAxisTitle(name, "Track length (mm)");
    analysisManager->SetH1YAxisTitle(name, "Counts");
    name = analysisManager->GetH2Id("hist8");			      
    analysisManager->SetH2XAxisTitle(name, "HPGe energy (MeV)");   
    analysisManager->SetH2YAxisTitle(name, "Scintillator2 energy (MeV)");      
    analysisManager->SetH2ZAxisTitle(name, "Counts");     
    name = analysisManager->GetH2Id("hist9");			      
    analysisManager->SetH2XAxisTitle(name, "HPGe energy (MeV)");   
    analysisManager->SetH2YAxisTitle(name, "Scintillator1+2 energy (MeV)");      
    analysisManager->SetH2ZAxisTitle(name, "Counts");     


     // Creating ntuple
    analysisManager->CreateNtuple("ctree", "Edep and TrackL");
    analysisManager->CreateNtupleDColumn("HPGeE");
    analysisManager->CreateNtupleDColumn("target1E");
    analysisManager->CreateNtupleDColumn("target2E");
    analysisManager->CreateNtupleDColumn("HPGeL");
    analysisManager->CreateNtupleDColumn("target1L");
    analysisManager->CreateNtupleDColumn("target2L");
    analysisManager->FinishNtuple();
}

ComptonRunAction::~ComptonRunAction()
{
    delete G4AnalysisManager::Instance();
}

void ComptonRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    //inform the runManager to save random number seed
    //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    ComptonDetectorConstruction* ComptonDetector = new ComptonDetectorConstruction();

    time_t now = time(0);
    tm *ltm = localtime(&now);

    int month = 1+int(ltm->tm_mon);
    int day = int(ltm->tm_mday);
    int year = 1900+int(ltm->tm_year);

    int angle   = int(180/3.14159*ComptonDetector->armAngle);
    int C1HR = ComptonDetector->collimator1HoleRadius;
    int C1SD = ComptonDetector->collimator1toScintillator;
    int C2HR = ComptonDetector->collimator2HoleRadius;
    int C2SD = ComptonDetector->collimator2toScintillator;
    int SHR  = ComptonDetector->shield1HoleRadius;
    int SSD  = ComptonDetector->shield1toScintillator; 

    char numstr[21]; //hold all numbers < 64 bits

    sprintf(numstr,"%d%d%d_%ddeg_%dmm_%dcm_%dmm_%dcm_%dmm_%dcm",
	    month, day, year, angle, C1HR, C1SD, C2HR, C2SD, SHR, SSD);

    //G4String fileName = numstr;
    
    G4String fileName = "ComptonSimOutput";
    //analysisManager->OpenFile(fileName); //will be overwritten if specified by macro
    analysisManager->OpenFile();
}

void ComptonRunAction::EndOfRunAction(const G4Run* /*run*/)
{
    // print histogram statistics
    //
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if ( analysisManager->GetH1(1) ) {
        
        G4cout << G4endl << " ----> print histograms statistic ";
        
        if(isMaster) {
            G4cout << "for the entire run " << G4endl << G4endl;
        }
        else {
            G4cout << "for the local thread " << G4endl << G4endl;
        }
        
        G4cout << " HPGe Energy   : mean = "
               << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy")
               << " rms = "
               << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") 
               << G4endl;
        
        G4cout << " Scintillator1 Energy : mean = "
               << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy")
               << " rms = "
               << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") 
               << G4endl;

        G4cout << " Scintillator2 Energy : mean = "
               << G4BestUnit(analysisManager->GetH1(3)->mean(), "Energy")
               << " rms = "
               << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Energy") 
               << G4endl;
        
        G4cout << "NOTE: histogram statistics depend on ranges used in code."
               << G4endl;

    }
    

    analysisManager->Write();
    analysisManager->CloseFile();
    
}
