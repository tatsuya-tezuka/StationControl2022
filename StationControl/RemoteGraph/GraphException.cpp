// GraphException.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "GraphException.h"


// CGraphException

// 宣言
CCriticalSection CGraphMutex::mComSection;
CCriticalSection CGraphMutex::mRealTimeDataSection;
CCriticalSection CGraphMutex::mRealTimeSaveSection;
CCriticalSection CGraphMutex::mAnalysisSaveSection;
CCriticalSection CGraphMutex::mAnalysisSection[MAX_VIEW];
CCriticalSection CGraphMutex::mDrawTrendSection;
CCriticalSection CGraphMutex::mDisplayCommentSection;
CCriticalSection CGraphMutex::mLimitStateSection;
CCriticalSection CGraphMutex::mMessageSection;
CCriticalSection CGraphMutex::mMessageSection1;
CCriticalSection CGraphMutex::mMessageSection2;
CCriticalSection CGraphMutex::mMessageSection3;
CCriticalSection CGraphMutex::mMessageSection4;

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
