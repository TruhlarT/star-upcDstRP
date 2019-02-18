/***************************************************************************
 *
 * $Id: StUPCRpsPlane.cxx,v 2.1 2009/11/23 22:18:25 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:  Reconstructed cluster in the Roman Pot Silicon 
 *               detectors.         
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsPlane.cxx,v $
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include "StUPCRpsPlane.h"
#include "StUPCRpsCluster.h"

static const char rcsid[] = "$Id: StUPCRpsPlane.cxx,v 2.1 2009/11/23 22:18:25 ullrich Exp $";

ClassImp(StUPCRpsPlane)

StUPCRpsPlane::StUPCRpsPlane()
{
    mOffset = mZ = mAngle = 0;
    mOrientation = 0;
    mStatus = 0;
    mRomanPotId = mPlaneId = 0;
}

StUPCRpsPlane::~StUPCRpsPlane() { /* noop */ }

unsigned int 
StUPCRpsPlane::planeId() const { return mPlaneId; }

double 
StUPCRpsPlane::offset() const { return mOffset; }

double 
StUPCRpsPlane::z() const { return mZ; }

double 
StUPCRpsPlane::angle() const { return mAngle; } 

short  
StUPCRpsPlane::orientation() const { return mOrientation; } 

unsigned char 
StUPCRpsPlane::status() const { return mStatus; } 

unsigned int  
StUPCRpsPlane::numberOfClusters() const { return mClusters.size(); }

const StUPCRpsCluster* 
StUPCRpsPlane::cluster(unsigned int i) const
{
    if (i < mClusters.size())
        return mClusters[i];
    else
        return 0;
}

StUPCRpsCluster* 
StUPCRpsPlane::cluster(unsigned int i)
{
    if (i < mClusters.size())
        return mClusters[i];
    else
        return 0;
}

unsigned int  
StUPCRpsPlane::romanPotId() const { return mRomanPotId; }

const vector<StUPCRpsCluster*>& 
StUPCRpsPlane::clusters() const { return mClusters; }

vector<StUPCRpsCluster*>&       
StUPCRpsPlane::clusters() { return mClusters; }

void 
StUPCRpsPlane::addCluster(StUPCRpsCluster* cluster) 
{
    if (cluster) {
        mClusters.push_back(cluster);
        cluster->setPlaneId(mPlaneId);
        cluster->setRomanPotId(mRomanPotId);
    }
}

void 
StUPCRpsPlane::setOffset(double val) { mOffset = val; }

void 
StUPCRpsPlane::setZ(double val) { mZ = val; }

void 
StUPCRpsPlane::setAngle(double val) { mAngle = val; }

void 
StUPCRpsPlane::setOrientation(short val) { mOrientation = val; }

void 
StUPCRpsPlane::setStatus(unsigned char val) { mStatus = val; }

void 
StUPCRpsPlane::setPlaneId(unsigned char i) { mPlaneId = i; }

void 
StUPCRpsPlane::setRomanPotId(unsigned char i) { mRomanPotId = i; }

