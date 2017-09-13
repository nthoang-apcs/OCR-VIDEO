#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ComputerVision.h"



using namespace cv;
using namespace std;

int main()
{

	Mat image;
	image = imread("img1.jpg");   // Read the file

	textDetection(image, false);

	waitKey(0);

	return 0;
}