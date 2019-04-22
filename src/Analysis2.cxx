// Run: ./Analysis input index
// e.g. ./Analysis /gpfs01/star/pwg/truhlar/Final/CPtrig/merge_files/StUPCRP_production.list -1



// c++ headers
#include <iostream>
#include <utility>
#include <sstream> 
#include <algorithm> 
#include <stdio.h> 
#include <stdlib.h> 
#include <vector> 
#include <fstream> 
#include <cmath> 
#include <cstdlib>

// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include <TH2.h> 
#include <TF1.h> 
#include <TF2.h> 
#include <THStack.h> 
#include <TStyle.h> 
#include <TGraph.h> 
#include <TGraph2D.h> 
#include <TGraphErrors.h> 
#include <TCanvas.h> 
#include <TLegend.h> 
#include <TGaxis.h> 
#include <TString.h> 
#include <TColor.h> 
#include <TLine.h> 
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

enum {kCPtrig=1, kEl, kInEl, kTPC2t, kTOF2t, kSameVrtx, kTotCH0, kMissPt, kMaxCount};
enum SIDE {E=0, East=0, W=1, West=1, nSides};
enum XY_COORDINATE {X, Y, nCoordinates};
enum RP_ID {E1U, E1D, E2U, E2D, W1U, W1D, W2U, W2D, nRomanPots};
enum BRANCH_ID { EU, ED, WU, WD, nBranches };
enum ARM_ID { EU_WU, ED_WD, nArms };
enum STATION_ID { E1, E2, W1, W2, nStations };
enum RP_CONFIGURATION {EUD, EDU, IUU, IDD, nConfiguration}; 
// EUD means elastic combination from east up to west down
// EDU means elastic combination from east down to west up
// IUU means inelastic combination from east up to west up
// IDD means inelastic combination from east down to west down

const double pionMass = 0.138;
const int nTriggers = 21;
const int triggerID[] = {1,2,3,4,5,8,9,570701,570702,570703,
570704,570705,570709,570711,570712,570719,590701,590703,590705,590708,590709};

TString triggerName[nTriggers] = {  TString("CPT2_test"), // 1
                        TString("CPT2noBBCL_test"), // 2
                        TString("SDT_test"), // 3
                        TString("ET_test"), // 4
                        TString("CPTnoBBCL_test"), // 5
                        TString("IT_PREPOST_test"), // 8
                        TString("ET_test"), // 9
                        TString("CPT2"),  // 570701
                        TString("UPC"), // 570702
                        TString("SDT"), // 570703
                        TString("Zerobias"), // 570704
                        TString("CPT2noBBCL"), // 570705
                        TString("ET"), // 570709
                        TString("CPT2"), // 570711
                        TString("UPC"), // 570712
                        TString("ET"), // 570719
                        TString("CPT2"), // 590701
                        TString("SDT"), // 590703
                        TString("CPT2noBBCL"), // 590705
                        TString("CPTnoBBCL"), // 590708
                        TString("ET")}; // 590709

// Histograms declarations
TH1F* hTriggerBits; // control plot

TFile *infile;
TFile *outfile;
TChain *upcChain;
TChain *rpChain;
StUPCEvent *upcEvt;
StRPEvent *rpEvt;

TTree *upcTree;
TTree *rpTree;

TTree *recTree;
Int_t nTracks;
Int_t nTracksEU, nTracksED, nTracksWU, nTracksWD;
Int_t iConfiguration;
Int_t totalCharge;
Int_t nTofTrks;
Int_t nTpcTrks;
Double_t missingPt; 
Double_t invMass;
Double_t dEdx0,dEdx1;
Double_t momentum0,momentum1;
Double_t charge0,charge1;
Double_t nSigmaTPCPion0,nSigmaTPCPion1;
Double_t vexterId0,vexterId1;
Double_t nSigPPion;
Double_t vertexZ;
Double_t xCorrelationsE,xCorrelationsW;
Double_t yCorrelationsE,yCorrelationsW;


void Init();
void Make();
void FillPlots(int state, int configuration );
TFile *CreateOutputTree(const string& out);
bool ConnectInput(const string& in, int fileId);

