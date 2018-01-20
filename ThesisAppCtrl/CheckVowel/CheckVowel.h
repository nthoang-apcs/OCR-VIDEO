#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

namespace CheckVowel {

	const string TmpCheckVowelPath = "./Root/TmpCheckVowel";
	const string TmpRectPath = "./Root/TmpRect";
	const string TmpImagePath = "./Root/TmpImage";
	
	//Adapted from BBoxStructure.h
	struct tsRect
	{
		string rectID;
		string originalName;
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
			originalName = "";
		}
		tsRect(int x, int y, int width, int height, string rectID, string originalName) : 
			rectID(rectID), originalName(originalName)
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
			ofs << "<ROI>" << rect.nX << " " << rect.nY << " " << rect.nWidth << " " << rect.nHeight << "</ROI>\n";
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
		ifstream ifs = ifstream(CheckVowel::TmpRectPath + "/OtherBoxes.txt");
		if (ifs.is_open()) {
			ProcessEachLine(ifs, rectids, outputOtherBoxes);
			ifs.close();
		}

		ifs = ifstream(CheckVowel::TmpRectPath + "/Lines.txt");
		if (ifs.is_open()) {
			ProcessEachLine(ifs, rectids, outputLines);
			ifs.close();
		}
	}

	void WriteAllRects(const unordered_map<string, tsRect>& lines, const unordered_map<string, tsRect>& otherboxes) {
		ofstream ofs = ofstream(CheckVowel::TmpRectPath + "/OtherBoxes.txt");
		if (ofs.is_open()) {
			WriteRectsToFile(ofs, otherboxes);
			ofs.close();
		}
		else {
			std::cout << "ERROR: Cannot open file" << CheckVowel::TmpRectPath + "/OtherBoxes.txt" << endl;
		}

		ofs = ofstream(CheckVowel::TmpRectPath + "/Lines.txt");
		if (ofs.is_open()) {
			WriteRectsToFile(ofs, lines);
			ofs.close();
		}
		else {
			std::cout << "ERROR: Cannot open file" << CheckVowel::TmpRectPath + "/Lines.txt" << endl;
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
		if (rect.nHeight < 10 && rect.nHeight < 10) {
			rect.nHeight += 2;
			rect.nY += 2;
		}
		else {
			rect.nHeight += rect.nHeight * 0.15;
			rect.nY += rect.nY * 0.15;
		}
	}

	void Process() {
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
	
		//Write all rects to 2 files Lines.txt and OtherBoxes.txt
		WriteAllRects(lineSet, otherboxSet);
	}
}