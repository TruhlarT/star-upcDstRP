// c++ headers
#include <iostream>

// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
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

// picoDst headers
#include "StRPEvent.h"
#include "StUPCRpsTrack.h"
#include "StUPCRpsTrackPoint.h"

#include "StUPCEvent.h"
#include "StUPCTrack.h"
#include "StUPCBemcCluster.h"
#include "StUPCVertex.h"

using namespace std;


// Enumerations - very helpful and convenient !
enum SIDE {E=0, East=0, W=1, West=1, nSides};
enum XY_COORDINATE {X, Y, nCoordinates};
enum RP_ID {E1U, E1D, E2U, E2D, W1U, W1D, W2U, W2D, nRomanPots};
enum BRANCH_ID { EU, ED, WU, WD, nBranches };
enum ARM_ID { EU_WU, ED_WD, nArms };
enum STATION_ID { E1, E2, W1, W2, nStations };
enum RP_CONFIGURATION {EUD, EDU, IUU, IDD, nConfiguration}; // EUD means elastic combination from east up to west down
// EDU means elastic combination from east down to west up
// IUU means inelastic combination from east up to west up
// IDD means inelastic combination from east down to west down


// Constants
const double ADCmin = 100;
const double TACmin = 100;
const double TACmax = 2500;

TString armName[nArms] = { TString("EU-WU"), TString("ED-WD") };
TString branchName[nBranches] = { TString("EU"), TString("ED"), TString("WU"), TString("WD") };
TString rpName[nRomanPots] = { TString("E1U"), TString("E1D"), TString("E2U"), TString("E2D"),
                    TString("W1U"), TString("W1D"), TString("W2U"), TString("W2D") };
TString stationName[nStations] = { TString("E1"), TString("E2"), TString("W1"), TString("W2") };
TString summaryLabels[10] = { TString("After triggers"), TString("Elastic"), TString("Inelastic"), TString("2 TPC prim trks"), TString("2 TOF tracks"), 
                    TString("Same vertex"), TString("TotCharge 0"), TString("MissingPt < 0.1 GeV"), TString(""), TString("")};
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

TFile* outfile; 

