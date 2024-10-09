// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#pragma once

//#include "const.h"
#include "ListCtrlEx.h"
#include "TabCtrlEx.h"
#include "StartDateDlg.h"

// CPropertiesDataItem
class CPropertiesDataItem
{
public:
	static const int SIZE_DATANAME = 80;
	static const int SIZE_DATAGNAME = 80;
	static const int SIZE_DATAUNIT = 10;

	CPropertiesDataItem()
	{
		wmemset(dataName, 0, SIZE_DATANAME);
		wmemset(dataGName, 0, SIZE_DATAGNAME);
		wmemset(dataUnit, 0, SIZE_DATAUNIT);
		isViewMode = false;
		drawMode = Line;
		color = RGB(0, 0, 0);
		dotSize = Small;
		vscaleKind = LScale;
		isComment = false;
		comment.Empty();
		isLimitWarningUpper = false;
		limitWarningUpper = 0.0;
		isLimitWarningLower = false;
		limitWarningLower = 0.0;
		isLimitAlarmUpper = false;
		limitAlarmUpper = 0.0;
		isLimitAlarmLower = false;
		limitAlarmLower = 0.0;
	};
	virtual ~CPropertiesDataItem(){};

public:
	TCHAR		dataName[SIZE_DATANAME];			// データ名称
	TCHAR		dataGName[SIZE_DATAGNAME];			// データ名称(グラフ用名)
	TCHAR		dataUnit[SIZE_DATAUNIT];			// 単位
	bool		isViewMode;			// 表示On/Off
	DrawMode	drawMode;			// 描画モード（Line、Dots、L&D）
	COLORREF	color;				// 表示色
	DotSize		dotSize;			// ドットサイズ
	VScaleKind	vscaleKind;			// 縦軸スケール選択（右/左）
	bool		isComment;			// コメント表示
	CString		comment;			// コメント
	bool		isLimitWarningUpper;	// ワーニング上限(表示)
	double		limitWarningUpper;		// ワーニング上限
	bool		isLimitWarningLower;	// ワーニング下限(表示)
	double		limitWarningLower;		// ワーニング下限 
	bool		isLimitAlarmUpper;		// アラーム上限(表示)
	double		limitAlarmUpper;		// アラーム上限
	bool		isLimitAlarmLower;		// アラーム下限(表示)
	double		limitAlarmLower;		// アラーム下限
};

// CPropertiesData
class CPropertiesData
{
public:
	CPropertiesData(){};
	virtual ~CPropertiesData(){};

public:
	int			wndIdx;						// ウィンドー番号
	CString		wndName;					// ウィンドー名称
	LPARAM		wnd;						// CView

	bool		isRealTime;					// リアルタイム（true）/ ノンリアルタイム（false）
	//	CTime		startTime;					// 開始日時
	stVScale	trandVScale;				// 縦軸スケール[0:左、1:右]
	HScale		trandHScale[MAX_HSCALE];	// 横軸スケール[0:トレンドグラフビュー1、1:トレンドグラフビュー2]

	CPropertiesDataItem item[MAX_TDATA];

public:
	int getHScal(CString str) {
		for (int i = 0; i < sizeof(strHScale) / sizeof(strHScale[0]); ++i)
		{
			if (str == strHScale[i])
			{
				return i;
			}
		};
		return -1;
	};

	void	WritePropertiesData(CString szFilePath, int pos);
	void	ReadPropertiesData(CString szFilePath, int pos);
};

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
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// コンストラクション
public:
	CPropertiesWnd();

	void AdjustLayout();

// 属性
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	void	SetNewPropertyData(int pos, LPARAM wnd);
	void	UpdateSimplicityList()
	{
		// 簡易リストを変更
		if( m_wndTabs.GetCurSel() == 1 )
			OnTcnSelchangeTab(0, 0);
	}
	void	ChangeView(int pos);
	void	CloseView(int pos);
	void	ChangePropaties(BOOL bUpdate = TRUE);
	void	UpdatePropatyDataList(BOOL bDataName = TRUE, BOOL bAutoAssign = TRUE);
	void	ExchangeViewCombo(BOOL *vBiew, CString name);

	void	SetStartDate(CString str) { m_StartDate = str; }
	CString	GetStartDate() { return m_StartDate; }
	BOOL	GetAutoScaleEnable(int pos) { return m_AutoScale[pos]; }

	void	ChangeViewMode();
	void	SetViewMode(CPropertiesData& prop, CString str);

protected:
	CString		GetDataSelectNameAt(int idx);
	int			GetDrawMode(CString str);

protected:
	CFont					m_fntPropList;
	CComboBox				m_wndObjectCombo;
	CPropertiesToolBar		m_wndToolBar;
	CMFCPropertyGridCtrl	m_wndPropList;
	CTabCtrlEx				m_wndTabs;
	CListCtrlEx				m_wndList;

	int						m_ActiveView;
	CStringList				m_DataNameList;
	CString					m_StartDate;
	BOOL					m_AutoScale[MAX_VIEW];

	// OnPropertyChanged 用
	enum
	{
		idViewMode,
//		idViewDate,
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
		idDotSize,
		idVScale,
		idCommentEnable,
		idComment,
		idLimitWarningUpperEnable,
		idLimitWarningUpper,
		idLimitWarningLowerEnable,
		idLimitWarningLower,
		idLimitAlarmUpperEnable,
		idLimitAlarmUpper,
		idLimitAlarmLowerEnable,
		idLimitAlarmLower,
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
//		idViewGrDate			// 開始日時
	};	// idViewGr


	enum 
	{
		idVAxisGrScalLGr,		// 横軸設定-表示スケール(左)
		idVAxisGrScalRGr,		// 横軸設定-表示スケール(右)
		idVAxisGrScalInvl,		// 横軸設定-目盛りの間隔
	};	// idVAxisGr

	enum
	{
		idVAxisGrScalLGrAuto,	// 横軸設定-表示スケール(左)-オートスケール
		idVAxisGrScalLGrMax,	// 横軸設定-表示スケール(左)-最大値
		idVAxisGrScalLGrMin,	// 横軸設定-表示スケール(左)-最小値
	};	// idVAxisGrScalLGr

	enum
	{
		idVAxisGrScalRGrMax,	// 横軸設定-表示スケール(右)-最大値
		idVAxisGrScalRGrMin,	// 横軸設定-表示スケール(右)-最小値
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
		idDataGrDotSiz,			// データ-ドットサイズ
		idDataGrSelScale,		// データ-縦軸スケール選択
		idDataGrComment,		// データ-コメント
		idDataGrLimit,			// データ-リミット
	};	// 	idDataGr

	enum
	{
		idDataGrCommentEnable,	// コメント-表示
		idDataGrCommentStr,		// コメント-コメント
	};//idDataGrComment

	enum
	{
		idDataGrLimitWarningUpperEnable,	// ワーニング-上限(表示)
		idDataGrLimitWarningUpper,			// ワーニング-上限
		idDataGrLimitWarningLowerEnable,	// ワーニング-下限(表示)
		idDataGrLimitWarningLower,			// ワーニング-下限
		idDataGrLimitAlarmUpperEnable,		// アラーム-上限(表示)
		idDataGrLimitAlarmUpper,			// アラーム-上限
		idDataGrLimitAlarmLowerEnable,		// アラーム-下限(表示)
		idDataGrLimitAlarmLower,			// アラーム-下限
	};//idDataGrLimit

// 実装
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnComboBoxSelChange();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

	int m_nComboHeight;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

