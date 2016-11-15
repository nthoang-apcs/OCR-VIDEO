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
#include "GaussianBlur.h"

using namespace std;
using namespace cv;

#define M_PI 3.14159265358979323846264338327
#define MAX_BRIGHTNESS 255

// The range of tmin and tmax is not normalize to the range [0..255]

class MyCannyAlgorithms
{
private:
	char* sourcePath;
	char* destPath;


	/*		support methods		*/

	
	
	


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
	MyCannyAlgorithms()
	{
		sourcePath = NULL;
		destPath = NULL;
	}

	MyCannyAlgorithms(char *src, char *dest)
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

	/*		Run		*/
	// The result is much worse than the default canny
	// 0 < tmin < tmax < 1.xx
	void Run(int kernelsize, double tmin, double tmax)
	{
		// create variables
		SupportFunctions sf;
		MyGaussianBlur gb;

		// gaussian blur
		Mat x = gb.Run(kernelsize, sourcePath);
		vector<double> Gx, Gy, G, nms;
		
		// Compute gradient and magnitude 
		sf.FindGradient(x, Gx, Gy, G, 3, false);
		
		// Create size for nms
		for (int i = 0; i < Gx.size(); i++)
		{
			nms.push_back(0);
		}
		NMS(Gx, Gy, G, nms, x);
		vector<int> out = hysteresis(Gx, Gy, G, nms, x, tmin, tmax);
		
		Mat y = sf.ConvertOneChannelToGrayScaleImage(out, x.cols, x.rows);
		
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
	void Run(int kernelsize)
	{
		// create variables
		SupportFunctions sf;
		MyGaussianBlur gb;

		// gaussian blur
		Mat x = gb.Run(kernelsize, sourcePath);
		vector<double> Gx, Gy, G, nms;

		// Compute gradient and magnitude 
		sf.FindGradient(x, Gx, Gy, G, 3, false);

		// Create size for nms
		for (int i = 0; i < Gx.size(); i++)
		{
			nms.push_back(0);
		}
		NMS(Gx, Gy, G, nms, x);

		// input tmin, tmax
		cout << "Minimum value for threshold is: 0.0" << endl;
		cout << "Maximum value for threshold is: " << sf.GetMaximumValueInVector(nms) << endl;
		double tmin, tmax;
		cout << "Input tmin: ";
		cin >> tmin;
		cout << "Input tmax: ";
		cin >> tmax;

		vector<int> out = hysteresis(Gx, Gy, G, nms, x, tmin, tmax);

		Mat y = sf.ConvertOneChannelToGrayScaleImage(out, x.cols, x.rows);

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

	~MyCannyAlgorithms()
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
// or
c->Run(3);
// or
c->Run(3, 1, 1.5);
*/