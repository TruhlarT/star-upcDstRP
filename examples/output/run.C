
void run(TString input, TString output) {

  gROOT->ProcessLine(".include ../../include");

  gSystem->Load("../../build/libstar-upc.so");

  TString command = ".x rpAnalysis.C+g(\"" + input + "\",\"" + output +"\")"; 

  gROOT->ProcessLine(command);
  //gROOT->ProcessLine(".x analysis.C+g");

}

