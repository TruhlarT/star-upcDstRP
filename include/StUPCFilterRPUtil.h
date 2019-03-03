#ifndef StUPCFilterRPUtil_h
#define StUPCFilterRPUtil_h

//_____________________________________________________________________________
//    Class for making picoDst RP data 2019
//    Author: Truhlar Tomas
//_____________________________________________________________________________

#include "StMaker.h"

class StRPEvent;
class StMuDst;
class StMuRpsTrackPoint;

class StUPCFilterRPUtil {

public:

  StUPCFilterRPUtil();
  virtual ~StUPCFilterRPUtil() {}

  void processEvent(StRPEvent *rpEvt, StMuDst *mMuDst);

private:

	vector<StMuRpsTrackPoint*>  mTrackPoints;
 // StUPCFilterRPUtil(const StUPCFilterRPUtil &o); // not implemented
 // StUPCFilterRPUtil &operator=(const StUPCFilterRPUtil &o); // not implemented

};

#endif

