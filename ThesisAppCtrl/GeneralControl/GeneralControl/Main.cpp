﻿//////////////////////////////////////////////////////////////////////
//
//	File name：	Source.cpp
//	Description：	Main process of OCRAutorun project
//	Notes:			None
//	History：	<0> 2017.12.13 : Dang Tuan Vu : Create file
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

// strInput is an absolute path of a file
string GetFileNameFromPath(string strInput);

// strInput is the absolute path of the *.exe file
// return value: the absolute path of Root folder, -which is the folder contains this file.
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

// increase each rect: left, right, bottom, top values. if all values < 10, + 1 pixel, else + 2 pixels
// this function just make OCR run better
void IncreaseRectToBoxes(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

// bindg computing running time to each box
// this computing time is the average running time
void BindingRunningTimeToBox(float fTime, vector<tsOtherBox> &atsOtherBoxes, 
	vector<tsLineBox> &atsLines);

// add all current rect to the original image and write the image to folder Root/Debug
// strInput is the absolute path of the image file.
void AddRectToOriginalImage(string strInput, vector<tsOtherBox> &atsOtherBoxes, 
	vector<tsLineBox> &atsLines);

// nPos is the start search position
// nSize is the size of aFreeList
// search and merge tsOtherBox into a tsLineBox and add to atsLines
// this function support function MergeLineBox, use for merge intersect or lying next boxes only
void GetALineBoxFromIntersectBoxes(int &nPos, int &nSize, vector<int> &aFreeList,
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

// nPos is the start search position
// nSize is the size of aFreeList
// search and merge tsOtherBox into a tsLineBox and add to atsLines
// this function support function MergeLineBox, use for merge seperated boxes only
void GetALineBoxFromSeperateBoxes(int &nPos, int &nSize, vector<int> &aFreeList, 
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

// Remove similar indexes from vector A inside vector B
void RemoveSameIndexesFromAInB(vector<int> &A, vector<int> &B);

// Remove OtherBoxes which have been merge into Lines
void RemoveOtherBoxesMergeInLines(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

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

// check if a point is inside a Rect
bool IsPointAInsideRectB(int nXA, int nYA, Rect rB);

// intersect or less than 4 pixels seperate rect
// h / h > 0.8 & < 1.25
// w / w > 0.7 & < 1.43
// y + 1/2 h > y && y - 1/2 h < y
// nX of A is on the left of nX of B
bool CheckConditionOfMergIntersectBoxes(tsOtherBox A, tsOtherBox B);

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
// bDebug = true: add rects back as in ProcessOneImage function, not doing crop ROI from the original image
void Run(int argc, char **argv, bool bDebug);

// input an absolute path of a image, get out result: otherboxes, lines and time running
// preprocessing -	Sharpen, grayscale
// processing -		MSER
// postprocessing - Remove Unusual Area Boxes; remove unbalanced ratio width, height
//					Sort y coordinate ascending; remove single box text line
//					Merge inside box
// bDebug = true: add rects back to the original image for a fast result examination.
void ProcessOneImage(string strInput, float &fTimeRunning, vector<tsLineBox> &atsLines, 
	vector<tsOtherBox> &atsOtherBoxes, bool bDebug);

// read an absolute path of an image into a Mat with grayscale format
void ReadImageGrayScale(string strPath, Mat &mInput);

// sharpen an Mat
// using gaussian formula
void SharpenImage(Mat &mInput, Mat &mOutput, double sigma = 1, double threshold = 5, double amount = 1);

// MSER method to get bounding boxes
void MSEROneImage(Mat &input, vector<Rect> &arBBoxes);

// convert from BBoxes to tsOtherBox
// default number version is 1, more version = increase the number version
void ConvertFromBBoxesToOtherBoxes(string strImagename, vector<Rect> &arBBoxes, 
	vector<tsOtherBox> &atsOtherboxes);

// from OtherBoxes, merge possible boxes that can be assemble into a line
void MergeLineBox(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/

// read otherboxes from txt file
void ReadOtherBoxesDataFile(string strOtherBoxPath, vector<tsOtherBox> &atsOtherBoxes);

// write data to OtherBoxes.txt and Lines.txt in folder TmpRect
void WriteDataToTxtFile(string strOtherBoxPath, string strLinesPath, 
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

// crop rects in the original image to folder TmpImage
// strInput is the absolute path of the original image
// strTmpImage is the absolute path of TmpImage folder
void CropROIByDataToFolderImage(string strTmpImage, string strInput, 
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Test function		*/

// Test get a line from intersect box
void TestGetLineIntersect();

// reduce a number of otherboxes by a width range
// max = 0 => NOT have upper limit
// min = 0 => NOT have lower limit
void CutDownOtherBoxesByXNumber(vector<tsOtherBox> &atsOtherBoxes, int start, int max);

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
	// program Run 
	//Run(argc, argv, true);

	// Test part
	TestGetLineIntersect();

	return 1;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Support functions		*/

string GetFileNameFromPath(string strInput)
{
	int nSize = strInput.size();
	if (nSize == 0)
		return "";
	int nPos = nSize - 1;
	int nStart = 0;
	int nEnd = 0;
	// get end of file name
	while (nPos > 0 && strInput[nPos] != '.' && strInput[nPos] != '/' && strInput[nPos] != '\\')
	{
		nPos--;
	}
	if (nPos == 0)
		return "";
	if (strInput[nPos] == '/' || strInput[nPos] == '\\')
	{
		// no extension in this file
		// => get both nEnd and nStart
		nEnd = nSize;
		nStart = nPos + 1;
	}
	else
	{
		nEnd = nPos;
		// get start of file name
		while (nPos > 0 && strInput[nPos] != '\\' && strInput[nPos] != '/')
		{
			nPos--;
		}
		if (nPos == 0)
			return "";
		nStart = nPos + 1;
	}
	// create new char array
	nSize = nEnd - nStart;
	char *aTmp = new char[nEnd - nStart + 1];
	for (int nI = 0; nI < nSize; nI++)
	{
		aTmp[nI] = strInput[nStart + nI];
	}
	aTmp[nSize] = 0;
	string strResult = string(aTmp);
	delete[] aTmp;
	return strResult;
}

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

void IncreaseRectToBoxes(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	int nSize = atsOtherBoxes.size();
	for (int nI = 0; nI < nSize; nI++)
	{
		// check case 1
		if (atsOtherBoxes[nI].rROI.nWidth < 10 && atsOtherBoxes[nI].rROI.nHeight < 10)
		{
			// + 1 pixel
			if (atsOtherBoxes[nI].rROI.nX > 0)
			{
				atsOtherBoxes[nI].rROI.nX = atsOtherBoxes[nI].rROI.nX - 1;
			}
			if (atsOtherBoxes[nI].rROI.nY > 0)
			{
				atsOtherBoxes[nI].rROI.nY = atsOtherBoxes[nI].rROI.nY - 1;
			}
			atsOtherBoxes[nI].rROI.nWidth = atsOtherBoxes[nI].rROI.nWidth + 2;
			atsOtherBoxes[nI].rROI.nHeight = atsOtherBoxes[nI].rROI.nHeight + 2;
		}
		else
		{
			// + 2 pixel
			// + 1 pixel
			if (atsOtherBoxes[nI].rROI.nX > 1)
			{
				atsOtherBoxes[nI].rROI.nX = atsOtherBoxes[nI].rROI.nX - 2;
			}
			if (atsOtherBoxes[nI].rROI.nY > 1)
			{
				atsOtherBoxes[nI].rROI.nY = atsOtherBoxes[nI].rROI.nY - 2;
			}
			atsOtherBoxes[nI].rROI.nWidth = atsOtherBoxes[nI].rROI.nWidth + 4;
			atsOtherBoxes[nI].rROI.nHeight = atsOtherBoxes[nI].rROI.nHeight + 4;
		}
	}
	nSize = atsLines.size();
	for (int nI = 0; nI < nSize; nI++)
	{
		// check case 1
		if (atsLines[nI].tsCore.rROI.nWidth < 10 && atsLines[nI].tsCore.rROI.nHeight < 10)
		{
			// + 1 pixel
			if (atsLines[nI].tsCore.rROI.nX > 0)
			{
				atsLines[nI].tsCore.rROI.nX = atsLines[nI].tsCore.rROI.nX - 1;
			}
			if (atsLines[nI].tsCore.rROI.nY > 0)
			{
				atsLines[nI].tsCore.rROI.nY = atsLines[nI].tsCore.rROI.nY - 1;
			}
			atsLines[nI].tsCore.rROI.nWidth = atsLines[nI].tsCore.rROI.nWidth + 2;
			atsLines[nI].tsCore.rROI.nHeight = atsLines[nI].tsCore.rROI.nHeight + 2;
		}
		else
		{
			// + 2 pixel
			// + 1 pixel
			if (atsLines[nI].tsCore.rROI.nX > 1)
			{
				atsLines[nI].tsCore.rROI.nX = atsLines[nI].tsCore.rROI.nX - 2;
			}
			if (atsLines[nI].tsCore.rROI.nY > 1)
			{
				atsLines[nI].tsCore.rROI.nY = atsLines[nI].tsCore.rROI.nY - 2;
			}
			atsLines[nI].tsCore.rROI.nWidth = atsLines[nI].tsCore.rROI.nWidth + 4;
			atsLines[nI].tsCore.rROI.nHeight = atsLines[nI].tsCore.rROI.nHeight + 4;
		}
	}
}

void BindingRunningTimeToBox(float fTime, vector<tsOtherBox> &atsOtherBoxes, 
	vector<tsLineBox> &atsLines)
{
	float average = fTime / ((int)(atsOtherBoxes.size() + atsLines.size()));
	int nSize = atsOtherBoxes.size();
	for (int nI = 0; nI < nSize; nI++)
	{
		atsOtherBoxes[nI].fTimeRunning = average;
	}
	nSize = atsLines.size();
	for (int nI = 0; nI < nSize; nI++)
	{
		atsLines[nI].tsCore.fTimeRunning = average;
	}
	return;
}

void AddRectToOriginalImage(string strInput, vector<tsOtherBox> &atsOtherBoxes, 
	vector<tsLineBox> &atsLines)
{
	// get root folder
	string strRootFolder = GetRootFolder(strInput);
	string strFilePath = strRootFolder + GetFileNameFromPath(strInput) + "-debug.jpg";
	Mat mInput = imread(strInput);
	int nSize = atsOtherBoxes.size();
	Rect rTmp = Rect(0, 0, 0, 0);
	// copy rects from OtherBoxes to the image
	for (int nI = 0; nI < nSize; nI++)
	{
		rTmp.x = atsOtherBoxes[nI].rROI.nX;
		rTmp.y = atsOtherBoxes[nI].rROI.nY;
		rTmp.width = atsOtherBoxes[nI].rROI.nWidth;
		rTmp.height = atsOtherBoxes[nI].rROI.nHeight;
		rectangle(mInput, rTmp, CV_RGB(0, 255, 0), 2);
	}
	nSize = atsLines.size();
	// copy rects from Lines to the image
	for (int nI = 0; nI < nSize; nI++)
	{
		rTmp.x = atsLines[nI].tsCore.rROI.nX;
		rTmp.y = atsLines[nI].tsCore.rROI.nY;
		rTmp.width = atsLines[nI].tsCore.rROI.nWidth;
		rTmp.height = atsLines[nI].tsCore.rROI.nHeight;
		rectangle(mInput, rTmp, CV_RGB(0, 255, 0), 2);
	}
	// write image to file
	imwrite(strFilePath, mInput);
	return;
}

void GetALineBoxFromIntersectBoxes(int &nPos, int &nSize, vector<int> &aFreeList,
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	// check condition
	if (nPos == nSize || (nPos + 1) == nSize || (nPos + 2) == nSize)
	{
		nPos = nSize;
		return;
	}
	vector<int> aCurLine;
	aCurLine.push_back(nPos);
	int nPosCurLine = 0;		// the current position of aCurLine to search Rect
	for (int nI = nPos + 1; nI < nSize; nI++)
	{
		// find intersect or at least only 1-3 pixels seperated for width < 50 pixels
		// height / height > 0.8 & < 1.2
		// width / width > 0.7 & < 1.3
		if (CheckConditionOfMergIntersectBoxes(atsOtherBoxes[aFreeList[aCurLine[nPosCurLine]]], 
			atsOtherBoxes[aFreeList[nI]]) == true)
		{
			// add to CurLine
			aCurLine.push_back(nI);
			nPosCurLine++;
		}
	}
	// check line's points count
	if (aCurLine.size() < 3)
	{
		nPos++;
		aCurLine.clear();
		return;
	}
	// create new line
	tsLineBox tsLineTmp;
	// create new ID = last highest ID + 1
	int nNewID = 0;
	if (atsLines.size() == 0)
	{
		nNewID = atsOtherBoxes[atsOtherBoxes.size() - 1].nID + 1;
	}
	else
	{
		nNewID = atsLines[atsLines.size() - 1].tsCore.nID + 1;
	}
	tsLineTmp.tsCore.nID = nNewID;
	// get original image's name
	tsLineTmp.tsCore.strNameImage = atsOtherBoxes[aFreeList[aCurLine[0]]].strNameImage;
	// binding default value
	tsLineTmp.tsCore.fTimeRunning = 0;
	tsLineTmp.tsCore.rACVROI = tsRect(0, 0, 0, 0);
	// default number version is 1
	tsLineTmp.tsCore.nNumberVersion = 1;
	// copy IDs and Rects from tsOtherBoxes to new tsLineBox
	for (int nJ = 0; nJ < aCurLine.size(); nJ++)
	{
		tsLineTmp.anSubID.push_back(atsOtherBoxes[aFreeList[aCurLine[nJ]]].nID);
		tsLineTmp.atsSubROI.push_back(tsRect(atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nX,
			atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nY,
			atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nWidth,
			atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nHeight));
	}
	// create new Rect cover all old rects
	tsLineTmp.tsCore.InputROIByCreateCoverRect(tsLineTmp.atsSubROI);
	// add to atsLine
	atsLines.push_back(tsLineTmp);
	// remove indexes in aFreeList
	RemoveSameIndexesFromAInB(aCurLine, aFreeList);
	// nPos keep the same, because the current pos is replaced
	// update nSize
	nSize = aFreeList.size();
	// clear aCurLine
	aCurLine.clear();
}

// have bugs
void GetALineBoxFromSeperateBoxes(int &nPos, int &nSize, vector<int> &aFreeList, 
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	// check condition
	if (nPos == nSize || (nPos + 1) == nSize || (nPos + 2) == nSize)
	{
		nPos = nSize;
		return;
	}
	// aCurLine contains list of index that forms a line in aFreeList
	vector<int> aCurLine;
	// create search area rectangle - same x y height, width*3
	Rect rSearchRect = Rect(atsOtherBoxes[aFreeList[nPos]].rROI.nX, atsOtherBoxes[aFreeList[nPos]].rROI.nY, 
		(atsOtherBoxes[aFreeList[nPos]].rROI.nWidth * 3), atsOtherBoxes[aFreeList[nPos]].rROI.nHeight);
	aCurLine.push_back(nPos);
	int nPosCurLine = 0;
	for (int nI = nPos + 1; nI < nSize; nI++)
	{
		// Check if the center of mass of the searching rect is in the rSearchRect,
		// and the height is not > +30% of rSearchRect, width is not > +100% of rSearchRect
		// Get Center of mass
		int nCX = atsOtherBoxes[aFreeList[nI]].rROI.nX + atsOtherBoxes[aFreeList[nI]].rROI.nWidth / 2;
		int nCY = atsOtherBoxes[aFreeList[nI]].rROI.nY + atsOtherBoxes[aFreeList[nI]].rROI.nHeight / 2;
		// check location
		if (IsPointAInsideRectB(nCX, nCY, rSearchRect) == true)
		{
			// check height and width
			if ((atsOtherBoxes[aFreeList[nPosCurLine]].rROI.nWidth * 2) > atsOtherBoxes[aFreeList[nI]].rROI.nWidth &&
				(atsOtherBoxes[aFreeList[nPosCurLine]].rROI.nHeight * 1.3) > atsOtherBoxes[aFreeList[nI]].rROI.nHeight)
			{
				// check condition base on average width of boxes in the aCurLine
				// special case, aCurLine only has the start point or 2 points
				// => check condition of height only
				if (aCurLine.size() == 1 || aCurLine.size() == 2
					&& (atsOtherBoxes[aFreeList[nI]].rROI.nHeight / rSearchRect.height) > 0.7
					&& (rSearchRect.height / atsOtherBoxes[aFreeList[nI]].rROI.nHeight) < 1.3)
				{
					// add to line
					aCurLine.push_back(nI);
					// create new search rect
					rSearchRect = Rect(atsOtherBoxes[aFreeList[nI]].rROI.nX, atsOtherBoxes[aFreeList[nI]].rROI.nY,
						(atsOtherBoxes[aFreeList[nI]].rROI.nWidth * 3), atsOtherBoxes[aFreeList[nI]].rROI.nHeight);
					// increase cur pos in CurLine
					nPosCurLine++;
				}
				// normal case, more than 3 points in the aCurLine
				else
				{
					// check condition of width - not too far from the average width
					int nAverWidth = 0;
					for (int nJ = 0; nJ < aCurLine.size(); nJ++)
					{
						nAverWidth += atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nWidth;
					}
					nAverWidth = nAverWidth / aCurLine.size();
					// 30% width extend from average width
					// 30% height extend
					if (nCX < (rSearchRect.x + (nAverWidth *1.3)) 
						&& (atsOtherBoxes[aFreeList[nI]].rROI.nHeight / rSearchRect.height) > 0.7
						&& (rSearchRect.height / atsOtherBoxes[aFreeList[nI]].rROI.nHeight) < 1.3)
					{
						// add to line
						aCurLine.push_back(nI);
						// create new search rect
						rSearchRect = Rect(atsOtherBoxes[aFreeList[nI]].rROI.nX, atsOtherBoxes[aFreeList[nI]].rROI.nY,
							(atsOtherBoxes[aFreeList[nI]].rROI.nWidth * 3), atsOtherBoxes[aFreeList[nI]].rROI.nHeight);
						// increase cur pos in CurLine
						nPosCurLine++;
					}
				}
			}
		}
	}
	// find less than 2 points to form a line with the start point
	if (aCurLine.size() < 3)
	{
		aCurLine.clear();
		// increase position
		nPos++;
		return;
	}
	// find more than 1 point to form a line with the start point
	// => create new line, remove index from aFreeList
	else
	{
		tsLineBox tsLineTmp;
		// create new ID = last highest ID + 1
		int nNewID = 0;
		if (atsLines.size() == 0)
		{
			nNewID = atsOtherBoxes[atsOtherBoxes.size() - 1].nID + 1;
		}
		else
		{
			nNewID = atsLines[atsLines.size() - 1].tsCore.nID + 1;
		}
		tsLineTmp.tsCore.nID = nNewID;
		// get original image's name
		tsLineTmp.tsCore.strNameImage = atsOtherBoxes[aFreeList[aCurLine[0]]].strNameImage;
		// binding default value
		tsLineTmp.tsCore.fTimeRunning = 0;
		tsLineTmp.tsCore.rACVROI = tsRect(0, 0, 0, 0);
		// default number version is 1
		tsLineTmp.tsCore.nNumberVersion = 1;
		// copy IDs and Rects from tsOtherBoxes to new tsLineBox
		for (int nJ = 0; nJ < aCurLine.size(); nJ++)
		{
			tsLineTmp.anSubID.push_back(atsOtherBoxes[aFreeList[aCurLine[nJ]]].nID);
			tsLineTmp.atsSubROI.push_back(tsRect(atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nX, 
				atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nY,
				atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nWidth, 
				atsOtherBoxes[aFreeList[aCurLine[nJ]]].rROI.nHeight));
		}
		// create new Rect cover all old rects
		tsLineTmp.tsCore.InputROIByCreateCoverRect(tsLineTmp.atsSubROI);
		// add to atsLine
		atsLines.push_back(tsLineTmp);
		// remove indexes in aFreeList
		RemoveSameIndexesFromAInB(aCurLine, aFreeList);
		// nPos keep the same, because the current pos is replaced
		// update nSize
		nSize = aFreeList.size();
		// clear aCurLine
		aCurLine.clear();
	}
}

void RemoveSameIndexesFromAInB(vector<int> &A, vector<int> &B)
{
	vector<int> aTmp;
	int nPos = 0;
	int nSize = A.size();
	for (int nI = 0; nI < B.size(); nI++)
	{
		if (nPos == nSize)
		{
			// load the rest number in B to aTmp
			aTmp.push_back(B[nI]);
			continue;
		}
		if (nI == A[nPos])
		{
			nPos++;
			continue;
		}
		else if (nI < A[nPos])
		{
			aTmp.push_back(B[nI]);
		}
		else if (nI > A[nPos])
		{
			// increase nPos to when B[nI] < A[nPos]
			nI--;
			nPos++;
			continue;
		}
	}
	B.clear();
	B = aTmp;
}

void RemoveOtherBoxesMergeInLines(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	int nSize = atsLines.size();
	vector<int> aRemoveIndexes;
	// get remove indexes list
	for (int nI = 0; nI < nSize; nI++)
	{
		for (int nJ = 0; nJ < atsLines[nI].anSubID.size(); nJ++)
		{
			aRemoveIndexes.push_back(atsLines[nI].anSubID[nJ]);
		}
	}
	vector<tsOtherBox> atsTmp;
	nSize = atsOtherBoxes.size();
	int nPos = 0;
	int nSize2 = aRemoveIndexes.size();
	for (int nI = 0; nI < nSize; nI++)
	{
		if (nPos == nSize2)
		{
			// load the rest
			atsTmp.push_back(atsOtherBoxes[nI]);
			continue;
		}
		if (nI < aRemoveIndexes[nPos])
		{
			atsTmp.push_back(atsOtherBoxes[nI]);
			continue;
		}
		if (nI == aRemoveIndexes[nPos])
		{
			nPos++;
			continue;
		}
		if (nI > aRemoveIndexes[nPos])
		{
			nI--;
			nPos++;
			continue;
		}
	}
	atsOtherBoxes.clear();
	atsOtherBoxes = atsTmp;
	atsTmp.clear();
	aRemoveIndexes.clear();
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

bool IsPointAInsideRectB(int nXA, int nYA, Rect rB)
{
	if (nXA >= rB.x && nYA >= rB.y && nXA <= (rB.x + rB.width) && nYA <= (rB.y + rB.height))
	{
		return true;
	}
	return false;
}

bool CheckConditionOfMergIntersectBoxes(tsOtherBox A, tsOtherBox B)
{
	// check y position, cannot be too different
	if ((A.rROI.nY + A.rROI.nHeight / 2) > B.rROI.nY && (A.rROI.nY - A.rROI.nHeight / 2) < B.rROI.nY)
	{
		// check intersect or seperate in acceptable limit - 3 pixels
		if ((B.rROI.nX - (A.rROI.nX + A.rROI.nWidth)) < 4)
		{
			// check height ratio
			if ((A.rROI.nHeight / B.rROI.nHeight) > 0.8 && (B.rROI.nHeight / A.rROI.nHeight) < 1.25)
			{
				// check width ratio
				if ((A.rROI.nWidth / B.rROI.nWidth) > 0.7 && (B.rROI.nWidth / A.rROI.nWidth) < 1.43)
				{
					
					return true;
				}
			}
		}
	}
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

void Run(int argc, char **argv, bool bDebug)
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
		ProcessOneImage(astrListPaths[nI], fTimeRun, atsLines, atsOtherBoxes, bDebug);
		// get paths
		string strOBPath = strRootFolder + "\\TmpRect\\OtherBoxes.txt";
		string strOBImagePath = strRootFolder + "\\TmpRect\\" + GetFileNameFromPath(astrListPaths[nI]) + "-" + "OtherBoxes.txt";
		string strLinesPath = strRootFolder + "\\TmpRect\\Lines.txt";
		string strLinesImagePath = strRootFolder + "\\TmpRect\\" + GetFileNameFromPath(astrListPaths[nI]) + "-" + "Lines.txt";
		string strTmpImage = strRootFolder + "\\TmpImage\\";
		// write to file
		WriteDataToTxtFile(strOBPath, strLinesPath, atsOtherBoxes, atsLines);
		WriteDataToTxtFile(strOBImagePath, strLinesImagePath, atsOtherBoxes, atsLines);
		// make images
		if (bDebug == false)
		{
			CropROIByDataToFolderImage(strTmpImage, astrListPaths[nI], atsOtherBoxes, atsLines);
		}
		cout << "Finish an image at Path: " << astrListPaths[nI] << endl;
		// clean
		atsLines.clear();
		atsOtherBoxes.clear();
	}
	astrListPaths.clear();
}

void ProcessOneImage(string strInput, float &fTimeRunning, vector<tsLineBox> &atsLines, 
	vector<tsOtherBox> &atsOtherBoxes, bool bDebug)
{
	// init variables
	fTimeRunning = 0.00;
    	vector<Rect> arBBoxes;		// bounding boxes
    	clock_t start = clock();
	Mat mOriGS;					// original gray scale image
	Mat mOriSharpGS;			// original sharpening grayscale image
		
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
	// sort x coordinate ascending
	SortXCoordinate(arBBoxes);
	
	// convert to tsOtherBox and tsLine
	ConvertFromBBoxesToOtherBoxes(GetFileNameFromPath(strInput), arBBoxes, atsOtherBoxes);
	arBBoxes.clear();
	// merge line box
	MergeLineBox(atsOtherBoxes, atsLines);
	// increase each rect: left, right, top, bottom value + 1 pixel if all of them < 10, else + 2 pixels
	// it make the OCR recognize text easier
	IncreaseRectToBoxes(atsOtherBoxes, atsLines);
	// calculate running time
	fTimeRunning += (float)(clock() - start) / (float)CLOCKS_PER_SEC;
	// binding running time
	BindingRunningTimeToBox(fTimeRunning, atsOtherBoxes, atsLines);
	// debug mode
	if (bDebug == true)
	{
		// add current rect to the origin image, then write image to folder which has the original image
		AddRectToOriginalImage(strInput, atsOtherBoxes, atsLines);
	}
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

void ConvertFromBBoxesToOtherBoxes(string strImagename, vector<Rect> &arBBoxes, 
	vector<tsOtherBox> &atsOtherboxes)
{
	int nSize = arBBoxes.size();
	if (nSize == 0)
		return;
	for(int nI = 0; nI < nSize; nI++)
	{
		// convert bboxes
		tsOtherBox tsTmp = tsOtherBox(nI, arBBoxes[nI].x, arBBoxes[nI].y, arBBoxes[nI].width, arBBoxes[nI].height,
					      0,0,0,0, strImagename, 1, 0);
		atsOtherboxes.push_back(tsTmp);
		tsTmp.Destroy();
	}
	return;
}

void MergeLineBox(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	// Condition: OtherBoxes have been sorted by X coordinate before this function
	// Method: search from left to right
	// How it run:
	//		- set nPos = 0, this pos is the pos in the list below
	// 		- Have a list of free OtherBoxes - boxes that have not been included in any line.
	//		- Search from left to right, find boxes which satisfy condition
	//		- If > 2 boxes in a line, create a Line, add to array atsLines
	//		- continue to do this until the nPos meet the end of list
	int nPos = 0;
	int nSize = atsOtherBoxes.size();
	vector<int> aFreeList;			// free list is the list of free indexes which can be searched
	for(int nI = 0; nI < nSize; nI++)
	{
		aFreeList.push_back(nI);
	}
	// for intersect or next boxes
	nSize = aFreeList.size();
	while (nPos < nSize && nSize != 0)
	{
		// handle function for intersect or next boxes
		GetALineBoxFromIntersectBoxes(nPos, nSize, aFreeList, atsOtherBoxes, atsLines);
	}
	// Remove OtherBoxes which have been merge into Lines
	RemoveOtherBoxesMergeInLines(atsOtherBoxes, atsLines);

	// init again
	//aFreeList.clear();
	//nSize = atsOtherBoxes.size();
	//for (int nI = 0; nI < nSize; nI++)
	//{
	//	aFreeList.push_back(nI);
	//}
	//// for seperated boxes
	//while(nPos < nSize && nSize != 0)
	//{
	//	// handle function for seperated boxes
	//	GetALineBoxFromSeperateBoxes(nPos, nSize, aFreeList, atsOtherBoxes, atsLines);
	//}
	//// Remove OtherBoxes which have been merge into Lines
	//RemoveOtherBoxesMergeInLines(atsOtherBoxes, atsLines);

	// clear
	aFreeList.clear();
}



//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/

void ReadOtherBoxesDataFile(string strOtherBoxPath, vector<tsOtherBox> &atsOtherBoxes)
{
	BBoxIOStream bboxTmp;
	// get list info
	bboxTmp.ReadOtherBoxes(atsOtherBoxes, strOtherBoxPath);
}

void WriteDataToTxtFile(string strOtherBoxPath, string strLinesPath, 
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	BBoxIOStream bboxTmp;
	bool bChecked = false;
	bChecked = bboxTmp.WriteOtherBoxes(atsOtherBoxes, strOtherBoxPath);
	if (bChecked == false)
	{
		cout << "Failed to write OtherBoxes at path: " << strOtherBoxPath << endl;
	}
	bChecked = false;
	bChecked = bboxTmp.WriteLines(atsLines, strLinesPath);
	if (bChecked == false)
	{
		cout << "Failed to write Lines at path: " << strLinesPath << endl;
	}
}

void CropROIByDataToFolderImage(string strTmpImage, string strInput, 
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	Mat mInput = imread(strInput);
	int nSize = atsOtherBoxes.size();
	// crop ROIs for OtherBoxes
	for (int nI = 0; nI < nSize; nI++)
	{
		cv::Rect myROI(atsOtherBoxes[nI].rROI.nX, atsOtherBoxes[nI].rROI.nY, atsOtherBoxes[nI].rROI.nWidth, atsOtherBoxes[nI].rROI.nHeight);
		if (myROI.x >= 0 && myROI.y >= 0 && (myROI.width + myROI.x) < mInput.cols && (myROI.height + myROI.y) < mInput.rows)
		{
			// your code
			cv::Mat croppedImage = mInput(myROI);
			imwrite(strTmpImage + atsOtherBoxes[nI].strNameImage + "-" + to_string(atsOtherBoxes[nI].nID) + "-" + to_string(atsOtherBoxes[nI].nNumberVersion) + ".jpg", croppedImage);
		}
	}
	nSize = atsLines.size();
	// crop ROIs for Lines
	for (int nI = 0; nI < nSize; nI++)
	{
		cv::Rect myROI(atsLines[nI].tsCore.rROI.nX, atsLines[nI].tsCore.rROI.nY, atsLines[nI].tsCore.rROI.nWidth, atsLines[nI].tsCore.rROI.nHeight);
		if (myROI.x >= 0 && myROI.y >= 0 && (myROI.width + myROI.x) < mInput.cols && (myROI.height + myROI.y) < mInput.rows)
		{
			// your code
			cv::Mat croppedImage = mInput(myROI);
			imwrite(strTmpImage + atsLines[nI].tsCore.strNameImage + "-" + to_string(atsLines[nI].tsCore.nID) + "-" + to_string(atsLines[nI].tsCore.nNumberVersion) + ".jpg", croppedImage);
		}
	}
}


//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Test function		*/

// Test get a line from intersect box
void TestGetLineIntersect()
{
	string strOtherBoxPath = "E:\\Code\\OCR-Five-Git\\Root\\TmpRect\\img250-OtherBoxes.txt";
	vector<tsOtherBox> atsOtherBoxes;
	vector<tsLineBox> atsLines;
	ReadOtherBoxesDataFile(strOtherBoxPath, atsOtherBoxes);
	string strImagePath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250.jpg";
	CutDownOtherBoxesByXNumber(atsOtherBoxes, 200, 0);
	MergeLineBox(atsOtherBoxes, atsLines);
	AddRectToOriginalImage(strImagePath, atsOtherBoxes, atsLines);



}

void CutDownOtherBoxesByXNumber(vector<tsOtherBox> &atsOtherBoxes, int start, int max)
{
	vector<tsOtherBox> atsTmp;
	int nSize = atsOtherBoxes.size();
	for (int nI = 0; nI < nSize; nI++)
	{

	}
}


//----------------------------------------------------------------------