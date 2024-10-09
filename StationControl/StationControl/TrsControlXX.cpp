/*============================================================================*/
/*! TrsControlXX.cpp

-# X�ё��M������
*/
/*============================================================================*/
// TrsControlXX.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlXX.h"
#include "afxdialogex.h"
#include "TrsControlMng.h"
#include "HookMsgBox.h"


// CTrsControlXX �_�C�A���O

IMPLEMENT_DYNAMIC(CTrsControlXX, CDialogBase)

/*============================================================================*/
/*! CTrsControlXX

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CTrsControlXX::CTrsControlXX(CWnd* pParent /*=NULL*/)
: CDialogBase(CTrsControlXX::IDD, pParent)
	, m_strIndexCmd(_T(""))
	, m_strIndexRng(_T(""))
	, m_strIndexCpr(_T(""))
	, m_strXXRng(_T(""))
	, m_strXXRngCont(_T(""))
{

}

/*============================================================================*/
/*! CTrsControlXX

-# �R���X�g���N�^

@param	IDD�FIDD
@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CTrsControlXX::CTrsControlXX(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CTrsControlXX

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CTrsControlXX::~CTrsControlXX()
{
}

void CTrsControlXX::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRXXCTRL_IDX_CMD_EDIT, m_strIndexCmd);
	DDX_Text(pDX, IDC_TRXXCTRL_IDX_RNG_EDIT, m_strIndexRng);
	DDX_Text(pDX, IDC_TRXXCTRL_IDX_CR_EDIT, m_strIndexCpr);
	DDX_Text(pDX, IDC_TRXXCTRL_XXRNG_EDIT, m_strXXRng);
	DDX_Text(pDX, IDC_TRXXCTRL_XXRNG_CONT_STATIC, m_strXXRngCont);
	DDX_Control(pDX, IDC_TRXXCTRL_RF_OFF_BTN, m_RfPwrOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RF_ON_BTN, m_RfPwrOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_NOUSE_OFF_BTN, m_NoUseOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_NOUSE_ON_BTN, m_NoUseOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_STOP_BTN, m_SweepCountMStopBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_INC_BTN, m_SweepCountMIncBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_INCDEC_BTN, m_SweepCountMIncDecBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_DEC_BTN, m_SweepCountMDecBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_DECINC_BTN, m_SweepCountMDecIncBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_STOP_BTN, m_SweepCountSStopBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_INC_BTN, m_SweepCountSIncBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_INCDEC_BTN, m_SweepCountSIncDecBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_DEC_BTN, m_SweepCountSDecBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_DECINC_BTN, m_SweepCountSDecIncBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_MDL_OFF_BTN, m_ModulationOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_MDL_ON_BTN, m_ModulationOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_HOLD_OFF_BTN, m_HoldToneOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_HOLD_ON_BTN, m_HoldToneOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DC_OFF_BTN, m_DoppContOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DC_ON_BTN, m_DoppContOnBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_XXCOH_COH_BTN, m_XXCohCohBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_XXCOH_INCOH_BTN, m_XXCohInCohBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RNGING_STOP_BTN, m_RngingStopBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RNGING_START_BTN, m_RngingStartBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RGNTYPE_STOP_BTN, m_RRngStopBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RGNTYPE_START_BTN, m_RRngStartBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_IDX_CMD_BTN, m_IndexCmdBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_IDX_RNG_BTN, m_IndexRngBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_IDX_CR_BTN, m_IndexCRBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_XXRNG_C_BTN, m_XXRngNumBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SN_BTN, m_SweepNumBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_CHECK, m_SweepContSubChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_NOR_CHECK, m_SweepContNorChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_TRI_CHECK, m_SweepContTriChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_NOR_CHECK, m_SweepContSubNorChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_TRI_CHECK, m_SweepContSubTriChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_TX_STATIC, m_grTxControl);
	DDX_Control(pDX, IDC_TRXXCTRL_RF_STATIC, m_grRfPower);
	DDX_Control(pDX, IDC_TRXXCTRL_NOUSE_STATIC, m_grNoUse);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_STATIC, m_grSweepCont);
	DDX_Control(pDX, IDC_TRXXCTRL_SWEET_SUB_STATIC, m_grSweepContSub);
	DDX_Control(pDX, IDC_TRXXCTRL_DC_STATIC, m_grDoppCont);
	DDX_Control(pDX, IDC_TRXXCTRL_MDL_STATIC, m_grModulation);
	DDX_Control(pDX, IDC_TRXXCTRL_HOLD_STATIC, m_grHoldTone);
	DDX_Control(pDX, IDC_TRXXCTRL_IDX_STATIC, m_grIndex);
	DDX_Control(pDX, IDC_TRXXCTRL_RNGCTRL_STATIC, m_grRngControl);
	DDX_Control(pDX, IDC_TRXXCTRL_XXRNG_STATIC, m_grXXRng);
	DDX_Control(pDX, IDC_TRXXCTRL_XXCOH_STATIC, m_grXXCoh);
	DDX_Control(pDX, IDC_TRXCTRL_RNGING_STATIC, m_grRnging);
	DDX_Control(pDX, IDC_TRXXCTRL_RGNTYPE_STATIC, m_grRRng);
}


BEGIN_MESSAGE_MAP(CTrsControlXX, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CTrsControlXX::OnMenuCloseClose)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_NOR_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetNorCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_TRI_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetTriCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_NOR_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubNorCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_TRI_CHECK, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubTriCheck)
	ON_BN_CLICKED(IDC_TRXXCTRL_SN_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_IDX_CMD_BTN, &CTrsControlXX::OnBnClickedTrxxctrlIdxCmdBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_IDX_RNG_BTN, &CTrsControlXX::OnBnClickedTrxxctrlIdxRngBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_IDX_CR_BTN, &CTrsControlXX::OnBnClickedTrxxctrlIdxCrBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXRNG_C_BTN, &CTrsControlXX::OnBnClickedTrxxctrlXxrngCBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRXXCTRL_RF_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRfOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RF_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRfOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_NOUSE_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlNouseOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_NOUSE_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlNouseOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_STOP_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_INC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetIncBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_INCDEC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetIncdecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_DEC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetDecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_DECINC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetDecincBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_STOP_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_INC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubIncBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_INCDEC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubIncdecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_DEC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubDecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_DECINC_BTN, &CTrsControlXX::OnBnClickedTrxxctrlSweetSubDecincBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_MDL_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlMdlOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_MDL_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlMdlOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_DC_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlDcOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_DC_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlDcOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_HOLD_OFF_BTN, &CTrsControlXX::OnBnClickedTrxxctrlHoldOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_HOLD_ON_BTN, &CTrsControlXX::OnBnClickedTrxxctrlHoldOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXCOH_COH_BTN, &CTrsControlXX::OnBnClickedTrxxctrlXxcohCohBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXCOH_INCOH_BTN, &CTrsControlXX::OnBnClickedTrxxctrlXxcohIncohBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RNGING_STOP_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRngingStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RNGING_START_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRngingStartBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RGNTYPE_STOP_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRgntypeStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RGNTYPE_START_BTN, &CTrsControlXX::OnBnClickedTrxxctrlRgntypeStartBtn)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrsControlXX ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CTrsControlXX

-# �t�@�C��-���郁�j���[����

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnMenuCloseClose()
{
	// �����ꂩ�̃{�^�������s���̏ꍇ
	if (m_RfPwrOffBtn.IsColorIgnore2() ||
		m_RfPwrOnBtn.IsColorIgnore2() ||
		m_NoUseOffBtn.IsColorIgnore2() ||
		m_NoUseOnBtn.IsColorIgnore2() ||
		m_SweepCountMStopBtn.IsColorIgnore2() ||
		m_SweepCountMIncBtn.IsColorIgnore2() ||
		m_SweepCountMIncDecBtn.IsColorIgnore2() ||
		m_SweepCountMDecBtn.IsColorIgnore2() ||
		m_SweepCountMDecIncBtn.IsColorIgnore2() ||
		m_SweepCountSStopBtn.IsColorIgnore2() ||
		m_SweepCountSIncBtn.IsColorIgnore2() ||
		m_SweepCountSIncDecBtn.IsColorIgnore2() ||
		m_SweepCountSDecBtn.IsColorIgnore2() ||
		m_SweepCountSDecIncBtn.IsColorIgnore2() ||
		m_SweepNumBtn.IsColorIgnore2() ||
		m_DoppContOffBtn.IsColorIgnore2() ||
		m_DoppContOnBtn.IsColorIgnore2() ||
		m_ModulationOffBtn.IsColorIgnore2() ||
		m_ModulationOnBtn.IsColorIgnore2() ||
		m_HoldToneOffBtn.IsColorIgnore2() ||
		m_HoldToneOnBtn.IsColorIgnore2() ||
		m_IndexCmdBtn.IsColorIgnore2() ||
		m_IndexRngBtn.IsColorIgnore2() ||
		m_IndexCRBtn.IsColorIgnore2() ||
		m_XXRngNumBtn.IsColorIgnore2() ||
		m_XXCohCohBtn.IsColorIgnore2() ||
		m_XXCohInCohBtn.IsColorIgnore2() ||
		m_RngingStopBtn.IsColorIgnore2() ||
		m_RngingStartBtn.IsColorIgnore2() ||
		m_RRngStopBtn.IsColorIgnore2() ||
		m_RRngStartBtn.IsColorIgnore2())
	{
		CString title;
		GetWindowText(title);

		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			return;
		}
	}

	CloseDlg();
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CTrsControlXX

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CTrsControlXX::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	// RF-POWER
	m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	m_RfPwrOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// NO-USE
	m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	m_NoUseOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	m_NoUseOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SWEEP-CONT
	m_SweepContNorChkBtn.SetCheck(1);
	m_SweepCountMStopBtn.SetColor(COLOR_REMOTES);
	m_SweepCountMStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountMIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountMIncDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountMDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountMDecIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// SWEEP-CONT(SubCarr)
	m_SweepContSubNorChkBtn.SetCheck(1);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);
	m_SweepCountSStopBtn.SetColor(COLOR_REMOTES);
	m_SweepCountSStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountSStopBtn.EnableWindow(FALSE);
	m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountSIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountSIncBtn.EnableWindow(FALSE);
	m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountSIncDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountSIncDecBtn.EnableWindow(FALSE);
	m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
	m_SweepCountSDecBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountSDecBtn.EnableWindow(FALSE);
	m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	m_SweepCountSDecIncBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_SweepCountSDecIncBtn.EnableWindow(FALSE);

	// MODULATION
	m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	m_ModulationOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_ModulationOnBtn.SetColor(COLOR_REMOTE);
	m_ModulationOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// HOLD-TONE
	m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	m_HoldToneOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	m_HoldToneOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// DOPP-CONT
	m_DoppContOffBtn.SetColor(COLOR_REMOTES);
	m_DoppContOffBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_DoppContOnBtn.SetColor(COLOR_REMOTE);
	m_DoppContOnBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// X/X-COH
	m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	m_XXCohCohBtn.SetCtrlButton(TRUE);			// Ctrl�L�[ +
	m_XXCohInCohBtn.SetColor(COLOR_REMOTE);
	m_XXCohInCohBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// RNGING
	m_RngingStopBtn.SetColor(COLOR_REMOTES);
	m_RngingStopBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +
	m_RngingStartBtn.SetColor(COLOR_REMOTE);
	m_RngingStartBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// �Đ��^
	m_RRngStopBtn.SetColor(COLOR_REMOTES);
	m_RRngStopBtn.SetShiftButton(TRUE);			// Shift�L�[ +
	m_RRngStartBtn.SetColor(COLOR_REMOTE);
	m_RRngStartBtn.SetShiftButton(TRUE);		// Shift�L�[ +

	// SWEEP-NUM
	m_SweepNumBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_SweepNumBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// INDEX-CMD
	m_IndexCmdBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCmdBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// INDEX-RNG
	m_IndexRngBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexRngBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// INDEX-C+R
	m_IndexCRBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_IndexCRBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +

	// X/X-RNG Cnt
	m_XXRngNumBtn.SetColor(GetSysColor(COLOR_3DFACE));
	m_XXRngNumBtn.SetCtrlButton(TRUE);		// Ctrl�L�[ +


	// �O���[�v�{�b�N�X
	m_grTxControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grTxControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRfPower.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grNoUse.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSweepCont.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grSweepContSub.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grSweepContSub.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grDoppCont.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grDoppCont.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grModulation.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grHoldTone.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grIndex.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grIndex.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRngControl.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRngControl.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grXXRng.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grXXRng.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grXXCoh.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grXXCoh.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRnging.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRnging.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRRng.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRRng.SetGroupType(CGroupBoxEx::eGroupType_Round);

	// �^�C���A�E�g�J�E���g�_�E���l�����ݒ�
	m_l_rf_power_off_cntdwn = -1;
	m_l_rf_power_on_cntdwn = -1;
	m_l_no_use_off_cntdwn = -1;
	m_l_no_use_on_cntdwn = -1;
	m_l_main_sweep_cont_stop_cntdwn = -1;
	m_l_main_sweep_cont_inc_cntdwn = -1;
	m_l_main_sweep_cont_incdec_cntdwn = -1;
	m_l_main_sweep_cont_dec_cntdwn = -1;
	m_l_main_sweep_cont_decinc_cntdwn = -1;
	m_l_sub_sweep_cont_stop_cntdwn = -1;
	m_l_sub_sweep_cont_inc_cntdwn = -1;
	m_l_sub_sweep_cont_incdec_cntdwn = -1;
	m_l_sub_sweep_cont_dec_cntdwn = -1;
	m_l_sub_sweep_cont_decinc_cntdwn = -1;
	m_l_sweep_num_cntdwn = -1;
	m_l_dopp_cont_off_cntdwn = -1;
	m_l_dopp_cont_on_cntdwn = -1;
	m_l_cmd_mod_off_cntdwn = -1;
	m_l_cmd_mod_on_cntdwn = -1;
	m_l_rng_mod_off_cntdwn = -1;
	m_l_rng_mod_on_cntdwn = -1;
	m_l_modulation_off_cntdwn = -1;
	m_l_modulation_on_cntdwn = -1;
	m_l_hold_tone_off_cntdwn = -1;
	m_l_hold_tone_on_cntdwn = -1;
	m_l_xx_rng_off_cntdwn = -1;
	m_l_xx_rng_on_cntdwn = -1;
	m_l_coh_incoh_coh_cntdwn = -1;
	m_l_coh_incoh_incoh_cntdwn = -1;
	m_l_cmd_mod_idx_cntdwn = -1;
	m_l_rng_mod_idx_cntdwn = -1;
	m_l_car_mod_idx_cntdwn = -1;
	m_l_rnging_stop_cntdwn = -1;
	m_l_rnging_start_cntdwn = -1;
	m_l_rep_rnging_stop_cntdwn = -1;
	m_l_rep_rnging_start_cntdwn = -1;
	m_l_rng_type = -1;
	m_l_dsnrng_turn_start_flag = -1;
	m_l_dsnrng_regen_start_flag = -1;
	m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �A��SWEEP�񐔓����o�b�t�@���Ď��f�[�^�ŏ���������
	// ���݂̏�Ԃ��Ď��c�a����擾 
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_l_sweep_num_set = (long)obs_adr->d_data;
	}

#if 0
	// �z�X�g���擾
	memset(m_sz_hostname, (char)NULL, sizeof(m_sz_hostname));
	gethostname(m_sz_hostname, sizeof(m_sz_hostname));

	// ���M�����ʏ�ԕ\���^�C���A�E�g�R�[���o�b�N���[�`���N��
	show_opsc_condition();

	// �J�E���g�_�E���^�C���A�E�g�R�[���o�b�N���[�`���N��
	count_down();
#endif

	CenterWindowEx();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CTrsControlXX

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTrsControlXX::UpdateDlg()
{
	// ���M�����ʏ�ԕ\���^�C���A�E�g�R�[���o�b�N���[�`���N��
	show_opsc_condition();

	// �J�E���g�_�E���^�C���A�E�g�R�[���o�b�N���[�`���N��
	count_down();

	return TRUE;
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wRF-POWER�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRfOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_ON/OFF_CONT"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_XX_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wRF-POWER�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRfOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_ON/OFF_CONT"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_XX_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wNO-USE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// �P�c64m
		if (col == COLOR_REMOTE)
		{
			// �����[�g�i��I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// ���䒆

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_on_cntdwn = -1;
		}
	}
	else if (station == 2)
	{
		// ���V�Y34m
		if (col == COLOR_REMOTE)
		{
			// �����[�g�i��I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// ���䒆

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_on_cntdwn = -1;
		}
		else if (col == COLOR_REMOTES)
		{
			// �����[�g�i�I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE_RESET"));

			// �J�E���g�_�E���J�n
			m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_on_cntdwn = -1;
		}
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wNO-USE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();

	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// �P�c64m
		if (col == COLOR_REMOTE)
		{
			// �����[�g�i��I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// ���䒆

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_off_cntdwn = -1;
		}
	}
	else if (station == 2)
	{
		// ���V�Y34m
		if (col == COLOR_REMOTE)
		{
			// �����[�g�i��I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;
		}
		else if (col == COLOR_CTRLIN)
		{
			// ���䒆

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_off_cntdwn = -1;
		}
		else if (col == COLOR_REMOTES)
		{
			//�@�����[�g�i�I���j

			// �{�^���𐧌䒆�ɂ���
			m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

			// ����ʒm���M
			theApp.GetSatelliteData().opctproc(_T("X-UL_CNT.RF_TRIP_AREA_NO_USE"), _T("NO_USE"));

			// �J�E���g�_�E���J�n
			m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;

			// �����̃J�E���g�_�E���I��
			m_l_no_use_off_cntdwn = -1;
		}
	}

	// �X�V
	m_NoUseOnBtn.Invalidate();
}

/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wNOR�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetNorCheck()
{
	// NOR �� TRI ��r������
	m_SweepContNorChkBtn.SetCheck(1);
	m_SweepContTriChkBtn.SetCheck(0);
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wTRI�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetTriCheck()
{
	// NOR �� TRI ��r������
	m_SweepContTriChkBtn.SetCheck(1);
	m_SweepContNorChkBtn.SetCheck(0);
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetStopBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

	// �J�E���g�_�E���J�n
	m_l_main_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetIncBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_main_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wINCDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetIncdecBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_main_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetDecBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_main_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT�x-�wDECINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetDecincBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_main_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountMStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountMDecIncBtn.SetColor(COLOR_CTRLIN);
	m_l_flg_ctrl_sweep = CTRL_SWEEP_BEFORE;

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�`�F�b�N�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubCheck()
{
	if (m_SweepContSubChkBtn.GetCheck())
	{
		// ����
		m_SweepCountSStopBtn.EnableWindow(TRUE);
		m_SweepCountSIncBtn.EnableWindow(TRUE);
		m_SweepCountSIncDecBtn.EnableWindow(TRUE);
		m_SweepCountSDecBtn.EnableWindow(TRUE);
		m_SweepCountSDecIncBtn.EnableWindow(TRUE);

		m_SweepContSubNorChkBtn.EnableWindow(TRUE);
		m_SweepContSubTriChkBtn.EnableWindow(TRUE);
	}
	else
	{
		// �񊈐�
		m_SweepCountSStopBtn.EnableWindow(FALSE);
		m_SweepCountSIncBtn.EnableWindow(FALSE);
		m_SweepCountSIncDecBtn.EnableWindow(FALSE);
		m_SweepCountSDecBtn.EnableWindow(FALSE);
		m_SweepCountSDecIncBtn.EnableWindow(FALSE);

		m_SweepContSubNorChkBtn.EnableWindow(FALSE);
		m_SweepContSubTriChkBtn.EnableWindow(FALSE);
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wNOR�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubNorCheck()
{
	// NOR �� TRI ��r������
	m_SweepContSubNorChkBtn.SetCheck(1);
	m_SweepContSubTriChkBtn.SetCheck(0);
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wTRI�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubTriCheck()
{
	// NOR �� TRI ��r������
	m_SweepContSubTriChkBtn.SetCheck(1);
	m_SweepContSubNorChkBtn.SetCheck(0);
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubStopBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

	// �J�E���g�_�E���J�n
	m_l_sub_sweep_cont_stop_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubIncBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sub_sweep_cont_inc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wINCDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubIncdecBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sub_sweep_cont_incdec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wDEC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubDecBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sub_sweep_cont_dec_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wDECINC�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSweetSubDecincBtn()
{
	//  SAW or TRIANGLE �X�C�[�v�̎擾
	if (m_SweepContSubNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_sub_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_sub_sweep_cont_decinc_cntdwn = SWEEP_CONT_TIME_XX_LIMIT;

	// �{�^���𐧌䒆�ɂ���
	m_SweepCountSStopBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSIncDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecBtn.SetColor(COLOR_CTRLIN);
	m_SweepCountSDecIncBtn.SetColor(COLOR_CTRLIN);

	m_SweepContSubChkBtn.EnableWindow(FALSE);
	m_SweepContSubNorChkBtn.EnableWindow(FALSE);
	m_SweepContSubTriChkBtn.EnableWindow(FALSE);

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wSWEEP-NUM�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlSnBtn()
{
	CString str1,str2;

	// �����[�g��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (m_i_priv_flag == 1)) 
	{
		// ����ʒm���M
		m_SweepNumBtn.GetWindowTextW(str2);

		str1.Format(_T("%d"), _wtoi(str2));
		CString str3 = theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_CYCLE"), _T(""), str1);
		if (str3 == _T(""))
		{
			// �L�����Z�����́A���̐���𑗂�Ȃ�
			return;
		}

		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_CYCLE"), str3);

		// �J�E���g�_�E���J�n
		m_l_sweep_num_cntdwn = SWEEP_NUM_TIME_XX_LIMIT;
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wDOPP-CONT�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlDcOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_DoppContOffBtn.SetColor(COLOR_CTRLIN);
	m_DoppContOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
//	theApp.GetSatelliteData().opctproc(_T("XX-DSNTCRFC.TX_DOPP_AID"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("CTRL.AID_UL"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_dopp_cont_off_cntdwn = DOPP_CONT_TIME_XX_LIMIT;

	// �X�V
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wDOPP-CONT�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlDcOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_DoppContOffBtn.SetColor(COLOR_CTRLIN);
	m_DoppContOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
//	theApp.GetSatelliteData().opctproc(_T("XX-DSNTCRFC.TX_DOPP_AID"), _T("ON"));
	theApp.GetSatelliteData().opctproc(_T("CTRL.AID_UL"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_dopp_cont_on_cntdwn = DOPP_CONT_TIME_XX_LIMIT;

	// �X�V
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wMODULATION�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlMdlOffBtn()
{
#if 0	// ���s�\�[�X���R�����g�A�E�g�I
	// �{�^���𐧌䒆�ɂ���
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("XX-TCRMOD.MODULATION"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_modulation_off_cntdwn = MODULATION_TIME_XX_LIMIT;

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
#else	// CMD-MOD

	// �{�^���𐧌䒆�ɂ���
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_IF_MODULATION"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_modulation_off_cntdwn = MODULATION_TIME_XX_LIMIT;

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
#endif
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wMODULATION�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlMdlOnBtn()
{
#if 0	// ���s�\�[�X���R�����g�A�E�g�I
	// �{�^���𐧌䒆�ɂ���
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("XX-TCRMOD.MODULATION"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_modulation_on_cntdwn = MODULATION_TIME_XX_LIMIT;

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
#else	// CMD-MOD
	// �{�^���𐧌䒆�ɂ���
	m_ModulationOffBtn.SetColor(COLOR_CTRLIN);
	m_ModulationOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_IF_MODULATION"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_modulation_on_cntdwn = MODULATION_TIME_XX_LIMIT;

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
#endif
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wHOLD-TONE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlHoldOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-CMDOUT2.HOLD_TONE"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_hold_tone_off_cntdwn = HOLD_TONE_TIME_XX_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wHOLD-TONE�x-�wON�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlHoldOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-CMDOUT2.HOLD_TONE"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_hold_tone_on_cntdwn = HOLD_TONE_TIME_XX_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wINDEX�x-�wCMD�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlIdxCmdBtn()
{
	CString str;
	// �����[�g��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (m_i_priv_flag == 1)) 
	{
		// ����ʒm���M
		str.Format(_T("%s"), (LPCTSTR)m_strIndexCmd);
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_MOD_INDEX"), _T(""), str);
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wINDEX�x-�wRNG�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlIdxRngBtn()
{
	CString str;

	// �����[�g��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (m_i_priv_flag == 1)) 
	{
		// ����ʒm���M
		str.Format(_T("%s"), (LPCTSTR)m_strIndexRng);
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MOD_INDEX"), _T(""), str);
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wINDEX�x-�wC+R�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlIdxCrBtn()
{
	CString str;

	// �����[�g��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (m_i_priv_flag == 1)) 
	{
		// ����ʒm���M
		str.Format(_T("%s"), (LPCTSTR)m_strIndexCmd);
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_MOD_INDEX"), str);
		str.Format(_T("%s"), (LPCTSTR)m_strIndexRng);
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MOD_INDEX"), str);
	}
}


/*============================================================================*/
/*! CTrsControlXX

-# �wRX CONTROL�x-�wX/X-RNG�x-�wNUM�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlXxrngCBtn()
{
	// ����ʒm���M
	UpdateData();
	theApp.GetSatelliteData().opctproc(_T("V-EQU.RNG_NUM"), _T(""), m_strXXRng);
}


/*============================================================================*/
/*! CTrsControlXX

-# �wRX CONTROL�x-�wX/X-COH�x-�wCOH�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlXxcohCohBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	int station = theApp.GetSelectStation();
	if (station == 0)
	{
		// �P�c64m
		theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COH"));
	}
	else if (station == 2)
	{
		// ���V�Y34m
		theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COHI"));
	}

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_XX_LIMIT;

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wRX CONTROL�x-�wX/X-COH�x-�wINCOH�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlXxcohIncohBtn()
{
	int station = theApp.GetSelectStation();

	// �{�^���𐧌䒆�ɂ���
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	if (station == 0)
	{
		// �P�c64m
		theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOH"));
	}
	else if (station == 2)
	{
		// ���V�Y34m
		theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOHI"));
	}

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_XX_LIMIT;

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}



/*============================================================================*/
/*! CTrsControlXX

-# �wRX CONTROL�x-�wRNGING�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRngingStopBtn()
{
	int station = theApp.GetSelectStation();

	// �{�^���𐧌䒆�ɂ���
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	if (station == 0)
	{
		// �P�c64m
		m_l_rnging_stop_cntdwn = RNGING_TIME_XX64_LIMIT;
	}
	else if (station == 2)
	{
		// ���V�Y34m
		m_l_rnging_stop_cntdwn = RNGING_TIME_XX34_LIMIT;
	}

	// RNG��ʃt���O���N���A����
	m_l_rng_type = RNG_TYPE_NON;

	// �X�V
	m_RngingStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wRX CONTROL�x-�wRNGING�x-�wSTART�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRngingStartBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);
	m_RngingStartBtn.SetColor(COLOR_CTRLIN);

	// �Đ��^�{�^�����C���Z���V�e�B�u�ɂ���
	if (m_RngingStartBtn.IsWindowEnabled() == TRUE)
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.SEL_DSN_RNG_MODE"), _T("TURNAROUND"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MODE"), _T("TURNAROUND"));

	// �]���^�؂�ւ�������s����ݒ�
	m_l_dsnrng_turn_start_flag = 1;

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}



/*============================================================================*/
/*! CTrsControlXX

-# �wRX CONTROL�x-�w�Đ��^�x-�wSTOP�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRgntypeStopBtn()
{
	int station = theApp.GetSelectStation();

	// �Đ��^�{�^�����C���Z���V�e�B�u�ɂ���
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	if (station == 0)
	{
		// �P�c64m
		m_l_rep_rnging_stop_cntdwn = REP_RNGING_TIME_XX64_LIMIT;
	}
	else if (station == 2)
	{
		// ���V�Y34m
		m_l_rep_rnging_stop_cntdwn = REP_RNGING_TIME_XX34_LIMIT;
	}

	// RNG��ʃt���O���N���A����
	m_l_rng_type = RNG_TYPE_NON;

	// �X�V
	m_RRngStopBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �wRX CONTROL�x-�w�Đ��^�x-�wSTART�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnBnClickedTrxxctrlRgntypeStartBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);
	m_RRngStartBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.SEL_DSN_RNG_MODE"), _T("REGEN"));
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.RNG_MODE"), _T("REGEN"));

	// �]���^�{�^�����C���Z���V�e�B�u�ɂ���
	if (m_RngingStopBtn.IsWindowEnabled() == TRUE)
	{
		m_RngingStopBtn.EnableWindow(FALSE);
		m_RngingStartBtn.EnableWindow(FALSE);
	}

	// �Đ��^�؂�ւ�������s����ݒ�
	m_l_dsnrng_regen_start_flag = 1;

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# OnTimer

@param  nIDEvent:�^�C�}�[�̎��ʎq
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_CONDITION)
	{
		// ���M�����ʏ�ԕ\���ݒ�
		show_opsc_condition();
	}
	else if (nIDEvent == TID_COUNTDOWN)
	{
		// �J�E���g�_�E���^�C���A�E�g
		count_down();
	}

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CTrsControlXX

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_opsc_condition()
{
	TRACE(_T("show_opsc_condition\n"));
	KillTimer(TID_CONDITION);

	int station = theApp.GetSelectStation();
	if (station == eStation_Usuda64)
	{
		// �P�c64m
		show_opsc_condition64();
	}
	else if (station == eStation_Usuda642)
	{
		// �P�c54m
		show_opsc_condition54();
	}
	else if (station == eStation_Uchinoura34)
	{
		// ���V�Y34m
		show_opsc_condition34();
	}

	SetTimer(TID_CONDITION, INTERVAL_CONDITION, 0);
}

/*============================================================================*/
/*! CTrsControlXX

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_opsc_condition64()
{
	TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];
	CString str1, str2;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// ���䌠�͏�ɗL��
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/
	// RF POWER ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_ENBL_X), CString(m_sz_rf_on_enbl_x), nLogEx::debug);
	}

	// RF-TRIP NOUSE ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NO_USE_ENBL_X), CString(m_sz_no_use_enbl_x), nLogEx::debug);
	}

	// UP-LINK CONTROL ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_UPLINK_CONT_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont_x, sizeof(m_sz_uplink_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_UPLINK_CONT_X), CString(m_sz_uplink_cont_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_DET_ANS_X), CString(m_sz_rf_on_det_ans_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ��� <<XUP>> S-TX�p�ɕύX
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_S));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_s, sizeof(m_sz_rf_on_det_ans_s), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_DET_ANS_S), CString(m_sz_rf_on_det_ans_s), nLogEx::debug);
	}

	// HPA-2���ݑΉ�
	// HPA SEL ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_HPA_SEL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_hpa_sel, sizeof(m_sz_hpa_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_HPA_SEL), CString(m_sz_hpa_sel), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ��� (HPA-2)
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_X_H2));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_x_h2, sizeof(m_sz_rf_on_det_ans_x_h2), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_DET_ANS_X_H2), CString(m_sz_rf_on_det_ans_x_h2), nLogEx::debug);
	}


	/*** �����[�g�^���[�J�� ��Ԏ擾 ******************************************************/

