
// SharedAlarm.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル
#include "CmdInf.h"
#include "ShareAlarmIf.h"


// CSharedAlarmApp:
// このクラスの実装については、SharedAlarm.cpp を参照してください。
//

class CSharedAlarmApp : public CWinApp
{
public:
	CSharedAlarmApp();

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
	CCommandLineInfoEx	m_cmdInfo;
	bool				mDebug;
	CString				mAlarmFile;

	CShareAlarmIf		mShareAlarmIf;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	GetDebugMode() { return mDebug; }
	CString	GetAlarmFile() { return mAlarmFile; }

	bool	WriteShareMemory();

protected:

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSharedAlarmApp theApp;