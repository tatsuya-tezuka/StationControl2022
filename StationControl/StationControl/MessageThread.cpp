// MessageThread.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "MessageThread.h"
#include "MainFrm.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CMessageThread

IMPLEMENT_DYNCREATE(CMessageThread, CWinThread)

CMessageThread::CMessageThread()
: m_bStop(false)
{
}

CMessageThread::~CMessageThread()
{
}

BOOL CMessageThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CMessageThread::ExitInstance()
{
	// TODO:    �X���b�h���Ƃ̌㏈���������Ŏ��s���܂��B
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMessageThread, CWinThread)
END_MESSAGE_MAP()


// CMessageThread ���b�Z�[�W �n���h���[


int CMessageThread::Run()
{
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CMessageThread"), _T("Run"), _T("Start"), _T(""), nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	try{
		// �f�B�X�p�b�`������̃f�[�^���e���|��������擾����
		// �H�w�l�ϊ��A�e�̈�ւ̐ݒ���s��
		while (m_bStop==false){

			while (m_bStop == false)
			{
				//for (int i = 0; i < eStation_MAX; i++){
				//	CGraphMutex::Lock(eMessage);
				//	deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp(i);
				//	if (data.empty() == false){
				//		PIPEMONITORACK newData = data.front();
				//		data.pop_front();
				//		CGraphMutex::Unlock(eMessage);
				//		MessageProc(&newData);
				//	}
				//	else{
				//		CGraphMutex::Unlock(eMessage);
				//		break;
				//	}
				//}
			}

			Sleep(1);
		}
	}
	catch (CGraphException ex){
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CMessageThread"), _T("Run"), _T("Exception"), ex.GetErrorMessage(), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
	}
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CMessageThread"), _T("Run"), _T("Stop"), _T(""), nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//
	return 0;
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# AP�w����̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void CMessageThread::MessageProc(PIPEMONITORACK* pbuf)
{
	if (CMainFrame::GetEngval(0) == NULL){
		// �H�w�l�ϊ��p�̃N���X���쐬
		for (int i = 0; i < eStation_MAX; i++)
		{
			CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, i);
			char szFilePath[_MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);

			CMainFrame::SetEngval(i, new CEngValCnv(szFilePath));
		}
	}

	CWnd* pMain = CMainFrame::GetPipeMessageWnd();
	if (pMain == NULL)
		return;

	int err;
	size_t	size;

	BOOL	bAddTrend = FALSE;
	UINT nStation = 0;
	if (pbuf->header.station == 1) nStation = 0;
	if (pbuf->header.station == 2) nStation = 2;
	if (pbuf->header.station == 3) nStation = 3;
	if (pbuf->header.station == 4) nStation = 1;

	// StationControl�A�v���̏ꍇ
	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
		if (theApp.GetPipeIgnore() & 0x01)
			return;

		DWORD	dwStart = timeGetTime();
		UINT nSelectStationMode = theApp.GetSelectStationMode();
		UINT nSelectStation = theApp.GetSelectStation();
		// �Ď����f�[�^
		// buf��App��deque�ɓo�^
		// ���g������ł́A���ǈȊO�̑��M���g�����擾���邽�߁A�S�ǂ̊Ď��f�[�^���擾����
		if (pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX){
			if (CMainFrame::GetEngval(nStation) != NULL){
#ifdef _DEBUG
				DWORD	dwStart = timeGetTime();
#endif

				err = CMainFrame::GetEngval(nStation)->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));

#ifdef _DEBUG
//				TRACE("### MessageProc(%d)\n", timeGetTime() - dwStart);
#endif

				if (err == 0){
					vector<CEngValCnv::obs_tbl_res_t>& data = CMainFrame::GetEngval(nStation)->GetAllVal();
					size = data.size();

					if (size > 0){
						// �q���f�[�^�L���[�ɓo�^
						theApp.GetDataCollection().AddCollection(nStation, data);
						theApp.GetSatelliteData().GetDBAccessCls().SetEqMonData(nStation, data);
						bAddTrend = TRUE;
					}
				}
			}
		}
		DWORD dwElapsed = (timeGetTime() - dwStart);
		//		TRACE("### ePipeCommand_monitorget Elapsed(%d)\n", dwElapsed);
	}
	break;
	case	ePipeCommand_history:	// �����f�[�^�ʒm
		if (theApp.GetPipeIgnore() & 0x02)
			return;
		theApp.GetDataCollection().CreateDataToHistory(pbuf);
		::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
		return;
		break;
	default:
		return;
		break;
	}

	static DWORD mCallbackProcStart = 0;
	DWORD mDrawInterval = theApp.GetDrawUpdateInterval(false);
	DWORD dwElaps = timeGetTime() - mCallbackProcStart;
	if (dwElaps < mDrawInterval){
		return;
	}
	mCallbackProcStart = timeGetTime();

	// ��ʍX�V�͑Ώۋǂ݂̂Ƃ���
	if (theApp.GetSelectStationMode() == eStation_1){
		if (theApp.GetSelectStation() == nStation){
			::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
		}
		else{
		}
	}
	else{
		::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
	}
}
