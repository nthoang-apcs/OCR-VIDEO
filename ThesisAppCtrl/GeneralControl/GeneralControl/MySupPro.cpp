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

void MergeLineText(RectDLL &OtherBoxes, vector<RectDLL> &Lines)
{
	int nSize = OtherBoxes.size();
	int	HandleSize = nSize - 2;
	for (int i = 0; i < HandleSize; i++)
	{
		int count = Lines.size();
		// check overlapped
		if(HandleOverlappedLineText(OtherBoxes, i, Lines) == false)
		{
			// check horizontal
		}
		else
		{
			// check number of words
			count = Lines.size() - count;
			// remove existed RectNode in new lines and OtherBoxes


			
		}

	}
}

int ComputeHorizontalAngle(RectDLL &OtherBoxes, int firstIndex, int secondIndex)
{
	float a = 0.0;
	float h = 0.0;
	float resultRad = 0.0;
	int resultDeg = 0;
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
		a = -a;
		h =  (fx - sx)*(fx - sx) + (fy - sy)*(fy - sy);
		h = sqrt(h);
		resultRad = - 1 * asin((double)(a/h));
	}
	else if (a > 0)
	{
		// the degree is > 0
		h =  (fx - sx)*(fx - sx) + (fy - sy)*(fy - sy);
		h = sqrt(h);
		resultRad = asin((double)(a/h));
	}
	else
	{
		// 2 rect centers are on the same line
		return 0;
	}
	const double PI = 3.141592653589793;
	// convert resultRad to resultDeg
	resultDeg = (int)(resultRad * 180 / PI);
	return resultDeg;
}

bool HandleOverlappedLineText(RectDLL &OtherBoxes, int index, vector<RectDLL> &Lines)
{

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
				Handle files
*/
//////////////////////////////////////

// convert rect get from mser
// -> output the rect to recognize
Rect ConvertTheoryRectToRealRect(Rect Input)
{
	Rect result;
	if(Input.height < 10 && Input.width < 10)
	{
		// 3 pixels
		result = Rect(Input.x, Input.y, Input.width + 3, Input.height + 3);
	}
	else if(Input.height < 20 && Input.width < 20)
	{
		// 5 pixels
		result = Rect(Input.x, Input.y, Input.width + 5, Input.height + 5);
	}
	else if(Input.height < 100 && Input.width < 100)
	{
		// 10 pixels
		result = Rect(Input.x, Input.y, Input.width + 10, Input.height + 10);
	}
	else
	{
		// 10%
		result = Rect(Input.x, Input.y, Input.width * 1.1, Input.height * 1.1);
	}
	return result;
}

// convert rect from recognition
// -> output rect from mser
Rect ConvertRealRectToTheoryRect(Rect Input)
{
	Rect result;
	if(Input.height < 10 && Input.width < 10)
	{
		// 3 pixels
		result = Rect(Input.x, Input.y, Input.width - 3, Input.height - 3);
	}
	else if(Input.height < 20 && Input.width < 20)
	{
		// 5 pixels
		result = Rect(Input.x, Input.y, Input.width - 5, Input.height - 5);
	}
	else if(Input.height < 100 && Input.width < 100)
	{
		// 10 pixels
		result = Rect(Input.x, Input.y, Input.width - 10, Input.height - 10);
	}
	else
	{
		// 10%
		result = Rect(Input.x, Input.y, Input.width / 1.1, Input.height / 1.1);
	}
	return result;
}

void SaveLines(Mat &mOriginImage, char *CurrentFolder, vector<RectDLL> &Lines)
{
	string folder = string(CurrentFolder) + "\\tmp\\";
	// break into small mat
	int nSize = Lines.size();
	vector<string> ListFiles;
	for (int i = 0; i < nSize; i++)
	{
		Rect tmp1 = Lines[i].MergeAllRectInside();
		Mat tmp2 = mOriginImage(tmp1);
		// write tmp2
		string filename = folder + "line-" + to_string(i) + ".jpg";
		imwrite(filename, tmp2);
		ListFiles.push_back(filename);
		// write info of each file
		ofstream ofs;
		ofs.open(filename + ".txt", ofstream::out);
		if (ofs.is_open())
		{
			ofs << "ID: " << Lines[i].getStringID() << "\n";
			ofs << "Rect member: " << Lines[i].getStringRect() << "\n";
			ofs << "Rect merge: " << Lines[i].getStringRectMerge() << "\n";
			ofs.close();
		}
	}

	// write list filename into tmp folder with name: "files.txt"
	ofstream ofs2;
	ofs2.open(folder + "files.txt", ofstream::out);
	if (ofs2.is_open())
	{
		nSize = ListFiles.size();
		for (int i = 0; i < nSize; i++)
		{
			ofs2 << ListFiles[i] << "\n";
		}
		ofs2.close();
	}
	return;
}

void SaveOtherBoxes(Mat &mOriginImage, char *CurrentFolder, RectDLL &OtherBoxes)
{
	string folder = string(CurrentFolder) + "\\tmp\\";
	// break into small mat
	int nSize = OtherBoxes.size();
	vector<string> ListFiles;
	for (int i = 0; i < nSize; i++)
	{
		Rect tmp1 = OtherBoxes.getRectAtIndex(i);
		Mat tmp2 = mOriginImage(tmp1);
		// write tmp2
		string filename = folder + "OtherBoxes-" + to_string(i) + ".jpg";
		imwrite(filename, tmp2);
		ListFiles.push_back(filename);
		// write info of each file
		ofstream ofs;
		ofs.open(filename + ".txt", ofstream::out);
		if (ofs.is_open())
		{
			ofs << "ID: " << OtherBoxes.getStringIDatIndex(i) << "\n";
			ofs << "Rect: " << OtherBoxes.getStringRectatIndex(i) << "\n";
			ofs.close();
		}
	}

	// write list filename into tmp folder with name: "files.txt"
	ofstream ofs2;
	ofs2.open(folder + "files.txt", ofstream::app);
	if (ofs2.is_open())
	{
		nSize = ListFiles.size();
		for (int i = 0; i < nSize; i++)
		{
			ofs2 << ListFiles[i] << "\n";
		}
		ofs2.close();
	}
	return;
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
