/*============================================================================*/
/*! FrequencyCtrlCtrl.cpp

-# ���g��������-�ʐݒ�^�u
*/
/*============================================================================*/
// FrequencyCtrlCtrl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlCtrl.h"
#include "afxdialogex.h"


// CFrequencyCtrlCtrl �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlCtrl, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlCtrl::CFrequencyCtrlCtrl(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlCtrl::IDD, pParent)
	, m_strUC(_T(""))
	, m_strDC(_T(""))
	, m_strCmdCareer(_T(""))
	, m_strTFreqOffset(_T(""))
	, m_strTimeOffset(_T(""))
	, m_strRFreqOffset(_T(""))
	, m_strTimeOffsetPre(_T(""))
{

}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �R���X�g���N�^

@param	IDD�FIDD
@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlCtrl::CFrequencyCtrlCtrl(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �f�X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlCtrl::~CFrequencyCtrlCtrl()
{
}

void CFrequencyCtrlCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_SET_UC_EDIT, m_strUC);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_SET_DC_EDIT, m_strDC);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_SET_CMD_EDIT, m_strCmdCareer);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT, m_strTFreqOffset);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, m_strTimeOffset);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_TD_CMB, m_TDopplerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_RD_CMB, m_RDopplerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_CMD_CMB, m_DopplerCmdCareerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_SCM_CMB, m_SpacecraftCOHModeCtrl);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT2, m_strRFreqOffset);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlCtrl, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_UC_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlSetUcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_CMD_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlSetCmdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TF_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlOfsTfBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TO_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlOfsToBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_TD_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlDpTdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_RD_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlDpRdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_CMD_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlDpCmdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SCM_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlScmBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_CMD_TIME_BTN, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlSetCmdTimeBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TF_BTN2, &CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlOfsTfBtn2)
	ON_EN_CHANGE(IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, &CFrequencyCtrlCtrl::OnEnChangeFreqCtrlCtrlOfsToEdit)
END_MESSAGE_MAP()


