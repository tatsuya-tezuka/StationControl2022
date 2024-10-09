/*============================================================================*/
/*! TabKarecvView54.cpp

-# 捕捉状況ステータス画面（Ka帯受信）
*/
/*============================================================================*/
// TabKarecvView54.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabKarecvView54.h"
#include "afxdialogex.h"


// CTabKarecvView54 ダイアログ

IMPLEMENT_DYNCREATE(CTabKarecvView54, CDialogEx)

CTabKarecvView54::CTabKarecvView54(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabKarecvView54::IDD, pParent)
{

}

CTabKarecvView54::~CTabKarecvView54()
{
}

void CTabKarecvView54::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabKarecvView54, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DCCTRL, &CTabKarecvView54::OnBnClickedButtonDcctrl)
	ON_BN_CLICKED(IDC_BUTTON_RX, &CTabKarecvView54::OnBnClickedButtonRx)
END_MESSAGE_MAP()


// CTabKarecvView54 メッセージ ハンドラー

BOOL CTabKarecvView54::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.SetMonitorMode(this);

	InitStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! Ka帯受信

-# D/C CTRLボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabKarecvView54::OnBnClickedButtonDcctrl()
{
	ControlParamExecute(0);
}


/*============================================================================*/
/*! Ka帯受信

-# RXボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabKarecvView54::OnBnClickedButtonRx()
{
	ControlParamExecute(1);
}

/*============================================================================*/
/*! Ka帯受信

-# 制御パラメタの実行

@param  なし
@retval なし
*/
/*============================================================================*/
void CTabKarecvView54::ControlParamExecute(UINT pos)
{
	CString mControlString = mKarecvParam54[pos].nameS;
	CString str;
	GetDlgItem(mKarecvParam54[pos].id)->GetWindowText(str);

	stControlValue val;
	val.ret = 0;
	val.type = mKarecvParam54[pos].type;
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
/*! Ka帯受信

-# 捕捉状況ステータスの更新

@param
@retval

*/
/*============================================================================*/
void CTabKarecvView54::UpdateStatus()
{
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mKarecvParam54) / sizeof(mKarecvParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mKarecvParam54[i].nameK;
		CString str;
		GetDlgItem(mKarecvParam54[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mKarecvParam54[i].type;

		if (mControlString.IsEmpty())
		{
			continue;
		}

		UINT sta = theApp.GetSelectStation();
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, mControlString);

		if (obs_adr == NULL)
		{
			continue;
		}

		switch (val.type){
		case	eControlStatus:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		case	eControlNumeric:
			str = CSatelliteData::AddCanma((double)obs_adr->l_data, 3);
			break;
		case	eControlDouble:
			str = CSatelliteData::AddCanma(obs_adr->d_data, 3);
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		GetDlgItem(mKarecvParam54[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! Ka帯受信

-# 捕捉状況ステータスの初期化

@param
@retval

*/
/*============================================================================*/
void CTabKarecvView54::InitStatus()
{
	int szControl = sizeof(mKarecvParam54) / sizeof(mKarecvParam54[0]);

	for (int i = 0; i < szControl; i++)
	{
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mKarecvParam54[i].type;

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

		GetDlgItem(mKarecvParam54[i].id)->SetWindowText(str);
	}
	UpdateData(FALSE);
}


/*============================================================================*/
/*! Ka帯受信

-# 捕捉状況ステータスの更新

@param  なし
@retval なし
*/
/*============================================================================*/
LRESULT CTabKarecvView54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
