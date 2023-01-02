#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST3D_ 2
#define _N_PROJ_ 3

TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
TString axes[_N_PROJ_] = {"x","y","z"};
TString casesNice[_N_CASES_] = {"Case A","Case B", "Case C"};

void compOutSideLong() {
    TH1D* fitProj[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];
    TH1D* histProj[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];

    //READING
    TFile* fileFitProj[_N_CASES_][_N_PAIRS_];    
    TFile* fileHistProj[_N_CASES_][_N_PAIRS_];
    for(int i = 0; i < _N_CASES_; i ++) {
        //for(int j = 0; j < _N_PAIRS_; j++) {
        for(int j = 0; j < 1; j++) {

            fileFitProj[i][j] = new TFile(Form("./outputCorrFunc3d/outputFitProj/%s%sFitProj.root",cases[i].Data(),pairs[j].Data()));
            fileHistProj[i][j] = new TFile(Form("./outputCorrFunc3d/outputHistProj/%s%sHistProj.root",cases[i].Data(),pairs[j].Data()));
            for(int k = 0; k < _N_EPSILON_; k++) {
                for(int l = 0; l < _N_PROJ_; l++) {
                    fitProj[i][j][k][l] = (TH1D*) fileFitProj[i][j]->Get(Form("%sE%d%sFitProj%s",cases[i].Data(),k,pairs[j].Data(),axes[l].Data()));
                    histProj[i][j][k][l] = (TH1D*) fileHistProj[i][j]->Get(Form("%sE%d%sHistProj%s",cases[i].Data(),k,pairs[j].Data(),axes[l].Data()));

                }
            }
        }
    }

    




}


/*
    TFile* fileFitProj[_N_CASES_][_N_PAIRS_];    
    TFile* fileHistProj[_N_CASES_][_N_PAIRS_];
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