#pragma once


// CForecastValueGroupView �r���[

class CForecastValueGroupView : public CView
{
	DECLARE_DYNCREATE(CForecastValueGroupView)

protected:
	CForecastValueGroupView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CForecastValueGroupView();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	static const int m_nLeftOffset = 4;
	static const int m_nRightOffset = 4;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	COLORREF	m_colorText;		// �e�L�X�g�̐F
	CFont		m_txtFont;			// �e�L�X�g�t�H���g

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL DrawGroup(CDC& memDC, CRect& rectClient);

public:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	/* ------------------------------------------------------------------------------------ */
};


