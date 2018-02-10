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

	// more height intersect than width
	// true: 2 rect are intersecting each other horizontally
	// false: other case (vertically or even not intersect)
	bool IsTwoRectIntersectHorizontally(sRect tsOther)
	{
		if (nX < tsOther.nX)
		{
			if (nY < tsOther.nY)
			{
				// check if 2 rect are at least intersected
				if ((tsOther.nX < (nX + nWidth)) && (tsOther.nY < (nY + nHeight)))
				{
					// check condition
					if ((nX + nWidth - tsOther.nX) < (nY + nHeight - tsOther.nY))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else  // nY >= tsOther.nY
			{
				// check if 2 rect are at least intersected
				if ((tsOther.nX < (nX + nWidth)) && (nY < (tsOther.nY + tsOther.nHeight)))
				{
					// check condition
					if ((nX + nWidth - tsOther.nX) < (tsOther.nY + tsOther.nHeight - nY))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		else  //nX >= tsOther.nX
		{
			if (tsOther.nY < nY)
			{
				// check if 2 rect are at least intersected
				if ((nX < (tsOther.nX + tsOther.nWidth)) && (nY < (tsOther.nY + tsOther.nHeight)))
				{
					// check condition
					if ((tsOther.nX + tsOther.nWidth - nX) < (tsOther.nY + tsOther.nHeight - nY))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else  // nY >= tsOther.nY
			{
				// check if 2 rect are at least intersected
				if ((nX < (tsOther.nX + tsOther.nWidth)) && (tsOther.nY < (nY + nHeight)))
				{
					// check condition
					if ((tsOther.nX + tsOther.nWidth - nX) < (nY + nHeight - tsOther.nY))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}
	bool IsTwoRectIntersectVertically(sRect tsOther)
	{
		if (nX < tsOther.nX)
		{
			if (nY < tsOther.nY)
			{
				// check if 2 rect are at least intersected
				if ((tsOther.nX < (nX + nWidth)) && (tsOther.nY < (nY + nHeight)))
				{
					// check condition
					if ((nX + nWidth - tsOther.nX) > (nY + nHeight - tsOther.nY))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else  // nY >= tsOther.nY
			{
				// check if 2 rect are at least intersected
				if ((tsOther.nX < (nX + nWidth)) && (nY < (tsOther.nY + tsOther.nHeight)))
				{
					// check condition
					if ((nX + nWidth - tsOther.nX) > (tsOther.nY + tsOther.nHeight - nY))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		else  //nX >= tsOther.nX
		{
			if (tsOther.nY < nY)
			{
				// check if 2 rect are at least intersected
				if ((nX < (tsOther.nX + tsOther.nWidth)) && (nY < (tsOther.nY + tsOther.nHeight)))
				{
					// check condition
					if ((tsOther.nX + tsOther.nWidth - nX) > (tsOther.nY + tsOther.nHeight - nY))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else  // nY >= tsOther.nY
			{
				// check if 2 rect are at least intersected
				if ((nX < (tsOther.nX + tsOther.nWidth)) && (tsOther.nY < (nY + nHeight)))
				{
					// check condition
					if ((tsOther.nX + tsOther.nWidth - nX) > (nY + nHeight - tsOther.nY))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}
}tsRect;

//////////////////////////////////////////////////////////////////////
//	Enum name:	teBBoxElementDataType
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
	sBBoxInfo(int ID, int ROIx, int ROIy, int ROIwidth, int ROIheight,
		int ACVROIx, int ACVROIy, int ACVROIwidth, int ACVROIheight,
		string ImageName, int VersionNumber, float RunningTime)
	{
		nID = ID;
		rROI =  tsRect(ROIx, ROIy, ROIwidth, ROIheight);
		rACVROI = tsRect(ACVROIx, ACVROIy, ACVROIwidth, ACVROIheight);
		strNameImage = ImageName;
		nNumberVersion = VersionNumber;
		fTimeRunning = RunningTime;
	}
	sBBoxInfo(int ID, int ROIx, int ROIy, int ROIwidth, int ROIheight,
		string ImageName, int VersionNumber, float RunningTime)
	{
		nID = ID;
		rROI = tsRect(ROIx, ROIy, ROIwidth, ROIheight);
		rACVROI = tsRect(0, 0, 0, 0);
		strNameImage = ImageName;
		nNumberVersion = VersionNumber;
		fTimeRunning = RunningTime;
	}
	sBBoxInfo(int ID, string ImageName, int VersionNumber, float RunningTime)
	{
		nID = ID;
		rROI = tsRect(0, 0, 0, 0);
		rACVROI = tsRect(0, 0, 0, 0);
		strNameImage = ImageName;
		nNumberVersion = VersionNumber;
		fTimeRunning = RunningTime;
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
	// Last Image name = [strNameImage]-[nID]-[nNumberVersion]
	string GetLastImageName()
	{
		if(nNumberVersion == 0 && nID == 0 && fTimeRunning == 0)
			return string();
		string strResult = strNameImage + "-" + to_string(nID) + "-" + to_string(nNumberVersion);
		return strResult;
	}
	// Input rROI =  a cover rect from multiple inside rects
	void InputROIByCreateCoverRect(vector<tsRect> atsInsideRect)
	{
		// find the most left x and the most right x
		// find the most top and the most bottom
		int nSize = atsInsideRect.size();
		int nX = 0;
		int nY = 0;
		int nW = 0;
		int nH = 0;
		for (int nI = 0; nI < nSize; nI++)
		{
			if (atsInsideRect[nI].nX < nX || nI == 0)
			{
				nX = atsInsideRect[nI].nX;
			}
			if (atsInsideRect[nI].nY < nY || nI == 0)
			{
				nY = atsInsideRect[nI].nY;
			}
			if ((atsInsideRect[nI].nWidth + atsInsideRect[nI].nX) > (nX + nW) || nI == 0)
			{
				nW = (atsInsideRect[nI].nWidth + atsInsideRect[nI].nX) - nX;
			}
			if ((atsInsideRect[nI].nHeight + atsInsideRect[nI].nY) > (nY + nH) || nI == 0)
			{
				nH = (atsInsideRect[nI].nHeight + atsInsideRect[nI].nY) - nY;
			}
		}
		rROI = tsRect(nX, nY, nW, nH);
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
	sLineBox(int ID, int ROIx, int ROIy, int ROIwidth, int ROIheight,
		int ACVROIx, int ACVROIy, int ACVROIwidth, int ACVROIheight,
		string ImageName, int VersionNumber, float RunningTime)
	{
		tsCore = tsBBoxInfo(ID, ROIx, ROIy, ROIwidth, ROIheight, 
			ACVROIx, ACVROIy, ACVROIwidth, ACVROIheight,
			ImageName, VersionNumber, RunningTime);
		anSubID.clear();
		atsSubROI.clear();
	}
	sLineBox(int ID, int ROIx, int ROIy, int ROIwidth, int ROIheight,
		string ImageName, int VersionNumber, float RunningTime)
	{
		tsCore = tsBBoxInfo(ID, ROIx, ROIy, ROIwidth, ROIheight,
			ImageName, VersionNumber, RunningTime);
		anSubID.clear();
		atsSubROI.clear();
	}
	sLineBox(int ID, string ImageName, int VersionNumber, float RunningTime)
	{
		tsCore = tsBBoxInfo(ID, ImageName, VersionNumber, RunningTime);
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

