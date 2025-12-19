/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef QuSeN_QuantumAcousticSensitivity_h
#define QuSeN_QuantumAcousticSensitivity_h 1

#include "G4CMPElectrodeSensitivity.hh"

class QuSeN_QuantumAcousticSensitivity final : public G4CMPElectrodeSensitivity {
public:
  QuSeN_QuantumAcousticSensitivity(G4String name);
  virtual ~QuSeN_QuantumAcousticSensitivity();
  // No copies
  QuSeN_QuantumAcousticSensitivity(const QuSeN_QuantumAcousticSensitivity&) = delete;
  QuSeN_QuantumAcousticSensitivity& operator=(const QuSeN_QuantumAcousticSensitivity&) = delete;

  QuSeN_QuantumAcousticSensitivity(QuSeN_QuantumAcousticSensitivity&&) = delete;
  QuSeN_QuantumAcousticSensitivity& operator=(QuSeN_QuantumAcousticSensitivity&&) = delete;

  virtual void EndOfEvent(G4HCofThisEvent*);

  void SetOutputFile(const G4String& fn);

protected:
  virtual G4bool IsHit(const G4Step*, const G4TouchableHistory*) const;

private:
  std::ofstream output;
  G4String fileName;
};

#endif
