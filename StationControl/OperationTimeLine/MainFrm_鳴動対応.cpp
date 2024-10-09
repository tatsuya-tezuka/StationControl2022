
// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "OperationTimeLine.h"

#include "MainFrm.h"
#include "OperationTimeLineDoc.h"
#include "TimeLineView.h"
#include "../StationControl/HookMsgBox.h"

#ifdef MON_UT
#include "MonitorInfo.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CEngValCnv*	CMainFrame::m_pEngval = NULL;
CEngValCnv* CMainFrame::GetEngval()
{
	return m_pEngval;
}
void CMainFrame::SetEngval(CEngValCnv* eval)
{
	m_pEngval = eval;
}
void CMainFrame::DeleteEngval()
{
	TRY
	{
		delete m_pEngval;
		m_pEngval = NULL;
	} CATCH_ALL(e){
		m_pEngval = NULL;
	}
	END_CATCH_ALL
}

BOOL CALLBACK CMonitors::MonitorNumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	CMonitors* pmon = (CMonitors*)dwData;

	// ���j�^���̎擾
	MONITORINFOEX mInfo;
	mInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mInfo);

	// ���j�^�̈�̎擾
	CRect rect(lprcMonitor);
	pmon->AddMonitor(hMonitor, rect, &mInfo);

	return TRUE;
}

CMonitors::CMonitors()
{
	// �}���`���j�^�Ή�
	EnumDisplayMonitors(NULL, NULL, MonitorNumProc, (LPARAM)this);
}

CMonitors::~CMonitors()
{
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_MS_PLANNING, &CMainFrame::OnMsPlanning)
	ON_COMMAND(ID_TIMEOFFSET_SET, &CMainFrame::OnTimeoffsetSet)
	ON_COMMAND(ID_OP_STARTTIME_CHG, &CMainFrame::OnOpStarttimeChg)
	ON_COMMAND(ID_OP_ENDTIME_CHG, &CMainFrame::OnOpEndtimeChg)
	ON_COMMAND(ID_FORCED_OP_STOP, &CMainFrame::OnForcedOpStop)
	ON_COMMAND(ID_MP_PLANLIST, &CMainFrame::OnMpPlanlist)
	ON_COMMAND(ID_MP_FORECASTLIST, &CMainFrame::OnMpForecastlist)
	ON_COMMAND(ID_MC_CALIBDIAGNS, &CMainFrame::OnMcCalibdiagns)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_PASSSTATUS, &CMainFrame::OnViewPassstatus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PASSSTATUS, &CMainFrame::OnUpdateViewPassstatus)
	ON_COMMAND(ID_VIEW_EQUIPMENT, &CMainFrame::OnViewEquipment)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EQUIPMENT, &CMainFrame::OnUpdateViewEquipment)
	ON_COMMAND(ID_VIEW_EVENT, &CMainFrame::OnViewEvent)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EVENT, &CMainFrame::OnUpdateViewEvent)
	ON_COMMAND(ID_VIEW_LIMITERROR, &CMainFrame::OnViewLimiterror)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIMITERROR, &CMainFrame::OnUpdateViewLimiterror)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, &CMainFrame::OnWindowTileHorz)
	ON_COMMAND(ID_HCOPY_NORMAL, &CMainFrame::OnHcopyNormal)
	ON_UPDATE_COMMAND_UI(ID_HCOPY_NORMAL, &CMainFrame::OnUpdateHcopyNormal)
	ON_COMMAND(ID_HCOPY_REVERSE, &CMainFrame::OnHcopyReverse)
	ON_UPDATE_COMMAND_UI(ID_HCOPY_REVERSE, &CMainFrame::OnUpdateHcopyReverse)
	ON_COMMAND(ID_MH_MANUAL, &CMainFrame::OnMhManual)
	ON_WM_CLOSE()
#ifdef MON_UT
	ON_COMMAND(ID_MON_UT, &CMainFrame::OnMonUt)
	ON_UPDATE_COMMAND_UI(ID_MON_UT, &CMainFrame::OnUpdateMonUt)
#endif
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^�[
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	mReadThread = NULL;
	mThreadEnd = false;
	m_nPlayStatusSound = eStatusSound_Stop;
	mCloseMessage = true;
	m_pMessageThread = NULL;

	mStatusSoundId = 0;

	for (UINT type = 0; type < eStation_MAX; type++){
		m_bCreatePipeService[type] = false;
	}
}

