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

// Enumerations - very helpful and convenient !
enum SIDE {E=0, East=0, W=1, West=1, nSides};
enum XY_COORDINATE {X, Y, nCoordinates};
//enum TRIGGER_ID {SD, CPT2, SDT, RPZMU, RP2MU, ET, CP, CPT, RP2E, Zerobias, CPX, SDZ, CPEI, ZE, CPTnoBBCL, CPT2noBBCL, nTriggers};
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
TString conection[2] = { TString("Good"), TString("Bad") };

// Histograms declarations
TH1F* hAnalysisFlow; // control plot
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
StUPCEvent *upcEvt;
StRPEvent *rpEvt;

TTree *recTree;
Int_t triggerBits, nTracks;
Double_t vertexZ;



void Init();
void Make();
TFile *CreateOutputTree(const string& out);

//_____________________________________________________________________________
int main(void) {

  //get picoDst tree in file
  TTree *upcTree;
  TTree *rpTree;

  //open output file
  outfile = CreateOutputTree("test.root"); 
  if(!outfile) {cout << "Can not open output file." << endl; return -1;}

  Init();
  Long64_t total =0;
  Long64_t counter1 =0;
  Long64_t counter2 =0;
  Long64_t counter3 =0;
  for(int iFile = 0; iFile < 6; ++iFile){
  //for(int iFile = 0; iFile < 1; ++iFile){
    //open input file
    TString fileName; 
    fileName.Form("/gpfs01/star/pwg/truhlar/NewBaseLine/CPtrig/merge_files/StUPCRP_production_000%d.root",iFile);
    cout<<"Start proccesing file: "<<fileName<<endl;
    infile = TFile::Open(fileName, "read");
    if(!infile) {cout << "Can not open input file." << endl; return -1;}

    //get picoDst tree in file
    rpTree = dynamic_cast<TTree*>( infile->Get("mRPTree") );
    upcTree = dynamic_cast<TTree*>( infile->Get("mUPCTree") );

    //connect upc event to the tree
    rpTree->SetBranchAddress("mRPEvent", &rpEvt);
    upcTree->SetBranchAddress("mUPCEvent", &upcEvt);

    //ask for number of events
    Long64_t nev = upcTree->GetEntries();
    cout << "Number of UPC events: " << nev <<" RP: "<<rpTree->GetEntries() <<endl;
    total +=nev;
    //event loop
    for(Long64_t iev=0; iev<nev; ++iev) { //get the event
      counter1++;
      upcTree->GetEntry(iev); 
      rpTree->GetEntry(iev); 
      counter2++;
      Make();
      recTree->Fill();
      counter3++;
    } 

    infile->Close();
  }


  //close the outputs
  outfile->Write();
  outfile->Close();
  cout<<"Total number of events: "<<total<<endl;
  cout<<"Counter 1,2 and 3: "<<counter1<<" "<<counter2<<" "<<counter3<<endl;
  cout<<"Ending Analysis... GOOD BYE!"<<endl;
  return 0;
}//main

void Init(){
// creating histograms
// Having defined labels/names it is very easy and fast to create multiple histograms differing only by name
// (e.g. track multiplicity in single branch, theta angle for each arm and coordinate, etc.)
  hAnalysisFlow = new TH1F("AnalysisFlow", "CutsFlow", 10, -0.5, 9.5);
  for(int tb=0; tb<10; ++tb) hAnalysisFlow->GetXaxis()->SetBinLabel(tb+1, summaryLabels[tb]);

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
  hAnalysisFlow->Fill(0);

  for(int var = 0; var < nTriggers; ++var){
    if(upcEvt->isTrigger(triggerID[var])){
      hTriggerBits->Fill(var);
      triggerBits = (Int_t) var;
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


  // Get ID of a branch in which this k-th track was reconstructed
    int j = trk->branch();
    int side = j<2 ? E : W;
    ++numberOfTracks;
    ++numberOfTracksPerBranch[j];
    for(Int_t kj = 0; kj < 2 ; ++kj){
      if(!trk) break; 
      StUPCRpsTrackPoint *trkPoint = trk->trackPoint(kj);\
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
    if( (trk->trackPoint(0) ? trk->trackPoint(0)->planesUsed()>=3 : true) &&
      (trk->trackPoint(1) ? trk->trackPoint(1)->planesUsed()>=3 : true) ) rpTrackIdVec_perSide[side].push_back( k );
  }


  for(int i=0; i<nBranches; i++) hNumberTrackPerBranch[i]->Fill(numberOfTracksPerBranch[i]);

/////////////////////////////////////////////////////////////////////////
//              START OF THE ANALYSIS FLOW                  //
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

      const unsigned int nTpcVertex = upcEvt->getNumberOfVertices();
      if(nTpcVertex != 1) 
        return;
      hAnalysisFlow->Fill(3);
      const StUPCVertex* vertex = upcEvt->getVertex(0);

      vertexZ = (Double_t) vertex->getPosZ();

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

      if( upcEvt->getNPrimTracks() !=2)
        return;
      hAnalysisFlow->Fill(4);

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
          int state = 0;
          for(int tmp =0; tmp<3;++tmp){
            if(tmp == 1 && (i!=IUU && i!=IDD))
              continue;
            if(tmp == 2 && (i!=EUD && i!=EDU))
              continue;
            hdEdxVsP[4*tmp +state]->Fill(trkLVector.P(),trk->getDEdxSignal()*1000000);
            hdEdxVsqP[4*tmp+state]->Fill(trkLVector.P()*trk->getCharge(),trk->getDEdxSignal()*1000000);
            hNSigmaPion[4*tmp+state]->Fill(trk->getNSigmasTPCPion());
          } 

          if(nTofTrks < 2){
            trkInfo[nTofTrks][0] = trk->getDEdxSignal()*1000000;
            trkInfo[nTofTrks][1] = trkLVector.P();
            trkInfo[nTofTrks][2] = trk->getCharge();
            trkInfo[nTofTrks][3] = trk->getNSigmasTPCPion();
          }
          totalCharge += static_cast<int>( trk->getCharge() );
          centralTracksTotalFourMomentum += trkLVector;
          nSigmaPairPion2 += pow(trk->getNSigmasTPCPion(),2);
          ++nTofTrks;
        }

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
        hZvertex[4*tmp+state]->Fill(vertexZ);
      } 


      if( nTofTrks!=2)
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


        hMissingPt[4*tmp+state]->Fill(missingPt); 
        if(nSigPPion < 3 )
          hInvMass[4*tmp+state]->Fill(invMass);
        for(int j = 0; j < nSides;++j)
          hXYCorrelations[4*tmp+state]->Fill(rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().X(),rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().Y());
        hZvertex[4*tmp+state]->Fill(vertexZ);
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


        hMissingPt[4*tmp+state]->Fill(missingPt);
        if(nSigPPion < 3 )
          hInvMass[4*tmp+state]->Fill(invMass);
        for(int j = 0; j < nSides;++j)
          hXYCorrelations[4*tmp+state]->Fill(rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().X(),rpEvt->getTrack(rpTrackIdVec_perSide[j][0])->pVec().Y());
        hZvertex[4*tmp+state]->Fill(vertexZ);
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
        hZvertex[4*tmp+state]->Fill(vertexZ);
      }

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
  recTree ->Branch("vertexZ", &vertexZ, "vertexZ/D");
  recTree ->Branch("nTracks", &nTracks, "nTracks/I");


  return outputFile;

}//CreateOutputTree
