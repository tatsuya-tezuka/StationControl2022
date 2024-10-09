// GraphException.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "GraphException.h"


// CGraphException

// 宣言
CCriticalSection CGraphMutex::mComSection;
CCriticalSection CGraphMutex::mDataSection;
CCriticalSection CGraphMutex::mDrawTrendSection;
CCriticalSection CGraphMutex::mPassInfoSection;
CCriticalSection CGraphMutex::mMessageSection;

/*============================================================================*/
/*! CGraphException例外クラス
コンストラクタ

@param[in]  pchMessage  例外メッセージ

@retval なし
*/
/*============================================================================*/
CGraphException::CGraphException(TCHAR* pchMessage)
{
	m_strMessage = pchMessage;
	m_nError = ::GetLastError();
}
