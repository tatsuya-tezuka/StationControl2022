
// StationControlView.h : CStationControlView クラスのインターフェイス
//

#pragma once


class CStationControlView : public CView
{
protected: // シリアル化からのみ作成します。
	CStationControlView();
	DECLARE_DYNCREATE(CStationControlView)

// 属性
public:
	CStationControlDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~CStationControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // StationControlView.cpp のデバッグ バージョン
inline CStationControlDoc* CStationControlView::GetDocument() const
   { return reinterpret_cast<CStationControlDoc*>(m_pDocument); }
#endif

