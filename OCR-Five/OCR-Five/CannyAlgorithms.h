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
using namespace std;
using namespace cv;


class CannyAlgorithms
{
private:
	char* sourcePath;
	char* destPath;


	/*		support methods		*/

	// reference: http://blog.ivank.net/fastest-gaussian-blur.html
	vector<double> boxesForGauss(double sigma, int n)  // standard deviation, number of boxes
	{
		double wIdeal = sqrt((12 * sigma*sigma / n) + 1);  // Ideal averaging filter width 
		double wl = floor(wIdeal);  
		if ((int)wl % 2 == 0) 
			wl--;
		double wu = wl + 2;

		double mIdeal = (12 * sigma*sigma - n*wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
		double m =	round(mIdeal);
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

public:
	CannyAlgorithms()
	{
		sourcePath = NULL;
		destPath = NULL;
	}

	CannyAlgorithms(char *src, char *dest)
	{
		int k1 = strlen(src);
		int k2 = strlen(dest);
		sourcePath = new char[k1 + 1];
		destPath = new char[k2 + 1];
		sourcePath[k1] = '\0';
		destPath[k2] = '\0';

		for (int i = 0; i < k1; i++)
			sourcePath[i] = src[i];
		for (int i = 0; i < k2; i++)
			destPath[i] = dest[i];
	}


	/*		Gaussian Filter		*/
	void GaussianFilter(int kernelsize)
	{
		Mat src = imread(sourcePath, IMREAD_GRAYSCALE);
		if (!src.data)
		{
			cout << "Cannot load immage at path " << sourcePath << endl;
			return;
		}
		vector<int> scl;
		vector<int> tcl;
		int w = src.cols;
		int h = src.rows;
		
		// ~ 58s to run
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				scl.push_back(src.at<uchar>(Point(j, i)));
				//cout << scl.size() << endl;
				tcl.push_back(0);
			}
		}
		gaussBlur_4(scl, tcl, w, h, 3);
		
		waitKey(0);
	}



	~CannyAlgorithms()
	{

	}
};
