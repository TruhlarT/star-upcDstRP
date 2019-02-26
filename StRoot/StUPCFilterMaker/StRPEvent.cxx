
//_____________________________________________________________________________
//    Class for RP data
//    Author: Tomas Truhlar
//
//    RP event
//_____________________________________________________________________________

//root headers
#include "TClonesArray.h"
#include "TIterator.h"
#include "TParticle.h"

//local headers
#include "StUPCRpsCollection.h"
#include "StUPCRpsRomanPot.h"
#include "StUPCRpsPlane.h"
#include "StUPCRpsCluster.h"
#include "StUPCRpsTrack.h"
#include "StUPCRpsTrackPoint.h"
#include "StRPEvent.h"

ClassImp(StRPEvent);



TClonesArray *StRPEvent::mgCollections = 0;
TClonesArray *StRPEvent::mgTrackPoints = 0;
TClonesArray *StRPEvent::mgTracks = 0;



//_____________________________________________________________________________
StRPEvent::StRPEvent():
	mTracks(0x0), mNTracks(0),
   mTrackPoints(0x0), mNTrackPoints(0),
   mCollections(0x0), mNCollections(0)
{
  //default constructor

  mTrgIDs.Set(0);

  if(!mgCollections) {
    mgCollections = new TClonesArray("StUPCRpsCollection");
    mCollections = mgCollections;
    mCollections->SetOwner(kTRUE);
  }

  if(!mgTrackPoints) {
    mgTrackPoints = new TClonesArray("StUPCRpsTrackPoint");
    mTrackPoints = mgTrackPoints;
    mTrackPoints->SetOwner(kTRUE);
  }

  if(!mgTracks) {
    mgTracks = new TClonesArray("StUPCRpsTrack");
    mTracks = mgTracks;
    mTracks->SetOwner(kTRUE);
  }

}//StRPEvent

//_____________________________________________________________________________
StRPEvent::~StRPEvent()
{
  //destructor

  if(mTracks) {delete mTracks; mTracks = 0x0;}
  if(mTrackPoints) {delete mTrackPoints; mTrackPoints = 0x0;}
  if(mCollections) {delete mCollections; mCollections = 0x0;}


}//~StRPEvent

//_____________________________________________________________________________
void StRPEvent::clearEvent()
{
  // clear event variables

  mTrgIDs.Set(0);
  mCollections->Clear("C"); 
  mNCollections = 0;
  mTrackPoints->Clear("C"); 
  mNTrackPoints = 0;
  mTracks->Clear("C"); 
  mNTracks = 0;

}//clearEvent

//_____________________________________________________________________________
void StRPEvent::addTriggerId(Int_t id) {
  //add fired trigger ID

  //position to put the ID
  Int_t pos = mTrgIDs.GetSize();

  //extend the array
  mTrgIDs.Set(pos+1);

  //put the ID
  mTrgIDs.AddAt(id, pos);

}//addTriggerId


//_____________________________________________________________________________
StUPCRpsTrack *StRPEvent::addTrack()
{
  // construct new upc track

  return dynamic_cast<StUPCRpsTrack*>( mTracks->ConstructedAt(mNTracks++) );

}//addTrack


//_____________________________________________________________________________
StUPCRpsTrackPoint *StRPEvent::addTrackPoint()
{
  // construct new upc track

  return dynamic_cast<StUPCRpsTrackPoint*>( mTrackPoints->ConstructedAt(mNTrackPoints++) );

}//addTrackPoint

//_____________________________________________________________________________
StUPCRpsCollection *StRPEvent::addCollection()
{
  // construct new upc track

  return dynamic_cast<StUPCRpsCollection*>( mCollections->ConstructedAt(mNCollections++) );

}//addCollection

//_____________________________________________________________________________
Bool_t StRPEvent::isTrigger(Int_t id) const
{
  //get fired trigger ID
  for(Int_t i=0; i<mTrgIDs.GetSize(); i++) {
    if( mTrgIDs.At(i) == id ) return kTRUE;
  }

  return kFALSE;

}//setTrigger

//_____________________________________________________________________________
Int_t StRPEvent::getNumberOfTracks() const {

  //number of tracks in event

  if( !mTracks ) return 0;

  return mTracks->GetEntriesFast();

}//getNumberOfTracks

//_____________________________________________________________________________
StUPCRpsTrack *StRPEvent::getTrack(Int_t iTrack) const
{
  // get RP track

  return dynamic_cast<StUPCRpsTrack*>( mTracks->At(iTrack) );

}//getTrack

//_____________________________________________________________________________
Int_t StRPEvent::getNumberOfTrackPoints() const {

  //number of trackPoints in event

  if( !mTrackPoints ) return 0;

  return mTrackPoints->GetEntriesFast();

}//getNumberOfTrackPoints

//_____________________________________________________________________________
StUPCRpsTrackPoint *StRPEvent::getTrackPoint(Int_t iTrackPoint) const
{
  // get RP trackPoint

  return dynamic_cast<StUPCRpsTrackPoint*>( mTrackPoints->At(iTrackPoint) );

}//getTrackPoint


//_____________________________________________________________________________
Int_t StRPEvent::getNumberOfCollections() const {

  //number of mCollections in event

  if( !mCollections ) return 0;

  return mCollections->GetEntriesFast();

}//getNumberOfCollections

//_____________________________________________________________________________
StUPCRpsCollection *StRPEvent::getCollection(Int_t iCollection) const
{
  // get RP Collection

  return dynamic_cast<StUPCRpsCollection*>( mCollections->At(iCollection) );

}//getCollection














