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



struct RectNode
{
    int m_ID;
    Rect m_a;
    RectNode *next;
    RectNode *prev;

	RectNode()
	{
		m_ID = 0;
		m_a = Rect(0, 0, 0, 0);
		next = NULL;
		prev = NULL;
	}

    RectNode(int id, Rect input)
    {
        m_ID = id;
        m_a = input;
        next = NULL;
        prev = NULL;
    }

	string getStringRect()
	{
		return to_string(m_a.x) + "," + to_string(m_a.y) + "," + to_string(m_a.width) + "," + to_string(m_a.height);
	}

	int ConvertVectorCharToInt(vector<char> &input)
	{
		int x = 0;
		int nSize = input.size();
		for (int i = 0; i < nSize; i++)
		{
			// check if input[i] is a digit
			if (input[i] < '0' && input[i] > '9')
			{
				return x;
			}
			x = x * 10 + ((int)(input[i]) - (int)('0'));
		}
		return x;
	}

	bool setRectFromString(string x1)
	{
		int x, y, width, height;
		if (x1 == "")
			return false;
		int leng = x1.length();
		int pos = 0;
		vector<char> tmp1;
		// ignore space
		while (pos < leng && x1[pos] == ' ')
		{
			pos++;
		}
		if (pos == leng)
		{
			return false;
		}
		// x
		while (pos < leng && x1[pos] != ' ')
		{
			tmp1.push_back(x1[pos]);
			pos++;
		}
		if (pos == leng)
		{
			return false;
		}
		x = ConvertVectorCharToInt(tmp1);
		tmp1.clear();
		// ignore space
		while (pos < leng && x1[pos] == ' ')
		{
			pos++;
		}
		if (pos == leng)
		{
			return false;
		}
		// y
		while (pos < leng && x1[pos] != ' ')
		{
			tmp1.push_back(x1[pos]);
			pos++;
		}
		if (pos == leng)
		{
			return false;
		}
		y = ConvertVectorCharToInt(tmp1);
		tmp1.clear();
		// ignore space
		while (pos < leng && x1[pos] == ' ')
		{
			pos++;
		}
		if (pos == leng)
		{
			return false;
		}
		// width
		while (pos < leng && x1[pos] != ' ')
		{
			tmp1.push_back(x1[pos]);
			pos++;
		}
		if (pos == leng)
		{
			return false;
		}
		width = ConvertVectorCharToInt(tmp1);
		tmp1.clear();
		// ignore space
		while (pos < leng && x1[pos] == ' ')
		{
			pos++;
		}
		if (pos == leng)
		{
			return false;
		}
		// height
		while (pos < leng && x1[pos] != ' ')
		{
			tmp1.push_back(x1[pos]);
			pos++;
		}
		height = ConvertVectorCharToInt(tmp1);
		tmp1.clear();
		m_a = Rect(x, y, width, height);
		return true;
	}
};

class RectDLL
{
private:
    RectNode* Head;

protected:

    // convert rect get from mser
    // -> output the rect to recognize
    Rect ConvertTheoryRectToRealRect(Rect Input)
    {
    	Rect result;
    	if(Input.height < 10 && Input.width < 10)
    	{
    		// 3 pixels
    		result = Rect(Input.x, Input.y, Input.width + 3, Input.height + 3);
    	}
    	else if(Input.height < 20 && Input.width < 20)
    	{
    		// 5 pixels
    		result = Rect(Input.x, Input.y, Input.width + 5, Input.height + 5);
    	}
    	else if(Input.height < 100 && Input.width < 100)
    	{
    		// 10 pixels
    		result = Rect(Input.x, Input.y, Input.width + 10, Input.height + 10);
    	}
    	else
    	{
    		// 10%
    		result = Rect(Input.x, Input.y, Input.width * 1.1, Input.height * 1.1);
    	}
    	return result;
    }

