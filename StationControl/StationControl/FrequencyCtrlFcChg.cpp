/*============================================================================*/
/*! FrequencyCtrlFcChg.cpp

-# ���g��������-�\��l�ؑփ^�u
*/
/*============================================================================*/
// FrequencyCtrlFcChg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlFcChg.h"
#include "afxdialogex.h"
#include "FrequencyCtrl.h"

#include "WaitDialog.h"


// CFrequencyCtrlFcChg �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlFcChg, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlFcChg::CFrequencyCtrlFcChg(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlFcChg::IDD, pParent)
	, m_PassStartDay(0)
	, m_PassStartTime(0)
	, m_PassEndDay(0)
	, m_PassEndTime(0)
	, m_nTxDopp(0)
{
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlFcChg::~CFrequencyCtrlFcChg()
{
}

void CFrequencyCtrlFcChg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_SPCR_CMB, m_SpacecraftCmb);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_TDP_CMB, m_TDoppCmb);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_FCCHG_PASST_DAY_DP, m_PassStartDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_FCCHG_PASST_TIME_DP, m_PassStartTime);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_FCCHG_PASET_DAY_DP, m_PassEndDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_FCCHG_PASET_TIME_DP, m_PassEndTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_PASST_DAY_DP, m_PassStartDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_PASET_DAY_DP, m_PassEndDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_FCCHG_ANTFC_BTN, m_AntPredBtnCtrl);
	DDX_CBIndex(pDX, IDC_FREQ_CTRL_FCCHG_TDP_CMB, m_nTxDopp);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlFcChg, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_FCCHG_ANTFC_BTN, &CFrequencyCtrlFcChg::OnBnClickedFreqCtrlFcchgAntfcBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_FCCHG_BTN, &CFrequencyCtrlFcChg::OnBnClickedFreqCtrlFcchgBtn)
	ON_BN_CLICKED(IDC_FREQ_CTRL_OPE_BTN, &CFrequencyCtrlFcChg::OnBnClickedFreqCtrlOpeBtn)
	ON_CBN_SELCHANGE(IDC_FREQ_CTRL_FCCHG_SPCR_CMB, &CFrequencyCtrlFcChg::OnCbnSelchangeFreqCtrlFcchgSpcrCmb)
	ON_CBN_SELCHANGE(IDC_FREQ_CTRL_FCCHG_TDP_CMB, &CFrequencyCtrlFcChg::OnCbnSelchangeFreqCtrlFcchgTdpCmb)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FREQ_CTRL_FCCHG_PASST_DAY_DP, &CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasstDayDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FREQ_CTRL_FCCHG_PASST_TIME_DP, &CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasstTimeDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FREQ_CTRL_FCCHG_PASET_DAY_DP, &CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasetDayDp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FREQ_CTRL_FCCHG_PASET_TIME_DP, &CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasetTimeDp)
END_MESSAGE_MAP()


