#ifndef StRPEvent_h
#define StRPEvent_h

//_____________________________________________________________________________
//    Class for UPC data
//    Author: Truhlar Tomas
//_____________________________________________________________________________


class StUPCRpsCollection; 
class StUPCRpsRomanPot; 
class StUPCRpsPlane; 
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
  StUPCRpsTrack *addTrack();
  StUPCRpsTrackPoint *addTrackPoint();
  void setSiliconBunch(UChar_t);
  void setStatus(UInt_t, UChar_t);
  void setNumberPlanes(UInt_t, UInt_t);
  void setNumberPlanesWithluster(UInt_t, UInt_t);
  void setAdc(UInt_t, UInt_t, UInt_t);
  void setTac(UInt_t, UInt_t, UInt_t);    
  void setOffset(UInt_t, UInt_t, Double_t);
  void setZ(UInt_t, UInt_t, Double_t);
  void setAngle(UInt_t, UInt_t, Double_t);
  void setOrientation(UInt_t, UInt_t, Short_t);
  void setStatus(UInt_t, UInt_t, UChar_t);
  void setNumberOfClusters(UInt_t, UInt_t, UInt_t);
  void setPosition(UInt_t, UInt_t, Double_t);
  void setPositionRMS(UInt_t, UInt_t, Double_t);
  void setLength(UInt_t, UInt_t, Short_t);
  void setEnergy(UInt_t, UInt_t, Double_t);
  void setXY(UInt_t, UInt_t, Double_t);
  void setQuality(UInt_t, UInt_t, UChar_t);

  //getters

  Int_t getNumberOfTracks() const;
  StUPCRpsTrack *getTrack(Int_t iTrack) const;
  Int_t getNumberOfTrackPoints() const;
  StUPCRpsTrackPoint *getTrackPoint(Int_t iTrackPoint) const;

  UChar_t siliconBunch() const;
  UInt_t numberOfPlanes(UInt_t) const;
  UInt_t numberOfPlanesWithClusters(UInt_t) const;
  UChar_t status(UInt_t) const;
  UInt_t adc(UInt_t, UInt_t) const;
  UInt_t tac(UInt_t, UInt_t) const;
  Double_t offset(UInt_t, UInt_t) const;
  Double_t z(UInt_t, UInt_t) const;
  Double_t angle(UInt_t, UInt_t) const;
  Short_t  orientation(UInt_t, UInt_t) const;
  UChar_t status(UInt_t, UInt_t) const;
  UInt_t  numberOfClusters(UInt_t, UInt_t) const;
  Double_t position(UInt_t, UInt_t) const;
  Double_t positionRMS(UInt_t, UInt_t) const;
  Short_t  length(UInt_t, UInt_t) const;
  Double_t energy(UInt_t, UInt_t) const;
  Double_t xy(UInt_t, UInt_t) const;
  UChar_t quality(UInt_t, UInt_t) const;

private:

  StRPEvent (const StRPEvent &o); 
  StRPEvent &operator=(const StRPEvent &o); 

  UChar_t mSiliconBunch;
  UInt_t  mNumberPlanes[8]; 
  UInt_t  mNumberPlanesWithluster[8]; 
  UChar_t  mStatusRomanPot[8];
  UInt_t  mADC[8][2];
  UInt_t  mTAC[8][2];
  Double_t  mOffsetPlane[8][4];
  Double_t  mzPlane[8][4];
  Double_t  mAnglePlane[8][4];
  Short_t  mOrientationPlane[8][4];
  UChar_t  mStatusPlane[8][4];   
  UInt_t  mNumberOfClusters[8][4]; 
  Double_t  mPositionCluster[8][4];
  Double_t  mPositionRMSCluster[8][4];
  Short_t  mLengthCluster[8][4];
  Double_t  mEnergyCluster[8][4];
  Double_t  mXYCluster[8][4];
  UChar_t  mQualityCluster[8][4];

  static TClonesArray *mgTracks; // array of upc tracks
  TClonesArray *mTracks; //-> array of upc tracks
  Int_t mNTracks; //! number of upc tracks in event, local use when filling

  static TClonesArray *mgTrackPoints; // array of upc tracks
  TClonesArray *mTrackPoints; //-> array of upc tracks
  Int_t mNTrackPoints; //! number of upc tracks in event, local use when filling

  ClassDef(StRPEvent, 2);
};

#endif













