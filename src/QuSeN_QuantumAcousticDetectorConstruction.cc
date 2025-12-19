/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file exoticphysics/phonon/src/QuSeN_QuantumAcousticDetectorConstruction.cc \brief
/// Implementation of the QuSeN_QuantumAcousticDetectorConstruction class
//
// $Id: a2016d29cc7d1e75482bfc623a533d20b60390da $
//
// 20140321  Drop passing placement transform to G4LatticePhysical
// 20211207  Replace G4Logical*Surface with G4CMP-specific versions.
// 20220809  [ For M. Hui ] -- Add frequency dependent surface properties.
// 20221006  Remove unused features; add phonon sensor pad with use of
//		G4CMPPhononElectrode to demonstrate KaplanQP.

#include "QuSeN_QuantumAcousticDetectorConstruction.hh"
#include "QuSeN_QuantumAcousticDetectorParameters.hh"
#include "QuSeN_QuantumAcousticSensitivity.hh"
#include "G4CMPLogicalBorderSurface.hh"
#include "G4CMPPhononElectrode.hh"
#include "G4CMPSurfaceProperty.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4LatticeLogical.hh"
#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"


//using namespace QuSeN_QuantumAcousticDetectorParameters;

using namespace QuasiparticleDetectorParameters;
QuSeN_QuantumAcousticDetectorConstruction::QuSeN_QuantumAcousticDetectorConstruction()
  : fLiquidHelium(0), fSilicon(0), fAluminum(0), fTungsten(0), fCopper(0),
    fNiobium(0),fWorldPhys(0), topSurfProp(0), vacSurfProp(0), wallSurfProp(0), topSurfProp2(0), alNbSurfProp(0),
    electrodeSensitivity(0), fConstructed(false) {;}


QuSeN_QuantumAcousticDetectorConstruction::~QuSeN_QuantumAcousticDetectorConstruction() {
  delete topSurfProp;
  delete topSurfProp2;
  delete vacSurfProp;
  delete wallSurfProp;
  delete alNbSurfProp;
}

G4VPhysicalVolume* QuSeN_QuantumAcousticDetectorConstruction::Construct() {
  if (fConstructed) {
    if (!G4RunManager::IfGeometryHasBeenDestroyed()) {
      // Run manager hasn't cleaned volume stores. This code shouldn't execute
      G4GeometryManager::GetInstance()->OpenGeometry();
      G4PhysicalVolumeStore::GetInstance()->Clean();
      G4LogicalVolumeStore::GetInstance()->Clean();
      G4SolidStore::GetInstance()->Clean();
    }
    // Have to completely remove all lattices to avoid warning on reconstruction
    G4LatticeManager::GetLatticeManager()->Reset();
    // Clear all LogicalSurfaces
    // NOTE: No need to redefine the G4CMPSurfaceProperties
    G4CMPLogicalBorderSurface::CleanSurfaceTable();
  }

  DefineMaterials();
  SetupGeometry();
  fConstructed = true;
  return fWorldPhys;
}

void QuSeN_QuantumAcousticDetectorConstruction::DefineMaterials() {
  G4NistManager* nistManager = G4NistManager::Instance();

  //"Liquid helium" is a historical relic that should be replaced soon.
  //Here we're using it as a standin for vacuum
  fLiquidHelium = nistManager->FindOrBuildMaterial("G4_AIR");
  fSilicon = nistManager->FindOrBuildMaterial("G4_Si");
  fAluminum = nistManager->FindOrBuildMaterial("G4_Al");
  fCopper = nistManager->FindOrBuildMaterial("G4_Cu");
  fTungsten = nistManager->FindOrBuildMaterial("G4_W");
  fNiobium = nistManager->FindOrBuildMaterial("G4_Nb");
}

