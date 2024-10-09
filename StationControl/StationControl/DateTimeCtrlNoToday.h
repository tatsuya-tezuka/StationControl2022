#pragma once


// CDateTimeCtrlNoToday

class CDateTimeCtrlNoToday : public CDateTimeCtrl
{
	DECLARE_DYNAMIC(CDateTimeCtrlNoToday)

public:
	CDateTimeCtrlNoToday();
	virtual ~CDateTimeCtrlNoToday();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDtnDropdown(NMHDR *pNMHDR, LRESULT *pResult);
};


