//////////////////////////////////////////////////////////////////////
//
//	File name：	LexIO.h
//	Description：	Contain input and output stream for LexStruct, limit for C++11 or newer only
//	Notes:			None
//	History：	<0> 2017.12.22 : Dang Tuan Vu : Create structure definition
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <locale>
#include <codecvt>
#include <fstream>
#include <string>
#include <vector>
#include "LexStruct.h"
using namespace std;


class LexIO
{
	
private:
	
	
protected:	

	/*
		return value for tag name
		Unknow:			0
		Name:			1
		ID:				2
		OldStr:			3
		NewStr:			4
		TimeRunning:	5
	*/
	int GetTagNameNumber(vector<wchar_t> &aTagName)
	{
		size_t nSize = aTagName.size();
		if(nSize == 2)
		{
			if((aTagName[0] == L'I') && (aTagName[1] == L'D'))
			{
				return 2;
			}
		}
		else if (nSize == 4)
		{
			if((aTagName[0] == L'N') && (aTagName[1] == L'a') && (aTagName[2] == L'm') && (aTagName[3] == L'e'))
			{
				return 1;
			}
		}
		else if (nSize == 6)
		{
			if((aTagName[0] == L'O') && (aTagName[1] == L'l') && (aTagName[2] == L'd') && (aTagName[3] == L'S') && (aTagName[4] == L't') && (aTagName[5] == L'r'))
			{
				return 3;
			}
			if((aTagName[0] == L'N') && (aTagName[1] == L'e') && (aTagName[2] == L'w') && (aTagName[3] == L'S') && (aTagName[4] == L't') && (aTagName[5] == L'r'))
			{
				return 4;
			}
		}
		else if (nSize == 11)
		{
			if((aTagName[0] == L'T') && (aTagName[1] == L'i') && (aTagName[2] == L'm') && (aTagName[3] == L'e') && (aTagName[4] == L'R') && (aTagName[5] == L'u')
				&& (aTagName[6] == L'n') && (aTagName[7] == L'n') && (aTagName[8] == L'i') && (aTagName[9] == L'n') && (aTagName[10] == L'g'))
			{
				return 5;
			}
		}
		return 0;
	}

	// convert vector of wchar_t to integer
	int ConvertWCharArrayToInt(vector<wchar_t> &aContent)
	{
		int nRes = 0;
		if(aContent.size() == 0)
			return nRes;
		bool bPositive = true;
		size_t nSize = aContent.size();
		size_t nPos = 0;
		if(aContent[nPos] == L'-')
		{
			bPositive = false;
			nPos++;
		}
		else if (aContent[nPos] == L'+')
		{
			bPositive = true;
			nPos++;
		}
		else if ((aContent[nPos] < L'0') || (aContent[nPos] > L'9'))
		{
			return nRes;
		}
		while((nPos < nSize) && (aContent[nPos] >= L'0') && (aContent[nPos] <= L'9')) 
		{
			nRes = (nRes * 10) + (int)(aContent[nPos] - L'0');
		}
		if(bPositive == false)
		{
			nRes = nRes * (-1);
		}
		return nRes;
	}

	// convert vector of wchar_t to float
	float ConvertWCharArrayToFloat(vector<wchar_t> &aContent)
	{
		float nRes = 0.0;
		size_t nSize = aContent.size();
		if(nSize == 0)
			return nRes;

		return nRes;
	}

