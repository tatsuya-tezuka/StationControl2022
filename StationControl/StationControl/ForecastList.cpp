/*============================================================================*/
/*! ForecastList.cpp

-# �\��l�ꗗ���
*/
/*============================================================================*/
// ForecastList.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ForecastList.h"
#include "afxdialogex.h"
#include "TleList.h"
#include "OperationPlanTimeOffsetSet.h"
#include "..\Utility\SshShell.h"
#include "HookMsgBox.h"

//----------------------------------------------------------------------
// �_�~�[�f�[�^

static const CString strSampleSatelliteName[6] = {
	_T("ALL"),
	_T("GCOM-C"),
	_T("GCOM-W"),
	_T("ALOS-2"),
	_T("HAYABUSA2"),
	_T("123456789012345")
};

static const CString strSampleStationName[5] = {
	_T("ALL"),
	_T("�P�c64m��"),
	_T("���V�Y34m��"),
	_T("���V�Y20m��"),
	_T("�P�c54m��")
};

// ���L�t�@�C���̃��b�Z�[�W��`
#define eShareFile_PredList		0x00000001
#define eShareFile_PlanList		0x00000002

// CForecastList �_�C�A���O

IMPLEMENT_DYNAMIC(CForecastList, CDialogBase)

/*============================================================================*/
/*! CForecastList

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CForecastList::CForecastList(CWnd* pParent /*=NULL*/)
	: CDialogBase(CForecastList::IDD, pParent)
	, m_bUseSelectBtn(FALSE)
	, m_pCallWnd(NULL)
	, m_bCloseUnCheck(FALSE)
{

}

/*============================================================================*/
/*! CForecastList

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CForecastList::~CForecastList()
{
	if (mDetailDlg.GetSafeHwnd() != NULL){
		mDetailDlg.DestroyWindow();
	}
}

void CForecastList::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORCASTLIST_TAB1, m_tabCtrl);
	DDX_Control(pDX, IDC_FORCASTLIST_COMBO, m_CCBoxSatelliteName);
	DDX_Control(pDX, IDC_FORCASTLIST_STATION_COMBO, m_CCBoxStationName);
	DDX_Control(pDX, IDC_STATIC_FORECAST_RADAR_AREA, m_viewArea);
	DDX_Control(pDX, IDC_FORECASTLIST_BUTTON1, m_CButonDelete);
	DDX_Control(pDX, IDC_FORECASTLIST_BUTTON6, m_CButonOffsetTime);
}

BEGIN_MESSAGE_MAP(CForecastList, CDialogBase)
	ON_COMMAND(ID_MENU_OPTION_CLOSE, &CForecastList::OnMenuClose)
	ON_COMMAND(ID_MENU_GET_AUTOMODE, &CForecastList::OnMenuSetAutoMode)
	ON_COMMAND(ID_MENU_GET_MANUALMODE, &CForecastList::OnMenuSetManualModel)
	ON_COMMAND(ID_MENU_MANUAL_GET_FROM_STOCKDATA, &CForecastList::OnMenuGetManual)
	ON_COMMAND(ID_MENU_RESEND_TO_EQUIPMENT_CTRL, &CForecastList::OnMenuResend)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FORCASTLIST_TAB1, &CForecastList::OnTcnSelchangeForcastlistTab1)
	ON_CBN_SELCHANGE(IDC_FORCASTLIST_COMBO, &CForecastList::OnCbnSelchangeForcastlistCombo)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON1, &CForecastList::OnBnClickedForecastlistButton1)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON2, &CForecastList::OnBnClickedForecastlistButton2)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON3, &CForecastList::OnBnClickedForecastlistButton3)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON4, &CForecastList::OnBnClickedForecastlistButton4)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON5, &CForecastList::OnBnClickedForecastlistButton5)
	ON_BN_CLICKED(IDC_FORECASTLIST_BUTTON6, &CForecastList::OnBnClickedForecastlistButton6)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_FORCASTLIST_STATION_COMBO, &CForecastList::OnCbnSelchangeForcastlistStationCombo)
	ON_WM_INITMENUPOPUP()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CForecastList ���b�Z�[�W �n���h���[



/*============================================================================*/
/*! CForecastList

-# ���j���[����̃t�@�C���N���[�Y���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnMenuClose()
{
//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CForecastList

-# ���j���[����̍đ����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnMenuResend()
{
	CString strTitle;
	CString strMsg;
	if (!strTitle.LoadStringW(IDS_FORECASTLIST_RESEND_TITLE))
		return;
	if (!strMsg.LoadStringW(IDS_FORECASTLIST_RESEND_MSG))
		return;

	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// �\��l�đ��v��
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d ALL"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_GETPRE);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// �V�F���҂����Ȃ�
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuResend"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// �������Ȃ�
		}
	}
}

/*============================================================================*/
/*! CForecastList

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CForecastList::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CString str;
	CRect rect;

	m_bCloseUnCheck = FALSE;

	// �^�u������
	if (!m_forcastValue.Create(CForecastValue::IDD, this))
		return FALSE;

	if (!m_operationPlan.Create(CForecastOperationPlan::IDD, this))
		return FALSE;

	m_tabCtrl.GetClientRect(&rect);
	rect.top += nTabTopOffset;
	rect.bottom += nTabBotOffset;
	rect.left += nTabLeftOffset;
	m_forcastValue.MoveWindow(&rect, FALSE);
	m_operationPlan.MoveWindow(&rect, FALSE);

	if (str.LoadStringW(IDS_FORECASTLIST_FORECASTVALUE) != TRUE)
		return FALSE;
	m_tabCtrl.InsertItem(0, str);

	if (str.LoadStringW(IDS_FORECASTLIST_OPERATIONPLAN) != TRUE)
		return FALSE;
	m_tabCtrl.InsertItem(1, str);

	// ���[�_�[������
	CRect rectGraph;
	m_viewArea.GetWindowRect(&rectGraph);
	ScreenToClient(&rectGraph);
	CRuntimeClass* pClass = RUNTIME_CLASS(CForecastRadarGraphView);
	if (pClass == NULL)
	{
		return FALSE;
	}

	m_pView = (CForecastRadarGraphView*)(pClass->CreateObject());
	if (m_pView != NULL)
	{
		m_pView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rectGraph, this, 0, NULL);
		m_pView->SendMessage(WM_INITIALUPDATE);
	}

	OnTcnSelchangeForcastlistTab1(0, 0);

	InitCombobox();

	// �Ď����[�h���̓{�^����񊈐��ɂ���
	if (theApp.GetExecuteMode() != eExecuteMode_Control)
	{
		GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(FALSE);
	}
	//���䃂�[�h�ł��A�������̓{�^����񊈐��ɂ���
	else if(theApp.GetLimitedMode() == eLimitedMode_Enable)
	{
		GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(FALSE);
	}

	m_bUseSelectBtn = FALSE;

	m_pCallWnd = NULL;

	UpdateData(FALSE);

	CenterWindowEx();

	// ���䃂�[�h�ŁA���������łȂ��ꍇ�A�^�C�}�[��������
	if (theApp.GetExecuteMode() == eExecuteMode_Control &&
		theApp.GetLimitedMode() == eLimitedMode_Disable)
	{
		SetTimer(1, 1000, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CForecastList

-# �^�u�I��ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnTcnSelchangeForcastlistTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bUseSelectBtn == TRUE)
	{
		m_tabCtrl.SetCurSel(eForecastListType_Forecast);
	}
	else
	{
		int sel = m_tabCtrl.GetCurSel();
		SelectForcastlistAndInitCombo(sel);
	}

	if (pResult)
		*pResult = 0;
}


/*============================================================================*/
/*! CForecastList

-# �q�����R���{�{�b�N�X�I��ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnCbnSelchangeForcastlistCombo()
{
	// �ĕ`��
	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();
}

/*============================================================================*/
/*! CForecastList

-# �ǖ��擾

@param	�Ȃ�
@retval	�q����

*/
/*============================================================================*/
CString CForecastList::GetStationName()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxStationName.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxStationName.GetLBText(nIndex, str);

	return str;
}

