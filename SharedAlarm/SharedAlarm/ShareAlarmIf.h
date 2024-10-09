#pragma once

#include <vector>
#include <map>
using namespace std;

static const CString mShareMemoryString = _T("AlarmShareMemory");
static const CString mShareMemoryMutex = _T("AlarmShareMutex");
static const CString mShareMemorySend = _T("AlarmShareSend");
static const CString mShareMemoryRecv = _T("AlarmShareRecv");

// ���M��
static const CString mAlarmEquipmentString[] = {
	_T("�ǉ^�p�ǐ����u"),
	_T("�ݔ����䑕�u"),
	_T("X��TLM���͑��u(2)"),
	_T("X��CMD�o�͑��u(2)"),
};
enum eAlarmStation{
	eAlarmStation_UDSC64,
	eAlarmStation_USC34,
	eAlarmStation_USC20,
	eAlarmStation_UDSC54,
	eAlarmStation_MAX,
};
static const TCHAR* mAlarmStationString[]{
	_T("�P�c64m��"),
	_T("���V�Y34m��"),
	_T("���V�Y20m��"),
	_T("�P�c54m��"),
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
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mAlarmStationMax = 4;
	static const UINT mAlarmItemMax = 1000;
	static const UINT mAlarmItemTimeMax = 32;
	static const UINT mAlarmItemNameMax = 80;
	static const UINT mAlarmItemValMax = 40;

	typedef struct{
		DWORD	no;				// �ԍ��i�P�`�j
		DWORD	station;		// ��ID
		DWORD	sender;			// ���M��
		DWORD	status;			// �X�e�[�^�X
		char	monname[mAlarmItemNameMax];	// �Ď����ږ�
		char	monval[mAlarmItemValMax];		// �Ď�����
	} stAlarmItemLine;
	typedef struct{
		char	updatetime[mAlarmItemTimeMax];	// �X�V����
		stAlarmItemLine	line;	// ���C�����
	} stAlarmItem;

protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
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
	/* �����o�֐�                                                                           */
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

