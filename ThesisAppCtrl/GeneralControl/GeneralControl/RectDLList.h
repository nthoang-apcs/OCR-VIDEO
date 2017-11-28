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
