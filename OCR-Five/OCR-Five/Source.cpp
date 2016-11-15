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
#include "BackGround.h"
#include "GaussianBlur.h"
#include "HarrisDetector.h"

using namespace std;
using namespace cv;
using namespace cv::text;


/*			Note		*/
// all algorithms have named 'My' before their filenames

int main()
{
	
	
	return 0;
}


// default ocr in open cv
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