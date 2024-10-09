#pragma once
#include "afxwin.h"

#include <vector>

using namespace std;

// CSelectForecast �_�C�A���O

class CSelectForecast : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectForecast)

public:
	CSelectForecast(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSelectForecast();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SELECTFORECAST };

	vector<CString>		mForecastList;
	int					mCurrentItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox mDataList;
	virtual void OnOK();
};
