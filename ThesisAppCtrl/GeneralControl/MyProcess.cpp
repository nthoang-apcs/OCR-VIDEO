#include "MyProcess.h"


int RunProcessAll(vector<Mat> &mOriginImages, vector<Mat> &mOutputImages, char *CurrentFolder)
{
    int k = mOriginImages.size();
    for(int i = 0; i < k; i++)
    {
		Mat mResultImage;
        int checked = RunProcessOne(mOriginImages[i], mResultImage, CurrentFolder);
        if(checked == 0)
        {
            return 0;
        }
		else
		{
			mOutputImages.push_back(mResultImage);
		}
    }
    return 1;
}

int RunProcessOne(Mat &mOriginImage, Mat &mOutputImage, char *CurrentFolder)
{
    double timerunning = 0.00;
    vector<Rect> BBoxes;
    clock_t start = clock();
	RectDLL OtherBoxes;
	vector<RectDLL> Lines;
    /*      Pre-processing    */
    PreProcessing(mOriginImage, BBoxes, timerunning);

    /*      Post processing step 1    */
    PostProcessing(BBoxes, timerunning);

    /*      bind ID to each BBox, merge line text    */
	PostProcessingStepTwo(BBoxes, timerunning, OtherBoxes, Lines);

	// the image cut out is bigger size (width and height increase) than the size in saved file:
    // -> change Rect
	PostProcessingStepThree(timerunning, OtherBoxes, Lines);

    // cut image -> save format: tmp\imgTmp-[x].jpg
	// it is the folder where the recogition.exe is stored
	// All stored file names are in tmp\files.txt
	SaveLines(mOriginImage, CurrentFolder, Lines);
    // save info of each tmp file
    SaveOtherBoxes(mOriginImage, CurrentFolder, OtherBoxes);



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
	TimeRunning += (double)(clock() - start) / (double)CLOCKS_PER_SEC;

    /*      MSER    */
    MSEROneImage(mTmp2, BBoxes, TimeRunning);
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

void PostProcessingStepTwo(vector<Rect> &BBoxes, double &TimeRunning, RectDLL &OtherBoxes, vector<RectDLL> &Lines)
{
	clock_t start = clock();
    // Sort X
    SortXCoordinate(BBoxes);
    // binding ID
	BindingID(BBoxes, OtherBoxes, 0);
	// merge line text
	MergeLineText(OtherBoxes, Lines);
	// reduce false positive

	// get time running calculation
	TimeRunning += (double)(clock() - start) / (double)CLOCKS_PER_SEC;
	return;
}

void PostProcessingStepThree(double &TimeRunning, RectDLL &OtherBoxes, vector<RectDLL> &Lines)
{
	clock_t start = clock();
    // convert Rects
    OtherBoxes.ConvertTheoryListToRealList();
    int nSize = Lines.size();
    for(int i = 0; i < nSize; i++)
    {
        Lines[i].ConvertTheoryListToRealList();
    }


    // get time running calculation
	TimeRunning += (double)(clock() - start) / (double)CLOCKS_PER_SEC;
	return;
}
