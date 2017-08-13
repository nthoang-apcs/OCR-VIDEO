#include <opencv2\text.hpp>
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include <vector>
#include <string>
#include "PartOneProcess.h"

using namespace std;
using namespace cv;

void RefineVNTestingSet();

void SharpenImage(string pathIn, string pathOut);

void BlurImage(string pathIn, string pathOut);

int display_caption(char* caption, Mat &src, Mat &dst, int DELAY_CAPTION);

int display_dst(int delay);

int main(char* argv)
{
	string input = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\thesis-scene-images-1-250\\img8.jpg";
	string output1 = "E:\\Code\\OCR-Five-Git\\OCR-Five\\OCR-Five\\testsharpen.jpg";
	string output2 = "E:\\Code\\OCR-Five-Git\\OCR-Five\\OCR-Five\\testblur.jpg";
	SharpenImage(input, output1);
	BlurImage(input, output2);

	return 0;
}



void RefineVNTestingSet()
{
	vector<string> fileList;
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img250.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img251.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img252.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img253.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img254.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img255.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img256.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img257.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img258.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img259.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img260.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img261.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img262.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img263.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img264.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img265.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img266.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img267.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img268.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img269.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img270.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img271.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img272.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img273.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img274.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img275.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img276.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img277.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img278.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img279.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img280.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img281.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img282.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img283.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img284.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img285.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img286.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img287.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img288.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img289.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img290.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img291.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img292.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img293.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img294.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img295.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img296.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img297.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img298.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img299.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img300.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img301.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img302.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img303.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img304.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img305.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img306.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img307.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img308.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img309.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img310.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img311.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img312.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img313.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img314.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img315.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img316.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img317.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img318.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img319.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img320.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img321.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img322.png");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img323.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img324.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img325.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img326.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img327.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img328.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img329.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img330.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img331.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img332.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img333.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img334.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img335.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img336.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img337.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img338.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img339.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img340.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img341.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img342.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img343.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img344.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img345.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img346.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img347.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img348.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img349.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img350.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img351.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img352.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img353.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img354.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img355.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img356.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img357.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img358.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img359.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img360.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img361.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img362.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img363.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img364.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img365.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img366.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img367.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img368.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img369.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img370.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img371.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img372.png");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img373.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img374.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img375.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img376.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img377.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img378.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img379.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img380.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img381.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img382.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img383.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img384.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img385.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img386.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img387.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img388.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img389.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img390.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img391.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img392.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img393.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img394.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img395.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img396.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img397.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img398.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img399.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img400.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img401.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img402.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img403.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img404.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img405.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img406.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img407.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img408.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img409.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img410.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img411.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img412.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img413.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img414.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img415.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img416.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img417.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img418.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img419.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img420.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img421.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img422.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img423.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img424.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img425.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img426.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img427.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img428.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img429.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img430.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img431.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img432.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img433.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img434.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img435.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img436.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img437.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img438.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img439.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img440.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img441.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img442.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img443.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img444.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img445.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img446.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img447.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img448.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img449.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img450.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img451.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img452.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img453.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img454.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img455.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img456.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img457.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img458.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img459.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img460.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img461.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img462.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img463.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img464.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img465.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img466.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img467.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img468.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img469.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img470.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img471.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img472.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img473.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img474.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img475.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img476.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img477.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img478.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img479.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img480.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img481.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img482.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img483.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img484.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img485.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img486.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img487.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img488.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img489.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img490.png");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img491.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img492.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img493.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img494.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img495.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img496.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img497.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img498.JPG");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img499.jpg");
	fileList.push_back("D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set\\img500.jpg");

	string resultFolder = "D:\\Document\\Thesis-2017\\Data\\500sceneimages\\Vietnamese-testing-set-1\\";

	PartOneProcess *One = new PartOneProcess();
	One->ConvertandResizeMultipleFiles(fileList, resultFolder);
	fileList.clear();
	resultFolder.clear();
}

void SharpenImage(string pathIn, string pathOut)
{
	// changing values of sigma, threshold, amount will give different results
	// sharpen image using "unsharp mask" algorithm
	Mat blurred; double sigma = 1, threshold = 5, amount = 1;
	Mat img = imread(pathIn);
	GaussianBlur(img, blurred, Size(), sigma, sigma);
	Mat lowContrastMask = abs(img - blurred) < threshold;
	Mat sharpened = img*(1 + amount) + blurred*(-amount);
	img.copyTo(sharpened, lowContrastMask);
	imwrite(pathOut, sharpened);
}

void BlurImage(string pathIn, string pathOut)
{
	// init
	Mat src = imread(pathIn);
	Mat dst = src.clone();
	int DELAY_CAPTION = 1500;
	int DELAY_BLUR = 100;
	int MAX_KERNEL_LENGTH = 31;

	if (display_caption("Gaussian Blur", src, dst, DELAY_CAPTION) != 0)
	{ 
		return; 
	}

	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		GaussianBlur(src, dst, Size(i, i), 0, 0);
		if (display_dst(DELAY_BLUR) != 0) 
		{ 
			return; 
		}
	}
	imwrite(pathOut, dst);

}

int display_caption(char* caption, Mat &src, Mat &dst, int DELAY_CAPTION)
{
	dst = Mat::zeros(src.size(), src.type());
	putText(dst, caption,
		Point(src.cols / 4, src.rows / 2),
		CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));

	//imshow(window_name, dst);
	int c = waitKey(DELAY_CAPTION);
	if (c >= 0) { return -1; }
	return 0;
}

int display_dst(int delay)
{
	//imshow(window_name, dst);
	int c = waitKey(delay);
	if (c >= 0) { return -1; }
	return 0;
}