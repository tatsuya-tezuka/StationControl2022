#pragma once
#include "afxdtctl.h"


// CUpdateOpEnd ダイアログ

class CUpdateOpEnd : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateOpEnd)

public:
	CUpdateOpEnd(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CUpdateOpEnd();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_UPDATEOPEND };

	CTime	mUpdateTime;
	CTime	mDefTime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDateTimeCtrl m_Date;
	CDateTimeCtrl m_Time;
	virtual void OnOK();
	CString m_strTime;
};
