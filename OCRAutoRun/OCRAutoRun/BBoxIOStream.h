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
protected:

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

	}

	float ConvertStringToFloat(string &strTagContent)
	{

	}

	vector<int> ConvertStringToIntArray(string &strTagContent)
	{

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

	// true: retrieve sucessfully, format: [space or tab]<[tagname]>[space or tab][info][space or tab]</[tabname]>[space or tab]
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
			teBBoxElementDataType teType = GetTypeOfTag(strTag);
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

		}
		else if (teType == BBETypeROI)
		{
			// Load tsRect

		}
		else if (teType == BBETypeACVROI)
		{
			// load tsRect

		}
		else if (teType == BBETypeNameImage)
		{
			// load string

		}
		else if (teType == BBETypeNumberVersion)
		{
			// load int

		}
		else if (teType == BBETypeTimeRunning)
		{
			// load float

		}
		else if (teType == BBETypeSubID)
		{
			// load array of int

		}
		else if (teType == BBETypeSubROI)
		{
			// load array of int

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

