/*============================================================================*/
/*! FrequencyCtrlSm3W.cpp

-# ���g��������-�V�~�����[�V����-3WAY�^�u
*/
/*============================================================================*/
// FrequencyCtrlSm3W.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSm3W.h"
#include "afxdialogex.h"
#include "FrequencyCtrlSm.h"


// CFrequencyCtrlSm3W �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlSm3W, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSm3W::CFrequencyCtrlSm3W(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSm3W::IDD, pParent)
	, m_str3WAYFreq(_T(""))
	, m_SpecDay(0)
	, m_SpecTime(0)
{
	for (int row = 0; row < mItemRows; row++){
		for (int col = 0; col < mItemCols; col++){
			m_strSimValue[row][col] = _T("");
		}
	}
}

/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSm3W::~CFrequencyCtrlSm3W()
{
}

void CFrequencyCtrlSm3W::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF1_EDIT, m_str3WAYFreq);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_3W_SPT_DAY_DP, m_SpecDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_3W_SPT_TIME_DP, m_SpecTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_3W_SPT_DAY_DP, m_SpecDayDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_3W_GETFREQ_CMB, m_cbSimSendGet);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF2_PST_EDIT, m_strSimValue[0][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF2_PSET_EDIT, m_strSimValue[0][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF2_PEDT_EDIT, m_strSimValue[0][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_TF2_PEDT_SPT_EDIT, m_strSimValue[0][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_1W_PST_EDIT, m_strSimValue[1][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_1W_PSET_EDIT, m_strSimValue[1][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_1W_PEDT_EDIT, m_strSimValue[1][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_1W_SPT_EDIT, m_strSimValue[1][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_2W_PST_EDIT, m_strSimValue[2][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_2W_PSET_EDIT, m_strSimValue[2][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_2W_PEDT_EDIT, m_strSimValue[2][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_2W_SPT_EDIT, m_strSimValue[2][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_3W_PST_EDIT, m_strSimValue[3][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_3W_PSET_EDIT, m_strSimValue[3][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_3W_PEDT_EDIT, m_strSimValue[3][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_3W_3W_SPT_EDIT, m_strSimValue[3][3]);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSm3W, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_3W_CALC_BTN, &CFrequencyCtrlSm3W::OnBnClickedFreqCtrlSml3wCalcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_3W_CALC_BTN2, &CFrequencyCtrlSm3W::OnBnClickedFreqCtrlSml3wCalcBtn2)
	ON_BN_CLICKED(IDC_BUTTON_GET, &CFrequencyCtrlSm3W::OnBnClickedButtonGet)
END_MESSAGE_MAP()


// CFrequencyCtrlSm3W ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# �v�Z�{�^����������(�擾�擙)

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSm3W::OnBnClickedFreqCtrlSml3wCalcBtn2()
{
	UpdateData(TRUE);
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return;

	double	d_aos;
	double	d_los;
	errno_t err = 0;
	bool bNewFormat = false;
	if (err != 0 || mTrendData.load_pred_data((char*)(const char*)CStringA(data.predfilepath), &d_aos, &d_los, &bNewFormat) != 0){
		CString msg;
		msg.Format(_T("�\��l�t�@�C��(%s)�����݂��܂���"), data.predfilepath);
		MessageBox(msg, _T("���g������"), MB_OK | MB_ICONWARNING);
		// �G���[
		//=====================================================//
		//������������������������ Log ������������������������//
		msg.Format(_T("ANP(%s) open error"), data.predfilepath);
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSm3W"), _T("OnBnClickedFreqCtrlSml3wCalcBtn2"), msg, _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return;
	}

	double val;
	time_t center = data.passStart + ((data.passStop - data.passStart) / 2);

	// �p�X�J�n�����̎��g�������߂�
	val = CalculatioFreq(data.passStart, 0, 0);
	m_strSimValue[0][0].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStart, 1, 0);
	m_strSimValue[1][0].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStart, 2, 0);
	m_strSimValue[2][0].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStart, 3, 0);
	m_strSimValue[3][0].Format(_T("%0.1f"), val);
	// �p�X�Z���^�[�����̎��g�������߂�
	val = CalculatioFreq(center, 0, 1);
	m_strSimValue[0][1].Format(_T("%0.1f"), val);
	val = CalculatioFreq(center, 1, 1);
	m_strSimValue[1][1].Format(_T("%0.1f"), val);
	val = CalculatioFreq(center, 2, 1);
	m_strSimValue[2][1].Format(_T("%0.1f"), val);
	val = CalculatioFreq(center, 3, 1);
	m_strSimValue[3][1].Format(_T("%0.1f"), val);
	// �p�X�I�������̎��g�������߂�
	val = CalculatioFreq(data.passStop, 0, 2);
	m_strSimValue[0][2].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStop, 1, 2);
	m_strSimValue[1][2].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStop, 2, 2);
	m_strSimValue[2][2].Format(_T("%0.1f"), val);
	val = CalculatioFreq(data.passStop, 3, 2);
	m_strSimValue[3][2].Format(_T("%0.1f"), val);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# �w�莞���̎��g���v�Z

@param  time	����
@param  type	0=����Ǒ��M���g���@1=�n��Ǒ��M���g��(1WAY)�@2=�n��ǎ�M���g��(2WAY)�@3=�n��ǎ�M���g��(3WAY)
@param  timetype	0=�v��J�n�����@1=�v��Z���^�[�����@2=�v��I������
@retval �Ȃ�
*/
/*============================================================================*/
double CFrequencyCtrlSm3W::CalculatioFreq(time_t time, int type, int timetype, bool bPassUse/*=true*/)
{
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return 0.0;

	// DB�A�N�Z�X�N���X
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
	struct satinf_t *satinf_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[data.satname]);

	double txf = 0.0;
	double rrup;
	double rxfast;
	double dfdown;
#if 1
	double C = V_C;
#else
	double C = V_C/1000;
#endif

	switch (type){
	case	0:
		// ����Ǒ��M���g��
		// ����t�̃A�b�v�����N�ω����̎擾
		rrup = mTrendData.GetUpDownLinkRate(time, 0);
		// �q�����m�~�i����M���g���̌v�Z
//		rxfast = _wtof(m_str3WAYFreq) + (-(rrup / C) * _wtof(m_str3WAYFreq));
		rxfast = _wtof(m_str3WAYFreq);
		// ����t�̑��M���g���̌v�Z
		txf = C / (C - rrup) * rxfast;
		break;
	case	1:
		// �n��ǎ�M���g��(1WAY)
		// ����t�̃_�E�������N�ω����̎擾
		rrup = mTrendData.GetUpDownLinkRate(time, 1);
		// �q�����m�~�i����M���g���̌v�Z
//		rxfast = _wtof(m_str3WAYFreq) + (-(rrup / C) * _wtof(m_str3WAYFreq));
		rxfast = _wtof(m_str3WAYFreq);
		// ����t��1WAY�h�b�v�����g�������߂�
		dfdown = (-(rrup / C)) * rxfast;
		// ����t�̎�M���g���̌v�Z
		txf = rxfast + dfdown;
		break;
	case	2:
		// �n��ǎ�M���g��(2WAY)
		// ����t��2WAY�����ω����̎擾
		rrup = mTrendData.GetUpDownLinkRate(time, 2);
		// ����t��2WAY�h�b�v�����g�������߂�
		if (satinf_adr != NULL)
			dfdown = (-(rrup / C)) * _wtof(m_strSimValue[0][timetype]) * satinf_adr->d_xcoh;
		else
			dfdown = (-(rrup / C)) * _wtof(m_strSimValue[0][timetype]);
		// ����t�̎�M���g���̌v�Z
		if (satinf_adr != NULL)
			txf = _wtof(m_strSimValue[0][timetype]) * satinf_adr->d_xcoh + dfdown;
		else
			txf = _wtof(m_strSimValue[0][timetype]) + dfdown;
		break;
	case	3:
		// �n��ǎ�M���g��(3WAY)
		// ����t��2WAY�����ω����̎擾
		rrup = mTrendData.GetUpDownLinkRate(time, 3);
		// ����t��2WAY�h�b�v�����g�������߂�
		if (satinf_adr != NULL)
			dfdown = (-(rrup / C)) * _wtof(m_strSimValue[0][timetype]) * satinf_adr->d_xcoh;
		else
			dfdown = (-(rrup / C)) * _wtof(m_strSimValue[0][timetype]);
		// ����t�̎�M���g���̌v�Z
		if (satinf_adr != NULL)
			txf = _wtof(m_strSimValue[0][timetype]) * satinf_adr->d_xcoh + dfdown;
		else
			txf = _wtof(m_strSimValue[0][timetype]) + dfdown;
		break;
	}

	return txf;
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �u�擾�v�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSm3W::OnBnClickedButtonGet()
{
	UpdateData(TRUE);
	// ����Ǒ��M���g�����擾����
	int pos = m_cbSimSendGet.GetCurSel();
	UINT target = (UINT)m_cbSimSendGet.GetItemData(pos);
	CSatelliteData& sd = theApp.GetSatelliteData();
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(target, _T("X-UC.FREQUENCY"));
	m_str3WAYFreq = _T("0.0");
	if (obs_adr != NULL){
		m_str3WAYFreq.Format(_T("%0.1f"), obs_adr->d_data);
//#ifdef _DEBUG
//		m_str3WAYFreq.Format(_T("100.0"));
//#endif
	}
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# �v�Z�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSm3W::OnBnClickedFreqCtrlSml3wCalcBtn()
{
	UpdateData(TRUE);
	CTime t = CTime(m_SpecDay.GetYear(), m_SpecDay.GetMonth(), m_SpecDay.GetDay(), m_SpecTime.GetHour(), m_SpecTime.GetMinute(), m_SpecTime.GetSecond());
	time_t time = t.GetTime();
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return;

	double	d_aos;
	double	d_los;
	errno_t err = 0;
	bool bNewFormat = false;
	if (err != 0 || mTrendData.load_pred_data((char*)(const char*)CStringA(data.predfilepath), &d_aos, &d_los, &bNewFormat) != 0){
		CString msg;
		msg.Format(_T("�\��l�t�@�C��(%s)�����݂��܂���"), data.predfilepath);
		MessageBox(msg, _T("���g������"), MB_OK | MB_ICONWARNING);
		// �G���[
		//=====================================================//
		//������������������������ Log ������������������������//
		msg.Format(_T("ANP(%s) open error"), data.predfilepath);
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSm3W"), _T("OnBnClickedFreqCtrlSml3wCalcBtn"), msg, _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return;
	}

	double val;

	// �C�ӎ����̎��g�������߂�
	val = CalculatioFreq(time, 0, 3, false);
	m_strSimValue[0][3].Format(_T("%0.1f"), val);
	val = CalculatioFreq(time, 1, 3, false);
	m_strSimValue[1][3].Format(_T("%0.1f"), val);
	val = CalculatioFreq(time, 2, 3, false);
	m_strSimValue[2][3].Format(_T("%0.1f"), val);
	val = CalculatioFreq(time, 3, 3, false);
	m_strSimValue[3][3].Format(_T("%0.1f"), val);
	UpdateData(FALSE);
}


/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSm3W::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// ���M���g���擾��
	for (int i = 0; i < sizeof(mSimSenderFreq[theApp.GetSelectStation()]) / sizeof(mSimSenderFreq[theApp.GetSelectStation()][0]); i++){
		int pos = m_cbSimSendGet.AddString(mSimSenderFreq[theApp.GetSelectStation()][i].sendname);
		m_cbSimSendGet.SetItemData(pos, mSimSenderFreq[theApp.GetSelectStation()][i].sendid);
	}
	m_cbSimSendGet.SetCurSel(0);

	// �����t�H�[�}�b�g�ύX
	m_SpecDayDTCtrl.SetFormat(_T("yyyy-MM-dd"));

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	// �w�莞��
	m_SpecDay = time;
	m_SpecTime = time;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CFrequencyCtrlSm3W

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSm3W::UpdateDlg()
{
	return TRUE;
}
