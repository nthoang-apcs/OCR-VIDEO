#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp>

using namespace std;
using namespace cv;



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
