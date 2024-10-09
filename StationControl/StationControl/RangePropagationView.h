#pragma once

// CRangePropagationView �r���[

class CRangePropagationView : public CView
{
	DECLARE_DYNCREATE(CRangePropagationView)

protected:
	CRangePropagationView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CRangePropagationView();


	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	COLORREF	m_colorStop;		// ��~���̖��̐F
	COLORREF	m_colorStart;		// �^�p���̖��̐F
	COLORREF	m_colorText;		// �e�L�X�g�̐F
	CFont		m_txtFont;			// �e�L�X�g�t�H���g

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	BOOL DrawArrow(CDC& memDC, CRect& rectClient);
	BOOL DrawPropagation(CDC& memDC, CRect& rectClient);
//	void AddZeroString(UINT val, UINT refVal, CString& str);

	DECLARE_MESSAGE_MAP()
	/* ------------------------------------------------------------------------------------ */
public:
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


