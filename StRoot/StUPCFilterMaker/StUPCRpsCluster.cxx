/***************************************************************************
 *
 * $Id: StUPCRpsCluster.cxx,v 2.2 2015/10/02 19:50:09 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsCluster.cxx,v $
 * Revision 2.2  2015/10/02 19:50:09  ullrich
 * Added mPositionRMS and accessors.
 *
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include "StUPCRpsCluster.h"

static const char rcsid[] = "$Id: StUPCRpsCluster.cxx,v 2.2 2015/10/02 19:50:09 ullrich Exp $";

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

StUPCRpsCluster::~StUPCRpsCluster() { /* noop */ };

double 
StUPCRpsCluster::position() const { return mPosition; }

double 
StUPCRpsCluster::positionRMS() const { return mPositionRMS; }

short  
StUPCRpsCluster::length() const { return mLength; }

double 
StUPCRpsCluster::energy() const { return mEnergy; }

double 
StUPCRpsCluster::xy() const { return mXY; }

unsigned char 
StUPCRpsCluster::quality() const { return mQuality; }

unsigned int 
StUPCRpsCluster::romanPotId() const { return mRomanPotId; }

unsigned int 
StUPCRpsCluster::planeId() const { return mPlaneId; }

void 
StUPCRpsCluster::setPosition(double val) { mPosition = val; }

void 
StUPCRpsCluster::setPositionRMS(double val) { mPositionRMS = val; }

void 
StUPCRpsCluster::setLength(short val) { mLength = val; }

void 
StUPCRpsCluster::setEnergy(double val) { mEnergy = val; }

void 
StUPCRpsCluster::setXY(double val) { mXY = val; }

void 
StUPCRpsCluster::setQuality(unsigned char val) { mQuality = val; }

void 
StUPCRpsCluster::setPlaneId(unsigned char val) { mPlaneId = val; }

void 
StUPCRpsCluster::setRomanPotId(unsigned char val) { mRomanPotId = val; }

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
