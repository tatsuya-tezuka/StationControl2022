#pragma once
#include "afxwin.h"
#include "DownlinkCapture.h"

// CDownlinkCapture54 ダイアログ

class CDownlinkCapture54 : public CDownlinkCapture
{
	DECLARE_DYNAMIC(CDownlinkCapture54)

public:
	CDownlinkCapture54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CDownlinkCapture54();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_DOWNLINKCAPTURE54 };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString				m_strBandSelect;				// X帯<=>Ka帯切り替え文字列
	CComboBox			m_CCBoxBandSelect;				// X帯<=>Ka帯切り替えコンボボックス
	eBandSelectType		m_bandSelect;					// X帯<=>Ka帯切り替え

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL CreateDlgItems();								// ダイアログアイテムを生成
	BOOL CreateDlgBandSelect();    						// X帯/Ka帯選択ダイアログアイテムを生成
	BOOL UpdateKanshiData();							// 監視データ更新
	void ShowRcvFreqMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDownlinkcaptureBandSelectButton();
};
