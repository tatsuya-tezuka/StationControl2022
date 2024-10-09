#pragma once

#include "MFCEqTabCtrl.h"
#include "TabBlock.h"
#include "TabAntenna.h"

enum{
	eTabBlock,
	//eTabAntenna,

	eTabMax,
};

typedef struct  {
	UINT			id;
	CRuntimeClass*	pset;
	CString			name;
} stTabItem;
static const stTabItem mTabItem[] = {
	{ IDD_TAB_BLOCK, RUNTIME_CLASS(CTabBlock), _T("FA-Panel"), },
	{ IDD_TAB_ANTENNA, RUNTIME_CLASS(CTabAntenna), _T("アンテナ"), },
};

// CEquipmentSystem フォーム ビュー

class CEquipmentSystem : public CFormView
{
	DECLARE_DYNCREATE(CEquipmentSystem)

protected:
	CEquipmentSystem();           // 動的生成で使用される protected コンストラクター
	virtual ~CEquipmentSystem();

public:
	enum { IDD = IDD_DIALOG_EQSYSTEM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	COperationTimeLineDoc* GetDocument() const;
	CDialogEx *mpDlg;

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SIZE				mSizeTotal, mSizePage, mSizeLine;
	int					mSaveCx, mSaveCy;
	CMFCEqTabCtrl		mTabCtrl;
	vector<CWnd*>		mControlList;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	void	createTab();

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // OperationTimeLineView.cpp のデバッグ バージョン
inline COperationTimeLineDoc* CEquipmentSystem::GetDocument() const
{
	return reinterpret_cast<COperationTimeLineDoc*>(m_pDocument);
}
#endif