void begin(){


// creating ROOT file which will contain all the histograms created afterwards
  outfile = new TFile("test.root", "recreate"); // outfile = TFile::Open("output.root", "recreate");
  
// creating histograms
// Having defined labels/names it is very easy and fast to create multiple histograms differing only by name
// (e.g. track multiplicity in single branch, theta angle for each arm and coordinate, etc.)
  hAnalysisFlow = new TH1F("AnalysisFlow", "CutsFlow", 10, -0.5, 9.5);
  for(int tb=0; tb<10; ++tb) hAnalysisFlow->GetXaxis()->SetBinLabel(tb+1, summaryLabels[tb]);

  hConfiguration = new TH1F("Configuration", "Track Configuration", 4, -0.5, 3.5);
  for(int tb=0; tb<4; ++tb) hConfiguration->GetXaxis()->SetBinLabel(tb+1, configLabels[tb]);

  for(int i=0; i<nBranches; ++i)
    hNumberTrackPerBranch[i] = new TH1F("NumberTracksPerBranch,"+branchName[i],"Number of tracks in branch "+branchName[i], 8, -0.5, 7.5);
  hNumberTrack = new TH1F("NumberTracks", "Number of Tracks in RP, trigger ", 40, -0.5, 39.5);


  for(int i=0; i<12;++i){
    if(i==0){
      outfile->mkdir("All")->cd();
    }else if(i==4){
      outfile->cd();
      outfile->mkdir("Inelastic")->cd();
    }else if(i==8){
      outfile->cd();
      outfile->mkdir("Elastic")->cd();
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
  outfile->cd();
}

void writeAndClose(){
  outfile->Write(); 
  outfile->Close();
}

void porcces( StRPEvent *rpEvt, StUPCEvent *upcEvt){
  // Vector below will be filled with indices of good-quality tracks
  vector<int> rpTrackIdVec_perBranch[nBranches];
  vector<int> rpTrackIdVec_perSide[nSides];
  int numberOfTracks = 0;
  int numberOfTracksPerBranch[nBranches] = {0, 0, 0, 0};

  // Loop over all tracks reconstructed in Roman Pots  
  for(int k=0; k<rpEvt->getNumberOfTracks(); ++k){
  // Get pointer to k-th track in Roman Pot data collection
    StUPCRpsTrack *trk =  rpEvt->getTrack(k);
  // Get ID of a branch in which this k-th track was reconstructed
    int j = trk->branch();
    int side = j<2 ? E : W;
    ++numberOfTracks;
    ++numberOfTracksPerBranch[j];
  // If track is global (made of track-points in 1st and 2nd station)
  // and all 8 planes were used in reconstruction - store its ID
    if( trk->type()==StUPCRpsTrack::rpsGlobal && trk->planesUsed()==8) rpTrackIdVec_perBranch[j].push_back( k );
  // a bit looser selection
    if( (trk->trackPoint(0) ? trk->trackPoint(0)->planesUsed()>=3 : true) &&
      (trk->trackPoint(1) ? trk->trackPoint(1)->planesUsed()>=3 : true) ) rpTrackIdVec_perSide[side].push_back( k );
  }
  
  // Fill histograms with number of tracks
  hNumberTrack->Fill(numberOfTracks);
  for(int i=0; i<nBranches; i++) hNumberTrackPerBranch[i]->Fill(numberOfTracksPerBranch[i]);

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
/*
      const unsigned int nTpcVertex = upcEvt->getNumberOfVertices();
      if(nTpcVertex != 1) 
        return;
      hAnalysisFlow->Fill(3);
      const StUPCVertex* vertex = upcEvt->getVertex(0);
      const double vertexZ = vertex->getPosZ();
*/
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

      if(upcEvt->getNPrimTracks()!=2)
        return;
      hAnalysisFlow->Fill(3);

    // loop over all TPC tracks
      for(int j=0; j<upcEvt->getNumberOfTracks(); ++j){
    // get TPC track object
        const StUPCTrack* trk = upcEvt->getTrack(j);

       if( !trk->getFlag(StUPCTrack::kPrimary) ) continue; // skip global tracks, analyze only primary
        // read basic information about the track    
        TLorentzVector trkMomentum;
        trk->getLorentzVector(trkMomentum, 0.138);
        if( !trk->getFlag(StUPCTrack::kTof))
          return;
 
          int state = 0;
          for(int tmp =0; tmp<3;++tmp){
            if(tmp == 1 && (i!=IUU && i!=IDD))
              continue;
            if(tmp == 2 && (i!=EUD && i!=EDU))
              continue;
            hdEdxVsP[4*tmp +state]->Fill(trkMomentum.P(),trk->getDEdxSignal()*1000000);
            hdEdxVsqP[4*tmp+state]->Fill(trkMomentum.P()*trk->getCharge(),trk->getDEdxSignal()*1000000);
            hNSigmaPion[4*tmp+state]->Fill(trk->getNSigmasTPCPion());
          } 

          if(nTofTrks < 2){
            trkInfo[nTofTrks][0] = trk->getDEdxSignal()*1000000;
            trkInfo[nTofTrks][1] = trkMomentum.P();
            trkInfo[nTofTrks][2] = trk->getCharge();
            trkInfo[nTofTrks][3] = trk->getNSigmasTPCPion();
            trkInfo[nTofTrks][4] = trk->getVertexId();
          }
          totalCharge += static_cast<int>( trk->getCharge() );
          centralTracksTotalFourMomentum += trkMomentum;
          nSigmaPairPion2 += pow(trk->getNSigmasTPCPion(),2);
          ++nTofTrks;

      }

      const double missingPt = (centralTracksTotalFourMomentum.Vect() + rpEvt->getTrack( rpTrackIdVec_perSide[E][0] )->pVec() + rpEvt->getTrack( rpTrackIdVec_perSide[W][0] )->pVec() ).Pt();
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
          hXYCorrelations[4*tmp+state]->Fill(rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().X(),rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().Y());
     //   hZvertex[4*tmp+state]->Fill(vertexZ);
      } 

      if( nTofTrks!=2)
        return;
      hAnalysisFlow->Fill(4);

      if( trkInfo[0][4] != trkInfo[1][4])
        return;
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
        hNSigmaPionPair[4*tmp+state]->Fill(nSigPPion);

        hMissingPt[4*tmp+state]->Fill(missingPt); 
        if(nSigPPion < 3 )
          hInvMass[4*tmp+state]->Fill(invMass);
        for(int j = 0; j < nSides;++j)
          hXYCorrelations[4*tmp+state]->Fill(rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().X(),rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().Y());
    //    hZvertex[4*tmp+state]->Fill(vertexZ);
      }


      if(totalCharge != 0) 
        return;
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
        hNSigmaPionPair[4*tmp+state]->Fill(nSigPPion);

        hMissingPt[4*tmp+state]->Fill(missingPt);
        if(nSigPPion < 3 )
          hInvMass[4*tmp+state]->Fill(invMass);
        for(int j = 0; j < nSides;++j)
          hXYCorrelations[4*tmp+state]->Fill(rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().X(),rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().Y());
     //   hZvertex[4*tmp+state]->Fill(vertexZ);
      }

      if( missingPt > 0.1 )
        return;
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
          hXYCorrelations[4*tmp+state]->Fill(rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().X(),rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().Y());
       // hZvertex[4*tmp+state]->Fill(vertexZ);
      }

    }
  } // end of loop over arms
}

int main(){
  begin();
  for(int iFile = 0; iFile < 4; ++iFile){
  //for(int iFile = 0; iFile < 1; ++iFile){
    //open input file
    TString fileName; 
    fileName.Form("/gpfs01/star/pwg/truhlar/Final/CPtrig/merge_files/StUPCRP_production_000%d.root",iFile);
    //fileName.Form("/gpfs01/star/pwg/truhlar/star-upcDst/final/2D251B7815ED928A46D588DBB2165B4A_7.root",iFile);
    cout<<"Start proccesing file: "<<fileName<<endl;
    TFile *infile = TFile::Open(fileName, "read");

    //get picoDst tree in file
    TTree *rpTree = dynamic_cast<TTree*>( infile->Get("mRPTree") );
    TTree *upcTree = dynamic_cast<TTree*>( infile->Get("mUPCTree") );

    //connect upc event to the tree
    static StRPEvent *rpEvt = 0x0;
    rpTree->SetBranchAddress("mRPEvent", &rpEvt);
    static StUPCEvent *upcEvt = 0x0;
    upcTree->SetBranchAddress("mUPCEvent", &upcEvt);

    //ask for number of events
    Long64_t nev = upcTree->GetEntries();
    cout << "Number of events: " << nev << endl;

    //event loop
    for(Long64_t iev=0; iev<nev; iev++) { //get the event

      upcTree->GetEntry(iev); 
      rpTree->GetEntry(iev);  
      hAnalysisFlow->Fill(0);
      porcces(rpEvt,upcEvt);
    }
  }

  writeAndClose();
  cout<<"Ending. Goodbye!"<<endl;
}