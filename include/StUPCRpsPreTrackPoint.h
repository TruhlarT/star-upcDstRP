#ifndef __StUPCRpsPreTrackPoint_hh__
#define __StUPCRpsPreTrackPoint_hh__

#include "TObject.h"
#include "TVector3.h"

// #include "StEvent/StUPCRpsTrackPoint.h"

class StUPCRpsTrackPoint;

class StUPCRpsPreTrackPoint : public TObject {

public:
    enum StRpsPreTrackPointQuality {rpsNormal, rpsGolden, rpsNotSet};
    enum {mNumberOfPmtsInRp = 2, mNumberOfPlanesInRp = 4};
protected:

	// position (x, y, z)
	TVector3	mPosition;
	// 	RP# (e.g. E1U=0)
	Int_t		mRpId;
	// IDs of clusters in RpsCollection
	Int_t 		mClusterId[mNumberOfPlanesInRp];
	// Track Point Quality
    StRpsPreTrackPointQuality mQuality;
    // time
    double mTime[mNumberOfPmtsInRp];

public:
	StUPCRpsPreTrackPoint();
    StUPCRpsPreTrackPoint(const StUPCRpsPreTrackPoint&);
	~StUPCRpsPreTrackPoint();

    StUPCRpsPreTrackPoint& operator=(const StUPCRpsPreTrackPoint&);

	TVector3 positionVec() const;
    Int_t rpId() const;
    Int_t clusterId(unsigned int planeId ) const;
    double time(unsigned int) const;
    StRpsPreTrackPointQuality quality() const;
    
    double x() const;
    double y() const;
    double z() const;
    
    void setPosition(const TVector3&);
    void setRpId(Int_t);
    void setClusterId(Int_t, unsigned int);
    void setQuality(StRpsPreTrackPointQuality quality);
    unsigned int planesUsed() const;

    void setTime(double, unsigned int);


private:

	ClassDef(StUPCRpsPreTrackPoint,1)
};

inline TVector3 StUPCRpsPreTrackPoint::positionVec() const { return mPosition; }
inline Int_t StUPCRpsPreTrackPoint::rpId() const { return mRpId; }
inline Int_t StUPCRpsPreTrackPoint::clusterId(unsigned int planeId ) const {
    return planeId<mNumberOfPlanesInRp ? mClusterId[planeId] : -1;
}
inline StUPCRpsPreTrackPoint::StRpsPreTrackPointQuality StUPCRpsPreTrackPoint::quality() const { return mQuality; }
inline double StUPCRpsPreTrackPoint::x() const { return mPosition.x(); }
inline double StUPCRpsPreTrackPoint::y() const { return mPosition.y(); }
inline double StUPCRpsPreTrackPoint::z() const { return mPosition.z(); }

inline void StUPCRpsPreTrackPoint::setPosition(const TVector3& position){
    mPosition = position;
}
inline void StUPCRpsPreTrackPoint::setRpId(Int_t rpId) { mRpId = rpId; }
inline void StUPCRpsPreTrackPoint::setClusterId(int clusterId, unsigned int planeId) {
    if( planeId<mNumberOfPlanesInRp )
        mClusterId[planeId] = clusterId;
}
inline void StUPCRpsPreTrackPoint::setQuality(StRpsPreTrackPointQuality quality ) {
    mQuality = quality;
}
inline double StUPCRpsPreTrackPoint::time(unsigned int pmtId) const {
    return pmtId<mNumberOfPmtsInRp ? mTime[pmtId] : -1;
}
inline void StUPCRpsPreTrackPoint::setTime(double timeVal, unsigned int pmtId){
    if( pmtId<mNumberOfPmtsInRp ) mTime[pmtId] = timeVal;
}



#endif