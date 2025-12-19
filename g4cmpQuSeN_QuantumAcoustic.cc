/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file QuSeN_QuantumAcoustic/QuSeN_QuantumAcoustic.cc
/// \brief Main program of the G4CMP/QuSeN_QuantumAcoustic For neutrino Simualtions
//
// $Id$
//

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "G4CMPPhysicsList.hh"
#include "G4CMPConfigManager.hh"
#include "QuSeN_QuantumAcousticActionInitialization.hh"
#include "QuSeN_QuantumAcousticConfigManager.hh"
#include "QuSeN_QuantumAcousticDetectorConstruction.hh"
#include "QuSeN_QuantumAcousticDetectorParameters.hh"

int main(int argc,char** argv)
{
 // Construct the run manager
 //
 G4RunManager * runManager = new G4RunManager;

 // Set mandatory initialization classes
 //
 QuSeN_QuantumAcousticDetectorConstruction* detector = new QuSeN_QuantumAcousticDetectorConstruction();
 runManager->SetUserInitialization(detector);

 G4VUserPhysicsList* physics = new G4CMPPhysicsList();
 physics->SetCuts();
 runManager->SetUserInitialization(physics);

 // Set user action classes (different for Geant4 10.0)
 //
 runManager->SetUserInitialization(new QuSeN_QuantumAcousticActionInitialization);

 // Create configuration managers to ensure macro commands exist
 G4CMPConfigManager::Instance();
 QuSeN_QuantumAcousticConfigManager::Instance();

 // Visualization manager
 //
 G4VisManager* visManager = new G4VisExecutive;
 visManager->Initialize();

 // Get the pointer to the User Interface manager
 //
 G4UImanager* UImanager = G4UImanager::GetUIpointer();
 //
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
   //  UImanager->ApplyCommand("/control/execute vis.mac");
  UImanager->ApplyCommand("/run/initialize");
   UImanager->ApplyCommand("/gps/number 1");
   UImanager->ApplyCommand("/gps/particle phononTF");
 UImanager->ApplyCommand("/vis/open OGL 600x600-0+0");
 UImanager->ApplyCommand("/vis/viewer/set/style cloud");
 UImanager->ApplyCommand("/vis/drawVolume");
 // Enable Phonon Processes
 UImanager->ApplyCommand("/g4cmp/phononBounces 3000");
 UImanager->ApplyCommand("/process/inactivate phononScattering");
 UImanager->ApplyCommand("/process/setVerbose 0 G4CMPSecondaryProduction");
 UImanager->ApplyCommand("/process/inactivate phononDownconversion");
 UImanager->ApplyCommand("/process/inactivate phononScattering");
 UImanager->ApplyCommand("/process/inactivate phononDownconversion");
 UImanager->ApplyCommand("/process/inactivate phononPolycrystalElasticScattering");
 UImanager->ApplyCommand("/gps/energy 0.009 eV");
 UImanager->ApplyCommand("/gps/pos/type Volume");
 UImanager->ApplyCommand("/gps/pos/shape Para");
 UImanager->ApplyCommand("/gps/ang/type iso");
UImanager->ApplyCommand("/gps/direction 0.0 0.0 1.0");
 UImanager->ApplyCommand("/gps/pos/centre 0.0 0.0 0.0 cm");
UImanager->ApplyCommand("/random/setSeeds 1");
UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
UImanager->ApplyCommand("/vis/scene/add/trajectories");




     // Auto Refresh Viewer
     UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
     UImanager->ApplyCommand("/vis/viewer/refresh");

     // Run Simulation
     UImanager->ApplyCommand("/run/beamOn 1");


    ui->SessionStart();


// G4UImanager* UImanager = G4UImanager::GetUIpointer();
//
//  if (argc==1)   // Define UI session for interactive mode
//  {
//       G4UIExecutive * ui = new G4UIExecutive(argc,argv);
//       ui->SessionStart();
//       delete ui;
//  }
//  else           // Batch mode
//  {
//    G4String command = "/control/execute ";
//    G4String fileName = argv[1];
//    UImanager->ApplyCommand(command+fileName);
//  }
//
//  delete visManager;
//  delete runManager;
//
//  return 0;


}
