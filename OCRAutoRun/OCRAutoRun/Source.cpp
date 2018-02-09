//////////////////////////////////////////////////////////////////////
//
//	File name：	Source.cpp
//	Description：	Main process of OCRAutorun project
//	Notes:			None
//	History：	<0> 2017.12.22 : Dang Tuan Vu : Create structure definition
//					<1>	2018.01.13 : Dang Tuan Vu : Complete project
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

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Support functions		*/

// pathIn is an absolute path of a file.
// return the file of that file - without extension
string ExtractNameOfFileFromPathIn(string pathIn);

// strInput is the absolute path of the *.exe file
// return value: the absolute path of TmpRect folder
string GetTmpRectFolderPath(string strInput);

// strInput is the absolute path of the *.exe file
// return value: the absolute path of TmpImage folder
string GetTmpImageFolderPath(string strInput);

// strInput is the absolute path of the *.exe file
// return value: the absolute path of TmpImage folder
string GetTmpCheckVowelFolderPath(string strInput);

//---------------------------------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Operation		*/

// Resample for each file in list, ListFilesOutput = ListFileTIFF
// ListFileInput = list files' name without extension
// the default file extension input is .jpg, please change it if you use other format
bool ResampleFiles(string strTmpRectFolder, vector<string> &ListFilesInput);

// Run command line for each file in list
// ListFiles = list files' name without extension
void OCRRun(vector<string> &ListFiles);

// ListFileInput = list files' name without extension
// check if the text file of that version is already done
// text file format: name of file + '.txt', like format of tiff file: name of file + '.tiff'
// new list is also in ListFileInput
void RemoveDoneImageFile(string strTmpImageFolder, vector<string> &ListFileInput);

// input path is the TmpCheckVowel folder path
void OutputToInputCheckVowel(string strPath, vector<string> &ListFiles);

//---------------------------------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/

// read file OtherBoxes.txt to get list files' name
// ListFilesOutput = list files' name without extension
bool ReadFileOtherBoxes(string strFilePath, string strFolderImagePath, vector<string> &ListFilesOutput);

// read file Lines.txt to get list files' name
// ListFilesOutput = list files' name without extension
bool ReadFileLines(string strFilePath, string strFolderImagePath, vector<string> &ListFilesOutput);

//---------------------------------------------------------------------------------------------------------------------------------

/*		Run and Debug functions		*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Run whole programe, get argument from main function
void Run(int ac, char** av);

void TestBBoxStreamWriting();

void TestBBoxStreamReading();

void TestOCRRun();

void TestResample();

//---------------------------------------------------------------------------------------------------------------------------------


int main(int argc, char** argv)
{
	InitializeMagick(NULL);
	Run(argc, argv);

	// please modify hardcode path inside the function before running
	//TestBBoxStreamWriting();

	// please modify hardcode path inside the function before running
	//TestBBoxStreamReading();

	// please modify hardcode files' names inside the function before running
	//TestOCRRun();
	
	// please modify hardcode path inside the function before running
	//TestResample();

	return 0;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

string GetTmpCheckVowelFolderPath(string strInput)
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
	char* aTmp = new char[nPos + 15];
	for (int nI = 0; nI <= nPos; nI++)
	{
		aTmp[nI] = strInput[nI];
	}
	// add TmpRect
	aTmp[nPos + 1] = 'T';
	aTmp[nPos + 2] = 'm';
	aTmp[nPos + 3] = 'C';
	aTmp[nPos + 4] = 'h';
	aTmp[nPos + 5] = 'e';
	aTmp[nPos + 6] = 'c';
	aTmp[nPos + 7] = 'k';
	aTmp[nPos + 8] = 'V';
	aTmp[nPos + 9] = 'o';
	aTmp[nPos + 10] = 'w';
	aTmp[nPos + 11] = 'e';
	aTmp[nPos + 12] = 'l';
	aTmp[nPos + 13] = '\\';
	aTmp[nPos + 14] = 0;
	strResult = string(aTmp);
	delete[] aTmp;
	return strResult;
}

//---------------------------------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Operation		*/

