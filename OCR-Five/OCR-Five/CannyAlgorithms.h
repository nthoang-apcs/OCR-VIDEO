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
#include "MyAlgorithms.h"
using namespace std;
using namespace cv;

#define M_PI 3.14159265358979323846264338327
#define MAX_BRIGHTNESS 255

class CannyAlgorithms : public MyAlgorithms
{
private:
	char* sourcePath;
	char* destPath;


	/*		support methods		*/

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

	// copy all channel value into src and also create 1 extra vector that contains all value is 0;
	void ConvertGrayScaleImageToOneChannel(Mat &x, vector<int> &src, vector<int> &extra)
	{
		int w = x.cols;
		int h = x.rows;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				src.push_back(x.at<uchar>(Point(j, i)));
				//cout << scl.size() << endl;
				extra.push_back(0);
			}
		}
		return;
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

	double GetMaximumValueInVector(vector<double> &x)
	{
		double k = 0;
		int m = x.size();
		for (int i = 0; i < m; i++)
		{
			if (x[i] > k)
			{
				k = x[i];
			}
		}
		return k;
	}

	void WriteDownResult(vector<double> &After_Gx, vector<double> &After_Gy, vector<double> &G, vector<double> &nms, Mat &afterFilter, vector<int> &out, Mat &afterCanny)
	{
		imwrite("After-gaussian-filter.jpg", afterFilter);
		imwrite(destPath, afterCanny);
		WriteDownVector(After_Gx, "Gx.txt");
		WriteDownVector(After_Gy, "Gy.txt");
		WriteDownVector(G, "G.txt");
		WriteDownVector(nms, "nms.txt");
		WriteDownVector(out, "out.txt");
	}
	void WriteDownResult(vector<double> &After_Gx, vector<double> &After_Gy, vector<double> &G, vector<double> &nms, vector<int> &out, Mat &afterCanny)
	{
		imwrite(destPath, afterCanny);
		WriteDownVector(After_Gx, "Gx.txt");
		WriteDownVector(After_Gy, "Gy.txt");
		WriteDownVector(G, "G.txt");
		WriteDownVector(nms, "nms.txt");
		WriteDownVector(out, "out.txt");
	}
	void WriteDownVector(vector<double> &x, char *name)
	{
		FILE * pFile;
		pFile = fopen(name, "w");
		if (pFile != NULL)
		{
			int k = x.size();
			for (int i = 0; i < k; i++)
			{
				fputs(to_string(x[i]).c_str(), pFile);
				fputs(" ", pFile);
			}
			fclose(pFile);
		}
	}
	void WriteDownVector(vector<int> &x, char *name)
	{
		FILE * pFile;
		pFile = fopen(name, "w");
		if (pFile != NULL)
		{
			int k = x.size();
			for (int i = 0; i < k; i++)
			{
				fputs(to_string(x[i]).c_str(), pFile);
				fputs(" ", pFile);
			}
			fclose(pFile);
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


	
	Mat GaussianFilter(int kernelsize)
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
		ConvertGrayScaleImageToOneChannel(src, scl, tcl);
		// Run gaussian blur
		gaussBlur_4(scl, tcl, w, h, 3);
		// convert target channel to image
		src = ConvertOneChannelToGrayScaleImage(tcl, w, h);
		cout << "Finish Gaussian filter." << endl;
		scl.clear();
		tcl.clear();
		return src;
	}

	void FindGradient(Mat &afterFilter, vector<double> &After_Gx, vector<double> &After_Gy, vector<double> &G, int kernelsize)
	{
		int khalf = kernelsize / 2;
		int ny = afterFilter.cols;
		int nx = afterFilter.rows;
		// create an array gray channel for source channels
		vector<int> in;
		vector<double> kernel;
		CreateKernelForFindGradient(kernel);

		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				in.push_back(afterFilter.at<uchar>(Point(j, i)));
				//cout << scl.size() << endl;
				After_Gx.push_back(0);
				After_Gy.push_back(0);
				G.push_back(0);
			}
		}

		for (int m = khalf; m < nx - khalf; m++)
		{
			for (int n = khalf; n < ny - khalf; n++) 
			{
				float pixel = 0.0;
				size_t c = 0;
				for (int j = -khalf; j <= khalf; j++)
				{ 
					for (int i = -khalf; i <= khalf; i++) 
					{
						int k1 = (n - j) * nx + m - i;
						pixel += in[k1] * kernel[c];
						c++;
					}
				}
				int k2 = n * nx + m;
				After_Gx[k2] = pixel;
			}
		}
		cout << "Finish Gx" << endl;
		for (int m = khalf; m < nx - khalf; m++)
		{
			for (int n = khalf; n < ny - khalf; n++)
			{
				float pixel = 0.0;
				size_t c = 0;
				for (int j = -khalf; j <= khalf; j++)
				{
					for (int i = -khalf; i <= khalf; i++)
					{
						pixel += in[(n - j) * nx + m - i] * kernel[c];
						c++;
					}
				}
				After_Gy[n * nx + m] = pixel;
			}
		}
		cout << "Finish Gy" << endl;
		for (int i = 1; i < nx - 1; i++)
		{
			for (int j = 1; j < ny - 1; j++) {
				const int c = i + nx * j;
				// G[c] = abs(after_Gx[c]) + abs(after_Gy[c]);
				G[c] = hypot(After_Gx[c], After_Gy[c]);
			}
		}
		cout << "Finish G" << endl;
		kernel.clear();
		in.clear();
	}

	// Non-Maximal Suppression
	void NMS(vector<double> &After_Gx, vector<double> &After_Gy, vector<double> &G, vector<double> &nms, Mat &afterFilter)
	{
		int ny = afterFilter.cols;
		int nx = afterFilter.rows;
		for (int i = 1; i < nx - 1; i++)
		{
			for (int j = 1; j < ny - 1; j++)
			{
				const int c = i + nx * j;
				const int nn = c - nx;
				const int ss = c + nx;
				const int ww = c + 1;
				const int ee = c - 1;
				const int nw = nn + 1;
				const int ne = nn - 1;
				const int sw = ss + 1;
				const int se = ss - 1;

				const float dir = (float)(fmod(atan2(After_Gy[c],
					After_Gx[c]) + M_PI,
					M_PI) / M_PI) * 8;

				if (((dir <= 1 || dir > 7) && G[c] > G[ee] &&
					G[c] > G[ww]) || // 0 deg
					((dir > 1 && dir <= 3) && G[c] > G[nw] &&
					G[c] > G[se]) || // 45 deg
					((dir > 3 && dir <= 5) && G[c] > G[nn] &&
					G[c] > G[ss]) || // 90 deg
					((dir > 5 && dir <= 7) && G[c] > G[ne] &&
					G[c] > G[sw]))   // 135 deg
					nms[c] = G[c];
				else
					nms[c] = 0;
			}
		}
		cout << "Finish Non-Maximal Suppression" << endl;
	}
	// tmin = threshold min, tmax = threshold max
	vector<int> hysteresis(vector<double> &After_Gx, vector<double> &After_Gy, vector<double> &G, vector<double> &nms, Mat &afterFilter, double tmin, double tmax)
	{
		int ny = afterFilter.cols;
		int nx = afterFilter.rows;
		vector<int> edges, out;
		for (int i = 0; i < nms.size(); i++)
		{
			edges.push_back(0);
			out.push_back(0);
		}
		// Tracing edges with hysteresis . Non-recursive implementation.
		size_t c = 1;
		for (int j = 1; j < ny - 1; j++)
		{ 
			for (int i = 1; i < nx - 1; i++) {
				if (nms[c] >= tmax && out[c] == 0) { // trace edges
					out[c] = MAX_BRIGHTNESS;
					int nedges = 1;
					edges[0] = c;

					do {
						nedges--;
						const int t = edges[nedges];

						int nbs[8]; // neighbours
						nbs[0] = t - nx;     // nn
						nbs[1] = t + nx;     // ss
						nbs[2] = t + 1;      // ww
						nbs[3] = t - 1;      // ee
						nbs[4] = nbs[0] + 1; // nw
						nbs[5] = nbs[0] - 1; // ne
						nbs[6] = nbs[1] + 1; // sw
						nbs[7] = nbs[1] - 1; // se

						for (int k = 0; k < 8; k++)
							if (nms[nbs[k]] >= tmin && out[nbs[k]] == 0) {
								out[nbs[k]] = MAX_BRIGHTNESS;
								edges[nedges] = nbs[k];
								nedges++;
							}
					} while (nedges > 0);
				}
				c++;
			}
		}
		cout << "Perform Hysteresis Threshold" << endl;
		return out;
	}

	/*		Run step by step		*/
	// The result is much worse than the default canny
	// 0 < tmin < tmax < 1.xx
	void StepbyStepRun(int kernelsize, double tmin, double tmax)
	{
		Mat x = GaussianFilter(3);
		vector<double> Gx, Gy, G, nms;
		// Compute gradient and magnitude 
		FindGradient(x, Gx, Gy, G, 3);
		// Create size for nms
		for (int i = 0; i < Gx.size(); i++)
		{
			nms.push_back(0);
		}
		NMS(Gx, Gy, G, nms, x);
		vector<int> out = hysteresis(Gx, Gy, G, nms, x, tmin, tmax);
		Mat y = ConvertOneChannelToGrayScaleImage(out, x.cols, x.rows);
		// write down result
		cout << "Write down result." << endl;
		WriteDownResult(Gx, Gy, G, nms, x, out, y);
		cout << "Clear memory." << endl;
		Gx.clear();
		Gy.clear();
		G.clear();
		nms.clear();
		cout << "Show result: " << endl;
		imshow("result", y);
		waitKey(0);
	}
	// You can input tmin and tmax in middle of function
	void StepbyStepRun(int kernelsize)
	{
		Mat x = GaussianFilter(3);
		vector<double> Gx, Gy, G, nms;
		// Compute gradient and magnitude 
		FindGradient(x, Gx, Gy, G, 3);
		// Create size for nms
		for (int i = 0; i < Gx.size(); i++)
		{
			nms.push_back(0);
		}
		NMS(Gx, Gy, G, nms, x);
		cout << "Minimum value for threshold is: 0.0" << endl;
		cout << "Maximum value for threshold is: " << GetMaximumValueInVector(nms) << endl;
		double tmin, tmax;
		cout << "Input tmin: ";
		cin >> tmin;
		cout << "Input tmax: ";
		cin >> tmax;
		vector<int> out = hysteresis(Gx, Gy, G, nms, x, tmin, tmax);
		Mat y = ConvertOneChannelToGrayScaleImage(out, x.cols, x.rows);
		// write down result
		cout << "Write down result." << endl;
		WriteDownResult(Gx, Gy, G, nms, x, out, y);
		cout << "Clear memory." << endl;
		Gx.clear();
		Gy.clear();
		G.clear();
		nms.clear();
		cout << "Show result: " << endl;
		imshow("result", y);
		waitKey(0);
	}


	/*		Fast run - all in one - still use few function outside		*/
	void AllInOneRun(int kernelsize)
	{
		Mat src = imread(sourcePath, IMREAD_GRAYSCALE);
		if (!src.data)
		{
			cout << "Cannot load immage at path " << sourcePath << endl;
			return;
		}
		vector<int> scl;
		vector<int> in;
		int ny = src.cols;
		int nx = src.rows;

		// create an array gray channel for source channels
		ConvertGrayScaleImageToOneChannel(src, scl, in);
		// Run gaussian blur
		gaussBlur_4(scl, in, ny, nx, 3);
		cout << "Finish Gaussian filter." << endl;
		scl.clear();
		vector<double> After_Gx, After_Gy, G, nms;
		// Find gradient
		int khalf = kernelsize / 2;
		// create an array gray channel for source channels
		
		vector<double> kernel;
		CreateKernelForFindGradient(kernel);

		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				//cout << scl.size() << endl;
				After_Gx.push_back(0);
				After_Gy.push_back(0);
				G.push_back(0);
			}
		}

		for (int m = khalf; m < nx - khalf; m++)
		{
			for (int n = khalf; n < ny - khalf; n++)
			{
				float pixel = 0.0;
				size_t c = 0;
				for (int j = -khalf; j <= khalf; j++)
				{
					for (int i = -khalf; i <= khalf; i++)
					{
						int k1 = (n - j) * nx + m - i;
						pixel += in[k1] * kernel[c];
						c++;
					}
				}
				int k2 = n * nx + m;
				After_Gx[k2] = pixel;
			}
		}
		cout << "Finish Gx" << endl;
		for (int m = khalf; m < nx - khalf; m++)
		{
			for (int n = khalf; n < ny - khalf; n++)
			{
				float pixel = 0.0;
				size_t c = 0;
				for (int j = -khalf; j <= khalf; j++)
				{
					for (int i = -khalf; i <= khalf; i++)
					{
						pixel += in[(n - j) * nx + m - i] * kernel[c];
						c++;
					}
				}
				After_Gy[n * nx + m] = pixel;
			}
		}
		cout << "Finish Gy" << endl;
		for (int i = 1; i < nx - 1; i++)
		{
			for (int j = 1; j < ny - 1; j++) {
				const int c = i + nx * j;
				// G[c] = abs(after_Gx[c]) + abs(after_Gy[c]);
				G[c] = hypot(After_Gx[c], After_Gy[c]);
			}
		}
		cout << "Finish G" << endl;
		kernel.clear();
		in.clear();
		// Create size for nms
		for (int i = 0; i < After_Gx.size(); i++)
		{
			nms.push_back(0);
		}
		// NMS
		for (int i = 1; i < nx - 1; i++)
		{
			for (int j = 1; j < ny - 1; j++)
			{
				const int c = i + nx * j;
				const int nn = c - nx;
				const int ss = c + nx;
				const int ww = c + 1;
				const int ee = c - 1;
				const int nw = nn + 1;
				const int ne = nn - 1;
				const int sw = ss + 1;
				const int se = ss - 1;

				const float dir = (float)(fmod(atan2(After_Gy[c],
					After_Gx[c]) + M_PI,
					M_PI) / M_PI) * 8;

				if (((dir <= 1 || dir > 7) && G[c] > G[ee] &&
					G[c] > G[ww]) || // 0 deg
					((dir > 1 && dir <= 3) && G[c] > G[nw] &&
					G[c] > G[se]) || // 45 deg
					((dir > 3 && dir <= 5) && G[c] > G[nn] &&
					G[c] > G[ss]) || // 90 deg
					((dir > 5 && dir <= 7) && G[c] > G[ne] &&
					G[c] > G[sw]))   // 135 deg
					nms[c] = G[c];
				else
					nms[c] = 0;
			}
		}
		cout << "Finish Non-Maximal Suppression" << endl;
		vector<int> edges, out;
		for (int i = 0; i < nms.size(); i++)
		{
			edges.push_back(0);
			out.push_back(0);
		}
		cout << "Minimum value for threshold is: 0.0" << endl;
		cout << "Maximum value for threshold is: " << GetMaximumValueInVector(nms) << endl;
		double tmin, tmax;
		cout << "Input tmin: ";
		cin >> tmin;
		cout << "Input tmax: ";
		cin >> tmax;
		// Tracing edges with hysteresis . Non-recursive implementation.
		size_t c = 1;
		for (int j = 1; j < ny - 1; j++)
		{
			for (int i = 1; i < nx - 1; i++) {
				if (nms[c] >= tmax && out[c] == 0) { // trace edges
					out[c] = MAX_BRIGHTNESS;
					int nedges = 1;
					edges[0] = c;

					do {
						nedges--;
						const int t = edges[nedges];

						int nbs[8]; // neighbours
						nbs[0] = t - nx;     // nn
						nbs[1] = t + nx;     // ss
						nbs[2] = t + 1;      // ww
						nbs[3] = t - 1;      // ee
						nbs[4] = nbs[0] + 1; // nw
						nbs[5] = nbs[0] - 1; // ne
						nbs[6] = nbs[1] + 1; // sw
						nbs[7] = nbs[1] - 1; // se

						for (int k = 0; k < 8; k++)
							if (nms[nbs[k]] >= tmin && out[nbs[k]] == 0) {
								out[nbs[k]] = MAX_BRIGHTNESS;
								edges[nedges] = nbs[k];
								nedges++;
							}
					} while (nedges > 0);
				}
				c++;
			}
		}
		cout << "Perform Hysteresis Threshold" << endl;
		Mat y = ConvertOneChannelToGrayScaleImage(out, ny, nx);
		// write down result
		cout << "Write down result." << endl;
		WriteDownResult(After_Gx, After_Gy, G, nms, out, y);
		cout << "Clear memory." << endl;
		After_Gx.clear();
		After_Gy.clear();
		G.clear();
		nms.clear();
		cout << "Show result: " << endl;
		namedWindow("result", WINDOW_AUTOSIZE);
		imshow("result", y);
		waitKey(0);

	}




	void Run()
	{

	}

	/*		Other methods		*/
	
	Mat ConvertOneChannelToGrayScaleImage(vector<int> &tcl, int w, int h)
	{
		Mat src = Mat(h, w, CV_8U);
		int m = 0;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				src.at<uchar>(Point(j, i)) = tcl[m];
				m++;
			}
		}
		return src;
	}
	Mat ConvertOneChannelToGrayScaleImage(vector<double> &tcl, int w, int h)
	{
		Mat src = Mat(h, w, CV_8U);
		int m = 0;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				src.at<uchar>(Point(j, i)) = (int)tcl[m];
				m++;
			}
		}
		return src;
	}
	
	// Canny algorithms by opencv
	void DefaultCanny()
	{
		Mat src;
		Mat dst, detected_edges;
		char* window_name = "Edge Map";
		int lowThreshold = 20;
		int const max_lowThreshold = 100;
		int ratio = 3;
		int kernel_size = 3;

		src = imread(sourcePath, IMREAD_GRAYSCALE);
		if (!src.data)
		{
			return;
		}

		/// Create a matrix of the same type and size as src (for dst)
		dst.create(src.size(), src.type());

		blur(src, detected_edges, Size(3, 3));

		/// Canny detector
		Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

		/// Using Canny's output as a mask, we display our result
		dst = Scalar::all(0);

		src.copyTo(dst, detected_edges);
		imshow(window_name, dst);
		waitKey(0);
	}

	~CannyAlgorithms()
	{
		delete sourcePath;
		delete destPath;
	}
};


/*		Main function sample	*/
/*
CannyAlgorithms *c = new CannyAlgorithms("D:\\document\\LVTN2016\\DataOCR\\ICDA-2003\\SceneTrialTest\\ryoungt_13.08.2002\\dPICT0082.JPG", "TestCanny-1.jpg");
// canny by opencv
c->DefaultCanny();
// canny by this project
c->AllInOneRun(3);
// or
c->StepbyStepRun(3);
// or
c->StepbyStepRun(3, 1, 1.5);
*/