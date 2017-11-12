#include "MyMSER.h"


void MSEROneImage(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning)
{
	clock_t start = clock();
	// convert gray from src to textImg
	Mat textImg;
	cvtColor(input, textImg, CV_BGR2GRAY);
	// sharpen
	Mat sharpened;
	SharpenOneImage(textImg, sharpened);
	// run mser
	vector<vector<Point>> contours;
	Ptr<MSER> mser = MSER::create(
		21,
		(int)(0.00002*sharpened.cols*sharpened.rows),
		(int)(0.05*sharpened.cols*sharpened.rows),
		1, 0.7
	);
	mser->detectRegions(sharpened, contours, BBoxes);

	// remove duplicated Rects
	RemoveDuplicatedBoxes(BBoxes);

	TimeRunning = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
	// add rect to output Mat
	AddRectToMat(BBoxes, input, output);
	// cleaning
	contours.clear();
}

void SharpenOneImage(Mat &input, Mat &output, double sigma, double threshold, double amount)
{
	Mat blurred;
	GaussianBlur(input, blurred, Size(), sigma, sigma);

	Mat lowContrastMask = abs(input - blurred) < threshold;

	output = input*(1 + amount) + blurred*(-amount);

	input.copyTo(output, lowContrastMask);
}

void RemoveDuplicatedBoxes(vector<Rect> &BBoxes)
{
	vector<Rect> tmpBoxes;
	int k1 = BBoxes.size();
	int k2 = tmpBoxes.size();
	for (int i = 0; i < k1; i++)
	{
		k2 = tmpBoxes.size();
		bool existed = false;
		for (int j = 0; j < k2; j++)
		{
			if (tmpBoxes[j].x == BBoxes[i].x && tmpBoxes[j].y == BBoxes[i].y && tmpBoxes[j].width == BBoxes[i].width && tmpBoxes[j].height == BBoxes[i].height)
			{
				existed = true;
				break;
			}
		}
		if (existed == false)
		{
			tmpBoxes.push_back(BBoxes[i]);
		}
	}
	BBoxes.clear();
	BBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void AddRectToMat(vector<Rect> &BBoxes, Mat &input, Mat &output)
{
	output = input.clone();
	int k = BBoxes.size();
	for (int i = 0; i < k; i++)
	{
		rectangle(output, BBoxes[i], CV_RGB(0, 255, 0), 2);
	}
}