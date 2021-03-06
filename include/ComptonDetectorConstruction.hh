// ComptonDectorConstruction.hh

#ifndef ComptonDetectorConstruction_h
#define ComptonDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class ComptonDetectorMessenger;

// Detector construction class to define materials and geometry.
// The calorimeter is a cylinder made of nested cylinders.

// In ConstructSDandField() sensitive detectors of B4cCalorimeterSD type
// are created and associated with the Scintillator and HPGe volumes.
class ComptonDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  ComptonDetectorConstruction();
  virtual ~ComptonDetectorConstruction();

public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  ComptonDetectorMessenger* detectorMessenger; //pointer to detector messenger
  
  G4double sourcetoScintillator;
  G4double collimator1toScintillator; 
  G4double collimator2toScintillator;
  G4double shield1toScintillator ;
  //  G4double shield2toScintillator;
  //  G4double shield3toScintillator;
  G4double HPGetoScintillator;
  G4double armAngle;
  
  void SetCollimator1Radius(G4double);
  void SetCollimator1HoleRadius(G4double);
  void SetCollimator1X(G4double);

  void SetCollimator2Radius(G4double);
  void SetCollimator2HoleRadius(G4double);
  void SetCollimator2X(G4double);

  void SetShield1YZ(G4double);
  void SetShield1X(G4double);
  void SetShield1HoleRadius(G4double);

  //  void SetShield2YZ(G4double);
  //  void SetShield2X(G4double);
  //  void SetShield2HoleRadius(G4double);

  //  void SetShield3YZ(G4double);
  //  void SetShield3X(G4double);
  //  void SetShield3HoleRadius(G4double);

  void SetSourcetoScintillator(G4double);
  void SetCollimator1toScintillator(G4double);
  void SetCollimator2toScintillator(G4double);
  void SetShield1toScintillator(G4double);
  //  void SetShield2toScintillator(G4double);
  //  void SetShield3toScintillator(G4double);
  void SetHPGetoScintillator(G4double);
  void SetArmAngle(G4double);
  
  void UpdateGeometry();
  void printBasicInfo();

  // probably bad idea but need to access this stuff elsewhere
public:
  void DefineMaterials();
  G4VPhysicalVolume* DefineVolumes();
  G4bool  fCheckOverlaps;

    // geometry parameters
  G4double worldSizeXYZ;

  G4double sourceRadius;
  G4double sourceHoleRadius;
  G4double sourceX;
  G4double sourceHoleX;

  G4double collimator1Radius;
  G4double collimator1HoleRadius;
  G4double collimator1X;
  //G4double collimator1HoleX;
 
  G4double collimator2Radius;
  G4double collimator2HoleRadius;
  G4double collimator2X;
  
  G4double cellRadius;
  G4double cellX;

  G4double diffuserIR;
  G4double diffuserOR;
  G4double diffuserX;

  G4double scintillator1Radius;
  G4double scintillator1X;

  G4double scintillator2Radius;
  G4double scintillator2X;

  G4double shield1YZ; 
  G4double shield1X;
  G4double shield1HoleRadius;

  //  G4double shield2YZ;
  //  G4double shield2X;
  //  G4double shield2HoleRadius;

  //  G4double shield3YZ;
  //  G4double shield3X;
  //  G4double shield3HoleRadius;
  
  G4double HPGeRadius;
  G4double HPGeX;
  
  
};
#endif
