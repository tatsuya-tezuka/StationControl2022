#pragma once

#include <vector>
using namespace std;

#include "TimeLine.h"
#include "afxwin.h"
#include "afxmenubutton.h"


// CTimeLineView フォーム ビュー

enum{
	ANCHORE_LEFTTOP = 0x0000,	// 左上は固定させる
	ANCHORE_RIGHT = 0x0001,	// 右側は固定させる
	ANCHORE_BOTTOM = 0x0002,	// 下側は固定させる
	RESIZE_HOR = 0x0004,	// 横のリサイズを可能にする
	RESIZE_VER = 0x0008,	// 縦のリサイズを可能にする
	RESIZE_BOTH = (RESIZE_HOR | RESIZE_VER),
};

enum{
	eChange_PredListFile,	// 予報値一覧
	eChange_PLanListFile,	// 計画一覧

	eChange_Max,
};
static const UINT mAllFileMask = 0xFFFFFFFF;

class CTimeLineView : public CFormView
{
	DECLARE_DYNCREATE(CTimeLineView)

protected:
	CTimeLineView();           // 動的生成で使用される protected コンストラクター
	virtual ~CTimeLineView();

public:
	enum { IDD = IDD_DIALOG_TIMELINE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	CStationControlDoc* GetDocument() const;

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT		mTimeRate;
	bool		mFirst;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SIZE				mSizeTotal, mSizePage, mSizeLine;
	int					mSaveCx, mSaveCy;
	vector<DWORD>		mControl;
	CTimeLine			mGraphHeader;
	CTimeLine			mGraph;
	__int64				mNextPrePassTime;
	CTime				mCurrentTime;
	CMenu				mBaseMenu;
	UINT				mSelectBase;
	CTime				mCheckTime[eChange_Max];
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	CreateTimelineData();
	void	GetCurrentUtcTime(CTime& time);
	void	GetNearPrePassTime();
protected:
	void	getAntForecastTime(CTime &start, CTime &end);
	void	setControlInfo(WORD id, WORD anchore);
	void	GetMaintComment(CString strPlan, CString& comment);
	void	GetPostPass(CString strPlan, CString& post);
//	UINT	IsChangeShareFile(UINT nMask);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	int m_nTimeScal;
	afx_msg void OnBnClickedButtonForward();
	afx_msg void OnBnClickedButtonNext();
	BOOL m_bAutoScrolOff;
	afx_msg void OnBnClickedCheckAutoscrol();
	CButton m_btForward;
	CButton m_btNext;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboTimescale();
	CNotesButton m_btCheck[12];
	CGroupBoxNote m_grpNotes;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateBaseSpace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBaseStation(CCmdUI* pCmdUI);
public:
	afx_msg void OnBnClickedButtonForecast();
	afx_msg void OnBnClickedButtonPlanlist();
	CString m_strPrePass;
	CStatic m_stNextPass;
	CMFCMenuButton mTimeLineBase;
	afx_msg void OnBnClickedMfcmenubuttonBase();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};

#ifndef _DEBUG  // OperationTimeLineView.cpp のデバッグ バージョン
inline CStationControlDoc* CTimeLineView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif

