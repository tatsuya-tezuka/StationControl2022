/*============================================================================*/
/*! TabAntennaView54.cpp

-# 捕捉状況ステータス画面（アンテナ）
*/
/*============================================================================*/
// TabAntennaView54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabAntennaView54.h"
#include "afxdialogex.h"
#include "EngValCnv.h"


// CTabAntennaView54 ダイアログ

IMPLEMENT_DYNCREATE(CTabAntennaView54, CDialogEx)

CTabAntennaView54::CTabAntennaView54(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabAntennaView54::IDD, pParent)
	, mStrRaVal(_T(""))
	, mStrDecVal(_T(""))
	, mCurRealElAngle(-DBL_MAX)
	, mPreRealElAngle(-DBL_MAX)
{

}

CTabAntennaView54::~CTabAntennaView54()
{
}

void CTabAntennaView54::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AZ_ANGLE, mAzAngle);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_EL_ANGLE, mElAngle);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_AZ_ERROR, mAzError);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_EL_ERROR, mElError);
	DDX_Control(pDX, IDC_STATIC_PREDDIFF, mPredDiff);
	DDX_Control(pDX, IDC_STATIC_RA, mRa);
	DDX_Control(pDX, IDC_STATIC_DEC, mDec);
	DDX_Control(pDX, IDC_STATIC_CONTDATE, mContDate);
}


BEGIN_MESSAGE_MAP(CTabAntennaView54, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AZ_DRIVE, &CTabAntennaView54::OnBnClickedButtonAzDrive)
	ON_BN_CLICKED(IDC_BUTTON_EL_DRIVE, &CTabAntennaView54::OnBnClickedButtonElDrive)
	ON_BN_CLICKED(IDC_BUTTON_AZ_OFFSET, &CTabAntennaView54::OnBnClickedButtonAzOffset)
	ON_BN_CLICKED(IDC_BUTTON_EL_OFFSET, &CTabAntennaView54::OnBnClickedButtonElOffset)
	ON_BN_CLICKED(IDC_BUTTON_MODE, &CTabAntennaView54::OnBnClickedButtonMode)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &CTabAntennaView54::OnBnClickedButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_RADEC_OFFSET, &CTabAntennaView54::OnBnClickedButtonRadecOffset)
	ON_BN_CLICKED(IDC_BUTTON_KAAUTOOFFSET, &CTabAntennaView54::OnBnClickedButtonKaautooffset)
END_MESSAGE_MAP()


// CTabAntennaView54 メッセージ ハンドラー


