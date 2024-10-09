#pragma once

#include "CommBase.h"
#include "const.h"

using namespace std;

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

//	DWORD	GetData(LPBYTE lpData, DWORD dwSize);
	DWORD	GetData(LPBYTE lpData, DWORD dwSize, UINT nStation = eStation_MAX);
	DWORD	GetData2(LPBYTE lpData, DWORD dwSize, BOOL bSig1st, UINT nStation = eStation_MAX);
//++ 20181023 UUN-18-074�m�F�p >>>
#ifdef _DEBUG_STEP_TEST_WAVE
	DWORD	GetData3(LPBYTE lpData, DWORD dwSize);
	FILE *mFp;
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074�m�F�p <<<
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

	int				m_nCntWaitReadForecast[eStation_MAX];	// �\��l�ǂݑ҂���

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


