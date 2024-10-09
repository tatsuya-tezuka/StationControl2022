
// StationControl.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "StationControl.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "StationControlDoc.h"
#include "StationControlView.h"
#include "ControlHistoryView.h"
#include "EquipmentOperationView.h"
#include "OperationDetailView.h"
#include "OperationHistoryView.h"
#include "StationSituationView.h"
#include "SystemDiagram.h"
#include "TimelineView.h"
#include "TrendGraphView.h"

#include "EquipmentHistoryView.h"
#include "OperationSearchView.h"
#include "ControlSearchView.h"
#include "EquipmentSearchView.h"
#include "EquipmentStatusView.h"
#include "EquipmentStatusView54.h"
#include "EquipmentStatusView34.h"
#include "EquipmentStatusView20.h"
#include "SelectStation.h"
#include "EquipmentStationStatusView.h"
#include "SearchView.h"
#include "SearchEqView.h"

#include "EventHistoryView.h"
#include "EventSearchView.h"
#include "AlarmListView.h"
#include "ControlAcquisition.h"
#include "AlarmListFormView.h"

#include "HookMsgBox.h"

#include "..\Utility\SshShell.h"
#include "WaitDialog.h"
#include "AgcLoopBandSelectDlg.h"

#include <lm.h>
#pragma comment( lib, "Netapi32.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStationControlApp

BEGIN_MESSAGE_MAP(CStationControlApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CStationControlApp::OnAppAbout)
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CStationControlApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CStationControlApp �R���X�g���N�V����

