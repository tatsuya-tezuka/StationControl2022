#pragma once

#include <vector>
#include <map>
using namespace std;

static const CString mShareMemoryString = _T("AlarmShareMemory");
static const CString mShareMemoryMutex = _T("AlarmShareMutex");
static const CString mShareMemorySend = _T("AlarmShareSend");
static const CString mShareMemoryRecv = _T("AlarmShareRecv");

// M³
static const CString mAlarmEquipmentString[] = {
	_T("Ç^pÇ§u"),
	_T("Ýõ§äu"),
	_T("XÑTLMüÍu(2)"),
	_T("XÑCMDoÍu(2)"),
};
enum eAlarmStation{
	eAlarmStation_UDSC64,
	eAlarmStation_USC34,
	eAlarmStation_USC20,
	eAlarmStation_UDSC54,
	eAlarmStation_MAX,
};
static const TCHAR* mAlarmStationString[]{
	_T("Pc64mÇ"),
		_T("àVY34mÇ"),
		_T("àVY20mÇ"),
		_T("Pc54mÇ"),
};

enum eAlarmStatus{
	eAlarmStatus_Warning,
	eAlarmStatus_Fault,
	eAlarmStatus_Local,
	eAlarmStatus_PowerOff,
	eAlarmStatus_Max,
};
static const TCHAR* mAlarmStatusString[] = {
	_T("WARNING"),
	_T("FAULT"),
	_T("LOCAL"),
	_T("POWER OFF"),
};

static const TCHAR* mStationFileString[] = {
	_T("U64"),
	_T("U34"),
	_T("U20"),
	_T("U54"),
};

enum eAlarmImStatus{
	eAlarmImStatus_Info,
	eAlarmImStatus_Warning,
	eAlarmImStatus_Error,
	eAlarmImStatus_Max,
};
static const TCHAR* mAlarmImStatusString[] = {
	_T(""), _T("x"), _T("Ùí"),
};

typedef struct {
	WORD	station;
	BYTE	imstatus;
	BYTE	status;
	SYSTEMTIME utime;
} stAlarmData;

class CShareAlarmIf
{
public:
	CShareAlarmIf();
	~CShareAlarmIf();

	/* ------------------------------------------------------------------------------------ */
	/* è`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mAlarmStationMax = 4;
	static const UINT mAlarmItemMax = 1000;
	static const UINT mAlarmItemTimeMax = 32;
	static const UINT mAlarmItemNameMax = 80;
	static const UINT mAlarmItemValMax = 40;

	typedef struct{
		DWORD	no;				// ÔiP`j
		DWORD	station;		// ÇID
		DWORD	sender;			// M³
		DWORD	status;			// Xe[^X
		char	monname[mAlarmItemNameMax];	// ÄÚ¼
		char	monval[mAlarmItemValMax];		// ÄÚ
	} stAlarmItemLine;
	typedef struct{
		char	updatetime[mAlarmItemTimeMax];	// XVú
		stAlarmItemLine	line;	// Cîñ
	} stAlarmItem;

protected:
	/* ------------------------------------------------------------------------------------ */
	/* oÏ                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	bool	mServer;
	DWORD	m_dwProcessId;
	HANDLE	m_hMap;
	HANDLE	m_hMutex;
	HANDLE	m_hSend;
	HANDLE	m_hRecv;
	char	*m_pAlarmBuf;

	/* ------------------------------------------------------------------------------------ */
	/* oÖ                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	Create(bool bServer, int nStation);
	void	Delete();
	void	Send(stAlarmItem* stAlarmBuf, UINT count);
	bool	Recv(stAlarmItem* stAlarmBuf, UINT count);
	int		ConvertAlarmData(CString strFileName, vector<stAlarmItem>& list);
	bool	IsValidMap() {
		return (m_hMap != NULL) ? true : false;
	}

protected:

};

