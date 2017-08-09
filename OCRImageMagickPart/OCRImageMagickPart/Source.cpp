#include <Magick++.h> 
#include <Magick++\Image.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace Magick;


// get filename betwwen first '.' from the end of name and '\' or '/' from the end of name 
string ExtractNameOfFileFromPathIn(string pathIn);

// file input must be .jpg or .tiff type to get the dpi = 300.
// if file input is .png type, it will be > 400 dpi
bool ResampleOneFile(string pathIn, string pathOut);

bool ResampleFolder(string resampleFolder, string filelist, string fileresamplelist);

bool RecognizeEngOneFile(string fullfilename, string pathOut);

bool RecognizeVieOneFile(string fullfilename, string pathOut);

// require default extension is .tiff
bool RecognizeEngFolder(string recognizeEngFolder, string fileresamplelist);

// require default extension is .tiff
bool RecognizeVieFolder(string recognizeEngFolder, string fileresamplelist);

bool CheckTextDetectionInFileName(string filename);

void MergeVieAndEngTxt(string fileresamplelist, string summaryfilePath, string cropboxesresamplefolder);

int main()
{
	cout << "Hello world." << endl;
	string cropboxesfolder = "D:\\Document\\Thesis-2017\\Data\\Result-cropboxes-experiment\\";
	string cropboxesresamplefolder = "D:\\Document\\Thesis-2017\\Data\\Result-cropboxes-resample-experiment\\";
	string resampleComputingTimeFolder = "D:\\Document\\Thesis-2017\\Data\\Result-resample-calculating-run-time\\";
	string cropboxesrecognizefolder = "D:\\Document\\Thesis-2017\\Data\\Result-cropboxes-resample-recognize-experiment\\";
	string filelist = "E:\\Code\\OCR-Five-Git\\OCRImageMagickPart\\OCRImageMagickPart\\file.txt";
	string fileresamplelist = "E:\\Code\\OCR-Five-Git\\OCRImageMagickPart\\OCRImageMagickPart\\file-resample.txt";
	string filesummary = "D:\\Document\\Thesis-2017\\Data\\Result-cropboxes-resample-recognize-experiment\\Summary-Resample-Recognize-Purely.txt";
	// resample the crop boxes folder
	ResampleFolder(resampleComputingTimeFolder, filelist, fileresamplelist);
	
	// recognize text in the resample folder
	// need to bring the .exe file to the folder resampling
	//RecognizeEngFolder(cropboxesresamplefolder, fileresamplelist);
	//RecognizeVieFolder(cropboxesresamplefolder, fileresamplelist);

	// merge -vie and -eng into 1 file
	//MergeVieAndEngTxt(fileresamplelist, cropboxesrecognizefolder, cropboxesresamplefolder);


	return 0;
}

string ExtractNameOfFileFromPathIn(string pathIn)
{
	int leng = pathIn.length();
	int pos = leng - 1;
	do
	{
		pos--;
	} while (pathIn[pos] != '.' && pos > 0);
	// pos at char '.'
	int pos2 = pos - 1;
	do
	{
		pos2--;
	} while (pathIn[pos2] != '\\' && pathIn[pos2] != '/' && pos2 > 0);
	// pos2 at char '\'
	pos2++;
	// get name
	vector<char> tmp;
	for (int i = pos2; i < pos; i++)
	{
		tmp.push_back(pathIn[i]);
	}
	int tmplen = tmp.size();
	char *tmp2 = new char[tmplen + 1];
	for (int i = 0; i < tmplen; i++)
	{
		tmp2[i] = tmp[i];
	}
	tmp2[tmplen] = '\0';
	tmp.clear();
	return string(tmp2);
}

bool ResampleOneFile(string pathIn, string pathOut)
{
	Image image;
	try {
		// Read a file into image object
		image.read(pathIn);
		Magick::Geometry gm = image.size();
		image.resample(Point(300, 300));
		image.resize(gm);
		image.write(pathOut);
		return true;
	}
	catch (Exception &error_)
	{
		cout << "Caught exception: " << error_.what() << endl;
		return false;
	}
}

bool ResampleFolder(string resampleFolder, string filelist, string fileresamplelist)
{
	vector<string> PathIn;
	// read PathIn
	cout << "Read file in." << endl;
	ifstream ifs;
	ifs.open(filelist);
	if (ifs.is_open())
	{
		string line;
		while (getline(ifs, line))
		{
			PathIn.push_back(line);
			//line.clear();
		}
		ifs.close();
	}
	else
	{
		cout << "Cannot open filelist: " << filelist << endl;
		return false;
	}
	// start resample
	cout << "Start resampling each file." << endl;
	vector<double> resampleTime;
	int k = PathIn.size();
	for (int i = 0; i < k; i++)
	{
		cout << "Run index " << i << endl;
		string name = ExtractNameOfFileFromPathIn(PathIn[i]);
		if (CheckTextDetectionInFileName(name) == true)
		{
			continue;
		}
		string pathOut = resampleFolder + name + ".tiff";
		time_t begin, end;
		time(&begin);
		bool check = ResampleOneFile(PathIn[i], pathOut);
		time(&end);
		resampleTime.push_back(difftime(end, begin));
		if (check == false)
		{
			cout << "False 1 file above here: " << PathIn[i] << endl;
		}
	}
	// compute the average running time
	cout << "start writing calculating time." << endl;
	double average = 0;
	for (int i = 0; i < k; i++)
	{
		average = average + resampleTime[i];
	}
	average = average / k;
	// write time calculating to file
	ofstream ofs;
	ofs.open(resampleFolder + "running-time.txt");
	if (ofs.is_open())
	{
		ofs << "AVERAGE-TIME-RUNNING: " << average << " seconds.\n";
		for (int i = 0; i < k; i++)
		{
			ofs << PathIn[i] << "\n";
			ofs << "TIME-RUNNING: " << resampleTime[i] << " seconds.\n";
		}
		ofs.close();
	}
	resampleTime.clear();
	PathIn.clear();
	return true;

}