// CFrequencyCtrlCtrl ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlCtrl::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �T���@COH���[�h
	for (int i = 0; i < sizeof(mSatMode) / sizeof(mSatMode[0]); i++){
		m_SpacecraftCOHModeCtrl.AddString(mSatMode[i]);
	}
	m_SpacecraftCOHModeCtrl.SetCurSel(0);

	// ���M�h�b�v��
	for (int i = 0; i < sizeof(mTxDopp) / sizeof(mTxDopp[0]); i++){
		m_TDopplerCtrl.AddString(mTxDopp[i]);
	}
	m_TDopplerCtrl.SetCurSel(0);

	// ��M�h�b�v��
	for (int i = 0; i < sizeof(mRxDopp) / sizeof(mRxDopp[0]); i++){
		m_RDopplerCtrl.AddString(mRxDopp[i]);
	}
	m_RDopplerCtrl.SetCurSel(0);

	// �R�}���h�T�u�L�����A
	for (int i = 0; i < sizeof(mCmdCarr) / sizeof(mCmdCarr[0]); i++){
		m_DopplerCmdCareerCtrl.AddString(mCmdCarr[i]);
	}
	m_DopplerCmdCareerCtrl.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlCtrl::UpdateDlg()
{
	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �ݒ���g��-U/C�AD/D �ݒ�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlSetUcBtn()
{
	SetData(eControl_UC);
	SetData(eControl_DC);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �ݒ���g�� - �R�}���h�L�����A �ݒ�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlSetCmdBtn()
{
	SetData(eControl_CS);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �I�t�Z�b�g-���M���g���I�t�Z�b�g �ݒ�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlOfsTfBtn()
{
	SetData(eControl_TOFFSET);
}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �I�t�Z�b�g-��M���g���I�t�Z�b�g �ݒ�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlOfsTfBtn2()
{
	SetData(eControl_ROFFSET);
}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �I�t�Z�b�g-�����I�t�Z�b�g �ݒ�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlOfsToBtn()
{
	SetData(eControl_TIMEOFFSET);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �h�b�v���⏞-���M�h�b�v�� �ݒ�{�^������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlDpTdBtn()
{
	SetData(eControl_TDP);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �h�b�v���⏞-��M�h�b�v�� �ݒ�{�^������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlDpRdBtn()
{
	SetData(eControl_RDP);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �h�b�v���⏞-�R�}���h�L�����A �ݒ�{�^������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlDpCmdBtn()
{
	SetData(eControl_CDP);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �T���@COH���[�h �ݒ�{�^������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlScmBtn()
{
	SetData(eControl_COH);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �ꊇ�ݒ�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlBtn()
{
	SetData(eControl_ALLSET);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# �R�}���h�T�u�L�����A-���ݎ����{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::OnBnClickedFreqCtrlCtrlSetCmdTimeBtn()
{
	SetData(eControl_TIMELOCK);
}


/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# ��ʍX�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlCtrl::SetData(int type)
{
	CFrequencyCtrlCtrl::stSetCtlData mCtlTable[] = {
		{ _T("DSNTCR.TRP_RATIO"), _T(""), IDC_FREQ_CTRL_CTRL_SCM_CMB, eCOMBO },	// �T���@COH���[�h
		{ _T("X-UC.FREQUENCY"), _T("CTRL.TX_DOPP_ZERO"), IDC_FREQ_CTRL_CTRL_SET_UC_EDIT, eEDIT },	// U/C
		{ _T("S70-DC.SET_FREQ"), _T("CTRL.RX_DOPP_ZERO"), IDC_FREQ_CTRL_CTRL_SET_DC_EDIT, eEDIT },	// D/C
		{ _T("DSNTCR.TX_SC_FREQ"), _T("DSNTCR.TX_SC_DOPP_PRE"), IDC_FREQ_CTRL_CTRL_SET_CMD_EDIT, eEDIT },	// �R�}���h�T�u�L�����A
		{ _T("CTRL.CMD_SC_NOW"), _T(""), IDC_FREQ_CTRL_CTRL_SET_CMD_TIME_BTN, eNONE },	// ���ݎ����x�X�g���b�N
		{ _T("CTRL.DOPP_MODE"), _T(""), IDC_FREQ_CTRL_CTRL_DP_TD_CMB, eCOMBO },	// ���M�h�b�v��
		{ _T("XX-DSNTCRFC.1WAY/2WAY"), _T(""), IDC_FREQ_CTRL_CTRL_DP_RD_CMB, eCOMBO },	// ��M�h�b�v��
		{ _T("CTRL.SC_DOPP_MODE"), _T(""), IDC_FREQ_CTRL_CTRL_DP_CMD_CMB, eCOMBO },	// �R�}���h�T�u�L�����A
		{ _T("XX-DSNTCRFC.FREQ_OFFSET_U"), _T(""), IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT, eEDIT },	// ���M���g���I�t�Z�b�g
		{ _T("XX-DSNTCRFC.FREQ_OFFSET"), _T(""), IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT2, eEDIT },	// ��M���g���I�t�Z�b�g
		{ _T("XX-DSNTCRFC.TIME_OFFSET"), _T(""), IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, eEDIT },	// �����I�t�Z�b�g
	};

	const CString strMsg = _T("�����I�t�Z�b�g�̕��b�w��(�}M:SS)���Ԉ���Ă��܂��B");

	UpdateData(TRUE);

	/****************************************************************/
	/* ������ */
	CString ctrl, val;
	bool bAll = false;
	if (type == eControl_ALLSET)
		bAll = true;
	for (int item = 0; item < eControl_ALLSET; item++){
		if (bAll == true){
			type = item;
		}
		for (int i = 0; i < 2; i++){
			switch (type){
			case	eControl_COH:
				ctrl = mCtlTable[type].ctlname1;
				val = mSatMode[m_SpacecraftCOHModeCtrl.GetCurSel()];
				i = 2;
				break;
			case	eControl_UC:
				if (i == 0){
					ctrl = mCtlTable[type].ctlname1;
					val = m_strUC;
				}
				else{
					ctrl = mCtlTable[type].ctlname2;
					val = _T(",");
				}
				break;
			case	eControl_DC:
				if (i == 0){
					ctrl = mCtlTable[type].ctlname1;
					val = m_strDC;
				}
				else{
					ctrl = mCtlTable[type].ctlname2;
					val = _T(",");
				}
				break;
			case	eControl_CS:
				if (i == 0){
					ctrl = mCtlTable[type].ctlname1;
					val = m_strCmdCareer;
				}
				else{
					ctrl = mCtlTable[type].ctlname2;
					val = _T("OFF");
				}
				break;
			case	eControl_TIMELOCK:
				ctrl = mCtlTable[type].ctlname1;
				val = _T(",");
				i = 2;
				break;
			case	eControl_TDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mCmdCtrl[m_TDopplerCtrl.GetCurSel()];
				i = 2;
				break;
			case	eControl_RDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mRxDopp[m_RDopplerCtrl.GetCurSel()];
				i = 2;
				break;
			case	eControl_CDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mCmdCtrl[m_DopplerCmdCareerCtrl.GetCurSel()];
				i = 2;
				break;
			case	eControl_TOFFSET:
				ctrl = mCtlTable[type].ctlname1;
				val = m_strTFreqOffset;
				i = 2;
				break;
			case	eControl_ROFFSET:
				ctrl = mCtlTable[type].ctlname1;
				val = m_strRFreqOffset;
				i = 2;
				break;
			case	eControl_TIMEOFFSET:
				ctrl = mCtlTable[type].ctlname1;
				{
					// ��M:SS�����t�H�[�}�b�g�`�F�b�N
					int min, sec;
					TCHAR sz_time_in[6], sz_time_chk[6];
					int sz = 6;
					CString strTimeOffset, strSign, strChk;

					bool bFormatErr = false;
					if (m_strTimeOffset.IsEmpty() == true || m_strTimeOffset.GetLength() >= sz)		// �T�C�Y�i�I�[������'\0'���l���j�`�F�b�N
					{
						// ���̓t�H�[�}�b�g���قȂ�
						bFormatErr = true;
					}
					else
					{
						strChk = m_strTimeOffset;
						if (strChk.Remove(_T(':')) > 1)
						{
							// ���̓t�H�[�}�b�g���قȂ�
							bFormatErr = true;
						}
						else
						{
							strTimeOffset = m_strTimeOffset;

							// �����`�F�b�N
							if (strTimeOffset.Left(1) == _T("+"))
							{
								strSign = _T("+");
								strTimeOffset.TrimLeft(_T('+'));
							}
							else if (strTimeOffset.Left(1) == _T("-"))
							{
								strSign = _T("-");
								strTimeOffset.TrimLeft(_T('-'));
							}
							else if (strTimeOffset.Left(1) == _T(' '))
							{
								strSign = _T("+");
								strTimeOffset.TrimLeft(_T(' '));
							}
							else
							{
								strSign = _T("+");
							}

							if (wcscpy_s(sz_time_in, sz, (LPCTSTR)strTimeOffset) != 0)
							{
								// ���̓t�H�[�}�b�g���قȂ�
								bFormatErr = true;
							}
							else
							{
								// m:ss�`��
								if (swscanf_s(sz_time_in, L"%01d:%d", &min, &sec) == 2)
								{
									if ((min < 0) || (min  > 9) ||
										(sec < 0) || (sec  > 59))
									{
										// ���̓t�H�[�}�b�g���قȂ�
										bFormatErr = true;
									}
								}
								else
								{
									// ���̓t�H�[�}�b�g���قȂ�
									bFormatErr = true;
								}
							}
						}
					}

					if (bFormatErr)
					{
						MessageBox(strMsg, 0, MB_OK | MB_ICONERROR);
						i = 2;
						continue;
					}

					swprintf_s(sz_time_chk, sz, L"%s%01d:%02d", (LPCTSTR)strSign, min, sec);
					m_strTimeOffset = sz_time_chk;
					//TRACE("�����w��:%s\n", CStringA(m_strTimeOffset));
				}
				val = m_strTimeOffset;
				i = 2;
				break;
			case	eControl_ALLSET:
				break;
			}
			theApp.SendControl(ctrl, val, _T(""), _T(""));
		}
		if (bAll == false)
			break;
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlCtrl

-# ����l�͈̓`�F�b�N

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
bool CFrequencyCtrlCtrl::CheckCtrlValRange(CString& strCtrl, CString& strVal, CString& strTitle)
{
	struct ctlname_t *ctrl_adr;
	struct paramtbl_t *param_adr[1];
	int l_rc;

	double val = _wtof(strVal);

	CDBAccess& dbAcccess = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64> &ctlAdrList = dbAcccess.GetCtlAdrList();
	ctrl_adr = (struct ctlname_t *)((__int64*)ctlAdrList[strCtrl]);

	if (ctrl_adr == NULL)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl"), _T("CheckCtrlValRange"), _T("ControlName None"), strCtrl, nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return false;
	}

	/*----- �ݔ����䍀�ڃX�e�[�^�X�`�F�b�N ----*/
	l_rc = dbAcccess.ud_adr_to_resvec((char*)ctrl_adr, PARAMTBL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)param_adr, 1);

	if (l_rc < 1)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl"), _T("CheckCtrlValRange"), _T("Control Error"), strCtrl, nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return false;
	}

	/* �A�i���O�l�̐��� */
	if (param_adr[0]->l_intype >= IN_D_ID && param_adr[0]->l_intype <= IN_F_ID)
	{
		struct val_t *val_adr[1];
		l_rc = dbAcccess.ud_adr_to_resvec((char*)ctrl_adr, VAL_ID, ENDOFCTL_ID, ENDOFPARAM_ID, (char**)val_adr, 1);

		if (l_rc < 1)
		{
			// ���䂪�擾�ł��Ȃ��������߉������Ȃ�
			//=====================================================//
			//������������������������ Log ������������������������//
			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl"), _T("CheckCtrlValRange"), _T("Could not get Control"), strCtrl, nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
			return false;
		}
		else
		{
			// �͈͒l�`�F�b�N������
			if (strVal.IsEmpty() == true || val < val_adr[0]->d_min || val > val_adr[0]->d_max)
			{
				CString strMsg;
				strMsg.Format(_T("%s��%.*f�`%.*fHz����͂��Ă��������B\n"), strTitle, param_adr[0]->l_incolflt, val_adr[0]->d_min, param_adr[0]->l_incolflt, val_adr[0]->d_max);
				AfxMessageBox(strMsg, MB_OK | MB_ICONERROR);
				return false;
			}
		}
	}

	return true;
}

void CFrequencyCtrlCtrl::OnEnChangeFreqCtrlCtrlOfsToEdit()
{
	UpdateData(TRUE);

	CString strTimeChk = m_strTimeOffset;

	// �����񂩂琔��0�`9, '-', '+', ':'������΍폜���A�c��������������΋֎~�����L��
	WCHAR szPermitMoji[] = {
		_T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), _T('8'), _T('9'), _T('-'), _T('+'), _T(':')
	};

	for (int i = 0; i < sizeof(szPermitMoji) / sizeof(szPermitMoji[0]); i++)
	{
		strTimeChk.Remove(szPermitMoji[i]);
	}

	if (strTimeChk.IsEmpty() == false)
	{
		// �֎~�������c���Ă��邽�߁A���[�j���O��\�����A���͑O�̒l�ɖ߂�
		//		CString strMsg;
		//		strMsg.Format(_T("����[%s]�͓��͂ł��܂���B\n�����I�t�Z�b�g�͕��b�w��(�}M:SS)����͂��Ă��������B"), strTimeChk);
		//		MessageBox(strMsg, 0, MB_OK | MB_ICONERROR);
		m_strTimeOffset = m_strTimeOffsetPre;
		UpdateData(FALSE);
	}
	else
	{
		// �֎~�����������ꍇ
		m_strTimeOffsetPre = m_strTimeOffset;
	}
}
