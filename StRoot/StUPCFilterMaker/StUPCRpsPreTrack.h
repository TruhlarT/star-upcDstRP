#ifndef __StUPCRpsPreTrack_hh__
#define __StUPCRpsPreTrack_hh__

#include "TObject.h"
#include "TVector3.h"
#include "TRef.h"
#include <vector>

using namespace std;

class StUPCRpsTrackPoint;
class StUPCRpsTrack;

class StUPCRpsPreTrack : public TObject {
public:
	enum StRpsPreTrackType { rpsLocal, rpsGlobal, rpsUndefined };
    enum StRpsPreAngles { rpsAngleThetaX, rpsAngleThetaY, rpsAngleTheta, mNumberOfAngleTypes };
    enum {mNumberOfStationsInBranch = 2};

protected:
    TRef mTrackPoints[mNumberOfStationsInBranch];   // reference to track points (local tracks)
    TVector3 mP;								    // three-vector with reconstructed track momentum
    Int_t          mBranch;                         // detectors branch, EU=0, ED=1, WU=2, WD=3 
    StRpsPreTrackType mType;                         // type of the track
    
public:
    StUPCRpsPreTrack();
    StUPCRpsPreTrack(const StUPCRpsPreTrack&);
    ~StUPCRpsPreTrack();

    StUPCRpsPreTrack& operator=(const StUPCRpsPreTrack&);

    const StUPCRpsPreTrackPoint* trackPoint(unsigned int)const ;
    TVector3 pVec() const;
    int branch() const;
    StRpsPreTrackType type() const;
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

    void setTrackPoint( StUPCRpsPreTrackPoint*, unsigned int);
    void setP(const TVector3&);
    void setBranch(int);
    void setType(StRpsPreTrackType type);
    
    ClassDef(StUPCRpsPreTrack, 1)
};

inline const StUPCRpsPreTrackPoint* StUPCRpsPreTrack::trackPoint(unsigned int station) const {
    return station < mNumberOfStationsInBranch ? static_cast<const StUPCRpsPreTrackPoint*>(mTrackPoints[station].GetObject()) : nullptr;
}
inline TVector3 StUPCRpsPreTrack::pVec() const { return mP; }
inline int StUPCRpsPreTrack::branch() const { return mBranch; }
inline StUPCRpsPreTrack::StRpsPreTrackType StUPCRpsPreTrack::type() const { return mType; }
inline double StUPCRpsPreTrack::phi() const { return mP.Phi(); }
inline double StUPCRpsPreTrack::t(double beamMomentum) const {
  return -2*beamMomentum*beamMomentum*(1-xi(beamMomentum))*(1-cos(theta(rpsAngleTheta)));
}
inline double StUPCRpsPreTrack::xi(double beamMomentum) const {
    return (beamMomentum - mP.Mag())/beamMomentum;
}
inline double StUPCRpsPreTrack::p() const { return mP.Mag(); }
inline double StUPCRpsPreTrack::pt() const { return mP.Perp(); }
inline double StUPCRpsPreTrack::eta() const { return mP.PseudoRapidity(); }

inline void StUPCRpsPreTrack::setTrackPoint(StUPCRpsPreTrackPoint* trackPoint, unsigned int station) {
    if (station<mNumberOfStationsInBranch)
        mTrackPoints[station] = trackPoint;
}
inline void StUPCRpsPreTrack::setP(const TVector3& P) { mP = P; }
inline void StUPCRpsPreTrack::setBranch(int branch) { mBranch = branch; }
inline void StUPCRpsPreTrack::setType(StRpsPreTrackType type) { mType = type; }

#endif