/*============================================================================*/
/*! CForecastList

-# �ȈՋǖ��擾

@param	�Ȃ�
@retval	�q����

*/
/*============================================================================*/
CString CForecastList::GetSimpleStationName()
{
	CString str = GetStationName();

	for (int i = 0; i < eStation_MAX; i++)
	{
		if (str.Compare(mStationString[i]) == 0)
			return mStationSimpleString[i];
	}

	return NULL;
}

/*============================================================================*/
/*! CForecastList

-# �ǃC���f�b�N�X�擾

@param	�Ȃ�
@retval	�ǃC���f�b�N�X

*/
/*============================================================================*/
int CForecastList::GetStationIndex()
{
	UpdateData(TRUE);
	return m_CCBoxStationName.GetCurSel();
}


/*============================================================================*/
/*! CForecastList

-# �ǖ��ɉ������ǃC���f�b�N�X�擾

@param	�ǖ�
@retval	�ǃC���f�b�N�X

*/
/*============================================================================*/
int CForecastList::GetStationIndex(CString& strStationName)
{
	for (int i = 0; i < eStation_MAX; i++)
	{
		if (strStationName.Compare(mStationString[i]) == 0)
			return i;
	}

	return -1;
}


/*============================================================================*/
/*! CForecastList

-# �ǐ����擾

@param	�Ȃ�
@retval	�ǐ�

*/
/*============================================================================*/
int CForecastList::GetStationCount()
{
	return m_CCBoxStationName.GetCount();
}

/*============================================================================*/
/*! CForecastList

-# �q�����擾

@param	�Ȃ�
@retval	�q����

*/
/*============================================================================*/
CString CForecastList::GetSatelliteName()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxSatelliteName.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxSatelliteName.GetLBText(nIndex, str);

	return str;
}

/*============================================================================*/
/*! CForecastList

-# �����I�t�Z�b�g�{�^���Ƀt�H�[�J�X�����Ă�

@param	�Ȃ�
@retval	�q����

*/
/*============================================================================*/
void CForecastList::SetFocusToOffsetTimeBtn()
{
	GotoDlgCtrl(GetDlgItem(IDC_FORECASTLIST_BUTTON6));
}


