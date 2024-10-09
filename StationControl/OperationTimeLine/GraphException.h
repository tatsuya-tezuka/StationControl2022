#pragma once

// CGraphException コマンド ターゲット



//!< Mutex種別
enum eMutex{
	eCom,
	eData,
	eDrawTrend,
	ePassInfo,
	eMessage,
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
		case	eDrawTrend:	ret = mDrawTrendSection.Lock();	break;
		case	ePassInfo:	ret = mPassInfoSection.Lock();	break;
		case	eMessage:	ret = mMessageSection.Lock();	break;
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
		case	eDrawTrend:	ret = mDrawTrendSection.Unlock();	break;
		case	ePassInfo:	ret = mPassInfoSection.Unlock();	break;
		case	eMessage:	ret = mMessageSection.Unlock();		break;
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
		case	eDrawTrend:	ret = (mDrawTrendSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	ePassInfo:	ret = (mPassInfoSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
		case	eMessage:	ret = (mMessageSection.m_sect.OwningThread) ? TRUE : FALSE;	break;
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
	static CCriticalSection mDrawTrendSection;
	static CCriticalSection mPassInfoSection;
	static CCriticalSection mMessageSection;
};
