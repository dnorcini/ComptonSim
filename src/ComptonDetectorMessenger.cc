// ComptonDetectorMessenger.cc

#include "ComptonDetectorMessenger.hh"
#include "ComptonDetectorConstruction.hh"

#include "G4UIdirectory.hh"
//#include "G4UIcmdWithAString.hh"
//#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

ComptonDetectorMessenger::ComptonDetectorMessenger(ComptonDetectorConstruction* ComptonDetector) 
  :ComptonDetector(ComptonDetector)
{
  ComptonDetectorDirectory = new G4UIdirectory("/ComptonDetector/");
  ComptonDetectorDirectory->SetGuidance("ComptonDetector geometry"); 

  // collimators
  collimator1RadiusCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setCol1Rad", this);
  collimator1RadiusCmd->SetGuidance("Choose radius of collimator 1.");
  collimator1RadiusCmd->SetParameterName("Size", false);
  collimator1RadiusCmd->SetRange("Size>=0.0");
  collimator1RadiusCmd->AvailableForStates(G4State_Idle);

  collimator1HoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setCol1HoleRad", this);
  collimator1HoleRadiusCmd->SetGuidance("Choose radius of collimator 1 hole.");
  collimator1HoleRadiusCmd->SetParameterName("Size", false);
  collimator1HoleRadiusCmd->SetRange("Size>=0.0");
  collimator1HoleRadiusCmd->AvailableForStates(G4State_Idle);

  collimator1XCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setCol1Len", this);
  collimator1XCmd->SetGuidance("Choose length of collimator 1.");
  collimator1XCmd->SetParameterName("Size", false);
  collimator1XCmd->SetRange("Size>=0.0");
  collimator1XCmd->AvailableForStates(G4State_Idle);

  collimator2RadiusCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setCol2Rad", this);
  collimator2RadiusCmd->SetGuidance("Choose radius of collimator 2.");
  collimator2RadiusCmd->SetParameterName("Size", false);
  collimator2RadiusCmd->SetRange("Size>=0.0");
  collimator2RadiusCmd->AvailableForStates(G4State_Idle);

  collimator2HoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setCol2HoleRad", this);
  collimator2HoleRadiusCmd->SetGuidance("Choose radius of collimator 2 hole.");
  collimator2HoleRadiusCmd->SetParameterName("Size", false);
  collimator2HoleRadiusCmd->SetRange("Size>=0.0");
  collimator2HoleRadiusCmd->AvailableForStates(G4State_Idle);

  collimator2XCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setCol2Len", this);
  collimator2XCmd->SetGuidance("Choose length of collimator 2.");
  collimator2XCmd->SetParameterName("Size", false);
  collimator2XCmd->SetRange("Size>=0.0");
  collimator2XCmd->AvailableForStates(G4State_Idle);

  // shields
  shield1YZCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield1Width", this);
  shield1YZCmd->SetGuidance("Choose width of shield 1.");
  shield1YZCmd->SetParameterName("Size", false);
  shield1YZCmd->SetRange("Size>=0.0");
  shield1YZCmd->AvailableForStates(G4State_Idle);
  
  shield1XCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield1Thick", this);
  shield1XCmd->SetGuidance("Choose thickness of shield 1.");
  shield1XCmd->SetParameterName("Size", false);
  shield1XCmd->SetRange("Size>=0.0");
  shield1XCmd->AvailableForStates(G4State_Idle);

  shield1HoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield1HoleRad", this);
  shield1HoleRadiusCmd->SetGuidance("Choose radius of shield 1 hole.");
  shield1HoleRadiusCmd->SetParameterName("Size", false);
  shield1HoleRadiusCmd->SetRange("Size>=0.0");
  shield1HoleRadiusCmd->AvailableForStates(G4State_Idle);
  
  //shield2YZCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield2Width", this);
  //shield2YZCmd->SetGuidance("Choose width of shield 2.");
  //shield2YZCmd->SetParameterName("Size", false);
  //shield2YZCmd->SetRange("Size>=0.0");
  //shield2YZCmd->AvailableForStates(G4State_Idle);
  
  //shield2XCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield2Thick", this);
  //shield2XCmd->SetGuidance("Choose thickness of shield 2.");
  //shield2XCmd->SetParameterName("Size", false);
  //shield2XCmd->SetRange("Size>=0.0");
  //shield2XCmd->AvailableForStates(G4State_Idle);

  //shield2HoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield2HoleRad", this);
  //shield2HoleRadiusCmd->SetGuidance("Choose radius of shield 2 hole.");
  //shield2HoleRadiusCmd->SetParameterName("Size", false);
  //shield2HoleRadiusCmd->SetRange("Size>=0.0");
  //shield2HoleRadiusCmd->AvailableForStates(G4State_Idle);
  
  //shield3YZCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield3Width", this);
  //shield3YZCmd->SetGuidance("Choose width of shield 3.");
  //shield3YZCmd->SetParameterName("Size", false);
  //shield3YZCmd->SetRange("Size>=0.0");
  //shield3YZCmd->AvailableForStates(G4State_Idle);
  
  //shield3XCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield3Thick", this);
  //shield3XCmd->SetGuidance("Choose thickness of shield 3.");
  //shield3XCmd->SetParameterName("Size", false);
  //shield3XCmd->SetRange("Size>=0.0");
  //shield3XCmd->AvailableForStates(G4State_Idle);

  //shield3HoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield3HoleRad", this);
  //shield3HoleRadiusCmd->SetGuidance("Choose radius of shield 3 hole.");
  //shield3HoleRadiusCmd->SetParameterName("Size", false);
  //shield3HoleRadiusCmd->SetRange("Size>=0.0");
  //shield3HoleRadiusCmd->AvailableForStates(G4State_Idle);

  // distances
  sourcetoScintillatorCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setSourcetoScint", this);
  sourcetoScintillatorCmd->SetGuidance("Choose distance between source and scintillator.");
  sourcetoScintillatorCmd->SetParameterName("Size", false);
  sourcetoScintillatorCmd->SetRange("Size>=0.0");
  sourcetoScintillatorCmd->AvailableForStates(G4State_Idle);

  collimator1toScintillatorCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setCol1toScint", this);
  collimator1toScintillatorCmd->SetGuidance("Choose distance between collimator 1 and scintillator.");
  collimator1toScintillatorCmd->SetParameterName("Size", false);
  collimator1toScintillatorCmd->SetRange("Size>=0.0");
  collimator1toScintillatorCmd->AvailableForStates(G4State_Idle);

  collimator2toScintillatorCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setCol2toScint", this);
  collimator2toScintillatorCmd->SetGuidance("Choose distance between collimator 2 and scintillator.");
  collimator2toScintillatorCmd->SetParameterName("Size", false);
  collimator2toScintillatorCmd->SetRange("Size>=0.0");
  collimator2toScintillatorCmd->AvailableForStates(G4State_Idle);
  
  shield1toScintillatorCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield1toScint", this);
  shield1toScintillatorCmd->SetGuidance("Choose distance between shield 1 and scintillator.");
  shield1toScintillatorCmd->SetParameterName("Size", false);
  shield1toScintillatorCmd->SetRange("Size>=0.0");
  shield1toScintillatorCmd->AvailableForStates(G4State_Idle);

  //shield2toScintillatorCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield2toScint", this);
  //shield2toScintillatorCmd->SetGuidance("Choose distance between shield 2 and scintillator.");
  //shield2toScintillatorCmd->SetParameterName("Size", false);
  //shield2toScintillatorCmd->SetRange("Size>=0.0");
  //shield2toScintillatorCmd->AvailableForStates(G4State_Idle);
  
  //shield3toScintillatorCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setShield3toScint", this);
  //shield3toScintillatorCmd->SetGuidance("Choose distance between shield 3 and scintillator.");
  //shield3toScintillatorCmd->SetParameterName("Size", false);
  //shield3toScintillatorCmd->SetRange("Size>=0.0");
  //shield3toScintillatorCmd->AvailableForStates(G4State_Idle);
  
  HPGetoScintillatorCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setHPGetoScint", this);
  HPGetoScintillatorCmd->SetGuidance("Choose distance between HPGe and scintillator.");
  HPGetoScintillatorCmd->SetParameterName("Size", false);
  HPGetoScintillatorCmd->SetRange("Size>=0.0");
  HPGetoScintillatorCmd->AvailableForStates(G4State_Idle);
  
  armAngleCmd = new G4UIcmdWithADoubleAndUnit("/ComptonDetector/setArmAngle", this);
  armAngleCmd->SetGuidance("Choose arm angle (in degrees).");
  armAngleCmd->SetParameterName("Size", false);
  armAngleCmd->SetRange("Size>=0.0");
  armAngleCmd->AvailableForStates(G4State_Idle);
  
  // update
  UpdateCmd = new G4UIcmdWithoutParameter("/ComptonDetector/update",this);
  UpdateCmd->SetGuidance("Update detector geometry");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you change geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

  // info
  printInfoCmd = new G4UIcmdWithoutParameter("/ComptonDetector/printInfo",this);
  printInfoCmd->SetGuidance("Print geometry info.");
  printInfoCmd->AvailableForStates(G4State_Idle);
}

