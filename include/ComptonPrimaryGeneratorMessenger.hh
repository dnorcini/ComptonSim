// ComptonPrimaryGeneratorMessenger.hh

#ifndef ComptonPrimaryGeneratorMessenger_h
#define ComptonPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class ComptonPrimaryGeneratorAction;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

class ComptonPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  ComptonPrimaryGeneratorMessenger(ComptonPrimaryGeneratorAction*);
  ~ComptonPrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  G4UIdirectory* ComptonActionDirectory;
  ComptonPrimaryGeneratorAction* ComptonAction;
  G4UIcmdWithAString* sourceTypeCmd;
  G4UIcmdWithADoubleAndUnit* sourceEmitCmd;
  G4UIcmdWithADoubleAndUnit* sourceXCmd;
  //G4UIcmdWithADoubleAndUnit* sourceAngleCmd;
};

#endif
