// OperationProperty.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "OperationProperty.h"
#include "afxdialogex.h"
#include "OperationTimeLineDoc.h"
#include "TimeLineView.h"


//=============================================================================
// CFloatEdit
//=============================================================================
IMPLEMENT_DYNAMIC(CFloatEdit, CEdit)

CFloatEdit::CFloatEdit()
{
	m_nDecimal = 4;
	m_MinValue = -FLT_MAX;
	m_MaxValue = FLT_MAX;
	m_nDelta = 1;
	m_bUsePlusChar = TRUE;
	m_bUseMinusChar = TRUE;
}

CFloatEdit::~CFloatEdit()
{
}

/*============================================================================*/
/*! 小数点用エディットボックス

-# 小数点以下の桁数を設定する

@param	len		桁数
@retval

*/
/*============================================================================*/
void CFloatEdit::SetDecimal(UINT len)
{
	m_nDecimal = len;
}

/*============================================================================*/
/*! 小数点用エディットボックス

-# 整数部分のチェックを行う

@param
@retval

*/
/*============================================================================*/
BOOL CFloatEdit::CheckDecimal()
#if 0
{
	if (m_nDecimal == 0)
		return TRUE;

	int dpos = -1, dlen = 0;
	int	startPos, endPos;
	CString	str;
	GetWindowText(str);
	for (int i = 0; i<str.GetLength(); i++){
		if (dpos >= 0)
			dlen++;

		if (str.GetAt(i) == 0x2e/*.*/)
			dpos = i;
	}
	if (dpos < 0)
		return TRUE;

	GetSel(startPos, endPos);
	if (startPos > dpos && m_nDecimal <= (UINT)dlen)
		return FALSE;

	return TRUE;
}
#else
{
	int dpos = -1, dlen = 0;
	int	startPos, endPos;
	CString	str;
	GetWindowText(str);

	if (str.IsEmpty())
		return TRUE;

	for (int i = 0; i < str.GetLength() && dpos < 0; i++){
		if (str.GetAt(i) == 0x2e/*.*/)
		{
			// 小数点位置セット
			dpos = i;
		}
		else if (str.GetAt(i) == 0x2b/*-*/)
		{
			// + の時、何もしない
		}
		else if (str.GetAt(i) == 0x2d/*-*/)
		{
			// - の時、何もしない
		}
		else if (dpos < 0)
		{
			// 小数点が来るまではカウント
			dlen++;
		}
	}

	if (dpos < 0)
	{
		// 小数点がない場合はデータ長の位置
		dpos = str.GetLength();
	}

	GetSel(startPos, endPos);
	if (startPos <= dpos && (UINT)dlen >= m_nDecimal - m_nDelta - 1)
	{
		// 入力位置が整数位置でかつ、桁数が既に整数桁数以上の場合、桁数がオーバー
		return FALSE;
	}

	return TRUE;
}
#endif

