#pragma once

// CRangePropagationView ビュー

class CRangePropagationView : public CView
{
	DECLARE_DYNCREATE(CRangePropagationView)

protected:
	CRangePropagationView();           // 動的生成で使用される protected コンストラクター
	virtual ~CRangePropagationView();


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
	COLORREF	m_colorStop;		// 停止中の矢印の色
	COLORREF	m_colorStart;		// 運用中の矢印の色
	COLORREF	m_colorText;		// テキストの色
	CFont		m_txtFont;			// テキストフォント

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	BOOL DrawArrow(CDC& memDC, CRect& rectClient);
	BOOL DrawPropagation(CDC& memDC, CRect& rectClient);
//	void AddZeroString(UINT val, UINT refVal, CString& str);

	DECLARE_MESSAGE_MAP()
	/* ------------------------------------------------------------------------------------ */
public:
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


