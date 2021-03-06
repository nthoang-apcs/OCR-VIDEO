#include "MSERIO.h"

/*		Private		*/


string MSERFILESTREAM::GetSrcFolder(string line)
{
	int i = 0;
	int k = line.length();
	while (i < k && line[i] != ':')
	{
		i++;
	}
	i++;
	i++;
	// Get folder path
	vector<char> tmp;
	while (i < k)
	{
		tmp.push_back(line[i]);
	}
	k = tmp.size();
	char *tmp2 = new char[k + 1];
	for (i = 0; i < k; i++)
	{
		tmp2[i] = tmp[i];
	}
	tmp2[k] = 0;
	tmp.clear();
	string result(tmp2);
	delete[] tmp2;
	return result;
}

double MSERFILESTREAM::GetAverageTime(string line)
{
	double average = 0.000;
	int i = 0;
	int k = line.length();
	while (i < k && line[i] != ':')
	{
		i++;
	}
	i++;
	i++;
	vector<char> tmp;
	while (i < k && line[i] != ' ')
	{
		tmp.push_back(line[i]);
	}
	k = tmp.size();
	char *tmp2 = new char[k + 1];
	for (i = 0; i < k; i++)
	{
		tmp2[i] = tmp[i];
	}
	tmp2[k] = 0;
	tmp.clear();
	string result(tmp2);
	delete[] tmp2;
	std::string::size_type sz;     // alias of size_t
	average = stod(result, &sz);
	result.clear();
	return average;
}

string MSERFILESTREAM::GetSrcPath(string line)
{
	int i = 0;
	int k = line.length();
	while (i < k && line[i] != ':')
	{
		i++;
	}
	i++;
	i++;
	// get path
	vector<char> tmp;
	while (i < k)
	{
		tmp.push_back(line[i]);
	}
	k = tmp.size();
	char *tmp2 = new char[k + 1];
	for (i = 0; i < k; i++)
	{
		tmp2[i] = tmp[i];
	}
	tmp2[k] = 0;
	tmp.clear();
	string result(tmp2);
	delete[] tmp2;
	return result;
}

double MSERFILESTREAM::GetTimeRunning(string line)
{
	int i = 0;
	int k = line.length();
	while (i < k && line[i] != ':')
	{
		i++;
	}
	i++;
	i++;
	// get time
	double timerun = 0.000;
	vector<char> tmp;
	while (i < k && line[i] != ' ')
	{
		tmp.push_back(line[i]);
	}
	k = tmp.size();
	char *tmp2 = new char[k + 1];
	for (i = 0; i < k; i++)
	{
		tmp2[i] = tmp[i];
	}
	tmp2[k] = 0;
	tmp.clear();
	string result(tmp2);
	delete[] tmp2;
	std::string::size_type sz;     // alias of size_t
	timerun = stod(result, &sz);
	result.clear();
	return timerun;
}

int MSERFILESTREAM::GetTotalBoxes(string line)
{
	int i = 0;
	int k = line.length();
	while (i < k && line[i] != ':')
	{
		i++;
	}
	i++;
	i++;
	// get total boxes
	// get time
	double totalboxes = 0.000;
	vector<char> tmp;
	while (i < k && line[i] != ' ')
	{
		tmp.push_back(line[i]);
	}
	k = tmp.size();
	char *tmp2 = new char[k + 1];
	for (i = 0; i < k; i++)
	{
		tmp2[i] = tmp[i];
	}
	tmp2[k] = 0;
	tmp.clear();
	string result(tmp2);
	delete[] tmp2;
	std::string::size_type sz;     // alias of size_t
	totalboxes = stoi(result, &sz);
	result.clear();
	return totalboxes;
}

