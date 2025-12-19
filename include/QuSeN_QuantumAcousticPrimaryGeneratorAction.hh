/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file exoticphysics/QuSeN_QuantumAcoustic/include/QuSeN_QuantumAcousticPrimaryGeneratorAction.hh
/// \brief Definition of the QuSeN_QuantumAcousticPrimaryGeneratorAction class
//
// $Id: ecbf57649dfaeb88e0fac25491bf8fb68c9308ec $
//

#ifndef QuSeN_QuantumAcousticPrimaryGeneratorAction_h
#define QuSeN_QuantumAcousticPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"


class G4ParticleGun;
class G4Event;

class QuSeN_QuantumAcousticPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  QuSeN_QuantumAcousticPrimaryGeneratorAction();
  virtual ~QuSeN_QuantumAcousticPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);

  private:
    G4GeneralParticleSource*                fParticleGun;

};


#endif
