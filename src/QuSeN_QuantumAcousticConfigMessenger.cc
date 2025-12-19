/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
// File:  QuSeN_QuantumAcousticConfigMessenger.cc
//
// Description:	Macro command defitions to set user configuration in
//		QuSeN_QuantumAcousticConfigManager.
//
// 20170816  Michael Kelsey

#include "QuSeN_QuantumAcousticConfigMessenger.hh"
#include "QuSeN_QuantumAcousticConfigManager.hh"
#include "G4UIcmdWithAString.hh"


// Constructor and destructor

QuSeN_QuantumAcousticConfigMessenger::
QuSeN_QuantumAcousticConfigMessenger(QuSeN_QuantumAcousticConfigManager* mgr)
  : G4UImessenger("/g4cmp/", "User configuration for G4CMP phonon example"),
    theManager(mgr), hitsCmd(0) {
  hitsCmd =
    CreateCommand<G4UIcmdWithAString>("HitsFile",
                                      "Set filename for output of phonon hit locations");
}


QuSeN_QuantumAcousticConfigMessenger::~QuSeN_QuantumAcousticConfigMessenger() {
  delete hitsCmd; hitsCmd=0;
}


// Parse user input and add to configuration

void QuSeN_QuantumAcousticConfigMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
  if (cmd == hitsCmd) theManager->SetHitOutput(value);
}
