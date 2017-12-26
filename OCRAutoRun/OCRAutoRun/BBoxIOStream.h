//////////////////////////////////////////////////////////////////////
//
//	File name：	BBoxIOStream.h
//	Description：	Contain structures and definition for BBox
//	Notes:			None
//	History：	<0> 2017.12.22 : Dang Tuan Vu : Create structure definition
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BBoxStructure.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


//////////////////////////////////////////////////////////////////////
//	Class name:	BBoxIOStream
//	Description:	Handle input output stream for BBox structure
//	Notes:
//////////////////////////////////////////////////////////////////////
class BBoxIOStream
{
public:

	// return type of this string if it is in list of teBBoxElementDataType
	teBBoxElementDataType GetTypeOfTag(string &strTag)
	{
		if (strTag.compare("ID") == 0)
		{
			return BBETypeID;
		}
		else if (strTag.compare("ROI") == 0)
		{
			return BBETypeROI;
		}
		else if (strTag.compare("ROI") == 0)
		{
			return BBETypeROI;
		}
		else if (strTag.compare("NewROI") == 0)
		{
			return BBETypeACVROI;
		}
		else if (strTag.compare("Name") == 0)
		{
			return BBETypeNameImage;
		}
		else if (strTag.compare("NumberVersion") == 0)
		{
			return BBETypeNumberVersion;
		}
		else if (strTag.compare("SubID") == 0)
		{
			return BBETypeSubID;
		}
		else if (strTag.compare("SubROI") == 0)
		{
			return BBETypeSubROI;
		}
		else
		{
			return BBETypeNone;
		}
	}

	// return string convert from vector char
	string ConvertCharArrayToString(vector<char> &aTmp)
	{
		int nSize = aTmp.size();
		char* aTmp2 = new char[nSize + 1];
		for (int nI = 0; nI < nSize; nI++)
		{
			aTmp2[nI] = aTmp[nI];
		}
		aTmp2[nSize] = 0;
		string strResult = string(aTmp2);
		delete[] aTmp2;
		return strResult;
	}

	int ConvertStringToInt(string &strTagContent)
	{
		int nPos = 0;
		int nResult = 0;
		bool bPositive = true;
		int nSize = strTagContent.length();
		if (nSize == 0)
			return 0;
		// remove space or tab
		while (nPos < nSize && (strTagContent[nPos] == ' ' || strTagContent[nPos] == '\t'))
		{
			nPos++;
		}
		// check case conditions
		if (strTagContent[nPos] == '-')
		{
			bPositive = false;
			nPos++;
		}
		else if (strTagContent[nPos] < '0' || strTagContent[nPos] > '9')
		{
			return 0;
		}
		
		// get digits
		while (nPos < nSize)
		{
			if (strTagContent[nPos] >= '0' && strTagContent[nPos] <= '9')
			{
				nResult = nResult * 10 + (int)(strTagContent[nPos] - '0');
				nPos++;
			}
			else
			{
				if (bPositive == false)
				{
					return (nResult * (-1));
				}
				return nResult;
			}
		}
		if (bPositive == false)
		{
			return (nResult * (-1));
		}
		return nResult;
	}

	int ConvertArrayCharToInt(vector<char> &aTmp)
	{
		int nPos = 0;
		int nResult = 0;
		bool bPositive = true;
		int nSize = aTmp.size();
		if (nSize == 0)
			return 0;
		// remove space or tab
		while (nPos < nSize && (aTmp[nPos] == ' ' || aTmp[nPos] == '\t'))
		{
			nPos++;
		}
		// check case conditions
		if (aTmp[nPos] == '-')
		{
			bPositive = false;
			nPos++;
		}
		else if (aTmp[nPos] < '0' || aTmp[nPos] > '9')
		{
			return 0;
		}
		
		// get digits
		while (nPos < nSize)
		{
			if (aTmp[nPos] >= '0' && aTmp[nPos] <= '9')
			{
				nResult = nResult * 10 + (int)(aTmp[nPos] - '0');
				nPos++;
			}
			else
			{
				if (bPositive == false)
				{
					return (nResult * (-1));
				}
				return nResult;
			}
		}
		if (bPositive == false)
		{
			return (nResult * (-1));
		}
		return nResult;
	}

