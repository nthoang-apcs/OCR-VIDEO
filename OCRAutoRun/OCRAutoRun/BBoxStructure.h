//////////////////////////////////////////////////////////////////////
//
//	File name：	BBoxStructure.h
//	Description：	Contain structures and definition for BBox
//	Notes:			None
//	History：	<0> 2017.12.22 : Dang Tuan Vu : Create structure definition
//
//////////////////////////////////////////////////////////////////////

#ifndef	_BBOXSTRUCTURE_H_
#define _BBOXSTRUCTURE_H_

#if _MSC_VER > 1000
#pragma once
#endif 	// _MSC_VER > 1000

#include <opencv2\highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv:

//////////////////////////////////////////////////////////////////////
//	Struct name:	tsBBoxInfo
//	Description:	Contain the core information of a BBox
//	Notes:
//////////////////////////////////////////////////////////////////////
typedef struct sBBoxInfo
{
	int 			nID;
	Rect 		rROI;
	Rect			rACVROI;				// after check vowel ROI if have
	string		strNameImage;		// name of origin image, not include extension
	int				nNumberVersion;	// 1 or 2 or 3, the last number is the last number display in OCRAutoRun image sub-title
	float			TimeRunning;		// total time running of process for this bbox
	
	// constructor
	sBBoxInfo()
	{
		nID = 0;
		rROI = Rect(0, 0, 0, 0);
		rACVROI = Rect(0, 0, 0, 0);
		strNameImage = "";
		nNumberVersion = 0;
		TimeRunning = 0.00;
	}
	// copy constructor
	sBBoxInfo (const sBBoxInfo &obj) 
	{
		nID = obj.nID;
		rROI = Rect(obj.rROI.x, obj.rROI.y, obj.rROI.width, obj.rROI.height);
		rACVROI = Rect(obj.rACVROI.x, obj.rACVROI.y, obj.rACVROI.width, obj.rACVROI.height);
		strNameImage = obj.strNameImage;
		nNumberVersion = obj.nNumberVersion;
		TimeRunning = obj.TimeRunning;
	}
	// assign operator
	void operator=(const sBBoxInfo& obj)
	{
		nID = obj.nID;
		rROI = Rect(obj.rROI.x, obj.rROI.y, obj.rROI.width, obj.rROI.height);
		rACVROI = Rect(obj.rACVROI.x, obj.rACVROI.y, obj.rACVROI.width, obj.rACVROI.height);
		strNameImage = obj.strNameImage;
		nNumberVersion = obj.nNumberVersion;
		TimeRunning = obj.TimeRunning;
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
	vector<pair<ID, Rect>> aComponentBoxes;
	
	// constructor
	sLineBox()
	{
		Core = tsBBoxInfo();
		ComponentBoxes.clear();
	}
	// copy constructor
	sLineBox (const sLineBox &obj) 
	{
		Core = obj.Core;
		ComponentBoxes.clear();
		ComponentBoxes = obj.ComponentBoxes;
	}
	// assign operator
	void operator=(const sLineBox& obj)
	{
		Core = obj.Core;
		ComponentBoxes.clear();
		ComponentBoxes = obj.ComponentBoxes;
	}
	
}tsLineBox;



#endif	// define bboxstructure.h