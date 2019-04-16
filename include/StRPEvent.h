#ifndef StRPEvent_h
#define StRPEvent_h

//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________

class StUPCRpsTrack;
class StUPCRpsTrackPoint;

class TClonesArray;
class TIterator;
class TParticle;

#include "TArrayI.h"
#include "TObject.h"
#include <vector>

using namespace std;

class StRPEvent
{

public:

  StRPEvent();
  virtual ~StRPEvent(); 

  void clearEvent(); 

  //setters
  StUPCRpsTrack *addTrack();
  StUPCRpsTrackPoint *addTrackPoint();


  //getters
  UShort_t getNumberOfTracks() const;
  StUPCRpsTrack *getTrack(Int_t iTrack) const;
  UShort_t getNumberOfTrackPoints() const;
  StUPCRpsTrackPoint *getTrackPoint(Int_t iTrackPoint) const;

// Setters, romanPotID == RomanPot ID, planeID == Plane Id, val == value of current setter
void setSiliconBunch(UChar_t val) 
{
  mSiliconBunch = val;
}
void setStatus(UInt_t romanPotID, UChar_t val) 
{
  if( romanPotID >= mNumberOfRomanPots) return;
  mStatusRomanPot[romanPotID] = val;
}
void setNumberPlanes(UInt_t romanPotID, UShort_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  mNumberPlanes[romanPotID] = val;
}

void setNumberPlanesWithCluster(UInt_t romanPotID, UShort_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  mNumberPlanesWithCluster[romanPotID] = val;
}

void setAdc(UInt_t romanPotID, UInt_t adc0, UShort_t adc1) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;

  mADC[romanPotID][0] = adc0;
  mADC[romanPotID][1] = adc1;
}
void setTac(UInt_t romanPotID, UInt_t tac0, UShort_t tac1)
{ 
  if( romanPotID >= mNumberOfRomanPots ) return;
  mTAC[romanPotID][0] = tac0;
  mTAC[romanPotID][1] = tac1;
}   
void setOffset(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;

  mOffsetPlane[romanPotID][planeID] = val;
}
void setZ(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mzPlane[romanPotID][planeID] = val;
}
void setAngle(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mAnglePlane[romanPotID][planeID] = val;
}
void setOrientation(UInt_t romanPotID, UInt_t planeID, Short_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mOrientationPlane[romanPotID][planeID] = val;
}
void setStatus(UInt_t romanPotID, UInt_t planeID, UChar_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mStatusPlane[romanPotID][planeID] = val;
}

void setNumberOfClusters(UInt_t romanPotID, UInt_t planeID, UShort_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mNumberOfClusters[romanPotID][planeID] = val;
}
void setPosition(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mPositionCluster[romanPotID][planeID].push_back(val);
}
void setPositionRMS(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mPositionRMSCluster[romanPotID][planeID].push_back(val);
}
void setLength(UInt_t romanPotID, UInt_t planeID, Short_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mLengthCluster[romanPotID][planeID].push_back(val);
}
void setEnergy(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mEnergyCluster[romanPotID][planeID].push_back(val);
}
void setXY(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mXYCluster[romanPotID][planeID].push_back(val);
}
void setQuality(UInt_t romanPotID, UInt_t planeID, UChar_t val) 
{
  if( romanPotID >= mNumberOfRomanPots ) return;
  if( planeID >= mNumberOfPlanesInRP ) return;
  mQualityCluster[romanPotID][planeID].push_back(val);
}

// Getters, romanPotID == RomanPot ID, planeID == Plane Id
UChar_t siliconBunch() const 
{ 
  return mSiliconBunch;
}

UShort_t numberOfPlanes(UInt_t romanPotID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  return mNumberPlanes[romanPotID];
}

UShort_t numberOfPlanesWithClusters(UInt_t romanPotID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  return mNumberPlanesWithCluster[romanPotID];
}