	// input from a line in the "Lex" element
	bool InputInfoForTsLex(wstring wstrLine, tsLex &LexTmp)
	{
		size_t nSize = wstrLine.size();
		size_t nPos = 0;
		// skip space \t \n
		while((nPos < nSize) && ((wstrLine[nPos] == L' ') || (wstrLine[nPos] == L'\t') || (wstrLine[nPos] == L'\n') ))
		{
			nPos++;
		}
		if(nPos == nSize)
			return false;
		// get tag name
		vector<wchar_t> aWchar;
		// check format of tag
		if((wstrLine[nPos] == L'<') && ((nPos + 1) < nSize))
		{
			nPos++;
			while((nPos < nSize) && (wstrLine[nPos] != L'>'))
			{
				aWchar.push_back(wstrLine[nPos]);
				nPos++;
			}
			nPos++;
		}
		else
		{
			return false;
		}
		int nTagNumber = GetTagNameNumber(aWchar);
		aWchar.clear();
		// check condition and get content
		if(nTagNumber == 0)
			return false;
		else if (nTagNumber == 1)
		{
			// get Name
			while((nPos < nSize) && (wstrLine[nPos] != L'<'))
			{
				aWchar.push_back(wstrLine[nPos]);
				nPos++;
			}
			if((nPos == nSize) || (aWchar.size() == 0))
				return false;
			wchar_t *awcharTmp = new wchar_t[aWchar.size() + 1];
			for(size_t nJ = 0; nJ < aWchar.size(); nJ++)
			{
				awcharTmp[nJ] = aWchar[nJ];
			}
			awcharTmp[aWchar.size()] = 0;
			wstring wstrTmp = wstring(awcharTmp);

			wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			LexTmp.strNameImage = converter.to_bytes(wstrTmp);

			delete awcharTmp;
			aWchar.clear();
			return true;
		}
		else if (nTagNumber == 2)
		{
			// get ID
			while((nPos < nSize) && (wstrLine[nPos] != L'<'))
			{
				aWchar.push_back(wstrLine[nPos]);
				nPos++;
			}
			if((nPos == nSize) || (aWchar.size() == 0))
				return false;
			LexTmp.nID = ConvertWCharArrayToInt(aWchar);
			aWchar.clear();
			return true;
		}
		else if (nTagNumber == 3)
		{
			// get OldStr
			while((nPos < nSize) && (wstrLine[nPos] != L'<'))
			{
				aWchar.push_back(wstrLine[nPos]);
				nPos++;
			}
			if((nPos == nSize) || (aWchar.size() == 0))
				return false;
			wchar_t *awcharTmp = new wchar_t[aWchar.size() + 1];
			for(size_t nJ = 0; nJ < aWchar.size(); nJ++)
			{
				awcharTmp[nJ] = aWchar[nJ];
			}
			awcharTmp[aWchar.size()] = 0;

			LexTmp.wstrOldStr = wstring(awcharTmp);

			delete awcharTmp;
			aWchar.clear();
			return true;
		}
		else if (nTagNumber == 4)
		{
			// get NewStr
			while((nPos < nSize) && (wstrLine[nPos] != L'<'))
			{
				aWchar.push_back(wstrLine[nPos]);
				nPos++;
			}
			if((nPos == nSize) || (aWchar.size() == 0))
				return false;
			wchar_t *awcharTmp = new wchar_t[aWchar.size() + 1];
			for(size_t nJ = 0; nJ < aWchar.size(); nJ++)
			{
				awcharTmp[nJ] = aWchar[nJ];
			}
			awcharTmp[aWchar.size()] = 0;

			LexTmp.wstrNewStr = wstring(awcharTmp);

			delete awcharTmp;
			aWchar.clear();
			return true;
		}
		else if (nTagNumber == 5)
		{
			// get TimeRunning
			while((nPos < nSize) && (wstrLine[nPos] != L'<'))
			{
				aWchar.push_back(wstrLine[nPos]);
				nPos++;
			}
			if((nPos == nSize) || (aWchar.size() == 0))
				return false;
			LexTmp.fTimeRunning = ConvertWCharArrayToFloat(aWchar);
			aWchar.clear();
			return true;
		}
		else
		{
			return false;
		}
	}
	
public:
	
	// read data from file LexiconResult.txt
	bool ReadLexFile(vector<tsLex> &LexList, string strPathFile)
	{
		wifstream wfsRead;
		wfsRead.open(strPathFile, std::wfstream::in);
		tsLex LexTmp;
		if(wfsRead.is_open())
		{
			wstring wstrLine;
			while(getline(wfsRead, wstrLine))
			{
				if(wstrLine.compare(L"") != 0 && wstrLine.compare(L" ") != 0)
				{
					// check if this is the "<Lex>" string
					if(wstrLine.compare(L"<Lex>") == 0 || wstrLine.compare(L"<Lex>\n") == 0)
					{
						bool bChecked = false;
						// read name
						getline(wfsRead, wstrLine);
						bChecked = InputInfoForTsLex(wstrLine, LexTmp);
						if(bChecked == false)
						{
							continue;
						}
						// read ID
						getline(wfsRead, wstrLine);
						bChecked = InputInfoForTsLex(wstrLine, LexTmp);
						if(bChecked == false)
						{
							continue;
						}
						// read OldStr
						getline(wfsRead, wstrLine);
						bChecked = InputInfoForTsLex(wstrLine, LexTmp);
						if(bChecked == false)
						{
							continue;
						}
						// read NewStr
						getline(wfsRead, wstrLine);
						bChecked = InputInfoForTsLex(wstrLine, LexTmp);
						if(bChecked == false)
						{
							continue;
						}
						// read TimeRunning
						getline(wfsRead, wstrLine);
						bChecked = InputInfoForTsLex(wstrLine, LexTmp);
						if(bChecked == false)
						{
							continue;
						}
						// skip last </Rect>
						getline(wfsRead, wstrLine);
					}
					continue;
				}
			}
		}
		return false;
	}
	
	// write data to file LexiconResult.txt
	//	Notes:			Format sample:
	//<Lex>
	//	<Name>[Name of the original image without extension]</Name>
	//	<ID>[ID of the rect]</ID>
	//	<OldStr>[The string content before using lexicon]</OldStr>
	//	<NewStr>[The string content after using lexicon]</NewStr>
	//	<TimeRunning>[Time running of the rect for lexicon]</TimeRunning>
	//</Lex>
	bool WriteLexFile(vector<tsLex> &LexList, string strPathFile)
	{
		const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
		wofstream wfsWrite;
		wfsWrite.open(strPathFile, std::wfstream::out);
		if(wfsWrite.is_open())
		{
			wfsWrite.imbue(utf8_locale);
			size_t nSize = LexList.size();
			for(size_t nI = 0; nI < nSize; nI++)
			{
				wfsWrite << L"<Lex>\n";
				wfsWrite << L"<Name>" << LexList[nI].GetWstrImageName() << L"</Name>\n";
				wfsWrite << L"<ID>" << LexList[nI].nID << L"</ID>\n";
				wfsWrite << L"<OldStr>" << LexList[nI].wstrOldStr << L"</OldStr>\n";
				wfsWrite << L"<NewStr>" << LexList[nI].wstrNewStr << L"</NewStr>\n";
				wfsWrite << L"<TimeRunning>" << LexList[nI].fTimeRunning << L"</TimeRunning>\n";
				wfsWrite << L"</Lex>\n";
			}
			wfsWrite.close();
			return true;
		}
		return false;
	}
	
	
};