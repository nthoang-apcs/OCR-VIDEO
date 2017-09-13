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




void SharpenOneImage(Mat &input, Mat &output, double sigma = 1, double threshold = 5, double amount = 1);

void MSEROneImage(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning);

void RemoveSingleBoxes(vector<Rect> &BBoxes);

void MergeInsideBoxes(vector<Rect> &BBoxes);


bool IsB1insideB2(Rect B1, Rect B2);


string ExtractNameOfFileFromPathIn(string PathIn);