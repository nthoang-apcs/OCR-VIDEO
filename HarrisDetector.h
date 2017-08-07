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

/*		How it work	 - not finish yet	*/
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
		// clear gx gy g
		Gx.clear();
		Gy.clear();
		G.clear();
		// computer response of the detector at each pixel
		sf.ComputeResponseOfDetector(x.cols, x.rows, Sxx, Syy, Sxy, R);
		// Threshold on value of R
	}
};


/*		Harris detector for c# sample		*/

/*
/// Process image looking for corners.
/// </summary>
///
/// <param name="image">Source image data to process.</param>
///
/// <returns>Returns list of found corners (X-Y coordinates).</returns>
///
/// <exception cref="UnsupportedImageFormatException">
///   The source image has incorrect pixel format.
/// </exception>
///
public unsafe List<IntPoint> ProcessImage(UnmanagedImage image)
{
	// check image format
	if (
			(image.PixelFormat != PixelFormat.Format8bppIndexed) &amp;&amp;
			(image.PixelFormat != PixelFormat.Format24bppRgb) &amp;&amp;
			(image.PixelFormat != PixelFormat.Format32bppRgb) &amp;&amp;
			(image.PixelFormat != PixelFormat.Format32bppArgb)
		)
	{
		throw new UnsupportedImageFormatException("Unsupported pixel format of the source image.");
	}

	// make sure we have grayscale image
	UnmanagedImage grayImage = null;

	if (image.PixelFormat == PixelFormat.Format8bppIndexed)
	{
		grayImage = image;
	}
	else
	{
		// create temporary grayscale image
		grayImage = Grayscale.CommonAlgorithms.BT709.Apply(image);
	}


	// get source image size
	int width = grayImage.Width;
	int height = grayImage.Height;
	int srcStride = grayImage.Stride;
	int srcOffset = srcStride - width;


	// 1. Calculate partial differences
	float[,] diffx = new float[height, width];
	float[,] diffy = new float[height, width];
	float[,] diffxy = new float[height, width];


	fixed (float* pdx = diffx, pdy = diffy, pdxy = diffxy)
	{
		byte* src = (byte*)grayImage.ImageData.ToPointer() + srcStride + 1;

		// Skip first row and first column
		float* dx = pdx + width + 1;
		float* dy = pdy + width + 1;
		float* dxy = pdxy + width + 1;

		// for each line
		for (int y = 1; y < height - 1; y++)
		{
			// for each pixel
			for (int x = 1; x < width - 1; x++, src++, dx++, dy++, dxy++)
			{
				// Convolution with horizontal differentiation kernel mask
				float h = ((src[-srcStride + 1] + src[+1] + src[srcStride + 1]) -
					(src[-srcStride - 1] + src[-1] + src[srcStride - 1])) * 0.166666667f;

				// Convolution vertical differentiation kernel mask
				float v = ((src[+srcStride - 1] + src[+srcStride] + src[+srcStride + 1]) -
					(src[-srcStride - 1] + src[-srcStride] + src[-srcStride + 1])) * 0.166666667f;

				// Store squared differences directly
				*dx = h * h;
				*dy = v * v;
				*dxy = h * v;
			}

			// Skip last column
			dx++; dy++; dxy++;
			src += srcOffset + 1;
		}

		// Free some resources which wont be needed anymore
		if (image.PixelFormat != PixelFormat.Format8bppIndexed)
			grayImage.Dispose();
	}


	// 2. Smooth the diff images
	if (sigma > 0.0)
	{
		float[,] temp = new float[height, width];

		// Convolve with Gaussian kernel
		convolve(diffx, temp, kernel);
		convolve(diffy, temp, kernel);
		convolve(diffxy, temp, kernel);
	}


	// 3. Compute Harris Corner Response Map
	float[,] map = new float[height, width];

	fixed (float* pdx = diffx, pdy = diffy, pdxy = diffxy, pmap = map)
	{
		float* dx = pdx;
		float* dy = pdy;
		float* dxy = pdxy;
		float* H = pmap;
		float M, A, B, C;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++, dx++, dy++, dxy++, H++)
			{
				A = *dx;
				B = *dy;
				C = *dxy;

				if (measure == HarrisCornerMeasure.Harris)
				{
					// Original Harris corner measure
					M = (A * B - C * C) - (k * ((A + B) * (A + B)));
				}
				else
				{
					// Harris-Noble corner measure
					M = (A * B - C * C) / (A + B + Accord.Math.Special.SingleEpsilon);
				}

				if (M > threshold)
				{
					*H = M; // insert value in the map
				}
			}
		}
	}


	// 4. Suppress non-maximum points
	List<IntPoint> cornersList = new List<IntPoint>();

	// for each row
	for (int y = r, maxY = height - r; y < maxY; y++)
	{
		// for each pixel
		for (int x = r, maxX = width - r; x < maxX; x++)
		{
			float currentValue = map[y, x];

			// for each windows' row
			for (int i = -r; (currentValue != 0) &amp;&amp; (i <= r); i++)
			{
				// for each windows' pixel
				for (int j = -r; j <= r; j++)
				{
					if (map[y + i, x + j] > currentValue)
					{
						currentValue = 0;
						break;
					}
				}
			}

			// check if this point is really interesting
			if (currentValue != 0)
			{
				cornersList.Add(new IntPoint(x, y));
			}
		}
	}


	return cornersList;
}


///				Usage

C#

// Open a image
Bitmap image = ...

// Create a new Harris Corners Detector using the given parameters
HarrisCornersDetector harris = new HarrisCornersDetector(k)
{
	Threshold = threshold,
	Sigma = sigma
};

// Create a new AForge's Corner Marker Filter
CornersMarker corners = new CornersMarker(harris, Color.White);

// Apply the filter and display it on a picturebox
pictureBox1.Image = corners.Apply(image);


*/