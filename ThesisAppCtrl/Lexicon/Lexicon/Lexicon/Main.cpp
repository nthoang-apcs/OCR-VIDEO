#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include "LexStruct.h"
#include "LexIO.h"
using namespace std;
  
int main(int argc, char** argv)  
{  
	wcout << (int)(L'1') << endl;

    return 0;  
}  


/*
		**** Code to write unicode in c++
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	wofstream wfsWrite;
	wfsWrite.open("TestWrite1.txt", std::wfstream::out);
	if(wfsWrite.is_open())
	{
		wfsWrite.imbue(utf8_locale);
		wfsWrite << L"Chào các bạn. Hôm nay tui đang mắc cạn trong nhà.";
		wfsWrite.close();
	}
		**** Code to convert between string and wstring
	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	string narrow = converter.to_bytes(wide_utf16_source_string);
	wstring wide = converter.from_bytes(narrow_utf8_source_string);
*/