// ����� ���C�� DLL �t�@�C���ł��B
//#include "stdafx.h"
//
//#ifdef _STATIONCONTROL
//#include "..\StationControl\stdafx.h"
//#include "..\StationControl\StationControl.h"
//#include "..\StationControl\EngValCnv.h"
//#else
//#include "..\OperationTimeLine\stdafx.h"
//#include "..\OperationTimeLine\OperationTimeLine.h"
//#endif
//#include "StationPipe.h"

#include "stdafx.h"
#include "..\StationControl\StationControl.h"
#include "StationPipe.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

	-# �R���X�g���N�^

	@param

	@retval
*/
/*============================================================================*/
CStationPipe::CStationPipe()
{
	m_pParam = NULL;
	m_pParamService = NULL;
	m_strPipeName = _T("");
	m_strFullPipeName = _T("\\\\.\\PIPE\\");

	m_hOutPipe = NULL;
	m_hInPipe  = NULL;
	m_hListner = NULL;
	mPipeCount = 1;
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

	-# �f�X�g���N�^

	@param

	@retval
*/
/*============================================================================*/
CStationPipe::~CStationPipe()
{
	Delete();
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �p�C�v�̍쐬

@param	bServer		true�̏ꍇ�T�[�o�[�Afalse�̏ꍇ�N���C�A���g
@param	fCallBack	�擾�X���b�h�t�@���N�V����

@retval
*/
/*============================================================================*/
bool CStationPipe::Create(bool bServer, void (WINAPI *fCallBack)(LPPIPEBUFFER pbuf), UINT type, CWnd* pCallWnd)
{
	m_bServer = bServer;
	if (fCallBack == NULL)
		return false;

	if (bServer){
		m_hInPipe = CreateNamedPipe(GetRealPipeName(true),
			PIPE_ACCESS_INBOUND,
			PIPE_WAIT,
			1,
			sizeof(PIPEBUFFER),
			sizeof(PIPEBUFFER),
			mPipeTimeout,
			NULL);
		if (m_hInPipe == NULL || m_hInPipe == INVALID_HANDLE_VALUE)
			return false;

		m_hOutPipe = CreateNamedPipe(GetRealPipeName(false),
			PIPE_ACCESS_OUTBOUND,
			PIPE_WAIT,
			1,
			sizeof(PIPEBUFFER),
			sizeof(PIPEBUFFER),
			mPipeTimeout,
			NULL);
		if (m_hOutPipe == NULL || m_hOutPipe == INVALID_HANDLE_VALUE)
			return false;
	}
	else{
		m_hInPipe = CreateFile(GetRealPipeName(false),
			GENERIC_READ,
			0, // No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hInPipe == NULL || m_hInPipe == INVALID_HANDLE_VALUE)
			return false;

		m_hOutPipe = CreateFile(GetRealPipeName(true),
			GENERIC_WRITE,
			0, // No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hOutPipe == NULL || m_hOutPipe == INVALID_HANDLE_VALUE)
			return false;
	}

	// �X���b�h�̍쐬
	m_pParam = new ListnerParam;
	m_pParam->funcCallBack = fCallBack;
	m_pParam->hPipe = m_hInPipe;
	m_pParam->type = type;
	m_pParam->pCallWnd = pCallWnd;
	m_hListner = CreateThread(NULL,
		0,
		&ListnerProc,
		(LPVOID)m_pParam,
		0,
		&m_dwListnerThreadId);
	if (m_hListner == NULL || m_hListner == INVALID_HANDLE_VALUE)
		return false;

	return true;
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �p�C�v�̍쐬

@param	bServer		true�̏ꍇ�T�[�o�[�Afalse�̏ꍇ�N���C�A���g
@param	fCallBack	�擾�X���b�h�t�@���N�V����

@retval
*/
/*============================================================================*/
bool CStationPipe::CreateService(bool bServer, void (WINAPI *fCallBack)(PIPEMONITORACK* pbuf), UINT type, CWnd* pCallWnd)
{
	m_bServer = bServer;
	if (fCallBack == NULL)
		return false;

	if (bServer){
		m_hOutPipe = CreateNamedPipe(GetRealPipeName(true),
			PIPE_ACCESS_OUTBOUND,
			PIPE_WAIT,
			1,
			sizeof(PIPEACK),
			sizeof(PIPEACK),
			mPipeTimeout,
			NULL);
		if (m_hOutPipe == NULL || m_hOutPipe == INVALID_HANDLE_VALUE)
			return false;
		m_hInPipe = CreateNamedPipe(GetRealPipeName(false),
			PIPE_ACCESS_INBOUND,
			PIPE_WAIT,
			1,
			sizeof(PIPEACK),
			sizeof(PIPEACK),
			mPipeTimeout,
			NULL);
		if (m_hInPipe == NULL || m_hInPipe == INVALID_HANDLE_VALUE)
			return false;
	}
	else{
		m_hOutPipe = CreateFile(GetRealPipeName(true),
			GENERIC_WRITE,
			0, // No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hOutPipe == NULL || m_hOutPipe == INVALID_HANDLE_VALUE)
			return false;
		m_hInPipe = CreateFile(GetRealPipeName(false),
			GENERIC_READ,
			0, // No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hInPipe == NULL || m_hInPipe == INVALID_HANDLE_VALUE)
			return false;
	}

	// �X���b�h�̍쐬
	m_pParamService = new ListnerServiceParam;
	m_pParamService->funcCallBack = fCallBack;
	m_pParamService->hPipe = m_hInPipe;
	m_pParamService->type = type;
	m_pParamService->pCallWnd = pCallWnd;
	m_hListner = CreateThread(NULL,
		0,
		&ListnerServiceProc,
		(LPVOID)m_pParamService,
		0,
		&m_dwListnerThreadId);
	if (m_hListner == NULL || m_hListner == INVALID_HANDLE_VALUE)
		return false;

	return true;
}

bool CStationPipe::Delete()
{
	DWORD dwTemp;
	if(GetExitCodeThread(m_hListner, &dwTemp)){
		if(dwTemp == STILL_ACTIVE)
			TerminateThread(m_hListner, 3);
	}
	if(m_hListner != NULL && m_hListner != INVALID_HANDLE_VALUE){
		CloseHandle(m_hListner);
		m_hListner = NULL;
	}

	if(m_hOutPipe != NULL && m_hOutPipe != INVALID_HANDLE_VALUE){
		CloseHandle(m_hOutPipe);
		m_hOutPipe = NULL;
	}
	if(m_hInPipe != NULL && m_hInPipe != INVALID_HANDLE_VALUE){
		CloseHandle(m_hInPipe);
		m_hInPipe = NULL;
	}

	if (m_pParam != NULL){
		delete m_pParam;
		m_pParam = NULL;
	}
	if (m_pParamService != NULL){
		delete m_pParamService;
		m_pParamService = NULL;
	}

	return true;
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

	-# �X���b�h�t�@���N�V����

	@param	lParam		�X���b�h�p�����^

	@retval
*/
/*============================================================================*/
DWORD WINAPI CStationPipe::ListnerProc(LPVOID lParam)
{
	ListnerParam* pParam = (ListnerParam*)lParam;
	if(pParam == NULL)
		return 1;

	bool	bConnect = false;

	DWORD dwRetVal = 0;
	DWORD dwRead;
	BOOL ret;
	for(bool bContinue = true; bContinue && dwRetVal == 0; Sleep(50)){
		PIPEBUFFER buf;
		ret = ReadFile(pParam->hPipe, &buf, sizeof(PIPEBUFFER), &dwRead, NULL);
		if(dwRead == 0){
			if(bConnect == true){
				(pParam->pCallWnd)->PostMessage(ID_PIPE_FINISH, 0, (LPARAM)pParam->type); 
				break;
			}
			continue;
		}
		if(!ret)
			dwRetVal = 2;

		bConnect = true;

		pParam->funcCallBack(&buf);
	}
	return dwRetVal;
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �X���b�h�t�@���N�V����

@param	lParam		�X���b�h�p�����^

@retval
*/
/*============================================================================*/
DWORD WINAPI CStationPipe::ListnerServiceProc(LPVOID lParam)
{
	ListnerServiceParam* pParam = (ListnerServiceParam*)lParam;
	if (pParam == NULL)
		return 1;

	bool	bConnect = false;

	DWORD dwRetVal = 0;
	DWORD dwRead;
	BOOL ret;

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);

	for (bool bContinue = true; bContinue && dwRetVal == 0;/* Sleep(0)*/){
#ifdef _DEBUG
		DWORD	dwStart = timeGetTime();
#endif

		PIPEMONITORACK buf;
		ret = ReadFile(pParam->hPipe, &buf, sizeof(PIPEMONITORACK), &dwRead, NULL);

		if (dwRead == 0){
			if (bConnect == true){
				(pParam->pCallWnd)->PostMessage(ID_PIPE_FINISH, 0, (LPARAM)pParam->type);
				break;
			}
			continue;
		}
		if (!ret)
			dwRetVal = 2;

		bConnect = true;

		pParam->funcCallBack(&buf);

#ifdef _DEBUG
//		TRACE("### Pipe Listner(%d)\n", timeGetTime() - dwStart);
#endif
	}

	return dwRetVal;
}


/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �p�C�v���̂̐ݒ�

@param	lParam		�X���b�h�p�����^

@retval
*/
/*============================================================================*/
void CStationPipe::SetPipeName(CString strName, CString strHost)
{
	m_strPipeName = strName;
	m_strFullPipeName = _T("\\\\");
	m_strFullPipeName += strHost;
	m_strFullPipeName += "\\PIPE\\";
	m_strFullPipeName += m_strPipeName;
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

	-# �p�C�v���̂̎擾

	@param	lParam		�X���b�h�p�����^

	@retval
*/
/*============================================================================*/
CString CStationPipe::GetRealPipeName(bool bServer)
{
	CString strRetVal = m_strFullPipeName;
	strRetVal += (bServer==true)?_T("_IN"):_T("_OUT");
	return strRetVal;
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

	-# �R�}���h���M

	@param	lParam		strMsg	���M������

	@retval
*/
/*============================================================================*/
bool CStationPipe::Send(LPPIPEBUFFER pbuf)
{
	DWORD dw;
	if (m_hOutPipe == NULL)
		return false;

	int len = sizeof(DWORD) + sizeof(DWORD) + sizeof(WPARAM) + sizeof(LPARAM);
	if (pbuf->bufsize == 0)
		len = sizeof(PIPEBUFFER);
	else
		len += sizeof(UINT) + (int)pbuf->bufsize;

	BOOL ret = WriteFile(m_hOutPipe, pbuf, len/*sizeof(PIPEBUFFER)*/, &dw, NULL);

	return true;
}

bool CStationPipe::Send(PIPESTART* pbuf)
{
	DWORD dw;
	if (m_hOutPipe == NULL)
		return false;

	pbuf->header.matching = mPipeCount++;
	BOOL ret = WriteFile(m_hOutPipe, pbuf, sizeof(PIPESTART), &dw, NULL);

	return (ret == TRUE) ? true : false;
}

bool CStationPipe::Send(PIPESTOP* pbuf)
{
	DWORD dw;
	if (m_hOutPipe == NULL)
		return false;

	pbuf->header.matching = mPipeCount++;
	BOOL ret = WriteFile(m_hOutPipe, pbuf, sizeof(PIPESTOP), &dw, NULL);

	return (ret == TRUE) ? true : false;
}

CShareMemory::CShareMemory()
{
	mFileMap = NULL;
	mMutex = NULL;
	mApplication = false;
	mMessagePtr = NULL;
	mExec = NULL;
	mDone = NULL;
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

	-# �f�X�g���N�^

	@param

	@retval
*/
/*============================================================================*/
CShareMemory::~CShareMemory()
{
	Delete();
}

/*============================================================================*/
/*! ���L������

	-# ���L�������̍쐬�i���C���������ōs���j

	@param

	@retval
*/
/*============================================================================*/
void CShareMemory::Create()
{
	Delete();

	// ���L�������̍쐬
	mFileMap = CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(stManagement) + (mMemorySize*mMemoryCount), mMemoryName);
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		// WTViewerMonitor��
		mFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, mMemoryName);
	} else {
		// WTViewer��
		mApplication = true;
	}

	if (mFileMap != NULL)
	{
		mMessagePtr = (char*)MapViewOfFile(mFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		mMutex = CreateMutex(NULL, FALSE, mMutexName);

		mExec = CreateEvent(NULL, FALSE, FALSE, mEventExecName);
		mDone = CreateEvent(NULL, FALSE, FALSE, mEventDoneName);
	}
}

/*============================================================================*/
/*! ���L������

	-# ���L�������̍폜�i���C���������ōs���j

	@param

	@retval
*/
/*============================================================================*/
void CShareMemory::Delete()
{
	mApplication = false;

	if(mExec != NULL){
		CloseHandle(mExec);
		mExec = NULL;
	}
	if(mDone != NULL){
		CloseHandle(mDone);
		mDone = NULL;
	}

	if(mMutex != NULL){
		CloseHandle(mMutex);
		mMutex = NULL;
	}

	if(mMessagePtr != NULL){
		UnmapViewOfFile(mMessagePtr);
		mMessagePtr = NULL;
	}

	if(mFileMap != NULL){
		CloseHandle(mFileMap);
		mFileMap = NULL;
	}
}

/*============================================================================*/
/*! ���L������

	-# ���L�������̖����ɒǉ�����

	@param	data	�f�[�^

	@retval
*/
/*============================================================================*/
void CShareMemory::Push(CStringA data)
{
	char	buf[mMemorySize+1];
	// �r������

	try
	{
		if(WaitForSingleObject(mMutex, 1000) != WAIT_OBJECT_0){
			// �^�C���A�E�g
			return;
		}

		char *ptr = mMessagePtr;
		stManagement* pst = (stManagement*)ptr;
		ptr += sizeof(stManagement);
		// ���[�N�̈�ɓW�J
		if(data.GetLength() > mMemorySize){
			CStringA temp = data.Left(mMemorySize);
			int len = temp.GetLength();
			sprintf_s(buf, mMemorySize + 1, "%s", (LPCSTR)temp);
		} else{
			sprintf_s(buf, mMemorySize + 1, "%s", (LPCSTR)data);
		}
		// �������ɔz�u
		if(pst->count == mMemoryCount){
			// �擪�̃��������폜���ď�ɋl�߂�
			for(UINT i=0; i<mMemoryCount-1; i++){
				memcpy(&ptr[mMemorySize*(i)], &ptr[mMemorySize*(i+1)], mMemorySize);
			}
			sprintf_s(&ptr[mMemorySize*(mMemoryCount-1)], mMemorySize+1, "%s", buf);
			pst->count = mMemoryCount;
		} else{
			sprintf_s(&ptr[mMemorySize*(pst->count)], mMemorySize+1, "%s", buf);
			pst->count++;
		}

		ReleaseMutex(mMutex);
	}
	catch (CFileException* e)
	{
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}
}

/*============================================================================*/
/*! ���L������

	-# ���L�������̐擪�f�[�^���擾���āA�擪���폜����

	@param

	@retval	CString	�f�[�^
*/
/*============================================================================*/
CStringA CShareMemory::Pop()
{
	char	buf[mMemorySize + 1] = { 0 };

	try
	{
		// �r������
		if(WaitForSingleObject(mMutex, 1000) != WAIT_OBJECT_0){
			// �^�C���A�E�g
			return CStringA("");
		}
		char *ptr = mMessagePtr;
		stManagement* pst = (stManagement*)ptr;
		if(pst->count != 0){
			ptr += sizeof(stManagement);
			// ���[�N�̈�ɓW�J
			memcpy(buf, &ptr[0], mMemorySize);
			// �������̍Ĕz�u
			for(UINT i=0; i<pst->count-1; i++){
				memcpy(&ptr[mMemorySize*(i)], &ptr[mMemorySize*(i+1)], mMemorySize);
			}
			memset(&ptr[mMemorySize*(pst->count-1)], 0, mMemorySize);
			pst->count--;
		}
		ReleaseMutex(mMutex);
		return CStringA(buf);
	}
	catch (CFileException* e)
	{
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}
	return CStringA("");
}

/*============================================================================*/
/*! ���L������

	-# ���L���������̃f�[�^���̎擾

	@param

	@retval	UINT	�f�[�^��
*/
/*============================================================================*/
UINT CShareMemory::Count()
{
	try
	{
		// �r������
		if(WaitForSingleObject(mMutex, 1000) != WAIT_OBJECT_0){
			// �^�C���A�E�g
			return 0;
		}
		char *ptr = mMessagePtr;
		stManagement* pst = (stManagement*)ptr;
		UINT count = pst->count;
		ReleaseMutex(mMutex);
		return count;
	}
	catch (CFileException* e)
	{
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}
	return 0;
}

#include <locale.h>
#include <mlang.h>
#pragma comment(lib,"mlang64.lib")
using namespace std;

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �R���X�g���N�^

@param

@retval
*/
/*============================================================================*/
CServerFile::CServerFile()
{
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# �f�X�g���N�^

@param

@retval
*/
/*============================================================================*/
CServerFile::~CServerFile()
{
}

/*
if (_wfopen_s(&fp, _T("n:\\temp\\ASTRO-E2.pln"), _T("r")) != 0)
{
return;
}
char szline[2048];
// �w�b�_�[�ǂݔ�΂�
fgets(szline, static_cast<unsigned>(_countof(szline)), fp);
CString str, cbuf;
while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
CString a = CServerFile::ConvertUnicode(szline);
}
if (fp != NULL)
fclose(fp);
*/
/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# EUC����Unicode�ւ̕ϊ�

@param

@retval
*/
/*============================================================================*/
CString CServerFile::ConvertUnicode(char* euc)
{
	DWORD mode = 0;
	WCHAR utf16[2024];
	int len = sizeof(utf16) / sizeof(WCHAR);
	ConvertINetMultiByteToUnicode(&mode, 51932, euc, 0, utf16, &len);
	utf16[len] = 0;
	CString ret = CString(utf16);
	return ret;
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM�F�p�C�v�N���X

-# Unicode����EUC�ւ̕ϊ�

@param

@retval
*/
/*============================================================================*/
CStringA CServerFile::ConvertEuc(WCHAR* utf)
{
	DWORD mode = 0;
	char euc[2024];
	int len = sizeof(euc) / sizeof(char);
	ConvertINetUnicodeToMultiByte(&mode, 51932, utf, 0, euc, &len);
	euc[len] = 0;
	CStringA ret = CStringA(euc);
	return ret;
}
