// LightVersion, Cut of to minimum


#define rpAnalysis_cxx
#define PI 3.14159265
#include "rpAnalysis.hh"
#include "picoDstDescriptionFiles/particle_event.h"
#include "picoDstDescriptionFiles/StMuRpsCollection.h"
#include "picoDstDescriptionFiles/StMuRpsUtil/StMuRpsCollection2.h"
#include "picoDstDescriptionFiles/StBbcTriggerDetector.h"
#include "picoDstDescriptionFiles/StMuRpsUtil/StMuRpsUtil.h"
#include <TH2.h> 
#include <TF1.h> 
#include <TF2.h> 
#include <THStack.h> 
#include <TStyle.h> 
#include <TGraph.h> 
#include <TGraph2D.h> 
#include <TGraphErrors.h> 
#include <iostream>
#include <utility>
#include <sstream> 
#include <algorithm> 
#include <stdio.h> 
#include <stdlib.h> 
#include <TCanvas.h> 
#include <TLegend.h> 
#include <TGaxis.h> 
#include <vector> 
#include <fstream> 
#include <TString.h> 
#include <TColor.h> 
#include <TLine.h> 
#include <cmath> 
#include <TExec.h> 
#include <TFitResultPtr.h> 
#include <TFitResult.h> 
#include <TLatex.h> 
#include <TMath.h>
#include <TLorentzVector.h>

using namespace std;


// Enumerations - very helpful and convenient !
enum SIDE {E=0, East=0, W=1, West=1, nSides};
enum XY_COORDINATE {X, Y, nCoordinates};
enum TRIGGER_ID {SD, CPT2, SDT, RPZMU, RP2MU, ET, CP, CPT, RP2E, Zerobias, CPX, SDZ, CPEI, ZE, CPTnoBBCL, CPT2noBBCL, nTriggers};
enum RP_ID {E1U, E1D, E2U, E2D, W1U, W1D, W2U, W2D, nRomanPots};
enum BRANCH_ID { EU, ED, WU, WD, nBranches };
enum ARM_ID { EU_WU, ED_WD, nArms };
enum STATION_ID { E1, E2, W1, W2, nStations };
enum RP_CONFIGURATION {EUD, EDU, IUU, IDD, nConfiguration}; // EUD means elastic combination from east up to west down
// EDU means elastic combination from east down to west up
// IUU means inelastic combination from east up to west up
// IDD means inelastic combination from east down to west down






// Constants
const double beamMomentum=100.14;
const double ADCmin = 100;
const double TACmin = 100;
const double TACmax = 2500;
const double PionMass = 0.13957; // GeV /c^2
const double speedOfLifgt = 299792458; // m/s

// Labels, names etc. (defined as TString to gain higher functionality than const char*, e.g. defined "+" operator)
TString triggerName[nTriggers] = { TString("SD"), TString("CPT2"), TString("SDT"), TString("RPZMU"), TString("RP2MU"),
											TString("ET"), TString("CP"), TString("CPT"), TString("RP2E"), TString("Zerobias"),
											TString("CPX"), TString("SDZ"), TString("CPEI"), TString("ZE"), TString("CPTnoBBCL"), 
											TString("CPT2noBBCL") }; /*last two were running only in 2017*/
TString triggerBitLabel[8] = { TString("ET"), TString("IT"), TString("EOR"), TString("WOR"),
										TString("EU"), TString("ED"), TString("WU"), TString("WD") };
TString armName[nArms] = { TString("EU-WU"), TString("ED-WD") };
TString branchName[nBranches] = { TString("EU"), TString("ED"), TString("WU"), TString("WD") };
TString rpName[nRomanPots] = { TString("E1U"), TString("E1D"), TString("E2U"), TString("E2D"),
										TString("W1U"), TString("W1D"), TString("W2U"), TString("W2D") };
