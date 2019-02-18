#ifndef StUPCFilterRPUtil_h
#define StUPCFilterRPUtil_h

#include "StMaker.h"

class StRPEvent;
class StMuDst;

/*
class StUPCRpsCollection;
class StMuRpsCollection;
class StUPCRpsRomanPot; 
class StUPCRpsPlane;
class StUPCRpsCluster;
class StMuRpsTrackPoint;
class StUPCRpsTrackPoint;
class StMuRpsTrack;
class StUPCRpsTrack;
*/
class StUPCFilterRPUtil {

public:

  StUPCFilterRPUtil();
  virtual ~StUPCFilterRPUtil() {}

  void processEvent(StRPEvent *rpEvt, StMuDst *mMuDst);

private:

 // StUPCFilterRPUtil(const StUPCFilterRPUtil &o); // not implemented
 // StUPCFilterRPUtil &operator=(const StUPCFilterRPUtil &o); // not implemented

};

#endif

