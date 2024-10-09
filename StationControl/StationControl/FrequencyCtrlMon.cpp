/*============================================================================*/
/*! FrequencyCtrlMon.cpp

-# ���g��������-���g�����j�^�^�u
*/
/*============================================================================*/
// FrequencyCtrlMon.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlMon.h"
#include "afxdialogex.h"

#include "FrequencyCtrl.h"
#include "SatelliteData.h"


// CFrequencyCtrlMon �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlMon, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlMon::CFrequencyCtrlMon(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlMon::IDD, pParent)
	, m_strUC(_T(""))
	, m_strDC(_T(""))
	, m_strCmdCareer(_T(""))
	, m_strTFreqOffset(_T(""))
	, m_strTimeOffset(_T(""))
	, m_strForecastRTTUp(_T(""))
	, m_strForecastRTTDw(_T(""))
	, m_strTDoppler(_T(""))
	, m_strRDoppler(_T(""))
	, m_strDopplerCmdCareer(_T(""))
	, m_strSpacecraftCOHMode(_T(""))
	, m_tPredDay(0)
	, m_tPredTime(0)
	, m_strRFreqOffset(_T(""))
{
	mTargetSatellite.Empty();
	mTargetPredAnp.Empty();
}

CFrequencyCtrlMon::CFrequencyCtrlMon(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlMon::~CFrequencyCtrlMon()
{
}

void CFrequencyCtrlMon::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_UC_EDIT, m_strUC);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_DC_EDIT, m_strDC);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SET_CMD_EDIT, m_strCmdCareer);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_OFS_TF_EDIT, m_strTFreqOffset);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_OFS_TO_EDIT, m_strTimeOffset);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_FCV_RU_EDIT, m_strForecastRTTUp);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_FCV_RD_EDIT, m_strForecastRTTDw);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_DP_TD_EDIT, m_strTDoppler);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_DP_RD_EDIT, m_strRDoppler);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_DP_CMD_EDIT, m_strDopplerCmdCareer);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_SCM_EDIT, m_strSpacecraftCOHMode);
	DDX_Control(pDX, IDC_FREQ_CTRL_MONITOR_PRED_DAY_DP, m_PredTimeDayCtrl);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_MONITOR_PRED_DAY_DP, m_tPredDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_MONITOR_PRED_TIME_DP, m_tPredTime);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_PRE_TX_EDIT, m_strPreFreqTx);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_PRE_RX_EDIT, m_strPreFreqRx);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_PRE_SC_EDIT, m_strPreFreqSc);
	DDX_Text(pDX, IDC_FREQ_CTRL_MONITOR_OFS_TF2_EDIT, m_strRFreqOffset);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlMon, CDialogBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFrequencyCtrlMon ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CFrequencyCtrlMon

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlMon::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_PredTimeDayCtrl.SetFormat(_T("yyyy-MM-dd"));

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	m_tPredDay = time;
	m_tPredTime = time;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlMon::UpdateDlg()
{
	// ��ʍX�V
	SetData();

	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# ��ʍX�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlMon::SetData()
{
	CString mPreFreqTx;	// �\�����M���g��(TX)�̌v�Z�p
	CString mPreFreqRx;	// �\�����M���g��(RX)�̌v�Z�p
	CString mPreFreqSc;	// �\�����M���g��(SC)�̌v�Z�p
	CString mPredFile;	// �\��l�t�@�C����

	CFrequencyCtrlMon::stSetMonData mMonTable[] = {
		{ _T("X-UC.FREQUENCY"), m_strUC, _T("%0.1f"), eDOUBLE },					// �ݒ���g���iU/C�j
		{ _T("S70-DC.MON_FREQ"), m_strDC, _T("%0.1f"), eDOUBLE },					// �ݒ���g���iD/C�j
		{ _T("DSNTCR.TX_SC_FREQ"), m_strCmdCareer, _T("%0.1f"), eDOUBLE },			// �ݒ���g���i�R�}���h�T�u�L�����A�j
		{ _T("CTRL.UP_FREQ_MODE"), m_strTDoppler, _T("%s"), eSTRING },				// �h�b�v���⏞�i���M�h�b�v���j
		{ _T("CTRL.MODEM_12WAY"), m_strRDoppler, _T("%s"), eSTRING },				// �h�b�v���⏞�i��M�h�b�v���j
		{ _T("CTRL.CMD_SC_DOPP_MODE"), m_strDopplerCmdCareer, _T("%s"), eSTRING },	// �h�b�v���⏞�i�R�}���h�T�u�L�����A�j
		{ _T("CTRL.FREQ_OFFSET_UL"), m_strTFreqOffset, _T("%0.1f"), eDOUBLE },		// �I�t�Z�b�g�i���M���g���I�t�Z�b�g�j
		{ _T("CTRL.FREQ_OFFSET_DL"), m_strRFreqOffset, _T("%0.1f"), eDOUBLE },		// �I�t�Z�b�g�i��M���g���I�t�Z�b�g�j
		{ _T("CTRL.PRED_TIME_OFFSET"), m_strTimeOffset, _T("%s"), eSTRING },		// �I�t�Z�b�g�i�����I�t�Z�b�g�j
		{ _T("DSNTCR.TRP_RATIO"), m_strSpacecraftCOHMode, _T("%s"), eSTRING },		// �T���@COH���[�h

		{ _T("DSNTCR.TX_CARR_FREQ_DEV_MON"), mPreFreqTx, _T("%0.1f"), eDOUBLE },	// �\�����M���g���̌v�Z�p
		{ _T("DSNTCR.RX_CARR_FREQ_DEV_MON"), mPreFreqRx, _T("%0.1f"), eDOUBLE },	// �\�����M���g���̌v�Z�p
		{ _T("DSNTCR.TX_SC_DEV_MON"), mPreFreqSc, _T("%0.1f"), eDOUBLE },			// �\�����M���g���̌v�Z�p

		{ _T("CTRL.PRED_FILE"), mPredFile, _T("%s"), eSTRING },						// �\��l�t�@�C����
	};

	// �^�C�}�j��
	KillTimer(TID_FREQMON);

	UpdateData(TRUE);

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	UINT sta = theApp.GetSelectStation();
	CSatelliteData& sd = theApp.GetSatelliteData();

	CString str;

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
		}
	}

	/****************************************************************/
	/* �\�����g�� */
	// TX
	double fval = _wtoi(m_strUC) + _wtoi(mPreFreqTx);
	m_strPreFreqTx.Format(_T("%0.1f"), fval);

	// RX
	fval = _wtoi(m_strDC) + _wtoi(mPreFreqRx);
	m_strPreFreqRx.Format(_T("%0.1f"), fval);

	// SC
	fval = _wtoi(m_strCmdCareer) + _wtoi(mPreFreqSc);
	m_strPreFreqSc.Format(_T("%0.1f"), fval);

	/****************************************************************/
	/* �\��l��� */
	// ����
	CTime time;
	theApp.GetCurrentUtcTime(time);
	m_tPredDay = time;
	m_tPredTime = time;

