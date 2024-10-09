#pragma once
#include "TrsControlS20.h"
#include "TrsControlS34.h"
#include "TrsControlS64.h"
#include "TrsControlX.h"
#include "TrsControlXX.h"
#include "afxwin.h"


// CTrsControl ダイアログ

class CTrsControl : public CDialogBase
{
	DECLARE_DYNAMIC(CTrsControl)

public:
	CTrsControl(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTrsControl();

// ダイアログ データ
	enum { IDD = IDD_DLG_TRSCONTROL };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum {
		TYPE_NA,
		TYPE_64,
		TYPE_34,
	};
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	UINT m_SecelctType;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();
	void SetSelectType(UINT val) { m_SecelctType = val; }

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int m_BandRadio;
	afx_msg void OnBnClickedTrsctrlViewBtn();
	virtual BOOL OnInitDialog();
};
