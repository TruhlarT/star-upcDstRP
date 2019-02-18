/***************************************************************************
 *
 * $Id: StUPCRpsPlane.h,v 2.1 2009/11/23 22:18:25 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsPlane.h,v $
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StUPCRpsPlane_hh
#define StUPCRpsPlane_hh

#include "TObject.h" // "StObject.h"
#include "TClonesArray.h" // "StContainers.h"
#include <vector>

using namespace std; // added

class StUPCRpsCluster;
class StUPCRpsRomanPot;

class StUPCRpsPlane : public TObject {
public:
    StUPCRpsPlane();
    ~StUPCRpsPlane();

    double offset() const;
    double z() const;
    double angle() const;
    short  orientation() const;
    unsigned char status() const;
    unsigned int  numberOfClusters() const;
    unsigned int  planeId() const;
    
    unsigned int  romanPotId() const;

    const StUPCRpsCluster* cluster(unsigned int) const;
    StUPCRpsCluster*       cluster(unsigned int);
        
    const vector<StUPCRpsCluster*>& clusters() const; // const StSPtrVecRpsCluster& clusters() const;
    vector<StUPCRpsCluster*>&       clusters(); // StSPtrVecRpsCluster&       clusters();

    void addCluster(StUPCRpsCluster*);
    void setOffset(double);
    void setZ(double);
    void setAngle(double);
    void setOrientation(short);
    void setStatus(unsigned char);
  
protected:    
    friend class StUPCRpsCollection;
    void setPlaneId(unsigned char);
    void setRomanPotId(unsigned char);
    
protected:
    UChar_t  mPlaneId; // 0-3
    Double_t mOffset;
    Double_t mZ;
    Double_t mAngle;
    Short_t  mOrientation;
    UChar_t  mStatus;
    UChar_t  mRomanPotId; // 0-7
    vector<StUPCRpsCluster*> 	mClusters; //StSPtrVecRpsCluster mClusters;

    ClassDef(StUPCRpsPlane,1)
};

#endif
