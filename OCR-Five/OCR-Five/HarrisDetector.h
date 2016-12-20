#pragma once
#include <opencv2\text.hpp>
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp> 
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <cmath>
#include "SupportFunctions.h"
#include "GaussianBlur.h"

using namespace std;
using namespace cv;

#define M_PI 3.14159265358979323846264338327

/*		How it work		*/
/*
1: Compute x and y derivatives of image: After_Gx, After_Gy, extra is After_G
2: Compute products of derivatives every pixels: X2 = Gx * Gx, Y2 = Gy * Gy, XY =  Gx * Gy
3: Compute sum of products of derivatives every pixels: Sx2 = After_G * X2 , Sy2 = After_G * Y2 , Sxy = After_G * XY
4: Define at each pixel matrix: H(x,y) = 
[	Sx2		Sxy
	Sxy		Sy2		]
5: Computer the response of the detector at each pixel: R = det(H) - k*(Trace(H))^2
6: Threshold on value of R. Compute non-max suppression
*/

class MyHarrisDetector
{
private:

public:

	/*		Run		*/
	void Run(char *srcPath, int kernelsize)
	{
		// create var
		SupportFunctions sf;
		MyGaussianBlur gb;
		vector<double> Gx, Gy, G, Sxx, Syy, Sxy, R;

		// blur image
		Mat x = gb.Run(3, srcPath);
		sf.FindGradient(x, Gx, Gy, G, kernelsize, false);

		// compute sum of products of derivatives every pixels 
		sf.CreateSx2Sy2Sxy(x.cols, x.rows, Gx, Gy, G, Sxx, Syy, Sxy);

		// computer response of the detector at each pixel

	}
};