    // convert rect from recognition
    // -> output rect from mser
    Rect ConvertRealRectToTheoryRect(Rect Input)
    {
    	Rect result;
    	if(Input.height < 10 && Input.width < 10)
    	{
    		// 3 pixels
    		result = Rect(Input.x, Input.y, Input.width - 3, Input.height - 3);
    	}
    	else if(Input.height < 20 && Input.width < 20)
    	{
    		// 5 pixels
    		result = Rect(Input.x, Input.y, Input.width - 5, Input.height - 5);
    	}
    	else if(Input.height < 100 && Input.width < 100)
    	{
    		// 10 pixels
    		result = Rect(Input.x, Input.y, Input.width - 10, Input.height - 10);
    	}
    	else
    	{
    		// 10%
    		result = Rect(Input.x, Input.y, Input.width / 1.1, Input.height / 1.1);
    	}
    	return result;
    }

	void GetAverageLineEquation(vector<Rect> &CurrentLines, float &a, float &b, float &c)
	{
		// all direction is from left to right
		// 0: stand
		// 1: north east
		// 2: east
		// 3: south east
		int direction = 0;
		
		vector<Point2i> aCenterOfMass;
		int nSize = CurrentLines.size();

		// get all center
		for (int i = 0; i < nSize; i++)
		{
			int x = CurrentLines[i].x + (CurrentLines[i].width / 2);
			int y = CurrentLines[i].y + (CurrentLines[i].height / 2);
			aCenterOfMass.push_back(Point2i(x, y));
		}

		// check if centers are on the same direction 
		


	}

public:
    RectDLL()
    {
        Head = NULL;
    }

    // convert all rect in list
    // - 3 pixel if the height & width < 10
	// - 5 pixels if the height & width < 20
	// - 10 pixels if the height & width < 100
	// - 10% of height + width else
    void ConvertTheoryListToRealList()
    {
        if(Head == NULL)
            return;
        RectNode* tmp = Head;
        while(tmp != NULL)
        {
            tmp->m_a = ConvertTheoryRectToRealRect(tmp->m_a);
            tmp = tmp->next;
        }
    }
    // vice versa to the above condition
    void ConvertRealListToTheoryList()
    {
        if(Head == NULL)
            return;
        RectNode* tmp = Head;
        while(tmp != NULL)
        {
            tmp->m_a = ConvertRealRectToTheoryRect(tmp->m_a);
            tmp = tmp->next;
        }
    }

    void push_back(int ID, Rect input)
    {
        if(Head == NULL)
        {
            Head = new RectNode(ID, input);
            return;
        }
		if (Head->next == NULL)
		{
			RectNode* tmp = new RectNode(ID, input);
			Head->next = tmp;
			tmp->prev = Head;
			return;
		}
		RectNode* tmp = Head->next;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		RectNode* tmp2 = new RectNode(ID, input);
		tmp->next = tmp2;
		tmp2->prev = tmp;
		return;
    }

    void push_back(int ID, RectNode &X)
    {
		if (Head == NULL)
		{
			Head = new RectNode(ID, X.m_a);
			return;
		}
		if (Head->next == NULL)
		{
			RectNode* tmp = new RectNode(ID, X.m_a);
			Head->next = tmp;
			tmp->prev = Head;
			return;
		}
		RectNode* tmp = Head->next;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		RectNode* tmp2 = new RectNode(ID, X.m_a);
		tmp->next = tmp2;
		tmp2->prev = tmp;
		return;
    }

	void BindingArrayWithID(vector<Rect> &BBoxes, int start)
	{
		int nSize = BBoxes.size();
		if (nSize == 0)
			return;
		if (Head == NULL)
		{
			Head = new RectNode(start, BBoxes[0]);
			RectNode* sTail = Head;
			int nID = start + 1;
			for (int i = 1; i < nSize; i++)
			{
				RectNode* tmp = new RectNode(nID, BBoxes[i]);
				sTail->next = tmp;
				tmp->prev = sTail;
				sTail = sTail->next;
				nID++;
			}
			return;
		}
		else
		{
			RectNode* sTail = Head;
			while (sTail->next != NULL)
			{
				sTail = sTail->next;
			}
			int nID = start;
			for (int i = 1; i < nSize; i++)
			{
				RectNode* tmp = new RectNode(nID, BBoxes[i]);
				sTail->next = tmp;
				tmp->prev = sTail;
				sTail = sTail->next;
				nID++;
			}
			return;
		}
	}

