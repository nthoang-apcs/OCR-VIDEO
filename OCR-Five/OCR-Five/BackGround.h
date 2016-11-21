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
#include <cstdlib>
#include "SupportFunctions.h"
#include "GaussianBlur.h"
#include "CannyAlgorithms.h"
using namespace std;
using namespace cv;


class BackGround
{
private:
	char *srcFolder;
	char *destFolder;
	char *srcFilePath;
	char *destFilePath;
	vector<string> currentWindowName;
	int temporaryFiles;
	vector<char> PreviousOption;
	
	// Ask for resources
	void AskForSrcFilePath()
	{
		cout << "Input source file path: ";
		srcFilePath = new char[200];
		cin.getline(srcFilePath, 200);
	}
	bool AskForDestFilePath()
	{
		cout << "Do you want to input destination file path ? (y/n) " << endl;
		char c;
		cin >> c;
		if (c == 'n' || c == 'N')
		{
			cout << "Now, the result file path is at the folder where the program executes. Each time run an algorithm, the result will be announced." << endl;
			return false;
		}
		cout << "Input source file path: ";
		destFilePath = new char[200];
		cin.getline(destFilePath, 200);
		return true;
	}
	bool AskForSrcFolder()
	{
		cout << "Do you want to input source folder ? (y/n) " << endl;
		char c;
		cin >> c;
		if (c == 'n' || c == 'N')
		{
			cout << "Now, each time run an algorithm, the source will be ask to input if it is necessary." << endl;
			return false;
		}
		cout << "Input source file path: ";
		srcFolder = new char[200];
		cin.getline(srcFolder, 200);
		return true;
	}
	bool AskForDestFolder()
	{
		cout << "Do you want to input destination folder ? (y/n) " << endl;
		char c;
		cin >> c;
		if (c == 'n' || c == 'N')
		{
			cout << "Now, the result folder is at the folder where the program executes. Each time run an algorithm, the result will be announced." << endl;
			return false;
		}
		cout << "Input source file path: ";
		destFolder = new char[200];
		cin.getline(destFolder, 200);
		return true;
	}
	
	// Output to console
	void OutputConsoleListKeyboardFeature()
	{
		cout << "Input keyboard option: " << endl;
		cout << "i - Reload the original image (i.e. cancel any previous processing)" << endl;
		cout << "w - Save the current(possibly processed) image into the file out.jpg" << endl;
		cout << "g - Convert the image to grayscale using the openCV conversion function." << endl;
		cout << "G - Convert the image to grayscale using your implementation of conversion function." << endl;
		cout << "c - Cycle through the color channels of the image showing a different channel every time the key is pressed." << endl;
		cout << "C - Clear console screen." << endl;
		cout << "s - Convert the image to grayscale and smooth it using the openCV function. Use a track bar to control the amount of smoothing." << endl;
		cout << "S - Convert the image to grayscale and smooth it using your function which should perform convolution with a suitable filter.Use a track bar to control the amount of smoothing." << endl;
		cout << "x - Convert the image to grayscale and perform convolution with an x derivative filter. Normalize the obtained values to the range[0, 255]." << endl;
		cout << "y - Convert the image to grayscale and perform convolution with a y derivative filter.Normalize the obtained values to the range[0, 255]." << endl;
		cout << "m - Show the magnitude of the gradient normalized to the range[0, 255].The gradient is computed based on the x and y derivatives of the image." << endl;
		cout << "p - Convert the image to grayscale and plot the gradient vectors of the image every N pixels and let the plotted gradient vectors have a length of K.Use a track bar to control N.Plot the vectors as short line segments of length K." << endl;
		cout << "r - Convert the image to grayscale and rotate it using an angle of Q degrees. Use a track bar to control the rotation angle.The rotation of the image should be performed using an inverse map so there are no holes in it." << endl;
		cout << "R: Run algorithms." << endl;
		cout << "h - Display a short description of the program, its command line arguments, and the keys it supports." << endl;
	}
	void OutputConsoleDescription()
	{
		cout << "This program runs all algorithms that it has. The program has no arguments input by command line. Keys that it supports is show below" << endl;
	}
	
