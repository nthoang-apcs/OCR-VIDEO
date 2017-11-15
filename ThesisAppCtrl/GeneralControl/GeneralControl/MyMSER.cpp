#include "MyMSER.h"


void MSEROneImage(Mat &input, vector<Rect> &BBoxes, double &TimeRunning)
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
	// cleaning
	contours.clear();
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
