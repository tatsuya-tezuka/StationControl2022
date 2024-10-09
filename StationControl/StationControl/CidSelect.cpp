/*============================================================================*/
/*! OperationPlan.cpp

-# CID�I�����
-# 
-# �q�������w�肵���ꍇ�́A�q���I���R���{�{�b�N�X�͎w�肵���q�����Ń}�X�N����܂��B
-# �܂��A�q������CID���w�肵���ꍇ�́A����CID�̃p�����[�^�������l�ɂȂ�܂��B
*/
/*============================================================================*/
// CidSelect.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "CidSelect.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"


#define CIDSEL_GREAT	1

// CCidSelect �_�C�A���O

IMPLEMENT_DYNAMIC(CCidSelect, CDialogBase)

CCidSelect::CCidSelect(CWnd* pParent /*=NULL*/)
	: CDialogBase(CCidSelect::IDD, pParent)
	, m_strSatelliteCmb(_T(""))
	, m_strCIDCmb(_T(""))
	, m_strTlmBRSCmb(_T(""))
	, m_strTlmBRXCmb(_T(""))
	, m_strTlmBRHXCmb(_T(""))
	, m_strTlmBRKaCmb(_T(""))
	, m_strCmdBRSCmb(_T(""))
	, m_strCmdBRXCmb(_T(""))
	, m_strTPwrCmb(_T(""))
	, m_strCarrTLbSCmb(_T(""))
	, m_strCarrTLbXCmb(_T(""))
	, m_strCarrTLbKaCmb(_T(""))
	, m_strUpLinkShapeCmb(_T(""))
	, m_strRngPrmModeCmb(_T(""))
	, m_strCmdMdlCmb(_T(""))
	, m_strCmdFreqCmb(_T(""))
	, m_strMdlSCmb(_T(""))
	, m_strMdlXCmb(_T(""))
	, m_strTDoppCmb(_T(""))
	, m_strAntAutoCmb(_T(""))
	, m_strAntStandbyCmb(_T(""))
	, m_strAntStandbyPosCmb(_T(""))
{

}

CCidSelect::~CCidSelect()
{
}

void CCidSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CIDSEL_SATELLITE_CMB, m_SetalliteCmb);
	DDX_Control(pDX, IDC_CIDSEL_CID_CMB, m_CidCmb);
	DDX_Control(pDX, IDC_CIDSEL_TLMBR_S_CMB, m_TlmBrSCmb);
	DDX_Control(pDX, IDC_CIDSEL_TLMBR_X_CMB, m_TlmBrXCmb);
	DDX_Control(pDX, IDC_CIDSEL_TLMBR_HX_CMB, m_TlmBrHXCmb);
	DDX_Control(pDX, IDC_CIDSEL_TLMBR_KA_CMB, m_TlmBrKaCmb);
	DDX_Control(pDX, IDC_CIDSEL_CLB_S_CMB, m_CarrTLbSCmb);
	DDX_Control(pDX, IDC_CIDSEL_CLB_X_CMB, m_CarrTLbXCmb);
	DDX_Control(pDX, IDC_CIDSEL_CLB_KA_CMB, m_CarrTLbKaCmb);
	DDX_Control(pDX, IDC_CIDSEL_RPRM_SR_CMB, m_RngPrmModeCmb);
	DDX_Control(pDX, IDC_CIDSEL_MMTD_S_CMB, m_MdlSCmb);
	DDX_Control(pDX, IDC_CIDSEL_MMTD_X_CMB, m_MdlXCmb);
	DDX_Control(pDX, IDC_CIDSEL_ANTAT_MIG_CMB, m_AntAutoCmb);
	DDX_Control(pDX, IDC_CIDSEL_CMDBR_S_CMB, m_CmdBrSCmb);
	DDX_Control(pDX, IDC_CIDSEL_CMDBR_X_CMB, m_CmdBrXCmb);
	DDX_Control(pDX, IDC_CIDSEL_TPWR_PWR_CMB, m_TPwrCmb);
	DDX_Control(pDX, IDC_CIDSEL_UPLNK_SHP_CMB, m_UpLinkShapeCmb);
	DDX_Control(pDX, IDC_CIDSEL_CMDMDL_MDL_CMB, m_CmdMdlCmb);
	DDX_Control(pDX, IDC_CIDSEL_CMDSUBFREQ_FREQ_CMB, m_CmdFreqCmb);
	DDX_Control(pDX, IDC_CIDSEL_TDOP_CMP_CMB, m_TDoppCmb);
	DDX_CBString(pDX, IDC_CIDSEL_SATELLITE_CMB, m_strSatelliteCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CID_CMB, m_strCIDCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TLMBR_S_CMB, m_strTlmBRSCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TLMBR_X_CMB, m_strTlmBRXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TLMBR_HX_CMB, m_strTlmBRHXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TLMBR_KA_CMB, m_strTlmBRKaCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CMDBR_S_CMB, m_strCmdBRSCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CMDBR_X_CMB, m_strCmdBRXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TPWR_PWR_CMB, m_strTPwrCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CLB_S_CMB, m_strCarrTLbSCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CLB_X_CMB, m_strCarrTLbXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CLB_KA_CMB, m_strCarrTLbKaCmb);
	DDX_CBString(pDX, IDC_CIDSEL_UPLNK_SHP_CMB, m_strUpLinkShapeCmb);
	DDX_CBString(pDX, IDC_CIDSEL_RPRM_SR_CMB, m_strRngPrmModeCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CMDMDL_MDL_CMB, m_strCmdMdlCmb);
	DDX_CBString(pDX, IDC_CIDSEL_CMDSUBFREQ_FREQ_CMB, m_strCmdFreqCmb);
	DDX_CBString(pDX, IDC_CIDSEL_MMTD_S_CMB, m_strMdlSCmb);
	DDX_CBString(pDX, IDC_CIDSEL_MMTD_X_CMB, m_strMdlXCmb);
	DDX_CBString(pDX, IDC_CIDSEL_TDOP_CMP_CMB, m_strTDoppCmb);
	DDX_CBString(pDX, IDC_CIDSEL_ANTAT_MIG_CMB, m_strAntAutoCmb);
	DDX_Control(pDX, IDC_CIDSEL_ANTSTDORIENTED_CMB, m_AntStandbyCmb);
	DDX_CBString(pDX, IDC_CIDSEL_ANTSTDORIENTED_CMB, m_strAntStandbyCmb);
	DDX_Control(pDX, IDC_CIDSEL_ANTSTDPOSORIENTED_CMB, m_AntStandbyPosCmb);
	DDX_CBString(pDX, IDC_CIDSEL_ANTSTDPOSORIENTED_CMB, m_strAntStandbyPosCmb);
}


