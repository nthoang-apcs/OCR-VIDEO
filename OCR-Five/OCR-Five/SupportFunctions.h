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


/*		Contain functions to help other classes		*/
class SupportFunctions
{
private:
	// Attribute
public:

	// Constructor
	SupportFunctions()
	{

	}


	// Methods

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


	// Destructor
	~SupportFunctions()
	{

	}
};