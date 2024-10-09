#pragma once

#include "HistoryList.h"
#include "MFCTabCtrlEx.h"
#include "SenderButton.h"
#include "afxcmn.h"
#include "afxwin.h"


// CSearchEqView フォーム ビュー

class CSearchEqView : public CFormView
{
	DECLARE_DYNCREATE(CSearchEqView)

protected:
	CSearchEqView();           // 動的生成で使用される protected コンストラクター
	virtual ~CSearchEqView();

	enum eOptionType{
		eOptionType_Operation,
		eOptionType_Control,
		eOptionType_Equipment,
		eOptionType_Event,
		eOptionType_Max,
	};
	enum eSearchFileTye{
		eSearchFileType_Date,
		eSearchFileType_Time,
		eSearchFileType_Equipment,
		eSearchFileType_MessageID,
		eSearchFileType_Status,
		eSearchFileType_MessageString,
		eSearchFileType_Station,
	};

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	SIZE				mSizeTotal, mSizePage, mSizeLine;
	bool				m_bOneStation;
	UINT				m_nHistoryType;
	UINT				m_nControlID;
	list<stSearchHistoryData>	mHistoryData;
	CString				m_strTitle;
	stFilterData		m_SearchData;

public:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:
	void	CreateHistroyList();
	void	AutoSizeColumns(int col = -1);
	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);
	void	setControlInfo(WORD id, WORD anchore);

	void	SetDisplaySearch(int nFilterType, stFilterData& data);
	bool	IsMessageEvent(TCHAR* pmsg);
	bool	_GetDate(stSearchHistoryData& hdata, CString cbuf);
	bool	_GetTime(stSearchHistoryData& hdata, CString cbuf);
	bool	_GetStation(stSearchHistoryData& hdata, CString cbuf);
	bool	_GetEquipment(stSearchHistoryData& hdata, CString cbuf);
	bool	_GetImStatus(stSearchHistoryData& hdata, CString cbuf);
	bool	_GetStatus(stSearchHistoryData& hdata, CString cbuf);
	bool	_GetMessageID(stSearchHistoryData& hdata, CString cbuf);
	bool	_GetMessageString(stSearchHistoryData& hdata, CString cbuf);
	bool	WildCard(TCHAR* pwild, TCHAR* pstr);

public:
	CStationControlDoc* GetDocument() const;
	void	SetOneStation(bool val) { m_bOneStation = val; }
	void	SetHistoryType(UINT val) { m_nHistoryType = val; }
	//-----------------------------------------------------------------------------

public:
	enum { IDD = IDD_VIEW_EQSEARCH };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	CDateTimeCtrl m_StartDate;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_StopDate;
	CDateTimeCtrl m_StopTime;
	CGroupBoxEx m_grTime;
	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grSendEq;
	CGroupBoxEx m_grStatus;
	CGroupBoxEx m_grSStatus;
	CGroupBoxEx m_grMessage;
	CGroupBoxEx m_grOption;
	CGroupBoxEx m_grText;
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonPrint();
	int m_nSender[CBarChild::mStationMax];
	BOOL m_bStation[CBarChild::mStationMax];
	BOOL m_bImStatus[CBarChild::mStatusFlagMax];
	BOOL m_bStatus[eEqHistStatus_Max];
	CEdit m_edMessage;
	CSenderButton m_btSender[CBarChild::mStationMax];
	CColorCheckButton m_btStation[CBarChild::mStatusFlagMax];
	BOOL m_bOption[eOptionType_Max];
	CHistoryList m_History;
	CButton m_btNext;
	CButton m_btPrev;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrev();
	CString m_strText;
	afx_msg void OnBnClickedCheckStation1();
	afx_msg void OnBnClickedCheckStation2();
	afx_msg void OnBnClickedCheckStation3();
	afx_msg void OnBnClickedCheckStation4();
};

#ifndef _DEBUG  // StationControlView.cpp のデバッグ バージョン
inline CStationControlDoc* CSearchEqView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
