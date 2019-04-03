
void run() {

  gROOT->ProcessLine(".include ../include");

  gSystem->Load("../build/libstar-upc.so");

  gROOT->ProcessLine(".x analysis.C+g");

}

