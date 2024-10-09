/*============================================================================*/
/*! OperationPlan_54.cpp

-# 計画立案画面 (54m)
*/
/*============================================================================*/
// OperationPlan_54.cpp : 実装ファイル

#include "stdafx.h"
#include "StationControl.h"
#include "OperationPlan.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "HookMsgBox.h"
#include "..\Utility\SshShell.h"
#include <locale.h>

#define PASS_BASE_TIME54	52	// パス開始からの基本時刻（分）
#define ANT_TIME54			11
#define ANT_TIME			2

#define PASS_BASE_TIME54_NAME	_T("PASS_BASE_TIME")
#define ANT_TIME54_NAME			_T("ANT_TIME")
#define NON_ANT_TIME_NAME		_T("NON_ANT_TIME")

/*============================================================================*/
// レディネス時刻算出 (54m)
/*============================================================================*/
void COperationPlan::CalcRdyTime54()
{
	CSatelliteData& sd = theApp.GetSatelliteData();
	m_IdxEqsat = GetIndexEqSatDBData(m_SatelliteName);

	long tPassBaseTime = GetPlnTime54(PASS_BASE_TIME54_NAME);
	if (tPassBaseTime < 0)
	{
		tPassBaseTime = PASS_BASE_TIME54;
	}
	long tAntTime54 = GetPlnTime54(ANT_TIME54_NAME);
	if (tAntTime54 < 0)
	{
		tAntTime54 = ANT_TIME54;
	}
	long tNonAntTime = GetPlnTime54(NON_ANT_TIME_NAME);
	if (tNonAntTime < 0)
	{
		tNonAntTime = ANT_TIME;
	}

	if (m_bAntStandby)
	{
		// ANT待ち受け指向あり
		m_PlanData.l_ant_time = tAntTime54;

		// レディネスチェック開始時刻
		m_PlanData.l_rdy_time = tPassBaseTime;
	}
	else
	{
		// ANT待ち受け指向なし
		m_PlanData.l_ant_time = tNonAntTime;

		// レディネスチェック開始時刻
		m_PlanData.l_rdy_time = tPassBaseTime - (tAntTime54 - tNonAntTime);
	}
}

/*============================================================================*/
// 計画雛形ファイルを読み込む (54m)
/*============================================================================*/
int COperationPlan::ReadPlanFormFile54(CString sz_satname, CString sz_passno, stPlandata& stpd)
{
	FILE *fp_smpl;
	CString sz_s_file;

	TCHAR sz_buff[512];
	TCHAR sz_time[50];

	long l_ncount;

	long i;
	long j;
	long k;
	long l;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, sizeof(sz_ele));

	CSatelliteData& sd = theApp.GetSatelliteData();

	CString filename = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname);

	stpd.l_mod_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_tx_mode = 0;
	stpd.l_route_mode = 0;
	stpd.l_term_ant = 0;
	stpd.l_offset = 0;
	stpd.l_auto_acq = 0;
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.l_delay_sel = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.l_pl_sel = 0;
	stpd.l_range_band = 0;
	stpd.l_acq_route = 0;
	stpd.sz_priority = _T("");

	/*-------------------------------------------------------------------------*/
	/*-----   雛形ファイル読込処理                                        -----*/
	/*-------------------------------------------------------------------------*/
	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイルを開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	char sz_line[512];

	/*---------- コメント用計画内容ファイル出力処理 ----------*/
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));

		if (sz_buff[0] == '#')
		{
			continue;
		}

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; k++)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/*----- X-TX 送信出力選択処理 -----*/
		if (sz_c_ele54[32] == CString(sz_ele[0]))	//GREAT SSPA
		{
			for (i = 0; sz_tx_pwr_sel54[i] != _T(""); ++i)
			{
				if (sz_tx_pwr_sel54[i] == CString(sz_ele[1]))
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
		}

		/*----- Ｘ帯変調装置選択処理 -----*/
		if (sz_c_ele54[35] == CString(sz_ele[0]))
		{
			for (i = 0; sz_mod_sel[i] != _T(""); i++) 
			{
				if (sz_mod_sel[i] == CString(sz_ele[1]))
				{
					stpd.l_mod_sel = i;
					break;
				}
			}
		}

		/* 送信出力がX-TX */
		if (sz_c_ele54[50] == CString(sz_ele[0]))
		{
			stpd.l_tx_mode = 1;
		}

		/* X帯送受信ルート判定文字列を探す */
		if (CString(sz_ele[0]).Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}
	}	//while fgets(, , fp_smpl)

	fclose(fp_smpl);
	return 0;
}

