/***************************************************************************
 *
 * $Id: StUPCRpsRomanPot.cxx,v 2.1 2009/11/23 22:18:25 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsRomanPot.cxx,v $
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include "StUPCRpsRomanPot.h"
#include "StUPCRpsCollection.h"
#include "StUPCRpsCluster.h"

static const char rcsid[] = "$Id: StUPCRpsRomanPot.cxx,v 2.1 2009/11/23 22:18:25 ullrich Exp $";

ClassImp(StUPCRpsRomanPot)

StUPCRpsRomanPot::StUPCRpsRomanPot()
{
    mStatus = 0;
    mAdc[0] = mAdc[1] = mTac[0] = mTac[1] = 0;
    // Roman Pot ID gets set in 
    // StUPCRpsCollection constructor    
}

StUPCRpsRomanPot::~StUPCRpsRomanPot() { /* noop */ }

unsigned int 
StUPCRpsRomanPot::romanPotId() const
{
    return mRomanPotId;
}

unsigned int 
StUPCRpsRomanPot::numberOfPlanes() const
{
    return mNumberOfPlanes;
}

unsigned int 
StUPCRpsRomanPot::numberOfPlanesWithClusters() const
{
    unsigned int count = 0;
    for (int i=0; i<mNumberOfPlanes; i++)  
        count += mPlanes[i].numberOfClusters();
    return count;
}

unsigned int 
StUPCRpsRomanPot::adc(unsigned int i) const
{
    return (i < 2 ? mAdc[i] : 0);
}

unsigned int 
StUPCRpsRomanPot::tac(unsigned int i) const
{
    return (i < 2 ? mTac[i] : 0);
}

unsigned char 
StUPCRpsRomanPot::status() const
{
    return mStatus;
}

const StUPCRpsPlane* 
StUPCRpsRomanPot::plane(unsigned int i) const
{
    if (i < mNumberOfPlanes)
        return &mPlanes[i];
    else
        return 0;
}

StUPCRpsPlane* StUPCRpsRomanPot::plane(unsigned int i)
{
    if (i < mNumberOfPlanes)
        return &mPlanes[i];
    else
        return 0;
}

void 
StUPCRpsRomanPot::setStatus(unsigned char val)
{
    mStatus = val;
}

void StUPCRpsRomanPot::setRomanPotId(unsigned char i) 
{
    mRomanPotId = i;
}

void StUPCRpsRomanPot::setAdc(unsigned int adc0, unsigned int adc1) 
{
    mAdc[0] = adc0;
    mAdc[1] = adc1;
}

void StUPCRpsRomanPot::setTac(unsigned int tac0, unsigned int tac1) 
{
    mTac[0] = tac0;
    mTac[1] = tac1;
}

