#include "MyCondition.h"


bool IsB1onsamelineB2(Rect B1, Rect B2)
{
	// range from center to upper or lower
	int range1 = B1.height / 4;
	int c1y = B1.y + (B1.height / 2);
	int range2 = B2.height / 4;
	int c2y = B2.y + (B2.height / 2);
	// check
	int dif = abs(c2y - c1y);
	if (dif < (range1 + range2))
	{
		return true;
	}
	return false;
}

bool IsB1Balanced(Rect B1)
{
	if ((B1.width / B1.height) >= 6)
	{
		return false;
	}
	if ((B1.height / B1.width) >= 6)
	{
		return false;
	}
	if (B1.height < 9)
	{
		return false;
	}
	return true;
}

bool IsB1insideB2(Rect B1, Rect B2)
{
	Rect a = B1 & B2;
	if (a.area() == B1.area() && (B2.area() / B1.area()) <= 3)
		return true;
	return false;

}
