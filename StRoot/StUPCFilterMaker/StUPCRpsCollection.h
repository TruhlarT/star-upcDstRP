/***************************************************************************
 *
 * $Id: StUPCRpsCollection.h,v 2.3 2015/10/02 19:50:50 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsCollection.h,v $
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
#ifndef StUPCRpsCollection_hh
#define StUPCRpsCollection_hh

#include "TObject.h" // "StObject.h"
#include "TClonesArray.h" // "StContainers.h"
#include "StUPCRpsRomanPot.h"
#include "StUPCRpsTrackPoint.h"
#include "StUPCRpsTrack.h"
#include <vector> // added

using namespace std; // added

class StUPCRpsCollection : public TObject {
public:
    StUPCRpsCollection();
    ~StUPCRpsCollection();
    
    unsigned int numberOfRomanPots() const;
    
    const StUPCRpsRomanPot* romanPot(unsigned int) const;
    StUPCRpsRomanPot* romanPot(unsigned int);
    
    vector<StUPCRpsCluster*> clusters() const; // StPtrVecRpsCluster clusters() const;
    vector<StUPCRpsTrackPoint*> trackPoints() const; // StPtrVecRpsTrackPoint trackPoints() const;
    vector<StUPCRpsTrack*> tracks() const; // StPtrVecRpsTrack tracks() const;

    unsigned char siliconBunch() const;
    
    void setSiliconBunch(unsigned char);
    void addTrackPoint(StUPCRpsTrackPoint*); // void addTrackPoint(const StUPCRpsTrackPoint*);
    void addTrack(StUPCRpsTrack*); // void addTrack(const StUPCRpsTrack*);
    
    enum {mNumberOfRomanPots = 8};
    
private:
    StUPCRpsRomanPot mRomanPots[mNumberOfRomanPots];
    UChar_t mSiliconBunch;
    
    vector<StUPCRpsTrackPoint*> 	mTrackPoints; // StSPtrVecRpsTrackPoint mTrackPoints;
	vector<StUPCRpsTrack*> 		mTracks; // StSPtrVecRpsTrack mTracks;
    

    ClassDef(StUPCRpsCollection, 3)
};

// inline void StUPCRpsCollection::addTrackPoint(const StUPCRpsTrackPoint *trackPoint) {
inline void StUPCRpsCollection::addTrackPoint(StUPCRpsTrackPoint *trackPoint) {
    mTrackPoints.push_back(trackPoint);
}

// inline void StUPCRpsCollection::addTrack(const StUPCRpsTrack *track) {
inline void StUPCRpsCollection::addTrack(StUPCRpsTrack *track) {
    mTracks.push_back(track);
}

#endif
