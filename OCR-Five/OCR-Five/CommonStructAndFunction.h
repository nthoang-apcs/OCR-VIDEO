#pragma once
#include <opencv2\imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\features2d.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
using namespace cv;


struct SWTPoint2d {
	int x;
	int y;
	float SWT;
};

typedef std::pair<SWTPoint2d, SWTPoint2d> SWTPointPair2d;
typedef std::pair<cv::Point, cv::Point>   SWTPointPair2i;

struct Ray {
	SWTPoint2d p;
	SWTPoint2d q;
	std::vector<SWTPoint2d> points;
};

struct Chain {
	int p;
	int q;
	float dist;
	bool merged;
	Point2f direction;
	std::vector<int> components;
};

// ax + by + c = 0
struct Line {
	float a;
	float b;
	float c;
	Line()
	{
		a = 0;
		b = 0;
		c = 0;
	}
	Line(float a1, float b1, float c1)
	{
		a = a1;
		b = b1;
		c = c1;
	}
};


/*		Main functions		*/


void SharpenOneImage(Mat &input, Mat &output, double sigma = 1, double threshold = 5, double amount = 1);

void MSEROneImage(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning);

void PostProcessing(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning);

// file name format: [namedefault]-[box-index].jpg
void CropBoxesInOneImage(Mat &input, vector<Rect> &BBoxes, double &TimeRunning, string resultFolder, string namedefault);



/*		Statistic functions		*/


void GetListName(vector<string> &Paths, string resultFolder, string filename);

void GetListName(vector<string> &Paths, vector<string> &ListName);

void GetListTotalBoxes(vector<string> &Paths, string resultFolder, string filename);

void GetListTotalBoxes(vector<string> &Paths, string resultFolder, vector<int> &ListBoxes, int index);

void GetListRunTime(vector<string> &Paths, string resultFolder, string filename);

void GetListRunTime(vector<string> &Paths, string resultFolder, vector<double> &runTime, int index);

// line format: img[number].jpg
// number range: [start, end)
void SetListName(int start, int end, string filepath);

// line format: [fileformat]img[number].jpg
// number range: [start, end)
void SetListPath(int start, int end, string filepath, string fileformat);

void GetListBoxesInOneImage(vector<Rect> &BBoxes, string filepath);

void GetListCentroids(vector<Rect> &BBoxes, vector<Point> &Centroids);


/*		Support functions		*/


void RemoveDuplicatedBoxes(vector<Rect> &BBoxes);

void RemoveUnusualAreaBoxes(vector<Rect> &BBoxes);

// from center of Rect, text 2 lines, 1 upper 25%, 1 lower 25%
// to check if that space created by 2 lines cuts any other spaces
// if cut -> on same line
void RemoveSingleBoxTextLine(vector<Rect> &BBoxes);

// h / w or w / h > 6 -> remove
void RemoveUnbalancedRatio(vector<Rect> &BBoxes);

void MergeInsideBoxes(vector<Rect> &BBoxes);

// multiple boxes on the same line text and overlap to each other -> comebine into 1 box and check the line text
// have a high chance to become from 1 char / 1 box -> > 1 word / 1 box
void MergeOverlapOnTextLineNearRatioBoxes(vector<Rect> &BBoxes);

// not finish
void CheckStrokeWidthVariation(vector<Rect> &BBoxes);

void AddRectToMat(vector<Rect> &BBoxes, Mat &input, Mat &output);

void AddListPath(vector<string> &Paths, string filepath);

void writeBBoxesToFile(vector<Rect> &BBoxes, string filename);

void writeTotalBoxesToFile(vector<int> &TotalBoxes, string filename);

void writeRunTimeToFile(vector<double> &RunTime, string filename);

Line GetLineEquation(Point A, Point B);

Line GetLineEquation(vector<int> &ListPos, vector<Rect> &BBoxes);


/*		Sort functions		*/


void SortYCoordinate(vector<Rect> &BBoxes);

void SortArea(vector<Rect> &BBoxes);


/*		Check functions		*/

// check if B1 is inside B2 and B1.area * 3 >= B2
bool IsB1insideB2(Rect B1, Rect B2);

// from center of Rect, text 2 lines, 1 upper 25%, 1 lower 25% 
// to check if that space created by 2 lines cuts any other spaces
// if cut -> on same line
bool IsB1onsamelineB2(Rect B1, Rect B2);

// B1 is unbalanced if h / w > 6 || w / h > 6
bool IsB1Balanced(Rect B1);

bool IsB1OverlappingB2(Rect B1, Rect B2);

// return true if B1.y < B2.y
bool CompareYCoordinate(Rect B1, Rect B2);

// return true if B1.area() < B2.area()
bool CompareArea(Rect B1, Rect B2);

// 2 boxes are seperated, check condition about ratio, inside or outside
bool CheckConditionOfBoxLine(Rect B1, Rect B2);

// check if point B on Line A or stay outside but have the angle <= threshold
bool CheckLineEquation(Line A, Point B, int threshold);

bool Point2dSort(SWTPoint2d const & lhs, SWTPoint2d const & rhs);

bool chainSortDist(const Chain &lhs, const Chain &rhs);

bool chainSortLength(const Chain &lhs, const Chain &rhs);


/*		Others		*/

string ExtractNameOfFileFromPathIn(string PathIn);

int ExtractTotalBoxesFromString(string line);

double ExtractTimeRunningFromString(string line);
