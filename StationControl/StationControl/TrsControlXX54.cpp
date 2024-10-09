/*============================================================================*/
/*! CTrsControlXX54.cpp

-# X�ё��M������ (54m��)
*/
/*============================================================================*/
// CTrsControlXX54.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControlXX54.h"
#include "afxdialogex.h"

// CTrsControlXX54 �_�C�A���O

IMPLEMENT_DYNAMIC(CTrsControlXX54, CTrsControlXX)

CTrsControlXX54::CTrsControlXX54(CWnd* pParent /*=NULL*/)
: CTrsControlXX(CTrsControlXX54::IDD, pParent)
{

}

CTrsControlXX54::~CTrsControlXX54()
{
}

void CTrsControlXX54::DoDataExchange(CDataExchange* pDX)
{
	CTrsControlXX::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DELTAF_RECOVERY_GROUP, m_grDeltafRecovery);
	DDX_Control(pDX, IDC_STATIC_DELTAF_TIMERATE_GROUP, m_grDelfafTimeRate);
	DDX_Text(pDX, IDC_STATIC_DELTAF_MON, m_DeltafCtrlFreqMoni);
	DDX_Control(pDX, IDC_STATIC_DELTAF_RECOVERY_BOX, m_grRecovery);
	DDX_Control(pDX, IDC_TRXXCTRL_RECOVERY_BTNOFF, m_RecoveryOffBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_RECOVERY_BTNON, m_RecoveryOnBtn);
	DDX_Check(pDX, IDC_TRXXCTRL_DELTAF_CTRL_CHECK, m_DeltafCtrlFreqCtrChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_CTRL_CHECK, m_DeltafCtrlChkBtnCtrl);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_CTRL_EDIT, m_strDeltafCtrl);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_TIME_CHECK, m_DeltafCtrlTimeChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_TIME_EDIT, m_strDeltafTime);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_RATE_CHECK, m_DeltafCtrlFreqChkBtn);
	DDX_Control(pDX, IDC_TRXXCTRL_DELTAF_RATE_EDIT, m_strDeltafFreq);
	DDX_Text(pDX, IDC_STATIC_DELTAF_ENDTIME, m_DeltafCtrlFreqEndTime);
}

BEGIN_MESSAGE_MAP(CTrsControlXX54, CTrsControlXX)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRXXCTRL_RF_OFF_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRfOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RF_ON_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRfOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_NOUSE_OFF_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlNouseOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_NOUSE_ON_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlNouseOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXCOH_COH_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlXxcohCohBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_XXCOH_INCOH_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlXxcohIncohBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RNGING_STOP_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRngingStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RNGING_START_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRngingStartBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RGNTYPE_STOP_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRgntypeStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RGNTYPE_START_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlRgntypeStartBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RECOVERY_BTNOFF, &CTrsControlXX54::OnBnClickedRecoveryOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_RECOVERY_BTNON, &CTrsControlXX54::OnBnClickedRecoveryOnBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_DELTAF_CTRL_CHECK, &CTrsControlXX54::OnBnClickedDeltafCtrlFreqCtrChk)
	ON_BN_CLICKED(IDC_TRXXCTRL_DELTAF_TIME_CHECK, &CTrsControlXX54::OnBnClickedDeltafCtrlTimeChk)
	ON_BN_CLICKED(IDC_TRXXCTRL_DELTAF_RATE_CHECK, &CTrsControlXX54::OnBnClickedDeltafCtrlFreqChk)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_STOP_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_INC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetIncBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_INCDEC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetIncdecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_DEC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetDecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_DECINC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetDecincBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_STOP_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubStopBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_INC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubIncBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_INCDEC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubIncdecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_DEC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubDecBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_SWEET_SUB_DECINC_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlSweetSubDecincBtn)
	ON_EN_CHANGE(IDC_TRXXCTRL_DELTAF_TIME_EDIT, &CTrsControlXX54::OnEnChangeTrxxctrlDeltafTimeEdit)
	ON_BN_CLICKED(IDC_TRXXCTRL_HOLD_OFF_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlHoldOffBtn)
	ON_BN_CLICKED(IDC_TRXXCTRL_HOLD_ON_BTN, &CTrsControlXX54::OnBnClickedTrxxctrlHoldOnBtn)
END_MESSAGE_MAP()

