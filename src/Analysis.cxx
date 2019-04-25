// Run: ./Analysis input index
// e.g. ./Analysis /gpfs01/star/pwg/truhlar/Final/CPtrig/merge_files/StUPCRP_production.list -1
// or you can open just root file
// ./Analysis /gpfs01/star/pwg/truhlar/ConnectionTest/ELtrig/test/A297EEE504EDA768A8E98674FDF699B7_63.root


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

// Labels, names etc. (defined as TString to gain higher functionality than const char*, e.g. defined "+" operator)

const double pionMass = 0.13957; // GeV /c^2
const double speedOfLight = 299792458; // m/s
const int nTriggers = 21;
const int triggerID[] = {1,2,3,4,5,8,9,570701,570702,570703,
570704,570705,570709,570711,570712,570719,590701,590703,590705,590708,590709};

//bool PID = true;
bool PID = false;

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


TString armName[nArms] = { TString("EU-WU"), TString("ED-WD") };
TString branchName[nBranches] = { TString("EU"), TString("ED"), TString("WU"), TString("WD") };
TString rpName[nRomanPots] = { TString("E1U"), TString("E1D"), TString("E2U"), TString("E2D"),
                    TString("W1U"), TString("W1D"), TString("W2U"), TString("W2D") };
TString stationName[nStations] = { TString("E1"), TString("E2"), TString("W1"), TString("W2") };
TString summaryLabels[10] = { TString("CPTnoBBCL"), TString("Elastic"), TString("Inelastic"), TString("2 TPC tracks"), 
                    TString("2 TOF tracks"), TString("Same vertex"), TString("TotCharge 0"), TString("MissingPt < 0.1 GeV"), TString(""), TString("")};
TString systemID[3] = { TString("Combi"), TString("InEl"), TString("El")};
TString systemState[4] = { TString("TPC2t"), TString("TOF2trk"), TString("Q0"), TString("Excl")};
TString configLabels[4] = { TString("EUD"), TString("EDU"), TString("IUU"), TString("IDD")};
TString conection[2] = { TString("Good"), TString("Bad") };

// Histograms declarations
TH1I* hAnalysisFlow; // control plot
TH1F* hTriggerBits; // control plot
TH1F* hConfiguration;
TH1F* hNumberTrackPerBranch[nBranches]; // number of track per each brunch (west up, west down, east up, east down)
TH1D *hConnection;

// PID
TH2D* hdEdxVsP[12];
TH2D* hdEdxVsqP[12];
TH1D* hNSigmaPion[12];
TH1D* hNSigmaPionPair[12];

TH1F* hMissingPt[12]; 
TH1F* hInvMass[12];

TH2F* hXYCorrelations[12];
TH1D* hZvertex[12];

TFile *infile;
TFile *outfile;
TChain *upcChain;
TChain *rpChain;
StUPCEvent *upcEvt;
StRPEvent *rpEvt;

TTree *upcTree;
TTree *rpTree;

TTree *recTree;
Int_t triggerBits, nTracks;
Int_t totalCharge;
Int_t nTofTrks;
Int_t nTpcTrks;
Double_t nSigPPion;
Double_t missingPt; 
Double_t invMass;
vector<Double_t> dEdx;
vector<Double_t> momentum;
vector<Double_t> TOFtime;
vector<Double_t> TOFlength;
vector<Double_t> charge;
vector<Double_t> nSigmaTPCPion;
vector<Double_t> vexterId;
vector<Double_t> vertexZ;
Double_t xCorrelations[nSides];
Double_t yCorrelations[nSides];


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
  int fileId = 0;
  const string& input = argv[1];
  if( input.find(".root") == string::npos )
    fileId = atoi(argv[2]);

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
  hAnalysisFlow = new TH1I("AnalysisFlow", "CutsFlow", kMaxCount-1, 1, kMaxCount);
  for(int tb=1; tb<kMaxCount; ++tb) hAnalysisFlow->GetXaxis()->SetBinLabel(tb, summaryLabels[tb-1]);

  hTriggerBits = new TH1F("TriggerBits", "TriggerBits", nTriggers, -0.5, 20.5);
  for(int tb=0; tb<nTriggers; ++tb) hTriggerBits->GetXaxis()->SetBinLabel(tb+1, triggerName[tb]);

  hConfiguration = new TH1F("Configuration", "Track Configuration", 4, -0.5, 3.5);
  for(int tb=0; tb<4; ++tb) hConfiguration->GetXaxis()->SetBinLabel(tb+1, configLabels[tb]);

  hConnection = new TH1D("hConnection", "Agreement between track brunch and trackPoints RPIds", 2, -0.5, 1.5);
  for(int tb=0; tb<2; ++tb) hConnection->GetXaxis()->SetBinLabel(tb+1, conection[tb]);

  for(int i=0; i<nBranches; ++i)
    hNumberTrackPerBranch[i] = new TH1F("NumberTracksPerBranch,"+branchName[i],"Number of tracks in branch "+branchName[i], 8, -0.5, 7.5);

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