/*============================================================================*/
// 計画ファイルを読み込む (54m)
/*============================================================================*/
int COperationPlan::ReadPlanFile54(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw)
{
	FILE	*fp_plan;
	CString sz_p_file;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_work[256];
	TCHAR sz_date[50] = { 0 };
	TCHAR sz_time[50] = { 0 };

	TCHAR sz_work2[256];
	TCHAR sz_work3[256];
	TCHAR sz_work4[256];
	TCHAR sz_work5[256];
	TCHAR sz_work6[256];
	TCHAR sz_work7[256];
	TCHAR sz_work8[256];

	long i;
	long j;
	long l;
	long k;
	long l_rc;

	__int64 d_s_start;
	__int64 d_time;

	CString str;
	CString strTime;

	TCHAR sz_ele[10][100];
	memset(&sz_ele[0], 0, static_cast<unsigned>(_countof(sz_ele)));

	CSatelliteData& sd = theApp.GetSatelliteData();

	CString filename = theApp.GetShareFilePath(eFileType_Plan_Folder, theApp.GetSelectStation());
	if (dw == 1)
		sz_p_file.Format(_T("%senp\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	else
		sz_p_file.Format(_T("%ssttn\\%s\\%s.pln"), (LPCTSTR)filename, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);

	sz_s_start[0] = '\0';

	/*----- ftp file get  "sz_p_file" -----*/
	CString path(sd.m_satinfo[m_IdxEqsat].sz_plan_dir);
	CString file(sz_p_file);

	//	if (GetPlanFile(path, file) == FALSE)
	//	{
	//		return -1;
	//	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイルを開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	stpd.sz_start = _T("");
	stpd.sz_end = _T("");
	stpd.sz_s_bitrate = _T("");
	stpd.sz_x_bitrate = _T("");
	stpd.sz_ka_bitrate = _T("");
	stpd.sz_x_qpsk = _T("");
	stpd.sz_cmd_bit_rate = _T("");
	stpd.sz_x_cmd_bit_rate = _T("");
	stpd.l_cmd_mod = 0;
	stpd.l_rng_mod = 0;
	stpd.sz_rng_band = _T("");
	stpd.sz_rng_start = _T("");
	stpd.sz_rng_end = _T("");
	stpd.l_rdychk = 0;
	stpd.l_init = 0;
	stpd.l_auto_acq = 0;
	stpd.l_offset = 0;
	stpd.l_a_flag = 0;
	stpd.l_l_flag = 0;
	stpd.l_term_ant = 1;
	stpd.l_tx_ramp = 0;
	stpd.l_rx_ramp = 0;
	stpd.l_acq_route = 0;
	stpd.l_pl_sel = 0;
	stpd.l_top_mon = 0;
	stpd.l_ent_angle = 0;
	stpd.l_ext_angle = 0;
	stpd.sz_level_s = _T("");
	stpd.sz_level_x = _T("");
	stpd.sz_ant_s = _T("");
	stpd.sz_ant_x = _T("");
	stpd.l_delay_sel = 0;
	stpd.l_tx_pwr_sel = 0;
	stpd.l_mod_sel = 0;
	stpd.l_tx_uc_sel = -1;
	stpd.l_antwait_flg = 0;
	stpd.l_cmd_index = 0;
	stpd.l_tlm_index = 0;
	stpd.l_agc_index = 0;
	stpd.l_rng_index = 0;
	stpd.l_range_band = 0;
	stpd.l_ant_stow = 2;
	stpd.sz_cid = _T("");
	stpd.sz_station = _T("");
	stpd.sz_priority = _T("");
	stpd.sz_ope_mode = _T("");
	stpd.l_uplink = 0;
	stpd.sz_uplink_s_timelst.clear();
	stpd.sz_uplink_e_timelst.clear();
	stpd.l_rng = 0;
	stpd.sz_rng_s_timelst.clear();
	stpd.sz_rng_e_timelst.clear();
	stpd.l_cmd = 0;
	stpd.sz_cmd_s_timelst.clear();
	stpd.sz_cmd_e_timelst.clear();
	stpd.l_tlms = 0;
	stpd.sz_tlms_s_timelst.clear();
	stpd.sz_tlms_e_timelst.clear();
	stpd.l_tlmx = 0;
	stpd.sz_tlmx_s_timelst.clear();
	stpd.sz_tlmx_e_timelst.clear();
	stpd.l_tlmka = 0;
	stpd.sz_tlmka_s_timelst.clear();
	stpd.sz_tlmka_e_timelst.clear();

	__int64 d_tm;

	d_s_start = 0;
	d_time = 0;

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_plan) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				swscanf_s(sz_buff, _T("%s%s%s%s%s%s%s%s"),
					sz_work, static_cast<unsigned>(_countof(sz_work)),
					sz_work2, static_cast<unsigned>(_countof(sz_work2)),
					sz_work3, static_cast<unsigned>(_countof(sz_work3)),
					sz_work4, static_cast<unsigned>(_countof(sz_work4)),
					sz_work5, static_cast<unsigned>(_countof(sz_work5)),
					sz_work6, static_cast<unsigned>(_countof(sz_work6)),
					sz_work7, static_cast<unsigned>(_countof(sz_work7)),
					sz_work8, static_cast<unsigned>(_countof(sz_work8)));
			}

			l_rc = swscanf_s(&sz_buff[1], _T("%s%s%s%s%s%s%s%s%s%s"),
				sz_ele[0], static_cast<unsigned>(_countof(sz_ele[0])),
				sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])),
				sz_ele[2], static_cast<unsigned>(_countof(sz_ele[2])),
				sz_ele[3], static_cast<unsigned>(_countof(sz_ele[3])),
				sz_ele[4], static_cast<unsigned>(_countof(sz_ele[4])),
				sz_ele[5], static_cast<unsigned>(_countof(sz_ele[5])),
				sz_ele[6], static_cast<unsigned>(_countof(sz_ele[6])),
				sz_ele[7], static_cast<unsigned>(_countof(sz_ele[7])),
				sz_ele[8], static_cast<unsigned>(_countof(sz_ele[8])),
				sz_ele[9], static_cast<unsigned>(_countof(sz_ele[9])));

			for (i = 0; i < l_rc; ++i)
			{
				str = CString(sz_ele[i]);
				/*---------- 初期値 チェック ----------*/
				if (str.Find(sz_c_ele54[2]) >= 0)
				{
					j = sz_c_ele54[2].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_init = k;
							break;
						}
					}
				}

				/*---------- CMD MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele54[6]) >= 0)
				{
					j = sz_c_ele54[6].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_cmd_mod = k;
							break;
						}
					}
				}
				/*---------- RNG MOD の AUTO/MANチェック ----------*/
				if (str.Find(sz_c_ele54[7]) >= 0)
				{
					j = sz_c_ele54[7].GetLength() + 1;
					for (k = 0; sz_auto[k] != _T(""); k++)
					{
						if (CString(&sz_ele[i][j]) == sz_auto[k])
						{
							stpd.l_rng_mod = k;
							break;
						}
					}
				}
				/*---------- RNG BAND の 取り込み ----------*/
				if (str.Find(sz_c_ele54[8]) >= 0)
				{
					j = sz_c_ele54[8].GetLength() + 1;
					stpd.sz_rng_band = CString(&sz_ele[i][j]);
				}
				/*---------- RNG START の 取り込み ----------*/
				if (str.Find(sz_c_ele54[9]) >= 0)
				{
					j = sz_c_ele54[9].GetLength() + 1;
					stpd.sz_rng_start = CString(&sz_ele[i][j]);
				}
				/*---------- RNG END の 取り込み ----------*/
				if (str.Find(sz_c_ele54[10]) >= 0)
				{
					j = sz_c_ele54[10].GetLength() + 1;
					stpd.sz_rng_end = CString(&sz_ele[i][j]);
				}
				/*---------- 時刻オフセットへの追従 ----------*/
				if (str.Find(sz_c_ele54[11]) >= 0)
				{
					j = sz_c_ele54[11].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_offset = k;
							break;
						}
					}
				}
				/*---------- AOS FLAG ----------*/
				if (str.Find(sz_c_ele54[14]) >= 0)
				{
					j = sz_c_ele54[14].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_a_flag = k;
							break;
						}
					}
				}
				/*---------- LOS FLAG ----------*/
				if (str.Find(sz_c_ele54[15]) >= 0)
				{
					j = sz_c_ele54[15].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_l_flag = k;
							break;
						}
					}
				}
				/*---------- 自動捕捉の有無チェック ----------*/
				if (str.Find(sz_c_ele54[16]) >= 0)
				{
					j = sz_c_ele54[16].GetLength() + 1;
					for (k = 0; sz_on[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on[k])
						{
							stpd.l_auto_acq = k;
							break;
						}
					}
				}
				/*---------- パス終了時のＡＮＴモード ----------*/
				if (str.Find(sz_c_ele54[18]) >= 0)
				{
					j = sz_c_ele54[18].GetLength() + 1;
					for (k = 0; sz_term_ant[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_term_ant[k])
						{
							stpd.l_term_ant = k;
							break;
						}
					}
				}
				/*---------- 送信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele54[19]) >= 0)
				{
					j = sz_c_ele54[19].GetLength() + 1;
					for (k = 0; sz_tx_ramp[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_tx_ramp[k])
						{
							stpd.l_tx_ramp = k;
							break;
						}
					}
				}
				/*---------- 受信設備ランプ制御有無 ----------*/
				if (str.Find(sz_c_ele54[20]) >= 0)
				{
					j = sz_c_ele54[20].GetLength() + 1;
					for (k = 0; sz_rx_ramp54[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_rx_ramp54[k])
						{
							stpd.l_rx_ramp = k;
							break;
						}
					}
				}
				/*---------- 補足ルート ----------*/
				if (str.Find(sz_c_ele54[21]) >= 0)
				{
					j = sz_c_ele54[21].GetLength() + 1;
					stpd.l_acq_route = _wtoi(&sz_ele[i][j]);
					stpd.l_acq_route -= 1;	/* 配列番号に変換 */
				}
				/*---------- ACQ/MAIN判定有無 ----------*/
				if (str.Find(sz_c_ele54[22]) >= 0)
				{
					j = sz_c_ele54[22].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_pl_sel = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視有無 ----------*/
				if (str.Find(sz_c_ele54[23]) >= 0)
				{
					j = sz_c_ele54[23].GetLength() + 1;
					for (k = 0; sz_on_off[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_on_off[k])
						{
							stpd.l_top_mon = k;
							break;
						}
					}
				}
				/*---------- 天頂通過監視入角 ----------*/
				if (str.Find(sz_c_ele54[24]) >= 0)
				{
					j = sz_c_ele54[24].GetLength() + 1;
					stpd.l_ent_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 天頂通過監視出角 ----------*/
				if (str.Find(sz_c_ele54[25]) >= 0)
				{
					j = sz_c_ele54[25].GetLength() + 1;
					stpd.l_ext_angle = _wtoi(&sz_ele[i][j]);
				}
				/*---------- 再生測距対応　局内ディレイ値選択 ----------*/
				if (str.Find(sz_c_ele54[33]) >= 0)
				{
					j = sz_c_ele54[33].GetLength() + 1;
					for (k = 0; sz_delay_sel[k] != _T(""); ++k) {
						if (CString(&sz_ele[i][j]) == sz_delay_sel[k])
						{
							stpd.l_delay_sel = k;
							break;
						}
					}
				}

				/*---------- ANT天頂指向有無 ----------*/
				if (str.Find(sz_c_ele54[46]) >= 0)
				{
					j = sz_c_ele54[46].GetLength() + 1;
					for (k = 0; sz_ant_stow[k] != _T(""); ++k)
					{
						if (CString(&sz_ele[i][j]) == sz_ant_stow[k])
						{
							stpd.l_ant_stow = k;
							break;
						}
					}
				}

				/*---------- CID ----------*/
				if (str.Find(sz_c_ele54[47]) >= 0)
				{
					j = sz_c_ele54[47].GetLength() + 1;
					stpd.sz_cid = CString(&sz_ele[i][j]);
				}

				/*---------- 局名 ----------*/
				if (str.Find(sz_c_ele54[48]) >= 0)
				{
					j = sz_c_ele54[48].GetLength() + 1;
					stpd.sz_station = CString(&sz_ele[i][j]);
				}

				/*---------- 主従局 ----------*/
				if (str.Find(sz_c_ele54[49]) >= 0)
				{
					j = sz_c_ele54[49].GetLength() + 1;
					stpd.sz_priority = CString(&sz_ele[i][j]);
				}

				/*---------- オペレーションモード ----------*/
				if (str.Find(sz_c_ele54[50]) >= 0)
				{
					j = sz_c_ele54[50].GetLength() + 1;
					stpd.sz_ope_mode = CString(&sz_ele[i][j]);
				}

			}
			continue;
		}

		swscanf_s(sz_buff, _T("%s%s%s"),
			sz_date, static_cast<unsigned>(_countof(sz_date)),
			sz_time, static_cast<unsigned>(_countof(sz_time)),
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		j = 0;
		l = 0;
		for (k = 0; sz_work[k] != '\0'; ++k)
		{
			if (sz_work[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_work[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
				break;
			}
			else if (sz_work[k] != ' ')
			{
				sz_ele[j][l] = sz_work[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		/* X帯送受信ルート判定文字列を探す */
		str = CString(sz_ele[0]);
		if (str.Find(X_ROUTE_DEF_TXT) >= 0)
		{
			stpd.l_route_mode = 1;
		}

		for (k = 0; sz_c_ele54[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{	/* 判定ケースを追加 */
				if (str.Find(sz_c_ele54[k]) == 0)
				{
					break;
				}
			}
			else
			{
				if (str == sz_c_ele54[k])
				{
					break;
				}
			}
		}

		if (sz_c_ele54[k] == _T(""))
		{
			continue;
		}

		swprintf_s(sz_work, static_cast<unsigned>(_countof(sz_work)), _T("%s %s"), sz_date, sz_time);
		if (d_s_start == 0.0 || d_s_start > d_time)
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);
			d_s_start = d_time;
		}

		switch (k)
		{
			/*----- レディネスチェック処理 -----*/
		case 0:
			(stpd.l_rdychk) |= 0x01;
			break;

		case 1:
			(stpd.l_rdychk) |= 0x02;
			break;

			/*----- 初期設定処理 -----*/
		case 2:
			stpd.l_init = 1;
			break;

			/*----- ビットレートKa -----*/
		case 3:
			stpd.sz_ka_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸ -----*/
		case 4:
			stpd.sz_x_bitrate = CString(sz_ele[1]);
			break;

			/*----- ビットレートＸＱＰＳＫ -----*/
		case 17:
			stpd.sz_x_qpsk = CString(sz_ele[1]);
			break;

			/*----- S/X コマンドビットレート -----*/
		case 5:
			stpd.sz_cmd_bit_rate = CString(sz_ele[1]);
			break;

			/*----- X コマンドビットレート -----*/
		case 36:
		case 43:
			stpd.sz_x_cmd_bit_rate = CString(sz_ele[1]);
			break;


			/*----- 開始時刻処理 -----*/
		case 12:
			stpd.sz_start.Format(_T("%s %s"), sz_date, sz_time);

			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), _T(" "));
			wcscat_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_time);

			TCHAR sz_lwork[50];
			__int64 d_work;
			__int64 l_tm;
			swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s.0000"), sz_s_start);

			d_work = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
			l_tm = d_work - d_tm;
			stpd.l_ant_time = (long)(l_tm / 60);
			break;

			/*----- 終了時刻処理 -----*/
		case 13:
			stpd.sz_end.Format(_T("%s %s"), sz_date, sz_time);
			break;

			/*----- Ｓ帯衛星送信情報処理 -----*/
		case 28:
			{
				/* 送信レベルとアンテナ選択情報に分ける */
				CString str = CString(sz_ele[1]);
				int idx = str.Find(_T("-"));
				stpd.sz_level_s = str.Mid(0, idx);
				stpd.sz_ant_s = str.Mid(idx+1);
			}
			break;

			/*----- Ｘ帯衛星送信情報処理 -----*/
		case 29:
			{
				/* 送信レベルとアンテナ選択情報に分ける */
				CString str = CString(sz_ele[1]);
				int idx = str.Find(_T("-"));
				stpd.sz_level_x = str.Mid(0, idx);
				stpd.sz_ant_x = str.Mid(idx+1);
			}
			break;

			/*----- ４ＷＡＹ運用有無情報処理 -----*/
		case 30:
			if (wcsncmp(sz_ele[1], CTRL_4WAY, wcslen(CTRL_4WAY)) == 0) 
			{
				stpd.l_selene_4way = 0;
			}
			else if (wcsncmp(sz_ele[1], _T("ANT"), wcslen(_T("ANT"))) == 0) 
			{
				TCHAR sz_lwork[50];
				swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s %s.0000"), sz_date, sz_time);
				d_tm = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
			}
			break;

			/*----- X-TX 送信出力選択処理 -----*//* 2006-11-15 追加 */
		case 32:
			for (i = 0; sz_tx_pwr_sel54[i] != (char*)NULL; i++) 
			{
				if (wcscmp(sz_tx_pwr_sel54[i], sz_ele[1]) == 0) 
				{
					stpd.l_tx_pwr_sel = i;
					break;
				}
			}
			break;

			/*----- Ｘ帯変調装置選択処理 -----*//* 2009-02-06 追加 */
		case 35:
			for (i = 0; sz_mod_sel[i] != (char*)NULL; i++) 
			{
				if (wcscmp(sz_mod_sel[i], sz_ele[1]) == 0) 
				{
					stpd.l_mod_sel = i;
					break;
				}
			}
			break;

		case 37: //レディネスチェック（AGC)
			if( (CString(sz_ele[1]) == STR_DSN_MODEM_X) ||
				(CString(sz_ele[1]) == STR_DSN_MODEM_KA) )
			{
				stpd.l_agc_index = 1;
			}

			break;

		case 38: //レディネスチェック（RNG)
			if( (CString(sz_ele[1]) == STR_DXN)  ||
				(CString(sz_ele[1]) == STR_DXR)  ||
				(CString(sz_ele[1]) == STR_DKAN) ||
				(CString(sz_ele[1]) == STR_DKAR) )
			{
				stpd.l_rng_index = 1;
			}

			break;

		case 39: //レディネスチェック（テレメトリ伝送）
			if( (CString(sz_ele[1]) == STR_X) ||
				(CString(sz_ele[1]) == STR_KA) )
			{
				stpd.l_tlm_index = 1;
			}
			break;

		case 40: //レディネスチェック（コマンド伝送）
			if(CString(sz_ele[1]) == STR_1)
			{
				stpd.l_cmd_index = 1;
			}
			break;


		case 41: //アンテナ待ち受け指向
			stpd.l_antwait_flg = 1;
			break;


		case 53: // UP START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_s_timelst.push_back(CString(strTime));
			break;

		case 54: // UP END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_uplink_e_timelst.push_back(CString(strTime));
			break;

		case 55: // RNG_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_s_timelst.push_back(CString(strTime));

			// 種別
			stpd.sz_rng_kindlst.push_back(CString(sz_ele[1]));
			break;

		case 56: // RNG_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_rng_e_timelst.push_back(CString(strTime));
			break;

		case 57: // CMD_START
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_s_timelst.push_back(CString(strTime));
			break;
			
		case 58: // CMD_END
			strTime.Format(_T("%s %s"), sz_date, sz_time);
			stpd.sz_cmd_e_timelst.push_back(CString(strTime));
			break;

		case 59: // TLM_START
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_s_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_s_timelst.push_back(CString(strTime));
			}
			break;

		case 60: // TLM_END
			if (CString(sz_ele[1]) == TLM_S)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlms_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_X)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmx_e_timelst.push_back(CString(strTime));
			}
			else if (CString(sz_ele[1]) == TLM_KA)
			{
				strTime.Format(_T("%s %s"), sz_date, sz_time);
				stpd.sz_tlmka_e_timelst.push_back(CString(strTime));
			}
			break;


		}
	}

	fclose(fp_plan);

	return 0;
}

