#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <math.h>
#include <time.h>
#include <utility>
#include <algorithm>
#include <vector>
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

bool Point2dSort(SWTPoint2d const & lhs,
	SWTPoint2d const & rhs);

cv::Mat textDetection(const cv::Mat& input, bool dark_on_light);

void strokeWidthTransform(const cv::Mat& edgeImage,
	cv::Mat& gradientX,
	cv::Mat& gradientY,
	bool dark_on_light,
	cv::Mat& SWTImage,
	std::vector<Ray> & rays);

void SWTMedianFilter(cv::Mat& SWTImage, std::vector<Ray> & rays);

void filterComponents(cv::Mat& SWTImage,
	std::vector<std::vector<SWTPoint2d> > & components,
	std::vector<std::vector<SWTPoint2d> > & validComponents,
	std::vector<Point2f> & compCenters,
	std::vector<float> & compMedians,
	std::vector<SWTPoint2d> & compDimensions,
	std::vector<SWTPointPair2d > & compBB);

std::vector<Chain> makeChains(const cv::Mat& colorImage,
	std::vector<std::vector<SWTPoint2d> > & components,
	std::vector<Point2f> & compCenters,
	std::vector<float> & compMedians,
	std::vector<SWTPoint2d> & compDimensions,
	std::vector<SWTPointPair2d > & compBB);

