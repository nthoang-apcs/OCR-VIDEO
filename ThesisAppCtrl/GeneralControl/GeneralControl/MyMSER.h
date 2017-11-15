#pragma once
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include "MySupPro.h"

using namespace std;
using namespace cv;


void MSEROneImage(Mat &input, vector<Rect> &BBoxes, double &TimeRunning);

void RemoveDuplicatedBoxes(vector<Rect> &BBoxes);