TString stationName[nStations] = { TString("E1"), TString("E2"), TString("W1"), TString("W2") };
TString summaryLabels[10] = { TString("CPTnoBBCL"), TString("Elastic"), TString("Inelastic"), TString("1 TPCvertex"), TString("2 TPC tracks"), 
										TString("2 TOF tracks"), TString("TotCharge 0"), TString("MissingPt < 0.1 GeV"), TString(""), TString("")};
TString systemID[3] = { TString("Combi"), TString("InEl"), TString("El")};
TString systemState[4] = { TString("TPCv1"), TString("TOF2trk"), TString("Q0"), TString("Excl")};
TString configLabels[4] = { TString("EUD"), TString("EDU"), TString("IUU"), TString("IDD")};


// Histograms declarations
TH1F* hAnalysisFlow; // control plot
TH1F* hConfiguration;
TH1F* hNumberTrack; // number of track in trigger (RP)
TH1F* hNumberTrackPerBranch[nBranches]; // number of track per each brunch (west up, west down, east up, east down)


// hist[9] =>
// hist[0] = 1 TPC vertex - inelastic + elastic
// hist[1] = totCharge = 0 - inelastic + elastic
// hist[2] = missingPt < 0.1 - inelastic + elastic
// hist[3] = 1 TPC vertex - inelastic
// hist[4] = totCharge = 0 - inelastic
// hist[5] = missingPt < 0.1 - inelastic
// hist[6] = 1 TPC vertex - elastic
// hist[7] = totCharge = 0 - elastic
// hist[8] = missingPt < 0.1 - elastic

// PID
TH2D* hdEdxVsP[12];
TH2D* hdEdxVsqP[12];

TH1D* hNSigmaPion[12];

TH1D* hNSigmaPionPair[12];


TH1F* hMissingPt[12]; 
TH1F* hInvMass[12];

TH2F* hXYCorrelations[12];
TH1D* hZvertex[12];


void rpAnalysis::Begin(TTree * tree){

// reading options (trigger name, jobId, any other...)
	readOptions();
// setting the trigger to be analyzed (following the arguments parsed to the program)
	setAnalyzedTrigger();
// creating ROOT file which will contain all the histograms created afterwards
	fHistFile = new TFile(outputDirStr+"/analysisOutput."+jobIdStr+"."+triggerName[analyzedTriggerId]+".root", "RECREATE"); 

// creating histograms
// Having defined labels/names it is very easy and fast to create multiple histograms differing only by name
// (e.g. track multiplicity in single branch, theta angle for each arm and coordinate, etc.)
	hAnalysisFlow = new TH1F("AnalysisFlow", "CutsFlow", 10, -0.5, 9.5);
	for(int tb=0; tb<10; ++tb) hAnalysisFlow->GetXaxis()->SetBinLabel(tb+1, summaryLabels[tb]);

	hConfiguration = new TH1F("Configuration", "Track Configuration", 4, -0.5, 3.5);
	for(int tb=0; tb<4; ++tb) hConfiguration->GetXaxis()->SetBinLabel(tb+1, configLabels[tb]);

	for(int i=0; i<nBranches; ++i)
		hNumberTrackPerBranch[i] = new TH1F("NumberTracksPerBranch,"+branchName[i],"Number of tracks in branch "+branchName[i], 8, -0.5, 7.5);
	hNumberTrack = new TH1F("NumberTracks", "Number of Tracks in RP, trigger "+triggerName[analyzedTriggerId], 40, -0.5, 39.5);


	for(int i=0; i<12;++i){
		if(i==0){
			fHistFile->mkdir("All")->cd();
		}else	if(i==4){
			fHistFile->cd();
			fHistFile->mkdir("Inelastic")->cd();
		}else if(i==8){
			fHistFile->cd();
			fHistFile->mkdir("Elastic")->cd();
		}
		
		hdEdxVsP[i] = new TH2D("dEdxVsP_"+systemState[i%4]+"_"+systemID[i/4],"dE/dx Vs P",100,0,2,100,0,20 );
		hdEdxVsqP[i] = new TH2D("dEdxVsqP_"+systemState[i%4]+"_"+systemID[i/4],"dE/dx Vs #frac{q}{e} P",200,-2,2,100,0,20);

		hNSigmaPion[i] = new TH1D("NSigmaPion_"+systemState[i%4]+"_"+systemID[i/4],"NSigmaPion",100,-10,10);
		hNSigmaPionPair[i] = new TH1D("NSigmaPionPair_"+systemState[i%4]+"_"+systemID[i/4],"NSigmaPionPair",100,0,100);

		hMissingPt[i] = new TH1F( "MissingPt_"+systemState[i%4]+"_"+systemID[i/4], "p^{miss}_{T} [GeV/c]", 200, 0, 2 );
		hInvMass[i] = new TH1F( "InvMass_"+systemState[i%4]+"_"+systemID[i/4],"M_{#pi#pi} [GeV/c^{2}]", 100, 0, 5 );
		hXYCorrelations[i] = new TH2F("Py_vs_Px_"+systemState[i%4]+"_"+systemID[i/4], "Y coordinate vs. X coordinate of proton momentum", 250, -3, 3, 250, -3, 3);
		hZvertex[i] = new TH1D("Zvertex_"+systemState[i%4]+"_"+systemID[i/4],"Z coordinate of vertex in TPC",100,-200,200);
	}
	fHistFile->cd();
}


