// ComptonSim.cc
// main() program of ComptonSim

// detector construction and physics processes
#include "ComptonDetectorConstruction.hh"
#include "ComptonActionInitialization.hh"

// multithreaded run manager toolkit
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

// user interfaces
#include "G4UImanager.hh"
#include "G4UIcommand.hh"

// visualization
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "G4ScoringManager.hh"

#include "FTFP_BERT.hh" //requires datafiles for EM, hadronic physics
//#include "G4Scintillaton.hh"
//#include "G4OpBoundaryProcess.hh"

#include "Randomize.hh"
 

namespace {
  void PrintUsage() {
    G4cerr << "Usage: " << G4endl;
    G4cerr << "ComptonSim [-m macro] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "\tnote: -t option is only for multi-threaded mode." << G4endl;
  }
}

int main(int argc, char** argv) {
  if (argc > 7) {
    PrintUsage();
    return 1;
  }

  // check for multithread option
  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  for (G4int i = 1; i<argc; i = i+2) {
    if      (G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if (G4String(argv[i]) == "-u") session = argv[i+1];
#ifdef G4MULTITHREADED
    else if (G4String(argv[i]) == "-t") {
	nThreads = G4UIcommand::ConverttoInt(argv[i+1]);
    }
#endif
    else {
	PrintUsage();
	return 1;
    }
  }
  
  // define UI session if no macro given
  G4UIExecutive* ui = 0;
  if (! macro.size()) {
    ui = new G4UIExecutive(argc, argv);
  }


  // Activate UI-command base scorer 
  G4ScoringManager * scManager = G4ScoringManager::GetScoringManager(); 
  scManager->SetVerboseLevel(1);

  // set random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // construct default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  if (nThreads > 0) {
    runManager->SetNumberOfThreads(nThreads);
  }
#else
  G4RunManager* runManager = new G4RunManager;
#endif


  // initialize detector construction
  ComptonDetectorConstruction* detectorConstruction = new ComptonDetectorConstruction();
  runManager->SetUserInitialization(detectorConstruction);

  // initialize hadronic, EM physics processes
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);

  // initialize primary generator
  ComptonActionInitialization* actionInitialization = new ComptonActionInitialization();
  runManager->SetUserInitialization(actionInitialization);


  // initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (macro.size()) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }


  // job termination
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
}
