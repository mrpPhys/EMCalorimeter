#include "G4ParticleGun.hh"

#include "EMCalDetectorConstruction.hh"
#include "EMCalPrimaryGeneratorAction.hh"
#include "EMCalRun.hh"

EMCalRun::EMCalRun() :
  G4Run(),
  fOutputTree( 0 ),
  fTitle( "DetectorEnergy/D:SGVolumeEnergy/D:LostEnergy/D" ),
  fDetectorEnergy( 0 ),
  fLostEnergy( 0 ),
  fNhits( 0 ),
  fSGVolumeEnergy( 0 ),
  fTrueEnergy( 0 ),
  fVariablesVector( 0 ) {

  const EMCalDetectorConstruction *detector
   = static_cast<const EMCalDetectorConstruction*>
     ( G4RunManager::GetRunManager() -> GetUserDetectorConstruction() );

  fNbranches       = detector -> GetNmodules();
  fVariablesVector = new EMCalRun::PhysicalVariables[ fNbranches ];
} 

EMCalRun::~EMCalRun() { delete[] fVariablesVector; }

EMCalRun::PhysicalVariables::PhysicalVariables() :
  DetectorEnergy( 0 ), SGVolumeEnergy( 0 ), LostEnergy( 0 ) { }

EMCalRun::PhysicalVariables::~PhysicalVariables() { }

void EMCalRun::AddEnergyToDetector( G4double edep,
				    G4int    idet ) {

  fVariablesVector[ idet ].DetectorEnergy += edep;
}

void EMCalRun::AddEnergyToSGVolume( G4double edep,
				    G4int    idet ) {

  fVariablesVector[ idet ].SGVolumeEnergy += edep;
}
 
void EMCalRun::Fill( const G4int &evtNb ) {

  // Obtains the pointer to the particle gun
  const EMCalPrimaryGeneratorAction* generatorAction
   = static_cast<const EMCalPrimaryGeneratorAction*>
     ( G4RunManager::GetRunManager() -> GetUserPrimaryGeneratorAction() );

  const G4ParticleGun* particleGun = generatorAction -> GetParticleGun();

  // Gets the energy of the incident particle
  fDetectorEnergy = 0;
  fLostEnergy     = 0;
  fNhits          = 0;
  fSGVolumeEnergy = 0;
  fTrueEnergy     = particleGun -> GetParticleEnergy();

  // Gets the energy deposited in each module
  G4double ModDetectorEnergy, ModSGVolumeEnergy;
  
  for ( size_t idet = 0; idet < fNbranches; idet++ ) {

    ModDetectorEnergy = fVariablesVector[ idet ].DetectorEnergy;
    ModSGVolumeEnergy = fVariablesVector[ idet ].SGVolumeEnergy;

    fVariablesVector[ idet ].LostEnergy
      = fTrueEnergy - ModDetectorEnergy - ModSGVolumeEnergy;

    fDetectorEnergy += ModDetectorEnergy;
    fSGVolumeEnergy += ModSGVolumeEnergy;

    if ( ModDetectorEnergy > 0. )
      fNhits++;
  }

  fLostEnergy = fTrueEnergy - fDetectorEnergy;

  fOutputTree -> Fill();

  if ( evtNb % 100000 == 0 ) {

    G4cout << "\n ******************************* "   << G4endl;
    G4cout <<   " **** Autosaving output tree *** "   << G4endl;
    G4cout <<   " ******************************* \n" << G4endl;
    fOutputTree -> AutoSave();
  }
}

void EMCalRun::Reset() {

    for ( size_t idet = 0; idet < fNbranches; idet++ ) {

      fVariablesVector[ idet ].DetectorEnergy  = 0;
      fVariablesVector[ idet ].SGVolumeEnergy  = 0;
      fVariablesVector[ idet ].LostEnergy      = 0;
    }
}
