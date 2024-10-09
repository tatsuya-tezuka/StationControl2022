
// FAPanelBrowserView.h : CFAPanelBrowserView クラスのインターフェイス
//

#pragma once


class CFAPanelBrowserView : public CView
{
protected: // シリアル化からのみ作成します。
	CFAPanelBrowserView();
	DECLARE_DYNCREATE(CFAPanelBrowserView)

// 属性
public:
	CFAPanelBrowserDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CFAPanelBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // FAPanelBrowserView.cpp のデバッグ バージョン
inline CFAPanelBrowserDoc* CFAPanelBrowserView::GetDocument() const
   { return reinterpret_cast<CFAPanelBrowserDoc*>(m_pDocument); }
#endif

