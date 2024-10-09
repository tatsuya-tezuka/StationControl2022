#pragma once


// CForecastValueGroupView ビュー

class CForecastValueGroupView : public CView
{
	DECLARE_DYNCREATE(CForecastValueGroupView)

protected:
	CForecastValueGroupView();           // 動的生成で使用される protected コンストラクター
	virtual ~CForecastValueGroupView();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	static const int m_nLeftOffset = 4;
	static const int m_nRightOffset = 4;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	COLORREF	m_colorText;		// テキストの色
	CFont		m_txtFont;			// テキストフォント

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL DrawGroup(CDC& memDC, CRect& rectClient);

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
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	/* ------------------------------------------------------------------------------------ */
};


