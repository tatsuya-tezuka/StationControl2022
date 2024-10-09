/*============================================================================*/
/*! MessageStationThread.cpp

-#�Ǖʃ��b�Z�[�W�X���b�h

@param
@retval

*/
/*============================================================================*/
// MessageStationThread.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "MessageStationThread.h"
#include "MainFrm.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CMessageStationThread

IMPLEMENT_DYNCREATE(CMessageStationThread, CWinThread)

CMessageStationThread::CMessageStationThread()
: m_bStop(false)
, m_nStation(eStation_Usuda64)
, m_nMutex(eMessageUdsc64)
{
}

CMessageStationThread::~CMessageStationThread()
{
}

BOOL CMessageStationThread::InitInstance()
{
	m_bAutoDelete = FALSE;

	return TRUE;
}

int CMessageStationThread::ExitInstance()
{
	// TODO:    �X���b�h���Ƃ̌㏈���������Ŏ��s���܂��B
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMessageStationThread, CWinThread)
END_MESSAGE_MAP()


// CMessageStationThread ���b�Z�[�W �n���h���[

int CMessageStationThread::Run()
{
	CString strLog;
	strLog.Format(_T("Run Station(%u)"), m_nStation);

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CMessageStationThread"), strLog, _T("Start"), _T(""), nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);

	if (CMainFrame::GetEngval(m_nStation) == NULL){
		// �H�w�l�ϊ��p�̃N���X���쐬
		CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, m_nStation);
		char szFilePath[_MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);

		CMainFrame::SetEngval(m_nStation, new CEngValCnv(szFilePath));
	}

	try{
		// �f�B�X�p�b�`������̃f�[�^���e���|��������擾����
		// �H�w�l�ϊ��A�e�̈�ւ̐ݒ���s��
		while (m_bStop == false){

			while (m_bStop == false)
			{
//++ 20181023 UUN-18-074�m�F�p >>>
#ifndef _DEBUG_STEP_TEST_WAVE
				CGraphMutex::Lock(m_nMutex);
				deque<PIPEMONITORACK>& data = theApp.GetDataCollection().GetMessageTemp(m_nStation);
				if (data.empty() == false){
					PIPEMONITORACK newData = data.front();
					data.pop_front();
					//TRACE("##########[%d] SIZE=%d\n", m_nStation, data.size());
					CGraphMutex::Unlock(m_nMutex);
					MessageProc(&newData);
				}
				else{
					CGraphMutex::Unlock(m_nMutex);
					break;
				}
#else
				if (m_nStation == 0)
				{
					CWnd* pMain = CMainFrame::GetPipeMessageWnd();
					if (pMain == NULL)
						return 0;
					// �q���f�[�^�L���[�ɓo�^
					theApp.GetDataCollection().AddCollection(NULL, NULL, NULL);

					static DWORD mCallbackProcStart = 0;
					DWORD mDrawInterval = theApp.GetDrawUpdateInterval(false);
					DWORD dwElaps = timeGetTime() - mCallbackProcStart;
					if (dwElaps < mDrawInterval){
						continue;
					}
					mCallbackProcStart = timeGetTime();

					// ��ʍX�V�͑Ώۋǂ݂̂Ƃ���
					if (theApp.GetSelectStationMode() == eStation_1){
						if (theApp.GetSelectStation() == m_nStation){
							::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
						}
						else{
						}
					}
					else{
						::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
					}

				}
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074�m�F�p <<<
			}

			Sleep(1);
		}
	}
	catch (CGraphException ex){
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CMessageStationThread"), strLog, _T("Exception"), ex.GetErrorMessage(), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
	}
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CMessageStationThread"), strLog, _T("Stop"), _T(""), nLogEx::info);
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
void CMessageStationThread::MessageProc(PIPEMONITORACK* pbuf)
{
	CWnd* pMain = CMainFrame::GetPipeMessageWnd();
	if (pMain == NULL)
		return;

	int err;
	size_t	size;

	UINT nStation = 0;
	if (pbuf->header.station == 1) nStation = 0;
#ifdef _DEBUG_PIPEUSE64M
	if (pbuf->header.station == 1) nStation = 1;
#endif
	if (pbuf->header.station == 2) nStation = 2;
#ifdef _DEBUG_PIPEUSE34M
	if (pbuf->header.station == 2) nStation = 1;
#endif
	if (pbuf->header.station == 3) nStation = 3;
	if (pbuf->header.station == 4) nStation = 1;
	UINT nSelectStationMode = theApp.GetSelectStationMode();

	// StationControl�A�v���̏ꍇ
	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
		if (theApp.GetPipeIgnore() & 0x01)
			return;

		DWORD	dwStart = timeGetTime();
		// �Ď����f�[�^
		// buf��App��deque�ɓo�^
		// 1�ǃ��[�h�̏ꍇ�͎��ǁA�܂��͂S�ǃ��[�h���̂ݓo�^
		if (nStation == m_nStation){
			if (CMainFrame::GetEngval(m_nStation) != NULL){
#ifdef _DEBUG
				DWORD	dwStart = timeGetTime();
//				TRACE("### Station(%d) MessageProc(%d) \n", m_nStation, timeGetTime() - dwStart);
#endif
				err = CMainFrame::GetEngval(m_nStation)->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));

#ifdef _DEBUG
				//				TRACE("### MessageProc(%d)\n", timeGetTime() - dwStart);
#endif

				if (err == 0){
					vector<CEngValCnv::obs_tbl_res_t>& data = CMainFrame::GetEngval(m_nStation)->GetAllVal();
					size = data.size();

					if (size > 0){
						// �q���f�[�^�L���[�ɓo�^
						theApp.GetDataCollection().AddCollection(m_nStation, data);
						theApp.GetSatelliteData().GetDBAccessCls().SetEqMonData(m_nStation, data);
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
//		theApp.GetDataCollection().CreateDataToHistory(pbuf);
		// �ǎ��ʎq��100�܂���101�̏ꍇ�͂P�ǃ��[�h�͎��ǁA�S�ǃ��[�h�͉P�c�̂�
		if (pbuf->header.station == eTkscLog_Ope || pbuf->header.station == eTkscLog_Ctl)
		{
			if ((nSelectStationMode == eStation_1 && theApp.GetSelectStation() == m_nStation) ||
				(nSelectStationMode == eStation_5 && eStation_Usuda64 == m_nStation))
			{
				// ���𐶐�
				theApp.GetDataCollection().CreateDataToHistory(pbuf);
			}
			else
			{
				// ����j���̂��߉������Ȃ�
				return;
			}
		}
		else
		{
			// ���𐶐�
			theApp.GetDataCollection().CreateDataToHistory(pbuf);
		}
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
		if (theApp.GetSelectStation() == m_nStation){
			::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
		}
		else{
		}
	}
	else{
		::PostMessage(pMain->m_hWnd, eMessage_ThreadNormal, 0, 0);
	}
}
