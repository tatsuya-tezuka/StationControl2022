#pragma once


// CTabCtrlEx

class CTabCtrlEx : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlEx)

public:
	CTabCtrlEx();
	virtual ~CTabCtrlEx();

	CWnd*	mpMessageWnd;
	void SetMessageWnd(CWnd* p) { mpMessageWnd = p; }

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
};


