#pragma once

#include <list>
#include <map>
#include <vector>
using namespace std;

static const CString mBaseStationString[] = {
	_T("UDSC64"),
	_T("USC20"),
	_T("USC34"),
	_T("UDSC54"),
};
static const CString mEquipmentString[] = {
	_T("ANT"),
	_T("BITRATE"),
	_T("CMD"),
	_T("CTRL"),
};
static const CString mDataNameString[] = {
	_T("AZ_DRIVE"),
	_T("AZ_OFFSET"),
	_T("DRIVE_MODE"),
	_T("EL_DRIVE"),
	_T("EL_OFFSET"),
};
static const CString mDataUnitString[] = {
	_T("Hz"),
	_T("V"),
	_T("A"),
	_T("W"),
	_T("��"),
};

//=============================================================================
//	�Ď��f�[�^
//=============================================================================
#pragma pack(1)
#define _MAX_NAME	64
#define _MAX_UNIT	8

enum{
	eCommand_ReqSignal = 0x01,
	eCommand_ReqStart = 0x02,
	eCommand_ReqStop = 0x03,
	eCommand_ReqDataList = 0x04,
	eCommand_ReqGetData = 0x05,
	eCommand_SendData = 0x10,
};

enum{
	eMessage_ClientConnection = (WM_USER + 1000),
	eMessage_ClientDisConnection,
	eMessage_ClientCommand,
};

//=============================================================================
//	�w�b�_�[
//=============================================================================
typedef struct  {
	DWORD		size;				// �w�b�_�[���܂ރf�[�^�T�C�Y
	BYTE		command;			// �R�}���hID
	BYTE		reserved[3];		// �\��
} stCommandHeader;

//=============================================================================
//	�Ď��f�[�^�o�^�v���R�}���h�ieCommand_ReqSignal�j
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// �R�}���h�w�b�_�[
	BYTE			name[_MAX_NAME];// �Ď��f�[�^���́i�����L��j
} stReqSignal;
typedef struct  {
	BYTE			name[_MAX_NAME];// �Ď��f�[�^���́i�����L��j
} stReqSignalName;

//=============================================================================
//	�Ď��f�[�^���M�J�n�v���R�}���h�ieCommand_ReqStart�j
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// �R�}���h�w�b�_�[
} stReqStart;

//=============================================================================
//	�Ď��f�[�^���M��~�v���R�}���h�ieCommand_ReqStop�j
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// �R�}���h�w�b�_�[
} stReqStop;

//=============================================================================
//	�Ď��f�[�^���ꗗ�擾�v���R�}���h�ieCommand_ReqDataList�j
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// �R�}���h�w�b�_�[
} stReqDataList;

//=============================================================================
//	�Ď��f�[�^���M�R�}���h�ieCommand_SendData�j
//=============================================================================
typedef struct  {
	WORD			wYear;				// �N
	WORD			wDay;				// �P�N�Ԃ̒ʎZ��
	WORD			wHour;				// ����
	WORD			wMinute;			// ��
	WORD			wSecond;			// �b
	WORD			wMilliseconds;		// �~���b
} stServerTime;
typedef struct  {
	stServerTime	systime;			// ����
	BYTE			name[_MAX_NAME];	// �f�[�^����
	DWORD			kind;				// �f�[�^���
	double			value;				// �f�[�^
	BYTE			status[8];			// �X�e�[�^�X
	BYTE			reserved[32];
} stServerItemData;
typedef struct  {
	stCommandHeader		header;				// �R�}���h�w�b�_�[
	stServerItemData	item;				// �A�C�e��
} stServerData;

//=============================================================================
//	����
//=============================================================================
typedef struct  {
	DWORD		size;				// �w�b�_�[���܂ރf�[�^�T�C�Y
	BYTE		command;			// �R�}���hID
	BYTE		status;				// Ack/Nack
	BYTE		reserved[2];		// �\��
} stAckNack;
typedef struct  {
	BYTE		name[_MAX_NAME];	// �M������
	BYTE		unit[_MAX_UNIT];	// �M���P��
} stDataList;
typedef struct  {
	stAckNack	header;				// �R�}���h�w�b�_�[
	stDataList	data[1];			// �M�����X�g
} stSendDataList;
//=============================================================================
typedef struct  {
	BOOL	start;
	BOOL	requestData;
	HANDLE	thread;
	ULONG	ipClient;
	SOCKET	sockClient;
} stThreadData;
//=============================================================================
#pragma pack()


// CCommBase �R�}���h �^�[�Q�b�g

