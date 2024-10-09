/*============================================================================*/
/*! TrsControlMng.cpp

-# ���M����}�l�[�W���[�N���X
*/
/*============================================================================*/

#include "stdafx.h"
#include "StationControl.h"
#include "MainFrm.h"
#include "TrsControlMng.h"
#include "TrsControlXX54.h"


CTrsControlMng::CTrsControlMng()
{
}


CTrsControlMng::~CTrsControlMng()
{
}


/*============================================================================*/
/*! CTrsControl

-# ���M�����ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlMng::DisplayTrsControl()
{
	UINT sta = theApp.GetSelectStation();
	if (sta == eStation_Usuda642)
	{
		// 54m�̏ꍇ
		// �w-X��TCR���M������(54m)
		CTrsControlXX54* pDlg = (CTrsControlXX54*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_XX54);
		if (pDlg != NULL){
			pDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg = new CTrsControlXX54();
			if (pDlg != NULL)
			{
				if (pDlg->Create(IDD_DLG_TRSCTRL_XX54, (CMainFrame*)theApp.GetMainWnd()))
				{
					pDlg->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_XX54);
				}
			}
		}

		return;
	}

	char	sz_status[256] = { 0 };
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �p�X�i�s�󋵊m�F
	memset(sz_status, (char)NULL, sizeof(sz_status));

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_PASS_INFO_NO));
	if (obs_adr != NULL)
	{
		strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlMng"), _T("DisplayTrsControl"), _T("CTRL.PASS_INFO"), CString(sz_status), nLogEx::info);
	}

	// CHG 2017.08.28 >>>
	//if ((strcmp(sz_status, CT_PASS_INFO_INIT) == 0) 			// �����ݒ�
	//	|| (strcmp(sz_status, CT_PASS_INFO_1WAY) == 0) 			// �P�v�`�x�ߑ���
	//	|| (strcmp(sz_status, CT_PASS_INFO_2WAY) == 0)			// �Q�v�`�x�ߑ���
	//	|| (strcmp(sz_status, CT_PASS_INFO_PASS_START1) == 0)	// �p�X�J�n(1WAY)
	//	|| (strcmp(sz_status, CT_PASS_INFO_PASS_START2) == 0)	// �p�X�J�n(2WAY)
	//	|| (strcmp(sz_status, CT_PASS_INFO_POL) == 0)			// �V���ʉߊĎ��̈��
	//	|| (strcmp(sz_status, CT_PASS_INFO_POSN) == 0)			// ANT POSN MODE �ڍs�p���o
	//	|| (strcmp(sz_status, CT_PASS_INFO_EXTEND) == 0)		// �p�X����
	//	)
	CStringA status = CStringA(sz_status);
	status.TrimLeft();
	status.TrimRight();
	if (status == CStringA(CT_PASS_INFO_INIT) ||			// �����ݒ�
		status == CStringA(CT_PASS_INFO_1WAY) ||			// �P�v�`�x�ߑ���
		status == CStringA(CT_PASS_INFO_2WAY) ||			// �Q�v�`�x�ߑ���
		status == CStringA(CT_PASS_INFO_PASS_START1) ||		// �p�X�J�n(1WAY)
		status == CStringA(CT_PASS_INFO_PASS_START2) ||		// �p�X�J�n(2WAY)
		status == CStringA(CT_PASS_INFO_POL) ||				// �V���ʉߊĎ��̈��
		status == CStringA(CT_PASS_INFO_POSN) ||			// ANT POSN MODE �ڍs�p���o
		status == CStringA(CT_PASS_INFO_EXTEND)				// �p�X����
		)
// CHG 2017.08.28 <<<
	{
		// �p�X���Ȃ̂Ŏg�p�ш�̑��M�����ʂ�\������
		memset(sz_status, (char)NULL, sizeof(sz_status));

		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_X_TX_EXIST));
		if (obs_adr != NULL)
		{
			strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
			CLogTraceEx::Write(_T(""), _T("CTrsControlMng"), _T("DisplayTrsControl"), _T("CTRL.CTRL.X_TX_EXIST"), CString(sz_status), nLogEx::info);
		}

		// �w�ё��M�ݔ����g�p����p�X��
		if (strcmp(sz_status, "����") == 0)
		{
			// �w�ё��M�ݔ�
			memset(sz_status, (char)NULL, sizeof(sz_status));
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(XIFSW_TCRMOD_SEL));
			if (obs_adr != NULL)
			{
				strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
				CLogTraceEx::Write(_T(""), _T("CTrsControlMng"), _T("DisplayTrsControl"), _T("X-IFSW.MOD_TCRMOD_SE"), CString(sz_status), nLogEx::info);
			}

			if (strcmp(sz_status, "TCRMOD") == 0)
			{
				// X�ё��M������
				CTrsControlX* pDlg = (CTrsControlX*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_X);
				if (pDlg != NULL){
					pDlg->ShowWindow(SW_SHOW);
				}
				else
				{
					pDlg = new CTrsControlX();
					if (pDlg != NULL)
					{
						if (pDlg->Create(IDD_DLG_TRSCTRL_X, (CMainFrame*)theApp.GetMainWnd()))
						{
							pDlg->ShowWindow(SW_SHOW);
							theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_X);
						}
					}
				}
			}
			else
			{
				// �w-X��TCR���M������
				CTrsControlXX* pDlg = (CTrsControlXX*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_XX);
				if (pDlg != NULL){
					pDlg->ShowWindow(SW_SHOW);
				}
				else
				{
					pDlg = new CTrsControlXX();
					if (pDlg != NULL)
					{
						if (pDlg->Create(IDD_DLG_TRSCTRL_XX, (CMainFrame*)theApp.GetMainWnd()))
						{
							pDlg->ShowWindow(SW_SHOW);
							theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_XX);
						}
					}
				}
			}
		}
		else
		{
			// S�ё��M������
			if (sta == eStation_Usuda64)
			{
				CTrsControlS64* pDlg = (CTrsControlS64*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S64);
				if (pDlg != NULL){
					pDlg->ShowWindow(SW_SHOW);
				}
				else
				{
					pDlg = new CTrsControlS64();
					if (pDlg != NULL)
					{
						if (pDlg->Create(IDD_DLG_TRSCTRL_S64, (CMainFrame*)theApp.GetMainWnd()))
						{
							pDlg->ShowWindow(SW_SHOW);
							theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S64);
						}
					}
				}
			}
			else if (sta == eStation_Usuda642)
			{
				CTrsControlS64* pDlg = (CTrsControlS64*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S64);
				if (pDlg != NULL){
					pDlg->ShowWindow(SW_SHOW);
				}
				else
				{
					pDlg = new CTrsControlS64();
					if (pDlg != NULL)
					{
						if (pDlg->Create(IDD_DLG_TRSCTRL_S64, (CMainFrame*)theApp.GetMainWnd()))
						{
							pDlg->ShowWindow(SW_SHOW);
							theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S64);
						}
					}
				}
			}
			else if (sta == eStation_Uchinoura34)
			{
				CTrsControlS34* pDlg = (CTrsControlS34*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S34);
				if (pDlg != NULL){
					pDlg->ShowWindow(SW_SHOW);
				}
				else
				{
					pDlg = new CTrsControlS34();
					if (pDlg != NULL)
					{
						if (pDlg->Create(IDD_DLG_TRSCTRL_S34, (CMainFrame*)theApp.GetMainWnd()))
						{
							pDlg->ShowWindow(SW_SHOW);
							theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S34);
						}
					}
				}
			}
			else if (sta == eStation_Uchinoura20)
			{
				CTrsControlS20* pDlg20 = (CTrsControlS20*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S20);
				if (pDlg20 != NULL){
					pDlg20->ShowWindow(SW_SHOW);
				}
				else
				{
					pDlg20 = new CTrsControlS20();
					if (pDlg20 != NULL)
					{
						if (pDlg20->Create(IDD_DLG_TRSCTRL_S20, (CMainFrame*)theApp.GetMainWnd()))
						{
							pDlg20->ShowWindow(SW_SHOW);
							theApp.AddWindowHandle((LPARAM)pDlg20, IDD_DLG_TRSCTRL_S20);
						}
					}
				}
			}
		}
	}
	else
	{
		// �p�X�O�Ȃ̂ŁA���M�����ʑI���E�B���h�E��\��
		BOOL bSelect = FALSE;
		UINT type = CTrsControl::TYPE_NA;
		if (sta == eStation_Usuda64 || sta == eStation_Usuda642)
		{
			type = CTrsControl::TYPE_64;
			bSelect = TRUE;
		}
		else if (sta == eStation_Uchinoura34)
		{
			type = CTrsControl::TYPE_34;
			bSelect = TRUE;
		}
		else if (sta == eStation_Uchinoura20)
		{
			// 20m �� S�ё��M�����ʂ�\������B
			CTrsControlS20* pDlg20 = (CTrsControlS20*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S20);
			if (pDlg20 != NULL){
				pDlg20->ShowWindow(SW_SHOW);
			}
			else
			{
				pDlg20 = new CTrsControlS20();
				if (pDlg20 != NULL)
				{
					if (pDlg20->Create(IDD_DLG_TRSCTRL_S20, (CMainFrame*)theApp.GetMainWnd()))
					{
						pDlg20->ShowWindow(SW_SHOW);
						theApp.AddWindowHandle((LPARAM)pDlg20, IDD_DLG_TRSCTRL_S20);
					}
				}
			}
		}

		// �I���_�C�A���O�\��
		if (bSelect == TRUE)
		{
			CTrsControl* pDlg = (CTrsControl*)theApp.GetWindowHandle(IDD_DLG_TRSCONTROL);
			if (pDlg != NULL){
				pDlg->SetSelectType(type);
				pDlg->ShowWindow(SW_SHOW);
				return;
			}

			pDlg = new CTrsControl();
			if (pDlg != NULL)
			{
				if (pDlg->Create(IDD_DLG_TRSCONTROL, (CMainFrame*)theApp.GetMainWnd()))
				{
					pDlg->SetSelectType(type);
					pDlg->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCONTROL);
				}
			}
		}
	}
}
