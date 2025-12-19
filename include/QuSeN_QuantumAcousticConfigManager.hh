/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef QuSeN_QuantumAcousticConfigManager_hh
#define QuSeN_QuantumAcousticConfigManager_hh 1

// $Id$
// File:  QuSeN_QuantumAcousticConfigManager.hh
//
// Description:	Singleton container class for user configuration of G4CMP
//		QuSeN_QuantumAcoustic example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see QuSeN_QuantumAcousticConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "globals.hh"

class QuSeN_QuantumAcousticConfigMessenger;


class QuSeN_QuantumAcousticConfigManager {
public:
  ~QuSeN_QuantumAcousticConfigManager();	// Must be public for end-of-job cleanup
  static QuSeN_QuantumAcousticConfigManager* Instance();   // Only needed by static accessors

  // Access current values
  static const G4String& GetHitOutput()  { return Instance()->Hit_file; }

  // Change values (e.g., via Messenger)
  static void SetHitOutput(const G4String& name)
  { Instance()->Hit_file=name; UpdateGeometry(); }

  static void UpdateGeometry();

private:
  QuSeN_QuantumAcousticConfigManager();		// Singleton: only constructed on request
  QuSeN_QuantumAcousticConfigManager(const QuSeN_QuantumAcousticConfigManager&) = delete;
  QuSeN_QuantumAcousticConfigManager(QuSeN_QuantumAcousticConfigManager&&) = delete;
  QuSeN_QuantumAcousticConfigManager& operator=(const QuSeN_QuantumAcousticConfigManager&) = delete;
  QuSeN_QuantumAcousticConfigManager& operator=(QuSeN_QuantumAcousticConfigManager&&) = delete;

  static QuSeN_QuantumAcousticConfigManager* theInstance;

private:
  G4String Hit_file;	// Output file of e/h hits ($G4CMP_HIT_FILE)

  QuSeN_QuantumAcousticConfigMessenger* messenger;
};

#endif	/* QuSeN_QuantumAcousticConfigManager_hh */
