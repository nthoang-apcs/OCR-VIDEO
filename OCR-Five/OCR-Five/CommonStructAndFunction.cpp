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

void PostProcessing(Mat &input, Mat &output, vector<Rect> &BBoxes, double &TimeRunning)
{
	clock_t start = clock();
	// sort area ascending
	SortArea(BBoxes);
	// remove areas of stand alone single box
	RemoveUnusualAreaBoxes(BBoxes);
	// remove unbalanced ratio width, height
	RemoveUnbalancedRatio(BBoxes);
	// sort y coordinate ascending
	SortYCoordinate(BBoxes);
	// remove single box text line
	RemoveSingleBoxTextLine(BBoxes);
	// merge inside box
	MergeInsideBoxes(BBoxes);
	// stroke width
	CheckStrokeWidthVariation(BBoxes);
	// remove trash boxes

	// merge on 1 line text box with nearly the same ratio h/w
	MergeNonOverlapTextLineNearRatioBoxes(BBoxes);

	TimeRunning = (double)(clock() - start) / (double)CLOCKS_PER_SEC;

	AddRectToMat(BBoxes, input, output);
}

void CropBoxesInOneImage(Mat &input, vector<Rect> &BBoxes, double &TimeRunning, string resultFolder, string namedefault)
{
	clock_t start = clock();
	int k = BBoxes.size();
	for (int i = 0; i < k; i++)
	{
		cv::Rect myROI(BBoxes[i].x, BBoxes[i].y, BBoxes[i].width, BBoxes[i].height);
		if (myROI.x >= 0 && myROI.y >= 0 && (myROI.width + myROI.x) < input.cols && (myROI.height + myROI.y) < input.rows)
		{
			// your code
			cv::Mat croppedImage = input(myROI);
			imwrite(resultFolder + namedefault + "-" + to_string(i) + ".jpg", croppedImage);
		}
	}
	TimeRunning = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
}


/*		****************************************		*/


void GetListName(vector<string> &Paths, string resultFolder, string filename)
{
	int k = Paths.size();
	ofstream ofs;
	ofs.open(resultFolder + filename);
	if (ofs.is_open())
	{
		for (int i = 0; i < k; i++)
		{
			// extract name
			string name = ExtractNameOfFileFromPathIn(Paths[i]);
			ofs << name << "\n";
		}
		ofs.close();
	}
}

void GetListName(vector<string> &Paths, vector<string> &ListName)
{
	int k = Paths.size();
	for (int i = 0; i < k; i++)
	{
		// extract name
		string name = ExtractNameOfFileFromPathIn(Paths[i]);
		ListName.push_back(name);
	}
}

void GetListTotalBoxes(vector<string> &Paths, string resultFolder, string filename)
{
	int k = Paths.size();
	vector<int> bboxes;
	for (int i = 0; i < k; i++)
	{
		// extract name
		string name = ExtractNameOfFileFromPathIn(Paths[i]);
		// access result file
		string tmpPath = resultFolder + name + "-0.txt";
		string totalBoxes;
		ifstream ifs;
		ifs.open(tmpPath);
		if (ifs.is_open())
		{
			getline(ifs, totalBoxes);	// ignore src
			getline(ifs, totalBoxes);	// ignore time
			getline(ifs, totalBoxes);	// get boxes
			ifs.close();
		}
		int boxes = ExtractTotalBoxesFromString(totalBoxes);
		bboxes.push_back(boxes);
	}
	k = bboxes.size();
	ofstream ofs;
	ofs.open(resultFolder + filename);
	if (ofs.is_open())
	{
		for (int i = 0; i < k; i++)
		{
			ofs << bboxes[i] << "\n";
		}
		ofs.close();
	}
}

void GetListTotalBoxes(vector<string> &Paths, string resultFolder, vector<int> &ListBoxes, int index)
{
	int k = Paths.size();
	for (int i = 0; i < k; i++)
	{
		// extract name
		string name = ExtractNameOfFileFromPathIn(Paths[i]);
		// access result file
		string tmpPath = resultFolder + name + "-" + to_string(index) + ".txt";
		string totalBoxes;
		ifstream ifs;
		ifs.open(tmpPath);
		if (ifs.is_open())
		{
			getline(ifs, totalBoxes);	// ignore src
			getline(ifs, totalBoxes);	// ignore time
			getline(ifs, totalBoxes);	// get boxes
			ifs.close();
		}
		int boxes = ExtractTotalBoxesFromString(totalBoxes);
		ListBoxes.push_back(boxes);
	}
}