CMainFrame::~CMainFrame()
{
	if (mCaptionFont.GetSafeHandle())
		mCaptionFont.DeleteObject();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndCaption.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, 1500, -1))
	{
		TRACE0("Failed to create caption bar\n");
		return FALSE;
	}

	m_wndCaption.SetFlatBorder();
	m_wndCaption.SetFont(&afxGlobalData.fontBold);
	m_wndCaption.m_clrBarText = RGB(0, 0, 0);
	CString str = mStationString[theApp.GetSelectStation()];
	m_wndCaption.m_clrBarBackground = mTimeLineStationColor[theApp.GetSelectStation()];
	m_wndCaption.SetText(str, CMFCCaptionBar::ALIGN_CENTER);

	CFont defFont;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)){
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 30;
		mCaptionFont.CreateFontIndirect(&lf);
	}
	m_wndCaption.SetFont(&mCaptionFont);

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("���j���[ �o�[���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// �A�N�e�B�u�ɂȂ����Ƃ����j���[ �o�[�Ƀt�H�[�J�X���ړ����Ȃ�
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	//{
	//	TRACE0("�c�[�� �o�[�̍쐬�Ɏ��s���܂����B\n");
	//	return -1;      // �쐬�ł��܂���ł����B
	//}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	//m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	//// ���[�U�[��`�̃c�[�� �o�[�̑���������܂�:
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("�X�e�[�^�X �o�[�̍쐬�Ɏ��s���܂����B\n");
	//	return -1;      // �쐬�ł��Ȃ��ꍇ
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �c�[�� �o�[����у��j���[ �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�́A���� 5 �̍s���폜���܂�
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	//DockPane(&m_wndToolBar);


	// Visual Studio 2005 �X�^�C���̃h�b�L���O �E�B���h�E�����L���ɂ��܂�
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 �X�^�C���̃h�b�L���O �E�B���h�E�̎�����\�������L���ɂ��܂�
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// �Œ�l�Ɋ�Â��ăr�W���A�� �}�l�[�W���[�� visual �X�^�C����ݒ肵�܂�
	OnApplicationLook(theApp.m_nAppLook);

	//// �g���E�B���h�E�Ǘ��_�C�A���O �{�b�N�X��L���ɂ��܂�
	//EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	//// �c�[�� �o�[�ƃh�b�L���O �E�B���h�E ���j���[�̔z�u�ύX��L���ɂ��܂�
	//EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	//// �c�[�� �o�[�̃N�C�b�N (Alt �L�[�������Ȃ���h���b�O) �J�X�^�}�C�Y��L���ɂ��܂�
	//CMFCToolBar::EnableQuickCustomization();

	//if (CMFCToolBar::GetUserImages() == NULL)
	//{
	//	// ���[�U�[��`�̃c�[�� �o�[ �C���[�W��ǂݍ��݂܂�
	//	if (m_UserImages.Load(_T(".\\UserImages.bmp")))
	//	{
	//		CMFCToolBar::SetUserImages(&m_UserImages);
	//	}
	//}

	// ���j���[�̃p�[�\�i���� (�ŋߎg�p���ꂽ�R�}���h) ��L���ɂ��܂�
	// TODO: ���[�U�[�ŗL�̊�{�R�}���h���`���A�e���j���[���N���b�N�����Ƃ��Ɋ�{�R�}���h�� 1 �ȏ�\�������悤�ɂ��܂��B
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// ���j���[���Œ肷��
	DWORD style;
	style = m_wndMenuBar.GetPaneStyle();
	style &= ~CBRS_GRIPPER;
	m_wndMenuBar.SetPaneStyle(style);
	//style = m_wndToolBar.GetPaneStyle();
	//style &= ~CBRS_GRIPPER;
	//m_wndToolBar.SetPaneStyle(style);

	mMem.Create();

	// �^�C�}�[�N��
	SetTimer(0, 1000, NULL);

#ifdef _DEBUG
	SetTimer(9, 1000, NULL);
#endif

	return 0;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���j���[�o�[�ؑ�

@brief	���j���[�o�[��\�����[�h�ɂ��؂�ւ���B

@param
@retval	�Ȃ�

*/
/*============================================================================*/
void CMainFrame::SetStationMenuBar()
{
#ifdef MON_UT
	m_wndMenuBar.SetDefaultMenuResId(IDR_PASSSTATUS_MON_UT);
#else
	m_wndMenuBar.SetDefaultMenuResId(IDR_PASSSTATUS);
#endif /* MON_UT */
	m_wndMenuBar.RestoreOriginalState();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return TRUE;
}

// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ���b�Z�[�W �n���h���[

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ���j���[���X�L�������܂�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���N���X�����ۂ̓�����s���܂��B

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// ���ׂẴ��[�U�[��`�c�[�� �o�[�̃{�^���̃J�X�^�}�C�Y��L���ɂ��܂�
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXIMIZED){
		RECT rect;
		MONITORINFOEX info;
		if (mMonitor.GetCount() != 0 && mMonitor.GetMonitor(1, &rect, &info) == true){
			MoveWindow(&rect);
		}
	}
}


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
// ADD 2021.11.02 >>>
		//=====================================================================
		//	�T�E���h
		//=====================================================================
	case	MM_MCINOTIFY:
		if (wParam == MCI_NOTIFY_SUCCESSFUL){
			if (mStatusSoundId == (MCIDEVICEID)lParam)
				mciSendString(_T("play serror from 0 notify"), NULL, 0, m_hWnd);
			else
				mciSendString(_T("play sevent from 0 notify"), NULL, 0, m_hWnd);
		}
		break;
// ADD 2021.11.02 <<<
	case eMessage_TimeLineHandle:
		break;
	case eMessage_TimeLineFinish:
		if (wParam == 0){
			// Station����I���˗�
			mCloseMessage = false;

			map<CString, CWnd*> mp = theApp.GetDlgMap();
			for (auto itr = mp.begin(); itr != mp.end(); ++itr)
			{
				if (itr->second != NULL)
					itr->second->SendMessage(WM_CLOSE);
			}

			PostMessage(WM_CLOSE, 0, 0);
		}
		break;
	case eMessage_TimeLinePlan:
		break;
	case eMessage_TimeLineForcast:
		break;
	case eMessage_TimeLineEmg:
	case eMessage_TimeLineStop:
		break;
	case ID_PIPE_RECEIVE:
	case ID_PIPE_SEND:
	case ID_PIPE_CREATE:
	case ID_PIPE_FINISH:
	case ID_TIMELINE_MESSAGE:
		messagePipe(message, wParam, lParam);
		break;
	case ID_PIPE_SERVER:
	{
		UINT status;
		status = theApp.AddHistory(message, wParam, lParam);
		if ((theApp.GetView(eTimeViewType_Event))->GetSafeHwnd())		(theApp.GetView(eTimeViewType_Event))->PostMessage(ID_PIPE_RECEIVE, wParam, lParam);
		if ((theApp.GetView(eTimeViewType_LimitError))->GetSafeHwnd())	(theApp.GetView(eTimeViewType_LimitError))->PostMessage(ID_PIPE_RECEIVE, wParam, lParam);
		PlayStatusSound(status);
		break;
	}
		//=====================================================================
		//	�R���g���[��
		//=====================================================================
	case eTimeLineCtrl_AntEmg:
		// �A���e�i������~
		sendPipe(eANTEMG);
		break;
	case eTimeLineCtrl_SndStop:
		// ���M�@��~
		sendPipe(eSENDSTOP);
		break;
	case eTimeLineCtrl_Sound:
		// �G���[���̃T�E���hOFF
		if (lParam != 9){
			sendPipe(eALARMSTOP);
		}
		StopStatusSound();
		break;
	case eTimeLineCtrl_Mute:
		// �G���[���̃T�E���h���~���[�g
// CHG 2021.11.02 >>>
		//if (lParam != 9){
		//	if (lParam == 0)
		//		sendPipe(eALARMMUTEOFF);
		//	if (lParam == 1)
		//		sendPipe(eALARMMUTEON);
		//}
		//MuteStatusSound();
		if (lParam != 9){
			::SendMessage(theApp.GetCallWnd(), eTIMELINE_SET_MUTE, 0, (LPARAM)lParam);
		}
		else{
			if (lParam != 9){
				if (lParam == 0)
					sendPipe(eALARMMUTEOFF);
				if (lParam == 1)
					sendPipe(eALARMMUTEON);
			}
			//MuteStatusSound();
		}
		MuteStatusSound();
// CHG 2021.11.02 <<<
		break;
		// ADD 2021.12.02 >>>
	case eTIMELINE_SET_MUTE:
	{
		// �G���[���̃T�E���h���~���[�g
		CMFCButtonEx& btn = m_wndCaption.GetMuteBtnCtrl();
		btn.ExecuteClicked(true, (int)lParam);
		break;
	}
	// ADD 2021.12.02 <<<
	case eTimeLineCtrl_SnapShot:
		// ��ʈ��
		OnDirectPrint();
		break;
	case eTimeLineCtrl_Help:
	{
		PostMessage(WM_COMMAND, ID_MH_MANUAL, 0);
	}
	break;

	/**********************************************************************************************
	����ȍ~�AStationControl.exe���Ăяo���āA�����擾����
	*/
	case	eTIMELINE_GET_SATNAME:
	{
		TCHAR* p = (TCHAR*)lParam;
		if (theApp.GetCallWnd() == NULL){
			p[0] = '\0';
			return TRUE;
		}
		::SendMessage(theApp.GetCallWnd(), eTIMELINE_GET_SATNAME, 0, (LPARAM)0);
		wsprintf(p, _T("%s"), (LPCTSTR)CString(mMem.Pop()));
		break;
	} //case	eTIMELINE_GET_SATNAME:
	case	eTIMELINE_GET_XS:
	{
		TCHAR* p = (TCHAR*)lParam;
		if (theApp.GetCallWnd() == NULL){
			p[0] = '\0';
			return TRUE;
		}
		// �q���������L��������Push
		mMem.Push(CStringA(p));
		::SendMessage(theApp.GetCallWnd(), eTIMELINE_GET_XS, 0, (LPARAM)0);
		// X�сAS�ё��M�ݔ��g�p�L�����擾
		wsprintf(p, _T("%s"), (LPCTSTR)CString(mMem.Pop()));
		break;
	} //case	eTIMELINE_GET_XS:
	case	eTIMELINE_GET_PRED:
	{
		TCHAR* p = (TCHAR*)lParam;
		if (theApp.GetCallWnd() == NULL){
			p[0] = '\0';
			return TRUE;
		}
		// �q���������L��������Push
		mMem.Push(CStringA(p));
		::SendMessage(theApp.GetCallWnd(), eTIMELINE_GET_PRED, 0, (LPARAM)0);
		break;
	} //case	eTIMELINE_GET_PRED:
	case	eMessage_UpdateForecastFilePath:
	{
		// �\��l�t�@�C�������擾
		TCHAR p[_MAX_PATH];
		wsprintf(p, _T("%s"), (LPCTSTR)CString(mMem.Pop()));
		//MessageBox(p);
		if (theApp.GetView(eTimeViewType_OperatioTimeline) != NULL){
			(theApp.GetView(eTimeViewType_OperatioTimeline))->SendMessage(eMessage_UpdateForecastFilePath, 0, (LPARAM)p);
		}
		break;
	}
	/*
	�����܂�
	************************************************************************************************/


	default:
		return CMDIFrameWndEx::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}

/*============================================================================*/
/*! �^�p�^�C�����C��

-# ���L��������M����

@param

@retval
*/
/*============================================================================*/
UINT ReadLoopThread(LPVOID pParam)
{
	CMainFrame* pWnd = (CMainFrame*)pParam;

	UINT	mCount = 0;
	while (pWnd->GetThreadEnd() == false){
		if ((pWnd->mMem).Count() != 0){
			pWnd->SendMessage(ID_TIMELINE_MESSAGE, 0, 0);
		}
		Sleep(10);
	}

	return 0;
}

/*============================================================================*/
/*! �^�p�^�C�����C��

-# AP�w����̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveServerCallBack(LPPIPEBUFFER pbuf)
{
	::SendMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_RECEIVE, 1, (LPARAM)pbuf);
}

/*============================================================================*/
/*! �^�p�^�C�����C��

-# ���C����ʂ���̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveClientCallBack(LPPIPEBUFFER pbuf)
{
	::SendMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_RECEIVE, 0, (LPARAM)pbuf);
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# AP�w����̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveServerServiceCallBack01(PIPEMONITORACK* pbuf)
{
	// UDSC64
	CGraphMutex::Lock(eMessage);
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(0);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessage);
}
void WINAPI CMainFrame::receiveServerServiceCallBack02(PIPEMONITORACK* pbuf)
{
	// USC34
	CGraphMutex::Lock(eMessage);
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(2);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessage);
}
void WINAPI CMainFrame::receiveServerServiceCallBack03(PIPEMONITORACK* pbuf)
{
	// USC20
	CGraphMutex::Lock(eMessage);
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(3);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessage);
}
void WINAPI CMainFrame::receiveServerServiceCallBack04(PIPEMONITORACK* pbuf)
{
	// UDSC54
	CGraphMutex::Lock(eMessage);
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(1);
	data.push_back(*pbuf);
	CGraphMutex::Unlock(eMessage);
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# AP�w����̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveServerServiceCallBack(PIPEMONITORACK* pbuf)
{
	//if (theApp.GetMessageThread() == 1){
	//	CGraphMutex::Lock(eMessage);
	//	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp();
	//	data.push_back(*pbuf);
	//	CGraphMutex::Unlock(eMessage);
	//	if (data.size() >= 100)
	//		Sleep(10);
	//	return;
	//}

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

	// �H�w�l�ϊ�����ʍX�V�͑Ώۋǂ݂̂Ƃ���
	if (nStation != theApp.GetSelectStation())
		return;

	// StationControl�A�v���̏ꍇ
	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
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

	//static DWORD mCallbackProcStart = 0;
	//DWORD mDrawInterval = theApp.GetDrawUpdateInterval(false);
	//DWORD dwElaps = timeGetTime() - mCallbackProcStart;
	//if (dwElaps < mDrawInterval)
	//	return;
	//mCallbackProcStart = timeGetTime();

	//::PostMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_SERVER, 9, (LPARAM)pbuf);
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# ���C����ʂ���̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void WINAPI CMainFrame::receiveClientServiceCallBack(PIPEMONITORACK* pbuf)
{
	::PostMessage(theApp.m_pMainWnd->m_hWnd, ID_PIPE_RECEIVE, 9, (LPARAM)pbuf);
}
/*============================================================================*/
/*! �^�p�^�C�����C��

-# �v���Z�X�ԒʐM���s�����߂̃p�C�v�쐬

@param

@retval
*/
/*============================================================================*/
void CMainFrame::createPipe()
{
	m_bCreatePipe = false;
	mPipeSend.SetPipeName(mPipeName_TIMELINE);
	mPipeRecv.SetPipeName(mPipeName_TIMELINE + mPipeName_STATION);
	bool ret;
	ret = mPipeSend.Create(false, &receiveServerCallBack, ePIPETYPE_TIMELINE, this);
	if (ret == true){
		ret = mPipeRecv.Create(false, &receiveClientCallBack, ePIPETYPE_TIMELINE, this);
		if (ret == true){
			m_bCreatePipe = true;
		}
		else{
			mPipeSend.Delete();
		}
	}
	sendPipe(eTIMELINESTART);

	// ���b�Z�[�W���W�p�X���b�h�̍쐬
	CreateMessageThread();

	// �Ď��A�����f�[�^�擾�p�p�C�v�̍쐬
// CHG 2017.10.16 >>>
//	m_bCreatePipeService = false;
//	mPipeServiceSend.SetPipeName(mPipeNameString[ePIPETYPE_STATION]);
//	mPipeServiceRecv.SetPipeName(mPipeNameString[ePIPETYPE_STATION]);
//	ret = mPipeServiceSend.CreateService(false, &receiveServerServiceCallBack, ePIPETYPE_STATION, this);
//	if (ret == true){
//		m_bCreatePipeService = true;
//		SendServicePipe(eSTATIONSTART);
//	}
	m_bCreatePipeService[theApp.GetPipeStationIndex()] = false;
	mPipeServiceSend[theApp.GetPipeStationIndex()].SetPipeName(mPipeNameString[ePIPETYPE_STATION + theApp.GetPipeStationIndex()]);
	mPipeServiceRecv[theApp.GetPipeStationIndex()].SetPipeName(mPipeNameString[ePIPETYPE_STATION + theApp.GetPipeStationIndex()]);
	switch (theApp.GetPipeStationIndex()){
	case	0: ret = mPipeServiceSend[0].CreateService(false, &receiveServerServiceCallBack01, ePIPETYPE_STATION, this); break;
	case	1: ret = mPipeServiceSend[1].CreateService(false, &receiveServerServiceCallBack02, ePIPETYPE_STATION, this); break;
	case	2: ret = mPipeServiceSend[2].CreateService(false, &receiveServerServiceCallBack03, ePIPETYPE_STATION, this); break;
	case	3: ret = mPipeServiceSend[3].CreateService(false, &receiveServerServiceCallBack04, ePIPETYPE_STATION, this); break;
	}
	if (ret == true){
		m_bCreatePipeService[theApp.GetPipeStationIndex()] = true;
		SendServicePipe(eSTATIONSTART, theApp.GetPipeStationIndex());
	}
// CHG 2017.10.16 <<<
}

/*============================================================================*/
/*! �^�p�^�C�����C��

-# �v���Z�X�ԒʐM���s�����߂̃p�C�v�폜

@param

@retval
*/
/*============================================================================*/
void CMainFrame::deletePipe()
{
	if (m_bCreatePipe == true){
		sendPipe(eTIMELINEEND);
		// �p�C�v�̍폜
		mPipeSend.Delete();
		mPipeRecv.Delete();
	}
	m_bCreatePipe = false;
	if (mReadThread != NULL){
		mThreadEnd = true;
		WaitForSingleObject(mReadThread, 10000);
		mReadThread = NULL;
	}
	mMem.Delete();

// CHG 2017.10.16 >>>	
//	if (m_bCreatePipeService == true){
//		SendServicePipe(eSTATIONEND);
//		// �p�C�v�̍폜
//		mPipeServiceSend.Delete();
//		mPipeServiceRecv.Delete();
//		if (CMainFrame::GetEngval() != NULL){
//			CMainFrame::DeleteEngval();
//		}
//	}
//	m_bCreatePipeService = false;
	if (m_bCreatePipeService[theApp.GetPipeStationIndex()] == true){
		SendServicePipe(eSTATIONEND, theApp.GetPipeStationIndex());
		// �p�C�v�̍폜
		mPipeServiceSend[theApp.GetPipeStationIndex()].Delete();
		mPipeServiceRecv[theApp.GetPipeStationIndex()].Delete();
		if (CMainFrame::GetEngval() != NULL){
			CMainFrame::DeleteEngval();
		}
	}
	m_bCreatePipeService[theApp.GetPipeStationIndex()] = false;
// CHG 2017.10.16 >>>
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# �T�[�r�X���W���[���ւ̃p�C�v�ʐM

@param

@retval
*/
/*============================================================================*/
void CMainFrame::SendServicePipe(DWORD command, int nStation)
{
	switch (command)
	{
	case	eSTATIONSTART:
	{
		return;
		PIPESTART buf;
		memset(&buf, 0, sizeof(PIPESTART));
		buf.header.size = sizeof(PIPESTART) - sizeof(PIPEHEADER);
		buf.header.command = ePipeCommand_start;
		if (theApp.GetSelectStation() == eStation_Usuda64)
			buf.header.station = ePipeStation_udsc64;
		if (theApp.GetSelectStation() == eStation_Usuda642)
			buf.header.station = ePipeStation_xdsc54;
		if (theApp.GetSelectStation() == eStation_Uchinoura34)
			buf.header.station = ePipeStation_usc34;
		if (theApp.GetSelectStation() == eStation_Uchinoura20)
			buf.header.station = ePipeStation_usc20;
		DWORD ip = theApp.GetHostIpInfo();
		buf.val[0] = ((BYTE)ip & 0xff);
		buf.val[1] = ((BYTE)(ip >> 8) & 0xff);
		buf.val[2] = ((BYTE)(ip >> 16) & 0xff);
		buf.val[3] = ((BYTE)(ip >> 24) & 0xff);
		mPipeServiceSend[nStation].Send(&buf);
		break;
	}
	case	eSTATIONEND:
	{
		PIPESTART buf;
		memset(&buf, 0, sizeof(PIPESTART));
		buf.header.size = sizeof(PIPESTART) - sizeof(PIPEHEADER);
		buf.header.command = ePipeCommand_stop;
		if (theApp.GetSelectStation() == eStation_Usuda64)
			buf.header.station = ePipeStation_udsc64;
		if (theApp.GetSelectStation() == eStation_Usuda642)
			buf.header.station = ePipeStation_xdsc54;
		if (theApp.GetSelectStation() == eStation_Uchinoura34)
			buf.header.station = ePipeStation_usc34;
		if (theApp.GetSelectStation() == eStation_Uchinoura20)
			buf.header.station = ePipeStation_usc20;
		DWORD ip = theApp.GetHostIpInfo();
		buf.val[0] = ((BYTE)ip & 0xff);
		buf.val[1] = ((BYTE)(ip >> 8) & 0xff);
		buf.val[2] = ((BYTE)(ip >> 16) & 0xff);
		buf.val[3] = ((BYTE)(ip >> 24) & 0xff);
		mPipeServiceSend[nStation].Send(&buf);
		break;
	}
	default:
		return;
	}
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# �v���Z�X�ԒʐM�p�C�v�ɃR�}���h�𑗂�

@param

@retval
*/
/*============================================================================*/
void CMainFrame::sendPipe(DWORD command, CString param/* = _T("")*/)
{
	if (m_bCreatePipe == true){
		PIPEBUFFER buf;
		buf.command = mCommand;
		buf.commandIndex = command;
		memset(buf.buf, 0, mPipeBufferSize + 1);
		if (param.IsEmpty() == false){
			sprintf_s((char*)buf.buf, (mPipeBufferSize + 1), "%s", (LPCSTR)CStringA(param));
		}
		mPipeSend.Send(&buf);
	}
}
/*============================================================================*/
/*! �^�p�^�C�����C��

-# �v���Z�X�ԒʐM���b�Z�[�W����������

@param		message		���b�Z�[�WID

@retval
*/
/*============================================================================*/
void CMainFrame::messagePipe(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == ID_PIPE_RECEIVE){
		LPPIPEBUFFER pbuf = (LPPIPEBUFFER)lParam;
		switch (pbuf->commandIndex){
		case	eSTATIONEND:
			mCloseMessage = false;
			PostMessage(WM_CLOSE, 0, 0);
			break;
		case	eHELLO:
			TRACE("## Hello Message\n");
			break;
		case	eSENDMSG:
			TRACE("## Send Message\n");
			break;
		case	eRECVMSG:
			TRACE("## Receive Message\n");
			break;
		case	eANTEMG:
			TRACE("## AntEmg Message\n");
			break;
		case	eSENDSTOP:
			TRACE("## SendStop Message\n");
			break;

		// �A���[���֘A
		case	eALARMSTOP:
		{
			// �^�C�����C��������ʒm
			CMFCButtonEx& btn = m_wndCaption.GetSoundBtnCtrl();
			btn.ExecuteClicked(true, 0);
			break;
		}
		case	eALARMMUTEOFF:
		{
			// �^�C�����C��������ʒm
			CMFCButtonEx& btn = m_wndCaption.GetMuteBtnCtrl();
			btn.ExecuteClicked(true, 0);
			break;
		}
		case	eALARMMUTEON:
		{
			// �^�C�����C��������ʒm
			CMFCButtonEx& btn = m_wndCaption.GetMuteBtnCtrl();
			btn.ExecuteClicked(true, 1);
			break;
		}

		case	eMONITORDATA:
		{
			// �Ď��f�[�^�ʒm
			CMonitorDataTemp data;
			CEngValCnv::obs_tbl_res_t val;

			int len = pbuf->bufsize;
			int pos = 0;
			while (len>0){
				memcpy(&val, &pbuf->buf[pos], sizeof(CEngValCnv::obs_tbl_res_t));
				data.mData.push_back(val);
				pos += sizeof(CEngValCnv::obs_tbl_res_t);
				len -= sizeof(CEngValCnv::obs_tbl_res_t);
			}
			int i = 0;
			break;
		}

		case	eHISTORYEQ:
		{
			break;
			CGraphMutex::Lock(eData);
			TRACE("## History Equipment Message\n");
			deque<stHistoryData>& list = theApp.GetEquipmentHistoryData();
			if (list.size() == MAX_HISTORY_DATA){
				list.pop_front();
			}
			stHistoryData data;
			memcpy(&data, pbuf->buf, sizeof(stHistoryData));
			list.push_back(data);
			PlayStatusSound(data.status);
			if ((theApp.GetView(eTimeViewType_LimitError))->GetSafeHwnd())	(theApp.GetView(eTimeViewType_LimitError))->PostMessage(message, 0, 0);
			CGraphMutex::Unlock(eData);
			break;
		}
		case	eHISTORYEVENT:
		{
			break;
			CGraphMutex::Lock(eData);
			TRACE("## History Event Message\n");
			deque<stHistoryData>& list = theApp.GetEventHistoryData();
			if (list.size() == MAX_HISTORY_DATA){
				list.pop_front();
			}
			stHistoryData data;
			memcpy(&data, pbuf->buf, sizeof(stHistoryData));
			list.push_back(data);
			PlayStatusSound(data.status);
			if ((theApp.GetView(eTimeViewType_Event))->GetSafeHwnd())		(theApp.GetView(eTimeViewType_Event))->PostMessage(message, 0, 0);
			CGraphMutex::Unlock(eData);
			break;
		}
		}
		return;

	}
	if (message == ID_PIPE_CREATE){
		createPipe();
		return;
	}
	if (message == ID_TIMELINE_MESSAGE){
		//CStringA str = mMem.Pop();
		//TRACE("### MSG %s\n", str);
		//if (str.GetAt(0) == '0'){
		//	SendMonitorMessage(str);
		//}
		//else{
		//	RecvMonitorMessage(str);
		//}
		return;
	}
}

void CMainFrame::OnDestroy()
{
	// ���b�Z�[�W���W�p�X���b�h�̏I��
	RemoveMessageThread();

	deletePipe();

	CMDIFrameWndEx::OnDestroy();
}

void CMainFrame::OnWindowTileHorz()
{
	OnTileHorz();
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �E�B���h�E�̕��ׂĕ\�����s��

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnTileHorz()
{
	CView* pview;
	CWnd* pwnd;
	CRect rc, rect[eTimeViewType_Max], maxRect, workArea;

	// +----------------------++----------------------------------+
	// + �^�C�����C��         ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +                      ++                                  +
	// +----------------------------------------------------------+

	// MDI�̍�Ɨ̈���擾����
	//GetDlgItem(AFX_IDW_PANE_FIRST)->GetWindowRect(rc);
	GetClientRect(rc);
	CWnd* pWnd = CWnd::FromHandle(m_hWndMDIClient);
	pWnd->GetClientRect(rc);
	workArea = rc;

	//MDITile(MDITILE_HORIZONTAL);
	maxRect = workArea;

	UINT	width, height;
	// �^�p�󋵉��
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height();
	pview = theApp.GetView(eTimeViewType_OperatioTimeline);
	if (pview){
		pwnd = pview->GetParent();
		rect[eTimeViewType_OperatioTimeline].left = 0;
		rect[eTimeViewType_OperatioTimeline].top = 0;
		rect[eTimeViewType_OperatioTimeline].right = rect[eTimeViewType_OperatioTimeline].left + width;
		rect[eTimeViewType_OperatioTimeline].bottom = maxRect.bottom;
		pwnd->SetWindowPos(&wndTop, rect[eTimeViewType_OperatioTimeline].left, rect[eTimeViewType_OperatioTimeline].top, rect[eTimeViewType_OperatioTimeline].Width(), rect[eTimeViewType_OperatioTimeline].Height(), SWP_NOZORDER);
	}
	// �ݔ��󋵉��
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 2 * 1;
	pview = theApp.GetView(eTimeViewType_EquipmentSystem);
	if (pview){
		pwnd = pview->GetParent();
		rect[eTimeViewType_EquipmentSystem].left = rect[eTimeViewType_OperatioTimeline].right;
		rect[eTimeViewType_EquipmentSystem].top = 0;
		rect[eTimeViewType_EquipmentSystem].right = maxRect.right;
		rect[eTimeViewType_EquipmentSystem].bottom = rect[eTimeViewType_EquipmentSystem].top + height;
		pwnd->SetWindowPos(&wndTop, rect[eTimeViewType_EquipmentSystem].left, rect[eTimeViewType_EquipmentSystem].top, rect[eTimeViewType_EquipmentSystem].Width(), rect[eTimeViewType_EquipmentSystem].Height(), SWP_NOZORDER);
	}

	// �C�x���g���O
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 4 * 1;
	pview = theApp.GetView(eTimeViewType_Event);
	if (pview){
		pwnd = pview->GetParent();
		rect[eTimeViewType_Event].left = rect[eTimeViewType_OperatioTimeline].right;
		rect[eTimeViewType_Event].top = rect[eTimeViewType_EquipmentSystem].bottom;
		rect[eTimeViewType_Event].right = maxRect.right;
		rect[eTimeViewType_Event].bottom = rect[eTimeViewType_Event].top + height;
		pwnd->SetWindowPos(&wndTop, rect[eTimeViewType_Event].left, rect[eTimeViewType_Event].top, rect[eTimeViewType_Event].Width(), rect[eTimeViewType_Event].Height(), SWP_NOZORDER);
	}

	// ���~�b�g�G���[���O
	width = maxRect.Width() / 2 * 1;
	height = maxRect.Height() / 4 * 1;
	pview = theApp.GetView(eTimeViewType_LimitError);
	if (pview){
		pwnd = pview->GetParent();
		rect[eTimeViewType_LimitError].left = rect[eTimeViewType_OperatioTimeline].right;
		rect[eTimeViewType_LimitError].top = rect[eTimeViewType_Event].bottom;
		rect[eTimeViewType_LimitError].right = maxRect.right;
		rect[eTimeViewType_LimitError].bottom = maxRect.bottom;
		pwnd->SetWindowPos(&wndTop, rect[eTimeViewType_LimitError].left, rect[eTimeViewType_LimitError].top, rect[eTimeViewType_LimitError].Width(), rect[eTimeViewType_LimitError].Height(), SWP_NOZORDER);
	}
}


void CMainFrame::OnMsPlanning()
{
	// �^�p�v�旧�ĉ�ʌĂяo��
	sendPipe(eTIMELINE_OPEPLAN);
}


void CMainFrame::OnTimeoffsetSet()
{
	// �����I�t�Z�b�g��ʌĂяo��
	sendPipe(eTIMELINE_TIMEOFFSET);
}


void CMainFrame::OnOpStarttimeChg()
{
	// �v��J�n�����ύX��ʌĂяo��
	sendPipe(eTIMELINE_PLANSTARTTIME);
}


void CMainFrame::OnOpEndtimeChg()
{
	// �v��I�������ύX��ʌĂяo��
	sendPipe(eTIMELINE_PLANSTOPTIME);
}


void CMainFrame::OnForcedOpStop()
{
	// �����^�p��~�m�F��ʌĂяo��
	sendPipe(eTIMELINE_PLANEMGSTOP);
}


void CMainFrame::OnMpPlanlist()
{
	// �^�p�v��ꗗ��ʌĂяo��
	sendPipe(eTIMELINE_FORECASTPLAN);
}


void CMainFrame::OnMpForecastlist()
{
	// �\��l�ꗗ��ʌĂяo��
	sendPipe(eTIMELINE_FORECAST);
}


void CMainFrame::OnMcCalibdiagns()
{
	// �Z���f�f��ʌĂяo��
	sendPipe(eTIMELINE_CALIBRATION);
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0){
		m_wndCaption.RedrawWindow();
		return;
	}

	if (nIDEvent == 9){
		PIPEMONITORACK buf;
		buf.header.size = 137;
		theApp.CreateDataToHistory(&buf);
		PostMessage(ID_PIPE_SERVER, 0, 0);
		return;
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::OnViewPassstatus()
{
	if (theApp.GetView(eTimeViewType_OperatioTimeline) == NULL)
		theApp.OpenView(eTimeViewType_OperatioTimeline);
	else
	{
//		theApp.CloseView(eTimeViewType_OperatioTimeline);
		CView *view = theApp.GetView(eTimeViewType_OperatioTimeline);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateViewPassstatus(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eTimeViewType_OperatioTimeline) != NULL) ? 1 : 0);
}


void CMainFrame::OnViewEquipment()
{
	if (theApp.GetView(eTimeViewType_EquipmentSystem) == NULL)
		theApp.OpenView(eTimeViewType_EquipmentSystem);
	else
	{
//		theApp.CloseView(eTimeViewType_EquipmentSystem);
		CView *view = theApp.GetView(eTimeViewType_EquipmentSystem);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateViewEquipment(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eTimeViewType_EquipmentSystem) != NULL) ? 1 : 0);
}


void CMainFrame::OnViewEvent()
{
	if (theApp.GetView(eTimeViewType_Event) == NULL)
		theApp.OpenView(eTimeViewType_Event);
	else
	{
//		theApp.CloseView(eTimeViewType_Event);
		CView *view = theApp.GetView(eTimeViewType_Event);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateViewEvent(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eTimeViewType_Event) != NULL) ? 1 : 0);
}


void CMainFrame::OnViewLimiterror()
{
	if (theApp.GetView(eTimeViewType_LimitError) == NULL)
		theApp.OpenView(eTimeViewType_LimitError);
	else
	{
//		theApp.CloseView(eTimeViewType_LimitError);
		CView *view = theApp.GetView(eTimeViewType_LimitError);
		((CMDIChildWnd*)view->GetParent())->MDIActivate();
	}
}


void CMainFrame::OnUpdateViewLimiterror(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetView(eTimeViewType_LimitError) != NULL) ? 1 : 0);
}

void CMainFrame::OnHcopyNormal()
{
	theApp.SetHcopyType(ID_HCOPY_NORMAL);
}
void CMainFrame::OnUpdateHcopyNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetHcopyType() == ID_HCOPY_NORMAL) ? 1 : 0);
}
void CMainFrame::OnHcopyReverse()
{
	theApp.SetHcopyType(ID_HCOPY_REVERSE);
}
void CMainFrame::OnUpdateHcopyReverse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((theApp.GetHcopyType() == ID_HCOPY_REVERSE) ? 1 : 0);
}

// �戵�������\��
void CMainFrame::OnMhManual()
{
//	AfxMessageBox(_T("DUMMY �I�I�I����\��"), MB_OK | MB_ICONEXCLAMATION);
	CString strDocFilePath;
	strDocFilePath.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppPath(), (LPCTSTR)CString(DOCUMENT_FILENAME));
	// ���s
	ShellExecute(NULL, _T("open"), (LPTSTR)strDocFilePath.GetBuffer(), NULL, NULL, SW_HIDE);
	strDocFilePath.ReleaseBuffer();
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���A���^�C���̗�����񂩂�x���A�G���[���̓T�E���h�Đ����s��

@param	status	�����X�e�[�^�X
@retval	�Ȃ�

*/
/*============================================================================*/
void CMainFrame::PlayStatusSound(UINT status)
{
	// ADD 2021.11.02 >>>
	if (status == eHistStatus_Fault){
		// ���w���Ȃ̂Ń{�^����Ԃ��`�F�b�N����
		if (m_wndCaption.GetMuteBtnCtrl().GetCheck() == 0){
			// Mute_Off���
			if (m_nPlayStatusSound == eStatusSound_Mute){
				m_nPlayStatusSound = eStatusSound_Stop;
			}
		}
		else if (m_wndCaption.GetMuteBtnCtrl().GetCheck() != 0){
			// Mute_On���
			if (m_nPlayStatusSound != eStatusSound_Mute){
				MuteStatusSound();
			}
		}
	}
	// ADD 2021.11.02 <<<

	if (m_nPlayStatusSound == eStatusSound_Mute){
		return;
	}
	if (m_nPlayStatusSound != eStatusSound_Stop){
		return;
	}

	// �A���[�����̂ݖ�
	if (status != eHistStatus_Fault)
	{
		return;
	}

	//mciSendString(_T("open StatusError.wav type waveaudio alias serror"), NULL, 0, NULL);
	//mStatusSoundId = mciGetDeviceID(_T("serror"));
	//mciSendString(_T("play serror from 0 notify"), NULL, 0, m_hWnd);
	//m_nPlayStatusSound = eStatusSound_Play;

	WCHAR strCurDir[1024];
	GetCurrentDirectory(1024, strCurDir);

	CString strOpenMsg;
	strOpenMsg.Format(_T("open %sStatusError.wav type waveaudio alias serror"), (LPCTSTR)theApp.GetAppPath());
	int err = mciSendString((LPCTSTR)strOpenMsg, NULL, 0, NULL);
	mStatusSoundId = mciGetDeviceID(_T("serror"));
	err = mciSendString(_T("play serror from 0 notify"), NULL, 0, m_hWnd);
	m_nPlayStatusSound = eStatusSound_Play;

	// �{�^���J���[�ύX
	m_wndCaption.GetSoundBtnCtrl().SetFaceColor(RGB(0, 255, 0));
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���A���^�C���̗�����񂩂�̌x���A�G���[���̃T�E���h���~����

@param
@retval	�Ȃ�

*/
/*============================================================================*/
void CMainFrame::StopStatusSound()
{
	if (m_nPlayStatusSound == eStatusSound_Mute){
		return;
	}
	if (m_nPlayStatusSound == eStatusSound_Play){
		mciSendString(_T("close serror"), NULL, 0, NULL);
		m_nPlayStatusSound = eStatusSound_Stop;
		mStatusSoundId = 0;
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���A���^�C���̗�����񂩂�x���A�G���[���̃T�E���h�Đ����֎~����

@param
@retval	�Ȃ�

*/
/*============================================================================*/
void CMainFrame::MuteStatusSound()
{
	if (m_nPlayStatusSound == eStatusSound_Mute){
		m_nPlayStatusSound = eStatusSound_Stop;
		return;
	}
	StopStatusSound();
	m_nPlayStatusSound = eStatusSound_Mute;
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ��ʃn�[�h�R�s�[

@param
@retval

*/
/*============================================================================*/
void CMainFrame::OnDirectPrint()
{
	// �ꉞ�S���
	// �A�N�e�B�u�E�B���h�E���n�[�h�R�s�[����ꍇ�̓A�N�e�B�u�̃E�B���h�E�n���h����n��
	//SetTimer(1, 100, NULL);
	CString	strExePath, strParam;
	if (theApp.GetHcopyType() == ID_HCOPY_NORMAL)
		strExePath.Format(_T("C:\\HardCopy\\NormalHardCopy.exe"));
	else
		strExePath.Format(_T("C:\\HardCopy\\ReverseHardCopy.exe"));
	strParam.Empty();
	::ShellExecute(m_hWnd, _T(""), strExePath, strParam, NULL, SW_NORMAL);
	//DirectPrint(this);
}




void CMainFrame::OnClose()
{
	if (mCloseMessage == true){
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), _T("OperationTimeLine"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	CMDIFrameWndEx::OnClose();
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���b�Z�[�W���W�X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CMainFrame::CreateMessageThread()
{
	if (theApp.GetMessageThread() == 1){
		if (m_pMessageThread == NULL){
			m_pMessageThread = (CMessageThread*)AfxBeginThread(RUNTIME_CLASS(CMessageThread), THREAD_PRIORITY_HIGHEST/*THREAD_PRIORITY_NORMAL*/, 0, CREATE_SUSPENDED);
			m_pMessageThread->ResumeThread();
		}
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���b�Z�[�W���W�X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CMainFrame::RemoveMessageThread()
{
	if (m_pMessageThread != NULL){
		m_pMessageThread->StopThread();
		WaitForSingleObject(m_pMessageThread->m_hThread, 10000);
		delete m_pMessageThread;
		m_pMessageThread = NULL;
	}
}

#ifdef MON_UT
/*============================================================================*/
// �Ď����_�C���O
/*============================================================================*/
void CMainFrame::OnMonUt()
{
	CMonitorInfo dlg;
	dlg.DoModal();
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}

/*============================================================================*/
// �Ď����_�C���O
/*============================================================================*/
void CMainFrame::OnUpdateMonUt(CCmdUI *pCmdUI)
{
	// TODO:�����ɃR�}���h�X�V UI �n���h���[ �R�[�h��ǉ����܂��B
}
#endif