	// Keyboard input function & clear previousOption
	bool LoadOriginalImage(Mat& a)
	{
		a = imread(srcFilePath);
		if (!a.data)
		{
			cout << "Can't load original image. File path have problem" << endl;
			cout << "File path: " << srcFilePath << endl;
			return false;
		}
		PreviousOption.clear();
		return true;
	}
	bool LoadOriginalImage(Mat& a, char *path)
	{
		a = imread(path);
		if (!a.data)
		{
			return false;
		}
		PreviousOption.clear();
		return true;
	}
	
	void WriteCurrentProcessedImage(Mat& a, Mat& dest1, Mat& dest2)
	{
		if (dest2.data)
		{
			imwrite("out.jpg", dest2);
		}
		else if (dest1.data)
		{
			imwrite("out.jpg", dest1);
		}
		else
		{
			imwrite("out.jpg", a);
		}
	}
	
	bool ConvertGrayScaleByOpencv(Mat& a, Mat& dest1, Mat& dest2)
	{
		try
		{
			if (!dest1.data)
			{
				cvtColor(a, dest1, CV_BGR2GRAY);
			}
			else if (!dest2.data)
			{
				cvtColor(dest1, dest2, CV_BGR2GRAY);
			}
			else
			{
				dest1.release();
				dest1 = dest2;
				dest2.release();
				cvtColor(dest1, dest2, CV_BGR2GRAY);
			}
			return true;
		}
		catch (Exception e)
		{
			cout << "Can't convert to gray scale image by opencv. Have problems with current processed image." << endl;
			return false;
		}
	}
	bool ConvertGrayScaleBySelf(Mat& a, Mat& dest1, Mat& dest2)
	{
		try
		{
			if (!dest1.data)
			{
				dest1 = Mat(a.rows, a.cols, CV_8U);
				int y = a.rows;
				int x = a.cols;
				for (int i = 0; i < y; i++)
				{
					for (int j = 0; j < x; j++)
					{
						// Gray = (Red * 0.299 + Green * 0.587 + Blue * 0.114)
						dest1.at<uchar>(Point(j, i)) = (float)a.at<Vec3b>(j, i).val[0] * 0.114 + (float)a.at<Vec3b>(j, i).val[1] * 0.587 + (float)a.at<Vec3b>(j, i).val[2] * 0.299;
					}
				}
			}
			else if (!dest2.data)
			{
				if (dest1.type() == CV_8U)
				{
					dest2 = dest1;
				}
				else
				{
					dest2 = Mat(dest1.rows, dest1.cols, CV_8U);
					int y = dest1.rows;
					int x = dest1.cols;
					for (int i = 0; i < y; i++)
					{
						for (int j = 0; j < x; j++)
						{
							// Gray = (Red * 0.299 + Green * 0.587 + Blue * 0.114)
							dest2.at<uchar>(Point(j, i)) = (float)dest1.at<Vec3b>(j, i).val[0] * 0.114 + (float)dest1.at<Vec3b>(j, i).val[1] * 0.587 + (float)dest1.at<Vec3b>(j, i).val[2] * 0.299;
						}
					}
				}
			}
			else
			{
				dest1.release();
				dest1 = dest2;
				dest2.release();
				if (dest1.type() == CV_8U)
				{
					dest2 = dest1;
				}
				else
				{
					dest2 = Mat(dest1.rows, dest1.cols, CV_8U);
					int y = dest1.rows;
					int x = dest1.cols;
					for (int i = 0; i < y; i++)
					{
						for (int j = 0; j < x; j++)
						{
							// Gray = (Red * 0.299 + Green * 0.587 + Blue * 0.114)
							dest2.at<uchar>(Point(j, i)) = (float)dest1.at<Vec3b>(j, i).val[0] * 0.114 + (float)dest1.at<Vec3b>(j, i).val[1] * 0.587 + (float)dest1.at<Vec3b>(j, i).val[2] * 0.299;
						}
					}
				}
			}
			return true;
		}
		catch (Exception e)
		{
			cout << "Can't convert to gray scale image by my implement. Have problems with current processed image." << endl;
			return false;
		}
	}
	
