#pragma once
/*
* ExperimentThree.h
*
* A demo source of an combination of Extremal Regions and Tesseract in Scene Text Recognition:
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


class ExpThree
{
private:
	vector<string> PathIn;
	vector<string> PathOut;

	Ptr<ERFilter> er_filter1;
	Ptr<ERFilter> er_filter2;
	Ptr<OCRTesseract> ocr;

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

	bool isRepetitive(const string& s)
	{
		int count = 0;
		for (int i = 0; i<(int)s.size(); i++)
		{
			if ((s[i] == 'i') ||
				(s[i] == 'l') ||
				(s[i] == 'I'))
				count++;
		}
		if (count >((int)s.size() + 1) / 2)
		{
			return true;
		}
		return false;
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

	bool sort_by_lenght(const string &a, const string &b) 
	{ 
		return (a.size()>b.size()); 
	}

public:
	ExpThree()
	{
		// init filter and ocr
		er_filter1 = createERFilterNM1(loadClassifierNM1("trained_classifierNM1.xml"), 8, 0.00015f, 0.13f, 0.2f, true, 0.1f);
		er_filter2 = createERFilterNM2(loadClassifierNM2("trained_classifierNM2.xml"), 0.5);

		ocr = OCRTesseract::create();
	}

	ExpThree(vector<string> PathFiles)
	{
		PathIn = PathFiles;
		// init filter and ocr
		er_filter1 = createERFilterNM1(loadClassifierNM1("trained_classifierNM1.xml"), 8, 0.00015f, 0.13f, 0.2f, true, 0.1f);
		er_filter2 = createERFilterNM2(loadClassifierNM2("trained_classifierNM2.xml"), 0.5);

		ocr = OCRTesseract::create();
	}

	// start running mser for all images in PathIn and save images to PathOut
	void RunAll(string pathfolder)
	{
		// create PathOut by: pathfolder + [name of file] + [-MSER] + extension(.jpg)
		int k = PathIn.size();
		for (int i = 0; i < k; i++)
		{
			string name = this->ExtractNameOfFileFromPathIn(i);
			PathOut.push_back(pathfolder + name + "-OCR" + ".jpg");
		}
		
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

		string output;
		double t_r = (double)getTickCount();
		Mat out_img;
		Mat out_img_detection;
		Mat out_img_segmentation = Mat::zeros(image.rows + 2, image.cols + 2, CV_8UC1);
		image.copyTo(out_img);
		image.copyTo(out_img_detection);
		float scale_img = 600.f / image.rows;
		float scale_font = (float)(2 - scale_img) / 1.4f;
		vector<string> words_detection;

		t_r = (double)getTickCount();

		for (int i = 0; i<(int)nm_boxes.size(); i++)
		{

			rectangle(out_img_detection, nm_boxes[i].tl(), nm_boxes[i].br(), Scalar(0, 255, 255), 3);

			Mat group_img = Mat::zeros(image.rows + 2, image.cols + 2, CV_8UC1);
			er_draw(channels, regions, nm_region_groups[i], group_img);
			Mat group_segmentation;
			group_img.copyTo(group_segmentation);
			//image(nm_boxes[i]).copyTo(group_img);
			group_img(nm_boxes[i]).copyTo(group_img);
			copyMakeBorder(group_img, group_img, 15, 15, 15, 15, BORDER_CONSTANT, Scalar(0));

			vector<Rect>   boxes;
			vector<string> words;
			vector<float>  confidences;
			ocr->run(group_img, output, &boxes, &words, &confidences, OCR_LEVEL_WORD);

			output.erase(remove(output.begin(), output.end(), '\n'), output.end());
			//cout << "OCR output = \"" << output << "\" length = " << output.size() << endl;
			if (output.size() < 3)
				continue;

			for (int j = 0; j<(int)boxes.size(); j++)
			{
				boxes[j].x += nm_boxes[i].x - 15;
				boxes[j].y += nm_boxes[i].y - 15;

				//cout << "  word = " << words[j] << "\t confidence = " << confidences[j] << endl;
				if ((words[j].size() < 2) || (confidences[j] < 51) ||
					((words[j].size() == 2) && (words[j][0] == words[j][1])) ||
					((words[j].size()< 4) && (confidences[j] < 60)) ||
					isRepetitive(words[j]))
					continue;
				words_detection.push_back(words[j]);
				rectangle(out_img, boxes[j].tl(), boxes[j].br(), Scalar(255, 0, 255), 3);
				Size word_size = getTextSize(words[j], FONT_HERSHEY_SIMPLEX, (double)scale_font, (int)(3 * scale_font), NULL);
				rectangle(out_img, boxes[j].tl() - Point(3, word_size.height + 3), boxes[j].tl() + Point(word_size.width, 0), Scalar(255, 0, 255), -1);
				putText(out_img, words[j], boxes[j].tl() - Point(1, 1), FONT_HERSHEY_SIMPLEX, scale_font, Scalar(255, 255, 255), (int)(3 * scale_font));
				out_img_segmentation = out_img_segmentation | group_segmentation;
			}

		}

		cout << "TIME_OCR = " << ((double)getTickCount() - t_r) * 1000 / getTickFrequency() << endl;
		imwrite(pathOut, out_img);
	}

};