//_____________________________________________________________________________
int main(int argc, char** argv) {
  //open output file
  outfile = CreateOutputTree("AnalysisOutput.root"); 
  if(!outfile) {cout << "Can not open output file." << endl; return -1;}

  Init();


  int fileId = atoi(argv[2]);
  cout<<fileId<<" / "<<argv[2]<<endl;
  if(!ConnectInput(argv[1], fileId)){
  //if(!ConnectInput("/gpfs01/star/pwg/truhlar/Final/CPtrig/merge_files/StUPCRP_production_0000.root")){
    cout << "No input." << endl; 
    return 1;
  }

  //ask for number of events
  Long64_t nev = upcTree->GetEntries();
  cout << "Number of UPC events: " << nev <<" RP: "<<rpTree->GetEntries() <<endl;
  //event loop
  //nev = 1000;
  for(Long64_t iev=0; iev<nev; ++iev) { //get the event
    upcTree->GetEntry(iev); 
    rpTree->GetEntry(iev); 
    Make();
    recTree->Fill();
  } 

  //close the outputs
  outfile->Write();
  outfile->Close();
  cout<<"Ending Analysis... GOOD BYE!"<<endl;
  return 0;
}//main

void Init(){
  hTriggerBits = new TH1F("TriggerBits", "TriggerBits", nTriggers, -0.5, 20.5);
  for(int tb=0; tb<nTriggers; ++tb) hTriggerBits->GetXaxis()->SetBinLabel(tb+1, triggerName[tb]);
}

void Make(){
  for(int var = 0; var < nTriggers; ++var){
    if(upcEvt->isTrigger(triggerID[var])){
      hTriggerBits->Fill(var);
    }
  }
  nTracks = (Int_t) rpEvt->getNumberOfTracks();
  // Vector below will be filled with indices of good-quality tracks
  vector<int> rpTrackIdVec_perBranch[nBranches];
  vector<int> rpTrackIdVec_perSide[nSides];

  int numberOfTracksPerBranch[nBranches] = {0, 0, 0, 0};

  // Loop over all tracks reconstructed in Roman Pots  
  for(int k=0; k<rpEvt->getNumberOfTracks(); ++k){
  // Get pointer to k-th track in Roman Pot data collection
    StUPCRpsTrack *trk = rpEvt->getTrack(k);

  // Get ID of a branch in which this k-th track was reconstructed
    int j = trk->branch();
    int side = j<2 ? E : W;

    ++numberOfTracksPerBranch[j];

  // If track is global (made of track-points   in 1st and 2nd station)
  // and all 8 planes were used in reconstruction - store its ID
    if( trk->type()==StUPCRpsTrack::rpsGlobal && trk->planesUsed()==8) rpTrackIdVec_perBranch[j].push_back( k );
  // a bit looser selection
    if( (trk->trackPoint(0) ? trk->trackPoint(0)->planesUsed()>=3 : true) &&
      (trk->trackPoint(1) ? trk->trackPoint(1)->planesUsed()>=3 : true) ) rpTrackIdVec_perSide[side].push_back( k );
  }

  nTracksEU = numberOfTracksPerBranch[0];
  nTracksED = numberOfTracksPerBranch[1];
  nTracksWU = numberOfTracksPerBranch[2];
  nTracksWD = numberOfTracksPerBranch[3];

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

// If exactly one good-quality track in each branch in the arm and there is no track in the other RP- do some staff
    if( rpTrackIdVec_perBranch[ branch[E] ].size()==1 
      && rpTrackIdVec_perBranch[ branch[W] ].size()==1
      && rpTrackIdVec_perSide[E].size()==1 && rpTrackIdVec_perSide[W].size()==1){
  // Get pointers to good-quality tracks
      iConfiguration = i;

      xCorrelationsE = rpEvt->getTrack(rpTrackIdVec_perSide[0][0])->pVec().X();
      yCorrelationsE = rpEvt->getTrack(rpTrackIdVec_perSide[0][0])->pVec().Y();
      xCorrelationsW = rpEvt->getTrack(rpTrackIdVec_perSide[1][0])->pVec().X();
      yCorrelationsW = rpEvt->getTrack(rpTrackIdVec_perSide[1][0])->pVec().Y();

      totalCharge = missingPt = invMass = 0;
      nTofTrks = nTpcTrks = nSigPPion = 0;
      dEdx.clear();
      momentum.clear();
      charge.clear();
      nSigmaTPCPion.clear();
      vexterId.clear();
      vertexZ.clear();
      double nSigmaPairPion2 = 0;
      TLorentzVector centralTracksTotalFourMomentum;

      if( upcEvt->getNPrimTracks() !=2)
        return;


    // loop over all TPC tracks
      for(int j=0; j<upcEvt->getNumberOfTracks(); ++j){
    // get TPC track object
        const StUPCTrack* trk = upcEvt->getTrack(j);
        TLorentzVector trkLVector;
        trk->getLorentzVector(trkLVector,pionMass);

        if( !trk->getFlag(StUPCTrack::kPrimary) ) continue; // skip global tracks, analyze only primary
        // read basic information about the track
        ++nTpcTrks;     

        if( trk->getFlag(StUPCTrack::kTof)){  
          dEdx.push_back(trk->getDEdxSignal()*1000000);
          momentum.push_back(trkLVector.P());
          charge.push_back(trk->getCharge());
          nSigmaTPCPion.push_back(trk->getNSigmasTPCPion());
          vexterId.push_back(trk->getVertexId());
          vertexZ.push_back(trk->getVertex()->getPosZ());
          totalCharge += static_cast<int>( trk->getCharge() );
          centralTracksTotalFourMomentum += trkLVector;
          nSigmaPairPion2 += pow(trk->getNSigmasTPCPion(),2);
          ++nTofTrks;
        }

      }

      missingPt = (centralTracksTotalFourMomentum.Vect() + rpEvt->getTrack( rpTrackIdVec_perSide[E][0] )->pVec() + rpEvt->getTrack( rpTrackIdVec_perSide[W][0] )->pVec() ).Pt();
      invMass = centralTracksTotalFourMomentum.M();
      nSigPPion = sqrt(nSigmaPairPion2);

      if( nTofTrks!=2)
        return;

      if( vexterId[0] != vexterId[1])
        return;

      if(totalCharge != 0) 
        return;

      if( missingPt > 0.1 )
        return;



    }
  } // end of loop over arms

}

