/***************************************************************************
 *
 * $Id: StUPCRpsCollection.cxx,v 2.3 2015/10/02 19:50:50 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsCollection.cxx,v $
 * Revision 2.3  2015/10/02 19:50:50  ullrich
 * Added containers for tracks and points.
 *
 * Revision 2.2  2010/02/04 18:16:09  ullrich
 * Added new member mSiliconBunch and referring access methods.
 *
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include "StUPCRpsCollection.h"
#include "StUPCRpsPlane.h"
#include <vector> // added

static const char rcsid[] = "$Id: StUPCRpsCollection.cxx,v 2.3 2015/10/02 19:50:50 ullrich Exp $";

ClassImp(StUPCRpsCollection)

StUPCRpsCollection::StUPCRpsCollection()
{
    //
    // Set up all the roman pot and plane IDs
    //
    for (unsigned int i=0; i<mNumberOfRomanPots; i++)  { // Roman Pots
        mRomanPots[i].setRomanPotId(static_cast<unsigned char>(i));
        for (unsigned int j=0; j<mRomanPots[i].numberOfPlanes(); j++) { // planes
            mRomanPots[i].plane(j)->setPlaneId(static_cast<unsigned char>(j));
            mRomanPots[i].plane(j)->setRomanPotId(static_cast<unsigned char>(i));
        }
    }
    mSiliconBunch = 0;
}

StUPCRpsCollection::~StUPCRpsCollection() { /* no op */ }

unsigned int
StUPCRpsCollection::numberOfRomanPots() const
{
    return mNumberOfRomanPots;
}

const StUPCRpsRomanPot*
StUPCRpsCollection::romanPot(unsigned int i) const
{
    if (i < mNumberOfRomanPots)
        return &mRomanPots[i];
    else
        return 0;
}

StUPCRpsRomanPot*
StUPCRpsCollection::romanPot(unsigned int i)
{
    if (i < mNumberOfRomanPots)
        return &mRomanPots[i];
    else
        return 0;
}

vector<StUPCRpsCluster*> StUPCRpsCollection::clusters() const
{
    vector<StUPCRpsCluster*> vec;
    for (unsigned int i=0; i<mNumberOfRomanPots; i++) {
        const StUPCRpsRomanPot *seq = &mRomanPots[i];
        for (unsigned int j=0; j<seq->numberOfPlanes(); j++) {
            const StUPCRpsPlane *plane = seq->plane(j);
            vector<StUPCRpsCluster*> vecTmp = plane->clusters();
            for (unsigned int k=0; k< vecTmp.size(); k++)
                vec.push_back(vecTmp[k]);
            //for (unsigned int k=0; k<plane->numberOfClusters(); k++)
              //  vec.push_back(plane->cluster(k));
        }
    }
    return vec;
}

vector<StUPCRpsTrackPoint*> StUPCRpsCollection::trackPoints() const {
    vector<StUPCRpsTrackPoint*> trackPointsVec;
    for (unsigned int i=0; i<mTrackPoints.size(); ++i) {
        trackPointsVec.push_back( mTrackPoints[i] );
    }
    return trackPointsVec;
}

vector<StUPCRpsTrack*> StUPCRpsCollection::tracks() const {
    vector<StUPCRpsTrack*> tracksVec;
    for(unsigned int i=0; i<mTracks.size(); ++i){
        tracksVec.push_back( mTracks[i] );
    }
    return tracksVec;
}

unsigned char
StUPCRpsCollection::siliconBunch() const
{
    return mSiliconBunch;
}

void
StUPCRpsCollection::setSiliconBunch(unsigned char val)
{
    mSiliconBunch = val;
}

