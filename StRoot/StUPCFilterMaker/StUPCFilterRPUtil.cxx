
//_____________________________________________________________________________
//
//    Utility class for RP in UPC filter maker
//    Author: Truhlar Tomas
//
//_____________________________________________________________________________
//c++ headers
#include "string.h"
#include <vector>
#include <iostream>

//root headers
#include "TObjArray.h"
#include "TClonesArray.h"

//StRoot headers
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"


//local headers
#include "StRPEvent.h"
#include "StUPCRpsCollection.h"
#include "StUPCRpsRomanPot.h"
#include "StUPCRpsPlane.h"
#include "StUPCRpsCluster.h"
#include "StUPCRpsTrackPoint.h"
#include "StUPCRpsTrack.h"

#include "StUPCFilterRPUtil.h"


//_____________________________________________________________________________
StUPCFilterRPUtil::StUPCFilterRPUtil() {
  //constructor

}//StUPCFilterRPUtil

//_____________________________________________________________________________
void StUPCFilterRPUtil::processEvent(StRPEvent *rpEvt, StMuDst *mMuDst) {
//_________________MY Part_______________________________//

  StMuRpsCollection *collection = mMuDst->RpsCollection();
  StUPCRpsCollection *rpCollection; //= rpEvt->addCollection(); 
  rpEvt->setSiliconBunch(collection->siliconBunch());

  for(UInt_t iRomanPotId = 0; iRomanPotId < collection->numberOfRomanPots(); ++iRomanPotId){
    //StUPCRpsRomanPot *rpRomanPot;; // = rpCollection->romanPot(iRomanPotId);

    rpEvt->setStatus(iRomanPotId, collection->status(iRomanPotId));
    rpEvt->setAdc(iRomanPotId, collection->adc(iRomanPotId, 0), collection->adc(iRomanPotId, 1));
    rpEvt->setTac(iRomanPotId, collection->tac(iRomanPotId, 0), collection->tac(iRomanPotId, 1)); 
    rpEvt->setNumberPlanes(iRomanPotId, collection->numberOfPlanes());
    rpEvt->setNumberPlanesWithluster(iRomanPotId, collection->numberOfPlanesWithClusters(iRomanPotId));
    for(UInt_t iPlaneId=0; iPlaneId < collection->numberOfPlanes(); ++iPlaneId){
      //StUPCRpsPlane *rpPlane = rpRomanPot->plane(iPlaneId);
      rpEvt->setOffset(iRomanPotId, iPlaneId, collection->offsetPlane(iRomanPotId, iPlaneId));
      rpEvt->setZ(iRomanPotId, iPlaneId, collection->zPlane(iRomanPotId, iPlaneId));  
      rpEvt->setAngle(iRomanPotId, iPlaneId, collection->anglePlane(iRomanPotId, iPlaneId));  
      rpEvt->setOrientation(iRomanPotId, iPlaneId, collection->orientationPlane(iRomanPotId, iPlaneId));  
      rpEvt->setStatus(iRomanPotId, iPlaneId, collection->statusPlane(iRomanPotId, iPlaneId));

      rpEvt->setNumberOfClusters(iRomanPotId, iPlaneId, collection->numberOfClusters(iRomanPotId, iPlaneId));
      for(UInt_t iCluster=0; iCluster < collection->numberOfClusters(iRomanPotId, iPlaneId); ++iCluster){
        //StUPCRpsCluster *rpCluster = rpPlane->cluster(iCluster); 
        rpEvt->setPosition(iRomanPotId, iPlaneId, collection->positionCluster(iRomanPotId, iPlaneId, iCluster));
        rpEvt->setPositionRMS(iRomanPotId, iPlaneId, collection->positionRMSCluster(iRomanPotId, iPlaneId, iCluster)); 
        rpEvt->setLength(iRomanPotId, iPlaneId, collection->lengthCluster(iRomanPotId, iPlaneId, iCluster)); 
        rpEvt->setEnergy(iRomanPotId, iPlaneId, collection->energyCluster(iRomanPotId, iPlaneId, iCluster)); 
        rpEvt->setXY(iRomanPotId, iPlaneId, collection->xyCluster(iRomanPotId, iPlaneId, iCluster)); 
        rpEvt->setQuality(iRomanPotId, iPlaneId, collection->qualityCluster(iRomanPotId, iPlaneId, iCluster));
       // rpPlane->addCluster(rpCluster); 
      }

      for(Int_t iTrack=0; iTrack < collection->numberOfTracks(); ++iTrack){
			StMuRpsTrack *track = collection->track(iTrack);
			StUPCRpsTrack *rpTrack = rpEvt->addTrack();
			for(Int_t iTrackPoint=0; iTrackPoint < 2; ++iTrackPoint){
				const StMuRpsTrackPoint *trackPoint = track->trackPoint(iTrackPoint); 
				StUPCRpsTrackPoint *rpTrackPoint = rpEvt->addTrackPoint();
				rpTrackPoint->setPosition(trackPoint->positionVec());
				rpTrackPoint->setRpId(trackPoint->rpId());
				rpTrackPoint->setClusterId(trackPoint->clusterId(iPlaneId), iPlaneId);
				rpTrackPoint->setTime(trackPoint->time(0), 0); // pmtId = 0
				rpTrackPoint->setTime(trackPoint->time(1), 1); // pmtId = 1
				switch(trackPoint->quality()){
					case StMuRpsTrackPoint::rpsNormal: rpTrackPoint->setQuality(StUPCRpsTrackPoint::rpsNormal);
					break;      
					case StMuRpsTrackPoint::rpsGolden: rpTrackPoint->setQuality(StUPCRpsTrackPoint::rpsGolden);
					break; 
					default: rpTrackPoint->setQuality(StUPCRpsTrackPoint::rpsNotSet);
					break; 
				}
				rpTrack->setTrackPoint(rpTrackPoint, iTrackPoint);
			}        
			rpTrack->setP(track->pVec()); 
			rpTrack->setBranch(track->branch());
 			switch(track->type()){
				case StMuRpsTrack::rpsLocal: rpTrack->setType(StUPCRpsTrack::rpsLocal);
				break;      
				case StMuRpsTrack::rpsGlobal: rpTrack->setType(StUPCRpsTrack::rpsGlobal);
				break; 
				default: rpTrack->setType(StUPCRpsTrack::rpsUndefined);
				break; 
			} 
      }

    }
  }

}//processEvent

























