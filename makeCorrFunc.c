#include <TH1D.h>
#include <TFile.h>
#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST_ 3


TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
//TString hubb[_N_CASES_] = {"H100","H225","H165"};
//TString delt[_N_CASES_] = {"D2","D4","D4"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
//TString hists[_N_HIST_] = {"num1d","den1d","num1dqsc"};



Double_t fitFunction(Double_t *x, Double_t *par) { // 0 - lambda, 1 - R
    if(x[0] >= 0)
        return 1+par[0]*TMath::Exp(-par[1]*par[1]*x[0]*x[0]);
    else    
        return 0;
}

void makeCorrFunc() {
    TH1D* hist[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_HIST_-1];
    TFile *fileIn[_N_CASES_];
    TFile *fileOut[_N_CASES_][_N_PAIRS_];

    //READING

    for (int i = 0; i < _N_CASES_; i++) {
        fileIn[i] = new TFile(Form("./outputLustre/%s.root",cases[i].Data()));
        for (int j = 0; j < _N_PAIRS_; j++) {
            fileOut[i][j] = new TFile(Form("./outputCorrFunc/%s%s.root",cases[i].Data(),pairs[j].Data()), "RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {
                hist[i][j][k][0] = (TH1D*) fileIn[i]->Get(Form("%sE%d%snum1d",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k][0] = (TH1D*) hist[i][j][k][0]->Clone(Form("%sE%d%sCorrFunc",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k][0]->Divide((TH1D*) fileIn[i]->Get(Form("%sE%d%sden1d",cases[i].Data(),k,pairs[j].Data())));
                //fileOut[i][j]->cd();
                hist[i][j][k][0]->SetTitle("");
                //hist[i][j][k][0]->Write();
                if(j == 0) { //pairs with qsc
                    hist[i][j][k][1] = (TH1D*) fileIn[i]->Get(Form("%sE%d%snum1dqsc",cases[i].Data(),k,pairs[j].Data()));
                    hist[i][j][k][1] = (TH1D*) hist[i][j][k][1]->Clone(Form("%sE%d%sCorrFuncQsc",cases[i].Data(),k,pairs[j].Data()));
                    hist[i][j][k][1]->Divide((TH1D*) fileIn[i]->Get(Form("%sE%d%sden1d",cases[i].Data(),k,pairs[j].Data())));
                    fileOut[i][j]->cd();
                    hist[i][j][k][1]->SetTitle("");
                    hist[i][j][k][1]->Write();
                }

            }
            //fileOut[i][j]->Close();
            //fileOut[i][j]->Save();


        }
        //fileIn[i]->Close();
    }


    //FUNCTION
    Int_t params = 2;
    TF1 *fun = new TF1("fun",fitFunction,0,0.5,params);  //0 - lambda, 1 - R
    fun->SetParameter(0,0.5);
    fun->SetParameter(1,10);

    //FITTING
    Double_t lambdy[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t r[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t lambdyErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];

    Double_t xValues[_N_EPSILON_] = {0.0,0.1,0.2,0.3,0.4,0.5,0.6};
    Double_t xErr[_N_EPSILON_] = {0};
    Double_t minFit = 0;
    Double_t maxFit = 0.5;


    for(int i = 0; i < _N_CASES_; i ++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
            for(int k = 0; k < _N_EPSILON_; k++) {
                hist[i][j][k][0]->Fit("fun","","",minFit,maxFit);
                fileOut[i][j]->cd();
                hist[i][j][k][0]->Write();

                lambdy[i][j][k] = fun->GetParameter(0);
                lambdyErr[i][j][k] = fun->GetParError(0);
                r[i][j][k] = fun->GetParameter(1);
                r[i][j][k] = fun->GetParError(1);
            }
            fileOut[i][j]->Close();
            fileOut[i][j]->Save();

        }
    }

    //COMPARING FITTING RESULTS
    TGraphErrors *grLambda[_N_CASES_][_N_PAIRS_];
    TGraphErrors *grR[_N_CASES_][_N_PAIRS_];


    TFile *fileOut2 = new TFile("./outputCorrFunc/fitResults.root", "RECREATE");
    for(int i = 0; i < _N_CASES_; i++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
            grLambda[i][j] = new TGraphErrors(_N_EPSILON_,xValues,lambdy[i][j],xErr,lambdyErr[i][j]);
            grR[i][j] = new TGraphErrors(_N_EPSILON_,xValues,r[i][j],xErr,rErr[i][j]);

            grLambda[i][j]->SetTitle(Form("%s%s: lambda",cases[i].Data(),pairs[j].Data()));
            grR[i][j]->SetTitle(Form("%s%s: r",cases[i].Data(),pairs[j].Data()));

            fileOut2->cd();
            grLambda[i][j]->Write();
            grR[i][j]->Write();
        }

    }

    fileOut2->Close();
    fileOut2->Save();




}



