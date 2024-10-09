// NumericEdit.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "NumericEdit.h"


// CNumericEdit

IMPLEMENT_DYNAMIC(CNumericEdit, CEdit)

CNumericEdit::CNumericEdit()
{
	// determine the decimal delimiter buffer size
	const int nBuffLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, NULL, 0);
	_ASSERT(nBuffLen > 0);

	// get the decimal number delimiter
	const int nResult = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, m_strDelim.GetBuffer(nBuffLen), nBuffLen);
	_ASSERT(nResult != 0);

	m_strDelim.ReleaseBuffer();
	m_iRoundPlaceValue = 1;
}

CNumericEdit::~CNumericEdit()
{
}


BEGIN_MESSAGE_MAP(CNumericEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()


	
// CNumericEdit メッセージ ハンドラー
void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString strText;
	GetWindowText(strText);

	int iStart, iEnd;
	GetSel(iStart, iEnd);

	// 小数点が入力された場合
	if (nChar == m_strDelim)
	{
		// 空欄の場合は小数点の入力を許可しない
		if (strText.IsEmpty())
		{
			return;
		}

		// 先頭の場合は許可しない
		if (iStart == 0)
		{
			return;
		}

		// 小数点桁数が0の場合は入力を許可しない
		if (m_iRoundPlaceValue == 0)
		{
			return;
		}

		// 小数点が既に入力されている場合は入力を許可しない
		if (strText.Find(m_strDelim) >= 0)
		{
			return;
		}

		// プラスマイナスの後の小数点は許可しない
		if (iStart == 1 && (strText.Left(1) == _T("-") || strText.Left(1) == _T("+")))
		{
			return;
		}
	}

	// マイナスプラスは先頭以外は許可しない
	// マイナスプラスは既に入力されている場合は入力を許可しない
	if (nChar == '-')
	{
		if (iStart != 0)
		{
			return;
		}
		if (strText.Find('-') >= 0)
		{
			return;
		}
	}
	if (nChar == '+')
	{
		if (iStart != 0)
		{
			return;
		}
		if (strText.Find('+') >= 0)
		{
			return;
		}
	}

	// 小数点以下の入力文字数を制限
	if (nChar >= '0' && nChar <= '9' &&
		strText.Find(m_strDelim) != -1)
	{
		int iLength = strText.GetLength();
		int iRoundPlaceValue = strText.Find(m_strDelim);

		if (iStart == iEnd)
		{
			if (iStart > iRoundPlaceValue && (iLength - iRoundPlaceValue) > m_iRoundPlaceValue)
			{
				return;
			}
		}
	}

	if ((nChar >= '0' && nChar <= '9')
	 || (nChar == m_strDelim)
	 || (nChar == '-')
	 || (nChar == '+')
	 || (nChar == '\b'))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}



