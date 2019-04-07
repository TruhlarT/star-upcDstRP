#ifndef StRPEvent_h
#define StRPEvent_h

//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________

class StUPCRpsCluster;
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
  StUPCRpsCluster *addCluster();
  StUPCRpsTrack *addTrack();
  StUPCRpsTrackPoint *addTrackPoint();


  //getters
  Int_t getNumberOfClusters() const;
  StUPCRpsCluster *getCluster(Int_t iCluster) const;  
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
  if( romanPotID > mNumberOfRomanPots -1) return;
  mStatusRomanPot[romanPotID] = val;
}
void setNumberPlanes(UInt_t romanPotID, UInt_t val) 
{
  if( romanPotID > mNumberOfRomanPots -1) return;
  mNumberPlanes[romanPotID] = val;
}
void setNumberPlanesWithcluster(UInt_t romanPotID, UInt_t val) 
{
  if( romanPotID > mNumberOfRomanPots -1) return;
  mNumberPlanesWithcluster[romanPotID] = val;
}
void setAdc(UInt_t romanPotID, UInt_t adc0, UInt_t adc1) 
{
  if( romanPotID > mNumberOfRomanPots -1) return;

  mADC[romanPotID][0] = adc0;
  mADC[romanPotID][1] = adc1;
}
void setTac(UInt_t romanPotID, UInt_t tac0, UInt_t tac1)
{ 
  if( romanPotID > mNumberOfRomanPots -1) return;
  mTAC[romanPotID][0] = tac0;
  mTAC[romanPotID][1] = tac1;
}   
void setOffset(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID > mNumberOfRomanPots -1) return;
  if( planeID > mNumberOfPlanesInRP -1) return;

  mOffsetPlane[romanPotID][planeID] = val;
}
void setZ(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID > mNumberOfRomanPots -1) return;
  if( planeID > mNumberOfPlanesInRP -1) return;
  mzPlane[romanPotID][planeID] = val;
}
void setAngle(UInt_t romanPotID, UInt_t planeID, Float_t val) 
{
  if( romanPotID > mNumberOfRomanPots -1) return;
  if( planeID > mNumberOfPlanesInRP -1) return;
  mAnglePlane[romanPotID][planeID] = val;
}
void setOrientation(UInt_t romanPotID, UInt_t planeID, Short_t val) 
{
  if( romanPotID > mNumberOfRomanPots -1) return;
  if( planeID > mNumberOfPlanesInRP -1) return;
  mOrientationPlane[romanPotID][planeID] = val;
}
void setStatus(UInt_t romanPotID, UInt_t planeID, UChar_t val) 
{
  if( romanPotID > mNumberOfRomanPots -1) return;
  if( planeID > mNumberOfPlanesInRP -1) return;
  mStatusPlane[romanPotID][planeID] = val;
}

// Getters, romanPotID == RomanPot ID, planeID == Plane Id
UChar_t siliconBunch() const 
{ 
  return mSiliconBunch;
}
UInt_t numberOfPlanes(UInt_t romanPotID) const 
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  return mNumberPlanes[romanPotID];
}
UInt_t numberOfPlanesWithClusters(UInt_t romanPotID) const 
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  return mNumberPlanesWithcluster[romanPotID];
}
UChar_t status(UInt_t romanPotID) const 
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  return mStatusRomanPot[romanPotID];
}
UInt_t adc(UInt_t romanPotID, UInt_t pmtID) const // return adc value from first or second pmt
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  if( pmtID > mNumberOfPmtsInRp -1) return 0;
  return mADC[romanPotID][pmtID];
}
UInt_t tac(UInt_t romanPotID, UInt_t pmtID) const  // return tac value from first or second pmt
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  if( pmtID > mNumberOfPmtsInRp -1) return 0;
  return mTAC[romanPotID][pmtID];
}
Float_t offset(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID > mNumberOfPlanesInRP -1) return 0;
  return mOffsetPlane[romanPotID][planeID];
}
Float_t z(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID > mNumberOfPlanesInRP -1) return 0;
  return mzPlane[romanPotID][planeID];
}
Float_t angle(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID > mNumberOfPlanesInRP -1) return 0;
  return mAnglePlane[romanPotID][planeID];
}
Short_t  orientation(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID > mNumberOfPlanesInRP -1) return 0;
  return mOrientationPlane[romanPotID][planeID];
}

UChar_t status(UInt_t romanPotID, UInt_t planeID) const 
{ 
  if( romanPotID > mNumberOfRomanPots -1) return 0;
  if( planeID > mNumberOfPlanesInRP -1) return 0;
  return mStatusPlane[romanPotID][planeID];
}


private:

  StRPEvent (const StRPEvent &o); // not implemented
  StRPEvent &operator=(const StRPEvent &o); // not implemented

  static const Int_t mNumberOfRomanPots = 8;
  static const Int_t mNumberOfPlanesInRP = 4;
  static const Int_t mNumberOfPmtsInRp = 2;

  
  Float_t  mOffsetPlane[mNumberOfRomanPots][mNumberOfPlanesInRP]; //-0.1 to 0.1
  Float_t  mzPlane[mNumberOfRomanPots][mNumberOfPlanesInRP]; // -20 to 20
  Float_t  mAnglePlane[mNumberOfRomanPots][mNumberOfPlanesInRP]; // 0

  UInt_t  mNumberPlanes[mNumberOfRomanPots]; // = 4 Can I move it out?
  UInt_t  mNumberPlanesWithcluster[mNumberOfRomanPots]; // 0-800  co to znamená
  UInt_t  mADC[mNumberOfRomanPots][mNumberOfPmtsInRp]; // ADC in 2 pmt
  UInt_t  mTAC[mNumberOfRomanPots][mNumberOfPmtsInRp]; // TAC in 2 pmt

  Short_t  mOrientationPlane[mNumberOfRomanPots][mNumberOfPlanesInRP]; // -1 to 2
  
  UChar_t mSiliconBunch; // values: cca. -50 to 200 // should be 0 to 120
  UChar_t  mStatusRomanPot[mNumberOfRomanPots]; // from -1 to 155
  UChar_t  mStatusPlane[mNumberOfRomanPots][mNumberOfPlanesInRP]; // 0


  static TClonesArray *mgClusters; // array of upc tracks
  TClonesArray *mClusters; //-> array of upc tracks
  Int_t mNClusters; //! number of upc tracks in event, local use when filling

  static TClonesArray *mgTracks; // array of upc tracks
  TClonesArray *mTracks; //-> array of upc tracks
  Int_t mNTracks; //! number of upc tracks in event, local use when filling

  static TClonesArray *mgTrackPoints; // array of upc tracks
  TClonesArray *mTrackPoints; //-> array of upc tracks
  Int_t mNTrackPoints; //! number of upc tracks in event, local use when filling

  ClassDef(StRPEvent, 2);
};

#endif













