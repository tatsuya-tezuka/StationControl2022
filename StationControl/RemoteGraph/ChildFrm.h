// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ChildFrm.h : CChildFrame クラスのインターフェイス
//

#pragma once

#include "const.h"

using namespace std;

#if 0
typedef struct {
	int			wndIdx;				// ウィンドー番号
	CString		wndName;			// ウィンドー名称

	CString		itemName;			// データ名称
	bool		isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
	bool		isViewMode;			// 表示On/Off
	DrawMode	drawMode;			// 描画モード（Line、Dots、L&D）
	COLORREF	color;				// 表示色
	DotSize		dotSize;			// ドットサイズ
	bool		isVAxisAutRange;	// 縦軸オートスケール
	float		VAxisScaleIntvl;	// 縦軸目盛りの間隔
	float		VAxisLabelIntvl;	// 縦軸ラベルの間隔
	float		VAxisMaxRange;		// 縦軸最大値
	float		VAxisMinRange;		// 縦軸最小値
	bool		isHAxisAutRange;	// 横軸オートスケール
	AxisUnit	HAxisUnit;			// 横軸単位（秒/分/時）
	float		HAxisScale;			// 横軸表示スケール
	float		HAxisScaleIntvl;	// 横軸目盛りの間隔
	float		HAxisLabelIntvl;	// 横軸ラベルの間隔
} dataParam;
#endif

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 属性
public:
	CSplitterWndEx	m_wndSplitter;
	UINT			m_nID;
	unsigned __int64	m_nZoomStart;
	int	m_nZoomTblNo;
	unsigned __int64	m_nCursor;

// 操作
public:
	void	GetMinMaxValue(double& min, double& max);
	void	GetMinMaxValueEx(double& min, double& max);


// オーバーライド
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL IsReadOnly() { return TRUE/*FALSE*/; }	// ウィンドウダイアログの保存ボタンを消すため

// 実装
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// ビューにパラメータを追加する
	BOOL AddViewParam(CPropertiesData* pParam);
	void CChildFrame::UpdateDataCollection(bool bClear);

public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	// 生成された、メッセージ割り当て関数
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