/*============================================================================*/
// �_�C�A���O����������
/*============================================================================*/
BOOL CTrsControlXX54::OnInitDialog()
{
	CTrsControlXX::OnInitDialog();

	m_DeltafCtrlFreqCtrChkBtn = FALSE;			//��f�iCTRL�j�`�F�b�N�{�b�N�X
	m_strDeltafCtrl.EnableWindow(FALSE);		//��f�iCTRL�j���͔񊈐�
	m_strDeltafFreq.SetWindowText(_T("0.1"));	//RATE
	m_strPreDeltafTime = _T("1");
	m_strDeltafTime.SetWindowText(m_strPreDeltafTime);		//TIME
	m_strDeltafFreq.EnableWindow(FALSE);		//RATE EDIT
	m_strDeltafTime.EnableWindow(FALSE);		//TIME EDIT

	// RECOVERY
	m_RecoveryOffBtn.EnableWindow(FALSE);		//RECOVERY OFF
	m_RecoveryOffBtn.SetColor(COLOR_REMOTES);	//�I��   �I�����W
	m_RecoveryOffBtn.SetCtrlButton(TRUE);		//Ctrl�L�[ +
	m_RecoveryOnBtn.SetColor(COLOR_REMOTE);		//��I�� ��
	m_RecoveryOnBtn.SetCtrlButton(TRUE);		//Ctrl�L�[ +
	m_RecoveryCont = FALSE;
	m_RngFlg = FALSE;
	m_strDeltafCtrlInit = FALSE;

	// �O���[�v�{�b�N�X
	m_grDeltafRecovery.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grDeltafRecovery.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grDelfafTimeRate.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grDelfafTimeRate.SetGroupType(CGroupBoxEx::eGroupType_Round);
	m_grRecovery.SetBackColor(GetSysColor(COLOR_3DFACE));
	m_grRecovery.SetGroupType(CGroupBoxEx::eGroupType_Round);

    m_grXXRng.SetWindowText(_T("RANGING NUM"));
    m_grXXCoh.SetWindowText(_T("COH"));

	m_grNoUse.ShowWindow(SW_HIDE);
	m_NoUseOffBtn.ShowWindow(SW_HIDE);
	m_NoUseOnBtn.ShowWindow(SW_HIDE);

	CenterWindowEx();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

/*============================================================================*/
// �_�C�A���O�X�V
/*============================================================================*/
BOOL CTrsControlXX54::UpdateDlg()
{
	// ��ʍX�V
	SetData();

	CTrsControlXX::UpdateDlg();

	return TRUE;
}

/*============================================================================*/
// ��ʍX�V
/*============================================================================*/
void CTrsControlXX54::SetData()
{
	CTrsControlXX54::stSetMonData mMonTable[] = {
		{ _T("CTRL.DELTA_FREQ"),       m_DeltafCtrlFreqMoni,    _T("%0.1f"), eDOUBLE },	// ��f�iMON�j
		{ _T("CTRL.RECOVER_END_TIME"), m_DeltafCtrlFreqEndTime, _T("%s"),    eSTRING },	// �I���\�����
		{ _T("CTRL.RECOVER_CONT"),     m_RecoveryOffOn,         _T("%s"),    eSTRING },	// RECOVERY OFF/ON
		{ _T("CTRL.RNG_FLG"),          m_strRngFlg,             _T("%s"),    eSTRING },	// Range�v���� �I��/�J�n
	};

	// �^�C�}�j��
	KillTimer(TID_DELTAF_TRS);

	UpdateData(TRUE);

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();
	CSatelliteData& sd = theApp.GetSatelliteData();

	/****************************************************************/
	/* �Ď���� */
	for (int index = 0; index < sizeof(mMonTable) / sizeof(mMonTable[0]); index++){
		// �e�[�u���̊Ď�������Ď��f�[�^���擾���āA�R���g���[���l�ɐݒ肷��
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, mMonTable[index].monname);
		mMonTable[index].controlval.Empty();
		if (obs_adr != NULL){
			switch (mMonTable[index].type){
			case	eINT:
				mMonTable[index].controlval.Format(mMonTable[index].format, obs_adr->l_data);
				break;
			case	eDOUBLE:
				mMonTable[index].controlval.Format(mMonTable[index].format, obs_adr->d_data);
				break;
			case	eSTRING:
				mMonTable[index].controlval.Format(mMonTable[index].format, (LPCTSTR)CString(obs_adr->sz_sts_name));
				break;
			}
			if( (m_strDeltafCtrlInit == FALSE) && (index == 0) )
			{
				m_strDeltafCtrl.SetWindowText(m_DeltafCtrlFreqMoni);	//��f(CTRL)�̏����l�́A�Ď��f�[�^�u��f�v�̒l
				m_strDeltafCtrlInit = TRUE;								//�����l�ݒ��
			}
		}
		else
		{
			//TRACE("GetObsname_t()�G���[ �Ď���:%s\n", CStringA(mMonTable[index].monname));
		}
	}

//	//RECOVERY:ON or Range�v��:�J�n �Ȃ�{�^������͂�񊈐���
//	if( (m_RecoveryOffOn == _T("ON")) || (m_strRngFlg == _T("�J�n")) )
//	{
//		if(m_RecoveryCont != TRUE)
//		{
//			m_RecoveryCont = TRUE;
//			m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//��f (CTRL) CHECK
//			m_strDeltafCtrl.EnableWindow(FALSE);			//��f (CTRL) EDIT
//			m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//TIME CHECK
//			m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
//			m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//RATE CHECK
//			m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
//			m_RecoveryOffBtn.SetColor(COLOR_REMOTE);		//��I�� ��
//			m_RecoveryOnBtn.SetColor(COLOR_REMOTES);		//�I��   �I�����W
//			// �X�V
//			// m_RecoveryOffBtn.Invalidate();
//			// m_RecoveryOnBtn.Invalidate();
//			//RNG START�{�^��(�]���^�y�эĐ��^)�́A�Ď��f�[�^�uCTRL.RECOVER_CONT�v���uON�v�̏ꍇ�̓N���b�N�ł��Ȃ��悤�ɂ���B
//			m_RngingStartBtn.EnableWindow(FALSE);			//RANGING START�{�^��
//			m_RRngStartBtn.EnableWindow(FALSE);				//�Đ��^ START�{�^��
//			// �����E�񊈐�
//			m_RecoveryOffBtn.EnableWindow(TRUE);			//RECOVERY OFF
//			m_RecoveryOnBtn.EnableWindow(FALSE);			//RECOVERY ON
//
//			// �ĕ`�揈��
//			InvalidateRect(NULL, TRUE);
//		}
//	}
//	//RECOVERY:OFF and Range�v��:�I�� �Ȃ�{�^������͂�������
//	else
//	{
//		if(m_RecoveryCont == TRUE)
//		{
//			m_RecoveryCont = FALSE;
//			m_DeltafCtrlChkBtnCtrl.EnableWindow(TRUE);		//��f (CTRL) CHECK
//			m_strDeltafCtrl.EnableWindow(TRUE);				//��f (CTRL) EDIT
//			m_DeltafCtrlTimeChkBtn.EnableWindow(TRUE);		//TIME CHECK
//			m_DeltafCtrlFreqChkBtn.EnableWindow(TRUE);		//RATE CHECK
//			if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)			// TIME �Ƀ`�F�b�N
//			{
//				m_strDeltafTime.EnableWindow(TRUE);				//TIME EDIT
//				m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
//			}
//			else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//RATE �Ƀ`�F�b�N
//			{
//				m_strDeltafFreq.EnableWindow(TRUE);				//RATE EDIT
//				m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
//			}
//			m_RecoveryOffBtn.SetColor(COLOR_REMOTES);		//�I��   �I�����W
//			m_RecoveryOnBtn.SetColor(COLOR_REMOTE);			//��I�� ��
//			// �X�V
//			// m_RecoveryOffBtn.Invalidate();
//			// m_RecoveryOnBtn.Invalidate();
//			// ����
//			m_RngingStartBtn.EnableWindow(TRUE);			//RANGING START�{�^��
//			m_RRngStartBtn.EnableWindow(TRUE);				//�Đ��^ START�{�^��
//			// �����E�񊈐�
//			m_RecoveryOffBtn.EnableWindow(FALSE);			//RECOVERY OFF
//			m_RecoveryOnBtn.EnableWindow(TRUE);				//RECOVERY ON
//
//			// �ĕ`�揈��
//			InvalidateRect(NULL, TRUE);
//		}
//	}

	//Range�v��:�J�n�Ȃ�{�^������͂�S���񊈐���
	if (m_strRngFlg == _T("�J�n"))
	{
		// ���O�������W�v�����łȂ������ꍇ�A�`��ύX
		if (m_RngFlg != TRUE)
		{
			// �����W�v�����t���O�Z�b�g
			m_RngFlg = TRUE;
			m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//��f (CTRL) CHECK
			m_strDeltafCtrl.EnableWindow(FALSE);			//��f (CTRL) EDIT
			m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//TIME CHECK
			m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
			m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//RATE CHECK
			m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
			m_RecoveryOffBtn.SetColor(COLOR_REMOTES);		//�I��   �I�����W
			m_RecoveryOnBtn.SetColor(COLOR_LOCAL);			//���[�J���@��
			// �X�V
			// m_RecoveryOffBtn.Invalidate();
			// m_RecoveryOnBtn.Invalidate();
			//RNG START�{�^��(�]���^�y�эĐ��^)�́A�Ď��f�[�^�uCTRL.RECOVER_CONT�v���uON�v�̏ꍇ�̓N���b�N�ł��Ȃ��悤�ɂ���B
			m_RngingStartBtn.EnableWindow(FALSE);			//RANGING START�{�^��
			m_RRngStartBtn.EnableWindow(FALSE);				//�Đ��^ START�{�^��
			// �����E�񊈐�
			m_RecoveryOffBtn.EnableWindow(FALSE);			//RECOVERY OFF
			m_RecoveryOnBtn.EnableWindow(FALSE);			//RECOVERY ON

			// �ĕ`�揈��
			InvalidateRect(NULL, TRUE);
		}
	}
	//Range�v��:�J�n�ȊO
	else
	{
		//RECOVERY:ON�Ȃ�OFF�{�^���̂݊�����
		if (m_RecoveryOffOn == _T("ON"))
		{
			// ���O�������W�v�����A�܂��̓��J�o�����łȂ������ꍇ�A�`��ύX
			if (m_RngFlg == TRUE || m_RecoveryCont != TRUE)
			{
				// �����W�v�����t���O�N���A
				m_RngFlg = FALSE;
				// ���J�o���[�t���O�Z�b�g
				m_RecoveryCont = TRUE;
				m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//��f (CTRL) CHECK
				m_strDeltafCtrl.EnableWindow(FALSE);			//��f (CTRL) EDIT
				m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//TIME CHECK
				m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
				m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//RATE CHECK
				m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
				m_RecoveryOffBtn.SetColor(COLOR_REMOTE);		//��I�� ��
				m_RecoveryOnBtn.SetColor(COLOR_REMOTES);		//�I��   �I�����W
				// �X�V
				// m_RecoveryOffBtn.Invalidate();
				// m_RecoveryOnBtn.Invalidate();
				//RNG START�{�^��(�]���^�y�эĐ��^)�́A�Ď��f�[�^�uCTRL.RECOVER_CONT�v���uON�v�̏ꍇ�̓N���b�N�ł��Ȃ��悤�ɂ���B
				m_RngingStartBtn.EnableWindow(FALSE);			//RANGING START�{�^��
				m_RRngStartBtn.EnableWindow(FALSE);				//�Đ��^ START�{�^��
				// �����E�񊈐�
				m_RecoveryOffBtn.EnableWindow(TRUE);			//RECOVERY OFF
				m_RecoveryOnBtn.EnableWindow(FALSE);			//RECOVERY ON

				// �ĕ`�揈��
				InvalidateRect(NULL, TRUE);
			}
		}
		//RECOVERY:OFF�Ȃ��~�{�^���ȊO�Ɠ��͂��������B
		else
		{
			// ���O�������W�v�����A�܂��̓��J�o���[���������ꍇ�A�`��ύX
			if (m_RngFlg == TRUE || m_RecoveryCont == TRUE)
			{
				// �����W�v�����t���O�N���A
				m_RngFlg = FALSE;
				// ���J�o�����t���O�N���A
				m_RecoveryCont = FALSE;
				m_DeltafCtrlChkBtnCtrl.EnableWindow(TRUE);		//��f (CTRL) CHECK
				m_strDeltafCtrl.EnableWindow(TRUE);				//��f (CTRL) EDIT
				m_DeltafCtrlTimeChkBtn.EnableWindow(TRUE);		//TIME CHECK
				m_DeltafCtrlFreqChkBtn.EnableWindow(TRUE);		//RATE CHECK
				if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)			// TIME �Ƀ`�F�b�N
				{
					m_strDeltafTime.EnableWindow(TRUE);				//TIME EDIT
					m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
				}
				else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//RATE �Ƀ`�F�b�N
				{
					m_strDeltafFreq.EnableWindow(TRUE);				//RATE EDIT
					m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
				}
				m_RecoveryOffBtn.SetColor(COLOR_REMOTES);		//�I��   �I�����W
				m_RecoveryOnBtn.SetColor(COLOR_REMOTE);			//��I�� ��
				// �X�V
				// m_RecoveryOffBtn.Invalidate();
				// m_RecoveryOnBtn.Invalidate();
				// ����
				m_RngingStartBtn.EnableWindow(TRUE);			//RANGING START�{�^��
				m_RRngStartBtn.EnableWindow(TRUE);				//�Đ��^ START�{�^��
				// �����E�񊈐�
				m_RecoveryOffBtn.EnableWindow(FALSE);			//RECOVERY OFF
				m_RecoveryOnBtn.EnableWindow(TRUE);				//RECOVERY ON

				// �ĕ`�揈��
				InvalidateRect(NULL, TRUE);
			}
		}
	}

	UpdateData(FALSE);

	// �^�C�}�쐬
	SetTimer(TID_DELTAF_TRS, INTERVAL_DELTAF, 0);
}

