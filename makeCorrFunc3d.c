#include "HBTFit.h"
#define _N_CASES_ 3
#define _N_EPSILON_ 7
#define _N_PAIRS_ 3
#define _N_HIST3D_ 2
#define _N_PROJ_ 3

TString cases[_N_CASES_] = {"caseA","caseB","caseC"};
TString pairs[_N_PAIRS_] = {"pp","pipi","piMpiM"};
TString axes[_N_PROJ_] = {"x","y","z"};
TString casesNice[_N_CASES_] = {"Case A","Case B", "Case C"};
double gev = 1 / 0.1975;

TF3 *fun3d;

void getfitprojc(TH3D *expden, TH3D **projhist)
{
  (*projhist) = new TH3D(*expden);
  //(*projhist)->SetName(Form("%sE%d%sfitProj%s",cases[i].Data(),k,pairs[j].Data(),axes[l].Data()));

  for (int q1int = 1; q1int <= expden->GetNbinsX(); q1int++)
    for (int q2int = 1; q2int <= expden->GetNbinsY(); q2int++) 
      for (int q3int = 1; q3int <= expden->GetNbinsZ(); q3int++) {
	(*projhist)->SetBinContent(q1int, q2int, q3int,
				   expden->GetBinContent(q1int, q2int, q3int) *
				   fun3d->Eval(expden->GetXaxis()->GetBinCenter(q1int),
					       expden->GetYaxis()->GetBinCenter(q2int),
					       expden->GetZaxis()->GetBinCenter(q3int)));
      }
}

