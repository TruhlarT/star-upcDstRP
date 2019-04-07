#ifndef StUPCRpsCluster_hh
#define StUPCRpsCluster_hh

//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________


#include <iostream> 
#include "TObject.h" 


class StUPCRpsCluster{
public:
    StUPCRpsCluster();
    StUPCRpsCluster(Float_t pos, Float_t posRMS, Short_t len,
                 Float_t e, Float_t xy, UChar_t qual);
    ~StUPCRpsCluster() { /* noop */ };

double position() const { return mPosition; }

double positionRMS() const { return mPositionRMS; }

short  length() const { return mLength; }

double energy() const { return mEnergy; }

double xy() const { return mXY; }

unsigned char quality() const { return mQuality; }

unsigned int romanPotId() const { return mRomanPotId; }

unsigned int planeId() const { return mPlaneId; }

void setPosition(double val) { mPosition = val; }

void setPositionRMS(double val) { mPositionRMS = val; }

void setLength(short val) { mLength = val; }

void setEnergy(double val) { mEnergy = val; }

void setXY(double val) { mXY = val; }

void setQuality(unsigned char val) { mQuality = val; }

void setPlaneId(unsigned char val) { mPlaneId = val; }

void setRomanPotId(unsigned char val) { mRomanPotId = val; };


 protected:
    Float_t       mPosition;
    Float_t       mPositionRMS;    
    Float_t       mEnergy;
    Float_t       mXY;
    Short_t        mLength;
    UChar_t        mQuality;
    UChar_t        mPlaneId;
    UChar_t        mRomanPotId;

    ClassDef(StUPCRpsCluster,2)
};

#endif
