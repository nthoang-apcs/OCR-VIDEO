#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp>
#include "MyMSER.h"
#include "MySupPro.h"
#include "RectDList.h"
#include "MySort.h"

using namespace std;
using namespace cv;

// return 0 - failed
// return 1 - success
int RunProcessAll(vector<Mat> &mOriginImage, vector<Mat> &mOutputImage, char *CurrentFolder);

// return 0 - failed
// return 1 - success
// processing of 1 image from preprocessing to postprocessing
int RunProcessOne(Mat &mOriginImage, Mat &mOutputImage, char *CurrentFolder);


// Pre processing
void PreProcessing(Mat mOriginImage, vector<Rect> &BBoxes, double &TimeRunning);


// Post processing step 1: reduce non-text BBoxes
void PostProcessing(vector<Rect> &BBoxes, double &TimeRunning);

// Post processing step 2: combine text line BBoxes, bind BBoxes to IDs
void PostProcessingStepTwo(vector<Rect> &BBoxes, double &TimeRunning, RectDList &OtherBoxes, vector<RectDList> &Lines);