BEGIN_MESSAGE_MAP(CCidSelect, CDialogBase)
	ON_CBN_SELCHANGE(IDC_CIDSEL_SATELLITE_CMB, &CCidSelect::OnCbnSelchangeCidselSatelliteCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CID_CMB, &CCidSelect::OnCbnSelchangeCidselCidCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TLMBR_S_CMB, &CCidSelect::OnCbnSelchangeCidselTlmbrSCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TLMBR_X_CMB, &CCidSelect::OnCbnSelchangeCidselTlmbrXCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TLMBR_HX_CMB, &CCidSelect::OnCbnSelchangeCidselTlmbrHxCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TLMBR_KA_CMB, &CCidSelect::OnCbnSelchangeCidselTlmbrKaCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CMDBR_S_CMB, &CCidSelect::OnCbnSelchangeCidselCmdbrSCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CMDBR_X_CMB, &CCidSelect::OnCbnSelchangeCidselCmdbrXCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TPWR_PWR_CMB, &CCidSelect::OnCbnSelchangeCidselTpwrPwrCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CLB_S_CMB, &CCidSelect::OnCbnSelchangeCidselClbSCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CLB_X_CMB, &CCidSelect::OnCbnSelchangeCidselClbXCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CLB_KA_CMB, &CCidSelect::OnCbnSelchangeCidselClbKaCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_UPLNK_SHP_CMB, &CCidSelect::OnCbnSelchangeCidselUplnkShpCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_RPRM_SR_CMB, &CCidSelect::OnCbnSelchangeCidselRprmSrCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CMDMDL_MDL_CMB, &CCidSelect::OnCbnSelchangeCidselCmdmdlMdlCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_CMDSUBFREQ_FREQ_CMB, &CCidSelect::OnCbnSelchangeCidselCmdsubfreqFreqCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_MMTD_S_CMB, &CCidSelect::OnCbnSelchangeCidselMmtdSCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_MMTD_X_CMB, &CCidSelect::OnCbnSelchangeCidselMmtdXCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_TDOP_CMP_CMB, &CCidSelect::OnCbnSelchangeCidselTdopCmpCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_ANTAT_MIG_CMB, &CCidSelect::OnCbnSelchangeCidselAntatMigCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_ANTSTDORIENTED_CMB, &CCidSelect::OnCbnSelchangeCidselAntstdorientedCmb)
	ON_CBN_SELCHANGE(IDC_CIDSEL_ANTSTDPOSORIENTED_CMB, &CCidSelect::OnCbnSelchangeCidselAntstdposorientedCmb)
	ON_BN_CLICKED(IDOK, &CCidSelect::OnBnClickedOk)
END_MESSAGE_MAP()


// CCidSelect ���b�Z�[�W �n���h���[




