#pragma once
/*
* ExperimentOne.h
*
* A demo source of MSER in Scene Text Detection:
* 
* 
* 
*
* Created on: Jul 21, 2017
* Author: Dang Tuan Vu and Nguyen Thanh Hoang
*/

#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;
using namespace cv::text;


class ExpOne
{
private:
	vector<string> PathIn;
	vector<string> PathOut;

	string ExtractNameOfFileFromPathIn(int index)
	{
		int leng = PathIn[index].length();
		int pos = leng - 1;
		do
		{
			pos--;
		} while (PathIn[index][pos] != '.' && pos > 0);
		// pos at char '.'
		int pos2 = pos - 1;
		do
		{
			pos2--;
		} while (PathIn[index][pos2] != '\\' && pos2 > 0);
		// pos2 at char '\'
		pos2++;
		// get name
		vector<char> tmp;
		for (int i = pos2; i < pos; i++)
		{
			tmp.push_back(PathIn[index][i]);
		}
		int tmplen = tmp.size();
		char *tmp2 = new char[tmplen + 1];
		for (int i = 0; i < tmplen; i++)
		{
			tmp2[i] = tmp[i];
		}
		tmp2[tmplen] = '\0';
		tmp.clear();
		return string(tmp2);
	}

public:

	// constructor
	ExpOne()
	{
		
	}
	ExpOne(vector<string> PathFiles)
	{
		PathIn = PathFiles;
	}
	// start running mser for all images in PathIn and save images to PathOut
	void RunAll(string pathfolder)
	{
		// create PathOut by: pathfolder + [name of file] + [-MSER] + extension(.jpg)
		int k = PathIn.size();
		for (int i = 0; i < k; i++)
		{
			string name = this->ExtractNameOfFileFromPathIn(i);
			PathOut.push_back(pathfolder + name + "-MSER" + ".jpg");
		}
		// run
		for (int i = 250; i < k; i++)
		{
			cout << "Run index " << i << endl;
			Run(PathIn[i], PathOut[i]);
		}
	}

	void Run(string pathIn, string pathOut)
	{
		
		Mat inImg = imread(pathIn);
		// convert gray from inImg to textImg
		Mat textImg;
		cvtColor(inImg, textImg, CV_BGR2GRAY);
		//Extract MSER
		vector< vector< Point> > contours;
		vector< Rect> bboxes;
		Ptr< MSER> mser = MSER::create(21, (int)(0.00002*textImg.cols*textImg.rows), (int)(0.05*textImg.cols*textImg.rows), 1, 0.7);
		mser->detectRegions(textImg, contours, bboxes);

		for (int i = 0; i < bboxes.size(); i++)
		{
			rectangle(inImg, bboxes[i], CV_RGB(0, 255, 0));
		}
		imwrite(pathOut, inImg);
		contours.clear();
		bboxes.clear();
	}

};