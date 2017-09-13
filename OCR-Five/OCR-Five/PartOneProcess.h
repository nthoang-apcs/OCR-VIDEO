#pragma once
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "MSERIO.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "CommonStructAndFunction.h"
using namespace std;
using namespace cv;


class PartOneProcess
{
private:
	
	void WriteOneImageToFile(string PathIn, string resultFolder, vector<Rect> &BBoxes, double &TimeRunning, int index);

	void WriteOneImageToFile(string PathIn, string resultFolder, vector<vector<Rect>> &LinesText, double &TimeRunning, int index);

	

	/*		Practice method		*/

	void ConvertandResizeOneFile(string pathIn, string pathOut);

	void ConvertandResizeMultipleFiles(vector<string> fileList, string resultFolder);

	void MSERCropboxesOneImage(string pathIn, string cropboxesFolder);

	void MSERCropboxesImages(string fileListPath, string cropboxesFolder);


	// result is in LinesText, BBoxes is the same as before input
	void PostMserProcessing(Mat& input, vector<Rect> &BBoxes, double &TimeRunning);

public:

	/*		Constructor		*/
	PartOneProcess();

	/*		Process method		*/

	void doProcessOneImageWithPostProcessing(string PathIn, string resultFolder);

	

	

	/*		Destructor		*/
	~PartOneProcess();
};