/*============================================================================*/
/*! 小数点用エディットボックス

-# 小数部分のチェックを行う

@param
@retval

*/
/*============================================================================*/
BOOL CFloatEdit::CheckDelta()
{
	int dpos = -1, dlen = 0;
	int	startPos, endPos;
	CString	str;
	GetWindowText(str);

	if (str.IsEmpty())
		return TRUE;

	for (int i = 0; i < str.GetLength(); i++){
		if (str.GetAt(i) == 0x2e/*.*/)
		{
			// 小数点位置セット
			dpos = i;
		}
		else if (str.GetAt(i) == 0x2b/*-*/)
		{
			// + の時、何もしない
		}
		else if (str.GetAt(i) == 0x2d/*-*/)
		{
			// - の時、何もしない
		}
		else if (dpos >= 0)
		{
			// 小数点が来てからカウント
			dlen++;
		}
	}

	if (dpos < 0)
	{
		// 小数点がない場合は何もしない
		return TRUE;	
	}

	GetSel(startPos, endPos);
	if (startPos > dpos && (UINT)dlen >= m_nDelta)
	{
		// 入力位置が小数位置でかつ、桁数が既に小数桁数以上の場合
		// 桁数がオーバー
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
/*! 小数点用エディットボックス

-# 入力データの取得

@param
@retval

*/
/*============================================================================*/
float CFloatEdit::GetValue() const
{
	const TCHAR szFormat[] = _T("%f");

	if (m_MinValue == -FLT_MAX)
		return 0.0f;

	float f = m_MinValue;
	CString str;
	GetWindowText(str);
	swscanf_s(str, szFormat, &f);
	return f;
}

/*============================================================================*/
/*! 小数点用エディットボックス

-# 表示データの設定

@param
@retval

*/
/*============================================================================*/
BOOL CFloatEdit::SetValue(float val)
{
	if (m_MinValue == -FLT_MAX)
		return FALSE;

	if (val > m_MaxValue || val < m_MinValue)
		return FALSE;

	CString str;
	if (m_nDelta == 0)
		str.Format(_T("%.0f"), val);
	else if (m_nDelta == 1)
		str.Format(_T("%.1f"), val);
	else
		str.Format(_T("%.2f"), val);
	SetWindowText(str);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
	//{{AFX_MSG_MAP(CFloatEdit)
	ON_WM_CHAR()
	ON_WM_NCDESTROY()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatEdit メッセージ ハンドラ

void CFloatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar < '0' || '9' < nChar) && nChar != 0x08/*BS*/ && nChar != 0x2e/*.*/ && nChar != 0x2b/*+*/ && nChar != 0x2d/*-*/){
		MessageBeep((UINT)-1);
		return;
	}

	int startPos, endPos;
	GetSel(startPos, endPos);
	if (nChar == 0x2e){
		// 「.」文字入力
		CString	str;
		GetWindowText(str);
		// 全選択されている時の「.」はOK
		if (str.GetLength() != (endPos - startPos)){
			for (int i = 0; i<str.GetLength(); i++){
				if (str.GetAt(i) == 0x2e){
					MessageBeep((UINT)-1);
					return;
				}
			}
		}
	}
	else if (nChar == 0x2b){
		// 「+」文字入力
		if (m_bUsePlusChar == FALSE || startPos != 0){
			// 「+」文字禁止時、または先頭位置以外は符号入力不可
			MessageBeep((UINT)-1);
			return;
		}
	}
	else if (nChar == 0x2d){
		// 「-」文字入力
		if (m_bUseMinusChar == FALSE || startPos != 0){
			// 「-」文字禁止時、先頭位置以外は符号入力不可
			MessageBeep((UINT)-1);
			return;
		}
	}
	else if (isdigit(nChar) != FALSE){
		// 数値入力
		CString	str;
		GetWindowText(str);
		if (str.GetLength() == (endPos - startPos))
		{
			// マウスで選択された開始位置と終了位置が全選択した文字列ならば、既存文字列を入力文字で上書きのためチェックしない
		}
		else if (endPos != startPos)
		{
			CString strSelected;
			strSelected = str.Mid(startPos, endPos - startPos);
			// マウスで選択された文字列が全選択以外で、かつ一文字以上の場合
			if (strSelected.Find(_T(".")) >= 0)
			{
				// 選択文字の中に「.」が含まれている場合は数値入力できないようにする
				MessageBeep((UINT)-1);
				return;
			}
		}
		else if (CheckDecimal() == FALSE || CheckDelta() == FALSE){
			// 整数部と小数部の桁数チェック
			MessageBeep((UINT)-1);
			return;
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CFloatEdit::OnNcDestroy()
{
	CEdit::OnNcDestroy();
}

void CFloatEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
}

BOOL CFloatEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == this->m_hWnd && pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			pMsg->wParam = VK_TAB;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

// COperationProperty ダイアログ

IMPLEMENT_DYNAMIC(COperationProperty, CDialogEx)

COperationProperty::COperationProperty(CWnd* pParent)
	: CDialogEx(COperationProperty::IDD, pParent)
	, m_nLeapYear(0)
	, m_nLeapSec(0)
	, m_nAutoCalc(0)
{
	mpTimeLine = pParent;
	mOpeProp_Station.Empty();
	mOpeProp_Equipment.Empty();
	mOpeProp_TxtMode = XTX_N_ID;
}

COperationProperty::~COperationProperty()
{
}

void COperationProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_PROPEVENTAUTOVAL, m_nOptionEvent);
	DDX_Control(pDX, IDC_EDIT_PROPEVENTSUNSET, m_edPropEventSunset);
	DDX_Text(pDX, IDC_EDIT_PROPEVENTSUNSET, m_strPropEventSunset);
//	DDV_MinMaxUInt(pDX, m_nPropEventSunset, 0, 90);
	DDX_Control(pDX, IDC_EDIT_PROPEVENTSUNRISE, m_edPropEventSunrise);
	DDX_Text(pDX, IDC_EDIT_PROPEVENTSUNRISE, m_strPropEventSunrise);
//	DDV_MinMaxUInt(pDX, m_nPropEventSunrise, 0, 90);
	DDX_Control(pDX, IDC_EDIT_PROPMARGINULON, m_edPropMarginUlon);
	DDX_Text(pDX, IDC_EDIT_PROPMARGINULON, m_strPropMarginUlon);
//	DDV_MinMaxUInt(pDX, m_nPropMarginUlon, 0, 90);
	DDX_Control(pDX, IDC_EDIT_PROPMARGINULOFF, m_edPropMarginUloff);
	DDX_Text(pDX, IDC_EDIT_PROPMARGINULOFF, m_strPropMarginUloff);
//	DDV_MinMaxUInt(pDX, m_nPropMarginUloff, 0, 90);
	DDX_Control(pDX, IDC_EDIT_PROPMARGINDL, m_edPropMarginDl);
	DDX_Text(pDX, IDC_EDIT_PROPMARGINDL, m_strPropMarginDl);
//	DDV_MinMaxUInt(pDX, m_nPropMarginDl, 0, 90);
	DDX_Control(pDX, IDC_EDIT_PROPULCMD, m_edPropUlCmd);
	DDX_Text(pDX, IDC_EDIT_PROPULCMD, m_nPropUlCmd);
	DDV_MinMaxUInt(pDX, m_nPropUlCmd, 0, 999);
	DDX_Control(pDX, IDC_EDIT_PROPULRNG, m_edPropUlRng);
	DDX_Text(pDX, IDC_EDIT_PROPULRNG, m_nPropUlRng);
	DDV_MinMaxUInt(pDX, m_nPropUlRng, 0, 999);
	DDX_CBIndex(pDX, IDC_COMBO_PROPSPACEVAL, m_nSmdRng);
	DDX_CBIndex(pDX, IDC_COMBO_LEAPYEAR, m_nLeapYear);
	DDX_CBIndex(pDX, IDC_COMBO_LEAPSEC, m_nLeapSec);
	DDX_CBIndex(pDX, IDC_COMBO_RECLEVEL, m_nAutoCalc);
	DDX_Text(pDX, IDC_EDIT_EIRP, m_strEirp);
	DDX_Text(pDX, IDC_EDIT_EXPECT, m_strExpect);
	DDX_Control(pDX, IDC_EDIT_EIRP, m_edEirp);
	DDX_Control(pDX, IDC_EDIT_EXPECT, m_edExpect);
}


