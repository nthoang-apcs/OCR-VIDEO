#include <iostream>
#include <cmath>
using namespace std;

struct Rect 
{
    int w;
    int h;
    int x;
    int y;
};

struct BBoxNode
{
	Rect a;
	int id;
};

void FindLineTextBBoxes(vector<Rect> &BBoxes, vector<BBoxNode> &PreLines, vector<Rect> &Lines);

int NextNearestHorizontal(int index, vector<Rect> &BBoxes);

vector<int> AllInteractWithRect(int index, vector<Rect> &BBoxes);

void FindTextLineInteract(vector<int> &Indexes, vector<Rect> &BBoxes);

int GetDegreeFromCenterOfAToB(Rect A, Rect B);

bool IsOnSameLine(int input, vector<int> &line);




int main()
{
    
    return 0;
}

// sort follow by x before go in this function
void FindLineTextBBoxes(vector<Rect> &BBoxes, vector<BBoxNode> &PreLines, vector<Rect> &Lines)
{
	int k = BBoxes.size();
	vector<Rect> tmpBBoxes;
	bool checked = false;
	vector<Rect> pivot;
	for(int i = 0; i < k; i++)
	{
		// find any interact Rect with this Rect
		vector<int> tmp1 = AllInteractWithRect(i, BBoxes);
		if(tmp1.size() == 0)
		{
			// empty case -> no box interact
		}
		else
		{
			
		}
		
	}
}

// distance between 2 Rect central point is < 2.5 * width of Rect[index]
int NextNearestHorizontal(int index, vector<Rect> &BBoxes)
{
	
}

// find interact, not inside -> inside merge later
vector<int> AllInteractWithRect(int index, vector<Rect> &BBoxes)
{
	vector<int> ResultIndex;
	int k = BBoxes.size();
	for(int i = 0; i < k; i++)
	{
		if(i == index)
			continue;
		Rect tmp = BBoxes[i] & BBoxes[index];
		if(tmp.area() > 0 && tmp.area() < BBoxes[index].area() && tmp.area < BBoxes[i].area())
		{
			ResultIndex.push_back(i);
		}
	}
	return ResultIndex;
}

// find the next point interact with Indexes[1] -> if form a line -> add to Indexes
void FindTextLineInteract(vector<int> &Indexes, vector<Rect> &BBoxes)
{
	int k = BBoxes.size();
	int index = Indexes[1];
	vector<int> degList;
	for(int i = 1; i <Indexes.size(); i++)
	{
		degList.push_back(GetDegreeFromCenterOfAToB(BBoxes[Indexes[0]], BBoxes[Indexes[i]]);
	}		
	for(int i = 0; i < k; i++)
	{
		if(i == index)
			continue;
		Rect tmp = BBoxes[i] & BBoxes[index];
		if(tmp.area() > 0 && tmp.area() < BBoxes[index].area() && tmp.area < BBoxes[i].area())
		{
			// check if form a line
			// get degree
			int deg1 = GetDegreeFromCenterOfAToB(BBoxes[Indexes[0]], BBoxes[i]);
			if(deg1 == 360 )
			{
				// not allow -> skip
				
			}
			// compare
			
			
		}
	}
}

// result in range [-79, +79], otherwise it is 360
int GetDegreeFromCenterOfAToB(Rect A, Rect B)
{
	if (B.x <= A.x)
	{
		return 360;
	}
	bool sig = false;
	if(B.y > A.y)
	{
		sig = false;
	}
	else if (B.y < A.y)
	{
		sig = true;
	}
	else
	{
		return 0;
	}
	
	int a = abs(A.y - B.y);
	int b = B.x - A.x;
	
	int result = (int)(atan(a / b) * 180 / PI);
	if(sig == true)
	{
		result = - result;
	}
	return result;
}

// limit allow: +- 10 degree
bool IsOnSameLine(int input, vector<int> &line)
{
	// vector only 1 angle
	if(line.size() == 1)
	{
		if(input > line[0] && input < (line[0] + 10))
		{
			return true;
		}
		else if (input < line[0] && input > (line[0] - 10))
		{
			return true;
		}
		else if (input == line[0])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// multiple angle
	int average = -1;
	int direction = -1;
	int k = line.size(); 
	for(int i = 0; i < k; i++)
	{
		average +=  line[i];
		if(direction == -1)
		{
			if(line[i]
		}
	}
	
	
}

