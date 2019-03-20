//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________

#include "StUPCRpsTrackPoint.h"
#include "StUPCRpsTrack.h"
#include <cmath>

ClassImp(StUPCRpsTrack)

StUPCRpsTrack::StUPCRpsTrack(){
    for(unsigned int i=0; i<mNumberOfStationsInBranch; ++i)
        mTrackPoints[i] = nullptr;
    mBranch = -1;
    mType = rpsUndefined;
}

StUPCRpsTrack::~StUPCRpsTrack() { /* no op */ }

StUPCRpsTrack& StUPCRpsTrack::operator=(const StUPCRpsTrack& track) {
    if (this != &track) {
        for(unsigned int i=0; i<mNumberOfStationsInBranch; ++i)
            mTrackPoints[i] = const_cast<StUPCRpsTrackPoint*>(track.trackPoint(i));
        mP = track.pVec();
        mType = track.type();
    }
    return *this;
}

unsigned int StUPCRpsTrack::planesUsed() const {
    unsigned int nPlanes = 0;
    for(unsigned int i=0; i<mNumberOfStationsInBranch; ++i)
        nPlanes += mTrackPoints[i].GetObject() ? trackPoint(i)->planesUsed() : 0;
    return nPlanes;
}

double StUPCRpsTrack::thetaRp(unsigned int coordinate) const {
    if(coordinate>rpsAngleTheta) return 0.0;
    if(mType==rpsLocal) return theta(coordinate);
    TVector3 deltaVector = trackPoint(1)->positionVec() - trackPoint(0)->positionVec();
    return atan((coordinate<rpsAngleTheta ? deltaVector[coordinate] : deltaVector.Perp())/abs(deltaVector.z()));
}

double StUPCRpsTrack::phiRp() const{
    if(mType==rpsLocal) return phi();
    TVector3 deltaVector = trackPoint(1)->positionVec() - trackPoint(0)->positionVec();
    return deltaVector.Phi();
}

double StUPCRpsTrack::time() const{
    double sumTime=0.0;
    unsigned int numberOfPmtsWithSignal=0;
    for(unsigned int i=0; i<mNumberOfStationsInBranch; ++i){
        if(trackPoint(i))
            for(int j=0; j< trackPoint(i)->mNumberOfPmtsInRp; ++j){
                if(trackPoint(i)->time(j)>0){
                    sumTime += trackPoint(i)->time(j);
                    ++numberOfPmtsWithSignal;
                }
            }
    }
    return numberOfPmtsWithSignal>0 ? sumTime/numberOfPmtsWithSignal : -1;
}

double StUPCRpsTrack::theta(unsigned int coordinate) const
{
    return coordinate < mNumberOfAngleTypes ? atan((coordinate<rpsAngleTheta ? mP[coordinate] : mP.Perp())/abs(mP.z())) : 0.0;
}
