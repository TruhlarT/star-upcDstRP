#include "StUPCRpsPreCollection.h"
#include "StUPCRpsCollection.h"
#include "StUPCRpsRomanPot.h"
#include "StUPCRpsPlane.h"
#include "StUPCRpsCluster.h"
#include <algorithm>


ClassImp(StUPCRpsPreCollection)

StUPCRpsPreCollection::StUPCRpsPreCollection(const StUPCRpsCollection & rps){

    mSiliconBunch = rps.siliconBunch();

	for(int i=0;i<mNumberOfRomanPot;i++){

		mNumberPlanesWithClusters[i]  = rps.romanPot(i)->numberOfPlanesWithClusters();
		mStatusRomanPot[i]   = rps.romanPot(i)->status();
		mADC[i][0] = rps.romanPot(i)->adc(0);
		mADC[i][1] = rps.romanPot(i)->adc(1);
		mTAC[i][0] = rps.romanPot(i)->tac(0);
		mTAC[i][1] = rps.romanPot(i)->tac(1);
	 												
		for(int j=0;j<mNumberOfPlanes;j++){

			mOffsetPlane[i][j]  = rps.romanPot(i)->plane(j)->offset();
			mzPlane[i][j]  = rps.romanPot(i)->plane(j)->z();
			mAnglePlane[i][j]  = rps.romanPot(i)->plane(j)->angle();
			mOrientationPlane[i][j]  = rps.romanPot(i)->plane(j)->orientation();		
			mStatusPlane[i][j]  = rps.romanPot(i)->plane(j)->status();	
			mNumberOfClusters[i][j]  = rps.romanPot(i)->plane(j)->numberOfClusters();
			
			for(int k=0;k<mNumberOfClusters[i][j];k++){
			
				mPositionCluster[i][j].push_back(rps.romanPot(i)->plane(j)->cluster(k)->position());
				mPositionRMSCluster[i][j].push_back(rps.romanPot(i)->plane(j)->cluster(k)->positionRMS());
				mLengthCluster[i][j].push_back(rps.romanPot(i)->plane(j)->cluster(k)->length());
				mEnergyCluster[i][j].push_back(rps.romanPot(i)->plane(j)->cluster(k)->energy());				
				mXYCluster[i][j].push_back(rps.romanPot(i)->plane(j)->cluster(k)->xy());				
				mQualityCluster[i][j].push_back(rps.romanPot(i)->plane(j)->cluster(k)->quality());		

			}
		}
	}

	// Add all of the track points
	int nTrackPoints = rps.trackPoints().size();
	for ( int i = 0; i < nTrackPoints; i++ ){
		addTrackPoint( rps.trackPoints()[i] );
	}

	// Need to mirror over the StRpsTracks
	int nTracks = rps.tracks().size();
	for ( int iTrack = 0; iTrack < nTracks; iTrack++ ){
		StUPCRpsPreTrack * RpsPreTrack = new StUPCRpsPreTrack();

		// Add the track points to the Tracks, if they are already in the collection (and they should be)
		// then they wont be added again
		for ( int iStation = 0; iStation < StUPCRpsPreTrack::mNumberOfStationsInBranch; iStation++ ){
			if ( rps.tracks()[iTrack] && rps.tracks()[iTrack]->trackPoint( iStation ) ){
				StUPCRpsPreTrackPoint * ptp = addTrackPoint( rps.tracks()[iTrack]->trackPoint( iStation ) );
				RpsPreTrack->setTrackPoint( ptp, iStation );
			}
		}
		// Set the Track's attributes
		RpsPreTrack->setP( TVector3( rps.tracks()[iTrack]->pVec().x(), rps.tracks()[iTrack]->pVec().y(), rps.tracks()[iTrack]->pVec().z() ) );
		RpsPreTrack->setBranch( rps.tracks()[iTrack]->branch() );
		RpsPreTrack->setType( (StUPCRpsPreTrack::StRpsPreTrackType)rps.tracks()[iTrack]->type() );

		// add it to collection
		mTracks.push_back( RpsPreTrack );
	}
}



StUPCRpsPreTrackPoint* StUPCRpsPreCollection::addTrackPoint( StUPCRpsTrackPoint * rpsTP ){

	if ( mTrackPointsMap.count( rpsTP ) ){
		return mTrackPointsMap[ rpsTP ]; 
	}

	StUPCRpsPreTrackPoint * RpsPreTrackPoint = new StUPCRpsPreTrackPoint();

	RpsPreTrackPoint->setPosition( TVector3( rpsTP->x(), rpsTP->y(), rpsTP->z() ) );
	RpsPreTrackPoint->setQuality( (StUPCRpsPreTrackPoint::StRpsPreTrackPointQuality)rpsTP->quality() );
	RpsPreTrackPoint->setRpId( rpsTP->rpId() );

	for ( int iPlane = 0; iPlane < StUPCRpsPreTrackPoint::mNumberOfPlanesInRp; iPlane++ ){
		RpsPreTrackPoint->setClusterId( rpsTP->clusterId( iPlane ), iPlane );
	}

	for ( int iPmt = 0; iPmt < StUPCRpsPreTrackPoint::mNumberOfPmtsInRp; iPmt++ ){
		RpsPreTrackPoint->setTime( rpsTP->time( iPmt ), iPmt );
	}

	mTrackPoints.push_back( RpsPreTrackPoint );
	mTrackPointsMap[ rpsTP ] = RpsPreTrackPoint;
	return RpsPreTrackPoint;
}
