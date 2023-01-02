#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>

#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST_ 3

TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString hubb[_N_CASES_] = {"H100","H225","H165"};
TString delt[_N_CASES_] = {"D2","D4","D4"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
TString hists[_N_HIST_] = {"num1d","den1d","num1dqsc"};

TString casesNice[_N_CASES_] = {"case A","case B", "case C"};
TString pairsNice[_N_PAIRS_] = {"p-p","#pi^{+}-#pi^{+}","#pi^{-}-#pi^{-}"};
TString epsilonNice[7] = {"#epsilon = 0.0","#epsilon = 0.1","#epsilon = 0.2","#epsilon = 0.3","#epsilon = 0.4","#epsilon = 0.5","#epsilon = 0.6"};
int colors[]={1, 600, 629, 414, 802, 880, kTeal+2, 922,433,618};
int colorsCute[]={kBlue+2, kAzure-1, kAzure+2, kAzure+7, kAzure+10, kTeal+2, kOrange-3,kOrange+7,kRed,kRed+2};

void makePaveText(TVirtualPad* can, TString text, double x1, double y1, double x2, double y2, double size = 0.04, Int_t color=kBlack) {
    TPaveText *description=new TPaveText(x1,y1,x2,y2,"NDC");
    description->SetLineWidth(0);
  description->AddText(text);
  description->SetTextSize(size);
  description->SetBorderSize(0.0);
  description->SetTextFont(42);
  description->SetTextColor(color);
  description->SetFillColor(0);
  description->SetFillStyle(0);
  description->SetTextAlign(13);
      can->cd();
    description->Draw("same");
}

void multiplePlot(TH1D *histTab[], TCanvas* can, TString* entry, size_t n, bool up) {
	
    Double_t max = 0;
    Double_t min = 0;

    for (int i = 0; i < n; i++) {
        if(histTab[i]->GetMaximum()>max)
            max = histTab[i]->GetMaximum();
        if(histTab[i]->GetMinimum()<min)
            min = histTab[i]->GetMinimum();
    }

 
    histTab[0]->GetYaxis()->SetRangeUser(0.85*min,1.15*max);
    histTab[0]->SetTitle("");
    histTab[0]->SetStats(kFALSE);

    TLegend* legend;
    if (up)
        legend	= new TLegend(0.7,0.6,0.93,0.8, "",	"NDC");
    else
        legend	= new TLegend(0.7,0.2,0.93,0.4, "",	"NDC");
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.03);
    legend->SetLineWidth(0);
    legend->SetName("legend");

    //gPad->SetLeftMargin(0.13); 

    histTab[0]->GetXaxis()->SetTitleSize(0.04);
    histTab[0]->GetYaxis()->SetTitleSize(0.04);
    histTab[0]->GetXaxis()->SetTitleFont(42);
    histTab[0]->GetYaxis()->SetTitleFont(42);
    histTab[0]->GetXaxis()->SetTitleOffset(1.2);
    histTab[0]->GetXaxis()->SetLabelFont(42);
    histTab[0]->GetYaxis()->SetLabelFont(42);
    histTab[0]->GetXaxis()->SetLabelSize(0.035);
    histTab[0]->GetYaxis()->SetLabelSize(0.035);


    can->cd();
    TF1* funtmp;
    for (int i = 0; i < n; i++) {        

		histTab[i]->SetMarkerStyle(21);
        histTab[i]->SetMarkerSize(0.8);
        histTab[i]->SetLineWidth(2);
	
        histTab[i]->SetMarkerColor(colors[i+1]);
        histTab[i]->SetLineColor(colors[i+1]);
        
        legend->AddEntry(histTab[i],entry[i],"AP");

        if (i == 0)
            histTab[i]->Draw();
        else
            histTab[i]->Draw("same");

        
    }
    legend->Draw("same");



}

void compFixedEpsilon() {

    TH1D* hist[_N_CASES_][_N_EPSILON_][_N_PAIRS_];
    TFile *fileIn[_N_CASES_][_N_PAIRS_];
    TCanvas *can[_N_CASES_][_N_EPSILON_];
   

    gROOT -> SetBatch(kTRUE);

    //READING
    for(int i = 0; i < _N_CASES_; i ++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
            fileIn[i][j] = new TFile(Form("./outputCorrFunc/%s%s.root",cases[i].Data(),pairs[j].Data()));
            
            for (int k = 0; k < _N_EPSILON_; k++) 
                hist[i][k][j] = (TH1D*) fileIn[i][j]->Get(Form("%sE%d%sCorrFunc",cases[i].Data(),k,pairs[j].Data()));

            

        }

    }

    //DRAWING

    TFile *fileOut[_N_CASES_];

    for(int i = 0; i < _N_CASES_; i++) {
        fileOut[i] = new TFile(Form("./outputFixedEpsilon/%s.root",cases[i].Data()), "RECREATE");
        for(int j = 0; j < _N_EPSILON_; j++) {
            
            for(int k = 0; k < _N_PAIRS_; k++) {
                hist[i][j][k]->GetFunction("fun")->SetLineColor(colors[k+1]);
                hist[i][j][k]->GetXaxis()->SetRangeUser(0.0,0.15);
            }


                           
            can[i][j] = new TCanvas(Form("%sE%dPairsComp",cases[i].Data(),j),Form("%s%dPairsComp",cases[i].Data(),j),800,800);
            multiplePlot(hist[i][j],can[i][j],pairsNice,3,true);
            makePaveText(can[i][j],casesNice[i].Data(),0.55,0.7,0.7,0.75);
            makePaveText(can[i][j],epsilonNice[j].Data(),0.55,0.75,0.7,0.8);
            fileOut[i]->cd();
            can[i][j]->Write();
            can[i][j]->SaveAs(Form("./ladne/fixedEpsilon/%sE%dPairsComp.pdf",cases[i].Data(),j));

            

        }
        fileOut[i]->Save();
        fileOut[i]->Close();
    }




    gROOT -> SetBatch(kFALSE);

}