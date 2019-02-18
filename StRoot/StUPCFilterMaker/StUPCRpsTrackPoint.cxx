/***************************************************************************
 *
 * $Id: StUPCRpsTrackPoint.cxx,v 2.1 2015/10/02 19:48:14 ullrich Exp $
 *
 * Author: Rafal Sikora, 1 Oct 2015
 *
 ***************************************************************************
 *
 * Description: StUPCRpsTrackPoint class representing reconstructed (x,y,z)
 * position of the hit in single Roman Pot detector
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsTrackPoint.cxx,v $
 * Revision 2.1  2015/10/02 19:48:14  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#include "StUPCRpsTrackPoint.h"

ClassImp(StUPCRpsTrackPoint)

StUPCRpsTrackPoint::StUPCRpsTrackPoint()
{
    mRpId = -1;
    for (unsigned int i=0; i<mNumberOfPlanesInRp; ++i) mClusterId[i] = -1;
    for (unsigned int i=0; i<mNumberOfPmtsInRp; ++i) mTime[i] = -1;
    mQuality = rpsNotSet;
}

StUPCRpsTrackPoint::StUPCRpsTrackPoint(const StUPCRpsTrackPoint& trackPoint)
{
    *this = trackPoint;
}

StUPCRpsTrackPoint::~StUPCRpsTrackPoint() { /* no op */ }

StUPCRpsTrackPoint& StUPCRpsTrackPoint::operator=(const StUPCRpsTrackPoint& trackPoint)
{
    if (this != &trackPoint) {
        mPosition = trackPoint.positionVec();
        mRpId = trackPoint.rpId();
        for (unsigned int i=0; i<mNumberOfPlanesInRp; ++i ) mClusterId[i] = trackPoint.clusterId(i);
        for (unsigned int i=0; i<mNumberOfPmtsInRp; ++i ) mTime[i] = trackPoint.time(i);
        mQuality = trackPoint.quality();
    }
    return *this;
}

unsigned int StUPCRpsTrackPoint::planesUsed() const
{
    unsigned int nPlanesUsed = 0;
    for(unsigned int i=0; i<mNumberOfPlanesInRp; ++i)
        if (mClusterId[i]>-1) ++nPlanesUsed;
    return nPlanesUsed;
}