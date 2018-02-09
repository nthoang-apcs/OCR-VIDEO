#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\imgcodecs.hpp>

using namespace std;

//Adapted from BBoxStructure.h
struct tsRect
{
	string rectID;
	string originalName;
	int nX, oX;
	int nY, oY;
	int nWidth, oWidth;
	int nHeight, oHeight;
	tsRect()
	{
		nX = oX = 0;
		nY = oY = 0;
		nWidth = oWidth = 0;
		nHeight = oHeight = 0;
		rectID = "";
		originalName = "";
	}
	tsRect(int x, int y, int width, int height, string rectID, string originalName) :
		rectID(rectID), originalName(originalName)
	{
		oX = x;
		oY = y;
		oWidth = width;
		oHeight = height;
		nX = nY = nWidth = nHeight = 0;
	}

	string NewRectStr() {
		auto ss = stringstream();
		ss << nX << " " << nY << " " << nWidth << " " << nHeight;
		return ss.str();
	}

	string OldRectStr() {
		auto ss = stringstream();
		ss << oX << " " << oY << " " << oWidth << " " << oHeight;
		return ss.str();
	}


	int GetArea()
	{
		return (nWidth*nHeight);
	}
};

struct OCRResult {
	string rectID;
	string ocrText;
	OCRResult(string rectID, string ocrText) :
		rectID(rectID), ocrText(ocrText) { }
};

class CheckVowel {
private:
	string RootPath;
	string TmpCheckVowelPath;
	string TmpRectPath;
	string TmpImagePath;

public:
	
	CheckVowel(string rootPath): RootPath(rootPath) {
		TmpCheckVowelPath = RootPath + "\\TmpCheckVowel";
		TmpRectPath = RootPath + "\\TmpRect";
		TmpImagePath = RootPath + "\\TmpImage";
	}

	//line format in TmpCheckVowel: {RectID}.txt

	//Read from TmpChckVowel/InputCVowel a list of RectID (output from OCR) in TmpImage folder 
	vector<string> GetRectIDList() {
		ifstream ifs = ifstream(TmpCheckVowelPath + "\\InputCVowel.txt");
		vector<string> result;
		if (ifs.is_open()) {
			string line;
			while (ifs >> line) {
				//Remove the '.txt' extension from the string
				result.push_back(line.substr(0, line.length() - 4));
			}
			ifs.close();
		}
		else {
			cout << "ERROR: Cannot open file " << TmpCheckVowelPath + "\\InputCVowel.txt\n";
		}
		return result;
	}

	vector<OCRResult> GetOCRTextFromRectIDs(const vector<string>& rectIds) {
		vector<OCRResult> result;
		for (auto rectID : rectIds) {
			ifstream ifs = ifstream(TmpImagePath + "\\" + rectID + ".txt");
			if (ifs.is_open()) {
				string line;
				while (ifs >> line) {
					result.push_back(OCRResult(rectID, line));
				}
				ifs.close();
			}
			else {
				cout << "ERROR: Cannot read file: " << rectID + ".txt" << endl;
			}
		}
		return result;
	}

	struct tsRectComparator {
		bool operator() (const tsRect& r1, const tsRect& r2) { return (r1.rectID < r2.rectID); }
	} tsRectComp;

	void ProcessEachLine(ifstream& ifs, 
						 const vector<string>& rectids,
						 unordered_map<string, tsRect>& output) 
	{
		string line;
		string curid = "", curOrignalName = "";
		int curX, curY, curW, curH;
		while (ifs >> line) {
			if (line.substr(0, 4) == "<ID>") {
				//Get the inner text of the tag <ID>
				string tmp_curid = line.substr(4, line.length() - 9);
				//if (std::find(rectids.begin(), rectids.end(), tmp_curid) != rectids.end()) {
				curid = tmp_curid;
				//}
				//else {
				//	cout << "ERROR: Rect ID: " << tmp_curid << " not found in list of IDs in InputCVowel.txt";
				//}
			}
			else if (line.substr(0, 5) == "<ROI>") {
				//Get the inner text of the tag <ROI>
				string inner = line.substr(5, line.length() - 11);
				stringstream ss = stringstream(inner);
				ss >> curX >> curY >> curW >> curH;
			}
			else if (line.substr(0, 6) == "<Name>") {
				curOrignalName = line.substr(6, line.length() - 13);
			}
			else if (line == "</Rect>") {
				output.insert(std::pair<string, tsRect>(
					curid, 
					tsRect(curX, curY, curW, curH, curid, curOrignalName)));
			}
			else if (line == "<Rect>") {
				curid = "";
				curOrignalName = "";
				curX = curY = curW = curH = 0;
			}
			else
			{
				cout << "ERROR, malformed line format, skipping: " << line << endl;
			}
		}
	}

	void WriteRectsToFile(ofstream& ofs, const unordered_map<string, tsRect>& input) {
		for (auto it = input.begin(); it != input.end(); it++) {
			auto rect = it->second;
			ofs << "<Rect>";
			ofs << "<ID>" << rect.rectID << "</ID>\n";
			ofs << "<ROI>" << rect.oX << " " << rect.oY << " " << rect.oWidth << " " << rect.oHeight << "</ROI>\n";
			ofs << "<NewROI>" << rect.nX << " " << rect.nY << " " << rect.nWidth << " " << rect.nHeight << "</NewROI>\n";
			ofs << "<Name>" << rect.originalName << "</Name>\n";
			ofs << "<NumberVersion>2</NumberVersion>\n";
			ofs << "<TimeRunning></TimeRunning>\n";
			ofs << "</Rect>\n";
		}
	}

