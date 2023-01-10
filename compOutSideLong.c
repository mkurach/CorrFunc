#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST3D_ 2
#define _N_PROJ_ 3

TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
TString axes[_N_PROJ_] = {"x","y","z"};
TString casesNice[_N_CASES_] = {"Case A","Case B", "Case C"};
TString pairsNice[_N_PAIRS_] = {"p-p","#pi^{+}-#pi^{+}","#pi^{-}-#pi^{-}"};
TString entries[7] = {"#epsilon = 0.0","#epsilon = 0.1","#epsilon = 0.2","#epsilon = 0.3","#epsilon = 0.4","#epsilon = 0.5","#epsilon = 0.6"};
TString axesNice[_N_PROJ_] = {"out","side","long"};

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

void drawLine(TH1D* hist) {
    TLine* line = new TLine(hist->GetXaxis()->GetBinLowEdge(hist->FindBin(0.0)),1,hist->GetXaxis()->GetBinLowEdge(hist->FindBin(0.25)),1);
    line->SetLineWidth(1);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw();

}

void multiplePlot(TH1D *histTab[],TH1D *fitTab[], TString* entry, size_t n, bool up) {
	
    Double_t max = 0;
    Double_t min = 0;

    for (int i = 0; i < n; i++) {
        if(histTab[i]->GetMaximum()>max)
            max = histTab[i]->GetMaximum();
        if(histTab[i]->GetMinimum()<min)
            min = histTab[i]->GetMinimum();
    }

 
    histTab[0]->GetYaxis()->SetRangeUser(0.4,1.15);
    histTab[0]->GetXaxis()->SetRangeUser(0,0.25);
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
    histTab[0]->GetXaxis()->SetNdivisions(10);

    


    //xpad->cd();
    for (int i = 0; i < n; i++) { 
        //if( i == 1 ||i == 2 ||i == 3 ||i == 4 ||i == 5 )     
          //  continue;  

		histTab[i]->SetMarkerStyle(21);
        histTab[i]->SetMarkerSize(0.6);
        histTab[i]->SetLineWidth(1);
        histTab[i]->SetMarkerColor(colors[i]);
        histTab[i]->SetLineColor(colors[i]);

        fitTab[i]->SetMarkerStyle(21);
        fitTab[i]->SetMarkerSize(0.6);
        fitTab[i]->SetLineWidth(1);
        fitTab[i]->SetMarkerColor(colors[i]);
        fitTab[i]->SetLineColor(colors[i]);


        
        legend->AddEntry(histTab[i],entry[i],"AP");

        if (i == 0) 
            histTab[i]->Draw();

            
        else
            histTab[i]->Draw("same");
        fitTab[i]->Draw("samehistl");

        
    }
    legend->Draw("same");



}
void multiplePlotPion(TH1D *histTab[],TH1D *fitTab[], TString* entry, size_t n, bool up) {
	
    Double_t max = 0;
    Double_t min = 0;

    for (int i = 0; i < n; i++) {
        if(histTab[i]->GetMaximum()>max)
            max = histTab[i]->GetMaximum();
        if(histTab[i]->GetMinimum()<min)
            min = histTab[i]->GetMinimum();
    }

 
    histTab[0]->GetYaxis()->SetRangeUser(0.9,1.15*max);
    histTab[0]->GetXaxis()->SetRangeUser(0,0.15);
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
    histTab[0]->GetXaxis()->SetNdivisions(10);

    


    //xpad->cd();
    for (int i = 0; i < n; i++) { 
        //if( i == 1 ||i == 2 ||i == 3 ||i == 4 ||i == 5 )     
          //  continue;  

		histTab[i]->SetMarkerStyle(21);
        histTab[i]->SetMarkerSize(0.6);
        histTab[i]->SetLineWidth(1);
        histTab[i]->SetMarkerColor(colors[i]);
        histTab[i]->SetLineColor(colors[i]);

        fitTab[i]->SetMarkerStyle(21);
        fitTab[i]->SetMarkerSize(0.6);
        fitTab[i]->SetLineWidth(1);
        fitTab[i]->SetMarkerColor(colors[i]);
        fitTab[i]->SetLineColor(colors[i]);


        
        legend->AddEntry(histTab[i],entry[i],"AP");

        if (i == 0) 
            histTab[i]->Draw();

            
        else
            histTab[i]->Draw("same");
        fitTab[i]->Draw("samehistl");

        
    }
    legend->Draw("same");



}

