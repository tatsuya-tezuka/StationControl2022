#pragma once

// CTrendPipe �R�}���h �^�[�Q�b�g

static const DWORD		mPipeBufferSize = 16 * 1024;			//!< �o�b�t�@�T�C�Y
static const DWORD		mPipeTimeout = (10 * 1000);				//!< 10�b
static const CString	mPipeName_STATION = _T("STATIONEX");		//!< ���C����ʑ��p�C�v����(Station��)

static const CString mPipeNameString[] = {
	_T("STATIONEX01"),
	_T("STATIONEX02"),
	_T("STATIONEX03"),
	_T("STATIONEX04"),
};

enum eCOMMAND{
	eSTATIONSTART,					//!< Station�J�n
	eSTATIONEND,					//!< Station�I��

	eHELLO,							//!< ���̑�

	eTERM = 0xFFFF,					//!< �R�}���h�I�[
};

// �R�}���hID
enum{
	ePipeCommand_start = 501,		// �[���J�n�v��
	ePipeCommand_stop = 502,		// �[���I���v��
	ePipeCommand_calib = 105,		// �Z���f�f
	ePipeCommand_monitorget = 201,	// �Ď��f�[�^�v��
	ePipeCommand_history = 301,		// �����f�[�^�ʒm
	ePipeCommand_finish = 999,		// �p�C�v�ʐM�I��
};


#pragma pack(1)
typedef struct
{
	DWORD		command;
	DWORD		commandIndex;
	WPARAM		wparam;
	LPARAM		lparam;
	UINT		bufsize;
	BYTE		buf[mPipeBufferSize + 1];
} PIPEBUFFER, *FAR LPPIPEBUFFER;

// ���ʃw�b�_�[
typedef struct
{
	DWORD		size;			// �f�[�^���̃T�C�Y
	DWORD		command;		// �R�}���hID
	DWORD		station;		// ��ID
	DWORD		matching;		// �v������ID
} PIPEHEADER;

// �[���J�n�v��
typedef struct
{
	PIPEHEADER	header;
	BYTE		val[4];			// IP�A�h���X
} PIPESTART;
typedef struct
{
	PIPEHEADER	header;
	DWORD		code;			// ���v���C�R�[�h
} PIPEACK;

// �[���I���v��
typedef struct
{
	PIPEHEADER	header;
	BYTE		val[4];			// IP�A�h���X
} PIPESTOP;

// �Ď��f�[�^�v��
#define PIPE_MON_MAX	(32*1024)
typedef struct
{
	PIPEHEADER	header;
	BYTE		buf[16 * 1024];
} PIPEMONITORACK;

#pragma pack()

enum ePIPEMSG{
	ID_PIPE_RECEIVE = WM_USER + 9000,
	ID_PIPE_SEND,
	ID_PIPE_CREATE,
	ID_PIPE_FINISH,

	/*
	�����܂�
	*/
};


class CTrendPipe
{
public:
	CTrendPipe();
	virtual ~CTrendPipe();

	//-----------------------------------------------------------------------------
	//	��`
	//-----------------------------------------------------------------------------
public:
	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	bool	Create(bool bServer, void (WINAPI *fCallBack)(PIPEMONITORACK* pbuf), UINT type, DWORD threadNotify);
	bool	Delete();

	static DWORD WINAPI ListnerProc(LPVOID lParam);

	struct ListnerParam{
		HANDLE	hPipe;			// �p�C�v�n���h��
		UINT	type;			// AP���
		DWORD	threadid;		// �ʒm�p�X���b�hID
		HANDLE	thread;			// �X���b�h�n���h��
		void (WINAPI *funcCallBack)(PIPEMONITORACK* pbuf);	// �R�}���h�擾�t�@���N�V����
	};

	void	SetPipeName(CString strName, CString strHost = _T("."));
	CString	GetPipeName()	{ return m_strFullPipeName; }
	CString	GetRealPipeName(bool bServer);

	bool	Send(LPPIPEBUFFER pbuf);
	bool	Send(PIPESTART* pbuf);
	bool	Send(PIPESTOP* pbuf);

protected:
	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	ListnerParam*			m_pParam;
	bool					m_bServer;				//!< true=Server false=Client
	CString					m_strPipeName;			//!< �p�C�v��
	CString					m_strFullPipeName;		//!< ���S�p�C�v��

	HANDLE					m_hInPipe;				//!< ���͗p�p�C�v�n���h��
	HANDLE					m_hOutPipe;				//!< �o�͗p�p�C�v�n���h��
	HANDLE					m_hListner;				//!< �擾�p�n���h��
	DWORD					m_dwListnerThreadId;
	DWORD					mPipeCount;
};


