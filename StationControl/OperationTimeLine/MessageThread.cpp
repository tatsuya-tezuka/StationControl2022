// MessageThread.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
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
				CGraphMutex::Lock(eMessage);
				deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(theApp.GetSelectStation());
				if (data.empty() == false){
					PIPEMONITORACK newData = data.front();
					data.pop_front();
					CGraphMutex::Unlock(eMessage);
					MessageProc(&newData);
				}
				else{
					CGraphMutex::Unlock(eMessage);
					break;
				}
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
	if (CMainFrame::GetEngval() == NULL){
		// �H�w�l�ϊ��p�̃N���X���쐬
		CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, theApp.GetSelectStation());
		char szFilePath[_MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);
		CMainFrame::SetEngval(new CEngValCnv(szFilePath));
	}


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
		// �H�w�l�ϊ�����ʍX�V�͑Ώۋǂ݂̂Ƃ���
		if (nStation != theApp.GetSelectStation())
			return;

		DWORD	dwStart = timeGetTime();

		// �Ď����f�[�^
		// buf��App��deque�ɓo�^
		// �ǔԍ����͈͓��̏ꍇ
		if (pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX)
		{
			if (CMainFrame::GetEngval() != NULL)
			{
				err = CMainFrame::GetEngval()->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));

				if (err == 0)
				{

					vector<CEngValCnv::obs_tbl_res_t>& data = CMainFrame::GetEngval()->GetAllVal();
					size = data.size();

					if (size > 0)
					{
						// �Ď��f�[�^�o�^
						theApp.SetEqMonData(nStation, data);
					}
				}
			}
			else{
			}
		}
		DWORD dwElapsed = (timeGetTime() - dwStart);
		TRACE("### ePipeCommand_monitorget Elapsed(%d)\n", dwElapsed);
	}
	break;
	case	ePipeCommand_history:	// �����f�[�^�ʒm
		theApp.CreateDataToHistory(pbuf);
		::PostMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_SERVER, 9, (LPARAM)pbuf);
		break;
	default:
		return;
		break;
	}

	// �����͎擾�����璼���X�V
	// �Ď��͎擾���Ă��X�V���Ȃ��ŁACTimeLineView::OnTimer�ł�1�b�X�V�Ƃ���
}
