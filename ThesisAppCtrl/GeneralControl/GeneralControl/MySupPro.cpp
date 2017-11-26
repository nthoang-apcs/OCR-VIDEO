#include "MySupPro.h"


///////////////////////////////////////
/*
			Support Process
*/
//////////////////////////////////////

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

void BindingID(vector<Rect> &BBoxes, RectDLL &OtherBoxes, int start)
{
	OtherBoxes.BindingArrayWithID(BBoxes, start);
}

void MergeLineText(RectDLL &OtherBoxes, vector<RectDLL> Lines)
{
	int nSize = OtherBoxes.size();
	int	HandleSize = nSize - 2;
	for (int i = 0; i < HandleSize; i++)
	{
		
	}
}

int ComputeHorizontalAngle(RectDLL &OtherBoxes, int firstIndex, int secondIndex)
{
	float a = 0.0;
	float h = 0.0;
	Rect first = OtherBoxes.GetRectAtIndex(firstIndex);
	Rect second = OtherBoxes.GetRectAtIndex(secondIndex);
	int fx = first.x + (first.width / 2);
	int fy = first.y + (first.height / 2);
	int sx = second.x + (second.width / 2);
	int sy = second.y + (second.height / 2);
	a = sy - fy;
	if (a < 0)
	{
		// the degree is < 0
		
	}
	else if (a > 0)
	{
		// the degree is > 0
	}
	else
	{
		// 2 rect centers are on the same line
		return 0;
	}
}

///////////////////////////////////////
/*
				Condition
*/
//////////////////////////////////////

bool IsB1onsamelineB2(Rect B1, Rect B2)
{
	// range from center to upper or lower
	int range1 = B1.height / 4;
	int c1y = B1.y + (B1.height / 2);
	int range2 = B2.height / 4;
	int c2y = B2.y + (B2.height / 2);
	// check
	int dif = abs(c2y - c1y);
	if (dif < (range1 + range2))
	{
		return true;
	}
	return false;
}

bool IsB1Balanced(Rect B1)
{
	if ((B1.width / B1.height) >= 6)
	{
		return false;
	}
	if ((B1.height / B1.width) >= 6)
	{
		return false;
	}
	if (B1.height < 9)
	{
		return false;
	}
	return true;
}

bool IsB1insideB2(Rect B1, Rect B2)
{
	Rect a = B1 & B2;
	if (a.area() == B1.area() && (B2.area() / B1.area()) <= 3)
		return true;
	return false;

}

bool CompareYCoordinate(Rect B1, Rect B2)
{
	return (B1.y < B2.y);
}

bool CompareArea(Rect B1, Rect B2)
{
	return (B1.area() < B2.area());
}

bool CompareXCoordinate(Rect B1, Rect B2)
{
	return (B1.x < B2.x);
}


///////////////////////////////////////
/*
					Sort
*/
//////////////////////////////////////

void SortYCoordinate(vector<Rect> &BBoxes)
{
	sort(BBoxes.begin(), BBoxes.end(), CompareYCoordinate);
}

void SortArea(vector<Rect> &BBoxes)
{
	sort(BBoxes.begin(), BBoxes.end(), CompareArea);
}

void SortXCoordinate(vector<Rect> &BBoxes)
{
	sort(BBoxes.begin(), BBoxes.end(), CompareXCoordinate);
}


///////////////////////////////////////
/*
					MSER
*/
//////////////////////////////////////

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

