#pragma once

#include <vector>
#include "GridButton.h"

using namespace std;

// CEqStatusGridView ビュー

class CEqStatusGridView : public CScrollView
{
	DECLARE_DYNCREATE(CEqStatusGridView)

protected:
	CEqStatusGridView();           // 動的生成で使用される protected コンストラクター
	virtual ~CEqStatusGridView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
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
	vector<CGridButton*>	mButtonList;
	vector<CString>			mStatusTagList;
	UINT					mStation;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	void	UpdateStatus();

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
	virtual void OnInitialUpdate();     // 構築後 1 回目

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPushButton(UINT nID);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


