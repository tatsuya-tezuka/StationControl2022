#pragma once
#include "afxwin.h"
#include "const.h"
#include "TblFailSelection.h"

// CTblSatelliteSelection ダイアログ


class CTblSatelliteSelection : public CDialogEx
{
	DECLARE_DYNAMIC(CTblSatelliteSelection)

public:
	CTblSatelliteSelection(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTblSatelliteSelection();

// ダイアログ データ
	enum { IDD = IDD_DLG_TBL_SATSEL };

	//-----------------------------------------------------------------------------
	// 定義
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTblSatselBtn();
	virtual BOOL OnInitDialog();
	// 衛星選択コンボボックス
	CComboBox m_SatCmb;
};
