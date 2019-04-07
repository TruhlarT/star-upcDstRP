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

	unsigned int planesUsed() const;


	static const Int_t mNumberOfPmtsInRp = 2;
	static const Int_t mNumberOfPlanesInRp = 4;

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
    Float_t mTime[mNumberOfPmtsInRp];
    Int_t mRpId;
    Int_t mClusterId[mNumberOfPlanesInRp];
    
    TVector3         mPosition;
    StUPCRpsTrackPointQuality mQuality;

    ClassDef( StUPCRpsTrackPoint, 1 )
};

#endif