/*============================================================================*/
/*! CCidSelect

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CCidSelect::OnInitDialog()
{
	CDialogBase::OnInitDialog();

//	InitDlg();
	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CCidSelect

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CCidSelect::UpdateDlg()
{
	InitDlg();
	return TRUE;
}

/*============================================================================*/
/*! CCidSelect

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CCidSelect::InitDlg()
{
	// �R���{�{�b�N�X �ݒ�
	SetComboboxSatCIDParam(m_strSatelliteName);
	return TRUE;
}

/*============================================================================*/
/*! CCidSelect

-# �q���I��ύX����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CCidSelect::SetComboboxSatCIDParam(CString sat)
{
	CString strSatName = sat;
	BOOL isFindSat = FALSE;

	m_SetalliteCmb.ResetContent();
	map<CString, stCIDPrmEle> &cidEleList = theApp.GetSatelliteData().m_CIDSlctData.GetCidElementList();
	for (auto itrs = cidEleList.begin(); itrs != cidEleList.end(); ++itrs)
	{
		CString sat = itrs->first;

		m_SetalliteCmb.InsertString(-1, sat);

		if (strSatName.CompareNoCase(sat) == 0)
		{
			isFindSat = TRUE;
		}
	}

	if (strSatName != _T("") && isFindSat == FALSE)
	{
		AfxMessageBox(_T("�Y������q����CID�t�@�C��������܂���B"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// �q�������ݒ肳��Ă���ꍇ�͉q���I���R���{�{�b�N�X��񊈐��ɂ���
	if (m_strSatelliteName != _T("") && isFindSat == TRUE)
	{
		m_SetalliteCmb.SelectStringEx(-1, strSatName);
		m_SetalliteCmb.EnableWindow(FALSE);
	}
	else
	{
		if (strSatName != _T(""))
		{
			m_SetalliteCmb.SelectStringEx(-1, strSatName);
		}
		else
		{
			m_SetalliteCmb.SetCurSel(0);
			m_SetalliteCmb.GetLBText(m_SetalliteCmb.GetCurSel(), strSatName);
		}
	}

	// �q���ŗL���擾
	GetEqSatDB(m_strSatelliteName);

	// �e�R���{������
	m_CidCmb.ResetContent();
	m_TlmBrSCmb.ResetContent();
	m_TlmBrXCmb.ResetContent();
	m_TlmBrHXCmb.ResetContent();
	m_TlmBrKaCmb.ResetContent();
	m_CmdBrSCmb.ResetContent();
	m_CmdBrXCmb.ResetContent();
	m_TPwrCmb.ResetContent();
	m_CarrTLbSCmb.ResetContent();
	m_CarrTLbXCmb.ResetContent();
	m_CarrTLbKaCmb.ResetContent();
	m_UpLinkShapeCmb.ResetContent();
	m_RngPrmModeCmb.ResetContent();
	m_MdlSCmb.ResetContent();
	m_MdlXCmb.ResetContent();
	m_CmdMdlCmb.ResetContent();
	m_CmdFreqCmb.ResetContent();
	m_TDoppCmb.ResetContent();
	m_AntAutoCmb.ResetContent();
	m_AntStandbyCmb.ResetContent();
	m_AntStandbyPosCmb.ResetContent();

	map<CString, stCIDPrmFlg> &cidPrmFlgList = theApp.GetSatelliteData().m_CIDSlctData.GetCidPrmFlgList();
	stCIDPrmFlg stPrmflg = cidPrmFlgList[strSatName];


	for (auto itrs = cidEleList.begin(); itrs != cidEleList.end(); ++itrs)
	{
		CString sat = itrs->first;
		stCIDPrmEle &st = itrs->second;

		if (sat == strSatName)
		{
			// CID
			int selectCID = -1;
			int idx = 0;
			for (auto itri = st.CID.begin(); itri != st.CID.end(); ++itri,++idx)
			{
				CString item = (*itri);
				m_CidCmb.InsertString(-1, item);

				if (m_strCID == item)
				{
					selectCID = idx;
				}

			}
			if (selectCID != -1)
			{
				m_CidCmb.SetCurSel(selectCID);
			}
			else
			{
				m_CidCmb.SetCurSel(0);
			}

			// �e�����g���r�b�g���[�g S��
			if (stPrmflg.TlmBitRateS)
			{
				for (auto itri = st.TlmBitRateS.begin(); itri != st.TlmBitRateS.end(); ++itri)
				{
					CString item = (*itri);
					m_TlmBrSCmb.InsertString(-1, item);
				}
				m_TlmBrSCmb.SetCurSel(0);
				m_TlmBrSCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TlmBrSCmb.EnableWindow(FALSE);
			}

			// �e�����g���r�b�g���[�g X��
			if (stPrmflg.TlmBitRateX)
			{
				for (auto itri = st.TlmBitRateX.begin(); itri != st.TlmBitRateX.end(); ++itri)
				{
					CString item = (*itri);
					m_TlmBrXCmb.InsertString(-1, item);
				}
				m_TlmBrXCmb.SetCurSel(0);
				m_TlmBrXCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TlmBrXCmb.EnableWindow(FALSE);
			}

			// �e�����g���r�b�g���[�g HX��
			if (stPrmflg.TlmBitRateHX)
			{
				for (auto itri = st.TlmBitRateHX.begin(); itri != st.TlmBitRateHX.end(); ++itri)
				{
					CString item = (*itri);
					m_TlmBrHXCmb.InsertString(-1, item);
				}
				m_TlmBrHXCmb.SetCurSel(0);
				m_TlmBrHXCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TlmBrHXCmb.EnableWindow(FALSE);
			}

			// �e�����g���r�b�g���[�g Ka��
			if (stPrmflg.TlmBitRateKa)
			{
				for (auto itri = st.TlmBitRateKa.begin(); itri != st.TlmBitRateKa.end(); ++itri)
				{
					CString item = (*itri);
					m_TlmBrKaCmb.InsertString(-1, item);
				}
				m_TlmBrKaCmb.SetCurSel(0);
				m_TlmBrKaCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TlmBrKaCmb.EnableWindow(FALSE);
			}

			// �R�}���h�r�b�g���[�g S��
			if (stPrmflg.CmdBitRateS)
			{
				for (auto itri = st.CmdBitRateS.begin(); itri != st.CmdBitRateS.end(); ++itri)
				{
					CString item = (*itri);
					m_CmdBrSCmb.InsertString(-1, item);
				}
				m_CmdBrSCmb.SetCurSel(0);
				m_CmdBrSCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CmdBrSCmb.EnableWindow(FALSE);
			}

			// �R�}���h�r�b�g���[�g X��
			if (stPrmflg.CmdBitRateX)
			{
				for (auto itri = st.CmdBitRateX.begin(); itri != st.CmdBitRateX.end(); ++itri)
				{
					CString item = (*itri);
					m_CmdBrXCmb.InsertString(-1, item);
				}
				m_CmdBrXCmb.SetCurSel(0);
				m_CmdBrXCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CmdBrXCmb.EnableWindow(FALSE);
			}

			// ���M�d��
			if (stPrmflg.TrsPower)
			{
				for (auto itri = st.TrsPower.begin(); itri != st.TrsPower.end(); ++itri)
				{
					CString item = (*itri);
					m_TPwrCmb.InsertString(-1, item);
				}
				m_TPwrCmb.SetCurSel(0);
				m_TPwrCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TPwrCmb.EnableWindow(FALSE);
			}

			// �L�����A��M���[�v�o���h S��
			if (stPrmflg.CarrLoopBandS)
			{
				for (auto itri = st.CarrLoopBandS.begin(); itri != st.CarrLoopBandS.end(); ++itri)
				{
					CString item = (*itri);
					m_CarrTLbSCmb.InsertString(-1, item);
				}
				m_CarrTLbSCmb.SetCurSel(0);
				m_CarrTLbSCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CarrTLbSCmb.EnableWindow(FALSE);
			}

			// �L�����A��M���[�v�o���h X��
			if (stPrmflg.CarrLoopBandX)
			{
				for (auto itri = st.CarrLoopBandX.begin(); itri != st.CarrLoopBandX.end(); ++itri)
				{
					CString item = (*itri);
					m_CarrTLbXCmb.InsertString(-1, item);
				}
				m_CarrTLbXCmb.SetCurSel(0);
				m_CarrTLbXCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CarrTLbXCmb.EnableWindow(FALSE);
			}

			// �L�����A��M���[�v�o���h Ka��
			if (stPrmflg.CarrLoopBandKa)
			{
				for (auto itri = st.CarrLoopBandKa.begin(); itri != st.CarrLoopBandKa.end(); ++itri)
				{
					CString item = (*itri);
					m_CarrTLbKaCmb.InsertString(-1, item);
				}
				m_CarrTLbKaCmb.SetCurSel(0);
				m_CarrTLbKaCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CarrTLbKaCmb.EnableWindow(FALSE);
			}

			// �A�b�v�����N�|���p�����[�^ �`��
			if (stPrmflg.UpLinkSweepShape)
			{
				for (auto itri = st.UpLinkSweepShape.begin(); itri != st.UpLinkSweepShape.end(); ++itri)
				{
					CString item = (*itri);
					m_UpLinkShapeCmb.InsertString(-1, item);
				}
				m_UpLinkShapeCmb.SetCurSel(0);
				m_UpLinkShapeCmb.EnableWindow(TRUE);
			}
			else
			{
				m_UpLinkShapeCmb.EnableWindow(FALSE);
			}

			// �����W�v���p�����[�^ ���[�h
			if (stPrmflg.RngMesMode)
			{
				for (auto itri = st.RngMesMode.begin(); itri != st.RngMesMode.end(); ++itri)
				{
					CString item = (*itri);
					m_RngPrmModeCmb.InsertString(-1, item);
				}
				m_RngPrmModeCmb.SetCurSel(0);
				m_RngPrmModeCmb.EnableWindow(TRUE);
			}
			else
			{
				m_RngPrmModeCmb.EnableWindow(FALSE);
			}

			// �ϒ����� S��/X��
			if (stPrmflg.Modulation)
			{
				if (m_xtx == XTX_Y_ID)	// X��
				{
					for (auto itri = st.Modulation.begin(); itri != st.Modulation.end(); ++itri)
					{
						CString item = (*itri);
						m_MdlXCmb.InsertString(-1, item);
					}
					m_MdlXCmb.SetCurSel(0);
					m_MdlXCmb.EnableWindow(TRUE);
					m_MdlSCmb.EnableWindow(FALSE);
				}
				else	// S��
				{
					for (auto itri = st.Modulation.begin(); itri != st.Modulation.end(); ++itri)
					{
						CString item = (*itri);
						m_MdlSCmb.InsertString(-1, item);
					}
					m_MdlSCmb.SetCurSel(0);
					m_MdlSCmb.EnableWindow(TRUE);
					m_MdlXCmb.EnableWindow(FALSE);
				}
			}
			else
			{
				m_MdlSCmb.EnableWindow(FALSE);
				m_MdlXCmb.EnableWindow(FALSE);
			}

			// �R�}���h�ϒ��x
			if (stPrmflg.CmdMod)
			{
				for (auto itri = st.CmdMod.begin(); itri != st.CmdMod.end(); ++itri)
				{
					CString item = (*itri);
					m_CmdMdlCmb.InsertString(-1, item);
				}
				m_CmdMdlCmb.SetCurSel(0);
				m_CmdMdlCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CmdMdlCmb.EnableWindow(FALSE);
			}

			// �R�}���h�T�u�L�����A���g��
			if (stPrmflg.CmdSubCarrFreq)
			{
				for (auto itri = st.CmdSubCarrFreq.begin(); itri != st.CmdSubCarrFreq.end(); ++itri)
				{
					CString item = (*itri);
					m_CmdFreqCmb.InsertString(-1, item);
				}
				m_CmdFreqCmb.SetCurSel(0);
				m_CmdFreqCmb.EnableWindow(TRUE);
			}
			else
			{
				m_CmdFreqCmb.EnableWindow(FALSE);
			}

			// ���M�h�b�v��
			if (stPrmflg.TransDoppler)
			{
				for (auto itri = st.TransDoppler.begin(); itri != st.TransDoppler.end(); ++itri)
				{
					CString item = (*itri);
					m_TDoppCmb.InsertString(-1, item);
				}
				m_TDoppCmb.SetCurSel(0);
				m_TDoppCmb.EnableWindow(TRUE);
			}
			else
			{
				m_TDoppCmb.EnableWindow(FALSE);
			}

			// �A���e�i�����ǔ�
			if (stPrmflg.AntAutoTrack)
			{
				for (auto itri = st.AntAutoTrack.begin(); itri != st.AntAutoTrack.end(); ++itri)
				{
					CString item = (*itri);
					m_AntAutoCmb.InsertString(-1, item);
				}
				m_AntAutoCmb.SetCurSel(0);
				m_AntAutoCmb.EnableWindow(TRUE);
			}
			else
			{
				m_AntAutoCmb.EnableWindow(FALSE);
			}

			// �A���e�i�҂��󂯎w��
			if (stPrmflg.AntAosPosn)
			{
				for (auto itri = st.AntAosPosn.begin(); itri != st.AntAosPosn.end(); ++itri)
				{
					CString item = (*itri);
					m_AntStandbyCmb.InsertString(-1, item);
				}
				m_AntStandbyCmb.SetCurSel(0);
				m_AntStandbyCmb.EnableWindow(TRUE);
			}
			else
			{
				m_AntStandbyCmb.EnableWindow(FALSE);
			}

			// �A���e�i�ҋ@�ʒu�w��
			if (stPrmflg.AntWaitPoint)
			{
				for (auto itri = st.AntWaitPoint.begin(); itri != st.AntWaitPoint.end(); ++itri)
				{
					CString item = (*itri);
					m_AntStandbyPosCmb.InsertString(-1, item);
				}
				m_AntStandbyPosCmb.SetCurSel(0);
				m_AntStandbyPosCmb.EnableWindow(TRUE);
			}
			else
			{
				m_AntStandbyPosCmb.EnableWindow(FALSE);
			}
		}
	}

	CString strCid;
	m_CidCmb.GetWindowTextW(strCid);

	// 
	SetComboboxCIDParam(strSatName, strCid);
	return TRUE;
}

/*============================================================================*/
/*! CCidSelect

-# CID���̃p�����[�^��ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CCidSelect::SetComboboxCIDParam(CString sat, CString cid)
{
	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
	stCIDParam stCid = cidlist[sat][cid];

	m_TlmBrSCmb.SelectStringEx(-1, stCid.TlmBitRateS);
	m_TlmBrXCmb.SelectStringEx(-1, stCid.TlmBitRateX);
	m_TlmBrHXCmb.SelectStringEx(-1, stCid.TlmBitRateHX);
	m_TlmBrKaCmb.SelectStringEx(-1, stCid.TlmBitRateKa);
	m_CmdBrSCmb.SelectStringEx(-1, stCid.CmdBitRateS);
	m_CmdBrXCmb.SelectStringEx(-1, stCid.CmdBitRateX);
	m_TPwrCmb.SelectStringEx(-1, stCid.TrsPower);
	m_CarrTLbSCmb.SelectStringEx(-1, stCid.CarrLoopBandS);
	m_CarrTLbXCmb.SelectStringEx(-1, stCid.CarrLoopBandX);
	m_CarrTLbKaCmb.SelectStringEx(-1, stCid.CarrLoopBandKa);
	m_UpLinkShapeCmb.SelectStringEx(-1, stCid.UpLinkSweepShape);
	m_RngPrmModeCmb.SelectStringEx(-1, stCid.RngMesMode);
	if (m_xtx == XTX_Y_ID)
	{
		m_MdlXCmb.SelectStringEx(-1, stCid.Modulation);
	}
	else
	{
		m_MdlSCmb.SelectStringEx(-1, stCid.Modulation);
	}
	m_CmdMdlCmb.SelectStringEx(-1, stCid.CmdMod);
	m_CmdFreqCmb.SelectStringEx(-1, stCid.CmdSubCarrFreq);
	m_TDoppCmb.SelectStringEx(-1, stCid.TransDoppler);
	m_AntAutoCmb.SelectStringEx(-1, stCid.AntAutoTrack);
	m_AntStandbyCmb.SelectStringEx(-1, stCid.AntAosPosn);
	m_AntStandbyPosCmb.SelectStringEx(-1, stCid.AntWaitPoint);

	return TRUE;
}


/*============================================================================*/
/*! CCidSelect

-# �w�q���I���x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselSatelliteCmb()
{
	CString str;

	int idx = m_SetalliteCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_SetalliteCmb.GetLBText(idx, str);

	SetComboboxSatCIDParam(str);
}

/*============================================================================*/
/*! CCidSelect

-# �wCID�x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCidCmb()
{
	CString cid;

	int idx = m_CidCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CidCmb.GetLBText(idx, cid);

	if (cid == CID_INVALID)
	{
		return;
	}
	else
	{
		int invalid = m_CidCmb.SelectStringEx(-1, CID_INVALID);
		m_CidCmb.DeleteString(invalid);
		m_CidCmb.SetCurSel(idx);
	}

	CString sat;
	m_SetalliteCmb.GetWindowTextW(sat);

	SetComboboxCIDParam(sat, cid);
}


/*============================================================================*/
/*! CCidSelect

-# �ύX���ꂽ�p�����[�^��CID�o�^����Ă��邩�m�F

@param  �Ȃ�
@retval �Y��CID�ԍ�
*/
/*============================================================================*/
CString CCidSelect::CheckCid()
{
	UpdateData();

	// CID�擾
	map<CString, map<CString, stCIDParam>> &cidAllList = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
	map<CString, stCIDParam> &cidList = cidAllList[m_strSatelliteCmb];

	// CID�p�����[�^�L���t���O�擾
	map<CString, stCIDPrmFlg> &cidPrmFlg = theApp.GetSatelliteData().m_CIDSlctData.GetCidPrmFlgList();
	stCIDPrmFlg stPrmFlg = cidPrmFlg[m_strSatelliteCmb];

	CString cid = _T("");
	for (auto itr = cidList.begin(); itr != cidList.end(); ++itr)
	{
		cid = itr->first;
		stCIDParam &st = itr->second;

		if (m_xtx == XTX_Y_ID)
		{
			if ((m_strTlmBRSCmb.IsEmpty() == true || st.TlmBitRateS == m_strTlmBRSCmb)					// �e�����g���r�b�g���[�g S��
				&& (m_strTlmBRXCmb.IsEmpty() == true || st.TlmBitRateX == m_strTlmBRXCmb)				// �e�����g���r�b�g���[�g X��
				&& (m_strTlmBRHXCmb.IsEmpty() == true || st.TlmBitRateHX == m_strTlmBRHXCmb)			// �e�����g���r�b�g���[�g X����
				&& (m_strTlmBRKaCmb.IsEmpty() == true || st.TlmBitRateKa == m_strTlmBRKaCmb)			// �e�����g���r�b�g���[�g Ka����
				&& (m_strCmdBRSCmb.IsEmpty() == true || st.CmdBitRateS == m_strCmdBRSCmb)				// �R�}���h�r�b�g���[�g S��
				&& (m_strCmdBRXCmb.IsEmpty() == true || st.CmdBitRateX == m_strCmdBRXCmb)				// �R�}���h�r�b�g���[�g X��
				&& (m_strTPwrCmb.IsEmpty() == true || st.TrsPower == m_strTPwrCmb)						// ���M�d��
				&& (m_strCarrTLbSCmb.IsEmpty() == true || st.CarrLoopBandS == m_strCarrTLbSCmb)			// �L�����A��M���[�v�o���h S��
				&& (m_strCarrTLbXCmb.IsEmpty() == true || st.CarrLoopBandX == m_strCarrTLbXCmb)			// �L�����A��M���[�v�o���h X��
				&& (m_strCarrTLbKaCmb.IsEmpty() == true || st.CarrLoopBandKa == m_strCarrTLbKaCmb)		// �L�����A��M���[�v�o���h Ka��
				&& (m_strUpLinkShapeCmb.IsEmpty() == true || st.UpLinkSweepShape == m_strUpLinkShapeCmb)	// �A�b�v�����N�|���p�����[�^ �`��
				&& (m_strRngPrmModeCmb.IsEmpty() == true || st.RngMesMode == m_strRngPrmModeCmb)		// �����W�v���p�����[�^ ���[�h
				&& (m_strMdlXCmb.IsEmpty() == true || st.Modulation == m_strMdlXCmb)					// �ϒ����� X��
				&& (m_strCmdMdlCmb.IsEmpty() == true || st.CmdMod == m_strCmdMdlCmb)					// �R�}���h�ϒ��x
				&& (m_strCmdFreqCmb.IsEmpty() == true ||  st.CmdSubCarrFreq == m_strCmdFreqCmb)			// �R�}���h�T�u�L�����A���g��
				&& (m_strTDoppCmb.IsEmpty() == true || st.TransDoppler == m_strTDoppCmb)				// ���M�h�b�v��
				&& (m_strAntAutoCmb.IsEmpty() == true || st.AntAutoTrack == m_strAntAutoCmb)			// �A���e�i�����ǔ�
				&& (m_strAntStandbyCmb.IsEmpty() == true || st.AntAosPosn == m_strAntStandbyCmb)		// �A���e�i�҂��󂯎w��
				&& (m_strAntStandbyPosCmb.IsEmpty() == true || st.AntWaitPoint == m_strAntStandbyPosCmb)	// �A���e�i�ҋ@�ʒu�w��
				)
			{
				return cid;
			}
		}
		else
		{
			if ((m_strTlmBRSCmb.IsEmpty() == true || st.TlmBitRateS == m_strTlmBRSCmb)					// �e�����g���r�b�g���[�g S��
				&& (m_strTlmBRXCmb.IsEmpty() == true || st.TlmBitRateX == m_strTlmBRXCmb)				// �e�����g���r�b�g���[�g X��
				&& (m_strTlmBRHXCmb.IsEmpty() == true || st.TlmBitRateHX == m_strTlmBRHXCmb)			// �e�����g���r�b�g���[�g X����
				&& (m_strTlmBRKaCmb.IsEmpty() == true || st.TlmBitRateKa == m_strTlmBRKaCmb)			// �e�����g���r�b�g���[�g Ka����
				&& (m_strCmdBRSCmb.IsEmpty() == true || st.CmdBitRateS == m_strCmdBRSCmb)				// �R�}���h�r�b�g���[�g S��
				&& (m_strCmdBRXCmb.IsEmpty() == true || st.CmdBitRateX == m_strCmdBRXCmb)				// �R�}���h�r�b�g���[�g X��
				&& (m_strTPwrCmb.IsEmpty() == true || st.TrsPower == m_strTPwrCmb)						// ���M�d��
				&& (m_strCarrTLbSCmb.IsEmpty() == true || st.CarrLoopBandS == m_strCarrTLbSCmb)			// �L�����A��M���[�v�o���h S��
				&& (m_strCarrTLbXCmb.IsEmpty() == true || st.CarrLoopBandX == m_strCarrTLbXCmb)			// �L�����A��M���[�v�o���h X��
				&& (m_strCarrTLbKaCmb.IsEmpty() == true || st.CarrLoopBandKa == m_strCarrTLbKaCmb)		// �L�����A��M���[�v�o���h Ka��
				&& (m_strUpLinkShapeCmb.IsEmpty() == true || st.UpLinkSweepShape == m_strUpLinkShapeCmb)	// �A�b�v�����N�|���p�����[�^ �`��
				&& (m_strRngPrmModeCmb.IsEmpty() == true || st.RngMesMode == m_strRngPrmModeCmb)		// �����W�v���p�����[�^ ���[�h
				&& (m_strMdlSCmb.IsEmpty() == true || st.Modulation == m_strMdlSCmb)					// �ϒ����� S��
				&& (m_strCmdMdlCmb.IsEmpty() == true || st.CmdMod == m_strCmdMdlCmb)					// �R�}���h�ϒ��x
				&& (m_strCmdFreqCmb.IsEmpty() == true || st.CmdSubCarrFreq == m_strCmdFreqCmb)			// �R�}���h�T�u�L�����A���g��
				&& (m_strTDoppCmb.IsEmpty() == true || st.TransDoppler == m_strTDoppCmb)				// ���M�h�b�v��
				&& (m_strAntAutoCmb.IsEmpty() == true || st.AntAutoTrack == m_strAntAutoCmb)			// �A���e�i�����ǔ�
				&& (m_strAntStandbyCmb.IsEmpty() == true || st.AntAosPosn == m_strAntStandbyCmb)		// �A���e�i�҂��󂯎w��
				&& (m_strAntStandbyPosCmb.IsEmpty() == true || st.AntWaitPoint == m_strAntStandbyPosCmb)	// �A���e�i�ҋ@�ʒu�w��
				)
			{
				return cid;
			}
		}
	}

	return CID_INVALID;
}
	