CStationControlApp::CStationControlApp()
{
	m_bHiColorIcons = TRUE;

	// �ċN���}�l�[�W���[���T�|�[�g���܂�
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// �A�v���P�[�V���������ʌ��ꃉ���^�C�� �T�|�[�g (/clr) ���g�p���č쐬����Ă���ꍇ:
	//     1) ���̒ǉ��ݒ�́A�ċN���}�l�[�W���[ �T�|�[�g������ɋ@�\���邽�߂ɕK�v�ł��B
	//     2) �쐬����ɂ́A�v���W�F�N�g�� System.Windows.Forms �ւ̎Q�Ƃ�ǉ�����K�v������܂��B
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ���̃A�v���P�[�V���� ID ���������ӂ� ID ������Œu�����܂��B���������
	// ������̌`���� CompanyName.ProductName.SubProduct.VersionInformation �ł�
	SetAppID(_T("StationControl.AppID.NoVersion"));

	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B

	m_nWindowType = 0;
	m_strDefaultPrinterName.Empty();
	m_TimeLineWnd = NULL;
	m_bTimeLineCall = false;
	m_nHcopyType = ID_HCOPY_REVERSE;
	m_nOpenHistroySearch = 0;
	m_nExecuteMode = eExecuteMode_Monitor;
	
	mpAlarmThread = NULL;
}

// �B��� CStationControlApp �I�u�W�F�N�g�ł��B

CStationControlApp theApp;


// CStationControlApp ������

#ifdef _DEBUG
//#define _DEBUG_NOAUTH 1
#endif
BOOL CStationControlApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit �R���g���[�����g�p����ɂ� AfxInitRichEdit2() ���K�v�ł�	
	// AfxInitRichEdit2();

	//============================================================================
	// �R�}���h���C���̉��
	ParseCommandLine(m_cmdInfo);

	if (m_cmdInfo.mLBCtrl.IsEmpty() == FALSE)
	{
		if (m_cmdInfo.mLBStation == 1)	// �P�c64m
		{
			m_nSelectStation = 0;
		}
		else if (m_cmdInfo.mLBStation == 2)	// ���V�Y34m
		{
			m_nSelectStation = 2;
		}
		else if (m_cmdInfo.mLBStation == 3)	// ���V�Y20m
		{
			m_nSelectStation = 3;
		}
		else if (m_cmdInfo.mLBStation == 4)	// �P�c54m
		{
			m_nSelectStation = 1;
		}
		else
		{
			return TRUE;
		}

		// �A�v���P�[�V�����̏�����
		if (InitializeAppl(true) == false){
			return FALSE;
		}

		// �ݔ�������DB�Ǎ�
		GetSatelliteData().GetDBAccessCls().eqctl_db_read();

		// CURRENT_LBW �擾�pIF�쐬
		CreateShareLbwReadIF();

		// LoopBand�ݒ�_�C�A���O�N��
		ExecLoopBandSetDlg(m_cmdInfo.mLBCtrl);

		m_cmdInfo.mLBCtrl = _T("");
		return TRUE;
	}

	if (InitializeAppl(true) == false){
		return FALSE;
	}

	mControlAccess = GetOperationAccess();

	// �R�}���h���C����͂̌��ʁA�u1�ǃ��[�h����SELECT�v�̏ꍇ�͋ǑI���_�C�A���O��\������
	m_nSelectMode = m_cmdInfo.mStationMode;
	m_nSelectStation = m_cmdInfo.mStation;
	if (m_nSelectMode != eStation_1 && m_nSelectStation == 9)
		m_nSelectStation = eStation_Usuda64;
	if (m_nSelectMode == eStation_1 && m_nSelectStation >= 9){
		if (mControlAccess == 0){
			AfxMessageBox(_T("�S�Ă̋ǂɑ΂��ĉ^�p�A�J�E���g���ݒ肳��Ă��܂���B\n�A�v���P�[�V�������I�����܂��B"));
			return FALSE;
		}
		CSelectStation dlgsel;
		dlgsel.mEnableStation = mControlAccess;
		dlgsel.SetSelectStation(m_nSelectStation);
		if (dlgsel.DoModal() == IDABORT)
		{
			AfxMessageBox(_T("�I���\�ȋǂɑ΂��ĉ^�p�A�J�E���g���ݒ肳��Ă��܂���B\n�A�v���P�[�V�������I�����܂��B"));
			return FALSE;
		}
		// ���\���C���f�b�N�X����V�Y20m, ���V�Y34m, �P�c64m, �P�c54m�̏��ɂ���
		m_nSelectStation = mStationViewOrder[dlgsel.m_nStation];

		bool bAuthDlg = true;
		switch (mServiceMode)
		{
		case eServiceMode_admin:
		{
			// ���䌠���_�C�A���O�͊J�����ɐ��䌠���ƋǃG���A�擾
			m_nExecuteMode = eExecuteMode_Control;
			CFileAccess ac;
			mClientTerminal = ac.GetClientTerminal();
			bAuthDlg = false;
			break;
		}
		case eServiceMode_sttn:
		{
			// ���䌠���_�C�A���O�͊J�����A�ǃG���A�͒n��ǌŒ�
			m_nExecuteMode = eExecuteMode_Control;
			mClientTerminal = eClientSttn;
			bAuthDlg = false;
			break;
		}
		case eServiceMode_ssoc:
		{
			// ���䌠���_�C�A���O�͊J�����A�ǃG���A�͑��͌��ǌŒ�
			m_nExecuteMode = eExecuteMode_Control;
			mClientTerminal = eClientSsoc;
			bAuthDlg = false;
			break;
		}
		case eServiceMode_user:
		default:
			// �ʏ폈��
			break;
		}

		// ���䌠���擾
#ifdef _DEBUG_NOAUTH
		m_nExecuteMode = eExecuteMode_Control;
#else
		if (bAuthDlg)
		{
			CControlAcquisition dlgmode;
			INT_PTR mode = dlgmode.DoModal();
			if (mode == IDCANCEL){
				return FALSE;
			}
			if (mode == IDOK){
				m_nExecuteMode = eExecuteMode_Control;
			}
			if (mode == IDABORT){
				m_nExecuteMode = eExecuteMode_Monitor;
			}

			mClientTerminal = dlgmode.GetClientTerminal();
		}
#endif
	}

	SetLimitedMode();	//���������䃂�[�h�̐ݒ�

#if 1
	// �q���ߑ��O���t�̊Ď����ږ���ݒ�
	GetDataCollection().SetKanshiName();
#endif

	// ���䃂�[�h��FA�ɋ����邽�߁A���䃂�[�h���t�@�C����������
	if (m_nSelectMode == eStation_1){
		if (m_nExecuteMode == eExecuteMode_Control){
			WriteExecuteMode2FAFile(1);
		}
	}
	//============================================================================

	mIgnore = m_cmdInfo.mIgnore;


	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));
	LoadStdProfileSettings(4);  // �W���� INI �t�@�C���̃I�v�V���������[�h���܂� (MRU ���܂�)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���W�X�g�������폜����
	Unregister();

	// ���C�� MDI �t���[�� �E�B���h�E���쐬���܂��B
	CMainFrame* pMainFrame = new CMainFrame;
	if (theApp.GetExecuteMode() == eExecuteMode_Control)
	{
		if(theApp.GetLimitedMode() == eLimitedMode_Enable)
		{
			if (!pMainFrame || !pMainFrame->LoadFrame(IDR_STATION_1_LIMITED))
			{
				delete pMainFrame;
				return FALSE;
			}
		}
		else
		{
#ifdef MON_UT
			if (!pMainFrame || !pMainFrame->LoadFrame(IDR_STATION_1_MON_UT))
#else
			if (!pMainFrame || !pMainFrame->LoadFrame(IDR_STATION_1))
#endif /* MON_UT */
			{
				delete pMainFrame;
				return FALSE;
			}
		}
	}
	else
	{
		if (!pMainFrame || !pMainFrame->LoadFrame(IDR_STATION_1_DISABLE))
		{
			delete pMainFrame;
			return FALSE;
		}
	}
	m_pMainWnd = pMainFrame;

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h ���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// �R�}���h ���C���Ŏw�肳�ꂽ�f�B�X�p�b�` �R�}���h�ł��B�A�v���P�[�V������
	// /RegServer�A/Register�A/Unregserver �܂��� /Unregister �ŋN�����ꂽ�ꍇ�AFalse ��Ԃ��܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// �A�v���P�[�V�����̏�����
	if (InitializeAppl() == false){
		return FALSE;
	}

	int dbret;
	// �Ď�DB�Ǎ�
	dbret = GetSatelliteData().GetDBAccessCls().eqmon_db_read();
	if (dbret < 0){
		AfxMessageBox(_T("�Ď�DB�̎擾�Ɏ��s���܂����B\n�Ď�DB���m�F���Ă��������B"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// �ݔ�������DB�Ǎ�
	dbret = GetSatelliteData().GetDBAccessCls().eqctl_db_read();
	if (dbret < 0){
		AfxMessageBox(_T("�ݔ�������DB�̎擾�Ɏ��s���܂����B\n�ݔ�������DB���m�F���Ă��������B"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// �q���ŗLDB�Ǎ�
	dbret = GetSatelliteData().GetDBAccessCls().eqsat_db_read();
	if (dbret < 0){
		AfxMessageBox(_T("�q���ŗLDB�̎擾�Ɏ��s���܂����B\n�q���ŗLDB���m�F���Ă��������B"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// ���f�B�l�X�`�F�b�N���ԓǍ�
	dbret = GetSatelliteData().GetDBAccessCls().rdychk_read();
	if (dbret < 0){
		//AfxMessageBox(_T("���f�B�l�X�`�F�b�N���ԃt�@�C���̎擾�Ɏ��s���܂����B\n���f�B�l�X�`�F�b�N���ԃt�@�C�����m�F���Ă��������B"), MB_OK | MB_ICONERROR);
		//return FALSE;
	}

	// �X�J�C���C���f�[�^�擾
	CGraphMutex::Lock(eRadar);
	if (m_nSelectMode == eStation_1)
	{
		// 1�ǃ��[�h
		CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(GetSelectStation());
		if (pCtrlMonData != NULL)
		{
			// ���X�V
			pCtrlMonData->SetViewID(GetSelectStation());
			pCtrlMonData->SetStationID(GetSelectStation());
			pCtrlMonData->Gtgetskyl();
		}
	}
	else
	{
		// 4�ǃ��[�h
		for (int i = eStation_Usuda64; i < eStation_MAX; i++)
		{
			CTrackingRadarCtrlMonData* pCtrlMonData = &theApp.GetSatelliteData().GetTrackingRadarCtrlMonData(i);
			if (pCtrlMonData != NULL)
			{
				// ���X�V
				pCtrlMonData->SetViewID(i);
				pCtrlMonData->SetStationID(i);
				pCtrlMonData->Gtgetskyl();
			}
		}
	}
	CGraphMutex::Unlock(eRadar);

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B
	if (CreateDocTemplate(m_nSelectMode) == FALSE)
		return FALSE;

	if (m_nSelectMode == eStation_1){
		// 54m�̏ꍇ�́A�v�掞�ԃt�@�C���ǂݍ���
		if (GetSelectStation() == eStation_Usuda642)
		{
			// �v�掞�ԓǍ�
			dbret = GetSatelliteData().GetDBAccessCls().plntime_read();
			if (dbret < 0){
				//AfxMessageBox(_T("�v�掞�ԃt�@�C���̎擾�Ɏ��s���܂����B\n�v�掞�ԃt�@�C�����m�F���Ă��������B"), MB_OK | MB_ICONERROR);
				//return FALSE;
			}
		}

		// �P�ǉ�ʂɕ\������r���[���J��
		for (int i = eViewType1_Trend; i < eViewType1_EquipmentHist; i++){
			OpenView(i);
			// �g�����h�E�B���h�E��AGC,RNG,EL�ŏ���������
			if (i == eViewType1_Trend)
				m_pMainWnd->PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_AGC_RNG_EL, 0);
		}
		((CMainFrame*)pMainFrame)->SetStationMenuBar(eStation_1);

	}
	if (m_nSelectMode == eStation_5){
		// �T�ǉ�ʂɕ\������r���[���J��
		UINT count = 0;
		for (int i = eViewType5_EqOperation1; i < eViewType5_EquipmentHist; i++){
			if (i >= eViewType5_EqOperation1 && i <= eViewType5_EqOperation5){
				if (count >= m_nStationMax)
					continue;
			}
			OpenView(i);
			// �g�����h�E�B���h�E��AGC,RNG,EL�ŏ���������
			if (i == eViewType1_Trend)
				m_pMainWnd->PostMessage(eMessage_InitTrendWnd, eTrendWndSetType_AGC_RNG_EL, 0);
			count++;
		}
		((CMainFrame*)pMainFrame)->SetStationMenuBar(eStation_5);
	}

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED/*m_nCmdShow*/);
	pMainFrame->UpdateWindow();

	((CMainFrame*)pMainFrame)->OnTileHorz();

	((CMainFrame*)pMainFrame)->createPipe(ePIPETYPE_STATION);

	GetDataCollection().StartCollection();

	// �ݔ��A���[���󋵈ꗗ�p�X���b�h�̍쐬
	CreateAlarmThread();

	// CURRENT_LBW �������ݗpIF�쐬
	CreateShareLbwWriteIF();

	// CheckDialo�N��
	ExecCheckDialog();

	return TRUE;
}

int CStationControlApp::ExitInstance()
{
	RemoveWindowHandle(0);

	// �ݔ��A���[���󋵈ꗗ�p�X���b�h�̏I��
	DeleteAlarmThread();

	GetDataCollection().StopCollection();

	if (m_nSelectMode == eStation_1){
		if (m_nExecuteMode == eExecuteMode_Control){
			// ����擾���Ă���ꍇ�̓t���t���N���A����
			CControlAcquisition ac;
			ac.ResetControlFlag();

			// ���䃂�[�h��FA�ɋ����邽�߁A���䃂�[�h�N���A���t�@�C����������
			WriteExecuteMode2FAFile(0);
		}
	}

	//=====================================================//
	//������������������������ Log ������������������������//
	if (m_nLogLevel < nLogEx::none){
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("ExitInstance"), _T("StationControl"), _T("Stop"), nLogEx::info);
		CLogTraceEx::Close();
		CLogTraceEx::Delete();
	}
	//������������������������ Log ������������������������//
	//=====================================================//

	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �h�b�L���O�E�B���h�E�̍쐬

@param
@retval BOOL

*/
/*============================================================================*/
BOOL CStationControlApp::CreateDocTemplate(UINT nStation)
{
	UINT count = 0;
	if (nStation == eStation_1){
		for (int i = eViewType1_Trend; i < eViewType1_Max; i++){
			CRuntimeClass* pViewClass;
			switch (i){
			case	eViewType1_Trend:			pViewClass = RUNTIME_CLASS(CTrendGraphView); break;
			case	eViewType1_EquipmentStatus:
				if (m_nSelectStation == eStation_Usuda64)
					// 64m
					pViewClass = RUNTIME_CLASS(CEquipmentStatusView);
				else if (m_nSelectStation == eStation_Uchinoura34)
					// 34m
					pViewClass = RUNTIME_CLASS(CEquipmentStatusView34);
				else if (m_nSelectStation == eStation_Uchinoura20)
					// 20m
					pViewClass = RUNTIME_CLASS(CEquipmentStatusView20);
				else
					// 54m
					pViewClass = RUNTIME_CLASS(CEquipmentStatusView54);
				break;
			case	eViewType1_OperationHist:	pViewClass = RUNTIME_CLASS(COperationHistoryView); break;
			case	eViewType1_ControlHist:		pViewClass = RUNTIME_CLASS(CControlHistoryView); break;
			case	eViewType1_EquipmentHist:	pViewClass = RUNTIME_CLASS(CEquipmentHistoryView); break;
			case	eViewType1_EventHist:		pViewClass = RUNTIME_CLASS(CEventHistoryView); break;
			case	eViewType1_OperationSearch:	pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType1_ControlSearch:	pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType1_EquipmentSearch:	pViewClass = RUNTIME_CLASS(CSearchEqView); break;
			case	eViewType1_EventSearch:		pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType1_AlarmList:		pViewClass = RUNTIME_CLASS(CAlarmListFormView); break;
			default:	return TRUE;
			}
			UINT nIDResource = IDR_StationControlTYPE;
			switch (i){
			case	eViewType1_Trend:			nIDResource = IDI_ICON_TREND; break;
			case	eViewType1_EquipmentStatus:	nIDResource = IDI_ICON_STATUS; break;
			case	eViewType1_OperationHist:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_ControlHist:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_EquipmentHist:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_EventHist:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_OperationSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_ControlSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_EquipmentSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_EventSearch:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType1_AlarmList:		nIDResource = IDI_ICON_HISTORY; break;
			default:	return TRUE;
			}

			m_pDocTemplate[i] = new CMultiDocTemplate(nIDResource,
				RUNTIME_CLASS(CStationControlDoc),
				RUNTIME_CLASS(CChildFrame), // custom MDI child frame
				pViewClass);
			if (!m_pDocTemplate[i])
				return FALSE;
			AddDocTemplate(m_pDocTemplate[i]);
			count++;
		}
	}
	if (nStation == eStation_5){
		for (int i = eViewType5_EqOperation1; i < eViewType5_Max; i++){
			if (i >= eViewType5_EqOperation1 && i <= eViewType5_EqOperation5){
				if (count >= m_nStationMax)
					continue;
			}
			CRuntimeClass* pViewClass;
			switch (i){
			case	eViewType5_EqOperation1:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_EqOperation2:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_EqOperation3:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_EqOperation4:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_EqOperation5:	pViewClass = RUNTIME_CLASS(CEquipmentStationStatusView); break;
			case	eViewType5_OperationHist:	pViewClass = RUNTIME_CLASS(COperationHistoryView); break;
			case	eViewType5_ControlHist:		pViewClass = RUNTIME_CLASS(CControlHistoryView); break;
			case	eViewType5_EquipmentHist:	pViewClass = RUNTIME_CLASS(CEquipmentHistoryView); break;
			case	eViewType5_EventHist:		pViewClass = RUNTIME_CLASS(CEventHistoryView); break;
			case	eViewType5_OperationSearch:	pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType5_ControlSearch:	pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType5_EquipmentSearch:	pViewClass = RUNTIME_CLASS(CSearchEqView); break;
			case	eViewType5_EventSearch:		pViewClass = RUNTIME_CLASS(CSearchView); break;
			case	eViewType5_AlarmList:		pViewClass = RUNTIME_CLASS(CAlarmListFormView); break;
			default:	return TRUE;
			}
			UINT nIDResource = IDR_StationControlTYPE;
			switch (i){
			case	eViewType5_EqOperation1:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_EqOperation2:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_EqOperation3:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_EqOperation4:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_EqOperation5:	nIDResource = IDI_ICON_SITUATION; break;
			case	eViewType5_OperationHist:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_ControlHist:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_EquipmentHist:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_EventHist:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_OperationSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_ControlSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_EquipmentSearch:	nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_EventSearch:		nIDResource = IDI_ICON_HISTORY; break;
			case	eViewType5_AlarmList:		nIDResource = IDI_ICON_HISTORY; break;
			default:	return TRUE;
			}

			m_pDocTemplate[i] = new CMultiDocTemplate(nIDResource,
				RUNTIME_CLASS(CStationControlDoc),
				RUNTIME_CLASS(CChildFrame), // custom MDI child frame
				pViewClass);
			if (!m_pDocTemplate[i])
				return FALSE;
			AddDocTemplate(m_pDocTemplate[i]);
			count++;
		}
	}

	// ���ʃc�[���E�B���h�E�̍쐬
	// �^�p�v��^�C�����C���̍쐬
	m_pDocTemplate[eViewTypeTool_TimeLine] = new CMultiDocTemplate(IDI_ICON_TIMELINE,
		RUNTIME_CLASS(CStationControlDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTimeLineView));
	if (!m_pDocTemplate[eViewTypeTool_TimeLine])
		return FALSE;
	AddDocTemplate(m_pDocTemplate[eViewTypeTool_TimeLine]);
	count++;

	return TRUE;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �A�v���P�[�V�����̏��������s��

@param
@retval

*/
/*============================================================================*/
bool CStationControlApp::InitializeAppl(bool bPathOnly/* = false*/)
{
	// �e��t�@�C���E�t�H���_�̍쐬
	TCHAR	szFullPath[_MAX_PATH] = { 0 };
	TCHAR	szDir[_MAX_DIR] = { 0 };
	TCHAR	szDrive[_MAX_DRIVE] = { 0 };
	TCHAR	szFname[_MAX_FNAME] = { 0 };

	DWORD ret = ::GetModuleFileName(NULL, szFullPath, _MAX_PATH);

	if (ret == 0){
		AfxMessageBox(_T("���W���[�����̎擾�Ɏ��s���܂����B\n�ċN�����Ă��������B"), MB_OK | MB_ICONERROR);
		return false;
	}

	_wsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	m_strAppPath.Format(_T("%s%s"), szDrive, szDir);
	m_strAppInifileName.Format(_T("%s%s%s.ini"), szDrive, szDir, szFname);
	m_strAppDataPath.Format(_T("%s%sDATA"), szDrive, szDir);
	m_strAppLogPath.Format(_T("%s%sLOG\\"), szDrive, szDir);
	m_strAppWorkPath.Format(_T("%s%sWORK\\"), szDrive, szDir);
	m_strAppFaPanelPath.Format(_T("C:\\FA-Panel\\PROJECT\\"));
	m_strAppLayoutPath.Format(_T("%s%s.scl"), (LPCTSTR)m_strAppWorkPath, szFname);

	CreateDirectory(m_strAppDataPath, NULL);	//�@�t�H���_�쐬
	CreateDirectory(m_strAppLogPath, NULL);	//�@�t�H���_�쐬
	CreateDirectory(m_strAppWorkPath, NULL);	//�@�t�H���_�쐬

	DeleteFile(m_strAppLayoutPath);				// �e���|�����̃��C�A�E�g�t�@�C�����폜���Ă���

	GetDrawUpdateInterval(true);

	m_nStationMax = GetPrivateProfileInt(SECTION_SYSTEM, KEY_STATIONMAX, eStation_MAX, m_strAppInifileName);
	TCHAR buf[MAX_PATH];
	GetPrivateProfileString(SECTION_SYSTEM, KEY_FILESERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strFileServerName = buf;

	m_strManagementPath.Format(_T("\\\\%s\\share\\%s\\"), (LPCTSTR)theApp.GetFileServerName(), (LPCTSTR)mManagementPath);
	if (m_strFileServerName.IsEmpty() == true){
		m_strManagementPath.Format(_T("C:\\share\\%s\\"), (LPCTSTR)mManagementPath);
	}

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHSERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshServerName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHUSER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshUserName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHKEY_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strSshKeyPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHBATCH_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strBatchPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_EVENTTONE_PATH, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strEventTonePath = buf;
	if (m_strEventTonePath.IsEmpty()){
		m_strEventTonePath.Format(_T("%s%sEventTone\\"), szDrive, szDir);
		CreateDirectory(m_strEventTonePath, NULL);	//�@�t�H���_�쐬
	}
	if (m_strEventTonePath.Right(1) != _T("\\")){
		m_strEventTonePath += _T("\\");
	}
	CString str;
	str.Format(_T("%s"), (LPCTSTR)m_strEventTonePath);
	WritePrivateProfileString(SECTION_SYSTEM, KEY_EVENTTONE_PATH, str, m_strAppInifileName);

	// �l�b�g���[�N�h���C�u�̊m��
	//CreateNetworkDrive();
	if (m_strFileServerName.IsEmpty() == false){
		m_strFileServerName += _T("\\");
	}

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SERVICE_MODE, _T(""), buf, MAX_PATH, m_strAppInifileName);
	CString strServiceIni = buf;
	mServiceMode = eServiceMode_user;	// �T�[�r�X�̃f�t�H���g�̓��[�U���[�h
	for (int i = 0; i < eServiceMode_max; i++)
	{
		if (strServiceIni.CompareNoCase(mServiceModeString[i]) == 0)
		{
			mServiceMode = i;
			break;
		}
	}

	// PRED DIFF�ݒ�
	m_nPredDiffEnable = GetPrivateProfileInt(SECTION_SYSTEM, KEY_PREDDIFF_ENABLE, 1, m_strAppInifileName);
	GetPrivateProfileString(SECTION_SYSTEM, KEY_PREDDIFF_MIN, DEFAULT_PREDDIFFMIN, buf, MAX_PATH, m_strAppInifileName);
	m_dPredDiffMin = _ttof(buf);

	// 54m �������ʔԍ��ݒ�
	GetPrivateProfileString(SECTION_SYSTEM, KEY_HISTID54, DEFAULT_HISTID54, buf, MAX_PATH, m_strAppInifileName);
	DWORD nHistID54 = ::_tcstol(buf, NULL, 16);
	theApp.GetDataCollection().SetHistID54(nHistID54);

	if (bPathOnly == true)
		return true;

	m_nMessageThread = GetPrivateProfileInt(SECTION_SYSTEM, KEY_MESSAGETHREAD, 1, m_strAppInifileName);
	str.Format(_T("%d"), m_nMessageThread);
	WritePrivateProfileString(SECTION_SYSTEM, KEY_MESSAGETHREAD, str, m_strAppInifileName);

	m_nLogLevel = GetPrivateProfileInt(SECTION_SYSTEM, KEY_LOGLEVEL, nLogEx::fatal, m_strAppInifileName);
	CLogTraceEx::Create(m_strAppLogPath, _T("StationControl"), m_nLogLevel, nLogEx::text);

	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("InitializeAppl"), _T("StationControl"), _T("Start"), nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//

#if 0
	// �q���ߑ��O���t�̊Ď����ږ���ݒ�
	GetDataCollection().SetKanshiName();
#endif

	// �T�}���ǔ��O���t�̋ǐݒ�
	GetSatelliteData().SetSummarySpectrumStation();

	// �\��l�ꗗ�t�@�C�����f�[�^�擾(EUC�Ή�)
	CGraphMutex::Lock(eFile);
	GetSatelliteData().SetPredList_Euc();
	CGraphMutex::Unlock(eFile);

	// �v��ꗗ�t�@�C�����f�[�^�擾(EUC�Ή�)
	CGraphMutex::Lock(eFile);
	GetSatelliteData().SetPlanList_Euc();
	CGraphMutex::Unlock(eFile);

	// CID�t�@�C�����f�[�^�擾
	if (GetSatelliteData().ReadCIDFile()==FALSE){
		AfxMessageBox(_T("CID�t�@�C���̎擾�Ɏ��s���܂����B\nCID�t�@�C�����m�F���Ă��������B"), MB_OK | MB_ICONERROR);
		return false;
	}

	// �T�}���O���t�̃v���p�e�B�f�[�^�쐬
	GetSatelliteData().CreateSummaryPropertiesData();

	// RNG�`�����j�^�̏�����
	GetSatelliteData().GetRangePropagation().InitStatus();

	// �Â��Z�b�V�������t�@�C�����폜����
	GetSatelliteData().DeleteSessionStatusFile();

	// RF�g���b�v�p��`�t�@�C�����f�[�^�擾
	GetSatelliteData().ReadRFTripFile();

	return true;
}

// CStationControlApp ���b�Z�[�W �n���h���[


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CStationControlApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CStationControlApp �̃J�X�^�}�C�Y���ꂽ�ǂݍ��݃��\�b�h�ƕۑ����\�b�h

void CStationControlApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CStationControlApp::LoadCustomState()
{
}

void CStationControlApp::SaveCustomState()
{
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[���J��

@param	val			�E�B���h�E���
@retval

*/
/*============================================================================*/
void CStationControlApp::OpenView(UINT val)
{
	// �E�B���h�E���
	SetWindowType(val, true);

	//CStationControlDoc* pdoc = (CStationControlDoc*)m_pDocTemplate[val]->m_pDocClass;

	// �w��̃E�B���h�E���J��
	m_nOpenViewType = val;
	CStationControlDoc* pdoc = (CStationControlDoc*)m_pDocTemplate[val]->OpenDocumentFile(NULL);
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[�����

@param	val			�E�B���h�E���
@retval

*/
/*============================================================================*/
void CStationControlApp::CloseView(UINT val)
{
	// �E�B���h�E���
	SetWindowType(val, false);
	// �w��̃E�B���h�E�����
	if (m_pDocTemplate[val] != NULL){
		POSITION pt = m_pDocTemplate[val]->GetFirstDocPosition();
		while (pt){
			CDocument* pdoc = m_pDocTemplate[val]->GetNextDoc(pt);
			if (pdoc == NULL)
				break;
			POSITION pt2 = pdoc->GetFirstViewPosition();
			while (pt2){
				CView *pview = pdoc->GetNextView(pt2);
				CWnd *p;
				p = pview->GetParent();
				p->SendMessage(WM_CLOSE, 0, 0L);
				break;
			}
		}
	}
}
/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[���擾����

@param	val			�E�B���h�E���
@retval

*/
/*============================================================================*/
CView* CStationControlApp::GetView(UINT val)
{
	if (m_pDocTemplate[val] != NULL){
		POSITION pt = m_pDocTemplate[val]->GetFirstDocPosition();
		while (pt){
			CDocument* pdoc = m_pDocTemplate[val]->GetNextDoc(pt);
			if (pdoc == NULL)
				break;
			POSITION pt2 = pdoc->GetFirstViewPosition();
			while (pt2){
				CView *pview = pdoc->GetNextView(pt2);
				return pview;
			}
		}
	}
	return NULL;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[���擾����

@param		lParam		�e��_�C�A���O�A�E�B���h�E�n���h��
@param		wParam		�e��_�C�A���O(IDD)�A�E�B���h�E�p�����^
@retval

*/
/*============================================================================*/
void CStationControlApp::AddWindowHandle(LPARAM lParam, WPARAM wParam/* = 0*/)
{
	if (wParam != 0){
		map<LPARAM, WPARAM>::iterator itr;
		for (itr = mWindowList.begin(); itr != mWindowList.end(); itr++){
			if (wParam == (WPARAM)((*itr).second)){
				RemoveWindowHandle((LPARAM)(*itr).first);
				break;
			}
		}
	}

	map<LPARAM, WPARAM>::iterator itr;
	itr = mWindowList.find(lParam);
	if (itr == mWindowList.end()){
		mWindowList.insert(pair<LPARAM, WPARAM>(lParam, wParam));
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[���擾����

@param		lParam		�e��_�C�A���O�A�E�B���h�E�n���h��
@param		wParam		�e��_�C�A���O(IDD)�A�E�B���h�E�p�����^
@retval

*/
/*============================================================================*/
CWnd* CStationControlApp::GetWindowHandle(WPARAM wParam/* = 0*/)
{
	if (wParam != 0){
		map<LPARAM, WPARAM>::iterator itr;
		for (itr = mWindowList.begin(); itr != mWindowList.end(); itr++){
			if (wParam == (WPARAM)((*itr).second)){
				CWnd* p = (CWnd*)(*itr).first;
				return p;
			}
		}
	}
	return NULL;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�r���[���擾����

@param		lParam		�e��_�C�A���O�A�E�B���h�E�n���h��(0�̏ꍇ�͑S�č폜)
@retval

*/
/*============================================================================*/
void CStationControlApp::RemoveWindowHandle(LPARAM lParam)
{
	map<LPARAM, WPARAM>::iterator itr;
	if (lParam == 0){
		for (itr = mWindowList.begin(); itr != mWindowList.end(); itr++){
			CWnd* p = (CWnd*)(*itr).first;
			if ((p != NULL) && (p->GetSafeHwnd())){
				p->DestroyWindow();
			}
			delete p;
		}
		mWindowList.clear();
		return;
	}

	itr = mWindowList.find(lParam);
	if (itr != mWindowList.end()){
		CWnd* p = (CWnd*)(*itr).first;
		if (p->GetSafeHwnd()){
			p->DestroyWindow();
		}
		delete p;
		mWindowList.erase(itr);
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �w�肳�ꂽ�E�B���h�E�̎�ʂ��擾����

@param		pWnd	�E�B���h�E�n���h��
@retval

*/
/*============================================================================*/
UINT CStationControlApp::GetStationWindowType(CWnd* pWnd)
{
	// ��v�r���[�̊m�F
	if (m_nSelectMode == eStation_1){
		// �P�ǉ�ʂɕ\������r���[���J��
		for (UINT i = eViewType1_Trend; i < eViewType1_Max; i++){
			CView *p = GetView(i);
			if (p != NULL){
				if (p->GetParent() == pWnd)
					return i;
			}
		}
	}
	if (m_nSelectMode == eStation_5){
		// �T�ǉ�ʂɕ\������r���[���J��
		for (UINT i = eViewType5_EqOperation1; i < eViewType5_Max; i++){
			CView *p = GetView(i);
			if (p != NULL){
				if (p->GetParent() == pWnd)
					return i;
			}
		}
	}

	// �ʃ_�C�A���O���̊m�F
	map<LPARAM, WPARAM>::iterator itr;
	for (itr = mWindowList.begin(); itr != mWindowList.end(); itr++){
		if (pWnd == (CWnd*)((*itr).first)){
			return (UINT)(*itr).second;
		}
	}

	for (UINT i = eViewTypeTool_TimeLine; i < eViewType_Max; i++){
		CView *p = GetView(i);
		if (p != NULL){
			if (p->GetParent() == pWnd)
				return i;
		}
	}

	return 0xFFFFFFFF;
}

// CStationControlApp ���b�Z�[�W �n���h���[

BOOL CStationControlApp::PreTranslateMessage(MSG* pMsg)
{
	CView* p;
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN){
		if (m_nSelectMode == eStation_1){
			for (int i = eViewType1_Trend; i < eViewType1_Max; i++){
				if (i == eViewType1_OperationHist || i == eViewType1_ControlHist || i == eViewType1_EquipmentHist || i == eViewType1_EventHist){
					p = GetView(i);
					if (p != NULL){
						if (i == eViewType1_OperationHist)
							((COperationHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType1_ControlHist)
							((CControlHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType1_EquipmentHist)
							((CEquipmentHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType1_EventHist)
							((CEventHistoryView*)p)->CloseChildWindow(pMsg);
					}
				}
				if (i == eViewType1_OperationSearch || i == eViewType1_ControlSearch || i == eViewType1_EquipmentSearch || i == eViewType1_EventSearch){
					p = GetView(i);
					if (p != NULL){
						if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == p->m_hWnd){
							continue;
						}
					}
					if (p != NULL){
						//if (i == eViewType1_OperationSearch)
						//	((COperationSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType1_ControlSearch)
						//	((CControlSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType1_EquipmentSearch)
						//	((CEquipmentSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType1_EventSearch)
						//	((CEventSearchView*)p)->CloseChildWindow(pMsg);
					}
				}
			}
		}
		if (m_nSelectMode == eStation_5){
			for (int i = eViewType5_EqOperation1; i < eViewType5_Max; i++){
				if (i == eViewType5_OperationHist || i == eViewType5_ControlHist || i == eViewType5_EquipmentHist || i == eViewType5_EventHist){
					p = GetView(i);
					if (p != NULL){
						if (i == eViewType5_OperationHist)
							((COperationHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType5_ControlHist)
							((CControlHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType5_EquipmentHist)
							((CEquipmentHistoryView*)p)->CloseChildWindow(pMsg);
						if (i == eViewType5_EventHist)
							((CEventHistoryView*)p)->CloseChildWindow(pMsg);
					}
				}
				if (i == eViewType5_OperationSearch || i == eViewType5_ControlSearch || i == eViewType5_EquipmentSearch || i == eViewType5_EventSearch){
					p = GetView(i);
					if (p != NULL){
						//if (i == eViewType5_OperationSearch)
						//	((COperationSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType5_ControlSearch)
						//	((CControlSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType5_EquipmentSearch)
						//	((CEquipmentSearchView*)p)->CloseChildWindow(pMsg);
						//if (i == eViewType5_EventSearch)
						//	((CEventSearchView*)p)->CloseChildWindow(pMsg);
					}
				}
			}
		}
	}

	return CWinAppEx::PreTranslateMessage(pMsg);
}

void CStationControlApp::OnFilePrintSetup()
{
	CPrintDialog* printDlg = NULL;
	printDlg = new CPrintDialog(FALSE, PD_DISABLEPRINTTOFILE);
	if (printDlg->DoModal() == IDCANCEL){
		delete printDlg;
		return;
	}
	m_strDefaultPrinterName = printDlg->GetDeviceName();
	delete printDlg;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# ���ݎ����̐ݒ�

@brief ���ݎ����iUTC�j���擾����

@param	time	UTC����
@retval �Ȃ�
*/
/*============================================================================*/
void CStationControlApp::GetCurrentUtcTime(CTime& time)
{
	// ���ݎ����̕`��
	SYSTEMTIME st_UTC;
	GetSystemTime(&st_UTC);
	time = CTime(st_UTC);
}

/*============================================================================*/
/*! �A�v���P�[�V����

-#  ��IP�A�h���X�̎擾

@param

@retval

*/
/*============================================================================*/
DWORD CStationControlApp::GetHostIpInfo()
{
	char 		host_name[256];				// �z�X�g�̖��O
	memset(host_name, 0, sizeof(host_name));
	DWORD	ipAddress = 0;

	// �z�X�g���̎擾
	if (gethostname(host_name, 256) == 0){
		struct addrinfo hints, *res;
		struct in_addr addr;
		int err;

		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_INET;

		if ((err = getaddrinfo(host_name, NULL, &hints, &res)) != 0) {
			printf("error %d\n", err);
			return 1;
		}

		addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
		ipAddress = addr.S_un.S_addr;
	}
	return ipAddress;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-#  ���[���ԍ��̎擾

@param�@�Ȃ�
@retval �[���ԍ�

*/
/*============================================================================*/
UINT CStationControlApp::GetTermNo()
{
	char 		host_name[256];				// �z�X�g�̖��O
	memset(host_name, 0, sizeof(host_name));
	UINT	ret = 0;

	// �z�X�g���̎擾
	if (gethostname(host_name, 256) == 0){
		CString strHostName(host_name);
		CString strRight2 = strHostName.Right(2);
		UINT termNo = (UINT)(_ttol(strRight2));
		if (termNo >= 0 && termNo <= 255)
		{
			if (theApp.GetSelectStation() == eStation_Usuda642)
			{
				if (termNo >= 1 && termNo <= 7 ||
					termNo >= 11 && termNo <= 13)
				// 54m�[���ԍ� ����01�`07:54m�Ǔ��A����11�`13:64m�Ǔ�
				ret = (termNo / 10) * 7 + termNo % 10;
			}
			else
			{
				// 64m, 34m, 20m�[���ԍ�
				ret = termNo;
			}
		}
	}

	return ret;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# �t�@�C���p�X�̎擾

@brief ���L�t�H���_�̃t�@�C���p�X�܂��̓t�H���_�p�X���擾����

@param	fileType	�t�@�C���^�C�v
@param	nStation	�ǔԍ��i�ȗ����́A�P�ǉ�ʂŏȗ����͎��ǂ��A�T�ǉ�ʂł͉P�c64m�ǂ�Ԃ��j	
@retval �t�@�C���p�X�܂��̓t�H���_�p�X
*/
/*============================================================================*/
CString CStationControlApp::GetShareFilePath(UINT fileType, UINT nStation/* = eStation_MAX*/)
{
	// �ǔԍ��̈����ȗ���
	if (nStation == eStation_MAX)
	{
		// �ǃ��[�h�`�F�b�N
		switch (m_nSelectMode)
		{
		case eStation_1:
			// ���ǔԍ����Z�b�g
			nStation = m_nSelectStation;
			break;

		case eStation_5:
			// �P�c64m���Z�b�g
			nStation = eStation_Usuda64;
			break;

		case eStation_10:
			// 10�ǃ��[�h
			break;

		default:
			break;
		}
	}

	if (nStation >= eStation_MAX)
	{
		// �P�c64m���Z�b�g
		nStation = eStation_Usuda64;
	}

	// �ǔԍ��ƃt�@�C���^�C�v����Ǖʃt�@�C���E�t�H���_�p�X���擾
	CString strFilePath;
	//	strFilePath.Format(_T("%s%s"), mNetworkDrive[nStation], mSvFileFolder[fileType]);
	strFilePath.Format(_T("\\\\%sshare\\%s\\%s"), (LPCTSTR)theApp.GetFileServerName(), mStationFolder[nStation], mSvFileFolder[fileType]);

	if (m_strFileServerName.IsEmpty() == true){
		strFilePath.Format(_T("C:\\share\\%s\\%s"), mStationFolder[nStation], mLocalFileFolder[fileType]);
	}

	return strFilePath;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �`�F�b�N�_�C�A���O�{�b�N�X�t�@�C���p�X�̎擾

@brief ���L�t�H���_�̃t�@�C���p�X�܂��̓t�H���_�p�X���擾����

@param	nStation	�ǔԍ��i�ȗ����́A�P�ǉ�ʂŏȗ����͎��ǂ��A�T�ǉ�ʂł͉P�c64m�ǂ�Ԃ��j
@retval �t�@�C���p�X�܂��̓t�H���_�p�X
*/
/*============================================================================*/
CString CStationControlApp::GetCheckDialogPath(UINT nStation)
{
	// �ǔԍ��ƃt�@�C���^�C�v����Ǖʃt�@�C���E�t�H���_�p�X���擾
	CString strFilePath;
#if 1
	strFilePath.Format(_T("\\\\%sshare\\DialogReq\\"), (LPCTSTR)theApp.GetFileServerName());

	if (m_strFileServerName.IsEmpty() == true)
	{
		strFilePath = _T("C:\\share\\DialogReq\\");
	}
#else
	//	strFilePath.Format(_T("%s%s"), mNetworkDrive[nStation], mSvFileFolder[fileType]);
	strFilePath.Format(_T("\\\\%sshare\\DialogReq\\%s\\"), (LPCTSTR)theApp.GetFileServerName(), mStationFolder[nStation]);

	if (m_strFileServerName.IsEmpty() == true){
		strFilePath.Format(_T("C:\\share\\DialogReq\\%s\\"), mStationFolder[nStation]);
	}
#endif

	return strFilePath;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# �`�F�b�N�_�C�A���O�{�b�N�X�N��

@param	�Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CStationControlApp::ExecCheckDialog()
{
	CString bat;
	CString stop;

	// 1�ǃ��[�h���̂ݎ��s
	if (m_nSelectMode != eStation_1 || m_nExecuteMode != eExecuteMode_Control)
	{
		return;
	}

	// �`�F�b�N�_�C�A���O�N���t�@�C�����擾
	CString dir = GetCheckDialogPath(m_nSelectStation);
	CString tmp = CString(mStationSimpleString[m_nSelectStation]).MakeLower();
	bat.Format(_T("C:\\sos\\opr\\prog\\ap\\Dialog\\CheckDialog_%s"), (LPCTSTR)tmp);

	// Stop�t�@�C�������݂���Ȃ�폜����B
	stop = dir + tmp + _T("\\") + _T("Stop");
	if (PathFileExists(stop) == TRUE)
	{
		DeleteFile(stop);
	}

	// ���s
	ShellExecute(NULL, _T("open"), (LPTSTR)bat.GetBuffer(), NULL, NULL, SW_HIDE);
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# �`�F�b�N�_�C�A���O�{�b�N�X�I��

@brief Stop�t�@�C�����쐬����

@param	�Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CStationControlApp::StopCheckDialog()
{
	CString stop;

	// 1�ǃ��[�h���̂ݎ��s
	if (m_nSelectMode != eStation_1 || m_nExecuteMode != eExecuteMode_Control)
	{
		return;
	}

	// �`�F�b�N�_�C�A���O�N���t�@�C�����擾
	CString dir = GetCheckDialogPath(m_nSelectStation);
#if 1
	CString tmp = CString(mStationSimpleString[m_nSelectStation]).MakeLower();
	stop = dir + tmp + _T("\\") + _T("Stop");
#else
	stop = dir + _T("Stop");
#endif

	// ���s
	// STOP�t�@�C���쐬
	FILE *fp = NULL;
	if (_wfopen_s(&fp, stop, _T("wt")) == 0 && fp != NULL)
	{
		fclose(fp);
	}
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# �Z���f�f�pAGC�Z���t�@�C���擾�v��

@param	statin		: �ǖ�
@param	satellite	: �q����
@param	file		: �擾�v���t�@�C����
@param	bWaitComplete�FTRUE:�V�F���I����҂i�f�t�H���g�j / FALSE:�V�F���I����҂��Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CStationControlApp::GetCalib(CString station, CString satellite, CString cust, CString dflt, BOOL bWaitComplete /*= TRUE*/)
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCCALIB_GET_SH;

	stParam.strRemShellParam.Format(_T("%s %s %s %s"), (LPCTSTR)station, (LPCTSTR)satellite.MakeLower(), (LPCTSTR)cust, (LPCTSTR)dflt);

	CString msg;
	msg.Format(_T("GetCalib(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("GetCalib"), msg, _T(""), nLogEx::debug);

//	CDlgWaitCmd wait;
//	wait.Open(_T("�Z���f�f"), _T("AGC�Z���e�[�u���t�@�C���擾���B\n���΂炭���҂����������B"), false, true);
	int err = CSshShell::ExecShell(stParam, bWaitComplete);
//	wait.Close();

	CString strDebug;
	strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("GetCalib"), _T("debug"), strDebug, nLogEx::debug);

	if (err != 0)
	{
		AfxMessageBox(_T("AGC�Z���e�[�u���t�@�C���̓ǂݍ��݂Ɏ��s���܂����B"), MB_OK | MB_ICONWARNING);
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �Z���f�f�pAGC�Z���t�@�C���]���v��

@param	statin		: �ǖ�
@param	satellite	: �q����
@param	file		: �]���v���t�@�C����
@param	bWaitComplete�FTRUE:�V�F���I����҂i�f�t�H���g�j / FALSE:�V�F���I����҂��Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CStationControlApp::RequestCalib(CString station, CString satellite, CString file, CString ctrl, BOOL bWaitComplete /*= TRUE*/)
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCCALIB_REQ_SH;

	stParam.strRemShellParam.Format(_T("%s %s %s %s"), (LPCTSTR)station, (LPCTSTR)satellite.MakeLower(), (LPCTSTR)file, (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("GetCalib(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("RequestCalib"), msg, _T(""), nLogEx::debug);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);

	CString strDebug;
	strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("RequestCalib"), _T("debug"), strDebug, nLogEx::debug);

#if 0
	if (err != 0)
	{
		msg.Format(_T("AGC�Z���e�[�u���t�@�C���̓]���Ɏ��s���܂����B\n%s"), file);
		AfxMessageBox(msg, MB_OK | MB_ICONWARNING);
	}
#endif
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M

@brief	���䑗�M

@param	val1: ����Ώ�
@param	val2~4: �p�����[�^
@param	bWaitComplete�FTRUE:�V�F���I����҂i�f�t�H���g�j / FALSE:�V�F���I����҂��Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CStationControlApp::SendControl(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	if (theApp.GetSelectStationMode() == eStation_5)
		return;

	CString ctrl = val1;
	if (val2 != _T(""))
	{
		ctrl += _T(" ") + val2;
		if (val3 != _T(""))
		{
			ctrl += _T(" ") + val3;
			if (val4 != _T(""))
			{
				ctrl += _T(" ") + val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("SendControl(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControl"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControl"), _T("debug"), strDebug, nLogEx::debug);
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M

@brief	���䑗�M

@param	val1: ����Ώ�
@param	val2~4: �p�����[�^
@param	bWaitComplete�FTRUE:�V�F���I����҂i�f�t�H���g�j / FALSE:�V�F���I����҂��Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CStationControlApp::SendControl2(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	if (theApp.GetSelectStationMode() == eStation_5)
		return;

	CString ctrl = val1;
	if (val2 != _T(""))
	{
		ctrl += _T(" ") + val2;
		if (val3 != _T(""))
		{
			ctrl += _T(" ") + val3;
			if (val4 != _T(""))
			{
				ctrl += _T(" ") + val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_NOUI_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("SendControl(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControl"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControl"), _T("debug"), strDebug, nLogEx::debug);
	}
}

void CStationControlApp::SendControlNoUI(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	if (theApp.GetSelectStationMode() == eStation_5)
		return;

	CString ctrl = val1;
	if (val2 != _T(""))
	{
		ctrl += _T(" ") + val2;
		if (val3 != _T(""))
		{
			ctrl += _T(" ") + val3;
			if (val4 != _T(""))
			{
				ctrl += _T(" ") + val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_NOUI_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
		stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Usuda642)
		stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
		stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
		stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);

	CString msg;
	msg.Format(_T("SendControlNoUI(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControlNoUI"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControlNoUI"), _T("debug"), strDebug, nLogEx::debug);
	}
}

void CStationControlApp::SendControlAgcTblLoad(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete /*= TRUE*/)
{
	if (theApp.GetSelectStationMode() == eStation_5)
		return;

	CString ctrl = val1;
	if (val2 != _T(""))
	{
		if ( ctrl != _T(""))
		{
			ctrl += _T(" ");
		}
		ctrl += val2;

		if (val3 != _T(""))
		{
			if (ctrl != _T(""))
			{
				ctrl += _T(" ");
			}
			ctrl += val3;

			if (val4 != _T(""))
			{
				if (ctrl != _T(""))
				{
					ctrl += _T(" ");
				}
				ctrl += val4;
			}
		}
	}

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = AGCTBL_LOAD_SH;

	if (theApp.GetSelectStation() == eStation_Usuda64)
	{
		if (ctrl != _T(""))
			stParam.strRemShellParam.Format(_T("udsc64 %s"), (LPCTSTR)ctrl);
		else
			stParam.strRemShellParam.Format(_T("udsc64"));
	}
	else if (theApp.GetSelectStation() == eStation_Usuda642)
	{
		if (ctrl != _T(""))
			stParam.strRemShellParam.Format(_T("udsc54 %s"), (LPCTSTR)ctrl);
		else
			stParam.strRemShellParam.Format(_T("udsc54"));
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura34)
	{
		if (ctrl != _T(""))
			stParam.strRemShellParam.Format(_T("usc34 %s"), (LPCTSTR)ctrl);
		else
			stParam.strRemShellParam.Format(_T("usc34"));
	}
	else if (theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		if (ctrl != _T(""))
			stParam.strRemShellParam.Format(_T("usc20 %s"), (LPCTSTR)ctrl);
		else
			stParam.strRemShellParam.Format(_T("usc20"));
	}

	CString msg;
	msg.Format(_T("SendControlAgcTblLoad(%s)!!!"), (LPCTSTR)stParam.strRemShellName);
	CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControlAgcTblLoad"), msg, _T(""), nLogEx::info);

	int err = CSshShell::ExecShell(stParam, bWaitComplete);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CStationControlApp"), _T("SendControlAgcTblLoad"), _T("debug"), strDebug, nLogEx::debug);
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M

@brief	���䑗�M

@param	val1: ����Ώ�
@param	val2~: �p�����[�^
@retval	TRUE: ���䑗�M���s / FALSE: �L�����Z��

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlChkDlg(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("���䖼:%s\n��������s���܂����H"), (LPCTSTR)val1);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("�ݔ��ڍא���"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	SendControl(val1, val2, val3, val4, FALSE);

	return TRUE;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M

@brief	���䑗�M�i�Q�̐���ɓ��l�𑗐M�j

@param	val1: ����Ώ�1
@param	val2: ����Ώ�2
@param	val3~: �p�����[�^
@retval	TRUE: ���䑗�M���s / FALSE: �L�����Z��

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlSameNumChkDlg(CString val1, CString val2, CString val3, CString val4, CString val5)
{
	CString strText;
	strText.Format(_T("���䖼:%s\n���䖼:%s\n��������s���܂����H"), (LPCTSTR)val1, (LPCTSTR)val2);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("�ݔ��ڍא���"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	SendControl(val1, val3, val4, val5, TRUE);
	SendControl(val2, val3, val4, val5, FALSE);

	return TRUE;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M

@brief	���䑗�M

@param	val1: ����Ώ�
@param	val2~: �p�����[�^
@retval	TRUE: ���䑗�M���s / FALSE: �L�����Z��

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlChkDlg2(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("���䖼:%s, %s\n��������s���܂����H"), (LPCTSTR)val1, (LPCTSTR)val3);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("�ݔ��ڍא���"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	SendControl2(val1, val2, val3, val4, FALSE);

	return TRUE;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M

@brief	���䑗�M

@param	val1: ����Ώ�
@param	val2~: �p�����[�^
@retval	TRUE: ���䑗�M���s / FALSE: �L�����Z��

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlNoUIChkDlg(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("���䖼:%s\n��������s���܂����H"), (LPCTSTR)val1);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("�ݔ��ڍא���"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	SendControlNoUI(val1, val2, val3, val4, FALSE);

	return TRUE;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M(CURRENT_LBW)

@brief	���䑗�M

@param	val1: ����Ώ�
@param	val2~: �p�����[�^
@retval	TRUE: ���䑗�M���s / FALSE: �L�����Z��

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlDlgCURRENT_LBW(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("���䖼:%s\n��������s���܂����H"), (LPCTSTR)val1);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("�ݔ��ڍא���"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(EISEI_NAME));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(VEQU_EISEI_NAME));
	CString strSatellite;
	if (obs_adr == NULL)
	{
		strSatellite = _T("");
	}
	else
	{
		strSatellite = CString(obs_adr->sz_sts_name);
	}

//#ifdef _DEBUG
//	strSatellite = _T("GETAIL");
//	strSatellite = _T("       ");
//#endif
	CString strSatelliteTmp = strSatellite;
	strSatelliteTmp.Trim(_T(" "));
	if (strSatelliteTmp == _T(""))
	{
		AfxMessageBox(_T("�J�����g�̉q�����Ȃ����߁AAGC�Z���e�[�u���ǂݍ��݂����s�ł��܂���B"));
		return FALSE;
	}

	if (GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		// 20m
		// �e�[�u���̃������W�J����
		SendControlAgcTblLoad(strSatellite, _T(""), val3, val4, TRUE);
	}
	else
	{
		// 20m�ȊO
		// �e�[�u���t�@�C�����R�s�[����V�F���𑗂�
		SendControlAgcTblLoad(strSatellite, val1, val3, val4, TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# ���䑗�M(PLL BW)

@brief	���䑗�M

@param	val1: ����Ώ�
@param	val2~: �p�����[�^
@retval	TRUE: ���䑗�M���s / FALSE: �L�����Z��

*/
/*============================================================================*/
BOOL CStationControlApp::SendControlDlgPLL_BW(CString val1, CString val2, CString val3, CString val4)
{
	CString strText;
	strText.Format(_T("���䖼:%s\n��������s���܂����H"), (LPCTSTR)val1);

	if (IDCANCEL == MessageBoxHooked(m_pMainWnd->m_hWnd, strText, _T("�ݔ��ڍא���"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return FALSE;

	// ����𑗂�
	SendControl(val1, val2, val3, val4, TRUE);

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
//	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(EISEI_NAME));
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(theApp.GetSelectStation(), CString(VEQU_EISEI_NAME));
	CString strSatellite;
	if (obs_adr == NULL)
	{
		strSatellite = _T("");
	}
	else
	{
		strSatellite = CString(obs_adr->sz_sts_name);
	}

//#ifdef _DEBUG
//	strSatellite = _T("GETAIL");
//	strSatellite = _T(" ");
//#endif
	CString strSatelliteTmp = strSatellite;
	strSatelliteTmp.Trim(_T(" "));
	if (strSatelliteTmp == _T(""))
	{
		AfxMessageBox(_T("�J�����g�̉q�����Ȃ����߁AAGC�Z���e�[�u���ǂݍ��݂����s�ł��܂���B"));
		return FALSE;
	}

	if (GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() == eStation_Uchinoura20)
	{
		// 20m
		// �e�[�u���̃������W�J����
		SendControlAgcTblLoad(strSatellite, _T(""), val3, val4, TRUE);
	}
	else
	{
		// 20m�ȊO
		// �e�[�u���t�@�C�����R�s�[����V�F���𑗂�
		SendControlAgcTblLoad(strSatellite, val1, val3, val4, TRUE);
	}

	return TRUE;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# �^�p�A�J�E���g�ݒ�t�@�C����ǂݍ��݁A�N�����[�U�[�̋ǃA�N�Z�X�󋵂��擾����

@param
@retval	�ǃA�N�Z�X��

*/
/*============================================================================*/
BYTE CStationControlApp::GetOperationAccess()
{
	TCHAR buf[100];
	DWORD dw = 100;
	GetUserName(buf, &dw);
	CString strLoginUser = buf;

	vector<stAccount> val;
	CFile file;
	CString name;
	val.clear();

	CFileAccess ac;
	ac.ReadOperationAccount(val);

	CString strSid;
	getUserSid(strLoginUser, strSid);

	BYTE ret = 0;
	vector<stAccount>::iterator itr;
	for (itr = val.begin(); itr != val.end(); itr++){
		if ((*itr).enable == false){
			// �폜�ςȂ̂ŉ������Ȃ�
			continue;
		}

		if (strLoginUser.CompareNoCase((*itr).user) == 0 && strSid == (*itr).usersid){
			// ���O�C�����[�U�̐ݒ肪��������
			ret |= ((*itr).udsc64 == true) ? 0x01 : 0;
			ret |= ((*itr).xdsc54 == true) ? 0x02 : 0;
			ret |= ((*itr).usc34 == true) ? 0x04 : 0;
			ret |= ((*itr).usc20 == true) ? 0x08 : 0;
			break;
		}
	}

	return ret;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# �p�X���[�h�̃G���R�[�h,�f�R�[�h

@param
@retval

*/
/*============================================================================*/
CString CStationControlApp::ManEncodeDecode(CString str)
{
	// 8�����Œ�
	char tobuf[mMaxPassword + 1];
	char frombuf[mMaxPassword + 1];
	sprintf_s((char*)tobuf, mMaxPassword + 1, "%s", (LPCSTR)CStringA(str));
	memset(frombuf, 0, mMaxPassword + 1);

	short* toptr = (short*)tobuf;
	short* fromptr = (short*)frombuf;

	for (int i = 0; i < mMaxPassword / 2; i++){
		*fromptr = htons(*toptr);
		toptr++;
		fromptr++;
	}
	CString ret = CString(frombuf);
	return ret;
}

bool CStationControlApp::getUserSid(CString& strUserName, CString& strSid)
{
	// ���[�J���A�J�E���g��SID���擾����
	DWORD			cbDomain = 0;
	DWORD			cbUserSID = 0;
	SID_NAME_USE	snuType;
	TCHAR			*pszDomain = NULL;
	LPVOID			pUserSID = NULL;
	BOOL ret;

	strSid.Empty();

	// �ŏ��̎��s�Ŋe��T�C�Y���擾����
	ret = ::LookupAccountName(NULL, (TCHAR*)(const TCHAR*)strUserName, pUserSID, &cbUserSID, pszDomain, &cbDomain, &snuType);
	if (ret == FALSE){
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER){
			return false;
		}
	}
	pUserSID = (PSID)malloc(cbUserSID);
	if (pUserSID == NULL){
		// �������m�ێ��s
		return false;
	}

	pszDomain = (TCHAR*)::malloc(cbDomain*sizeof(TCHAR));
	if (NULL == pszDomain){
		// �������m�ێ��s
		free(pUserSID);
		return false;
	}
	memset(pszDomain, 0, cbDomain * sizeof(TCHAR));

	// SID�擾
	ret = ::LookupAccountName(NULL, (TCHAR*)(const TCHAR*)strUserName, pUserSID, &cbUserSID, pszDomain, &cbDomain, &snuType);
	if (ret == FALSE){
		free(pUserSID);
		free(pszDomain);
		return false;
	}

	// �����܂ł����SID�͐���Ɏ擾�ł���
	PSID_IDENTIFIER_AUTHORITY psia;
	DWORD dwSidSize, dwSubAuthorities;

	if (!IsValidSid(pUserSID)){
		free(pUserSID);
		free(pszDomain);
		return false;
	}

	// �R���s���[�^���̈�̃N���A
	free(pszDomain);

	psia = GetSidIdentifierAuthority(pUserSID);
	dwSubAuthorities = *GetSidSubAuthorityCount(pUserSID);
	// S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
	dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);

	//LPTSTR pSID = NULL;
	//DWORD size = dwSidSize;
	//pSID = (LPTSTR)malloc(size);
	//if (pSID){
	//	memset(pSID, 0, size);
	//}
	//else{
	//	free(pUserSID);
	//	free(pszDomain);
	//	return false;
	//}

	CString temp;
	strSid.Empty();
	temp.Format(_T("S-%lu-"), SID_REVISION);
	strSid += temp;
	//dwSidSize = swprintf_s(pSID, dwSidSize * 2, _T("S-%lu-"), SID_REVISION);


	if ((psia->Value[0] != 0) || (psia->Value[1] != 0))
	{
		temp.Format(_T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
			(USHORT)psia->Value[0],
			(USHORT)psia->Value[1],
			(USHORT)psia->Value[2],
			(USHORT)psia->Value[3],
			(USHORT)psia->Value[4],
			(USHORT)psia->Value[5]);
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + lstrlen(pSID),
		//	size,
		//	_T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
		//	(USHORT)psia->Value[0],
		//	(USHORT)psia->Value[1],
		//	(USHORT)psia->Value[2],
		//	(USHORT)psia->Value[3],
		//	(USHORT)psia->Value[4],
		//	(USHORT)psia->Value[5]);
	}
	else
	{
		temp.Format(_T("%lu"),
			(ULONG)(psia->Value[5]) +
			(ULONG)(psia->Value[4] << 8) +
			(ULONG)(psia->Value[3] << 16) +
			(ULONG)(psia->Value[2] << 24));
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + lstrlen(pSID),
		//	size,
		//	_T("%lu"),
		//	(ULONG)(psia->Value[5]) +
		//	(ULONG)(psia->Value[4] << 8) +
		//	(ULONG)(psia->Value[3] << 16) +
		//	(ULONG)(psia->Value[2] << 24));
	}

	// Add SID subauthorities to the string
	for (DWORD dwCounter = 0; dwCounter < dwSubAuthorities; dwCounter++)
	{
		temp.Format(_T("-%lu"), *GetSidSubAuthority(pUserSID, dwCounter));
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + dwSidSize,
		//	size,
		//	_T("-%lu"),
		//	*GetSidSubAuthority(pUserSID, dwCounter));
	}

	free(pUserSID);

	return true;
}

void CStationControlApp::SetMonitorMode(CWnd* pMain)
{
	if (GetExecuteMode() == eExecuteMode_Control)
		return;

	TCHAR name[100];
	CWnd *pwnd = pMain->GetWindow(GW_CHILD);
	while (pwnd != NULL){
		GetClassName(pwnd->m_hWnd, name, 100);
		CString str = name;
		if (str.CompareNoCase(_T("Button")) == 0){
			pwnd->EnableWindow(FALSE);
		}
		pwnd = pwnd->GetWindow(GW_HWNDNEXT);
	}
}

/*============================================================================*/
// ���������䃂�[�h�̐ݒ�
/*============================================================================*/
void CStationControlApp::SetLimitedMode()
{
	char host_name[256]; //�z�X�g��

	m_nLimitedMode = eLimitedMode_Disable;

	if(theApp.GetSelectStation() != eStation_Usuda642)
	{
		return;		//�A�v��54m�p�ȊO
	}

	if(theApp.GetExecuteMode() != eExecuteMode_Control)
	{
		return;		//���䃂�[�h�ȊO
	}

	if (mServiceMode == eServiceMode_limited)
	{
		// INI�t�@�C���ݒ�ɂ��f�o�b�O�p�̐������[�h�ݒ�
		m_nLimitedMode = eLimitedMode_Enable;
		return;
	}
	else if (mServiceMode == eServiceMode_unlimited)
	{
		// INI�t�@�C���ݒ�ɂ�鐧�����[�h�����ݒ�̂��߁A�������Ȃ�
		return;
	}

	if(gethostname(host_name, sizeof(host_name)) != 0)
	{
		return;
	}

	CString strHost = CString(host_name);
	if(strHost.Left(8).CompareNoCase(mHostNameUsuda) == 0)	//�z�X�g�� �P�c�F����Ԋϑ���(�P�c64m�ǎɓ�)
	{
		m_nLimitedMode = eLimitedMode_Enable;
	}

	return;
}

/*============================================================================*/
/*! �ݔ��A���[���󋵈ꗗ

-# �ݔ��A���[���󋵈ꗗ�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::CreateAlarmThread()
{
	if (m_nSelectMode == eStation_1){
		for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++) {
			mShareAlarmIf[i].Create(true, i);
		}
	}
	//if (mpAlarmThread == NULL){
	//	mpAlarmThread = (CAlarmListThread*)AfxBeginThread(RUNTIME_CLASS(CAlarmListThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	//	mpAlarmThread->ResumeThread();
	//}
}
/*============================================================================*/
/*! �ݔ��A���[���󋵈ꗗ

-# �ݔ��A���[���󋵈ꗗ�̍폜

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::DeleteAlarmThread()
{
	//if (mpAlarmThread != NULL){
	//	mpAlarmThread->StopAlarmThread();
	//	WaitForSingleObject(mpAlarmThread->m_hThread, 5000);
	//	delete mpAlarmThread;
	//	mpAlarmThread = NULL;
	//}
}
/*============================================================================*/
/*! �A�v���P�[�V����

-# �ݔ��A���[���󋵈ꗗ�̍폜

@param

@retval void
*/
/*============================================================================*/
UINT CStationControlApp::GetDrawUpdateInterval(bool bRead)
{
	if (bRead == true){
		m_nDrawUpdateInterval = GetPrivateProfileInt(SECTION_SYSTEM, KEY_DRAWUPDATEINTERVAL, 500, m_strAppInifileName);
	}
	return m_nDrawUpdateInterval;
}


/*============================================================================*/
/*! �A�v���P�[�V����

-# ���[�v�o���h�ݒ�_�C�A���O�N��

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::ExecLoopBandSetDlg(CString ctrl)
{
	vector<CString> list;

	list.push_back(CURRENT_LBW);
	theApp.GetSatelliteData().get_paramlist(ctrl, list);

	AgcLoopBandSelectDlg dlg;
	dlg.SetLBCmbList(list);
	dlg.DoModal();

#if 1
	CString strLB = dlg.m_strLoopBand;
	if (strLB.CompareNoCase(CURRENT_LBW) != 0)
	{
		SendControl(ctrl, strLB, _T(""), _T(""));
	}
#else
#if 1
	CString strLB = dlg.m_strLoopBand;

	CString strSat;
	vector<CShareLbwIf::stItem> slist;
	slist.clear();
	slist.resize(CShareLbwIf::mItemMax);
	mShareLbwIf.Recv((CShareLbwIf::stItem*)&slist.at(0), (UINT)slist.size());

	for (int i = 0; i < slist.size(); ++i)
	{
		CString sCtrl = CString(slist[i].ctrl);
		if (sCtrl.CompareNoCase(ctrl) == 0)
		{
			//			strLB = CString(slist[i].value);
			strSat = CString(slist[i].satellite);
		}
	}

	if (strLB.CompareNoCase(CURRENT_LBW) != 0)
	{
		SendControl(ctrl, strLB, _T(""), _T(""));
	}
	SendControlAgcTblLoad(ctrl, strSat, _T(""), _T(""));

#else
	CString strLB = dlg.m_strLoopBand;
	if (strLB.CompareNoCase(CURRENT_LBW) == 0)
	{
		vector<CShareLbwIf::stItem> list;
		list.clear();
		list.resize(CShareLbwIf::mItemMax);
		mShareLbwIf.Recv((CShareLbwIf::stItem*)&list.at(0), (UINT)list.size());

		for (int i = 0; i < list.size(); ++i)
		{
			CString sCtrl = CString(list[i].ctrl);
			if (sCtrl.CompareNoCase(ctrl) == 0)
			{
				strLB = CString(list[i].value);
			}
		}
	}

	SendControl(ctrl, strLB, _T(""), _T(""));
	SendControlAgcTblLoad(ctrl, strLB, _T(""), _T(""));
#endif
#endif

}

/*============================================================================*/
/*! �A�v���P�[�V����

-# CURRENT_LBW �X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::CreateShareLbwWriteIF()
{
	if ( (m_nSelectMode == eStation_1)
		&& (m_nExecuteMode == eExecuteMode_Control))
	{
		mShareLbwIf.Create(true);
	}
}

void CStationControlApp::CreateShareLbwReadIF()
{
	mShareLbwIf.Create(false);
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# CURRENT_LBW �X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CStationControlApp::WriteExecuteMode2FAFile(UINT mode)
{
	// FA�t�H���_�p�X�擾
	CString fname;
	fname.Format(_T("%s%s\\%s"), (LPCTSTR)theApp.GetAppFaPanelPath(), (LPCTSTR)CString(mStationFolder[theApp.GetSelectStation()]).MakeUpper(), FA_AUTH_FILE);

	FILE* fp;

	if (_wfopen_s(&fp, fname, _T("w")) != 0 || fp == NULL)
	{
		return;
	}

	fwprintf(fp, _T("%u"), mode);

	fclose(fp);
}

