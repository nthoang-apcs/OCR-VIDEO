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
	//SetListName(1, 500, "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\filestext.txt");
	//SetListPath(1, 500, "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\files.txt", "D:\\Document\\Thesis-2017\Data\\500sceneimages\\vietnamese-images\\");

	vector<string> Paths;
	//vector<string> boxesPaths;
	vector<int> B1;
	vector<double> R1;
	vector<double> R2;
	string filepath = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\files.txt";
	//string fileboxes = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentSharpenMSERGeometric\\filesboxes.txt";
	string resultFolder;
	AddListPath(Paths, filepath);
	//AddListPath(boxesPaths, fileboxes);
	PartOneProcess *one = new PartOneProcess();

	/*		Sharpen part		*/
	//int k = Paths.size();
	//resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentSharpen\\";
	//for (int i = 0; i < k; i++)
	//{
	//	Mat input = imread(Paths[i]);
	//	Mat output;
	//	SharpenOneImage(input, output);
	//	string name = ExtractNameOfFileFromPathIn(Paths[i]);
	//	imwrite(resultFolder + name + ".jpg", output);
	//}



	/*		MSER part		*/
	//resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentSharpenMSER\\";
	//one->doProcessOneImageWithPostProcessing("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-training-set\\img26.jpg", "D:\\Document\\Thesis-2017\\Data\\Result-sharpen-postprocessing-image-experiment\\");
	//one->doProcessImagesWithoutPostProcessing(Paths, resultFolder);
	
	//GetListName(Paths, resultFolder, "listname.txt");
	//GetListTotalBoxes(Paths, resultFolder, "listboxes.txt");
	//GetListRunTime(Paths, resultFolder, "listtimes.txt");



	/*		MSER and Geometric part		*/
	resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentSharpenMSERGeometric\\";
	one->doProcessImagesWithPostProcessing(Paths, resultFolder);
	GetListName(Paths, resultFolder, "listname.txt");
	GetListTotalBoxes(Paths, resultFolder, B1, 1);
	writeTotalBoxesToFile(B1, resultFolder + "listboxes.txt");
	B1.clear();
	GetListRunTime(Paths, resultFolder, R1, 0);
	GetListRunTime(Paths, resultFolder, R2, 1);
	// add
	int k = R1.size();
	for (int i = 0; i < k; i++)
	{
		R1[i] = R1[i] + R2[i];
	}
	R2.clear();
	writeRunTimeToFile(R1, resultFolder + "listtimes.txt");
	R1.clear();


	/*		Crop boxes part		*/
	//resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentCropBoxesImages\\";
	//one->CropBoxesImages(Paths, resultFolder, boxesPaths);


	return 0;
}


