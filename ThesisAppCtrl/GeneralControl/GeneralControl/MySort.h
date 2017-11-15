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



void SortYCoordinate(vector<Rect> &BBoxes);

void SortArea(vector<Rect> &BBoxes);

void SortXCoordinate(vector<Rect> &BBoxes);

// return true if B1.y < B2.y
bool CompareYCoordinate(Rect B1, Rect B2);

// return true if B1.area() < B2.area()
bool CompareArea(Rect B1, Rect B2);

// return true if B1.x < B2.x
bool CompareXCoordinate(Rect B1, Rect B2);