vector<Rect> MSERFILESTREAM::GetBoundingBoxes(string line)
{
	int i = 0;
	int k = line.length();
	while (i < k && line[i] != ':')
	{
		i++;
	}
	i++;
	i++;
	// get bounding boxes
	vector<Rect> Result;
	while (i < k)
	{
		// read one by one, each seperate by ' ; '
		int x, y, width, height;
		vector<char> tmp;
		// get x
		while (i < k && line[i] != ' ')
		{
			tmp.push_back(line[i]);
			i++;
		}
		i++;
		int k1 = tmp.size();
		char *tmp2 = new char[k1 + 1];
		for (int j = 0; j < k1; j++)
		{
			tmp2[j] = tmp[j];
		}
		tmp2[k1] = 0;
		tmp.clear();
		x = atoi(tmp2);
		delete[] tmp2;
		// get y
		while (i < k && line[i] != ' ')
		{
			tmp.push_back(line[i]);
			i++;
		}
		i++;
		k1 = tmp.size();
		tmp2 = new char[k1 + 1];
		for (int j = 0; j < k1; j++)
		{
			tmp2[j] = tmp[j];
		}
		tmp2[k1] = 0;
		tmp.clear();
		y = atoi(tmp2);
		delete[] tmp2;
		// get width
		while (i < k && line[i] != ' ')
		{
			tmp.push_back(line[i]);
			i++;
		}
		i++;
		k1 = tmp.size();
		tmp2 = new char[k1 + 1];
		for (int j = 0; j < k1; j++)
		{
			tmp2[j] = tmp[j];
		}
		tmp2[k1] = 0;
		tmp.clear();
		width = atoi(tmp2);
		delete[] tmp2;
		// get height
		while (i < k && line[i] != ' ')
		{
			tmp.push_back(line[i]);
			i++;
		}
		i++;
		k1 = tmp.size();
		tmp2 = new char[k1 + 1];
		for (int j = 0; j < k1; j++)
		{
			tmp2[j] = tmp[j];
		}
		tmp2[k1] = 0;
		tmp.clear();
		height = atoi(tmp2);
		delete[] tmp2;
		// add rect
		Result.push_back(Rect(x, y, width, height));
		// ignore ' ; '
		while (i < k && line[i] == ';')
		{
			i++;
		}
		while (i < k && line[i] == ' ')
		{
			i++;
		}
		// next box
	}
	return Result;
}


/*		Public		*/

MSERFILESTREAM::MSERFILESTREAM()
{
	
}

MSERFILESTREAM::~MSERFILESTREAM()
{

}

void MSERFILESTREAM::WriteImagesToFile(string path, string srcFolder, vector<string> &PathIn, vector<vector<Rect>> &BBoxes, vector<double> &TimeRunning)
{
	// get the average
	double average = 0.00000;
	int countindex = TimeRunning.size();
	for (int i = 0; i < countindex; i++)
	{
		average = average + TimeRunning[i];
	}
	average = average / (double)countindex;

	// write
	ofstream ofs;
	ofs.open(path);
	if (ofs.is_open())
	{
		ofs << "SRC-FOLDER: " << srcFolder << "\n";
		ofs << "AVERAGE-TIME-RUNNING: " << average << " seconds.\n";
		for (int i = 0; i < countindex; i++)
		{
			int k = BBoxes[i].size();
			ofs << "SRC-" << i << ": " << PathIn[i] << "\n";
			ofs << "TIME-RUNNING: " << TimeRunning[i] << " seconds.\n";
			ofs << "TOTAL-BBOXES: " << k << "\n";
			ofs << "BOUNDING-BOXES: ";
			for (int j = 0; j < k; j++)
			{
				ofs << BBoxes[i][j].x << " " << BBoxes[i][j].y << " " << BBoxes[i][j].width << " " << BBoxes[i][j].height << " ; ";
			}
			ofs << "\n";
		}
		ofs.close();
	}
}

void MSERFILESTREAM::WriteOneImageToFile(string pathIn, string srcPath, vector<Rect> &BBoxes, double &TimeRunning)
{
	ofstream ofs;
	ofs.open(pathIn);
	if (ofs.is_open())
	{
		int k = BBoxes.size();
		ofs << "SRC: " << srcPath << "\n";
		ofs << "TIME-RUNNING: " << TimeRunning << " seconds.\n";
		ofs << "TOTAL-BBOXES: " << k << "\n";
		ofs << "BOUNDING-BOXES: ";
		for (int j = 0; j < k; j++)
		{
			ofs << BBoxes[j].x << " " << BBoxes[j].y << " " << BBoxes[j].width << " " << BBoxes[j].height << " ; ";
		}
		ofs << "\n";
		ofs.close();
	}
}