/*============================================================================*/
// �^�C�}����
/*============================================================================*/
void CTrsControlXX54::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_DELTAF_TRS)
	{
		SetData();
	}

	CTrsControlXX::OnTimer(nIDEvent);
}

/*============================================================================*/
// �wTX CONTROL�x-�wRF-POWER�x-�wOFF�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRfOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-SSPA.TX_ON/OFF"), _T("OFF"));
	theApp.GetSatelliteData().opctproc(_T("X-TX_IF.TX1_OUTPUT"), _T("OFF"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_off_cntdwn = RF_POWER_TIME_XX_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
// �wTX CONTROL�x-�wRF-POWER�x-�wON�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRfOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RfPwrOffBtn.SetColor(COLOR_CTRLIN);
	m_RfPwrOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-TX_IF.TX1_OUTPUT"), _T("ON"));
	theApp.GetSatelliteData().opctproc(_T("X-SSPA.TX_ON/OFF"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_rf_power_on_cntdwn = RF_POWER_TIME_XX_LIMIT;

	// �X�V
	m_RfPwrOffBtn.Invalidate();
	m_RfPwrOnBtn.Invalidate();
}

/*============================================================================*/
// �wTX CONTROL�x-�wNO-USE�x-�wOFF�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlNouseOffBtn()
{
	COLORREF col = m_NoUseOffBtn.GetColor();

	if (col == COLOR_REMOTE)
	{
		// �����[�g�i��I���j

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("SSPA.RF_CUT_NO_USE"), _T("OFF"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;
	}
	else if (col == COLOR_CTRLIN)
	{
		// ���䒆

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOffBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("SSPA.RF_CUT_NO_USE"), _T("OFF"));

		// �J�E���g�_�E���J�n
		m_l_no_use_off_cntdwn = NO_USE_TIME_XX_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_on_cntdwn = -1;
	}

	// �X�V
	m_NoUseOffBtn.Invalidate();
}

/*============================================================================*/
// �wTX CONTROL�x-�wNO-USE�x-�wON�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlNouseOnBtn()
{
	COLORREF col = m_NoUseOnBtn.GetColor();

	if (col == COLOR_REMOTE)
	{
		// �����[�g�i��I���j

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("SSPA.RF_CUT_NO_USE"), _T("ON"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;
	}
	else if (col == COLOR_CTRLIN)
	{
		// ���䒆

		// �{�^���𐧌䒆�ɂ���
		m_NoUseOnBtn.SetColor(COLOR_CTRLIN);

		// ����ʒm���M
		theApp.GetSatelliteData().opctproc(_T("SSPA.RF_CUT_NO_USE"), _T("ON"));

		// �J�E���g�_�E���J�n
		m_l_no_use_on_cntdwn = NO_USE_TIME_XX_LIMIT;

		// �����̃J�E���g�_�E���I��
		m_l_no_use_off_cntdwn = -1;
	}

	// �X�V
	m_NoUseOnBtn.Invalidate();
}

/*============================================================================*/
// �wTX CONTROL�x-�wSWEEP-CONT�x-�wSTOP�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetStopBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_OFF"));
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
// �wTX CONTROL�x-�wSWEEP-CONT�x-�wINC�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetIncBtn()
{
	if (m_SweepContNorChkBtn.GetCheck() == 1)	// NOR �Ƀ`�F�b�N
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_SAW;
	}
	else
	{
		m_i_main_sweep_cont_mode = SWEEP_MODE_TRI;
	}

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_ON"));

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
// �wTX CONTROL�x-�wSWEEP-CONT�x-�wINCDEC�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetIncdecBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_ON"));

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
// �wTX CONTROL�x-�wSWEEP-CONT�x-�wDEC�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetDecBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_ON"));

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
// �wTX CONTROL�x-�wSWEEP-CONT�x-�wDECINC�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetDecincBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_CARR_SWP"), _T("SWEEP_ON"));

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
// �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wSTOP�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubStopBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_OFF"));
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
// �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wINC�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubIncBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_ON"));

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
// �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wINCDEC�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubIncdecBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_ON"));

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
// �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wDEC�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubDecBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_ON"));

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
// �wTX CONTROL�x-�wSWEEP-CONT(SubCarr)�x-�wDECINC�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlSweetSubDecincBtn()
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
	theApp.GetSatelliteData().opctproc(_T("DSNTCR.TX_SC_SWP"), _T("SWEEP_ON"));

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
// �wRX CONTROL�x-�wX/X-COH�x-�wCOH�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlXxcohCohBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("COH"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_coh_cntdwn = COH_INCOH_TIME_XX_LIMIT;

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}

/*============================================================================*/
// �wRX CONTROL�x-�wX/X-COH�x-�wINCOH�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlXxcohIncohBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_XXCohCohBtn.SetColor(COLOR_CTRLIN);
	m_XXCohInCohBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("V-EQU.COHI"), _T("INCOH"));

	// �J�E���g�_�E���J�n
	m_l_coh_incoh_incoh_cntdwn = COH_INCOH_TIME_XX_LIMIT;

	// �X�V
	m_XXCohCohBtn.Invalidate();
	m_XXCohInCohBtn.Invalidate();
}

/*============================================================================*/
// �wRX CONTROL�x-�wRNGING�x-�wSTOP�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRngingStopBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_RngingStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	m_l_rnging_stop_cntdwn = RNGING_TIME_XX64_LIMIT;

	// RNG��ʃt���O���N���A����
	m_l_rng_type = RNG_TYPE_NON;

	// �X�V
	m_RngingStopBtn.Invalidate();
}