void GetListRunTime(vector<string> &Paths, string resultFolder, string filename)
{
	int k = Paths.size();
	vector<double> timerun;
	for (int i = 0; i < k; i++)
	{
		// extract name
		string name = ExtractNameOfFileFromPathIn(Paths[i]);
		// access result file
		string tmpPath = resultFolder + name + "-0.txt";
		string totalBoxes;
		ifstream ifs;
		ifs.open(tmpPath);
		if (ifs.is_open())
		{
			getline(ifs, totalBoxes);	// ignore src
			getline(ifs, totalBoxes);	// get time
			ifs.close();
		}
		double time = ExtractTimeRunningFromString(totalBoxes);
		timerun.push_back(time);
	}
	k = timerun.size();
	ofstream ofs;
	ofs.open(resultFolder + filename);
	if (ofs.is_open())
	{
		for (int i = 0; i < k; i++)
		{
			ofs << timerun[i] << "\n";
		}
		ofs.close();
	}
}

void GetListRunTime(vector<string> &Paths, string resultFolder, vector<double> &runTime, int index)
{
	int k = Paths.size();
	for (int i = 0; i < k; i++)
	{
		// extract name
		string name = ExtractNameOfFileFromPathIn(Paths[i]);
		// access result file
		string tmpPath = resultFolder + name + "-" + to_string(index) + ".txt";
		string totalBoxes;
		ifstream ifs;
		ifs.open(tmpPath);
		if (ifs.is_open())
		{
			getline(ifs, totalBoxes);	// ignore src
			getline(ifs, totalBoxes);	// get time
			ifs.close();
		}
		double time = ExtractTimeRunningFromString(totalBoxes);
		runTime.push_back(time);
	}
}

void SetListName(int start, int end, string filepath)
{
	ofstream ofs;
	ofs.open(filepath);
	if (ofs.is_open())
	{
		for (int i = start; i < end; i++)
		{
			ofs << "img" << i << ".jpg" << "\n";
		}

		ofs.close();
	}
}

void SetListPath(int start, int end, string filepath, string fileformat)
{
	ofstream ofs;
	ofs.open(filepath);
	if (ofs.is_open())
	{
		for (int i = start; i < end; i++)
		{
			ofs << fileformat << "img" << i << ".jpg" << "\n";
		}

		ofs.close();
	}
}

void GetListBoxesInOneImage(vector<Rect> &BBoxes, string filepath)
{
	ifstream ifs;
	ifs.open(filepath);
	if (ifs.is_open())
	{
		string line;
		// ignore first 3 information
		getline(ifs, line);
		getline(ifs, line);
		getline(ifs, line);
		// get bounding boxes
		getline(ifs, line);
		int k = line.length();
		int i = 0;
		// ignore text + ':'
		while (i < k && line[i] != ':')
		{
			i++;
		}
		i++;
		// ignore space
		while (i < k && line[i] == ' ')
		{
			i++;
		}
		// start to read rect
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
		while (i < k)
		{
			// exception
			if (line[i] < '0' && line[i] > '9')
			{
				break;
			}
			x = 0;
			y = 0;
			w = 0;
			h = 0;
			// read x
			while (line[i] != ' ')
			{
				x = x * 10 + (int)(line[i] - '0');
				i++;
			}
			i++;
			// read y
			while (line[i] != ' ')
			{
				y = y * 10 + (int)(line[i] - '0');
				i++;
			}
			i++;
			// read w
			while (line[i] != ' ')
			{
				w = w * 10 + (int)(line[i] - '0');
				i++;
			}
			i++;
			// read h
			while (line[i] != ' ')
			{
				h = h * 10 + (int)(line[i] - '0');
				i++;
			}
			i++;
			BBoxes.push_back(Rect(x, y, w, h));
			// ignore seperated string '; '
			i += 2;
		}

		ifs.close();
	}
}

void GetListCentroids(vector<Rect> &BBoxes, vector<Point> &Centroids)
{
	int k = BBoxes.size();
	for (int i = 0; i < k; i++)
	{
		int x = BBoxes[i].x + (BBoxes[i].width / 2);
		int y = BBoxes[i].y + (BBoxes[i].height / 2);
		Centroids.push_back(Point(x, y));
	}
}



/*		****************************************		*/


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
}

void RemoveTrashBoxes(vector<Rect> &BBoxes)
{

}

