#pragma once
#include <opencv2\text.hpp>
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
using namespace std;
using namespace cv;
using namespace cv::text;

class MyMSER
{
private:
	char* srcVietDir;
	char* srcEngDir;
	int srcVietDirCount;
	int srcEngDirCount;
	char* destVietDir;
	char* destEngDir;
	char* errorFilePath;
	char* groupboxesVietFilePath;
	char* groupboxesEngFilePath;

	// Support function
	void WriteDownLog(char* msg)
	{
		FILE *pFile;
		pFile = fopen(errorFilePath, "a");
		if (pFile != NULL)
		{
			fputs(msg, pFile);
			fclose(pFile);
		}
		return;
	}
	void WriteDownTimeLog()
	{
		FILE *pFile;
		pFile = fopen(errorFilePath, "a");
		if (pFile != NULL)
		{
			fputs(currentDateTime().c_str(), pFile);
			fputs(" ", pFile);
			fclose(pFile);
		}
		return;
	}
	void WriteDownGroupRectLog(vector<Rect> &groups)
	{
		FILE *pFile;
		pFile = fopen(errorFilePath, "a");
		if (pFile != NULL)
		{
			fputs("(Format groups_boxes: x y with height): ", pFile);
			for (int i = (int)groups.size() - 1; i >= 0; i--)
			{
				string s = to_string(groups.at(i).x) + " " + to_string(groups.at(i).y) + " " + to_string(groups.at(i).width) + " " + to_string(groups.at(i).height);
				fputs(s.c_str(), pFile);
				fputs(" | ", pFile);
			}
			fputs("\n", pFile);
			fclose(pFile);
		}
		return;
	}
	void WriteDownGroupBoxesVietData(vector<Rect> &groups)
	{
		FILE *pFile;
		pFile = fopen(groupboxesVietFilePath, "a");
		if (pFile != NULL)
		{
			for (int i = (int)groups.size() - 1; i >= 0; i--)
			{
				string s = to_string(groups.at(i).x) + " " + to_string(groups.at(i).y) + " " + to_string(groups.at(i).width) + " " + to_string(groups.at(i).height);
				fputs(s.c_str(), pFile);
				fputs(" | ", pFile);
			}
			fputs("\n", pFile);
			fclose(pFile);
		}
		return;
	}
	void WriteDownGroupBoxesEngData(vector<Rect> &groups)
	{
		FILE *pFile;
		pFile = fopen(groupboxesEngFilePath, "a");
		if (pFile != NULL)
		{
			for (int i = (int)groups.size() - 1; i >= 0; i--)
			{
				string s = to_string(groups.at(i).x) + " " + to_string(groups.at(i).y) + " " + to_string(groups.at(i).width) + " " + to_string(groups.at(i).height);
				fputs(s.c_str(), pFile);
				fputs(" | ", pFile);
			}
			fputs("\n", pFile);
			fclose(pFile);
		}
		return;
	}
	const std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		// for more information about date/time format
		strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
		// Example output: 2012-05-06.21:47:59
		return buf;
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

public:
	MyMSER()
	{
		srcEngDir = NULL;
		srcVietDir = NULL;
		srcVietDirCount = 0;
		srcEngDirCount = 0;
		groupboxesVietFilePath = NULL;
		groupboxesEngFilePath = NULL;
		destEngDir = NULL;
		destVietDir = NULL;
		errorFilePath = NULL;
	}

