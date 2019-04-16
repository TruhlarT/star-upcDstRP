//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________

//root headers
#include "TClonesArray.h"
#include "TIterator.h"
#include "TParticle.h"

//local headers
#include "StUPCRpsTrack.h"
#include "StUPCRpsTrackPoint.h"
#include "StRPEvent.h"

ClassImp(StRPEvent);


TClonesArray *StRPEvent::mgTrackPoints = 0;
TClonesArray *StRPEvent::mgTracks = 0;


//_____________________________________________________________________________
StRPEvent::StRPEvent():
   mSiliconBunch(0),
   mTracks(0x0), mNTracks(0),
   mTrackPoints(0x0), mNTrackPoints(0)
{
  //default constructor

  for (UInt_t iRP = 0; iRP < mNumberOfRomanPots; ++iRP){
    mNumberPlanes[iRP] = 0;
    mNumberPlanesWithCluster[iRP] = 0;
    mStatusRomanPot[iRP] = 0;
    for (UInt_t iPMT = 0; iPMT < mNumberOfPmtsInRp; ++iPMT){
      mADC[iRP][iPMT] = 0;
      mTAC[iRP][iPMT] = 0;
    }
    for (UInt_t iPlane = 0; iPlane < mNumberOfPlanesInRP; ++iPlane){
      mOffsetPlane[iRP][iPlane] = 0;
      mzPlane[iRP][iPlane] = 0;
      mAnglePlane[iRP][iPlane] = 0;
      mOrientationPlane[iRP][iPlane] = 0;
      mStatusPlane[iRP][iPlane] = 0; 
      mNumberOfClusters[iRP][iPlane] = 0;
    }
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

}//~StRPEvent

//_____________________________________________________________________________
void StRPEvent::clearEvent()
{
  // clear event variables

  mTrackPoints->Clear("C"); 
  mNTrackPoints = 0;
  mTracks->Clear("C"); 
  mNTracks = 0;


  for(UInt_t i = 0; i < mNumberOfRomanPots; ++i){ 
    for(UInt_t j = 0; j < mNumberOfPlanesInRP; ++j){
      mPositionCluster[i][j].clear();
      mPositionRMSCluster[i][j].clear();
      mLengthCluster[i][j].clear();
      mEnergyCluster[i][j].clear();
      mXYCluster[i][j].clear();
      mQualityCluster[i][j].clear();
    }
  }

}//clearEvent

//_____________________________________________________________________________
StUPCRpsTrack *StRPEvent::addTrack()
{
  // construct new RP track

  return dynamic_cast<StUPCRpsTrack*>( mTracks->ConstructedAt(mNTracks++) );

}//addTrack


//_____________________________________________________________________________
StUPCRpsTrackPoint *StRPEvent::addTrackPoint()
{
  // construct new RP trackPoint

  return dynamic_cast<StUPCRpsTrackPoint*>( mTrackPoints->ConstructedAt(mNTrackPoints++) );

}//addTrackPoint

//_____________________________________________________________________________
UShort_t StRPEvent::getNumberOfTracks() const {

  //number of RP tracks in event

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
UShort_t StRPEvent::getNumberOfTrackPoints() const {

  //number of RP trackPoints in event

  if( !mTrackPoints ) return 0;

  return mTrackPoints->GetEntriesFast();

}//getNumberOfTrackPoints

//_____________________________________________________________________________
StUPCRpsTrackPoint *StRPEvent::getTrackPoint(Int_t iTrackPoint) const
{
  // get RP trackPoint

  return dynamic_cast<StUPCRpsTrackPoint*>( mTrackPoints->At(iTrackPoint) );

}//getTrackPoint


