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
    double timerunning = 0.00;
    vector<Rect> BBoxes;
    clock_t start = clock();
    /*      Pre-processing    */
    PreProcessing(mOriginImage, BBoxes, timerunning);

    /*      Post processing step 1    */



    return 1;
}

void PreProcessing(Mat mOriginImage, vector<Rect> &BBoxes, double &TimeRunning)
{
    clock_t start = clock();
    /*      Pre-processing    */
    // grayscale
    Mat mTmp1;
    cvtColor(mOriginImage, mTmp1, CV_BGR2GRAY);
	// sharpen
	Mat mTmp2;
	SharpenOneImage(mTmp1, mTmp2);
    timerunning += (double)(clock() - start) / (double)CLOCKS_PER_SEC;

    /*      MSER    */
    MSEROneImage(mTmp2, BBoxes, timerunning);
}

void PostProcessing(vector<Rect> &BBoxes, double &TimeRunning)
{
    clock_t start = clock();
	// sort area ascending
	SortArea(BBoxes);
	// remove areas of stand alone single box
	RemoveUnusualAreaBoxes(BBoxes);
	// remove unbalanced ratio width, height
	RemoveUnbalancedRatio(BBoxes);
	// sort y coordinate ascending
	SortYCoordinate(BBoxes);
	// remove single box text line
	RemoveSingleBoxTextLine(BBoxes);
	// merge inside box
	MergeInsideBoxes(BBoxes);

    TimeRunning += (double)(clock() - start) / (double)CLOCKS_PER_SEC;
}
