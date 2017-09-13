#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

class MyCannyEdge {
private:
	Mat img; //Original Image
	Mat grayscaled; // Grayscale
	Mat gFiltered; // Gradient
	Mat sFiltered; //Sobel Filtered
	Mat angles; //Angle Map
	Mat non; // Non-maxima supp.
	Mat thres; //Double threshold and final
public:

	MyCannyEdge(String); //Constructor

	Mat toGrayScale(Mat img);
	vector<vector<double>> createFilter(int, int, double); //Creates a gaussian filter
	Mat useFilter(Mat, vector<vector<double>>); //Use some filter
	Mat sobel(); //Sobel filtering
	Mat nonMaxSupp(); //Non-maxima supp.
	Mat threshold(Mat, int, int); //Double threshold and finalize picture

};