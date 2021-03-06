#pragma once
#include <opencv2\imgproc.hpp>
#include "opencv2/highgui.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "CommonStructAndFunction.h"
using namespace std;
using namespace cv;

class MSERFILESTREAM
{
private:
	// variable for mser store


	// support ReadFromFile function
	string GetSrcFolder(string line);
	double GetAverageTime(string line);
	string GetSrcPath(string line);
	double GetTimeRunning(string line);
	int GetTotalBoxes(string line);
	vector<Rect> GetBoundingBoxes(string line);


public:
	MSERFILESTREAM();

	/*		Format:		*/
	//			head part
	// SRC-FOLDER: [Source-Folder]
	// AVERAGE-TIME-RUNNING: [time] seconds
	//			one index part
	// SRC-[index]: [PathSource]
	// TIME-RUNNING: [time] seconds
	// TOTAL-BBOXES: [number of boxes]
	// BOUNDING-BOXES: [vector of boxes]
	//			another index
	// ...
	// ...
	/*		Format of vector of boxes		*/
	// [x1 y1 width1 height1] ; [x2 y2 width2 height2] ; ... ; [xn yn widthn heightn] ;
	
	// write to path
	// srcFolder = Folder to get the resource
	void WriteImagesToFile(string path, string srcFolder, vector<string> &PathIn, vector<vector<Rect>> &BBoxes, vector<double> &TimeRunning);
	
	void WriteOneImageToFile(string pathIn, string srcPath, vector<Rect> &BBoxes, double &TimeRunning);

	/*		Format		*/
	// SRC: [PathSource]
	// TIME-RUNNING: [time] seconds
	// TOTAL-BBOXES: [number of boxes]
	// BBOXES-PATH:
	// ... (each line is a filename)
	void WriteOneImageCropBoxesToFile(string pathIn, string srcPath, int numberboxes, double &TimeRunning);
	
	void WriteOneImageToFile(string pathIn, string srcPath, vector<vector<Rect>> &LinesText, double &TimeRunning);

	// read from path
	void ReadImagesFromFile(string path, string &srcFolder, double &averagetime, vector<string> &PathIn, vector<vector<Rect>> &BBoxes, vector<double> &TimeRunning);

	void ReadOneImageFromFile(string pathIn, string &srcPath, vector<Rect> &BBoxes, double &TimeRunning);

	// not finish yet
	void ReadOneImageFromFile(string pathIn, string &srcPath, vector<vector<Rect>> &LinesText, double &TimeRunning);

	~MSERFILESTREAM();
};