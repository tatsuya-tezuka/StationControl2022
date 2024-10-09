/*============================================================================*/
/*! FrequencyCtrlCtrl54.cpp

-# ���g��������-�ʐݒ�^�u(54m)
*/
/*============================================================================*/
// FrequencyCtrlCtrl54.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlCtrl54.h"
#include "afxdialogex.h"


// CFrequencyCtrlCtrl54 �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlCtrl54, CFrequencyCtrlCtrl)

CFrequencyCtrlCtrl54::CFrequencyCtrlCtrl54(CWnd* pParent /*=NULL*/)
: CFrequencyCtrlCtrl(CFrequencyCtrlCtrl54::IDD, pParent)
{

}

CFrequencyCtrlCtrl54::~CFrequencyCtrlCtrl54()
{

}

void CFrequencyCtrlCtrl54::DoDataExchange(CDataExchange* pDX)
{
	CFrequencyCtrlCtrl::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_SET_UC_EDIT, m_strUC);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_SET_DC_EDIT, m_strXDC);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_SET_CMD_EDIT, m_strCmdCareer);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT, m_strTFreqOffset);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, m_strTimeOffset);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_TD_CMB, m_TDopplerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_RD_CMB, m_RDopplerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DP_CMD_CMB, m_DopplerCmdCareerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_SCM_CMB, m_SpacecraftCOHModeCtrl);
	DDX_Text(pDX, IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT2, m_strRFreqOffset);
	DDX_Text(pDX, IDC_STATIC_FREQ_CTRL_DELTAF_MON, m_DeltafCtrlFreqMoni);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DELTAF_CTRL_EDIT, m_strDeltafCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_DELTAF_CTRLFREQ_CHECK, m_DeltafCtrlFreqChkBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DEFTAF_CTRLFREQ_EDIT, m_strDeltafFreq);
	DDX_Control(pDX, IDC_FREQ_CTRL_DELTAF_CTRLTIME_CHECK, m_DeltafCtrlTimeChkBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DELTAF_CTRLTIME_EDIT, m_strDeltafTime);
	DDX_Text(pDX, IDC_STATIC_FREQ_CTRL_DELTAF_ENDTIME, m_DeltafCtrlFreqEndTime);
	DDX_Check(pDX, IDC_FREQ_CTRL_DELTAF_CTRL_CHECK, m_DeltafCtrlFreqCtrChkBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_DELTAF_CTRL_CHECK, m_DeltafCtrlChkBtnCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DELTAF_START_BTN, m_DeltafStartBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_DELTAF_STOP_BTN, m_DeltafStopBtn);
	DDX_Control(pDX, IDC_FREQ_CTRL_DC_COH_CMB, m_DcCOHModeCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_CTRL_SET_KADC_EDIT, m_strKaDC);
}

BEGIN_MESSAGE_MAP(CFrequencyCtrlCtrl54, CFrequencyCtrlCtrl)
	ON_BN_CLICKED(IDC_FREQ_CTRL_DELTAF_CTRL_CHECK, &CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlFreqCtrChk)
	ON_BN_CLICKED(IDC_FREQ_CTRL_DELTAF_CTRLFREQ_CHECK, &CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlFreqChk)
	ON_BN_CLICKED(IDC_FREQ_CTRL_DELTAF_CTRLTIME_CHECK, &CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlTimeChk)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DELTAF_START_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlStartBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DELTAF_STOP_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlStopBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SCM_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlScmBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_CMD_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetCmdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_DC_COH_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDcCOHBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_UC_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetUcBtn)
	ON_CBN_SELCHANGE(IDC_FREQ_CTRL_DC_COH_CMB, &CFrequencyCtrlCtrl54::OnCbnSelchangeFreqCtrlDcCohCmb)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TF_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsTfBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TF_BTN2, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsTfBtn2)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_OFS_TO_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsToBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_TD_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpTdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_RD_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpRdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_DP_CMD_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpCmdBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_SET_CMD_TIME_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetCmdTimeBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_CTRL_BTN, &CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlBtn)
	ON_EN_CHANGE(IDC_FREQ_CTRL_CTRL_DELTAF_CTRLTIME_EDIT, &CFrequencyCtrlCtrl54::OnEnChangeFreqCtrlCtrlDeltafCtrltimeEdit)
	ON_EN_CHANGE(IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, &CFrequencyCtrlCtrl54::OnEnChangeFreqCtrlCtrlOfsToEdit)
END_MESSAGE_MAP()

