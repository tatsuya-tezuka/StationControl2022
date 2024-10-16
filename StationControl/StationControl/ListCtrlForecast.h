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
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	map<CString, COLORREF>	m_mapColor;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
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


