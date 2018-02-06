//////////////////////////////////////////////////////////////////////
//
//	File name：	LexStruct.h
//	Description：	Contain structures and definition for Lexicon
//	Notes:			None
//	History：	<0> 2017.12.22 : Dang Tuan Vu : Create structure definition
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
using namespace std;

//////////////////////////////////////////////////////////////////////
//	Struct name:	tsLex
//	Description:	Contain the information of a string after lexicon, limit for C++11 or newer only
//	Notes:			Format sample:
//<Lex>
//	<Name>[Name of the original image without extension]</Name>
//	<ID>[ID of the rect]</ID>
//	<OldStr>[The string content before using lexicon]</OldStr>
//	<NewStr>[The string content after using lexicon]</NewStr>
//	<TimeRunning>[Time running of the rect for lexicon]</TimeRunning>
//</Lex>
//////////////////////////////////////////////////////////////////////
typedef struct sLex
{
	string 		strNameImage;
	int 			nID;
	wstring		wstrOldStr;
	wstring		wstrNewStr;
	float			fTimeRunning;
	
	sLex()
	{
		nID = 0;
		fTimeRunning = 0;
		strNameImage = "";
		wstrOldStr = L"";
		wstrNewStr = L"";
	}
	sLex(int ID, string ImageName, wstring BeforeLex, wstring AfterLex, float TimeRunning = 0.0)
	{
		strNameImage = ImageName;
		nID = ID;
		wstrOldStr = BeforeLex;
		wstrNewStr = AfterLex;
		fTimeRunning = TimeRunning;
	}
	
	wstring GetWstrImageName()
	{
		wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		wstring wstrRes = converter.from_bytes(strNameImage);
		return wstrRes;
	}


}tsLex;