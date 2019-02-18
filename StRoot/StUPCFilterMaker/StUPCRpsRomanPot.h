/***************************************************************************
 *
 * $Id: StUPCRpsRomanPot.h,v 2.1 2009/11/23 22:18:25 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StUPCRpsRomanPot.h,v $
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StUPCRpsRomanPot_hh
#define StUPCRpsRomanPot_hh

#include "TObject.h" // "StObject.h"
#include "TClonesArray.h" // "StContainers.h"
#include "StUPCRpsPlane.h"

class StUPCRpsCollection;

class StUPCRpsRomanPot : public TObject {
public:
    StUPCRpsRomanPot();
    ~StUPCRpsRomanPot();

    unsigned int romanPotId() const;
    unsigned int numberOfPlanes() const;
    unsigned int numberOfPlanesWithClusters() const;
    unsigned int adc(unsigned int) const;
    unsigned int tac(unsigned int) const;
    unsigned char status() const;
    
    const StUPCRpsPlane* plane(unsigned int) const;
    StUPCRpsPlane* plane(unsigned int);
    
    void setStatus(unsigned char);
    void setAdc(unsigned int, unsigned int);
    void setTac(unsigned int, unsigned int);
    
protected:
    void setRomanPotId(unsigned char);
    friend class StUPCRpsCollection;
    
protected:
    enum {mNumberOfPlanes = 4};
    StUPCRpsPlane mPlanes[mNumberOfPlanes];
    UChar_t  mRomanPotId; // 0-7
    UChar_t  mStatus;
    UInt_t   mAdc[2];
    UInt_t   mTac[2];

    ClassDef(StUPCRpsRomanPot,1)
};

#endif
