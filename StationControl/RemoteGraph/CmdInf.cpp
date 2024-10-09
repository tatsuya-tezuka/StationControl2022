#include "stdafx.h"
#include "CmdInf.h"
#include "const.h"

CCommandLineInfoEx::CCommandLineInfoEx(void)
{
	mStationMode = eStation_5;
	mStation = eStation_Usuda64;
	mServerName.Empty();
}

void CCommandLineInfoEx::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CString str = lpszParam;
	if(str.Left(5).CompareNoCase(_T("MODE:")) == 0){
		mStationMode = _wtoi(str.Mid(5)) - 1;
		mStation = 9;
		// Å¶
		if (mStationMode == eStation_10)
			mStationMode = eStation_1;
	}
	if(str.Left(8).CompareNoCase(_T("STATION:")) == 0){
		if (str.Mid(8).CollateNoCase(_T("SELECT")) == 0)
			mStation = 9;
		else
			mStation = _wtoi(str.Mid(8)) - 1;
	}
	if (str.Left(11).CompareNoCase(_T("SERVERNAME:")) == 0){
		mServerName = str.Mid(11);
	}

	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
}