BEGIN_MESSAGE_MAP(COperationProperty, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PROPRESET, &COperationProperty::OnBnClickedButtonPropreset)
	ON_BN_CLICKED(IDOK, &COperationProperty::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COperationProperty::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_PROPCID, &COperationProperty::OnBnClickedButtonPropcid)
	ON_CBN_SELCHANGE(IDC_COMBO_RECLEVEL, &COperationProperty::OnCbnSelchangeComboReclevel)
END_MESSAGE_MAP()


// COperationProperty メッセージ ハンドラー


BOOL COperationProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_InitializeControl();
	if (_ReadOpeProp() == true){
		memcpy(&mSaveOpeProp, &mOpeProp, sizeof(stOpeProperty));
	}
	else{
		// 読み込みに失敗したので「リセット」ボタンを非活性
		GetDlgItem(IDC_BUTTON_PROPRESET)->EnableWindow(FALSE);
	}

	_SetData(mOpeProp);

	_UpdateControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
/*============================================================================*/
/*! 運用プロパティ

-# コントロールの初期化

@param
@retval なし
*/
/*============================================================================*/
void COperationProperty::_InitializeControl()
{
	// プロパティ変更コントロール
	m_edPropEventSunset.SetDecimal(5);
	m_edPropEventSunset.SetDelta(2);
	m_edPropEventSunset.SetUsePlusMinusChar(FALSE, FALSE);

	m_edPropEventSunrise.SetDecimal(5);
	m_edPropEventSunrise.SetDelta(2);
	m_edPropEventSunrise.SetUsePlusMinusChar(FALSE, FALSE);

	m_edPropMarginUlon.SetDecimal(5);
	m_edPropMarginUlon.SetDelta(2);
	m_edPropMarginUlon.SetUsePlusMinusChar(FALSE, FALSE);

	m_edPropMarginUloff.SetDecimal(5);
	m_edPropMarginUloff.SetDelta(2);
	m_edPropMarginUloff.SetUsePlusMinusChar(FALSE, FALSE);

	m_edPropMarginDl.SetDecimal(5);
	m_edPropMarginDl.SetDelta(2);
	m_edPropMarginDl.SetUsePlusMinusChar(FALSE, FALSE);

	m_edEirp.SetDecimal(6);
	m_edEirp.SetDelta(2);
	m_edEirp.SetUsePlusMinusChar(FALSE, FALSE);

	m_edExpect.SetDecimal(6);
	m_edExpect.SetDelta(2);
	m_edExpect.SetUsePlusMinusChar(FALSE, FALSE);
}

