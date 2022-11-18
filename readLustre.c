#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST_ 3
#define _N_HIST3D_ 2


TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString hubb[_N_CASES_] = {"H100","H225","H165"};
TString delt[_N_CASES_] = {"D2","D4","D4"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
TString hists[_N_HIST_] = {"num1d","den1d","num1dqsc"};
TString hists3d[_N_HIST3D_] = {"cnuma","cdena"};


void readLustre(){
    TH1D* hist[_N_CASES_][_N_EPSILON_][_N_PAIRS_][_N_HIST_];
    TFile* file[_N_CASES_][_N_EPSILON_][_N_PAIRS_];
    TFile *fileOut[_N_CASES_];

    TH3D* hist3d[_N_CASES_][_N_EPSILON_][_N_PAIRS_][_N_HIST3D_];
    TFile* fileOut3d[_N_CASES_];
    
    for(int i = 0; i < _N_CASES_; i++) {
        fileOut[i] = new TFile(Form("./outputLustre/%s.root",cases[i].Data()), "RECREATE");
        fileOut3d[i] = new TFile(Form("./outputLustre/%s3d.root",cases[i].Data()), "RECREATE");
        for(int j = 0; j < _N_EPSILON_; j++) {
            for(int k = 0; k < _N_PAIRS_; k++) {
                file[i][j][k] = new TFile(Form("../lustre/hades/user/mkurach/inz/%s/%sE%d%s/femto%s19a.root",cases[i].Data(),hubb[i].Data(),j,delt[i].Data(),pairs[k].Data()));
                //1D
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

                //3D
                for(int l = 0; l < _N_HIST3D_; l++) {
                    hist3d[i][j][k][l] = (TH3D*) file[i][j][k]->Get(hists3d[l].Data());
                    hist3d[i][j][k][l] = (TH3D*)hist3d[i][j][k][l]->Clone(Form("%sE%d%s%s",cases[i].Data(),j,pairs[k].Data(),hists3d[l].Data()));
                    fileOut3d[i]->cd();
                    hist3d[i][j][k][l]->Write();
                }
            }
        }
        fileOut[i]->Save();
        fileOut[i]->Close();
        fileOut3d[i]->Save();
        fileOut3d[i]->Close();
        cout<<"End of "<<cases[i].Data()<<endl;

    }

}