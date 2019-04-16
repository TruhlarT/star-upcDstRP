void makro(){
    TFile *infile = TFile::Open("0.AnalysisOutput.root", "read");
    TH1F*  hAnalysisFlow = (TH1F*)infile -> Get("AnalysisFlow");
    int total = 0;
    for(int i = 0; i < 12; ++i){
    	cout<<hAnalysisFlow->GetBinContent(i)<<endl;
    	total+=hAnalysisFlow->GetBinContent(i);
    }
    cout<<"Total: "<<total<<" / "<<hAnalysisFlow->GetEntries()<<endl;
}