/*============================================================================*/
/*! ColorButtonEx2.cpp

-# �J�X�^���{�^���i�{�^���̔w�i�F�ύX�j
-# �w�肳�ꂽ�F�ɏ]���ĕ\������B
-# Ctrl�{�^���������Ɏ��s���Ȃ��F�͂Q�̖����F�Ŕ��肷��B
*/
/*============================================================================*/
// ColorButtonEx2.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ColorButtonEx2.h"


// CColorButtonEx2

IMPLEMENT_DYNAMIC(CColorButtonEx2, CColorButtonEx)

CColorButtonEx2::CColorButtonEx2()
{
	m_ignoreColor1 = COLOR_LOCAL;		// �����F1:�f�t�H���g���F
	m_ignoreColor2 = COLOR_CTRLIN;		// �����F2:�f�t�H���g�D�F
}

CColorButtonEx2::~CColorButtonEx2()
{
}


BEGIN_MESSAGE_MAP(CColorButtonEx2, CColorButtonEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CColorButtonEx2 ���b�Z�[�W �n���h���[


void CColorButtonEx2::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ((m_ColorNormal1 != m_ignoreColor1) && (m_ColorNormal1 != m_ignoreColor2) && CheckKeyClick(nFlags))
	{
		CButton::OnLButtonDown(nFlags, point);
		return;
	}
	else
	{
		return;
	}

	CButton::OnLButtonDown(nFlags, point);
}


void CColorButtonEx2::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if ((m_ColorNormal1 != m_ignoreColor1) && (m_ColorNormal1 != m_ignoreColor2) && CheckKeyClick(nFlags))
	{
		CButton::OnLButtonDblClk(nFlags, point);
		return;
	}
	else
	{
		return;
	}

	CButton::OnLButtonDblClk(nFlags, point);
}
