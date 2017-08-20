#pragma once
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
using namespace cv;


class PartOneProcess
{
private:
	


	string ExtractNameOfFileFromPathIn(string PathIn);

public:
	PartOneProcess();

	void ConvertandResizeOneFile(string pathIn, string pathOut);

	void ConvertandResizeMultipleFiles(vector<string> fileList, string resultFolder);

	void SharpenOneImage(string pathIn, string pathOut);

	void MSEROneImage(string pathIn, string pathOut);

	void MSERCropboxesOneImage(string pathIn, string cropboxesFolder);

	void MSERCropboxesImages(string fileListPath, string cropboxesFolder);

	~PartOneProcess();
};


// ratio for line height and width:
/*

*/