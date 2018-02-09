//////////////////////////////////////////////////////////////////////
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

// Add all current rect to the original image and write the image to folder Root/Debug
// with the format [strInput]-debug.jpg
// strInput is the absolute path of the image file.
void AddRectToOriginalImage(string strInput, vector<tsOtherBox> &atsOtherBoxes, 
	vector<tsLineBox> &atsLines);
// strOutput is the file path of output image file
void AddRectToOriginalImage(string strInput, string strOutput, 
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

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

// Remove similar ID from A inside B
void RemoveSameIDFromAInB(vector<tsOtherBox> &A, vector<tsOtherBox> &B);
void RemoveSameIDFromAInB(vector<tsLineBox> &A, vector<tsLineBox> &B);
void RemoveSameIDFromAInB(tsLineBox A, vector<tsLineBox> &B);

// Remove OtherBoxes which have been merge into Lines
void RemoveOtherBoxesMergeInLines(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

// After merge otherboxes into lines, there are many otherboxes which do not have enough condition
// and completely(or at least 85% area) stay inside a Line box => remove these OtherBoxes
void RemoveOtherBoxesInsideLines(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

// merge all lines in atsNeedMerge into 1 line, push_back that line to atsLine
// remove merging lines from atsLines
// need not empty atsLines 
void MergeLineBoxesIntoALine(vector<tsLineBox> &atsNeedMerge, vector<tsLineBox> &atsLines);

// the ID of atsLines is not in an order, need to distribute again after MergeLineBox function
void ReDistributeIDForLines(vector<tsLineBox> &atsLines);

// merge a line with other otherboxes for a condition, they intersect horizontally
// nIndex is the index of merging line
void MergeLineAndOtherBoxes(vector<tsOtherBox> &atsNeedMerge, vector<tsLineBox> &atsLines, int nIndex);

void SortYCoordinate(vector<Rect> &arBBoxes);

void SortArea(vector<Rect> &arBBoxes);

void SortXCoordinate(vector<Rect> &arBBoxes);

void SortXCoordinate(vector<tsLineBox> &atsLines);

// check if any boxes in the same line - +- 50% height
bool IsB1onsamelineB2(Rect B1, Rect B2);

// check if ratio between height and width >= 6 times
// check if height < 9
bool IsB1Balanced(Rect B1);

// check if the intersection area between B1 and B2 is equal B1.area()
// and the B2.area() / B1.area() <= 3
bool IsB1insideB2(Rect B1, Rect B2);

// check if 2 otherboxes intersect or lying next - seperate < 3 pixels
bool IsOtherBoxAIntersectOtherBoxB(tsOtherBox tsA, tsOtherBox tsB);

// check if tsOtherBox A is inside tsLineBox B, use in case rROI only, not ACVowel
// true: inside, false: otherwise
bool IsOtherBoxAInsideLineBoxB(tsOtherBox tsA, tsLineBox tsB);

// check if >= 75% of tsOtherBox A area is inside tsLineBox B, use in case rROI only, not ACVowel
// true: inside, false: otherwise
bool IsOtherBoxAMostlyInsideLineBoxB(tsOtherBox tsA, tsLineBox tsB);

// check if a point is inside a Rect
bool IsPointAInsideRectB(int nXA, int nYA, Rect rB);

// check if tsA intersect tsB on a horizontal definition:
// based on 2 tsRect
bool IsIntersectHorizontally(tsLineBox tsA, tsLineBox tsB);
bool IsIntersectHorizontally(tsLineBox tsA, tsOtherBox tsB);

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

// return true if B1.tsCore.rROI.nX < B2.tsCore.rROI.nX
bool CompareXCoordinateLineBox(tsLineBox B1, tsLineBox B2);

// Merge multiple otherbox into a line
tsLineBox MergeOtherBoxesIntoALine(vector<tsOtherBox> &atsTmp, int nID);


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

// there are > 3 otherboxes intersect each other -> merge them together
// need atsOtherBoxes size > 0
void MergeALotOtherBoxesIntersect(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

// after merge line box, there are cases that line box are intersect with otherboxes on horizontal -> check
// condition of these pair to decide to merge them or not
// need atsLines size > 0
// merge lines and otherboxes into a new line
void MergeLinesAndOtherBoxesHorizontally(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines);

// merge if 2 line box are intersect and satisfy some condition
void MergeLineIntersectHorizontally(vector<tsLineBox> &atsLines);

//----------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Input/Output Stream		*/

// read otherboxes from txt file
void ReadOtherBoxesDataFile(string strOtherBoxPath, vector<tsOtherBox> &atsOtherBoxes);

// read line boxes from txt file
void ReadLinesDataFile(string strLinePath, vector<tsLineBox> &atsLines);

// write otherboxes to text file
void WriteOtherBoxesToFile(string strOtherBoxPath, vector<tsOtherBox> &atsOtherBoxes);

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

// Simulate otherboxes which may create a situation that cause bug
/*		The test rect:
	102 4 27 26;
	127 6 23 27;
	141 9 10 25;
	148 10 24 25;
	170 11 23 26;
	172 21 10 16;
*/
void TestSimulateOtherBoxesC1(vector<tsOtherBox> &atsOtherBoxes);

// Test get a line from intersect box
void TestGetLineIntersect();

// Test get a line from intersect boxes which are detected by mser
void TestGetLineIntersectFromImage();

// Input:
// - The original image path
// - The output path to write down
// - The ID of linebox in Lines txt file
// Output: the image write as the output path.
void TestShowLineBoxOnImage();

// reduce a number of otherboxes by a width range
// max = 0 => NOT have upper limit
// min = 0 => NOT have lower limit
void CutDownOtherBoxesByX(vector<tsOtherBox> &atsOtherBoxes, int start, int max);

// reduce a number of otherboxes by a height range
// max = 0 => NOT have upper limit
// min = 0 => NOT have lower limit
void CutDownOtherBoxesByY(vector<tsOtherBox> &atsOtherBoxes, int start, int max);

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
	Run(argc, argv, true);

	// Test function
	//TestShowLineBoxOnImage();
	char a;
	cout << "Press any key to continue ... ";
	cin >> a;

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
		rectangle(mInput, rTmp, CV_RGB(255, 0, 0), 2);
	}
	// write image to file
	imwrite(strFilePath, mInput);
	return;
}

void AddRectToOriginalImage(string strInput, string strOutput,
	vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	// get root folder
	string strRootFolder = GetRootFolder(strInput);
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
	imwrite(strOutput, mInput);
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
	for (size_t nJ = 0; nJ < aCurLine.size(); nJ++)
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
					for (size_t nJ = 0; nJ < aCurLine.size(); nJ++)
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
		for (size_t nJ = 0; nJ < aCurLine.size(); nJ++)
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

void RemoveSameIDFromAInB(vector<tsOtherBox> &A, vector<tsOtherBox> &B)
{
	size_t nS1 = A.size();
	size_t nS2 = B.size();
	if ((nS1 == 0) || (nS2 == 0))
		return;
	vector<tsOtherBox> atsC;
	size_t nPosA = 0;
	for (size_t nI = 0; nI < nS2; nI++)
	{
		bool bChecked = false;
		for (size_t nJ = nPosA; nJ < nS1; nJ++)
		{
			if (B[nI].nID == A[nJ].nID)
			{
				bChecked = true;
				nPosA = nJ;		// all list are sorted in ascending order of ID
				break;
			}
		}
		if (bChecked == false)
		{
			atsC.push_back(B[nI]);
		}
	}
	B.clear();
	B = atsC;
	atsC.clear();
}

void RemoveSameIDFromAInB(vector<tsLineBox> &A, vector<tsLineBox> &B)
{
	size_t nS1 = A.size();
	size_t nS2 = B.size();
	if ((nS1 == 0) || (nS2 == 0))
		return;
	vector<tsLineBox> atsC;
	size_t nPosA = 0;
	for (size_t nI = 0; nI < nS2; nI++)
	{
		bool bChecked = false;
		for (size_t nJ = nPosA; nJ < nS1; nJ++)
		{
			if (B[nI].tsCore.nID == A[nJ].tsCore.nID)
			{
				bChecked = true;
				nPosA = nJ;		// all list are sorted in ascending order of ID
				break;
			}
		}
		if (bChecked == false)
		{
			atsC.push_back(B[nI]);
		}
	}
	B.clear();
	B = atsC;
	atsC.clear();
}

void RemoveSameIDFromAInB(tsLineBox A, vector<tsLineBox> &B)
{
	size_t nS2 = B.size();
	if (nS2 == 0)
		return;
	vector<tsLineBox> atsC;
	size_t nPosA = 0;
	for (size_t nI = 0; nI < nS2; nI++)
	{
		bool bChecked = false;
		if (B[nI].tsCore.nID == A.tsCore.nID)
		{
			bChecked = true;
			nPosA = nJ;		// all list are sorted in ascending order of ID
			break;
		}
		if (bChecked == false)
		{
			atsC.push_back(B[nI]);
		}
	}
	B.clear();
	B = atsC;
	atsC.clear();
}

void RemoveOtherBoxesMergeInLines(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	size_t nSize = atsLines.size();
	vector<int> aRemoveIndexes;
	// get remove indexes list
	for (size_t nI = 0; nI < nSize; nI++)
	{
		for (size_t nJ = 0; nJ < atsLines[nI].anSubID.size(); nJ++)
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
		if (atsOtherBoxes[nI].nID < aRemoveIndexes[nPos])
		{
			atsTmp.push_back(atsOtherBoxes[nI]);
			continue;
		}
		if (atsOtherBoxes[nI].nID == aRemoveIndexes[nPos])
		{
			nPos++;
			continue;
		}
		if (atsOtherBoxes[nI].nID > aRemoveIndexes[nPos])
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

void RemoveOtherBoxesInsideLines(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	int nS1 = atsOtherBoxes.size();
	int nS2 = atsLines.size();
	vector<tsOtherBox> atsTmp;
	for (int nI = 0; nI < nS1; nI++)
	{
		bool bChecked = false;
		for (int nJ = 0; nJ < nS2; nJ++)
		{
			// check condition
			bChecked = IsOtherBoxAInsideLineBoxB(atsOtherBoxes[nI], atsLines[nJ]);
			if (bChecked == true)
				break;
			// check at least 85% area
			bChecked = IsOtherBoxAMostlyInsideLineBoxB(atsOtherBoxes[nI], atsLines[nJ]);
			if (bChecked == true)
				break;
		}
		// add to atsTmp if satisfy the condition
		if (bChecked == false)
		{
			atsTmp.push_back(atsOtherBoxes[nI]);
		}
	}
	atsOtherBoxes.clear();
	atsOtherBoxes = atsTmp;
	atsTmp.clear();
}

void MergeLineBoxesIntoALine(vector<tsLineBox> &atsNeedMerge, vector<tsLineBox> &atsLines)
{
	if (atsLines.size() == 0)
		return;
	int nNextID = atsLines[atsLines.size() - 1].tsCore.nID + 1;
	tsLineBox tsNewLine;
	size_t nS1 = atsNeedMerge.size();
	// add list sub ID
	for (size_t nI = 0; nI < nS1; nI++)
	{
		size_t nS2 = atsNeedMerge[nI].anSubID.size();
		for (size_t nJ = 0; nJ < nS2; nJ++)
		{
			tsNewLine.anSubID.push_back(atsNeedMerge[nI].anSubID[nJ]);
		}
	}
	// add list sub Rect
	for (size_t nI = 0; nI < nS1; nI++)
	{
		size_t nS2 = atsNeedMerge[nI].atsSubROI.size();
		for (size_t nJ = 0; nJ < nS2; nJ++)
		{
			tsNewLine.atsSubROI.push_back(atsNeedMerge[nI].atsSubROI[nJ]);
		}
	}
	// add to tsCore information
	tsNewLine.tsCore.nID = nNextID;
	tsNewLine.tsCore.InputROIByCreateCoverRect(tsNewLine.atsSubROI);
	tsLineTmp.tsCore.rACVROI = tsRect(0, 0, 0, 0);
	tsNewLine.tsCore.strNameImage = atsNeedMerge[0].tsCore.strNameImage;
	tsNewLine.tsCore.nNumberVersion = 1;
	tsNewLine.tsCore.fTimeRunning = 0.0;
	// add to atsLines
	atsLines.push_back(tsNewLine);
	
	// remove merge lines
	vector<tsLineBox> atsTmp;
	nS1 = atsLines.size();
	size_t nS3 = atsNeedMerge.size();
	for (size_t nI = 0; nI < nS1; nI++)
	{
		bool bChecked = false;
		for (size_t nJ = 0; nJ < nS3; nJ++)
		{
			if (atsLines[nI].tsCore.nID == atsNeedMerge[nJ].tsCore.nID)
			{
				bChecked = true;
				break;
			}
		}
		if(bChecked == false)
		{
			atsTmp.push_back(atsLines[nI]);
		}
	}
	atsLines.clear();
	atsLines = atsTmp;
}

void ReDistributeIDForLines(vector<tsLineBox> &atsLines)
{
	if(atsLines.size() == 0)
		return;
	int nStartID = atsLines[0].tsCore.nID;
	size_t nSize = atsLines.size();
	for (size_t nI = 0; nI < nSize; nI++)
	{
		atsLines[nI].tsCore.nID = nStartID + nI;
	}
	return;
}

// not finish
void MergeLineAndOtherBoxes(vector<tsOtherBox> &atsNeedMerge, vector<tsLineBox> &atsLines, int nIndex)
{
	
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

void SortXCoordinate(vector<tsLineBox> &atsLines)
{
	sort(atsLines.begin(), atsLines.end(), CompareXCoordinateLineBox);
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

bool IsOtherBoxAIntersectOtherBoxB(tsOtherBox tsA, tsOtherBox tsB)
{
	// check area
	Rect B = Rect(tsA.rROI.nX, tsA.rROI.nY, tsA.rROI.nWidth, tsA.rROI.nHeight);
	Rect A = Rect(tsB.rROI.nX, tsB.rROI.nY, tsB.rROI.nWidth, tsB.rROI.nHeight);
	Rect C = A & B;
	if (C.area() > 0)
		return true;
	if (tsA.rROI.nX < tsB.rROI.nX)
	{
		if ((tsA.rROI.nX + tsA.rROI.nWidth + 3) > tsB.rROI.nX)
		{
			// between the upper 50% height and the height of B range = 200% -> 0% of height of A
			if ((tsA.rROI.nY < tsB.rROI.nY) && ((tsA.rROI.nY + 0.5* tsA.rROI.nHeight) > tsB.rROI.nY)
				&& ((tsA.rROI.nHeight / tsB.rROI.nHeight) < 2) && ((tsB.rROI.nHeight / tsA.rROI.nHeight) < 2) )
			{
				return true;
			}
			else if ((tsA.rROI.nY > tsB.rROI.nY) && ((tsA.rROI.nY + tsA.rROI.nHeight) > (tsB.rROI.nY + tsB.rROI.nHeight))
				&& ((tsB.rROI.nY + tsB.rROI.nHeight) > tsA.rROI.nY) && ((tsA.rROI.nHeight / tsB.rROI.nHeight) < 2)
				&& ((tsB.rROI.nHeight / tsA.rROI.nHeight) < 2))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		if ((tsB.rROI.nX + tsB.rROI.nWidth + 3) > tsA.rROI.nX)
		{
			if ((tsB.rROI.nY < tsA.rROI.nY) && ((tsB.rROI.nY + 0.5* tsB.rROI.nHeight) > tsA.rROI.nY)
				&& ((tsB.rROI.nHeight / tsA.rROI.nHeight) < 2) && ((tsA.rROI.nHeight / tsB.rROI.nHeight) < 2)
				)
			{
				return true;
			}
			else if ((tsB.rROI.nY > tsA.rROI.nY) && ((tsB.rROI.nY + tsB.rROI.nHeight) > (tsA.rROI.nY + tsA.rROI.nHeight))
				&& ((tsA.rROI.nY + tsA.rROI.nHeight) > tsB.rROI.nY) && ((tsB.rROI.nHeight / tsA.rROI.nHeight) < 2)
				&& ((tsA.rROI.nHeight / tsB.rROI.nHeight) < 2))
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

bool IsOtherBoxAInsideLineBoxB(tsOtherBox tsA, tsLineBox tsB)
{
	// check basic condition for x, y of tsA to x, y of tsB
	if (tsA.rROI.nX >= tsB.tsCore.rROI.nX && tsA.rROI.nY >= tsB.tsCore.rROI.nY)
	{
		if ((tsA.rROI.nX + tsA.rROI.nWidth) <= (tsB.tsCore.rROI.nX + tsB.tsCore.rROI.nWidth)
			&& (tsA.rROI.nY + tsA.rROI.nHeight) <= (tsB.tsCore.rROI.nY + tsB.tsCore.rROI.nHeight))
		{
			return true;
		}
	}
	return false;
}

bool IsOtherBoxAMostlyInsideLineBoxB(tsOtherBox tsA, tsLineBox tsB)
{
	// check basic condition for x, y of tsA to x, y of tsB
	if ((tsA.rROI.nX >= tsB.tsCore.rROI.nX) && (tsA.rROI.nY >= tsB.tsCore.rROI.nY)
		&& (tsA.rROI.nX < (tsB.tsCore.rROI.nX + tsB.tsCore.rROI.nWidth))
		&& (tsA.rROI.nY < (tsB.tsCore.rROI.nY + tsB.tsCore.rROI.nHeight)) )
	{
		// check area inside
		Rect B = Rect(tsA.rROI.nX, tsA.rROI.nY, tsA.rROI.nWidth, tsA.rROI.nHeight);
		Rect C = Rect(tsB.tsCore.rROI.nX, tsB.tsCore.rROI.nY, tsB.tsCore.rROI.nWidth, tsB.tsCore.rROI.nHeight);
		Rect A = B & C;
		if ((float)A.area() > (0.75 * ((float)(tsA.rROI.GetArea()))))
		{
			return true;
		}
	}
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

bool IsIntersectHorizontally(tsLineBox tsA, tsLineBox tsB)
{
	// check if 2 tsRect intersect horizontally
	return tsA.tsCore.rROI.IsTwoRectIntersectHorizontally(tsB.tsCore.rROI);
}

bool IsIntersectHorizontally(tsLineBox tsA, tsOtherBox tsB)
{
	// check if 2 tsRect intersect horizontally
	return tsA.tsCore.rROI.IsTwoRectIntersectHorizontally(tsB.rROI);
}

bool CheckConditionOfMergIntersectBoxes(tsOtherBox A, tsOtherBox B)
{
	// check y position, cannot be too different
	if (((float)A.rROI.nY + (float)A.rROI.nHeight / 2) > B.rROI.nY && ((float)A.rROI.nY - (float)A.rROI.nHeight / 2) < B.rROI.nY)
	{
		// check intersect or seperate in acceptable limit - 3 pixels
		if (((float)B.rROI.nX - ((float)A.rROI.nX + (float)A.rROI.nWidth)) < 4)
		{
			// check height ratio
			if (((float)A.rROI.nHeight / (float)B.rROI.nHeight) > 0.8 && ((float)B.rROI.nHeight / (float)A.rROI.nHeight) < 1.25)
			{
				// check width ratio
				/*if (((float)A.rROI.nWidth / (float)B.rROI.nWidth) > 0.7 && ((float)B.rROI.nWidth / (float)A.rROI.nWidth) < 1.43)
				{
					
					return true;
				}*/

				return true;
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

bool CompareXCoordinateLineBox(tsLineBox B1, tsLineBox B2)
{
	return (B1.tsCore.rROI.nX < B2.tsCore.rROI.nX);
}

tsLineBox MergeOtherBoxesIntoALine(vector<tsOtherBox> &atsTmp, int nID)
{
	tsLineBox tsALine;
	tsALine.tsCore.nID = nID;
	size_t nSize = atsTmp.size();
	if (nSize = 0)
		return tsALine;
	for (size_t nI = 0; nI < nSize; nI++)
	{
		tsALine.anSubID.push_back(atsTmp[nI].nID);
		tsALine.atsSubROI.push_back(atsTmp[nI].rROI);
	}
	tsALine.tsCore.nNumberVersion = 1;
	tsALine.tsCore.strNameImage = atsTmp[0].strNameImage;
	tsALine.tsCore.InputROIByCreateCoverRect(tsALine.atsSubROI);
	return tsALine;
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

	// Remove OtherBoxes which are completely (or at least 75% area) inside Line Boxes
	RemoveOtherBoxesInsideLines(atsOtherBoxes, atsLines);
	// clear
	aFreeList.clear();
	// increase each rect: left, right, top, bottom value + 1 pixel if all of them < 10, else + 2 pixels
	// it make the OCR recognize text easier
	IncreaseRectToBoxes(atsOtherBoxes, atsLines);
	/*		Re-check 2nd time		*/
	// merge > 3 otherboxes intersect
	MergeALotOtherBoxesIntersect(atsOtherBoxes, atsLines);
	
	// merge intersect lines and otherbox
	MergeLinesAndOtherBoxesHorizontally(atsOtherBoxes, atsLines);

	// merge lines intersect
	MergeLineIntersectHorizontally(atsLines);
	
	// sort atsLines by x coordinate
	SortXCoordinate(atsLines);
	// re-distribute nID for all atsLines
	ReDistributeIDForLines(atsLines);
}

void MergeALotOtherBoxesIntersect(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	vector<size_t> aIndexIntersect;
	size_t nSize = 0;
	size_t nPos = 0;
	int nNextID = 0;
	if (atsOtherBoxes.size() == 0)
		return;
	// get first nNextID
	if (atsLines.size() != 0)
	{
		nNextID = atsLines[atsLines.size() - 1].tsCore.nID + 1;
	}
	else
	{
		nNextID = atsOtherBoxes[atsOtherBoxes.size() - 1].nID + 1;
	}
	do
	{
		// prepare next ID
		nNextID++;
		// loop
		nSize = atsOtherBoxes.size();
		for (size_t nI = nPos + 1; nI < nSize; nI++)
		{
			// check with the rest of this list
			if (IsOtherBoxAIntersectOtherBoxB(atsOtherBoxes[nPos], atsOtherBoxes[nI]) == true)
			{
				aIndexIntersect.push_back(nI);
			}
		}
		// check if > 3 otherboxes
		if (aIndexIntersect.size() > 3)
		{
			// take them out as a seperate array
			vector<tsOtherBox> atsTmp;
			nSize = aIndexIntersect.size();
			for (size_t nI = 0; nI < nSize; nI++)
			{
				atsTmp.push_back(atsOtherBoxes[aIndexIntersect[nI]]);
			}
			// merge them into an Line box
			atsLines.push_back(MergeOtherBoxesIntoALine(atsTmp, nNextID));
			// remove used index in atsOtherBoxes
			RemoveSameIDFromAInB(atsTmp, atsOtherBoxes);
			// clear atsTmp
			atsTmp.clear();
			nPos--;
		}
		aIndexIntersect.clear();
		nPos++;
	} while (nPos < nSize);
	
}

void MergeLinesAndOtherBoxesHorizontally(vector<tsOtherBox> &atsOtherBoxes, vector<tsLineBox> &atsLines)
{
	if (atsLines.size() == 0)
		return;
	size_t nS1 = atsOtherBoxes.size();
	size_t nS2 = atsLines.size();
	// use this loop outside because each line is searched 1 time only
	for (size_t nI = 0; nI < nS2; nI++)
	{
		vector<tsOtherBox> atsNeedMerge;
		for (size_t nJ = 0; nJ < nS1; nJ++)
		{
			// check if they are intersect horizontally
			if (IsIntersectHorizontally(atsLines[nI], atsOtherBoxes[nJ]) == true)
			{
				atsNeedMerge.push_back(atsOtherBoxes[nJ]);
			}
		}
		// check condition of atsNeedMerge
		if(atsNeedMerge.size() > 0)
		{
			// merge into a new line
			MergeLineAndOtherBoxes(atsNeedMerge, atsLines, nI);
			// remove existing tsOtherBoxes in atsOtherBoxes
			RemoveSameIDFromAInB(atsNeedMerge, atsOtherBoxes);
			// remove existing the line in atsLines
			RemoveSameIDFromAInB(atsLines[nI], atsLines);
			// reduce number position
			nI--;
			// update size
			nS2 = atsLines.size();
			nS1 = atsOtherBoxes.size();
			// clean
			atsNeedMerge.clear();
		}
	}


}

void MergeLineIntersectHorizontally(vector<tsLineBox> &atsLines)
{
	size_t nSize = atsLines.size();
	if (nSize == 0)
		return;
	size_t nPos = 0;
	vector<tsLineBox> atsTmp;
	do
	{
		if ((nPos + 1) == nSize)
			break;
		atsTmp.push_back(atsLines[nPos]);
		for (size_t nI = nPos + 1; nI < nSize; nI++)
		{
			if (IsIntersectHorizontally(atsLines[nPos], atsLines[nI]) == true)
			{
				atsTmp.push_back(atsLines[nI]);
			}
		}
		if (atsTmp.size() > 1)
		{
			// merge and create new atsLines
			MergeLineBoxesIntoALine(atsTmp, atsLines);
			// keep position
			nPos--;
		}
		atsTmp.clear();
		nPos++;
	} while (nPos < nSize);
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

void ReadLinesDataFile(string strLinePath, vector<tsLineBox> &atsLines)
{
	BBoxIOStream bboxTmp;
	// get list info
	bboxTmp.ReadLines(atsLines, strLinePath);
}

void WriteOtherBoxesToFile(string strOtherBoxPath, vector<tsOtherBox> &atsOtherBoxes)
{
	BBoxIOStream bboxTmp;
	bboxTmp.WriteOtherBoxes(atsOtherBoxes, strOtherBoxPath);
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


/*		The test rect:
102 4 27 26;
127 6 23 27;
141 9 10 25;
148 10 24 25;
170 11 23 26;
172 21 10 16;
*/
void TestSimulateOtherBoxesC1(vector<tsOtherBox> &atsOtherBoxes)
{
	atsOtherBoxes.push_back(tsOtherBox(0, 102, 4, 27, 26, "img250", 1, 0));
	atsOtherBoxes.push_back(tsOtherBox(1, 127, 6, 23, 27, "img250", 1, 0));
	atsOtherBoxes.push_back(tsOtherBox(2, 141, 9, 10, 25, "img250", 1, 0));
	atsOtherBoxes.push_back(tsOtherBox(3, 148, 10, 24, 25, "img250", 1, 0));
	atsOtherBoxes.push_back(tsOtherBox(4, 170, 11, 23, 26, "img250", 1, 0));
	atsOtherBoxes.push_back(tsOtherBox(5, 172, 21, 10, 16, "img250", 1, 0));
}

// Test get a line from intersect box
void TestGetLineIntersect()
{
	string strOtherBoxPath = "E:\\Code\\OCR-Five-Git\\Root\\TmpRect\\img250-OtherBoxes.txt";
	vector<tsOtherBox> atsOtherBoxes;
	vector<tsLineBox> atsLines;
	string strImagePath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250.jpg";
	string strTestOBPath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250-OtherBoxes.txt";
	string strTestLinePath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250-Lines.txt";
	
	ReadOtherBoxesDataFile(strOtherBoxPath, atsOtherBoxes);
	//TestSimulateOtherBoxesC1(atsOtherBoxes);

	CutDownOtherBoxesByX(atsOtherBoxes, 100, 0);
	CutDownOtherBoxesByY(atsOtherBoxes, 0, 70);

	MergeLineBox(atsOtherBoxes, atsLines);

	AddRectToOriginalImage(strImagePath, atsOtherBoxes, atsLines);
	WriteDataToTxtFile(strTestOBPath, strTestLinePath, atsOtherBoxes, atsLines);
}

void TestGetLineIntersectFromImage()
{
	vector<Rect> arBBoxes;
	string strOtherBoxPath = "E:\\Code\\OCR-Five-Git\\Root\\TmpRect\\img250-OtherBoxes.txt";
	vector<tsOtherBox> atsOtherBoxes;
	vector<tsLineBox> atsLines;
	string strImagePath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250.jpg";
	string strTestOBPath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250-OtherBoxes.txt";
	string strTestLinePath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250-Lines.txt";

	// read image and output OtherBoxes
	Mat mOriGS;					// original gray scale image
	Mat mOriSharpGS;			// original sharpening grayscale image

								// read image in gray scale
	ReadImageGrayScale(strImagePath, mOriGS);
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
	ConvertFromBBoxesToOtherBoxes(GetFileNameFromPath(strImagePath), arBBoxes, atsOtherBoxes);
	arBBoxes.clear();

	CutDownOtherBoxesByX(atsOtherBoxes, 100, 0);
	CutDownOtherBoxesByY(atsOtherBoxes, 0, 70);

	MergeLineBox(atsOtherBoxes, atsLines);

	// increase each rect: left, right, top, bottom value + 1 pixel if all of them < 10, else + 2 pixels
	// it make the OCR recognize text easier
	//IncreaseRectToBoxes(atsOtherBoxes, atsLines);

	AddRectToOriginalImage(strImagePath, atsOtherBoxes, atsLines);
	WriteDataToTxtFile(strTestOBPath, strTestLinePath, atsOtherBoxes, atsLines);
}

void TestShowLineBoxOnImage()
{
	vector<tsLineBox> atsLines;
	vector<tsOtherBox> atsOtherBoxes;
	string strInImPath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250.jpg";
	string strOutImPath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250-outputline.jpg";
	string strTestLinePath = "E:\\Code\\OCR-Five-Git\\Root\\Test\\img250-OtherBoxes.txt";
	int nID = 58;
	
	//ReadLinesDataFile(strTestLinePath, atsLines);
	ReadOtherBoxesDataFile(strTestLinePath, atsOtherBoxes);

	// get need Line
	vector<tsOtherBox> atsTmp;
	size_t nSize = atsOtherBoxes.size();
	for (size_t nI = 0; nI < nSize; nI++)
	{
		if (atsOtherBoxes[nI].nID == nID)
		{
			atsTmp.push_back(atsOtherBoxes[nI]);
			break;
		}
	}
	
	AddRectToOriginalImage(strInImPath, strOutImPath, atsTmp, atsLines);
	atsTmp.clear();
	atsLines.clear();
}

void CutDownOtherBoxesByX(vector<tsOtherBox> &atsOtherBoxes, int start, int max)
{
	vector<tsOtherBox> atsTmp;
	int nSize = atsOtherBoxes.size();
	for (int nI = 0; nI < nSize; nI++)
	{
		if (atsOtherBoxes[nI].rROI.nX > start)
		{
			if (max == 0)
			{
				atsTmp.push_back(atsOtherBoxes[nI]);
			}
			else
			{
				if (atsOtherBoxes[nI].rROI.nX < max)
				{
					atsTmp.push_back(atsOtherBoxes[nI]);
				}
			}
		}
	}
	atsOtherBoxes.clear();
	atsOtherBoxes = atsTmp;
	atsTmp.clear();
}

void CutDownOtherBoxesByY(vector<tsOtherBox> &atsOtherBoxes, int start, int max)
{
	vector<tsOtherBox> atsTmp;
	int nSize = atsOtherBoxes.size();
	for (int nI = 0; nI < nSize; nI++)
	{
		if (atsOtherBoxes[nI].rROI.nY > start)
		{
			if (max == 0)
			{
				atsTmp.push_back(atsOtherBoxes[nI]);
			}
			else
			{
				if (atsOtherBoxes[nI].rROI.nY < max)
				{
					atsTmp.push_back(atsOtherBoxes[nI]);
				}
			}
		}
	}
	atsOtherBoxes.clear();
	atsOtherBoxes = atsTmp;
	atsTmp.clear();
}

//----------------------------------------------------------------------