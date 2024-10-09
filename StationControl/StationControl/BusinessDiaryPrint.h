#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "BusinessDiaryEdit.h"


// CBusinessDiaryPrint ダイアログ

class CBusinessDiaryPrint : public CDialogBase
{
	DECLARE_DYNAMIC(CBusinessDiaryPrint)

public:
	CBusinessDiaryPrint(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CBusinessDiaryPrint();

// ダイアログ データ
	enum { IDD = IDD_DLG_DIARY_PRNTPERIOD };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	typedef struct {
		CString file;
		CString dir;
	}stPrintFile;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd*				m_pParent;
	vector<CString>		mSpaceList;
	CTime				mStartOpeTime, mEndOpeTime;
	vector<CString>		mPrintList;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	SetParameter(vector<CString> list, CTime st, CTime et) { mSpaceList = list; mStartOpeTime = st; mEndOpeTime = et; }
	void	WriteCoverAndHeader(CString formDir, CString satname, int iYear, int iMonth, int iSDay, int iEDay, int iAllTime);
	void	WriteCoverCsv(FILE *fp, CString formDir, CString satname, int iYear, int iMonth, int iSDay, int iEDay, int iAllTime);
	void	CreatePrintList(CString formDir, CString satname, int iYear, int iMonth);

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDateTimeCtrl mStartDate;
	CDateTimeCtrl mEndDate;
	CComboBox mCbStation;
	BOOL mSendCount;
	afx_msg void OnBnClickedDiaryPrintperiodPrntBtn();
};
