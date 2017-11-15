#include "MySupPro.h"



void SharpenOneImage(Mat &input, Mat &output, double sigma, double threshold, double amount)
{
	Mat blurred;
	GaussianBlur(input, blurred, Size(), sigma, sigma);

	Mat lowContrastMask = abs(input - blurred) < threshold;

	output = input*(1 + amount) + blurred*(-amount);

	input.copyTo(output, lowContrastMask);
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

void RemoveUnusualAreaBoxes(vector<Rect> &BBoxes)
{
	int k1 = BBoxes.size();
	if (k1 == 0 || k1 == 1)
		return;
	vector<Rect> tmpBoxes;
	// start to check the condition:
	// if a single box areas[i] stands alone and have areas / areas[i - 1] > 5 && have areas / areas[i + 1] > 5
	// => not add to tmpboxes
	int i = 0;
	while (i < k1)
	{
		if (i == 0)
		{
			// limit
			if ((BBoxes[i].area() / BBoxes[i + 1].area()) >= 5 || (BBoxes[i + 1].area() / BBoxes[i].area()) >= 5 || BBoxes[i].area() < 30)
			{

			}
			// get result
			else
			{
				tmpBoxes.push_back(BBoxes[i]);
			}
			i++;
			continue;
		}
		else if (i == (k1 - 1))
		{
			if ((BBoxes[i].area() / BBoxes[i - 1].area()) >= 5 || (BBoxes[i - 1].area() / BBoxes[i].area()) >= 5 || BBoxes[i].area() < 30)
			{

			}
			else
			{
				tmpBoxes.push_back(BBoxes[i]);
			}
			i++;
			continue;
		}
		else
		{
			if ((BBoxes[i].area() / BBoxes[i + 1].area()) >= 5 || (BBoxes[i + 1].area() / BBoxes[i].area()) >= 5 || BBoxes[i].area() < 30)
			{
				if ((BBoxes[i].area() / BBoxes[i - 1].area()) >= 5 || (BBoxes[i - 1].area() / BBoxes[i].area()) >= 5 || BBoxes[i].area() < 30)
				{

				}
				else
				{
					tmpBoxes.push_back(BBoxes[i]);
				}
			}
			else
			{
				tmpBoxes.push_back(BBoxes[i]);
			}
			i++;
			continue;
		}
	}
	BBoxes.clear();
	BBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void RemoveSingleBoxTextLine(vector<Rect> &BBoxes)
{
	int k = BBoxes.size();
	vector<Rect> tmpBoxes;
	for (int i = 0; i < k; i++)
	{
		bool checked = false;
		for (int j = 0; j < k; j++)
		{
			if (i == j)
				continue;
			if (IsB1onsamelineB2(BBoxes[i], BBoxes[j]))
			{
				checked = true;
				break;
			}
		}
		if (checked == true)
			tmpBoxes.push_back(BBoxes[i]);
	}
	BBoxes.clear();
	BBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void RemoveUnbalancedRatio(vector<Rect> &BBoxes)
{
	vector<Rect> tmpBoxes;
	int k = BBoxes.size();
	for (int i = 0; i < k; i++)
	{
		if (IsB1Balanced(BBoxes[i]) == true)
		{
			tmpBoxes.push_back(BBoxes[i]);
		}
	}
	BBoxes.clear();
	BBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void MergeInsideBoxes(vector<Rect> &BBoxes)
{
	vector<Rect> tmpBoxes;
	int k = BBoxes.size();
	// check boxes inside
	for (int i = 0; i < k; i++)
	{
		bool checked = false;
		for (int j = 0; j < k; j++)
		{
			if (i == j)
				continue;
			if (IsB1insideB2(BBoxes[i], BBoxes[j]))
			{
				checked = true;
				break;
			}
		}
		if (checked == false)
		{
			tmpBoxes.push_back(BBoxes[i]);
		}
	}
	// cleaning
	BBoxes.clear();
	BBoxes = tmpBoxes;
	tmpBoxes.clear();
	// check too big box
	k = BBoxes.size();
	for (int i = 0; i < k; i++)
	{
		int count = 0;
		for (int j = 0; j < k; j++)
		{
			if (i == j)
				continue;
			Rect A = BBoxes[i] & BBoxes[j];
			if (A.area() == BBoxes[j].area())
				count++;
		}
		if (count < 7)
		{
			tmpBoxes.push_back(BBoxes[i]);
		}
	}
	// cleaning
	BBoxes.clear();
	BBoxes = tmpBoxes;
	tmpBoxes.clear();
}
