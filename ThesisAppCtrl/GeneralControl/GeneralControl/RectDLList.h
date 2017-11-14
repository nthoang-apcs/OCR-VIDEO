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
    int ID;
    Rect a;
    RectNode *next;
    RectNode *prev;
};

class RectDLL
{
private:
    RectNode *Head;


public:
    RectDLL()
    {
        Head = NULL;
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
}