void MergeNonOverlapTextLineNearRatioBoxes(vector<Rect> &BBoxes)
{
	int k = BBoxes.size();
	vector<Rect> tmpBoxes;
	for (int i = 0; i < k; i++)
	{
		// check if BBoxes[i] already exists
		int k1 = tmpBoxes.size();
		bool checked = false;
		for (int m = 0; m < k1; m++)
		{
			Rect x = BBoxes[i] & tmpBoxes[m];
			if (x.area() == BBoxes[i].area())
			{
				checked = true;
				break;
			}
		}
		if (checked == true)
		{
			continue;
		}

		TreeNode tmp;
		tmp.ID = i;
		int cenx = BBoxes[i].x + (BBoxes[i].width / 2);
		int ceny = BBoxes[i].y + (BBoxes[i].height / 2);
		for (int j = i + 1; j < k; j++)
		{
			int cen1x = BBoxes[j].x + (BBoxes[j].width / 2);
			int cen1y = BBoxes[j].y + (BBoxes[j].height / 2);
			if (ceny == cen1y && CheckRatioBox(BBoxes[i], BBoxes[j]) == true)
			{
				tmp.ListOverlap.push_back(j);
			}
			else if (ceny < cen1y)
			{
				if ((ceny + (BBoxes[i].height / 5)) >= cen1y && CheckRatioBox(BBoxes[i], BBoxes[j]) == true)
				{
					tmp.ListOverlap.push_back(j);
				}
			}
			else if (ceny > cen1y)
			{
				if ((ceny - (BBoxes[i].height / 5)) >= cen1y && CheckRatioBox(BBoxes[i], BBoxes[j]) == true)
				{
					tmp.ListOverlap.push_back(j);
				}
			}
		}
		if (tmp.ListOverlap.size() == 0)
		{
			tmpBoxes.push_back(BBoxes[i]);
		}
		else
		{
			// build a rect from i and list overlap
			vector<Rect> tmp2;
			tmp2.push_back(BBoxes[tmp.ID]);
			int k2 = tmp.ListOverlap.size();
			for (int l = 0; l < k2; l++)
			{
				tmp2.push_back(BBoxes[tmp.ListOverlap[l]]);
			}
			Rect ne;
			for (int l = 0; l < k2; l++)
			{
				if (l == 0)
				{
					ne = tmp2[l];
				}
				else
				{
					ne = ne | tmp2[l];
				}
			}
			tmpBoxes.push_back(ne);
			tmp2.clear();
			tmp.ListOverlap.clear();
		}
	}
	BBoxes.clear();
	BBoxes = tmpBoxes;
	tmpBoxes.clear();
}