ComptonDetectorMessenger::~ComptonDetectorMessenger()
{
  delete collimator1RadiusCmd;
  delete collimator1HoleRadiusCmd;
  delete collimator1XCmd;

  delete collimator2RadiusCmd;
  delete collimator2HoleRadiusCmd;
  delete collimator2XCmd;

  delete shield1YZCmd;
  delete shield1XCmd;
  delete shield1HoleRadiusCmd;

  //delete shield2YZCmd;
  //delete shield2XCmd;
  //delete shield2HoleRadiusCmd;

  //delete shield3YZCmd;
  //delete shield3XCmd;
  //delete shield3HoleRadiusCmd;

  delete collimator1toScintillatorCmd;
  delete collimator2toScintillatorCmd;
  delete shield1toScintillatorCmd;
  //delete shield2toScintillatorCmd;
  //delete shield3toScintillatorCmd;
  //delete HPGetoScintillatorCmd;
  delete armAngleCmd;

  delete UpdateCmd;
  delete printInfoCmd;
}

void ComptonDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  // collimators
  if (command == collimator1RadiusCmd)
    {ComptonDetector->SetCollimator1Radius(collimator1RadiusCmd->GetNewDoubleValue(newValue));}

  if (command == collimator1HoleRadiusCmd)
    {ComptonDetector->SetCollimator1HoleRadius(collimator1HoleRadiusCmd->GetNewDoubleValue(newValue));}

  if (command == collimator1XCmd)
    {ComptonDetector->SetCollimator1X(collimator1XCmd->GetNewDoubleValue(newValue));}

  if (command == collimator2RadiusCmd)
    {ComptonDetector->SetCollimator2Radius(collimator2RadiusCmd->GetNewDoubleValue(newValue));}

  if (command == collimator2HoleRadiusCmd)
    {ComptonDetector->SetCollimator2HoleRadius(collimator2HoleRadiusCmd->GetNewDoubleValue(newValue));}

  if (command == collimator2XCmd)
    {ComptonDetector->SetCollimator2X(collimator2XCmd->GetNewDoubleValue(newValue));}

  // shields
  if (command == shield1YZCmd)
    {ComptonDetector->SetShield1YZ(shield1YZCmd->GetNewDoubleValue(newValue));}

  if (command == shield1XCmd)
    {ComptonDetector->SetShield1X(shield1XCmd->GetNewDoubleValue(newValue));}

  if (command == shield1HoleRadiusCmd)
    {ComptonDetector->SetShield1HoleRadius(shield1HoleRadiusCmd->GetNewDoubleValue(newValue));}

  //if (command == shield2YZCmd)
  //{ComptonDetector->SetShield2YZ(shield2YZCmd->GetNewDoubleValue(newValue));}

  //if (command == shield2XCmd)
  //{ComptonDetector->SetShield2X(shield2XCmd->GetNewDoubleValue(newValue));}

  //if (command == shield2HoleRadiusCmd)
  //{ComptonDetector->SetShield2HoleRadius(shield2HoleRadiusCmd->GetNewDoubleValue(newValue));}

  //if (command == shield3YZCmd)
  //{ComptonDetector->SetShield3YZ(shield3YZCmd->GetNewDoubleValue(newValue));}

  //if (command == shield3XCmd)
  //{ComptonDetector->SetShield3X(shield3XCmd->GetNewDoubleValue(newValue));}

  //if (command == shield3HoleRadiusCmd)
  //{ComptonDetector->SetShield3HoleRadius(shield3HoleRadiusCmd->GetNewDoubleValue(newValue));}

  // distances
  if (command == collimator1toScintillatorCmd)
    {ComptonDetector->SetCollimator1toScintillator(collimator1toScintillatorCmd->GetNewDoubleValue(newValue));}

  if (command == collimator2toScintillatorCmd)
    {ComptonDetector->SetCollimator2toScintillator(collimator2toScintillatorCmd->GetNewDoubleValue(newValue));}

  if (command == shield1toScintillatorCmd)
    {ComptonDetector->SetShield1toScintillator(shield1toScintillatorCmd->GetNewDoubleValue(newValue));}

  //if (command == shield2toScintillatorCmd)
  //{ComptonDetector->SetShield2toScintillator(shield2toScintillatorCmd->GetNewDoubleValue(newValue));}

  //if (command == shield3toScintillatorCmd)
  //{ComptonDetector->SetShield3toScintillator(shield3toScintillatorCmd->GetNewDoubleValue(newValue));}

  if (command == HPGetoScintillatorCmd)
    {ComptonDetector->SetHPGetoScintillator(HPGetoScintillatorCmd->GetNewDoubleValue(newValue));}

  if (command == armAngleCmd)
    {
      ComptonDetector->SetArmAngle(armAngleCmd->GetNewDoubleValue(newValue));
      //G4cout << "Running with armAngle is: " << newValue << G4endl;
    }

  if (command == UpdateCmd)
    {ComptonDetector->UpdateGeometry();}

  if (command == printInfoCmd)
    {ComptonDetector->printBasicInfo();}
}
