/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef QuSeN_QuantumAcousticConfigMessenger_hh
#define QuSeN_QuantumAcousticConfigMessenger_hh 1

// $Id$
// File:  QuSeN_QuantumAcousticConfigMessenger.hh
//
// Description:	Macro command defitions to set user configuration in
//		QuSeN_QuantumAcousticConfigManager.
//
// 20170816  Michael Kelsey

#include "G4UImessenger.hh"

class QuSeN_QuantumAcousticConfigManager;
class G4UIcmdWithAString;
class G4UIcommand;


class QuSeN_QuantumAcousticConfigMessenger : public G4UImessenger {
public:
  QuSeN_QuantumAcousticConfigMessenger(QuSeN_QuantumAcousticConfigManager* theData);
  virtual ~QuSeN_QuantumAcousticConfigMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String value);

private:
  QuSeN_QuantumAcousticConfigManager* theManager;
  G4UIcmdWithAString* hitsCmd;

private:
  QuSeN_QuantumAcousticConfigMessenger(const QuSeN_QuantumAcousticConfigMessenger&);	// Copying is forbidden
  QuSeN_QuantumAcousticConfigMessenger& operator=(const QuSeN_QuantumAcousticConfigMessenger&);
};

#endif /* QuSeN_QuantumAcousticConfigMessenger_hh */
