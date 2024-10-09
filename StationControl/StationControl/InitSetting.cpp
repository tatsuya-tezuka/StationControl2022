/*============================================================================*/
/*! InitSetting.cpp

-# �����ݒ���
*/
/*============================================================================*/
// InitSetting.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "InitSetting.h"
#include "afxdialogex.h"
#include "CidSelect.h"

#define CID_CTRL _T("INIT.CID")


// CInitSetting �_�C�A���O

IMPLEMENT_DYNAMIC(CInitSetting, CDialogBase)

/*============================================================================*/
/*! CInitSetting

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CInitSetting::CInitSetting(CWnd* pParent /*=NULL*/)
: CDialogBase(CInitSetting::IDD, pParent)
	, m_CidChkBox(FALSE)
{

}

/*============================================================================*/
/*! CInitSetting

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CInitSetting::~CInitSetting()
{
}

void CInitSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INITSETTING_COMBO1, m_CCBoxSatelliteName);
	DDX_Control(pDX, IDC_INITSETTING_CID_BUTTON, m_CButonSelCid);
	DDX_Control(pDX, IDC_INITSETTING_LIST1, m_listInitSetting);
	DDX_Control(pDX, IDC_INITSETING_CHECK, m_chkAllSelect);
	DDX_Check(pDX, IDC_CIDCHECK, m_CidChkBox);
}


BEGIN_MESSAGE_MAP(CInitSetting, CDialogBase)
	ON_BN_CLICKED(IDOK, &CInitSetting::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_INITSETTING_COMBO1, &CInitSetting::OnCbnSelchangeInitsettingCombo1)
	ON_BN_CLICKED(IDC_INITSETTING_CID_BUTTON, &CInitSetting::OnBnClickedInitsettingCidButton)
	ON_BN_CLICKED(IDC_INITSETING_CHECK, &CInitSetting::OnBnClickedInitsetingCheck)
	ON_BN_CLICKED(IDC_CIDCHECK, &CInitSetting::OnBnClickedCidcheck)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CInitSetting ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CInitSetting

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CInitSetting::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	struct satinf_t *satinf_adr;
	char			*sattbl_adr;
//	struct satinf_t *original_adr;
	struct mac_t	*mac_t_adr = NULL;

	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	m_listSatellite.clear();
	m_listInitMacr.clear();

	vector<CString> initList;
	int cnt = (int)saNameList.size();
	for (int i = 0; i < cnt; i++)
	{
		/*--------------------------*/
		/* �q���{�̏��A�h���X�擾 */
		CString satname = saNameList[i];

		if (satname == _T("DEFAULT") || satname == _T("�c�d�e�`�t�k�s"))
		{
			continue;
		}

		satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
		sattbl_adr = (char *)satinf_adr;	/* char�^�ɕϊ�                   */

		m_listSatellite.push_back(satname);
		initList.clear();

		/*----------------------------------*/
		/* INIT. */
		/*----------------------------------*/
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_t�ɕϊ�*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*���ʂh�c���I���̏ꍇ*/
				break;
			}
			if (satinf_adr->l_id == MAC_ID)
			{
				/*���ʂh�c���̏ꍇ*/
				mac_t_adr = (struct mac_t *)satinf_adr;

				CString mac = CString(mac_t_adr->sz_macname);
				if (mac.Find(_T("INIT.")) == 0)
				{
					initList.push_back(mac);
				}
			}
			sattbl_adr += satinf_adr->l_len;
		}
		m_listInitMacr[satname] = initList;
	}

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CInitSetting

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CInitSetting::UpdateDlg()
{
	InitDlg();
	return TRUE;
}


