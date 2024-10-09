#pragma once

#include "PropertiesCommonDlg.h"

// CSummaryPropertiesDlg ダイアログ

class CSummaryPropertiesDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSummaryPropertiesDlg)

public:
	CSummaryPropertiesDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSummaryPropertiesDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SUMMARYPROPERTIES };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
//	UINT			m_nStationID;				// 局ID
//	UINT			m_nViewID;					// アクティブビューID
//	UINT			m_nParentID;				// 親ウィンドウID
//	vector<CString>	m_listViewName;				// リストビュー名
//	CComboBox		m_wndObjectCombo;			// グラフのコンボボックス
//	CButton			m_btnSetting;				// 設定ボタン

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static BOOL GetViewNameList(vector<CString>& listViewName);
	static BOOL GetViewTrendNameList(vector<CString>& listViewName);
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nViewID, UINT nStaionID, UINT nParentID = 0);
//	UINT GetStationID()		{ return m_nStationID; }
//	UINT GetParentID()		{ return m_nParentID; }
//	int GeCurComboSel()		{ return m_wndObjectCombo.GetCurSel(); }

protected:
//	CPropertiesData&  GetPropertiesData(int pos);
//	void SetPropertiesData(int pos, CPropertiesData& val);
//	void UpdateComboBoxSelect();
//	void SendPropertiesChange();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnComboBoxSelChange();
//	afx_msg void OnBnClickedSummarygrapthPropButton();
	virtual BOOL OnInitDialog();
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
