//////////////////////////////////////////////////////////////////////
//
//	File name：	Source.cpp
//	Description：	Main process of Lexicon project. This project has manipulate with vietnamese so
//					becareful if you try to run this project on other platform of windows and Unix.
//	Notes:			None
//	History：	<0> 2017.12.22 : Dang Tuan Vu : Create structure definition
//					<1>	2018.01.13 : Dang Tuan Vu : Complete project
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "BBoxIOStream.h"
#include "BBoxStructure.h"

using namespace std;

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

// strInput is the absolute path of a file
// return value: the absolute path of the file containing folder
string GetCurrentFolderOfPath(string strInput);

//---------------------------------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Operation		*/

// Run whole program
void Run(int argc, char** argv);

// strInput is the word containing file name input which need to be modified.
void RunLexiconOneWord(string strInput, string strInputExe);

// strInput is the absolute path of the .exe file
void RunLexiconAll(string strInputExe);



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

void TestAsciiAndUnicode();

//---------------------------------------------------------------------------------------------------------------------------------


int main(int argc, char** argv)
{
	//Run(argc, argv);
	TestAsciiAndUnicode();

	return 1;
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

string GetCurrentFolderOfPath(string strInput)
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
	// it should be the containing folder.
	while (nPos > 0 && strInput[nPos] != '\\' && strInput[nPos] != '/')
	{
		nPos--;
	}
	if (nPos == 0)
	{
		return strResult;
	}
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

//---------------------------------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Operation		*/

void Run(int argc, char** argv)
{
	// check argument input
	if(argc == 2)
	{
		// check only 1 word
		// this word containing file need to stay the same folder of this file .exe
		RunLexiconOneWord(string(argv[1]), string(argv[0]));
	}
	else
	{
		// get full list word aand check from team leader.
		RunLexiconAll(string(argv[0]));
	}
}

// strInput is the word containing file name input which need to be modified.
void RunLexiconOneWord(string strInput, string strInputExe)
{
	string strRootFolder = GetCurrentFolderOfPath(strInputExe);
	wstring strWords;
	wifstream wifsRead;
	// load file
	wifsRead.open(strRootFolder + strInput + ".txt", std::ifstream::in);
	if(wifsRead.is_open())
	{
		// get words
		while(getline(wifsRead, wstrWords))
		{
			if(wstrWords.compare("") != 0 && wstrWords.compare(" ") != 0)
				break;
		}
		ifsRead.close();
	}
	else
	{
		return;
		cout << "Error: Unable to read file: " << strRootFolder << strInput << endl;
	}

	// Load data and structure

}

// strInput is the absolute path of the .exe file
void RunLexiconAll(string strInputExe)
{
	string strRootFolder = GetCurrentFolderOfPath(strInputExe);
	// Load data and structure
	
	
	
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


void TestAsciiAndUnicode()
{
	const wchar_t wtext[] = L"mùa đông" ;

   std::cout << "sizeof(wtext)   : " << sizeof(wtext) << std::endl ;
   std::cout << "wcslen(wtext)   : " << wcslen(wtext) << std::endl ;

   std::cout << "wtext(bytes)    :" ;

   for(size_t i = 0, iMax = wcslen(wtext); i < iMax; ++i)
   {
      std::cout << " " << static_cast<unsigned int>(
                              static_cast<unsigned short>(wtext[i])
                              );
   }
   std::cout << std::endl << std::endl ;

   const char text[] = "mùa đông" ;

   std::cout << "sizeof(text)   : " << sizeof(text) << std::endl ;
   std::cout << "strlen(text)   : " << strlen(text) << std::endl ;

   std::cout << "text(bytes)    :" ;

   for(size_t i = 0, iMax = strlen(text); i < iMax; ++i)
   {
      std::cout << " " << static_cast<unsigned int>(
                              static_cast<unsigned short>(text[i])
                              );
   }
   std::cout << std::endl << std::endl ;
}



//---------------------------------------------------------------------------------------------------------------------------------