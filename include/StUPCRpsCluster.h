/***************************************************************************
 *
 * $Id: StUPCRpsCluster.h,v 2.2 2015/10/02 19:50:09 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:  Reconstructed cluster in the Roman Pot Silicon 
 *               detectors.         
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsCluster.h,v $
 * Revision 2.2  2015/10/02 19:50:09  ullrich
 * Added mPositionRMS and accessors.
 *
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StUPCRpsCluster_hh
#define StUPCRpsCluster_hh

#include <iostream> // #include <Stiostream.h>
#include "TObject.h" // #include "StObject.h"

class StUPCFilterRPUtil;

class StUPCRpsCluster : public TObject {
public:
    StUPCRpsCluster();
    StUPCRpsCluster(double pos, double posRMS, short len,
                 double e, double xy, unsigned char qual);
    ~StUPCRpsCluster();

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


protected:
    friend class StUPCFilterRPUtil;
    void setPlaneId(unsigned int val) { mPlaneId = val; }
    void setRomanPotId(unsigned int val) { mRomanPotId = val; }

 protected:
    Double_t       mPosition;
    Double_t       mPositionRMS;
    Short_t        mLength;    
    Double_t       mEnergy;
    Double_t       mXY;
    UChar_t        mQuality;
    UInt_t        mPlaneId;
    UInt_t        mRomanPotId;

    ClassDef(StUPCRpsCluster,2)
};

ostream& operator<<(ostream&, const StUPCRpsCluster&);

#endif