/*============================================================================*/
// �_�C�A���O����������
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::OnInitDialog()
{
	CFrequencyCtrlCtrl::OnInitDialog();

	// D/C COH���[�h
	for (int i = 0; i < sizeof(mDcCOHMode) / sizeof(mDcCOHMode[0]); i++){
		m_DcCOHModeCtrl.AddString(mDcCOHMode[i]);
	}
	m_DcCOHModeCtrl.SetCurSel(1);	//X-INCOH
	m_strXDC.EnableWindow(TRUE);
	m_strKaDC.EnableWindow(FALSE);

	// ��f���J�o��
	m_DeltafStopBtn.EnableWindow(FALSE);		//��~�{�^��
	m_DeltafCtrlFreqCtrChkBtn = FALSE;			//��f�i����j�`�F�b�N�{�b�N�X
	m_strDeltafCtrl.EnableWindow(FALSE);		//��f�i����j���͔񊈐�
	m_strDeltafFreq.SetWindowText(_T("0.1"));	//������g��
	m_strPreDeltafTime = _T("1");
	m_strDeltafTime.SetWindowText(m_strPreDeltafTime);		//���䎞��
	m_strDeltafFreq.EnableWindow(FALSE);		//������g�� EDIT
	m_strDeltafTime.EnableWindow(FALSE);		//���䎞�� EDIT
	m_RecoveryCont = FALSE;
	m_RngFlg = FALSE;
	m_strDeltafCtrlInit = FALSE;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
// �_�C�A���O�X�V
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::UpdateDlg()
{
	// ��ʍX�V
	GetData();

	return TRUE;
}

/*============================================================================*/
// ��ʍX�V
/*============================================================================*/
void CFrequencyCtrlCtrl54::GetData()
{
	CFrequencyCtrlCtrl54::stGetMonData mMonTable[] = {
		{ _T("CTRL.DELTA_FREQ"),       m_DeltafCtrlFreqMoni,    _T("%0.1f"), eDOUBLE },	// ��f�i���j�^�j
		{ _T("CTRL.RECOVER_END_TIME"), m_DeltafCtrlFreqEndTime, _T("%s"),    eSTRING },	// �I���\�莞��
		{ _T("CTRL.RECOVER_CONT"),     m_strRecoverCont,        _T("%s"),    eSTRING },	// RECOVERY OFF/ON
		{ _T("CTRL.RNG_FLG"),          m_strRngFlg,             _T("%s"),    eSTRING },	// Range�v���� �I��/�J�n
	};

	// �^�C�}�j��
	KillTimer(TID_DELTAF_FREQ);

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
				m_strDeltafCtrl.SetWindowText(m_DeltafCtrlFreqMoni);	//��f(����)�̏����l�́A�Ď��f�[�^�u��f�v�̒l
				m_strDeltafCtrlInit = TRUE;								//�����l�ݒ��
			}
		}
		else
		{
			//TRACE("GetObsname_t()�G���[ �Ď���:%s\n", CStringA(mMonTable[index].monname));
		}
	}

