/***************************************************************************
 * 
 * $Id: StUPCRpsTrack.h,v 2.4 2015/10/22 20:31:31 ullrich Exp $
 *
 * Author: Rafal Sikora, 1 Oct 2015
 *
 ***************************************************************************
 *
 * Description: StUPCRpsTrack class representing reconstructed track in 
 * the Roman Pot system, with all associated observables, such as 
 * momentum (px, py, pz) etc..
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsTrack.h,v $
 * Revision 2.4  2015/10/22 20:31:31  ullrich
 * StUPCRpsTrack.cxx
 *
 * Revision 2.3  2015/10/08 20:53:34  ullrich
 * Changed comment of mBranch
 *
 * Revision 2.2  2015/10/07 17:30:13  ullrich
 * Changed const to enums and related changes.
 *
 * Revision 2.1  2015/10/02 19:48:14  ullrich
 * Initial Revision.
 *
 ***************************************************************************/
#ifndef StUPCRpsTrack_hh
#define StUPCRpsTrack_hh

#include "TObject.h" // "StObject.h"
#include "TClonesArray.h" // "StContainers.h"
#include "TVector3.h" // "StThreeVectorF.h"
#include <vector> // added

using namespace std; // added

class StUPCRpsTrackPoint;

class StUPCRpsTrack : public TObject {
public:
    StUPCRpsTrack();
    StUPCRpsTrack(const StUPCRpsTrack&);
    ~StUPCRpsTrack();

    StUPCRpsTrack& operator=(const StUPCRpsTrack&);
    enum StRpsTrackType { rpsLocal, rpsGlobal, rpsUndefined };
    enum StRpsAngles { rpsAngleThetaX, rpsAngleThetaY, rpsAngleTheta, mNumberOfAngleTypes };

    StUPCRpsTrackPoint* trackPoint(unsigned int) const;
    TVector3 pVec() const;
    int branch() const;
    StRpsTrackType type() const;
    unsigned int planesUsed() const;
    
    double theta(unsigned int = rpsAngleTheta) const;
    double thetaRp(unsigned int = rpsAngleTheta) const;
    double phi() const;
    double phiRp() const;
    double t(double) const;
    double xi(double) const;
    double p() const;
    double pt() const;
    double eta() const;
    double time() const;

    void setTrackPoint(StUPCRpsTrackPoint*, unsigned int);
    void setP(const TVector3&);
    void setBranch(int);
    void setType(StRpsTrackType);

    enum {mNumberOfStationsInBranch = 2};

private:
    vector<StUPCRpsTrackPoint*> 	mTrackPoints; //StPtrVecRpsTrackPoint mTrackPoints;	 // pointers to track points (local tracks)
    TVector3 mP;				// three-vector with reconstructed track momentum
    Int_t          mBranch;			// detectors branch, EU=0, ED=1, WU=2, WD=3 
    StRpsTrackType mType;			// type of the track
    
    ClassDef(StUPCRpsTrack, 1)
};

inline StUPCRpsTrackPoint* StUPCRpsTrack::trackPoint(unsigned int station) const
{
    return station < mNumberOfStationsInBranch ? mTrackPoints[station] : nullptr;
}
inline TVector3 StUPCRpsTrack::pVec() const { return mP; }
inline int StUPCRpsTrack::branch() const { return mBranch; }
inline StUPCRpsTrack::StRpsTrackType StUPCRpsTrack::type() const { return mType; }
inline double StUPCRpsTrack::phi() const { return mP.Phi(); } // return mP.phi();
inline double StUPCRpsTrack::t(double beamMomentum) const
{
  return -2*beamMomentum*beamMomentum*(1-xi(beamMomentum))*(1-cos(theta(rpsAngleTheta)));
}
inline double StUPCRpsTrack::xi(double beamMomentum) const
{
    return (beamMomentum - mP.Mag())/beamMomentum; // return (beamMomentum - mP.mag())/beamMomentum;
}
inline double StUPCRpsTrack::p() const { return mP.Mag(); } // return mP.mag();
inline double StUPCRpsTrack::pt() const { return mP.Perp(); } // return mP.perp();
inline double StUPCRpsTrack::eta() const { return mP.PseudoRapidity(); } // return mP.pseudoRapidity();

inline void StUPCRpsTrack::setTrackPoint(StUPCRpsTrackPoint* trackPoint, unsigned int station)
{
    if (station<mNumberOfStationsInBranch)
        mTrackPoints[station] = trackPoint;
}
inline void StUPCRpsTrack::setP(const TVector3& P) { mP = P; }
inline void StUPCRpsTrack::setBranch(int branch) { mBranch = branch; }
inline void StUPCRpsTrack::setType(StUPCRpsTrack::StRpsTrackType type) { mType = type; }

#endif
