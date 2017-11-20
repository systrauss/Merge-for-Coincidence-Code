//This file is for the creation of the arrays of histograms to be used with the cuts code. It also contains the fill histograms command.

#include <histograms.h>

//C++ libraries
#include <vector>
#include <iostream>

//ROOT libraries
#include <TH1F.h>
#include <TString.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TObject.h>

using namespace std;

//File path info
extern std::string sFilepath;
extern std::string sTree;
extern std::string sRun;
extern std::string sFType;

//From Constraints.cxx
extern std::vector<std::vector<double> > dGeBounds; //bounds for cuts
extern std::vector<std::vector<double> > dSiLiBounds; //bounds for cuts
extern int nGeConstraints;
extern int nSiLiConstraints;

extern std::vector<std::vector<double> > dGeGeBounds; //bounds for cuts
extern std::vector<std::vector<double> > dGeSiLiBounds; //bounds for cuts
extern std::vector<std::vector<double> > dSiLiSiLiBounds; //bounds for cuts
extern int nGeGeConstraints;
extern int nGeSiLiConstraints;
extern int nSiLiSiLiConstraints;

//From coefficients.cxx
extern int nGeOrder; //Order of calibration i.e. 1 = linear.
extern int nGeDets; //Total number of signals from Germanium detectors
extern int nGeSegments; //number of segments in a single Germanium crystal, for adding purposes

extern int nSiLiOrder; //Order of calibration i.e. 1 = linear.
extern int nSiLiDets; //Total number of signals from SiLi
extern int nSiLiPlace; //Start of SiLis in generalized array detectors

//From this file
std::vector<std::vector<TH1F*> > ge_en_ge_cut;
std::vector<std::vector<TH1F*> > sili_en_ge_cut;
std::vector<std::vector<TH1F*> > ge_en_sili_cut;
std::vector<std::vector<TH1F*> > sili_en_sili_cut;

std::vector<std::vector<TH1F*> > ge_en_ge_ge_cut;
std::vector<std::vector<TH1F*> > sili_en_ge_ge_cut;
std::vector<std::vector<TH1F*> > ge_en_ge_sili_cut;
std::vector<std::vector<TH1F*> > sili_en_ge_sili_cut;
std::vector<std::vector<TH1F*> > ge_en_sili_sili_cut;
std::vector<std::vector<TH1F*> > sili_en_sili_sili_cut;

