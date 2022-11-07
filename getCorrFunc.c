#include <TH1D.h>
#include <TFile.h>
#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST_ 3


TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString hubb[_N_CASES_] = {"H100","H225","H165"};
TString delt[_N_CASES_] = {"D2","D4","D4"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
TString hists[_N_HIST_] = {"num1d","den1d","num1dqsc"};

void readLustre(){
    TH1D* hist[_N_CASES_][_N_EPSILON_][_N_PAIRS_][_N_HIST_];
    TFile* file[_N_CASES_][_N_EPSILON_][_N_PAIRS_];
    TFile *fileOut[_N_CASES_];
    
    for(int i = 0; i < _N_CASES_; i++) {
        fileOut[i] = new TFile(Form("./output/%s.root",cases[i].Data()), "RECREATE");
        for(int j = 0; j < _N_EPSILON_; j++) {
            for(int k = 0; k < _N_PAIRS_; k++) {
                file[i][j][k] = new TFile(Form("../lustre/hades/user/mkurach/inz/%s/%sE%d%s/femto%s19a.root",cases[i].Data(),hubb[i].Data(),j,delt[i].Data(),pairs[k].Data()));
                for (int l = 0; l < _N_HIST_; l++) {
                    if((l == 2 && k == 1) || (l == 2 && k == 2)) //pairs without qsc
                        continue;
                    else {
                        hist[i][j][k][l] = (TH1D*) file[i][j][k]->Get(hists[l].Data());
                        hist[i][j][k][l] = (TH1D*)hist[i][j][k][l]->Clone(Form("%sE%d%s%s",cases[i].Data(),j,pairs[k].Data(),hists[l].Data()));
                        fileOut[i]->cd();
                        hist[i][j][k][l]->Write();
                    }
                }
            }
        }
        fileOut[i]->Save();
        fileOut[i]->Close();
    }

}

void makeCorrFunc() {
    TH1D* hist[_N_CASES_][_N_EPSILON_][_N_PAIRS_][_N_HIST_-1];
    TFile *fileIn[_N_CASES_];
    TFile *fileOut[_N_CASES_];

    for(int i = 0; i < _N_CASES_; i++) {
        fileIn[i] = new TFile(Form("./output/%s.root",cases[i].Data()));
        fileOut[i] = new TFile(Form("./output/%sCorrFun.root",cases[i].Data()), "RECREATE");
        for(int j = 0; j < _N_EPSILON_; j++) {
            for(int k = 0; k < _N_PAIRS_; k++) { 

                hist[i][j][k][0] = (TH1D*) fileIn[i]->Get(Form("%sE%d%snum1d",cases[i].Data(),j,pairs[k].Data()));
                hist[i][j][k][0] = (TH1D*) hist[i][j][k][0]->Clone(Form("%sE%d%sCorrFunc",cases[i].Data(),j,pairs[k].Data()));
                hist[i][j][k][0]->Divide((TH1D*) fileIn[i]->Get(Form("%sE%d%sden1d",cases[i].Data(),j,pairs[k].Data())));
                fileOut[i]->cd();
                hist[i][j][k][0]->SetTitle("");
                hist[i][j][k][0]->Write();
                if(k == 0) { //pairs with qsc
                    hist[i][j][k][1] = (TH1D*) fileIn[i]->Get(Form("%sE%d%snum1dqsc",cases[i].Data(),j,pairs[k].Data()));
                    hist[i][j][k][1] = (TH1D*) hist[i][j][k][1]->Clone(Form("%sE%d%sCorrFuncQsc",cases[i].Data(),j,pairs[k].Data()));
                    hist[i][j][k][1]->Divide((TH1D*) fileIn[i]->Get(Form("%sE%d%sden1d",cases[i].Data(),j,pairs[k].Data())));
                    fileOut[i]->cd();
                    hist[i][j][k][1]->SetTitle("");
                    hist[i][j][k][1]->Write();
                }

                
            }
        }
        fileIn[i]->Close();
        fileOut[i]->Save();
        fileOut[i]->Close();
    }



}

void makeCorrFuncPairs() {
    TH1D* hist[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_HIST_-1];
    TFile *fileIn[_N_CASES_];
    TFile *fileOut[_N_CASES_][_N_PAIRS_];

    for (int i = 0; i < _N_CASES_; i++) {
        fileIn[i] = new TFile(Form("./output/%s.root",cases[i].Data()));
        for (int j = 0; j < _N_PAIRS_; j++) {
            fileOut[i][j] = new TFile(Form("./output/%s%s.root",cases[i].Data(),pairs[j].Data()), "RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {
                hist[i][j][k][0] = (TH1D*) fileIn[i]->Get(Form("%sE%d%snum1d",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k][0] = (TH1D*) hist[i][j][k][0]->Clone(Form("%sE%d%sCorrFunc",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k][0]->Divide((TH1D*) fileIn[i]->Get(Form("%sE%d%sden1d",cases[i].Data(),k,pairs[j].Data())));
                fileOut[i][j]->cd();
                hist[i][j][k][0]->SetTitle("");
                hist[i][j][k][0]->Write();
                if(j == 0) { //pairs with qsc
                    hist[i][j][k][1] = (TH1D*) fileIn[i]->Get(Form("%sE%d%snum1dqsc",cases[i].Data(),k,pairs[j].Data()));
                    hist[i][j][k][1] = (TH1D*) hist[i][j][k][1]->Clone(Form("%sE%d%sCorrFuncQsc",cases[i].Data(),k,pairs[j].Data()));
                    hist[i][j][k][1]->Divide((TH1D*) fileIn[i]->Get(Form("%sE%d%sden1d",cases[i].Data(),k,pairs[j].Data())));
                    fileOut[i][j]->cd();
                    hist[i][j][k][1]->SetTitle("");
                    hist[i][j][k][1]->Write();
                }

            }
            fileOut[i][j]->Close();
            fileOut[i][j]->Save();


        }
        fileIn[i]->Close();
    }

}


void getCorrFunc() {
    //readLustre();
    //makeCorrFunc();
    //makeCorrFuncPairs();


}