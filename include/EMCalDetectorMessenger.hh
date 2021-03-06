///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
//                                                                               //
//  AUTHOR: Miguel Ramos Pernas                                                  //
//  e-mail: miguel.ramos.pernas@cern.ch                                          //
//                                                                               //
//  Last update: 29/10/2015                                                      //
//                                                                               //
// ----------------------------------------------------------------------------- //
//                                                                               //
//  Description:                                                                 //
//                                                                               //
//  This is the messenger for the EMCalDetectorConstruction class. All commands  //
//  are available in the states PreInit and Idle.                                //
//                                                                               //
// ----------------------------------------------------------------------------- //
///////////////////////////////////////////////////////////////////////////////////


#ifndef EMCalDetectorMessenger_h
#define EMCalDetectorMessenger_h 1

#include "EMCalDetectorConstruction.hh"

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "globals.hh"


//_______________________________________________________________________________

class EMCalDetectorMessenger: public G4UImessenger {

public:

  // Constructor
  EMCalDetectorMessenger( EMCalDetectorConstruction *detector );

  // Destructor
  ~EMCalDetectorMessenger();

  // Method
  void SetNewValue( G4UIcommand *command, G4String value );

protected:

  // Attributes
  EMCalDetectorConstruction *fDetector;
  G4UIdirectory             *fEMCalDir;
  G4UIdirectory             *fDetDir;
  G4UIcmdWithAString        *fDetectorColourCmd;
  G4UIcmdWithAString        *fDetectorMaterialCmd;
  G4UIcmdWithADoubleAndUnit *fDistanceCmd;
  G4UIcmdWithADoubleAndUnit *fModuleHalfLengthXcmd;
  G4UIcmdWithADoubleAndUnit *fModuleHalfLengthYcmd;
  G4UIcmdWithADoubleAndUnit *fModuleHalfLengthZcmd;
  G4UIcmdWithADouble        *fModuleProportionCmd;
  G4UIcmdWithAnInteger      *fNxModulesCmd;
  G4UIcmdWithAnInteger      *fNyModulesCmd;
  G4UIcmdWithAnInteger      *fNzModulesCmd;
  G4UIcmdWithoutParameter   *fPrintCmd;
  G4UIcmdWithABool          *fSGVolumeCmd;
  G4UIcmdWithAString        *fSGVolumeColourCmd;
  G4UIcmdWithAString        *fSGVolumeMaterialCmd;
  G4UIcmdWithoutParameter   *fUpdateCmd;
  G4UIcmdWithADoubleAndUnit *fWorldHalfLengthXcmd;
  G4UIcmdWithADoubleAndUnit *fWorldHalfLengthYcmd;
  G4UIcmdWithADoubleAndUnit *fWorldHalfLengthZcmd;
  G4UIcmdWithAString        *fWorldMaterialCmd;

};

#endif