#if 0
	// S-TX �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_TX_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_tx_rl, sizeof(m_sz_x_tx_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.TX_TOTAL_CONT"), CString(m_sz_x_tx_rl), nLogEx::debug);
	}
#endif

	// S-TXSYN �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DSNTCR_RL), CString(m_sz_dsntcr_rl), nLogEx::debug);
	}

	// X-UL_CNT �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UL_CNT_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_ul_cnt_rl, sizeof(m_sz_x_ul_cnt_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UL_CNT_RL), CString(m_sz_x_ul_cnt_rl), nLogEx::debug);
	}

	// X-UC �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UC_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_uc_rl, sizeof(m_sz_x_uc_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UC_RL), CString(m_sz_x_uc_rl), nLogEx::debug);
	}

	// X-RF_IF �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_RF_IF_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_rf_if_rl, sizeof(m_sz_x_rf_if_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_RF_IF_RL), CString(m_sz_x_rf_if_rl), nLogEx::debug);
	}

	/*** �A���[���`�F�b�N  <<XUP>> X-TX�p�ɕύX *******************************************/
#if 1
	strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), "OFF");
#else
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_ALARM_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.TOTAL_FAULT"), CString(m_sz_alarm_x), nLogEx::debug);
	}
#endif

	// �A���[����
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// �g��Ԃ�����
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grSweepContSub.SetBackColor(COLOR_ALARM);
		m_grDoppCont.SetBackColor(COLOR_ALARM);
		m_grModulation.SetBackColor(COLOR_ALARM);
		m_grHoldTone.SetBackColor(COLOR_ALARM);
	}

	// �A���[������
	else
	{
		// �g�̐F�����ɖ߂�
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContSub.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grDoppCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.RF_ON_ANS"), CString(m_sz_rf_power_x), nLogEx::debug);
	}

	// RF-POWER ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rf_power_on_cntdwn >= 0 &&
		strcmp(m_sz_rf_power_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_on_cntdwn = -1;
	}
	// RF-POWER ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rf_power_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_on_cntdwn = -1;
	}

	// RF-POWER OFF �J�E���g�_�E�����ɊĎ��f�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_rf_power_off_cntdwn >= 0 &&
		strcmp(m_sz_rf_power_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_off_cntdwn = -1;
	}
	// RF-POWER OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rf_power_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_off_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rf_power_off_cntdwn == -1 && m_l_rf_power_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_rf_power_x, "ON") == 0)
		{
			show_rf_power_on64();
		}
		// OFF �̏ꍇ
		else
		{
			/*** ���ꏈ������ *************************************************************/
			show_rf_power_off64();
		}
	}

	/*** NO-USE ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
	}

	// NO-USE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_no_use_on_cntdwn >= 0 &&
		strcmp(m_sz_no_use_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}
	// NO-USE ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_no_use_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}

	// NO-USE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_no_use_off_cntdwn >= 0 &&
		strcmp(m_sz_no_use_x, "ON") != 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}
	// NO-USE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_no_use_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_no_use_off_cntdwn == -1 && m_l_no_use_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_no_use_x, "ON") == 0)
		{
			show_no_use_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_no_use_off64();
		}
	}

	/*** MAIN SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}

	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

		// SWEEP���s�t���OOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� +SAW/+TRIANGLE �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+SAW") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0))
	{

		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT INCDEC �J�E���g�_�E�����Ƀf�[�^�� +-SAW/+-TRIANGLE �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+-SAW") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� -SAW/-TRIANGLE �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-SAW") != 0)) {
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DECINC �J�E���g�_�E�����Ƀf�[�^�� -+SAW/-+TRIANGLE �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-+SAW") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_main_sweep_cont_stop_cntdwn == -1 && m_l_main_sweep_cont_inc_cntdwn == -1 &&
		m_l_main_sweep_cont_incdec_cntdwn == -1 && m_l_main_sweep_cont_dec_cntdwn == -1 &&
		m_l_main_sweep_cont_decinc_cntdwn == -1)
	{
		// INC �̏ꍇ
		if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
		{
			show_main_sweep_cont_inc64();
		}

		// INCDEC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
		{
			show_main_sweep_cont_incdec64();
		}

		// DEC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
		{
			show_main_sweep_cont_dec64();
		}

		// DECINC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
		{
			show_main_sweep_cont_decinc64();
		}

		// STOP �̏ꍇ
		else {
			// CONT��ON��STS��RUNNING�ȊO�Ȃ�΁ACONT��OFF����
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

				// SWEEP���s�t���OOFF
				m_i_sweep_flag = 0;
			}

			// ���S�ɒ�~���Ă���ASTOP�\���ɂ���
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_cmd, "STOP") == 0 &&
				(strcmp(m_sz_sweep_sts, "INITIALIZE") == 0 ||
				strcmp(m_sz_sweep_sts, "STOP") == 0))
			{

				show_main_sweep_cont_stop64();
			}
		}
	}

	/*** SUB SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}

	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_sub_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_stop_cntdwn = -1;

		// SWEEP���s�t���OOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� +SAW/+TRIANGLE �ɂȂ����ꍇ
	if (m_l_sub_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_inc_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_inc_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+SAW"));
	}
	else if ((m_l_sub_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+TRIANGLE"));
	}

	// SWEEP-CONT INCDEC �J�E���g�_�E�����Ƀf�[�^�� +-SAW/+-TRIANGLE �ɂȂ����ꍇ
	if (m_l_sub_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_incdec_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_incdec_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+-SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-SAW"));
	}
	else if ((m_l_sub_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-TRIANGLE"));
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� -SAW/-TRIANGLE �ɂȂ����ꍇ
	if (m_l_sub_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_dec_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_dec_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-SAW"));
	}
	else if ((m_l_sub_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-TRIANGLE"));
	}

	// SWEEP-CONT DECINC �J�E���g�_�E�����Ƀf�[�^�� -+SAW/-+TRIANGLE �ɂȂ����ꍇ 
	if (m_l_sub_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_decinc_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_decinc_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-+SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+SAW"));
	}
	else if ((m_l_sub_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+TRIANGLE"));
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_sub_sweep_cont_stop_cntdwn == -1 && m_l_sub_sweep_cont_inc_cntdwn == -1 &&
		m_l_sub_sweep_cont_incdec_cntdwn == -1 && m_l_sub_sweep_cont_dec_cntdwn == -1 &&
		m_l_sub_sweep_cont_decinc_cntdwn == -1)
	{
		// INC �̏ꍇ
		if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
		{
			show_sub_sweep_cont_inc64();
		}

		// INCDEC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
		{
			show_sub_sweep_cont_incdec64();
		}

		// DEC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
		{
			show_sub_sweep_cont_dec64();
		}

		// DECINC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
		{
			show_sub_sweep_cont_decinc64();
		}

		// STOP �̏ꍇ
		else
		{
			// CONT��ON��STS��RUNNING�ȊO�Ȃ�΁ACONT��OFF����
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

				// SWEEP���s�t���OOFF
				m_i_sweep_flag = 0;
			}

			// 2013-03-11 ���S�ɒ�~���Ă���ASTOP�\���ɂ���
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_cmd, "STOP") == 0 &&
				(strcmp(m_sz_sweep_sts, "INITIALIZE") == 0 ||
				strcmp(m_sz_sweep_sts, "STOP") == 0))
			{
				show_sub_sweep_cont_stop64();
			}
		}
	}


	/*** SWEEP-NM ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_d_sweep_num = obs_adr->d_data;
	}

	// ���l�\�������X�V
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_sweep_num % 100);
	m_SweepNumBtn.SetWindowTextW(CString(sz_buff));

	// Tri-SWEEP-NUM �J�E���g�_�E�����Ƀf�[�^���ݒ�l�ɂȂ����ꍇ
	if (m_l_sweep_num_cntdwn >= 0 &&
		m_l_sweep_num_set == (long)m_d_sweep_num)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_num_cntdwn = -1;
	}
	// Tri-SWEEP-NUM �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_num_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_num_cntdwn = -1;

		// �^�p���O�ɕ\��
		sprintf_s(sz_buff, sizeof(sz_buff), "SWEEP-NUM %2d", m_l_sweep_num_set);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(sz_buff), _T(""), nLogEx::debug);
	}


	/*** DOPP-CONT <<XUP>> �ǉ� ***********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DOPP_PRE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dopp_pre, sizeof(m_sz_dopp_pre), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DOPP_PRE), CString(m_sz_dopp_pre), nLogEx::debug);
	}
	// DOPP-CONT OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_dopp_cont_off_cntdwn >= 0 &&
		strcmp(m_sz_dopp_pre, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_off_cntdwn = -1;
	}
	// DOPP-CONT OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ 
	else if (m_l_dopp_cont_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_off_cntdwn = -1;
	}

	// DOPP-CONT ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_dopp_cont_on_cntdwn >= 0 &&
		strcmp(m_sz_dopp_pre, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_on_cntdwn = -1;
	}
	// DOPP-CONT ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_dopp_cont_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_dopp_cont_off_cntdwn == -1 && m_l_dopp_cont_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_dopp_pre, "ON") == 0)
		{
			show_dopp_cont_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_dopp_cont_off64();
		}
	}


	/*** TCRMOD MODULATION ****************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_X), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}

	// TCRMOD MODULATION OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_modulation_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_off_cntdwn = -1;
	}
	// TCRMOD MODULATION OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_modulation_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_off_cntdwn = -1;
	}

	// TCRMOD MODULATION ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_modulation_on_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_modulation_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_modulation_off_cntdwn == -1 && m_l_modulation_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_modulation_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_modulation_off64();
		}
	}

	/*** X-CMDOUT HOLD_TONE 2008-09-01 X-HPA���ݑΉ��ǉ� **********************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
	}

	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_hold_tone_off_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn = -1;
	}
	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn = -1;
	}

	// X-CMDOUT HOLD_TONE ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_hold_tone_on_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_hold_tone_off_cntdwn == -1 && m_l_hold_tone_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
		{
			show_hold_tone_on64();
		}
		// OFF �̏ꍇ
		else
		{
			show_hold_tone_off64();
		}
	}

	/*** CMD MOD INDEX ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		CString tmp;
		tmp.Format(_T("%f"), obs_adr->d_data);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_IDX), tmp, nLogEx::debug);

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("INDEX-CMD"), m_strIndexCmd, nLogEx::debug);
	}

	/*** RNG MOD INDEX ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		CString tmp;
		tmp.Format(_T("%f"), obs_adr->d_data);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RNG_MOD_IDX), tmp, nLogEx::debug);

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("INDEX-RNG"), m_strIndexRng, nLogEx::debug);
	}

	/*** DATA SOURCE **********************************************************************/
	/* ���݂̏�Ԃ��Ď��c�a����擾                                                       */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_data_src, sizeof(m_sz_data_src), "%s", obs_adr->sz_sts_name);

		// ��ԕ\�������X�V
		m_strIndexCpr = CString(m_sz_data_src);
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	// �I�΂�Ă��郌���W���u���]���^���Đ��^����RNG�񐔂̕\������؂�ւ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), "%s", obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// �Đ��^���I�΂�Ă���ꍇ
	if (strcmp(m_sz_norm_rep, "REGEN") == 0)
	{
		// �Đ��^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// �]���^���I�΂�Ă���ꍇ
	else if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
	{
		// �]���^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// ����ł��Ȃ��ꍇ�͂O�N���A
	else
	{
		m_d_xx_rng_cnt = 0;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_xx_rng_num = obs_adr->d_data;
	}

	// ���l�\�������X�V
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_cnt % 100);
	m_strXXRng = CString(sz_buff);

	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_num % 100);
	m_XXRngNumBtn.SetWindowTextW(CString(sz_buff));


	/*** "CONTINUATION" �\���L�� ����� �Z���V�e�B�u���� **********************************/
	// XX-RNG �̐��l�\������� "CONTINUATION" �������i����΁j
	m_strXXRngCont = _T("");


	/*** COH/INCOH ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("V-EQU.COHI"), CString(m_sz_coh_incoh), nLogEx::debug);
	}

	// COH/INCOH COH �J�E���g�_�E�����Ƀf�[�^�� COH �ɂȂ����ꍇ
	if (m_l_coh_incoh_coh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "COH") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_coh_cntdwn = -1;
	}
	// COH/INCOH COH �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_coh_incoh_coh_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_coh_cntdwn = -1;
	}

	// COH/INCOH INCOH �J�E���g�_�E�����Ƀf�[�^�� INCOH �ɂȂ����ꍇ
	if (m_l_coh_incoh_incoh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "INCOH") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_incoh_cntdwn = -1;
	}
	// SX-RNG ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_coh_incoh_incoh_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_incoh_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_coh_incoh_coh_cntdwn == -1 && m_l_coh_incoh_incoh_cntdwn == -1)
	{
		// INCOH �̏ꍇ
		if (strcmp(m_sz_coh_incoh, "INCOH") == 0)
		{
			show_coh_incoh_incoh64();
		}
		// COH �̏ꍇ
		else
		{
			show_coh_incoh_coh64();
		}
	}


	/*** RNGING ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("CTRL.RNG_FLG"), CString(m_sz_rnging), nLogEx::debug);
	}

	// RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�I��") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_stop_cntdwn = -1;
	}
	// RNGING STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rnging_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_stop_cntdwn = -1;
	}

	// RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if (m_l_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�J�n") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_start_cntdwn = -1;
	}
	// RNGING START �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rnging_start_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_start_cntdwn = -1;
	}

	// (�Đ��^) RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rep_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�I��") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_stop_cntdwn = -1;
	}
	// (�Đ��^) RNGING STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rep_rnging_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_stop_cntdwn = -1;
	}

	// (�]���^) �����W�`���؂�ւ�����̎��s���ʊm�F
	if (m_l_dsnrng_turn_start_flag == 1)
	{
		// �]���^�؂�ւ�������s��
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
		{
			// ����ʒm���M 
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// �J�E���g�_�E���J�n
			m_l_rnging_start_cntdwn = RNGING_TIME_XX64_LIMIT;

			//RNG��ʃt���O���]���^�ɐݒ�
			m_l_rng_type = RNG_TYPE_NOR;

			// �]���^�؂�ւ�������s���t���O��߂�
			m_l_dsnrng_turn_start_flag = 0;
		}
	}

	// (�Đ��^) �����W�`���؂�ւ�����̎��s���ʊm�F
	if (m_l_dsnrng_regen_start_flag == 1)
	{
		// �Đ��^�؂�ւ�������s��
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "REGEN") == 0)
		{
			// ����ʒm���M
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// �J�E���g�_�E���J�n
			m_l_rep_rnging_start_cntdwn = REP_RNGING_TIME_XX64_LIMIT;

			// RNG��ʃt���O���Đ��^�ɐݒ�
			m_l_rng_type = RNG_TYPE_REP;

			// �Đ��^�؂�ւ�������s���t���O��߂�
			m_l_dsnrng_regen_start_flag = 0;
		}
	}

	// (�Đ��^) RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if (m_l_rep_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�J�n") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_start_cntdwn = -1;
	}
	// (�Đ��^) RNGING START �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rep_rnging_start_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_start_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rnging_stop_cntdwn == -1 && m_l_rnging_start_cntdwn == -1)
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rnging_start64();
			show_rrng_start64();
		}
		// STOP �̏ꍇ
		else
		{
			show_rnging_stop64();
			show_rrng_stop64();
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rep_rnging_stop_cntdwn == -1 && m_l_rep_rnging_start_cntdwn == -1)
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rrng_start64();
		}
		// STOP �̏ꍇ
		else
		{
			show_rrng_stop64();
		}
	}

	/*** �ϒ����u�̑I����Ԋm�F 2008-09-01 X-HPA���ݑΉ��ǉ� ******************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_MOD_IN_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_in_sel, sizeof(m_sz_mod_in_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-IFSW.IF_CH_INPUT_SEL_ANS"), CString(m_sz_mod_in_sel), nLogEx::debug);
	}

	// TCRMOD���I������Ă���ꍇ
	if (strcmp(m_sz_mod_in_sel, "TCRMOD") == 0)
	{
		// TCRMOD���Z���V�e�B�u�APM MOD���C���Z���V�e�B�u�ɂ���
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
	}
	// PM MOD���I������Ă���ꍇ
	else if (strcmp(m_sz_mod_in_sel, "DSN-MODEM") == 0)
	{
		// TCRMOD���C���Z���V�e�B�u�APM MOD���Z���V�e�B�u�ɂ���
		m_HoldToneOffBtn.EnableWindow(TRUE);
		m_HoldToneOnBtn.EnableWindow(TRUE);
		m_ModulationOffBtn.EnableWindow(TRUE);
		m_ModulationOnBtn.EnableWindow(TRUE);
	}
	// ���Â�̏�Ԃł��Ȃ��ꍇ
	else
	{
		// �ǂ�����C���Z���V�e�B�u�ɂ���
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CTrsControlXX

-# ���M�����ʏ�ԕ\���ݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_opsc_condition34()
{
	TRACE(_T("show_opsc_condition\n"));

	char sz_buff[64];
	char szRfPowerSts[32];
	CString str1, str2;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// ���䌠�͏�ɗL��
	m_i_priv_flag = 1;

	/*-- TX CONTROL ----------------------------------------------------------------------*/
	// RF POWER ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_ON_ENBL_X), CString(m_sz_rf_on_enbl_x), nLogEx::debug);
	}

	// RF-TRIP NOUSE ON ENABLE ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NO_USE_ENBL_X), CString(m_sz_no_use_enbl_x), nLogEx::debug);
	}

	// UP-LINK CONTROL ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_UPLINK_CONT_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_uplink_cont_x, sizeof(m_sz_uplink_cont_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_UPLINK_CONT_X), CString(m_sz_uplink_cont_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_RF_ON_DET_ANS_X), CString(m_sz_rf_on_det_ans_x), nLogEx::debug);
	}

	// RF POWER ON DETECT ANSWER ��� <<XUP>> S-TX�p�ɕύX
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_RF_ON_DET_ANS_S));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_rf_on_det_ans_s, sizeof(m_sz_rf_on_det_ans_s), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_RF_ON_DET_ANS_S), CString(m_sz_rf_on_det_ans_s), nLogEx::debug);
	}


	/*** �����[�g�^���[�J�� ��Ԏ擾 ******************************************************/

	// S-TXSYN �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_X_TXSYN_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_txsyn_rl, sizeof(m_sz_x_txsyn_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_X_TXSYN_RL), CString(m_sz_x_txsyn_rl), nLogEx::debug);
	}

	// DSN-MODEM �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DSNTCR_RL), CString(m_sz_dsntcr_rl), nLogEx::debug);
	}

	// X-UL_CNT �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UL_CNT_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_ul_cnt_rl, sizeof(m_sz_x_ul_cnt_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UL_CNT_RL), CString(m_sz_x_ul_cnt_rl), nLogEx::debug);
	}

	// X-UC �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_UC_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_uc_rl, sizeof(m_sz_x_uc_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_UC_RL), CString(m_sz_x_uc_rl), nLogEx::debug);
	}

	// X-RF_IF �����[�g�^���[�J�� ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_X_RF_IF_RL));
	if (obs_adr != NULL)
	{
		m_l_inflg = obs_adr->l_inflg;
		strcpy_s(m_sz_x_rf_if_rl, sizeof(m_sz_x_rf_if_rl), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_X_RF_IF_RL), CString(m_sz_x_rf_if_rl), nLogEx::debug);
	}

	/*** �A���[���`�F�b�N  <<XUP>> X-TX�p�ɕύX *******************************************/