void makeCorrFunc3d() {
    TH3D* num[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    TH3D *den[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    TFile* file[_N_CASES_];

    //READING
    for(int i = 0; i < _N_CASES_; i++) {
        file[i] = new TFile(Form("./outputLustre3d/%s3d.root",cases[i].Data()));
        for (int j = 0; j < _N_PAIRS_; j++) {
        //for (int j = 0; j < 1; j++) {
            for(int k = 0; k < _N_EPSILON_; k++) {
                num[i][j][k] = (TH3D*) file[i]->Get(Form("%sE%d%scnuma",cases[i].Data(),k,pairs[j].Data()));
                den[i][j][k] = (TH3D*) file[i]->Get(Form("%sE%d%scdena",cases[i].Data(),k,pairs[j].Data()));
                
            }
        }
    }

    //FUNCTION 3D
    HBTFit *hbtFit = new HBTFit();
    fun3d = new TF3("fun3d",hbtFit, &HBTFit::fungek, 0, 0.15, 0, 0.15, 0, 0.15, 5);
    //0 - normalizacja, 1 - lambda, 2 - Rout, 3 - Rside, 4 - Rlong 

    fun3d->SetParameter(0,1);
    fun3d->SetParameter(1,0.5);
    fun3d->SetParameter(2,10);
    fun3d->SetParameter(3,10);
    fun3d->SetParameter(4,10);

    //CORR FUNC AND FITTING 3D

    Double_t lambdy[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rOut[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rSide[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rLong[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t norm[_N_CASES_][_N_PAIRS_][_N_EPSILON_];

    Double_t lambdyErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rOutErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rSideErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    Double_t rLongErr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];



    TFile *fileOut3d[_N_CASES_][_N_PAIRS_];
    TH3D* histCorr[_N_CASES_][_N_PAIRS_][_N_EPSILON_];

    TH3D* fit[_N_CASES_][_N_PAIRS_][_N_EPSILON_];
    TH1D* fitProj[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];
    TFile* fileOutFitProj[_N_CASES_][_N_PAIRS_];

    for(int i = 0; i < _N_CASES_; i++) {
        cout<<cases[i].Data()<<endl;
        for (int j = 0; j < _N_PAIRS_; j++) {
        //for (int j = 0; j < 1; j++) {
            cout<<pairs[j].Data()<<endl;
            fileOut3d[i][j] = new TFile(Form("./outputCorrFunc3d/%s%s3d.root",cases[i].Data(),pairs[j].Data()),"RECREATE");
            fileOutFitProj[i][j] = new TFile(Form("./outputCorrFunc3d/outputFitProj/%s%sFitProj.root",cases[i].Data(),pairs[j].Data()),"RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {
                cout<<k<<endl;

                histCorr[i][j][k] = new TH3D(*num[i][j][k]);
                histCorr[i][j][k]->Divide(num[i][j][k],den[i][j][k],1.0,1.0);
                histCorr[i][j][k]->SetTitle("corr func");
                histCorr[i][j][k]->Fit(fun3d,"");

                norm[i][j][k] = fun3d->GetParameter(0);
                lambdy[i][j][k] = fun3d->GetParameter(1);
                rOut[i][j][k] = fun3d->GetParameter(2);
                rSide[i][j][k] = fun3d->GetParameter(3);
                rLong[i][j][k] = fun3d->GetParameter(4);

                lambdyErr[i][j][k] = fun3d->GetParError(1);
                rOutErr[i][j][k] = fun3d->GetParError(2);
                rSideErr[i][j][k] = fun3d->GetParError(3);
                rLongErr[i][j][k] = fun3d->GetParError(4);
                
                getfitprojc(den[i][j][k],&fit[i][j][k]);

                fileOut3d[i][j]->cd();
                histCorr[i][j][k]->Write();

                for(int l = 0; l < _N_PROJ_; l++) {
                    fitProj[i][j][k][l] = hbtFit->getproj(fit[i][j][k],den[i][j][k],l,10,norm[i][j][k]);
                    fitProj[i][j][k][l]->SetName(Form("%sE%d%sFitProj%s",cases[i].Data(),k,pairs[j].Data(),axes[l].Data()));
                    fileOutFitProj[i][j]->cd();
                    fitProj[i][j][k][l]->Write();
                }


            }
            fileOut3d[i][j]->Save();
            fileOut3d[i][j]->Close();
            fileOutFitProj[i][j]->Save();
            fileOutFitProj[i][j]->Close();

        }
    }


    //TXT FILE WITH RESULTS
    ofstream filetxt;
    filetxt.open("./outputCorrFunc3d/fitResults.txt");

    for(int i = 0; i < _N_CASES_; i++) {
        filetxt<<cases[i].Data()<<endl;
        for (int j = 0; j < _N_PAIRS_; j++) {
        //for (int j = 0; j < 1; j++) {
            filetxt<<pairs[j].Data()<<endl;
            for(int k = 0; k < _N_EPSILON_; k++) {
                filetxt<<"epsilon="<<k<<endl;
                filetxt<<"\t\tlambda = "<<lambdy[i][j][k];
                filetxt<<"\tRout = "<<rOut[i][j][k];
                filetxt<<"\tRside = "<<rSide[i][j][k];
                filetxt<<"\tRlong = "<<rLong[i][j][k]<<"\n";
   
            }
        }
    }

    filetxt.close();

    //TXT FILE FOR LATEX

    ofstream filetxt2;
    filetxt2.open("./outputCorrFunc3d/fitResultsLatex.txt");

    for(int i = 0; i < _N_PAIRS_; i++) {
    //for (int i = 0; i < 1; i++) {
        for(int j = 0; j <_N_CASES_; j++) {
            filetxt2<<pairs[i].Data()<<"\n";
            filetxt2<<"\\begin{tabular}{|c|c|c|c|c|}"<<endl;
            filetxt2<<"\\hline\n\\hline\n";
            filetxt2<<"\\multicolumn{5}{|c|}{"<<casesNice[j].Data()<<"}\\\\"<<endl;
            filetxt2<<"\\hline"<<endl;
            filetxt2<<"$\\epsilon$ & $\\lambda$ & $R_{out}$ (fm) & $R_{side}$ (fm) & $R_{long}$ (fm) \\\\"<<endl;
            filetxt2<<"\\hline"<<endl;

            for(int k = 0; k < _N_EPSILON_; k++) {
                filetxt2<<"0."<<k;
                filetxt2<<std::setprecision(3);
                filetxt2<<"&"<<lambdy[j][i][k];//<<" ("<<lambdyErr[j][i][k]<<")";
                filetxt2<<"&"<<rOut[j][i][k];//<<" ("<<rErr[j][i][k]<<")";
                filetxt2<<"&"<<rSide[j][i][k];
                filetxt2<<"&"<<rLong[j][i][k];
                
                filetxt2<<"\\\\"<<endl;


            }

            filetxt2<<"\\hline\n\\hline\n\\end{tabular}"<<endl;

        
        }
    }

    //MAKING PROJ

    TH1D* histProj[_N_CASES_][_N_PAIRS_][_N_EPSILON_][_N_PROJ_];
    TFile* fileOutProj[_N_CASES_][_N_PAIRS_];

    TString xaxis[_N_PROJ_] = {"q_{out} [GeV/c]","q_{side} [GeV/c]","q_{long} [GeV/c]"};
    TString yaxis[_N_PROJ_] = {"C(q_{out})","C(q_{side})","C(q_{long})"};

    for(int i = 0; i < _N_CASES_; i++) {
        for (int j = 0; j < _N_PAIRS_; j++) {
        //for (int j = 0; j < 1; j++) {
            fileOutProj[i][j] = new TFile(Form("./outputCorrFunc3d/outputHistProj/%s%sHistProj.root",cases[i].Data(),pairs[j].Data()),"RECREATE");
            for(int k = 0; k < _N_EPSILON_; k++) {

                for(int l = 0; l < _N_PROJ_; l++) {
                    histProj[i][j][k][l] = hbtFit->getproj(num[i][j][k],den[i][j][k],l,10,norm[i][j][k]);
                    histProj[i][j][k][l]->SetName(Form("%sE%d%sHistProj%s",cases[i].Data(),k,pairs[j].Data(),axes[l].Data()));
                    
                    histProj[i][j][k][l]->GetXaxis()->SetTitle(xaxis[l].Data());
                    histProj[i][j][k][l]->GetYaxis()->SetTitle(yaxis[l].Data());

                    fileOutProj[i][j]->cd();
                    histProj[i][j][k][l]->Write();

                }
            
            }
            fileOutProj[i][j]->Close();
            fileOutProj[i][j]->Save();
        }
    }




    


}