/*============================================================================*/
// 計画ファイルを書き込む (54m)
/*============================================================================*/
int	COperationPlan::WritePlanFile54(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist)
{
	FILE *fp_smpl;
	FILE *fp_plan;
	CString sz_s_file;
	CString sz_p_folder;
	CString sz_p_file;

	TCHAR sz_s_start[50];
	TCHAR sz_buff[512];
	TCHAR sz_time[50];
	TCHAR sz_ele[10][100];
	TCHAR sz_work[256];
	TCHAR sz_date[50];
	TCHAR *chp_pos;

	long j;
	long k;
	long l;
	long m;
	long l_ncount;

	long l_hour;
	long l_min;
	long l_sec;

	__int64 d_ntime[10];
	__int64 d_time;
	__int64 d_start;

	int count = 0;
	int t_rdy = 0;

	memset(&sz_ele[0], 0, sizeof(sz_ele));

	/*----- 雛形＆計画ファイルのパス名作成 -----*/
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString filenamep = theApp.GetShareFilePath(eFileType_PlanOut_Folder, theApp.GetSelectStation());
	sz_p_file.Format(_T("%s%s\\%s.pln"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname, (LPCTSTR)sz_passno);
	sz_p_folder.Format(_T("%s%s"), (LPCTSTR)filenamep, (LPCTSTR)sz_satname.MakeLower());
	if (PathFileExists(sz_p_folder) == FALSE)
	{
		CreateDirectory(sz_p_folder, NULL);
	}

	CString filenames = theApp.GetShareFilePath(eFileType_Form_Folder, theApp.GetSelectStation());
	sz_s_file.Format(_T("%s%s.pln"), (LPCTSTR)filenames, (LPCTSTR)sz_satname);

	sz_s_start[0] = '\0';

	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	/*---------- 開始時刻コメント用計画内容ファイル出力処理 ----------*/
	for (k = 0; k < 10; k++)
	{
		d_ntime[k] = 0;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	char sz_line[512];
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ')
			{
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',') {
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			continue;
		}

		for (k = 0; sz_c_ele54[k] != _T(""); ++k)
		{
			if (CString(sz_ele[0]).Find(sz_c_ele54[k]) == 0)
			{
				break;
			}
		}

		m = swscanf_s(sz_time, _T("%d:%d:%d%s"),
			&l_hour, &l_min, &l_sec,
			sz_work, static_cast<unsigned>(_countof(sz_work)));

		if (m != 3)
		{
			if (sz_c_ele54[k] == _T(""))
			{
				continue;
			}
			fclose(fp_smpl);
			return -1;
		}
		d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
		if (sz_time[0] == '-')
		{
			d_time = -d_time;
		}
		if (k < 2)
		{
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}
		/*----- 初期設定処理 -----*/
		if (k == 2)
		{
			if (stpd.l_init == 0)
			{
				continue;
			}
		}
		if (l_ncount >= 0)
		{
			if (d_ntime[l_ncount] > d_time)
			{
				d_ntime[l_ncount] = d_time;
			}
		}
	}

	TCHAR sz_lwork[50];
	swprintf_s(sz_lwork, static_cast<unsigned>(_countof(sz_lwork)), _T("%s"), (LPCTSTR)stpd.sz_start);
	d_start = CSatelliteData::StrTimeToCTime(CString(sz_lwork)).GetTime();
	for (k = 0; k < 9; ++k)
	{
		d_ntime[k + 1] += d_ntime[k];
	}

	fclose(fp_smpl);


	if (_wfopen_s(&fp_smpl, sz_s_file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("計画雛形ファイル開くことができません。\n%s"), (LPCTSTR)sz_s_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	if (_wfopen_s(&fp_plan, sz_p_file, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("計画ファイル開くことができません。\n%s"), (LPCTSTR)sz_p_file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		fclose(fp_smpl);
		return -1;
	}

	/*---------- コメント用計画内容ファイル出力処理 ----------*/

	TCHAR szwline[2048];
	CStringA cbuf;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp_smpl) != NULL)
	{
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_line));
		if (sz_buff[0] == '#')
		{
			if (wcsncmp(sz_buff, SOS_HEADER_ID, wcslen(SOS_HEADER_ID)) == 0)
			{
				// UTC
				SYSTEMTIME timeSys;
				GetSystemTime(&timeSys);
				CTime time = CTime(timeSys);

				CTime timegm(CTime::GetCurrentTime());
				CString str_gm = timegm.Format(_T("%Y-%m-%d %H:%M:%S"));
				CString str_date = time.Format(_T("%Y-%m-%d %H:%M:%S"));

				sz_date[19] = '\0';
				wsprintf(szwline, _T("%s PLAN %s     1 %2d %s %s\n"),
					SOS_HEADER_ID,
					(LPCTSTR)CString(str_date),
					sd.m_satinfo[m_IdxEqsat].l_sat_no,
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)stpd.sz_pred_path);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# 衛星名=%s パス番号=%s RDYCHK=%s\n"),
					(LPCTSTR)CString(sz_satname).MakeUpper(),
					(LPCTSTR)CString(sz_passno),
					(LPCTSTR)sz_rdychk[stpd.l_rdychk]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele54[2], (LPCTSTR)sz_on[stpd.l_init],
					(LPCTSTR)sz_c_ele54[4], (LPCTSTR)stpd.sz_x_bitrate,
					(LPCTSTR)sz_c_ele54[3], (LPCTSTR)stpd.sz_ka_bitrate,
					(LPCTSTR)sz_c_ele54[17], (LPCTSTR)stpd.sz_x_qpsk);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

//				if (stpd.l_mod_sel == 0)
				if (stpd.sz_cmd_bit_rate.GetLength() > 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele54[5], (LPCTSTR)stpd.sz_cmd_bit_rate,
						(LPCTSTR)sz_c_ele54[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele54[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
//				else if (stpd.l_mod_sel == 1)
				else if (stpd.sz_x_cmd_bit_rate.GetLength() > 0)
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele54[36], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele54[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele54[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}
				else
				{
					wsprintf(szwline, _T("# %s=%s %s=%s %s=%s\n"),
						(LPCTSTR)sz_c_ele54[36], (LPCTSTR)stpd.sz_x_cmd_bit_rate,
						(LPCTSTR)sz_c_ele54[12], (LPCTSTR)stpd.sz_start,
						(LPCTSTR)sz_c_ele54[13], (LPCTSTR)stpd.sz_end);
					cbuf = CServerFile::ConvertEuc(szwline);
					fputs(cbuf, fp_plan);
				}

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele54[16], (LPCTSTR)sz_on[stpd.l_auto_acq],
					(LPCTSTR)sz_c_ele54[11], (LPCTSTR)sz_on[stpd.l_offset],
					(LPCTSTR)sz_c_ele54[14], (LPCTSTR)sz_on[stpd.l_a_flag],
					(LPCTSTR)sz_c_ele54[15], (LPCTSTR)sz_on[stpd.l_l_flag],
					(LPCTSTR)sz_c_ele54[18], (LPCTSTR)sz_term_ant[stpd.l_term_ant]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele54[6], (LPCTSTR)sz_auto[stpd.l_cmd_mod],
					(LPCTSTR)sz_c_ele54[7], (LPCTSTR)sz_auto[stpd.l_rng_mod],
					(LPCTSTR)sz_c_ele54[8], (LPCTSTR)stpd.sz_rng_band,
					(LPCTSTR)sz_c_ele54[9], (LPCTSTR)stpd.sz_rng_start,
					(LPCTSTR)sz_c_ele54[10], (LPCTSTR)stpd.sz_rng_end);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d\n"),
					(LPCTSTR)sz_c_ele54[19], (LPCTSTR)sz_tx_ramp[stpd.l_tx_ramp],
					(LPCTSTR)sz_c_ele54[20], (LPCTSTR)sz_rx_ramp54[stpd.l_rx_ramp],
					(LPCTSTR)sz_c_ele54[21], stpd.l_acq_route + 1);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%d %s=%d %s=%s\n"),
					(LPCTSTR)sz_c_ele54[22], (LPCTSTR)sz_on_off[stpd.l_pl_sel],
					(LPCTSTR)sz_c_ele54[23], (LPCTSTR)sz_on_off[stpd.l_top_mon],
					(LPCTSTR)sz_c_ele54[24], stpd.l_ent_angle,
					(LPCTSTR)sz_c_ele54[25], stpd.l_ext_angle,
					(LPCTSTR)sz_c_ele54[33], (LPCTSTR)sz_delay_sel[stpd.l_delay_sel]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s %s=%s %s=%s %s=%s\n"),
					(LPCTSTR)sz_c_ele54[47], (LPCTSTR)stpd.sz_cid,								// CID(CID)
					(LPCTSTR)sz_c_ele54[48], mStationSimpleString[theApp.GetSelectStation()],	// 局名(ST_NAME)
					(LPCTSTR)sz_c_ele54[49], _T("MASTER"),										// 主従局(PRI):MASTER固定
					(LPCTSTR)sz_c_ele54[50], (LPCTSTR)GetOpeMode54());							// 運用モード(OPE_MODE)
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				wsprintf(szwline, _T("# %s=%s\n"),
					(LPCTSTR)sz_c_ele54[61], _T(""));											// Auto
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				continue;
			}
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		/*---------- 計画内容作成処理 ----------*/

		l = 0;
		if (sz_buff[0] == 'N')
		{
			if (sz_buff[1] >= 0x30 && sz_buff[1] <= 0x39)
			{
				l_ncount = sz_buff[1] - 0x30;
				k = 2;
			}
			else
			{
				l_ncount = 0;
				k = 1;
			}
		}
		else
		{
			l_ncount = -1;
			k = 0;
		}
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] != ' ') {
				sz_time[l] = sz_buff[k];
				l++;
			}
			else
			{
				if (l > 0)
				{
					sz_time[l] = '\0';
					break;
				}
			}
		}

		j = 0;
		l = 0;
		sz_ele[2][0] = '\0';
		for (; sz_buff[k] != '\0'; ++k)
		{
			if (sz_buff[k] == ',')
			{
				sz_ele[j][l] = '\0';
				j++;
				l = 0;
			}
			else if (sz_buff[k] == ';')
			{
				sz_ele[j][l] = '\0';
				j++;
				sz_ele[j][l] = '\0';
				l = 0;
				break;
			}
			else if (sz_buff[k] != ' ')
			{
				sz_ele[j][l] = sz_buff[k];
				l++;
			}
		}
		if (j < 2 || j > 4)
		{
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp_plan);
			continue;
		}

		for (k = 0; sz_c_ele54[k] != _T(""); ++k)
		{
			if ((k == 2) || (k == 4) || (k == 35))
			{
				if (CString(sz_ele[0]).Find(sz_c_ele54[k]) == 0)
				{
					if ((k == 4) && (sz_c_ele54[17] == CString(sz_ele[0])))
					{
						continue;
					}
					break;
				}
			}
			else if (k == 27) {
				/* PREDGETを確認する */
				chp_pos = wcschr(sz_ele[0], '.');
				if (chp_pos != (TCHAR *)NULL)
				{
					chp_pos++;
					if (CString(chp_pos).Find(sz_c_ele54[k]) == 0)
					{
						break;
					}
				}
			}
			else
			{
				if (sz_c_ele54[k] == CString(sz_ele[0]))
				{
					break;
				}
			}
		}

		if (k == 12)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_start);
		}
		else if (k == 13)
		{
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)stpd.sz_end);
		}
		else
		{
			m = swscanf_s(sz_time, _T("%d:%d:%d%s"), &l_hour, &l_min, &l_sec, sz_work, static_cast<unsigned>(_countof(sz_work)));
			if (m != 3)
			{
				if (sz_c_ele54[k] == _T(""))
				{
					cbuf = CServerFile::ConvertEuc(sz_buff);
					fputs(cbuf, fp_plan);
					continue;
				}
				fclose(fp_smpl);
				fclose(fp_plan);
				return -1;
			}
			d_time = (l_hour * 3600) + (l_min * 60) + l_sec;
			if (sz_time[0] == '-')
			{
				d_time = -d_time;
			}
			if (l_ncount >= 0)
			{
				d_time = d_ntime[l_ncount];
			}

			// ANT待ち受け指向なし時、2分前以前のイベントは2分とする。
			if (stpd.l_antwait_flg == 0 && d_time < -120)
			{
				d_time = -120;
			}

			d_time += d_start;
			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		if (k < 2)
		{
			/*----- レディネスチェック処理 -----*/
			l = (stpd.l_rdychk >> k) % 2;
			if (l == 0)
			{
				continue;
			}
		}

		switch (k) {
			/*----- 初期設定処理 -----*/
		case 2:
		case 26:	/* <<XUP>> モードチェックを追加 */
			if (stpd.l_init == 0)
			{
				continue;
			}
			break;

			/*----- BITRATE-KA処理 -----*/
		case 3:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_ka_bitrate);
			break;

			/*----- BITRATE-X処理 -----*/
		case 4:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_bitrate);
			break;

			/*----- BITRATE-XQPSK処理 -----*/
		case 17:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_qpsk);
			break;

			/*----- S/X CMD_BITRATE処理 -----*/
		case 5:
			if (stpd.sz_cmd_bit_rate.GetLength() > 0)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_cmd_bit_rate);
			}
			break;

			/*----- X CMD_BITRATE処理 -----*/
		case 36:
		case 43:
			if (stpd.sz_x_cmd_bit_rate.GetLength() > 0)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)stpd.sz_x_cmd_bit_rate);
			}
			break;

			/*----- PREDGET処理 -----*/
		case 27:
			/* Ｘ帯送信機ランプ制御の場合 ランプなし時の処理追加 */
			if (wcsncmp(sz_ele[0], X_TXSYN_TXT, wcslen(X_TXSYN_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp == 1) || (stpd.l_tx_ramp == 2) || (stpd.l_tx_ramp == 3))
				{
					/* 送信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* DSN-MODEM(MOD) ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], DSN_PREDGET_TXT, wcslen(DSN_PREDGET_TXT)) == 0)
			{
				if( (stpd.l_tx_ramp != 0) && (stpd.l_rx_ramp != 0) )
				{
					/* 送/受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* S帯送信機ランプ制御の場合 ランプなし時の処理追加 */
			if (wcsncmp(sz_ele[0], SS_TCRMOD_TXT, wcslen(SS_TCRMOD_TXT)) == 0)
			{
				if ((stpd.l_tx_ramp == 1) || (stpd.l_tx_ramp == 2) || (stpd.l_tx_ramp == 3))
				{
					/* 送信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}
			/* Ｓ帯Ｄ／Ｃ　ＳＹＮ１ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], S_DCSYN_TXT, wcslen(S_DCSYN_TXT)) == 0) 
			{
				if(stpd.l_rx_ramp != 0)
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* Ｘ帯受信機１ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], X_RX1SYN_TXT, wcslen(X_RX1SYN_TXT)) == 0) 
			{
				if(stpd.l_rx_ramp != 0)
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* Ｘ帯受信機２ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], X_RX2SYN_TXT, wcslen(X_RX2SYN_TXT)) == 0)
			{
				if(stpd.l_rx_ramp != 0)
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			/* Ｘ帯高速受信機ランプ制御の場合 */
			if (wcsncmp(sz_ele[0], X_QPSKSYN_TXT, wcslen(X_QPSKSYN_TXT)) == 0)
			{
				if(stpd.l_rx_ramp != 0)
				{
					/* 受信機ランプ制御なしの場合はPREDGETを外す */
					continue;
				}
			}

			break;

			/*----- Ｓ帯衛星送信情報処理 -----*/
		case 28:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s-%s"), _T("H"), _T("A"));
			break;

			/*----- Ｘ帯衛星送信情報処理 -----*/
		case 29:
			swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s-%s"), _T("H"), _T("A"));
			break;

			/*----- ４ＷＡＹ運用有無 -----*/
		case 30:
			if (wcscmp(sz_ele[1], _T("CTRL")) == 0) 
			{
				if (stpd.l_selene_4way == 0) 
				{
					wcscpy_s(sz_ele[1], CTRL_4WAY);
				}
			}
			else if (wcscmp(sz_ele[1], _T("CTRL_OLD")) == 0) 
			{
				if (stpd.l_selene_4way == 0) 
				{
					wcscpy_s(sz_ele[1], static_cast<unsigned>(_countof(sz_s_start)), CTRL_4WAY);
					wcscat_s(sz_ele[1], static_cast<unsigned>(_countof(sz_s_start)), CTRL_OLD);
				}
			}
			else if (wcscmp(sz_ele[1], _T("ANT")) == 0) 
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				if (stpd.l_antwait_flg == 0)
				{
					d_time -= 2;
				}
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			break;

			/*----- X-TX 送信出力選択処理 -----*/
		case 32:
			if (stpd.l_tx_pwr_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_tx_pwr_sel54[stpd.l_tx_pwr_sel]);
			}
			break;

			/*----- 再生型測距使用選択処理 -----*/
		case 34:
			if (stpd.l_delay_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_delay_sel_prm[stpd.l_delay_sel]);
			}
			break;

			/*----- 再生型測距使用選択処理(DSN-MODEM) -----*/
		case 42:
			if (stpd.l_delay_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_delay_sel_dsn_prm[stpd.l_delay_sel]);
			}
			break;

			/*----- Ｘ帯変調装置選択処理 -----*/
		case 35:
			if (stpd.l_mod_sel != -1)
			{
				swprintf_s(sz_ele[1], static_cast<unsigned>(_countof(sz_ele[1])), _T("%s"), (LPCTSTR)sz_mod_sel[stpd.l_mod_sel]);
			}
			break;

		case 37: //レディネスチェック（AGC)
		{
			if (stpd.l_agc_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], STR_DSN_MODEM_X) == 0)
			{
				if (stpd.sz_x_bitrate.IsEmpty())
				{
					// CIDのテレメXビットレートが無いならば、処理を飛ばす
					continue;
				}

				if (sd.m_satinfo[m_IdxEqsat].i_x_dsntcr1 == AGCRX_Y_ID)	//X帯AGC校正の可否
				{
					t_rdy = GetRdyTime54(_T("AGC_X"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if(wcscmp(sz_ele[1], STR_DSN_MODEM_KA) == 0)
			{
				if (stpd.sz_ka_bitrate.IsEmpty())
				{
					// CIDのテレメKaビットレートが無いならば、処理を飛ばす
					continue;
				}

				if (sd.m_satinfo[m_IdxEqsat].i_ka_dsntcr1 == AGCRX_Y_ID)	//Ka帯AGC校正の可否
				{
					t_rdy = GetRdyTime54(_T("AGC_KA"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 38: //レディネスチェック（RNG)
		{
			if (stpd.l_rng_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], STR_DXN) == 0)		//X/X
			{
				if (stpd.sz_x_bitrate.IsEmpty())
				{
					// CIDのテレメXビットレートが無いならば、処理を飛ばす
					continue;
				}
				
				if( (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_X_ID) ||
					((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID) && (m_strRngMesMode.CompareNoCase(_T("通常")) == 0)) ||
					((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_XKA_ID) && (m_strRngMesMode.CompareNoCase(_T("通常")) == 0)) ||
					((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RXKA_ID) && (m_strRngMesMode.CompareNoCase(_T("通常")) == 0)) )
				{
					t_rdy = GetRdyTime54(_T("RNG_XX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], STR_DXR) == 0)	//X/X(再生型)
			{
				if (stpd.sz_x_bitrate.IsEmpty())
				{
					// CIDのテレメXビットレートが無いならば、処理を飛ばす
					continue;
				}
				
				if( ((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RX_ID) && (m_strRngMesMode.CompareNoCase(_T("再生")) == 0)) ||
					((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RXKA_ID) && (m_strRngMesMode.CompareNoCase(_T("再生")) == 0)) )
				{
					t_rdy = GetRdyTime54(_T("RNG_RX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}

			else if (wcscmp(sz_ele[1], STR_DKAN) == 0)	//X/Ka
			{
				if (stpd.sz_ka_bitrate.IsEmpty())
				{
					// CIDのテレメKaビットレートが無いならば、処理を飛ばす
					continue;
				}

				if( (sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_XKA_ID) ||
					((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RXKA_ID) && (m_strRngMesMode.CompareNoCase(_T("通常")) == 0)) )
				{
					t_rdy = GetRdyTime54(_T("RNG_XKA"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], STR_DKAR) == 0)	//X/Ka(再生型)
			{
				if (stpd.sz_ka_bitrate.IsEmpty())
				{
					// CIDのテレメKaビットレートが無いならば、処理を飛ばす
					continue;
				}
				
				if((sd.m_satinfo[m_IdxEqsat].i_rarr_band == RARRBAND_RXKA_ID) && (m_strRngMesMode.CompareNoCase(_T("再生")) == 0))
				{
					t_rdy = GetRdyTime54(_T("RNG_RXKA"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}

			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 39: //レディネスチェック（テレメトリ伝送）
		{
			if (stpd.l_tlm_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], STR_X) == 0)
			{
				if (stpd.sz_x_bitrate.IsEmpty())
				{
					// CIDのテレメXビットレートが無いならば、処理を飛ばす
					continue;
				}

				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_x2 > 0)
				{
					t_rdy = GetRdyTime54(_T("TLM_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else if (wcscmp(sz_ele[1], STR_KA) == 0)
			{
				if (stpd.sz_ka_bitrate.IsEmpty())
				{
					// CIDのテレメKaビットレートが無いならば、処理を飛ばす
					continue;
				}

				if (sd.m_satinfo[m_IdxEqsat].i_bitrate_ka > 0)
				{
					t_rdy = GetRdyTime54(_T("TLM_DKA"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 40: //レディネスチェック（コマンド伝送）
		{
			if (stpd.l_cmd_index == 0)	// OFF
			{
				continue;
			}

			if (wcscmp(sz_ele[1], _T("1")) == 0)
			{
				if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
				{
					t_rdy = GetRdyTime54(_T("CMD_DX"));
					if (t_rdy == -1)
					{
						continue;
					}
					d_time = d_start - ((stpd.l_rdy_time) * 60);
					stpd.l_rdy_time -= t_rdy;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			CTime time(d_time);
			swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
			sz_date[19] = '\0';
		}
		break;

		case 41: //アンテナ待ち受け指向　
		{
			if (stpd.l_antwait_flg == 1)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				d_time = d_time + (5 * 60); //アンテナ待ち受け通知の５分後
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			else
			{
				continue;
			}
		}
		break;

		case 44: //送信予報値制御開始　
			/* DSN-MODEM 送信ランプ制御なしの場合 */
			if (CString(sz_ele[0]) == DSN_TX_AID_ON_TXT)
			{
				if (stpd.l_tx_ramp != 0)
				{
					/* 送信機ランプ制御なしの場合は送信予報値制御開始を外す */
					continue;
				}
			}
			break;

		case 45: //受信予報値制御開始　
			/* DSN-MODEM 受信ランプ制御なしの場合 */
			if (CString(sz_ele[0]) == DSN_RX_AID_ON_TXT)
			{
				if(stpd.l_rx_ramp != 0)
				{
					/* 受信機ランプ制御なしの場合は受信予報値制御開始を外す */
					continue;
				}
			}
			break;

		case 63: // CTRL.PROP_READ
			// アンテナ待ち受け指向なしの場合
			if (stpd.l_antwait_flg == 0)
			{
				d_time = d_start - (stpd.l_ant_time * 60);
				CTime time(d_time);
				swprintf_s(sz_date, static_cast<unsigned>(_countof(sz_date)), _T("%s"), (LPCTSTR)time.Format(_T("%Y-%m-%d %H:%M:%S")));
				sz_date[19] = '\0';
			}
			break;
		}

		if (j == 4)
		{
			wsprintf(szwline, _T("%s %s,%s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2], sz_ele[3]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
		else if ((k == 5) || (k == 35))
		{
			if (wcslen(sz_ele[1]) > 0)
			{
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}
		}
		else if (k == 12) // P-START
		{
			/* P-START*/
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);


			/* DOWN */
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, DOWN_START, sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);

			wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)stpd.sz_end, DOWN_END, sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);

			/* Up Link */
			count = m_UpLinkList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_UpLinkList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, UP_START, sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_UpLinkList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, UP_END, sz_ele[1], sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* CMD */
			CString strCmdType = _T("");
			if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
			{
				strCmdType = _T("X");
			}

			count = m_CmdList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_CmdList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, CMD_START, (LPCTSTR)strCmdType, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_CmdList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, CMD_END, (LPCTSTR)strCmdType, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* RNG */
			count = m_RngList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strRngType = m_RngList.GetItemText(i, 2);
				CString strSTime = m_RngList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, RNG_START, (LPCTSTR)ExchageRngTypePlanFile54(strRngType), sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_RngList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, RNG_END, (LPCTSTR)ExchageRngTypePlanFile54(strRngType), sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM S */
			count = m_TlmSList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmSList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_S, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmSList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_S, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM X */
			count = m_TlmXList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmXList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_X, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmXList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_X, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}

			/* TLM Ka */
			count = m_TlmKaList.GetItemCount();
			for (int i = 0; i < count; ++i)
			{
				CString strSTime = m_TlmKaList.GetItemText(i, 0);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strSTime, TLM_START, TLM_KA, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);

				CString strETime = m_TlmKaList.GetItemText(i, 1);
				wsprintf(szwline, _T("%s %s,%s,%s;\n"), (LPCTSTR)strETime, TLM_END, TLM_KA, sz_ele[2]);
				cbuf = CServerFile::ConvertEuc(szwline);
				fputs(cbuf, fp_plan);
			}
		}
		else if (CString(sz_ele[0]).Find(sz_c_ele54[62]) == 0)	// INIT.CID
		{
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], (LPCTSTR)stpd.sz_cid, sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}
		else
		{
			wsprintf(szwline, _T("%s %s,%s,%s;\n"), sz_date, sz_ele[0], sz_ele[1], sz_ele[2]);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp_plan);
		}

		/* 最初の一件は無条件でコピー */
		if (sz_s_start[0] == '\0')
		{
			wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
		}
		else
		{
			if (wcscmp(sz_s_start, sz_date) > 0)
			{
				wcscpy_s(sz_s_start, static_cast<unsigned>(_countof(sz_s_start)), sz_date);
			}
		}
	}
	fclose(fp_plan);
	fclose(fp_smpl);

	/*---------- 計画一覧リスト作成処理 ----------*/
	TCHAR sz_plan_lst[256];

	// 衛星名16文字幅
	wcscpy_s(sz_work, static_cast<unsigned>(_countof(sz_work)), sz_satname);

	sz_work[16] = '\0';
	swprintf_s(sz_plan_lst, static_cast<unsigned>(_countof(sz_plan_lst)), _T("%-8s %-16s %s %s %s %s %s 1 %1d %5d %-6s %-15s"),
		mStationString[theApp.GetSelectStation()],
		sz_work,
		(LPCTSTR)CString(sz_passno),
		(LPCTSTR)stpd.sz_start,
		(LPCTSTR)stpd.sz_end,
		sz_s_start,
		(LPCTSTR)stpd.sz_pred_path,
		stpd.l_offset,
		stpd.l_offset_val,
		PLAN_GETFORM,
		PLAN_TYPE
		);

	stPlanListData new_listplan;
	CTime cTime;

	new_listplan.strStationName = mStationString[theApp.GetSelectStation()];	// 局名
	new_listplan.strSatelliteName = sz_satname;									// 衛星名
	new_listplan.strPassId = sz_passno;											// パスID
	cTime = CSatelliteData::StrTimeToCTime(stpd.sz_start);
	new_listplan.tPassStart = cTime.GetTime();									// パス開始日時
	cTime = CSatelliteData::StrTimeToCTime(stpd.sz_end);
	new_listplan.tPassEnd = cTime.GetTime();									// パス終了日時
	cTime = CSatelliteData::StrTimeToCTime(CString(sz_s_start));
	new_listplan.tPrePassStart = cTime.GetTime();								// プリパス開始日時
	new_listplan.strPredName = stpd.sz_pred_path;								// 予報値ファイル名
	new_listplan.lValidFlag = 1;												// チェック結果フラグ
	new_listplan.lOfsFlag = stpd.l_offset;										// 時刻オフセット追従条件フラグ
	new_listplan.tOffset = stpd.l_offset_val;									// 時刻オフセット
	new_listplan.strGetFrom = PLAN_GETFORM;										// 取得先
	new_listplan.strPlanType = PLAN_TYPE;										// 計画種別
	new_listplan.strResult = _T("");											// 実行結果

	// 詳細表示及び詳細印刷時は以降処理はしない
	if (bRegist == FALSE)
	{
		return 0;
	}

	/*---------- 計画一覧ファイル更新処理 ----------*/
	// (実際に更新していない。)

	// 計画時刻　＞　現在時刻 
	__int64 d_time2 = CSatelliteData::StrTimeToCTime(CString(sz_s_start)).GetTime() + stpd.l_offset_val;
	// UTC
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	__int64 d_time3 = CTime(timeSys).GetTime();

	CTime time2(d_time2);
	CTime time3(d_time3);
	CString strTime2 = time2.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString strTime3 = time3.Format(_T("%Y-%m-%d %H:%M:%S"));

	if (d_time2 < d_time3 + START_OFFSET)
	{
		CString mes = _T("");
		mes.Format(_T("制御開始時刻までに時間の余裕がないので計画を登録できません。\n\n 制御開始時刻 :%s\n パス開始時刻 :%s\n パス終了時刻 :%s\n\n 現在時刻       :%s\n ガードタイム :%d秒")
			, (LPCTSTR)strTime2, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end, (LPCTSTR)strTime3, (int)START_OFFSET);

// CHG 2017.09.16 >>>
//		AfxMessageBox(mes, MB_OK | MB_ICONQUESTION);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
// CHG 2017.09.16 <<<
		DeleteFile(sz_p_file);
		return 1;
	}

	// 開始時刻　＜　終了時刻　
	if (new_listplan.tPassStart > new_listplan.tPassEnd)
	{
		CString mes = _T("");
		mes.Format(_T("パス終了時刻がパス開始時刻よりも過去のため計画を登録できません。\n\n パス開始時刻 :%s\n パス終了時刻 :%s")
			, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		DeleteFile(sz_p_file);
		return 1;
	}

	// AOS < 開始時刻 , LOS > 終了時刻
	if (m_tAOS.GetTime() > new_listplan.tPassStart || m_tLOS.GetTime() < new_listplan.tPassEnd)
	{
		CString strTAos, strTLos;
		strTAos = m_tAOS.Format(_T("%Y-%m-%d %H:%M:%S"));
		strTLos = m_tLOS.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString mes = _T("");
		mes.Format(_T("計画の時刻が予報値の範囲外のため計画を登録できません。\n\n AOS時刻 :%s\n LOS時刻 :%s\n\n パス開始時刻 :%s\n パス終了時刻 :%s")
			, (LPCTSTR)strTAos, (LPCTSTR)strTLos, (LPCTSTR)stpd.sz_start, (LPCTSTR)stpd.sz_end);

		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		DeleteFile(sz_p_file);
		return 1;
	}

	vector<stPlanListData> v_plan_list2;
	stPlanListData st;

	vector<stPlanListData> &plist = theApp.GetSatelliteData().GetPlanList();
	if ((int)plist.size() > 0)
	{
		long i;
		l = (long)plist.size();
		for (i = 0; i < l; ++i)
		{
			st = plist[i];
			if ((st.strSatelliteName == sz_satname) && (st.strPassId == sz_passno))
			{
				break;
			}
		}
		if (i >= l)
		{
			l++;
		}
	}
	else
	{
		l = 1;
	}

	v_plan_list2 = plist;
	v_plan_list2.push_back(new_listplan);

	// 全時刻をオフセットを考慮したものに変更
	long n = (long)v_plan_list2.size();
	for (long i = 0; i < n; ++i)
	{
		stPlanListData &st = v_plan_list2[i];
		if (st.tOffset != 0)
		{
			// パス開始日時変更
			st.tPassStart += st.tOffset;

			// パス終了日時変更
			st.tPassEnd += st.tOffset;

			// 制御開始日時変更
			st.tPrePassStart += st.tOffset;
		}
	}

	// 計画リストソーティング＆重複チェック処理

	for (long i = (l - 1); i > 0; --i)
	{
		for (long j = 0; j < i; ++j)
		{
			stPlanListData st1 = v_plan_list2[j];
			stPlanListData st2 = v_plan_list2[j + 1];

			if (st1.tPassStart > st2.tPassEnd)
			{
				// データ入れ替え
				if (st1.tPassEnd > st2.tPassStart + START_OFFSET)
				{
					v_plan_list2[j] = st2;
					v_plan_list2[j + 1] = st1;
				}
				else
				{
					// 重複チェック異常
					CString mes1 = _T("");
					CString mes2 = _T("");

					mes1.Format(_T("%sの%sと"), (LPCTSTR)st1.strSatelliteName, (LPCTSTR)st1.strPassId);
					mes2.Format(_T("%sの%sの運用時間が重複しています。"), (LPCTSTR)st2.strSatelliteName, (LPCTSTR)st2.strPassId);
					mes1 += mes2;
					AfxMessageBox(mes1, MB_OK | MB_ICONQUESTION);
					DeleteFile(sz_p_file);
				}
			}
		}
	}

	/*----- ftp file put "sz_p_file"  -----*/
	if (PutPlanFile(sz_satname, sz_passno) == FALSE)
	{
		return -1;
	}

	return 0;
}

/*============================================================================*/
// RNG種別取得 (54m)
/*============================================================================*/
CString	COperationPlan::ExchageRngTypePlanFile54(CString str)
{
	CString type = _T("");

	if (str == _T("XX"))
	{
		type = _T("XX-RNG_NOR");
	}
	else if (str == _T("XX再"))
	{
		type = _T("XX-RNG_REG");
	}
	else if (str == _T("XKa"))
	{
		type = _T("XKa-RNG_NOR");
	}
	else if (str == _T("XKa再"))
	{
		type = _T("XKa-RNG_REG");
	}

	return type;
}

/*============================================================================*/
// 運用モード(OPE_MODE)取得 (54m)
/*============================================================================*/
CString COperationPlan::GetOpeMode54()
{
	CSatelliteData& sd = theApp.GetSatelliteData();
	m_IdxEqsat = GetIndexEqSatDBData(m_SatelliteName);

	CString mode = _T("");

	// TLM X
	if (m_TLMXChkBox)
	{
		if (m_TlmXList.GetItemCount() > 0)
		{
			GetOpeModeAddString(mode, _T("X-TLM"));
		}
	}

	// TLM Ka
	if (m_TLMKaChkBox)
	{
		if (m_TlmKaList.GetItemCount() > 0)
		{
			GetOpeModeAddString(mode, _T("Ka-TLM"));
		}
	}

	// CMD
	if (m_CmdChkBox)
	{
		if (m_CmdList.GetItemCount() > 0)
		{
			if (sd.m_satinfo[m_IdxEqsat].i_x_tx_sel == XTX_Y_ID)
			{
				GetOpeModeAddString(mode, _T("X-CMD"));
			}
			else
			{
				GetOpeModeAddString(mode, _T("S-CMD"));
			}
		}
	}

	// RNG
	if (m_RngChkBox)
	{
		int count = m_RngList.GetItemCount();
		for (int i = 0; i < count; ++i)
		{
			CString strRngType = m_RngList.GetItemText(i, 2);
			GetOpeModeAddString(mode, ExchageRngTypePlanFile54(strRngType));
		}
	}

	return mode;
}

/*============================================================================*/
// レディネスチェック時刻取得 (54m)
/*============================================================================*/
int COperationPlan::GetRdyTime54(CString str)
{
	int time = -1;
	CSatelliteData &sd = theApp.GetSatelliteData();
	CDBAccess &dba = theApp.GetSatelliteData().GetDBAccessCls();

	time = dba.GetRdyChkTimeFile(str);	//レディネスチェック定義ファイルより該当の時間を取得する

	return time;
}

/*============================================================================*/
// データベースから情報を取得する (54m)
/*============================================================================*/
int COperationPlan::data_get_from_db54()
{

	struct satinf_t *satinf_adr;
	CSatelliteData& sd = theApp.GetSatelliteData();

	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	int	i_i;
	int i_j;
	int i_m;

	struct satinf_t *original_adr;
	char *sattbl_adr;

	struct txpwr_inf_t *txpwr_a = NULL;
	struct sattx_t *v_sx_adr[5];
	int l_sx_cnt = 0;
	int l_sx_no = 0;
	struct txpwr_t *v_adr[20];
	long l_pwr_cnt;
	long l_pwr_no;
	struct satant_t *v_adr_sp[20];
	long l_sp_cnt;
	long l_sp_no;

	struct prd_t		*prd_t_adr;
	struct plan_t		*plan_t_adr = NULL;
	struct xtx_t		*xtx_t_adr;		/* Ｘ帯送信設備使用有無情報テーブルアドレス */
	struct xrxsel_t		*xrxsel_t_adr;	/* Ｘ帯受信設備対象情報テーブルアドレス */
	struct rampcnt_t	*rampcnt_t_adr;	/* 周波数ランプ制御許可情報テーブルアドレス */
	struct topangl_t	*topangl_t_adr;	/* 天頂通過監視角度情報テーブルアドレス */
	struct catch_t		*catch_t_adr;	/* 衛星捕捉情報テーブルアドレス */
	struct agcrx54_t	*agcrx54_t_adr;	/* ＡＧＣ校正対象受信機情報テーブルアドレス */
	struct rarrband_t	*rarr_t_adr;	/* ＲＡＲＲ使用帯域情報テーブルアドレス */
	struct rxfqval_t	*rxfqval_adr;	

	char *cmd_br_vec[TBLMAX];
	char *br_x_vec[TBLMAX * 2];
	char *br_ka_vec[TBLMAX * 2];

	int cnt = (int)saNameList.size();
	for (i_j = 0, i_i = 0; i_j < cnt; i_j++)
	{
		/*--------------------------*/
		/* 衛星本体情報アドレス取得 */
		CString satname = saNameList[i_j];
		satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);

		original_adr = satinf_adr;			/* アドレスの退避                 */
		sattbl_adr = (char *)satinf_adr;	/* char型に変換                   */
		satinf_adr = original_adr;			/* 衛星本体情報先頭アドレスセット */

		/*--------------------*/
		/* 衛星本体情報の取得 */
		/*--------------------*/
		strcpy_s(sd.m_satinfo[i_i].sz_satnames, sizeof(sd.m_satinfo[i_i].sz_satnames), satinf_adr->sz_satname);
		sd.m_satinfo[i_i].l_sat_no = satinf_adr->l_satno;                 /* 衛星番号     */
		sd.m_satinfo[i_i].l_use_band = satinf_adr->l_useband;             /* 使用バンド帯 */
		sd.m_satinfo[i_i].l_level = satinf_adr->l_level;                  /* 使用帯域     */

		/*----------------------------------*/
		/* 使用バンド帯関連情報項目数の取得 */
		/*----------------------------------*/
		l_sx_cnt = dba.ud_adr_to_resvec((char*)satinf_adr, SATTX_ID, ENDOFSAT_ID, NULL, (char**)v_sx_adr, 5);
		for (l_sx_no = 0; l_sx_no < l_sx_cnt; l_sx_no++)
		{
			if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
			{
				txpwr_a = sd.m_satinfo[i_i].s_txpwr_sval;
			}
			if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_X_STR) == 0)
			{
				txpwr_a = sd.m_satinfo[i_i].x_txpwr_sval;
			}
			l_pwr_cnt = dba.ud_adr_to_resvec((char*)v_sx_adr[l_sx_no], TXPWR_ID, ENDOFSAT_ID, ENDOFSATTX_ID, (char**)v_adr, 20);
			for (l_pwr_no = 0; l_pwr_no < l_pwr_cnt; l_pwr_no++)
			{
				if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
				{
					sd.m_satinfo[i_i].i_s_power++;      /* S-BAND送信パワー項目数 */
				}
				else
				{
					sd.m_satinfo[i_i].i_x_power++;      /* X-BAND送信パワー項目数 */
				}

				txpwr_a->sz_txpwr_val = &v_adr[l_pwr_no]->sz_txpwr[0];
				l_sp_cnt = dba.ud_adr_to_resvec((char*)v_adr[l_pwr_no], SATANT_ID, ENDOFSAT_ID, ENDOFTXPWR_ID, (char**)v_adr_sp, 20);
				for (l_sp_no = 0; l_sp_no < l_sp_cnt; l_sp_no++)
				{
					if (strcmp(v_sx_adr[l_sx_no]->sz_sattx, TXBAND_S_STR) == 0)
					{
						sd.m_satinfo[i_i].s_txpwr_sval[l_pwr_no].i_s_snd_level++;       /* S-BAND送信レベル項目数 */
					}
					else
					{
						sd.m_satinfo[i_i].x_txpwr_sval[l_pwr_no].i_x_snd_level++;       /* X-BAND送信レベル項目数 */
					}
					txpwr_a->satant_sval[l_sp_no] = &v_adr_sp[l_sp_no]->sz_satant[0];
				}
				txpwr_a->satant_sval[l_sp_no] = NULL;
				txpwr_a++;
			}
			txpwr_a->sz_txpwr_val = NULL;
		}

		i_m = 0;
		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		/*----------------------------------*/
		/* 予報値ファイルディレクトリの取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == PRD_ID)
			{
				/*識別ＩＤがの場合*/
				prd_t_adr = (struct prd_t *)satinf_adr;
				strcpy_s(sd.m_satinfo[i_i].sz_pred_dir, sizeof(sd.m_satinfo[i_i].sz_pred_dir), prd_t_adr->sz_prdpath);
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}

		i_m = 0;
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*------------------------------*/
		/* 計画ファイルディレクトリ取得 */
		/*------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == PLAN_ID)
			{				/*識別ＩＤがの場合*/
				plan_t_adr = (struct plan_t *)satinf_adr;
				strcpy_s(sd.m_satinfo[i_i].sz_plan_dir, sizeof(sd.m_satinfo[i_i].sz_plan_dir), plan_t_adr->sz_planpath);
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------------------*/
		/* Ｘ帯送信設備使用有無情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == XTX_ID)
			{
				/*識別ＩＤが「Ｘ帯送信設備使用有無情報」の場合*/
				xtx_t_adr = (struct xtx_t *)satinf_adr;
				sd.m_satinfo[i_i].i_x_tx_sel = xtx_t_adr->l_x_tx;
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*----------------------------------*/
		/* Ｘ帯受信設備対象情報取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == XRXSEL_ID)
			{
				/*識別ＩＤが「Ｘ帯受信設備対象情報」の場合*/
				xrxsel_t_adr = (struct xrxsel_t *)satinf_adr;
				sd.m_satinfo[i_i].i_x_rx_sel = xrxsel_t_adr->l_x_rx_sel;
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------------------*/
		/* 周波数ランプ制御許可情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RAMPCNT_ID)
			{
				/*識別ＩＤが「周波数ランプ制御許可情報」の場合*/
				rampcnt_t_adr = (struct rampcnt_t *)satinf_adr;
				sd.m_satinfo[i_i].i_s_tx_ramp = rampcnt_t_adr->l_s_tx;			/* Ｓ帯送信機ランプ制御有無 */
				sd.m_satinfo[i_i].i_x_tx_ramp = rampcnt_t_adr->l_x_tx;			/* Ｘ帯送信機ランプ制御有無 */
				sd.m_satinfo[i_i].i_s_rx_ramp = rampcnt_t_adr->l_s_rx;			/* Ｓ帯受信設備ランプ制御有無 */
				sd.m_satinfo[i_i].i_x_rx_ramp = rampcnt_t_adr->l_x_rx;			/* Ｘ帯受信設備ランプ制御有無（鹿児島専用） */
				sd.m_satinfo[i_i].i_x_rx1_ramp = rampcnt_t_adr->l_x_rx1;		/* Ｘ帯受信設備１ランプ制御有無（臼田専用） */
				sd.m_satinfo[i_i].i_x_rx2_ramp = rampcnt_t_adr->l_x_rx2;		/* Ｘ帯受信設備２ランプ制御有無（臼田専用） */
				sd.m_satinfo[i_i].i_sx_acqrx_ramp = rampcnt_t_adr->l_sx_acqrx;	/* ＳＸ帯捕捉受信設備ランプ制御有無（鹿児島専用） */
				sd.m_satinfo[i_i].i_x_qpskrx_ramp = rampcnt_t_adr->l_x_qpskrx;	/* Ｘ帯高速受信設備ランプ制御有無 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*----------------------------------*/
		/* 天頂通過監視角度情報取得 */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == TOPANGL_ID)
			{
				/*識別ＩＤが「天頂通過監視角度情報」の場合*/
				topangl_t_adr = (struct topangl_t *)satinf_adr;
				sd.m_satinfo[i_i].i_ent_angle = topangl_t_adr->l_in;		/* 天頂通過監視入角 */
				sd.m_satinfo[i_i].i_ext_angle = topangl_t_adr->l_out;		/* 天頂通過監視出角 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;			/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;	/*char型に変換*/

		/*--------------------------*/
		/* <<XUP>> 衛星捕捉情報取得 */
		/*--------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == CATCH_ID)
			{
				/*識別ＩＤが「衛星捕捉情報」の場合*/
				catch_t_adr = (struct catch_t *)satinf_adr;
				sd.m_satinfo[i_i].i_acq_route = catch_t_adr->l_route;	/* 捕捉ルート番号 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		/*--------------------------------------*/
		/* ＡＧＣ校正対象受信機情報取得 */
		/*--------------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{          /*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == AGCRX_ID)
			{
				/*識別ＩＤが「ＡＧＣ校正対象受信機情報」の場合*/
				agcrx54_t_adr = (struct agcrx54_t *)satinf_adr;
				sd.m_satinfo[i_i].i_s_rx = agcrx54_t_adr->l_s_rx;				/* Ｓ帯主受信設備 */
				sd.m_satinfo[i_i].i_x_rx = agcrx54_t_adr->l_x_rx;				/* Ｘ帯主受信設備 */
				sd.m_satinfo[i_i].i_x_rx1 = agcrx54_t_adr->l_x_rx1;				/* Ｘ帯主受信設備１ */
				sd.m_satinfo[i_i].i_x_dsntcr1 = agcrx54_t_adr->l_x_dsntcr1;		/* X帯DSN-MODEM1 */
				sd.m_satinfo[i_i].i_ka_dsntcr1 = agcrx54_t_adr->l_ka_dsntcr1;	/* Ka帯DSN-MODEM1 */
				sd.m_satinfo[i_i].i_sx_acqrx_s = agcrx54_t_adr->l_sx_acqrx_s;	/* ＳＸ帯補足用受信設備　Ｓ帯 */
				sd.m_satinfo[i_i].i_sx_acqrx_x = agcrx54_t_adr->l_sx_acqrx_x;	/* ＳＸ帯補足用受信設備　Ｘ帯 */
				sd.m_satinfo[i_i].i_x_qpskrx = agcrx54_t_adr->l_x_qpskrx;		/* Ｘ帯高速データ用受信設備 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		/*----------------------------------*/
		/* ノミナル受信周波数情報取得 */
		/*----------------------------------*/
		sd.m_satinfo[i_i].i_rxfq_ka = 0;
		sd.m_satinfo[i_i].i_rxfq_x = 0;
		sd.m_satinfo[i_j].i_rxfq_s = 0;
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFRXFQ_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RXFQVAL_ID)
			{
				/*識別ＩＤがの場合*/
				rxfqval_adr = (struct rxfqval_t *)satinf_adr;
				if (rxfqval_adr->l_band == RXBAND_S_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// S 帯有り
						sd.m_satinfo[i_i].i_rxfq_s = 1;
					}
				}
				if (rxfqval_adr->l_band == RXBAND_X_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// X 帯有り
						sd.m_satinfo[i_i].i_rxfq_x = 1;
					}
				}
				if (rxfqval_adr->l_band == RXBAND_KA_ID)
				{
					if (rxfqval_adr->d_fq != 0)
					{
						// Ka 帯有り
						sd.m_satinfo[i_i].i_rxfq_ka = 1;
					}
				}
			}

			sattbl_adr += satinf_adr->l_len;
		}

		satinf_adr = original_adr;				/*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;		/*char型に変換*/

		/*-----------------------*/
		/* S/X-CMD BITRATEの取得 */
		/*-----------------------*/
		if ((sd.m_satinfo[i_i].i_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT2).GetBuffer(), sd.m_satinfo[i_i].cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
		{
			if ((sd.m_satinfo[i_i].i_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT).GetBuffer(), sd.m_satinfo[i_i].cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
			{
				sd.m_satinfo[i_i].i_cmd_bitrate = 0;
			}
		}

		/*---------------------*/
		/* X-CMD BITRATEの取得 */
		/*---------------------*/
		if ((sd.m_satinfo[i_j].i_x_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT3).GetBuffer(), sd.m_satinfo[i_j].x_cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
		{
			if ((sd.m_satinfo[i_j].i_x_cmd_bitrate = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(C_BIT4).GetBuffer(), sd.m_satinfo[i_j].x_cmd_tbl, cmd_br_vec, TBLMAX)) == -1)
			{
				sd.m_satinfo[i_j].i_x_cmd_bitrate = 0;
			}
		}

		/*------------------*/
		/* X2 BITRATEの取得 */
		/*------------------*/
		if ((sd.m_satinfo[i_i].i_bitrate_x2 = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_X2).GetBuffer(), sd.m_satinfo[i_i].br_x_tbl, br_x_vec, TBLMAX * 2)) == -1)
		{
			sd.m_satinfo[i_i].i_bitrate_x2 = 0;
		}

		/*------------------*/
		/* KA BITRATEの取得 */
		/*------------------*/
		if ((sd.m_satinfo[i_i].i_bitrate_ka = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(BIT_Ka).GetBuffer(), sd.m_satinfo[i_i].br_x_tbl, br_ka_vec, TBLMAX * 2)) == -1)
		{
			sd.m_satinfo[i_i].i_bitrate_ka = 0;
		}

		/*--------------------------*//* 2008-02-07 再生測距対応　追加 */
		/* ＲＡＲＲ使用帯域情報取得 */
		/*--------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_tに変換*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*識別ＩＤが終了の場合*/
				break;
			}
			if (satinf_adr->l_id == RARRBAND_ID)
			{
				/*識別ＩＤが「ＲＡＲＲ使用帯域情報」の場合*/
				rarr_t_adr = (struct rarrband_t *)satinf_adr;
				sd.m_satinfo[i_i].i_rarr_band = rarr_t_adr->l_rarr_band;	/* ＲＡＲＲ使用帯域番号 */
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
		satinf_adr = original_adr;            /*衛星本体情報先頭アドレスセット*/
		sattbl_adr = (char *)satinf_adr;    /*char型に変換*/

		i_i++;
	}

	if ((cnt != 0) && (plan_t_adr != NULL))
	{
		char *cp_ptr;
		strcpy_s(sd.m_satinfo[i_i].sz_satnames, sizeof(sd.m_satinfo[i_i].sz_satnames), "MAINT");
		strcpy_s(sd.m_satinfo[i_i].sz_plan_dir, sizeof(sd.m_satinfo[i_i].sz_plan_dir), plan_t_adr->sz_planpath);
		cp_ptr = strrchr(sd.m_satinfo[i_i].sz_plan_dir, '/');
		*cp_ptr = '\0';
		strcpy_s(strrchr(sd.m_satinfo[i_i].sz_plan_dir, '/') + 1, sizeof(sd.m_satinfo[i_i].sz_plan_dir), "maint/");
		i_i++;
		strcpy_s(sd.m_satinfo[i_i].sz_satnames, sizeof(sd.m_satinfo[i_i].sz_satnames), "VLBI");
		strcpy_s(sd.m_satinfo[i_i].sz_plan_dir, sizeof(sd.m_satinfo[i_i].sz_plan_dir), plan_t_adr->sz_planpath);
		cp_ptr = strrchr(sd.m_satinfo[i_i].sz_plan_dir, '/');
		*cp_ptr = '\0';
		strcpy_s(strrchr(sd.m_satinfo[i_i].sz_plan_dir, '/') + 1, sizeof(sd.m_satinfo[i_i].sz_plan_dir), "vlbi/");
		i_i++;
		//	i_sat_num = i_i;
	}


	return 0;
}

/*============================================================================*/
// レディネスチェック時刻取得 (54m)
/*============================================================================*/
int COperationPlan::GetPlnTime54(CString str)
{
	int time = -1;
	CSatelliteData &sd = theApp.GetSatelliteData();
	CDBAccess &dba = theApp.GetSatelliteData().GetDBAccessCls();

	time = dba.GetPlnTimeFile(str);	//計画定義ファイルより該当の時間を取得する

	return time;
}
