
// SharedAlarmDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

#include "ShareAlarmIf.h"
#include "afxcmn.h"
#include "afxwin.h"

// CSharedAlarmDlg �_�C�A���O
class CSharedAlarmDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CSharedAlarmDlg(int mode, CString alarmFile = _T(""), CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SHAREDALARM_DIALOG };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CShareAlarmIf		mShareAlarmIf;
	CString				mAlarmFile;
	static CCriticalSection	mUpdateSection;
	vector<CShareAlarmIf::stAlarmItem> mAlarmList;
	int					mStation;
	CButton				m_ctlSta64;
	CButton				m_ctlSta34;
	CButton				m_ctlSta20;
	CButton				m_ctlSta54;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	void	UpdateControl();
	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwAll=0);
	void	AutoSizeColumns(int col=-1);
	void	UpdateAlarmList(vector<CShareAlarmIf::stAlarmItem>& list);
	int		GetStation();
	void	SetStation(int nStation);
	void	ClearStation();

	void	ReadAlarmFile();
	bool	CreateShareMem(int nStation);
	bool	CreateShareMemUpdateDisplay(int nStation);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAlarm();
	int m_nClient;
	CListCtrl mList;
	afx_msg void OnBnClickedRadioType1();
	afx_msg void OnBnClickedRadioType2();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonRecv();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	BOOL m_bStation[CShareAlarmIf::mAlarmStationMax];
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	CString m_strUpdateTime;
};
