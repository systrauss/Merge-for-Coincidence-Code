#include "Filelist.h"

//C++ libraries
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

//ROOT libraries
#include <TChain.h>
#include <TString.h>
#include <TSystem.h>


//sFilepath,sRun and sFType are all in the header file Filelist.h
std::string sFilepath;
//Change sTree for the program to look at a different tree in the files.
std::string sTree;
//Formatting for the beginning and end of the rootfile name
std::string sRun;
std::string sFType;

TChain* chain;

using namespace std;

void readPaths()
{
   fstream fFile("user/Filelist.dat"); //Coefficient File
   if(!fFile.is_open())
   {
      cout << "Filelist did not open" << endl;
   }
   string buffer;
   std::getline(fFile,buffer); //Label
   std::getline(fFile,sFilepath); //Filepath
   sFilepath = sFilepath.substr(0,sFilepath.size()-1); //make a substring without a return

   std::getline(fFile,buffer); //Label
   std::getline(fFile,sTree); //Tree
   sTree = sTree.substr(0,sTree.size()-1); //make a substring without a return

   std::getline(fFile,buffer); //Label
   std::getline(fFile,sRun); //Run
   std::getline(fFile,sFType); //Type
   sRun = sRun.substr(0,sRun.size()-1); //make a substring without a return
   sFType = sFType.substr(0,sFType.find_first_of('\n')-1); //make a substring without a return

   fFile.close();
}