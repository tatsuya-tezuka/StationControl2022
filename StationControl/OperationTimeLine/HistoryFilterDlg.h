#pragma once

#include "MFCTabCtrlEx.h"
#include "SenderButton.h"

// CHistoryFilterDlg �_�C�A���O

class CHistoryFilterDlg : public CBarChild
{
	DECLARE_DYNAMIC(CHistoryFilterDlg)

public:
	CHistoryFilterDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CHistoryFilterDlg();

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
	void	SetCallback(SETUPDLGCALLBACK callback, UINT val) { m_Callback = callback; m_nHistoryType = val; }
	//-----------------------------------------------------------------------------

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grSendEq;
	CGroupBoxEx m_grStatus;
	CGroupBoxEx m_grMessage;
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	int m_nSender;
	BOOL m_bStation[mStationMax];
	BOOL m_bStatus[mStatusFlagMax];
	CEdit m_edMessage;
	CSenderButton m_btSender;
	CColorCheckButton m_btStation[mStatusFlagMax];
};