	float ConvertStringToFloat(string &strTagContent)
	{
		int nPos = 0;
		float nResult = 0.00;
		int nCount = 10;
		bool bPositive = true;
		if (strTagContent.length() == 0)
			return 0;
		// check conditions
		if (strTagContent[nPos] == '-')
		{
			bPositive = false;
			nPos++;
		}
		else if (strTagContent[nPos] < '0' || strTagContent[nPos] > '9')
		{
			return 0;
		}
		int nSize = strTagContent.length();
		// get digits before '.'
		while (nPos < nSize && strTagContent[nPos] != '.')
		{
			if (strTagContent[nPos] >= '0' && strTagContent[nPos] <= '9')
			{
				nResult = nResult * 10 + (int)(strTagContent[nPos] - '0');
				nPos++;
			}
			else if (strTagContent[nPos] != '.')
			{
				if (bPositive == false)
				{
					return (nResult * (-1));
				}
				return nResult;
			}
			else
			{
				nPos++;
				break;
			}
		}
		// get digits after '.'
		while (nPos < nSize)
		{
			if (strTagContent[nPos] >= '0' && strTagContent[nPos] <= '9')
			{
				nResult = nResult + ((int)(strTagContent[nPos] - '0')) / nCount;
				nPos++;
				nCount = nCount * 10;
			}
			else
			{
				if (bPositive == false)
				{
					return (nResult * (-1));
				}
				return nResult;
			}
		}
		if (bPositive == false)
		{
			return (nResult * (-1));
		}
		return nResult;
	}

	vector<int> ConvertStringToIntArray(string &strTagContent)
	{
		int nPos = 0;
		int nSize = strTagContent.length();
		if (nSize == 0)
			return vector<int>();
		vector<int> aResult;
		vector<char> aTmp;
		// loop all integer
		while (nPos < nSize)
		{
			// clear every first loop
			aTmp.clear();
			// ignore space if have
			while (nPos < nSize && (strTagContent[nPos] == ' ' || strTagContent[nPos] == '\t'))
			{
				nPos++;
			}
			// check length, if exceed limit, result result
			if (nPos == nSize)
			{
				return aResult;
			}

			// take inside int
			// check negative sign
			if (strTagContent[nPos] == '-')
			{
				aTmp.push_back(strTagContent[nPos]);
				nPos++;
			}
			// check special case - not a number or negative size -> return result
			if (strTagContent[nPos] < '0' || strTagContent[nPos] >'9')
			{
				aTmp.clear();
				return aResult;
			}
			// input number
			while (nPos < nSize && strTagContent[nPos] >= '0' && strTagContent[nPos] <= '9')
			{
				aTmp.push_back(strTagContent[nPos]);
				nPos++;
			}
			aResult.push_back(ConvertArrayCharToInt(aTmp));
			// check strange character
			if ((strTagContent[nPos] < '0' || strTagContent[nPos] >'9') && strTagContent[nPos] != '-')
			{
				aTmp.clear();
				return aResult;
			}
		}
		if (nPos == nSize)
		{
			aTmp.clear();
			return aResult;
		}
	}
	
	// format 1 ROI: [space or tab][x][space or tab][y][space or tab][width][space or tab][height][space or tab]
	tsRect ConvertStringTotsRect(string &strTagContent)
	{
		int nPos = 0;
		int nSize = strTagContent.length();
		tsRect tsResult;
		vector<char> aTmp;
		if (nSize == 0)
			return tsResult;
		// ignore space or tab
		while (nPos < nSize && (strTagContent[nPos] == ' ' || strTagContent[nPos] == '\t'))
		{
			nPos++;
		}
		if (nPos == nSize)
			return tsResult;
		// get x
		while (nPos < nSize && strTagContent[nPos] != ' ' && strTagContent[nPos] != '\t')
		{
			aTmp.push_back(strTagContent[nPos]);
			nPos++;
		}
		tsResult.nX = ConvertArrayCharToInt(aTmp);
		aTmp.clear();
		if (nPos == nSize)
			return tsResult;
		// ignore space or tab
		while (nPos < nSize && (strTagContent[nPos] == ' ' || strTagContent[nPos] == '\t'))
		{
			nPos++;
		}
		if (nPos == nSize)
			return tsResult;
		// get y
		while (nPos < nSize && strTagContent[nPos] != ' ' && strTagContent[nPos] != '\t')
		{
			aTmp.push_back(strTagContent[nPos]);
			nPos++;
		}
		tsResult.nY = ConvertArrayCharToInt(aTmp);
		aTmp.clear();
		if (nPos == nSize)
			return tsResult;
		// ignore space or tab
		while (nPos < nSize && (strTagContent[nPos] == ' ' || strTagContent[nPos] == '\t'))
		{
			nPos++;
		}
		if (nPos == nSize)
			return tsResult;
		// get width
		while (nPos < nSize && strTagContent[nPos] != ' ' && strTagContent[nPos] != '\t')
		{
			aTmp.push_back(strTagContent[nPos]);
			nPos++;
		}
		tsResult.nWidth = ConvertArrayCharToInt(aTmp);
		aTmp.clear();
		if (nPos == nSize)
			return tsResult;
		// ignore space or tab
		while (nPos < nSize && (strTagContent[nPos] == ' ' || strTagContent[nPos] == '\t'))
		{
			nPos++;
		}
		if (nPos == nSize)
			return tsResult;
		// get height
		while (nPos < nSize && strTagContent[nPos] != ' ' && strTagContent[nPos] != '\t')
		{
			aTmp.push_back(strTagContent[nPos]);
			nPos++;
		}
		tsResult.nHeight = ConvertArrayCharToInt(aTmp);
		aTmp.clear();
		return tsResult;
	}

