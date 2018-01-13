#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

namespace std {

	/*
		Courtesy to https://stackoverflow.com/questions/19603975/binarysearch-returning-index-of-where-it-belongs
	*/
	template<class RandomIt, class T>
	RandomIt binary_locate(RandomIt first, RandomIt last, const T& val) {
		if (val == *first) return first;
		auto d = std::distance(first, last);
		if (d == 1) return first;
		auto center = (first + (d / 2));
		if (val < *center) return binary_locate(first, center, val);
		return binary_locate(center, last, val);
	}

}

namespace CheckVowel {

	//Adapted from BBoxStructure.h
	struct tsRect
	{
		string rectID;
		int nX;
		int nY;
		int nWidth;
		int nHeight;
		tsRect()
		{
			nX = 0;
			nY = 0;
			nWidth = 0;
			nHeight = 0;
			rectID = "";
		}
		tsRect(int x, int y, int width, int height, string rectID) : rectID(rectID)
		{
			nX = x;
			nY = y;
			nWidth = width;
			nHeight = height;
		}
		string ToString() {
			auto ss = stringstream();
			ss << nX << " " << nY << " " << nWidth << " " << nHeight;
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
		OCRResult(string rectID, string ocrText):
			rectID(rectID), ocrText(ocrText) { }
	};

	const string TmpCheckVowelPath = "./Root/TmpCheckVowel";
	const string TmpRectPath = "./Root/TmpRect";
	const string TmpImagePath = "./Root/TmpImage";
	
	//line format in TmpCheckVowel: {RectID}.txt

	//Read from TmpChckVowel/InputCVowel a list of RectID (output from OCR) in TmpImage folder 
	vector<string> GetRectIDList() {
		ifstream ifs = ifstream(CheckVowel::TmpCheckVowelPath + "/InputCVowel.txt");
		vector<string> result;
		if (ifs.is_open()) {
			string line;
			while (ifs >> line) {
				//Remove the '.txt' extension from the string
				result.push_back(line.substr(0, line.length() - 4));
			}
			ifs.close();
		}
		return result;
	}

	vector<OCRResult> GetOCRTextFromRectIDs(const vector<string>& rectIds) {
		vector<OCRResult> result;
		for (auto rectID : rectIds) {
			ifstream ifs = ifstream(CheckVowel::TmpImagePath + "/" + rectID + ".txt");
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

	void FindAllRectContainingVowel(vector<string>& rectids, 
		unordered_map<string, tsRect>& outputLines, unordered_map<string, tsRect>& outputOtherBoxes
	) {
		ifstream ifs = ifstream(CheckVowel::TmpRectPath + "/OtherBoxes.txt");
		if (ifs.is_open()) {
			string line;
			string curid = "";
			while (ifs >> line) {
				if (line.substr(0, 4) == "<ID>") {
					//Get the inner text of the tag <ID>
					string tmp_curid = line.substr(4, line.length() - 5);
					if (std::find(rectids.begin(), rectids.end(), tmp_curid) != rectids.end()) {
						curid = tmp_curid;
					}
				}
				else if (line.substr(0, 5) == "<ROI>" && curid != "") {
					//Get the inner text of the tag <ROI>
					string inner = line.substr(5, line.length() - 6);
					stringstream ss = stringstream(inner);
					int x, y, w, h;
					ss >> x >> y >> w >> h;
					outputLines.insert(std::pair<string, tsRect>(curid, tsRect(x, y ,w, h, curid)));
					curid = "";
				}
			}
			ifs.close();
		}

		ifs = ifstream(CheckVowel::TmpRectPath + "/Lines.txt");
		if (ifs.is_open()) {
			string line;
			string curid = "";
			while (ifs >> line) {
				if (line.substr(0, 4) == "<ID>") {
					//Get the inner text of the tag <ID>
					string tmp_curid = line.substr(4, line.length() - 5);
					if (std::find(rectids.begin(), rectids.end(), tmp_curid) != rectids.end()) {
						curid = tmp_curid;
					}
				}
				else if (line.substr(0, 5) == "<ROI>" && curid != "") {
					//Get the inner text of the tag <ROI>
					string inner = line.substr(5, line.length() - 6);
					stringstream ss = stringstream(inner);
					int x, y, w, h;
					ss >> x >> y >> w >> h;
					outputOtherBoxes.insert(std::pair<string, tsRect>(curid, tsRect(x, y, w, h, curid)));
					curid = "";
				}
			}
			ifs.close();
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
		rect.nHeight += 6;
		rect.nY += 6;
	}

	void Process() {
		auto rectIDList = GetRectIDList();
		unordered_map<string, tsRect> lineSet, otherboxSet;
		FindAllRectContainingVowel(rectIDList, lineSet, otherboxSet);
		auto ocrResults = GetOCRTextFromRectIDs(rectIDList);
		for (auto ocrResultIt = ocrResults.begin(); ocrResultIt != ocrResults.end(); ocrResultIt++) {
			if (HasVowel(ocrResultIt->ocrText)) {
				auto tsRectIt = lineSet.find(ocrResultIt->rectID);
				if (tsRectIt != lineSet.end()) {
					auto rectContent = tsRectIt->second;
					auto rectId = tsRectIt->first;
					cout << "INFO: Rect " << rectId << " increased from " << rectContent.ToString();
					IncreaseRect(rectContent);
					cout << " to " << rectContent.ToString() << "\n";
				}
				else {
					tsRectIt = otherboxSet.find(ocrResultIt->rectID);
					if (tsRectIt != otherboxSet.end()) {
						auto rectContent = tsRectIt->second;
						auto rectId = tsRectIt->first;
						cout << "INFO: Rect " << rectId << " increased from " << rectContent.ToString();
						IncreaseRect(rectContent);
						cout << " to " << rectContent.ToString() << "\n";
					}
					else {
						cout << "ERROR: " << tsRectIt->first << " not found in both lineSet and otherboxSet.\n";
					}
				}
			}
		}

		

	}

}