	// RectDLL must be sorted in X coordinate before running this function.
	void RemoveSimilarityOfThisToThat(vector<RectDLL> &That)
	{
		int nSize = That.size();
		if (nSize == 0)
			return;
		vector<int> SameID;
		for (int i = 0; i < nSize; i++)
		{
			RectNode* tmp1 = Head;
			RectNode* tmp2 = That[i].Head;
			while (tmp2 != NULL)
			{
				if (tmp1 == NULL)
				{
					break;
				}
				if (tmp2->m_ID < tmp1->m_ID)
				{
					tmp2 = tmp2->next;
				}
				else if (tmp2->m_ID > tmp1->m_ID)
				{
					tmp1 = tmp1->next;
				}
				else
				{
					SameID.push_back(tmp1->m_ID);
					tmp1 = tmp1->next;
					tmp2 = tmp2->next;
				}
			}
		}
		// remove similarity in SameID
		vector<int> SameID2;
		nSize = SameID.size();
		bool checked = false;
		for (int i = 0; i < nSize; i++)
		{
			checked = false;
			for (int j = 0; j < SameID2.size(); j++)
			{
				if (SameID[i] == SameID2[j])
				{
					checked = true;
					break;
				}
			}
			if (checked == false)
			{
				SameID2.push_back(SameID[i]);
			}
		}
		SameID.clear();
		// remove list SameID in this list
		RemoveSameIDInList(SameID2);
	}

	// each ID in vector SameID must be unique
	void RemoveSameIDInList(vector<int> &SameID)
	{
		RectNode* tmp = Head;
		int nI = 0;
		int nSize = SameID.size();
		if (Head == NULL || nSize == 0)
			return;
		while (tmp != NULL)
		{
			if (nSize == nI)
				return;
			if (tmp->m_ID == SameID[nI])
			{
				if (tmp->prev == NULL)
				{
					if (tmp->next == NULL)		// 1 element list
					{
						delete tmp;
						tmp = NULL;
						Head = NULL;
						return;
					}
					else    // at the head of the list
					{
						RectNode* tmp2 = tmp->next;
						tmp2->prev = NULL;
						tmp->next = NULL;
						delete tmp;
						tmp = tmp2;
						nI++;
					}
				}
				else
				{
					if (tmp->next == NULL)		// at the end of the list
					{
						RectNode* tmp2 = tmp->prev;
						tmp->prev = NULL;
						tmp2->next = NULL;
						delete tmp;
						tmp = NULL;
						Head = NULL;
						return;
					}
					else    // in the middle of the list
					{
						RectNode* tmp2 = tmp->prev;
						RectNode* tmp3 = tmp->next;
						tmp2->next = tmp3;
						tmp3->prev = tmp2;
						tmp->prev = NULL;
						tmp->next = NULL;
						delete tmp;
						tmp = tmp3;
						nI++;
					}
				}
			}
			else if (tmp->m_ID > SameID[nI])
			{
				nI++;
			}
			else
			{
				tmp = tmp->next;
			}
		}
	}

	int size()
	{
		if (Head == NULL)
			return 0;
		int nCount = 0;
		RectNode* tmp = Head;
		while (tmp != NULL)
		{
			tmp = tmp->next;
			nCount++;
		}
		return nCount;
	}

	string getStringID()
	{
		if (Head == NULL)
			return string();
		RectNode* tmp = Head;
		string res = "";
		while (tmp != NULL)
		{
			res = res + to_string(tmp->m_ID);
			res = res + " ";
			tmp = tmp->next;
		}
		return res;
	}

    string getStringIDatIndex(int index)
    {
        string res = "";
        int count = 0;
        if(Head == NULL)
        {
            return res;
        }
        RectNode* tmp = Head;
        while(tmp != NULL)
        {
            if(count == index)
            {
                break;
            }
            count++;
            tmp = tmp->next;
        }
        if(tmp == NULL)
        {
            return res;
        }
        res = to_string(tmp->m_ID);
        return res;
    }

	string getStringRect()
	{
		if (Head == NULL)
			return string();
		RectNode* tmp = Head;
		string res = "";
		while (tmp != NULL)
		{
			res = res + tmp->getStringRect();
			res = res + " ";
			tmp = tmp->next;
		}
		return res;
	}

