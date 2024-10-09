// NumericEdit.cpp : �����t�@�C��
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


	
// CNumericEdit ���b�Z�[�W �n���h���[
void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString strText;
	GetWindowText(strText);

	int iStart, iEnd;
	GetSel(iStart, iEnd);

	// �����_�����͂��ꂽ�ꍇ
	if (nChar == m_strDelim)
	{
		// �󗓂̏ꍇ�͏����_�̓��͂������Ȃ�
		if (strText.IsEmpty())
		{
			return;
		}

		// �擪�̏ꍇ�͋����Ȃ�
		if (iStart == 0)
		{
			return;
		}

		// �����_������0�̏ꍇ�͓��͂������Ȃ�
		if (m_iRoundPlaceValue == 0)
		{
			return;
		}

		// �����_�����ɓ��͂���Ă���ꍇ�͓��͂������Ȃ�
		if (strText.Find(m_strDelim) >= 0)
		{
			return;
		}

		// �v���X�}�C�i�X�̌�̏����_�͋����Ȃ�
		if (iStart == 1 && (strText.Left(1) == _T("-") || strText.Left(1) == _T("+")))
		{
			return;
		}
	}

	// �}�C�i�X�v���X�͐擪�ȊO�͋����Ȃ�
	// �}�C�i�X�v���X�͊��ɓ��͂���Ă���ꍇ�͓��͂������Ȃ�
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

	// �����_�ȉ��̓��͕������𐧌�
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



