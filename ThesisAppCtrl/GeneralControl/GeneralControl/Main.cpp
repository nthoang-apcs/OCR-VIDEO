#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "MyProcess.h"
#include "MyMSER.h"

using namespace std;
using namespace cv;


/////////////////////////////////
/*
- Input argument is the file path of filecapture.txt
- The return value of this main function is 1 if success, 0 if failed.
*/
////////////////////////////////

int main(int argc, const char * argv[])
{
	if (argc < 2)
	{
		return 0;
	}

	
	/*		Get paths of files		*/
	char *fCapturePath = new char[200];
	vector<string> fPaths;
	string tmp1;
	ifstream ifs(fCapturePath);

	if (ifs.is_open())
	{
		while (getline(ifs, tmp1))
		{
			fPaths.push_back(tmp1);
		}
		ifs.close();
	}
	else
	{
		return 0;
	}
	delete[] fCapturePath;


	/*		MSER		*/




	return 1;
}