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


/*		Main functions		*/

void SharpenOneImage(Mat &input, Mat &output, double sigma = 1, double threshold = 5, double amount = 1);

void MSEROneImage(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning);

void PostProcessing(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning);


/*		Statistic functions		*/

void GetListName(vector<string> &Paths, string resultFolder, string filename);

void GetListTotalBoxes(vector<string> &Paths, string resultFolder, string filename);

void GetListRunTime(vector<string> &Paths, string resultFolder, string filename);

// line format: img[number].jpg
// number range: [start, end)
void SetListName(int start, int end, string filepath);

// line format: [fileformat]img[number].jpg
// number range: [start, end)
void SetListPath(int start, int end, string filepath, string fileformat);

void GetListBoxesInOneImage(vector<Rect> &BBoxes, string filepath);


/*		Support functions		*/

void RemoveUnusualAreaBoxes(vector<Rect> &BBoxes);

// from center of Rect, text 2 lines, 1 upper 25%, 1 lower 25% 
// to check if that space created by 2 lines cuts any other spaces
// if cut -> on same line
void RemoveSingleBoxTextLine(vector<Rect> &BBoxes);

void MergeInsideBoxes(vector<Rect> &BBoxes);

void CheckStrokeWidthVariation(vector<Rect> &BBoxes);

void AddRectToMat(vector<Rect> &BBoxes, Mat &input, Mat &output);

void AddListPath(vector<string> &Paths, string filepath);

// file name format: [namedefault]-[box-index].jpg
void CropBoxesInOneImage(Mat &input, vector<Rect> &BBoxes, string resultFolder, string namedefault);


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

bool Point2dSort(SWTPoint2d const & lhs, SWTPoint2d const & rhs);

bool chainSortDist(const Chain &lhs, const Chain &rhs);

bool chainSortLength(const Chain &lhs, const Chain &rhs);


/*		Others		*/

string ExtractNameOfFileFromPathIn(string PathIn);

int ExtractTotalBoxesFromString(string line);

double ExtractTimeRunningFromString(string line);