	// format array of ROIs: ROI1;ROI2;ROI3;...ROIN
	vector<tsRect> ConvertStringTotsRectArray(string &strTagContent)
	{
		int nPos = 0;
		int nSize = strTagContent.length();
		vector<tsRect> atsResult;
		tsRect tsTmp;
		vector<char> aTmp;
		if (nSize == 0)
			return atsResult;
		while (nPos < nSize)
		{
			// remove space and tab
			while (nPos < nSize && (strTagContent[nPos] == ' ' || strTagContent[nPos] == '\t'))
			{
				nPos++;
			}
			if (nPos == nSize)
				return atsResult;
			// get content of 1 ROI
			while (nPos < nSize && strTagContent[nPos] != ';')
			{
				aTmp.push_back(strTagContent[nPos]);
				nPos++;
			}
			tsTmp = ConvertStringTotsRect(ConvertCharArrayToString(aTmp));
			// check condition
			if (tsTmp.nX == 0 && tsTmp.nY == 0 && tsTmp.nWidth == 0 && tsTmp.nHeight == 0)
			{
				// do nothing
			}
			else
			{
				atsResult.push_back(tsTmp);
				aTmp.clear();
			}
		}
		return atsResult;
	}

	// true: this is the format type: "[space or tab]<Rect>[space or tab]"
	bool IsLineTypeOpenningRect(string &strLine)
	{
		int nSize = strLine.length();
		if (nSize < 6)
		{
			return false;
		}
		// remove space or tab
		int nPos = 0;
		while (nPos < nSize && strLine[nPos] == ' ' || strLine[nPos] == '\t' || strLine[nPos] == '\n')
		{
			nPos++;
		}
		if ((nSize - nPos) < 6)
		{
			return false;
		}
		// check tagname: Rect
		if (strLine[nPos] == '<' && strLine[nPos + 1] == 'R' && strLine[nPos + 2] == 'e' && strLine[nPos + 3] == 'c' && strLine[nPos + 4] == 't' && strLine[nPos + 5] == '>')
		{
			nPos = nPos + 6;
			while (nPos < nSize && strLine[nPos] == ' ' || strLine[nPos] == '\t' || strLine[nPos] == '\n')
			{
				nPos++;
			}
			if (nPos != nSize)
			{
				return false;
			}
			return true;
		}
		return false;
	}
	
	// true: this is the format type: "[space or tab]</Rect>[space or tab]"
	bool IsLineTypeClosingRect(string &strLine)
	{
		int nSize = strLine.length();
		if (nSize < 7)
		{
			return false;
		}
		// remove space or tab
		int nPos = 0;
		while (nPos < nSize && strLine[nPos] == ' ' || strLine[nPos] == '\t' || strLine[nPos] == '\n')
		{
			nPos++;
		}
		if ((nSize - nPos) < 7)
		{
			return false;
		}
		// check tagname /Rect
		if (strLine[nPos] == '<' && strLine[nPos + 1] == '/' && strLine[nPos + 2] == 'R' && strLine[nPos + 3] == 'e' && strLine[nPos + 4] == 'c' && strLine[nPos + 5] == 't' && strLine[nPos + 6] == '>')
		{
			nPos = nPos + 7;
			while (nPos < nSize && strLine[nPos] == ' ' || strLine[nPos] == '\t' || strLine[nPos] == '\n')
			{
				nPos++;
			}
			if (nPos != nSize)
			{
				return false;
			}
			return true;
		}
		return false;
	}

