// ComptonDetectorMessenger.hh

#ifndef ComptonDetectorMessenger_h
#define ComptonDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class ComptonDetectorConstruction;
class G4UIdirectory;
//class G4UIcmdWithAString;
//class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class ComptonDetectorMessenger: public G4UImessenger
{
 public:
  ComptonDetectorMessenger(ComptonDetectorConstruction* );
  ~ComptonDetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  ComptonDetectorConstruction* ComptonDetector;
  G4UIdirectory* ComptonDetectorDirectory;

  // collimators
  G4UIcmdWithADoubleAndUnit* collimator1RadiusCmd;
  G4UIcmdWithADoubleAndUnit* collimator1HoleRadiusCmd;
  G4UIcmdWithADoubleAndUnit* collimator1XCmd;
  G4UIcmdWithADoubleAndUnit* collimator2RadiusCmd;
  G4UIcmdWithADoubleAndUnit* collimator2HoleRadiusCmd;
  G4UIcmdWithADoubleAndUnit* collimator2XCmd;

  // shields
  G4UIcmdWithADoubleAndUnit* shield1YZCmd;
  G4UIcmdWithADoubleAndUnit* shield1XCmd;
  G4UIcmdWithADoubleAndUnit* shield1HoleRadiusCmd;
  //  G4UIcmdWithADoubleAndUnit* shield2YZCmd;
  //  G4UIcmdWithADoubleAndUnit* shield2XCmd;
  //  G4UIcmdWithADoubleAndUnit* shield2HoleRadiusCmd;
  //  G4UIcmdWithADoubleAndUnit* shield3YZCmd;
  //  G4UIcmdWithADoubleAndUnit* shield3XCmd;
  //  G4UIcmdWithADoubleAndUnit* shield3HoleRadiusCmd;

  // distances
  G4UIcmdWithADoubleAndUnit* sourcetoScintillatorCmd;
  G4UIcmdWithADoubleAndUnit* collimator1toScintillatorCmd;
  G4UIcmdWithADoubleAndUnit* collimator2toScintillatorCmd;
  G4UIcmdWithADoubleAndUnit* shield1toScintillatorCmd;
  //  G4UIcmdWithADoubleAndUnit* shield2toScintillatorCmd;
  //  G4UIcmdWithADoubleAndUnit* shield3toScintillatorCmd;
  G4UIcmdWithADoubleAndUnit* HPGetoScintillatorCmd;

  G4UIcmdWithADoubleAndUnit* armAngleCmd;


  // UPDATE
  G4UIcmdWithoutParameter* UpdateCmd;
  
  // info
  G4UIcmdWithoutParameter* printInfoCmd;
};

#endif