	void FindAllRectContainingVowel(vector<string>& rectids, 
		unordered_map<string, tsRect>& outputLines, unordered_map<string, tsRect>& outputOtherBoxes
	) {
		ifstream ifs = ifstream(TmpRectPath + "\\Lines.txt");
		if (ifs.is_open()) {
			cout << "INFO: Read Rect Data from Lines.txt\n";
			ProcessEachLine(ifs, rectids, outputLines);
			ifs.close();
		}
		else {
			cout << "ERROR: Cannot open file " << TmpRectPath + "\\Lines.txt\n";
		}
		ifs = ifstream(TmpRectPath + "\\OtherBoxes.txt");
		if (ifs.is_open()) {
			cout << "INFO: Read Rect Data from OtherBoxes.txt\n";
			ProcessEachLine(ifs, rectids, outputOtherBoxes);
			ifs.close();
		}
		else {
			cout << "ERROR: Cannot open file " << TmpRectPath + "\\OtherBoxes.txt\n";
		}

	}

	void WriteAllRectsData(
		const unordered_map<string, tsRect>& lines, const unordered_map<string, tsRect>& otherboxes) 
	{
		ofstream ofs = ofstream(TmpRectPath + "\\Lines.txt");
		if (ofs.is_open()) {
			WriteRectsToFile(ofs, lines);
			ofs.close();
		}
		else {
			cout << "ERROR: Cannot open file" << TmpRectPath + "/Lines.txt\n";
		}

		ofs = ofstream(TmpRectPath + "\\OtherBoxes.txt");
		if (ofs.is_open()) {
			WriteRectsToFile(ofs, otherboxes);
			ofs.close();
		}
		else {
			cout << "ERROR: Cannot open file " << TmpRectPath + "/OtherBoxes.txt\n";
		}
	}

	bool HasVowel(string ocrText) {
		string vowels[12] = { "a", "e", "i", "o", "u", "y", "A", "E", "I", "O", "u", "Y" };
		for (int i = 0; i < 12; i++) {
			if (ocrText.find(vowels[i]) != string::npos) return true;
		}
		return false;
	}

	void IncreaseRect(tsRect& rect) {
		if (rect.oHeight < 10 && rect.oHeight < 10) {
			rect.nHeight = rect.oHeight + 4;
			rect.nY = rect.oY - 2;
		}
		else {
			rect.nHeight = rect.oHeight + rect.oHeight * 0.15 * 2;
			rect.nY = rect.oY - rect.oY * 0.15;
		}
	}

	void WriteAllRectImgs(
		const unordered_map<string, tsRect>& lines, const unordered_map<string, tsRect>& otherboxes)
	{
		for (auto it = lines.begin(); it != lines.end(); it++) {
			auto rect = it->second;
			string originImgPath = TmpImagePath + "\\" + rect.originalName + ".jpg";
			string rectNewFileName = TmpImagePath + "\\" + rect.originalName + "-" + rect.rectID + "-2.jpg";
			cv::Mat originImg = cv::imread(originImgPath);
			cv::Mat newROI = originImg(cv::Rect(rect.nX, rect.nY, rect.nWidth, rect.nHeight));
			cout << "INFO: Write Rect Image " << rectNewFileName << " ROI(" << rect.NewRectStr() << ")\n";
			cv::imwrite(rectNewFileName, newROI);
		}
		for (auto it = otherboxes.begin(); it != otherboxes.end(); it++) {
			auto rect = it->second;
			string originImgPath = TmpImagePath + "\\" + rect.originalName + ".jpg";
			string rectNewFileName = TmpImagePath + "\\" + rect.originalName + "-" + rect.rectID + "-2.jpg";
			cv::Mat originImg = cv::imread(originImgPath);
			cv::Mat newROI = originImg(cv::Rect(rect.nX, rect.nY, rect.nWidth, rect.nHeight));
			cout << "INFO: Write Rect Image " << rectNewFileName << " ROI(" << rect.NewRectStr() << ")\n";
			cv::imwrite(rectNewFileName, newROI);
		}
	}

	void Run() {
		auto rectIDList = GetRectIDList();
		unordered_map<string, tsRect> lineSet, otherboxSet;
		FindAllRectContainingVowel(rectIDList, lineSet, otherboxSet);
		auto ocrResults = GetOCRTextFromRectIDs(rectIDList);
		
		//Increase Rect whom OCR text contains vowel
		for (auto ocrResultIt = ocrResults.begin(); ocrResultIt != ocrResults.end(); ocrResultIt++) {
			if (HasVowel(ocrResultIt->ocrText)) {
				auto tsRectIt = lineSet.find(ocrResultIt->rectID);
				if (tsRectIt != lineSet.end()) {
					auto rectContent = tsRectIt->second;
					auto rectId = tsRectIt->first;
					IncreaseRect(rectContent);
					cout << "INFO: Rect Line " << rectId << " increased from " << rectContent.OldRectStr()
						 << " to " << rectContent.NewRectStr() << "\n";
				}
				else {
					tsRectIt = otherboxSet.find(ocrResultIt->rectID);
					auto rectId = tsRectIt->first;
					if (tsRectIt != otherboxSet.end()) {
						auto rectContent = tsRectIt->second;
						IncreaseRect(rectContent);
						cout << "INFO: Rect OtherBox " << rectId << " increased from " << rectContent.OldRectStr()
							<< " to " << rectContent.NewRectStr() << "\n";
					}
					else {
						cout << "ERROR: " << rectId << " not found in both lineSet and otherboxSet.\n";
					}
				}
			}
		}
	
		//Write all rects to 2 files Lines.txt and OtherBoxes.txt
		WriteAllRectsData(lineSet, otherboxSet);
		//Crop and write all rect images to TmpImage
		WriteAllRectImgs(lineSet, otherboxSet);
	}
};