/*============================================================================*/
/*! 運用プロパティ

-# 各種コントロールの更新

@param
@retval

*/
/*============================================================================*/
void COperationProperty::_UpdateControl()
{
}

/*============================================================================*/
/*! 運用プロパティ

-# プロパティデータの設定

@param
@retval

*/
/*============================================================================*/
void COperationProperty::_SetData(stOpeProperty& data)
{
	m_nOptionEvent = data.event_autocalc;
	m_nSmdRng = data.opespace;
	m_nLeapYear = data.init.leapyear;
	m_nLeapSec = data.init.leapsec;
	m_nAutoCalc = data.init.autocalc;
	m_strPropEventSunset.Format(_T("%2.2lf"), data.even_sunset);
	m_strPropEventSunrise.Format(_T("%2.2lf"), data.event_sunrise);
	m_strPropMarginUlon.Format(_T("%2.2lf"), data.mg_ULon);
	m_strPropMarginUloff.Format(_T("%2.2lf"), data.mg_ULoff);
	m_strPropMarginDl.Format(_T("%2.2lf"), data.mg_DL);
	m_nPropUlCmd = data.of_CMD;
	m_nPropUlRng = data.of_RNG;
	m_strEirp.Format(_T("%3.2lf"), data.init.eirp);
	m_strExpect.Format(_T("%3.2lf"), data.init.gain);
	GetDlgItem(IDC_BUTTON_PROPCID)->SetWindowText(CString(data.cid));

	UpdateData(FALSE);
}

