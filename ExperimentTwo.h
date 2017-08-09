#pragma once
/*
* ExperimentTwo.h
*
* A demo source of ER in Scene Text Detection:
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
#include <iostream>
#include <iomanip>
using namespace std;
using namespace cv;
using namespace cv::text;


class ExpTwo
{
private:
	vector<string> PathIn;
	vector<string> PathOut;

	Ptr<ERFilter> er_filter1;
	Ptr<ERFilter> er_filter2;

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

	void groups_draw(Mat &src, vector<Rect> &groups)
	{
		for (int i = (int)groups.size() - 1; i >= 0; i--)
		{
			if (src.type() == CV_8UC3)
				rectangle(src, groups.at(i).tl(), groups.at(i).br(), Scalar(0, 255, 255), 3, 8);
			else
				rectangle(src, groups.at(i).tl(), groups.at(i).br(), Scalar(255), 3, 8);
		}
	}

	void er_draw(vector<Mat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, Mat& segmentation)
	{
		for (int r = 0; r<(int)group.size(); r++)
		{
			ERStat er = regions[group[r][0]][group[r][1]];
			if (er.parent != NULL) // deprecate the root region
			{
				int newMaskVal = 255;
				int flags = 4 + (newMaskVal << 8) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
				floodFill(channels[group[r][0]], segmentation, Point(er.pixel%channels[group[r][0]].cols, er.pixel / channels[group[r][0]].cols),
					Scalar(255), 0, Scalar(er.level), Scalar(0), flags);
			}
		}
	}

public:

	ExpTwo()
	{

	}

	ExpTwo(vector<string> PathFiles)
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
			PathOut.push_back(pathfolder + name + "-ER" + ".jpg");
		}
		// init filter
		er_filter1 = createERFilterNM1(loadClassifierNM1("trained_classifierNM1.xml"), 8, 0.00015f, 0.13f, 0.2f, true, 0.1f);
		er_filter2 = createERFilterNM2(loadClassifierNM2("trained_classifierNM2.xml"), 0.5);
		// run
		for (int i = 0; i < k; i++)
		{
			cout << "Run index " << i << endl;
			Run(PathIn[i], PathOut[i]);
		}
	}
	// run 1 file at a time
	void Run(string pathIn, string pathOut)
	{
		Mat image = imread(pathIn);

		// Extract channels to be processed individually
		vector<Mat> channels;

		Mat grey;
		cvtColor(image, grey, COLOR_RGB2GRAY);

		// Notice here we are only using grey channel, see textdetection.cpp for example with more channels
		channels.push_back(grey);
		channels.push_back(255 - grey);

		double t_d = (double)getTickCount();
		
		//er_filter1 = createERFilterNM1(loadClassifierNM1("trained_classifierNM1.xml"), 8, 0.00015f, 0.13f, 0.2f, true, 0.1f);
		//er_filter2 = createERFilterNM2(loadClassifierNM2("trained_classifierNM2.xml"), 0.5);

		vector<vector<ERStat> > regions(channels.size());
		// Apply the default cascade classifier to each independent channel (could be done in parallel)
		for (int c = 0; c<(int)channels.size(); c++)
		{
			er_filter1->run(channels[c], regions[c]);
			er_filter2->run(channels[c], regions[c]);
		}
		cout << "TIME_REGION_DETECTION = " << ((double)getTickCount() - t_d) * 1000 / getTickFrequency() << endl;

		Mat out_img_decomposition = Mat::zeros(image.rows + 2, image.cols + 2, CV_8UC1);
		vector<Vec2i> tmp_group;
		for (int i = 0; i<(int)regions.size(); i++)
		{
			for (int j = 0; j<(int)regions[i].size(); j++)
			{
				tmp_group.push_back(Vec2i(i, j));
			}
			Mat tmp = Mat::zeros(image.rows + 2, image.cols + 2, CV_8UC1);
			er_draw(channels, regions, tmp_group, tmp);
			if (i > 0)
				tmp = tmp / 2;
			out_img_decomposition = out_img_decomposition | tmp;
			tmp_group.clear();
		}

		double t_g = (double)getTickCount();
		// Detect character groups
		vector< vector<Vec2i> > nm_region_groups;
		vector<Rect> nm_boxes;
		erGrouping(image, channels, regions, nm_region_groups, nm_boxes, ERGROUPING_ORIENTATION_HORIZ);
		cout << "TIME_GROUPING = " << ((double)getTickCount() - t_g) * 1000 / getTickFrequency() << endl;
		// draw groups
		groups_draw(image, nm_boxes);
		// imwrite
		imwrite(pathOut, image);
		nm_boxes.clear();
		nm_region_groups.clear();
		regions.clear();
		channels.clear();
	}

};