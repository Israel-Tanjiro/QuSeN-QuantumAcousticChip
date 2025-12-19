/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: e58a61fedbb99b167e16dafebc9c8664ae0c7b94 $

#ifndef QuSeN_QuantumAcousticActionInitialization_hh
#define QuSeN_QuantumAcousticActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

class QuSeN_QuantumAcousticActionInitialization : public G4VUserActionInitialization {
public:
  QuSeN_QuantumAcousticActionInitialization() {;}
  virtual ~QuSeN_QuantumAcousticActionInitialization() {;}
  virtual void Build() const;
};

#endif	/* QuSeN_QuantumAcousticActionInitialization_hh */