    string getStringRectatIndex(int index)
    {
        string res = "";
        int count = 0;
        if(Head == NULL)
        {
            return res;
        }
        RectNode* tmp = Head;
        while(tmp != NULL)
        {
            if(count == index)
            {
                break;
            }
            count++;
            tmp = tmp->next;
        }
        if(tmp == NULL)
        {
            return res;
        }
        res = tmp->getStringRect();
        return res;
    }

	string getStringRectMerge()
	{
		Rect tmp = MergeAllRectInside();
		return to_string(tmp.x) + "," + to_string(tmp.y) + "," + to_string(tmp.width) + "," + to_string(tmp.height);
	}

	// return Rect 0 0 0 0 if index < 0 or Head = NULL
	Rect GetRectAtIndex(int index)
	{
		if (index > -1 || Head == NULL)
		{
			return Rect(0, 0, 0, 0);
		}
		RectNode* tmp = Head;
		int count = 0;
		while (tmp != NULL)
		{
			if (count == index)
			{
				break;
			}
			tmp = tmp->next;
			count++;
		}
		if (tmp == NULL)
		{
			return Rect(0, 0, 0, 0);
		}
		return tmp->m_a;
	}

	Rect MergeAllRectInside()
	{
		Rect result = Rect(0, 0, 0, 0);
		if (Head == NULL)
		{

		}
		else
		{
			RectNode* tmp = Head;
			while (tmp != NULL)
			{
				result = result & tmp->m_a;
				tmp = tmp->next;
			}
		}
		return result;
	}