	MyMSER(char* sourceVietDir, char* sourceEngDir, int sourceVietNumberImageCount, int sourceEngNumberImageCount, char* destinationVietDir, char* destinationEngDir, char* ErrorFilePath, char* GroupBoxesVietFilePath, char* GroupBoxesEnglishFilePath)
	{
		// Create variables
		int k1 = strlen(sourceVietDir);
		int k2 = strlen(sourceEngDir);
		int k3 = strlen(GroupBoxesVietFilePath);
		int k4 = strlen(GroupBoxesEnglishFilePath);
		int k5 = strlen(destinationVietDir);
		int k6 = strlen(destinationEngDir);
		int k7 = strlen(ErrorFilePath);
		srcVietDir = new char[k1 + 1];
		srcEngDir = new char[k2 + 1];
		groupboxesVietFilePath = new char[k3 + 1];
		groupboxesEngFilePath = new char[k4 + 1];
		destVietDir = new char[k5 + 1];
		destEngDir = new char[k6 + 1];
		errorFilePath = new char[k7 + 1];
		// Copy variables' value
		srcEngDirCount = sourceEngNumberImageCount;
		srcVietDirCount = sourceVietNumberImageCount;
		for (int i = 0; i < k1; i++)
		{
			srcVietDir[i] = sourceVietDir[i];
		}
		srcVietDir[k1] = '\0';
		for (int i = 0; i < k2; i++)
		{
			srcEngDir[i] = sourceEngDir[i];
		}
		srcEngDir[k2] = '\0';
		for (int i = 0; i < k3; i++)
		{
			groupboxesVietFilePath[i] = GroupBoxesVietFilePath[i];
		}
		groupboxesVietFilePath[k3] = '\0';
		for (int i = 0; i < k4; i++)
		{
			groupboxesEngFilePath[i] = GroupBoxesEnglishFilePath[i];
		}
		groupboxesEngFilePath[k4] = '\0';
		for (int i = 0; i < k5; i++)
		{
			destVietDir[i] = destinationVietDir[i];
		}
		destVietDir[k5] = '\0';
		for (int i = 0; i < k6; i++)
		{
			destEngDir[i] = destinationEngDir[i];
		}
		destEngDir[k6] = '\0';
		for (int i = 0; i < k7; i++)
		{
			errorFilePath[i] = ErrorFilePath[i];
		}
		errorFilePath[k7] = '\0';
	}

	void Run(bool IsConvertToGreyImage)
	{
		RunForVietNameseSample(IsConvertToGreyImage);
		RunForEnglishSample(IsConvertToGreyImage);
	}

	void RunForVietNameseSample(bool IsConvertToGreyImage)
	{
		std::cout << "Start Running RunForVietNameseSample" << endl;
		WriteDownTimeLog();
		WriteDownLog("Start Running RunForVietNameseSample.\n\n");
		std::cout << endl;
		if (srcVietDir == NULL)
		{
			return;
		}
		// Run for source Viet Dir
		for (int i = 1; i <= srcVietDirCount; i++)
		{
			// Get Path
			string s = to_string(i);
			int k1 = s.length();
			int k2 = strlen(srcVietDir);
			char* CurPath = new char[k1 + k2 + 6];
			int pos = 0;
			for (int j = 0; j < k2; j++)
			{
				CurPath[pos] = srcVietDir[j];
				pos++;
			}
			CurPath[pos] = '\\';
			pos++;
			for (int j = 0; j < k1; j++)
			{
				CurPath[pos] = s[j];
				pos++;
			}
			CurPath[pos] = '.';
			pos++;
			CurPath[pos] = 'j';
			pos++;
			CurPath[pos] = 'p';
			pos++;
			CurPath[pos] = 'g';
			pos++;
			CurPath[pos] = '\0';
			pos++;

			// Load Image
			std::cout << "Start load image at path: " << CurPath << endl;
			WriteDownTimeLog();
			WriteDownLog("Start load image at path: ");
			WriteDownLog(CurPath);
			WriteDownLog("\n");
			Mat image = imread(CurPath);
			std::cout << "Image_With = " << image.cols << endl;
			std::cout << "Image_Height = " << image.rows << endl;

			// Extract channels to be processed individually
			vector<Mat> channels;
			Mat grey;
			if (IsConvertToGreyImage == true)
			{
				cvtColor(image, grey, COLOR_RGB2GRAY);
			}
			else
			{
				grey = image;
			}
			WriteDownTimeLog();
			WriteDownLog("Run Mser Detection.\n");
			// Extract MSER - detection region
			vector< vector< Point> > contours;
			vector< Rect> groups_boxes;
			Ptr< MSER> mser = MSER::create(21, (int)(0.00002*grey.cols*grey.rows), (int)(0.05*grey.cols*grey.rows), 1, 0.7);

			mser->detectRegions(grey, contours, groups_boxes);
			
			// output groups_boxes
			WriteDownGroupRectLog(groups_boxes);
			WriteDownTimeLog();
			WriteDownLog("Finish Mser Detection.\n");
			WriteDownGroupBoxesVietData(groups_boxes);

			// draw groups
			groups_draw(grey, groups_boxes);
			//imshow("grouping", grey);

			// Get result path
			int k3 = strlen(destVietDir);
			char* CurResultPath = new char[k1 + k3 + 6];
			pos = 0;
			for (int j = 0; j < k3; j++)
			{
				CurResultPath[pos] = destVietDir[j];
				pos++;
			}
			CurResultPath[pos] = '\\';
			pos++;
			for (int j = 0; j < k1; j++)
			{
				CurResultPath[pos] = s[j];
				pos++;
			}
			CurResultPath[pos] = '.';
			pos++;
			CurResultPath[pos] = 'j';
			pos++;
			CurResultPath[pos] = 'p';
			pos++;
			CurResultPath[pos] = 'g';
			pos++;
			CurResultPath[pos] = '\0';
			pos++;
			// Write to files
			cv::imwrite(CurResultPath, grey);

			WriteDownTimeLog();
			WriteDownLog("Finish image at path: ");
			WriteDownLog(CurResultPath);
			WriteDownLog("\n");
			WriteDownLog("\n");
			std::cout << "Finish image at path : " << CurResultPath << endl << endl;;

			// memory clean-up
			if (!groups_boxes.empty())
			{
				groups_boxes.clear();
			}
			delete CurPath;
			delete CurResultPath;
		}
	}