/*============================================================================*/
/*! CForecastList

-# �^�u��I�����A�q�������Z�b�g����

@param	sel:�^�u�ԍ�
@param	strSatelliteName:�q����
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastList::SelectTabAndSatellite(int sel, CString& strSatelliteName)
{
	m_tabCtrl.SetCurSel(sel);
	SelectForcastlist(sel, strSatelliteName);
}


/*============================================================================*/
/*! CForecastList

-# �^�u��I�����A�R���{����������

@param	�^�u�ԍ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CForecastList::SelectTabAndInitCombo(int sel)
{
	m_tabCtrl.SetCurSel(sel);
	SelectForcastlistAndInitCombo(sel);
}


/*============================================================================*/
/*! CForecastList

-# �\��l�ꗗ�^�v��ꗗ�^�u��I������

@param	�Ȃ�
@retval	�q����

*/
/*============================================================================*/
void CForecastList::SelectForcastlist(int sel, CString& strSatelliteName)
{
	CString str;
	CMenu* pMenu = GetMenu();

	// �^�u���̕\���ؑցA�폜/�I���{�^���ؑ�
	switch (sel)
	{
	case 0:
		// �I�v�V�������j���[��񊈐���
		if (pMenu != NULL)
		{
			if (theApp.GetExecuteMode() == eExecuteMode_Control)
			{
				//���䃂�[�h�ł��A�������̓{�^����񊈐��ɂ���
				if(theApp.GetLimitedMode() == eLimitedMode_Enable)
				{
					pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
				}
				else
				{
					pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
				}
			}
			else
			{
				pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			}
			DrawMenuBar();
		}

		// �ǖ��R���{�ꗗ�A�q�����R���{�ꗗ��\��l�ꗗ����擾����
		m_forcastValue.GetStationNameList(m_listStationName);
		m_forcastValue.GetSatelliteNameList(m_listSatelliteName);

		SetSatelliteName(strSatelliteName);

		/// �\��l�ꗗ��\��
		m_forcastValue.ShowWindow(SW_SHOW);
		m_operationPlan.ShowWindow(SW_HIDE);

		// �폜�{�^�����Z�b�g
		if (m_bUseSelectBtn == FALSE)
		{
			if (str.LoadStringW(IDS_FORECASTLIST_DELETE) != TRUE)
				return;
		}
		else
		// �I���{�^�����Z�b�g
		{
			if (str.LoadStringW(IDS_FORECASTLIST_SELECT) != TRUE)
				return;
		}

		// �\��l�ꗗ�̒ǔ��O���t��\��
		m_forcastValue.InitRadarGraphView();
		break;

	case 1:
	default:
		// �I�v�V�������j���[��񊈐���
		if (pMenu != NULL)
		{
			pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			DrawMenuBar();
		}

		// �ǖ��R���{�ꗗ�A�q�����R���{�ꗗ���v��ꗗ����擾����
		m_operationPlan.GetStationNameList(m_listStationName);
		m_operationPlan.GetSatelliteNameList(m_listSatelliteName);

		// �v��ꗗ��\��
		m_forcastValue.ShowWindow(SW_HIDE);
		m_operationPlan.ShowWindow(SW_SHOW);

		// �폜�{�^�����Z�b�g
		if (str.LoadStringW(IDS_FORECASTLIST_DELETE) != TRUE)
			return;

		// �v��ꗗ�̒ǔ��O���t��\��
		m_operationPlan.InitRadarGraphView();
		break;
	}

	// �폜/�I���{�^���\���ؑ�
	m_CButonDelete.SetWindowTextW(str);
}


/*============================================================================*/
/*! CForecastList

-# �\��l�ꗗ�^�v��ꗗ�^�u��I������

@param	�Ȃ�
@retval	�q����

*/
/*============================================================================*/
void CForecastList::SelectForcastlistAndInitCombo(int sel)
{
	CString str;
	CMenu* pMenu = GetMenu();

	// �^�u���̕\���ؑցA�폜/�I���{�^���ؑ�
	switch (sel)
	{
	case 0:
		// �I�v�V�������j���[��񊈐���
		if (pMenu != NULL)
		{
			if (theApp.GetExecuteMode() == eExecuteMode_Control)
			{
				//���䃂�[�h�ł��A�������̓{�^����񊈐��ɂ���
				if(theApp.GetLimitedMode() == eLimitedMode_Enable)
				{
					pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
				}
				else
				{
					pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
				}
			}
			else
			{
				pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			}
			DrawMenuBar();
		}

		// �ǖ��R���{�ꗗ�A�q�����R���{�ꗗ��\��l�ꗗ����擾����
		m_forcastValue.GetStationNameList(m_listStationName);
		m_forcastValue.GetSatelliteNameList(m_listSatelliteName);

		InitCombobox();

		/// �\��l�ꗗ��\��
		m_forcastValue.ShowWindow(SW_SHOW);
		m_operationPlan.ShowWindow(SW_HIDE);

		// �폜�{�^�����Z�b�g
		if (m_bUseSelectBtn == FALSE)
		{
			if (str.LoadStringW(IDS_FORECASTLIST_DELETE) != TRUE)
				return;
		}
		else
			// �I���{�^�����Z�b�g
		{
			if (str.LoadStringW(IDS_FORECASTLIST_SELECT) != TRUE)
				return;
		}

		// �\��l�ꗗ�̒ǔ��O���t��\��
		m_forcastValue.InitRadarGraphView();

		// �\��l�ꗗ�ł��邱�Ƃ�����
		SetWindowSubCode(0);
		break;

	case 1:
	default:
		// �I�v�V�������j���[��񊈐���
		if (pMenu != NULL)
		{
			pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			DrawMenuBar();
		}

		// �ǖ��R���{�ꗗ�A�q�����R���{�ꗗ���v��ꗗ����擾����
		m_operationPlan.GetStationNameList(m_listStationName);
		m_operationPlan.GetSatelliteNameList(m_listSatelliteName);

		InitCombobox();

		// �v��ꗗ��\��
		m_forcastValue.ShowWindow(SW_HIDE);
		m_operationPlan.ShowWindow(SW_SHOW);

		// �폜�{�^�����Z�b�g
		if (str.LoadStringW(IDS_FORECASTLIST_DELETE) != TRUE)
			return;

		// �v��ꗗ�̒ǔ��O���t��\��
		m_operationPlan.InitRadarGraphView();

		// �v��ꗗ�ł��邱�Ƃ�����
		SetWindowSubCode(1);
		break;
	}

	// �폜/�I���{�^���\���ؑ�
	m_CButonDelete.SetWindowTextW(str);
}


