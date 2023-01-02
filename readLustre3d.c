#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST_ 3
#define _N_HIST3D_ 2

TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString hubb[_N_CASES_] = {"H100","H225","H165"};
TString ending[_N_PAIRS_] = {"prot","",""};
TString delt[_N_CASES_] = {"D2","D4","D4"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
TString hists3d[_N_HIST3D_] = {"cnuma","cdena"};

void readLustre3d() {
    
    TFile* file[_N_CASES_][_N_EPSILON_][_N_PAIRS_];
    TH3D* hist3d[_N_CASES_][_N_EPSILON_][_N_PAIRS_][_N_HIST3D_];
    TFile* fileOut3d[_N_CASES_];

    for(int i = 0; i < _N_CASES_; i++) {
        fileOut3d[i] = new TFile(Form("./outputLustre3d/%s3d.root",cases[i].Data()), "RECREATE");
        for(int j = 0; j < _N_EPSILON_; j++) {
            //for(int k = 0; k < _N_PAIRS_; k++) {
            for(int k = 0; k < 1; k++) {
                file[i][j][k] = new TFile(Form("../lustre/hades/user/mkurach/inz/%sLinks/%sE%d%s%s/femto%s19a.root",cases[i].Data(),hubb[i].Data(),j,delt[i].Data(),ending[k].Data(),pairs[k].Data()));
                for(int l = 0; l < _N_HIST3D_; l++) {
                    hist3d[i][j][k][l] = (TH3D*) file[i][j][k]->Get(hists3d[l].Data());
                    hist3d[i][j][k][l] = (TH3D*)hist3d[i][j][k][l]->Clone(Form("%sE%d%s%s",cases[i].Data(),j,pairs[k].Data(),hists3d[l].Data()));
                    fileOut3d[i]->cd();
                    hist3d[i][j][k][l]->Write();
                }
            }
            
        }
        fileOut3d[i]->Save();
        fileOut3d[i]->Close();
        cout<<"End of "<<cases[i].Data()<<endl;

    }

    
}