void writeHistToFile(char* fileName, char* sOut, int nRunStart, int nRunStop, bool bTripleCoin)
{
	TH1F* temp;
	TH1F* toAdd;
	TFile* fOut = new TFile(fileName, "RECREATE");
	std::cout << fileName << std::endl;
	TFile* fIn = new TFile(Form("%s%s_run_00%i.root",sFilepath.c_str(),sOut,nRunStart));
	std::cout << Form("%s%s_run_00%i.root",sFilepath.c_str(),sOut,nRunStart) << std::endl;
	if(!bTripleCoin)
	{
		for(int i=0; i<nGeConstraints ; i++)
		{
			fIn->cd(Form("Ge Cut Detector %i, Peak %f",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2));
			fOut->cd();
			gDirectory->mkdir(Form("Ge Cut Detector %i, Peak %f",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2));
			fOut->cd(Form("Ge Cut Detector %i, Peak %f",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("Ge Cut Detector %i, Peak %f/Clover_%i",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2,j)))->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("Ge Cut Detector %i, Peak %f/SiLi_%i",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2,j+1)))->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
		for(int i=0; i<nSiLiConstraints ; i++)
		{
			fIn->cd(Form("SiLi Cut Detector %i, Peak %f",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2));
			fOut->cd();
			gDirectory->mkdir(Form("SiLi Cut Detector %i, Peak %f",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2));
			fOut->cd(Form("SiLi Cut Detector %i, Peak %f",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2,j)))->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2,j+1)))->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
		for(int nRunNum = nRunStart+1; nRunNum <=nRunStop; nRunNum++)
		{
			TFile* fIn = new TFile(Form("%s%s_run_00%i.root",sFilepath.c_str(),sOut,nRunNum));
			std::cout << Form("%s%s_run_00%i.root",sFilepath.c_str(),sOut,nRunNum) << std::endl;
			if(fIn->IsZombie()) continue;
			for(int i=0; i<nGeConstraints ; i++)
			{
				fIn->cd(Form("Ge Cut Detector %i, Peak %f",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2));
				fOut->cd();
				fOut->cd(Form("Ge Cut Detector %i, Peak %f",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2));
				for(int j=0; j<nGeDets/nGeSegments ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f/Clover_%i",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2,j)));
					toAdd = (TH1F*)(fIn->Get(Form("Ge Cut Detector %i, Peak %f/Clover_%i",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2,j)));
					temp->Add(toAdd);
					temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
					//for(int bin=0; bin < temp->GetSize(); bin++) temp->AddBinContent(bin,toAdd->GetBinContent(bin));
				}
				for(int j=0; j<nSiLiDets ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f/SiLi_%i",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2,j+1)));
					toAdd = (TH1F*)fIn->Get(Form("Ge Cut Detector %i, Peak %f/SiLi_%i",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2,j+1));
					temp->Add(toAdd);
					temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
				}
			}
			for(int i=0; i<nSiLiConstraints ; i++)
			{
				fIn->cd(Form("SiLi Cut Detector %i, Peak %f",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2));
				fOut->cd();
				fOut->cd(Form("SiLi Cut Detector %i, Peak %f",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2));
				for(int j=0; j<nGeDets/nGeSegments ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2,j)));
					toAdd = (TH1F*)fIn->Get(Form("SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2,j));
					temp->Add(toAdd);
					temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
				}
				for(int j=0; j<nSiLiDets ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2,j+1)));
					toAdd = (TH1F*)fIn->Get(Form("SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2,j+1));
					temp->Add(toAdd);
					temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
				}
			}
			fIn->Close();
			delete fIn;
		}
		for(int i=0; i<nGeConstraints ; i++)
		{
			fOut->cd();
			fOut->cd(Form("Ge Cut Detector %i, Peak %f",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f/Clover_%i",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2,j)));
				temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
				//for(int bin=0; bin < temp->GetSize(); bin++) temp->AddBinContent(bin,toAdd->GetBinContent(bin));
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f/SiLi_%i",(int)dGeBounds[i][0],(dGeBounds[i][1]+dGeBounds[i][2])/2,j+1)));
				temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
		for(int i=0; i<nSiLiConstraints ; i++)
		{
			fOut->cd();
			fOut->cd(Form("SiLi Cut Detector %i, Peak %f",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2,j)));
				temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dSiLiBounds[i][0]+1,(dSiLiBounds[i][1]+dSiLiBounds[i][2])/2,j+1)));
				temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
	}
	else
	{
		for(int i=0; i<nGeGeConstraints ; i++)
		{
			fIn->cd(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2));
			fOut->cd();
			gDirectory->mkdir(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2));
			fOut->cd(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f/Clover_%i",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2,j)))->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f/SiLi_%i",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2,j+1)))->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
		for(int i=0; i<nGeSiLiConstraints ; i++)
		{
			fIn->cd(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2));
			fOut->cd();
			gDirectory->mkdir(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2));
			fOut->cd(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2,j)))->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2,j+1)))->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
		for(int i=0; i<nSiLiSiLiConstraints ; i++)
		{
			fIn->cd(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2));
			fOut->cd();
			gDirectory->mkdir(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2));
			fOut->cd(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2,j)))->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				/*(TH1F*)*/(fIn->Get(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2,j+1)))->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
		for(int nRunNum = nRunStart+1; nRunNum <=nRunStop; nRunNum++)
		{
			TFile* fIn = new TFile(Form("%s%s_run_00%i.root",sFilepath.c_str(),sOut,nRunNum));
			std::cout << Form("%s%s_run_00%i.root",sFilepath.c_str(),sOut,nRunNum) << std::endl;
			if(fIn->IsZombie()) continue;
			for(int i=0; i<nGeGeConstraints ; i++)
			{
				fIn->cd(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2));
				fOut->cd();
				fOut->cd(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2));
				for(int j=0; j<nGeDets/nGeSegments ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f/Clover_%i",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2,j)));
					toAdd = (TH1F*)(fIn->Get(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f/Clover_%i",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2,j)));
					temp->Add(toAdd);
					temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
					//for(int bin=0; bin < temp->GetSize(); bin++) temp->AddBinContent(bin,toAdd->GetBinContent(bin));
				}
				for(int j=0; j<nSiLiDets ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f/SiLi_%i",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2,j+1)));
					toAdd = (TH1F*)fIn->Get(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f/SiLi_%i",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2,j+1));
					temp->Add(toAdd);
					temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
				}
			}
			for(int i=0; i<nGeSiLiConstraints ; i++)
			{
				fIn->cd(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2));
				fOut->cd();
				fOut->cd(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2));
				for(int j=0; j<nGeDets/nGeSegments ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2,j)));
					toAdd = (TH1F*)(fIn->Get(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2,j)));
					temp->Add(toAdd);
					temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
					//for(int bin=0; bin < temp->GetSize(); bin++) temp->AddBinContent(bin,toAdd->GetBinContent(bin));
				}
				for(int j=0; j<nSiLiDets ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2,j+1)));
					toAdd = (TH1F*)fIn->Get(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2,j+1));
					temp->Add(toAdd);
					temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
				}
			}
			for(int i=0; i<nSiLiSiLiConstraints ; i++)
			{
				fIn->cd(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2));
				fOut->cd();
				fOut->cd(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2));
				for(int j=0; j<nGeDets/nGeSegments ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2,j)));
					toAdd = (TH1F*)(fIn->Get(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2,j)));
					temp->Add(toAdd);
					temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
					//for(int bin=0; bin < temp->GetSize(); bin++) temp->AddBinContent(bin,toAdd->GetBinContent(bin));
				}
				for(int j=0; j<nSiLiDets ; j++)
				{
					temp = (TH1F*)(fOut->Get(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2,j+1)));
					toAdd = (TH1F*)fIn->Get(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2,j+1));
					temp->Add(toAdd);
					temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
				}
			}
			fIn->Close();
			delete fIn;
		}
		for(int i=0; i<nGeGeConstraints ; i++)
		{
			fOut->cd();
			fOut->cd(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f/Clover_%i",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2,j)));
				temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
				//for(int bin=0; bin < temp->GetSize(); bin++) temp->AddBinContent(bin,toAdd->GetBinContent(bin));
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f, Ge Cut Detector %i, Peak %f/SiLi_%i",(int)dGeGeBounds[i][0],(dGeGeBounds[i][1]+dGeGeBounds[i][2])/2, (int)dGeGeBounds[i][3],(dGeGeBounds[i][4]+dGeGeBounds[i][5])/2,j+1)));
				temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
		for(int i=0; i<nGeSiLiConstraints ; i++)
		{
			fOut->cd();
			fOut->cd(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2,j)));
				temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
				//for(int bin=0; bin < temp->GetSize(); bin++) temp->AddBinContent(bin,toAdd->GetBinContent(bin));
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("Ge Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dGeSiLiBounds[i][0],(dGeSiLiBounds[i][1]+dGeSiLiBounds[i][2])/2, (int)dGeSiLiBounds[i][3],(dGeSiLiBounds[i][4]+dGeSiLiBounds[i][5])/2,j+1)));
				temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
		for(int i=0; i<nSiLiSiLiConstraints ; i++)
		{
			fOut->cd();
			fOut->cd(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2));
			for(int j=0; j<nGeDets/nGeSegments ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/Clover_%i",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2,j)));
				temp->Write(Form("Clover_%i",j),TObject::kOverwrite,1E8);
				//for(int bin=0; bin < temp->GetSize(); bin++) temp->AddBinContent(bin,toAdd->GetBinContent(bin));
			}
			for(int j=0; j<nSiLiDets ; j++)
			{
				temp = (TH1F*)(fOut->Get(Form("SiLi Cut Detector %i, Peak %f, SiLi Cut Detector %i, Peak %f/SiLi_%i",(int)dSiLiSiLiBounds[i][0],(dSiLiSiLiBounds[i][1]+dSiLiSiLiBounds[i][2])/2, (int)dSiLiSiLiBounds[i][3],(dSiLiSiLiBounds[i][4]+dSiLiSiLiBounds[i][5])/2,j+1)));
				temp->Write(Form("SiLi_%i",j+1),TObject::kOverwrite,1E8);
			}
		}
	}
	std::cout << "Finishing it up" << std::endl;
	fOut->Write();
	fOut->Close();
	std::cout << "File closed" << std::endl;
	return;
}