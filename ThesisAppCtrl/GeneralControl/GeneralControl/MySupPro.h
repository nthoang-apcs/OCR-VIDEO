#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp>
#include "RectDLList.h"

using namespace std;
using namespace cv;

///////////////////////////////////////
/*
		Support Process
*/
//////////////////////////////////////
void SharpenOneImage(Mat &input, Mat &output, double sigma = 1, double threshold = 5, double amount = 1);

void AddRectToMat(vector<Rect> &BBoxes, Mat &input, Mat &output);

void RemoveUnusualAreaBoxes(vector<Rect> &BBoxes);

// from center of Rect, text 2 lines, 1 upper 25%, 1 lower 25%
// to check if that space created by 2 lines cuts any other spaces
// if cut -> on same line
void RemoveSingleBoxTextLine(vector<Rect> &BBoxes);

// h / w or w / h > 6 -> remove
void RemoveUnbalancedRatio(vector<Rect> &BBoxes);

// merge inside boxes
// remove too big size
void MergeInsideBoxes(vector<Rect> &BBoxes);

void BindingID(vector<Rect> &BBoxes, RectDLL &OtherBoxes, int start);

void MergeLineText(RectDLL &OtherBoxes, vector<RectDLL> Lines);

// from first index, draw a horizontal, computer the angle of second index to first index with that horizontal
int ComputeHorizontalAngle(RectDLL &OtherBoxes, int firstIndex, int secondIndex);



///////////////////////////////////////
/*
			Condition
*/
//////////////////////////////////////

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




///////////////////////////////////////
/*
				Sort
*/
//////////////////////////////////////

void SortYCoordinate(vector<Rect> &BBoxes);

void SortArea(vector<Rect> &BBoxes);

void SortXCoordinate(vector<Rect> &BBoxes);


///////////////////////////////////////
/*
			Handle files
*/
//////////////////////////////////////

Rect ConvertTheoryRectToRealRect(Rect Input);

Rect ConvertRealRectToTheoryRect(Rect Input);

void SaveLines(Mat &mOriginImage, char *CurrentFolder, vector<RectDLL> &Lines);

void SaveOtherBoxes(Mat &mOriginImage, char *CurrentFolder, RectDLL &OtherBoxes);



///////////////////////////////////////
/*
				MSER
*/
//////////////////////////////////////

void MSEROneImage(Mat &input, vector<Rect> &BBoxes, double &TimeRunning);

void RemoveDuplicatedBoxes(vector<Rect> &BBoxes);