/*============================================================================*/
/*! 運用プロパティ

-# 各種コントロールの更新

@param
@retval

*/
/*============================================================================*/
bool COperationProperty::_CheckData()
{
	UpdateData(TRUE);

	double val;

	val = _wtof(m_strPropEventSunset);
	if (val < 0.0 || val > 90.0){
		m_edPropEventSunset.SetSel(0, -1);
		m_edPropEventSunset.SetFocus();
		MessageBox(_T("天頂通過角度 入りは0.00から90.00を入力してください。"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	val = _wtof(m_strPropEventSunrise);
	if (val < 0.0 || val > 90.0){
		m_edPropEventSunrise.SetSel(0, -1);
		m_edPropEventSunrise.SetFocus();
		MessageBox(_T("天頂通過角度 明けは0.00から90.00を入力してください。"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	val = _wtof(m_strPropMarginUlon);
	if (val < 0.0 || val > 90.0){
		m_edPropMarginUlon.SetSel(0, -1);
		m_edPropMarginUlon.SetFocus();
		MessageBox(_T("マージン角度 U/L ONは0.00から90.00を入力してください。"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	val = _wtof(m_strPropMarginUloff);
	if (val < 0.0 || val > 90.0){
		m_edPropMarginUloff.SetSel(0, -1);
		m_edPropMarginUloff.SetFocus();
		MessageBox(_T("マージン角度 U/L OFFは0.00から90.00を入力してください。"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	val = _wtof(m_strPropMarginDl);
	if (val < 0.0 || val > 90.0){
		m_edPropMarginDl.SetSel(0, -1);
		m_edPropMarginDl.SetFocus();
		MessageBox(_T("マージン角度 D/Lは0.00から90.00を入力してください。"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	if (m_nPropUlCmd < 0 || m_nPropUlCmd > 999){
		m_edPropUlCmd.SetSel(0, -1);
		m_edPropUlCmd.SetFocus();
		return false;
	}
	if (m_nPropUlRng < 0 || m_nPropUlRng > 999){
		m_edPropUlRng.SetSel(0, -1);
		m_edPropUlRng.SetFocus();
		return false;
	}

	val = _wtof(m_strEirp);
	if (val < 0 || val > 999.99){
		m_edEirp.SetSel(0, -1);
		m_edEirp.SetFocus();
		MessageBox(_T("衛星送信EIRPは0から999.99を入力してください。"), 0, MB_OK | MB_ICONERROR);
		return false;
	}
	val = _wtof(m_strExpect);
	if (val < 0 || val > 999.99){
		m_edExpect.SetSel(0, -1);
		m_edExpect.SetFocus();
		MessageBox(_T("地上局ANT利得は0から999.99を入力してください。"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	// 項目設定
	mOpeProp.event_autocalc = m_nOptionEvent;
	mOpeProp.opespace = m_nSmdRng;
	mOpeProp.init.leapyear = m_nLeapYear;
	mOpeProp.init.leapsec = m_nLeapSec;
	mOpeProp.init.autocalc = m_nAutoCalc;
	mOpeProp.even_sunset = _wtof(m_strPropEventSunset);
	mOpeProp.event_sunrise = _wtof(m_strPropEventSunrise);
	mOpeProp.mg_ULon = _wtof(m_strPropMarginUlon);
	mOpeProp.mg_ULoff = _wtof(m_strPropMarginUloff);
	mOpeProp.mg_DL = _wtof(m_strPropMarginDl);
	mOpeProp.of_CMD = m_nPropUlCmd;
	mOpeProp.of_RNG = m_nPropUlRng;
	mOpeProp.init.eirp = _wtof(m_strEirp);
	mOpeProp.init.gain = _wtof(m_strExpect);
	CString cid;
	GetDlgItem(IDC_BUTTON_PROPCID)->GetWindowText(cid);
	sprintf_s(mOpeProp.cid, 30, "%s", (LPCSTR)CStringA(cid));

	return true;
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティの読み込み

@param
@retval

*/
/*============================================================================*/
bool COperationProperty::_ReadOpeProp()
{
#ifdef _OLDTYPE
	// 読み込みファイル名の決定
	CString fname = theApp.GetShareFilePath(eFileType_Plan_Folder);
	if (mOpeProp_Equipment.IsEmpty() == true)
		return false;
	fname += mOpeProp_Equipment;
	fname += _T("\\");
	fname += mOpePropFileName;

	FILE *fp = NULL;

	// 対象ファイルをオープンする
	if ((_wfopen_s(&fp, fname, _T("r")) != 0) || (fp == NULL)){
		return false;
	}

	bool ret = true;
	// 初期化
	memset(&mOpeProp, 0, sizeof(stOpeProperty));

	TRY
	{
		char szline[2048];
		CString cbuf;
		while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
			cbuf = szline;
			if (cbuf.Find(CString(mSeparator)) >= 0){
				// 区切り文字なので何もしない
				continue;
			}
			if (cbuf.Find(_T("COMMON")) >= 0){
				// 共通項目
				if (_ReadCommon(&mOpeProp, cbuf) == false)
					ret = false;
				continue;
			}
			else{
				// 衛星毎の項目

				// 衛星名の取得
				CString strSpace;
				AfxExtractSubString(strSpace, cbuf, 0, ' ');
				if (strSpace.CompareNoCase(mOpeProp_Station) != 0){
					// 対象の衛星と違うので何もしない
					continue;
				}
				if (_ReadSpace(&mOpeProp, cbuf) == false)
					ret = false;
			}
		}
	}CATCH_ALL(e){
		ret = false;
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);
#else
	bool ret = true;
	mOpeProp = ((CTimeLineView*)mpTimeLine)->GetOperationProperty();
#endif
	return ret;
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティの読み込み(共通項目)

@param
@retval

*/
/*============================================================================*/
bool COperationProperty::_ReadCommon(stOpeProperty* pdata, CString& buf)
{
#ifdef _OLDTYPE
	CString temp;
	AfxExtractSubString(temp, buf, 1, ' ');
	int index = _GetIdentifier(temp);
	if (index < 0)
		return false;

	int pos = buf.Find(CString(mIdentifier[index]));
	temp = buf.Mid(pos + (int)strlen(mIdentifier[index])+1);
	temp.TrimLeft();
	temp.TrimRight();

	switch (index){
	case	eLEAP_YEAR:
		if (temp == _T("OFF"))
			pdata->init.leapyear = 0;
		else
			pdata->init.leapyear = 1;
		break;
	case	eLEAP_SEC:
		if (temp == _T("OFF"))
			pdata->init.leapsec = 0;
		else{
			pdata->init.leapsec = 0;
			if (temp == _T("1"))	pdata->init.leapsec = 1;
			if (temp == _T("4"))	pdata->init.leapsec = 2;
			if (temp == _T("7"))	pdata->init.leapsec = 3;
			if (temp == _T("10"))	pdata->init.leapsec = 4;
		}
		break;
	case	eGAIN:
		pdata->init.gain = _wtof(temp);
		break;
	default:
		return false;
	}
#endif
	return true;
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティの読み込み(衛星項目)

@param
@retval

*/
/*============================================================================*/
bool COperationProperty::_ReadSpace(stOpeProperty* pdata, CString& buf)
{
#ifdef _OLDTYPE
	CString temp;
	AfxExtractSubString(temp, buf, 1, ' ');
	int index = _GetIdentifier(temp);
	if (index < 0)
		return false;

	int pos = buf.Find(CString(mIdentifier[index]));
	temp = buf.Mid(pos + (int)strlen(mIdentifier[index]) + 1);
	temp.TrimLeft();
	temp.TrimRight();

	switch (index){
	case	eEVENT_CALC:
		if (temp == _T("OFF"))
			pdata->event_autocalc = 0;
		else
			pdata->event_autocalc = 1;
		break;
	case	eZENITH_IN:
		pdata->even_sunset = _wtoi(temp);
		break;
	case	eZENITH_OUT:
		pdata->event_sunrise = _wtoi(temp);
		break;
	case	eMARGIN_UP_ON:
		pdata->mg_ULon = _wtoi(temp);
		break;
	case	eMARGIN_UP_OFF:
		pdata->mg_ULoff = _wtoi(temp);
		break;
	case	eMARGIN_DOWN:
		pdata->mg_DL = _wtoi(temp);
		break;
	case	eOFFSET_CMD_END:
		pdata->of_CMD = _wtoi(temp);
		break;
	case	eOFFSET_RNG_END:
		pdata->of_RNG = _wtoi(temp);
		break;
	case	eDEFAULT_CID:
		sprintf_s(pdata->cid, 30, "%s", (LPCSTR)CStringA(temp));
		break;
	case	eSAME_TIME_MOD:
		if (temp == _T("OFF"))
			pdata->opespace = 0;
		else
			pdata->opespace = 1;
		break;
	case	eEXPECT_LEVEL:
		if (temp == _T("FIXED"))
			pdata->init.autocalc = 1;
		else
			pdata->init.autocalc = 0;
		break;
	case	eEXPECT_LEVEL_FIXED_S:
	case	eEXPECT_LEVEL_FIXED_X:
		pdata->init.reclevel = _wtof(temp);
		break;
	case	eEIRP:
		pdata->init.eirp = _wtof(temp);
		break;
	case	eSUBSARR_TLM:
		pdata->init.tlmflag = true;
		pdata->init.tlm = _wtof(temp);
		break;
	case	eSUBSARR_CMD:
		pdata->init.cmdflag = true;
		pdata->init.cmd = _wtof(temp);
		break;
	default:
		return false;
	}
#endif
	return true;
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティの識別子の取得

@param
@retval

*/
/*============================================================================*/
int COperationProperty::_GetIdentifier(CString& buf)
{
	for (int i = 0; i < sizeof(mIdentifier) / sizeof(mIdentifier[0]); i++){
		if (buf.CompareNoCase(CString(mIdentifier[i])) == 0){
			return i;
		}
	}
	return -1;
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティのうるう秒文字列に対する番号の取得

@param
@retval

*/
/*============================================================================*/
int COperationProperty::_GetLeapSecNo(CString& buf)
{
	for (int i = 0; i < sizeof(mLeapSecString) / sizeof(mLeapSecString[0]); i++){
		if (buf.CompareNoCase(CString(mLeapSecString[i])) == 0){
			return i;
		}
	}
	return -1;
}

/*============================================================================*/
/*! 運用プロパティ

-# 運用プロパティのうるう秒番号に対する文字列の取得

@param
@retval

*/
/*============================================================================*/
CString COperationProperty::_GetLeapSecString(int val)
{
	CString str = mLeapSecString[val];
	return str;
}

void COperationProperty::OnBnClickedButtonPropreset()
{
	_SetData(mSaveOpeProp);
}

void COperationProperty::OnBnClickedOk()
{
	if (_CheckData() == false){
		//MessageBox(_T("入力値に誤りがあります。"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	stOpeProperty& data = ((CTimeLineView*)mpTimeLine)->GetOperationProperty();
	data = mOpeProp;

	CDialogEx::OnOK();
}


void COperationProperty::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}

/*============================================================================*/
/*! 運用プロパティ

-# プロパティ変更（CID切替）

@param
@retval

*/
/*============================================================================*/
void COperationProperty::OnBnClickedButtonPropcid()
{
	timeline::CCidSelect dlg;
	theApp.SetDlgMap(_T("timeline::CCidSelect"), (CWnd*)&dlg);
	CString& str = theApp.GetTargetSpaceName();
	dlg.SetSatelliteName(str);
	dlg.SetTxtMode(mOpeProp_TxtMode);
	if (dlg.DoModal() == IDCANCEL)
	{
		theApp.DeleteDlgMap(_T("timeline::CCidSelect"));
		return;
	}
	theApp.DeleteDlgMap(_T("timeline::CCidSelect"));

	GetDlgItem(IDC_BUTTON_PROPCID)->SetWindowText(CString(dlg.m_strCIDCmb));
	sprintf_s(mOpeProp.cid, "%s", (LPCSTR)CStringA(dlg.m_strCIDCmb));
}

void COperationProperty::OnCbnSelchangeComboReclevel()
{
	UpdateData(TRUE);
	_UpdateControl();
}

