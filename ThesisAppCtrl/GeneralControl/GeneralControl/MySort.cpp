#include "MySort.h"


void SortYCoordinate(vector<Rect> &BBoxes)
{
	sort(BBoxes.begin(), BBoxes.end(), CompareYCoordinate);
}

void SortArea(vector<Rect> &BBoxes)
{
	sort(BBoxes.begin(), BBoxes.end(), CompareArea);
}

void SortXCoordinate(vector<Rect> &BBoxes)
{
	sort(BBoxes.begin(), BBoxes.end(), CompareXCoordinate);
}

bool CompareYCoordinate(Rect B1, Rect B2)
{
	return (B1.y < B2.y);
}

bool CompareArea(Rect B1, Rect B2)
{
	return (B1.area() < B2.area());
}

bool CompareXCoordinate(Rect B1, Rect B2)
{
	return (B1.x < B2.x);
}
