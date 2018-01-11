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

// pathIn is an absolute path of a file.
// return the file of that file - without extension
string ExtractNameOfFileFromPathIn(string pathIn);

// strInput is the absolute path of the *.exe file
// return value: the absolute path of TmpRect folder
string GetTmpRectFolderPath(string strInput);

// strInput is the absolute path of image file (.png or .jpg)
// return value: replace the extension by .tiff
string GetPathOfTiffFromPathOfImage(string strInput);

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
bool ReadFileOtherBoxes(string strFilePath, string strFolderImagePath, vector<string> &ListFilesOutput);

// read file Lines.txt to get list file
bool ReadFileLines(string strFilePath, string strFolderImagePath, vector<string> &ListFilesOutput);

//----------------------------------------------------------------------








int main(int ac, char** av)
{
	// list path of file
	vector<string> ListFilesPNG;
	vector<string> ListFileTIFF;
	// get path to folder TmpRect
	string strTmpRectFolder = GetTmpRectFolderPath(string(av[0]));
	// var to check the bbox io stream
	bool bChecked = false;
	// path to OtherBoxes.txt & Lines.txt
	string strPathOtherBoxes = strTmpRectFolder + "OtherBoxes.txt";
	string strPathLines = strTmpRectFolder + "Lines.txt";
	
	// Read OtherBoxes.txt
	bChecked = ReadFileOtherBoxes(strPathOtherBoxes, strTmpRectFolder, ListFilesPNG);
	if(bChecked == false)
	{
		cout << "Failed to read OtherBoxes file." << endl;
	}
	else
	{
		// Check if any file is already done
		
		
	}
	// clear
	ListFilesPNG.clear();
	ListFileTIFF.clear();
	
	// Read Lines.txt
	bChecked = ReadFileLines(strPathOtherBoxes, strTmpRectFolder, ListFilesPNG);
	if(bChecked == false)
	{
		cout << "Failed to read OtherBoxes file." << endl;
	}
	else
	{
		// Check if any file is already done
		
		
	}
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
	string strResult = "";
	// check empty string
	if (strInput.length() == 0)
	{
		return strResult;
	}
	int nSize = strInput.length();
	int nPos = nSize - 1;
	// go from n-1 to 0 until meet \ or / character - this is the folder which contains .exe file
	while (nPos > 0 && strInput[nPos] != '\\' && strInput[nPos] != '/')
	{
		nPos--;
	}
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

string GetPathOfTiffFromPathOfImage(string strInput)
{
	if (strInput.length() == 0)
		return string();
	string strResult;
	int nPos = strInput.length() - 1;
	// ignore extension
	while (nPos > 0 && strInput[nPos] != '.')
	{
		nPos--;
	}
	if (nPos == 0)
		return strResult;
	// create new char array
	char* aTmp = new char[nPos + 6];
	for (int nI = 0; nI < nPos; nI++)
	{
		aTmp[nI] = strInput[nI];
	}
	aTmp[nPos] = strInput[nPos];
	aTmp[nPos + 1] = 't';
	aTmp[nPos + 2] = 'i';
	aTmp[nPos + 3] = 'f';
	aTmp[nPos + 4] = 'f';
	aTmp[nPos + 5] = 0;
	// get result and deallocate array
	strResult = string(aTmp);
	delete[] aTmp;
	return strResult;
}

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++
/*		Operation		*/

bool ResampleFiles(vector<string> &ListFilesInput, vector<string> &ListFilesOutput)
{
	int nSize = ListFilesInput.size();
	for(int i = 0; i < nSize; i++)
	{
		Image image;
		string pathIn = ListFilesInput[i];
		string pathOut = GetPathOfTiffFromPathOfImage(pathIn);
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
		ListFilesOutput.push_back(pathOut);
	}
	return false;
}

// not finish
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
bool ReadFileOtherBoxes(string strFilePath, string strFolderImagePath, vector<string> &ListFilesOutput);
{
	BBoxIOStream bboxTmp;
	vector<tsOtherBox> atsOtherBoxes;
	bool bChecked = false;
	// get list info
	bChecked = bboxTmp.ReadOtherBoxes(atsOtherBoxes, strFilePath);
	if(bChecked == false)
		return false;
	// get path
	int nSize = atsOtherBoxes.size();
	for(int nI = 0; nI < nSize; nI++)
	{
		ListFilesOutput.push_back(atsOtherBoxes[nI].GetLastImageName());
	}
	return true;
}

// Read Lines
bool ReadFileLines(string strFilePath, string strFolderImagePath, vector<string> &ListFilesOutput);
{
	BBoxIOStream bboxTmp;
	vector<tsLineBox> atsLines;
	bool bChecked = false;
	// get list info
	bChecked = bboxTmp.ReadLines(atsLines, strFilePath);
	if(bChecked == false)
		return false;
	// get path
	int nSize = atsLines.size();
	for(int nI = 0; nI < nSize; nI++)
	{
		ListFilesOutput.push_back(atsLines[nI].GetLastImageName());
	}
	return true;
}

//----------------------------------------------------------------------