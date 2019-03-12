
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
  TFile *infile = TFile::Open("/gpfs01/star/pwg/truhlar/star-upcDst/test/2FA1333680D49A7D8008EEB57BE6DE01_0.root", "read");

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
    // cout<<"event "<<iev<<endl;
    upcTree->GetEntry(iev);
    /*for(int i = 0; i <8 ;++i){
      if(upcEvt->numberOfPlanesWithClusters(i) != 0)  
        cout<<"numberOfPlanesWithClusters in "<<i<<" RP: "<<upcEvt->numberOfPlanesWithClusters(i)<<endl;
    
      cout<<"adc in "<<i<<" RP: "<<upcEvt->adc(i,0)<<endl;
    } */
   
   
    if(upcEvt->getNumberOfTracks() > 0){
      cout << "Number of tracks: " << upcEvt->getNumberOfTracks() << endl;
    } 
    
    //cout << "Number of trackPoints: " << upcEvt->getNumberOfTrackPoints() << endl;
    //tracks loop
    /*
    for(Int_t i=0; i<upcEvt->getNumberOfTracks(); ++i) {
      StUPCRpsTrack *trk = upcEvt->getTrack(i);

      for(Int_t j = 0; j < 2 ; ++j){
        StUPCRpsTrackPoint *trkPoint = trk->trackPoint(j);
        cout << trkPoint->rpId() << endl;
      }

      //cout << trk->branch() << endl;
      //hPt->Fill(trk->branch());
    }
    */
  }
  cout<< "closing"<<endl;
  //close outputs
  hPt->Write();
  outfile->Close();


}


