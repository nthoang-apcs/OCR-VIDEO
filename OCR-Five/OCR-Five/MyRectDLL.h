#pragma once
#include <opencv2\imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\features2d.hpp>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
using namespace cv;

struct BBoxNode
{
	Rect a;
	int id;
};

struct RectDLLNode
{
	Rect m_a;
	int m_ID;
	RectDLLNode *next;
	RectDLLNode *prev;

	RectDLLNode()
	{
		m_a = Rect(0, 0, 0, 0);
		m_ID = 0;
		next = NULL;
		prev = NULL;
	}
};


class MyRectDLL
{
private:
	RectDLLNode* m_Head;

public:
	MyRectDLL()
	{
		m_Head = NULL;
	}




	void Destroy()
	{

	}

	~MyRectDLL()
	{

	}
};