class CCommBase
{
public:
	CCommBase();
	virtual ~CCommBase();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mCtrlPort = 50001;
	static const UINT mDataPort = 50002;
	static const DWORD mDefaultTimeout = 100L;
	static const DWORD mDefaultCommTimeout = 1000L;
	static const DWORD mMaxHostName = MAX_PATH;
	static const DWORD mMaxData = 80;
	static const DWORD mMaxBufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData)*mMaxData);

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	bool			mThreadStop;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static int		WaitSelect(SOCKET fd, long lMilliSec);
	static bool		SendData(SOCKET fd, char* pcDataBuf, unsigned int uiDataSize);
	static bool		RecvData(SOCKET fd, char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
	bool			GetThreadStop()	{ return mThreadStop; }

	static void		ConvertTime(const stServerTime* unixTime, SYSTEMTIME* winTime);
	static void		ConvertTime(const SYSTEMTIME* winTime, stServerTime* unixTime);

	static bool		GetLocalAddress(char* strAddress, UINT nSize);
	static bool		GetLocalName(char* strName, UINT nSize);
	static USHORT	GetPortNumber(char* strServiceName);
	static ULONG	GetIPAddress(char* strHostName);

protected:

};


typedef struct{
	SOCKET	sockClient;
	char	ipClient[20];
} stClientSockInfo;

typedef std::map<ULONG, stClientSockInfo> CADDRLIST;

// CCommServer �R�}���h �^�[�Q�b�g

class CCommServer : public CCommBase
{
public:
	CCommServer();
	virtual ~CCommServer();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetMessageWnd(CWnd* val) { mMessageWnd = val; }
	map< ULONG, vector<stReqSignalName> >&	GetSignalList() { return mSignalList; }
	vector<stDataList>&	GetDataList() { return mDataList; }
	map<UINT, stThreadData>&	GetThreadData() { return mThreadClientData; }

protected:
	map< ULONG, vector<stReqSignalName> >	mSignalList;
	vector<stDataList>		mDataList;
	CWnd*					mMessageWnd;
	map<UINT, stThreadData>		mThreadClientData;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SOCKET			mSock;
	HANDLE			mThread;
	CADDRLIST		mClientSockAddrList;
	map<ULONG, DWORD>	mThreadList;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	Create(int iPort);
	bool	Release();
	ULONG	WaitAccept(long lTimeOut);
	bool	SendData(char* pcDataBuf, unsigned int uiDataSize);
	bool	RecvData(char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
	void	Wait();
	void	Client(DWORD threadid);
	CADDRLIST& GetClientAddrList() { return mClientSockAddrList; }
	bool	SendGetDataSignal(SOCKET sockClient, LPBYTE lpData, DWORD dwSize);

	/* ------------------------------------------------------------------------------------ */
	/* �R�}���h                                                                             */
	/* ------------------------------------------------------------------------------------ */
	bool	RequestSignal(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestSignalStart(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestSignalStop(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestDataList(SOCKET sockClient, stCommandHeader& header);
	bool	RequestGetDataSignal(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);

	DWORD	GetThreadId(ULONG ipClient)
	{
		map<ULONG, DWORD>::iterator itr = mThreadList.find(ipClient);
		if (itr == mThreadList.end())
			return 0;

		return (*itr).second;
	}
	ULONG	GetThreadClient(DWORD thread)
	{
		map<ULONG, DWORD>::iterator itr;
		for (itr = mThreadList.begin(); itr != mThreadList.end(); itr++){
			if ((*itr).second == thread)
				return (*itr).first;
		}

		return 0;
	}
	void	ResetThreadId(ULONG ipClient)
	{
		map<ULONG, DWORD>::iterator itr = mThreadList.find(ipClient);
		if (itr == mThreadList.end())
			return;

		(*itr).second = 0xffffffff;
	}
	void	Clear()
	{
		map<ULONG, DWORD>::iterator itr;
		bool ret = true;
		while (ret)
		{
			ret = false;
			for (itr = mThreadList.begin(); itr != mThreadList.end(); itr++){
				if ((*itr).second == 0xffffffff){
					mThreadList.erase(itr);
					ret = false;
					break;
				}
			}
		}
	}
protected:
	static UINT WINAPI WaitClientThread(LPVOID pParam);
	static UINT WINAPI ClientCommThread(LPVOID pParam);
	static UINT WINAPI ClientDataThread(LPVOID pParam);
};


// CCommClient �R�}���h �^�[�Q�b�g

class CCommClient : public CCommBase
{
public:
	CCommClient();
	virtual ~CCommClient();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SOCKET			mSock;
	bool			mOpen;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	IsOpen()	{ return mOpen; }

	bool	Open(int iPort, char* pServerIP);
	bool	Close();
	bool	SendData(char* pcDataBuf, unsigned int uiDataSize);
	bool	RecvData(char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
protected:

};



