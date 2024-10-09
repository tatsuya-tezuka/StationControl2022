/*============================================================================*/
/*! FrequencyCtrlSmTf.cpp

-# ���g��������-�V�~�����[�V����-�n��Ǒ��M���g���ύX�^�u
*/
/*============================================================================*/
// FrequencyCtrlSmTf.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSmTf.h"
#include "afxdialogex.h"
#include "FrequencyCtrlSm.h"


// CFrequencyCtrlSmTf �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlSmTf, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSmTf::CFrequencyCtrlSmTf(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSmTf::IDD, pParent)
, m_strTFreq(_T(""))
, m_strCFreq(_T(""))
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
/*! CFrequencyCtrlSmTf

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSmTf::~CFrequencyCtrlSmTf()
{
}

void CFrequencyCtrlSmTf::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF1_EDIT, m_strTFreq);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_CF_TF1_EDIT, m_strCFreq);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_TF_SPT_DAY_DP, m_SpecDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_TF_SPT_TIME_DP, m_SpecTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_TF_SPT_DAY_DP, m_SpecDayDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_DP_TF_CMB, m_TDopplerCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_DP_CF_CMB, m_CDopplerCtrl);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF2_PST_EDIT, m_strSimValue[0][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF2_PSET_EDIT, m_strSimValue[0][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF2_PEDT_EDIT, m_strSimValue[0][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_TF2_PEDT_SPT_EDIT, m_strSimValue[0][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_CCF_PST_EDIT, m_strSimValue[1][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_CCF_PSET_EDIT, m_strSimValue[1][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_CCF_PEDT_EDIT, m_strSimValue[1][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_CCF_SPT_EDIT, m_strSimValue[1][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_1W_PST_EDIT, m_strSimValue[2][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_1W_PSET_EDIT, m_strSimValue[2][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_1W_PEDT_EDIT, m_strSimValue[2][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_1W_SPT_EDIT, m_strSimValue[2][3]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_2W_PST_EDIT, m_strSimValue[3][0]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_2W_PSET_EDIT, m_strSimValue[3][1]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_2W_PEDT_EDIT, m_strSimValue[3][2]);
	DDX_Text(pDX, IDC_FREQ_CTRL_SML_TF_2W_SPT_EDIT, m_strSimValue[3][3]);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSmTf, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_TF_CALC_BTN, &CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfCalcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_TF_NOMINAL_BTN, &CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfNominalBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_CF_NOMINAL_BTN, &CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlCfNominalBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_TFCF_CALC_BTN, &CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfcfCalcBtn)
END_MESSAGE_MAP()


// CFrequencyCtrlSmTf ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# �v�Z�{�^����������(�p�X�J�n�����A�I������)

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfcfCalcBtn()
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
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSmTf"), _T("OnBnClickedFreqCtrlSmlTfcfCalcBtn"), msg, _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return;
	}

	double val;
	time_t center = data.passStart + ((data.passStop - data.passStart) / 2);

	GetEqstatDbValue(data.satname, 2);

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

@param  time		����
@param  type		0=�n��Ǒ��M���g���@1=�R�}���h�T�u�L�������g���@2=�n��ǎ�M���g��(1WAY)�@3=�n��ǎ�M���g��(2WAY)
@param  timetype	0=�v��J�n�����@1=�v��Z���^�[�����@2=�v��I������
@retval �Ȃ�
*/
/*============================================================================*/
double CFrequencyCtrlSmTf::CalculatioFreq(time_t time, int type, int timetype, bool bPassUse/*=true*/)
{
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return 0.0;

	// DB�A�N�Z�X�N���X
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
	struct satinf_t *satinf_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[data.satname]);

	int posT = m_TDopplerCtrl.GetCurSel();
	int posC = m_CDopplerCtrl.GetCurSel();

	double txf = 0.0;
	double rrup;
	double rxfast;
	double dfdown;
#if 1
	double C = V_C;
#else
	double C = V_C / 1000;