bool ResampleFiles(string strTmpImageFolder, vector<string> &ListFilesInput)
{
	int nSize = ListFilesInput.size();
	for(int i = 0; i < nSize; i++)
	{
		Image image;
		try {
			string pathIn = strTmpImageFolder + ListFilesInput[i] + ".jpg";
			string pathOut = strTmpImageFolder + ListFilesInput[i] + ".tiff";
			cout << pathIn << endl;
			cout << pathOut << endl;
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
			cout << "Caught resample exception: " << error_.what() << endl;
			return false;
		}
	}
	return false;
}

void OCRRun(vector<string> &ListFiles)
{
	int nSize = ListFiles.size();
	for(int nI = 0; nI < nSize; nI++)
	{
		// prepare string command, vie for vietnamese
		string strCmd = "tesseract " + ListFiles[nI] + ".tiff " + ListFiles[nI] + " -l vie -psm 7";
		// copy to char array
		int nLen = strCmd.length();
		char* aTmp = new char[nLen + 1];
		for (int nJ = 0; nJ < nLen; nJ++)
		{
			aTmp[nJ] = strCmd[nJ];
		}
		aTmp[nLen] = 0;
		system(aTmp);
		
		delete[] aTmp;
	}
}

void RemoveDoneImageFile(string strTmpImageFolder, vector<string> &ListFileInput)
{
	// get file name from list file input -> check if filename + ".txt" existed ?
	// Yes: true
	// No: false
	vector<bool> abListCheck;
	int nSize = ListFileInput.size();
	if (nSize == 0)
		return;
	for (int nI = 0; nI < nSize; nI++)
	{
		ifstream ifsRead;
		ifsRead.open((strTmpImageFolder + ListFileInput[nI] + ".txt"), std::ifstream::in);
		if (ifsRead.is_open())
		{
			abListCheck.push_back(true);
			ifsRead.close();
		}
		abListCheck.push_back(false);
	}
	// create new list
	vector<string> aTmp;
	for (int nI = 0; nI < nSize; nI++)
	{
		if (abListCheck[nI] == false)
		{
			aTmp.push_back(ListFileInput[nI]);
		}
	}
	ListFileInput.clear();
	ListFileInput = aTmp;
	aTmp.clear();
	abListCheck.clear();
	return;
}

