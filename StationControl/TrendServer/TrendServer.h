
// TrendServer.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル
#include <vector>

using namespace std;


// CTrendServerApp:
// このクラスの実装については、TrendServer.cpp を参照してください。
//

class CTrendServerApp : public CWinApp
{
public:
	CTrendServerApp();

// オーバーライド
public:
	virtual BOOL InitInstance();


// 実装

	DECLARE_MESSAGE_MAP()
};

extern CTrendServerApp theApp;