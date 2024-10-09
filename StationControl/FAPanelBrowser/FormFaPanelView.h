#pragma once
#include "fapanel1.h"



// CFormFaPanelView フォーム ビュー

class CFormFaPanelView : public CFormView
{
	DECLARE_DYNCREATE(CFormFaPanelView)

protected:
	CFormFaPanelView();           // 動的生成で使用される protected コンストラクター
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
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	CFAPanelBrowserDoc* GetDocument() const;
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SIZE				mSizeTotal, mSizePage, mSizeLine;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CFapanel1 mFaPanelCtrl;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // FAPanelBrowserView.cpp のデバッグ バージョン
inline CFAPanelBrowserDoc* CFormFaPanelView::GetDocument() const
{
	return reinterpret_cast<CFAPanelBrowserDoc*>(m_pDocument);
}
#endif

