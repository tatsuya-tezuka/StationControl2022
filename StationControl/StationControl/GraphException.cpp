// GraphException.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "GraphException.h"


// CGraphException

// 宣言
CCriticalSection CGraphMutex::mComSection;
CCriticalSection CGraphMutex::mDataSection;
CCriticalSection CGraphMutex::mFileSection;
CCriticalSection CGraphMutex::mDrawTrendSection;
CCriticalSection CGraphMutex::mTimelineSection;
CCriticalSection CGraphMutex::mAlarmSection;
CCriticalSection CGraphMutex::mRngPropSection;
CCriticalSection CGraphMutex::mRadarSection;
CCriticalSection CGraphMutex::mMessageSection;
CCriticalSection CGraphMutex::mMessageUdsc64Section;
CCriticalSection CGraphMutex::mMessageUdsc54Section;
CCriticalSection CGraphMutex::mMessageUsc34Section;
CCriticalSection CGraphMutex::mMessageUsc20Section;

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