void MSERFILESTREAM::WriteOneImageCropBoxesToFile(string pathIn, string srcPath, int numberboxes, double &TimeRunning)
{
	ofstream ofs;
	ofs.open(pathIn);
	if (ofs.is_open())
	{
		string name = ExtractNameOfFileFromPathIn(srcPath);
		ofs << "SRC: " << srcPath << "\n";
		ofs << "TIME-RUNNING: " << TimeRunning << " seconds.\n";
		ofs << "TOTAL-BBOXES: " << numberboxes << "\n";
		ofs << "BBOXES-PATH:" << "\n";
		for (int i = 0; i < numberboxes; i++)
		{
			ofs << name << "-" << i << ".jpg" << "\n";
		}
		ofs.close();
	}
}

void MSERFILESTREAM::WriteOneImageToFile(string pathIn, string srcPath, vector<vector<Rect>> &LinesText, double &TimeRunning)
{
	ofstream ofs;
	ofs.open(pathIn);
	if (ofs.is_open())
	{
		int k = LinesText.size();
		// get total boxes
		int total = 0;
		for (int i = 0; i < k; i++)
		{
			total += LinesText[i].size();
		}
		// write
		ofs << "SRC: " << srcPath << "\n";
		ofs << "TIME-RUNNING: " << TimeRunning << " seconds.\n";
		ofs << "TOTAL-BBOXES: " << total << "\n";
		ofs << "BOUNDING-BOXES: " << "\n";
		for (int i = 0; i < k; i++)
		{
			ofs << "Lines-" << to_string(i + 1) << ": ";
			int k1 = LinesText[i].size();
			for (int j = 0; j < k1; j++)
			{
				ofs << LinesText[i][j].x << " " << LinesText[i][j].y << " " << LinesText[i][j].width << " " << LinesText[i][j].height << " ; ";
			}
			ofs << "\n";
		}
		ofs << "\n";
		ofs.close();
	}
}

void MSERFILESTREAM::ReadImagesFromFile(string path, string &srcFolder, double &averagetime, vector<string> &PathIn, vector<vector<Rect>> &BBoxes, vector<double> &TimeRunning)
{
	// read
	ifstream ifs;
	ifs.open(path);
	if (ifs.is_open())
	{
		string line;
		// read srcFolder
		getline(ifs, line);
		srcFolder = GetSrcFolder(line);
		// read average time
		getline(ifs, line);
		averagetime = GetAverageTime(line);
		// read each index
		while (getline(ifs, line))
		{
			if (line == "")
			{
				continue;
			}
			// get src
			PathIn.push_back(GetSrcPath(line));
			// read time running
			getline(ifs, line);
			TimeRunning.push_back(GetTimeRunning(line));
			// read total boxes
			getline(ifs, line);
			// not necessary
			//double totalboxes = GetTotalBoxes(line);
			// read bounding boxes
			getline(ifs, line);
			BBoxes.push_back(GetBoundingBoxes(line));
		}
		ifs.close();
	}
}

void MSERFILESTREAM::ReadOneImageFromFile(string pathIn, string &srcPath, vector<Rect> &BBoxes, double &TimeRunning)
{
	// read
	ifstream ifs;
	ifs.open(pathIn);
	if (ifs.is_open())
	{
		string line;
		getline(ifs, line);
		srcPath = GetSrcPath(line);
		// read time running
		getline(ifs, line);
		TimeRunning = GetTimeRunning(line);
		// read total boxes
		getline(ifs, line);
		// not necessary
		//double totalboxes = GetTotalBoxes(line);
		// read bounding boxes
		getline(ifs, line);
		BBoxes = GetBoundingBoxes(line);
		ifs.close();
	}
}

void MSERFILESTREAM::ReadOneImageFromFile(string pathIn, string &srcPath, vector<vector<Rect>> &LinesText, double &TimeRunning)
{

}