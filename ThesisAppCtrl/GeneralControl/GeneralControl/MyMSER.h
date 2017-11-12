#pragma once
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

using namespace std;
using namespace cv;


void MSEROneImage(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning);

void SharpenOneImage(Mat &input, Mat &output, double sigma = 1, double threshold = 5, double amount = 1);

void RemoveDuplicatedBoxes(vector<Rect> &BBoxes);

void AddRectToMat(vector<Rect> &BBoxes, Mat &input, Mat &output);