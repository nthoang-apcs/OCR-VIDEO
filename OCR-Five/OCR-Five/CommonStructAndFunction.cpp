#include "CommonStructAndFunction.h"

const double PI = 3.14159265;

const Scalar BLUE(255, 0, 0);
const Scalar GREEN(0, 255, 0);
const Scalar RED(0, 0, 255);


void SharpenOneImage(Mat &input, Mat &output, double sigma, double threshold, double amount)
{
	Mat blurred;
	GaussianBlur(input, blurred, Size(), sigma, sigma);

	Mat lowContrastMask = abs(input - blurred) < threshold;

	output = input*(1 + amount) + blurred*(-amount);

	input.copyTo(output, lowContrastMask);
}

void MSEROneImage(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning)
{
	clock_t start = clock();
	// convert gray from src to textImg
	Mat textImg;
	cvtColor(input, textImg, CV_BGR2GRAY);
	// sharpen
	SharpenOneImage(textImg, input);
	// run mser
	vector<vector<Point>> contours;
	Ptr<MSER> mser = MSER::create(
		21,
		(int)(0.00002*input.cols*input.rows),
		(int)(0.05*input.cols*input.rows),
		1, 0.7
	);
	mser->detectRegions(input, contours, BBoxes);
	TimeRunning = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
	output = input.clone();
	for (int i = 0; i < BBoxes.size(); i++)
	{
		rectangle(output, BBoxes[i], CV_RGB(0, 255, 0));
	}
	contours.clear();
}

