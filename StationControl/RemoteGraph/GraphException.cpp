// GraphException.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "GraphException.h"


// CGraphException

// �錾
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
/*! CGraphException��O�N���X
�R���X�g���N�^

@param[in]  pchMessage  ��O���b�Z�[�W

@retval �Ȃ�
*/
/*============================================================================*/
CGraphException::CGraphException(TCHAR* pchMessage)
{
	m_strMessage = pchMessage;
	m_nError = ::GetLastError();
}