/*============================================================================*/
/*! CCidSelect

-# �w�e�����g���r�b�g���[�g S�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTlmbrSCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_TlmBrSCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TlmBrSCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strTlmBRSCmb)
	{
//		return;
	}

	UpdateData();

#ifdef CIDSEL_GREAT
	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
#else
	CString newCid = _T("");
	BOOL bFind = FALSE;
	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
	for (auto itr = cidlist.begin(); itr != cidlist.end(); ++itr)
	{
		CString sat = itr->first;

		if (sat == m_strSatelliteCmb)
		{
			CString cid = _T("");
			map<CString, stCIDParam> &cidplist = itr->second;
			for (auto itrp = cidplist.begin(); itrp != cidplist.end(); ++itrp)
			{
				cid = itrp->first;
				stCIDParam &st = itrp->second;

				if (st.TlmBitRateS == str)
				{
					bFind = TRUE;
					break;
				}
			}

			if (bFind)
			{
				newCid = cid;
				break;
			}
		}
	}

	if (bFind)
	{
		m_CidCmb.SelectStringEx(-1, newCid);
		SetComboboxCIDParam(m_strSatelliteCmb, newCid);
	}
#endif /* CIDSEL_GREAT */
}


/*============================================================================*/
/*! CCidSelect

-# �w�e�����g���r�b�g���[�g X�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTlmbrXCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_TlmBrXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TlmBrXCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strTlmBRXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�e�����g���r�b�g���[�g X�����x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTlmbrHxCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_TlmBrHXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TlmBrHXCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strTlmBRHXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�e�����g���r�b�g���[�g Ka�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTlmbrKaCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_TlmBrKaCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TlmBrKaCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strTlmBRKaCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�R�}���h�r�b�g���[�g S�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCmdbrSCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_CmdBrSCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CmdBrSCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strCmdBRSCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�R�}���h�r�b�g���[�g X�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCmdbrXCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_CmdBrXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CmdBrXCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strCmdBRXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w���M�d�́x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTpwrPwrCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_TPwrCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TPwrCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strTPwrCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�L�����A��M���[�v�o���h S�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselClbSCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_CarrTLbSCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CarrTLbSCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strCarrTLbSCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�L�����A��M���[�v�o���h X�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselClbXCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_CarrTLbXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CarrTLbXCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strCarrTLbXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�L�����A��M���[�v�o���h Ka�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselClbKaCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_CarrTLbKaCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CarrTLbKaCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strCarrTLbKaCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�A�b�v�����N�|���p�����[�^ �`��x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselUplnkShpCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_UpLinkShapeCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_UpLinkShapeCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strUpLinkShapeCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�����W�v���p�����[�^ �T���v�����O���[�g�x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselRprmSrCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_RngPrmModeCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_RngPrmModeCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strRngPrmModeCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�R�}���h�ϒ��x�x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCmdmdlMdlCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_CmdMdlCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CmdMdlCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strCmdMdlCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�R�}���h�T�u�L�����A���g���x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselCmdsubfreqFreqCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_CmdFreqCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_CmdFreqCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strCmdFreqCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�ϒ����� S�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselMmtdSCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_MdlSCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_MdlSCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strMdlSCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�ϒ����� X�сx�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselMmtdXCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_MdlXCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_MdlXCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strMdlXCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w���M�h�b�v���x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselTdopCmpCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_TDoppCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_TDoppCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strTDoppCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�A���e�i�����ǔ��x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselAntatMigCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_AntAutoCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_AntAutoCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strAntAutoCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�A���e�i�҂��󂯎w���x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselAntstdorientedCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_AntStandbyCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_AntStandbyCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strAntAutoCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# �w�A���e�i�ҋ@�ʒu�w���x�R���{ �ύX�ʒm

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnCbnSelchangeCidselAntstdposorientedCmb()
{
	CString str;

	// �ύX��̕�����擾
	int idx = m_AntStandbyPosCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_AntStandbyPosCmb.GetLBText(idx, str);

	// �O��Ɠ����Ȃ牽�����Ȃ�
	if (str == m_strAntStandbyPosCmb)
	{
//		return;
	}

	UpdateData();

	CString cid = CheckCid();
	int ret = m_CidCmb.SelectStringEx(-1, cid);
	if (ret == CB_ERR)
	{
		int idx = m_CidCmb.InsertString(-1, cid);
		m_CidCmb.SetCurSel(idx);
	}
}


/*============================================================================*/
/*! CCidSelect

-# CID �p�����[�^�\���̂�Ԃ�

@param  �Ȃ�
@retval CID �p�����[�^�\����
*/
/*============================================================================*/
stCIDParam CCidSelect::GetParameter()
{
	// �q����
	map<CString, map<CString, stCIDParam>> &cidlist = theApp.GetSatelliteData().m_CIDSlctData.GetCidParamList();
	stCIDParam stCid = cidlist[m_strSatelliteCmb][m_strCIDCmb];

	stCIDParam st;
	st.CID = m_strCIDCmb;							// CID
	st.TlmBitRateS = stCid.TlmBitRateS;				// �e�����g���r�b�g���[�g S��
	st.TlmBitRateX = stCid.TlmBitRateX;				// �e�����g���r�b�g���[�g X��
	st.TlmBitRateHX = stCid.TlmBitRateHX;			// �e�����g���r�b�g���[�g X����
	st.TlmBitRateKa = stCid.TlmBitRateKa;			// �e�����g���r�b�g���[�g Ka��
	st.CmdBitRateS = stCid.CmdBitRateS;				// �R�}���h�r�b�g���[�g S��
	st.CmdBitRateX = stCid.CmdBitRateX;				// �R�}���h�r�b�g���[�g X��
	st.TrsPower = stCid.TrsPower;					// ���M�d��
	st.CarrLoopBandS = stCid.CarrLoopBandS;			// �L�����A��M���[�v�o���h S��
	st.CarrLoopBandX = stCid.CarrLoopBandX;			// �L�����A��M���[�v�o���h X��
	st.CarrLoopBandKa = stCid.CarrLoopBandKa;		// �L�����A��M���[�v�o���h Ka��
	st.UpLinkSweepShape = stCid.UpLinkSweepShape;	// �A�b�v�����N�|���p�����[�^ �`��
	st.RngMesMode = stCid.RngMesMode;				// �����W�v���p�����[�^ ���[�h
	st.ModulationS = stCid.ModulationS;				// �ϒ����� S��
	st.ModulationX = stCid.ModulationX;				// �ϒ����� X��
	st.CmdMod = stCid.CmdMod;						// �R�}���h�ϒ��x
	st.CmdSubCarrFreq = stCid.CmdSubCarrFreq;		// �R�}���h�T�u�L�����A���g��
	st.TransDoppler = stCid.TransDoppler;			// ���M�h�b�v��
	st.AntAutoTrack = stCid.AntAutoTrack;			// �A���e�i�����ǔ�
	st.AntAosPosn = stCid.AntAosPosn;				// �A���e�i�҂��󂯎w��
	st.AntWaitPoint = stCid.AntWaitPoint;			// �A���e�i�ҋ@�ʒu�w��

	return st;
}


