// ComptonDetectorConstruction.cc

#include "ComptonDetectorConstruction.hh"
#include "ComptonCalorimeterSD.hh"
#include "ComptonDetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"

// ComptonDetectorConstruction object constructor, from parent class G4UserDetectorConstruction
ComptonDetectorConstruction::ComptonDetectorConstruction()
  : G4VUserDetectorConstruction(),
    fCheckOverlaps(true)

{
  // create commands for interactive definition of geometry
  ComptonDetectorMessenger* detectorMessenger = new ComptonDetectorMessenger(this);

    // DEFAULT geometry parameters
  worldSizeXYZ = 1.*m;
  // coordinate system: x right, y back, z up
  
  // collimator system
  // collimator #1
  collimator1Radius = 6.*cm;
  collimator1HoleRadius = 10.*mm;
  collimator1X = 10.*cm;
  //collimator1HoleX = collimator1X/2.;
  
// collimator #2
  //G4bool twoCollimator = true;
  collimator2Radius = collimator1Radius;
  collimator2HoleRadius = 1.2*collimator1HoleRadius; //from Daya Bay (1cm)
  collimator2X = 5.*cm;

  // source housing
  sourceRadius = collimator1Radius;
  sourceHoleRadius = 2*cm;
  sourceX = 10*cm;
  sourceHoleX = sourceX/2.;

  // target ordering: diffuser, acrylic cell, scintillator
  // RIGHT NOW no aluminum casing
  // acrylic cell
  cellRadius = (3.*2.54)/2*cm; //3" PMTs
  cellX = (3.*2.54)/2*cm;

  // diffuser coating (parameters defined by acrylic cell)
  diffuserIR = cellRadius; 
  diffuserOR = diffuserIR + .10*mm; // ??
  diffuserX = cellX;
  // scintillator target (define by mL, how best to do this ??)
  scintillator1Radius = cellRadius-(.25*2.54)*cm; //thickness of UVT??
  scintillator1X = cellX - (.25*2.54)*cm; //thickness of UVT??

  //see how many hits are in the smaller volume, interaction region 20%?
  scintillator2Radius = scintillator1Radius*0.2;
  scintillator2X = scintillator1X*0.2;

  // shield system
  // shield #1
  shield1YZ =  5.*cm; // ?? 
  shield1X = 85.*mm; //from Daya Bay for 45deg
  shield1HoleRadius = 15.*mm; //from Daya Bay for 45deg
  //shield #2
  //shield2YZ = 5.*cm; // ??
  //shield2X = 85.*mm; //from Daya Bay
  //shield2HoleRadius = 15./2.; //from Daya Bay
  //shield #3
  //shield3YZ = 5.*cm; // ??
  //shield3X = 65.*mm; //from Daya Bay
  //shield3HoleRadius = 25./2.; //from Daya Bay

  // HPGe detector (crystal only)
  HPGeRadius = 49.6/2*mm; //from spec sheet
  HPGeX = 59.6*mm; //from spec sheet
  
  // positioning of components (with respect to the target)
  // NEED TO BE CENTROID TO CENTROID
  sourcetoScintillator = .75*m;
  collimator1toScintillator = sourcetoScintillator - sourceX - collimator1X - .5*cm; 
  collimator2toScintillator = sourcetoScintillator - sourceX - collimator1X - collimator2X - 15*cm; 
  shield1toScintillator = diffuserOR + 15.*cm; //?? 
  //shield2toScintillator = shield1toScintillator + shield2X + .15*m;
  //shield3toScintillator = shield1toScintillator + shield2X + shield3X + .3*m;
  HPGetoScintillator = .75*m; 

  //
  // rotation of spectrometer arm
  armAngle = 45.*deg; //angle between HPGe
}

// ComptonDetectorConstruction object destructor
ComptonDetectorConstruction::~ComptonDetectorConstruction()
{ delete detectorMessenger; }

// construct physical volume
G4VPhysicalVolume* ComptonDetectorConstruction::Construct()
{
  // define materials
  DefineMaterials();

  // define volumes
  return DefineVolumes();
}

