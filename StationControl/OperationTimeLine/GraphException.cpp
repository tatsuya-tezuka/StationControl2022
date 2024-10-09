// GraphException.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "GraphException.h"


// CGraphException

// �錾
CCriticalSection CGraphMutex::mComSection;
CCriticalSection CGraphMutex::mDataSection;
CCriticalSection CGraphMutex::mDrawTrendSection;
CCriticalSection CGraphMutex::mPassInfoSection;
CCriticalSection CGraphMutex::mMessageSection;

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