#if 1
	strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), "OFF");
#else
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_ALARM_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_alarm_x, sizeof(m_sz_alarm_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-TX.TOTAL_FAULT"), CString(m_sz_alarm_x), nLogEx::debug);
	}
#endif

	// �A���[����
	if (strcmp(m_sz_alarm_x, "ON") == 0)
	{
		// �g��Ԃ�����
		m_grRfPower.SetBackColor(COLOR_ALARM);
		m_grNoUse.SetBackColor(COLOR_ALARM);
		m_grSweepCont.SetBackColor(COLOR_ALARM);
		m_grSweepContSub.SetBackColor(COLOR_ALARM);
		m_grDoppCont.SetBackColor(COLOR_ALARM);
		m_grModulation.SetBackColor(COLOR_ALARM);
		m_grHoldTone.SetBackColor(COLOR_ALARM);
	}

	// �A���[������
	else
	{
		// �g�̐F�����ɖ߂�
		m_grRfPower.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grNoUse.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grSweepContSub.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grDoppCont.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grModulation.SetBackColor(GetSysColor(COLOR_3DFACE));
		m_grHoldTone.SetBackColor(GetSysColor(COLOR_3DFACE));
	}


	/*** RF-POWER *************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_power_x, sizeof(m_sz_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RF_POWER_X), CString(m_sz_rf_power_x), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_HPA_RF_POWER_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_power_x, sizeof(m_sz_hpa_rf_power_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_HPA_RF_POWER_X), CString(m_sz_hpa_rf_power_x), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_XPA_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xpa_sel, sizeof(m_sz_xpa_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX34_XPA_SEL), CString(m_sz_xpa_sel), nLogEx::debug);
	}

	// HPA/SSPA�ŏ����𕪂���
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPA���I������Ă���ꍇ
		strcpy_s(szRfPowerSts, sizeof(szRfPowerSts), m_sz_hpa_rf_power_x);
	}
	else
	{
		// SSPA���I������Ă���ꍇ
		strcpy_s(szRfPowerSts, sizeof(szRfPowerSts), m_sz_rf_power_x);
	}

	// RF-POWER ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_rf_power_on_cntdwn >= 0 &&
		strcmp(szRfPowerSts, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_on_cntdwn = -1;
	}
	// RF-POWER ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rf_power_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_on_cntdwn = -1;
	}

	// RF-POWER OFF �J�E���g�_�E�����ɊĎ��f�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_rf_power_off_cntdwn >= 0 &&
		strcmp(szRfPowerSts, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_off_cntdwn = -1;
	}
	// RF-POWER OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rf_power_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rf_power_off_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rf_power_off_cntdwn == -1 && m_l_rf_power_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(szRfPowerSts, "ON") == 0)
		{
			show_rf_power_on34();
		}
		// OFF �̏ꍇ
		else
		{
			/*** ���ꏈ������ *************************************************************/
			show_rf_power_off34();
		}
	}


	/*** NO-USE ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_x, sizeof(m_sz_no_use_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NO_USE_X), CString(m_sz_no_use_x), nLogEx::debug);
	}
	// NO-USE ON �J�E���g�_�E�����ɊĎ��f�[�^�� ON �ɂȂ����ꍇ
	if (m_l_no_use_on_cntdwn >= 0 &&
		strcmp(m_sz_no_use_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}
	// NO-USE ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_no_use_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}

	// NO-USE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_no_use_off_cntdwn >= 0 &&
		strcmp(m_sz_no_use_x, "ON") != 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}
	// NO-USE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_no_use_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_no_use_off_cntdwn == -1 && m_l_no_use_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_no_use_x, "ON") == 0)
		{
			show_no_use_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_no_use_off34();
		}
	}


	/*** MAIN SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_TC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_TC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}
	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;

		// SWEEP���s�t���OOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_stop_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� +SAW/+TRIANGLE �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_inc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+SAW") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0))
	{

		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT INCDEC �J�E���g�_�E�����Ƀf�[�^�� +-SAW/+-TRIANGLE �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_incdec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+-SAW") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("+-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� -SAW/-TRIANGLE �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_dec_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-SAW") != 0)) {
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// SWEEP-CONT DECINC �J�E���g�_�E�����Ƀf�[�^�� -+SAW/-+TRIANGLE �ɂȂ����ꍇ
	if (m_l_main_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_main_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_main_sweep_cont_decinc_cntdwn = -1;
		m_l_flg_ctrl_sweep = CTRL_SWEEP_NA;      /* 1shot_sweep */
	}
	else if ((m_l_main_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-+SAW") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+SAW"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}
	else if ((m_l_main_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_main_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0))
	{
		/* 1shot-sweep */
		// �J�E���g�_�E�����ɂ͈��̂ݐ���ƃI�y���O���o���悤�t���O���䉻
		if (m_l_flg_ctrl_sweep == CTRL_SWEEP_BEFORE)
		{
			theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("-+TRIANGLE"));
			m_l_flg_ctrl_sweep = CTRL_SWEEP_AFTER;
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_main_sweep_cont_stop_cntdwn == -1 && m_l_main_sweep_cont_inc_cntdwn == -1 &&
		m_l_main_sweep_cont_incdec_cntdwn == -1 && m_l_main_sweep_cont_dec_cntdwn == -1 &&
		m_l_main_sweep_cont_decinc_cntdwn == -1)
	{
		// INC �̏ꍇ
		if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
		{
			show_main_sweep_cont_inc34();
		}

		// INCDEC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
		{
			show_main_sweep_cont_incdec34();
		}

		// DEC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
		{
			show_main_sweep_cont_dec34();
		}

		// DECINC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
		{
			show_main_sweep_cont_decinc34();
		}

		// STOP �̏ꍇ
		else {
			// CONT��ON��STS��RUNNING�ȊO�Ȃ�΁ACONT��OFF����
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP_MODE"), _T("STOP"));

				// SWEEP���s�t���OOFF
				m_i_sweep_flag = 0;
			}

			// ���S�ɒ�~���Ă���ASTOP�\���ɂ���
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_sts, "STOP") == 0)
			{

				show_main_sweep_cont_stop34();
			}
		}
	}


	/*** SUB SWEEP-CONT ***********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cont, sizeof(m_sz_sweep_cont), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CONT), CString(m_sz_sweep_cont), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_CMD));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_cmd, sizeof(m_sz_sweep_cmd), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_CMD), CString(m_sz_sweep_cmd), nLogEx::debug);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SC_SWEEP_STS));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sweep_sts, sizeof(m_sz_sweep_sts), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_SC_SWEEP_STS), CString(m_sz_sweep_sts), nLogEx::debug);
	}

	// SWEEP-CONT STOP �J�E���g�_�E�����ɊĎ��f�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_sub_sweep_cont_stop_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "OFF") == 0 ||
		strcmp(m_sz_sweep_cmd, "STOP") == 0 ||
		strcmp(m_sz_sweep_sts, "STOP") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_stop_cntdwn = -1;

		// SWEEP���s�t���OOFF
		m_i_sweep_flag = 0;
	}
	// SWEEP-CONT STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_stop_cntdwn = -1;
	}

	// SWEEP-CONT INC �J�E���g�_�E�����Ƀf�[�^�� +SAW/+TRIANGLE �ɂȂ����ꍇ
	if (m_l_sub_sweep_cont_inc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_inc_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_inc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_inc_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+SAW"));
	}
	else if ((m_l_sub_sweep_cont_inc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+TRIANGLE"));
	}

	// SWEEP-CONT INCDEC �J�E���g�_�E�����Ƀf�[�^�� +-SAW/+-TRIANGLE �ɂȂ����ꍇ
	if (m_l_sub_sweep_cont_incdec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_incdec_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT +-SAW/TRIANGLE �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_incdec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_incdec_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "+-SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-SAW"));
	}
	else if ((m_l_sub_sweep_cont_incdec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "+-TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("+-TRIANGLE"));
	}

	// SWEEP-CONT DEC �J�E���g�_�E�����Ƀf�[�^�� -SAW/-TRIANGLE �ɂȂ����ꍇ
	if (m_l_sub_sweep_cont_dec_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_dec_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT DEC �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_dec_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_dec_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-SAW"));
	}
	else if ((m_l_sub_sweep_cont_dec_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-TRIANGLE"));
	}

	// SWEEP-CONT DECINC �J�E���g�_�E�����Ƀf�[�^�� -+SAW/-+TRIANGLE �ɂȂ����ꍇ 
	if (m_l_sub_sweep_cont_decinc_cntdwn >= 0 &&
		(strcmp(m_sz_sweep_cont, "ON") == 0 &&
		strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
		(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_decinc_cntdwn = -1;

		// SWEEP���s�t���OON
		m_i_sweep_flag = 1;
	}
	// SWEEP-CONT -+SAW �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sub_sweep_cont_decinc_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sub_sweep_cont_decinc_cntdwn = -1;
	}
	else if ((m_l_sub_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_SAW &&
		strcmp(m_sz_sweep_cmd, "-+SAW") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+SAW"));
	}
	else if ((m_l_sub_sweep_cont_decinc_cntdwn >= 0) &&
		(m_i_sub_sweep_cont_mode == SWEEP_MODE_TRI &&
		strcmp(m_sz_sweep_cmd, "-+TRIANGLE") != 0))
	{
		theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("-+TRIANGLE"));
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_sub_sweep_cont_stop_cntdwn == -1 && m_l_sub_sweep_cont_inc_cntdwn == -1 &&
		m_l_sub_sweep_cont_incdec_cntdwn == -1 && m_l_sub_sweep_cont_dec_cntdwn == -1 &&
		m_l_sub_sweep_cont_decinc_cntdwn == -1)
	{
		// INC �̏ꍇ
		if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+TRIANGLE") == 0))
		{
			show_sub_sweep_cont_inc34();
		}

		// INCDEC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "+-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "+-TRIANGLE") == 0))
		{
			show_sub_sweep_cont_incdec34();
		}

		// DEC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-TRIANGLE") == 0))
		{
			show_sub_sweep_cont_dec34();
		}

		// DECINC �̏ꍇ
		else if ((strcmp(m_sz_sweep_cont, "ON") == 0 &&
			strcmp(m_sz_sweep_sts, "RUNNING") == 0) &&
			(strcmp(m_sz_sweep_cmd, "-+SAW") == 0 ||
			strcmp(m_sz_sweep_cmd, "-+TRIANGLE") == 0))
		{
			show_sub_sweep_cont_decinc34();
		}

		// STOP �̏ꍇ
		else
		{
			// CONT��ON��STS��RUNNING�ȊO�Ȃ�΁ACONT��OFF����
			if (strcmp(m_sz_sweep_cont, "ON") == 0 &&
				strcmp(m_sz_sweep_sts, "RUNNING") != 0 &&
				m_i_sweep_flag == 1)
			{
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("OFF"));
				theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP_MODE"), _T("STOP"));

				// SWEEP���s�t���OOFF
				m_i_sweep_flag = 0;
			}

			// 2013-03-11 ���S�ɒ�~���Ă���ASTOP�\���ɂ���
			if (strcmp(m_sz_sweep_cont, "OFF") == 0 &&
				strcmp(m_sz_sweep_sts, "STOP") == 0)
			{
				show_sub_sweep_cont_stop34();
			}
		}
	}


	/*** SWEEP-NM ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SWEEP_NUM));
	if (obs_adr != NULL)
	{
		m_d_sweep_num = obs_adr->d_data;
	}

	// ���l�\�������X�V
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_sweep_num % 100);
	m_SweepNumBtn.SetWindowTextW(CString(sz_buff));

	// Tri-SWEEP-NUM �J�E���g�_�E�����Ƀf�[�^���ݒ�l�ɂȂ����ꍇ
	if (m_l_sweep_num_cntdwn >= 0 &&
		m_l_sweep_num_set == (long)m_d_sweep_num)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_num_cntdwn = -1;
	}
	// Tri-SWEEP-NUM �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_sweep_num_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_sweep_num_cntdwn = -1;

		// �^�p���O�ɕ\��
		sprintf_s(sz_buff, sizeof(sz_buff), "SWEEP-NUM %2d", m_l_sweep_num_set);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(sz_buff), _T(""), nLogEx::debug);
	}


	/*** DOPP-CONT <<XUP>> �ǉ� ***********************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DOPP_PRE));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dopp_pre, sizeof(m_sz_dopp_pre), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_DOPP_PRE), CString(m_sz_dopp_pre), nLogEx::debug);
	}
	// DOPP-CONT OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_dopp_cont_off_cntdwn >= 0 &&
		strcmp(m_sz_dopp_pre, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_off_cntdwn = -1;
	}
	// DOPP-CONT OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ 
	else if (m_l_dopp_cont_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_off_cntdwn = -1;
	}

	// DOPP-CONT ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_dopp_cont_on_cntdwn >= 0 &&
		strcmp(m_sz_dopp_pre, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_on_cntdwn = -1;
	}
	// DOPP-CONT ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_dopp_cont_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_dopp_cont_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_dopp_cont_off_cntdwn == -1 && m_l_dopp_cont_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_dopp_pre, "ON") == 0)
		{
			show_dopp_cont_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_dopp_cont_off34();
		}
	}


	/*** TCRMOD MODULATION ****************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_cmd_mod_x, sizeof(m_sz_cmd_mod_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_CMD_MOD_X), CString(m_sz_cmd_mod_x), nLogEx::debug);
	}
	// TCRMOD MODULATION OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_modulation_off_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_off_cntdwn = -1;
	}
	// TCRMOD MODULATION OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_modulation_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_off_cntdwn = -1;
	}

	// TCRMOD MODULATION ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_modulation_on_cntdwn >= 0 &&
		strcmp(m_sz_cmd_mod_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_modulation_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_modulation_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_modulation_off_cntdwn == -1 && m_l_modulation_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_cmd_mod_x, "ON") == 0)
		{
			show_modulation_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_modulation_off34();
		}
	}

	/*** X-CMDOUT HOLD_TONE 2008-09-01 X-HPA���ݑΉ��ǉ� **********************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_HOLD_TONE_STS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hold_tone_sts_x, sizeof(m_sz_hold_tone_sts_x), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_HOLD_TONE_STS_X), CString(m_sz_hold_tone_sts_x), nLogEx::debug);
	}
	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E�����Ƀf�[�^�� OFF �ɂȂ����ꍇ
	if (m_l_hold_tone_off_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "OFF") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn = -1;
	}
	// X-CMDOUT HOLD_TONE OFF �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_off_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_off_cntdwn = -1;
	}

	// X-CMDOUT HOLD_TONE ON �J�E���g�_�E�����Ƀf�[�^�� ON �ɂȂ����ꍇ
	if (m_l_hold_tone_on_cntdwn >= 0 &&
		strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn = -1;
	}
	// TCRMOD MODULATION ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_hold_tone_on_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_hold_tone_on_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_hold_tone_off_cntdwn == -1 && m_l_hold_tone_on_cntdwn == -1)
	{
		// ON �̏ꍇ
		if (strcmp(m_sz_hold_tone_sts_x, "ON") == 0)
		{
			show_hold_tone_on34();
		}
		// OFF �̏ꍇ
		else
		{
			show_hold_tone_off34();
		}
	}


	/*** CMD MOD INDEX ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CMD_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_cmd_mod_idx = obs_adr->d_data;

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_cmd_mod_idx);
		m_strIndexCmd = CString(sz_buff);
	}

	/*** RNG MOD INDEX ********************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNG_MOD_IDX));
	if (obs_adr != NULL)
	{
		m_d_rng_mod_idx = obs_adr->d_data;

		// ���l�\�������X�V
		sprintf_s(sz_buff, sizeof(sz_buff), "%4.2f", m_d_rng_mod_idx);
		m_strIndexRng = CString(sz_buff);
	}

	/*** DATA SOURCE **********************************************************************/
	/* ���݂̏�Ԃ��Ď��c�a����擾                                                       */
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DATA_SOURCE));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_data_src, sizeof(m_sz_data_src), "%-10s", obs_adr->sz_sts_name);

		// ��ԕ\�������X�V
		m_strIndexCpr = CString(m_sz_data_src);
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	/*** SS-RNG / SX-RNG ******************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾 
	// �I�΂�Ă��郌���W���u���]���^���Đ��^����RNG�񐔂̕\������؂�ւ���
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		sprintf_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), "%-10s", obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_NORM_REP), CString(m_sz_norm_rep), nLogEx::debug);
	}

	// �Đ��^���I�΂�Ă���ꍇ
	if (strcmp(m_sz_norm_rep, "REGEN") == 0)
	{
		// �Đ��^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// �]���^���I�΂�Ă���ꍇ
	else if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
	{
		// �]���^�����񐔂𒊏o
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_CNT));
		if (obs_adr != NULL)
		{
			m_d_xx_rng_cnt = obs_adr->d_data;
		}
	}
	// ����ł��Ȃ��ꍇ�͂O�N���A
	else
	{
		m_d_xx_rng_cnt = 0;
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RNG_NUM));
	if (obs_adr != NULL)
	{
		m_d_xx_rng_num = obs_adr->d_data;
	}

	// ���l�\�������X�V
	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_cnt % 100);
	m_strXXRng = CString(sz_buff);

	sprintf_s(sz_buff, sizeof(sz_buff), "%2d", (int)m_d_xx_rng_num % 100);
	m_XXRngNumBtn.SetWindowTextW(CString(sz_buff));


	/*** "CONTINUATION" �\���L�� ����� �Z���V�e�B�u���� **********************************/
	// XX-RNG �̐��l�\������� "CONTINUATION" �������i����΁j
	m_strXXRngCont = _T("");


	/*** COH/INCOH ************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_COH_INCOH));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_coh_incoh, sizeof(m_sz_coh_incoh), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_COH_INCOH), CString(m_sz_coh_incoh), nLogEx::debug);
	}

	// COH/INCOH COH �J�E���g�_�E�����Ƀf�[�^�� COH �ɂȂ����ꍇ
	if (m_l_coh_incoh_coh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "COHI") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_coh_cntdwn = -1;
	}
	// COH/INCOH COH �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_coh_incoh_coh_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_coh_cntdwn = -1;
	}

	// COH/INCOH INCOH �J�E���g�_�E�����Ƀf�[�^�� INCOH �ɂȂ����ꍇ
	if (m_l_coh_incoh_incoh_cntdwn >= 0 &&
		strcmp(m_sz_coh_incoh, "INCOHI") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_incoh_cntdwn = -1;
	}
	// SX-RNG ON �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_coh_incoh_incoh_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_coh_incoh_incoh_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_coh_incoh_coh_cntdwn == -1 && m_l_coh_incoh_incoh_cntdwn == -1)
	{
		// INCOH �̏ꍇ
		if (strcmp(m_sz_coh_incoh, "INCOHI") == 0)
		{
			show_coh_incoh_incoh34();
		}
		// COH �̏ꍇ
		else
		{
			show_coh_incoh_coh34();
		}
	}


	/*** RNGING ***************************************************************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), CString(MON_XX_RNGING), CString(m_sz_rnging), nLogEx::debug);
	}

	// RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�I��") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_stop_cntdwn = -1;
	}
	// RNGING STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rnging_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_stop_cntdwn = -1;
	}

	// RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if (m_l_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�J�n") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_start_cntdwn = -1;
	}
	// RNGING START �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rnging_start_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rnging_start_cntdwn = -1;
	}

	// (�Đ��^) RNGING STOP �J�E���g�_�E�����Ƀf�[�^�� STOP �ɂȂ����ꍇ
	if (m_l_rep_rnging_stop_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�I��") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_stop_cntdwn = -1;
	}
	// (�Đ��^) RNGING STOP �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rep_rnging_stop_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_stop_cntdwn = -1;
	}

	// (�]���^) �����W�`���؂�ւ�����̎��s���ʊm�F
	if (m_l_dsnrng_turn_start_flag == 1)
	{
		// �]���^�؂�ւ�������s��
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "TURNAROUND") == 0)
		{
			// ����ʒm���M 
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// �J�E���g�_�E���J�n
			m_l_rnging_start_cntdwn = RNGING_TIME_XX34_LIMIT;

			//RNG��ʃt���O���]���^�ɐݒ�
			m_l_rng_type = RNG_TYPE_NOR;

			// �]���^�؂�ւ�������s���t���O��߂�
			m_l_dsnrng_turn_start_flag = 0;
		}
	}

	// (�Đ��^) �����W�`���؂�ւ�����̎��s���ʊm�F
	if (m_l_dsnrng_regen_start_flag == 1)
	{
		// �Đ��^�؂�ւ�������s��
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
		if (obs_adr != NULL)
		{
			strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
		}

		if (strcmp(m_sz_norm_rep, "REGEN") == 0)
		{
			// ����ʒm���M
			m_XXRngNumBtn.GetWindowTextW(str2);
			str1.Format(_T("DX%02d"), _wtoi(str2));
			theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING"), str1);

			// �J�E���g�_�E���J�n
			m_l_rep_rnging_start_cntdwn = REP_RNGING_TIME_XX34_LIMIT;

			// RNG��ʃt���O���Đ��^�ɐݒ�
			m_l_rng_type = RNG_TYPE_REP;

			// �Đ��^�؂�ւ�������s���t���O��߂�
			m_l_dsnrng_regen_start_flag = 0;
		}
	}

	// (�Đ��^) RNGING START �J�E���g�_�E�����Ƀf�[�^�� START �ɂȂ����ꍇ
	if (m_l_rep_rnging_start_cntdwn >= 0 &&
		strcmp(m_sz_rnging, "�J�n") == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_start_cntdwn = -1;
	}
	// (�Đ��^) RNGING START �J�E���g�_�E���^�C�����~�b�g�ɂȂ����ꍇ
	else if (m_l_rep_rnging_start_cntdwn == 0)
	{
		// �J�E���g�_�E���I��
		m_l_rep_rnging_start_cntdwn = -1;
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rnging_stop_cntdwn == -1 && m_l_rnging_start_cntdwn == -1)
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rnging_start34();
		}
		// STOP �̏ꍇ
		else
		{
			show_rnging_stop34();
		}
	}

	// �J�E���g�_�E���I�����ł���΃{�^���̃s�b�N�X�}�b�v�\��ւ��\
	if (m_l_rep_rnging_stop_cntdwn == -1 && m_l_rep_rnging_start_cntdwn == -1)
	{
		// START �̏ꍇ
		if (strcmp(m_sz_rnging, "�J�n") == 0)
		{
			show_rrng_start34();
		}
		// STOP �̏ꍇ
		else
		{
			show_rrng_stop34();
		}
	}

#if 0
	/*** �ϒ����u�̑I����Ԋm�F 2008-09-01 X-HPA���ݑΉ��ǉ� ******************************/
	// ���݂̏�Ԃ��Ď��c�a����擾
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_MOD_IN_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_mod_in_sel, sizeof(m_sz_mod_in_sel), obs_adr->sz_sts_name);
		CLogTraceEx::Write(_T(""), _T("CTrsControlXX"), _T("show_opsc_condition"), _T("X-IFSW.IF_CH_INPUT_SEL_ANS"), CString(m_sz_mod_in_sel), nLogEx::debug);
	}

	// TCRMOD���I������Ă���ꍇ
	if (strcmp(m_sz_mod_in_sel, "TCRMOD") == 0)
	{
		// TCRMOD���Z���V�e�B�u�APM MOD���C���Z���V�e�B�u�ɂ���
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
	}
	// PM MOD���I������Ă���ꍇ
	else if (strcmp(m_sz_mod_in_sel, "DSN-MODEM") == 0)
	{
		// TCRMOD���C���Z���V�e�B�u�APM MOD���Z���V�e�B�u�ɂ���
		m_HoldToneOffBtn.EnableWindow(TRUE);
		m_HoldToneOnBtn.EnableWindow(TRUE);
		m_ModulationOffBtn.EnableWindow(TRUE);
		m_ModulationOnBtn.EnableWindow(TRUE);
	}
	// ���Â�̏�Ԃł��Ȃ��ꍇ
	else
	{
		// �ǂ�����C���Z���V�e�B�u�ɂ���
		m_HoldToneOffBtn.EnableWindow(FALSE);
		m_HoldToneOnBtn.EnableWindow(FALSE);
		m_ModulationOffBtn.EnableWindow(FALSE);
		m_ModulationOnBtn.EnableWindow(FALSE);
	}
