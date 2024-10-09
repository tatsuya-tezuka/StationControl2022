#include "stdafx.h"
#include "CmdInf.h"

CCommandLineInfoEx::CCommandLineInfoEx(void)
{
	mTitleString.Empty();
	mPanelXmlName.Empty();
}

void CCommandLineInfoEx::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CString str = lpszParam;
	if(str.Left(6).CompareNoCase(_T("TITLE:")) == 0){
		mTitleString = str.Mid(6);
	}
	if(str.Left(4).CompareNoCase(_T("XML:")) == 0){
		mPanelXmlName = str.Mid(4);
	}
	
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
}
