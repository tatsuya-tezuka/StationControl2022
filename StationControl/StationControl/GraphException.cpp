// GraphException.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "GraphException.h"


// CGraphException

// �錾
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