	void CycleColorChannelImage(Mat& a, Mat& dest1, Mat& dest2)
	{

	}
	void ClearScreen()
	{
		system("cls");
		OutputConsoleListKeyboardFeature();
	}
	void SmoothByOpenCV(Mat& a, Mat& dest1, Mat& dest2)
	{
		ConvertGrayScaleByOpencv(a, dest1, dest2);
		if (!dest1.data)
		{
			blur(a, dest1, Size(3,3));
		}
		else if (!dest2.data)
		{
			blur(dest1, dest2, Size(3, 3));
		}
		else
		{
			dest1.release();
			dest1 = dest2;
			dest2.release();
			blur(dest1, dest2, Size(3, 3));
		}
	}
	void SmoothByMe(Mat& a, Mat& dest1, Mat& dest2)
	{
		ConvertGrayScaleBySelf(a, dest1, dest2);
		if (!dest1.data)
		{
			MyGaussianBlur gb;
			dest1 = gb.Run(3, a);
		}
		else if (!dest2.data)
		{
			MyGaussianBlur gb;
			dest2 = gb.Run(3, dest1);
		}
		else
		{
			dest1.release();
			dest1 = dest2;
			dest2.release();

			MyGaussianBlur gb;
			dest2 = gb.Run(3, dest1);
		}
	}

	void Runalgorithm()
	{
		// Get destination file path for the result of this algorithm
		char src[1000];
		cout << "Input image source file path for this algorithm: ";
		cin.getline(src, 1000);
		char dest[1000];
		cout << "Input result image full name of path for this algorithm: ";
		cin.getline(dest, 1000);
		MyCannyAlgorithms *c = new MyCannyAlgorithms(src, dest);
		c->Run(3);
		cout << "Finish running algorithm." << endl;
	}

public:
	BackGround()
	{
		srcFolder = NULL;
		destFolder = NULL;
		srcFilePath = NULL;
		destFilePath = NULL;
		temporaryFiles = 0;
	}

	void Run()
	{
		Mat a;	// source
		Mat dest1, dest2; // processed image
		do {
			AskForSrcFilePath();
			cout << "Now loading the image." << endl;
			a = imread(srcFilePath);
			if (a.data)
			{
				break;
			}
			else
			{
				cout << "Can't load image!!" << endl;
			}
		} while (true);
		OutputConsoleListKeyboardFeature();
		do{
			cout << "Choose your option." << endl;
			char c;
			cin >> c;
			cin.ignore(1000, '\n');
			switch (c)
			{
			case 'i':		// reload original image
				LoadOriginalImage(a);
				break;
			case 'w':		// write current processed image
				WriteCurrentProcessedImage(a, dest1, dest2);
				break;
			case 'g':		// grayscale by opencv
				ConvertGrayScaleByOpencv(a, dest1, dest2);
				break;
			case 'G':		// grayscale by my implement
				ConvertGrayScaleBySelf(a, dest1, dest2);
				break;
			case 'c':
				CycleColorChannelImage(a, dest1, dest2);
				break;
			case 'C':
				ClearScreen();
				break;
			case 's':
				SmoothByOpenCV(a, dest1, dest2);
				break;
			case 'S':
				SmoothByMe(a, dest1, dest2);
				break;
			case 'x':
				break;
			case 'y':
				break;
			case 'm':
				break;
			case 'p':
				break;
			case 'r':
				break;
			case 'R':
				Runalgorithm();
				break;
			case 'h':
				break;
			default:
				break;
			}
			PreviousOption.push_back(c);
		} while (true);
		
	}

	


	~BackGround()
	{
		delete srcFolder;
		delete destFolder;
		delete srcFilePath;
		delete destFilePath;
		currentWindowName.clear();
	}
};