TVirtualPad*** GetGridPad(Int_t x_pads, Int_t y_pads, Float_t x_margin, Float_t y_margin, TString name) {
	gStyle->SetCanvasColor(0);
	gStyle->SetPadColor(kWhite);
    TPad* padsav = (TPad*) gPad;
	if (gPad == NULL) {
		TCanvas* c = new TCanvas(name,name,2400,800);
		c->cd();
		padsav = (TPad*) gPad;
	}
	TVirtualPad*** array2d = new TVirtualPad**[x_pads];
	for (int i = 0; i < x_pads; i++) {
  		array2d[i] = new TVirtualPad*[y_pads];
	}
	Double_t colls   = x_pads;
	Double_t rows    = y_pads;
	Double_t x_shift = 1.0 / (colls - 1 + 1 / (1 - TMath::Abs(x_margin)));
	Double_t y_shift = 1.0 / (rows - 1 + 1 / (1 - TMath::Abs(y_margin)));
	Double_t x_pad   = x_shift / (1.0 - TMath::Abs(x_margin));
	Double_t y_pad   = y_shift / (1.0 - TMath::Abs(y_margin));
	int glob         = 0;
	if (x_margin >= 0 && y_margin >= 0) {  // OK
  		for (int i = x_pads - 1; i >= 0; i--) {
    		for (int j = 0; j < y_pads; j++) {
				Double_t x1 = i * x_shift;
				Double_t x2 = x1 + x_pad;
				Double_t y1 = 1 - j * y_shift;
				Double_t y2 = y1 - y_pad;
				if (x1 < 0) x1 = 0;
				if (x1 > 1) x1 = 1;
				if (x2 < 0) x2 = 0;
				if (x2 > 1) x2 = 1;
				if (y1 < 0) y1 = 0;
				if (y1 > 1) y1 = 1;
				if (y2 < 0) y2 = 0;
				if (y2 > 1) y2 = 1;

				TPad* pad = new TPad(Form("pad_%i_%i", i, j), Form("pad_%i_%i", i, j), x1, y1, x2, y2);
				pad->SetTopMargin(0);
				pad->SetRightMargin(0);
				pad->SetBottomMargin(y_margin);
				pad->SetLeftMargin(x_margin);
				pad->SetNumber(glob);
				pad->Draw();
				array2d[i][j] = pad;
				glob++;
				padsav->cd();
    		}
  		}
	}
	if (x_margin >= 0 && y_margin < 0) {  // ok
 		y_margin = TMath::Abs(y_margin);
  		for (int i = x_pads - 1; i >= 0; i--) {
    		for (int j = y_pads - 1; j >= 0; j--) {
				Double_t x1 = i * x_shift;
				Double_t x2 = x1 + x_pad;
				Double_t y1 = 1 - j * y_shift;
				Double_t y2 = y1 - y_pad;
				if (x1 < 0) x1 = 0;
				if (x1 > 1) x1 = 1;
				if (x2 < 0) x2 = 0;
				if (x2 > 1) x2 = 1;
				if (y1 < 0) y1 = 0;
				if (y1 > 1) y1 = 1;
				if (y2 < 0) y2 = 0;
				if (y2 > 1) y2 = 1;
				TPad* pad = new TPad(Form("pad_%i_%i", i, j), Form("pad_%i_%i", i, j), x1, y1, x2, y2);
				pad->SetTopMargin(y_margin);
				pad->SetRightMargin(0);
				pad->SetBottomMargin(0);
				pad->SetLeftMargin(x_margin);
				pad->Draw();
				array2d[i][j] = pad;
				glob++;
				padsav->cd();
    		}
  		}
	}

	if (x_margin < 0 && y_margin < 0) {  // ok
		y_margin = TMath::Abs(y_margin);
		x_margin = TMath::Abs(x_margin);
		for (int i = 0; i < x_pads; i++) {
    		for (int j = y_pads - 1; j >= 0; j--) {
				Double_t x1 = i * x_shift;
				Double_t x2 = x1 + x_pad;
				Double_t y1 = 1 - j * y_shift;
				Double_t y2 = y1 - y_pad;
				if (x1 < 0) x1 = 0;
				if (x1 > 1) x1 = 1;
				if (x2 < 0) x2 = 0;
				if (x2 > 1) x2 = 1;
				if (y1 < 0) y1 = 0;
				if (y1 > 1) y1 = 1;
				if (y2 < 0) y2 = 0;
				if (y2 > 1) y2 = 1;
				TPad* pad = new TPad(Form("pad_%i_%i", i, j), Form("pad_%i_%i", i, j), x1, y1, x2, y2);
				pad->SetTopMargin(y_margin);
				pad->SetRightMargin(x_margin);
				pad->SetBottomMargin(0);
				pad->SetLeftMargin(0);
				pad->Draw();
				array2d[i][j] = pad;
				glob++;
				padsav->cd();
    		}
  		}		
	}

	if (x_margin < 0 && y_margin >= 0) {  // OK
		x_margin = TMath::Abs(x_margin);
		for (int i = 0; i < x_pads; i++) {
			for (int j = 0; j < y_pads; j++) {
				Double_t x1 = i * x_shift;
				Double_t x2 = x1 + x_pad;
				Double_t y1 = 1 - j * y_shift;
				Double_t y2 = y1 - y_pad;
				if (x1 < 0) x1 = 0;
				if (x1 > 1) x1 = 1;
				if (x2 < 0) x2 = 0;
				if (x2 > 1) x2 = 1;
				if (y1 < 0) y1 = 0;
				if (y1 > 1) y1 = 1;
				if (y2 < 0) y2 = 0;
				if (y2 > 1) y2 = 1;
				TPad* pad = new TPad(Form("pad_%i_%i", i, j), Form("pad_%i_%i", i, j), x1, y1, x2, y2);
				pad->SetTopMargin(0);
				pad->SetRightMargin(x_margin);
				pad->SetBottomMargin(y_margin);
				pad->SetLeftMargin(0);
				pad->Draw();
				array2d[i][j] = pad;
				glob++;
				padsav->cd();
			}
		}
	}

	return array2d;
}

