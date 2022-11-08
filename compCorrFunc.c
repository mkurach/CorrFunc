
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
    for (int i = 0; i < n; i++) {        

		histTab[i]->SetMarkerStyle(21);
        histTab[i]->SetMarkerSize(0.8);
        histTab[i]->SetLineWidth(2);
	
        histTab[i]->SetMarkerColor(colors[i]);
        histTab[i]->SetLineColor(colors[i]);

        legend->AddEntry(histTab[i],entry[i],"AP");

        if (i == 0)
            histTab[i]->Draw();
        else
            histTab[i]->Draw("same");

        
    }
    legend->Draw("same");





}

void drawLine(TH1D* hist, TCanvas* can) {
    TLine* line = new TLine(hist->GetXaxis()->GetBinLowEdge(1),1,hist->GetXaxis()->GetBinLowEdge(hist->FindBin(0.15)),1);
    line->SetLineWidth(2);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    can->cd();
    line->Draw();

}


void fixedPairs() {

    TH1D* hist[_N_CASES_][_N_PAIRS_][_N_HIST_-1][_N_EPSILON_];
    TFile *fileIn[_N_CASES_][_N_PAIRS_];
    TCanvas *can[_N_CASES_][_N_PAIRS_][_N_HIST_-1];
    TString entries[7] = {"#epsilon = 0.0","#epsilon = 0.1","#epsilon = 0.2","#epsilon = 0.3","#epsilon = 0.4","#epsilon = 0.5","#epsilon = 0.6"};

    //READING
    TFile *fileOut[_N_PAIRS_];
    for (int i = 0; i < _N_PAIRS_; i ++)
        fileOut[i] = new TFile(Form("./output/outputComp/fixedPairs%s.root",pairs[i].Data()), "RECREATE");
    
    for(int i = 0; i < _N_CASES_; i ++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
            fileIn[i][j] = new TFile(Form("./output/%s%s.root",cases[i].Data(),pairs[j].Data()));
            
            for (int k = 0; k < _N_EPSILON_; k++) {
                hist[i][j][0][k] = (TH1D*) fileIn[i][j]->Get(Form("%sE%d%sCorrFunc",cases[i].Data(),k,pairs[j].Data()));
                if(j == 0)
                   hist[i][j][1][k] = (TH1D*) fileIn[i][j]->Get(Form("%sE%d%sCorrFuncQsc",cases[i].Data(),k,pairs[j].Data()));
            }

        }

    }

    //DRAWING
    for(int i = 0; i < _N_CASES_; i ++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
            for (int k = 0; k < _N_EPSILON_; k++) {

                if (j == 0) {
                    hist[i][j][0][k]->Rebin(5);
                    hist[i][j][0][k]->Scale(0.2);
                    hist[i][j][1][k]->Rebin(5);
                    hist[i][j][1][k]->Scale(0.2);
                    
                    hist[i][j][1][k]->GetXaxis()->SetRangeUser(0.0,0.15);
                    
                }
                hist[i][j][0][k]->GetXaxis()->SetRangeUser(0.0,0.15);
            }

            
            can[i][j][0] = new TCanvas(Form("%s%sEpsilonComp",cases[i].Data(),pairs[j].Data()),Form("%s%sEpsilonComp",cases[i].Data(),pairs[j].Data()),800,800);
            if( j == 0) {
                multiplePlot(hist[i][j][0],can[i][j][0],entries,7,false);
                makePaveText(can[i][j][0],casesNice[i].Data(),0.55,0.3,0.7,0.35);
                makePaveText(can[i][j][0],pairsNice[j].Data(),0.55,0.35,0.7,0.4);
            }
            else {
                multiplePlot(hist[i][j][0],can[i][j][0],entries,7,true);
                makePaveText(can[i][j][0],casesNice[i].Data(),0.55,0.7,0.7,0.75);
                makePaveText(can[i][j][0],pairsNice[j].Data(),0.55,0.75,0.7,0.8);
            }

            drawLine(hist[i][j][0][0],can[i][j][0]);
            fileOut[j]->cd();
            can[i][j][0]->Write();

            if (j == 0) {
                can[i][j][1] = new TCanvas(Form("%s%sEpsilonCompQsc",cases[i].Data(),pairs[j].Data()),Form("%s%sEpsilonComp",cases[i].Data(),pairs[j].Data()),800,800);
                if(i == 0) {
                    multiplePlot(hist[i][j][1],can[i][j][1],entries,7,false);
                    makePaveText(can[i][j][1],casesNice[i].Data(),0.55,0.3,0.7,0.35);
                    makePaveText(can[i][j][1],pairsNice[j].Data(),0.55,0.35,0.7,0.4);
                }
                else {
                    multiplePlot(hist[i][j][1],can[i][j][1],entries,7,true);
                    makePaveText(can[i][j][1],casesNice[i].Data(),0.55,0.7,0.7,0.75);
                    makePaveText(can[i][j][1],pairsNice[j].Data(),0.55,0.75,0.7,0.8);
                }

                drawLine(hist[i][j][0][1],can[i][j][1]);
                fileOut[j]->cd();
                can[i][j][1]->Write();
            }

        }

    }

    for (int i = 0; i < _N_PAIRS_; i ++) {
        fileOut[i]->Save();
        fileOut[i]->Close();
    }





    
}

void compCorrFunc() {
    gROOT -> SetBatch(kTRUE);
    fixedPairs();
    gROOT -> SetBatch(kFALSE);
}
