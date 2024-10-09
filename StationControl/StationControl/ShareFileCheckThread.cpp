// ShareFileCheckThread.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ShareFileCheckThread.h"

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CShareFileCheckThread

IMPLEMENT_DYNCREATE(CShareFileCheckThread, CWinThread)

CShareFileCheckThread::CShareFileCheckThread()
: m_bStop(false)
{
}

CShareFileCheckThread::~CShareFileCheckThread()
{
}

BOOL CShareFileCheckThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CShareFileCheckThread::ExitInstance()
{
	// TODO:    スレッドごとの後処理をここで実行します。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CShareFileCheckThread, CWinThread)
END_MESSAGE_MAP()


// CShareFileCheckThread メッセージ ハンドラー


int CShareFileCheckThread::Run()
{
	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	while (m_bStop == false){
		// 測定開始からの経過時間（ミリ秒）の計算
		DWORD dwElapsed = (timeGetTime() - dwStart);
		if (dwElapsed >= 1000){
			dwStart = timeGetTime();
			// サーバーの共有フォルダよりデータを取得する
			theApp.GetSatelliteData().GetShareFile();
		}
		else{
			Sleep(1);
			continue;
		}
	}

	return 0;
}
