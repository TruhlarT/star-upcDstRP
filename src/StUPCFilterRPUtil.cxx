
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
  StUPCRpsCollection *rpCollection = rpEvt->addCollection(); 
  rpCollection->setSiliconBunch(collection->siliconBunch());

  StUPCRpsRomanPot *rpRomanPot = rpEvt->addRomanPot(); 
  for(Int_t iRomanPotId=0; iRomanPotId < collection->numberOfRomanPots(); ++iRomanPotId){

    rpRomanPot->setStatus(collection->status(iRomanPotId));
    rpRomanPot->setAdc(collection->adc(iRomanPotId, 0), collection->adc(iRomanPotId, 1));
    rpRomanPot->setTac(collection->tac(iRomanPotId, 0), collection->tac(iRomanPotId, 1)); 

    for(Int_t iPlaneId=0; iPlaneId < collection->numberOfPlanes(); ++iPlaneId){
      StUPCRpsPlane *rpPlane = rpEvt->addPlane(); 
      rpPlane->setOffset(collection->offsetPlane(iRomanPotId, iPlaneId));
      rpPlane->setZ(collection->zPlane(iRomanPotId, iPlaneId));  
      rpPlane->setAngle(collection->anglePlane(iRomanPotId, iPlaneId));  
      rpPlane->setOrientation(collection->orientationPlane(iRomanPotId, iPlaneId));  
      rpPlane->setStatus(collection->statusPlane(iRomanPotId, iPlaneId));

      for(Int_t iCluster=0; iCluster < collection->numberOfClusters(iRomanPotId, iPlaneId); ++iCluster){
        StUPCRpsCluster *rpCluster = rpEvt->addCluster(); 
        rpCluster->setPosition(collection->positionCluster(iRomanPotId, iPlaneId, iCluster));
        rpCluster->setPositionRMS(collection->positionRMSCluster(iRomanPotId, iPlaneId, iCluster)); 
        rpCluster->setLength(collection->lengthCluster(iRomanPotId, iPlaneId, iCluster)); 
        rpCluster->setEnergy(collection->energyCluster(iRomanPotId, iPlaneId, iCluster)); 
        rpCluster->setXY(collection->xyCluster(iRomanPotId, iPlaneId, iCluster)); 
        rpCluster->setQuality(collection->qualityCluster(iRomanPotId, iPlaneId, iCluster));
        rpPlane->addCluster(rpCluster); 
      }

      for(Int_t iTrack=0; iTrack < collection->numberOfTracks(); ++iTrack){
			StMuRpsTrack *track = collection->track(iTrack);
			StUPCRpsTrack *rpTrack = rpEvt->addTrack();
			for(Int_t iTrackPoint=0; iTrackPoint < 2; ++iTrackPoint){
				const StMuRpsTrackPoint *trackPoint = track->trackPoint(iTrackPoint); // invalid conversion from 'const StMuRpsTrackPoint*' to 'StMuRpsTrackPoint*'
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
			rpCollection->addTrack(rpTrack); 
      }

      for(Int_t iTrackPoint=0; iTrackPoint < collection->numberOfTrackPoints(); ++iTrackPoint){ 
        StMuRpsTrackPoint *trackPoint = collection->trackPoint(iTrackPoint);
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

        rpCollection->addTrackPoint(rpTrackPoint); 
      }
    }
  }

}//processEvent

