void QuSeN_QuantumAcousticDetectorConstruction::SetupGeometry() {
  //
  // World
  //
  G4VSolid* worldSolid = new G4Box("World",16.*cm,16.*cm,16.*cm);
  G4LogicalVolume* worldLogical =
    new G4LogicalVolume(worldSolid,fLiquidHelium,"World");
  worldLogical->SetUserLimits(new G4UserLimits(100*mm, DBL_MAX, DBL_MAX, 0, 0));
  fWorldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"World",0,
                                 false,0);


  //Start by defining interface properties (since these are needed by classes
  //we instantiate objects of here)
  if (!fConstructed) {
    const G4double GHz = 1e9 * hertz;

    //the following coefficients and cutoff values are not well-motivated
    //the code below is used only to demonstrate how to set these values.
    const std::vector<G4double> anhCoeffs = {0, 0, 0, 0, 0, 1.51e-14};
    const std::vector<G4double> diffCoeffs = {};
    const std::vector<G4double> specCoeffs = {};

    const G4double anhCutoff = 520., reflCutoff = 350.;   // Units external

    //For the the interface of the Si and Aluminum
    fSiAlInterface = new G4CMPSurfaceProperty("SiAlSurf",
                                              0.0, 1.0, 0.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0,
                                              0.0, 1.0);
    fSiAlInterface->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
                                            diffCoeffs, specCoeffs, GHz, GHz,
                                            GHz);
    fBorderContainer.emplace("SiAl",fSiAlInterface);

    //For the the interface of the Si and the world
    fSiVacInterface = new G4CMPSurfaceProperty("SiVacSurf",
                                               0.0, 1.0, 0.0, 0.0,
                                               0.0, 1.0, 0.0, 0.0,
                                               0.0, 1.0);
    fSiVacInterface->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
                                             diffCoeffs, specCoeffs, GHz, GHz,
                                             GHz);
    fBorderContainer.emplace("SiVac",fSiVacInterface);

    //For the the interface of the Si and the Cu
    fSiCuInterface = new G4CMPSurfaceProperty("SiCuSurf",
                                              0.0, 1.0, 0.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0,
                                              0.0, 1.0);
    fSiCuInterface->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
                                            diffCoeffs, specCoeffs, GHz, GHz,
                                            GHz);
    fBorderContainer.emplace("SiCu",fSiCuInterface);

    //For the the interface of the Cu and the Vac
    fCuVacInterface = new G4CMPSurfaceProperty("CuVacSurf",
                                               0.0, 1.0, 0.0, 0.0,
                                               1.0, 1.0, 0.0, 0.0,
                                               0.0, 1.0);
    fCuVacInterface->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
                                             diffCoeffs, specCoeffs, GHz, GHz,
                                             GHz);
    fBorderContainer.emplace("CuVac",fCuVacInterface);


    //For the the interface of the Al and world
    fAlVacInterface = new G4CMPSurfaceProperty("AlVacSurf",
                                               0.0, 1.0, 0.0, 0.0,
                                               0.0, 1.0, 0.0, 0.0,
                                               0.0, 1.0);
    fAlVacInterface->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
                                             diffCoeffs, specCoeffs, GHz, GHz,
                                             GHz);
    fBorderContainer.emplace("AlVac",fAlVacInterface);

    //For the the interface of the Al and Al
    fAlAlInterface = new G4CMPSurfaceProperty("AlAlSurf",
                                              0.0, 1.0, 0.0, 0.0,
                                              0.0, 1.0, 0.0, 0.0,
                                              0.0, 0.0);
    fAlAlInterface->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
                                            diffCoeffs, specCoeffs, GHz, GHz,
                                            GHz);
    fBorderContainer.emplace("AlAl",fAlAlInterface);

    //For the the interface of the Vac and Vac
    fVacVacInterface = new G4CMPSurfaceProperty("VacVacSurf",
                                                0.0, 1.0, 0.0, 0.0,
                                                0.0, 1.0, 0.0, 0.0,
                                                0.0, 1.0);
    fVacVacInterface->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
                                              diffCoeffs, specCoeffs, GHz, GHz,
                                              GHz);
    fBorderContainer.emplace("VacVac",fVacVacInterface);

  }

  //Also need to define logical lattices *here* now, since the logical lattice
  //container needs to be passed into some classes
  // G4LatticeManager gives physics processes access to lattices by volume
  G4LatticeManager* LM = G4LatticeManager::GetLatticeManager();
  G4LatticeLogical* log_siliconLattice = LM->LoadLattice(fSilicon, "Si");
  G4LatticeLogical* log_aluminumLattice = LM->LoadLattice(fAluminum, "Al");
  G4LatticeLogical* log_copperLattice = LM->LoadLattice(fCopper, "Cu");
  fLogicalLatticeContainer.emplace("Silicon",log_siliconLattice);
  fLogicalLatticeContainer.emplace("Aluminum",log_aluminumLattice);
  fLogicalLatticeContainer.emplace("Copper",log_copperLattice);




  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  // Now we start constructing the various components and their interfaces
  bool checkOverlaps = true;


  //--------------------------------------------------------------------------
  //First, set up the qubit chip substrate.
  G4Box * solid_siliconChip = new G4Box("QubitChip_solid",
                                        0.5*10*cm,
                                        0.5*10*cm,
                                        0.5*1*cm);

  //Now attribute a physical material to the chip
  G4LogicalVolume * log_siliconChip = new G4LogicalVolume(solid_siliconChip,
                                                          fSilicon,
                                                          "SiliconChip_log");


  G4VPhysicalVolume * phys_siliconChip =
    new G4PVPlacement(0,G4ThreeVector(),log_siliconChip,"SiliconChip",
                      worldLogical,false,0,checkOverlaps);

  G4VisAttributes* siliconChipVisAtt =
    new G4VisAttributes(G4Colour(0.5,0.5,0.5));

  siliconChipVisAtt->SetVisibility(true);
  log_siliconChip->SetVisAttributes(siliconChipVisAtt);

  // G4LatticePhysical assigns G4LatticeLogical a physical orientation
  G4LatticePhysical* phys_siliconLattice =
    new G4LatticePhysical(log_siliconLattice);
  phys_siliconLattice->SetMillerOrientation(1,0,0);
  LM->RegisterLattice(phys_siliconChip,phys_siliconLattice);

  //Set up border surfaces
  G4CMPLogicalBorderSurface * border_siliconChip_world =
    new G4CMPLogicalBorderSurface("border_siliconChip_world",phys_siliconChip,
                                  fWorldPhys, fSiVacInterface);





  //--------------------------------------------------------------------------
  //If desired, set up the copper qubit housing


  //-----------------------------------------------------------------
  //Now set up the ground plane, in which the transmission line, resonators,
  //and qubits will be located.


    G4Box * solid_Downconverter =
      new G4Box("GroundPlane_solid",0.5*10*cm,
                0.5*10*cm,0.5*1*cm);

    //Now attribute a physical material to the chip
    G4LogicalVolume * log_Downconverter =
      new G4LogicalVolume(solid_Downconverter,fNiobium,"Downconverter_log");

    //Now, create a physical volume and G4PVPlacement for storing as the final
    //output

    G4VPhysicalVolume * phys_Downconverter =
      new G4PVPlacement(0,G4ThreeVector(0.0,0.0,1.0*cm),log_Downconverter,"Downconverter",
                        worldLogical,false,0,checkOverlaps);

    G4VisAttributes* DownconverterVisAtt =
      new G4VisAttributes(G4Colour(0.0,1.0,1.0,0.5));
    DownconverterVisAtt->SetVisibility(true);
    log_Downconverter->SetVisAttributes(DownconverterVisAtt);

    G4LatticePhysical* DownconverterLattice =
      new G4LatticePhysical(log_aluminumLattice,dp_polycryElScatMFP_Al,
                            dp_scDelta0_Al,dp_scTeff_Al,dp_scDn_Al,
                            dp_scTauQPTrap_Al);
    DownconverterLattice->SetMillerOrientation(1,0,0);
    LM->RegisterLattice(phys_Downconverter,DownconverterLattice);


    //Set up the logical border surface
    G4CMPLogicalBorderSurface * border_siliconChip_Downconverter =
      new G4CMPLogicalBorderSurface("border_siliconChip_groundPlane",
                                    phys_siliconChip, phys_Downconverter,
                                    fSiAlInterface);
    G4CMPLogicalBorderSurface * border_Downconverter_siliconChip =
      new G4CMPLogicalBorderSurface("border_siliconChip_groundPlane",
                                    phys_Downconverter, phys_siliconChip,
                                    fSiAlInterface);
    G4CMPLogicalBorderSurface * border_world_Downconverter =
      new G4CMPLogicalBorderSurface("border_world_groundPlane", fWorldPhys,
                                    phys_Downconverter, fAlVacInterface);
    G4CMPLogicalBorderSurface * border_Downconverter_world =
      new G4CMPLogicalBorderSurface("border_groundPlane_world",
                                    phys_Downconverter, fWorldPhys,
                                    fAlVacInterface);

    //----------------------------------------------------------------------



}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

