#pragma once
#include "afxdtctl.h"
#include "afxwin.h"

static const CString mEventAddText[] = {
	_T("アップリンクを追加します"),
	_T("CMDを追加します"),
	_T("RNGを追加します"),
};

enum{
	eEventAddModeUplink,
	eEventAddModeCmd,
	eEventAddModeRng,
};

// CEventAdd ダイアログ

class CEventAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CEventAdd)

public:
	CEventAdd(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEventAdd();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EVENTADD };

	int		mMode;
	CTime	mStartTime, mEndTime;
	CString mRngType;

protected:
	vector<CString> m_RngTypeList;

public:
	void	SetRngTypeList(vector<CString>& list) { m_RngTypeList = list; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strText;
	CDateTimeCtrl m_StartDate;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_EndDate;
	CDateTimeCtrl m_EndTime;
	virtual void OnOK();
	CComboBox m_RngTypeCmb;
	CStatic m_RngTypeStatic;
};
