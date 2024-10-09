#include "stdafx.h"
#include "CmdInf.h"
#include "const.h"

CCommandLineInfoEx::CCommandLineInfoEx(void)
{
	mStationMode = eStation_5;
	mStation = eStation_Usuda64;
	mIgnore = 0;
	mLBCtrl = _T("");
}

void CCommandLineInfoEx::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CString str = lpszParam;
	if(str.Left(5).CompareNoCase(_T("MODE:")) == 0){
		mStationMode = _wtoi(str.Mid(5)) - 1;
		mStation = 9;
		// ※
		if (mStationMode == eStation_10)
			mStationMode = eStation_1;
	}
	if (str.Left(8).CompareNoCase(_T("STATION:")) == 0){
		if (str.Mid(8).CollateNoCase(_T("SELECT")) == 0){
			// ダイアログ表示して局選択
			mStation = 9;
		}
		else if (str.Mid(8).CompareNoCase(_T("A")) == 0){
			// 臼田64mをダイアログ表示固定指定
			mStation = eFixedSelectStation_Usuda64;
		}
		else if (str.Mid(8).CompareNoCase(_T("B")) == 0){
			// 臼田54mをダイアログ表示固定指定
			mStation = eFixedSelectStation_Usuda642;
		}
		else if (str.Mid(8).CompareNoCase(_T("C")) == 0){
			// 内之浦34mをダイアログ表示固定指定
			mStation = eFixedSelectStation_Uchinoura34;
		}
		else if (str.Mid(8).CompareNoCase(_T("D")) == 0){
			// 内之浦20mをダイアログ表示固定指定
			mStation = eFixedSelectStation_Uchinoura20;
		}
		else{
			// 局をダイアログ表示無しで固定指定
			mStation = _wtoi(str.Mid(8)) - 1;
		}
	}
	if (str.Left(7).CompareNoCase(_T("IGNORE:")) == 0){
		mIgnore = _wtoi(str.Mid(7));
	}

	if (str.Left(9).CompareNoCase(_T("LOOPBAND:")) == 0){
		mLBStation = _wtoi(str.Mid(9, 1));
		mLBCtrl = str.Mid(11);
	}

	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
}