// CFrequencyCtrlFcChg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlFcChg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	Initialize();

	// �����t�H�[�}�b�g�ύX
	m_PassStartDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_PassEndDTCtrl.SetFormat(_T("yyyy-MM-dd"));

	// ���M�h�b�v���⏞
	for (int i = 0; mCtrlFcChgTxDopp[i] != _T(""); ++i)
	{
		m_TDoppCmb.InsertString(-1, mCtrlFcChgTxDopp[i]);
	}
	m_TDoppCmb.SetCurSel(0);
	m_nTxDopp = 0;

	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);

	// �p�X�J�n����
	m_PassStartDay = time;
	m_PassStartTime = time;

	// �p�X�I������
	m_PassEndDay = time;
	m_PassEndTime = time;

	UpdateData(FALSE);

	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStart = time.GetTime();
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStop = time.GetTime();
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().txDopp = m_nTxDopp;

	// �J�����g�̗\��l��ݒ肷��
	OnCbnSelchangeFreqCtrlFcchgSpcrCmb();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlFcChg::UpdateDlg()
{
	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �R���g���[���̏�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::Initialize()
{
	// �q�������X�g�̍쐬
	vector<CString>::iterator itr;
	m_SpacecraftCmb.ResetContent();
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	for (itr = stNameList.begin(); itr != stNameList.end(); itr++){
		m_SpacecraftCmb.AddString((*itr));
	}
	// ��荇�����擪�̉q���A�N�e�B�u�ɂ���
	m_SpacecraftCmb.SetCurSel(0);

	// �J�����g�̗\��l��ݒ肷��
	OnCbnSelchangeFreqCtrlFcchgSpcrCmb();
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �q���I������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::OnCbnSelchangeFreqCtrlFcchgSpcrCmb()
{
	UpdateData(TRUE);
	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	vector<stPredListData>& allPred = ((CFrequencyCtrl*)mCallWnd)->GetAllPredList();
	vector<stPredListData>& partPred = ((CFrequencyCtrl*)mCallWnd)->GetPartPredList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCmb.GetCurSel();
	satellite = stNameList[pos];
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().satellite = satellite;
	vector<stPredListData>::iterator itr;
	partPred.clear();
	for (itr = allPred.begin(); itr != allPred.end(); itr++){
		if (satellite.CompareNoCase((*itr).strSatellite) == 0){
			partPred.push_back((*itr));
		}
	}
	CString predfile = partPred[0].strPredPath;
	int idx = predfile.ReverseFind(_T('/'));
	predfile = predfile.Mid(idx + 1);
	idx = predfile.Find(_T("."));
	predfile = predfile.Mid(0, idx) + _T(".anp");
	m_AntPredBtnCtrl.SetWindowTextW(predfile);
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().predAnp = predfile;
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �A���e�i�\��l�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::OnBnClickedFreqCtrlFcchgAntfcBtn()
{
	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCmb.GetCurSel();
	satellite = stNameList[pos];
	theApp.m_pMainWnd->SendMessage(eMessage_OpenForecastListDlg, (WPARAM)this, (LPARAM)&satellite);
}

/*============================================================================*/
/*! CFrequencyCtrlSm

-# �\��l�t�@�C���p�X���{�^���ɕ\������

@param  message	�F���b�Z�[�W�̎��ʎq
@param  wParam	�F���b�Z�[�W�̍ŏ��̃p�����[�^
@param  lParam	�F���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
@retval ��������
*/
/*============================================================================*/
LRESULT CFrequencyCtrlFcChg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == eMessage_UpdateForecastFilePath)
	{
		//return TRUE;
		CString predfile = (*(CString*)lParam);
		predfile = predfile.Right(predfile.GetLength() - predfile.ReverseFind(_T('/')) - 1);

		int idx = predfile.Find(_T("."));
		predfile = predfile.Mid(0, idx) + _T(".anp");

		m_AntPredBtnCtrl.SetWindowTextW(predfile);
		((CFrequencyCtrl*)mCallWnd)->GetTargetData().predAnp = predfile;

		UpdateData(FALSE);
		return TRUE;
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}


/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �\��l�ݒ�{�^���{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::OnBnClickedFreqCtrlFcchgBtn()
{
	//CDlgWaitCmd wait;
	//wait.Open(_T("�e�X�g"), _T("�e��DB��ǂݍ���ł��܂��B\n\n���΂炭���܂܂���������"), false);
	//Sleep(10000);
	//wait.Close();

	UpdateData(TRUE);

	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	vector<stPredListData>& allPred = ((CFrequencyCtrl*)mCallWnd)->GetAllPredList();
	vector<stPredListData>& partPred = ((CFrequencyCtrl*)mCallWnd)->GetPartPredList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCmb.GetCurSel();
	satellite = stNameList[pos];
	CString predfile;
	m_AntPredBtnCtrl.GetWindowText(predfile);
	/****************************************************************/
	/* ������ */
	CString ctrl = _T("");
	ctrl.Format(_T("CTRL.PRED_CHANGE_FREQ %s@%s"), (LPCTSTR)satellite, (LPCTSTR)predfile);
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));
}

/*============================================================================*/
/*! CFrequencyCtrlFcChg

-# �^�p������v�Z�{�^���{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlFcChg::OnBnClickedFreqCtrlOpeBtn()
{
	const CString mCmdCtrl[] = {
		_T("ON"),
		_T("AVE"),
		_T("NOMINAL"),
		_T("STT"),
	};

	UpdateData(TRUE);
	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	vector<stPredListData>& allPred = ((CFrequencyCtrl*)mCallWnd)->GetAllPredList();
	vector<stPredListData>& partPred = ((CFrequencyCtrl*)mCallWnd)->GetPartPredList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCmb.GetCurSel();
	satellite = stNameList[pos];
	CString predfile;
	m_AntPredBtnCtrl.GetWindowText(predfile);
	/****************************************************************/
	/* ������ */
	CString ctrl = _T("");
	ctrl.Format(_T("CTRL.PRED_GEN %s@%s@%s"), (LPCTSTR)satellite, (LPCTSTR)predfile, (LPCTSTR)mCmdCtrl[m_nTxDopp]);
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));
}


void CFrequencyCtrlFcChg::OnCbnSelchangeFreqCtrlFcchgTdpCmb()
{
	UpdateData(TRUE);
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().txDopp = m_nTxDopp;
}


void CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasstDayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime time = CTime(m_PassStartDay.GetYear(), m_PassStartDay.GetMonth(), m_PassStartDay.GetDay(), m_PassStartTime.GetHour(), m_PassStartTime.GetMinute(), m_PassStartTime.GetSecond());
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStart = time.GetTime();
	*pResult = 0;
}


void CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasstTimeDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime time = CTime(m_PassStartDay.GetYear(), m_PassStartDay.GetMonth(), m_PassStartDay.GetDay(), m_PassStartTime.GetHour(), m_PassStartTime.GetMinute(), m_PassStartTime.GetSecond());
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStart = time.GetTime();
	*pResult = 0;
}


void CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasetDayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime time = CTime(m_PassEndDay.GetYear(), m_PassEndDay.GetMonth(), m_PassEndDay.GetDay(), m_PassEndTime.GetHour(), m_PassEndTime.GetMinute(), m_PassEndTime.GetSecond());
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStop = time.GetTime();
	*pResult = 0;
}


void CFrequencyCtrlFcChg::OnDtnDatetimechangeFreqCtrlFcchgPasetTimeDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime time = CTime(m_PassEndDay.GetYear(), m_PassEndDay.GetMonth(), m_PassEndDay.GetDay(), m_PassEndTime.GetHour(), m_PassEndTime.GetMinute(), m_PassEndTime.GetSecond());
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().timeStop = time.GetTime();
	*pResult = 0;
}
