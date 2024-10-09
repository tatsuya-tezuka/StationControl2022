#pragma once

#include "const.h"

// CGraphException �R�}���h �^�[�Q�b�g



//!< Mutex���
enum eMutex{
	eCom,
	eRealTimeData,	// ���A���^�C�����̃f�[�^����
	eRealTimeSave,	// ���A���^�C�����̃f�[�^�ۑ�
	eAnalysisSave,	// �m�����A���^�C�����̃f�[�^�ۑ�
	eAnalysisData1,	// �m�����A���^�C�����̃r���[�P�f�[�^����
	eAnalysisData2,	// �m�����A���^�C�����̃r���[�Q�f�[�^����
	eAnalysisData3,	// �m�����A���^�C�����̃r���[�R�f�[�^����
	eAnalysisData4,	// �m�����A���^�C�����̃r���[�S�f�[�^����
	eAnalysisData5,	// �m�����A���^�C�����̃r���[�T�f�[�^����
	eAnalysisData6,	// �m�����A���^�C�����̃r���[�U�f�[�^����
	eAnalysisData7,	// �m�����A���^�C�����̃r���[�V�f�[�^����
	eAnalysisData8,	// �m�����A���^�C�����̃r���[�W�f�[�^����
	eDrawTrend,
	eDisplayComment,
	eLimitState,
	eMessage,
	eMessage1,
	eMessage2,
	eMessage3,
	eMessage4,
};

class CGraphException
{
public:
	CGraphException(TCHAR* pchMessage);
	virtual ~CGraphException(){};

	CString GetErrorMessage() { return m_strMessage; };    //!< ��O���b�Z�[�W�擾

private:
	int     m_nError;       //!< ��O���b�Z�[�W�h�c
	CString m_strMessage;   //!< ��O���b�Z�[�W
};

