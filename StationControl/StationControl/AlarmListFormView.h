#pragma once

#include "HistoryList.h"
#include "MFCTabCtrlEx.h"
#include "SenderButton.h"
#include "ShareAlarmIf.h"
#include "afxwin.h"


// CAlarmListFormView �t�H�[�� �r���[

class CAlarmListFormView : public CFormView
{
	DECLARE_DYNCREATE(CAlarmListFormView)

protected:
	CAlarmListFormView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CAlarmListFormView();

public:
	enum { IDD = IDD_VIEW_ALARM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eAlarmItem{
		eAlarmItem_Station,
		eAlarmItem_Equipment,
		eAlarmItem_Status,
		eAlarmItem_MonName,
		eAlarmItem_MonValue,
		eAlarmItem_Max,
	};
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SIZE	mSizeTotal, mSizePage, mSizeLine;
	stAlarmData			mAlarmData;
/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CStationControlDoc* GetDocument() const;
protected:
	void	CreateHistroyList();
	void	AutoSizeColumns(int col=-1);
	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam=0);
	void	GetAlarmShareData();
	void	UpdateAlarmList();

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grStatus;
	CGroupBoxEx m_grSStatus;
	CHistoryList m_History;
	BOOL m_bStation[CBarChild::mStationMax];
	BOOL m_bImStatus[CBarChild::mStatusFlagMax];
	BOOL m_bStatus[eEqHistStatus_Max];
	CColorCheckButton m_btStation[CBarChild::mStationMax];
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_strUpdateTime;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // StationControlView.cpp �̃f�o�b�O �o�[�W����
inline CStationControlDoc* CAlarmListFormView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
