#pragma once

#include "PropertiesCommonDlg.h"


// CTrendPropertiesDlg ダイアログ

class CTrendPropertiesDlg : public CPropertiesCommonDlg
{
	DECLARE_DYNAMIC(CTrendPropertiesDlg)

public:
	CTrendPropertiesDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	CTrendPropertiesDlg(CWnd* pParent, UINT nViewID, vector<CString>& listViewName, UINT nPropertiesID, UINT nStaionID, UINT nParentID = 0, UINT nPropertiesType = ePropertiesType_Trend,
		CString strWndName = _T(""), CWnd* pSummaryProp = NULL, BOOL bChgDlg = FALSE);
	virtual ~CTrendPropertiesDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_TRENDPROPERTIES };

	enum ePropertiesType
	{
		ePropertiesType_Trend = 0,		// トレンド・プロパティ
		ePropertiesType_Summary,		// サマリ・プロパティ
	};

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT m_PropertiesType;				// プロパティ・タイプ
	UINT m_nStationID;					// 局ID
	CString m_strWndName;				// ウィンドウ名
	BOOL m_bChgGraphPropDlg;			// グラフプロパティダイアログ変更
	CWnd* m_pSummaryProp;				// サマリプロパティウィンドウポインタ
	BOOL m_bChgDlg;						// ダイアログ種類変更
	CPoint m_ptWnd;						// ウィンドウ位置
	BOOL m_bModified;					// 変更フラグ

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void AdjustLayout();
	virtual void ChangePropaties(BOOL bUpdate = TRUE);
	virtual void UpdatePropatyDataList();
	virtual void ChangeView(int pos);

protected:
	virtual void InitPropList();
	virtual CPropertiesData& GetPropertiesData(int pos);
	virtual void SetPropertiesData(int pos, CPropertiesData& val);
	virtual void SendPropertiesChange();

	void BackupPropertiesData();
	void RestorePropertiesData();

	void ShowProperties();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnComboBoxSelChange();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