#endif

	switch (type){
	case	0:
#if 1
		if (posT == eDoppOffNomi)
		{
			txf = _wtof(m_strTFreq);
			return txf;
		}
		else if (posT == eDoppOffCenter)
		{
			// �v��J�n�����̒n��Ǒ��M���g��
			// ����t�̃A�b�v�����N�ω����̎擾
			time = data.passStart;
			rrup = mTrendData.GetUpDownLinkRate(time, 0);
			// �q�����m�~�i����M���g���̌v�Z
			//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
			rxfast = _wtof(m_strTFreq);
			// ����t�̑��M���g���̌v�Z
			double txf_s = C / (C - rrup) * rxfast;

			// �v��I�������̒n��Ǒ��M���g��
			// ����t�̃A�b�v�����N�ω����̎擾
			time = data.passStop;
			rrup = mTrendData.GetUpDownLinkRate(time, 0);
			// �q�����m�~�i����M���g���̌v�Z
			//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
			rxfast = _wtof(m_strTFreq);
			// ����t�̑��M���g���̌v�Z
			double txf_e = C / (C - rrup) * rxfast;

			txf = (txf_s + txf_e) / 2.0;
			return txf;
		}
		else if (posT == eDoppOffAos)
		{
			time = data.passStart;
		}
		// �n��Ǒ��M���g��
		// ����t�̃A�b�v�����N�ω����̎擾
		rrup = mTrendData.GetUpDownLinkRate(time, 0);
		// �q�����m�~�i����M���g���̌v�Z
		//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
		rxfast = _wtof(m_strTFreq);
		// ����t�̑��M���g���̌v�Z
		txf = C / (C - rrup) * rxfast;
#else
		if (bPassUse == true){
			if (posT == eDoppOffNomi){
				txf = _wtof(m_strTFreq);
				return txf;
			}
			else if (posT == eDoppOffCenter){
				time_t center = data.passStart + ((data.passStop - data.passStart) / 2);
				time = center;
			}
			else if (posT == eDoppOffAos){
				time = data.passStart;
			}
		}
		// �n��Ǒ��M���g��
		// ����t�̃A�b�v�����N�ω����̎擾
		rrup = mTrendData.GetUpDownLinkRate(time, 0);
		// �q�����m�~�i����M���g���̌v�Z
		//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
		rxfast = _wtof(m_strTFreq);
		// ����t�̑��M���g���̌v�Z
		txf = C / (C - rrup) * rxfast;
#endif
		break;
	case	1:
#if 1
		if (posC == eDoppOffNomi){
			txf = _wtof(m_strCFreq);
			return txf;
		}
		else if (posC == eDoppOffCenter){
			// �v��J�n�����̃R�}���h�T�u�L�������g��
			// ����t�̃A�b�v�����N�ω����̎擾
			rrup = mTrendData.GetUpDownLinkRate(data.passStart, 0);
			// �q�����m�~�i����M���g���̌v�Z
			//		rxfast = _wtof(m_strCFreq) + (-(rrup / C) * _wtof(m_strCFreq));
			rxfast = _wtof(m_strCFreq);
			// ����t�̑��M���g���̌v�Z
			double txf_s = C / (C - rrup) * rxfast;

			// �v��I�������̃R�}���h�T�u�L�������g��
			// ����t�̃A�b�v�����N�ω����̎擾
			rrup = mTrendData.GetUpDownLinkRate(data.passStop, 0);
			// �q�����m�~�i����M���g���̌v�Z
			//		rxfast = _wtof(m_strCFreq) + (-(rrup / C) * _wtof(m_strCFreq));
			rxfast = _wtof(m_strCFreq);
			// ����t�̑��M���g���̌v�Z
			double txf_e = C / (C - rrup) * rxfast;

			txf = (txf_s + txf_e) / 2.0;
			return txf;
		}
		else if (posC == eDoppOffAos){
			time = data.passStart;
		}
#else
		if (bPassUse == true){
			if (posC == eDoppOffNomi){
				txf = _wtof(m_strCFreq);
				return txf;
			}
			else if (posC == eDoppOffCenter){
				time_t center = data.passStart + ((data.passStop - data.passStart) / 2);
				time = center;
			}
			else if (posC == eDoppOffAos){
				time = data.passStart;
			}
		}
#endif
		// �R�}���h�T�u�L�������g��
		// ����t�̃A�b�v�����N�ω����̎擾
		rrup = mTrendData.GetUpDownLinkRate(time, 0);
		// �q�����m�~�i����M���g���̌v�Z
//		rxfast = _wtof(m_strCFreq) + (-(rrup / C) * _wtof(m_strCFreq));
		rxfast = _wtof(m_strCFreq);
		// ����t�̑��M���g���̌v�Z
		txf = C / (C - rrup) * rxfast;
		break;
	case	2:
		// �n��ǎ�M���g��(1WAY)
		// ����t�̃_�E�������N�ω����̎擾
		rrup = mTrendData.GetUpDownLinkRate(time, 1);
		// �q�����m�~�i�����M���g���̌v�Z
//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
		rxfast = _wtof(m_strRFreq);
		// ����t��1WAY�h�b�v�����g�������߂�
		dfdown = (-(rrup / C)) * rxfast;
		// ����t�̎�M���g���̌v�Z
		txf = rxfast + dfdown;
		break;
	case	3:
		// �n��ǎ�M���g��(2WAY)
		if (posT == eDoppOn)
		{
			// ���I�⏞�̏ꍇ1WAY�p�̌v�Z���s���B

			// �n��ǎ�M���g��(1WAY)
			// ����t�̃_�E�������N�ω����̎擾
			rrup = mTrendData.GetUpDownLinkRate(time, 1);
			// �q�����m�~�i�����M���g���̌v�Z
			//		rxfast = _wtof(m_strTFreq) + (-(rrup / C) * _wtof(m_strTFreq));
			rxfast = _wtof(m_strRFreq);
			// ����t��1WAY�h�b�v�����g�������߂�
			dfdown = (-(rrup / C)) * rxfast;
			// ����t�̎�M���g���̌v�Z
			txf = rxfast + dfdown;
		}
		else 
		{
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
		}
		break;
	}

	return txf;
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# �n��Ǒ��M���g���u�m�~�i���ɖ߂��v�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfNominalBtn()
{
	UpdateData(TRUE);
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return;

	GetEqstatDbValue(data.satname, 0);
	GetEqstatDbValue(data.satname, 2);
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# �R�}���h�T�u�L�����A���g���u�m�~�i���ɖ߂��v�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlCfNominalBtn()
{
	UpdateData(TRUE);
	CFrequencyCtrlSm::stSimData data;
	if (((CFrequencyCtrlSm*)mCallWnd)->GetSimData(data) == false)
		return;

	GetEqstatDbValue(data.satname, 1);
	UpdateData(FALSE);
}

/*============================================================================*/
/*! CFrequencyCtrlSmTf

-# �v�Z�{�^�����������i�C�ӎ����j

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::OnBnClickedFreqCtrlSmlTfCalcBtn()
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
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSmTf"), _T("OnBnClickedFreqCtrlSmlTfcfCalcBtn"), msg, _T(""), nLogEx::error);
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
/*! CFrequencyCtrlSmTf

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmTf::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// ���M�h�b�v��
	for (int i = 0; i < sizeof(mSimTxDopp) / sizeof(mSimTxDopp[0]); i++){
		m_TDopplerCtrl.AddString(mSimTxDopp[i]);
	}
	m_TDopplerCtrl.SetCurSel(0);

	// �R�}���h�T�u�L�����A
	for (int i = 0; i < sizeof(mSimCmdCarr) / sizeof(mSimCmdCarr[0]); i++){
		m_CDopplerCtrl.AddString(mSimCmdCarr[i]);
	}
	m_CDopplerCtrl.SetCurSel(0);

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
/*! CFrequencyCtrlSmTf

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSmTf::UpdateDlg()
{
	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �q���ŗLDB����n��Ǒ��M���g���f�[�^���擾����

@param  pbuf	�q���ŗL���
@retval �Ȃ�
*/
/*============================================================================*/
bool CFrequencyCtrlSmTf::_getTxFqVal(char* pbuf)
{
	// �m�~�i�����M���g���ݒ�l��`�e�[�u��
	struct txfqval_t {
		long l_id;		// �f�[�^����: �萔=TXFQVAL_ID�l
		long l_len;		// �f�[�^��
		long l_no;		// �ݒ�l�ԍ�
		long l_band;	// �o���h�w��
		double d_fq;	// ���M���g���l[Hz]
	};
	struct txfqval_t *pdata = (struct txfqval_t *)pbuf;
	if (pdata->d_fq != 0.0)
	{
		m_strTFreq.Format(_T("%0.1f"), pdata->d_fq);
	}

	return true;
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �q���ŗLDB����n��Ǒ��M���g���f�[�^���擾����

@param  pbuf	�q���ŗL���
@retval �Ȃ�
*/
/*============================================================================*/
bool CFrequencyCtrlSmTf::_getRxFqVal(char* pbuf)
{
	// �m�~�i�����M���g���ݒ�l��`�e�[�u��
	struct rxfqval_t {
		long l_id;		// �f�[�^����: �萔=TXFQVAL_ID�l
		long l_len;		// �f�[�^��
		long l_no;		// �ݒ�l�ԍ�
		long l_band;	// �o���h�w��
		double d_fq;	// ���M���g���l[Hz]
	};
	struct rxfqval_t *pdata = (struct rxfqval_t *)pbuf;
	if (pdata->d_fq != 0.0)
	{
		m_strRFreq.Format(_T("%0.1f"), pdata->d_fq);
	}

	return true;
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �q���ŗLDB����R�}���h�T�u�L�����A���g���f�[�^���擾����

@param  pbuf	�q���ŗL���
@retval �Ȃ�
*/
/*============================================================================*/
bool CFrequencyCtrlSmTf::_getCmdFqVal(char* pbuf)
{
	// �m�~�i����M���g���ݒ�l��`�e�[�u��
	struct rxfqval_t {
		long l_id;		// �f�[�^����: �萔=TXFQVAL_ID�l
		long l_len;		// �f�[�^��
		long l_no;		// �ݒ�l�ԍ�
		long l_band;	// �o���h�w��
		double d_fq;	// ��M���g���l[Hz]
	};
	struct rxfqval_t *pdata = (struct rxfqval_t *)pbuf;
	m_strCFreq.Format(_T("%0.1f"), pdata->d_fq);

	return true;
}
/*============================================================================*/
/*! CFrequencyCtrlMon

-# �q���ŗLDB����R�}���h�T�u�L�����A���g���f�[�^���擾����

@param  pbuf	�q���ŗL���
@retval �Ȃ�
*/
/*============================================================================*/
bool CFrequencyCtrlSmTf::_getCmdFqValEx(CString pbuf)
{
	double val = _wtof(pbuf);
	m_strCFreq.Format(_T("%0.1f"), val);
//	m_strCFreq = pbuf;

	return true;
}
/*============================================================================*/
/*! CFrequencyCtrlMon

-# �q���ŗLDB����SIM�Ŏg�p����f�[�^���擾����

@param  strst	�q����
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSmTf::GetEqstatDbValue(CString& strst, int type)
{
	if (strst.IsEmpty() == TRUE)
	{
		MessageBox(_T("�q�����I������Ă��܂���B"), _T("���g������"), MB_OK | MB_ICONWARNING);
	}

	typedef struct{
		UINT	dbid;
		bool(CFrequencyCtrlSmTf::*func)(char*);
	} stFreqFuncTable;
	const stFreqFuncTable _FreqCtrlMon[] = {
#if 1
		{ TXFQVAL_ID, &CFrequencyCtrlSmTf::_getTxFqVal },
		{ RXFQVAL_ID, &CFrequencyCtrlSmTf::_getCmdFqVal },
		{ RXFQVAL_ID, &CFrequencyCtrlSmTf::_getRxFqVal },
#else
		{ TXFQVAL_ID, &CFrequencyCtrlSmTf::_getTxFqVal },
		{ RXFQVAL_ID, &CFrequencyCtrlSmTf::_getCmdFqVal },
#endif
	};

	// DB�A�N�Z�X�N���X
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[strst]);
	original_adr = satinf_adr;

	char* sattbl_adr = (char *)satinf_adr;
	while (1){
		if (type == 1){
			// �R�}���h�T�u�L�����A�́uDSN-MODEM MODULATION�����ݒ�v���擾����
			break;
		}
		satinf_adr = (struct satinf_t *)sattbl_adr;
		if (satinf_adr->l_id == ENDOFSAT_ID){
			// �X�̉q�����̒�`�̏I���������h�c
			break;
		}

		// �f�[�^���擾���邽�߁A�ݒ�e�[�u����S����������
		if (satinf_adr->l_id == _FreqCtrlMon[type].dbid){
			(this->*_FreqCtrlMon[type].func)(sattbl_adr);
//			break;
		}
		sattbl_adr += satinf_adr->l_len;
	}

	if (type != 1)
		return;

	//=========================================================================
	// �R�}���h�T�u�L�����A���g���u�m�~�i���ɖ߂��v�������s��
	// �q��DB�́uINIT.DSNTCR-MOD�v-�uDSNTCR.TX_SC_FREQ�v�̐ݒ�l���g�p����B
	// ID�����݂��Ȃ��ꍇ�͉������Ȃ�
	const CString mMacName = _T("INIT.DSNTCR-MOD");
	const CString mExecName = _T("DSNTCR.TX_SC_FREQ");
	const UINT MAC_MAX = 256;
	char *psz_tbl[MAC_MAX];				// �}�N�������i�[����e�[�u���̐擪�A�h���X
	char *psz_vec[MAC_MAX];				// �}�N�����A�h���X���i�[����e�[�u����
	int caseNum = 0;
	int execNum = 0;

	satinf_adr = original_adr;				/*�q���{�̏��擪�A�h���X�Z�b�g*/
	// MAC���́uINIT.DSNTCR-MOD�v��T���ACASE�p�����[�^�Q���擾����
	caseNum = dba.ud_macname_to_ststbl((char*)satinf_adr, CStringA(mMacName).GetBuffer(), psz_tbl, psz_vec, MAC_MAX);
	execNum = 0;
	bool bFind = false;
	if (caseNum > 0){
		// �擾���ꂽCASE�p�����^�Q���EXEC�p�����^�Q���擾����
		for (int i = 0; i < caseNum; i++){
			char *psz_exec[MAC_MAX];				// �}�N�����A�h���X���i�[����e�[�u����
			execNum = dba.ud_macstsadr_to_exectbl((char*)psz_vec[i], psz_tbl, psz_exec, MAC_MAX);
		}
		for(int i = 0; i < execNum; i++){
			// �w��̕�����uDSNTCR.TX_SC_FREQ�v��T��
			CString strExec, str, strName, strValue;
			strExec = CString(psz_tbl[i]);
			int pos = 0;
			while (1){
				if (AfxExtractSubString(str, strExec, pos, ',') == FALSE)
					break;
				str.TrimLeft();
				str.TrimRight();
				switch (pos){
				case	0:	// EXEC����
					strName = str;
					break;
				case	1:	// �l
					strValue = str;
					break;
				}
				pos++;
			}
			if (strName.CompareNoCase(mExecName) == 0){
				// �Ώۂ�EXEC���̂���������
				_getCmdFqValEx(strValue);
				bFind = true;
				break;
			}
		}
	}
	if (bFind == false){
		MessageBox(_T("DB�f�[�^�����݂��܂���"), _T("���g������"), MB_OK | MB_ICONWARNING);
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("DSNTCR.TX_SC_FREQ not found"));
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlSmTf"), _T("GetEqstatDbValue"), msg, _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
	}
}
