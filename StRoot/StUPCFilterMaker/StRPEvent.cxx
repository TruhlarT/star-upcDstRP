
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


TClonesArray *StRPEvent::mgTrackPoints = 0;
TClonesArray *StRPEvent::mgTracks = 0;




//_____________________________________________________________________________
StRPEvent::StRPEvent():
  mTracks(0x0), mNTracks(0),
   mTrackPoints(0x0), mNTrackPoints(0)
{
  //default constructor

  mTrgIDs.Set(0);


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


}//~StRPEvent

//_____________________________________________________________________________
void StRPEvent::clearEvent()
{
  // clear event variables

  mTrgIDs.Set(0);
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


// Setters, i == RomanPot ID, j == Plane Id, val == value of current setter
void StRPEvent::setSiliconBunch(UChar_t val) {mSiliconBunch = val;}
void StRPEvent::setStatus(UInt_t i, UChar_t val) {mStatusRomanPot[i] = val;}
void StRPEvent::setNumberPlanes(UInt_t i, UInt_t val) {mNumberPlanes[i] = val;}
void StRPEvent::setNumberPlanesWithluster(UInt_t i, UInt_t val) {mNumberPlanesWithluster[i] = val;}
void StRPEvent::setAdc(UInt_t i, UInt_t adc0, UInt_t adc1) 
{ 
 mADC[i][0] = adc0;
 mADC[i][1] = adc1;
}
void StRPEvent::setTac(UInt_t i, UInt_t tac0, UInt_t tac1)
{ 
 mTAC[i][0] = tac0;
 mTAC[i][1] = tac1;
}   
void StRPEvent::setOffset(UInt_t i, UInt_t j, Double_t val) {mOffsetPlane[i][j] = val;}
void StRPEvent::setZ(UInt_t i, UInt_t j, Double_t val) {mzPlane[i][j] = val;}
void StRPEvent::setAngle(UInt_t i, UInt_t j, Double_t val) {mAnglePlane[i][j] = val;}
void StRPEvent::setOrientation(UInt_t i, UInt_t j, Short_t val) {mOrientationPlane[i][j] = val;}
void StRPEvent::setStatus(UInt_t i, UInt_t j, UChar_t val) {mStatusPlane[i][j] = val;}
void StRPEvent::setNumberOfClusters(UInt_t i, UInt_t j, UInt_t val) {mNumberOfClusters[i][j] = val;}
void StRPEvent::setPosition(UInt_t i, UInt_t j, Double_t val) {mPositionCluster[i][j] = val;}
void StRPEvent::setPositionRMS(UInt_t i, UInt_t j, Double_t val) {mPositionRMSCluster[i][j] = val;}
void StRPEvent::setLength(UInt_t i, UInt_t j, Short_t val) {mLengthCluster[i][j] = val;}
void StRPEvent::setEnergy(UInt_t i, UInt_t j, Double_t val) {mEnergyCluster[i][j] = val;}
void StRPEvent::setXY(UInt_t i, UInt_t j, Double_t val) {mXYCluster[i][j] = val;}
void StRPEvent::setQuality(UInt_t i, UInt_t j, UChar_t val) {mQualityCluster[i][j] = val;}

// Getters
UChar_t StRPEvent::siliconBunch() const { return mSiliconBunch;}
UInt_t StRPEvent::numberOfPlanes(UInt_t i) const { return mNumberPlanes[i];}
UInt_t StRPEvent::numberOfPlanesWithClusters(UInt_t i) const { return mNumberPlanesWithluster[i];}
UChar_t StRPEvent::status(UInt_t i) const { return mStatusRomanPot[i];}
UInt_t StRPEvent::adc(UInt_t i, UInt_t j) const { return mADC[i][j];}
UInt_t StRPEvent::tac(UInt_t i, UInt_t j) const { return mTAC[i][j];}
Double_t StRPEvent::offset(UInt_t i, UInt_t j) const { return mOffsetPlane[i][j];}
Double_t StRPEvent::StRPEvent::z(UInt_t i, UInt_t j) const { return mzPlane[i][j];}
Double_t StRPEvent::angle(UInt_t i, UInt_t j) const { return mAnglePlane[i][j];}
Short_t  StRPEvent::orientation(UInt_t i, UInt_t j) const { return mOrientationPlane[i][j];}
UChar_t StRPEvent::status(UInt_t i, UInt_t j) const { return mStatusPlane[i][j];}
UInt_t  StRPEvent::numberOfClusters(UInt_t i, UInt_t j) const { return mNumberOfClusters[i][j];}
Double_t StRPEvent::position(UInt_t i, UInt_t j) const { return mPositionCluster[i][j];}
Double_t StRPEvent::positionRMS(UInt_t i, UInt_t j) const { return mPositionRMSCluster[i][j];}
Short_t  StRPEvent::length(UInt_t i, UInt_t j) const { return mLengthCluster[i][j];}
Double_t StRPEvent::energy(UInt_t i, UInt_t j) const { return mEnergyCluster[i][j];}
Double_t StRPEvent::xy(UInt_t i, UInt_t j) const { return mXYCluster[i][j];}
UChar_t StRPEvent::quality(UInt_t i, UInt_t j) const { return mQualityCluster[i][j];}
