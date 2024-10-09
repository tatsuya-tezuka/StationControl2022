
// TrendServerDlg.h : ヘッダー ファイル
//

#pragma once

#include "CommBase.h"
#include "afxcmn.h"


// CTrendServerDlg ダイアログ
class CTrendServerDlg : public CDialogEx
{
// コンストラクション
public:
	CTrendServerDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_TRENDSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
	static const UINT mMaxConnection = 10;
public:
protected:
	CCommServer	m_DataServer;						// 接続サーバー
	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
protected:
	bool	_StartServer();
	bool	_StopServer();
	int		_AddItem(CListCtrl& list, int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);
	int		_AppendMessage(CString msg, COLORREF color=RGB(0,0,0));

	//-----------------------------------------------------------------------------
	//	ダミーデータ
	//-----------------------------------------------------------------------------
	int				m_angle[80];

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	CListCtrl mListStation;
	CListCtrl mListEquipment;
	CListCtrl mListSignal;
	CRichEditCtrl mMessage;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
