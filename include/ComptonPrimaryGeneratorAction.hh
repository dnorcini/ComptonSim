// ComptonPrimaryGeneratorAction.hh

#ifndef ComptonPrimaryGeneratorAction_h
#define ComptonPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class ComptonPrimaryGeneratorMessenger;

// The primary generator action class with particle gun.
// It defines a single particle which hits the calorimeter
// perpendicular to the input face.

class ComptonPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  ComptonPrimaryGeneratorAction();
  virtual ~ComptonPrimaryGeneratorAction();

public:
  virtual void GeneratePrimaries(G4Event* event);
  ComptonPrimaryGeneratorMessenger* generatorMessenger;

  // set methods
  void SetRandomFlag(G4bool value);
  void SetSourceType(G4String);
  void SetSourceEmit(G4double);
  void SetSourceX(G4double);
  //void SetSourceAngle(G4double);

private:
  G4ParticleGun*  fParticleGun; // G4 particle gun
  G4String sourceType;
  G4double alphaMax;
  G4double sourceX;
  G4double sourceAngle;

};

#endif
