/*============================================================================*/
/*! TrsControlMng.cpp

-# 送信制御マネージャークラス
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

-# 送信制御画面表示

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControlMng::DisplayTrsControl()
{
	UINT sta = theApp.GetSelectStation();
	if (sta == eStation_Usuda642)
	{
		// 54mの場合
		// Ｘ-X帯TCR送信制御画面(54m)
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
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス

	CSatelliteData& sd = theApp.GetSatelliteData();

	// パス進行状況確認
	memset(sz_status, (char)NULL, sizeof(sz_status));

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_PASS_INFO_NO));
	if (obs_adr != NULL)
	{
		strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlMng"), _T("DisplayTrsControl"), _T("CTRL.PASS_INFO"), CString(sz_status), nLogEx::info);
	}

	// CHG 2017.08.28 >>>
	//if ((strcmp(sz_status, CT_PASS_INFO_INIT) == 0) 			// 初期設定
	//	|| (strcmp(sz_status, CT_PASS_INFO_1WAY) == 0) 			// １ＷＡＹ捕捉中
	//	|| (strcmp(sz_status, CT_PASS_INFO_2WAY) == 0)			// ２ＷＡＹ捕捉中
	//	|| (strcmp(sz_status, CT_PASS_INFO_PASS_START1) == 0)	// パス開始(1WAY)
	//	|| (strcmp(sz_status, CT_PASS_INFO_PASS_START2) == 0)	// パス開始(2WAY)
	//	|| (strcmp(sz_status, CT_PASS_INFO_POL) == 0)			// 天頂通過監視領域内
	//	|| (strcmp(sz_status, CT_PASS_INFO_POSN) == 0)			// ANT POSN MODE 移行角検出
	//	|| (strcmp(sz_status, CT_PASS_INFO_EXTEND) == 0)		// パス延長
	//	)
	CStringA status = CStringA(sz_status);
	status.TrimLeft();
	status.TrimRight();
	if (status == CStringA(CT_PASS_INFO_INIT) ||			// 初期設定
		status == CStringA(CT_PASS_INFO_1WAY) ||			// １ＷＡＹ捕捉中
		status == CStringA(CT_PASS_INFO_2WAY) ||			// ２ＷＡＹ捕捉中
		status == CStringA(CT_PASS_INFO_PASS_START1) ||		// パス開始(1WAY)
		status == CStringA(CT_PASS_INFO_PASS_START2) ||		// パス開始(2WAY)
		status == CStringA(CT_PASS_INFO_POL) ||				// 天頂通過監視領域内
		status == CStringA(CT_PASS_INFO_POSN) ||			// ANT POSN MODE 移行角検出
		status == CStringA(CT_PASS_INFO_EXTEND)				// パス延長
		)
// CHG 2017.08.28 <<<
	{
		// パス中なので使用帯域の送信制御画面を表示する
		memset(sz_status, (char)NULL, sizeof(sz_status));

		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_X_TX_EXIST));
		if (obs_adr != NULL)
		{
			strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
			CLogTraceEx::Write(_T(""), _T("CTrsControlMng"), _T("DisplayTrsControl"), _T("CTRL.CTRL.X_TX_EXIST"), CString(sz_status), nLogEx::info);
		}

		// Ｘ帯送信設備を使用するパスか
		if (strcmp(sz_status, "あり") == 0)
		{
			// Ｘ帯送信設備
			memset(sz_status, (char)NULL, sizeof(sz_status));
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(XIFSW_TCRMOD_SEL));
			if (obs_adr != NULL)
			{
				strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
				CLogTraceEx::Write(_T(""), _T("CTrsControlMng"), _T("DisplayTrsControl"), _T("X-IFSW.MOD_TCRMOD_SE"), CString(sz_status), nLogEx::info);
			}

			if (strcmp(sz_status, "TCRMOD") == 0)
			{
				// X帯送信制御画面
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
				// Ｘ-X帯TCR送信制御画面
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
			// S帯送信制御画面
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
		// パス外なので、送信制御画面選択ウィンドウを表示
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
			// 20m は S帯送信制御画面を表示する。
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

		// 選択ダイアログ表示
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
