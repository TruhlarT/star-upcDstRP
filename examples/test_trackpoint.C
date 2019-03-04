
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
  TFile *infile = TFile::Open("../test3.root", "read");

  //get picoDst tree in file
  TTree *upcTree = dynamic_cast<TTree*>( infile->Get("mRPTree") );

  //open output file
  TFile *outfile = TFile::Open("output.root", "recreate");

  //pT histogram
  TH1D *hPt = new TH1D("hTest", "hTest", 100, 0, 10);

  //connect upc event to the tree
  static StRPEvent *upcEvt = 0x0;
  upcTree->SetBranchAddress("mUPCEvent", &upcEvt);

  //ask for number of events
  Long64_t nev = upcTree->GetEntries();
  cout << "Number of events: " << nev << endl;

  nev = 100;

  //event loop
  for(Long64_t iev=0; iev<nev; iev++) {

    //get the event
    upcTree->GetEntry(iev);

    //tracks loop
    for(Int_t i=0; i<upcEvt->getNumberOfTracks(); ++i) {
      StUPCRpsTrack *trk = upcEvt->getTrack(i);

      for(Int_t j = 0; j < 2 ; ++j){
        StUPCRpsTrackPoint *trkPoint = trk->trackPoint(j);
        cout << trkPoint->rpId() << endl;
      }

      cout << trk->branch() << endl;
      hPt->Fill(trk->getPt());
    }

  }

  //close outputs
  hPt->Write();
  outfile->Close();


}