//_____________________________________________________________________________
TFile *CreateOutputTree(const string& out) {

  TFile *outputFile = TFile::Open(out.c_str(), "recreate");
  if(!outputFile) return 0x0;

  //standard reconstructed tree
  recTree = new TTree("recTree", "recTree");

  recTree ->Branch("triggerBits", &triggerBits, "triggerBits/I");
  recTree ->Branch("nTracks", &nTracks, "nTracks/I");
  recTree ->Branch("nTpcTrks", &nTpcTrks, "nTpcTrks/I");
  recTree ->Branch("nTofTrks", &nTofTrks, "nTofTrks/I");
  recTree ->Branch("totalCharge", &totalCharge, "totalCharge/I");
  recTree ->Branch("nSigPPion", &nSigPPion, "nSigPPion/D");
  recTree ->Branch("missingPt", &missingPt, "missingPt/D");
  recTree ->Branch("invMass", &invMass, "invMass/D");
//  recTree ->Branch("", &, "/");
//  vector<Double_t> dEdx;
//  vector<Double_t> momentum;
//  vector<Double_t> charge;
//  vector<Double_t> nSigmaTPCPion;
//  vector<Double_t> vexterId;
//  vector<Double_t> vertexZ;
//  Double_t xCorrelations[nSides];
//  Double_t yCorrelations[nSides];


  return outputFile;

}//CreateOutputTree

bool ConnectInput(const string& in, int fileId) {
  //input from file or chain
  upcTree = 0x0;
  rpTree = 0x0;
  if( in.find(".root") != string::npos ) {
    cout << "Input from root file" << endl;
    infile = TFile::Open(in.c_str(), "read");
    if(!infile) return false;
    upcTree = dynamic_cast<TTree*>( infile->Get("mUPCTree") );
    rpTree = dynamic_cast<TTree*>( infile->Get("mRPTree") );
  } else {
    cout << "Input from chain" << endl;
    upcChain = new TChain("mUPCTree");
    rpChain = new TChain("mRPTree");
    ifstream instr(in.c_str());
    string line;
    int lineId=0;
    while(getline(instr, line)) {
    	if(fileId==lineId || fileId== -1){
    		upcChain->AddFile(line.c_str());
      	rpChain->AddFile(line.c_str());
    	}
      lineId++;
    }
    instr.close();
    upcTree = dynamic_cast<TTree*>( upcChain );
    rpTree = dynamic_cast<TTree*>( rpChain );
  }

  if(!upcTree) return false;
  if(!rpTree) return false;

  rpTree->SetBranchAddress("mRPEvent", &rpEvt);
  upcTree->SetBranchAddress("mUPCEvent", &upcEvt);

  return true;

}//ConnectInput
