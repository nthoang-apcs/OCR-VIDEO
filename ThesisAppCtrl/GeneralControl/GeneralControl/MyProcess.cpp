#include "MyProcess.h"


int RunProcessAll(vector<Mat> &mOriginImage, vector<Mat> &mOutputImage, char *CurrentFolder)
{
    int k = mOriginImages.size();
    for(int i = 0; i < k; i++)
    {
        int checked = RunProcessOne(mOriginImages[i]);
        if(checked == 0)
        {
            return 0;
        }
    }
    return 1;
}

int RunProcessOne(Mat &mOriginImage, Mat &mOutputImage, char *CurrentFolder)
{
    /*      Pre-processing    */
    // grayscale
    Mat mTmp1;
    cvtColor(mOriginImage, mTmp1, CV_BGR2GRAY);
	// sharpen
	Mat mTmp2;
	SharpenOneImage(mTmp1, mTmp2);
    /*      MSER    */
    

    return 1;
}
