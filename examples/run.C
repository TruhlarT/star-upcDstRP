
void run() {

  gROOT->ProcessLine(".include ../include");

  gSystem->Load("../build/libstar-upc.so");

  gROOT->ProcessLine(".x rpAnalysis.C+g");
  //gROOT->ProcessLine(".x analysis.C+g");

}

