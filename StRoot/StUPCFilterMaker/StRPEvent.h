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
  Int_t getNumberOfTracks() const;
  StUPCRpsTrack *getTrack(Int_t iTrack) const;
  Int_t getNumberOfTrackPoints() const;
  StUPCRpsTrackPoint *getTrackPoint(Int_t iTrackPoint) const;

// Setters, romanPotID == RomanPot ID, planeID == Plane Id, val == value of current setter
void setSiliconBunch(UChar_t val) 
{
  mSiliconBunch = val;
}
void setStatus(UInt_t romanPotID, UChar_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  mStatusRomanPot[romanPotID] = val;
}
void setNumberPlanes(UInt_t romanPotID, UInt_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  mNumberPlanes[romanPotID] = val;
}
void setNumberPlanesWithluster(UInt_t romanPotID, UInt_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  mNumberPlanesWithluster[romanPotID] = val;
}
void setAdc(UInt_t romanPotID, UInt_t adc0, UInt_t adc1) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;

  mADC[romanPotID][0] = adc0;
  mADC[romanPotID][1] = adc1;
}
void setTac(UInt_t romanPotID, UInt_t tac0, UInt_t tac1)
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  mTAC[romanPotID][0] = tac0;
  mTAC[romanPotID][1] = tac1;
}   
void setOffset(UInt_t romanPotID, UInt_t planeID, Double_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;

  mOffsetPlane[romanPotID][planeID] = val;
}
void setZ(UInt_t romanPotID, UInt_t planeID, Double_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mzPlane[romanPotID][planeID] = val;
}
void setAngle(UInt_t romanPotID, UInt_t planeID, Double_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mAnglePlane[romanPotID][planeID] = val;
}
void setOrientation(UInt_t romanPotID, UInt_t planeID, Short_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mOrientationPlane[romanPotID][planeID] = val;
}
void setStatus(UInt_t romanPotID, UInt_t planeID, UChar_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mStatusPlane[romanPotID][planeID] = val;
}
void setNumberOfClusters(UInt_t romanPotID, UInt_t planeID, UInt_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mNumberOfClusters[romanPotID][planeID] = val;
}
void setPosition(UInt_t romanPotID, UInt_t planeID, Double_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mPositionCluster[romanPotID][planeID].push_back(val);
}
void setPositionRMS(UInt_t romanPotID, UInt_t planeID, Double_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mPositionRMSCluster[romanPotID][planeID].push_back(val);
}
void setLength(UInt_t romanPotID, UInt_t planeID, Short_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mLengthCluster[romanPotID][planeID].push_back(val);
}
void setEnergy(UInt_t romanPotID, UInt_t planeID, Double_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mEnergyCluster[romanPotID][planeID].push_back(val);
}
void setXY(UInt_t romanPotID, UInt_t planeID, Double_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mXYCluster[romanPotID][planeID].push_back(val);
}
void setQuality(UInt_t romanPotID, UInt_t planeID, UChar_t val) 
{
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return;
  mQualityCluster[romanPotID][planeID].push_back(val);
}

// Getters, romanPotID == RomanPot ID, planeID == Plane Id
UChar_t siliconBunch() const 
{ 
  return mSiliconBunch;
}
UInt_t numberOfPlanes(UInt_t romanPotID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  return mNumberPlanes[romanPotID];
}
UInt_t numberOfPlanesWithClusters(UInt_t romanPotID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  return mNumberPlanesWithluster[romanPotID];
}
UChar_t status(UInt_t romanPotID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  return mStatusRomanPot[romanPotID];
}
UInt_t adc(UInt_t romanPotID, UInt_t pmtID) const // return adc value from first or second pmt
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( pmtID < 0 || pmtID > mNumberOfPmtsInRp -1) return 0;
  return mADC[romanPotID][pmtID];
}
UInt_t tac(UInt_t romanPotID, UInt_t pmtID) const  // return tac value from first or second pmt
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( pmtID < 0 || pmtID > mNumberOfPmtsInRp -1) return 0;
  return mTAC[romanPotID][pmtID];
}
Double_t offset(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  return mOffsetPlane[romanPotID][planeID];
}
Double_t z(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  return mzPlane[romanPotID][planeID];
}
Double_t angle(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  return mAnglePlane[romanPotID][planeID];
}
Short_t  orientation(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  return mOrientationPlane[romanPotID][planeID];
}

