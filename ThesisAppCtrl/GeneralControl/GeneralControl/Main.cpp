#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp>
#include "MyProcess.h"

using namespace std;
using namespace cv;

/////////////////////////////////
/*
- Support main function
*/
/////////////////////////////////

void LoadSetting(bool &bRemoveUnusual, bool &bRemoveUnbalancedRatio, bool &bRemoveSingleBox, bool &bMergeInside, bool &bMergeTextLine, bool &bRecheckVowel);


/////////////////////////////////
/*
- Main Function
- Input argument is the file path of filecapture.txt
- The return value of this main function is 1 if success, 0 if failed.
*/
/////////////////////////////////

int main(int argc, const char * argv[])
{
	if (argc != 2)
	{
		return 0;
	}

	/*		Setting variables		*/
	bool bRemoveUnusual = true;
	bool bRemoveUnbalancedRatio = true;
	bool bRemoveSingleBox = true;
	bool bMergeInside = true;
	bool bMergeTextLine = true;
	bool bRecheckVowel = true;


	/*		Get paths of files		*/
	char *fCapturePath = new char[200];
	char *CurrentFolder = new char[200];
	int itmp1 = 0;
	// get CurrentFolder
	while(argv[0][itmp1] != 0)
	{
		itmp1++;
	}
	while(argv[0][itmp1] != '\\' && argv[0][itmp1] != '/')
	{
		itmp1--;
	}
	itmp1++;
	for(int i = 0; i < itmp1; i++)
	{
		CurrentFolder[i] == argv[0][itmp1];
	}
	CurrentFolder[itmp1] = 0;
	// get fCapturePath
	itmp1 = 0;
	while(argv[1][itmp1] != 0)
	{
		fCapturePath[itmp1] = argv[1][itmp1];
		itmp1++;
	}
	fCapturePath[itmp1] = 0;
	itmp1 = 0;

	// get list of path files
	vector<string> fPaths;
	string tmp1;
	ifstream ifs(fCapturePath);

	if (ifs.is_open())
	{
		while (getline(ifs, tmp1))
		{
			fPaths.push_back(tmp1);
		}
		ifs.close();
	}
	else
	{
		return 0;
	}
	delete[] fCapturePath;


	/*		Read setting file or input new setting		*/
	LoadSetting(bRemoveUnusual, bRemoveUnbalancedRatio, bRemoveSingleBox, bMergeInside, bMergeTextLine, bRecheckVowel);


	/*		Load image		*/
	vector<Mat> mOriginImages;

	int tmp2 = fPaths.size();
	for (int i = 0; i < tmp2; i++)
	{
		mOriginImages.push_back(imread(fPaths[i]));
	}

	/*		Check mode		*/
	if (tmp2 == 1)
	{
		Mat mOutputImage;
		// 1 image
		RunProcessOne(mOriginImages[0], mOutputImage, CurrentFolder);
	}
	else
	{
		vector<Mat> mOutputImages;
		// multiple image
		RunProcessAll(mOriginImages, mOutputImages, CurrentFolder);
	}

	delete[] CurrentFolder

	return 1;
}


/*		Additional functions		*/

void LoadSetting(bool &bRemoveUnusual, bool &bRemoveUnbalancedRatio, bool &bRemoveSingleBox, bool &bMergeInside, bool &bMergeTextLine, bool &bRecheckVowel)
{
	ifstream ifs1("ctrsetting.ini");
	if (ifs1.is_open())
	{
		// load
		// ignore first 2 line
		// 0 - false
		// 1 - true
		string line;
		int k;
		getline(ifs1, line);
		getline(ifs1, line);
		// bool bRemoveUnusual
		getline(ifs1, line);
		k = line.length();
		if (line[k - 1] == '0')
		{
			bRemoveUnusual = false;
		}
		else
		{
			bRemoveUnusual = true;
		}
		// bool bRemoveUnbalancedRatio
		getline(ifs1, line);
		k = line.length();
		if (line[k - 1] == '0')
		{
			bRemoveUnbalancedRatio = false;
		}
		else
		{
			bRemoveUnbalancedRatio = true;
		}
		// bool bRemoveSingleBox
		getline(ifs1, line);
		k = line.length();
		if (line[k - 1] == '0')
		{
			bRemoveSingleBox = false;
		}
		else
		{
			bRemoveSingleBox = true;
		}
		// bool bMergeInside
		getline(ifs1, line);
		k = line.length();
		if (line[k - 1] == '0')
		{
			bMergeInside = false;
		}
		else
		{
			bMergeInside = true;
		}
		// bool bMergeTextLine
		getline(ifs1, line);
		k = line.length();
		if (line[k - 1] == '0')
		{
			bMergeTextLine = false;
		}
		else
		{
			bMergeTextLine = true;
		}
		// bool bRecheckVowel
		getline(ifs1, line);
		k = line.length();
		if (line[k - 1] == '0')
		{
			bRecheckVowel = false;
		}
		else
		{
			bRecheckVowel = true;
		}
		// others


		ifs1.close();
	}
	else
	{
		ifs1.close();
		ofstream ofs("ctrsetting.ini");
		if (ofs.is_open())
		{
			// create new
			ofs << "Dang Tuan Vu - Application Control Setting\n";
			ofs << "0 is false, 1 is true\n";

			// bool bRemoveUnusual
			ofs << "Remove unusual area: 1\n";
			// bool bRemoveUnbalancedRatio
			ofs << "Remove unbalanced ratio: 1\n";
			// bool bRemoveSingleBox
			ofs << "Remove single box line: 1\n";
			// bool bMergeInside
			ofs << "Merge inside boxes: 1\n";
			// bool bMergeTextLine
			ofs << "Merge boxes on a line: 1\n";
			// bool bRecheckVowel
			ofs << "Recheck Vowel: 1\n";
			// others

			ofs.close();
		}
	}
}
