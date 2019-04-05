#ifndef StUPCRpsCluster_hh
#define StUPCRpsCluster_hh

//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________


#include <iostream> 
#include "TObject.h" 


class StUPCRpsCluster : public TObject {
public:
    StUPCRpsCluster();
    StUPCRpsCluster(double pos, double posRMS, short len,
                 double e, double xy, unsigned char qual);
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
    Double_t       mPosition;
    Double_t       mPositionRMS;
    Short_t        mLength;    
    Double_t       mEnergy;
    Double_t       mXY;
    UChar_t        mQuality;
    UChar_t        mPlaneId;
    UChar_t        mRomanPotId;

    ClassDef(StUPCRpsCluster,2)
};

ostream& operator<<(ostream&, const StUPCRpsCluster&);

#endif
