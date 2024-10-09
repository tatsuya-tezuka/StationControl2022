/*============================================================================*/
/*! OperationPlanTimeOffsetSetting.cpp

-# 時刻オフセット設定画面
*/
/*============================================================================*/
// OperationPlanTimeOffsetSetting.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationPlanTimeOffsetSet.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"

// COperationPlanTimeOffsetSet ダイアログ

IMPLEMENT_DYNAMIC(COperationPlanTimeOffsetSet, CDialogBase)

/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
COperationPlanTimeOffsetSet::COperationPlanTimeOffsetSet(CWnd* pParent /*=NULL*/)
	: CDialogBase(COperationPlanTimeOffsetSet::IDD, pParent)
	, m_time(0)
{
	m_Type = -1;
}

/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
COperationPlanTimeOffsetSet::~COperationPlanTimeOffsetSet()
{
}

void COperationPlanTimeOffsetSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OFFSETTIME_MFCMASKEDEDIT, m_CMaskedEdit);
	DDX_Control(pDX, IDC_OFFSETTIME_SPIN, m_CSpin);
}


BEGIN_MESSAGE_MAP(COperationPlanTimeOffsetSet, CDialogBase)
	ON_BN_CLICKED(IDC_OPERATIONPLANTIMEOFFSETSET_BUTTON1, &COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton1)
	ON_BN_CLICKED(IDC_OPERATIONPLANTIMEOFFSETSET_BUTTON2, &COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton2)
	ON_BN_CLICKED(IDC_OPERATIONPLANTIMEOFFSETSET_BUTTON3, &COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_OFFSETTIME_SPIN, &COperationPlanTimeOffsetSet::OnDeltaposOffsettimeSpin)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// COperationPlanTimeOffsetSet メッセージ ハンドラー


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# ダイアログ初期化処理

@param  なし
@retval TRUE：初期化成功 / FALSE：初期化失敗
*/
/*============================================================================*/
BOOL COperationPlanTimeOffsetSet::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_CMaskedEdit.EnableMask(_T("*d:dd"), _T("__:__"), _T(' '));
	m_CMaskedEdit.SetValidChars(_T("01234567890+-"));

	m_CSpin.SetRange32(0, (int)(m_timeMax - m_timeMin));

	UpdateOffsetTime();

	m_CSpin.SetPos32((int)m_time + m_timeMax);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# 時刻オフセットの時刻を更新する。