void OutputToInputCheckVowel(string strPath, vector<string> &ListFiles)
{
	ofstream ofsWrite;
	ofsWrite.open(strPath + "InputCVowel.txt", std::ofstream::out);
	if (ofsWrite.is_open())
	{
		int nSize = ListFiles.size();
		for(int nI = 0; nI < nSize; nI++)
		{
			ofsWrite << strPath << ListFiles[nI] << ".txt\n";
		}
		ofsWrite.close();
	}
	else
	{
		cout << "Unable to create and write to file InputCVowel.txt" << endl;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/

// Read OtherBoxes
bool ReadFileOtherBoxes(string strFilePath, string strFolderImagePath, vector<string> &ListFilesOutput)
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
bool ReadFileLines(string strFilePath, string strFolderImagePath, vector<string> &ListFilesOutput)
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

//---------------------------------------------------------------------------------------------------------------------------------

/*		Run and Debug functions		*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Run(int ac, char** av)
{
	// list name of files without extension
	vector<string> ListFileNames;
	// list name of files with extension .tiff
	vector<string> ListFileTIFF;
	// get path to folder TmpRect
	string strTmpRectFolder = GetTmpRectFolderPath(string(av[0]));
	string strTmpImageFolder = GetTmpImageFolderPath(string(av[0]));
	// var to check the bbox io stream
	bool bChecked = false;
	// path to OtherBoxes.txt & Lines.txt
	string strPathOtherBoxes = strTmpRectFolder + "OtherBoxes.txt";
	string strPathLines = strTmpRectFolder + "Lines.txt";
	// Read OtherBoxes.txt
	bChecked = ReadFileOtherBoxes(strPathOtherBoxes, strTmpRectFolder, ListFileNames);
	if (bChecked == false)
	{
		cout << "Failed to read OtherBoxes.txt file." << endl;
	}
	else
	{
		// Check if any file is already done
		RemoveDoneImageFile(strTmpImageFolder, ListFileNames);
		// resample
		ResampleFiles(strTmpImageFolder, ListFileNames);
		// Process
		OCRRun(ListFileNames);
		// output to file InputCVowel.txt in TmpCheckVowel folder
		OutputToInputCheckVowel(GetTmpCheckVowelFolderPath(string(av[0])), ListFileNames);
	}
	// clear
	ListFileNames.clear();
	ListFileTIFF.clear();

	// Read Lines.txt
	bChecked = ReadFileLines(strPathOtherBoxes, strTmpRectFolder, ListFileNames);
	if (bChecked == false)
	{
		cout << "Failed to read Lines.txt file." << endl;
	}
	else
	{
		// Check if any file is already done
		RemoveDoneImageFile(strTmpImageFolder, ListFileNames);
		// resample
		ResampleFiles(strTmpImageFolder, ListFileNames);
		// Process
		OCRRun(ListFileNames);
		// output to file InputCVowel.txt in TmpCheckVowel folder
		OutputToInputCheckVowel(GetTmpCheckVowelFolderPath(string(av[0])), ListFileNames);
	}
	// clear
	ListFileNames.clear();
	ListFileTIFF.clear();
}

void TestBBoxStreamWriting()
{
	// create sample data OtherBoxes
	vector<tsOtherBox> atsOtherBoxes;
	int nSize = 5;
	for (int nI = 0; nI < nSize; nI++)
	{
		tsOtherBox tsTmp (nI, nI + 10, nI + 10 + 1, nI + 10 + 2, nI + 10 + 3, "testWritingOtherBox" + to_string(nI), 1, 1000 + nI);
		atsOtherBoxes.push_back(tsTmp);
	}
	// emulate the writing OtherBoxes
	BBoxIOStream bboxTmp;
	bboxTmp.WriteOtherBoxes(atsOtherBoxes, "E:\\Code\\OCR-Five-Git\\OCRAutoRun\\x64\\Debug\\OtherBoxes.txt");
	atsOtherBoxes.clear();
	// create sample data Lines
	vector<tsLineBox> atsLineBoxes;
	for (int nI = 0; nI < nSize; nI++)
	{
		tsLineBox tsLineTmp(nI, nI + 10, nI + 10 + 1, nI + 10 + 2, nI + 10 + 3, "testWritingOtherBox" + to_string(nI), 1, 1000 + nI);
		tsLineTmp.anSubID.push_back(nI + 10000);
		tsLineTmp.anSubID.push_back(nI + 10001);
		tsLineTmp.atsSubROI.push_back(tsRect(nI + 20, nI + 21, nI + 22, nI + 23));
		tsLineTmp.atsSubROI.push_back(tsRect(nI + 24, nI + 25, nI + 26, nI + 27));
		atsLineBoxes.push_back(tsLineTmp);
	}
	// emulate the writing Lines
	bboxTmp.WriteLines(atsLineBoxes, "E:\\Code\\OCR-Five-Git\\OCRAutoRun\\x64\\Debug\\Lines.txt");
	atsLineBoxes.clear();
}

void TestBBoxStreamReading()
{
	vector<tsOtherBox> atsOtherBoxes;
	vector<tsLineBox> atsLineBoxes;
	BBoxIOStream bboxTmp;
	bool bChecked = false;
	bChecked = bboxTmp.ReadLines(atsLineBoxes, "E:\\Code\\OCR-Five-Git\\OCRAutoRun\\x64\\Debug\\Lines.txt");
	if (bChecked == true)
	{
		cout << "Read Lines successful." << endl;
	}
	cout << endl;
	bChecked = bboxTmp.ReadOtherBoxes(atsOtherBoxes, "E:\\Code\\OCR-Five-Git\\OCRAutoRun\\x64\\Debug\\OtherBoxes.txt");
	if (bChecked == true)
	{
		cout << "Read Lines successful." << endl;
	}
	cout << endl;
	atsOtherBoxes.clear();
	atsLineBoxes.clear();
}

void TestOCRRun()
{
	vector<string> aListFiles;
	aListFiles.push_back("img39-0");
	aListFiles.push_back("img39-1");
	OCRRun(aListFiles);
}

void TestResample()
{
	vector<string> ListFiles;
	ListFiles.push_back("img40");
	ResampleFiles("D:\\", ListFiles);
	char a;
	cin >> a;
	
}

//---------------------------------------------------------------------------------------------------------------------------------