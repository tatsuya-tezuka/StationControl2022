/*============================================================================*/
/*! CCtrlSearchDlg.cpp

-# ���䖼����
*/
/*============================================================================*/
// CtrlSearchDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "CtrlSearchDlg.h"
#include "afxdialogex.h"


// CCtrlSearchDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CCtrlSearchDlg, CDialogEx)

/*============================================================================*/
/*! CControlRegistDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CCtrlSearchDlg::CCtrlSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCtrlSearchDlg::IDD, pParent)
	, m_strCtrlName(_T(""))
	, m_strCtrlNameSearch(_T(""))
	, m_strCtrlValSearch(_T(""))
{

}

/*============================================================================*/
/*! CControlRegistDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CCtrlSearchDlg::~CCtrlSearchDlg()
{
}

void CCtrlSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CTRLNAME_EDIT, m_strCtrlName);
	DDX_Control(pDX, IDC_CTRLNAME_LIST, m_CtrlNameList);
}


BEGIN_MESSAGE_MAP(CCtrlSearchDlg, CDialogEx)
	ON_EN_CHANGE(IDC_CTRLNAME_EDIT, &CCtrlSearchDlg::OnEnChangeCtrlnameEdit)
	ON_LBN_DBLCLK(IDC_CTRLNAME_LIST, &CCtrlSearchDlg::OnLbnDblclkCtrlnameList)
	ON_BN_CLICKED(IDOK, &CCtrlSearchDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_CTRLNAME_LIST, &CCtrlSearchDlg::OnLbnSelchangeCtrlnameList)
END_MESSAGE_MAP()


// CCtrlSearchDlg ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CCtrlSearchDlg

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CCtrlSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>& list = dba.GetCtlNameList();
	int cnt = (int)list.size();
	for (int i = 0; i < cnt; ++i)
	{
		m_CtrlNameList.InsertString(-1, list[i]);
	}

	ResetList(m_strCtrlName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CCtrlSearchDlg

-# �G�f�B�b�g���͏���

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCtrlSearchDlg::OnEnChangeCtrlnameEdit()
{
	UpdateData();
	ResetList(m_strCtrlName);
}

/*============================================================================*/
/*! CCtrlSearchDlg

-# ���X�g�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCtrlSearchDlg::ResetList(CString name)
{
	m_CtrlNameList.SetRedraw(FALSE);
	m_CtrlNameList.ResetContent();

	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>& list = dba.GetCtlNameList();
	int cnt = (int)list.size();
	CString str;
	name.MakeLower();

	if (name.Find(_T("*")) < 0){
		for (int i = 0; i < cnt; ++i)
		{
			if (name.IsEmpty() == false){
				str = list[i];
				str.MakeLower();
				if (str.Find(name) == 0){
					m_CtrlNameList.InsertString(-1, list[i]);
				}
			}
			else{
				m_CtrlNameList.InsertString(-1, list[i]);
			}
		}
	}
	else
	{
		// ���C���h�J�[�h����
		for (int i = 0; i < cnt; ++i)
		{
			if (name.IsEmpty() == true){
				m_CtrlNameList.InsertString(-1, list[i]);
				continue;
			}

			name.MakeLower();
			TCHAR* pwild = (TCHAR*)(const TCHAR*)name;

			CString str = list[i];
			str.MakeLower();
			TCHAR* pstr = (TCHAR*)(const TCHAR*)str;

			if (WildCard(pwild, pstr) == true){
				// ��������
				m_CtrlNameList.InsertString(-1, list[i]);
			}
		}
	}

	if (m_CtrlNameList.GetCount() == 1)
	{
		// ���X�g���ڂ��P�����Ȃ��ꍇ�A���X�g�I�����Ă���
		m_CtrlNameList.SetCurSel(0);
	}

	m_CtrlNameList.SetRedraw(TRUE);
}

/*============================================================================*/
/*! CCtrlSearchDlg

-# ���X�g�X�V�����i���C���h�J�[�h�����j

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
bool CCtrlSearchDlg::WildCard(TCHAR* pwild, TCHAR* pstr)
{
	enum State {
		Exact,
		AnyRepeat,
	};

	const TCHAR *s = pstr;
	const TCHAR *w = pwild;
	const TCHAR *q = 0;
	int state = 0;

	bool match = true;
	while (match && *w)
	{
		if (*w == '*')
		{
			state = AnyRepeat;
			q = w + 1;
		}
		else
			state = Exact;

		if (*s == 0)
			break;

		switch (state)
		{
		case Exact:
			match = *s == *w;
			s++;
			w++;
			break;

		case AnyRepeat:
			match = true;
			s++;

			if (*s == *q)
				w++;
			break;
		}
	}

	if (state == AnyRepeat)
		return (*s == *q);
	else
		return match && (*s == *w);
}


/*============================================================================*/
/*! CCtrlSearchDlg

-# ���X�g�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCtrlSearchDlg::OnLbnDblclkCtrlnameList()
{
#if 1
	return;
#else
	int nIdx = m_CtrlNameList.GetCurSel();
	if (nIdx == LB_ERR)
		return;

	CString str;
	m_CtrlNameList.GetText(nIdx, str);

	m_strCtrlName = str;

	UpdateData(FALSE);

	ResetList(m_strCtrlName);
#endif
}


/*============================================================================*/
/*! CCtrlSearchDlg

-# ���s�{�^������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCtrlSearchDlg::OnBnClickedOk()
{
	int nIdx = m_CtrlNameList.GetCurSel();
	if (nIdx == LB_ERR)
		return;

	CString strCtrl, strVal;
	m_CtrlNameList.GetText(nIdx, strCtrl);

	stControlValue val;
	memset(&val, 0, sizeof(stControlValue));

	strVal = m_strCtrlValSearch;

	switch (val.type){
	case	eControlStatus:
		sprintf_s(val.buf, 80, "%s", (LPCSTR)CStringA(strVal));
		break;
	case	eControlNumeric:
		strVal.Remove(',');
		val.val = _wtoi(strVal);
		break;
	case	eControlDouble:
		strVal.Remove(',');
		val.fval = _wtof(strVal);
		break;
	case	eControlTime:
		sprintf_s(val.buf, 80, "%s", (LPCSTR)CStringA(strVal));
		break;
	}

	int nGetType = theApp.GetSatelliteData().RegistControlSelect(strCtrl, &val);
	if (val.ret == 0 || nGetType < 0)
	{
		strVal = _T("");
	}
	else
	{
		switch (nGetType){
		case	eControlStatus:
			strVal.Format(_T("%s"), (LPCTSTR)CString(val.buf));		// �X�e�[�^�X����
			break;
		case	eControlNumeric:
			strVal.Format(_T("%s"), (LPCTSTR)CString(val.buf));		// �r�b�g���[�g�o�^�𕶎���Ŏ擾
			break;
		case	eControlDouble:
			strVal.Format(_T("%f"), val.fval);						// ���l����
			break;
		case	eControlTime:
			strVal.Format(_T("%s"), (LPCTSTR)CString(val.buf));		// ����
			break;
		}
	}

	m_strCtrlNameSearch = strCtrl;
	m_strCtrlValSearch = strVal;

	CDialogEx::OnOK();
}

/*============================================================================*/
/*! CCtrlSearchDlg

-# ���X�g�I������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCtrlSearchDlg::OnLbnSelchangeCtrlnameList()
{
	int nIdx = m_CtrlNameList.GetCurSel();
	if (nIdx == LB_ERR)
		return;

	CString strCtrl;
	m_CtrlNameList.GetText(nIdx, strCtrl);

	m_strCtrlName = strCtrl;

	UpdateData(FALSE);
}