/*============================================================================*/
/*! CForecastList

-# �폜/�I���{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton1()
{
	switch (m_tabCtrl.GetCurSel())
	{
	case 0:
		// �\��l�폜
		if (m_bUseSelectBtn == FALSE)
		{
			// �m�F�_�C�A���O�\��
			CString strTitle;
			CString strMsg;
			if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
				return;
			if (strMsg.LoadString(IDS_DELETE_SELECTED_PRED_MSG) == FALSE)
				return;
			if (IDCANCEL == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
				return;

			vector<stPredListData> listPred;
			BOOL bGetFilePath = m_forcastValue.GetSelectedForecastStationNameAndFullFilePath(listPred);
			int nSize = (int)listPred.size();
			if (bGetFilePath == FALSE || nSize <= 0)
			{
				return;
			}

			for (int i = 0; i < nSize; i++)
			{
				CString strStationName = listPred[i].strStationName;
				CString strSatelliteName = listPred[i].strSatellite;
				CString strPassId = listPred[i].strPassId;
				CString strFilePath = listPred[i].strPredPath;
				// �g���q��prd�̏ꍇ�Aanp�ɒu��������
				strFilePath.TrimRight(PRED_FILE_TYPE);
				strFilePath += ANP_FILE_TYPE;

				// �ǖ����q�������\��l�p�XID���v��Ŏg���Ă��邩���ׂ�
				if (m_operationPlan.IsUsedPlan(strStationName, strSatelliteName, strPassId))
				{
					// �v��Ŏg���Ă���|��\��
					CString strTitle;
					GetWindowText(strTitle);
					MessageBox(_T("�v��Ŏg���Ă���\��l�̂��ߍ폜�ł��܂���B"), strTitle, MB_OK | MB_ICONWARNING);
					continue;
				}

				// �\��l�폜�v��
				CSshShell::structSshParam stParam;
				stParam.strBatchPath = theApp.GetSshBatchPath();
				stParam.strUserName = theApp.GetSshUserName();
				stParam.strServerName = theApp.GetSshServerName();
				stParam.strKeyFilePath = theApp.GetSshKeyPath();
				stParam.strRemShellName = SEND_NOTICE_SH;
				CString arg;

				// �폜
				{
					// �ǖ�����ǔԍ��擾
					int nStation = CForecastList::GetStationIndex(strStationName);
					arg.Format(_T("%s %d %s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_DELPRE, (LPCTSTR)strFilePath);

					stParam.strRemShellParam.Format(arg);

					int err = CSshShell::ExecShell(stParam, TRUE);
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
						CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedForecastlistButton1"), _T("debug"), strDebug, nLogEx::debug);
					}
					if (err == 0)
					{
						// �������Ȃ�
					}
				}
			}
		}
		// �\��l�I��
		else
		{
			CString str;
			if (m_forcastValue.GetSelectedForecastFilePath(str, TRUE))
			{
				m_bCloseUnCheck = TRUE;
				m_strAntFilePath = str;
				theApp.m_pMainWnd->SendMessage(eMessage_UpdateForecastFilePath, (WPARAM)this, (LPARAM)&m_strAntFilePath);
				PostMessage(WM_CLOSE);
			}
		}
		break;

	case 1:
	default:
	{
		// �m�F�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_DELETE_SELECTED_PLAN_MSG) == FALSE)
			return;
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;

		vector<stPlanListData> listPlan;
		BOOL bGetFilePath = m_operationPlan.GetSelectedStationNameAndPlanFilePath(listPlan, TRUE);
		int nSize = (int)listPlan.size();
		if (bGetFilePath == FALSE || nSize <= 0)
		{
			return;
		}
		for (int i = 0; i < nSize; i++)
		{
			// �v��폜�v��
			CSshShell::structSshParam stParam;
			stParam.strBatchPath = theApp.GetSshBatchPath();
			stParam.strUserName = theApp.GetSshUserName();
			stParam.strServerName = theApp.GetSshServerName();
			stParam.strKeyFilePath = theApp.GetSshKeyPath();
			stParam.strRemShellName = REGIST_PLAN_SH;
			CString arg;
			CString strStationName = listPlan[i].strStationName;
			CString strSatelliteName = listPlan[i].strSatelliteName;
			CString strFilePath = listPlan[i].strPlanName;
			CString strGetFrom = listPlan[i].strGetFrom;

			// �ǖ�����ǔԍ��擾
			int nStation = CForecastList::GetStationIndex(strStationName);
			// �擾�����Ǌǐ��̏ꍇ�A�A���t�@�x�b�g�ɕύX
			if (strGetFrom.Compare(PLAN_GETFORM) == 0)
				strGetFrom.Format(_T("sttn"));
			else
				strGetFrom.Format(_T("enp"));

			arg.Format(_T("%s del %s %s %s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), (LPCTSTR)strSatelliteName.MakeLower(), (LPCTSTR)strGetFrom.MakeLower(), (LPCTSTR)strFilePath.MakeLower());

			stParam.strRemShellParam.Format(arg);

			int err = CSshShell::ExecShell(stParam, TRUE);
			{
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
				CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnBnClickedForecastlistButton1"), _T("debug"), strDebug, nLogEx::debug);
			}
			if (err == 0)
			{
				// �������Ȃ�
			}
		}
	}
		break;
	}
}


/*============================================================================*/
/*! CForecastList

-# ���I���G���[�\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::ViewUnSelectedMessage()
{
	CString strTitle;
	CString strMsg;
	if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
		return;
	if (strMsg.LoadString(IDS_UNSELECTED_ERR) == FALSE)
		return;
	MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
}


/*============================================================================*/
/*! CForecastList

-# TLE�ꗗ�{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton2()
{
	// TLE�ꗗ��ʂ��J��
	CTleList dlg;
	if (dlg.DoModal() == IDCLOSE)
	{
		return;
	}
}


/*============================================================================*/
/*! CForecastList

-# �ڍו\���{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton3()
{
	CString strStationName, strSatelliteName, strFilePath, fpath, strGetFrom;
	int nFormat = 0;

	switch (m_tabCtrl.GetCurSel())
	{
	// �\��l�ڍו\��
	case 0:
		if (m_forcastValue.GetSelectedForecastStationNameAndFilePath(strStationName, strFilePath, nFormat, TRUE))
		{
			// �ǖ�����ǔԍ��擾
			int nStation = CForecastList::GetStationIndex(strStationName);

			// �g���q��prd�̏ꍇ�Aanp�ɒu��������
			strFilePath.TrimRight(PRED_FILE_TYPE);
			strFilePath += ANP_FILE_TYPE;

			fpath.Format(_T("%s%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Pred_Folder, nStation), (LPCTSTR)strFilePath);
			mDetailDlg.SetTitle(_T("�\��l�ڍו\��"));

			if (!fpath.IsEmpty())
			{
				// �ڍ׃r���[�\��
				fpath.Replace(_T("/"), _T("\\"));
				mDetailDlg.SetSelectFileName(fpath);
				if (mDetailDlg.GetSafeHwnd() != NULL){
					mDetailDlg.DestroyWindow();
				}
				mDetailDlg.Create(IDD_DIALOG_DETAILVIEW_PRD, this);
				mDetailDlg.ShowWindow(SW_SHOW);
			}
		}
		break;

	// �v��ڍו\��
	case 1:
		if (m_operationPlan.GetSelectedStationNameAndPlanFilePath(strStationName, strSatelliteName, strFilePath, strGetFrom, TRUE))
		{
			// �ǖ�����ǔԍ��擾
			int nStation = CForecastList::GetStationIndex(strStationName);
			// �擾�����Ǌǐ��̏ꍇ�A�A���t�@�x�b�g�ɕύX
			if (strGetFrom.Compare(PLAN_GETFORM) == 0)
				strGetFrom.Format(_T("sttn"));
			else
				strGetFrom.Format(_T("enp"));

			fpath.Format(_T("%s%s\\%s\\%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Plan_Folder, nStation), (LPCTSTR)strGetFrom, (LPCTSTR)strSatelliteName, (LPCTSTR)strFilePath);
			mDetailDlg.SetTitle(_T("�v��ڍו\��"));

			if (!fpath.IsEmpty())
			{
				// �ڍ׃r���[�\��
				fpath.Replace(_T("/"), _T("\\"));
				mDetailDlg.SetSelectFileName(fpath);
				if (mDetailDlg.GetSafeHwnd() != NULL){
					mDetailDlg.DestroyWindow();
				}
				mDetailDlg.Create(IDD_DIALOG_DETAILVIEW, this);
				mDetailDlg.ShowWindow(SW_SHOW);
			}
		}
		break;

	default:
		break;
	}
}


/*============================================================================*/
/*! CForecastList

-# �ڍ׈���{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton4()
{
	CString strStationName, strSatelliteName, strFilePath, fpath, strGetFrom;
	int nFormat = 0;

	CString excelPrm;
	CSatelliteData &sd = theApp.GetSatelliteData();

	switch (m_tabCtrl.GetCurSel())
	{
	// �\��l�t�@�C���p�X�擾
	case 0:
		if (m_forcastValue.GetSelectedForecastStationNameAndFilePath(strStationName, strFilePath, nFormat, TRUE))
		{
			// �ǖ�����ǔԍ��擾
			int nStation = CForecastList::GetStationIndex(strStationName);

			// �g���q��prd�̏ꍇ�Aanp�ɒu��������
			strFilePath.TrimRight(PRED_FILE_TYPE);
			strFilePath += ANP_FILE_TYPE;

			fpath.Format(_T("%s%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Pred_Folder, nStation), (LPCTSTR)strFilePath);
			excelPrm.Format(_T("%s /P%d"), (LPCTSTR)fpath, nFormat);
		}
		break;

	// �v��t�@�C���p�X�擾
	case 1:
		if (m_operationPlan.GetSelectedStationNameAndPlanFilePath(strStationName, strSatelliteName, strFilePath, strGetFrom, TRUE))
		{
			// �ǖ�����ǔԍ��擾
			int nStation = CForecastList::GetStationIndex(strStationName);
			// �擾�����Ǌǐ��̏ꍇ�A�A���t�@�x�b�g�ɕύX
			if (strGetFrom.Compare(PLAN_GETFORM) == 0)
				strGetFrom.Format(_T("sttn"));
			else
				strGetFrom.Format(_T("enp"));

			fpath.Format(_T("%s%s\\%s\\%s"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Plan_Folder, nStation), (LPCTSTR)strGetFrom, (LPCTSTR)strSatelliteName, (LPCTSTR)strFilePath);
			excelPrm = sd.CreatePlnPrintFile(fpath);
		}
		break;

	default:
		break;
	}

	if (!fpath.IsEmpty())
	{
#if 1
		// ���[
		CSatelliteData::ExcelOutPut(m_hWnd, excelPrm);

#else
		// �t�@�C�����
		fpath.Replace(_T("/"), _T("\\"));
		CString strWorkPath = fpath;
		if (CPrintUtility::PrintEucFile(strWorkPath))
		{
			// ����������b�Z�[�W�\��
			CString strTitle;
			CString strMsg, strMsgTmp;
			if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
				return;
			if (strMsgTmp.LoadString(IDS_PRINT_OK_MSG) == FALSE)
				return;
			strMsg.Format(strMsgTmp, fpath);
			MessageBox(strMsg, strTitle, MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			// ����ł��Ȃ��������b�Z�[�W�\��
			CString strTitle;
			CString strMsg, strMsgTmp;
			if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
				return;
			if (strMsgTmp.LoadString(IDS_PRINT_NG_MSG) == FALSE)
				return;
			strMsg.Format(strMsgTmp, fpath);
			MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);

		}
#endif
	}
}


/*============================================================================*/
/*! CForecastList

-# �ꗗ�������

@param  nStation�F�ǔԍ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CForecastList::PrintList(int nStation, CString& fname)
{
	CString fpath;
	switch (m_tabCtrl.GetCurSel())
	{
		// �\��l�ꗗ�t�@�C���p�X�擾
	case 0:
		fpath = theApp.GetShareFilePath(eFileType_PredList_File, nStation);
		break;

		// �v��ꗗ�t�@�C���p�X�擾
	case 1:
		fpath = theApp.GetShareFilePath(eFileType_PlanList_File, nStation);
//		fname = fpath.Right(fpath.GetLength() - fpath.ReverseFind(_T('\\')) - 1);
		break;

	default:
		break;
	}

	if (!fpath.IsEmpty())
	{
		// �t�@�C�����
		fpath.Replace(_T("/"), _T("\\"));
		CString strWorkPath = fpath;
		//		fname = strWorkPath.Right(strWorkPath.GetLength() - strWorkPath.ReverseFind(_T('\\')) - 1);
		fname = strWorkPath;
		CString str;
//		str.Format(_T("%s(%d) :: �ꗗ���"), __FILEW__, __LINE__);
//		str.Format(_T("%s(%d) :: (%s, %d)"), __FILEW__, __LINE__, strWorkPath, nStation);
		str.Format(_T("%s(%d) :: %s, %d"), __FILEW__, __LINE__, (LPCTSTR)strWorkPath, nStation);
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("PrintList"), _T("Info"), str, nLogEx::info);
		if (CPrintUtility::PrintEucFile(strWorkPath))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}


/*============================================================================*/
/*! CForecastList

-# �ꗗ����{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton5()
{
	BOOL ret = TRUE;
	CString fname;
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		ret = PrintList(theApp.GetSelectStation(), fname);
	}
	else if (theApp.GetSelectStationMode() == eStation_5)
	{
		for (int i = 0; i < eStation_MAX; i++)
		{
			if (PrintList(i, fname) == FALSE)
			{
				ret = FALSE;
			}
		}
	}

	if (ret)
	{
		// ����������b�Z�[�W�\��
		CString strTitle;
		CString strMsg, strMsgTmp;
		if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
			return;
		if (strMsgTmp.LoadString(IDS_PRINT_OK_MSG) == FALSE)
			return;
		strMsg.Format(strMsgTmp, fname);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		// ����ł��Ȃ��������b�Z�[�W�\��
		CString strTitle;
		CString strMsg, strMsgTmp;
		if (strTitle.LoadString(IDS_PRINT_TITLE) == FALSE)
			return;
		if (strMsgTmp.LoadString(IDS_PRINT_NG_MSG) == FALSE)
			return;
		strMsg.Format(strMsgTmp, fname);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
	}
}

/*============================================================================*/
/*! CForecastList

-# �����I�t�Z�b�g�{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnBnClickedForecastlistButton6()
{
	// �ꗗ���玞���I�t�Z�b�g�擾
	time_t offsetTime;
	if (!m_forcastValue.GetSelectedForecastOffsetTime(offsetTime))
	{
		return;
	}

	// �I������Ă���\��l�̈�ԏ�̋ǖ��ƃt�@�C���p�X���擾
	CString strStationName, strSatellite, strPassId, strFilePath;
	if (!m_forcastValue.GetSelectedForecastStationNameAndFullFilePath(strStationName, strSatellite, strPassId, strFilePath))
	{
		return;
	}
	// �ǖ�����ǔԍ��擾
	int nStation = CForecastList::GetStationIndex(strStationName);

	// �����I�t�Z�b�g�ݒ�_�C�A���O���Ă�
	COperationPlanTimeOffsetSet dlg;
	dlg.SetTime(offsetTime);
	dlg.SetStationName(CString(mStationSimpleString[nStation]).MakeLower());
	dlg.SetFullFilePath(strFilePath);
	if (dlg.DoModal() == IDCLOSE)
	{
		return;
	}
}


/*============================================================================*/
/*! CForecastList

-# �`�掞�ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// �^�p�v��^�u�̏ꍇ�́A�����I�t�Z�b�g�{�^�����f�B�Z�[�u���ɂ���
	if (m_tabCtrl.GetCurSel() == 1)
	{
		m_CButonOffsetTime.ShowWindow(SW_HIDE);
	}
	// �\��l�^�u�̏ꍇ�́A�����I�t�Z�b�g�{�^�����C�l�[�u���ɂ���
	else
	{
		m_CButonOffsetTime.ShowWindow(SW_SHOW);
	}
}


/*============================================================================*/
/*! CForecastList

-# �ǖ��R���{�{�b�N�X�I��ύX�����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnCbnSelchangeForcastlistStationCombo()
{
	// �ĕ`��
	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();
}


/*============================================================================*/
/*! CForecastList

-# �ǖ����ǃC���f�b�N�X�ɕϊ�����

@param  �Ȃ�
@retval -1�F�ǖ����݂���Ȃ����� / ����ȊO�F�ǖ��ɑΉ�����C���f�b�N�X
*/
/*============================================================================*/
int CForecastList::ChangeSimpleStationNameToIndex(CString stationName)
{
	int sz = sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]);

	for (int i = 0; i < sz; i++)
	{
		CString str(mStationSimpleString[i]);

		if (str.Compare(stationName) == 0)
		{
			return i;
		}
	}

	return -1;
}