/*============================================================================*/
// �wRX CONTROL�x-�wRNGING�x-�wSTART�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRngingStartBtn()
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
// �wRX CONTROL�x-�w�Đ��^�x-�wSTOP�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRgntypeStopBtn()
{
	// �Đ��^�{�^�����C���Z���V�e�B�u�ɂ���
	m_RRngStopBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("CTRL.RANGING_CANCEL"), _T(","));

	// �J�E���g�_�E���J�n
	m_l_rep_rnging_stop_cntdwn = REP_RNGING_TIME_XX64_LIMIT;

	// RNG��ʃt���O���N���A����
	m_l_rng_type = RNG_TYPE_NON;

	// �X�V
	m_RRngStopBtn.Invalidate();
}

/*============================================================================*/
// �wRX CONTROL�x-�w�Đ��^�x-�wSTART�x�{�^����������
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlRgntypeStartBtn()
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
// ��f�iCTRL�j�`�F�b�N�{�b�N�X
/*============================================================================*/
void CTrsControlXX54::OnBnClickedDeltafCtrlFreqCtrChk()
{
	UpdateData();

	if (m_DeltafCtrlFreqCtrChkBtn)
	{
		// ��f�iCTRL�j���͊���
		m_strDeltafCtrl.EnableWindow(TRUE);
	}
	else
	{
		// ��f�iCTRL�j���͔񊈐�
		m_strDeltafCtrl.EnableWindow(FALSE);
	}
}

