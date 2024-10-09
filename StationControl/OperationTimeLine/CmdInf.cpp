#include "stdafx.h"
#include "CmdInf.h"

CCommandLineInfoEx::CCommandLineInfoEx(void)
{
	mCallWindow = NULL;
	mSelectStation = 0;
	mExecuteMode = 0;
	mDebug = false;
}

void CCommandLineInfoEx::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CString str = lpszParam;
	if (str.Left(5).CompareNoCase(_T("HWND:")) == 0){
		CString strHwnd = str.Mid(5);
		mCallWindow = (HWND)((LONG_PTR)HexToDec(strHwnd));
	}
	if (str.Left(8).CompareNoCase(_T("STATION:")) == 0){
		CString temp = str.Mid(8);
		mSelectStation = (UINT)_wtoi(temp);
	}
	if (str.Left(9).CompareNoCase(_T("EXECMODE:")) == 0){
		CString temp = str.Mid(9);
		mExecuteMode = (UINT)_wtoi(temp);
	}
	if (str.Left(5).CompareNoCase(_T("DEBUG")) == 0){
		mDebug = true;
	}
	
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
}
unsigned long CCommandLineInfoEx::HexToDec(CString strHex)
{
    short i = 0;        /* 配列の添字として使用 */
    short n;
    unsigned long x = 0;

	strHex.MakeLower();

	while(strHex.GetAt(i) != '\0'){        /* 文字列の末尾でなければ */

            /* '0' から '9' の文字なら */
        if ('0' <= strHex.GetAt(i) && strHex.GetAt(i) <= '9')
            n = strHex.GetAt(i) - '0';        /* 数字に変換 */

            /* 'a' から 'f' の文字なら */
        else if ('a' <= strHex.GetAt(i) && strHex.GetAt(i) <= 'f')
            n = strHex.GetAt(i) - 'a' + 10;        /* 数字に変換 */

        else {        /* それ以外の文字なら */
            break;
        }
        i++;        /* 次の文字を指す */

        x = x *16 + n;    /* 桁上がり */
    }
    return (x);
}

