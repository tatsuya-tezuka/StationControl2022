// OperationProperty.cpp : �����t�@�C��
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
/*! �����_�p�G�f�B�b�g�{�b�N�X

-# �����_�ȉ��̌�����ݒ肷��

@param	len		����
@retval

*/
/*============================================================================*/
void CFloatEdit::SetDecimal(UINT len)
{
	m_nDecimal = len;
}

/*============================================================================*/
/*! �����_�p�G�f�B�b�g�{�b�N�X

-# ���������̃`�F�b�N���s��

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
			// �����_�ʒu�Z�b�g
			dpos = i;
		}
		else if (str.GetAt(i) == 0x2b/*-*/)
		{
			// + �̎��A�������Ȃ�
		}
		else if (str.GetAt(i) == 0x2d/*-*/)
		{
			// - �̎��A�������Ȃ�
		}
		else if (dpos < 0)
		{
			// �����_������܂ł̓J�E���g
			dlen++;
		}
	}

	if (dpos < 0)
	{
		// �����_���Ȃ��ꍇ�̓f�[�^���̈ʒu
		dpos = str.GetLength();
	}

	GetSel(startPos, endPos);
	if (startPos <= dpos && (UINT)dlen >= m_nDecimal - m_nDelta - 1)
	{
		// ���͈ʒu�������ʒu�ł��A���������ɐ��������ȏ�̏ꍇ�A�������I�[�o�[
		return FALSE;
	}

	return TRUE;
}
#endif

