#pragma once

#include "afxwin.h"
#include "TabCtrlEx.h"

#define ID_PROPERTIES_CB			(WM_USER +311)
#define ID_PROPERTIES_TAB           (WM_USER +500)
#define ID_PROPERTIES_LIST          (WM_USER +501)

// CCustomDlgProp
class CCustomDlgProp : public CMFCPropertyGridProperty
{
public:
	CCustomDlgProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData);

	void SetParent(CWnd* pWnd) { m_Parent = pWnd; }
	CWnd* GetParent() { return m_Parent; }

protected:
	CWnd*	m_Parent;

	virtual BOOL HasButton() const { return TRUE; }
	virtual void OnClickButton(CPoint point);
};

// CPropertiesToolBar
class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


// CPropertiesCommonDlg ダイアログ

class CPropertiesCommonDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CPropertiesCommonDlg)

public:
	CPropertiesCommonDlg(UINT IDD, CWnd* pParent = NULL);   // 標準コンストラクター
	CPropertiesCommonDlg(UINT IDD, CWnd* pParent, UINT nViewID, vector<CString>& listViewName, UINT nPropertiesID);
	virtual ~CPropertiesCommonDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_TRENDPROPERTIES };

	// OnPropertyChanged 用
	enum
	{
		idViewMode,
		idViewDate,
		idVLAxisAutoRange,
		idVLAxisMaxRange,
		idVLAxisMinRange,
		idVRAxisMaxRange,
		idVRAxisMinRange,
		idVAxisScaleInterval,
		idHUAxisScale,
		idHLAxisScale,
		idAxisMax,
	};
	enum
	{
		idSelectItem = idAxisMax,
		idItemName,
		idItemUnit,
		idDisplay,
		idDrawKind,
		idDrawColor,
		idLineWidth,
		idDotSize,
		idVScale,
		idComment,
		idCommentEnable,
		idAlarmEnable,
		idAlarm1Major,
		idAlarm1Minor,
		idAlarm2Major,
		idAlarm2Minor,
		idMax,
	};

	// データセット 用　>>>
	enum
	{
		idViewGr,				// 表示設定
		idVAxisGr,				// 横軸設定
		idHAxisGr,				// 縦軸設定
		idDataGr,				// データx
	};	// Top Group

	enum
	{
		idViewGrMode,			// 表示タイプ
		idViewGrDate			// 開始日時
	};	// idViewGr

	enum
	{
		idVAxisGrScalLGr,		// 縦軸設定-表示スケール(左)
		//		idVAxisGrScalRGr,		// 縦軸設定-表示スケール(右)
		idVAxisGrScalInvl,		// 縦軸設定-目盛りの間隔
	};	// idVAxisGr

	enum
	{
		idVAxisGrScalLGrAuto,	// 縦軸設定-表示スケール(左)-オートスケール
		idVAxisGrScalLGrMax,	// 縦軸設定-表示スケール(左)-最大値
		idVAxisGrScalLGrMin,	// 縦軸設定-表示スケール(左)-最小値
	};	// idVAxisGrScalLGr

	enum
	{
		idVAxisGrScalRGrMax,	// 縦軸設定-表示スケール(右)-最大値
		idVAxisGrScalRGrMin,	// 縦軸設定-表示スケール(右)-最小値
	};	// idVAxisGrScalRGr

	enum
	{
		idHAxisGrScaleU,		// 縦軸設定-表示スケール(上)
		idHAxisGrScaleL,		// 縦軸設定-表示スケール(下)
	};	// 	idHAxisGr

	enum
	{
		idDataGrDataSelect,		// データ-データ選択
		idDataGrName,			// データ-名称
		idDataGrItemUnit,		// データ-単位
		idDataGrDisplay,		// データ-表示
		idDataGrDrawKind,		// データ-描画モード
		idDataGrColor,			// データ-表示色
		idDataGrLineWidth,		// 線幅
		idDataGrDotSiz,			// データ-ドットサイズ
		//		idDataGrSelScale,		// データ-縦軸スケール選択
		idDataGrComment,		// データ-コメント
		idDataGrAlarm,			// データ-アラーム
	};	// 	idDataGr

	enum
	{
		idDataGrCommentStr,		// コメント-コメント
		idDataGrCommentEnable,	// コメント-表示
	};//idDataGrComment

	enum
	{
		idDataGrAlarmEnable,	// アラーム-表示
		idDataGrAlarm1Major,	// アラーム1-上限
		idDataGrAlarm1Minor,	// アラーム1-下限
		idDataGrAlarm2Major,	// アラーム2-上限
		idDataGrAlarm2Minor,	// アラーム2-下限
	};//idDataGrAlarm

	static const int mOKButtonAreaHeight = 40;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CFont					m_fntPropList;
	CComboBox				m_wndObjectCombo;
	//	CPropertiesToolBar		m_wndToolBar;
	CMFCPropertyGridCtrl	m_wndPropList;
	//	CTabCtrlEx				m_wndTabs;
	//	CListCtrlEx				m_wndList;

	int						m_nViewID;
	int						m_nPropertiesID;
	int						m_nParentID;
	CStringList				m_DataNameList;
	CString					m_StartDate;

	int						m_nComboHeight;

	vector<CString>			m_listViewName;

	CPropertiesData			m_backupPropertiesData[eTrendWndType_Num];
	CPropertiesData			m_backupSummaryPropertiesData[MAX_SUMMARY_VIEW];

	int						m_nListMax;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual void AdjustLayout() = 0;

	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	void	SetNewPropertyData(int pos, CString name, LPARAM wnd);
	virtual void ChangeView(int pos) = 0;
	void	CloseView(int pos);
	virtual void ChangePropaties(BOOL bUpdate = TRUE) = 0;
	virtual void UpdatePropatyDataList() = 0;
	void	ExchangeViewCombo(BOOL *vBiew, CString name);

	void	SetStartDate(CString str) { m_StartDate = str; }
	CString	GetStartDate() { return m_StartDate; }

	CString		GetDataSelectNameAt(int idx);

protected:
	int			GetDrawMode(CString str);

	virtual void InitPropList() = 0;
	virtual CPropertiesData& GetPropertiesData(int pos) = 0;
	virtual void SetPropertiesData(int pos, CPropertiesData& val) = 0;
	virtual void SendPropertiesChange() = 0;

	void SetPropListFont();

	//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
//	afx_msg void OnComboBoxSelChange();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	/* ------------------------------------------------------------------------------------ */
};