/*============================================================================*/
//��f���J�o�� TIME�`�F�b�N�{�b�N�X
/*============================================================================*/
void CTrsControlXX54::OnBnClickedDeltafCtrlTimeChk()
{
	m_DeltafCtrlFreqChkBtn.SetCheck(0);
	m_DeltafCtrlTimeChkBtn.SetCheck(1);
	m_strDeltafTime.EnableWindow(TRUE);				//TIME EDIT
	m_strDeltafFreq.EnableWindow(FALSE);			//RATE EDIT
}

/*============================================================================*/
//��f���J�o�� RATE�`�F�b�N�{�b�N�X
/*============================================================================*/
void CTrsControlXX54::OnBnClickedDeltafCtrlFreqChk()
{
	m_DeltafCtrlFreqChkBtn.SetCheck(1);
	m_DeltafCtrlTimeChkBtn.SetCheck(0);
	m_strDeltafFreq.EnableWindow(TRUE);				//RATE EDIT
	m_strDeltafTime.EnableWindow(FALSE);			//TIME EDIT
}

/*============================================================================*/
//��f���J�o�� ON�{�^��
/*============================================================================*/
void CTrsControlXX54::OnBnClickedRecoveryOnBtn()
{
	// �@���W�I�{�^���ŁuTIME�v���I�����ꂽ�ꍇ
	//   CTRL.DF_START,DFxxxxxxxxx@Tyyyyyyyyy,;
	// �A���W�I�{�^���ŁuRATE�v���I�����ꂽ�ꍇ
	//   CTRL.DF_START,DFxxxxxxxxx@Rzzzzzzzzz,;
	// xxxxxxxxx:��f(����)�`�F�b�N�{�b�N�X��
	//   �`�F�b�N����Ă���ꍇ�́A���̕\���l��ݒ�[Hz]
	//   �`�F�b�N����Ă��Ȃ��ꍇ�́A�u-�v��ݒ�
	// yyyyyyyyy:TIME���̕\���l��ݒ�[s�܂��́AYYYY-MM-DD_hh:mm:ss]
	// zzzzzzzzz:RATE���̕\���l��ݒ�[Hz/s]
	CString deltaFCtrl;	//xxxxxxxxx
	CString ctrlTime;	//yyyyyyyyy
	CString ctrlFreq;	//zzzzzzzzz
	CString strCtrl = _T("");
	BOOL check;

	if (m_DeltafCtrlFreqCtrChkBtn)
	{
		m_strDeltafCtrl.GetWindowText(deltaFCtrl);
		check = CheckFrequencyInput(deltaFCtrl);
		if(check != TRUE)
		{
			CString str;
			str.Format(_T("��f�iCTRL�j��0.0�������}4000000.0�ȓ��A����Βl�ɂ����ꍇ�Ƀ��j�^�l%s�̐�Βl�ȓ�����͂��Ă��������B"), m_DeltafCtrlFreqMoni);
			MessageBox(str, 0, MB_OK | MB_ICONERROR);
			return;
		}
	}
	else
	{
		deltaFCtrl = _T("-");
	}
	strCtrl = _T("CTRL.DF_START,DF") + deltaFCtrl;	//"CTRL.DF_START,DFxxxxxxxxx"

	if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)	//TIME �Ƀ`�F�b�N
	{
		int year, mon, day, hour, min, sec;
		TCHAR sz_time_in[40], sz_time_chk[40];
		int sz = 40;

		m_strDeltafTime.GetWindowText(ctrlTime);
		if (ctrlTime.GetLength() >= sz || wcscpy_s(sz_time_in, sz, (LPCTSTR)ctrlTime) != 0)		// �T�C�Y�i�I�[������'\0'���l���j�`�F�b�N��ɁA������R�s�[
		{
			MessageBox(_T("TIME�̓����w�肪�Ԉ���Ă��܂��B"), 0, MB_OK | MB_ICONERROR);
			return;
		}

		//YYYY-MM-DD hh:mm:ss�`��
		if(swscanf_s(sz_time_in, L"%04d-%02d-%02d %02d:%02d:%02d", &year, &mon, &day, &hour, &min, &sec) == 6)
		{
			if( (year < 2019) || (year > 2099) ||
				(mon  < 1)    || (mon  > 12)   ||
				(day  < 1)    || (day  > 31)   ||
				(hour < 0)    || (hour > 23)   ||
				(min  < 0)    || (min  > 59)   ||
				(sec  < 0)    || (sec  > 59)   )
			{
				MessageBox(_T("TIME�̓����w�肪�Ԉ���Ă��܂��B"), 0, MB_OK | MB_ICONERROR);
				return;
			}
			//YYYY-MM-DD hh:mm:ss �� YYYY-MM-DD_hh:mm:ss �ɕϊ� (�X�y�[�X���A���_�[�o�[�ɕύX)
			swprintf_s(sz_time_chk, sz, L"%04d-%02d-%02d_%02d:%02d:%02d", year, mon, day, hour, min, sec);
			ctrlTime = sz_time_chk;
			//TRACE("�����w��:%s\n", CStringA(ctrlTime));
		}
		//�b�`��
		else if(swscanf_s(sz_time_in, L"%d", &sec) == 1)
		{
			swprintf_s(sz_time_chk, sz, L"%d", sec);
			check = FALSE;
			if(_tcscmp(sz_time_in, sz_time_chk) == 0)
			{
				check = CheckTimeSec(ctrlTime);
			}
			if(check != TRUE)
			{
				MessageBox(_T("TIME��1�`86400[�b]����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
				return;
			}
			//TRACE("�b�w��:%d\n", CStringA(ctrlTime));
		}
		else
		{
			MessageBox(_T("TIME�͓����w�肩�b����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		strCtrl += _T("@T") + ctrlTime + _T(",;");	//"CTRL.DF_START,DFxxxxxxxxx@Tyyyyyyyyy,;"
	}
	else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//RATE �Ƀ`�F�b�N
	{
		m_strDeltafFreq.GetWindowText(ctrlFreq);
		check = CheckFrequencyRate(ctrlFreq);
		if(check != TRUE)
		{
			MessageBox(_T("RATE��0.1�`4000000.0����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		strCtrl += _T("@R") + ctrlFreq + _T(",;");	//"CTRL.DF_START,DFxxxxxxxxx@Rzzzzzzzzz,;"
	}
	else
	{
		MessageBox(_T("RATE��TIME���w�肵�Ă��������B"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	// ON�v�{�^�����N���b�N���ꂽ�ꍇ�A�ȉ��̐��䕶����𐶐������M����B
	//TRACE("%s\n", CStringA(strCtrl));
	theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
}

/*============================================================================*/
//��f���J�o�� OFF�{�^��
/*============================================================================*/
void CTrsControlXX54::OnBnClickedRecoveryOffBtn()
{
	// OFF�v�{�^�����N���b�N���ꂽ�ꍇ�A�ȉ��̐��䕶����𐶐������M����B
	//TRACE("CTRL.DF_STOP,,;\n");
	theApp.SendControlNoUI(_T("CTRL.DF_STOP,,;"), _T(""), _T(""), _T(""));
}

/*============================================================================*/
//TIME(�b) �͈̓`�F�b�N
/*============================================================================*/
BOOL CTrsControlXX54::CheckTimeSec(CString second)
{
	int val;

	val = _wtoi(second);
	if( (val < 1) || (val > 86400) )
	{
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
//RATE �͈̓`�F�b�N
/*============================================================================*/
BOOL CTrsControlXX54::CheckFrequencyRate(CString freq)
{
	double val;

	val = _wtof(freq);
	if( (val < 0.1) || (val > 4000000.0) )
	{
		return FALSE;
	}

	return TRUE;
}

/*============================================================================*/
//��f�iCTRL�j �͈̓`�F�b�N
/*============================================================================*/
BOOL CTrsControlXX54::CheckFrequencyInput(CString freq)
{
	double val, monVal;

	val = _wtof(freq);
	monVal = _wtof(m_DeltafCtrlFreqMoni);
	if ((val < -4000000.0) || (val > 4000000.0) || (val == 0.0) || fabs(val) > fabs(monVal))
	{
		return FALSE;
	}

	return TRUE;
}


/*============================================================================*/
//��f TIME �l�`�F�b�N
/*============================================================================*/
void CTrsControlXX54::OnEnChangeTrxxctrlDeltafTimeEdit()
{
	UpdateData(TRUE);

	CString strDeltafTime, strDeltafTimeChk;
	m_strDeltafTime.GetWindowText(strDeltafTime);
	strDeltafTimeChk = strDeltafTime;

	// �����񂩂琔��0�`9, '-', ':', ���p�󔒂�����΍폜���A�c��������������΋֎~�����L��
	WCHAR szPermitMoji[] = {
		_T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), _T('8'), _T('9'), _T('-'), _T(':'), _T(' ')
	};

	for (int i = 0; i < sizeof(szPermitMoji) / sizeof(szPermitMoji[0]); i++)
	{
		strDeltafTimeChk.Remove(szPermitMoji[i]);
	}

	if (strDeltafTimeChk.IsEmpty() == false)
	{
		// �֎~�������c���Ă��邽�߁A���[�j���O��\�����A���͑O�̒l�ɖ߂�
		CString strMsg;
		strMsg.Format(_T("����[%s]�͓��͂ł��܂���B\nTIME�͓����w�肩�A�����l�̕b����͂��Ă��������B"), strDeltafTimeChk);
		MessageBox(strMsg, 0, MB_OK | MB_ICONERROR);
		m_strDeltafTime.SetWindowTextW(m_strPreDeltafTime);
		UpdateData(FALSE);
	}
	else
	{
		// �֎~�����������ꍇ
		m_strPreDeltafTime = strDeltafTime;
	}
}

/*============================================================================*/
/*! CTrsControlXX

-# �wTX CONTROL�x-�wHOLD-TONE�x-�wOFF�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrsControlXX54::OnBnClickedTrxxctrlHoldOffBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-CMDOUT.HOLD_TONE"), _T("OFF"));

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
void CTrsControlXX54::OnBnClickedTrxxctrlHoldOnBtn()
{
	// �{�^���𐧌䒆�ɂ���
	m_HoldToneOffBtn.SetColor(COLOR_CTRLIN);
	m_HoldToneOnBtn.SetColor(COLOR_CTRLIN);

	// ����ʒm���M
	theApp.GetSatelliteData().opctproc(_T("X-CMDOUT.HOLD_TONE"), _T("ON"));

	// �J�E���g�_�E���J�n
	m_l_hold_tone_on_cntdwn = HOLD_TONE_TIME_XX_LIMIT;

	// �X�V
	m_HoldToneOffBtn.Invalidate();
	m_HoldToneOnBtn.Invalidate();
}
