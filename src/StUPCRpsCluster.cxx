#include "StUPCRpsCluster.h"

ClassImp(StUPCRpsCluster)

StUPCRpsCluster::StUPCRpsCluster()
{
    mPosition = mEnergy = mXY = 0;
    mLength = 0;
    mQuality = 0;
    mPlaneId = mRomanPotId = 0;
}


StUPCRpsCluster::StUPCRpsCluster(Float_t pos, Float_t posRMS, Short_t len,
                                    Float_t e, Float_t xy, UChar_t qual)
{
    mPosition = pos;
    mPositionRMS = posRMS;
    mLength = len;
    mEnergy = e;
    mXY = xy;
    mQuality = qual;
    mPlaneId = mRomanPotId = 0;
    // mPlaneId and mRomanPotId are later set by StRpsPlane::addCluster()
}