/*============================================================================*/
/*! CInitSetting

-# �����ݒ�}�N����`���X�g�Đݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CInitSetting::RestListBox(CString sat)
{
	m_listInitSetting.SetRedraw(FALSE);
	m_listInitSetting.ResetContent();

	vector<CString> &list = m_listInitMacr[sat];
	for (int i = 0; i < (int)list.size(); ++i)
	{
		m_listInitSetting.InsertString(-1, list[i]);
	}
	m_listInitSetting.SetRedraw(TRUE);
}
	
/*============================================================================*/
/*! CInitSetting

-# �����ݒ�}�N����`���X�g�Đݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CInitSetting::RestCIDBtn(CString sat)
{
	vector<CString> &list = theApp.GetSatelliteData().m_CIDSlctData.GetCidList(sat);

	if (list.size() > 0)
	{
#if 1
		stOpeProp stProp;
		theApp.GetSatelliteData().ReadPropertyFile(sat, stProp);
		CString strCid = CString(stProp.cid);
		auto itrl = std::find(list.begin(), list.end(), strCid);
		if (itrl == list.end())
		{
			// �f�t�H���gCID��CID�t�@�C���ɖ����ꍇ�A��ԍŏ���CID���Z�b�g�B
			strCid = list[0];
		}

		m_CButonSelCid.SetWindowTextW(strCid);
#else
		// �擪��CID���Z�b�g
		m_CButonSelCid.SetWindowTextW(list[0]);
#endif
		m_CButonSelCid.EnableWindow(TRUE);
	}
	else
	{
		// CID�t�@�C��������
		m_CButonSelCid.SetWindowTextW(_T("N/A"));
		m_CButonSelCid.EnableWindow(FALSE);
	}
}

/*============================================================================*/
/*! CInitSetting

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CInitSetting::InitDlg()
{
	// �q���R���{�{�b�N�X�ݒ�
#if 1
	for (int i = 0; i < (int)m_listSatellite.size(); ++i)
	{
		m_CCBoxSatelliteName.InsertString(-1, m_listSatellite[i]);
	}
#else
	for (auto itr = m_listInitMacr.begin(); itr != m_listInitMacr.end(); ++itr)
	{
		CString sat = itr->first;
		m_CCBoxSatelliteName.InsertString(-1, sat);
	}
#endif
	m_CCBoxSatelliteName.SetCurSel(0);

	// �����ݒ胊�X�g�{�b�N�X�ݒ�
	CString str;
	m_CCBoxSatelliteName.GetLBText(0, str);
	RestListBox(str);

	// CID�`�F�b�N�f�t�H���gOFF
	m_CidChkBox = FALSE;

	// CID�X�V
	RestCIDBtn(str);
	return TRUE;
}


/*============================================================================*/
/*! CInitSetting

-# ���s�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CInitSetting::OnBnClickedOk()
{
	// �q�����擾
	m_CCBoxSatelliteName.GetLBText(m_CCBoxSatelliteName.GetCurSel(), m_satSel);

	UpdateData();

	if (m_CidChkBox == TRUE)
	{
		/* INIT.CID���� */

		// CID�擾
		CString cid;
		m_CButonSelCid.GetWindowTextW(cid);
		CString ctrl = _T("");
		ctrl.Format(_T("%s %s,%s;"), CID_CTRL, (LPCTSTR)cid, (LPCTSTR)m_satSel);

		// ����V�F���ďo
		CWaitCursor wait;
		theApp.SendControlNoUI(ctrl, _T(""), _T(""), _T(""));
		wait.Restore();

		CDialogBase::OnOK();
	}
	else
	{
		/* INIT���� */

		// �ݔ��擾
		int nCount = m_listInitSetting.GetSelCount();
		if (nCount <= 0)
		{
			CString title;
			GetWindowText(title);
			MessageBox(_T("�����ݒ���s���ݔ����I������Ă��܂���B"), title, MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		m_aryListBoxSel.RemoveAll();
		m_aryListBoxSel.SetSize(nCount);
		m_listInitSetting.GetSelItems(nCount, m_aryListBoxSel.GetData());

		m_aryListBoxPos = 0;

		// ���s�A�L�����Z����񊈐���
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

		// �^�C�}�쐬
		SetTimer(1, 5, 0);
	}
}


/*============================================================================*/
/*! CInitSetting

-# �R���{�{�b�N�X�I��ύX���ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CInitSetting::OnCbnSelchangeInitsettingCombo1()
{
	CString satname;
	int idx = m_CCBoxSatelliteName.GetCurSel();
	if (idx == CB_ERR)
	{
		return;
	}
	m_CCBoxSatelliteName.GetLBText(idx, satname);

	// ���X�g�X�V
	RestListBox(satname);

	// CID�X�V
	RestCIDBtn(satname);
}


/*============================================================================*/
/*! CInitSetting

-# CID�I���{�^���N���b�N���ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CInitSetting::OnBnClickedInitsettingCidButton()
{
	UpdateData();

	// �q�����擾
	CString sat;
	m_CCBoxSatelliteName.GetLBText(m_CCBoxSatelliteName.GetCurSel(), sat);

	// CID�擾
	CString cid;
	m_CButonSelCid.GetWindowTextW(cid);

	// CID�I����ʕ\��
	CCidSelect dlg;
	dlg.SetSatelliteName(sat);
	dlg.SetCID(cid);
	if (dlg.DoModal() == IDOK)
	{
		stCIDParam st = dlg.GetParameter();
		m_CButonSelCid.SetWindowTextW(st.CID);
		UpdateData(FALSE);
	}
}


/*============================================================================*/
/*! CInitSetting

-# �ݔ��S�I���{�^���N���b�N���ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CInitSetting::OnBnClickedInitsetingCheck()
{
	UpdateData(TRUE);

	if (m_chkAllSelect.GetCheck() == BST_CHECKED)
	{
		CString str;
		if (str.LoadStringW(IDS_INITSETTING_UNSELECTALL) == FALSE)
			return;
		m_chkAllSelect.SetWindowTextW(str);
		m_listInitSetting.SetSel(-1, TRUE);
	}
	else
	{
		CString str;
		if (str.LoadStringW(IDS_INITSETTING_SELECTALL) == FALSE)
			return;
		m_chkAllSelect.SetWindowTextW(str);
		m_listInitSetting.SetSel(-1, FALSE);
	}
}


/*============================================================================*/
/*! CInitSetting

-# CID�`�F�b�N�{�^������������

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CInitSetting::OnBnClickedCidcheck()
{
	UpdateData();

	if (m_CidChkBox == TRUE)
	{
		// �����ݒ胊�X�g�A�I���{�^���񊈐�
		m_listInitSetting.EnableWindow(FALSE);
		m_chkAllSelect.EnableWindow(FALSE);
	}
	else
	{
		// �����ݒ胊�X�g�A�I���{�^������
		m_listInitSetting.EnableWindow(TRUE);
		m_chkAllSelect.EnableWindow(TRUE);
	}
}


/*============================================================================*/
/*! CInitSetting

-# �����ݒ���s���^�C�}�[����

@param  nIDEvent
@retval �Ȃ�

*/
/*============================================================================*/
void CInitSetting::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	CWaitCursor wait;
	CString strEquip;
	int pos;
	// 3�܂ŘA�����M
	for (pos = m_aryListBoxPos; pos < m_aryListBoxSel.GetCount() && pos < m_aryListBoxPos + 3; ++pos)
	{
		int idx = m_aryListBoxSel.GetAt(pos);
		m_listInitSetting.GetText(idx, strEquip);

		CString ctrl = _T("");
		ctrl.Format(_T("%s ,%s;"), (LPCTSTR)strEquip, (LPCTSTR)m_satSel);

		// ����V�F���ďo
		theApp.SendControlNoUI(ctrl, _T(""), _T(""), _T(""));
	}

//	wait.Restore();

	// ���X�g�̏I�����`�F�b�N
	if (pos == m_aryListBoxSel.GetCount())
	{
		// �����ݒ��ʂ����
		EndDialog(IDOK);
	}
	// �����M�L��
	else
	{
		m_aryListBoxPos = pos;
		// �^�C�}�쐬
		SetTimer(1, 3000, 0);
	}

	CDialogBase::OnTimer(nIDEvent);
}