	// true: retrieve sucessfully, format: [space or tab]<[tagname]>[space or tab][info]</[tabname]>[space or tab]
	// information 1 line store in tsElement
	bool LoadLineInfo(string &strLine, tsLineBox &tslElement)
	{
		int nSize = strLine.length();
		vector<char> aTmp;
		if (nSize < 9)	// minimum number of shortest line
		{
			return false;
		}
		// remove space or tab
		int nPos = 0;
		while (nPos < nSize && strLine[nPos] == ' ' || strLine[nPos] == '\t' || strLine[nPos] == '\n')
		{
			nPos++;
		}
		if ((nSize - nPos) < 9)
		{
			return false;
		}
		// init tag name type
		teBBoxElementDataType teType;
		// get tagname
		if (strLine[nPos] == '<')
		{
			nPos++;
			while (nPos < nSize && strLine[nPos] != '>')
			{
				aTmp.push_back(strLine[nPos]);
				nPos++;
			}
			if (nPos == nSize)
				return false;
			// get tag type
			string strTag = ConvertCharArrayToString(aTmp);
			teType = GetTypeOfTag(strTag);
			aTmp.clear();
		}
		else
		{
			return false;
		}
		// remove space or tab
		int nPos = 0;
		while (nPos < nSize && strLine[nPos] == ' ' || strLine[nPos] == '\t' || strLine[nPos] == '\n')
		{
			nPos++;
		}
		if ((nSize - nPos) < 9)
		{
			return false;
		}
		// get tag content
		while (nPos < nSize && strLine[nPos] != '<')
		{
			aTmp.push_back(strLine[nPos]);
			nPos++;
		}
		if (nPos == nSize)
			return false;
		// get tag content
		string strTagContent = ConvertCharArrayToString(aTmp);
		aTmp.clear();
		
		// check tagname type
		if (teType == BBETypeNone)
		{
			return false;
		}
		else if (teType == BBETypeID)
		{
			// load int
			tslElement.tsCore.nID = ConvertStringToInt(strTagContent);
		}
		else if (teType == BBETypeROI)
		{
			// Load tsRect
			tslElement.tsCore.rROI = ConvertStringTotsRect(strTagContent);
		}
		else if (teType == BBETypeACVROI)
		{
			// load tsRect
			tslElement.tsCore.rACVROI = ConvertStringTotsRect(strTagContent);
		}
		else if (teType == BBETypeNameImage)
		{
			// load string
			tslElement.tsCore.strNameImage = strTagContent;
		}
		else if (teType == BBETypeNumberVersion)
		{
			// load int
			tslElement.tsCore.nNumberVersion = ConvertStringToInt(strTagContent);
		}
		else if (teType == BBETypeTimeRunning)
		{
			// load float
			tslElement.tsCore.fTimeRunning = ConvertStringToFloat(strTagContent);
		}
		else if (teType == BBETypeSubID)
		{
			// load array of int
			tslElement.anSubID = ConvertStringToIntArray(strTagContent);
		}
		else if (teType == BBETypeSubROI)
		{
			// load array of tsRect
			tslElement.atsSubROI = ConvertStringTotsRectArray(strTagContent);
		}
		else
		{
			return false;
		}
	}

public:	
	
	// true: retrieve sucessfully
	bool ReadLines(vector<tsLineBox> &atsLines, string strPath)
	{
		ifstream ifsRead;
		ifsRead.open(strPath, std::ifstream::in);
		if (ifsRead.is_open())
		{
			// init variable
			string strLine;
			bool bIsInRect = false;
			tsLineBox tslElement;
			while (getline(ifsRead, strLine))
			{
				// start new tsRect
				if (bIsInRect == false)
				{
					// empty line between element, or end of file
					if (strLine == " " || strLine == "")
					{
						continue;
					}
					// check if this is the new element openning
					if (IsLineTypeOpenningRect(strLine) == true)
					{
						bIsInRect = true;
						tslElement.Destroy();
					}
					else
					{
						// failure
						return false;
					}
				}
				else
				{
					// check if closing element info
					if (IsLineTypeClosingRect(strLine) == true)
					{
						bIsInRect = false;
						continue;
					}
					// load information, do not care order
					bool bChecked = LoadLineInfo(strLine, tslElement);
					if (bChecked == false)
						return false;
				}
			}
			ifsRead.close();
			return true;
		}
		return false;
	}

	// true: retrieve sucessfully
	void ReadOtherBoxes(vector<tsOtherBox> &atsOtherBoxes, string strPath)
	{

	}

	void WriteLines(vector<tsLineBox> &atsLines, string strPath)
	{

	}

	void WriteOtherBoxes(vector<tsOtherBox> &atsOtherBoxes, string strPath)
	{

	}
	
};

