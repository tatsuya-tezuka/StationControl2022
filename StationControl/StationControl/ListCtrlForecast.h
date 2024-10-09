#pragma once

//#include "ListCtrlEx.h"

// CListCtrlForecast

class CListCtrlForecast : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlForecast)

public:
	CListCtrlForecast();
	virtual ~CListCtrlForecast();

	/* ------------------------------------------------------------------------------------ */
	/* íËã`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* ÉÅÉìÉoïœêî                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	map<CString, COLORREF>	m_mapColor;

	/* ------------------------------------------------------------------------------------ */
	/* ÉÅÉìÉoä÷êî                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetColorMap(map<CString, COLORREF>	mapColor)	{ m_mapColor = mapColor; }

protected:
	COLORREF GetCelColorAt(CString strItem);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	/* ------------------------------------------------------------------------------------ */
};