void rpAnalysis::SlaveBegin(TTree * /*tree*/){
// afterburner = new StMuRpsUtil(&particle_events);
}





Bool_t rpAnalysis::Process(Long64_t entry){
// Loading an event
	fChain->GetTree()->GetEntry(entry);
// Filling contror plot
	hAnalysisFlow->Fill(0);
// Checking if event was triggered by analyzed trigger, if not - skip to next event
	if( analyzedTriggerId != -1 )
		if( !isTrigger(analyzedTriggerId) ) return kTRUE;


// Obtaining pointer to RP data collection, BBC data collection, etc...
	StMuRpsCollection *rps = &(particle_events->mRpsCollection);
/* afterburner->updateVertex( 0.000415, 0.000455, 0.000 );  // vertex position used in angles reconstruction
StMuRpsCollection2 *rps = afterburner->process(); */
	StBbcTriggerDetector *bbc = &(particle_events->mBbcTriggerDetector);


	// Vector below will be filled with indices of good-quality tracks
	vector<int> rpTrackIdVec_perBranch[nBranches];
	vector<int> rpTrackIdVec_perSide[nSides];
	int numberOfTracks = 0;
	int numberOfTracksPerBranch[nBranches] = {0, 0, 0, 0};

	// Loop over all tracks reconstructed in Roman Pots  
	for(int k=0; k<rps->numberOfTracks(); ++k){
	// Get pointer to k-th track in Roman Pot data collection
		StMuRpsTrack *trk = rps->track(k);
	// Get ID of a branch in which this k-th track was reconstructed
		int j = trk->branch();
		int side = j<2 ? E : W;
		++numberOfTracks;
		++numberOfTracksPerBranch[j];
	// If track is global (made of track-points in 1st and 2nd station)
	// and all 8 planes were used in reconstruction - store its ID
		if( trk->type()==StMuRpsTrack::rpsGlobal && trk->planesUsed()==8) rpTrackIdVec_perBranch[j].push_back( k );
	// a bit looser selection
		if( (trk->trackPoint(0) ? trk->trackPoint(0)->planesUsed()>=3 : true) &&
			(trk->trackPoint(1) ? trk->trackPoint(1)->planesUsed()>=3 : true) ) rpTrackIdVec_perSide[side].push_back( k );
	}

	// Fill histograms with number of tracks
	hNumberTrack->Fill(numberOfTracks);
	for(int i=0; i<nBranches; i++) hNumberTrackPerBranch[i]->Fill(numberOfTracksPerBranch[i]);

/////////////////////////////////////////////////////////////////////////
//							START OF THE ANALYSIS FLOW 				  			  //
/////////////////////////////////////////////////////////////////////////
// Loop over arms - check if have good-quality tracks, selecting branch combination
	for(int i=0; i<nConfiguration; ++i){ 
// Define IDs of branches based on known ID of the arm
		int branch[nSides];
		switch(i) {
    		case EUD :{
    			branch[E] = EU;
    			branch[W] = WD;
    		}break;
    		case EDU :{
    			branch[E] = ED;
    			branch[W] = WU;
    		}break; 
    		case IUU :{
    			branch[E] = EU;
    			branch[W] = WU;
    		}break; 
    		case IDD :{
    			branch[E] = ED;
    			branch[W] = WD;
    		}break;        
		}
		int otherBranch[nSides];
		otherBranch[E] = branch[E]==EU ? ED : EU;
		otherBranch[W] = branch[W]==WU ? WD : WU;

// If exactly one good-quality track in each branch in the arm and there is no track in the other RP- do some staff
		if( rpTrackIdVec_perBranch[ branch[E] ].size()==1 
			&& rpTrackIdVec_perBranch[ branch[W] ].size()==1 
			&& rpTrackIdVec_perBranch[ otherBranch[E] ].size()==0 
			&& rpTrackIdVec_perBranch[ otherBranch[W] ].size()==0){
	// Get pointers to good-quality tracks
			if(i==EUD || i==EDU){
				hAnalysisFlow->Fill(1);
			}else{
				hAnalysisFlow->Fill(2);
			}
			hConfiguration->Fill(i);

			const unsigned int nTpcVertex = particle_events->getNumVertex();
			if(nTpcVertex != 1) 
				return kTRUE;
			hAnalysisFlow->Fill(3);

			const vector<st_track>* tpcTrkData = &(particle_events->getTrack());
			const vector<st_vertex>* tpcVertex = &(particle_events->getVertex());
			const st_vertex* vertex = &((*tpcVertex)[0]);

			const double vertexZ = vertex->mPosition.Z();

			int totalCharge = 0;
			int nTofTrks = 0;
			int nTpcTrks = 0;
			double nSigmaPairPion2 = 0;
			double nSigPPion;
			double trkInfo[2][6];
			for(int tr = 0; tr < 2;++tr)
				for(int tc = 0; tc<6;++tc)
					trkInfo[tr][tc]=-1000;
			TLorentzVector centralTracksTotalFourMomentum;


		// loop over all TPC tracks
			for(unsigned int j=0; j<tpcTrkData->size(); ++j){
		// get TPC track object
				const st_track* trk = &((*tpcTrkData)[j]);

				if( static_cast<int>(trk->mTOFMatchFlag) > 9 ) continue; // skip global tracks, analyze only primary
				// read basic information about the track
				++nTpcTrks;			

				const bool matchedWithTof = trk->mGotTOF;
				if( matchedWithTof){	
					int state = 0;
					for(int tmp =0; tmp<3;++tmp){
						if(tmp == 1 && (i!=IUU && i!=IDD))
							continue;
						if(tmp == 2 && (i!=EUD && i!=EDU))
							continue;
						hdEdxVsP[4*tmp +state]->Fill(trk->mP.P(),trk->mdEdx*1000000);
						hdEdxVsqP[4*tmp+state]->Fill(trk->mP.P()*trk->mCharge,trk->mdEdx*1000000);
						hNSigmaPion[4*tmp+state]->Fill(trk->mNSigmaPion);
					}	

					if(nTofTrks < 2){
						trkInfo[nTofTrks][0] = trk->mdEdx*1000000;
						trkInfo[nTofTrks][1] = trk->mP.P();
						trkInfo[nTofTrks][2] = trk->mCharge;
						trkInfo[nTofTrks][3] = trk->mNSigmaPion;
					}
					totalCharge += static_cast<int>( trk->mCharge );
					centralTracksTotalFourMomentum += trk->mP;
					nSigmaPairPion2 += pow(trk->mNSigmaPion,2);
					++nTofTrks;
				}

			}

			const double missingPt = (centralTracksTotalFourMomentum.Vect() + rps->track( rpTrackIdVec_perSide[E][0] )->pVec() + rps->track( rpTrackIdVec_perSide[W][0] )->pVec() ).Pt();
			const double invMass = centralTracksTotalFourMomentum.M();

			nSigPPion = sqrt(nSigmaPairPion2);

			int state = 0;
			for(int tmp =0; tmp<3;++tmp){
				if(tmp == 1 && (i!=IUU && i!=IDD))
					continue;
				if(tmp == 2 && (i!=EUD && i!=EDU))
					continue;
				hMissingPt[4*tmp+state]->Fill(missingPt); 
				if(nSigPPion < 3 )
					hInvMass[4*tmp+state]->Fill(invMass);
				for(int j = 0; j < nSides;++j)
					hXYCorrelations[4*tmp+state]->Fill(rps->track(rpTrackIdVec_perSide[j][0])->pVec().X(),rps->track(rpTrackIdVec_perSide[j][0])->pVec().Y());
				hZvertex[4*tmp+state]->Fill(vertexZ);
			}	


			if( nTpcTrks!=2)
				return kTRUE;
			hAnalysisFlow->Fill(4);

			if( nTofTrks!=2)
				return kTRUE;
			hAnalysisFlow->Fill(5);

			state = 1;
			for(int tmp =0; tmp<3;++tmp){
				if(tmp == 1 && (i!=IUU && i!=IDD))
					continue;
				if(tmp == 2 && (i!=EUD && i!=EDU))
					continue;
				for(int var=0;var<2;++var){
					hdEdxVsP[4*tmp +state]->Fill(trkInfo[var][1],trkInfo[var][0]);
					hdEdxVsqP[4*tmp+state]->Fill(trkInfo[var][1]*trkInfo[var][2],trkInfo[var][0]);
					hNSigmaPion[4*tmp+state]->Fill(trkInfo[var][3]);
				}


				hMissingPt[4*tmp+state]->Fill(missingPt); 
				if(nSigPPion < 3 )
					hInvMass[4*tmp+state]->Fill(invMass);
				for(int j = 0; j < nSides;++j)
					hXYCorrelations[4*tmp+state]->Fill(rps->track(rpTrackIdVec_perSide[j][0])->pVec().X(),rps->track(rpTrackIdVec_perSide[j][0])->pVec().Y());
				hZvertex[4*tmp+state]->Fill(vertexZ);
			}


			if(totalCharge != 0) 
				return kTRUE;
			hAnalysisFlow->Fill(6);

			state = 2;
			for(int tmp =0; tmp<3;++tmp){
				if(tmp == 1 && (i!=IUU && i!=IDD))
					continue;
				if(tmp == 2 && (i!=EUD && i!=EDU))
					continue;
				for(int var=0;var<2;++var){
					hdEdxVsP[4*tmp +state]->Fill(trkInfo[var][1],trkInfo[var][0]);
					hdEdxVsqP[4*tmp+state]->Fill(trkInfo[var][1]*trkInfo[var][2],trkInfo[var][0]);
					hNSigmaPion[4*tmp+state]->Fill(trkInfo[var][3]);
				}


				hMissingPt[4*tmp+state]->Fill(missingPt);
				if(nSigPPion < 3 )
					hInvMass[4*tmp+state]->Fill(invMass);
				for(int j = 0; j < nSides;++j)
					hXYCorrelations[4*tmp+state]->Fill(rps->track(rpTrackIdVec_perSide[j][0])->pVec().X(),rps->track(rpTrackIdVec_perSide[j][0])->pVec().Y());
				hZvertex[4*tmp+state]->Fill(vertexZ);
			}

			if( missingPt > 0.1 )
				return kTRUE;
			hAnalysisFlow->Fill(7);


			state = 3;
			for(int tmp =0; tmp<3;++tmp){
				if(tmp == 1 && (i!=IUU && i!=IDD))
					continue;
				if(tmp == 2 && (i!=EUD && i!=EDU))
					continue;
				for(int var=0;var<2;++var){
					hdEdxVsP[4*tmp +state]->Fill(trkInfo[var][1],trkInfo[var][0]);
					hdEdxVsqP[4*tmp+state]->Fill(trkInfo[var][1]*trkInfo[var][2],trkInfo[var][0]);
					hNSigmaPion[4*tmp+state]->Fill(trkInfo[var][3]);
				}

				hNSigmaPionPair[4*tmp+state]->Fill(nSigPPion);

				hMissingPt[4*tmp+state]->Fill(missingPt);
				if(nSigPPion < 3 )
					hInvMass[4*tmp+state]->Fill(invMass);
				for(int j = 0; j < nSides;++j)
					hXYCorrelations[4*tmp+state]->Fill(rps->track(rpTrackIdVec_perSide[j][0])->pVec().X(),rps->track(rpTrackIdVec_perSide[j][0])->pVec().Y());
				hZvertex[4*tmp+state]->Fill(vertexZ);
			}

		}
	} // end of loop over arms

	// afterburner->clear();
	return kTRUE;
}




