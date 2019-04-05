#include "StUPCRpsCluster.h"

ClassImp(StUPCRpsCluster)

StUPCRpsCluster::StUPCRpsCluster()
{
    mPosition = mEnergy = mXY = 0;
    mLength = 0;
    mQuality = 0;
    mPlaneId = mRomanPotId = 0;
}

StUPCRpsCluster::StUPCRpsCluster(double pos, double posRMS, short len,
                           double e, double xy, unsigned char qual)
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


// 
// Non class methods
//
ostream& operator<<(ostream& os, const StUPCRpsCluster& cluster)
{
    os << "position = " << cluster.position() << std::endl; // os << "position = " << cluster.position() << endl;
    os << "position rms = " << cluster.positionRMS() << std::endl; // ...
    os << "length = " << cluster.length() << std::endl;
    os << "energy = " << cluster.energy() << std::endl;
    os << "xy = " << cluster.xy() << std::endl;
    os << "quality = " << static_cast<unsigned int>(cluster.quality()) << std::endl;
    os << "roman pot id = " << cluster.romanPotId() << std::endl;
    os << "plane id = " << cluster.planeId() << std::endl;
    return os;
}