UChar_t status(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  return mStatusPlane[romanPotID][planeID];
}
UInt_t  numberOfClusters(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  return mNumberOfClusters[romanPotID][planeID];
}
Double_t position(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  if( cluster < 0 || cluster > mNumberOfClusters[romanPotID][planeID] -1) return 0;
  return mPositionCluster[romanPotID][planeID][cluster];
}
Double_t positionRMS(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  if( cluster < 0 || cluster > mNumberOfClusters[romanPotID][planeID] -1) return 0;
  return mPositionRMSCluster[romanPotID][planeID][cluster];
}
Short_t  length(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  if( cluster < 0 || cluster > mNumberOfClusters[romanPotID][planeID] -1) return 0;
  return mLengthCluster[romanPotID][planeID][cluster];
}
Double_t energy(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  if( cluster < 0 || cluster > mNumberOfClusters[romanPotID][planeID] -1) return 0;
  return mEnergyCluster[romanPotID][planeID][cluster];
}
Double_t xy(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  if( cluster < 0 || cluster > mNumberOfClusters[romanPotID][planeID] -1) return 0;
  return mXYCluster[romanPotID][planeID][cluster];
}
UChar_t quality(UInt_t romanPotID, UInt_t planeID, UInt_t cluster) const 
{ 
  if( romanPotID < 0 || romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID < 0 || planeID > mNumberOfPlanesInRP -1) return 0;
  if( cluster < 0 || cluster > mNumberOfClusters[romanPotID][planeID] -1) return 0;
  return mQualityCluster[romanPotID][planeID][cluster];
}

private:

  StRPEvent (const StRPEvent &o); 
  StRPEvent &operator=(const StRPEvent &o); 

  static const Int_t mNumberOfRomanPots = 8;
  static const Int_t mNumberOfPlanesInRP = 4;
  static const Int_t mNumberOfPmtsInRp = 2;

  UChar_t mSiliconBunch;
  UInt_t  mNumberPlanes[mNumberOfRomanPots]; 
  UInt_t  mNumberPlanesWithluster[mNumberOfRomanPots]; 
  UChar_t  mStatusRomanPot[mNumberOfRomanPots];
  UInt_t  mADC[mNumberOfRomanPots][mNumberOfPmtsInRp]; // ADC in 2 pmt
  UInt_t  mTAC[mNumberOfRomanPots][mNumberOfPmtsInRp]; // TAC in 2 pmt

  Double_t  mOffsetPlane[mNumberOfRomanPots][mNumberOfPlanesInRP];
  Double_t  mzPlane[mNumberOfRomanPots][mNumberOfPlanesInRP];
  Double_t  mAnglePlane[mNumberOfRomanPots][mNumberOfPlanesInRP];
  Short_t  mOrientationPlane[mNumberOfRomanPots][mNumberOfPlanesInRP];
  UChar_t  mStatusPlane[mNumberOfRomanPots][mNumberOfPlanesInRP];  

  UInt_t  mNumberOfClusters[mNumberOfRomanPots][mNumberOfPlanesInRP]; 
  vector<Double_t>  mPositionCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<Double_t>  mPositionRMSCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<Short_t>  mLengthCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<Double_t>  mEnergyCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<Double_t>  mXYCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];
  vector<UChar_t>  mQualityCluster[mNumberOfRomanPots][mNumberOfPlanesInRP];

  static TClonesArray *mgTracks; // array of upc tracks
  TClonesArray *mTracks; //-> array of upc tracks
  Int_t mNTracks; //! number of upc tracks in event, local use when filling

  static TClonesArray *mgTrackPoints; // array of upc tracks
  TClonesArray *mTrackPoints; //-> array of upc tracks
  Int_t mNTrackPoints; //! number of upc tracks in event, local use when filling

  ClassDef(StRPEvent, 2);
};

#endif