//	//RECOVERY:ON or Range�v��:�J�n �Ȃ�{�^������͂�񊈐���
//	if( (m_strRecoverCont == _T("ON")) || (m_strRngFlg == _T("�J�n")) )
//	{
//		//=====================================================//
//		//������������������������ Log ������������������������//
//		CString strDebug;
//		strDebug.Format(_T("Range Keisoku is on. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
//						   theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
//						   (LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
//		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
//		//������������������������ Log ������������������������//
//		//=====================================================//
//
//		if (m_RecoveryCont != TRUE)
//		{
//			//=====================================================//
//			//������������������������ Log ������������������������//
//			CString strDebug;
//			strDebug.Format(_T("Range Keisoku is on. RecoveryCont is false. station:%u"), theApp.GetSelectStation());
//			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
//			//������������������������ Log ������������������������//
//			//=====================================================//
//
//			m_RecoveryCont = TRUE;
//			m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//��f (����) CHECK
//			m_strDeltafCtrl.EnableWindow(FALSE);			//��f (����) EDIT
//			m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//������g�� CHECK
//			m_strDeltafFreq.EnableWindow(FALSE);			//������g�� EDIT
//			m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//���䎞�� CHECK
//			m_strDeltafTime.EnableWindow(FALSE);			//���䎞�� EDIT
//			m_DeltafStartBtn.EnableWindow(FALSE);			//�J�n
//			m_DeltafStopBtn.EnableWindow(TRUE);				//��~
//
//			// �ĕ`�揈��
//			InvalidateRect(NULL, TRUE);
//		}
//	}
//	//RECOVERY:OFF and Range�v��:�I�� �Ȃ�{�^������͂�������
//	else
//	{
//		//=====================================================//
//		//������������������������ Log ������������������������//
//		CString strDebug;
//		strDebug.Format(_T("Range Keisoku is off. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
//						theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
//						(LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
//		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
//		//������������������������ Log ������������������������//
//		//=====================================================//
//
//		if(m_RecoveryCont == TRUE)
//		{
//			//=====================================================//
//			//������������������������ Log ������������������������//
//			CString strDebug;
//			strDebug.Format(_T("Range Keisoku is off. RecoveryCont is true. station:%u"), theApp.GetSelectStation());
//			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
//			//������������������������ Log ������������������������//
//			//=====================================================//
//
//			m_RecoveryCont = FALSE;
//			m_DeltafCtrlChkBtnCtrl.EnableWindow(TRUE);		//��f (����) CHECK
//			m_strDeltafCtrl.EnableWindow(TRUE);				//��f (����) EDIT
//			m_DeltafCtrlFreqChkBtn.EnableWindow(TRUE);		//������g�� CHECK
//			m_DeltafCtrlTimeChkBtn.EnableWindow(TRUE);		//���䎞�� CHECK
//			if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)			//���䎞�� �Ƀ`�F�b�N
//			{
//				m_strDeltafTime.EnableWindow(TRUE);				//���䎞�� EDIT
//				m_strDeltafFreq.EnableWindow(FALSE);			//������g�� EDIT
//			}
//			else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//������g�� �Ƀ`�F�b�N
//			{
//				m_strDeltafFreq.EnableWindow(TRUE);				//������g�� EDIT
//				m_strDeltafTime.EnableWindow(FALSE);			//���䎞�� EDIT
//			}
//			m_DeltafStartBtn.EnableWindow(TRUE);			//�J�n
//			m_DeltafStopBtn.EnableWindow(FALSE);			//��~
//
//			// �ĕ`�揈��
//			InvalidateRect(NULL, TRUE);
//		}
//	}

	//Range�v��:�J�n�Ȃ�{�^������͂�S���񊈐���
	if (m_strRngFlg == _T("�J�n"))
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CString strDebug;
		strDebug.Format(_T("Range Keisoku is on. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
			theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
			(LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
		//������������������������ Log ������������������������//
		//=====================================================//

		// ���O�������W�v�����łȂ������ꍇ�A�`��ύX
		if (m_RngFlg != TRUE)
		{
			//=====================================================//
			//������������������������ Log ������������������������//
			CString strDebug;
			strDebug.Format(_T("Range Keisoku is on. RecoveryCont is false. station:%u"), theApp.GetSelectStation());
			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
			//������������������������ Log ������������������������//
			//=====================================================//

			// �����W�v�����t���O�Z�b�g
			m_RngFlg = TRUE;
			m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//��f (����) CHECK
			m_strDeltafCtrl.EnableWindow(FALSE);			//��f (����) EDIT
			m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//������g�� CHECK
			m_strDeltafFreq.EnableWindow(FALSE);			//������g�� EDIT
			m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//���䎞�� CHECK
			m_strDeltafTime.EnableWindow(FALSE);			//���䎞�� EDIT
			m_DeltafStartBtn.EnableWindow(FALSE);			//�J�n
			m_DeltafStopBtn.EnableWindow(FALSE);			//��~

			// �ĕ`�揈��
			InvalidateRect(NULL, TRUE);
		}
	}
	//Range�v��:�J�n�ȊO
	else
	{
		//RECOVERY:ON�Ȃ��~�{�^���̂݊�����
		if (m_strRecoverCont == _T("ON"))
		{
			//=====================================================//
			//������������������������ Log ������������������������//
			CString strDebug;
			strDebug.Format(_T("Range Keisoku is on. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
				theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
				(LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
			//������������������������ Log ������������������������//
			//=====================================================//

			// ���O�������W�v�����A�܂��̓��J�o�����łȂ������ꍇ�A�`��ύX
			if (m_RngFlg == TRUE || m_RecoveryCont != TRUE)
			{
				//=====================================================//
				//������������������������ Log ������������������������//
				CString strDebug;
				strDebug.Format(_T("Range Keisoku is on. RecoveryCont is false. station:%u"), theApp.GetSelectStation());
				CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
				//������������������������ Log ������������������������//
				//=====================================================//

				// �����W�v�����t���O�N���A
				m_RngFlg = FALSE;
				// ���J�o���[�t���O�Z�b�g
				m_RecoveryCont = TRUE;
				m_DeltafCtrlChkBtnCtrl.EnableWindow(FALSE);		//��f (����) CHECK
				m_strDeltafCtrl.EnableWindow(FALSE);			//��f (����) EDIT
				m_DeltafCtrlFreqChkBtn.EnableWindow(FALSE);		//������g�� CHECK
				m_strDeltafFreq.EnableWindow(FALSE);			//������g�� EDIT
				m_DeltafCtrlTimeChkBtn.EnableWindow(FALSE);		//���䎞�� CHECK
				m_strDeltafTime.EnableWindow(FALSE);			//���䎞�� EDIT
				m_DeltafStartBtn.EnableWindow(FALSE);			//�J�n
				m_DeltafStopBtn.EnableWindow(TRUE);				//��~

				// �ĕ`�揈��
				InvalidateRect(NULL, TRUE);
			}
		}
		//RECOVERY:OFF�Ȃ��~�{�^���ȊO�Ɠ��͂��������B
		else
		{
			//=====================================================//
			//������������������������ Log ������������������������//
			CString strDebug;
			strDebug.Format(_T("Range Keisoku is off. station:%u, %s:%s, %s:%s, RecoveryContFlag:%d"),
				theApp.GetSelectStation(), (LPCTSTR)mMonTable[2].monname, (LPCTSTR)m_strRecoverCont,
				(LPCTSTR)mMonTable[3].monname, (LPCTSTR)m_strRngFlg, m_RecoveryCont);
			CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
			//������������������������ Log ������������������������//
			//=====================================================//

			// ���O�������W�v�����A�܂��̓��J�o���[���������ꍇ�A�`��ύX
			if (m_RngFlg == TRUE || m_RecoveryCont == TRUE)
			{
				//=====================================================//
				//������������������������ Log ������������������������//
				CString strDebug;
				strDebug.Format(_T("Range Keisoku is off. RecoveryCont is true. station:%u"), theApp.GetSelectStation());
				CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlCtrl54"), _T("GetData"), _T("debug"), strDebug, nLogEx::debug);
				//������������������������ Log ������������������������//
				//=====================================================//

				// �����W�v�����t���O�N���A
				m_RngFlg = FALSE;
				// ���J�o�����t���O�N���A
				m_RecoveryCont = FALSE;
				m_DeltafCtrlChkBtnCtrl.EnableWindow(TRUE);		//��f (����) CHECK
				m_strDeltafCtrl.EnableWindow(TRUE);				//��f (����) EDIT
				m_DeltafCtrlFreqChkBtn.EnableWindow(TRUE);		//������g�� CHECK
				m_DeltafCtrlTimeChkBtn.EnableWindow(TRUE);		//���䎞�� CHECK
				if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)			//���䎞�� �Ƀ`�F�b�N
				{
					m_strDeltafTime.EnableWindow(TRUE);				//���䎞�� EDIT
					m_strDeltafFreq.EnableWindow(FALSE);			//������g�� EDIT
				}
				else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//������g�� �Ƀ`�F�b�N
				{
					m_strDeltafFreq.EnableWindow(TRUE);				//������g�� EDIT
					m_strDeltafTime.EnableWindow(FALSE);			//���䎞�� EDIT
				}
				m_DeltafStartBtn.EnableWindow(TRUE);			//�J�n
				m_DeltafStopBtn.EnableWindow(FALSE);			//��~

				// �ĕ`�揈��
				InvalidateRect(NULL, TRUE);
			}
		}
	}

	UpdateData(FALSE);

	// �^�C�}�쐬
	SetTimer(TID_DELTAF_FREQ, INTERVAL_DELTAF, 0);
}

/*============================================================================*/
// �T���@COH���[�h �ݒ�{�^������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlScmBtn()
{
	SetData(eControl_COH);
}

/*============================================================================*/
// D/C-COH���[�h �ݒ�{�^������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDcCOHBtn()
{
	SetData(eControl_DC_COH);
}

/*============================================================================*/
// �ݒ���g��-U/C�AX-D/C �AKa-D/C �ݒ�{�^����������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetUcBtn()
{
	bool bUC = SetData(eControl_UC, ePhase_chk);
	bool bX_UC = SetData(eControl_X_DC, ePhase_chk);
	bool bKa_DC = SetData(eControl_Ka_DC, ePhase_chk);

	if (bUC && bX_UC && bKa_DC)
	{
		// ���̓`�F�b�N��OK�Ȃ�Α��M
		SetData(eControl_UC, ePhase_send);
		SetData(eControl_X_DC, ePhase_send);
		SetData(eControl_Ka_DC, ePhase_send);
	}
}

/*============================================================================*/
// �ݒ���g�� - �R�}���h�L�����A �ݒ�{�^����������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetCmdBtn()
{
	SetData(eControl_CS);
}

/*============================================================================*/
// �I�t�Z�b�g-���M���g���I�t�Z�b�g �ݒ�{�^����������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsTfBtn()
{
	SetData(eControl_TOFFSET);
}

/*============================================================================*/
// �I�t�Z�b�g-��M���g���I�t�Z�b�g �ݒ�{�^����������
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsTfBtn2()
{
	SetData(eControl_ROFFSET);
}

/*============================================================================*/
// �I�t�Z�b�g-�����I�t�Z�b�g �ݒ�{�^����������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlOfsToBtn()
{
	SetData(eControl_TIMEOFFSET);
}

/*============================================================================*/
// �h�b�v���⏞-���M�h�b�v�� �ݒ�{�^������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpTdBtn()
{
	SetData(eControl_TDP);
}

/*============================================================================*/
// �h�b�v���⏞-��M�h�b�v�� �ݒ�{�^������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpRdBtn()
{
	SetData(eControl_RDP);
}

/*============================================================================*/
// �h�b�v���⏞-�R�}���h�L�����A �ݒ�{�^������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlDpCmdBtn()
{
	SetData(eControl_CDP);
}

/*============================================================================*/
// �R�}���h�T�u�L�����A-���ݎ����{�^����������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlSetCmdTimeBtn()
{
	SetData(eControl_TIMELOCK);
}

/*============================================================================*/
// �ꊇ�ݒ�{�^����������
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlCtrlBtn()
{
	CFrequencyCtrlCtrl54::SetData(eControl_ALLSET);
}

/*============================================================================*/
// �ݒ�{�^������
/*============================================================================*/
void CFrequencyCtrlCtrl54::SetData(int type)
{
	if (SetData(type, ePhase_chk))
	{
		// ���̓`�F�b�N��OK�Ȃ�Α��M
		SetData(type, ePhase_send);
	}
}

/*============================================================================*/
// �ݒ�{�^������
/*============================================================================*/
bool CFrequencyCtrlCtrl54::SetData(int type, int phase)
{
	CFrequencyCtrlCtrl54::stSetCtlData54 mCtlTable[] = {
		{ _T("DSNTCR.TRP_RATIO"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_SCM_CMB, eCOMBO },								// �T���@COH���[�h
		{ _T("X-DC.COH_INCOH"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_DC_COH_CMB, eCOMBO },									// D/C COH���[�h
		{ _T("X-UC.FREQUENCY"), _T("X-SSPA.TX_FREQUENCY"), _T("X-DC.X-TX_FREQUENCY"), _T("CTRL.TX_DOPP_ZERO"), _T("U/C,SSPA,X-D/C_TX"), IDC_FREQ_CTRL_CTRL_SET_UC_EDIT, eEDIT },			// U/C, SSPA, X-D/C_TX
		{ _T("X-DC.X-RX_FREQUENCY"), _T("CTRL.RX_DOPP_ZERO"), _T(""), _T(""), _T("X-D/C_RX"), IDC_FREQ_CTRL_CTRL_SET_DC_EDIT, eEDIT },		// X-D/C_RX
		{ _T("X-DC.Ka-RX_FREQUENCY"), _T("CTRL.RX_DOPP_ZERO"), _T(""), _T(""), _T("Ka-D/C"), IDC_FREQ_CTRL_CTRL_SET_KADC_EDIT, eEDIT },	// Ka-D/C
		{ _T("DSNTCR.TX_SC_FREQ"), _T("DSNTCR.TX_SC_DOPP_PRE"), _T(""), _T(""), _T("�R�}���h�T�u�L�����A"), IDC_FREQ_CTRL_CTRL_SET_CMD_EDIT, eEDIT },	// �R�}���h�T�u�L�����A
		{ _T("CTRL.CMD_SC_NOW"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_SET_CMD_TIME_BTN, eNONE },						// ���ݎ����x�X�g���b�N
		{ _T("CTRL.DOPP_MODE"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_DP_TD_CMB, eCOMBO },								// ���M�h�b�v��
		{ _T("XX-DSNTCRFC.1WAY/2WAY"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_DP_RD_CMB, eCOMBO },						// ��M�h�b�v��
		{ _T("CTRL.SC_DOPP_MODE"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_DP_CMD_CMB, eCOMBO },							// �R�}���h�T�u�L�����A
		{ _T("XX-DSNTCRFC.FREQ_OFFSET_U"), _T(""), _T(""), _T(""), _T("���M���g���I�t�Z�b�g"), IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT, eEDIT },					// ���M���g���I�t�Z�b�g
		{ _T("XX-DSNTCRFC.FREQ_OFFSET"), _T(""), _T(""), _T(""), _T("��M���g���I�t�Z�b�g"), IDC_FREQ_CTRL_CTRL_OFS_TF_EDIT2, eEDIT },					// ��M���g���I�t�Z�b�g
		{ _T("XX-DSNTCRFC.TIME_OFFSET"), _T(""), _T(""), _T(""), _T(""), IDC_FREQ_CTRL_CTRL_OFS_TO_EDIT, eEDIT },					// �����I�t�Z�b�g
	};

	const CString strTimeOffsetErrMsg = _T("�����I�t�Z�b�g�̕��b�w��(�}M:SS)���Ԉ���Ă��܂��B");

	UpdateData(TRUE);

	/****************************************************************/
	/* ������ */
	CString ctrl, val;
	bool bRet = true;
	bool bAll = false;
	if (type == eControl_ALLSET)
		bAll = true;

	for (int item = 0; item < eControl_ALLSET; item++){
		if (bAll == true){
			type = item;
		}
		for (int i = 0; i < 4; i++){
			bool bRangeChk = false;
			switch (type){
			case	eControl_COH:
				ctrl = mCtlTable[type].ctlname1;
				val = mSatMode[m_SpacecraftCOHModeCtrl.GetCurSel()];
				i = 4;
				break;
			case	eControl_DC_COH:
				ctrl = mCtlTable[type].ctlname1;
				val = mDcCOHModeCtl[m_DcCOHModeCtrl.GetCurSel()];	//�\�����Ɛ��䖼�͈Ⴄ
				i = 4;
				break;
			case	eControl_UC:
				switch (i)
				{
				case 0:
					ctrl = mCtlTable[type].ctlname1;
					val = m_strUC;
					bRangeChk = true;								//�͈͒l�`�F�b�N
					break;
				case 1:
					ctrl = mCtlTable[type].ctlname2;
					val = m_strUC;
					bRangeChk = true;								//�͈͒l�`�F�b�N
					break;
				case 2:
					ctrl = mCtlTable[type].ctlname3;
					val = m_strUC;
					bRangeChk = true;								//�͈͒l�`�F�b�N
					break;
				case 3:
					ctrl = mCtlTable[type].ctlname4;
					val = _T(",");
					break;
				default:
					break;
				}
				break;
			case	eControl_X_DC:
				val = mDcCOHMode[m_DcCOHModeCtrl.GetCurSel()];
				if (val == _T("X-INCOH"))
				{
					if (i == 0){
						ctrl = mCtlTable[type].ctlname1;
						m_strXDC.GetWindowText(val);
						bRangeChk = true;								//�͈͒l�`�F�b�N
					}
					else{
						ctrl = mCtlTable[type].ctlname2;
						val = _T(",");
						i = 4;
					}
				}
				else
				{
					continue;	//X-INCOH�ȊO�͗v���𑗐M���Ȃ�
				}
				break;
			case	eControl_Ka_DC:
				val = mDcCOHMode[m_DcCOHModeCtrl.GetCurSel()];
				if (val == _T("Ka-INCOH"))
				{
					if (i == 0){
						ctrl = mCtlTable[type].ctlname1;
						m_strKaDC.GetWindowText(val);
						bRangeChk = true;								//�͈͒l�`�F�b�N
					}
					else{
						ctrl = mCtlTable[type].ctlname2;
						val = _T(",");
						i = 4;
					}
				}
				else if (val != _T("X-INCOH"))	//X-INCOH�ł�Ka-INCOH�ł��Ȃ��ꍇ�́A"CTRL.RX_DOPP_ZERO"�̂ݑ��M
				{
					ctrl = mCtlTable[type].ctlname2;
					val = _T(",");
					i = 4;
				}
				else
				{
					continue;	//Ka-INCOH�ȊO�͗v���𑗐M���Ȃ�
				}
				break;
			case	eControl_CS:
				if (i == 0){
					ctrl = mCtlTable[type].ctlname1;
					val = m_strCmdCareer;
					bRangeChk = true;								//�͈͒l�`�F�b�N
				}
				else{
					ctrl = mCtlTable[type].ctlname2;
					val = _T("OFF");
					i = 4;
				}
				break;
			case	eControl_TIMELOCK:
				ctrl = mCtlTable[type].ctlname1;
				val = _T(",");
				i = 4;
				break;
			case	eControl_TDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mCmdCtrl[m_TDopplerCtrl.GetCurSel()];
				i = 4;
				break;
			case	eControl_RDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mRxDopp[m_RDopplerCtrl.GetCurSel()];
				i = 4;
				break;
			case	eControl_CDP:
				ctrl = mCtlTable[type].ctlname1;
				val = mCmdCtrl[m_DopplerCmdCareerCtrl.GetCurSel()];
				i = 4;
				break;
			case	eControl_TOFFSET:
				ctrl = mCtlTable[type].ctlname1;
				val = m_strTFreqOffset;
				bRangeChk = true;								//�͈͒l�`�F�b�N
				i = 4;
				break;
			case	eControl_ROFFSET:
				ctrl = mCtlTable[type].ctlname1;
				val = m_strRFreqOffset;
				bRangeChk = true;								//�͈͒l�`�F�b�N
				i = 4;
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

					if (phase == ePhase_chk)
					{
						// ���䑗�M�O�`�F�b�N�t�F�[�Y
						if (bFormatErr)
						{
							// �t�H�[�}�b�g�G���[�Ȃ�ΐ��䑗�M���Ȃ�
							MessageBox(strTimeOffsetErrMsg, 0, MB_OK | MB_ICONERROR);
							i = 4;
							bRet = false;
							continue;
						}
					}

					swprintf_s(sz_time_chk, sz, L"%s%01d:%02d", (LPCTSTR)strSign, min, sec);
					m_strTimeOffset = sz_time_chk;
					//TRACE("�����w��:%s\n", CStringA(m_strTimeOffset));
				}
				val = m_strTimeOffset;
				i = 4;
				break;
			case	eControl_ALLSET:
				break;
			}
			
			if (phase == ePhase_chk)
			{
				// ���䑗�M�O�`�F�b�N�t�F�[�Y
				CString title = mCtlTable[type].title;
				if (bRangeChk == true && CheckCtrlValRange(ctrl, val, title) == false)
				{
					// ���䑗�M���Ȃ�
					i = 4;
					bRet = false;
					continue;
				}
			}
			else
			{
				// ���䑗�M�t�F�[�Y
				TRACE("[%s] [%s]\n", CStringA(ctrl), CStringA(val));
				theApp.SendControl(ctrl, val, _T(""), _T(""));
			}
		}
		if (bAll == false)
			break;
	}

	UpdateData(FALSE);

	return bRet;
}

/*============================================================================*/
// �^�C�}����
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_DELTAF_FREQ)
	{
		GetData();
	}
	CDialogBase::OnTimer(nIDEvent);
}

/*============================================================================*/
// CFrequencyCtrlCtrl54 ���b�Z�[�W �n���h���[
/*============================================================================*/

/*============================================================================*/
// ��f�i����j�`�F�b�N�{�b�N�X
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlFreqCtrChk()
{
	UpdateData();

	if(m_DeltafCtrlFreqCtrChkBtn)
	{
		// ��f�i����j���͊���
		m_strDeltafCtrl.EnableWindow(TRUE);
	}
	else
	{
		// ��f�i����j���͔񊈐�
		m_strDeltafCtrl.EnableWindow(FALSE);
	}
}

/*============================================================================*/
//��f���J�o�� ������g���`�F�b�N�{�b�N�X
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlFreqChk()
{
	m_DeltafCtrlFreqChkBtn.SetCheck(1);
	m_DeltafCtrlTimeChkBtn.SetCheck(0);
	m_strDeltafFreq.EnableWindow(TRUE);				//������g�� EDIT
	m_strDeltafTime.EnableWindow(FALSE);			//���䎞�� EDIT
}

/*============================================================================*/
//��f���J�o�� ���䎞�ԃ`�F�b�N�{�b�N�X
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedDeltafCtrlTimeChk()
{
	m_DeltafCtrlFreqChkBtn.SetCheck(0);
	m_DeltafCtrlTimeChkBtn.SetCheck(1);
	m_strDeltafTime.EnableWindow(TRUE);				//���䎞�� EDIT
	m_strDeltafFreq.EnableWindow(FALSE);			//������g�� EDIT
}

/*============================================================================*/
//��f���J�o�� �J�n�{�^��
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlStartBtn()
{
	// �@���W�I�{�^���Łu���䎞�ԁv���I�����ꂽ�ꍇ
	//   CTRL.DF_START,DFxxxxxxxxx@Tyyyyyyyyy,;
	// �A���W�I�{�^���Łu������g���v���I�����ꂽ�ꍇ
	//   CTRL.DF_START,DFxxxxxxxxx@Rzzzzzzzzz,;
	// xxxxxxxxx:��f(����)�`�F�b�N�{�b�N�X��
	//   �`�F�b�N����Ă���ꍇ�́A���̕\���l��ݒ�[Hz]
	//   �`�F�b�N����Ă��Ȃ��ꍇ�́A�u-�v��ݒ�
	// yyyyyyyyy:���䎞�ԗ��̕\���l��ݒ�[s�܂��́AYYYY-MM-DD_hh:mm:ss]
	// zzzzzzzzz:������g�����̕\���l��ݒ�[Hz/s]
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
			str.Format(_T("��f�i����j��0.0�������}4000000.0�ȓ��A����Βl�ɂ����ꍇ�Ƀ��j�^�l%s�̐�Βl�ȓ�����͂��Ă��������B"), m_DeltafCtrlFreqMoni);
			MessageBox(str, 0, MB_OK | MB_ICONERROR);
			return;
		}
	}
	else
	{
		deltaFCtrl = _T("-");
	}
	strCtrl = _T("CTRL.DF_START,DF") + deltaFCtrl;	//"CTRL.DF_START,DFxxxxxxxxx"

	if (m_DeltafCtrlTimeChkBtn.GetCheck() == 1)	//���䎞�� �Ƀ`�F�b�N
	{
		int year, mon, day, hour, min, sec;
		TCHAR sz_time_in[40], sz_time_chk[40];
		int sz = 40;

		m_strDeltafTime.GetWindowText(ctrlTime);
		if (ctrlTime.GetLength() >= sz || wcscpy_s(sz_time_in, sz, (LPCTSTR)ctrlTime) != 0)		// �T�C�Y�i�I�[������'\0'���l���j�`�F�b�N��ɁA������R�s�[
		{
			MessageBox(_T("���䎞�Ԃ̓����w�肪�Ԉ���Ă��܂��B"), 0, MB_OK | MB_ICONERROR);
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
				MessageBox(_T("���䎞�Ԃ̓����w�肪�Ԉ���Ă��܂��B"), 0, MB_OK | MB_ICONERROR);
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
				MessageBox(_T("���䎞�Ԃ�1�`86400[�b]����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
				return;
			}
			//TRACE("�b�w��:%d\n", CStringA(ctrlTime));
		}
		else
		{
			MessageBox(_T("���䎞�Ԃ͓����w�肩�b����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		strCtrl += _T("@T") + ctrlTime + _T(",;");	//"CTRL.DF_START,DFxxxxxxxxx@Tyyyyyyyyy,;"
	}
	else if (m_DeltafCtrlFreqChkBtn.GetCheck() == 1)	//������g�� �Ƀ`�F�b�N
	{
		m_strDeltafFreq.GetWindowText(ctrlFreq);
		check = CheckFrequencyRate(ctrlFreq);
		if(check != TRUE)
		{
			MessageBox(_T("������g����0.1�`4000000.0����͂��Ă��������B"), 0, MB_OK | MB_ICONERROR);
			return;
		}
		strCtrl += _T("@R") + ctrlFreq + _T(",;");	//"CTRL.DF_START,DFxxxxxxxxx@Rzzzzzzzzz,;"
	}
	else
	{
		MessageBox(_T("������g�������䎞�Ԃ��w�肵�Ă��������B"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	// �J�n�v�{�^�����N���b�N���ꂽ�ꍇ�A�ȉ��̐��䕶����𐶐������M����B
	//TRACE("%s\n", CStringA(strCtrl));
	theApp.SendControlNoUI(strCtrl, _T(""), _T(""), _T(""));
}

/*============================================================================*/
//��f���J�o�� ��~�{�^��
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnBnClickedFreqCtrlStopBtn()
{
	// ��~�v�{�^�����N���b�N���ꂽ�ꍇ�A�ȉ��̐��䕶����𐶐������M����B
	//TRACE("CTRL.DF_STOP,,;\n");
	theApp.SendControlNoUI(_T("CTRL.DF_STOP,,;"), _T(""), _T(""), _T(""));
}

/*============================================================================*/
//D/C COH���[�h �R���{�{�b�N�X
/*============================================================================*/
void CFrequencyCtrlCtrl54::OnCbnSelchangeFreqCtrlDcCohCmb()
{
	CString val;

	val = mDcCOHMode[m_DcCOHModeCtrl.GetCurSel()];
	if(val == _T("X-INCOH"))
	{
		m_strXDC.EnableWindow(TRUE);
	}
	else
	{
		m_strXDC.EnableWindow(FALSE);
	}
	if(val == _T("Ka-INCOH"))
	{
		m_strKaDC.EnableWindow(TRUE);
	}
	else
	{
		m_strKaDC.EnableWindow(FALSE);
	}
}

/*============================================================================*/
//���䎞��(�b) �͈̓`�F�b�N
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::CheckTimeSec(CString second)
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
//������g�� �͈̓`�F�b�N
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::CheckFrequencyRate(CString freq)
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
//��f�i����j �͈̓`�F�b�N
/*============================================================================*/
BOOL CFrequencyCtrlCtrl54::CheckFrequencyInput(CString freq)
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



void CFrequencyCtrlCtrl54::OnEnChangeFreqCtrlCtrlDeltafCtrltimeEdit()
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


void CFrequencyCtrlCtrl54::OnEnChangeFreqCtrlCtrlOfsToEdit()
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
