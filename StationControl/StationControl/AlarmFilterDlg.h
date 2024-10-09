#pragma once

#include "MFCTabCtrlEx.h"
#include "SenderButton.h"

// CAlarmFilterDlg �_�C�A���O

class CAlarmFilterDlg : public CBarChild
{
	DECLARE_DYNAMIC(CAlarmFilterDlg)

public:
	CAlarmFilterDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CAlarmFilterDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_FILTER_AL };

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CGroupBoxEx m_grStation;
	CGroupBoxEx m_grImStatus;
	CGroupBoxEx m_grStatus;
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonExec();
	BOOL m_bStation[mStationMax];
	BOOL m_bImStatus[mImStatusKindMax];
	BOOL m_bStatus[mStatusKindMax];
	CColorCheckButton m_btStation[mStatusFlagMax];
};