/*============================================================================*/
/*! CForecastList

-# �q�������Z�b�g

@param  strStationName�F�q����
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::SetSatelliteName(CString& strStationName)
{
	m_CCBoxSatelliteName.ResetContent();
	if (strStationName.IsEmpty() == true){
		for (int i = 0; i < m_listSatelliteName.size(); i++)
		{
			m_CCBoxSatelliteName.InsertString(-1, m_listSatelliteName[i]);
		}
		m_CCBoxSatelliteName.SetCurSel(0);
	}
	else{
		m_CCBoxSatelliteName.InsertString(-1, strStationName);
	}
	m_CCBoxSatelliteName.SetCurSel(0);

	// �ĕ`��
	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CForecastList

-# �R���{�{�b�N�X��������

@param  strStationName�F�q����
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::InitCombobox()
#if 0
{
	InitComboAndList();
	UpdateData(FALSE);
	m_forcastValue.UpdateData(FALSE);
	m_operationPlan.UpdateData(FALSE);
}
#else
{
	CString str;
	int sz;

	// �R���{�̒��g���폜
	m_CCBoxStationName.ResetContent();
	m_CCBoxSatelliteName.ResetContent();

	// �R���{�{�b�N�X������
	// �ǖ�
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		str = mStationString[theApp.GetSelectStation()];
		m_CCBoxStationName.InsertString(-1, str);
	}
	else if (theApp.GetSelectStationMode() == eStation_5)
	{
		m_CCBoxStationName.InsertString(-1, _T("ALL"));
		sz = sizeof(mStationString) / sizeof(mStationString[0]);
		for (int i = 0; i < sz; ++i)
		{
			m_CCBoxStationName.InsertString(-1, mStationString[i]);
		}
	}
	else
	{
	}
	m_CCBoxStationName.SetCurSel(0);

	// �q����
	// �\��l�ꗗ or �v��ꗗ����q�������X�g������R���{�ɃZ�b�g
	m_CCBoxSatelliteName.InsertString(-1, _T("ALL"));
	for (int i = 0; i < m_listSatelliteName.size(); i++)
	{
		m_CCBoxSatelliteName.InsertString(-1, m_listSatelliteName[i]);
	}
	m_CCBoxSatelliteName.SetCurSel(0);

	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();
}
#endif

#if 0
/*============================================================================*/
/*! CForecastList

-# �R���{�{�b�N�X�ƃ��X�g��������

@param  strStationName�F�q����
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::InitComboAndList()
{
	CString str;
	int sz;

	// �R���{�̒��g���폜
	m_CCBoxStationName.ResetContent();
	m_CCBoxSatelliteName.ResetContent();

	// �R���{�{�b�N�X������
	// �ǖ�
	if (theApp.GetSelectStationMode() == eStation_1)
	{
		str = mStationString[theApp.GetSelectStation()];
		m_CCBoxStationName.InsertString(-1, str);
	}
	else if (theApp.GetSelectStationMode() == eStation_5)
	{
		m_CCBoxStationName.InsertString(-1, _T("ALL"));
		sz = sizeof(mStationString) / sizeof(mStationString[0]);
		for (int i = 0; i < sz; ++i)
		{
			m_CCBoxStationName.InsertString(-1, mStationString[i]);
		}
	}
	else
	{
	}
	m_CCBoxStationName.SetCurSel(0);

	// �q����
	// �\��l�ꗗ or �v��ꗗ����q�������X�g������R���{�ɃZ�b�g
	m_CCBoxSatelliteName.InsertString(-1, _T("ALL"));
	for (int i = 0; i < m_listSatelliteName.size(); i++)
	{
		m_CCBoxSatelliteName.InsertString(-1, m_listSatelliteName[i]);
	}
	m_CCBoxSatelliteName.SetCurSel(0);

	m_forcastValue.UpdateListItem();
	m_operationPlan.UpdateListItem();
}
#endif

/*============================================================================*/
/*! CForecastList

-# �ꗗ�X�V���ɌĂ΂��

@param  strStationName�F�q����
@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CForecastList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
//#ifdef _NOCOMM
//	return CDialogBase::WindowProc(message, wParam, lParam);
//#endif

	switch (message){

	case	eMessage_UpdateShareFile:
		// �\��l�ꗗ�t�@�C���X�V
		if ((int)lParam & eShareFile_PredList)
		{
			m_forcastValue.UpdateListItem();
		}

		if ((int)lParam & eShareFile_PlanList)
		{
			// �v��ꗗ�t�@�C���X�V
			m_operationPlan.UpdateListItem();
		}

		return TRUE;

	default:
		break;
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CForecastList

-# ���j���[�ŁA�擾���[�h�������I�����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnMenuSetAutoMode()
{
	CString strTitle;
	CString strMsg;
	if (!strTitle.LoadStringW(IDS_FORECASTLIST_SETMODE_TITLE))
		return;
	if (!strMsg.LoadStringW(IDS_FORECASTLIST_SETMODE_AUTO_MSG))
		return;

	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// �\��l�����擾���[�h�v��
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_AUTGET);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// �V�F���҂����Ȃ�
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuSetAutoMode"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// �������Ȃ�
		}
	}
}


/*============================================================================*/
/*! CForecastList

-# ���j���[�ŁA�擾���[�h���蓮�I�����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnMenuSetManualModel()
{
	CString strTitle;
	CString strMsg;
	if (!strTitle.LoadStringW(IDS_FORECASTLIST_SETMODE_TITLE))
		return;
	if (!strMsg.LoadStringW(IDS_FORECASTLIST_SETMODE_MANUAL_MSG))
		return;

	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// �\��l�蓮�擾���[�h�v��
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_MNUGET);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// �V�F���҂����Ȃ�
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuSetManualModel"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// �������Ȃ�
		}
	}
}


/*============================================================================*/
/*! CForecastList

-# �\��l���蓮�擾���郁�j���[�I�����ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnMenuGetManual()
{
	CString strTitle;
	CString strMsg;
	if (!strTitle.LoadStringW(IDS_FORECASTLIST_MANUALGET_TITLE))
		return;
	if (!strMsg.LoadStringW(IDS_FORECASTLIST_MANUALGET_MSG))
		return;

	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// �\��l�擾�v��
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;
		CString strSatelliteName = GetSatelliteName();

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d %s"), (LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(), INFO_ID_REQ_GETPRE, (LPCTSTR)strSatelliteName);

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// �V�F���҂����Ȃ�
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuGetManual"), _T("debug"), strDebug, nLogEx::debug);
		}
		if (err == 0)
		{
			// �\��l�擾���̏ꍇ�񊈐���
			GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON3)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON4)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON5)->EnableWindow(FALSE);
		}
	}
}


/*============================================================================*/
/*! CForecastList

-# �\��l�擾���[�h���Ď�����擾����

@param  �Ȃ�
@retval �擾���[�h�F�@ePredGetType_Auto:���� / ePredGetType_Manual:�蓮
*/
/*============================================================================*/
UINT CForecastList::GetPredGetMode()
{
	UINT nCheck = ePredGetType_Other;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();
	CString str;

	/* �f�[�^�~�ϑ��u����̎擾���[�h */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(PRED_GET_MODE));
	if (obs_adr == NULL)
	{
		str.Format(_T("%s(%d) :: Can not get db access."), __FILEW__, __LINE__);
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnUpdateManualMode"), _T("Error"), str, nLogEx::error);
	}
	else if (obs_adr->l_inflg != INFLG_ON)
	{
		str.Format(_T("%s(%d) :: %s is invalidate."), __FILEW__, __LINE__, (LPCTSTR)CString(obs_adr->sz_obsname));
		CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnUpdateManualMode"), _T("Error"), str, nLogEx::error);
	}
	else
	{
		CString str(obs_adr->sz_sts_name);
		if (str.Compare(_T(AUTO_STR)) == 0)
		{
			nCheck = ePredGetType_Auto;
		}
		else if (str.Compare(_T(MANUAL_STR)) == 0)
		{
			nCheck = ePredGetType_Manual;
		}
		else if (str.Compare(_T(STOP_STR)) == 0)
		{
			nCheck = ePredGetType_Stop;
		}
		else
		{
			nCheck = ePredGetType_Other;
		}
	}

	return nCheck;
}


