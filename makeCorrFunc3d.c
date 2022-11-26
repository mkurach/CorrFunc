#include <TH3D.h>
#include <TFile.h>
#include "HBTFit.h"
#include "THGlobal.h"
#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST3D_ 2

TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
double gev = 1 / 0.1975;

Double_t fitFunction3d (Double_t *x, Double_t *par)
{
    Double_t qosq = x[0]*x[0];
    Double_t qssq = x[1]*x[1];
    Double_t qlsq = x[2]*x[2];
    Double_t lam = par[1];

    Double_t gpart = exp((-par[2]*par[2]*qosq-par[3]*par[3]*qssq-par[4]*par[4]*qlsq)/0.038938);

    return (par[0] * (1 + lam*gpart));
}

void makeCorrFunc3d() {
    TH3D* hist[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_HIST3D_];
    TFile *fileIn[_N_CASES_];
    TFile *fileOut[_N_CASES_][_N_PAIRS_];

    //READING
    for (int i = 0; i < 1; i++) {
        fileIn[i] = new TFile(Form("./outputLustre/%s3d.root",cases[i].Data()));
        for (int j = 0; j < _N_PAIRS_; j++) {
            //fileOut[i][j] = new TFile(Form("./outputCorrFunc3d/%s%s.root",cases[i].Data(),pairs[j].Data()), "RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {
                hist[i][j][k][0] = (TH3D*) fileIn[i]->Get(Form("%sE%d%scnuma",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k][1] = (TH3D*) fileIn[i]->Get(Form("%sE%d%scdena",cases[i].Data(),k,pairs[j].Data()));
            }
        }
        cout<<"Finished reading of "<<cases[i].Data()<<endl;
    }

    //FUNCTION 3D
    HBTFit *hbtFit = new HBTFit();
    TF3 *fun3d = new TF3("fun3d",hbtFit, &HBTFit::fungek, -0.15, 0.15, -0.15, 0.15, -0.15, 0.15, 5);
    //0 - normalizacja, 1 - lambda, 2 - Rout, 3 - Rside, 4 - Rlong 

    fun3d->SetParameter(0,1);
    fun3d->SetParameter(1,0.5);
    fun3d->SetParameter(2,1);
    fun3d->SetParameter(3,1);
    fun3d->SetParameter(4,1);

    //CORR FUNC AND FITTING 3D
    TH3D* histCorr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t lambdy[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rOut[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rSide[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rLong[_N_CASES_][_N_PAIRS_][_N_EPSILON_];

    Double_t lambdyErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rOutErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rSideErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rLongErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < _N_PAIRS_; j++) {
            //fileOut[i][j] = new TFile(Form("./outputCorrFunc3d/%s%s.root",cases[i].Data(),pairs[j].Data()), "RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {
                histCorr[i][j][k] = new TH3D(*hist[i][j][k][0]);
                histCorr[i][j][k]->Divide(hist[i][j][k][0],hist[i][j][k][1],1.0,1.0);
                histCorr[i][j][k]->SetTitle("corr func");
                histCorr[i][j][k]->Fit(fun3d,"RBMPE");

                lambdy[i][j][k] = fun3d->GetParameter(1);
                rOut[i][j][k] = fun3d->GetParameter(2);
                rSide[i][j][k] = fun3d->GetParameter(3);
                rLong[i][j][k] = fun3d->GetParameter(4);

                lambdyErr[i][j][k] = fun3d->GetParError(1);
                rOutErr[i][j][k] = fun3d->GetParError(2);
                rSideErr[i][j][k] = fun3d->GetParError(3);
                rLongErr[i][j][k] = fun3d->GetParError(4);
                   // fileOut[i][j]->cd();
                //histCorr[i][j][k]->Write();

            }
            //fileOut[i][j]->Close();
            //fileOut[i][j]->Save();
        }
        cout<<"Finished corr func of "<<cases[i].Data()<<endl;
    }

    //TXT FILE WITH RESULTS
    ofstream filetxt;
    filetxt.open("./outputCorrFunc3d/fitResults.txt");


    for(int i = 0; i < _N_PAIRS_; i++) {
        filetxt<<pairs[i].Data()<<"\n";
        for(int j = 0; j < 1; j++) {
            filetxt<<"\t"<<cases[j].Data()<<"\n";
            for(int k = 0; k < _N_EPSILON_; k++) {
                filetxt<<"\t\tEpsilon = "<<k<<"\n";
                filetxt<<"\t\t\tlambda = "<<lambdy[j][i][k];
                filetxt<<"\tRout = "<<rOut[j][i][k];
                filetxt<<"\tRside = "<<rSide[j][i][k];
                filetxt<<"\tRlong = "<<rLong[j][i][k]<<"\n";
            }

        }

    }
    filetxt.close();



}