#include <TH3D.h>
#include <TFile.h>
#include "HBTFit.h"
#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST3D_ 2

TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
double gev = 1 / 0.1975;


void makeCorrFunc3d() {
    TH3D* hist[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    TFile *fileIn[_N_CASES_];
    TFile *fileOut[_N_CASES_][_N_PAIRS_];

    //READING
    for (int i = 0; i < 1; i++) {
        fileIn[i] = new TFile(Form("./outputLustre/%s3d.root",cases[i].Data()));
        for (int j = 0; j < _N_PAIRS_; j++) {
            fileOut[i][j] = new TFile(Form("./outputCorrFunc3d/%s%s.root",cases[i].Data(),pairs[j].Data()), "RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {
                hist[i][j][k] = (TH3D*) fileIn[i]->Get(Form("%sE%d%scnuma",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k] = (TH3D*) hist[i][j][k]->Clone(Form("%sE%d%sCorrFunc",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k]->Divide((TH3D*) fileIn[i]->Get(Form("%sE%d%scdena",cases[i].Data(),k,pairs[j].Data())));
                hist[i][j][k]->SetTitle("");

            }
        }
        cout<<"End of "<<cases[i].Data()<<endl;
    }

    //hist[0][0][0]->Draw();



    HBTFit *hbtFit = new HBTFit();

}