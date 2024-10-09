#pragma once
#include "afxcmn.h"
#include "ForecastValue.h"
#include "ForecastOperationPlan.h"
#include "ForecastRadarGraphView.h"
#include "DetailViewDlg.h"

// CForecastList ダイアログ

class CForecastList : public CDialogBase
{
	DECLARE_DYNAMIC(CForecastList)

public:
	CForecastList(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CForecastList();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_FORECASTLIST };

	// 画面設定
	const int nTabLeftOffset =					15;		// タブウィンドウのLeftオフセット
	const int nTabTopOffset =					65;		// タブウィンドウのTopオフセット
	const int nTabBotOffset =					15;		// タブウィンドウのBotオフセット

	// 予報値取得モード
	enum ePredGetType{
		ePredGetType_Auto = 0,
		ePredGetType_Manual,
		ePredGetType_Stop,
		ePredGetType_Other,
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	CTabCtrl m_tabCtrl;					// タブコントロール
	CForecastValue m_forcastValue;		// 予報値
	CForecastOperationPlan m_operationPlan;		// 運用計画
	CComboBox m_CCBoxSatelliteName;		// 衛星名コンボボックス
	CComboBox m_CCBoxStationName;		// 局名コンボボックス
	CStatic m_viewArea;					// 予報値レーダーグラフビューエリア
	CForecastRadarGraphView* m_pView;	// 予報値レーダーグラフビュー
	CButton m_CButonDelete;				// 削除ボタン
	CButton m_CButonOffsetTime;			// 時刻オフセットボタン
	BOOL m_bUseSelectBtn;				// 選択ボタンを使用
	CString m_SelectedForecastFilePath; // 選択された予報値のファイルパス
	CDetailViewDlg mDetailDlg;			// 詳細表示画面
	vector<CString> m_listStationName;	// 局名リスト
	vector<CString> m_listSatelliteName;	// 衛星名リスト
	CWnd* m_pCallWnd;					// 呼び出しウィンドウ
	CString m_strAntFilePath;			// 予報値ファイルパス
	BOOL m_bCloseUnCheck;				// 閉じる非チェック

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void InitCombobox();
//	void InitComboAndList();
	CString GetSatelliteName();
	CString GetStationName();
	CString GetSimpleStationName();
	int GetStationIndex();
	int GetStationCount();
	void SetFocusToOffsetTimeBtn();
	CTabCtrl& GetTabCtrl()	{ return m_tabCtrl; }
	void SelectTabAndSatellite(int sel, CString& strSatelliteName);
	void SelectTabAndInitCombo(int sel);
	CButton& GetCButonOffsetTime()	{ return m_CButonOffsetTime; }
	void SetUseSelectBtn(BOOL bSel)			{ m_bUseSelectBtn = bSel; }
	int ChangeSimpleStationNameToIndex(CString stationName);
	CForecastRadarGraphView* GetView()	{ return m_pView; }
	void ViewUnSelectedMessage();
	void SetSatelliteName(CString& strStationName);
	void SetCallWnd(CWnd* pWnd)			{ m_pCallWnd = pWnd; }
	static int GetStationIndex(CString& strStationName);
	BOOL PrintList(int nStation, CString& fname);
	BOOL IsForecastList() {
		if (m_tabCtrl.GetCurSel() == eForecastListType_Forecast){
			return TRUE;
		}
		else{
			return FALSE;
		}
	}
	BOOL IsOperationList() {
		if (m_tabCtrl.GetCurSel() == eForecastListType_Operation){
			return TRUE;
		}
		else{
			return FALSE;
		}
	}

protected:
	void SelectForcastlist(int sel, CString& strSatelliteName);
	void SelectForcastlistAndInitCombo(int sel);
	UINT GetPredGetMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnMenuSetAutoMode();
	afx_msg void OnMenuSetManualModel();
	afx_msg void OnMenuGetManual ();
	afx_msg void OnMenuResend();
	afx_msg void OnTcnSelchangeForcastlistTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeForcastlistCombo();
	afx_msg void OnBnClickedForecastlistButton1();
	afx_msg void OnBnClickedForecastlistButton2();
	afx_msg void OnBnClickedForecastlistButton3();
	afx_msg void OnBnClickedForecastlistButton4();
	afx_msg void OnBnClickedForecastlistButton5();
	afx_msg void OnBnClickedForecastlistButton6();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeForcastlistStationCombo();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	/* ------------------------------------------------------------------------------------ */
};
