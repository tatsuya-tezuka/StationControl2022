#pragma once

#include "ForecastValueGroupView.h"
#include "DateTimeCtrlEx.h"
#include "ListCtrlForecast.h"

// CForecastValue ダイアログ

class CForecastValue : public CDialogBase
{
	DECLARE_DYNAMIC(CForecastValue)

public:
	CForecastValue(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CForecastValue();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_FORECASTVALUE };

	enum eGenSen {
		eAnt = 0,
		eTle
	};

	enum eShikibetsu {
		eNOM = 0,
		eOFF
	};

	// リスト項目の表示幅
	static const int m_nStationNameWidth =			100;	// 衛星名項目幅
	static const int m_nSatelliteNameWidth =		120;	// 衛星名項目幅
	static const int m_nGensenWidth =				 40;	// 源泉項目幅
	static const int m_nSikibetuWidth =				 40;	// 識別項目幅
	static const int m_nPassIDWidth =				 80;	// パスID項目幅
	static const int m_nDateWidth =					120;	// DATE TIME項目幅
//	static const int m_nTimeWidth =					 60;	// DATE TIME項目幅
	static const int m_nAZWidth =					 60;	// AZ EL項目幅
	static const int m_nOffsetWidth	=				 60;	// OFFSET項目幅
	static const int m_nFormatWidth =				 60;	// FORMAT項目幅
	static const int m_nGetStateWidth =				 50;	// 取得項目幅

	typedef struct {
		UINT m_upper;
		UINT m_lower;
	} stPassId;

	typedef struct {
		CString			m_stationName;			// 局名
		CString			m_satelliteName;		// 衛星名
//		eGenSen			m_gensen;				// 源泉
//		eShikibetsu		m_shikibetsu;			// 識別
//		stPassId		m_passId;				// パスID
		CString			m_passId;				// パスID
		CTime			m_aosTime;				// DATE TIME (AOS)
		double			m_aosAz;				// AZ (AOS)
		double			m_aosEl;				// EL (AOS)
		CTime			m_losTime;				// DATE TIME (LOS)
		double			m_losAz;				// AZ (LOS)
		double			m_losEl;				// EL (LOS)
		CTime			m_maxelTime;			// DATE TIME (MAX EL)
		double			m_maxelAz;				// AZ (MAX EL)
		double			m_maxelEl;				// EL (MAX EL)
//		double			m_offset;				// OFFSET
		time_t			m_offset;				// OFFSET
		CString			m_format;				// FORMAT
		CString			m_getState;				// 取得
		CString			m_filepath;				// 予報値ファイルパス
	} structListItem;

	typedef struct {
		int x1;
		int x2;
	} structGroupXPos;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
//	CListCtrl m_listForecastValue;		// 予報値リストコントロール
//	CListCtrlEx m_listForecastValue;		// 予報値リストコントロール
	CListCtrlForecast m_listForecastValue;		// 予報値リストコントロール
	CStatic m_viewArea;					// 予報値グループビューエリア
	CForecastValueGroupView* m_pView;	// 予報値グループビュー
	BOOL m_bUpateView;					// 予報値グループビューの更新
	BOOL m_bUpperSort;					// 昇順ソート
	int m_nSelectedColumn;				// 選択されたカラム
	CString m_strStationName;			// 局名
	int m_nStationIndex;				// 局インデックス
	CString m_strSatelliteName;			// 衛星名
//	deque<structListItem> m_dequeItem;	// アイテムキュー
//	CEdit m_editCtrl;					// エディットコントロール
//	CDateTimeCtrl m_timeCtrl;			// 時刻コントロール
//	CDateTimeCtrlEx m_timeCtrl;			// 時刻コントロール
	int m_nPosEditCtrl;					// エディットコントロール位置
	time_t m_offset;					// OFFSET
	CString	m_strSelectedFilePath;		// 選択されたファイルパス

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL InitList();
	BOOL CreateGroupView();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int CompareFunc(LPARAM lParam1, LPARAM lParam2);
	BOOL SetStationName();
	BOOL SetStationIndex();
	BOOL SetSatelliteName();
	void CreateSampleData();
//	int GetPassIdIndex(CString strPassId, deque<structListItem>& list);

public:
	void GetAosXPos(structGroupXPos& stXPos);
	void GetLosXPos(structGroupXPos& stXPos);
	void GetMaxElXPos(structGroupXPos& stXPos);
	BOOL InsertListItem();
	BOOL UpdateListItem();
//	BOOL DeleteListItem();
	BOOL GetSelectedForecastFilePath(CString& strFilePath, BOOL bChk = FALSE);
//	BOOL GetSelectedForecastStationNameAndFilePath(CString& strStationName, CString& strFilePath, BOOL bChk = FALSE);
	BOOL GetSelectedForecastStationNameAndFilePath(CString& strStationName, CString& strFilePath, int& nFormat, BOOL bChk = FALSE);
	BOOL GetSelectedForecastStationNameAndFullFilePath(CString& strStationName, CString& strSatelliteName, CString& strPassId, CString& strFilePath, BOOL bChk = FALSE);
	BOOL GetSelectedForecastStationNameAndFullFilePath(vector<stPredListData>& listPred, BOOL bChk = FALSE);
	BOOL GetSelectedForecastOffsetTime(time_t& offsetTime);
	BOOL GetStationNameList(vector<CString>& listName);
	BOOL GetSatelliteNameList(vector<CString>& listName);

	void InitRadarGraphView();
	CForecastValueGroupView* GetGroupView()	{ return m_pView; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnLvnItemchangedForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHdnItemclickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLvnBeginScrollForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnEndtrackForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	/* ------------------------------------------------------------------------------------ */
};
