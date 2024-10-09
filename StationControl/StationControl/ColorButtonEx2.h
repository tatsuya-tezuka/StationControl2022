#pragma once

#include "ColorButtonEx.h"


// CColorButtonEx2

class CColorButtonEx2 : public CColorButtonEx
{
	DECLARE_DYNAMIC(CColorButtonEx2)

public:
	CColorButtonEx2();
	virtual ~CColorButtonEx2();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	COLORREF	m_ignoreColor1;		// �����F1:�f�t�H���g���F
	COLORREF	m_ignoreColor2;		// �����F2:�f�t�H���g�D�F

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetIgnoreColor1(COLORREF col)	{ m_ignoreColor1 = col; }	// �����F1�ݒ�
	void SetIgnoreColor2(COLORREF col)	{ m_ignoreColor2 = col; }	// �����F2�ݒ�
	// �{�^���F�������F1�ɂȂ��Ă��邩��Ԃ��Btrue:�����F�ɂȂ��Ă���B/ false:�Ȃ��Ă��Ȃ��B
	bool IsColorIgnore1()	{
		if (m_ColorNormal1 == m_ignoreColor1)
			return true;
		else
			return false;
	}
	// �{�^���F�������F2�ɂȂ��Ă��邩��Ԃ��Btrue:�����F�ɂȂ��Ă���B/ false:�Ȃ��Ă��Ȃ��B
	bool IsColorIgnore2()	{
		if (m_ColorNormal1 == m_ignoreColor2)
			return true;
		else
			return false;
	}

protected:
	
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


