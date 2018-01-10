//////////////////////////////////////////////////////////////////////
//
//	File name：	Source.cpp
//	Description：	Main process of OCRAutorun project
//	Notes:			None
//	History：	<0> 2017.12.22 : Dang Tuan Vu : Create structure definition
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Magick++.h> 
#include <Magick++\Image.h>
#include "BBoxIOStream.h"
#include "BBoxStructure.h"
using namespace std;
using namespace Magick;

//++++++++++++++++++++++++++++++++++++++++++
/*		Support functions		*/

string ExtractNameOfFileFromPathIn(string pathIn);

string GetTmpRectFolderPath(string strInput);

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++
/*		Operation		*/

// Resample for each file in list
bool ResampleFiles(vector<string> &ListFilesInput, vector<string> &ListFilesOutput);

// Run command line for each file in list
void OCRRun(vector<string> &ListFiles);

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/

// read file OtherBoxes.txt to get list file
void ReadFileOtherBoxes(string filename, vector<string> &ListFilesOutput);

// read file Lines.txt to get list file
void ReadFileLines(string filename, vector<string> &ListFilesOutput);

//----------------------------------------------------------------------








int main(int ac, char** av)
{
	// list path of file
	vector<string> ListFilesPNG;
	vector<string> ListFileTIFF;
	// get path to folder TmpRect
	string strTmpRectFolder = GetTmpRectFolderPath(string(av[0]));
	
	// path to OtherBoxes.txt & Lines.txt
	string strPathOtherBoxes = strTmpRectFolder + "OtherBoxes.txt";
	string strPathLines = strTmpRectFolder + "Lines.txt";
	
	// Read OtherBoxes.txt
	
	// Check if any file is already done
	
	
	
	// Read Lines.txt
	
	// check if any file is already done
	
}


//++++++++++++++++++++++++++++++++++++++++++
/*		Support functions		*/

string ExtractNameOfFileFromPathIn(string pathIn)
{
	int leng = pathIn.length();
	int pos = leng - 1;
	do
	{
		pos--;
	} while (pathIn[pos] != '.' && pos > 0);
	// pos at char '.'
	int pos2 = pos - 1;
	do
	{
		pos2--;
	} while (pathIn[pos2] != '\\' && pathIn[pos2] != '/' && pos2 > 0);
	// pos2 at char '\'
	pos2++;
	// get name
	vector<char> tmp;
	for (int i = pos2; i < pos; i++)
	{
		tmp.push_back(pathIn[i]);
	}
	int tmplen = tmp.size();
	char *tmp2 = new char[tmplen + 1];
	for (int i = 0; i < tmplen; i++)
	{
		tmp2[i] = tmp[i];
	}
	tmp2[tmplen] = '\0';
	tmp.clear();
	return string(tmp2);
}

string GetTmpRectFolderPath(string strInput)
{
	
}

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++
/*		Operation		*/

bool ResampleFiles(vector<string> &ListFilesInput, vector<string> &ListFilesOutput)
{
	int nSize = ListFiles.size();
	for(int i = 0; i < nSize; i++)
	{
		Image image;
		string pathIn = ListFiles[i];
		string pathOut = "";
		try {
			// Read a file into image object
			image.read(pathIn);
			Magick::Geometry gm = image.size();
			image.resample(Point(300, 300));
			image.resize(gm);
			image.write(pathOut);
			return true;
		}
		catch (Exception &error_)
		{
			cout << "Caught exception: " << error_.what() << endl;
			return false;
		}
	}
	return false;
}

void OCRRun(vector<string> &ListFiles)
{
	int nSize = ListFiles.size();
	for(int i = 0; i < nSize; i++)
	{
		// ocr command
	}
}

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/

// Read OtherBoxes
void ReadFileOtherBoxes(string filename, vector<string> &ListFilesOutput)
{
	ifstream ifs(filename);
	if(ifs.is_open())
	{
		string line;
		while(getline(ifs, line))
		{
			
		}
		ifs.close();
	}
}

// Read Lines
void ReadFileLines(string filename, vector<string> &ListFilesOutput)
{
	ifstream ifs(filename);
	if(ifs.is_open())
	{
		string line;
		while(getline(ifs, line))
		{
			
		}
		ifs.close();
	}
}

//----------------------------------------------------------------------