@param  なし
@retval なし

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::UpdateOffsetTime()
{
	CString str, strSign;
	time_t t = m_time;

	if (t >= 0)
	{
		strSign = _T("+");
	}
	else
	{
		strSign = _T("-");
		t *= -1;
	}
	str.Format(_T("%s%01d:%02d"), (LPCTSTR)strSign, (int)(t / 60), (int)(t % 60));
	m_CMaskedEdit.SetWindowTextW(str);

	UpdateData();
	UpdateWindow();
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# エディットコントロールからオフセット時刻を取得

@param  offsetTime：オフセット時刻出力
@param	bAutoCorrect：TRUE：範囲外時に最大値or最小値に自動修正 / FALSE：自動修正なし
@retval TRUE：範囲内だった / FALSE：範囲外だった

*/
/*============================================================================*/
BOOL COperationPlanTimeOffsetSet::GetOffsetTimeFromEditCtrl(time_t& offsetTime, BOOL bAutoCorrect /*= FALSE*/)
{
	UpdateData(TRUE);

	CString str;
	m_CMaskedEdit.GetWindowTextW(str);

	// 正負を抽出
	int nSign = 1;
	if (str.Left(1) == _T('+'))
	{
		nSign = 1;
		str.Remove(_T('+'));
	}
	else if (str.Left(1) == _T('-'))
	{
		nSign = -1;
		str.Remove(_T('-'));
	}

	// 右から２つが秒
	CString strSS = str.Right(2);
	// 残りが分
	int nLength = str.GetLength();
	CString strMM = str.Left(nLength - 2);
	int nMM = _ttoi(strMM);
	int nSS = _ttoi(strSS);
	BOOL bErr = FALSE;

	if (nMM > m_timeMMMax)
	{
		// 分が大きすぎる
		if (bAutoCorrect)
		{
			nMM = m_timeMMMax;
		}
		else
		{
			bErr = TRUE;
		}
	}
	if (nSS >= m_timeSSMax)
	{
		// 秒が大きすぎる
		if (bAutoCorrect)
		{
			nSS = m_timeSSMax;
		}
		else
		{
			bErr = TRUE;
		}
	}

	if (bErr == FALSE)
	{
		time_t nMMSS = nSign * (nMM * 60 + nSS);
		if (nMMSS > m_timeMax)
		{
			// 分と秒の合計が大きすぎる
			if (bAutoCorrect)
			{
				nMMSS = m_timeMax;
			}
		}
		else if (nMMSS < m_timeMin)
		{
			// 分と秒の合計が小さすぎる
			if (bAutoCorrect)
			{
				nMMSS = m_timeMin;
			}
		}
		else
		{
			offsetTime = nMMSS;
			return TRUE;
		}
	}

	// 手動修正の場合
	if (bAutoCorrect == FALSE)
	{
		// エラーダイアログ表示
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_OFFSETTIME_OUTOFRANGE_ERR) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
	}
	
	return FALSE;
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# OKボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton1()
{
	time_t nMMSS;
	// オフセット時刻取得
	if (GetOffsetTimeFromEditCtrl(nMMSS))
	{
		// 時刻オフセットをサーバに送信する
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strRemShellName = SEND_NOTICE_SH;

		CString arg;
		//		m_CMaskedEdit.GetWindowTextW(arg);
		CString strSign;
		if (nMMSS >= 0)
		{
			strSign = _T("+");
		}
		else
		{
			strSign = _T("-");
		}

		if (m_Type == TYPE_TIMELINE)
		{
			arg.Format(_T("%s %d %s%02d:%02d"), (LPCTSTR)m_strStationName, INFO_ID_REQ_TIMOFS, (LPCTSTR)strSign, (int)abs(nMMSS / 60), (int)abs(nMMSS % 60));
		}
		else
		{
			arg.Format(_T("%s %d %s,%s%02d:%02d"), (LPCTSTR)m_strStationName, INFO_ID_REQ_TIMOFS, (LPCTSTR)m_strFullFilePath, (LPCTSTR)strSign, (int)abs(nMMSS / 60), (int)abs(nMMSS % 60));
		}
		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// シェル待ちしない
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("COperationPlanTimeOffsetSet"), _T("OnBnClickedOperationplantimeoffsetsetButton1"), _T("debug"), strDebug, nLogEx::debug);
		}

		// ウィンドウを閉じる
		CDialogBase::OnOK();
	}
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# 時刻オフセット解除ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton2()
{
	// 時刻オフセットをクリアする
	m_time = 0;
	UpdateOffsetTime();
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# 終了ボタン押下時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton3()
{
// test >>>
//	CString strTest;
//strTest.Format(_T("%02d:%02d"), m_timeOffset.wMinute, m_timeOffset.wSecond);
//	AfxMessageBox(strTest);
// test <<<

	// 何もせずに閉じる
	PostMessage(WM_CLOSE);
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# オフセット時刻のスピンコントロール変更時に呼ばれる。

@param  なし
@retval なし

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnDeltaposOffsettimeSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if (m_CSpin.GetSafeHwnd() == NULL)
	{
		return;
	}

	// オフセット時刻取得し、スピン位置にセット
	(void)GetOffsetTimeFromEditCtrl(m_time, TRUE);
	int t = (int)m_time + m_timeMax;
	m_CSpin.SetPos32(t);

	*pResult = 0;
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# 縦スクロールバークリック時に呼ばれる

@param  pNMHDR：通知メッセージポインタ
@param	pResult：結果ポインタ
@retval なし

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == SB_THUMBPOSITION)
	{
		if ((m_CSpin.m_hWnd == pScrollBar->m_hWnd))
		{
			CString str;
			m_time = (time_t)((int)nPos - m_timeMax);
			UpdateOffsetTime();
		}
	}

	CDialogBase::OnVScroll(nSBCode, nPos, pScrollBar);
}
