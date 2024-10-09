
// FAPanelBrowser.h : FAPanelBrowser アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル
#include "CmdInf.h"


// CFAPanelBrowserApp:
// このクラスの実装については、FAPanelBrowser.cpp を参照してください。
//

class CFAPanelBrowserApp : public CWinApp
{
public:
	CFAPanelBrowserApp();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CCommandLineInfoEx	m_cmdInfo;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString				mTitle;
	CString				mXmlFile;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString		GetTitle()	{ return mTitle; }
	CString		GetXmFile()	{ return mXmlFile; }
protected:
	/* ------------------------------------------------------------------------------------ */

// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFAPanelBrowserApp theApp;
