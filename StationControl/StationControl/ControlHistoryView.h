#pragma once

#include "HistroyView.h"

// CControlHistoryView ビュー

class CControlHistoryView : public CHistroyView
{
	DECLARE_DYNCREATE(CControlHistoryView)

protected:
	CControlHistoryView();           // 動的生成で使用される protected コンストラクター
	virtual ~CControlHistoryView();

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
	BOOL bLost;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void	AddHistroy();
	virtual void	AddHistroy(stHistoryData& data, bool bDataAdd = true);
protected:
	/* ------------------------------------------------------------------------------------ */

	// 属性
public:

public:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
