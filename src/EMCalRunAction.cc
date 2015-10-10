#include "EMCalRunActionMessenger.hh"
#include "EMCalModule.hh"
#include "EMCalRunAction.hh"
#include "EMCalPrimaryGeneratorAction.hh"
#include "EMCalDetectorConstruction.hh"
#include "EMCalRun.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

EMCalRunAction::EMCalRunAction() :
  G4UserRunAction(),
  fOutputFile( 0 ),
  fOutputTree( 0 ),
  fTreeName( "DecayTree" ) {

  fMessenger = new EMCalRunActionMessenger( this );
}

EMCalRunAction::~EMCalRunAction() {

  delete fMessenger;

  if ( fOutputFile )
    fOutputFile -> Close();
}

G4Run* EMCalRunAction::GenerateRun() {

  // If the file does not exist it is created
  if ( !fOutputFile )
    this -> CreateNewFile( "EMCalorimeter_Results.root" );

  // The EMCalRun class is initiated
  fRun = new EMCalRun;

  return fRun;
}

void EMCalRunAction::BeginOfRunAction( const G4Run* ) { 

  // Creates a new tree
  fOutputFile -> cd();
  fOutputTree = new TTree( fTreeName.data(), fTreeName.data(), 0 );

  // Gives the path of the output tree to the EMCalRun class
  fRun -> SetOutputTree( fOutputTree );

  // Creates the structure of the output tree
  const EMCalDetectorConstruction *detector =
    static_cast<const EMCalDetectorConstruction*>
    ( G4RunManager::GetRunManager() -> GetUserDetectorConstruction() );

  std::vector<EMCalModule*> marray = detector -> GetModuleArray();

  // Sets the branch for the global detector ( situated on top of the tree )
  fOutputTree -> Branch( "Calorimeter", fRun -> GetPathTo( 0 ), fRun -> Title() );

  // Sets the branches for each of the modules
  for ( unsigned int idet = 0; idet < marray.size(); idet++ )
    fOutputTree -> Branch( ( marray[ idet ] -> GetID() ).data(),
			   fRun -> GetPathTo( idet + 1 ),
    			   fRun -> Title() );

  // Informs the runManager to save random number seed
  G4RunManager::GetRunManager() -> SetRandomNumberStore( false );
}

void EMCalRunAction::EndOfRunAction( const G4Run *run ) {

  G4int nofEvents = run -> GetNumberOfEvent();
  if ( nofEvents == 0 ) return;
  
  // Run conditions
  const EMCalPrimaryGeneratorAction* generatorAction
    = static_cast<const EMCalPrimaryGeneratorAction*>
    ( G4RunManager::GetRunManager() -> GetUserPrimaryGeneratorAction() );

  G4String runCondition;
  if ( generatorAction ) {

    const G4ParticleGun* particleGun    = generatorAction -> GetParticleGun();
    G4double             particleEnergy = particleGun -> GetParticleEnergy();

    runCondition += particleGun -> GetParticleDefinition() -> GetParticleName();
    runCondition += " of ";
    runCondition += G4BestUnit( particleEnergy, "Energy" );
  }
        
  // Print
  /*
    if ( IsMaster() ) {
    G4cout
    << G4endl
    << "--------------------End of Global Run-----------------------";
    }
    else {
    G4cout
    << G4endl
    << "--------------------End of Local Run------------------------";
    }
  
    G4cout
    << G4endl
    << " The run consists of " << nofEvents << " "<< runCondition
    << G4endl
    << " Energy in scoring volume : " 
    << G4BestUnit( edep, "Energy" )
    << G4endl
    << "------------------------------------------------------------"
    << G4endl
    << G4endl;
  */
  fOutputTree -> AutoSave();
}