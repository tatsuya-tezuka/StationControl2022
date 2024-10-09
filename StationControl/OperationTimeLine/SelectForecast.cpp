// SelectForecast.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "SelectForecast.h"
#include "afxdialogex.h"


// CSelectForecast ダイアログ

IMPLEMENT_DYNAMIC(CSelectForecast, CDialogEx)

CSelectForecast::CSelectForecast(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectForecast::IDD, pParent)
{
	mForecastList.clear();
	mCurrentItem = 0;
}

CSelectForecast::~CSelectForecast()
{
}

void CSelectForecast::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mDataList);
}


BEGIN_MESSAGE_MAP(CSelectForecast, CDialogEx)
END_MESSAGE_MAP()


// CSelectForecast メッセージ ハンドラー


BOOL CSelectForecast::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	vector<CString>::iterator itr;
	mDataList.ResetContent();
	for (itr = mForecastList.begin(); itr != mForecastList.end(); itr++){
		mDataList.AddString((*itr));
	}
	mDataList.SetCurSel(mCurrentItem);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSelectForecast::OnOK()
{
	UpdateData(TRUE);
	mCurrentItem = mDataList.GetCurSel();

	CDialogEx::OnOK();
}
