#pragma once

#include "const.h"

using namespace std;

#define _MAX_NAME	64
#define _MAX_UNIT	8

//=============================================================================
//	�w�b�_�[
//=============================================================================
typedef struct  {
	DWORD		size;				// �w�b�_�[���܂ރf�[�^�T�C�Y
	BYTE		command;			// �R�}���hID
	BYTE		reserved[3];		// �\��
} stCommandHeader;

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
//	BYTE			status[8];			// �X�e�[�^�X
	USHORT			status;				// �X�e�[�^�X
	BYTE			reserved[32];
} stServerItemData;

static const DWORD mMaxData = 80;
static const DWORD mMaxBufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData)*mMaxData);

//!< �X���b�h���
typedef void (CALLBACK *THREADCALLBACK)(UINT message, WPARAM wParam, LPARAM lParam);
typedef struct{
	void*			parent;		// �Ăяo�����n���h��
	THREADCALLBACK	callback;	// CALLBACK�֐��|�C���^
} stThreadParam;

// CDataCollectionThread

class CDataCollectionThread : public CWinThread
{
	DECLARE_DYNCREATE(CDataCollectionThread)

public:
	CDataCollectionThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CDataCollectionThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

#ifdef _NOCOMM
	int				m_angle[80];

	DWORD	GetData(LPBYTE lpData, DWORD dwSize);
#endif

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
	// �T�[�o�[�Ƃ̒ʐM�N���X
	stThreadParam	m_ThreadParam;		// �X���b�h���
	bool			m_bStop;			// �X���b�h�I���t���O

#ifdef HighSpeedDemo
	CTime	mRealTimeStart;
#endif // HighSpeedDemo

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetThreadParam(stThreadParam& param) { m_ThreadParam = param; }
	void	StopCollection()
	{
		m_bStop = true;
	}

protected:
	UINT	_ConvertNameToId(CString name);
	CString	_ConvertIdToName(UINT Id);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};

// CSaveRealTimeDataThread

class CSaveRealTimeDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CSaveRealTimeDataThread)

public:
	CSaveRealTimeDataThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CSaveRealTimeDataThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	stThreadParam	m_ThreadParam;		// �X���b�h���
	bool			m_bStop;			// �X���b�h�I���t���O

public:
	void	SetThreadParam(stThreadParam& param) { m_ThreadParam = param; }
	void	Stop()	{ m_bStop = true; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};

// CSaveAnalysisDataThread

class CSaveAnalysisDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CSaveAnalysisDataThread)

public:
	CSaveAnalysisDataThread();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CSaveAnalysisDataThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	stThreadParam	m_ThreadParam;		// �X���b�h���
	bool			m_bStop;			// �X���b�h�I���t���O
	bool			m_bFinish;			// �X���b�h�����t���O

	int				m_nView;			// �r���[�ԍ�

protected:
	void	CompAnalysisData(int tbl, vector<stSpaceData>& list, stSpaceData& data);

public:
	void	SetThreadParam(stThreadParam& param) { m_ThreadParam = param; }
	void	Stop()	{ m_bStop = true; }
	bool	IsFinish()	{ return m_bFinish; }
	void	ClrFinish()	{ m_bFinish = false; }

	void	SetViewNo(int nView)	{ m_nView = nView; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};

#pragma once
