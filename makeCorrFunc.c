#include <TH1D.h>
#include <TFile.h>
#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST_ 3


TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
//double gev = 1.0 / 0.1975;


Double_t fitFunction(Double_t *x, Double_t *par) { //  0 - normalizacja, 1 - lambda, 2 - R,
    Double_t lambda = par[1];
    Double_t exp = TMath::Exp(-par[2]*par[2]*x[0]*x[0]/0.038938);
    return par[0]*(1 + lambda * exp);

}

void makeCorrFunc() {
    TH1D* hist[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_HIST_-1];
    TFile *fileIn[_N_CASES_];
    TFile *fileOut[_N_CASES_][_N_PAIRS_];

    gROOT -> SetBatch(kTRUE);

    //READING
    for (int i = 0; i < _N_CASES_; i++) {
        fileIn[i] = new TFile(Form("./outputLustre/%s.root",cases[i].Data()));
        for (int j = 0; j < _N_PAIRS_; j++) {
            fileOut[i][j] = new TFile(Form("./outputCorrFunc/%s%s.root",cases[i].Data(),pairs[j].Data()), "RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {
                hist[i][j][k][0] = (TH1D*) fileIn[i]->Get(Form("%sE%d%snum1d",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k][0] = (TH1D*) hist[i][j][k][0]->Clone(Form("%sE%d%sCorrFunc",cases[i].Data(),k,pairs[j].Data()));
                hist[i][j][k][0]->Divide((TH1D*) fileIn[i]->Get(Form("%sE%d%sden1d",cases[i].Data(),k,pairs[j].Data())));
                hist[i][j][k][0]->SetTitle("");
                if(j == 0) { //pairs with qsc and ones needing rebinnning
                    hist[i][j][k][1] = (TH1D*) fileIn[i]->Get(Form("%sE%d%snum1dqsc",cases[i].Data(),k,pairs[j].Data()));
                    hist[i][j][k][1] = (TH1D*) hist[i][j][k][1]->Clone(Form("%sE%d%sCorrFuncQsc",cases[i].Data(),k,pairs[j].Data()));
                    hist[i][j][k][1]->Divide((TH1D*) fileIn[i]->Get(Form("%sE%d%sden1d",cases[i].Data(),k,pairs[j].Data())));
                    fileOut[i][j]->cd();
                    hist[i][j][k][1]->SetTitle("");
                    hist[i][j][k][1]->Rebin(5);
                    hist[i][j][k][1]->Scale(0.2);
                    hist[i][j][k][1]->Write();

                    hist[i][j][k][0]->Rebin(5);
                    hist[i][j][k][0]->Scale(0.2);



                }

            }

        }
    }


    //FUNCTION
    Int_t params = 3;
    TF1 *fun = new TF1("fun",fitFunction,0,0.5,params);  //0 - normalizacja, 1 - lambda, 2 - R
    fun->SetParameter(0,1);
    fun->SetParameter(1,0.5);
    fun->SetParameter(2,10);


    //FITTING
    Double_t lambdy[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t r[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t lambdyErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];

    Double_t xValues[_N_EPSILON_] = {0.0,0.1,0.2,0.3,0.4,0.5,0.6};
    Double_t xErr[_N_EPSILON_] = {0};
    Double_t minFit = 0.003;
    Double_t maxFit = 0.5;


    for(int i = 0; i < _N_CASES_; i ++) {
        for(int j = 0; j < _N_PAIRS_; j++) {
            for(int k = 0; k < _N_EPSILON_; k++) {

                hist[i][j][k][0]->Fit("fun","","",minFit,maxFit);
                fileOut[i][j]->cd();
                hist[i][j][k][0]->Write();

                lambdy[i][j][k] = fun->GetParameter(1);
                lambdyErr[i][j][k] = fun->GetParError(1);
                r[i][j][k] = fun->GetParameter(2);
                rErr[i][j][k] = fun->GetParError(2);

            }
            fileOut[i][j]->Close();
            fileOut[i][j]->Save();

        }
    }

    //TXT FILE WITH RESULTS

    ofstream filetxt;
    filetxt.open("./outputCorrFunc/fitResults.txt");

    for(int i = 0; i < _N_PAIRS_; i++) {
        filetxt<<pairs[i].Data()<<"\n";

        for(int j = 0; j <_N_CASES_; j++) {
            filetxt<<"\t"<<cases[j].Data()<<"\n";

            for(int k = 0; k < _N_EPSILON_; k++) {
                filetxt<<std::setprecision(3);
                filetxt<<"\t\tEpsilon = "<<k<<"\n";
                filetxt<<"\t\t\tlambda = "<<lambdy[j][i][k]<<" ("<<lambdyErr[j][i][k]<<")\t";
                filetxt<<"\tR = "<<r[j][i][k]<<" ("<<rErr[j][i][k]<<")\n";
            }

        }

    }

    //TXT FILE FOR LATEX

    ofstream filetxt2;
    filetxt2.open("./outputCorrFunc/fitResultsLatex.txt");

    for(int i = 0; i < _N_PAIRS_; i++) {
        filetxt2<<pairs[i].Data()<<"\n";
        filetxt2<<"\\begin{tabular}{|c|c|c|c|c|c|c|}"<<endl;
        filetxt2<<"\\hline\n\\hline\n";
        filetxt2<<"{} & \\multicolumn{2}{c|}{Case A} & \\multicolumn{2}{c|}{Case B}& \\multicolumn{2}{c|}{Case C}\\\\"<<endl;
        filetxt2<<"\\hline"<<endl;
        filetxt2<<"$\\epsilon$ & $\\lambda$ & R (fm) & $\\lambda$ & R (fm)& $\\lambda$ & R (fm)\\\\"<<endl;
        filetxt2<<"\\hline"<<endl;

        for(int k = 0; k < _N_EPSILON_; k++) {
            filetxt2<<"0."<<k;
            for(int j = 0; j <_N_CASES_; j++) {
                filetxt2<<std::setprecision(3);
                filetxt2<<"&"<<lambdy[j][i][k];//<<" ("<<lambdyErr[j][i][k]<<")";
                filetxt2<<"&"<<r[j][i][k];//<<" ("<<rErr[j][i][k]<<")";
            }
            filetxt2<<"\\\\"<<endl;

        }

        filetxt2<<"\\hline\n\\hline\n\\end{tabular}"<<endl;

        

    }

    /*\begin{tabular}{|c|c|c|c|}
    \hline
    \hline
    Parameter &  Case A & Case B & Case C\\
    \hline
    $T$ (MeV)  & $49.6$ & $70.3$ & $63.1$ \\
    $R$ (fm) & $ 15.7 $ & $6.06$ & $9.29$ \\
    $\mu_B$ (MeV)  & $776$ & $876$ & $782$ \\
    $\mu_S$ (MeV)  & $123.4$ & $198.3$ & $145.7$ \\
    $\mu_{I_3}$ (MeV) & $-14.1$ & $-21.5$ & $-24.5$ \\
    $\gamma_S$ &  $0.16$ & $0.05$ & $0.04$ \\
    $\chi^2/N_{\rm df}$  & $ N_{\rm df}=0 $ & $1.13/2$ & $62.30/5$ \\
    \hline
    \hline
    $H$ (GeV)  & $0.01$ & $0.0225$ & $0.016$ \\
    $\delta$ & $0.2$ & $0.4$ & $0.4$ \\
    $\sqrt{Q^2}$  & $0.238$ & $0.256$ & $0.323$ \\
    \hline
    \hline
    \end{tabular}*/

    //COMPARING FITTING RESULTS

   /* TGraphErrors *grLambda[_N_CASES_][_N_PAIRS_];
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
    fileOut2->Save();*/

    gROOT -> SetBatch(kFALSE);


}