#ifdef _DEBUG
	mPredFile = _T("C:/share/udsc64/ctrl/prd/geotail/1708270400.prd");
	mPredFile.MakeUpper();
#endif

	CString satName, anpName;
	bool bPredOk = _convertPredFileName(mPredFile, satName, anpName);

	if (satName.IsEmpty() || anpName.IsEmpty()){
		// �Ώۃt�@�C�����ȑO�̃t�@�C���ŕ\������
		satName = mTargetSatellite;
		anpName = mTargetPredAnp;
	}

	if (mTargetSatellite != satName){
		GetEqstatDbValue(satName);
		mTargetSatellite = satName;
	}
	if (mTargetPredAnp != anpName){
		mTargetPredAnp = anpName;
	}

	((CFrequencyCtrl*)mCallWnd)->GetAnpData(satName, anpName);
	vector<CFrequencyCtrl::stAnpData>& anp = ((CFrequencyCtrl*)mCallWnd)->GetAnpDataList();
	int anpsize = (int)anp.size();
	if (anpsize == 0){
		UpdateData(FALSE);
		SetTimer(TID_FREQMON, INTERVAL_FREQMON, 0);
		return;
	}

	// �\��l�t�@�C����ǂݍ���
	CString anppath, anpfile;
	anppath = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());
	anpfile.Format(_T("%s%s\\%s"), (LPCTSTR)anppath, (LPCTSTR)satName, (LPCTSTR)anpName);
	double	d_aos;
	double	d_los;
	errno_t err = 0;
	bool bNewFormat = false;
	if (err != 0 || mTrendData.load_pred_data((char*)(const char*)CStringA(anpfile), &d_aos, &d_los, &bNewFormat) != 0){
		// �G���[
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("ANP(%s) open error"), anpfile);
		CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlMon"), _T("SetData"), msg, _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
	}
	else{
		if (bNewFormat == true){
			// RTT(Up Link)
			// 2WAY���� / ����
			double db2Way = mTrendData.GetUpDownLinkRange(time.GetTime(), 2);
			m_strForecastRTTUp.Format(_T("%0.3f"), db2Way / V_C);

			// RTT(Down Link)
			// �A�b�v�����N���� x 2 / ����
			double dbUplinkRange = mTrendData.GetUpDownLinkRange(time.GetTime(), 0);
			m_strForecastRTTDw.Format(_T("%0.3f"), (dbUplinkRange * 2) / V_C);
		}
		else{
			// RTT(Up Link)
			// �A�b�v�����N���� x 2 / ����
			double dbUplinkRange = mTrendData.GetUpDownLinkRange(time.GetTime(), 0);
			m_strForecastRTTUp.Format(_T("%0.3f"), (dbUplinkRange * 2) / V_C);

			// RTT(Down Link)
			// �_�E�������N���� x 2 / ����
			double dbDownlinkRange = mTrendData.GetUpDownLinkRange(time.GetTime(), 1);
			m_strForecastRTTDw.Format(_T("%0.3f"), (dbDownlinkRange * 2) / V_C);
		}
	}

	UpdateData(FALSE);

	// �^�C�}�쐬
	SetTimer(TID_FREQMON, INTERVAL_FREQMON, 0);
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �^�C�}����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlMon::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_FREQMON)
	{
		SetData();
	}
	CDialogBase::OnTimer(nIDEvent);
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �q���ŗLDB������g�����j�^�Ŏg�p����f�[�^���擾����

@param  pbuf	�q���ŗL���
@retval �Ȃ�
*/
/*============================================================================*/
bool CFrequencyCtrlMon::_getTxFqVal(char* pbuf)
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
	mTxFreqVal = pdata->d_fq;

	return true;
}
bool CFrequencyCtrlMon::_getRxFqVal(char* pbuf)
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
	if (pdata->l_no == 1)	mRxFreqVal[0] = pdata->d_fq;
	if (pdata->l_no == 2)	mRxFreqVal[1] = pdata->d_fq;
	// ����ȊO�͂Ȃɂ����Ȃ�

	return true;
}
bool CFrequencyCtrlMon::_getXTXTypeVal(char* pbuf)
{
	// �m�~�i����M���g���ݒ�l��`�e�[�u��
	struct xtx_t {	/* �w�ё��M�ݔ��g�p�L������`�e�[�u��: �\�[�X��"xtx"�ɑ��� */
		long l_id;	 /* �f�[�^����: �萔=XTX_ID�l */
		long l_len;	 /* �f�[�^��  : sizeof( struct xtx_t ) */
		long l_x_tx; /* �w�ё��M�ݔ��g�p�L��:XTX_Y_ID .or. XTX_N_ID */
		long l_rfu;  /* �p�f�B���O */
	};
	struct xtx_t *pdata = (struct xtx_t *)pbuf;
	mTXTType = (pdata->l_x_tx == XTX_N_ID)?0:1;

	return true;
}
/*============================================================================*/
/*! CFrequencyCtrlMon

-# �q���ŗLDB������g�����j�^�Ŏg�p����f�[�^���擾����

@param  strst	�q����
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlMon::GetEqstatDbValue(CString& strst)
{
	typedef struct{
		UINT	dbid;
		bool(CFrequencyCtrlMon::*func)(char*);
	} stFreqFuncTable;
	const stFreqFuncTable _FreqCtrlMon[] = {
		{ TXFQVAL_ID, &CFrequencyCtrlMon::_getTxFqVal },
		{ RXFQVAL_ID, &CFrequencyCtrlMon::_getRxFqVal },
		{ XTX_ID, &CFrequencyCtrlMon::_getXTXTypeVal },
	};

	// ������
	mTxFreqVal = 0;
	mRxFreqVal[0] = 0;
	mRxFreqVal[1] = 0;
	mTXTType = 0;

	// DB�A�N�Z�X�N���X
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();
	struct satinf_t *satinf_adr;
	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[strst]);
	if (satinf_adr == NULL){
		return;
	}
	char* sattbl_adr = (char *)satinf_adr;
	while (1){
		satinf_adr = (struct satinf_t *)sattbl_adr;
		if (satinf_adr->l_id == ENDOFSAT_ID){
			// �X�̉q�����̒�`�̏I���������h�c
			break;
		}

		// �f�[�^���擾���邽�߁A�ݒ�e�[�u����S����������
		for (int index = 0; index < sizeof(_FreqCtrlMon) / sizeof(_FreqCtrlMon[0]); index++){
			if (satinf_adr->l_id == _FreqCtrlMon[index].dbid){
				(this->*_FreqCtrlMon[index].func)(sattbl_adr);
			}
		}
		sattbl_adr += satinf_adr->l_len;
	}
}

/*============================================================================*/
/*! CFrequencyCtrlMon

-# �\��l�t�@�C�����̕ϊ�

@param  strst	�Ď��\��l��
@retval �Ȃ�
*/
/*============================================================================*/
bool CFrequencyCtrlMon::_convertPredFileName(CString& monPredFileName, CString& satName, CString& anpName)
{
	// �\��l�t�@�C���t���p�X���i�\��l�t�@�C�����j
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CFrequencyCtrlMon"), _T("_convertPredFileName"), _T("Pred file name"), monPredFileName, nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	CString strFilePath = monPredFileName;
	strFilePath.Trim(_T(" "));
	if (strFilePath.IsEmpty()){
		// �t�@�C���������݂��Ȃ�
		return false;
	} else{
		// �g���q��prd�̏ꍇ�Aanp�ɒu��������
		CString fname = strFilePath;
		if (fname.Right(3).MakeLower() == PRED_FILE_TYPE){
			strFilePath = fname.Left(fname.GetLength() - 3);
			strFilePath += ANP_FILE_TYPE;
		}

		// �t�@�C�����Ɖq������؂�o��
		int i = 0;
		CString str;
		anpName.Empty();
		satName.Empty();
		while (AfxExtractSubString(str, strFilePath, i++, '/')){
			satName = anpName;
			anpName = str;
		}

		// �ǃt�H���_������
		strFilePath = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());
		//monPredFileName.Format(_T("%s%s\\%s"), (LPCTSTR)strFilePath, (LPCTSTR)satName, (LPCTSTR)anpName);
	}
	return true;
}
