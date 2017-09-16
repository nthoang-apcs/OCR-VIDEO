#include <opencv2\text.hpp>
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include <vector>
#include <string>
#include "PartOneProcess.h"
#include "CommonStructAndFunction.h"
#include "MSERIO.h"
using namespace std;
using namespace cv;



int main(char* argv)
{
	vector<string> Paths;
	string filepath = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set-1\\files.txt";
	string resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentSharpenMSER\\";
	AddListPath(Paths, filepath);
	PartOneProcess *one = new PartOneProcess();

	//one->doProcessOneImageWithPostProcessing("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-training-set\\img26.jpg", "D:\\Document\\Thesis-2017\\Data\\Result-sharpen-postprocessing-image-experiment\\");
	//one->doProcessImagesWithoutPostProcessing(Paths, resultFolder);
	
	//GetListName(Paths, resultFolder, "listname.txt");
	//GetListTotalBoxes(Paths, resultFolder, "listboxes.txt");
	GetListRunTime(Paths, resultFolder, "listtimes.txt");

	return 0;
}


