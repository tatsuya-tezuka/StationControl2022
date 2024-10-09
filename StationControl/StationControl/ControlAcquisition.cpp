// ControlAcquisition.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlAcquisition.h"
#include "afxdialogex.h"


// CControlAcquisition ダイアログ

IMPLEMENT_DYNAMIC(CControlAcquisition, CDialogEx)

CControlAcquisition::CControlAcquisition(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlAcquisition::IDD, pParent)
	, m_strText(_T(""))
	, m_strPassword(_T(""))
{
	mGetTimer = 0;
}

CControlAcquisition::~CControlAcquisition()
{
}

void CControlAcquisition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_strText);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CControlAcquisition, CDialogEx)
	ON_BN_CLICKED(IDOK, &CControlAcquisition::OnBnClickedOk)
	ON_BN_CLICKED(IDMODE, &CControlAcquisition::OnBnClickedMode)
	ON_BN_CLICKED(IDCANCEL, &CControlAcquisition::OnBnClickedCancel)
END_MESSAGE_MAP()


// CControlAcquisition メッセージ ハンドラー


BOOL CControlAcquisition::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFileAccess ac;

	stLocalControl local;
	stControl data;
	char buf[mMaxPassword + 1] = { 0 };
	CString strDefaultPass;
	int type = -1;
	bool bControl = true;

	// アプリ起動端末の対象局の取得
	// ◆現状SSOC、STTN、TKSCか判別できないので、TKSCで行う
	int target = ac.GetClientTerminal();
	mClientTerminal = target;

	/*
		以下の条件が全て成立した場合にのみ制御権利の取得が可能となる。
		①パスワード・制御権利管理ファイルの「制御権利付与有無フラグ」が 「1（有）」
		②現在日時が、パスワード・制御権利管理ファイルの「制御権利付与開始日時」から「制御権利付与時間」以内
		③パスワード・制御権利管理ファイルの「制御権利付与開始日時」が、制御権利取得状況ファイルの「制御権利取得開始日時」よりも未来
		④制御権利取得状況ファイルの「制御権利取得有無フラグ」が、「0（無）」
	*/

	// 制御権利取得状況ファイル取得
	switch (theApp.GetSelectStation()){
	case	eStation_Usuda64:
		type = eLOCALudsc64;
		break;
	case	eStation_Usuda642:
		type = eLOCALxdsc54;
		break;
	case	eStation_Uchinoura34:
		type = eLOCALusc34;
		break;
	case	eStation_Uchinoura20:
		type = eLOCALusc20;
		break;
	default:
		EndDialog(IDABORT);
	}

	memset(&local, 0, sizeof(stLocalControl));
	ac.ReadLocalControl(local, type);

	// パスワード・制御権利管理ファイルの取得
	type = GetControlType(target);
	if (type >= 0){
		bool ret = false;
		switch (target){
		case	eClientSttn:
			ret = ac.ReadSOSControl(data, type);
			break;
		case	eClientSsoc:
			ret = ac.ReadSSOCControl(data, type);
			break;
		case	eClientTksc:
			ret = ac.ReadTKSCControl(data, type);
			break;
		}
		if (ret == true){
			// パスワード取得
			memset(buf, 0, mMaxPassword + 1);
			memcpy(buf, data.password, mMaxPassword);
			strDefaultPass = CString(buf);
			strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
			CString str = CString(data.lifetime);
			mGetTimer = _wtoi64(str);
			bControl = CheckControl(target, data, local);
		}
		else{
			strDefaultPass = mDefaultPassword;
			bControl = false;
		}
		mGetPassword = strDefaultPass;
	}
	else{
		strDefaultPass = mDefaultPassword;
		bControl = false;
	}

	CString strStation = mStationString[theApp.GetSelectStation()];
	m_strText.Format(_T("%sの制御権利を取得する場合、\nパスワードを入力してください。"), (LPCTSTR)strStation);

	if (bControl == true){
		// 制御権利取得端末が存在していない
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
	}
	else{
		// 制御権利取得端末が存在している
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
		m_strText.Format(_T("%sの制御権利は他端末で取得中です。"), (LPCTSTR)strStation);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! 制御権利管理

-# パスワード・制御権利管理対象の取得

@param
@retval

*/
/*============================================================================*/
int CControlAcquisition::GetControlType(int target)
{
	int type;
	switch (theApp.GetSelectStation()){
	case	eStation_Usuda64:
		if (target == eClientSttn)
			type = eSOSudsc64;
		else if (target == eClientSsoc)
			type = eSSOCudsc64;
		else if (target == eClientTksc)
			type = eTKSCudsc64;
		else
			return -1;
		break;
	case	eStation_Usuda642:
		if (target == eClientSttn)
			type = eSOSxdsc54;
		else if (target == eClientSsoc)
			type = eSSOCxdsc54;
		else if (target == eClientTksc)
			type = eTKSCxdsc54;
		else
			return -1;
		break;
	case	eStation_Uchinoura34:
		if (target == eClientSttn)
			type = eSOSusc34;
		else if (target == eClientSsoc)
			type = eSSOCusc34;
		else if (target == eClientTksc)
			type = eTKSCusc34;
		else
			return -1;
		break;
	case	eStation_Uchinoura20:
		if (target == eClientSttn)
			type = eSOSusc20;
		else if (target == eClientSsoc)
			type = eSSOCusc20;
		else if (target == eClientTksc)
			type = eTKSCusc20;
		else
			return -1;
		break;
	default:
		return -1;
	}
	return type;
}

/*============================================================================*/
/*! 制御権利管理

-# 制御権利取得可能かのチェック

@param
@retval

*/
/*============================================================================*/
bool CControlAcquisition::CheckControl(int target, stControl& data, stLocalControl& local)
{
	/*
	以下の条件が全て成立した場合にのみ制御権利の取得が可能となる。
	①パスワード・制御権利管理ファイルの「制御権利付与有無フラグ」が 「1（有）」
	②現在日時が、パスワード・制御権利管理ファイルの「制御権利付与開始日時」から「制御権利付与時間」以内
	③パスワード・制御権利管理ファイルの「制御権利付与開始日時」が、制御権利取得状況ファイルの「制御権利取得開始日時」よりも未来
	④制御権利取得状況ファイルの「制御権利取得有無フラグ」が、「0（無）」
	*/

	// ①パスワード・制御権利管理ファイルの「制御権利付与有無フラグ」が 「1（有）」
	if (data.flag == 0)
		return false;

	if (target != eClientTksc){
		// 筑波局管制以外
		// ②現在日時が、パスワード・制御権利管理ファイルの「制御権利付与開始日時」から「制御権利付与時間」以内
		// 現在日時の取得
		CTime ct;
		theApp.GetCurrentUtcTime(ct);
		// 制御権利付与開始日時
		CString cbuf, strDate, strTime;
		cbuf = CString(data.latestdate);
		AfxExtractSubString(strDate, cbuf, 0, ' ');
		AfxExtractSubString(strTime, cbuf, 1, ' ');
		SYSTEMTIME st;
		memset(&st, 0, sizeof(SYSTEMTIME));
		cbuf = strDate;
		AfxExtractSubString(strDate, cbuf, 0, '-');
		st.wYear = _wtoi(strDate);
		AfxExtractSubString(strDate, cbuf, 1, '-');
		st.wMonth = _wtoi(strDate);
		AfxExtractSubString(strDate, cbuf, 2, '-');
		st.wDay = _wtoi(strDate);
		cbuf = strTime;
		AfxExtractSubString(strTime, cbuf, 0, ':');
		st.wHour = _wtoi(strTime);
		AfxExtractSubString(strTime, cbuf, 1, ':');
		st.wMinute = _wtoi(strTime);
		AfxExtractSubString(strTime, cbuf, 2, ':');
		st.wSecond = _wtoi(strTime);
		CTime tm(st);
		if (ct.GetTime() < tm.GetTime() || ct.GetTime() > (tm.GetTime() + mGetTimer))
			return false;

		// ③パスワード・制御権利管理ファイルの「制御権利付与開始日時」が、制御権利取得状況ファイルの「制御権利取得開始日時」よりも未来
		cbuf = CString(local.latestdate);
		AfxExtractSubString(strDate, cbuf, 0, ' ');
		AfxExtractSubString(strTime, cbuf, 1, ' ');
		memset(&st, 0, sizeof(SYSTEMTIME));
		cbuf = strDate;
		AfxExtractSubString(strDate, cbuf, 0, '-');
		st.wYear = _wtoi(strDate);
		AfxExtractSubString(strDate, cbuf, 1, '-');
		st.wMonth = _wtoi(strDate);
		AfxExtractSubString(strDate, cbuf, 2, '-');
		st.wDay = _wtoi(strDate);
		cbuf = strTime;
		AfxExtractSubString(strTime, cbuf, 0, ':');
		st.wHour = _wtoi(strTime);
		AfxExtractSubString(strTime, cbuf, 1, ':');
		st.wMinute = _wtoi(strTime);
		AfxExtractSubString(strTime, cbuf, 2, ':');
		st.wSecond = _wtoi(strTime);
		CTime ltm(st);
		if (ltm.GetTime() >= tm.GetTime())
			return false;
	}

	// ④制御権利取得状況ファイルの「制御権利取得有無フラグ」が、「0（無）」
	if (local.flag == 1)
		return false;

	return true;
}

/*============================================================================*/
/*! 制御権利管理

-# 制御権利取得状況ファイルのフラグをクリアする

@param
@retval

*/
/*============================================================================*/
void CControlAcquisition::ResetControlFlag()
{
	CFileAccess ac;
	stLocalControl local;
	int type;

	// 制御権利取得状況ファイル取得
	switch (theApp.GetSelectStation()){
	case	eStation_Usuda64:
		type = eLOCALudsc64;
		break;
	case	eStation_Usuda642:
		type = eLOCALxdsc54;
		break;
	case	eStation_Uchinoura34:
		type = eLOCALusc34;
		break;
	case	eStation_Uchinoura20:
		type = eLOCALusc20;
		break;
	default:
		return;
	}

	memset(&local, 0, sizeof(stLocalControl));
	ac.ReadLocalControl(local, type);

	local.flag = 0;
	sprintf_s(local.lifetime, mMaxLifetime, "0");
	ac.WriteLocalControl(local, type);
}

void CControlAcquisition::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_strPassword.GetLength() != mMaxPassword){
		MessageBox(_T("入力されたパスワードが8文字ではありません。\n監視モードで起動します。"), MB_OK);
		EndDialog(IDABORT);
		return;
	}
	if (m_strPassword != mGetPassword){
		MessageBox(_T("入力されたパスワードに誤りがあります。\n監視モードで起動します。"), MB_OK);
		EndDialog(IDABORT);
		return;
	}

	// 制御権利を取得
	CFileAccess ac;
	stLocalControl local;
	int type = -1;
	switch (theApp.GetSelectStation()){
	case	eStation_Usuda64:
		type = eLOCALudsc64;
		break;
	case	eStation_Usuda642:
		type = eLOCALxdsc54;
		break;
	case	eStation_Uchinoura34:
		type = eLOCALusc34;
		break;
	case	eStation_Uchinoura20:
		type = eLOCALusc20;
		break;
	default:
		EndDialog(IDABORT);
	}

	ac.ReadLocalControl(local, type);
	local.flag = 1;
	sprintf_s(local.lifetime, mMaxLifetime, "%I64d", mGetTimer);
	ac.WriteLocalControl(local, type);

	CDialogEx::OnOK();
}

void CControlAcquisition::OnBnClickedMode()
{
	EndDialog(IDABORT);
}


void CControlAcquisition::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}

