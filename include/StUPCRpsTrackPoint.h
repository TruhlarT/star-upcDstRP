/***************************************************************************
 *
 * $Id: StUPCRpsTrackPoint.h,v 2.2 2015/10/07 17:30:13 ullrich Exp $
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
 * $Log: StUPCRpsTrackPoint.h,v $
 * Revision 2.2  2015/10/07 17:30:13  ullrich
 * Changed const to enums and related changes.
 *
 * Revision 2.1  2015/10/02 19:48:14  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StUPCRpsTrackPoint_hh
#define StUPCRpsTrackPoint_hh

#include "TObject.h"  // "StObject.h"
#include "TVector3.h" // "StThreeVectorF.h"


class StUPCRpsTrackPoint : public TObject {
public:
    StUPCRpsTrackPoint();
    StUPCRpsTrackPoint(const StUPCRpsTrackPoint&);
    ~StUPCRpsTrackPoint();
    
    StUPCRpsTrackPoint& operator=(const StUPCRpsTrackPoint&);
    enum StUPCRpsTrackPointQuality {rpsNormal, rpsGolden, rpsNotSet}; // add = 0?
    
    TVector3 positionVec() const;
    int rpId() const;
    int clusterId(unsigned int) const;
    double time(unsigned int) const;
    StUPCRpsTrackPointQuality quality() const;
    unsigned int planesUsed() const;
    
    double x() const;
    double y() const;
    double z() const;
    
    void setPosition(const TVector3&);
    void setRpId(int);
    void setClusterId(int, unsigned int);
    void setTime(double, unsigned int);
    void setQuality(StUPCRpsTrackPointQuality);

    enum {mNumberOfPmtsInRp = 2, mNumberOfPlanesInRp = 4};
    
private:
    int mRpId;
    int mClusterId[mNumberOfPlanesInRp];
    double mTime[mNumberOfPmtsInRp];

    TVector3         mPosition;
    StUPCRpsTrackPointQuality mQuality;

    ClassDef( StUPCRpsTrackPoint, 1 )
};

inline TVector3 StUPCRpsTrackPoint::positionVec() const { return mPosition; }
inline int StUPCRpsTrackPoint::rpId() const { return mRpId; }
inline int StUPCRpsTrackPoint::clusterId(unsigned int planeId ) const
{
    return planeId<mNumberOfPlanesInRp ? mClusterId[planeId] : -1;
}
inline double StUPCRpsTrackPoint::time(unsigned int pmtId) const
{
    return pmtId<mNumberOfPmtsInRp ? mTime[pmtId] : -1;
}
inline StUPCRpsTrackPoint::StUPCRpsTrackPointQuality StUPCRpsTrackPoint::quality() const { return mQuality; }
inline double StUPCRpsTrackPoint::x() const { return mPosition.x(); }
inline double StUPCRpsTrackPoint::y() const { return mPosition.y(); }
inline double StUPCRpsTrackPoint::z() const { return mPosition.z(); }

inline void StUPCRpsTrackPoint::setPosition(const TVector3& position)
{
    mPosition = position;
}
inline void StUPCRpsTrackPoint::setRpId(int rpId) { mRpId = rpId; }
inline void StUPCRpsTrackPoint::setClusterId(int clusterId, unsigned int planeId)
{
    if( planeId<mNumberOfPlanesInRp )
        mClusterId[planeId] = clusterId;
}
inline void StUPCRpsTrackPoint::setTime(double timeVal, unsigned int pmtId)
{
    if( pmtId<mNumberOfPmtsInRp ) mTime[pmtId] = timeVal;
}
inline void StUPCRpsTrackPoint::setQuality(StUPCRpsTrackPointQuality quality )
{
    mQuality = quality;
}

#endif
