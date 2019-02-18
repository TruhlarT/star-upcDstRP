#include "StUPCRpsPreTrackPoint.h"
#include "StUPCRpsTrackPoint.h"

ClassImp(StUPCRpsPreTrackPoint)

StUPCRpsPreTrackPoint::StUPCRpsPreTrackPoint() {
    mRpId = -1;
    for (unsigned int i=0; i<StUPCRpsTrackPoint::mNumberOfPlanesInRp; ++i) mClusterId[i] = -1;
    for (unsigned int i=0; i<StUPCRpsTrackPoint::mNumberOfPmtsInRp; ++i) mTime[i] = -1;
    mQuality = rpsNotSet; // mQuality = StUPCRpsPreTrackPoint::StRpsPreTrackPointQuality::rpsNotSet
}

StUPCRpsPreTrackPoint::StUPCRpsPreTrackPoint(const StUPCRpsPreTrackPoint& trackPoint) {
    *this = trackPoint;
}

StUPCRpsPreTrackPoint::~StUPCRpsPreTrackPoint() { /* no op */ }

StUPCRpsPreTrackPoint& StUPCRpsPreTrackPoint::operator=(const StUPCRpsPreTrackPoint& trackPoint) {
    if (this != &trackPoint) {
        mPosition = trackPoint.positionVec();
        mRpId = trackPoint.rpId();
        for (unsigned int i=0; i<StUPCRpsTrackPoint::mNumberOfPlanesInRp; ++i ) mClusterId[i] = trackPoint.clusterId(i);
        for (unsigned int i=0; i<StUPCRpsTrackPoint::mNumberOfPmtsInRp; ++i ) mTime[i] = trackPoint.time(i);
        mQuality = (StUPCRpsPreTrackPoint::StRpsPreTrackPointQuality)trackPoint.quality();
    }
    return *this;
}

unsigned int StUPCRpsPreTrackPoint::planesUsed() const {
    unsigned int nPlanesUsed = 0;
    for(unsigned int i=0; i<StUPCRpsTrackPoint::mNumberOfPlanesInRp; ++i)
        if (mClusterId[i]>-1) ++nPlanesUsed;
    return nPlanesUsed;
}