void Make(){
  hAnalysisFlow->Fill(kCPtrig);
  for(int var = 0; var < nTriggers; ++var){
    if(upcEvt->isTrigger(triggerID[var])){
      hTriggerBits->Fill(var);
      triggerBits = var; // not 100% correct, some events have more than 1 trigger
    }
  }
  nTracks = (Int_t) rpEvt->getNumberOfTracks();
  // Vector below will be filled with indices of good-quality tracks
  vector<int> rpTrackIdVec_perBranch[nBranches];
  vector<int> rpTrackIdVec_perSide[nSides];
  int numberOfTracks = 0;
  int numberOfTracksPerBranch[nBranches] = {0, 0, 0, 0};

  // Loop over all tracks reconstructed in Roman Pots  
  for(int k=0; k<rpEvt->getNumberOfTracks(); ++k){
  // Get pointer to k-th track in Roman Pot data collection
    StUPCRpsTrack *trk = rpEvt->getTrack(k);
    trk->setEvent(rpEvt);
  // Get ID of a branch in which this k-th track was reconstructed
    int j = trk->branch();
    int side = j<2 ? E : W;

    ++numberOfTracks;
    ++numberOfTracksPerBranch[j];

    for(Int_t kj = 0; kj < 2 ; ++kj){ // Testing connection between tracks and tracksPoint
      if(!trk) break; 
      StUPCRpsTrackPoint *trkPoint = trk->getTrackPoint(kj);\
      if(!trkPoint) continue; 
      int rpID = trkPoint->rpId();
      switch (j) {
        case 0:
        case 1: 
          if(rpID == j || rpID == j +2){
            hConnection->Fill(0);
          }else{
            hConnection->Fill(1);
            cout << rpID << " / "<<j<< endl;
          }
        break;
        case 2: 
        case 3:  
          if(rpID == j+2 || rpID == j +4){
            hConnection->Fill(0);
          }else{
            hConnection->Fill(1);
            cout << rpID << " / "<<j<< endl;
          }
        break;
      };
    } 
  // If track is global (made of track-points   in 1st and 2nd station)
  // and all 8 planes were used in reconstruction - store its ID
    if( trk->type()==StUPCRpsTrack::rpsGlobal && trk->planesUsed()==8) rpTrackIdVec_perBranch[j].push_back( k );
  // a bit looser selection
    if( (trk->getTrackPoint(0) ? trk->getTrackPoint(0)->planesUsed()>=3 : true) &&
      (trk->getTrackPoint(1) ? trk->getTrackPoint(1)->planesUsed()>=3 : true) ) rpTrackIdVec_perSide[side].push_back( k );
  }

  for(int i=0; i<nBranches; ++i) 
    hNumberTrackPerBranch[i]->Fill(numberOfTracksPerBranch[i]);

// Loop over arms - check if have good-quality tracks, selecting branch combination
  int nConfig = 0;
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
      nConfig++;
      if(nConfig>1) cout<<"NConfig: "<<nConfig<<endl;
      if(i==EUD || i==EDU){
        hAnalysisFlow->Fill(kEl);
      }else{
        hAnalysisFlow->Fill(kInEl);
      }
      hConfiguration->Fill(i);

      for(int j = 0; j < nSides;++j){
        xCorrelations[j] = rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().X();
        yCorrelations[j] = rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().Y();
      }

      totalCharge = missingPt = invMass = 0;
      nTofTrks = nTpcTrks = nSigPPion = 0;
      dEdx.clear();
      momentum.clear();
      TOFtime.clear();
      TOFlength.clear();
      charge.clear();
      nSigmaTPCPion.clear();
      vexterId.clear();
      vertexZ.clear();
      double nSigmaPairPion2 = 0;
      TLorentzVector centralTracksTotalFourMomentum;

      if( upcEvt->getNPrimTracks() !=2)
        return;
      hAnalysisFlow->Fill(kTPC2t);

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
          TOFtime.push_back(trk->getTofTime());
          TOFlength.push_back(trk->getTofPathLength());
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

      if(PID){
        // DeltaDeltaTOF - Daniel
        double expectedTime1 = (TOFlength[0] / speedOfLight) * sqrt(1 + (pionMass * speedOfLight / momentum[0])*(pionMass * speedOfLight / momentum[0]));
        double expectedTime2 = (TOFlength[1] / speedOfLight) * sqrt(1 + (pionMass * speedOfLight / momentum[1])*(pionMass * speedOfLight / momentum[1]));
        double deltaTOF = TOFtime[1] - TOFtime[0];
        double deltaTOFexpected = expectedTime2 - expectedTime1;
        double deltaDeltaTOF = deltaTOF - deltaTOFexpected;
        // m2 - Rafal

  		  double length1Squared = TOFlength[0]*TOFlength[0];
  		  double length2Squared = TOFlength[1]*TOFlength[1];
        double speedOfLight2 = speedOfLight*speedOfLight;
        double speedOfLight4 = speedOfLight2*speedOfLight2;
        double deltaTime2 = (TOFtime[1] - TOFtime[0])*(TOFtime[1] - TOFtime[0]);
        double deltaTime4 = deltaTime2*deltaTime2;
        double oneOverMomentum1sq = 1/(momentum[0]*momentum[0]);
        double oneOverMomentum2sq = 1/(momentum[1]*momentum[1]);
  		  double cEq = -2*length1Squared*length2Squared + speedOfLight4*deltaTime4 + length2Squared*length2Squared + length1Squared*length1Squared -2*speedOfLight2*deltaTime2*(length2Squared + length1Squared);
        double bEq = -2*length1Squared*length2Squared*speedOfLight2*(oneOverMomentum1sq + oneOverMomentum2sq) + 2*length1Squared*length1Squared*speedOfLight2*oneOverMomentum1sq + 2*length2Squared*length2Squared*speedOfLight2*oneOverMomentum2sq -2*speedOfLight4*deltaTime2*(length1Squared*oneOverMomentum1sq + length2Squared*oneOverMomentum2sq);
        double aEq = -2*length1Squared*length2Squared*speedOfLight4*oneOverMomentum1sq*oneOverMomentum2sq + speedOfLight4*(length1Squared*length1Squared*oneOverMomentum1sq*oneOverMomentum1sq + length2Squared*length2Squared*oneOverMomentum2sq*oneOverMomentum2sq);
        double mSquared = (-bEq + sqrt(bEq*bEq-4*aEq*cEq)) / (2*aEq);
      }
       


      FillPlots(0,i);
      if( nTofTrks!=2)
        return;
      hAnalysisFlow->Fill(kTOF2t);

      if( vexterId[0] != vexterId[1])
        return;
      hAnalysisFlow->Fill(kSameVrtx);

      FillPlots(2,i);

      if(totalCharge != 0) 
        return;
      hAnalysisFlow->Fill(kTotCH0);

      FillPlots(2,i);

      if( missingPt > 0.1 )
        return;
      hAnalysisFlow->Fill(kMissPt);

      FillPlots(3,i);


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

void FillPlots(int state, int configuration){
  for(int tmp =0; tmp<3;++tmp){
    if(tmp == 1 && (configuration!=IUU && configuration!=IDD))
      continue;
    if(tmp == 2 && (configuration!=EUD && configuration!=EDU))
      continue;
    for(int trackId=0;trackId<nTofTrks;++trackId){
      hdEdxVsP[4*tmp +state]->Fill(momentum[trackId],dEdx[trackId]);
      hdEdxVsqP[4*tmp+state]->Fill(momentum[trackId]*charge[trackId],dEdx[trackId]);
      hNSigmaPion[4*tmp+state]->Fill(nSigmaTPCPion[trackId]);
      hZvertex[4*tmp+state]->Fill(vertexZ[trackId]);
    }
    hNSigmaPionPair[4*tmp+state]->Fill(nSigPPion);

    hMissingPt[4*tmp+state]->Fill(missingPt); 
    //if(nSigPPion < 3 )
    hInvMass[4*tmp+state]->Fill(invMass);
    for(int j = 0; j < nSides;++j)
      hXYCorrelations[4*tmp+state]->Fill(xCorrelations[j],yCorrelations[j]);
    
  }
}

