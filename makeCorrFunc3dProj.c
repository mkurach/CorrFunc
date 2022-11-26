#include <TH3D.h>
#include <TFile.h>
#include "HBTFit.h"
#include "THGlobal.h"
#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST3D_ 2
#define _N_PROJ_ 3

TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
double gev = 1 / 0.1975;



void makeCorrFunc3dProj() {
    TH3D* hist[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_HIST3D_];
    TFile *fileIn[_N_CASES_];

    //READING
    for (int i = 0; i < _N_CASES_; i++) {
        fileIn[i] = new TFile(Form("./outputLustre/%s3d.root",cases[i].Data()));
        for (int j = 0; j < _N_PAIRS_; j++) {
            for(int k = 0; k < _N_EPSILON_; k++) {
                hist[i][j][k][0] = (TH3D*) fileIn[i]->Get(Form("%sE%d%scnuma",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k][1] = (TH3D*) fileIn[i]->Get(Form("%sE%d%scdena",cases[i].Data(),k,pairs[j].Data()));
            }
        }
        cout<<"Finished reading of "<<cases[i].Data()<<endl;
    }

    //MAKING PROJ
    TH1D* histProj[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];
    HBTFit *hbtFit = new HBTFit();
    for(int i = 0; i < _N_CASES_; i++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
            for(int k = 0; k < _N_EPSILON_; k++) {
                for(int l = 0; l < _N_PROJ_; l++) {
                    histProj[i][j][k][l] = hbtFit->getproj(hist[i][j][k][0],hist[i][j][k][1],l,50,1.0);
                }
            }
        }
    }

    //FUNCTION
    TF1 *fun = new TF1("funqg1",hbtFit, &HBTFit::fungek1D, -0.15, 0.15, 3);
    //0 - normalizacja, 1 - lambda, 2 - R
    fun->SetParameter(0,1);
    fun->SetParameter(1,0.5);
    fun->SetParameter(2,1);

    //FITTING
    TFile *fileOut[_N_CASES_][_N_PAIRS_];

    Double_t lambdy[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];
    Double_t r[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];

    Double_t lambdyErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];
    Double_t rErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];

    for(int i = 0; i < _N_CASES_; i++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
            fileOut[i][j] = new TFile(Form("./outputCorrFunc3dProj/%s%s.root",cases[i].Data(),pairs[j].Data()), "RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {
                for(int l = 0; l < _N_PROJ_; l++) {
                    histProj[i][j][k][l]->Fit(fun,"");
                    
                    lambdy[i][j][k][l] = fun->GetParameter(1);
                    r[i][j][k][l] = fun->GetParameter(2);

                    lambdyErr[i][j][k][l] = fun->GetParError(1);
                    rErr[i][j][k][l] = fun->GetParError(2);

                    fileOut[i][j]->cd();
                    histProj[i][j][k][l]->Write();
                }
            }
            fileOut[i][j]->Close();
            fileOut[i][j]->Save();
        }
    }

    //TXT FILE WITH RESULTS
    ofstream filetxt;
    filetxt.open("./outputCorrFunc3dProj/fitResults.txt");


    for(int i = 0; i < _N_PAIRS_; i++) {
        filetxt<<pairs[i].Data()<<"\n";
        for(int j = 0; j < _N_CASES_; j++) {
            filetxt<<"\t"<<cases[j].Data()<<"\n";
            for(int k = 0; k < _N_EPSILON_; k++) {
                filetxt<<"\t\tEpsilon = "<<k<<"\n";
                for(int l = 0; l < _N_PROJ_; l++) {
                    filetxt<<"\t\t\tProj = "<<l;
                    filetxt<<"\tlambda = "<<lambdy[j][i][k][l];
                    filetxt<<"\tR = "<<r[j][i][k][l]<<"\n";
                }

            }

        }

    }
    filetxt.close();


    



}