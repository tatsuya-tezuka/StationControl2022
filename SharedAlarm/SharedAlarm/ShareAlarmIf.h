#pragma once

#include <vector>
#include <map>
using namespace std;

static const CString mShareMemoryString = _T("AlarmShareMemory");
static const CString mShareMemoryMutex = _T("AlarmShareMutex");
static const CString mShareMemorySend = _T("AlarmShareSend");
static const CString mShareMemoryRecv = _T("AlarmShareRecv");

// 送信元
static const CString mAlarmEquipmentString[] = {
	_T("局運用管制装置"),
	_T("設備制御装置"),
	_T("X帯TLM入力装置(2)"),
	_T("X帯CMD出力装置(2)"),
};
enum eAlarmStation{
	eAlarmStation_UDSC64,
	eAlarmStation_USC34,
	eAlarmStation_USC20,
	eAlarmStation_UDSC54,
	eAlarmStation_MAX,
};
static const TCHAR* mAlarmStationString[]{
	_T("臼田64m局"),
	_T("内之浦34m局"),
	_T("内之浦20m局"),
	_T("臼田54m局"),
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

class CShareAlarmIf
{
public:
	CShareAlarmIf();
	~CShareAlarmIf();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mAlarmStationMax = 4;
	static const UINT mAlarmItemMax = 1000;
	static const UINT mAlarmItemTimeMax = 32;
	static const UINT mAlarmItemNameMax = 80;
	static const UINT mAlarmItemValMax = 40;

	typedef struct{
		DWORD	no;				// 番号（１～）
		DWORD	station;		// 局ID
		DWORD	sender;			// 送信元
		DWORD	status;			// ステータス
		char	monname[mAlarmItemNameMax];	// 監視項目名
		char	monval[mAlarmItemValMax];		// 監視項目
	} stAlarmItemLine;
	typedef struct{
		char	updatetime[mAlarmItemTimeMax];	// 更新日時
		stAlarmItemLine	line;	// ライン情報
	} stAlarmItem;

protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
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
	/* メンバ関数                                                                           */
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