// DefineMaterials() definition
void ComptonDetectorConstruction::DefineMaterials()
{
  // materials defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_AIR");
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_PLEXIGLASS"); //use UVT when implement PMTs
  nistManager->FindOrBuildMaterial("G4_Ge");

  // define elements for EJ309 (LS) and EJ510 (diffuse reflector)
  G4Element* elH = new G4Element("Hydrogen", "H", 1., 1.01*g/mole);
  G4Element* elC = new G4Element("Carbon", "C", 6., 12.00*g/mole);
  G4Element* elO = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
  G4Element* elTi = new G4Element("Titanium", "C", 22., 47.87*g/mole);

  G4double scintillatorDensity = 0.959*g/cm3;
  G4double diffuserDensity = 0.118*g/cm3;
  G4double massFraction;
  G4int nComponents, nAtoms;

  // EJ309 definition
  G4Material* EJ309 = new G4Material("EJ309", scintillatorDensity, nComponents = 2);
  EJ309->AddElement(elH, nAtoms = 5);
  EJ309->AddElement(elC, nAtoms = 4);

  // EJ510 definition
  G4Material* EJ510 = new G4Material("EJ510", diffuserDensity, nComponents = 4);
  EJ510->AddElement(elH, massFraction = 0.02899);
  EJ510->AddElement(elC, massFraction = 0.17194);
  EJ510->AddElement(elO, massFraction = 0.38854);
  EJ510->AddElement(elTi, massFraction = 0.41053);

  //  G4Material* fGlass = new G4Material("Glass", density=1.032*g/cm3,2);
  //fGlass->AddElement(elC, massFraction = .91533);
  //fGlass->AddElement(elH, massFraction = .08467);

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

// DefineVolumes() definition
G4VPhysicalVolume* ComptonDetectorConstruction::DefineVolumes()
{
  
  // Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4ThreeVector armPosition = G4ThreeVector(std::cos(armAngle), std::sin(armAngle),0.);
  G4RotationMatrix armRotation = G4RotationMatrix();
  armRotation.rotateY(90*deg);
  armRotation.rotateZ(armAngle);
  // rotation of cylinders to be along x
  G4RotationMatrix cylXrotation = G4RotationMatrix();
  cylXrotation.rotateY(90*deg);

  // get materials
  G4Material* air = G4Material::GetMaterial("G4_AIR");
  G4Material* lead = G4Material::GetMaterial("G4_Pb");
  G4Material* germanium = G4Material::GetMaterial("G4_Ge");
  G4Material* scintillatorMaterial = G4Material::GetMaterial("EJ309");
  G4Material* diffuserMaterial = G4Material::GetMaterial("EJ510");
  G4Material* cellMaterial = G4Material::GetMaterial("G4_PLEXIGLASS");

  // world
  G4VSolid* worldS = new G4Box("World", worldSizeXYZ, worldSizeXYZ, worldSizeXYZ);
  G4LogicalVolume* worldLV = new G4LogicalVolume(worldS, air, "World");
  G4VPhysicalVolume* worldPV = new G4PVPlacement(
						 0,                // no rotation
						 G4ThreeVector(),  // at (0,0,0)
						 worldLV,          // its logical volume
						 "World",          // its name
						 0,                // its mother  volume
						 false,            // no boolean operation
						 0,                // copy number
						 fCheckOverlaps);  // checking overlaps

  // define logical volume of target = diffuser+cell+scintillator
  // caloriimeter = scintillator logical volume
  // positions of other components are with respect to this object
  G4VSolid* diffuserS = new G4Tubs("diffuserS", diffuserIR, diffuserOR, diffuserX, 0.*deg, 360.*deg);
  G4LogicalVolume* diffuserLV = new G4LogicalVolume(diffuserS, diffuserMaterial, "diffuserLV");
  new G4PVPlacement(
		    0,                 // no rotation
		    G4ThreeVector(),  // at (0,0,0)
		    diffuserLV,        // its logical volume
		    "Diffuser",        // its name
		    worldLV,           // its mother  volume
		    false,             // no boolean operation
		    0,                 // copy number
		    fCheckOverlaps);   // checking overlaps
  
  G4VSolid* cellS = new G4Tubs("cellS", 0, cellRadius, cellX, 0.*deg, 360.*deg);
  G4LogicalVolume* cellLV = new G4LogicalVolume(cellS, cellMaterial, "cellLV");
  new G4PVPlacement(
		    0,                 // no rotation
		    G4ThreeVector() ,  // at (0,0,0)
		    cellLV,            // its logical volume
		    "Cell",            // its name
		    worldLV,           // its mother  volume BE CAREFUL HERE!! 
		    false,             // no boolean operation
		    0,                 // copy number
		    fCheckOverlaps);   // checking overlaps
    
  G4VSolid* scintillator1S = new G4Tubs("scintillator1S", 0., scintillator1Radius, scintillator1X, 0.*deg, 360.*deg);
  G4LogicalVolume* scintillator1LV = new G4LogicalVolume(scintillator1S, scintillatorMaterial, "scintillator1LV");
  new G4PVPlacement(
		    0,                 // no rotation
		    G4ThreeVector(),  // at (0,0,0)
		    scintillator1LV,     // its logical volume
		    "Scintillator1",     // its name
		    cellLV,            // its mother  volume
		    false,             // no boolean operation
		    0,                 // copy number
		    fCheckOverlaps);   // checking overlaps

  G4VSolid* scintillator2S = new G4Tubs("scintillator2S", 0., scintillator2Radius, scintillator2X, 0.*deg, 360.*deg);
  G4LogicalVolume* scintillator2LV = new G4LogicalVolume(scintillator2S, scintillatorMaterial, "scintillator2LV");
  new G4PVPlacement(
		    0,                 // no rotation
		    G4ThreeVector(),  // at (0,0,0)
		    scintillator2LV,     // its logical volume
		    "Scintillator2",     // its name
		    scintillator1LV,            // its mother  volume
		    false,             // no boolean operation
		    0,                 // copy number
		    fCheckOverlaps);   // checking overlaps


 
  // collimators (lead cylinder + air cylinder)
  G4VSolid* collimator1S = new G4Tubs("collimator1S", 0, collimator1Radius, collimator1X, 0.*deg, 360.*deg);
  G4LogicalVolume* collimator1LV = new G4LogicalVolume(collimator1S, lead, "collimator1LV");
  G4Transform3D collimator1Transform = G4Transform3D(cylXrotation, G4ThreeVector(-collimator1toScintillator, 0., 0.));
  new G4PVPlacement(
		    collimator1Transform,   // transform
		    //0,
		    //G4ThreeVector(-collimator1toScintillator, 0., 0.),
		    collimator1LV,          // its logical volume
		    "Collimator1",          // its name
		    worldLV,                // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps

  // NOTE: coordinates of daughters are respect to mother volume, NOT WORLD
  G4VSolid* collimator1HoleS = new G4Tubs("collimator1HoleS", 0., collimator1HoleRadius, collimator1X, 0.*deg, 360.*deg);
  G4LogicalVolume* collimator1HoleLV = new G4LogicalVolume(collimator1HoleS, air, "collimator1HoleLV");
  //G4Transform3D collimator1HoleTransform = G4Transform3D(0, G4ThreeVector(0., 0., 0.));

  new G4PVPlacement(
		    // collimator1HoleTransform,
		    0,
		    G4ThreeVector(0.,0.,0.),
		    collimator1HoleLV,      // its logical volume
		    "Collimator1Hole",      // its name
		    collimator1LV,          // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps
  


  G4VSolid* collimator2S = new G4Tubs("collimator2S", 0., collimator2Radius, collimator2X, 0.*deg, 360.*deg);
  G4LogicalVolume* collimator2LV = new G4LogicalVolume(collimator2S, lead, "collimator2LV");
  G4Transform3D collimator2Transform = G4Transform3D(cylXrotation, G4ThreeVector(-collimator2toScintillator, 0., 0.));
  new G4PVPlacement(
		    collimator2Transform,   // transform
		    // 0,
		    //G4ThreeVector(-collimator2toScintillator, 0., 0.),
		    collimator2LV,          // its logical volume
		    "Collimator2",          // its name
		    worldLV,                // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps

  G4VSolid* collimator2HoleS = new G4Tubs("collimator2HoleS", 0, collimator2HoleRadius, collimator2X, 0.*deg, 360.*deg);
  G4LogicalVolume* collimator2HoleLV = new G4LogicalVolume(collimator2HoleS, air, "collimator2HoleLV");
  // G4Transform3D collimator2HoleTransform = G4Transform3D(cylXrotation, G4ThreeVector(0., 0., 0.));
  new G4PVPlacement(
		    //collimator2HoleTransform,
		    0,
		    G4ThreeVector(0.,0.,0.),
		    collimator2HoleLV,      // its logical volume
		    "Collimator2Hole",      // its name
		    collimator2LV,          // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps


  G4VSolid* sourceS = new G4Tubs("sourceS", 0, sourceRadius, sourceX, 0.*deg, 360.*deg);
  G4LogicalVolume* sourceLV = new G4LogicalVolume(sourceS, lead, "sourceLV");
  G4Transform3D sourceTransform = G4Transform3D(cylXrotation, G4ThreeVector(-sourcetoScintillator, 0., 0.));
  new G4PVPlacement(
		    sourceTransform,
		    //0,        // transform
		    //G4ThreeVector(-sourcetoScintillator, 0., 0.),
		    sourceLV,               // its logical volume
		    "Source",               // its name
		    worldLV,                // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps

  // NOTE: coordinates of daughters are respect to mother volume, NOT WORLD
  G4VSolid* sourceHoleS = new G4Tubs("sourceHoleS", 0., sourceHoleRadius, sourceHoleX, 0.*deg, 360.*deg);
  G4LogicalVolume* sourceHoleLV = new G4LogicalVolume(sourceHoleS, air, "sourceHoleLV");
  //G4Transform3D collimator1HoleTransform = G4Transform3D(0, G4ThreeVector(0., 0., 0.));

  new G4PVPlacement(
		    //collimator1HoleTransform,
		    0,
		    G4ThreeVector(0.,0.,sourceHoleX),
		    sourceHoleLV,           // its logical volume
		    "SourceHole",           // its name
		    sourceLV,               // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps
  

  // shields
  G4VSolid* shield1S = new G4Box("shield1S", shield1YZ, shield1YZ, shield1X);
  G4LogicalVolume* shield1LV = new G4LogicalVolume(shield1S, lead, "shield1LV");
  G4Transform3D shield1Transform = G4Transform3D(armRotation, shield1toScintillator*armPosition);
  new G4PVPlacement(
		    shield1Transform,
		    //0,                      // rotation
		    //G4ThreeVector(shield1toScintillator, 0., 0.),
		    shield1LV,              // its logical volume
		    "Shield1",              // its name
		    worldLV,                // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps

  G4VSolid* shield1HoleS = new G4Tubs("shield1HoleS", 0, shield1HoleRadius, shield1X, 0.*deg, 360.*deg);
  G4LogicalVolume* shield1HoleLV = new G4LogicalVolume(shield1HoleS, air, "shield1HoleLV");
  //G4Transform3D shield1HoleTransform = G4Transform3D(armRotation, shield1toScintillator*armPosition);
  //G4Transform3D shield1HoleTransform = G4Transform3D(cylXrotation, G4ThreeVector(0., 0., 0.));
  new G4PVPlacement(
		    //shield1HoleTransform,
		    0,
		    G4ThreeVector(0., 0., 0.),
		    shield1HoleLV,          // its logical volume
		    "Shield1Hole",          // its name
		    shield1LV,              // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps

  //  G4VSolid* shield2S = new G4Box("shield2S", shield2X, shield2YZ, shield2YZ);
  //  G4LogicalVolume* shield2LV = new G4LogicalVolume(shield2S, lead, "shield2LV");
  //  new G4PVPlacement(
  //		    0,                      // rotation
  //		    G4ThreeVector(shield2toScintillator, 0., 0.),
  //		    shield2LV,              // its logical volume
  //		    "Shield2",              // its name
  //		    worldLV,                // its mother  volume
  //		    false,                  // no boolean operation
  //		    0,                      // copy number
  //		    fCheckOverlaps);        // checking overlaps

//  G4VSolid* shield2HoleS = new G4Tubs("shield2HoleS", 0, shield2HoleRadius, shield2X, 0.*deg, 360.*deg);
//  G4LogicalVolume* shield2HoleLV = new G4LogicalVolume(shield2HoleS, air, "shield2HoleLV");
//  G4Transform3D shield2HoleTransform = G4Transform3D(cylXrotation, G4ThreeVector(0., 0., 0.));
//  new G4PVPlacement(
//		    shield2HoleTransform,
//		    shield2HoleLV,          // its logical volume
//		    "Shield2Hole",          // its name
//		    shield2LV,              // its mother  volume
//		    false,                  // no boolean operation
//		    0,                      // copy number
//		    fCheckOverlaps);        // checking overlaps

//  G4VSolid* shield3S = new G4Box("shield3S", shield3X, shield3YZ, shield3YZ);
//  G4LogicalVolume* shield3LV = new G4LogicalVolume(shield3S, lead, "shield3LV");
//  new G4PVPlacement(
//		    0,                      // rotation
//		    G4ThreeVector(shield3toScintillator, 0., 0.),
//		    shield3LV,              // its logical volume
//		    "Shield3",              // its name
//		    worldLV,                // its mother  volume
//		    false,                  // no boolean operation
//		    0,                      // copy number
//		    fCheckOverlaps);        // checking overlaps

//  G4VSolid* shield3HoleS = new G4Tubs("shield3HoleS", 0, shield3HoleRadius, shield3X, 0.*deg, 360.*deg);
//  G4LogicalVolume* shield3HoleLV = new G4LogicalVolume(shield3HoleS, air, "shield3HoleLV");
//  G4Transform3D shield3HoleTransform = G4Transform3D(cylXrotation, G4ThreeVector(0., 0., 0.));
//  new G4PVPlacement(
//		    shield3HoleTransform,
//		    shield3HoleLV,          // its logical volume
//		    "Shield3Hole",          // its name
//		    shield3LV,              // its mother  volume
//		    false,                  // no boolean operation
//		    0,                      // copy number
//		    fCheckOverlaps);        // checking overlaps

  // HPGe detector
  G4VSolid* HPGeS = new G4Tubs("HPGeS", 0, HPGeRadius, HPGeX, 0.*deg, 360.*deg);
  G4LogicalVolume* HPGeLV = new G4LogicalVolume(HPGeS, germanium, "HPGeLV");
  G4Transform3D HPGeTransform = G4Transform3D(armRotation, HPGetoScintillator*armPosition);
  new G4PVPlacement(
		    HPGeTransform,          // transform
		    HPGeLV,                 // its logical volume
		    "HPGe",                 // its name
		    worldLV,                // its mother  volume
		    false,                  // no boolean operation
		    0,                      // copy number
		    fCheckOverlaps);        // checking overlaps
 
  // Visualization attributes
  worldLV->SetVisAttributes(G4VisAttributes::Invisible);
  //lead, gray
  sourceLV->SetVisAttributes(G4VisAttributes(G4Colour(0.5, 0.5, 0.5)));
  collimator1LV->SetVisAttributes(G4VisAttributes(G4Colour(0.5, 0.5, 0.5))); 
  collimator2LV->SetVisAttributes(G4VisAttributes(G4Colour(0.5, 0.5, 0.5))); 
  shield1LV->SetVisAttributes(G4VisAttributes(G4Colour(0.5, 0.5, 0.5))); 
  //  shield2LV->SetVisAttributes(G4VisAttributes(G4Colour(0.5, 0.5, 0.5))); 
  //  shield3LV->SetVisAttributes(G4VisAttributes(G4Colour(0.5, 0.5, 0.5)));   
  //diffuser, green with alpha (transparency)
  diffuserLV->SetVisAttributes(G4VisAttributes(G4Colour(0., 1.0, 0., 0.2))); 
  //cell, light gray with alpha (transparency)
  cellLV->SetVisAttributes(G4VisAttributes(G4Colour(199./255, 193./255, 183./255, 0.5))); 
  //scintillator, light blue
  scintillator1LV->SetVisAttributes(G4VisAttributes(G4Colour(124./255, 179./255, 226./255, 0.5)));
  scintillator2LV->SetVisAttributes(G4VisAttributes(G4Colour(0.,0.,0.)));
  //HPGe, red
  HPGeLV->SetVisAttributes(G4VisAttributes(G4Colour(1.0, 0., 0.))); 

  //always return physical world
  return worldPV;
}

// needed for multithreaded mode
// "To reduce memory consumption geometry is shared among threads, but sensitive-detectors are not. For technical reasons also the magnetic field cannot be shared (every component that is non-invariant during the event loop must be thread-local)."
void ComptonDetectorConstruction::ConstructSDandField()
{
  ComptonCalorimeterSD* scintillator1SD = new ComptonCalorimeterSD("Scintillator1SD", "Scintillator1Hits");
  SetSensitiveDetector("scintillator1LV",scintillator1SD);   
  ComptonCalorimeterSD* scintillator2SD = new ComptonCalorimeterSD("Scintillator2SD", "Scintillator2Hits");
  SetSensitiveDetector("scintillator2LV",scintillator2SD);
  ComptonCalorimeterSD* HPGeSD = new ComptonCalorimeterSD("HPGeSD", "HPGeHits");
  SetSensitiveDetector("HPGeLV",HPGeSD);
} 

// collimators
void ComptonDetectorConstruction::SetCollimator1Radius(G4double val)
{ collimator1Radius = val; } 

void ComptonDetectorConstruction::SetCollimator1HoleRadius(G4double val)
{ collimator1HoleRadius = val; }

void ComptonDetectorConstruction::SetCollimator1X(G4double val)
{ collimator1X = val; }

void ComptonDetectorConstruction::SetCollimator2Radius(G4double val)
{ collimator2Radius = val; }

void ComptonDetectorConstruction::SetCollimator2HoleRadius(G4double val)
{ collimator2HoleRadius = val; }

void ComptonDetectorConstruction::SetCollimator2X(G4double val)
{ collimator2X = val; }

// shields
void ComptonDetectorConstruction::SetShield1YZ(G4double val)
{ shield1YZ = val; }

void ComptonDetectorConstruction::SetShield1X(G4double val)
{ shield1X = val; }

void ComptonDetectorConstruction::SetShield1HoleRadius(G4double val)
{ shield1HoleRadius = val; }

//void ComptonDetectorConstruction::SetShield2YZ(G4double val)
//{ shield2YZ = val; }

//void ComptonDetectorConstruction::SetShield2X(G4double val)
//{ shield2X = val; }

//void ComptonDetectorConstruction::SetShield2HoleRadius(G4double val)
//{ shield2HoleRadius = val; }

//void ComptonDetectorConstruction::SetShield3YZ(G4double val)
//{ shield3YZ = val; }

//void ComptonDetectorConstruction::SetShield3X(G4double val)
//{ shield3X = val; }

//void ComptonDetectorConstruction::SetShield3HoleRadius(G4double val)
//{ shield3HoleRadius = val; }

void ComptonDetectorConstruction::SetSourcetoScintillator(G4double val)
{ sourcetoScintillator = val; }

void ComptonDetectorConstruction::SetCollimator1toScintillator(G4double val)
{ collimator1toScintillator = val; }

void ComptonDetectorConstruction::SetCollimator2toScintillator(G4double val)
{ collimator2toScintillator = val; }

void ComptonDetectorConstruction::SetShield1toScintillator(G4double val)
{ shield1toScintillator = val; }

//void ComptonDetectorConstruction::SetShield2toScintillator(G4double val)
//{ shield2toScintillator = val; }

//void ComptonDetectorConstruction::SetShield3toScintillator(G4double val)
//{ shield3toScintillator = val; }

void ComptonDetectorConstruction::SetHPGetoScintillator(G4double val)
{ HPGetoScintillator = val; }

void ComptonDetectorConstruction::SetArmAngle(G4double val)
{ armAngle = val; }


// update
void ComptonDetectorConstruction::UpdateGeometry()
{ 
  G4RunManager::GetRunManager()->DefineWorldVolume(DefineVolumes());
  G4RunManager::GetRunManager()->ReinitializeGeometry(); 
}

void ComptonDetectorConstruction::printBasicInfo()
{
  G4cout << "Arm Angle = " << G4int(180./3.14159*armAngle) << "deg" << G4endl;
  G4cout << "Source Distance = " << sourcetoScintillator << "cm" << G4endl;

  G4cout << "Collimator 1:\n" 
  "OD = " << collimator1Radius << "cm\n"
  "ID = " << collimator1HoleRadius << "mm\n"
  "Length = " << collimator1X << "cm\n"
  "Distance = " << collimator1toScintillator << "cm" << G4endl;
 
  G4cout << "Collimator 2:\n "
  "OD = " << collimator2Radius << "cm\n"
  "ID = " << collimator2HoleRadius << "mm\n"
  "Length = " << collimator2X << "cm\n"
  "Distance = " << collimator2toScintillator << "cm" <<G4endl; 
 
  G4cout << "Shield:\n "
  "Width/Height = " << shield1YZ << "cm\n"
  "Thickness = " << shield1X << "cm\n"
  "ID = " << shield1HoleRadius << "mm\n"
  "Distance= " << shield1toScintillator << "cm" << G4endl; 
}
