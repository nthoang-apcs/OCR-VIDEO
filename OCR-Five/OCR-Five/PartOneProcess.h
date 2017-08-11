#pragma once
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;


class PartOneProcess
{
private:
	


	string ExtractNameOfFileFromPathIn(string PathIn)
	{
		int leng = PathIn.length();
		int pos = leng - 1;
		do
		{
			pos--;
		} while (PathIn[pos] != '.' && pos > 0);
		// pos at char '.'
		int pos2 = pos - 1;
		do
		{
			pos2--;
		} while (PathIn[pos2] != '\\' && pos2 > 0);
		// pos2 at char '\'
		pos2++;
		// get name
		vector<char> tmp;
		for (int i = pos2; i < pos; i++)
		{
			tmp.push_back(PathIn[i]);
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
	PartOneProcess()
	{

	}

	void ConvertandResizeOneFile(string pathIn, string pathOut)
	{
		Mat image = imread(pathIn);
		Size size(100, 100);
		Mat dst;
		int w = image.cols;
		int h = image.rows;
		if (w > 1300)
		{
			float propor = (float)w / (float)h;
			w = 1280;
			h = (int)((float)w / propor);
		}
		if (h > 1300)
		{
			float propor = (float)w / (float)h;
			h = 1280;
			w = (int)((float)h * propor);
		}
		size.width = w;
		size.height = h;
		resize(image, dst, size);
		imwrite(pathOut, dst);
	}

	void ConvertandResizeMultipleFiles(vector<string> fileList, string resultFolder)
	{
		int k = fileList.size();
		cout << "Total index: " << k << endl;
		for (int i = 0; i < k; i++)
		{
			cout << "Run index: " << i << endl;
			string name = ExtractNameOfFileFromPathIn(fileList[i]);
			string pathOut = resultFolder + name + ".jpg";
			ConvertandResizeOneFile(fileList[i], pathOut);
			name.clear();
			pathOut.clear();
		}
		return;
	}


	~PartOneProcess()
	{

	}
};