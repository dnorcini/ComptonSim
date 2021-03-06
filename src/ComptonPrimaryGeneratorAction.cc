// ComptonPrimaryGeneratorAction.cc

#include "ComptonPrimaryGeneratorAction.hh"
#include "ComptonDetectorConstruction.hh"
#include "ComptonPrimaryGeneratorMessenger.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

ComptonPrimaryGeneratorAction::ComptonPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(),
    fParticleGun(0)
{
// create commands for interactive definition of generator
  ComptonPrimaryGeneratorMessenger* generatorMessenger = new ComptonPrimaryGeneratorMessenger(this);
   
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
    
  // default particle kinematic
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particleDefinition);
    
  // point source, need GeneratePrimaries to add finite size
  //figure out best way to put this on Compton arm
  //fParticleGun->SetParticlePosition(G4ThreeVector(std::cos(ComptonDetector->armAngle)*(-ComptonDetector->sourcetoScintillator+1.*cm),
  //						  std::sin(ComptonDetector->armAngle)*(-ComptonDetector->sourcetoScintillator+1.*cm), 0.));
  //fParticleGun->SetParticlePosition(G4ThreeVector(-.8*m, 0. ,0.));
  //fParticleGun->SetParticleEnergy(1.*MeV);
 
  ComptonDetectorConstruction* ComptonDetector = new ComptonDetectorConstruction();

 //defaults
  sourceType = "1MeV";
  alphaMax = 2.*deg; // emittance angle
 
  sourceAngle = ComptonDetector->armAngle;
  sourceX = ComptonDetector->sourcetoScintillator;

}

ComptonPrimaryGeneratorAction::~ComptonPrimaryGeneratorAction()
{
    delete fParticleGun;
    delete generatorMessenger;
}

void ComptonPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event. 

  fParticleGun->SetParticlePosition(G4ThreeVector(-sourceX + 3.*cm, 0., 0.));
  
  G4double cosAlpha = 1. - G4UniformRand()*(1.- std::cos(alphaMax));
  G4double sinAlpha = std::sqrt(1. - cosAlpha*cosAlpha);
  G4double psi      = twopi*G4UniformRand();  //psi uniform [0, 2pi]  
  G4ThreeVector direction(sinAlpha*std::cos(psi),sinAlpha*std::sin(psi),cosAlpha);
  direction.rotateY(90.*deg);
  //direction.rotateX(30.*deg);
  //direction.rotateZ(90.*deg);
  fParticleGun->SetParticleMomentumDirection(direction);

  //fParticleGun->SetParticleEnergy(G4RandGauss::shoot(1.00*MeV, 0.00*MeV));

  // must be a better way to do this
  if (sourceType == "1MeV")
  { fParticleGun->SetParticleEnergy(G4RandGauss::shoot(1.000*MeV, 0.00*MeV)); }

  if (sourceType == "Cs137")
  { fParticleGun->SetParticleEnergy(G4RandGauss::shoot(.662*MeV, 0.00*MeV)); }

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

void ComptonPrimaryGeneratorAction::SetSourceType(G4String val)
{ 
  sourceType = val;
  G4cout << "Source type: " << sourceType << G4endl;
 }

void ComptonPrimaryGeneratorAction::SetSourceEmit(G4double val)
{ 
  alphaMax = val; 
  G4cout << "Source emittance: " << G4int(180./3.14159*alphaMax) << G4endl;
}

void ComptonPrimaryGeneratorAction::SetSourceX(G4double val)
{
  sourceX = val;
  G4cout << "Source distance: " << sourceX << G4endl;
}

//void ComptonPrimaryGeneratorAction::SetSourceAngle(G4double val)
//{
//  sourceAngle = val;
//  G4cout << "Source angle: " << G4int(180./3.14159*sourceAngle) << G4endl;
//}
