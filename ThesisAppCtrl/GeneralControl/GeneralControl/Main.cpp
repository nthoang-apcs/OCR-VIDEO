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
#include <ctime>
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

// Remove duplicated Rects after MSER
void RemoveDuplicatedBoxes(vector<Rect> &arBBoxes);

void RemoveUnusualAreaBoxes(vector<Rect> &arBBoxes);

// from center of Rect, text 2 lines, 1 upper 25%, 1 lower 25%
// to check if that space created by 2 lines cuts any other spaces
// if cut -> on same line
void RemoveSingleBoxTextLine(vector<Rect> &arBBoxes);

// h / w or w / h > 6 -> remove
void RemoveUnbalancedRatio(vector<Rect> &arBBoxes);

// merge inside boxes
// remove too big size
void MergeInsideBoxes(vector<Rect> &arBBoxes);

void SortYCoordinate(vector<Rect> &arBBoxes);

void SortArea(vector<Rect> &arBBoxes);

void SortXCoordinate(vector<Rect> &arBBoxes);

// check if any boxes in the same line - +- 50% height
bool IsB1onsamelineB2(Rect B1, Rect B2);

// check if ratio between height and width >= 6 times
// check if height < 9
bool IsB1Balanced(Rect B1);

// check if the intersection area between B1 and B2 is equal B1.area()
// and the B2.area() / B1.area() <= 3
bool IsB1insideB2(Rect B1, Rect B2);

// return true if B1.y < B2.y
bool CompareYCoordinate(Rect B1, Rect B2);

// return true if B1.area() < B2.area()
bool CompareArea(Rect B1, Rect B2);

// return true if B1.x < B2.x
bool CompareXCoordinate(Rect B1, Rect B2);


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

// read an absolute path of an image into a Mat with grayscale format
void ReadImageGrayScale(string strPath, Mat &mInput);

// sharpen an Mat
// using gaussian formula
void SharpenImage(Mat &mInput, Mat &mOutput, double sigma = 1, double threshold = 5, double amount = 1);

// MSER method to get bounding boxes
void MSEROneImage(Mat &input, vector<Rect> &arBBoxes);

// convert from BBoxes to tsOtherBox
void ConvertFromBBoxesToOtherBoxes(string strImagename, vector<Rect> &arBBoxes, vector<tsOtherBox> &atsOtherboxes);

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