class CGraphMutex
{
public:
	///// CriticalSection
	static BOOL Lock(eMutex type)
	{
		BOOL ret;
		switch (type){
		case	eCom:				ret = mComSection.Lock();		break;
		case	eRealTimeData:		ret = mRealTimeDataSection.Lock();		break;
		case	eRealTimeSave:		ret = mRealTimeSaveSection.Lock();		break;
		case	eAnalysisSave:		ret = mAnalysisSaveSection.Lock();		break;
		case	eDrawTrend:			ret = mDrawTrendSection.Lock();	break;
		case	eDisplayComment:	ret = mDisplayCommentSection.Lock();	break;
		case	eLimitState:		ret = mLimitStateSection.Lock();	break;
		case	eMessage:			ret = mMessageSection.Lock();	break;
		case	eMessage1:			ret = mMessageSection1.Lock();	break;
		case	eMessage2:			ret = mMessageSection2.Lock();	break;
		case	eMessage3:			ret = mMessageSection3.Lock();	break;
		case	eMessage4:			ret = mMessageSection4.Lock();	break;
		default:			return FALSE;
		}
		return ret;
	}
	static BOOL Unlock(eMutex type)
	{
		BOOL ret;
		switch (type){
		case	eCom:				ret = mComSection.Unlock();			break;
		case	eRealTimeData:		ret = mRealTimeDataSection.Unlock();		break;
		case	eRealTimeSave:		ret = mRealTimeSaveSection.Unlock();		break;
		case	eAnalysisSave:		ret = mAnalysisSaveSection.Unlock();		break;
		case	eDrawTrend:			ret = mDrawTrendSection.Unlock();	break;
		case	eDisplayComment:	ret = mDisplayCommentSection.Unlock();	break;
		case	eLimitState:		ret = mLimitStateSection.Unlock();	break;
		case	eMessage:			ret = mMessageSection.Unlock();	break;
		case	eMessage1:			ret = mMessageSection1.Unlock();	break;
		case	eMessage2:			ret = mMessageSection2.Unlock();	break;
		case	eMessage3:			ret = mMessageSection3.Unlock();	break;
		case	eMessage4:			ret = mMessageSection4.Unlock();	break;
		default:			return FALSE;
		}
		return ret;
	}
	static BOOL IsLock(eMutex type)
	{
		BOOL ret;
		switch (type){
		case	eCom:				ret = (mComSection.m_sect.OwningThread) ? TRUE : FALSE;			break;
		case	eRealTimeData:		ret = (mRealTimeDataSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eRealTimeSave:		ret = (mRealTimeSaveSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eAnalysisSave:		ret = (mAnalysisSaveSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eDrawTrend:			ret = (mDrawTrendSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eDisplayComment:	ret = (mDisplayCommentSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eLimitState:		ret = (mLimitStateSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessage:			ret = (mMessageSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessage1:			ret = (mMessageSection1.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessage2:			ret = (mMessageSection2.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessage3:			ret = (mMessageSection3.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessage4:			ret = (mMessageSection4.m_sect.OwningThread) ? TRUE : FALSE;	break;
		default:			return FALSE;
		}
		return ret;
	}

	static BOOL Lock_Analysis(int view)
	{
		BOOL ret;
		switch (view){
		case 1:		ret = mAnalysisSection[eAnalysisData2 - eAnalysisData1].Lock();	break;
		case 2:		ret = mAnalysisSection[eAnalysisData3 - eAnalysisData1].Lock();	break;
		case 3:		ret = mAnalysisSection[eAnalysisData4 - eAnalysisData1].Lock();	break;
		case 4:		ret = mAnalysisSection[eAnalysisData5 - eAnalysisData1].Lock();	break;
		case 5:		ret = mAnalysisSection[eAnalysisData6 - eAnalysisData1].Lock();	break;
		case 6:		ret = mAnalysisSection[eAnalysisData7 - eAnalysisData1].Lock();	break;
		case 7:		ret = mAnalysisSection[eAnalysisData8 - eAnalysisData1].Lock();	break;
		case 0:
		default:	ret = mAnalysisSection[eAnalysisData1 - eAnalysisData1].Lock();	break;
		}
		return ret;
	}
	static BOOL Unlock_Analysis(int view)
	{
		BOOL ret;
		switch (view){
		case 1:		ret = mAnalysisSection[eAnalysisData2 - eAnalysisData1].Unlock();	break;
		case 2:		ret = mAnalysisSection[eAnalysisData3 - eAnalysisData1].Unlock();	break;
		case 3:		ret = mAnalysisSection[eAnalysisData4 - eAnalysisData1].Unlock();	break;
		case 4:		ret = mAnalysisSection[eAnalysisData5 - eAnalysisData1].Unlock();	break;
		case 5:		ret = mAnalysisSection[eAnalysisData6 - eAnalysisData1].Unlock();	break;
		case 6:		ret = mAnalysisSection[eAnalysisData7 - eAnalysisData1].Unlock();	break;
		case 7:		ret = mAnalysisSection[eAnalysisData8 - eAnalysisData1].Unlock();	break;
		case 0:
		default:	ret = mAnalysisSection[eAnalysisData1 - eAnalysisData1].Unlock();	break;
		}
		return ret;
	}
	static BOOL IsLock_Analysis(int view)
	{
		BOOL ret;
		switch (view){
		case 1:		ret = (mAnalysisSection[eAnalysisData2 - eAnalysisData1].m_sect.OwningThread) ? TRUE : FALSE;	break;
		case 2:		ret = (mAnalysisSection[eAnalysisData3 - eAnalysisData1].m_sect.OwningThread) ? TRUE : FALSE;	break;
		case 3:		ret = (mAnalysisSection[eAnalysisData4 - eAnalysisData1].m_sect.OwningThread) ? TRUE : FALSE;	break;
		case 4:		ret = (mAnalysisSection[eAnalysisData5 - eAnalysisData1].m_sect.OwningThread) ? TRUE : FALSE;	break;
		case 5:		ret = (mAnalysisSection[eAnalysisData6 - eAnalysisData1].m_sect.OwningThread) ? TRUE : FALSE;	break;
		case 6:		ret = (mAnalysisSection[eAnalysisData7 - eAnalysisData1].m_sect.OwningThread) ? TRUE : FALSE;	break;
		case 7:		ret = (mAnalysisSection[eAnalysisData8 - eAnalysisData1].m_sect.OwningThread) ? TRUE : FALSE;	break;
		case 0:
		default:	ret = (mAnalysisSection[eAnalysisData1 - eAnalysisData1].m_sect.OwningThread) ? TRUE : FALSE;	break;
		}
		return ret;
	}

private:
	///// �R���X�g���N�^�C�f�X�g���N�^
	CGraphMutex(){ ; }
	~CGraphMutex(){ ; }

	// Critical Section
	static CCriticalSection mComSection;
	static CCriticalSection mRealTimeDataSection;
	static CCriticalSection mRealTimeSaveSection;
	static CCriticalSection mAnalysisSaveSection;
	static CCriticalSection mAnalysisSection[MAX_VIEW];
	static CCriticalSection mDrawTrendSection;
	static CCriticalSection mDisplayCommentSection;
	static CCriticalSection mLimitStateSection;
	static CCriticalSection mMessageSection;
	static CCriticalSection mMessageSection1;
	static CCriticalSection mMessageSection2;
	static CCriticalSection mMessageSection3;
	static CCriticalSection mMessageSection4;
};
