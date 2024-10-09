#include "stdafx.h"
#include "CmdInf.h"

CCommandLineInfoEx::CCommandLineInfoEx(void)
{
	mDebug = false;
	mAlarmFile.Empty();
}

void CCommandLineInfoEx::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CString str = lpszParam;
	if(str.Left(5).CompareNoCase(_T("DEBUG")) == 0){
		mDebug = true;
	}
	if(str.Left(5).CompareNoCase(_T("FILE:")) == 0){
		mAlarmFile = str.Mid(5);
	}
	
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
}
