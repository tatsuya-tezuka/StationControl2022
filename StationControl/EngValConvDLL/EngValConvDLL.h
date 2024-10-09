// EngValConvDLL.h : EngValConvDLL.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CEngValConvDLLApp
// このクラスの実装に関しては EngValConvDLL.cpp を参照してください。
//

class CEngValConvDLLApp : public CWinApp
{
public:
	CEngValConvDLLApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