// not finish
void CheckStrokeWidthVariation(vector<Rect> &BBoxes)
{

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

void AddListPath(vector<string> &Paths, string filepath)
{
	ifstream ifs;
	ifs.open(filepath);
	if (ifs.is_open())
	{
		string line;
		while (getline(ifs, line))
		{
			Paths.push_back(line);
		}

		ifs.close();
	}
}

void writeBBoxesToFile(vector<Rect> &BBoxes, string filename)
{
	ofstream ofs;
	ofs.open(filename);
	if (ofs.is_open())
	{
		int k = BBoxes.size();
		for (int i = 0; i < k; i++)
		{
			ofs << "Rect: " << BBoxes[i].x << ", " << BBoxes[i].y << ", " << BBoxes[i].width << ", " << BBoxes[i].height;
			ofs << "\n";
		}

		ofs.close();
	}
}

void writeTotalBoxesToFile(vector<int> &TotalBoxes, string filename)
{
	ofstream ofs;
	ofs.open(filename);
	if (ofs.is_open())
	{
		int k = TotalBoxes.size();
		for (int i = 0; i < k; i++)
		{
			ofs << TotalBoxes[i];
			ofs << "\n";
		}

		ofs.close();
	}
}

void writeRunTimeToFile(vector<double> &RunTime, string filename)
{
	ofstream ofs;
	ofs.open(filename);
	if (ofs.is_open())
	{
		int k = RunTime.size();
		for (int i = 0; i < k; i++)
		{
			ofs << RunTime[i];
			ofs << "\n";
		}

		ofs.close();
	}
}

Line GetLineEquation(Point A, Point B)
{
	if (A.x == B.x)
	{
		return Line(1, 0, A.x);
	}
	else if (A.y == B.y)
	{
		return Line(0, 1, A.y);
	}
	else
	{
		float a = (B.y - A.y) / (B.x - A.x);
		float b = -1;
		float c = B.y - (a*B.x);
		return Line(a, b, c);
	}
}




/*		****************************************		*/


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



/*		****************************************		*/


bool IsB1insideB2(Rect B1, Rect B2)
{
	if (B1.x >= B2.x && B1.y >= B2.y)
	{
		// B1 is on the right and below side of B2
		// check limit of B1
		if ((B1.x + B1.width) <= (B2.x + B2.width) && (B1.y + B1.height) <= (B2.y + B2.height) && (B1.area() * 3) >= B2.area())
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
	return true;
}

bool IsB1OverlappingB2(Rect B1, Rect B2)
{
	return ((B1 & B2).area() > 0);
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

int CheckConditionOfOverlappingBoxLine(Rect B1, Rect B2)
{
	// check overlapping
	Rect B3 = (B1 & B2);
	if (B3.area() > 0)
	{
		// have area in common
		if (B3.area() == B2.area())
		{
			return -2;
		}
		else if (B3.area() == B1.area())
		{
			return -3;
		}
		// get 2 centroids
		Point c1;
		c1.x = B1.x + (B1.width / 2);
		c1.y = B1.y + (B1.height / 2);
		Point c2;
		c2.x = B2.x + (B2.width / 2);
		c2.y = B2.y + (B2.height / 2);

		if (c1.y == c2.y)
		{
			if (c1.x < c2.x)
			{
				return 0;
			}
			else
			{
				return 180;
			}
		}
		if (c1.x == c2.x)
		{
			if (c1.y < c2.y)
			{
				return 270;
			}
			else
			{
				return 90;
			}
		}
		if (c1.x < c2.x)
		{
			if (c1.y < c2.y)
			{
				// 271-359 degree
				double tmp = (c2.y - c1.y) / (c2.x - c1.x);
				double tmp2 = atan(tmp);
				double result = 360 - (tmp2 * 180 / PI);
				return result;
			}
			else
			{
				// 0-89 degree
				double tmp = (c2.y - c1.y) / (c2.x - c1.x);
				double tmp2 = atan(tmp);
				double result = tmp2 * 180 / PI;
				return result;
			}
		}
		else
		{
			if (c1.y < c2.y)
			{
				// 181-269 degree
				double tmp = (c2.y - c1.y) / (c2.x - c1.x);
				double tmp2 = atan(tmp);
				double result = 180 + tmp2 * 180 / PI;
				return result;
			}
			else
			{
				// 91-179 degree
				double tmp = (c2.y - c1.y) / (c2.x - c1.x);
				double tmp2 = atan(tmp);
				double result = 90 + tmp2 * 180 / PI;
				return result;
			}
		}

	}


	return -1;
}

bool CheckRatioBox(Rect B1, Rect B2)
{
	if (B1.area() < B2.area() && (B2.area() / B1.area()) < 2.5)
	{
		// check w / h for 2 rect
		double tmp1 = B1.height / B1.width;
		double tmp2 = B2.height / B2.width;
		if (abs(tmp2 - tmp1) < 1.2)
		{
			return true;
		}
		return false;
	}
	else if (B2.area() <= B1.area() && (B1.area() / B2.area()) < 2.5)
	{
		// check w / h for 2 rect
		double tmp1 = B1.height / B1.width;
		double tmp2 = B2.height / B2.width;
		if (abs(tmp2 - tmp1) < 1.2)
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

bool CheckLineEquation(Line A, Point B, int threshold)
{


	return true;
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



/*		****************************************		*/



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

int ExtractTotalBoxesFromString(string line)
{
	int result = 0;
	int k = line.length();
	// ignore text
	int i = 0;
	while (i < k && line[i] != ':')
	{
		i++;
	}
	i++;
	// ignore space
	while (i < k && line[i] == ' ')
	{
		i++;
	}
	// get number
	while (i < k && line[i] >= '0' && line[i] <= '9')
	{
		result = result * 10 + (int)(line[i] - '0');
		i++;
	}
	return result;
}

double ExtractTimeRunningFromString(string line)
{
	double result = 0;
	int k = line.length();
	// ignore text
	int i = 0;
	while (i < k && line[i] != ':')
	{
		i++;
	}
	i++;
	// ignore space
	while (i < k && line[i] == ' ')
	{
		i++;
	}
	// get number before '.'
	while (i < k && line[i] >= '0' && line[i] <= '9')
	{
		result = result * 10 + (int)(line[i] - '0');
		i++;
	}
	if (line[i] != '.')
		return result;
	i++;
	int count =	10;
	// get number after '.'
	while (i < k && line[i] >= '0' && line[i] <= '9')
	{
		result = result + (((double)(line[i] - '0')) / (double)count);
		count = count * 10;
		i++;
	}
	return result;
}
