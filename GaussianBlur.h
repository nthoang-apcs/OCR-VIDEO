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
#include <cmath>
#include "SupportFunctions.h"
using namespace std;
using namespace cv;

#define M_PI 3.14159265358979323846264338327

class MyGaussianBlur
{
private:

	// reference: http://blog.ivank.net/fastest-gaussian-blur.html
	// Code is convert to c++
	vector<double> boxesForGauss(double sigma, int n)  // standard deviation, number of boxes
	{
		double wIdeal = sqrt((12 * sigma*sigma / n) + 1);  // Ideal averaging filter width 
		double wl = floor(wIdeal);
		if ((int)wl % 2 == 0)
			wl--;
		double wu = wl + 2;

		double mIdeal = (12 * sigma*sigma - n*wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
		double m = round(mIdeal);
		// var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

		vector<double>sizes;
		for (int i = 0; i < n; i++) {
			if (i < m)
			{
				sizes.push_back(wl);
			}
			else
			{
				sizes.push_back(wu);
			}
		}

		return sizes;
	}
	vector<double> boxesForGx()
	{
		vector<double>sizes;
		sizes.push_back(-1);
		sizes.push_back(0);
		sizes.push_back(1);
		sizes.push_back(-2);
		sizes.push_back(0);
		sizes.push_back(2);
		sizes.push_back(-1);
		sizes.push_back(0);
		sizes.push_back(1);

		return sizes;
	}
	vector<double> boxesForGy()
	{
		vector<double>sizes;
		sizes.push_back(1);
		sizes.push_back(2);
		sizes.push_back(1);
		sizes.push_back(0);
		sizes.push_back(0);
		sizes.push_back(0);
		sizes.push_back(-1);
		sizes.push_back(-2);
		sizes.push_back(-1);

		return sizes;
	}
	// scl = source channel - source image gray channel
	// tcl = target channel - dest image gray channel
	// w = width
	// h = height
	// r = radius or kernel size
	void gaussBlur_4(vector<int> &scl, vector<int> &tcl, int w, int h, double r) {

		vector<double> bxs = boxesForGauss(r, 3);

		boxBlur_4(scl, tcl, w, h, (bxs[0] - 1) / 2);
		boxBlur_4(tcl, scl, w, h, (bxs[1] - 1) / 2);
		boxBlur_4(scl, tcl, w, h, (bxs[2] - 1) / 2);
	}

	void boxBlur_4(vector<int> &scl, vector<int> & tcl, int w, int h, double r) {
		for (int i = 0; i < (int)scl.size(); i++)
			tcl[i] = scl[i];

		boxBlurH_4(tcl, scl, w, h, r);
		boxBlurT_4(scl, tcl, w, h, r);
	}

	void boxBlurH_4(vector<int> &scl, vector<int> &tcl, int w, int h, double r) {
		double iarr = 1 / (r + r + 1);
		for (int i = 0; i < h; i++)
		{
			int ti = i*w;
			int li = ti;
			double ri = (double)ti + r;
			int fv = scl[(int)ti];
			int lv = scl[ti + w - 1];
			double val = (r + 1) * (double)fv;

			for (int j = 0; j < r; j++)
				val += (double)scl[ti + j];

			for (int j = 0; j <= r; j++)
			{
				val += (double)(scl[ri++] - fv);
				tcl[ti++] = (int)round(val*iarr);
			}

			for (int j = r + 1; j< (w - r); j++)
			{
				val += (double)(scl[ri++] - scl[li++]);
				tcl[ti++] = (int)round(val*iarr);
			}

			for (int j = w - r; j < w; j++)
			{
				val += (double)(lv - scl[li++]);
				tcl[ti++] = (int)round(val*iarr);
			}
		}
	}

	void boxBlurT_4(vector<int> &scl, vector<int> &tcl, int w, int h, double r) {
		double iarr = 1 / (r + r + 1);
		for (int i = 0; i < w; i++)
		{
			int ti = i;
			int li = ti;
			double ri = (double)ti + r*w;
			int fv = scl[ti];
			int lv = scl[ti + w*(h - 1)];
			double val = (r + 1) * (double)fv;
			for (int j = 0; j<r; j++)
			{
				val += (double)scl[ti + j * w];
			}

			for (int j = 0; j <= r; j++)
			{
				val += (double)(scl[ri] - fv);
				tcl[ti] = (int)round(val * iarr);
				ri += (double)w;
				ti += w;
			}

			for (int j = r + 1; j<h - r; j++)
			{
				val += (double)(scl[ri] - scl[li]);
				tcl[ti] = (int)round(val * iarr);
				li += w; ri += w; ti += w;
			}

			for (int j = h - r; j<h; j++)
			{
				val += (double)(lv - scl[li]);
				tcl[ti] = (int)round(val * iarr);
				li += w;
				ti += w;
			}
		}
	}

	void CreateKernelForFindGradient(vector<double> &Kernel)
	{
		int sigma = 1;
		int n = 2 * (int)(2 * sigma) + 3;
		float mean = (float)floor(n / 2.0);
		size_t c = 0;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				Kernel.push_back(exp(-0.5 * (pow((i - mean) / sigma, 2.0) + pow((j - mean) / sigma, 2.0))) / (2 * M_PI * sigma * sigma));
			}
		}
	}

	

public:

	MyGaussianBlur()
	{
	}

	Mat Run(int kernelsize, char* sourcePath)
	{
		Mat src = imread(sourcePath, IMREAD_GRAYSCALE);
		if (!src.data)
		{
			cout << "Cannot load immage at path " << sourcePath << endl;
			return src;
		}
		vector<int> scl;
		vector<int> tcl;
		int w = src.cols;
		int h = src.rows;

		// create an array gray channel for source channels
		SupportFunctions sf;
		sf.ConvertGrayScaleImageToOneChannelWithExtra(src, scl, tcl);
		// Run gaussian blur
		gaussBlur_4(scl, tcl, w, h, 3);
		// convert target channel to image
		src = sf.ConvertOneChannelToGrayScaleImage(tcl, w, h);
		cout << "Finish Gaussian filter." << endl;
		scl.clear();
		tcl.clear();
		return src;
	}

	Mat Run(int kernelsize, Mat input)
	{
		Mat src;
		cvtColor(input, src, CV_BGR2GRAY);
		if (!src.data)
		{
			cout << "Can't load input Mat" << endl;
			return src;
		}
		vector<int> scl;
		vector<int> tcl;
		int w = src.cols;
		int h = src.rows;

		// create an array gray channel for source channels
		SupportFunctions sf;
		sf.ConvertGrayScaleImageToOneChannelWithExtra(src, scl, tcl);
		// Run gaussian blur
		gaussBlur_4(scl, tcl, w, h, 3);
		// convert target channel to image
		src = sf.ConvertOneChannelToGrayScaleImage(tcl, w, h);
		cout << "Finish Gaussian filter." << endl;
		scl.clear();
		tcl.clear();
		return src;
	}

	~MyGaussianBlur()
	{
	}
};