void rpAnalysis::SlaveTerminate(){  
}


void rpAnalysis::Terminate(){
	fHistFile->Write();
	fHistFile->Close();
}


// --- below only users methods --- 

bool rpAnalysis::smallBbcFired(StBbcTriggerDetector *bbc, int *nSegmentsWithSignal) const{
	int nFired = 0;
	for(unsigned int i=0; i<bbc->numberOfPMTs(); ++i){
		if( !isLargeBbc(i) ){
			if( bbc->tdc(i) > 100 && bbc->tdc(i) < 2400 && bbc->adc(i) > 20 ) ++nFired;
		}
	}
	if(nSegmentsWithSignal!=NULL) *nSegmentsWithSignal = nFired;
	if(nFired>0) return true; else return false;
}

bool rpAnalysis::largeBbcFired(StBbcTriggerDetector *bbc, int *nSegmentsWithSignal, double thresholdADC) const{
	int nFired = 0;
	for(unsigned int i=0; i<bbc->numberOfPMTs(); ++i){
		if( isLargeBbc(i) ){
			if( bbc->tdc(i) > 100 && bbc->adc(i) > thresholdADC ) ++nFired;
		}
	}
	if(nSegmentsWithSignal!=NULL) *nSegmentsWithSignal = nFired;
	if(nFired>0) return true; else return false;
}

