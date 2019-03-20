
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

//_____________________________________________________________________________
void test_trackpoint() {

  //open input file
  TFile *infile = TFile::Open("/gpfs01/star/pwg/truhlar/star-upcDst/testTrackPoint21/1778D29D6D479A3D577431C01CCB1323_0.root", "read");

  //get picoDst tree in file
  TTree *upcTree = dynamic_cast<TTree*>( infile->Get("mRPTree") );

  //open output file
  TFile *outfile = TFile::Open("output.root", "recreate");

  //pT histogram
  TH1D *hPt = new TH1D("hTest", "hTest", 100, 0, 10);

  //connect upc event to the tree
  static StRPEvent *upcEvt = 0x0;
  upcTree->SetBranchAddress("mRPEvent", &upcEvt);

  //ask for number of events
  Long64_t nev = upcTree->GetEntries();
  cout << "Number of events: " << nev << endl;

  // nev = 100;

  //event loop
  for(Long64_t iev=0; iev<nev; iev++) {
    //get the event
	// cout<<"Processing evt n: "<<iev<<endl;
    upcTree->GetEntry(iev);
    
    //tracks loop
    
    for(Int_t i=0; i<upcEvt->getNumberOfTracks(); ++i) {
      StUPCRpsTrack *trk = upcEvt->getTrack(i);
      cout<<"proccesimg track "<<i<<" evt "<<iev<<endl;
      if(!trk){
        cout<<"empty track"<<endl;
        continue; 
      }
      for(Int_t j = 0; j < 2 ; ++j){
        StUPCRpsTrackPoint *trkPoint = trk->trackPoint(j);
        if(!trkPoint){
          cout<<"empty trackPoint"<<endl;
          continue; 
        }
        //cout << trkPoint->rpId() << endl;
      }

     // cout << trk->branch() << endl;
      //hPt->Fill(trk->branch());
    }
    
  }
  cout<< "closing"<<endl;
  //close outputs
  hPt->Write();
  outfile->Close();


}


