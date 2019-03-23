
// c++ headers
#include <iostream>

// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

// picoDst headers
#include "StRPEvent.h"
#include "StUPCRpsTrack.h"
#include "StUPCRpsTrackPoint.h"

using namespace std;

TString branchName[4] = { TString("EU"), TString("ED"), TString("WU"), TString("WD") };
TString rpName[8] = { TString("E1U"), TString("E1D"), TString("E2U"), TString("E2D"),
                    TString("W1U"), TString("W1D"), TString("W2U"), TString("W2D") };
TString conection[2] = { TString("Good"), TString("Bad") };
//TString empty[2] = { TString("Track"), TString("TrackPoint") };
TString full[3] = { TString("Tracks"), TString("corres. TrackPoint"), TString("TrackPoints") };
TString type[3] = { TString("Local"), TString("Global"), TString("Not set") };

//_____________________________________________________________________________
void test_trackpoint() {

  //open input file
  TFile *infile = TFile::Open("/gpfs01/star/pwg/truhlar/star-upcDst/MemQuickComp15/88A0FFC754D3A211CD83CBC74531C164_0.root", "read");

  //get picoDst tree in file
  TTree *upcTree = dynamic_cast<TTree*>( infile->Get("mRPTree") );

  //open output file
  TFile *outfile = TFile::Open("output2.root", "recreate");

  //pT histogram
  TH1D *hBrunch = new TH1D("hBrunch", "Brunches of tracks", 4, -0.5, 3.5);
  for(int tb=0; tb<4; ++tb) hBrunch->GetXaxis()->SetBinLabel(tb+1, branchName[tb]);
  TH1D *hRPId = new TH1D("hRPId", "RPId of trackPoints", 8, -0.5, 7.5);
  for(int tb=0; tb<8; ++tb) hRPId->GetXaxis()->SetBinLabel(tb+1, rpName[tb]);
  TH1D *hConnection = new TH1D("hConnection", "Agreement between track brunch and trackPoints RPIds", 2, -0.5, 1.5);
  for(int tb=0; tb<2; ++tb) hConnection->GetXaxis()->SetBinLabel(tb+1, conection[tb]);
  TH1D *hEmpty = new TH1D("hEmpty", "Empty tracks, their trackPoints and trackPoints", 3, -0.5, 2.5);
  for(int tb=0; tb<3; ++tb) hEmpty->GetXaxis()->SetBinLabel(tb+1, full[tb]);
  TH1D *hFull = new TH1D("hFull", "Number of tracks, their trackPoints and trackPoints", 3, -0.5, 2.5);
  for(int tb=0; tb<3; ++tb) hFull->GetXaxis()->SetBinLabel(tb+1, full[tb]);
  TH1D *hType = new TH1D("hType", "Types of tracks", 3, -0.5, 2.5);
  for(int tb=0; tb<3; ++tb) hType->GetXaxis()->SetBinLabel(tb+1, type[tb]);

  TH1D *hTracks = new TH1D("hTracks", "Number of tracks per event", 20, -0.5, 399.5);
  TH1D *hTracksCut = new TH1D("hTracksCut", "Number of tracks per event with planesUsed >= 3", 100, -0.5, 99.5);
  //connect upc event to the tree
  static StRPEvent *upcEvt = 0x0;
  upcTree->SetBranchAddress("mRPEvent", &upcEvt);

  //ask for number of events
  Long64_t nev = upcTree->GetEntries();
  cout << "Number of events: " << nev << endl;

  // nev = 100;
  //event loop
  for(Long64_t iev=0; iev<nev; iev++) { //get the event

    upcTree->GetEntry(iev);  
    hTracks->Fill(upcEvt->getNumberOfTracks());
    int NumberOfTracks = 0;
    for(Int_t i=0; i<upcEvt->getNumberOfTracks(); ++i) { //tracks loop
      StUPCRpsTrack *trk = upcEvt->getTrack(i);
      if(!trk){
        hEmpty->Fill(0);
        continue; 
      }
      if(trk->planesUsed() > 2)
        NumberOfTracks++;
      hType->Fill(trk->type());
      hFull->Fill(0);
      for(Int_t j = 0; j < 2 ; ++j){
        StUPCRpsTrackPoint *trkPoint = trk->trackPoint(j);
        if(!trkPoint){
          hEmpty->Fill(1);
          continue; 
        }
        hFull->Fill(1);
        switch (trk->branch()) {
          case 0:
          case 1: 
            if(trkPoint->rpId() == trk->branch() || trkPoint->rpId() == trk->branch() +2){
              hConnection->Fill(0);
            }else{
              hConnection->Fill(1);
              cout << trkPoint->rpId() << " / "<<trk->branch()<< endl;
            }
             break;
          case 2: 
          case 3:  
            if(trkPoint->rpId() == trk->branch()+2 || trkPoint->rpId() == trk->branch() +4){
              hConnection->Fill(0);
            }else{
              hConnection->Fill(1);
              cout << trkPoint->rpId() << " / "<<trk->branch()<< endl;
            }
             break;
       }; 
      hRPId->Fill(trkPoint->rpId());
      //cout << trkPoint->rpId() << " / "<<trk->branch()<< endl;
      }
      hBrunch->Fill(trk->branch());
    }

    hTracksCut->Fill(NumberOfTracks);
    for(Int_t i=0; i<upcEvt->getNumberOfTrackPoints(); ++i) { //trackPoints loop
      StUPCRpsTrackPoint *trkPoint = upcEvt->getTrackPoint(i);
      if(!trkPoint){
        hEmpty->Fill(2);
        continue; 
      }
      hFull->Fill(2);
    }
    
  }
  cout<< "closing"<<endl;
  //close outputs
  hConnection->Write();
  hRPId->Write();
  hBrunch->Write();
  hEmpty->Write();
  hFull->Write();
  hType->Write();
  hTracks->Write();
  hTracksCut->Write();
  outfile->Close();


}


