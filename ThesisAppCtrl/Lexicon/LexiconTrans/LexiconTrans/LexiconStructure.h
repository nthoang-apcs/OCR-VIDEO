#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		Support structure function		*/



//---------------------------------------------------------------------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*		 Structure		*/

typedef struct sLexNode
{
	// 1: word
	// 2: compound word created by 2 words
	// 3: compound word created by 3 words
	// ...
	int nType;
	// ascii text
	string strRawText;
	// unicode text
	wstring wstrText;
	sLexNode()
	{
		nType = 0;
		strRawText = "";
		wstrText = L"";
	}



}tsLexNode;


//---------------------------------------------------------------------------------------------------------------------------------