UChar_t status(UInt_t romanPotID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  return mStatusRomanPot[romanPotID];
}
UShort_t adc(UInt_t romanPotID, UInt_t pmtID) const // return adc value from first or second pmt
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( pmtID >= mNumberOfPmtsInRp ) return 0;
  return mADC[romanPotID][pmtID];
}
UShort_t tac(UInt_t romanPotID, UInt_t pmtID) const  // return tac value from first or second pmt
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( pmtID >= mNumberOfPmtsInRp ) return 0;
  return mTAC[romanPotID][pmtID];
}
Float_t offset(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  return mOffsetPlane[romanPotID][planeID];
}
Float_t z(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  return mzPlane[romanPotID][planeID];
}
Float_t angle(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  return mAnglePlane[romanPotID][planeID];
}
Short_t  orientation(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  return mOrientationPlane[romanPotID][planeID];
}

UChar_t status(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  return mStatusPlane[romanPotID][planeID];
}

UInt_t  numberOfClusters(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  return mNumberOfClusters[romanPotID][planeID];
}
Float_t position(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  if( cluster >= mNumberOfClusters[romanPotID][planeID] ) return 0;
  return mPositionCluster[romanPotID][planeID][cluster];
}
Float_t positionRMS(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  if( cluster >= mNumberOfClusters[romanPotID][planeID] ) return 0;
  return mPositionRMSCluster[romanPotID][planeID][cluster];
}
Short_t  length(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  if( cluster >= mNumberOfClusters[romanPotID][planeID] ) return 0;
  return mLengthCluster[romanPotID][planeID][cluster];
}
Float_t energy(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  if( cluster >= mNumberOfClusters[romanPotID][planeID] ) return 0;
  return mEnergyCluster[romanPotID][planeID][cluster];
}
Float_t xy(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  if( cluster >= mNumberOfClusters[romanPotID][planeID] ) return 0;
  return mXYCluster[romanPotID][planeID][cluster];
}
UChar_t quality(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID >= mNumberOfRomanPots ) return 0;
  if( planeID >= mNumberOfPlanesInRP ) return 0;
  if( cluster >= mNumberOfClusters[romanPotID][planeID] ) return 0;
  return mQualityCluster[romanPotID][planeID][cluster];
}

private:

  StRPEvent (const StRPEvent &o); // not implemented
  StRPEvent &operator=(const StRPEvent &o); // not implemented

  static const Int_t mNumberOfRomanPots = 8;
  static const Int_t mNumberOfPlanesInRP = 4;
  static const Int_t mNumberOfPmtsInRp = 2;


  Float_t  mOffsetPlane[mNumberOfRomanPots][mNumberOfPlanesInRP];
  Float_t  mzPlane[mNumberOfRomanPots][mNumberOfPlanesInRP];
  Float_t  mAnglePlane[mNumberOfRomanPots][mNumberOfPlanesInRP];

  Short_t  mOrientationPlane[mNumberOfRomanPots][mNumberOfPlanesInRP];
  UShort_t mNumberPlanesWithCluster[mNumberOfRomanPots]; 
  UShort_t mADC[mNumberOfRomanPots][mNumberOfPmtsInRp]; // ADC in 2 pmt
  UShort_t mTAC[mNumberOfRomanPots][mNumberOfPmtsInRp]; // TAC in 2 pmt
  UShort_t mNumberPlanes[mNumberOfRomanPots]; 

  UChar_t  mStatusPlane[mNumberOfRomanPots][mNumberOfPlanesInRP];  
  UChar_t  mSiliconBunch;
  UChar_t  mStatusRomanPot[mNumberOfRomanPots];

  UShort_t  mNumberOfClusters[mNumberOfRomanPots][mNumberOfPlanesInRP]; 
  vector<Float_t>  mPositionCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<Float_t>  mPositionRMSCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<Short_t>  mLengthCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<Float_t>  mEnergyCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<Float_t>  mXYCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<UChar_t>  mQualityCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];

  static TClonesArray *mgTracks; // array of rp tracks
  TClonesArray *mTracks; //-> array of rp tracks
  UShort_t mNTracks; //! number of rp tracks in event, local use when filling

  static TClonesArray *mgTrackPoints; // array of rp trackPoints
  TClonesArray *mTrackPoints; //-> array of rp trackPoints
  UShort_t mNTrackPoints; //! number of rp trackPoints in event, local use when filling

  ClassDef(StRPEvent, 2);
};

#endif













