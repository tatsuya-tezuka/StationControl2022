// CMonSearchDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "MonSearchDlg.h"
#include "afxdialogex.h"


// CMonSearchDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CMonSearchDlg, CDialogEx)

CMonSearchDlg::CMonSearchDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMonSearchDlg::IDD, pParent)
, m_strMonName(_T(""))
{

}

CMonSearchDlg::~CMonSearchDlg()
{
}

void CMonSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MONNAME_EDIT, m_strMonName);
	DDX_Control(pDX, IDC_MONNAME_LIST, m_MonNameList);
}


BEGIN_MESSAGE_MAP(CMonSearchDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MONNAME_EDIT, &CMonSearchDlg::OnEnChangeMonnameEdit)
	ON_LBN_DBLCLK(IDC_MONNAME_LIST, &CMonSearchDlg::OnLbnDblclkMonnameList)
	ON_BN_CLICKED(IDOK, &CMonSearchDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMonSerachDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CMonSearchDlg

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CMonSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>& mList = dba.GetMonNameList();
	int cnt = (int)mList.size();
	for (int i = 0; i < cnt; ++i)
	{
		m_MonNameList.InsertString(-1, mList[i]);
	}

	ResetList(m_strMonName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CMonSearchDlg

-# �G�f�B�b�g���͏���

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CMonSearchDlg::OnEnChangeMonnameEdit()
{
	UpdateData();
//	TRACE("%s\n", CStringA(m_strMonName));
	ResetList(m_strMonName);
}

/*============================================================================*/
/*! CMonSearchDlg

-# ���X�g�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CMonSearchDlg::ResetList(CString name)
{
	m_MonNameList.SetRedraw(FALSE);
	m_MonNameList.ResetContent();

	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>& mList = dba.GetMonNameList();
	int cnt = (int)mList.size();
	CString str;
	name.MakeLower();

	if (name.Find(_T("*")) < 0){
		for (int i = 0; i < cnt; ++i)
		{
			if (name.IsEmpty() == false){
				str = mList[i];
				str.MakeLower();
				if (str.Find(name) == 0){
					m_MonNameList.InsertString(-1, mList[i]);
				}
			}
			else{
				m_MonNameList.InsertString(-1, mList[i]);
			}
		}
	}
	else
	{
		// ���C���h�J�[�h����
		for (int i = 0; i < cnt; ++i)
		{
			if (name.IsEmpty() == true){
				m_MonNameList.InsertString(-1, mList[i]);
				continue;
			}

			name.MakeLower();
			TCHAR* pwild = (TCHAR*)(const TCHAR*)name;

			CString str = mList[i];
			str.MakeLower();
			TCHAR* pstr = (TCHAR*)(const TCHAR*)str;

			if (WildCard(pwild, pstr) == true){
				// ��������
				m_MonNameList.InsertString(-1, mList[i]);
			}
		}
	}

	m_MonNameList.SetRedraw(TRUE);
}

/*============================================================================*/
/*! CMonSearchDlg

-# ���X�g�X�V�����i���C���h�J�[�h�����j

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
bool CMonSearchDlg::WildCard(TCHAR* pwild, TCHAR* pstr)
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
/*! CMonSearchDlg

-# ���X�g�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CMonSearchDlg::OnLbnDblclkMonnameList()
{
#if 1
	return;
#else
	int nIdx = m_MonNameList.GetCurSel();
	if (nIdx == LB_ERR)
		return;

	CString str;
	m_MonNameList.GetText(nIdx, str);

	m_strMonName = str;

	UpdateData(FALSE);

	ResetList(m_strMonName);
#endif
}


/*============================================================================*/
/*! CMonSearchDlg

-# ���s�{�^������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CMonSearchDlg::OnBnClickedOk()
{
	int nIdx = m_MonNameList.GetCurSel();
	if (nIdx == LB_ERR)
		return;

	CString str;
	m_MonNameList.GetText(nIdx, str);

	m_MonNameSearch = str;

	CDialogEx::OnOK();
}