#endif
	UpdateData(FALSE);
}


/*============================================================================*/
/*! CTrsControlXX

-# �J�E���g�_�E���^�C���A�E�g

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::count_down()
{
	TRACE(_T("count_down\n"));
	KillTimer(TID_COUNTDOWN);

	// 0 �ȏ�̓J�E���g�_�E��

	/*-- TX CONTROL ----------------------------------------------------------------------*/

	// RF-POWER OFF
	if (m_l_rf_power_off_cntdwn > 0) 
	{
		TRACE(_T("RF-POWER OFF �J�E���g�_�E��:%d\n"), m_l_rf_power_off_cntdwn);
		m_l_rf_power_off_cntdwn--;
	}

	// RF-POWER ON
	if (m_l_rf_power_on_cntdwn > 0)
	{
		TRACE(_T("RF-POWER ON �J�E���g�_�E��:%d\n"), m_l_rf_power_on_cntdwn);
		m_l_rf_power_on_cntdwn--;
	}

	// NO-USE OFF
	if (m_l_no_use_off_cntdwn > 0)
	{
		TRACE(_T("NO-USE OFF �J�E���g�_�E��:%d\n"), m_l_no_use_off_cntdwn);
		m_l_no_use_off_cntdwn--;
	}

	// NO-USE ON
	if (m_l_no_use_on_cntdwn > 0) 
	{
		TRACE(_T("NO-USE ON �J�E���g�_�E��:%d\n"), m_l_no_use_on_cntdwn);
		m_l_no_use_on_cntdwn--;
	}

	// SWEEP-CONT STOP
	if (m_l_main_sweep_cont_stop_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT STOP �J�E���g�_�E��:%d\n"), m_l_main_sweep_cont_stop_cntdwn);
		m_l_main_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT INC
	if (m_l_main_sweep_cont_inc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT INC �J�E���g�_�E��:%d\n"), m_l_main_sweep_cont_inc_cntdwn);
		m_l_main_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT INCDEC
	if (m_l_main_sweep_cont_incdec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT +-SAW/TRIANGLE �J�E���g�_�E��:%d\n"), m_l_main_sweep_cont_incdec_cntdwn);
		m_l_main_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT DEC
	if (m_l_main_sweep_cont_dec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT DEC �J�E���g�_�E��:%d\n"), m_l_main_sweep_cont_dec_cntdwn);
		m_l_main_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT DECINC
	if (m_l_main_sweep_cont_decinc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT -+SAWC �J�E���g�_�E��:%d\n"), m_l_main_sweep_cont_decinc_cntdwn);
		m_l_main_sweep_cont_decinc_cntdwn--;
	}

	// SWEEP-CONT(SUB) STOP
	if (m_l_sub_sweep_cont_stop_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT STOP �J�E���g�_�E��:%d\n"), m_l_sub_sweep_cont_stop_cntdwn);
		m_l_sub_sweep_cont_stop_cntdwn--;
	}

	// SWEEP-CONT(SUB) INC
	if (m_l_sub_sweep_cont_inc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT INC �J�E���g�_�E��:%d\n"), m_l_sub_sweep_cont_inc_cntdwn);
		m_l_sub_sweep_cont_inc_cntdwn--;
	}

	// SWEEP-CONT(SUB) INCDEC
	if (m_l_sub_sweep_cont_incdec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT +-SAW/TRIANGLE �J�E���g�_�E��:%d\n"), m_l_sub_sweep_cont_incdec_cntdwn);
		m_l_sub_sweep_cont_incdec_cntdwn--;
	}

	// SWEEP-CONT(SUB) DEC
	if (m_l_sub_sweep_cont_dec_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT DEC �J�E���g�_�E��:%d\n"), m_l_sub_sweep_cont_dec_cntdwn);
		m_l_sub_sweep_cont_dec_cntdwn--;
	}

	// SWEEP-CONT(SUB) DECINC
	if (m_l_sub_sweep_cont_decinc_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-CONT -+SAWC �J�E���g�_�E��:%d\n"), m_l_sub_sweep_cont_decinc_cntdwn);
		m_l_sub_sweep_cont_decinc_cntdwn--;
	}

	// SWEEP-NUM
	if (m_l_sweep_num_cntdwn > 0) 
	{
		TRACE(_T("SWEEP-NUM �J�E���g�_�E��:%d\n"), m_l_sweep_num_cntdwn);
		m_l_sweep_num_cntdwn--;
	}

	// DOPP-CONT OFF
	if (m_l_dopp_cont_off_cntdwn > 0) 
	{
		TRACE(_T("DOPP-CONT OFF �J�E���g�_�E��:%d\n"), m_l_dopp_cont_off_cntdwn);
		m_l_dopp_cont_off_cntdwn--;
	}

	// DOPP-CONT ON
	if (m_l_dopp_cont_on_cntdwn > 0) 
	{
		TRACE(_T("DOPP-CONT ON �J�E���g�_�E��:%d\n"), m_l_dopp_cont_on_cntdwn);
		m_l_dopp_cont_on_cntdwn--;
	}

	// TCRMOD MODULATION ON
	if (m_l_modulation_on_cntdwn > 0)
	{
		TRACE(_T("TCRMOD MODULATION ON �J�E���g�_�E��:%d\n"), m_l_modulation_on_cntdwn);
		m_l_modulation_on_cntdwn--;
	}

	// TCRMOD MODULATION OFF
	if (m_l_modulation_off_cntdwn > 0)
	{
		TRACE(_T("TCRMOD MODULATION OFF �J�E���g�_�E��:%d\n"), m_l_modulation_off_cntdwn);
		m_l_modulation_off_cntdwn--;
	}

	// X-CMDOUT HOLD TONE ON
	if (m_l_hold_tone_on_cntdwn > 0)
	{
		TRACE(_T("X-CMDOUT HOLD TONE ON �J�E���g�_�E��:%d\n"), m_l_hold_tone_on_cntdwn);
		m_l_hold_tone_on_cntdwn--;
	}

	// X-CMDOUT HOLD TONE OFF
	if (m_l_hold_tone_off_cntdwn > 0)
	{
		TRACE(_T("X-CMDOUT HOLD TONE OFF �J�E���g�_�E��:%d\n"), m_l_hold_tone_off_cntdwn);
		m_l_hold_tone_off_cntdwn--;
	}

	/*-- RNG CONTROL ---------------------------------------------------------------------*/

	// COH/INCOH COH
	if (m_l_coh_incoh_coh_cntdwn > 0) 
	{
		TRACE(_T("COH/INCOH COH �J�E���g�_�E��:%d\n"), m_l_coh_incoh_coh_cntdwn);
		m_l_coh_incoh_coh_cntdwn--;
	}

	// COH/INCOH INCOH
	if (m_l_coh_incoh_incoh_cntdwn > 0) 
	{
		TRACE(_T("COH/INCOH INCOH �J�E���g�_�E��:%d\n"), m_l_coh_incoh_incoh_cntdwn);
		m_l_coh_incoh_incoh_cntdwn--;
	}

	// RNGING STOP (�]���^)
	if (m_l_rnging_stop_cntdwn > 0) 
	{
		TRACE(_T("RNGING STOP (�]���^) �J�E���g�_�E��:%d\n"), m_l_rnging_stop_cntdwn);
		m_l_rnging_stop_cntdwn--;
	}

	// RNGING START (�]���^)
	if (m_l_rnging_start_cntdwn > 0) 
	{
		TRACE(_T("RNGING START (�]���^) �J�E���g�_�E��:%d\n"), m_l_rnging_start_cntdwn);
		m_l_rnging_start_cntdwn--;
	}

	// RNGING STOP (�Đ��^)
	if (m_l_rep_rnging_stop_cntdwn > 0) 
	{
		TRACE(_T("RNGING STOP (�Đ��^) �J�E���g�_�E��:%d\n"), m_l_rep_rnging_stop_cntdwn);
		m_l_rep_rnging_stop_cntdwn--;
	}

	// RNGING START (�Đ��^) 
	if (m_l_rep_rnging_start_cntdwn > 0) 
	{
		TRACE(_T("RNGING START (�Đ��^) �J�E���g�_�E��:%d\n"), m_l_rep_rnging_start_cntdwn);
		m_l_rep_rnging_start_cntdwn--;
	}

	SetTimer(TID_COUNTDOWN, INTERVAL_COUNTDOWN, 0);
}


/*============================================================================*/
/*! CTrsControlXX

-# RF-POWER OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rf_power_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF ��
	if ((strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0))
	{
		// �����[�g�i�I���j
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	}
	else 
	{
		// ���[�J��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// ON ��
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl_x, "ON") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// S-TX��LAN�ڑ���Ԃ��m�F����
		if (strcmp(m_sz_lan_s_tx, "�ڑ�") == 0) 
		{
			// S-TX RF DET ANS ���m�F����
			if (strcmp(m_sz_rf_on_det_ans_s, "OFF") == 0) 
			{
				// �����[�g�i��I���j
				m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
			}
			else 
			{
				// ���[�J��
				m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
			}
		}
		else 
		{
			// �����[�g�i��I���j
			m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
		}
	}
	else 
	{
		// ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RF-POWER OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rf_power_off34()
{
	char	szRfOnEnblSts[32];

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_S_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_s_tx, sizeof(m_sz_lan_s_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_HPA_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_enbl_x, sizeof(m_sz_hpa_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// HPA/SSPA�ŏ����𕪂���
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPA���I������Ă���ꍇ
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_hpa_rf_on_enbl_x);
	}
	else
	{
		// SSPA���I������Ă���ꍇ
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_rf_on_enbl_x);
	}

	if ((strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0))
	{
		// �����[�g�i�I���j
		m_RfPwrOffBtn.SetColor(COLOR_REMOTES);
	}
	else
	{
		// ���[�J��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// ON ��
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(szRfOnEnblSts, "ON") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// S-TX��LAN�ڑ���Ԃ��m�F����
		if (strcmp(m_sz_lan_s_tx, "�ڑ�") == 0)
		{
			// S-TX RF DET ANS ���m�F����
			if (strcmp(m_sz_rf_on_det_ans_s, "OFF") == 0)
			{
				// �����[�g�i��I���j
				m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
			}
			else
			{
				// ���[�J��
				m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
			}
		}
		else
		{
			// �����[�g�i��I���j
			m_RfPwrOnBtn.SetColor(COLOR_REMOTE);
		}
	}
	else
	{
		// ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RF-POWER ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rf_power_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	// HPA-2���ݑΉ�
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_HPA_SEL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_sel, sizeof(m_sz_hpa_sel), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_DET_ANS_X_H2));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x_h2, sizeof(m_sz_rf_on_det_ans_x_h2), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// ON ��
	if ((strcmp(m_sz_lan_x_tx, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0))
	{ 
		// ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}
	else if (((strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0)) && 
		(((strcmp(m_sz_hpa_sel, "NO1") == 0) && (strcmp(m_sz_rf_on_det_ans_x, "OFF") == 0))
		|| ((strcmp(m_sz_hpa_sel, "NO2") == 0) && (strcmp(m_sz_rf_on_det_ans_x_h2, "OFF") == 0)))) 
	{
		// �����[�g�i�I���j/ ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (((strcmp(m_sz_hpa_sel, "NO1") == 0) && (strcmp(m_sz_rf_on_det_ans_x, "OFF") == 0))
		|| ((strcmp(m_sz_hpa_sel, "NO2") == 0) && (strcmp(m_sz_rf_on_det_ans_x_h2, "OFF") == 0))) 
	{
		// �����[�g�i�I���j/ �����[�g�i��I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_rf_on_enbl_x, "OFF") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RfPwrOnBtn.Invalidate();
	m_RfPwrOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RF-POWER ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rf_power_on34()
{
	char	szRfOnEnblSts[32];
	char	szRfOnDetAnsSts[32];

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_enbl_x, sizeof(m_sz_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rf_on_det_ans_x, sizeof(m_sz_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_HPA_RF_ON_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_enbl_x, sizeof(m_sz_hpa_rf_on_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_HPA_RF_ON_DET_ANS_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_hpa_rf_on_det_ans_x, sizeof(m_sz_hpa_rf_on_det_ans_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// HPA/SSPA�ŏ����𕪂���
	if (strcmp(m_sz_xpa_sel, "HPA") == 0)
	{
		// HPA���I������Ă���ꍇ
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_hpa_rf_on_enbl_x);
		strcpy_s(szRfOnDetAnsSts, sizeof(szRfOnDetAnsSts), m_sz_hpa_rf_on_det_ans_x);
	}
	else
	{
		// SSPA���I������Ă���ꍇ
		strcpy_s(szRfOnEnblSts, sizeof(szRfOnEnblSts), m_sz_rf_on_enbl_x);
		strcpy_s(szRfOnDetAnsSts, sizeof(szRfOnDetAnsSts), m_sz_rf_on_det_ans_x);
	}

	if ((strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0))
	{
		// ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_LOCAL);
	}
	else if ((((strcmp(m_sz_x_ul_cnt_rl, "LOCAL") == 0) && (strcmp(m_sz_x_uc_rl, "LOCAL") == 0) && (strcmp(m_sz_x_rf_if_rl, "LOCAL") == 0))) && (strcmp(szRfOnDetAnsSts, "OFF") == 0)) 
	{
		// �����[�g�i�I���j/ ���[�J��
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_LOCAL, TRUE);
	}
	else if (strcmp(szRfOnDetAnsSts, "OFF") == 0)
	{
		// �����[�g�i�I���j/ �����[�g�i��I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES, COLOR_REMOTE, TRUE);
	}
	else
	{
		// �����[�g�i�I���j
		m_RfPwrOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(szRfOnEnblSts, "OFF") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RfPwrOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RfPwrOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_RfPwrOnBtn.Invalidate();
	m_RfPwrOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# NO-USE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_no_use_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF ��
	if ((strcmp(m_sz_lan_x_tx, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0))
	{
		// ���[�J��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J����
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# NO-USE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_no_use_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// OFF ��
	if ((strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0))
	{
		// ���[�J��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_NoUseOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_NoUseOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J����
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
	m_NoUseOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# NO-USE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_no_use_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TX));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_tx, sizeof(m_sz_lan_x_tx), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// ON ��
	if ((strcmp(m_sz_lan_x_tx, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0)) 
	{
		// ���[�J��
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_tx, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_NoUseOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOnBtn.Invalidate();
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# NO-USE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_no_use_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NO_USE_ENBL_X));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_no_use_enbl_x, sizeof(m_sz_no_use_enbl_x), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UL_CNT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_ul_cnt, sizeof(m_sz_lan_x_ul_cnt), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_UC));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_uc, sizeof(m_sz_lan_x_uc), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_RF_IF));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_rf_if, sizeof(m_sz_lan_x_rf_if), obs_adr->sz_sts_name);
	}

	// ON ��
	if ((strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") != 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") != 0))
	{
		// ���[�J��
		m_NoUseOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_NoUseOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_x_ul_cnt_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_uc_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_x_rf_if_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_ul_cnt, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_uc, "�ڑ�") == 0) &&
		(strcmp(m_sz_lan_x_rf_if, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_NoUseOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_NoUseOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_NoUseOnBtn.Invalidate();
	m_NoUseOffBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_stop64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_stop34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_inc64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_inc34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_incdec64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_incdec34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_dec64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_dec34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_decinc64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(MAIN) DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_main_sweep_cont_decinc34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountMDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountMDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountMStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountMDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountMStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountMDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountMStopBtn.Invalidate();
	m_SweepCountMIncBtn.Invalidate();
	m_SweepCountMIncDecBtn.Invalidate();
	m_SweepCountMDecBtn.Invalidate();
	m_SweepCountMDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_stop64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_stop34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_inc64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) INC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_inc34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_incdec64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) INCDEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_incdec34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_dec64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) DEC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_dec34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_decinc64()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# SWEEP-CONT(SUB) DECINC ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_sub_sweep_cont_decinc34()
{
	EnableSweetCtrlSubCarr();

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_SweepCountSDecIncBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_SweepCountSDecIncBtn.SetColor(COLOR_REMOTES);
	}

	// ���̑��̃{�^��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) && (strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) && (m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_SweepCountSStopBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSIncDecBtn.SetColor(COLOR_REMOTE);
		m_SweepCountSDecBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_SweepCountSStopBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSIncDecBtn.SetColor(COLOR_LOCAL);
		m_SweepCountSDecBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_SweepCountSStopBtn.Invalidate();
	m_SweepCountSIncBtn.Invalidate();
	m_SweepCountSIncDecBtn.Invalidate();
	m_SweepCountSDecBtn.Invalidate();
	m_SweepCountSDecIncBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# DOPP-CONT OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_dopp_cont_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// OFF ��
	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_DoppContOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_DoppContOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_DoppContOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_DoppContOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# DOPP-CONT OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_dopp_cont_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// OFF ��
	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_DoppContOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_DoppContOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_DoppContOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_DoppContOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# DOPP-CONT ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_dopp_cont_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// ON ��
	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_DoppContOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_DoppContOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_DoppContOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_DoppContOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# DOPP-CONT ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_dopp_cont_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// ON ��
	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_DoppContOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_DoppContOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_DoppContOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_DoppContOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_DoppContOffBtn.Invalidate();
	m_DoppContOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# MODULATION OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_modulation_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	// OFF ��
	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_ModulationOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# MODULATION OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_modulation_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// OFF ��
	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_ModulationOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_ModulationOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# MODULATION ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_modulation_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	// ON ��
	if (strcmp (m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_ModulationOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_ModulationOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# MODULATION ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_modulation_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_TXSYN));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_txsyn, sizeof(m_sz_lan_x_txsyn), obs_adr->sz_sts_name);
	}

	// ON ��
	if (strcmp(m_sz_lan_x_txsyn, "�ڑ�") != 0)
	{
		// ���[�J��
		m_ModulationOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_ModulationOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_lan_x_txsyn, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_ModulationOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_ModulationOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_ModulationOffBtn.Invalidate();
	m_ModulationOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# HOLD-TONE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_hold_tone_off64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// OFF ��
	if (strcmp(m_sz_lan_x_cmdout, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_lan_x_cmdout, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# HOLD-TONE OFF ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_hold_tone_off34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// OFF ��
	if (strcmp(m_sz_lan_x_cmdout, "�ڑ�") != 0)
	{
		// ���[�J��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_HoldToneOffBtn.SetColor(COLOR_REMOTES);
	}

	// ON ��
	if ((strcmp(m_sz_lan_x_cmdout, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_HoldToneOnBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# HOLD-TONE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_hold_tone_on64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// ON ��
	if (strcmp(m_sz_lan_x_cmdout, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_HoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_lan_x_cmdout, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_HoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# HOLD-TONE ON ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_hold_tone_on34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_X_CMDOUT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_x_cmdout, sizeof(m_sz_lan_x_cmdout), obs_adr->sz_sts_name);
	}

	// ON ��
	if (strcmp(m_sz_lan_x_cmdout, "�ڑ�") != 0)
	{
		// ���[�J��
		m_HoldToneOnBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_HoldToneOnBtn.SetColor(COLOR_REMOTES);
	}

	// OFF ��
	if ((strcmp(m_sz_lan_x_cmdout, "�ڑ�") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_HoldToneOffBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_HoldToneOffBtn.SetColor(COLOR_LOCAL);
	}

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# X/X-COH COH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_coh_incoh_coh64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_XXCohInCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
		}

		// �����[�g�i�I���j
		m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# X/X-COH COH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_coh_incoh_coh34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0)
	{
		// ���[�J��
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		if (m_i_priv_flag == 1)
		{
			// �����[�g�i��I���j
			m_XXCohInCohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ���[�J��
			m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
		}

		// �����[�g�i�I���j
		m_XXCohCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# X/X-COH INCOH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_coh_incoh_incoh64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		if (m_i_priv_flag == 1) 
		{
			// �����[�g�i��I���j
			m_XXCohCohBtn.SetColor(COLOR_REMOTE);
		}
		else 
		{
			// ���[�J��
			m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j
		m_XXCohInCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# X/X-COH INCOH ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_coh_incoh_incoh34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_V_EQU));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_vequ, sizeof(m_sz_lan_vequ), obs_adr->sz_sts_name);
	}

	if (strcmp(m_sz_lan_vequ, "�ڑ�") != 0)
	{
		// ���[�J��
		m_XXCohInCohBtn.SetColor(COLOR_LOCAL);
		m_XXCohCohBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		if (m_i_priv_flag == 1)
		{
			// �����[�g�i��I���j
			m_XXCohCohBtn.SetColor(COLOR_REMOTE);
		}
		else
		{
			// ���[�J��
			m_XXCohCohBtn.SetColor(COLOR_LOCAL);
		}
		// �����[�g�i�I���j
		m_XXCohInCohBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RNGING STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rnging_stop64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	// �؂�ւ����s���t���O�������Ă���ꍇ�́A���䒆�ɂ���
	if (m_l_dsnrng_turn_start_flag == 1) 
	{
		m_RngingStopBtn.SetColor(COLOR_CTRLIN);
		m_RngingStartBtn.SetColor(COLOR_CTRLIN);
		return;
	}

	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// �C���Z���V�e�B�u�̏ꍇ�́A�Z���V�e�B�u�ɂ���
	if (m_RngingStartBtn.IsWindowEnabled() == FALSE)
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	if (strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0) 
	{
		// ���[�J��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RngingStopBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RNGING STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rnging_stop34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
	}

	// �؂�ւ����s���t���O�������Ă���ꍇ�́A���䒆�ɂ���
	if (m_l_dsnrng_turn_start_flag == 1)
	{
		m_RngingStopBtn.SetColor(COLOR_CTRLIN);
		m_RngingStartBtn.SetColor(COLOR_CTRLIN);
		return;
	}

	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_pass_flag, "�p�X��") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTE);
	}
	// ����ȊO
	else
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}

	// �C���Z���V�e�B�u�̏ꍇ�́A�Z���V�e�B�u�ɂ���
	if ((m_RngingStartBtn.IsWindowEnabled() == FALSE) && (strcmp(m_sz_rnging, "�J�n") != 0))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	if (strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RngingStopBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RNGING START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rnging_start64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// �]���^���I�΂�Ă��Ȃ���΁A�{�^�����C���Z���V�e�B�u�ɂ���
	if ((strcmp(m_sz_norm_rep, "TURNAROUND") != 0) && (m_RngingStartBtn.IsWindowEnabled() != FALSE)) 
	{
		m_RngingStartBtn.EnableWindow(FALSE);
		m_RngingStopBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "TURNAROUND") == 0) && (m_RngingStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_NOR))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}
	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "TURNAROUND") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RngingStopBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}

	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0) ||
		(strcmp(m_sz_norm_rep, "TURNAROUND") != 0)) 
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# RNGING START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rnging_start34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// �]���^���I�΂�Ă��Ȃ���΁A�{�^�����C���Z���V�e�B�u�ɂ���
	if ((strcmp(m_sz_norm_rep, "TURNAROUND") != 0) && (m_RngingStartBtn.IsWindowEnabled() != FALSE))
	{
		m_RngingStartBtn.EnableWindow(FALSE);
		m_RngingStopBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "TURNAROUND") == 0) && (m_RngingStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_NOR))
	{
		m_RngingStartBtn.EnableWindow(TRUE);
		m_RngingStopBtn.EnableWindow(TRUE);
	}

	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "TURNAROUND") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RngingStopBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RngingStopBtn.SetColor(COLOR_LOCAL);
	}

	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0) ||
		(strcmp(m_sz_norm_rep, "TURNAROUND") != 0))
	{
		// ���[�J��
		m_RngingStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RngingStartBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_RngingStopBtn.Invalidate();
	m_RngingStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �Đ��^ STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rrng_stop64()
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;
	struct rarrband_t *rarrband_t_adr;	// �q�`�q�q�g�p�ш���e�[�u���A�h���X

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	// �؂�ւ����s���t���O�������Ă���ꍇ�́A���䒆�ɂ���
	if (m_l_dsnrng_regen_start_flag == 1) 
	{
		m_RRngStopBtn.SetColor(COLOR_CTRLIN);
		m_RRngStartBtn.SetColor(COLOR_CTRLIN);
		return;
	}

	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RRngStartBtn.SetColor(COLOR_REMOTE);
	}
	// ����ȊO
	else 
	{
		// ���[�J��
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}

	// �p�X���ƃp�X�O�ŏ����𕪂���
	if (strcmp(m_sz_pass_flag, MON_STR_PASS) != 0) 
	{
		// �p�X�O�̏ꍇ
		if (m_RRngStartBtn.IsWindowEnabled() == FALSE)
		{
			// �C���Z���V�e�B�u�Ȃ�Z���V�e�B�u�ɂ���
			m_RRngStopBtn.EnableWindow(TRUE);
			m_RRngStartBtn.EnableWindow(TRUE);
		}
	}
	else 
	{
		// �p�X���̏ꍇ

		/***
		�p�X���̏ꍇ�́A�^�p���̉q�����Đ������ɑΉ����Ă���ꍇ�̂݃Z���V�e�B�u�ɂ���
		***/

		CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

		vector<CString>			&saNameList = dba.GetSatNameList();
		map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

		if (saNameList.size() == 0 || satAdrList.size() == 0)
		{
			return ;
		}

		if (CString(m_sz_sat_name).IsEmpty() == FALSE)
		{
			satinf_adr = (struct satinf_t *)((__int64*)satAdrList[CString(m_sz_sat_name)]);
			original_adr = satinf_adr;
			sattbl_adr = (char *)satinf_adr;
			while (1)
			{
				satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_t�ɕϊ�*/
				if (satinf_adr->l_id == ENDOFSAT_ID)
				{
					/*���ʂh�c���I���̏ꍇ*/
					break;
				}

				// �w�q�`�q�q�g�p�ш���
				if (satinf_adr->l_id == RARRBAND_ID)
				{
					rarrband_t_adr = (struct rarrband_t *)satinf_adr;
					if( (rarrband_t_adr->l_rarr_band == RARRBAND_RX_ID) ||
						(rarrband_t_adr->l_rarr_band == RARRBAND_RXKA_ID) )
					{
						// �C���Z���V�e�B�u�̏ꍇ�̓Z���V�e�B�u�ɂ���
						if (m_RRngStartBtn.IsWindowEnabled() == FALSE)
						{
							m_RRngStartBtn.EnableWindow(TRUE);
							m_RRngStopBtn.EnableWindow(TRUE);
						}
					}
					else
					{
						// �Z���V�e�B�u�̏ꍇ�̓C���Z���V�e�B�u�ɂ���
						if (m_RRngStartBtn.IsWindowEnabled() == TRUE)
						{
							m_RRngStartBtn.EnableWindow(FALSE);
							m_RRngStopBtn.EnableWindow(FALSE);
						}
					}
					break;
				}
				sattbl_adr += satinf_adr->l_len;
			}
		}
	}

	if (strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RRngStopBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �Đ��^ STOP ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rrng_stop34()
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;
	struct rarrband_t *rarrband_t_adr;	// �q�`�q�q�g�p�ш���e�[�u���A�h���X

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_XX_RRNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RRNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_P_FLG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_pass_flag, sizeof(m_sz_pass_flag), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_CTRL_SAT_NAME));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_sat_name, sizeof(m_sz_sat_name), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_RNGING));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_rnging, sizeof(m_sz_rnging), obs_adr->sz_sts_name);
	}

	// �؂�ւ����s���t���O�������Ă���ꍇ�́A���䒆�ɂ���
	if (m_l_dsnrng_regen_start_flag == 1)
	{
		m_RRngStopBtn.SetColor(COLOR_CTRLIN);
		m_RRngStartBtn.SetColor(COLOR_CTRLIN);
		return;
	}

	// XX-RARR LAN�ڑ���
	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") == 0) &&
		(strcmp(m_sz_pass_flag, "�p�X��") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RRngStartBtn.SetColor(COLOR_REMOTE);
	}
	// ����ȊO
	else
	{
		// ���[�J��
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}

	// �p�X���ƃp�X�O�ŏ����𕪂���
	if (strcmp(m_sz_pass_flag, MON_STR_PASS) != 0)
	{
		// �p�X�O�̏ꍇ
		if ((m_RRngStartBtn.IsWindowEnabled() == FALSE) && (strcmp(m_sz_rnging, "�J�n") != 0))
		{
			// �C���Z���V�e�B�u�Ȃ�Z���V�e�B�u�ɂ���
			m_RRngStopBtn.EnableWindow(TRUE);
			m_RRngStartBtn.EnableWindow(TRUE);
		}
	}
	else
	{
		// �p�X���̏ꍇ

		/***
		�p�X���̏ꍇ�́A�^�p���̉q�����Đ������ɑΉ����Ă���ꍇ�̂݃Z���V�e�B�u�ɂ���
		***/

		CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

		vector<CString>			&saNameList = dba.GetSatNameList();
		map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

		if (saNameList.size() == 0 || satAdrList.size() == 0)
		{
			return;
		}

		satinf_adr = (struct satinf_t *)((__int64*)satAdrList[CString(m_sz_sat_name)]);
		original_adr = satinf_adr;
		sattbl_adr = (char *)satinf_adr;
		while (1)
		{
			satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_t�ɕϊ�*/
			if (satinf_adr->l_id == ENDOFSAT_ID)
			{
				/*���ʂh�c���I���̏ꍇ*/
				break;
			}

			// �w�q�`�q�q�g�p�ш���
			if (satinf_adr->l_id == RARRBAND_ID)
			{
				rarrband_t_adr = (struct rarrband_t *)satinf_adr;
				if (rarrband_t_adr->l_rarr_band == RARRBAND_RX_ID)
				{
					// �C���Z���V�e�B�u�̏ꍇ�̓Z���V�e�B�u�ɂ���
					if ((m_RRngStartBtn.IsWindowEnabled() == FALSE) && (strcmp(m_sz_rnging, "�J�n") != 0))
					{
						m_RRngStartBtn.EnableWindow(TRUE);
						m_RRngStopBtn.EnableWindow(TRUE);
					}
				}
				else
				{
					// �Z���V�e�B�u�̏ꍇ�̓C���Z���V�e�B�u�ɂ���
					if (m_RRngStartBtn.IsWindowEnabled() == TRUE)
					{
						m_RRngStartBtn.EnableWindow(FALSE);
						m_RRngStopBtn.EnableWindow(FALSE);
					}
				}
				break;
			}
			sattbl_adr += satinf_adr->l_len;
		}
	}

	if (strcmp(m_sz_lan_xx_rrng, "�ڑ�") != 0)
	{
		// ���[�J��
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RRngStopBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �Đ��^ START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rrng_start64()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_LAN_XX_RARR));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rarr, sizeof(m_sz_lan_xx_rarr), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_DSNTCR_RL));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_dsntcr_rl, sizeof(m_sz_dsntcr_rl), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// �Đ��^���I�΂�Ă��Ȃ���΁A�{�^�����C���Z���V�e�B�u�ɂ���
	if ((strcmp(m_sz_norm_rep, "REGEN") != 0) && (m_RRngStartBtn.IsWindowEnabled() != FALSE))
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "REGEN") == 0) && (m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_REP)) 
	{
		m_RRngStopBtn.EnableWindow(TRUE);
		m_RRngStartBtn.EnableWindow(TRUE);
	}

	// XX-RRNG LAN�ڑ����͗΂ɂ���
	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") == 0) &&
		(strcmp(m_sz_dsntcr_rl, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "REGEN") == 0) &&
		(m_i_priv_flag == 1)) 
	{
		// �����[�g�i��I���j
		m_RRngStopBtn.SetColor(COLOR_REMOTE);
	}
	else 
	{
		// ���[�J��
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}

	if ((strcmp(m_sz_lan_xx_rarr, "�ڑ�") != 0) ||
		(strcmp(m_sz_norm_rep, "REGEN") != 0)) 
	{
		// ���[�J��
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}
	else 
	{
		// �����[�g�i�I���j
		m_RRngStartBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# �Đ��^ START ��ԉ�ʕ\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::show_rrng_start34()
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();

	CSatelliteData& sd = theApp.GetSatelliteData();

	// �����`�F�b�N�p�Ď����ڂ��Q��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX34_LAN_XX_RRNG));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_lan_xx_rrng, sizeof(m_sz_lan_xx_rrng), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_XX_RRNG_CONT));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_xx_rrng_cont, sizeof(m_sz_xx_rrng_cont), obs_adr->sz_sts_name);
	}

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(MON_XX_NORM_REP));
	if (obs_adr != NULL)
	{
		strcpy_s(m_sz_norm_rep, sizeof(m_sz_norm_rep), obs_adr->sz_sts_name);
	}

	// �Đ��^���I�΂�Ă��Ȃ���΁A�{�^�����C���Z���V�e�B�u�ɂ���
	if ((strcmp(m_sz_norm_rep, "REGEN") != 0) && (m_RRngStartBtn.IsWindowEnabled() != FALSE))
	{
		m_RRngStopBtn.EnableWindow(FALSE);
		m_RRngStartBtn.EnableWindow(FALSE);
	}
	else if ((strcmp(m_sz_norm_rep, "REGEN") == 0) && (m_RRngStartBtn.IsWindowEnabled() == FALSE) && (m_l_rng_type == RNG_TYPE_REP))
	{
		m_RRngStopBtn.EnableWindow(TRUE);
		m_RRngStartBtn.EnableWindow(TRUE);
	}

	// XX-RRNG LAN�ڑ����͗΂ɂ���
	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") == 0) &&
		(strcmp(m_sz_xx_rrng_cont, "REMOTE") == 0) &&
		(strcmp(m_sz_norm_rep, "REGEN") == 0) &&
		(m_i_priv_flag == 1))
	{
		// �����[�g�i��I���j
		m_RRngStopBtn.SetColor(COLOR_REMOTE);
	}
	else
	{
		// ���[�J��
		m_RRngStopBtn.SetColor(COLOR_LOCAL);
	}

	if ((strcmp(m_sz_lan_xx_rrng, "�ڑ�") != 0) ||
		(strcmp(m_sz_norm_rep, "REGEN") != 0))
	{
		// ���[�J��
		m_RRngStartBtn.SetColor(COLOR_LOCAL);
	}
	else
	{
		// �����[�g�i�I���j
		m_RRngStartBtn.SetColor(COLOR_REMOTES);
	}

	// �X�V
	m_RRngStopBtn.Invalidate();
	m_RRngStartBtn.Invalidate();
}


/*============================================================================*/
/*! CTrsControlXX

-# HOLD TONE ON ��ԉ�ʕ\����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::printlg_endtime(CString str)
{
	// �Ď��f�[�^�擾

	// ���ݎ����擾

	// �X�C�[�v�����\�莞���Z�o

	// opelog�o��
}


void CTrsControlXX::OnClose()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		return;
	}

	CloseDlg();
	//	OnOK();
	ShowWindow(SW_HIDE);
}

void CTrsControlXX::CloseDlg()
{
	KillTimer(TID_CONDITION);
	KillTimer(TID_COUNTDOWN);

	// �E�B���h�E����郁�b�Z�[�W
	theApp.m_pMainWnd->PostMessageW(eMessage_WindowClose, 0, (LPARAM)this);
}

/*============================================================================*/
/*! CTrsControlXX

-# SWEET CTRL��Sub Carr��L��������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX::EnableSweetCtrlSubCarr()
{
	if (m_SweepContSubChkBtn.IsWindowEnabled() == FALSE)
	{
		m_SweepContSubChkBtn.EnableWindow(TRUE);
		OnBnClickedTrxxctrlSweetSubCheck();
	}
}
