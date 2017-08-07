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
#include <cstdlib>
#include "SupportFunctions.h"
#include "GaussianBlur.h"
#include "CannyAlgorithms.h"
using namespace std;
using namespace cv;


class AssignmentTwo
{
private:
	


public:
	AssignmentTwo()
	{

	}

	void Run(char* cmd)
	{
		if (cmd == NULL)
		{
			return;
		}
		int i = 0;
		int k = strlen(cmd);
		if (cmd[i] == 'm')
		{
			/*
			- m harris sift image1.jpg image2.jpg match and show results of image1 and image2 using Harris detector and SIFT descriptor.
			- m dog sift image1.jpg image2.jpg - match and show results of image1 and image2 using DoG detector and SIFT descriptor.
			- m blob sift image1.jpg image2.jpg - match and show results of image1 and image2 using using Blob detector and SIFT descriptor.
			- m harris lbp image1.jpg image2.jpg - match and show results of image1 and image2 using Harris detector and LBP descriptor.
			- m dog lbp image1.jpg image2.jpg - match and show results of image1 and image2 using DoG detector and LBP descriptor.
			- m blob lbp image1.jpg image2.jpg - match and show results of image1 and image2 using Blob detector and LBP descriptor.
			*/
			i++;
			char img1[200];
			char img2[200];
			if (cmd[i] != ' ')
			{
				cout << "Wrong command! Right format command: m <keyword-1> <keyword-2> <name-image-1 with extension> <name-image-2 with extension> with keyword-1 is harris or dog or blob; keyword-2 is sift or lbp." << endl;
				return;
			}
			// ignore space
			while (i < k && cmd[i] == ' ')
			{
				i++;
			}
			// minimum number chars for a command
			if (i < 12)
			{
				cout << "Wrong command! Right format command: m <keyword-1> <keyword-2> <name-image-1 with extension> <name-image-2 with extension> with keyword-1 is \"harris\" or \"dog\" or \"blob\"; keyword-2 is \"sift\" or \"lbp\"." << endl;
				return;
			}
			if (cmd[i] == 'h' && cmd[i + 1] == 'a' && cmd[i + 2] == 'r'&& cmd[i + 3] == 'r' && cmd[i + 4] == 'i' && cmd[i + 5] == 's')
			{
				i = i + 6;
				while (i < k && cmd[i] == ' ')
				{
					i++;
				}
				// check number char for sift or lbp
				if ((i + 4) >= k)
				{
					cout << "Wrong command! Right format command: m <keyword-1> <keyword-2> <name-image-1 with extension> <name-image-2 with extension> with keyword-1 is \"harris\" or \"dog\" or \"blob\"; keyword-2 is \"sift\" or \"lbp\"." << endl;
					return;
				}
				if (cmd[i] == 's' && cmd[i + 1] == 'i' && cmd[i + 2] == 'f' && cmd[i + 3] == 't')
				{
					// m harris sift
					i = i + 4;
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					int l1 = 0;
					int l2 = 0;
					// read first image
					while (i < k && cmd[i] != ' ')
					{
						img1[l1] = cmd[i];
						l1++;
						i++;
					}
					// ignore space
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read second image
					while (i < k && cmd[i] != ' ')
					{
						img2[l2] = cmd[i];
						l2++;
						i++;
					}
					// do work


				}
				else if (cmd[i] == 'l' && cmd[i + 1] == 'b' && cmd[i + 2] == 'p')
				{
					//m harris lbp
					i = i + 3;
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					int l1 = 0;
					int l2 = 0;
					// read first image
					while (i < k && cmd[i] != ' ')
					{
						img1[l1] = cmd[i];
						l1++;
						i++;
					}
					// ignore space
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read second image
					while (i < k && cmd[i] != ' ')
					{
						img2[l2] = cmd[i];
						l2++;
						i++;
					}
					// do work


				}
			}
			else if (cmd[i] == 'd' && cmd[i + 1] == 'o' && cmd[i + 2] == 'g')
			{
				i = i + 3;
				while (i < k && cmd[i] == ' ')
				{
					i++;
				}
				int l1 = 0;
				int l2 = 0;
				// check number char for sift or lbp
				if ((i + 4) >= k)
				{
					cout << "Wrong command! Right format command: m <keyword-1> <keyword-2> <name-image-1 with extension> <name-image-2 with extension> with keyword-1 is \"harris\" or \"dog\" or \"blob\"; keyword-2 is \"sift\" or \"lbp\"." << endl;
					return;
				}
				if (cmd[i] == 's' && cmd[i + 1] == 'i' && cmd[i + 2] == 'f' && cmd[i + 3] == 't')
				{
					// m dog sift
					i = i + 4;
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read first image
					while (i < k && cmd[i] != ' ')
					{
						img1[l1] = cmd[i];
						l1++;
						i++;
					}
					// ignore space
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read second image
					while (i < k && cmd[i] != ' ')
					{
						img2[l2] = cmd[i];
						l2++;
						i++;
					}
					// do work



				}
				else if (cmd[i] == 'l' && cmd[i + 1] == 'b' && cmd[i + 2] == 'p')
				{
					// m dog lbp
					i = i + 3;
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read first image
					while (i < k && cmd[i] != ' ')
					{
						img1[l1] = cmd[i];
						l1++;
						i++;
					}
					// ignore space
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read second image
					while (i < k && cmd[i] != ' ')
					{
						img2[l2] = cmd[i];
						l2++;
						i++;
					}
					// do work


				}
			}
			else if (cmd[i] == 'b' && cmd[i + 1] == 'l' && cmd[i + 2] == 'o' && cmd[i + 3] == 'g')
			{
				i = i + 4;
				while (i < k && cmd[i] == ' ')
				{
					i++;
				}
				int l1 = 0;
				int l2 = 0;
				// check number char for sift or lbp
				if ((i + 4) >= k)
				{
					cout << "Wrong command! Right format command: m <keyword-1> <keyword-2> <name-image-1 with extension> <name-image-2 with extension> with keyword-1 is \"harris\" or \"dog\" or \"blob\"; keyword-2 is \"sift\" or \"lbp\"." << endl;
					return;
				}
				if (cmd[i] == 's' && cmd[i + 1] == 'i' && cmd[i + 2] == 'f' && cmd[i + 3] == 't')
				{
					// m blob sift
					i = i + 4;
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read first image
					while (i < k && cmd[i] != ' ')
					{
						img1[l1] = cmd[i];
						l1++;
						i++;
					}
					// ignore space
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read second image
					while (i < k && cmd[i] != ' ')
					{
						img2[l2] = cmd[i];
						l2++;
						i++;
					}
					// do work


				}
				else if (cmd[i] == 'l' && cmd[i + 1] == 'b' && cmd[i + 2] == 'p')
				{
					// m blob lbp
					i = i + 3;
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read first image
					while (i < k && cmd[i] != ' ')
					{
						img1[l1] = cmd[i];
						l1++;
						i++;
					}
					// ignore space
					while (i < k && cmd[i] == ' ')
					{
						i++;
					}
					// read second image
					while (i < k && cmd[i] != ' ')
					{
						img2[l2] = cmd[i];
						l2++;
						i++;
					}
					// do work


				}
			}
			else
			{
				cout << "Wrong command! Right format command: m <keyword-1> <keyword-2> <name-image-1 with extension> <name-image-2 with extension> with keyword-1 is harris or dog or blob; keyword-2 is sift or lbp." << endl;
				return;
			}

		}
		/*
		- harris image.jpg detect key points using harris algorithm and show the keypoints in original image.
		- blob image.jpg - detect key points using blob algorithm and show the keypoints in original image.
		- dog image.jpg detect key points using DoG Algorithm and show keypoints in original image.
		*/
		else if (cmd[i] == 'h')
		{

		}
		else if (cmd[i] == 'b')
		{

		}
		else if (cmd[i] == 'd')
		{

		}
	}



	~AssignmentTwo()
	{

	}
};