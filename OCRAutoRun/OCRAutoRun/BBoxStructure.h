//////////////////////////////////////////////////////////////////////
//
//	File name：	BBoxStructure.h
//	Description：	Contain structures and definition for BBox
//	Notes:			None
//	History：	<0> 2017.12.22 : Dang Tuan Vu : Create structure definition
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
//	Struct name:	tsRect
//	Description:	Contain the information of a rectangle
//	Notes:
//////////////////////////////////////////////////////////////////////
typedef struct sRect
{
	int nX;
	int nY;
	int nWidth;
	int nHeight;
	sRect()
	{
		nX = 0;
		nY = 0;
		nWidth = 0;
		nHeight = 0;
	}
	sRect(int x, int y, int width, int height)
	{
		nX = x;
		nY = y;
		nWidth = width;
		nHeight = height;
	}
	sRect(const sRect &obj)
	{
		nX = obj.nX;
		nY = obj.nY;
		nWidth = obj.nWidth;
		nHeight = obj.nHeight;
	}
	void operator=(const sRect& obj)
	{
		nX = obj.nX;
		nY = obj.nY;
		nWidth = obj.nWidth;
		nHeight = obj.nHeight;
	}
	int GetArea()
	{
		return (nWidth*nHeight);
	}

}tsRect;

//////////////////////////////////////////////////////////////////////
//	Enum name:	tsRect
//	Description:	Contain the information of a rectangle
//	Notes:
//////////////////////////////////////////////////////////////////////
typedef enum eBBoxElementDataType
{
	BBETypeNone,
	BBETypeID,
	BBETypeROI,
	BBETypeACVROI,
	BBETypeNameImage,
	BBETypeNumberVersion,
	BBETypeTimeRunning,
	BBETypeSubID,
	BBETypeSubROI
}teBBoxElementDataType;

//////////////////////////////////////////////////////////////////////
//	Struct name:	tsBBoxInfo
//	Description:	Contain the core information of a BBox
//	Notes:
//////////////////////////////////////////////////////////////////////
typedef struct sBBoxInfo
{
	int 			nID;
	tsRect 			rROI;
	tsRect			rACVROI;				// after check vowel ROI if have
	string			strNameImage;		// name of origin image, not include extension
	int				nNumberVersion;	// 1 or 2 or 3, the last number is the last number display in OCRAutoRun image sub-title
	float			fTimeRunning;		// total time running of process for this bbox
	
	// constructor
	sBBoxInfo()
	{
		nID = 0;
		rROI = tsRect(0, 0, 0, 0);
		rACVROI = tsRect(0, 0, 0, 0);
		strNameImage = "";
		nNumberVersion = 0;
		fTimeRunning = 0.00;
	}
	// copy constructor
	sBBoxInfo (const sBBoxInfo &obj) 
	{
		nID = obj.nID;
		rROI = tsRect(obj.rROI.nX, obj.rROI.nY, obj.rROI.nWidth, obj.rROI.nHeight);
		rACVROI = tsRect(obj.rROI.nX, obj.rROI.nY, obj.rROI.nWidth, obj.rROI.nHeight);
		strNameImage = obj.strNameImage;
		nNumberVersion = obj.nNumberVersion;
		fTimeRunning = obj.fTimeRunning;
	}
	// assign operator
	void operator=(const sBBoxInfo& obj)
	{
		nID = obj.nID;
		rROI = tsRect(obj.rROI.nX, obj.rROI.nY, obj.rROI.nWidth, obj.rROI.nHeight);
		rACVROI = tsRect(obj.rROI.nX, obj.rROI.nY, obj.rROI.nWidth, obj.rROI.nHeight);
		strNameImage = obj.strNameImage;
		nNumberVersion = obj.nNumberVersion;
		fTimeRunning = obj.fTimeRunning;
	}
	void Destroy()
	{
		nID = 0;
		rROI = tsRect(0, 0, 0, 0);
		rACVROI = tsRect(0, 0, 0, 0);
		strNameImage = "";
		nNumberVersion = 0;
		fTimeRunning = 0.00;
	}
	string GetLastImageName()
	{
		if(nNumberVersion == 0 && nID == 0 && fTimeRunning == 0)
			return string();
		string strResult = strNameImage + "-" + to_string(nNumberVersion);
		return strResult;
	}
	
}tsBBoxInfo, tsOtherBox;

//////////////////////////////////////////////////////////////////////
//	Struct name:	tsLineBox
//	Description:	Contain the information of a line box
//	Notes:
//////////////////////////////////////////////////////////////////////
typedef struct sLineBox
{
	tsBBoxInfo	tsCore;
	vector<int> anSubID;
	vector<tsRect> atsSubROI;
	
	// constructor
	sLineBox()
	{
		tsCore = tsBBoxInfo();
		anSubID.clear();
		atsSubROI.clear();
	}
	// copy constructor
	sLineBox (const sLineBox &obj) 
	{
		tsCore = obj.tsCore;
		anSubID.clear();
		atsSubROI.clear();
		anSubID = obj.anSubID;
		atsSubROI = obj.atsSubROI;
	}
	// assign operator
	void operator=(const sLineBox& obj)
	{
		tsCore = obj.tsCore;
		anSubID.clear();
		atsSubROI.clear();
		anSubID = obj.anSubID;
		atsSubROI = obj.atsSubROI;
	}
	void Destroy()
	{
		anSubID.clear();
		atsSubROI.clear();
		tsCore.Destroy();
	}
	string GetLastImageName()
	{
		return tsCore.GetLastImageName();
	}
}tsLineBox;

