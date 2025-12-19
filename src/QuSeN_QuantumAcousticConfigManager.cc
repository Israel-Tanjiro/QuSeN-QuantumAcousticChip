/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
// File:  QuSeN_QuantumAcousticConfigManager.cc
//
// Description:	Singleton container class for user configuration of G4CMP
//		QuSeN_QuantumAcoustic example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see QuSeN_QuantumAcousticConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "QuSeN_QuantumAcousticConfigManager.hh"
#include "QuSeN_QuantumAcousticConfigMessenger.hh"
#include "G4RunManager.hh"
#include <stdlib.h>


// Constructor and Singleton Initializer

QuSeN_QuantumAcousticConfigManager* QuSeN_QuantumAcousticConfigManager::theInstance = 0;

QuSeN_QuantumAcousticConfigManager* QuSeN_QuantumAcousticConfigManager::Instance() {
  if (!theInstance) theInstance = new QuSeN_QuantumAcousticConfigManager;
  return theInstance;
}

QuSeN_QuantumAcousticConfigManager::QuSeN_QuantumAcousticConfigManager()
  : Hit_file(getenv("G4CMP_HIT_FILE")?getenv("G4CMP_HIT_FILE"):"phonon_hits.txt"),
    messenger(new QuSeN_QuantumAcousticConfigMessenger(this)) {;}

QuSeN_QuantumAcousticConfigManager::~QuSeN_QuantumAcousticConfigManager() {
  delete messenger; messenger=0;
}


// Trigger rebuild of geometry if parameters change

void QuSeN_QuantumAcousticConfigManager::UpdateGeometry() {
  G4RunManager::GetRunManager()->ReinitializeGeometry(true);
}