void RemoveSingleBoxes(vector<Rect> &BBoxes)
{
	int k1 = BBoxes.size();
	if (k1 == 0 || k1 == 1)
		return;
	vector<Rect> tmpBoxes;
	// sort list area
	for (int i = 0; i < (k1 - 1); i++)
	{
		bool checked = false;
		for (int j = 0; j < (k1 - i - 1); j++)
		{
			if ((BBoxes[j].width * BBoxes[j].height) >(BBoxes[j + 1].width * BBoxes[j + 1].height))
			{
				// swap
				int x = BBoxes[j].x;
				int y = BBoxes[j].y;
				int w = BBoxes[j].width;
				int h = BBoxes[j].height;
				BBoxes[j].x = BBoxes[j + 1].x;
				BBoxes[j].y = BBoxes[j + 1].y;
				BBoxes[j].width = BBoxes[j + 1].width;
				BBoxes[j].height = BBoxes[j + 1].height;
				BBoxes[j + 1].x = x;
				BBoxes[j + 1].y = y;
				BBoxes[j + 1].width = w;
				BBoxes[j + 1].height = h;
			}
		}
		if (checked == false)
		{
			break;
		}
	}
	// start to check the condition: 
	// if a single box areas[i] stands alone and have areas / areas[i - 1] > 5 && have areas / areas[i + 1] > 5
	// => not add to tmpboxes
	int i = 0;
	while (i < k1)
	{
		if (i == 0)
		{
			if ((BBoxes[i].area() / BBoxes[i + 1].area()) >= 5 || (BBoxes[i + 1].area() / BBoxes[i].area()) >= 5)
			{

			}
			else
			{
				tmpBoxes.push_back(BBoxes[i]);
			}
			i++;
			continue;
		}
		else if (i == (k1 - 1))
		{
			if ((BBoxes[i].area() / BBoxes[i - 1].area()) >= 5 || (BBoxes[i - 1].area() / BBoxes[i].area()) >= 5)
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
			if ((BBoxes[i].area() / BBoxes[i + 1].area()) >= 5 || (BBoxes[i + 1].area() / BBoxes[i].area()) >= 5)
			{
				if ((BBoxes[i].area() / BBoxes[i - 1].area()) >= 5 || (BBoxes[i - 1].area() / BBoxes[i].area()) >= 5)
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
}

void MergeInsideBoxes(vector<Rect> &BBoxes)
{
	// sort Y
	SortYCoordinate(BBoxes);
	// check boxes inside
	vector<Rect> tmpBoxes;
	vector<int> IgnoreBoxes;
	int k1 = BBoxes.size();
	for (int i = 0; i < (k1 - 1); i++)
	{
		bool checkedignore = false;
		int k2 = IgnoreBoxes.size();
		for (int j = 0; j < k2; j++)
		{
			if (IgnoreBoxes[j] == i)
			{
				checkedignore = true;
				break;
			}
		}
		if (checkedignore == true)
			continue;
		for (int j = i + 1; j < k1; j++)
		{
			checkedignore = false;
			//int k2 = IgnoreBoxes.size();
			// not change k2 because it is not necessary
			for (int j1 = 0; j1 < k2; j1++)
			{
				if (IgnoreBoxes[j1] == i)
				{
					checkedignore = true;
					break;
				}
			}
			if (checkedignore == true)
				continue;
			else
			{
				// check if inside
				if (IsB1insideB2(BBoxes[j], BBoxes[i]))
				{
					IgnoreBoxes.push_back(j);
				}
			}
		}
	}
	// new BBoxes
	int k2 = IgnoreBoxes.size();
	for (int i = 0; i < k1; i++)
	{
		bool checkedIgnore = false;
		for (int j = 0; j < k2; j++)
		{
			if (IgnoreBoxes[j] == i)
			{
				checkedIgnore = true;
				break;
			}
		}
		if (checkedIgnore == false)
		{
			tmpBoxes.push_back(BBoxes[i]);
		}
	}
	BBoxes.clear();
	BBoxes = tmpBoxes;
	tmpBoxes.clear();
}

void CheckStrokeWidthVariation(vector<Rect> &BBoxes)
{

}





void SortYCoordinate(vector<Rect> &BBoxes)
{
	bool checked = false;
	int k1 = BBoxes.size();
	int k2 = k1 - 1;
	for (int i = 0; i < k2; i++)
	{
		checked = false;
		k2 = k1 - 1 - i;
		for (int j = 0; j < k2; j++)
		{
			if (BBoxes[j].y > BBoxes[j + 1].y)
			{
				checked = true;
				int x = BBoxes[j].x;
				int y = BBoxes[j].y;
				int w = BBoxes[j].width;
				int h = BBoxes[j].height;
				BBoxes[j] = Rect(BBoxes[j + 1].x, BBoxes[j + 1].y, BBoxes[j + 1].width, BBoxes[j + 1].height);
				BBoxes[j + 1] = Rect(x, y, w, h);
			}
		}
		if (checked == false)
		{
			break;
		}
	}
}



bool IsB1insideB2(Rect B1, Rect B2)
{
	if (B1.x >= B2.x && B1.y >= B2.y)
	{
		// B1 is on the right and below side of B2
		// check limit of B1
		if ((B1.x + B1.width) <= (B2.x + B2.width) && (B1.y + B1.height) <= (B2.y + B2.height))
		{
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}

bool Point2dSort(SWTPoint2d const & lhs, SWTPoint2d const & rhs)
{
	return lhs.SWT < rhs.SWT;
}

bool chainSortDist(const Chain &lhs, const Chain &rhs) 
{
	return lhs.dist < rhs.dist;
}

bool chainSortLength(const Chain &lhs, const Chain &rhs) 
{
	return lhs.components.size() > rhs.components.size();
}



string ExtractNameOfFileFromPathIn(string PathIn)
{
	int leng = PathIn.length();
	int pos = leng - 1;
	do
	{
		pos--;
	} while (PathIn[pos] != '.' && pos > 0);
	// pos at char '.'
	int pos2 = pos - 1;
	do
	{
		pos2--;
	} while (PathIn[pos2] != '\\' && pos2 > 0);
	// pos2 at char '\'
	pos2++;
	// get name
	vector<char> tmp;
	for (int i = pos2; i < pos; i++)
	{
		tmp.push_back(PathIn[i]);
	}
	int tmplen = tmp.size();
	char *tmp2 = new char[tmplen + 1];
	for (int i = 0; i < tmplen; i++)
	{
		tmp2[i] = tmp[i];
	}
	tmp2[tmplen] = '\0';
	tmp.clear();
	return string(tmp2);
}