	// twoindex > oneindex
	// true: overlap and the ratio between 2 area is < 3
	// false: else
	bool IsOneOverlapTwo(int oneindex, int twoindex)
	{
		if (Head == NULL)
		{
			return false;
		}
		RectNode* tmp = Head;
		int count = 0;
		Rect r1, r2;

		while (tmp != NULL)
		{
			if (count == oneindex)
			{
				break;
			}
			count++;
			tmp = tmp->next;
		}
		if (tmp == NULL)
			return false;
		else
		{
			r1 = tmp->m_a;
		}
		while (tmp != NULL)
		{
			if (count == twoindex)
			{
				break;
			}
			count++;
			tmp = tmp->next;
		}
		if (tmp == NULL)
			return false;
		else
		{
			r2 = tmp->m_a;
		}
		Rect r3 = r1 & r2;
		if (r3.area() > 0 && (r2.area() / r1.area()) < 3 && (r1.area() / r2.area()) < 3)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// check if secondindex is on same line as firstindex with few conditions:
	// 2ndindex is on right side of 1stindex
	// limit width
	// limit height
	// limit area
	bool IsSatisfyLineTextCondition(int firstindex, int secondindex)
	{
		Rect r1, r2;
		if (Head == NULL)
			return false;
		RectNode* tmp = Head;
		int count = 0;
		while (tmp != NULL)
		{
			if (count == firstindex)
			{
				r1 = tmp->m_a;
				break;
			}
			count++;
			tmp = tmp->next;
		}
		if (tmp == NULL)
			return false;
		while (tmp != NULL)
		{
			if (count == firstindex)
			{
				r2 = tmp->m_a;
				break;
			}
			count++;
			tmp = tmp->next;
		}
		if (tmp == NULL)
			return false;
		// check condition
		// area different: < 3
		// center of mass of r2 must be in the Rect on right side of r1
		// Rect:	x = r1.x
		//			y = r1.y
		//			width = r1.width * 3.5
		//			height = r1.height
		int cx = r2.x + (r2.width / 2);
		int cy = r2.y + (r2.height / 2);
		// location of x: 3.5 times
		if (cx > r1.x && cx < (r1.x + r1.width * 3.5))
		{
			// location of y 30% ratio
			if (cy > (r1.y - r1.height * 0.3) && cy < (r1.y + r1.height * 0.3))
			{
				// area different < 3
				if ((r2.area() / r1.area()) < 3 && (r1.area() / r2.area()) < 3)
				{
					Rect r3 = r1 & r2;
					// prevent inside box
					if (r3.area() == r2.area() || r3.area() == r1.area())
						return false;
					else
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	
	// the same as IsSatisfyLineTextCondition with 2 more conditions:
	// the angle between currentline and second index center of mass must < 30 degree
	// the distance between secondindex and currentline must be in a limit range
	// range limit = 2* range average between rect in current line
	bool IsSatisfyCompleteLineTextCondition(vector<int> &currentLine, int secondindex)
	{
		if (currentLine.size() == 0 || Head == NULL)
			return false;
		RectNode* tmp = Head;
		int count = 0;
		int nSize = currentLine.size();
		vector<Rect> tmpline;
		Rect tmp2ndindex;
		int pos = 0;
		// get Rect
		while (tmp != NULL)
		{
			if (count == currentLine[pos])
			{
				tmpline.push_back(tmp->m_a);
				pos++;
			}
			count++;
			tmp = tmp->next;
		}
		if (tmp == NULL)
			return false;
		while (tmp != NULL)
		{
			if (count == secondindex)
			{
				tmp2ndindex = tmp->m_a;
				break;
			}
			count++;
			tmp = tmp->next;
		}
		if (tmp == NULL)
			return false;
		// get average line of currentLine
		float a, b, c;
		GetAverageLineEquation(tmpline, a, b, c);
		// compare angle



	}

	// return -1 if no rect exist
	int RecheckConditionLineText(int firstindex, vector<int> &input)
	{
		// get all rect out
		vector<Rect> tmp1;
		Rect first;
		if (Head == NULL)
		{
			return -1;
		}
		RectNode* tmp = Head;
		int count = 0;
		int nSize = input.size();
		int i = 0;
		if (nSize == 0)
			return -1;
		if (nSize == 1)
			return 0;
		// get coordinate rect
		while (tmp != NULL)
		{
			if (count == firstindex)
			{
				break;
			}
			count++;
			tmp = tmp->next;
		}
		if (tmp == NULL)
			return -1;
		first = tmp->m_a;
		// get samples rect
		while (tmp != NULL)
		{
			if (i < nSize && count == input[i])
			{
				tmp1.push_back(tmp->m_a);
				i++;
			}
			count++;
			tmp = tmp->next;
		}
		if (tmp == NULL)
			return -1;
		// now check condition
		nSize = tmp1.size();
		int bestpoint = -1;
		int ibp = -1;
		int fArea = first.area();
		for (i = 0; i < nSize; i++)
		{
			int point = 0;
			// estimate point
			// location of x
			if (tmp1[i].x < (first.x + first.width * 2))
			{
				point += 3;
			}
			else if (tmp1[i].x  < (first.x + first.width * 2.5))
			{
				point += 2;
			}
			else
			{
				point += 1;
			}
			// location of y
			if (tmp1[i].y == first.y && (tmp1[i].y + tmp1[i].height) < (first.y + first.height * 0.1))
			{
				point += 4;
			}
			else if (tmp1[i].y > (first.y - first.height * 0.1) && (tmp1[i].y + tmp1[i].height) < (first.y + first.height * 0.1))
			{
				point += 3;
			}
			else if (tmp1[i].y > (first.y - first.height * 0.2) && (tmp1[i].y + tmp1[i].height) < (first.y + first.height * 0.2))
			{
				point += 2;
			}
			else
			{
				point += 1;
			}
			// check area
			int tmp1Area = tmp1[i].area();
			if (tmp1Area > fArea)
			{
				if ((tmp1Area / fArea) < 1.5)
					point += 3;
				else if ((tmp1Area / fArea) < 2)
					point += 2;
				else
					point += 1;
			}
			else
			{
				if ((fArea / tmp1Area) < 1.5)
					point += 3;
				else if ((fArea / tmp1Area) < 2)
					point += 2;
				else
					point += 1;
			}
			// compare with best point
			if (point > bestpoint)
			{
				bestpoint = point;
				ibp = i;
			}
		}
		tmp1.clear();
		return i;
	}


    void Destroy()
    {
        if(Head == NULL)
        {
            return;
        }
        if(Head->next == NULL)
        {
            delete Head;
            Head = NULL;
        }
        RectNode *rnTmp1 = Head->next;
        while(rnTmp1 != NULL)
        {
            Head->next = NULL;
            rnTmp1->prev = NULL;
            delete Head;
            Head = rnTmp1;
            rnTmp1 = Head->next;
        }
        delete Head;
        Head = NULL;
    }

    ~RectDLL()
    {
        Destroy();
    }
};
