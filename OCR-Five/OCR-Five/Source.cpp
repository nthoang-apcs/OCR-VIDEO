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

void SharpenPart();
void SharpMSERPart();
void SharpMSERGeoPart();
void CropBoxesPart();

int main(char* argv)
{
	//SetListName(1, 500, "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\filestext.txt");
	//SetListPath(1, 500, "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\files.txt", "D:\\Document\\Thesis-2017\Data\\500sceneimages\\vietnamese-images\\");

	// get list boxes paths
	vector<string> boxesPaths;
	string fileboxes = "D:\\Document\\Thesis-2017\\Data2\\ResExpSharpMSERGeoVer3\\filesboxes.txt";
	AddListPath(boxesPaths, fileboxes);

	// get list file image paths
	vector<string> Paths;
	string filepath = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\files.txt";
	AddListPath(Paths, filepath);

	// get list boxes
	vector<Rect> BBoxes;
	vector<Point> Centroids;
	int k1 = boxesPaths.size();
	for (int i = 0; i < k1; i++)
	{
		GetListBoxesInOneImage(BBoxes, boxesPaths[i]);
		
		// create vector centroid
		GetListCentroids(BBoxes, Centroids);

		// build all possible path
		int k2 = BBoxes.size();
		vector<vector<int>> PointLines;
		vector<Line> EquationLines;
		
		for (int t1 = 0; t1 < k2; t1++)
		{
			vector<int> onelinepoint;
			for (int t2 = 0; t2 < k2; t2++)
			{
				if (t1 == t2)
					continue;
				// satisfy conditions to build path
				if (CheckConditionOfBoxLine(BBoxes[t1], BBoxes[t2]))
				{
					// first point
					if (onelinepoint.size() == 0)
					{
						onelinepoint.push_back(t2);
						// form an equation

					}
					else
					{
						// check line equation and threshold
						
					}
				}

			}

		}


	}

	




	return 0;
}


void SharpenPart()
{
	vector<string> Paths;
	string filepath = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\files.txt";
	AddListPath(Paths, filepath);
	int k = Paths.size();
	string resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentSharpen\\";
	for (int i = 0; i < k; i++)
	{
		Mat input = imread(Paths[i]);
		Mat output;
		SharpenOneImage(input, output);
		string name = ExtractNameOfFileFromPathIn(Paths[i]);
		imwrite(resultFolder + name + ".jpg", output);
	}
}

void SharpMSERPart()
{
	vector<string> Paths;
	string filepath = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\files.txt";
	AddListPath(Paths, filepath);
	PartOneProcess *one = new PartOneProcess();
	string resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentSharpenMSER\\";
	one->doProcessOneImageWithPostProcessing("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-training-set\\img26.jpg", "D:\\Document\\Thesis-2017\\Data\\Result-sharpen-postprocessing-image-experiment\\");
	one->doProcessImagesWithoutPostProcessing(Paths, resultFolder);

	GetListName(Paths, resultFolder, "listname.txt");
	GetListTotalBoxes(Paths, resultFolder, "listboxes.txt");
	GetListRunTime(Paths, resultFolder, "listtimes.txt");
}

void SharpMSERGeoPart()
{
	vector<string> Paths;
	vector<int> B1;
	vector<double> R1;
	vector<double> R2;
	string filepath = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\files.txt";
	string resultFolder;
	AddListPath(Paths, filepath);
	PartOneProcess *one = new PartOneProcess();

	resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResExpSharpMSERGeoVer3\\";
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
}

void CropBoxesPart()
{
	vector<string> Paths;
	vector<string> boxesPaths;
	string filepath = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\vietnamese-images\\files.txt";
	string fileboxes = "D:\\Document\\Thesis-2017\\Data2\\ResExpSharpMSERGeoVer1\\filesboxes.txt";
	string resultFolder;
	AddListPath(Paths, filepath);
	AddListPath(boxesPaths, fileboxes);
	PartOneProcess *one = new PartOneProcess();

	resultFolder = "D:\\Document\\Thesis-2017\\Data2\\ResultExperimentCropBoxesImages\\";
	one->CropBoxesImages(Paths, resultFolder, boxesPaths);
}