	void RunForEnglishSample(bool IsConvertToGreyImage)
	{
		std::cout << "Start Running RunForEnglishSample" << endl;
		WriteDownTimeLog();
		WriteDownLog("Start Running RunForEnglishSample.\n\n");
		std::cout << endl;
		if (srcEngDir == NULL)
		{
			return;
		}
		// Run for source Viet Dir
		for (int i = 1; i <= srcEngDirCount; i++)
		{
			// Get Path
			string s = to_string(i);
			int k1 = s.length();
			int k2 = strlen(srcEngDir);
			char* CurPath = new char[k1 + k2 + 6];
			int pos = 0;
			for (int j = 0; j < k2; j++)
			{
				CurPath[pos] = srcEngDir[j];
				pos++;
			}
			CurPath[pos] = '\\';
			pos++;
			for (int j = 0; j < k1; j++)
			{
				CurPath[pos] = s[j];
				pos++;
			}
			CurPath[pos] = '.';
			pos++;
			CurPath[pos] = 'j';
			pos++;
			CurPath[pos] = 'p';
			pos++;
			CurPath[pos] = 'g';
			pos++;
			CurPath[pos] = '\0';
			pos++;

			// Load Image
			std::cout << "Start load image at path: " << CurPath << endl;
			WriteDownTimeLog();
			WriteDownLog("Start load image at path: ");
			WriteDownLog(CurPath);
			WriteDownLog("\n");
			Mat image = imread(CurPath);
			std::cout << "Image_With = " << image.cols << endl;
			std::cout << "Image_Height = " << image.rows << endl;

			// Extract channels to be processed individually
			vector<Mat> channels;
			Mat grey;
			if (IsConvertToGreyImage == true)
			{
				cvtColor(image, grey, COLOR_RGB2GRAY);
			}
			else
			{
				grey = image;
			}
			WriteDownTimeLog();
			WriteDownLog("Run Mser Detection.\n");
			// Extract MSER - detection region
			vector< vector< Point> > contours;
			vector< Rect> groups_boxes;
			Ptr< MSER> mser = MSER::create(21, (int)(0.00002*grey.cols*grey.rows), (int)(0.05*grey.cols*grey.rows), 1, 0.7);

			mser->detectRegions(grey, contours, groups_boxes);

			// output groups_boxes
			WriteDownGroupRectLog(groups_boxes);
			WriteDownTimeLog();
			WriteDownLog("Finish Mser Detection.\n");
			WriteDownGroupBoxesEngData(groups_boxes);

			// draw groups
			groups_draw(grey, groups_boxes);
			//imshow("grouping", grey);

			// Get result path
			int k3 = strlen(destEngDir);
			char* CurResultPath = new char[k1 + k3 + 6];
			pos = 0;
			for (int j = 0; j < k3; j++)
			{
				CurResultPath[pos] = destEngDir[j];
				pos++;
			}
			CurResultPath[pos] = '\\';
			pos++;
			for (int j = 0; j < k1; j++)
			{
				CurResultPath[pos] = s[j];
				pos++;
			}
			CurResultPath[pos] = '.';
			pos++;
			CurResultPath[pos] = 'j';
			pos++;
			CurResultPath[pos] = 'p';
			pos++;
			CurResultPath[pos] = 'g';
			pos++;
			CurResultPath[pos] = '\0';
			pos++;
			// Write to files
			cv::imwrite(CurResultPath, grey);

			WriteDownTimeLog();
			WriteDownLog("Finish image at path: ");
			WriteDownLog(CurResultPath);
			WriteDownLog("\n");
			WriteDownLog("\n");
			std::cout << "Finish image at path : " << CurResultPath << endl << endl;;

			// memory clean-up
			if (!groups_boxes.empty())
			{
				groups_boxes.clear();
			}
			delete CurPath;
			delete CurResultPath;
		}
	}

