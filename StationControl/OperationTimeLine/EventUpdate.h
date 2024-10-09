#pragma once
#include "afxdtctl.h"

static const CString mEventUpdateText[] = {
	_T("アップリンクの日時を変更します"),
	_T("CMDの日時を変更します"),
	_T("RNGの日時を変更します"),
};

enum{
	eEventUpdateModeUplink,
	eEventUpdateModeCmd,
	eEventUpdateModeRng,
};

// CEventUpdate ダイアログ

class CEventUpdate : public CDialogEx
{
	DECLARE_DYNAMIC(CEventUpdate)

public:
	CEventUpdate(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEventUpdate();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EVENTUPDATE };

	int		mMode;
	CTime	mStartTime, mEndTime;
	CTime	mDefStartTime, mDefEndTime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CString m_strText;
	CString m_strStart;
	CString m_strEnd;
	CDateTimeCtrl m_StartDate;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_EndDate;
	CDateTimeCtrl m_EndTime;
};
