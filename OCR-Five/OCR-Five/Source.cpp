#include <opencv2\text.hpp>
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include <vector>
#include <string>
#include "SupportFunctions.h"
#include "MyMSER.h"
#include "CannyAlgorithms.h"

using namespace std;
using namespace cv;
using namespace cv::text;


//Perform text detection and recognition and evaluate results using edit distance
int main()
{
	// Get Dir and Data
	/*
	char srcVietDir[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentUse\\Vietnamese";
	char srcEngDir[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentUse\\English";
	int srcVietCount = 60;
	int srcEngCount = 350;
	char destResultViet[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultInOCR\\Vietnamese";
	char destResultEng[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultInOCR\\English";
	char errorPath[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultInOCR\\error-log.txt";
	char NM1[] = "D:\\document\\LVTN2016\\OCRThree\\OCRThree\\trained_classifierNM1.xml";
	char NM2[] = "D:\\document\\LVTN2016\\OCRThree\\OCRThree\\trained_classifierNM2.xml";
	char groupboxesVietFilePath[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultInOCR\\Vietnamese-data.txt";
	char groupboxesEngFilePath[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultInOCR\\English-data.txt";

	TDTCNM *a = new TDTCNM(srcVietDir, srcEngDir, srcVietCount, srcEngCount, NM1, NM2, destResultViet, destResultEng, errorPath, groupboxesVietFilePath, groupboxesEngFilePath);
	a->RunForVietNameseSample(false);
	a->RunForEnglishSample(false);
	*/



	// Get statistic
	/*
	char englishDataPath[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultInOCR\\English-data.txt";
	char vietnameseDataPath[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultInOCR\\Vietnamese-data.txt";
	vector<string>List;
	SupportFunctions* spfunc = new SupportFunctions();
	spfunc->ReadDataGroupBoxesFromFileText(List, vietnameseDataPath);
	*/


	/*
	// Mser detection

	Mat inImg = imread("D:\\document\\LVTN2016\\DataOCR\\CurrentUse\\Vietnamese\\31.jpg");
	Mat textImg = inImg;
	
	// convert to grey image
	//cvtColor(inImg, textImg, CV_BGR2GRAY);

	// Extract MSER
	vector< vector< Point> > contours;
	vector< Rect> bboxes;
	Ptr< MSER> mser = MSER::create(21, (int)(0.00002*textImg.cols*textImg.rows), (int)(0.05*textImg.cols*textImg.rows), 1, 0.7);
	
	mser->detectRegions(textImg, contours, bboxes);
	for (int i = 0; i < bboxes.size(); i++)
	{
		rectangle(inImg, bboxes[i], CV_RGB(0, 255, 0));
	}
	
	imshow("Mser Detection", inImg);
	waitKey(0);
	*/

	/*
	char srcVietDir[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentUse\\Vietnamese";
	char srcEngDir[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentUse\\English";
	int srcVietCount = 60;
	int srcEngCount = 350;
	char destResultViet[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultMser\\Vietnamese";
	char destResultEng[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultMser\\English";
	char errorPath[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultMser\\error-log.txt";
	char groupboxesVietFilePath[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultMser\\Vietnamese-data.txt";
	char groupboxesEngFilePath[] = "D:\\document\\LVTN2016\\DataOCR\\CurrentResult-DefaultMser\\English-data.txt";
	MyMSER* b = new MyMSER(srcVietDir, srcEngDir, srcVietCount, srcEngCount, destResultViet, destResultEng, errorPath, groupboxesVietFilePath, groupboxesEngFilePath);
	b->RunOneSample("D:\\document\\LVTN2016\\DataOCR\\CurrentUse\\Vietnamese\\32.jpg", false);
	*/

	CannyAlgorithms *c = new CannyAlgorithms("D:\\document\\LVTN2016\\DataOCR\\ICDA-2003\\SceneTrialTest\\ryoungt_13.08.2002\\dPICT0082.JPG", "TestGaussianFilter-1.jpg");
	Mat x = c->GaussianFilter(3);
	vector<double> Gx, Gy, G;
	c->FindGradient(x, Gx, Gy, G, 3);
	

	waitKey(0);
	
	return 0;
}

