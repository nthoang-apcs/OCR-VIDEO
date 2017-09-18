#include "PartOneProcess.h"




/*					Private					*/



void PartOneProcess::WriteOneImageToFile(string PathIn, string resultFolder, vector<Rect> &BBoxes, double &TimeRunning, int index)
{
	string filename = ExtractNameOfFileFromPathIn(PathIn);
	string pathout = resultFolder + filename + "-" + to_string(index) + ".txt";
	MSERFILESTREAM* one = new MSERFILESTREAM();
	one->WriteOneImageToFile(pathout, PathIn, BBoxes, TimeRunning);
}

void PartOneProcess::WriteOneImageToFile(string PathIn, string resultFolder, vector<vector<Rect>> &LinesText, double &TimeRunning, int index)
{
	string filename = ExtractNameOfFileFromPathIn(PathIn);
	string pathout = resultFolder + filename + "-" + to_string(index) + ".txt";
	MSERFILESTREAM* one = new MSERFILESTREAM();
	one->WriteOneImageToFile(pathout, PathIn, LinesText, TimeRunning);
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


/*					Public					*/

/*		Constructor		*/

PartOneProcess::PartOneProcess()
{

}

/*		Destructor		*/

PartOneProcess::~PartOneProcess()
{

}

/*		Process method		*/

void PartOneProcess::doProcessOneImageWithPostProcessing(string PathIn, string resultFolder)
{
	Mat src = imread(PathIn);
	// dest 1 = sharpen + mser
	// dest 2 = post processing
	Mat dest1, dest2;
	// all bounding boxes need to check in ocr
	vector<Rect> BBoxes;
	// total time run, time running does not include time write image output to have a clear view to result
	double totaltimerun = 0;	
	double timerun = 0;
	
	// mser
	MSEROneImage(src, dest1, BBoxes, timerun);
	totaltimerun += timerun;
	// write sharpen and mser images to file
	string filename = ExtractNameOfFileFromPathIn(PathIn);
	string pathout1 = resultFolder + filename + "-mser-before-postprocessing.png";
	imwrite(pathout1, dest1);
	// write to file bboxes, time run
	WriteOneImageToFile(PathIn, resultFolder, BBoxes, timerun, 0);
	
	// post processing to reduce bboxes
	PostProcessing(src, dest2, BBoxes, timerun);
	totaltimerun += timerun;
	// write mser image to file - after postprocessing
	string pathout2 = resultFolder + filename + "-mser-after-postprocessing.png";
	imwrite(pathout2, dest2);
	// write to file after reduce bboxes and possible Lines of Texts
	WriteOneImageToFile(PathIn, resultFolder, BBoxes, timerun, 1);

	cout << "Total run time: " << totaltimerun << endl;
	// clear memory
	BBoxes.clear();
}


void PartOneProcess::doProcessOneImageWithoutPostProcessing(string PathIn, string resultFolder)
{
	Mat src = imread(PathIn);
	// dest 1 = sharpen + mser
	// dest 2 = post processing
	Mat dest1;
	// all bounding boxes need to check in ocr
	vector<Rect> BBoxes;
	// total time run, time running does not include time write image output to have a clear view to result
	double totaltimerun = 0;
	double timerun = 0;

	// mser
	MSEROneImage(src, dest1, BBoxes, timerun);
	totaltimerun += timerun;
	// write sharpen and mser images to file
	string filename = ExtractNameOfFileFromPathIn(PathIn);
	string pathout1 = resultFolder + filename + "-mser-before-postprocessing.png";
	imwrite(pathout1, dest1);
	// write to file bboxes, time run
	WriteOneImageToFile(PathIn, resultFolder, BBoxes, timerun, 0);
	
	// cleaning
	BBoxes.clear();
}


void PartOneProcess::doProcessImagesWithoutPostProcessing(vector<string> PathIn, string resultFolder)
{
	int k = PathIn.size();
	for (int i = 0; i < k; i++)
	{
		cout << "Run index: " << i << endl;
		doProcessOneImageWithoutPostProcessing(PathIn[i], resultFolder);
	}
}

void PartOneProcess::doProcessImagesWithPostProcessing(vector<string> &PathIn, string resultFolder)
{
	int k = PathIn.size();
	for (int i = 0; i < k; i++)
	{
		cout << "Run index: " << i << endl;
		doProcessOneImageWithPostProcessing(PathIn[i], resultFolder);
	}
}

