#pragma once
#include "fapanel1.h"



// CFormFaPanelView �t�H�[�� �r���[

class CFormFaPanelView : public CFormView
{
	DECLARE_DYNCREATE(CFormFaPanelView)

protected:
	CFormFaPanelView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CFormFaPanelView();

public:
	enum { IDD = IDD_FORM_FAPANEL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	CFAPanelBrowserDoc* GetDocument() const;
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SIZE				mSizeTotal, mSizePage, mSizeLine;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CFapanel1 mFaPanelCtrl;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // FAPanelBrowserView.cpp �̃f�o�b�O �o�[�W����
inline CFAPanelBrowserDoc* CFormFaPanelView::GetDocument() const
{
	return reinterpret_cast<CFAPanelBrowserDoc*>(m_pDocument);
}
#endif

