// ComptonPrimaryGeneratorMessenger.cc

#include "ComptonPrimaryGeneratorMessenger.hh"
#include "ComptonPrimaryGeneratorAction.hh"
#include "ComptonDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

ComptonPrimaryGeneratorMessenger::ComptonPrimaryGeneratorMessenger
(ComptonPrimaryGeneratorAction* ComptonAction)
  :ComptonAction(ComptonAction)
{
  ComptonActionDirectory = new G4UIdirectory("/ComptonSource/");
  ComptonActionDirectory->SetGuidance("ComptonSource gun");


  sourceTypeCmd = new G4UIcmdWithAString("/ComptonSource/sourceType", this);
  sourceTypeCmd->SetGuidance("Set type of incident flux.");
  sourceTypeCmd->SetGuidance("Choices: 1MeV, Cs137");
  sourceTypeCmd->SetParameterName("Choice",true);
  sourceTypeCmd->SetDefaultValue("1MeV");
  sourceTypeCmd->AvailableForStates(G4State_Idle);

  sourceEmitCmd = new G4UIcmdWithADoubleAndUnit("/ComptonSource/sourceEmit",this);
  sourceEmitCmd->SetGuidance("Set emittance of incident flux.");
  sourceEmitCmd->SetParameterName("Size",false);
  sourceEmitCmd->SetRange("Size>=0.0 && Size<=360.0");
  sourceEmitCmd->AvailableForStates(G4State_Idle);

  sourceXCmd = new G4UIcmdWithADoubleAndUnit("/ComptonSource/sourceX",this);
  sourceXCmd->SetGuidance("Set X position of incident flux.");
  sourceXCmd->SetParameterName("Size",false);
  sourceXCmd->SetRange("Size>=0.0");
  sourceXCmd->AvailableForStates(G4State_Idle);

  //sourceAngleCmd = new G4UIcmdWithADoubleAndUnit("/ComptonSource/sourceAngle",this);
  //sourceAngleCmd->SetGuidance("Set angle of incident flux.");
  //sourceAngleCmd->SetParameterName("Size",false);
  //sourceAngleCmd->SetRange("Size>=0.0 && Size<=360.0");
  //sourceAngleCmd->AvailableForStates(G4State_Idle);

}

ComptonPrimaryGeneratorMessenger::~ComptonPrimaryGeneratorMessenger()
{
  delete sourceTypeCmd;
  delete sourceEmitCmd;
  delete sourceXCmd;
  //delete sourceAngleCmd;
}

void ComptonPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == sourceTypeCmd)
    {ComptonAction->SetSourceType(newValue);}

  if (command == sourceEmitCmd)
    {ComptonAction->SetSourceEmit(sourceEmitCmd->GetNewDoubleValue(newValue));}
  
  if (command == sourceXCmd)
    {ComptonAction->SetSourceX(sourceEmitCmd->GetNewDoubleValue(newValue));}

  //if (command == sourceAngleCmd)
  //  {ComptonAction->SetSourceAngle(sourceEmitCmd->GetNewDoubleValue(newValue));}
}
