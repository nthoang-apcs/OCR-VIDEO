#include <opencv2\text.hpp>
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include <vector>
#include <string>
#include "PartOneProcess.h"

using namespace std;
using namespace cv;


void SharpenImage(string pathIn, string pathOut);

void BlurImage(string pathIn, string pathOut);

int display_caption(char* caption, Mat &src, Mat &dst, int DELAY_CAPTION);

int display_dst(int delay);

int main(char* argv)
{
	PartOneProcess *one = new PartOneProcess();
	one->doProcessOneImageWithPostProcessing("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-training-set\\img26.jpg", "D:\\Document\\Thesis-2017\\Data\\Result-sharpen-postprocessing-image-experiment\\");

	return 0;
}


void SharpenImage(string pathIn, string pathOut)
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

void BlurImage(string pathIn, string pathOut)
{
	// init
	Mat src = imread(pathIn);
	Mat dst = src.clone();
	int DELAY_CAPTION = 1500;
	int DELAY_BLUR = 100;
	int MAX_KERNEL_LENGTH = 31;

	if (display_caption("Gaussian Blur", src, dst, DELAY_CAPTION) != 0)
	{ 
		return; 
	}

	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		GaussianBlur(src, dst, Size(i, i), 0, 0);
		if (display_dst(DELAY_BLUR) != 0) 
		{ 
			return; 
		}
	}
	imwrite(pathOut, dst);

}

int display_caption(char* caption, Mat &src, Mat &dst, int DELAY_CAPTION)
{
	dst = Mat::zeros(src.size(), src.type());
	putText(dst, caption,
		Point(src.cols / 4, src.rows / 2),
		CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));

	//imshow(window_name, dst);
	int c = waitKey(DELAY_CAPTION);
	if (c >= 0) { return -1; }
	return 0;
}

int display_dst(int delay)
{
	//imshow(window_name, dst);
	int c = waitKey(delay);
	if (c >= 0) { return -1; }
	return 0;
}