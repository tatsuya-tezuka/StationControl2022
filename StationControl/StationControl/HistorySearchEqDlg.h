#pragma once

#include "MFCTabCtrlEx.h"
#include "SenderButton.h"

// CHistorySearchEqDlg �_�C�A���O

class CHistorySearchEqDlg : public CBarChild
{
	DECLARE_DYNAMIC(CHistorySearchEqDlg)

public:
	CHistorySearchEqDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CHistorySearchEqDlg();

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
protected:
	bool				m_bOneStation;
	SETUPDLGCALLBACK	m_Callback;
	UINT				m_nHistoryType;

public:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
protected:

public:
	void	SetOneStation(bool val) { m_bOneStation = val; }
	void	SetHistoryType(UINT val) { m_nHistoryType = val; }
	void	SetCallback(SETUPDLGCALLBACK callback, UINT val) { m_Callback = callback; m_nHistoryType = val; }
	//-----------------------------------------------------------------------------

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SEARCH_EQ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_StartDate;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_StopDate;
	CDateTimeCtrl m_StopTime;
	virtual BOOL OnInitDialog();
	CGroupBoxEx m_grTime;
	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grSendEq;
	CGroupBoxEx m_grImStatus;
	CGroupBoxEx m_grStatus;
	CGroupBoxEx m_grMessage;
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonPrint();
	int m_nSender[mStationMax];
	BOOL m_bStation[mStationMax];
	BOOL m_bImStatus[mImStatusKindMax];
	BOOL m_bStatus[mStatusKindMax];
	CEdit m_edMessage;
	CSenderButton m_btSender[mStationMax];
	CColorCheckButton m_btStation[mStatusFlagMax];
};
