#ifndef StUPCRpsTrackPoint_hh
#define StUPCRpsTrackPoint_hh

//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________


#include "TObject.h" 
#include "TVector3.h" 


class StUPCRpsTrackPoint : public TObject {
public:
	StUPCRpsTrackPoint();
	virtual ~StUPCRpsTrackPoint();

	StUPCRpsTrackPoint& operator=(const StUPCRpsTrackPoint&);
	enum StUPCRpsTrackPointQuality {rpsNormal, rpsGolden, rpsNotSet}; 

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

	static const Int_t mNumberOfPmtsInRp = 2;
	static const Int_t mNumberOfPlanesInRp = 2;

	TVector3 positionVec() const { return mPosition; }
	int rpId() const { return mRpId; }
	int clusterId(unsigned int planeId ) const
	{
	    return planeId<mNumberOfPlanesInRp ? mClusterId[planeId] : -1;
	}
	double time(unsigned int pmtId) const
	{
	    return pmtId<mNumberOfPmtsInRp ? mTime[pmtId] : -1;
	}
	StUPCRpsTrackPointQuality quality() const { return mQuality; }
	double x() const { return mPosition.x(); }
	double y() const { return mPosition.y(); }
	double z() const { return mPosition.z(); }

	void setPosition(const TVector3& position)
	{
	    mPosition = position;
	}
	void setRpId(int rpId) { mRpId = rpId; }
	void setClusterId(int clusterId, unsigned int planeId)
	{
	    if( planeId<mNumberOfPlanesInRp )
	        mClusterId[planeId] = clusterId;
	}
	void setTime(double timeVal, unsigned int pmtId)
	{
	    if( pmtId<mNumberOfPmtsInRp ) mTime[pmtId] = timeVal;
	}
	void setQuality(StUPCRpsTrackPointQuality quality )
	{
	    mQuality = quality;
	}
    
private:
    int mRpId;
    int mClusterId[mNumberOfPlanesInRp];
    double mTime[mNumberOfPmtsInRp];

    TVector3         mPosition;
    StUPCRpsTrackPointQuality mQuality;

    ClassDef( StUPCRpsTrackPoint, 1 )
};

#endif
