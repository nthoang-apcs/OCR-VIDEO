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

#define M_PI 3.14159265358979323846264338327

/*		Contain functions to help other classes		*/
class SupportFunctions
{
private:
	

public:

	// Constructor
	SupportFunctions()
	{

	}


	// Methods
	// max-value the max value after normalize
	void NormalizeToRange0a255(vector<double> &x, int MAX_Value, int MIN_Value)
	{
		// get max
		double k = 0;
		int m = x.size();
		for (int i = 0; i < m; i++)
		{
			if (x[i] > k)
			{
				k = x[i];
			}
		}
		for (int i = 0; i < m; i++)
		{
			x[i] = MAX_Value * (x[i] - MIN_Value) / (k - MIN_Value);
		}
		return;
	}

	void ReadDataGroupBoxesFromFileText(vector<string> &List, char* pathFile)
	{
		
		FILE *pFile;
		pFile = fopen(pathFile, "r");
		if (pFile != NULL)
		{
			while (fgetc(pFile) != EOF) 
			{
				char sample[4000];
				fgets(sample, 4000, pFile);
				int k = strlen(sample);
				sample[k - 1] = '\0';
				List.push_back(string(sample));
			}
			fclose(pFile);
		}
		return;
	}
	
	// convert a target channel to a gray scale image
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
	