	void RunOneSample(char* filePath, bool IsConvertToGreyImage)
	{
		Mat image = imread(filePath);
		std::cout << "Image_With = " << image.cols << endl;
		std::cout << "Image_Height = " << image.rows << endl;

		// Extract channels to be processed individually
		vector<Mat> channels;
		Mat grey;
		if (IsConvertToGreyImage == true)
		{
			cvtColor(image, grey, COLOR_RGB2GRAY);
		}
		else
		{
			grey = image;
		}
		// Extract MSER - detection region
		vector< vector< Point> > contours;
		vector< Rect> groups_boxes;
		Ptr< MSER> mser = MSER::create(21, (int)(0.00002*grey.cols*grey.rows), (int)(0.05*grey.cols*grey.rows), 1, 0.7);

		mser->detectRegions(grey, contours, groups_boxes);

		// draw groups
		groups_draw(grey, groups_boxes);
		imshow("grouping", grey);
	}

	void Destroy()
	{
		if (srcEngDir != NULL)
		{
			delete srcEngDir;
		}
		if (srcVietDir != NULL)
		{
			delete srcVietDir;
		}
		if (groupboxesVietFilePath != NULL)
		{
			delete groupboxesVietFilePath;
		}
		if (groupboxesEngFilePath != NULL)
		{
			delete groupboxesEngFilePath;
		}
		if (destVietDir != NULL)
		{
			delete destVietDir;
		}
		if (destEngDir != NULL)
		{
			delete destEngDir;
		}
		if (errorFilePath != NULL)
		{
			delete errorFilePath;
		}
	}
	~MyMSER()
	{
		if (srcEngDir != NULL)
		{
			delete srcEngDir;
		}
		if (srcVietDir != NULL)
		{
			delete srcVietDir;
		}
		if (groupboxesVietFilePath != NULL)
		{
			delete groupboxesVietFilePath;
		}
		if (groupboxesEngFilePath != NULL)
		{
			delete groupboxesEngFilePath;
		}
		if (destVietDir != NULL)
		{
			delete destVietDir;
		}
		if (destEngDir != NULL)
		{
			delete destEngDir;
		}
		if (errorFilePath != NULL)
		{
			delete errorFilePath;
		}
	}
};