/*============================================================================*/
/*! CCidSelect

-# �q���ŗLDB���f�[�^�擾

@param  �q����
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::GetEqSatDB(CString satname)
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;

	struct xtx_t		*xtx_t_adr;		/* �w�ё��M�ݔ��g�p�L�����e�[�u���A�h���X */

	if (satname == _T(""))
	{
		return;
	}

	// DB�A�N�Z�X�N���X
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
	original_adr = satinf_adr;
	sattbl_adr = (char *)satinf_adr;

	/*--------------------------------------*/
	/* �w�ё��M�ݔ��g�p�L�����擾 */
	/*--------------------------------------*/
	while (1)
	{
		satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_t�ɕϊ�*/
		if (satinf_adr->l_id == ENDOFSAT_ID)
		{
			/*���ʂh�c���I���̏ꍇ*/
			break;
		}
		if (satinf_adr->l_id == XTX_ID)
		{
			/*���ʂh�c���u�w�ё��M�ݔ��g�p�L�����v�̏ꍇ*/
			xtx_t_adr = (struct xtx_t *)satinf_adr;
			m_xtx = xtx_t_adr->l_x_tx;
			break;
		}
		sattbl_adr += satinf_adr->l_len;
	}

}

/*============================================================================*/
/*! CCidSelect

-# �w����x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CCidSelect::OnBnClickedOk()
{
	UpdateData();

	if (m_strCIDCmb == CID_INVALID)
	{
		CString title;
		GetWindowText(title);
		MessageBox(_T("�I������Ă���CID���s���ł��B"), title, MB_OK | MB_ICONWARNING);
		return;
	}

	OnOK();
}
