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
	for (int i = 4; i < k; i++)
	{
		cout << "Run index: " << i << endl;
		doProcessOneImageWithPostProcessing(PathIn[i], resultFolder);
	}
}

