
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

  mTrackPoints->Clear("C"); 
  mNTrackPoints = 0;
  mTracks->Clear("C"); 
  mNTracks = 0;

}//clearEvent


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
void StRPEvent::setSiliconBunch(UChar_t val) 
{
  mSiliconBunch = val;
}
void StRPEvent::setStatus(UInt_t i, UChar_t val) 
{
  if( i < 0 || i > 7) return;
  mStatusRomanPot[i] = val;
}
void StRPEvent::setNumberPlanes(UInt_t i, UInt_t val) 
{
  if( i < 0 || i > 7) return;
  mNumberPlanes[i] = val;
}
void StRPEvent::setNumberPlanesWithluster(UInt_t i, UInt_t val) 
{
  if( i < 0 || i > 7) return;
  mNumberPlanesWithluster[i] = val;
}
void StRPEvent::setAdc(UInt_t i, UInt_t adc0, UInt_t adc1) 
{
  if( i < 0 || i > 7) return;

  mADC[i][0] = adc0;
  mADC[i][1] = adc1;
}
void StRPEvent::setTac(UInt_t i, UInt_t tac0, UInt_t tac1)
{ 
  if( i < 0 || i > 7) return;
  mTAC[i][0] = tac0;
  mTAC[i][1] = tac1;
}   
void StRPEvent::setOffset(UInt_t i, UInt_t j, Double_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;

  mOffsetPlane[i][j] = val;
}
void StRPEvent::setZ(UInt_t i, UInt_t j, Double_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mzPlane[i][j] = val;
}
void StRPEvent::setAngle(UInt_t i, UInt_t j, Double_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mAnglePlane[i][j] = val;
}
void StRPEvent::setOrientation(UInt_t i, UInt_t j, Short_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mOrientationPlane[i][j] = val;
}
void StRPEvent::setStatus(UInt_t i, UInt_t j, UChar_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mStatusPlane[i][j] = val;
}
void StRPEvent::setNumberOfClusters(UInt_t i, UInt_t j, UInt_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mNumberOfClusters[i][j] = val;
}
void StRPEvent::setPosition(UInt_t i, UInt_t j, Double_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mPositionCluster[i][j] = val;
}
void StRPEvent::setPositionRMS(UInt_t i, UInt_t j, Double_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mPositionRMSCluster[i][j] = val;
}
void StRPEvent::setLength(UInt_t i, UInt_t j, Short_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mLengthCluster[i][j] = val;
}
void StRPEvent::setEnergy(UInt_t i, UInt_t j, Double_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mEnergyCluster[i][j] = val;
}
void StRPEvent::setXY(UInt_t i, UInt_t j, Double_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mXYCluster[i][j] = val;
}
void StRPEvent::setQuality(UInt_t i, UInt_t j, UChar_t val) 
{
  if( i < 0 || i > 7) return;
  if( j < 0 || j > 3) return;
  mQualityCluster[i][j] = val;
}

// Getters
UChar_t StRPEvent::siliconBunch() const 
{ 
  return mSiliconBunch;
}
UInt_t StRPEvent::numberOfPlanes(UInt_t i) const 
{ 
  if( i < 0 || i > 7) return 0;
  return mNumberPlanes[i];
}
UInt_t StRPEvent::numberOfPlanesWithClusters(UInt_t i) const 
{ 
  if( i < 0 || i > 7) return 0;
  return mNumberPlanesWithluster[i];
}
UChar_t StRPEvent::status(UInt_t i) const 
{ 
  if( i < 0 || i > 7) return 0;
  return mStatusRomanPot[i];
}
UInt_t StRPEvent::adc(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mADC[i][j];
}
UInt_t StRPEvent::tac(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mTAC[i][j];
}
Double_t StRPEvent::offset(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mOffsetPlane[i][j];
}
Double_t StRPEvent::StRPEvent::z(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mzPlane[i][j];
}
Double_t StRPEvent::angle(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mAnglePlane[i][j];
}
Short_t  StRPEvent::orientation(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mOrientationPlane[i][j];
}

UChar_t StRPEvent::status(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mStatusPlane[i][j];
}
UInt_t  StRPEvent::numberOfClusters(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mNumberOfClusters[i][j];
}
Double_t StRPEvent::position(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mPositionCluster[i][j];
}
Double_t StRPEvent::positionRMS(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mPositionRMSCluster[i][j];
}
Short_t  StRPEvent::length(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mLengthCluster[i][j];
}
Double_t StRPEvent::energy(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mEnergyCluster[i][j];
}
Double_t StRPEvent::xy(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mXYCluster[i][j];
}
UChar_t StRPEvent::quality(UInt_t i, UInt_t j) const 
{ 
  if( i < 0 || i > 7) return 0;
  if( j < 0 || j > 3) return 0;
  return mQualityCluster[i][j];
}
