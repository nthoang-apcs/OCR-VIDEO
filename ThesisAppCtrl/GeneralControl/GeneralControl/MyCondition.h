#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp>

using namespace std;
using namespace cv;


// check if any boxes in the same line - +- 50% height
bool IsB1onsamelineB2(Rect B1, Rect B2);

// check if ratio between height and width >= 6 times
// check if height < 9
bool IsB1Balanced(Rect B1);

// check if the intersection area between B1 and B2 is equal B1.area()
// and the B2.area() / B1.area() <= 3
bool IsB1insideB2(Rect B1, Rect B2);
