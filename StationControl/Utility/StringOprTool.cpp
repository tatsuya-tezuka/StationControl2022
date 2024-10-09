/*============================================================================*/
/*! StringOprTool.cpp

-# 文字列操作ツール
*/
/*============================================================================*/
#include "stdafx.h"
#include "..\StationControl\StationControl.h"
#include "StringOprTool.h"


/*============================================================================*/
/*! CStringOprTool

-# コンストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CStringOprTool::CStringOprTool()
{
}


/*============================================================================*/
/*! CStringOprTool

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CStringOprTool::~CStringOprTool()
{
}


/*============================================================================*/
/*! CStringOprTool

-# コメント、空白の削除

@param  strDst：削除後の文字列
@param  comment：コメント文字
@param  strSrc：元の文字列
@retval TRUE：コメント、空白削除した／FALSE：コメント、空白しなかった
*/
/*============================================================================*/
BOOL CStringOprTool::TrimComment(CString& strDst, TCHAR comment, CString& strSrc)
{
	int bPos =	strSrc.Find(comment);
	if (bPos >= 0)
	{
		strDst = strSrc.Left(bPos);
	}
	else
	{
		strDst = strSrc;
	}
	strDst = strDst.Trim();

	if (strDst.Compare(strSrc) == 0)
		return FALSE;
	else
		return TRUE;
}


/*============================================================================*/
/*! CStringOprTool

-# 複数の空白を１つに置き換える

@param  strSrcDst：文字列
@retval TRUE：置き換えた／FALSE：置き換えなかった
*/
/*============================================================================*/
BOOL CStringOprTool::ReplaceMultipleSpace(CString& strSrcDst)
{
	BOOL bRet = FALSE;
	int nReplaced = 0;
	do
	{
		nReplaced = strSrcDst.Replace(_T("  "), _T(" "));
		if (nReplaced != 0)
		{
			bRet = TRUE;
		}
	} while (nReplaced != 0);

	return bRet;
}

