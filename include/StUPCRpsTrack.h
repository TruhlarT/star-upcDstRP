#ifndef StUPCRpsTrack_hh
#define StUPCRpsTrack_hh

//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________

#include "TObject.h"
#include "TRef.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include <vector> 

using namespace std;

class StUPCRpsTrackPoint;

class StUPCRpsTrack : public TObject {
public:
	StUPCRpsTrack();
	virtual ~StUPCRpsTrack();

	StUPCRpsTrack& operator=(const StUPCRpsTrack&);
	enum StRpsTrackType { rpsLocal, rpsGlobal, rpsUndefined };
	enum StRpsAngles { rpsAngleThetaX, rpsAngleThetaY, rpsAngleTheta, mNumberOfAngleTypes };

	unsigned int planesUsed() const;

	double theta(unsigned int = rpsAngleTheta) const;
	double thetaRp(unsigned int = rpsAngleTheta) const;
	double phiRp() const;
	double time() const;


	static const Int_t mNumberOfStationsInBranch = 2;

	StUPCRpsTrackPoint* trackPoint(unsigned int station) const
	{
	    return station < mNumberOfStationsInBranch ? (StUPCRpsTrackPoint*)mTrackPoints[station].GetObject() : nullptr;
	}
	TVector3 pVec() const { return mP; }
	int branch() const { return mBranch; }
	StRpsTrackType type() const { return mType; }
	double phi() const { return mP.Phi(); } 
	double t(double beamMomentum) const
	{
	  return -2*beamMomentum*beamMomentum*(1-xi(beamMomentum))*(1-cos(theta(rpsAngleTheta)));
	}
	double xi(double beamMomentum) const
	{
	    return (beamMomentum - mP.Mag())/beamMomentum; 
	}
	double p() const { return mP.Mag(); }
	double pt() const { return mP.Perp(); } 
	double eta() const { return mP.PseudoRapidity(); } 

	void setTrackPoint(StUPCRpsTrackPoint* trackPoint, unsigned int station)
	{
	    if (station<mNumberOfStationsInBranch)
	        mTrackPoints[station] = (TObject*)trackPoint;
	        //mTrackPoints[station] = static_cast<const TObject*>(trackPoint);
	}
	void setP(const TVector3& P) { mP = P; }
	void setBranch(int branch) { mBranch = branch; }
	void setType(StRpsTrackType type) { mType = type; }

private:	

	TRef mTrackPoints[mNumberOfStationsInBranch]; // pointers to track points (local tracks)
	TVector3 mP;				// three-vector with reconstructed track momentum
	Int_t          mBranch;			// detectors branch, EU=0, ED=1, WU=2, WD=3 
	StRpsTrackType mType;			// type of the track

	ClassDef(StUPCRpsTrack, 1)
};

#endif
