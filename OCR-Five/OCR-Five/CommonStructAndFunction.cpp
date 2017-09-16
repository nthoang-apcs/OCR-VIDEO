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
	// sort y coordinate ascending
	SortYCoordinate(BBoxes);
	// remove single box text line
	RemoveSingleBoxTextLine(BBoxes);
	// stroke width
	CheckStrokeWidthVariation(BBoxes);
	// merge inside box
	MergeInsideBoxes(BBoxes);

	TimeRunning = (double)(clock() - start) / (double)CLOCKS_PER_SEC;

	AddRectToMat(BBoxes, input, output);
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


/*		****************************************		*/


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

void RemoveSingleBoxTextLine(vector<Rect> &BBoxes)
{
	
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


/*		****************************************		*/


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

void SortArea(vector<Rect> &BBoxes)
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
			if (BBoxes[j].area() > BBoxes[j + 1].area())
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
