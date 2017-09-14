#pragma once
#include <opencv2\imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\features2d.hpp>
#include <vector>
#include <string>
#include <iostream>
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


void SharpenOneImage(Mat &input, Mat &output, double sigma = 1, double threshold = 5, double amount = 1);

void MSEROneImage(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning);

void RemoveSingleBoxes(vector<Rect> &BBoxes);

void MergeInsideBoxes(vector<Rect> &BBoxes);

void CheckStrokeWidthVariation(vector<Rect> &BBoxes);

void SortYCoordinate(vector<Rect> &BBoxes);


bool IsB1insideB2(Rect B1, Rect B2);

bool Point2dSort(SWTPoint2d const & lhs, SWTPoint2d const & rhs);

bool chainSortDist(const Chain &lhs, const Chain &rhs);

bool chainSortLength(const Chain &lhs, const Chain &rhs);


string ExtractNameOfFileFromPathIn(string PathIn);


