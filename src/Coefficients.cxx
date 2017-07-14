#include "Constraints.h"

//C++ libraries
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>

//ROOT libraries
#include <TString.h>

using namespace std;

int nGeOrder; //Order of calibration i.e. 1 = linear.
int nGeDets; //Total number of signals from Germanium detectors
int nGeSegments; //number of segments in a single Germanium crystal, for adding purposes

int nSiLiOrder; //Order of calibration i.e. 1 = linear.
int nSiLiDets; //Total number of signals from SiLi
int nSiLiPlace; //Start of SiLis in generalized array detectors

void defineGeCoeff() //Get Ge coefficients. Does not include run-by-run corrections
{
   //First thing: read in the coefficients for this run.
   fstream fCoeff("user/GeCoefficients.dat"); //Coefficient File, before run-by-run corrections
   if(!fCoeff.is_open())
   {
      cout << "Ge file did not open" << endl;
      return;
   }
   string buffer;
   std::getline(fCoeff,buffer);
   nGeOrder = std::atoi(buffer.substr(0,buffer.find_first_of(' ',0)).c_str()); //So this clusterfuck is due to gcc being a pain. it basically converts a substring of the string into a char to convert into an int because apparently it won't just go string to int
   getline (fCoeff,buffer);
   nGeDets = std::atoi(buffer.substr(0,buffer.find_first_of(' ',0)).c_str());
   getline (fCoeff,buffer);
   nGeSegments = std::atoi(buffer.substr(0,buffer.find_first_of(' ',0)).c_str());
   fCoeff.close(); //Close the coefficients file
}

void defineSiLiCoeff() //Get SiLi coefficients. Does not include run-by-run corrections
{
   //First thing: read in the coefficients for this run.
   fstream fCoeff("user/SiLiCoefficients.dat"); //Coefficient File
   if(!fCoeff.is_open())
   {
      cout << "SiLi file did not open" << endl;
      return;
   }
   string buffer;
   std::getline(fCoeff,buffer);
   nSiLiOrder = std::atoi(buffer.substr(0,buffer.find_first_of(' ',0)).c_str()); //So this clusterfuck is due to gcc being a pain. it basically converts a substring of the string into a char to convert into an int because apparently it won't just go string to int
   getline (fCoeff,buffer);
   nSiLiDets = std::atoi(buffer.substr(0,buffer.find_first_of(' ',0)).c_str());
   getline (fCoeff,buffer);
   nSiLiPlace = std::atoi(buffer.substr(0,buffer.find_first_of(' ',0)).c_str());
   fCoeff.close(); //Close the coefficients file
}