/*============================================================================*/
/*! �����_�p�G�f�B�b�g�{�b�N�X

-# ���������̃`�F�b�N���s��

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
			// �����_�ʒu�Z�b�g
			dpos = i;
		}
		else if (str.GetAt(i) == 0x2b/*-*/)
		{
			// + �̎��A�������Ȃ�
		}
		else if (str.GetAt(i) == 0x2d/*-*/)
		{
			// - �̎��A�������Ȃ�
		}
		else if (dpos >= 0)
		{
			// �����_�����Ă���J�E���g
			dlen++;
		}
	}

	if (dpos < 0)
	{
		// �����_���Ȃ��ꍇ�͉������Ȃ�
		return TRUE;	
	}

	GetSel(startPos, endPos);
	if (startPos > dpos && (UINT)dlen >= m_nDelta)
	{
		// ���͈ʒu�������ʒu�ł��A���������ɏ��������ȏ�̏ꍇ
		// �������I�[�o�[
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
/*! �����_�p�G�f�B�b�g�{�b�N�X

-# ���̓f�[�^�̎擾

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
/*! �����_�p�G�f�B�b�g�{�b�N�X

-# �\���f�[�^�̐ݒ�

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
// CFloatEdit ���b�Z�[�W �n���h��

void CFloatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar < '0' || '9' < nChar) && nChar != 0x08/*BS*/ && nChar != 0x2e/*.*/ && nChar != 0x2b/*+*/ && nChar != 0x2d/*-*/){
		MessageBeep((UINT)-1);
		return;
	}

	int startPos, endPos;
	GetSel(startPos, endPos);
	if (nChar == 0x2e){
		// �u.�v��������
		CString	str;
		GetWindowText(str);
		// �S�I������Ă��鎞�́u.�v��OK
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
		// �u+�v��������
		if (m_bUsePlusChar == FALSE || startPos != 0){
			// �u+�v�����֎~���A�܂��͐擪�ʒu�ȊO�͕������͕s��
			MessageBeep((UINT)-1);
			return;
		}
	}
	else if (nChar == 0x2d){
		// �u-�v��������
		if (m_bUseMinusChar == FALSE || startPos != 0){
			// �u-�v�����֎~���A�擪�ʒu�ȊO�͕������͕s��
			MessageBeep((UINT)-1);
			return;
		}
	}
	else if (isdigit(nChar) != FALSE){
		// ���l����
		CString	str;
		GetWindowText(str);
		if (str.GetLength() == (endPos - startPos))
		{
			// �}�E�X�őI�����ꂽ�J�n�ʒu�ƏI���ʒu���S�I������������Ȃ�΁A�������������͕����ŏ㏑���̂��߃`�F�b�N���Ȃ�
		}
		else if (endPos != startPos)
		{
			CString strSelected;
			strSelected = str.Mid(startPos, endPos - startPos);
			// �}�E�X�őI�����ꂽ�����񂪑S�I���ȊO�ŁA���ꕶ���ȏ�̏ꍇ
			if (strSelected.Find(_T(".")) >= 0)
			{
				// �I�𕶎��̒��Ɂu.�v���܂܂�Ă���ꍇ�͐��l���͂ł��Ȃ��悤�ɂ���
				MessageBeep((UINT)-1);
				return;
			}
		}
		else if (CheckDecimal() == FALSE || CheckDelta() == FALSE){
			// �������Ə������̌����`�F�b�N
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

// COperationProperty �_�C�A���O

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


// COperationProperty ���b�Z�[�W �n���h���[


BOOL COperationProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_InitializeControl();
	if (_ReadOpeProp() == true){
		memcpy(&mSaveOpeProp, &mOpeProp, sizeof(stOpeProperty));
	}
	else{
		// �ǂݍ��݂Ɏ��s�����̂Łu���Z�b�g�v�{�^����񊈐�
		GetDlgItem(IDC_BUTTON_PROPRESET)->EnableWindow(FALSE);
	}

	_SetData(mOpeProp);

	_UpdateControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
/*============================================================================*/
/*! �^�p�v���p�e�B

-# �R���g���[���̏�����

@param
@retval �Ȃ�
*/
/*============================================================================*/
void COperationProperty::_InitializeControl()
{
	// �v���p�e�B�ύX�R���g���[��
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
/*! �^�p�v���p�e�B

-# �e��R���g���[���̍X�V

@param
@retval

*/
/*============================================================================*/
void COperationProperty::_UpdateControl()
{
}

/*============================================================================*/
/*! �^�p�v���p�e�B

-# �v���p�e�B�f�[�^�̐ݒ�

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
/*! �^�p�v���p�e�B

-# �e��R���g���[���̍X�V

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
		MessageBox(_T("�V���ʉߊp�x �����0.00����90.00����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	val = _wtof(m_strPropEventSunrise);
	if (val < 0.0 || val > 90.0){
		m_edPropEventSunrise.SetSel(0, -1);
		m_edPropEventSunrise.SetFocus();
		MessageBox(_T("�V���ʉߊp�x ������0.00����90.00����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	val = _wtof(m_strPropMarginUlon);
	if (val < 0.0 || val > 90.0){
		m_edPropMarginUlon.SetSel(0, -1);
		m_edPropMarginUlon.SetFocus();
		MessageBox(_T("�}�[�W���p�x U/L ON��0.00����90.00����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	val = _wtof(m_strPropMarginUloff);
	if (val < 0.0 || val > 90.0){
		m_edPropMarginUloff.SetSel(0, -1);
		m_edPropMarginUloff.SetFocus();
		MessageBox(_T("�}�[�W���p�x U/L OFF��0.00����90.00����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	val = _wtof(m_strPropMarginDl);
	if (val < 0.0 || val > 90.0){
		m_edPropMarginDl.SetSel(0, -1);
		m_edPropMarginDl.SetFocus();
		MessageBox(_T("�}�[�W���p�x D/L��0.00����90.00����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
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
		MessageBox(_T("�q�����MEIRP��0����999.99����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
		return false;
	}
	val = _wtof(m_strExpect);
	if (val < 0 || val > 999.99){
		m_edExpect.SetSel(0, -1);
		m_edExpect.SetFocus();
		MessageBox(_T("�n���ANT������0����999.99����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
		return false;
	}

	// ���ڐݒ�
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
/*! �^�p�v���p�e�B

-# �^�p�v���p�e�B�̓ǂݍ���

@param
@retval

*/
/*============================================================================*/
bool COperationProperty::_ReadOpeProp()
{
#ifdef _OLDTYPE
	// �ǂݍ��݃t�@�C�����̌���
	CString fname = theApp.GetShareFilePath(eFileType_Plan_Folder);
	if (mOpeProp_Equipment.IsEmpty() == true)
		return false;
	fname += mOpeProp_Equipment;
	fname += _T("\\");
	fname += mOpePropFileName;

	FILE *fp = NULL;

	// �Ώۃt�@�C�����I�[�v������
	if ((_wfopen_s(&fp, fname, _T("r")) != 0) || (fp == NULL)){
		return false;
	}

	bool ret = true;
	// ������
	memset(&mOpeProp, 0, sizeof(stOpeProperty));

	TRY
	{
		char szline[2048];
		CString cbuf;
		while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
			cbuf = szline;
			if (cbuf.Find(CString(mSeparator)) >= 0){
				// ��؂蕶���Ȃ̂ŉ������Ȃ�
				continue;
			}
			if (cbuf.Find(_T("COMMON")) >= 0){
				// ���ʍ���
				if (_ReadCommon(&mOpeProp, cbuf) == false)
					ret = false;
				continue;
			}
			else{
				// �q�����̍���

				// �q�����̎擾
				CString strSpace;
				AfxExtractSubString(strSpace, cbuf, 0, ' ');
				if (strSpace.CompareNoCase(mOpeProp_Station) != 0){
					// �Ώۂ̉q���ƈႤ�̂ŉ������Ȃ�
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
/*! �^�p�v���p�e�B

-# �^�p�v���p�e�B�̓ǂݍ���(���ʍ���)

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
/*! �^�p�v���p�e�B

-# �^�p�v���p�e�B�̓ǂݍ���(�q������)

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
/*! �^�p�v���p�e�B

-# �^�p�v���p�e�B�̎��ʎq�̎擾

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
/*! �^�p�v���p�e�B

-# �^�p�v���p�e�B�̂��邤�b������ɑ΂���ԍ��̎擾

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
/*! �^�p�v���p�e�B

-# �^�p�v���p�e�B�̂��邤�b�ԍ��ɑ΂��镶����̎擾

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
		//MessageBox(_T("���͒l�Ɍ�肪����܂��B"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	stOpeProperty& data = ((CTimeLineView*)mpTimeLine)->GetOperationProperty();
	data = mOpeProp;

	CDialogEx::OnOK();
}


void COperationProperty::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}

/*============================================================================*/
/*! �^�p�v���p�e�B

-# �v���p�e�B�ύX�iCID�ؑցj

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

