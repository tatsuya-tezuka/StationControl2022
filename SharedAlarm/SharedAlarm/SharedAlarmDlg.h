
// SharedAlarmDlg.h : ヘッダー ファイル
//

#pragma once

#include "ShareAlarmIf.h"
#include "afxcmn.h"
#include "afxwin.h"

// CSharedAlarmDlg ダイアログ
class CSharedAlarmDlg : public CDialogEx
{
// コンストラクション
public:
	CSharedAlarmDlg(int mode, CString alarmFile = _T(""), CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_SHAREDALARM_DIALOG };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
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
	/* メンバ関数                                                                           */
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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
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
