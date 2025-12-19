/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: 539f524339ae53ad098a07cfa3bebd07784d23dd $

#include "QuSeN_QuantumAcousticActionInitialization.hh"
#include "QuSeN_QuantumAcousticPrimaryGeneratorAction.hh"
#include "QuSeN_QuantumAcousticSteppingAction.hh"
#include "G4CMPStackingAction.hh"

void QuSeN_QuantumAcousticActionInitialization::Build() const {
  SetUserAction(new QuSeN_QuantumAcousticPrimaryGeneratorAction);
  SetUserAction(new G4CMPStackingAction);
  SetUserAction(new QuSeN_QuantumAcousticSteppingAction);
}
