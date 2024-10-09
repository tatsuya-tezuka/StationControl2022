#pragma once

#include "ListCtrlForecast.h"

// CForecastOperationPlan ダイアログ

class CForecastOperationPlan : public CDialogBase
{
	DECLARE_DYNAMIC(CForecastOperationPlan)

public:
	CForecastOperationPlan(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CForecastOperationPlan();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_FORECASTOPERATIONPLAN };

	enum eOperationType {
		eOp		 = 0,
		eRdnChk 
	};

	enum eExecResult {
		eOK = 0,
		eNG
	};

	// リスト項目の表示幅
	static const int m_nStationNameWidth = 100;			// 局名項目幅
	static const int m_nSatelliteNameWidth = 120;		// 衛星名項目幅
	static const int m_nPassIdWidth = 80;				// パスID項目幅
	static const int m_createTypeWidth = 60;			// 作成項目幅
	static const int m_operationTypeWidth = 100;		// 運用種別項目幅
	static const int m_prePassStartTimeWidth = 120;		// プリパス開始日時項目幅
	static const int m_passStartTimeWidth = 120;		// パス開始日時項目幅
	static const int m_passEndTimeWidth = 120;			// パス終了日時項目幅
//	static const int m_execResultWidth = 60;			// 実行結果項目幅

	typedef struct {
		CString			m_stationName;			// 局名
		CString			m_satelliteName;		// 衛星名
//		UINT			m_nPlanId;				// 計画ID
		CString			m_passId;				// パスID
		CString			m_createBy;				// 作成
//		eOperationType  m_operationType;		// 運用種別
		CString			m_operationType;		// 運用種別
		CTime			m_prePassStartTime;		// プリパス開始日時
		CTime			m_passStartTime;		// パス開始日時
		CTime			m_passEndTime;			// パス終了日時
		CString			m_filepath;				// 計画ファイルパス
//		eExecResult		m_execResult;			// 実行結果
//		CString			m_execResult;			// 実行結果
	} structListItem;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CListCtrlForecast m_listOperationPlan;		// 運用計画リストコントロール
	BOOL m_bUpperSort;					// 昇順ソート
	int m_nSelectedColumn;				// 選択されたカラム
	CString m_strStationName;			// 局名
	CString m_strSatelliteName;			// 衛星名
	int m_nStationIndex;				// 局インデックス
//	deque<structListItem> m_dequeItem;	// アイテムキュー
//	vector<stPlanListData>	m_listPlanList;		// 計画一覧データ
	CString m_strSelectedStation;		// 選択された局
	CString m_strSelectedSatellite;		// 選択された衛星
	CString m_strSelectedPassId;		// 選択された計画ID
	CString m_strSelectedGetFrom;		// 選択された作成元
	CString	m_strSelectedFile;			// 選択されたファイル

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL InitList();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int CompareFunc(LPARAM lParam1, LPARAM lParam2);
	BOOL SetStationName();
	BOOL SetStationIndex();
	BOOL SetSatelliteName();
	void CreateSampleData();
	void GetPlanList();

public:
	BOOL InsertListItem();
	BOOL UpdateListItem();
	BOOL GetSelectedPredFilePath(CString& strFilePath, BOOL bChk = FALSE);
	BOOL GetSelectedPlanID(CString& strPlanID, __int64& tPassStart, __int64& tPassEnd, BOOL bChk = FALSE);
	BOOL GetSelectedStationNameAndPredFilePath(CString& strStationName, CString& strSatelliteName, CString& strPassId, CString& strGetFrom, CString& strFilePath, BOOL bChk = FALSE);
	BOOL GetSelectedStationNameAndPlanFilePath(CString& strStationName, CString& strSatelliteName, CString& strFilePath, CString& strGetFrom, BOOL bChk = FALSE);
	BOOL GetSelectedStationNameAndPlanFilePath(vector<stPlanListData>& listPlan, BOOL bChk = FALSE);
	BOOL IsUsedPlan(CString& strStationName, CString& strSatelliteName, CString& strPredPassId);
	void InitRadarGraphView();
	BOOL GetStationNameList(vector<CString>& listName);
	BOOL GetSatelliteNameList(vector<CString>& listName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHdnItemclickForecastoperationplanList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickForecastoperationplanList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	/* ------------------------------------------------------------------------------------ */
};