inline bool rpAnalysis::isLargeBbc(int tileId) const{
	return  ((tileId>=16&&tileId<24) || tileId>=40 );
}



void rpAnalysis::readOptions(){
	TString opt = GetOption();
	TObjArray *tokenizedOption = opt.Tokenize("|");
	for(Int_t i = 0; i < tokenizedOption->GetEntries(); ++i){
		TString option = ((TObjString *)(tokenizedOption->At(i)))->String();
		if(i==0) analyzedTriggerStr = option;
		else if(i==1) jobIdStr = option;
		else if(i==2) outputDirStr = option;
		else optionVec.push_back( option );
	}
}

bool rpAnalysis::isOption(TString opt) const{
	for(unsigned int i=0; i<optionVec.size(); ++i) if(optionVec[i]==opt) return true;
	return false;
}

void rpAnalysis::setAnalyzedTrigger(){
	analyzedTriggerId = -1;
	for(int i=0; i<nTriggers; ++i){
		if(triggerName[i]==analyzedTriggerStr){
			analyzedTriggerId = i;
			return;
		}
	}
	cerr << "Error in rpAnalysis::setAnalyzedTrigger(): no trigger \"" << analyzedTriggerStr << "\" has been found." << endl;
	cerr << "This will result in analysis of all events contained in the file(s) selected for analysis." << endl;
}

inline bool rpAnalysis::isTrigger(int trigId) const{
	return particle_events->getTrigger(trigId);
}

