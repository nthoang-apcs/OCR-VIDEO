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

string ExtractNameOfFileFromPathIn(string pathIn);

// read list file
void ReadFiles(string filename, vector<string> &ListFiles);

bool ResampleFiles(vector<string> &ListFiles);

void OCRRun(vector<string> &ListFiles);


int main()
{
	// input
	vector<string> ListFiles;
	string filename = "files.txt";
	ReadFiles(filename, ListFiles);
	// resample
	ResampleFiles(ListFiles);
	// ocr
	
}



void ReadFiles(string filename, vector<string> &ListFiles)
{
	ifstream ifs(filename);
	if(ifs.is_open())
	{
		string line;
		while(getline(ifs, line))
		{
			ListFiles.push_back(line);
		}
		ifs.close();
	}
}

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

bool ResampleFiles(vector<string> &ListFiles)
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