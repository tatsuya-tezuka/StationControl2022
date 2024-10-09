#pragma once

// CGraphException コマンド ターゲット



//!< Mutex種別
enum eMutex{
	eCom,
	eData,
	eFile,
	eDrawTrend,
	eTimeline,
	eAlarm,
	eRngProp,
	eRadar,
	eMessage,
	eMessageUdsc64,
	eMessageUdsc54,
	eMessageUsc34,
	eMessageUsc20,
};

class CGraphException
{
public:
	CGraphException(TCHAR* pchMessage);
	virtual ~CGraphException(){};

	CString GetErrorMessage() { return m_strMessage; };    //!< 例外メッセージ取得

private:
	int     m_nError;       //!< 例外メッセージＩＤ
	CString m_strMessage;   //!< 例外メッセージ
};

class CGraphMutex
{
public:
	///// CriticalSection
	static BOOL Lock(eMutex type)
	{
		BOOL ret;
		switch (type){
		case	eCom:		ret = mComSection.Lock();		break;
		case	eData:		ret = mDataSection.Lock();		break;
		case	eFile:		ret = mFileSection.Lock();		break;
		case	eDrawTrend:	ret = mDrawTrendSection.Lock();	break;
		case	eTimeline:	ret = mTimelineSection.Lock();	break;
		case	eAlarm:		ret = mAlarmSection.Lock();		break;
		case	eRngProp:	ret = mRngPropSection.Lock();	break;
		case	eRadar:		ret = mRadarSection.Lock();		break;
		case	eMessage:	ret = mMessageSection.Lock();	break;
		case	eMessageUdsc64:	ret = mMessageUdsc64Section.Lock();	break;
		case	eMessageUdsc54:	ret = mMessageUdsc54Section.Lock();	break;
		case	eMessageUsc34:	ret = mMessageUsc34Section.Lock();	break;
		case	eMessageUsc20:	ret = mMessageUsc20Section.Lock();	break;
		default:			return FALSE;
		}
		return ret;
	}
	static BOOL Unlock(eMutex type)
	{
		BOOL ret;
		switch (type){
		case	eCom:		ret = mComSection.Unlock();			break;
		case	eData:		ret = mDataSection.Unlock();		break;
		case	eFile:		ret = mFileSection.Unlock();		break;
		case	eDrawTrend:	ret = mDrawTrendSection.Unlock();	break;
		case	eTimeline:	ret = mTimelineSection.Unlock();	break;
		case	eAlarm:		ret = mAlarmSection.Unlock();		break;
		case	eRngProp:	ret = mRngPropSection.Unlock();		break;
		case	eRadar:		ret = mRadarSection.Unlock();		break;
		case	eMessage:	ret = mMessageSection.Unlock();		break;
		case	eMessageUdsc64:	ret = mMessageUdsc64Section.Unlock();	break;
		case	eMessageUdsc54:	ret = mMessageUdsc54Section.Unlock();	break;
		case	eMessageUsc34:	ret = mMessageUsc34Section.Unlock();	break;
		case	eMessageUsc20:	ret = mMessageUsc20Section.Unlock();	break;
		default:			return FALSE;
		}
		return ret;
	}
	static BOOL IsLock(eMutex type)
	{
		BOOL ret;
		switch (type){
		case	eCom:		ret = (mComSection.m_sect.OwningThread) ? TRUE : FALSE;			break;
		case	eData:		ret = (mDataSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eFile:		ret = (mFileSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eDrawTrend:	ret = (mDrawTrendSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eTimeline:	ret = (mTimelineSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eAlarm:		ret = (mAlarmSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eRngProp:	ret = (mRngPropSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eRadar:		ret = (mRadarSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eMessage:	ret = (mMessageSection.m_sect.OwningThread) ? TRUE : FALSE;		break;
		case	eMessageUdsc64:	ret = (mMessageUdsc64Section.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessageUdsc54:	ret = (mMessageUdsc54Section.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessageUsc34:	ret = (mMessageUsc34Section.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessageUsc20:	ret = (mMessageUsc20Section.m_sect.OwningThread) ? TRUE : FALSE;	break;
		default:			return FALSE;
		}
		return ret;
	}

private:
	///// コンストラクタ，デストラクタ
	CGraphMutex(){ ; }
	~CGraphMutex(){ ; }

	// Critical Section
	static CCriticalSection mComSection;
	static CCriticalSection mDataSection;
	static CCriticalSection mFileSection;
	static CCriticalSection mDrawTrendSection;
	static CCriticalSection mTimelineSection;
	static CCriticalSection mAlarmSection;
	static CCriticalSection mRngPropSection;
	static CCriticalSection mRadarSection;
	static CCriticalSection mMessageSection;
	static CCriticalSection mMessageUdsc64Section;
	static CCriticalSection mMessageUdsc54Section;
	static CCriticalSection mMessageUsc34Section;
	static CCriticalSection mMessageUsc20Section;
};