	// copy all channel value of image into src 
	void ConvertGrayScaleImageToOneChannel(Mat &x, vector<int> &src)
	{
		int w = x.cols;
		int h = x.rows;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				src.push_back(x.at<uchar>(Point(j, i)));
				//cout << scl.size() << endl;
			}
		}
		return;
	}
	// copy all channel value into src and also create 1 extra vector that contains all value is 0;
	void ConvertGrayScaleImageToOneChannelWithExtra(Mat &x, vector<int> &src, vector<int> &extra)
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

	// get max value in vector
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
	int GetMaximumValueInVector(vector<int> &x)
	{
		int k = 0;
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

	// find derivatives of image
	/* 
	After_Gx is Ix
	After_Gy is Iy
	G is the gradient
	output kernel is to print kernel matrix to console
	*/
	void FindGradient(Mat &afterFilter, vector<double> &After_Gx, vector<double> &After_Gy, vector<double> &G, int kernelsize, bool outputkernel)
	{
		int khalf = kernelsize / 2;
		int ny = afterFilter.cols;
		int nx = afterFilter.rows;
		// create an array gray channel for source channels
		vector<int> in;
		vector<double> kernel;
		CreateKernelForFindGradient(kernel, kernelsize);
		if (outputkernel == true)
		{
			// output kernel to console
			PrintKernelConsole(kernel);
		}

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
		for (int i = 1; i < nx - 1; i++)
		{
			for (int j = 1; j < ny - 1; j++) {
				const int c = i + nx * j;
				// G[c] = abs(after_Gx[c]) + abs(after_Gy[c]);
				G[c] = hypot(After_Gx[c], After_Gy[c]);
			}
		}
		kernel.clear();
		in.clear();
	}
	void FindGradient(Mat &afterFilter, vector<double> &G, int kernelsize, bool outputkernel)
	{
		int khalf = kernelsize / 2;
		int ny = afterFilter.cols;
		int nx = afterFilter.rows;
		// create an array gray channel for source channels
		vector<int> in;
		vector<double> kernel, After_Gx, After_Gy;
		CreateKernelForFindGradient(kernel, kernelsize);
		if (outputkernel == true)
		{
			// output kernel to console
			PrintKernelConsole(kernel);
		}

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
		for (int i = 1; i < nx - 1; i++)
		{
			for (int j = 1; j < ny - 1; j++) {
				const int c = i + nx * j;
				// G[c] = abs(after_Gx[c]) + abs(after_Gy[c]);
				G[c] = hypot(After_Gx[c], After_Gy[c]);
			}
		}
		kernel.clear();
		in.clear();
	}
	void FindMagnitude(Mat &afterFilter, vector<double> &After_Gx, vector<double> &After_Gy, int kernelsize, bool outputkernel)
	{
		int khalf = kernelsize / 2;
		int ny = afterFilter.cols;
		int nx = afterFilter.rows;
		// create an array gray channel for source channels
		vector<int> in;
		vector<double> kernel;
		CreateKernelForFindGradient(kernel, kernelsize);
		if (outputkernel == true)
		{
			cout << "Output kernel to console" << endl;
			// output kernel to console
			PrintKernelConsole(kernel);
		}

		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				in.push_back(afterFilter.at<uchar>(Point(j, i)));
				//cout << scl.size() << endl;
				After_Gx.push_back(0);
				After_Gy.push_back(0);
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
		kernel.clear();
		in.clear();
	}
	void FindGradientX(Mat &afterFilter, vector<double> &After_Gx, int kernelsize, bool outputkernel)
	{
		int khalf = kernelsize / 2;
		int ny = afterFilter.cols;
		int nx = afterFilter.rows;
		// create an array gray channel for source channels
		vector<int> in;
		vector<double> kernel;
		CreateKernelForFindGradient(kernel, kernelsize);
		if (outputkernel == true)
		{
			// output kernel to console
			PrintKernelConsole(kernel);
		}
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				in.push_back(afterFilter.at<uchar>(Point(j, i)));
				//cout << scl.size() << endl;
				After_Gx.push_back(0);
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
		kernel.clear();
		in.clear();
	}
	void FindGradientY(Mat &afterFilter, vector<double> &After_Gy, int kernelsize, bool outputkernel)
	{
		int khalf = kernelsize / 2;
		int ny = afterFilter.cols;
		int nx = afterFilter.rows;
		// create an array gray channel for source channels
		vector<int> in;
		vector<double> kernel;
		CreateKernelForFindGradient(kernel, kernelsize);
		if (outputkernel == true)
		{
			// output kernel to console
			PrintKernelConsole(kernel);
		}
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				in.push_back(afterFilter.at<uchar>(Point(j, i)));
				//cout << scl.size() << endl;
				After_Gy.push_back(0);
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
						pixel += in[(n - j) * nx + m - i] * kernel[c];
						c++;
					}
				}
				After_Gy[n * nx + m] = pixel;
			}
		}
		kernel.clear();
		in.clear();
	}
	/* determine size of kernel (odd #)
		* 0.0 <= sigma < 0.5 : 3
		* 0.5 <= sigma < 1.0 : 5
		* 1.0 <= sigma < 1.5 : 7
		* 1.5 <= sigma < 2.0 : 9
		* 2.0 <= sigma < 2.5 : 11
		* 2.5 <= sigma < 3.0 : 13 ...
		* kernelSize = 2 * int(2 * sigma) + 3;
		Sample:
	1.9641280346397437e-005		0.00023927977920047060		0.0010723775711956546		0.0017680517118520167		0.0010723775711956546		0.00023927977920047060		1.9641280346397437e-005
	0.00023927977920047060		0.0029150244650281935		0.013064233284684921		0.021539279301848631		0.013064233284684921		0.0029150244650281935		0.00023927977920047060
	0.0010723775711956546		0.013064233284684921		0.058549831524319168		0.096532352630053914		0.058549831524319168		0.013064233284684921		0.0010723775711956546
	0.0017680517118520167		0.021539279301848631		0.096532352630053914		0.15915494309189535			0.096532352630053914		0.021539279301848631		0.0017680517118520167
	0.0010723775711956546		0.013064233284684921		0.058549831524319168		0.096532352630053914		0.058549831524319168		0.013064233284684921		0.0010723775711956546
	0.00023927977920047060		0.0029150244650281935		0.013064233284684921		0.021539279301848631		0.013064233284684921		0.0029150244650281935		0.00023927977920047060
	1.9641280346397437e-005		0.00023927977920047060		0.0010723775711956546		0.0017680517118520167		0.0010723775711956546		0.00023927977920047060		1.9641280346397437e-005
	*/
	void CreateKernelForFindGradient(vector<double> &Kernel, float &sigma)
	{
		if (sigma < 0)
		{
			sigma = 1;
		}
		if (sigma == 0)
		{
			// random (0, 0.5)
			srand(static_cast <unsigned> (time(0)));
			sigma = (float)(rand() / 2) / (float)(RAND_MAX);
		}
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
	void CreateKernelForFindGradient(vector<double> &Kernel, int kernelsize)
	{
		if (kernelsize < 3 || kernelsize % 2 == 0)
		{
			kernelsize = 3;
		}
		float sigma = ((float)(kernelsize - 3)) / 4;
		if (sigma == 0)
		{
			// random (0, 0.5)
			srand(static_cast <unsigned> (time(0)));
			sigma = (float)(rand() / 2) / (float)(RAND_MAX);
		}
		size_t c = 0;
		float mean = (float)floor(kernelsize / 2.0);
		for (int i = 0; i < kernelsize; i++)
		{
			for (int j = 0; j < kernelsize; j++)
			{
				double x1 = exp(-0.5 * (pow((i - mean) / sigma, 2.0) + pow((j - mean) / sigma, 2.0)));
				double x2 = (2 * M_PI * sigma * sigma);
				double x = x1 / x2;
				Kernel.push_back(x);
			}
		}
	}
	void PrintKernelConsole(vector<double> &Kernel)
	{
		int n = sqrt((float)Kernel.size());
		int count = 0;
		cout << "Kernel matrix: " << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << Kernel[count] << "\t\t";
				count++;
			}
			cout << endl;
		}
	}

	// Destructor
	~SupportFunctions()
	{

	}
};