BOOL CTabAntennaView54::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mAzAngle.SetGradient(false);
	mAzAngle.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mAzAngle.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mAzAngle.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mElAngle.SetGradient(false);
	mElAngle.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mElAngle.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mElAngle.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mAzError.SetGradient(false);
	mAzError.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mAzError.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mAzError.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mElError.SetGradient(false);
	mElError.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mElError.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mElError.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mPredDiff.SetGradient(false);
	mPredDiff.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mPredDiff.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mPredDiff.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	mRa.SetGradient(false);
	mRa.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mRa.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mRa.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mDec.SetGradient(false);
	mDec.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mDec.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mDec.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	mContDate.SetGradient(false);
	mContDate.SetTextColor(RGB(0, 128, 0), RGB(0, 128, 0));
	mContDate.SetBackColor(GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	mContDate.SetTextStyle(DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! アンテナ

-# AZ DRIVEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::OnBnClickedButtonAzDrive()
{
	ControlParamExecute(0);
}



/*============================================================================*/
/*! アンテナ

-# EL DRIVEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::OnBnClickedButtonElDrive()
{
	ControlParamExecute(1);
}



/*============================================================================*/
/*! アンテナ

-# AZ OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::OnBnClickedButtonAzOffset()
{
	ControlParamExecute(2);
}


/*============================================================================*/
/*! アンテナ

-# EL OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::OnBnClickedButtonElOffset()
{
	ControlParamExecute(3);
}

/*============================================================================*/
/*! アンテナ

-# MODEボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::OnBnClickedButtonMode()
{
	ControlParamExecute(4);
}


/*============================================================================*/
/*! アンテナ

-# TIME OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::OnBnClickedButtonTime()
{
	ControlParamExecute(5);
}


/*============================================================================*/
/*! アンテナ

-# RA/DEC OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::OnBnClickedButtonRadecOffset()
{
	ControlParamExecute(6);
}


/*============================================================================*/
/*! アンテナ

-# Ka OUTO OFFSETボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::OnBnClickedButtonKaautooffset()
{
	ControlParamExecute(7);
}


/*============================================================================*/
/*! アンテナ

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabAntennaView54::ControlParamExecute(UINT pos)
{
	CString mControlString = mAntennaParam54[pos].nameS;
	CString str;
	GetDlgItem(mAntennaParam54[pos].id)->GetWindowText(str);
	stControlValue val;
	val.ret = 0;
	val.type = mAntennaParam54[pos].type;
	switch (val.type){
	case	eControlStatus:
		sprintf_s(val.buf, 80, "%s", (LPCSTR)CStringA(str));
		break;
	case	eControlNumeric:
		str.Remove(',');
		val.val = _wtoi(str);
		break;
	case	eControlDouble:
		str.Remove(',');
		val.fval = _wtof(str);
		break;
	case	eControlTime:
		sprintf_s(val.buf, 80, "%s", (LPCSTR)CStringA(str));
		break;
	case	eControlCommand:
		// ２つの制御値をセットして送る
		stControlValue raVal, decVal;

		mStrRaVal.TrimLeft(_T(" "));
		if (mStrRaVal.IsEmpty())
			raVal.fval = 0.0;
		else
			raVal.fval = _wtof(mStrRaVal);

		mStrDecVal.TrimLeft(_T(" "));
		if (mStrDecVal.IsEmpty())
			decVal.fval = 0.0;
		else
			decVal.fval = _wtof(mStrDecVal);

		if (theApp.GetSatelliteData().param_chk2(RA_OFFSET, &raVal, DEC_OFFSET, &decVal) < 0)
			return;
		if (raVal.ret == false || decVal.ret == false)
			return;

		mStrRaVal.Format(_T("%s"), (LPCTSTR)CString(raVal.buf));
		mStrDecVal.Format(_T("%s"), (LPCTSTR)CString(decVal.buf));

		if (mStrRaVal.IsEmpty() == true || mStrDecVal == true)
			return;

		theApp.SendControlChkDlg2(mControlString, mStrRaVal + _T("@") + mStrDecVal + _T(",;"), _T(""), _T(""));

#if  0
		SYSTEMTIME st_UTC;
		GetSystemTime(&st_UTC);
		CTime cTime = CTime(st_UTC);
		CString strTime = cTime.Format("%m-%d %H:%M:%S");

		mStrRaVal.TrimLeft(_T("+"));
		mStrDecVal.TrimLeft(_T("+"));
		mRa.SetText(mStrRaVal);
		mDec.SetText(mStrDecVal);
		mContDate.SetText(strTime);
		UpdateData(FALSE);
#endif
		return;
	}

	theApp.GetSatelliteData().SendControlSelect(mControlString, &val);
	if (val.ret == 0)
		return;

	switch (val.type){
	case	eControlStatus:
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
		break;
	case	eControlNumeric:
		str.Format(_T("%d"), val.val);
		break;
	case	eControlDouble:
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
		break;
	case	eControlTime:
		str.Format(_T("%s"), (LPCTSTR)CString(val.buf));
		break;
	}

	theApp.SendControlChkDlg(mControlString, str, _T(""), _T(""));
}


/*============================================================================*/
/*! アンテナ

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabAntennaView54::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mAntennaParam54) / sizeof(mAntennaParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
//		if (mAntennaParam54[i].id == IDC_STATIC_RA ||
//			mAntennaParam54[i].id == IDC_STATIC_DEC ||
//			mAntennaParam54[i].id == IDC_STATIC_CONTDATE)
//		{
//			// 制御送信時に更新
//			continue;
//		}

		CString mControlString = mAntennaParam54[i].nameK;
		CString str;
		GetDlgItem(mAntennaParam54[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mAntennaParam54[i].type;

		if (mControlString.IsEmpty())
		{
			continue;
		}

		UINT sta = theApp.GetSelectStation();
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, mControlString);

//#ifdef _DEBUG
//		if (mAntennaParam54[i].id != IDC_BUTTON_TIME && obs_adr == NULL)
//		{
//			continue;
//		}
//#else
		if (obs_adr == NULL)
		{
			continue;
		}
//#endif

		switch (val.type){
		case	eControlStatus:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		case	eControlNumeric:
			str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3, 0);
//			str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3, (obs_adr->l_cnvResType >> CEngValCnv::eCnvResType_Signflag) & 1);
			break;
		case	eControlDouble:
			if (mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE)
			{
				// PRED DIFF用の実EL値をコピー
				mPreRealElAngle = mCurRealElAngle;
				mCurRealElAngle = obs_adr->d_data;
			}

			if (mAntennaParam54[i].id != IDC_STATIC_PREDDIFF)
			{
				str = CSatelliteData::AddCanma(obs_adr->d_data, 4, 0);
			}

//			str = CSatelliteData::AddCanma(obs_adr->d_data, 4, (obs_adr->l_cnvResType >> CEngValCnv::eCnvResType_Signflag) & 1);
			break;
		case	eControlTime:
			if (mAntennaParam54[i].id == IDC_STATIC_CONTDATE)
			{
				char	sz_asctime[40], contdate[40];
				char*	psta_asctime = &sz_asctime[0];
				char*	pend_asctime;
				memset(sz_asctime, 0, 40);
				memset(contdate, 0, 40);
//#ifdef _DEBUG
//				CEngValCnv::obsname_t Obs_adr;
//				obs_adr = &Obs_adr;
//				strcpy_s(obs_adr->sz_sts_name, 40, "2018-10-12 01:02:03");
//#endif		
				if (CSatelliteData::ub_asc_to_asctime(obs_adr->sz_sts_name, psta_asctime) >= 0)
				{
					psta_asctime += 5;								// 年を消す
					pend_asctime = strchr(psta_asctime, '.');		// 小数点以下を消す
					strncpy_s(contdate, sizeof(contdate), psta_asctime, pend_asctime - psta_asctime);
					str.Format(_T("%s"), (LPCTSTR)CString(contdate));
				}
				else
				{
					//=====================================================//
					//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
					CLogTraceEx::Write(_T(""), _T("CTabAntennaView54"), _T("UpdateStatus"), _T("detail"), (LPCTSTR)mAntennaParam54[i].nameK, nLogEx::detail);
					//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
					//=====================================================//
				}
			}
			else
			{
//#ifdef _DEBUG
//				CEngValCnv::obsname_t Obs_adr;
//				obs_adr = &Obs_adr;
//				strcpy_s(obs_adr->sz_sts_name, 40, "-4:56");
//#endif			
				str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
				if (mAntennaParam54[i].id == IDC_BUTTON_TIME)
				{
					// 受信データが±MM:SSの場合は±M:SSにする
					if (str.GetLength() == 6)
					{
						str.Delete(1, 1);
					}
				}
			}
			break;
		}

		if (mAntennaParam54[i].id == IDC_STATIC_PREDDIFF)
		{
			str = _T("+00:00");
			// PRED DIFF計算有効
			if (theApp.IsPredDiffEnable())
			{
				// MON TIMEがAOSとLOSの間で、かつパス中の場合、PRED DIFF計算する
				double mon_time = obs_adr->d_data;
#ifdef _DEBUG
//				mon_time = mon_time - 3600 * 24 * 365;
//				mon_time += 50;
#endif

				obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(UNYOU_FLAG));
				CString strPassFlag(obs_adr->sz_sts_name);
				if (mon_time >= theApp.GetSatelliteData().GetTrendData().m_gst_dspinf.d_AOS_time &&
					mon_time <= theApp.GetSatelliteData().GetTrendData().m_gst_dspinf.d_LOS_time &&
					obs_adr != NULL && strPassFlag.Compare(_T("パス中")) == 0)
				{
					if (mPreRealElAngle != -DBL_MAX)
					{
						// 実ELの前回値がある場合、PRED DIFF計算
						// 上り下り判定
						double dRealSlope = mCurRealElAngle - mPreRealElAngle;
						double dPredSlope = -DBL_MAX;
						double dDiffEL;
						double dMinDiffEL = DBL_MAX;
						size_t search_idx = 0;
						// 実EL値に近い時刻を検索
						CGraphMutex::Lock(eRadar);
						vector<stPredData>& ELlist = theApp.GetDataCollection().GetELPredInterpolateList(sta);

						if (ELlist.size() <= 0)
						{
							((CColorStatic*)GetDlgItem(mAntennaParam54[i].id))->SetText(str);
							CGraphMutex::Unlock(eRadar);
							continue;
						}

						for (size_t elIndex = 0; elIndex < ELlist.size(); elIndex++)
						{
							if (elIndex > 0)
							{
								// 上り下りが同じで、EL値が近いものを調べる
								dDiffEL = fabs(ELlist[elIndex].data - mCurRealElAngle);
								dPredSlope = ELlist[elIndex].data - ELlist[elIndex - 1].data;
								if (dRealSlope >= 0 && dPredSlope >= 0 ||
									dRealSlope < 0 && dPredSlope < 0)
								{
									if (dDiffEL < dMinDiffEL)
									{
										dMinDiffEL = dDiffEL;
										search_idx = elIndex;
									}
								}
							}
						}

						double dDiffTime = mon_time - ELlist[search_idx].t;
						int mm = ((int)fabs(dDiffTime)) / 60;
						int ss = ((int)fabs(dDiffTime)) % 60;

						str.Format(_T("%02d:%02d"), mm, ss);
						CString strPredDiff;
						if (dDiffTime >= 0)
						{
							strPredDiff = _T("+") + str;
						}
						else
						{
							strPredDiff = _T("-") + str;
						}

						if (fabs(dDiffTime) < theApp.GetPredDiffMin())
						{
							str = strPredDiff + _T("*");
						}
						else
						{
							str = strPredDiff;
						}

						CGraphMutex::Unlock(eRadar);
					}
				}
			}
			((CColorStatic*)GetDlgItem(mAntennaParam54[i].id))->SetText(str);
		}
		else if (mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
			mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE ||
			mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_AZ_ERROR ||
			mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_EL_ERROR ||
//			mAntennaParam54[i].id == IDC_STATIC_PREDDIFF ||
			mAntennaParam54[i].id == IDC_STATIC_RA ||
			mAntennaParam54[i].id == IDC_STATIC_DEC)
		{
			((CColorStatic*)GetDlgItem(mAntennaParam54[i].id))->SetText(str);
		}
		else
		{
			GetDlgItem(mAntennaParam54[i].id)->SetWindowText(str);
		}

		if (mAntennaParam54[i].id == IDC_STATIC_RA)
		{
			mStrRaVal = str;
		}
		if (mAntennaParam54[i].id == IDC_STATIC_DEC)
		{
			mStrDecVal = str;
		}
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! アンテナ

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabAntennaView54::InitStatus()
{
	int szControl = sizeof(mAntennaParam54) / sizeof(mAntennaParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mAntennaParam54[i].type;

		switch (val.type){
		case	eControlStatus:
			str.Format(_T(""));
			break;
		case	eControlNumeric:
			str.Format(_T(""));
			break;
		case	eControlDouble:
			str.Format(_T(""));
			break;
		case	eControlTime:
			str.Format(_T(""));
			break;
		}

		if (mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_AZ_ANGLE ||
			mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_EL_ANGLE ||
			mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_AZ_ERROR ||
			mAntennaParam54[i].id == IDC_MFCMASKEDEDIT_EL_ERROR ||
			mAntennaParam54[i].id == IDC_STATIC_RA ||
			mAntennaParam54[i].id == IDC_STATIC_DEC ||
			mAntennaParam54[i].id == IDC_STATIC_CONTDATE)
		{
			if (str.IsEmpty())
				str.Format(_T(" "));
			((CColorStatic*)GetDlgItem(mAntennaParam54[i].id))->SetText(str);
		}
		if (mAntennaParam54[i].id == IDC_STATIC_PREDDIFF)
		{
			if (str.IsEmpty())
				str.Format(_T("+00:00"));
			((CColorStatic*)GetDlgItem(mAntennaParam54[i].id))->SetText(str);
		}

		if (mAntennaParam54[i].id == IDC_BUTTON_RADEC_OFFSET)
		{
			// 何もしない
		}
		else
		{
			GetDlgItem(mAntennaParam54[i].id)->SetWindowText(str);
		}
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! アンテナ

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabAntennaView54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
		UpdateStatus();
		return TRUE;
	default:
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
