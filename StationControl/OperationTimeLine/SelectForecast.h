#pragma once
#include "afxwin.h"

#include <vector>

using namespace std;

// CSelectForecast ダイアログ

class CSelectForecast : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectForecast)

public:
	CSelectForecast(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSelectForecast();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SELECTFORECAST };

	vector<CString>		mForecastList;
	int					mCurrentItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox mDataList;
	virtual void OnOK();
};
