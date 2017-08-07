#include <opencv2\text.hpp>
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include <vector>
#include <string>

using namespace std;
using namespace cv;
using namespace cv::text;



int main(char* argv)
{
	Mat image = imread("E:\\Code\\OCR-Five-Git\\OCR-Five\\OCR-Five\\testcanny-1.jpg");
	imshow("canny", image);

	return 0;
}