bool RecognizeEngOneFile(string fullfilename, string pathOut)
{
	string command = "tesseract " + fullfilename + " " + pathOut + " -l eng -psm 7";
	char *cmd = new char[command.length() + 1];
	for (int i = 0; i < command.length(); i++)
	{
		cmd[i] = command[i];
	}
	cmd[command.length()] = 0;
	system(cmd);
	delete cmd;
	return true;
}

bool RecognizeVieOneFile(string fullfilename, string pathOut)
{
	string command = "tesseract " + fullfilename + " " + pathOut + " -l vie -psm 7";
	char *cmd = new char[command.length() + 1];
	for (int i = 0; i < command.length(); i++)
	{
		cmd[i] = command[i];
	}
	cmd[command.length()] = 0;
	system(cmd);
	delete cmd;
	return true;
}

bool RecognizeEngFolder(string recognizeEngFolder, string fileresamplelist)
{
	vector<string> PathIn;
	// read PathIn
	cout << "Read file in." << endl;
	ifstream ifs;
	ifs.open(fileresamplelist);
	if (ifs.is_open())
	{
		string line;
		while (getline(ifs, line))
		{
			PathIn.push_back(line);
			//line.clear();
		}
		ifs.close();
	}
	else
	{
		cout << "Cannot open filelist: " << fileresamplelist << endl;
		return false;
	}
	// start recognize
	int k = PathIn.size();
	for (int i = 0; i < k; i++)
	{
		cout << "Run index " << i << endl;
		string name = ExtractNameOfFileFromPathIn(PathIn[i]);

		string pathOut = recognizeEngFolder + name + "-eng";
		bool check = RecognizeEngOneFile(name + ".tiff", pathOut);
		if (check == false)
		{
			cout << "False 1 file above here: " << PathIn[i] << endl;
		}
	}

	return true;
}

bool RecognizeVieFolder(string recognizeEngFolder, string fileresamplelist)
{
	vector<string> PathIn;
	// read PathIn
	cout << "Read file in." << endl;
	ifstream ifs;
	ifs.open(fileresamplelist);
	if (ifs.is_open())
	{
		string line;
		while (getline(ifs, line))
		{
			PathIn.push_back(line);
			//line.clear();
		}
		ifs.close();
	}
	else
	{
		cout << "Cannot open filelist: " << fileresamplelist << endl;
		return false;
	}
	// start recognize
	int k = PathIn.size();
	for (int i = 0; i < k; i++)
	{
		cout << "Run index " << i << endl;
		string name = ExtractNameOfFileFromPathIn(PathIn[i]);

		string pathOut = recognizeEngFolder + name + "-vie";
		bool check = RecognizeVieOneFile(name + ".tiff", pathOut);
		if (check == false)
		{
			cout << "False 1 file above here: " << PathIn[i] << endl;
		}
	}

	return true;
}

bool CheckTextDetectionInFileName(string filename)
{
	int k = filename.length();
	if (filename[k - 1] == 'n' && filename[k-2] == 'o' && filename[k - 3] == 'i' && filename[k - 4] == 't')
		return true;
	return false;
}

void MergeVieAndEngTxt(string fileresamplelist, string summaryfilePath, string cropboxesresamplefolder)
{
	vector<string> PathIn;
	// read PathIn
	cout << "Read file in." << endl;
	ifstream ifs;
	ifs.open(fileresamplelist);
	if (ifs.is_open())
	{
		string line;
		while (getline(ifs, line))
		{
			PathIn.push_back(line);
			//line.clear();
		}
		ifs.close();
	}
	else
	{
		cout << "Cannot open filelist: " << fileresamplelist << endl;
	}

	int k = PathIn.size();
	for (int i = 0; i < k; i++)
	{
		string name = ExtractNameOfFileFromPathIn(PathIn[i]);
		string engcontent;
		wstring viecontent;
		// read eng
		ifstream ifs;
		ifs.open(cropboxesresamplefolder + name + "-eng.txt");
		if (ifs.is_open())
		{
			getline(ifs, engcontent);
			ifs.close();
		}
		// read vie
		std::wifstream wif;
		wif.open(cropboxesresamplefolder + name + "-vie.txt");
		if (wif.is_open())
		{
			getline(wif, viecontent);
			wif.close();
		}
		// append to file summary
		ofstream ofs;
		ofs.open(summaryfilePath, std::ofstream::app);
		if (ofs.is_open())
		{
			

			ofs.close();
		}
	}
}

// resample code
/*
Image image;
try {
// Read a file into image object
image.read("E:\\Code\\OCRImageMagickPart\\OCRImageMagickPart\\img24.jpg");
Magick::Geometry gm = image.size();
image.resample(Point(300, 300));
image.resize(gm);
image.write("img24-2.tiff");
}
catch (Exception &error_)
{
cout << "Caught exception: " << error_.what() << endl;
}
*/