
void run_test() {

  gROOT->ProcessLine(".include ../include");

  gSystem->Load("../build/libstar-upc.so");

  gROOT->ProcessLine(".x test_trackpoint.C+g");

}