void compOutSideLong() {

	gROOT -> SetBatch(kTRUE);
    TH1D* fitProj[_N_CASES_][_N_PAIRS_][_N_PROJ_][_N_EPSILON_];
    TH1D* histProj[_N_CASES_][_N_PAIRS_][_N_PROJ_][_N_EPSILON_];

    //READING
    TFile* fileFitProj[_N_CASES_][_N_PAIRS_];    
    TFile* fileHistProj[_N_CASES_][_N_PAIRS_];
    for(int i = 0; i < _N_CASES_; i ++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
        //for(int j = 0; j < 1; j++) {

            fileFitProj[i][j] = new TFile(Form("./outputCorrFunc3d/outputFitProj/%s%sFitProj.root",cases[i].Data(),pairs[j].Data()));
            fileHistProj[i][j] = new TFile(Form("./outputCorrFunc3d/outputHistProj/%s%sHistProj.root",cases[i].Data(),pairs[j].Data()));
            for(int k = 0; k < _N_PROJ_; k++) {
                for(int l = 0; l < _N_EPSILON_; l++) {
					if ((i==0 && j==2 && l==4) || (i==0 && j==2 && l==5) || (i==0 && j==2 && l==6))
						continue;
					else {
						fitProj[i][j][k][l] = (TH1D*) fileFitProj[i][j]->Get(Form("%sE%d%sFitProj%s",cases[i].Data(),l,pairs[j].Data(),axes[k].Data()));
						histProj[i][j][k][l] = (TH1D*) fileHistProj[i][j]->Get(Form("%sE%d%sHistProj%s",cases[i].Data(),l,pairs[j].Data(),axes[k].Data()));
					}
                }
            }
        }
    }


    //DRAWING

    TCanvas* can[_N_CASES_][_N_PAIRS_];
    TVirtualPad ***xpad[_N_CASES_][_N_PAIRS_];
	TFile *fileOut[_N_CASES_];

    for(int i = 0; i < _N_CASES_; i ++) {
		fileOut[i] = new TFile(Form("./outputOutSideLong/%s.root",cases[i].Data()), "RECREATE");
        for(int j = 0; j < _N_PAIRS_; j++) {
        //for(int j = 0; j < 1; j++) {
			
            can[i][j] = new TCanvas(Form("%s%sCompOutSideLong",cases[i].Data(),pairs[j].Data()),Form("%s%sCompOutSideLong",cases[i].Data(),pairs[j].Data()),2400,800);
            can[i][j]->cd();
            xpad[i][j] = GetGridPad(3,1,0.15,0.1,Form("%s%sCompOutSideLong",cases[i].Data(),pairs[j].Data()));
            
            
            for(int k = 0; k < _N_PROJ_; k++) {
                xpad[i][j][k][0]->cd();
                histProj[i][j][k][0]->GetYaxis()->SetTitle("C(q)");
				if (i==0 && j==2)
                    multiplePlotPion(histProj[i][j][k],fitProj[i][j][k], entries, 4, true);
				else if(j==1 || j==2)
					multiplePlotPion(histProj[i][j][k],fitProj[i][j][k], entries, 7, true);
                else
					multiplePlot(histProj[i][j][k],fitProj[i][j][k], entries, 7, false); 
                
                drawLine(histProj[i][j][k][0]);
                makePaveText(xpad[i][j][k][0],axesNice[k].Data(),0.75,0.5,0.9,0.55);

                if(k==0) {
					if(j==0) {
						makePaveText(xpad[i][j][k][0],casesNice[i].Data(),0.55,0.3,0.7,0.35);
						makePaveText(xpad[i][j][k][0],pairsNice[j].Data(),0.55,0.35,0.7,0.4);
					}
					else {
						makePaveText(xpad[i][j][k][0],casesNice[i].Data(),0.55,0.6,0.7,0.65);
						makePaveText(xpad[i][j][k][0],pairsNice[j].Data(),0.55,0.65,0.7,0.7);
					}

                }

				fileOut[i]->cd();
                can[i][j]->Write();
                can[i][j]->SaveAs(Form("./ladne/compOutSideLong/%s%s.pdf",cases[i].Data(),pairs[j].Data()));
            }
        }

		fileOut[i]->Close();
		fileOut[i]->Save();
    }

	gROOT -> SetBatch(kFALSE);












    




}


/*
    for(int i = 0; i < _N_CASES_; i ++) {
        //for(int j = 0; j < _N_PAIRS_; j++) {
        for(int j = 0; j < 1; j++) {
            for(int k = 0; k < _N_EPSILON_; k++) {
                for(int l = 0; l < _N_PROJ_; l++) {

                }
            }
        }
    }
*/