void RemoveDuplicatedBoxes(vector<Rect> &arBBoxes)
{
	vector<Rect> tmpBoxes;
	int k1 = arBBoxes.size();
	int k2 = tmpBoxes.size();
	for (int i = 0; i < k1; i++)
	{
		k2 = tmpBoxes.size();
		bool existed = false;
		for (int j = 0; j < k2; j++)
		{
			if (tmpBoxes[j].x == arBBoxes[i].x && tmpBoxes[j].y == arBBoxes[i].y && tmpBoxes[j].width == arBBoxes[i].width && tmpBoxes[j].height == arBBoxes[i].height)
			{
				existed = true;
				break;
			}
		}
		if (existed == false)
		{
			tmpBoxes.push_back(arBBoxes[i]);
		}
	}
	arBBoxes.clear();
	arBBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void RemoveUnusualAreaBoxes(vector<Rect> &arBBoxes)
{
	int k1 = arBBoxes.size();
	if (k1 == 0 || k1 == 1)
		return;
	vector<Rect> tmpBoxes;
	// start to check the condition:
	// if a single box areas[i] stands alone and have areas / areas[i - 1] > 5 && have areas / areas[i + 1] > 5
	// => not add to tmpboxes
	int i = 0;
	while (i < k1)
	{
		if (i == 0)
		{
			// limit
			if ((arBBoxes[i].area() / arBBoxes[i + 1].area()) >= 5 || (arBBoxes[i + 1].area() / arBBoxes[i].area()) >= 5 || arBBoxes[i].area() < 30)
			{

			}
			// get result
			else
			{
				tmpBoxes.push_back(arBBoxes[i]);
			}
			i++;
			continue;
		}
		else if (i == (k1 - 1))
		{
			if ((arBBoxes[i].area() / arBBoxes[i - 1].area()) >= 5 || (arBBoxes[i - 1].area() / arBBoxes[i].area()) >= 5 || arBBoxes[i].area() < 30)
			{

			}
			else
			{
				tmpBoxes.push_back(arBBoxes[i]);
			}
			i++;
			continue;
		}
		else
		{
			if ((arBBoxes[i].area() / arBBoxes[i + 1].area()) >= 5 || (arBBoxes[i + 1].area() / arBBoxes[i].area()) >= 5 || arBBoxes[i].area() < 30)
			{
				if ((arBBoxes[i].area() / arBBoxes[i - 1].area()) >= 5 || (arBBoxes[i - 1].area() / arBBoxes[i].area()) >= 5 || arBBoxes[i].area() < 30)
				{

				}
				else
				{
					tmpBoxes.push_back(arBBoxes[i]);
				}
			}
			else
			{
				tmpBoxes.push_back(arBBoxes[i]);
			}
			i++;
			continue;
		}
	}
	arBBoxes.clear();
	arBBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void RemoveSingleBoxTextLine(vector<Rect> &arBBoxes)
{
	int k = arBBoxes.size();
	vector<Rect> tmpBoxes;
	for (int i = 0; i < k; i++)
	{
		bool checked = false;
		for (int j = 0; j < k; j++)
		{
			if (i == j)
				continue;
			if (IsB1onsamelineB2(arBBoxes[i], arBBoxes[j]))
			{
				checked = true;
				break;
			}
		}
		if (checked == true)
			tmpBoxes.push_back(arBBoxes[i]);
	}
	arBBoxes.clear();
	arBBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void RemoveUnbalancedRatio(vector<Rect> &arBBoxes)
{
	vector<Rect> tmpBoxes;
	int k = arBBoxes.size();
	for (int i = 0; i < k; i++)
	{
		if (IsB1Balanced(arBBoxes[i]) == true)
		{
			tmpBoxes.push_back(arBBoxes[i]);
		}
	}
	arBBoxes.clear();
	arBBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void MergeInsideBoxes(vector<Rect> &arBBoxes)
{
	vector<Rect> tmpBoxes;
	int k = arBBoxes.size();
	// check boxes inside
	for (int i = 0; i < k; i++)
	{
		bool checked = false;
		for (int j = 0; j < k; j++)
		{
			if (i == j)
				continue;
			if (IsB1insideB2(arBBoxes[i], arBBoxes[j]))
			{
				checked = true;
				break;
			}
		}
		if (checked == false)
		{
			tmpBoxes.push_back(arBBoxes[i]);
		}
	}
	// cleaning
	arBBoxes.clear();
	arBBoxes = tmpBoxes;
	tmpBoxes.clear();
	// check too big box
	k = arBBoxes.size();
	for (int i = 0; i < k; i++)
	{
		int count = 0;
		for (int j = 0; j < k; j++)
		{
			if (i == j)
				continue;
			Rect A = arBBoxes[i] & arBBoxes[j];
			if (A.area() == arBBoxes[j].area())
				count++;
		}
		if (count < 7)
		{
			tmpBoxes.push_back(arBBoxes[i]);
		}
	}
	// cleaning
	arBBoxes.clear();
	arBBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void SortYCoordinate(vector<Rect> &arBBoxes)
{
	sort(arBBoxes.begin(), arBBoxes.end(), CompareYCoordinate);
}

void SortArea(vector<Rect> &arBBoxes)
{
	sort(arBBoxes.begin(), arBBoxes.end(), CompareArea);
}

void SortXCoordinate(vector<Rect> &arBBoxes)
{
	sort(arBBoxes.begin(), arBBoxes.end(), CompareXCoordinate);
}

bool IsB1onsamelineB2(Rect B1, Rect B2)
{
	// range from center to upper or lower
	int range1 = B1.height / 4;
	int c1y = B1.y + (B1.height / 2);
	int range2 = B2.height / 4;
	int c2y = B2.y + (B2.height / 2);
	// check
	int dif = abs(c2y - c1y);
	if (dif < (range1 + range2))
	{
		return true;
	}
	return false;
}

bool IsB1Balanced(Rect B1)
{
	if ((B1.width / B1.height) >= 6)
	{
		return false;
	}
	if ((B1.height / B1.width) >= 6)
	{
		return false;
	}
	if (B1.height < 9)
	{
		return false;
	}
	return true;
}

bool IsB1insideB2(Rect B1, Rect B2)
{
	Rect a = B1 & B2;
	if (a.area() == B1.area() && (B2.area() / B1.area()) <= 3)
		return true;
	return false;

}

bool CompareYCoordinate(Rect B1, Rect B2)
{
	return (B1.y < B2.y);
}

bool CompareArea(Rect B1, Rect B2)
{
	return (B1.area() < B2.area());
}

bool CompareXCoordinate(Rect B1, Rect B2)
{
	return (B1.x < B2.x);
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
	// init variables
	fTimeRunning = 0.00;
    	vector<Rect> arBBoxes;		// bounding boxes
    	clock_t start = clock();
	Mat mOriGS;					// original gray scale image
	Mat mOriSharpGS;			// original sharpening grayscale image
	bool bDebug = false;		// bDebug = true -> output add rects after post-processing to original images to have an overview
	vector<tsOtherBox> atsOtherBoxes;
	
	// read image in gray scale
	ReadImageGrayScale(strInput, mOriGS);
	// sharpen image
	SharpenImage(mOriGS, mOriSharpGS);
	
	// MSER
	MSEROneImage(mOriSharpGS, arBBoxes);
	
	// sort area ascending
	SortArea(arBBoxes);
	// remove areas of stand alone single box
	RemoveUnusualAreaBoxes(arBBoxes);
	// remove unbalanced ratio width, height
	RemoveUnbalancedRatio(arBBoxes);
	// sort y coordinate ascending
	SortYCoordinate(arBBoxes);
	// remove single box text line
	RemoveSingleBoxTextLine(arBBoxes);
	// merge inside box
	MergeInsideBoxes(arBBoxes);
	
	// convert to tsOtherBox and tsLine
	ConvertFromBBoxesToOtherBoxes(strInput, arBBoxes, atsOtherBoxes);
	// merge line box
	
	// calculate running time
	fTimeRunning += (float)(clock() - start) / (float)CLOCKS_PER_SEC;
	// binding running time
	
	// write to files
	
	// write image
}

void ReadImageGrayScale(string strPath, Mat &mInput)
{
	Mat src = imread(strPath);
	cvtColor(src, mInput, CV_BGR2GRAY);
}

void SharpenImage(Mat &mInput, Mat &mOutput, double sigma, double threshold, double amount)
{
	Mat blurred;
	GaussianBlur(mInput, blurred, Size(), sigma, sigma);

	Mat lowContrastMask = abs(mInput - blurred) < threshold;

	mOutput = mInput*(1 + amount) + blurred*(-amount);

	mInput.copyTo(mOutput, lowContrastMask);
}

void MSEROneImage(Mat &input, vector<Rect> &arBBoxes)
{
	// run mser
	vector<vector<Point>> contours;
	Ptr<MSER> mser = MSER::create(
		21,
		(int)(0.00002*input.cols*input.rows),
		(int)(0.05*input.cols*input.rows),
		1, 0.7
	);
	mser->detectRegions(input, contours, arBBoxes);

	// mser regions have a deffect: many duplicated Rects
	RemoveDuplicatedBoxes(arBBoxes);
	// cleaning
	contours.clear();
}

void ConvertFromBBoxesToOtherBoxes(string strImagename, vector<Rect> &arBBoxes, vector<tsOtherBox> &atsOtherboxes)
{
	int nSize = arBBoxes.size();
	if (nSize == 0)
		return;
	for(int nI = 0; nI < nSize; nI++)
	{
		// convert bboxes
		tsOtherBox tsTmp = tsOtherBox(nI, arBBoxes[nI].x, arBBoxes[nI].y, arBBoxes[nI].width, arBBoxes[nI].height
					      0,0,0,0, strImagename, 1, 0);
		atsOtherboxes.push_back(tsTmp);
		tsTmp.Destroy();
	}
	return;
}

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/



//----------------------------------------------------------------------

