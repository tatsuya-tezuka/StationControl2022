// EquipmentCtrlTime.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrlTime.h"
#include "afxdialogex.h"


// CEquipmentCtrlTime ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentCtrlTime, CDialogEx)

CEquipmentCtrlTime::CEquipmentCtrlTime(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEquipmentCtrlTime::IDD, pParent)
	, m_strControlName(_T(""))
	, m_strWindowTitle(_T(""))
	, m_time(0)
	, m_nTimeType(eTimeType_MMSS)
{
	mTimeString.Empty();
}

CEquipmentCtrlTime::~CEquipmentCtrlTime()
{
}

void CEquipmentCtrlTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPT_STATIC, m_strControlName);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_CTRLREGIST, m_CMaskedEdit);
}


BEGIN_MESSAGE_MAP(CEquipmentCtrlTime, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrlTime::OnBnClickedOk)
//	ON_EN_KILLFOCUS(IDC_MFCMASKEDEDIT_CTRLREGIST, &CEquipmentCtrlTime::OnEnKillfocusMfcmaskededitCtrlregist)
END_MESSAGE_MAP()


// CEquipmentCtrlTime メッセージ ハンドラー


/*============================================================================*/
/*! CEquipmentCtrlTime

-# 初期化時に呼び出される。

@param  なし
@retval なし

*/
/*============================================================================*/
BOOL CEquipmentCtrlTime::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CTime time;

	if (!m_strWindowTitle.IsEmpty())
		SetWindowText(m_strWindowTitle);

	switch (m_nTimeType)
	{
	case eTimeType_MSS:
		m_CMaskedEdit.EnableMask(_T("*****"), _T("_____"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890+-: "));
		if (mTimeString.IsEmpty())
		{
			// 時刻初期化
			m_time = 0;
			UpdateOffsetTime();
		}
		else
		{
			// 文字列から値取得
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	case eTimeType_MMSS:
		m_CMaskedEdit.EnableMask(_T("******"), _T("______"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890+-: "));
		if (mTimeString.IsEmpty())
		{
			// 時刻初期化
			m_time = 0;
			UpdateOffsetTime();
		}
		else
		{
			// 文字列から値取得
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	case eTimeType_HHMMSS:
		m_CMaskedEdit.EnableMask(_T("*********"), _T("_________"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890+-: "));
		if (mTimeString.IsEmpty())
		{
			// 時刻初期化
			m_time = 0;
			UpdateOffsetTime();
		}
		else
		{
			// 文字列から値取得
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	case eTimeType_DDDHHMMSS:
		m_CMaskedEdit.EnableMask(_T("ddd dd:dd:dd"), _T("___ __:__:__"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890"));
		if (mTimeString.IsEmpty())
		{
			// 時刻初期化
			theApp.GetCurrentUtcTime(time);
			m_time = time.GetTime();
			UpdateOffsetTime();
		}
		else
		{
			// 文字列から値取得
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	case eTimeType_YYYYmmdd:
		m_CMaskedEdit.EnableMask(_T("dddd-dd-dd"), _T("____-__-__"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890"));
		if (mTimeString.IsEmpty())
		{
			// 時刻初期化
			theApp.GetCurrentUtcTime(time);
			m_time = time.GetTime();
			UpdateOffsetTime();
		}
		else
		{
			// 文字列から値取得
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	default:
		break;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CEquipmentCtrlTime

-# 実行時に呼び出される。

@param  なし
@retval なし

*/
/*============================================================================*/
void CEquipmentCtrlTime::OnBnClickedOk()
{
	UpdateData();
	m_CMaskedEdit.GetWindowText(mTimeString);

	if (GetOffsetTimeFromEditCtrl(mTimeString))
		CDialogEx::OnOK();
}


/*============================================================================*/
/*! CEquipmentCtrlTime

-# 時刻オフセットの時刻を更新する。

@param  なし
@retval なし

*/
/*============================================================================*/
void CEquipmentCtrlTime::UpdateOffsetTime()
{
	CString str, strSign;

	if (m_time > 0)
	{
		strSign = _T("+");
	}
	else if (m_time < 0)
	{
		strSign = _T("-");
		m_time *= -1;
	}
	else
	{
		strSign = _T(" ");
	}

	CTime t(m_time);
	CString strTime;

	switch (m_nTimeType)
	{
	case eTimeType_MSS:
		str.Format(_T("%s%01d%02d"), (LPCTSTR)strSign, t.GetMinute(), t.GetSecond());
		break;

	case eTimeType_MMSS:
		strTime = t.Format(_T("%M:%S"));
		str.Format(_T("%s%s"), (LPCTSTR)strSign, (LPCTSTR)strTime);
		break;

	case eTimeType_HHMMSS:
		strTime = t.Format(_T("%H:%M:%S"));
		str.Format(_T("%s%s"), (LPCTSTR)strSign, (LPCTSTR)strTime);
		break;

	case eTimeType_DDDHHMMSS:
		str = t.Format(_T("%j %H:%M:%S"));
		break;

	case eTimeType_YYYYmmdd:
		str = t.Format(_T("%Y-%m-%d"));
		break;

	default:
		break;
	}

	m_CMaskedEdit.SetWindowTextW(str);
}


/*============================================================================*/
/*! CEquipmentCtrlTime

-# エディットコントロールからオフセット時刻を取得

@param  str：オフセット時刻文字列
@param	bAutoCorrect：TRUE：範囲外時に最大値or最小値に自動修正 / FALSE：自動修正なし
@retval TRUE：範囲内かつ文字列整形されていない / FALSE：範囲外もしくは文字列整形された

*/
/*============================================================================*/
BOOL CEquipmentCtrlTime::GetOffsetTimeFromEditCtrl(CString& str, BOOL bAutoCorrect /*= FALSE*/)
{
	CString strTmp = str;

	// 正負を抽出
	int nSign = 0;
	if (strTmp.Left(1) == _T('+'))
	{
		nSign = 1;
		strTmp.TrimLeft(_T('+'));
	}
	else if (strTmp.Left(1) == _T('-'))
	{
		nSign = -1;
		strTmp.TrimLeft(_T('-'));
	}
	else if (strTmp.Left(1) == _T(' '))
	{
		nSign = 1;
		strTmp.TrimLeft(_T(' '));
	}

	BOOL ret = TRUE;

	int nMM = 0;
	int nSS = 0;
	int nHH = 0;
	int nDDD = 0;
	int nYYYY = 0;
	int nmm = 0;
	int ndd = 0;
	int t_total = 0;
	int nMMTmp = 0;


	// DDDHHMMSSの場合
	if (m_nTimeType == eTimeType_DDDHHMMSS)
	{
		CString str1;
		CString str2;

		CString strDDD;

		if (AfxExtractSubString(str1, strTmp, 0, ' '))
		{
			(void)AfxExtractSubString(str2, strTmp, 0, ' ');
		}

		if (!strTmp.IsEmpty())
		{
			if (!str1.IsEmpty())
			{
				if (!str2.IsEmpty())
				{
					// 時間と分と秒
					strTmp = str2;
				}
			}
			else
			{
				// 日にち
				strDDD = str1;
			}
		}

		nDDD = _ttoi(strDDD);

		if (nDDD >= m_timeDDDMax)
		{
			// 時間が大きすぎる
			if (bAutoCorrect)
			{
				nDDD = m_timeDDDMax - 1;
			}
			else
			{
				ret = FALSE;
			}
		}
	}

	// MSSまたはMMSSまたはHHMMSSの場合
	if (m_nTimeType == eTimeType_MSS || m_nTimeType == eTimeType_MMSS || m_nTimeType == eTimeType_HHMMSS)
	{
		CString str1;
		CString str2;
		CString str3;

		CString strHH;
		CString strMM;
		CString strSS;

		if (AfxExtractSubString(str1, strTmp, 0, ':'))
		{
			if (AfxExtractSubString(str2, strTmp, 1, ':'))
			{
				(void)AfxExtractSubString(str3, strTmp, 2, ':');
			}
		}

		if (!strTmp.IsEmpty())
		{
			if (!str1.IsEmpty())
			{
				if (!str2.IsEmpty())
				{
					if (!str3.IsEmpty())
					{
						// 時間と分と秒
						strHH = str1;
						strMM = str2;
						strSS = str3;
					}
					else
					{
						// 分と秒
						strMM = str1;
						strSS = str2;
					}
				}
				else
				{
					// 分のみ
					strSS = str1;
				}
			}
			else
			{
				// 秒のみ
				strSS = str1;
			}
		}

		nHH = _ttoi(strHH);
		nMM = _ttoi(strMM);
		nSS = _ttoi(strSS);

		t_total = nHH * m_timeMMMax + nMM * m_timeSSMax + nSS;

		switch (m_nTimeType)
		{
		case eTimeType_MSS:
			if (t_total >= m_timeMMMax * m_timeMMax)
			{
				if (bAutoCorrect)
					t_total = m_timeMMMax * m_timeMMax - 1;
				else
					ret = FALSE;
			}
			nMM = t_total / m_timeSSMax;
			nSS = t_total % m_timeSSMax;
			break;

		case eTimeType_MMSS:
			if (t_total >= m_timeMMMax * m_timeSSMax)
			{
				if (bAutoCorrect)
					t_total = m_timeMMMax * m_timeSSMax - 1;
				else
					ret = FALSE;
			}
			nMM = t_total / m_timeSSMax;
			nSS = t_total % m_timeSSMax;
			break;

		case eTimeType_HHMMSS:
			if (t_total >= m_timeHHMax * m_timeMMMax * m_timeSSMax)
			{
				if (bAutoCorrect)
					t_total = m_timeHHMax * m_timeMMMax * m_timeSSMax - 1;
				else
					ret = FALSE;
			}
			nHH = t_total / m_timeMMMax;
			nMMTmp = t_total % m_timeMMMax;
			nMM = nMMTmp / m_timeSSMax;
			nSS = t_total % m_timeSSMax;
			break;
		}
	}

	if (m_nTimeType == eTimeType_YYYYmmdd)
	{
		// 右から２つが日
		CString strdd = strTmp.Right(2);
		// 残りが月以上
		int nLength = strTmp.GetLength();
		CString strOvermm = strTmp.Left(nLength - 2);
		CString strmm = strOvermm.Right(2);
		ndd = _ttoi(strdd);
		nmm = _ttoi(strmm);

		if (ndd >= m_timeddMax)
		{
			// 日が大きすぎる
			if (bAutoCorrect)
			{
				ndd = m_timeddMax - 1;
			}
			else
			{
				ret = FALSE;
			}
		}
		if (nmm >= m_timemmMax)
		{
			// 月が大きすぎる
			if (bAutoCorrect)
			{
				nmm = m_timemmMax - 1;
			}
			else
			{
				ret = FALSE;
			}
		}
		// 残りが年
		nLength = strOvermm.GetLength();
		CString strOverYYYY = strOvermm.Left(nLength - 2);
		CString strYYYY = strOvermm.Right(4);
		nYYYY = _ttoi(strYYYY);
	}

	// 手動修正の場合
	if (bAutoCorrect == FALSE && ret == FALSE)
	{
		// エラーダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_OFFSETTIME_OUTOFRANGE_ERR) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		//# エラーなのでエディットコントロールの内容を全選択してフォーカス設定する
		m_CMaskedEdit.SetSel(0, -1);
		m_CMaskedEdit.SetFocus();
	}

	switch (m_nTimeType)
	{
	case eTimeType_MSS:
		strTmp.Format(_T("%d:%02d"), nMM, nSS);
		break;

	case eTimeType_MMSS:
		strTmp.Format(_T("%02d:%02d"), nMM, nSS);
		break;

	case eTimeType_HHMMSS:
		strTmp.Format(_T("%02d:%02d:%02d"), nHH, nMM, nSS);
		break;

	case eTimeType_DDDHHMMSS:
		strTmp.Format(_T("%03d %02d:%02d:%02d"), nDDD, nHH, nMM, nSS);
		break;

	case eTimeType_YYYYmmdd:
		strTmp.Format(_T("%04d-%02d-%02d"), nYYYY, nmm, ndd);
		break;

	default:
		break;
	}

	CString strText;
	if (nSign > 0)
	{
		strText.Format(_T("+%s"), (LPCTSTR)strTmp);
	}
	else if (nSign < 0)
	{
		strText.Format(_T("-%s"), (LPCTSTR)strTmp);
	}
	else if (m_nTimeType == eTimeType_MSS || m_nTimeType == eTimeType_MMSS || m_nTimeType == eTimeType_HHMMSS)
	{
		strText.Format(_T("+%s"), (LPCTSTR)strTmp);
	}
	else
	{
		strText = strTmp;
	}

	strTmp = strText;

	// 文字列整形時
	if (strTmp.CompareNoCase(str) != 0)
	{
		if (ret)
		{
			// 値が範囲内の場合、整形文字を表示しなおす
			m_CMaskedEdit.SetWindowTextW(strTmp);
			UpdateData(FALSE);
		}
		else
		{
			// 値が範囲外の場合、入力文字をそのまま表示する
		}

		ret = FALSE;
	}
	
	return ret;
}


BOOL CEquipmentCtrlTime::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_RETURN) {
		//# エディットコントロールの場合、ENTERキーを受信したらTABキーに変更する
		HWND p = m_CMaskedEdit.m_hWnd;
		if (p == pMsg->hwnd){
			pMsg->wParam = VK_TAB;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//void CEquipmentCtrlTime::OnEnKillfocusMfcmaskededitCtrlregist()
//{
//	//# フォーカスが解除されたら、時刻オフセットをフォーマットチェックする
//	m_CMaskedEdit.GetWindowText(mTimeString);
//	GetOffsetTimeFromEditCtrl(mTimeString);
//}