// Attach material properties and electrode/sensor handler to surface

void QuSeN_QuantumAcousticDetectorConstruction::
AttachPhononSensor(G4CMPSurfaceProperty* surfProp) {
  if (!surfProp) return;		// No surface, nothing to do

  // Specify properties of aluminum sensor, same on both detector faces
  // See G4CMPPhononElectrode.hh or README.md for property keys

  /*
  // Properties must be added to existing surface-property table
  auto sensorProp = surfProp->GetPhononMaterialPropertiesTablePointer();
  sensorProp->AddConstProperty("filmAbsorption", 0.20);    // True sensor area
  sensorProp->AddConstProperty("filmThickness", 600.*nm);
  sensorProp->AddConstProperty("gapEnergy", 173.715e-6*eV);
  sensorProp->AddConstProperty("lowQPLimit", 3.);
  sensorProp->AddConstProperty("phononLifetime", 242.*ps);
  sensorProp->AddConstProperty("phononLifetimeSlope", 0.29);
  sensorProp->AddConstProperty("vSound", 3.26*km/s);
  sensorProp->AddConstProperty("subgapAbsorption", 0.1);

  // Attach electrode object to handle KaplanQP interface
  surfProp->SetPhononElectrode(new G4CMPPhononElectrode);
  */
}
