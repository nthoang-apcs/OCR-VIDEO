#include "PartOneProcess.h"




/*					Private					*/

string PartOneProcess::ExtractNameOfFileFromPathIn(string PathIn)
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




/*					Public					*/

/*		Constructor		*/

PartOneProcess::PartOneProcess()
{

}

/*		Destructor		*/

PartOneProcess::~PartOneProcess()
{

}

/*		Practice method		*/

void PartOneProcess::ConvertandResizeOneFile(string pathIn, string pathOut)
{
	Mat image = imread(pathIn);
	Size size(100, 100);
	Mat dst;
	int w = image.cols;
	int h = image.rows;
	if (w > 1300)
	{
		float propor = (float)w / (float)h;
		w = 1280;
		h = (int)((float)w / propor);
	}
	if (h > 1300)
	{
		float propor = (float)w / (float)h;
		h = 1280;
		w = (int)((float)h * propor);
	}
	size.width = w;
	size.height = h;
	resize(image, dst, size);
	imwrite(pathOut, dst);
}

void PartOneProcess::ConvertandResizeMultipleFiles(vector<string> fileList, string resultFolder)
{
	int k = fileList.size();
	cout << "Total index: " << k << endl;
	for (int i = 0; i < k; i++)
	{
		cout << "Run index: " << i << endl;
		string name = ExtractNameOfFileFromPathIn(fileList[i]);
		string pathOut = resultFolder + name + ".jpg";
		ConvertandResizeOneFile(fileList[i], pathOut);
		name.clear();
		pathOut.clear();
	}
	return;
}

void PartOneProcess::SharpenOneImage(string pathIn, string pathOut)
{
	// changing values of sigma, threshold, amount will give different results
	// sharpen image using "unsharp mask" algorithm
	Mat blurred; double sigma = 1, threshold = 5, amount = 1;
	Mat img = imread(pathIn);
	GaussianBlur(img, blurred, Size(), sigma, sigma);
	Mat lowContrastMask = abs(img - blurred) < threshold;
	Mat sharpened = img*(1 + amount) + blurred*(-amount);
	img.copyTo(sharpened, lowContrastMask);
	imwrite(pathOut, sharpened);
}

void PartOneProcess::MSEROneImage(string pathIn, string pathOut)
{

	Mat inImg = imread(pathIn);
	// convert gray from inImg to textImg
	Mat textImg;
	cvtColor(inImg, textImg, CV_BGR2GRAY);
	//Extract MSER
	vector< vector< Point> > contours;
	vector< Rect> bboxes;
	Ptr< MSER> mser = MSER::create(21, (int)(0.00002*textImg.cols*textImg.rows), (int)(0.05*textImg.cols*textImg.rows), 1, 0.7);
	mser->detectRegions(textImg, contours, bboxes);

	for (int i = 0; i < bboxes.size(); i++)
	{
		rectangle(inImg, bboxes[i], CV_RGB(0, 255, 0));
	}
	imwrite(pathOut, inImg);
	contours.clear();
	bboxes.clear();
}

void PartOneProcess::MSERCropboxesOneImage(string pathIn, string cropboxesFolder)
{
	time_t begin, end;
	time(&begin);
	Mat inImg = imread(pathIn);
	// convert gray from inImg to textImg
	Mat textImg;
	cvtColor(inImg, textImg, CV_BGR2GRAY);
	//Extract MSER
	vector< vector< Point> > contours;
	vector< Rect> bboxes;
	Ptr< MSER> mser = MSER::create(21, (int)(0.00002*textImg.cols*textImg.rows), (int)(0.05*textImg.cols*textImg.rows), 1, 0.7);
	mser->detectRegions(textImg, contours, bboxes);
	// crop bboxes
	string name = ExtractNameOfFileFromPathIn(pathIn);
	int k = bboxes.size();
	for (int i = 0; i < k; i++)
	{
		cv::Rect myROI(bboxes[i].x, bboxes[i].y, bboxes[i].width, bboxes[i].height);
		if (myROI.x >= 0 && myROI.y >= 0 && (myROI.width + myROI.x) < inImg.cols && (myROI.height + myROI.y) < inImg.rows)
		{
			// your code
			cv::Mat croppedImage = inImg(myROI);
			imwrite(cropboxesFolder + name + "-" + to_string(i) + ".jpg", croppedImage);
		}
	}
	time(&end);
	double timerun = difftime(end, begin);
	fstream myFile(cropboxesFolder + "Time-Running-Calculation.txt", fstream::out | fstream::app);
	if (myFile.is_open())
	{
		myFile << "RUNNING_FILE: " << pathIn << "\n";
		myFile << "MSER_TIME_RUNNING: " << to_string(timerun) << " seconds.\n";
		myFile.close();
	}
	contours.clear();
	bboxes.clear();
}

void PartOneProcess::MSERCropboxesImages(string fileListPath, string cropboxesFolder)
{
	// init all path in from file list
	cout << "Init all pathIn." << endl;
	vector<string> PathIn;
	ifstream ifs;
	ifs.open(fileListPath);
	if (ifs.is_open())
	{
		string line;
		while (getline(ifs, line))
		{
			PathIn.push_back(line);
		}
		ifs.close();
		line.clear();
	}
	else
	{
		cout << "Cant open the file: " << fileListPath << endl;
	}
	int k = PathIn.size();
	// run mser
	time_t begin, end;
	time(&begin);
	cout << "Run 100 indexes." << endl;
	for (int i = 0; i < k; i++)
	{
		cout << "Run index: " << i << endl;
		MSERCropboxesOneImage(PathIn[i], cropboxesFolder);
	}
	time(&end);
	double totaltime = difftime(end, begin);
	double average = totaltime / 100;
	fstream myFile(cropboxesFolder + "Time-Running-Calculation.txt", fstream::out | fstream::app);
	if (myFile.is_open())
	{
		myFile << "TOTAL_TIME_RUNNING: " << to_string(totaltime) << " seconds.\n";
		myFile << "AVERAGE_TIME_RUNNING: " << to_string(average) << " seconds.\n";
		myFile.close();
	}
	PathIn.clear();
}



/*		Process method		*/

void PartOneProcess::doProcessOneImage(string PathIn, string resultFolder)
{

}

void PartOneProcess::SharpenOneImage(Mat &Input, Mat &Output)
{

}

void PartOneProcess::MSEROneImage(Mat &Input, Mat &Output)
{

}
