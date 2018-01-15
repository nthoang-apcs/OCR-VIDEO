//////////////////////////////////////////////////////////////////////
//
//	File name：	Source.cpp
//	Description：	Main process of OCRAutorun project
//	Notes:			None
//	History：	<0> 2017.01.13 : Dang Tuan Vu : Create project
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp>
#include "BBoxIOStream.h"
#include "BBoxStructure.h"

using namespace std;
using namespace cv;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Support functions		*/

// strInput is the absolute path of the *.exe file
// return value: the absolute path of Root folder
string GetRootFolder(string strInput);

// strInput is the absolute path of the *.exe file
// return value: the absolute path of TmpImage folder
string GetTmpImageFolderPath(string strInput);

// strInput is the absolute path of the *.exe file
// return value: the absolute path of TmpRect folder
string GetTmpRectFolderPath(string strInput);

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Operation functions		*/
// run whole process, input is the main function input
void Run(int argc, char **argv);

// input a string of a image, get out result: otherboxes, lines and time running
// preprocessing -	Sharpen, grayscale
// processing -		MSER
// postprocessing - Remove Unusual Area Boxes; remove unbalanced ratio width, height
//					Sort y coordinate ascending; remove single box text line
//					Merge inside box
void ProcessOneImage(string strInput, float &fTimeRunning, vector<tsLineBox> &atsLines, vector<tsOtherBox> &atsOtherBoxes);

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/



//----------------------------------------------------------------------

/////////////////////////////////
/*
- Main Function
- Input argument is the path of Files.txt
- The return value of this main function is 1 if success, 0 if failed.
*/
/////////////////////////////////

int main(int argc, char **argv)
{
	Run(argc, argv);

	return 1;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Support functions		*/

string GetRootFolder(string strInput)
{
	string strResult = "";
	// check empty string
	if (strInput.length() == 0)
	{
		return strResult;
	}
	int nSize = strInput.length();
	int nPos = nSize - 1;
	// go from n-1 to 0 until meet \ or / character - this is the folder which contains .exe file
	// it should be the TmpRect folder. However, for sure, just get the root folder in later loop
	while (nPos > 0 && strInput[nPos] != '\\' && strInput[nPos] != '/')
	{
		nPos--;
	}
	/*current text is: [drive]:\...\Root\*/ 
	char* aTmp = new char[nPos + 2];
	for (int nI = 0; nI <= nPos; nI++)
	{
		aTmp[nI] = strInput[nI];
	}
	aTmp[nPos + 1] = 0;
	strResult = string(aTmp);
	delete[] aTmp;
	return strResult;
}

string GetTmpRectFolderPath(string strInput)
{
	string strResult = "";
	// check empty string
	if (strInput.length() == 0)
	{
		return strResult;
	}
	int nSize = strInput.length();
	int nPos = nSize - 1;
	// go from n-1 to 0 until meet \ or / character - this is the folder which contains .exe file
	// it should be the TmpRect folder. However, for sure, just get the root folder in later loop
	while (nPos > 0 && strInput[nPos] != '\\' && strInput[nPos] != '/')
	{
		nPos--;
	}
	// ignore '\\' or '/'
	nPos--;
	if (nPos == 0)
	{
		return strResult;
	}
	// go from n-1 to 0 until meet \ or / character - this is the root folder
	while (nPos > 0 && strInput[nPos] != '\\' && strInput[nPos] != '/')
	{
		nPos--;
	}
	if (nPos == 0)
	{
		return strResult;
	}
	char* aTmp = new char[nPos + 10];
	for (int nI = 0; nI <= nPos; nI++)
	{
		aTmp[nI] = strInput[nI];
	}
	// add TmpRect
	aTmp[nPos + 1] = 'T';
	aTmp[nPos + 2] = 'm';
	aTmp[nPos + 3] = 'p';
	aTmp[nPos + 4] = 'R';
	aTmp[nPos + 5] = 'e';
	aTmp[nPos + 6] = 'c';
	aTmp[nPos + 7] = 't';
	aTmp[nPos + 8] = '\\';
	aTmp[nPos + 9] = 0;
	strResult = string(aTmp);
	delete[] aTmp;
	return strResult;
}

string GetTmpImageFolderPath(string strInput)
{
	string strResult = "";
	// check empty string
	if (strInput.length() == 0)
	{
		return strResult;
	}
	int nSize = strInput.length();
	int nPos = nSize - 1;
	// go from n-1 to 0 until meet \ or / character - this is the folder which contains .exe file
	// it should be the TmpRect folder. However, for sure, just get the root folder in later loop
	while (nPos > 0 && strInput[nPos] != '\\' && strInput[nPos] != '/')
	{
		nPos--;
	}
	// ignore '\\' or '/'
	nPos--;
	if (nPos == 0)
	{
		return strResult;
	}
	// go from n-1 to 0 until meet \ or / character - this is the root folder
	while (nPos > 0 && strInput[nPos] != '\\' && strInput[nPos] != '/')
	{
		nPos--;
	}
	if (nPos == 0)
	{
		return strResult;
	}
	char* aTmp = new char[nPos + 11];
	for (int nI = 0; nI <= nPos; nI++)
	{
		aTmp[nI] = strInput[nI];
	}
	// add TmpRect
	aTmp[nPos + 1] = 'T';
	aTmp[nPos + 2] = 'm';
	aTmp[nPos + 3] = 'p';
	aTmp[nPos + 4] = 'I';
	aTmp[nPos + 5] = 'm';
	aTmp[nPos + 6] = 'a';
	aTmp[nPos + 7] = 'g';
	aTmp[nPos + 8] = 'e';
	aTmp[nPos + 9] = '\\';
	aTmp[nPos + 10] = 0;
	strResult = string(aTmp);
	delete[] aTmp;
	return strResult;
}


//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Operation functions		*/

void Run(int argc, char **argv)
{
	// get Root folder
	string strRootFolder = GetRootFolder(argv[0]);
	// get File's path
	string strFilePath;
	if (argc == 2)
	{
		// get file path
		strFilePath = string(argv[1]);
		// check if file is existed
		ifstream ifsRead;
		ifsRead.open(strFilePath, std::ifstream::in);
		if (ifsRead.is_open())
		{
			// file exists
			ifsRead.close();
		}
		else
		{
			// file does not exist
			// use default value
			strFilePath = strRootFolder + "Files.txt";
		}
	}
	else
	{
		// use default
		strFilePath = strRootFolder + "Files.txt";
	}

	vector<string> astrListPaths;
	// read all files' Paths
	ifstream ifsRead;
	ifsRead.open(strFilePath, std::ifstream::in);
	if (ifsRead.is_open())
	{
		string line;
		while (getline(ifsRead, line))
		{
			if (line.compare("") == 0 || line.compare(" ") == 0)
			{
				continue;
			}
			else
			{
				astrListPaths.push_back(line);
			}
		}

		ifsRead.close();
	}
	else
	{
		cout << "Both user define files' path and default file's path do not exist!!! The program exits." << endl;
	}

	// start processing
	vector<tsLineBox> atsLines;
	vector<tsOtherBox> atsOtherBoxes;
	float fTimeRun = 0.00;
	int nSize = astrListPaths.size();

	for (int nI = 0; nI < nSize; nI++)
	{
		// process image 1 by 1
		ProcessOneImage(astrListPaths[nI], fTimeRun, atsLines, atsOtherBoxes);
	}
}

void ProcessOneImage(string strInput, float &fTimeRunning, vector<tsLineBox> &atsLines, vector<tsOtherBox> &atsOtherBoxes)
{

}


//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/



//----------------------------------------------------------------------