/*============================================================================*/
/*! CForecastList

-# ���j���[�Ăяo�����ɌĂ΂��

@param  pPopupMenu
@param  nIndex		
@param  bSysMenu
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogBase::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	if (pPopupMenu->GetMenuItemID(0) == ID_MENU_GET_AUTOMODE)
	{
		UINT nKanshiMode = GetPredGetMode();
		if (nKanshiMode == ePredGetType_Auto)
		{
			pPopupMenu->CheckMenuItem(ID_MENU_GET_AUTOMODE, MF_BYCOMMAND | MFS_CHECKED);
		}
		else
		{
			pPopupMenu->CheckMenuItem(ID_MENU_GET_AUTOMODE, MF_BYCOMMAND | MFS_UNCHECKED);
		}
	}

	if (pPopupMenu->GetMenuItemID(1) == ID_MENU_GET_MANUALMODE)
	{
		UINT nKanshiMode = GetPredGetMode();
		if (nKanshiMode == ePredGetType_Manual)
		{
			pPopupMenu->CheckMenuItem(ID_MENU_GET_MANUALMODE, MF_BYCOMMAND | MFS_CHECKED);
		}
		else
		{
			pPopupMenu->CheckMenuItem(ID_MENU_GET_MANUALMODE, MF_BYCOMMAND | MFS_UNCHECKED);
		}
	}
}


/*============================================================================*/
/*! CForecastList

-# ���鎞�ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnClose()
{
	if (m_bCloseUnCheck == FALSE)
	{
		CDialogBase::OnClose();
	}
	else
	{
		m_bCloseUnCheck = FALSE;
		CDialogEx::OnClose();
	}
}

/*============================================================================*/
/*! CForecastList

-# �^�C�}���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CForecastList::OnTimer(UINT_PTR nIDEvent)
{
	const CString mPRED_COND = _T("CTRL.PRED_COND");
	const CString mTAKING = _T("TAKING");

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT nStation = theApp.GetSelectStation();

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStation, mPRED_COND);
	if (obs_adr != NULL)
	{
		CString strPredStatus = CString(obs_adr->sz_sts_name);
		if (strPredStatus.CompareNoCase(mTAKING) == 0)
		{
			// �\��l�擾���̏ꍇ�񊈐���
			GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON3)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON4)->EnableWindow(FALSE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON5)->EnableWindow(FALSE);
		}
		else
		{
			// ����ȊO�͊�����
			GetDlgItem(IDC_FORECASTLIST_BUTTON1)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON6)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON3)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON4)->EnableWindow(TRUE);
			GetDlgItem(IDC_FORECASTLIST_BUTTON5)->EnableWindow(TRUE);
		}
	}

	CDialogBase::OnTimer(nIDEvent);
}
