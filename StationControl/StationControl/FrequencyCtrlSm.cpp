/*============================================================================*/
/*! FrequencyCtrlSm.cpp

-# ���g��������-�V�~�����[�V�����^�u
*/
/*============================================================================*/
// FrequencyCtrlSm.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrlSm.h"
#include "afxdialogex.h"
#include "FrequencyCtrl.h"


// CFrequencyCtrlSm �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrlSm, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrlSm

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSm::CFrequencyCtrlSm(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrlSm::IDD, pParent)
	, m_PassStartDay(0)
	, m_PassStartTime(0)
	, m_PassEndDay(0)
	, m_PassEndTime(0)
{

}

/*============================================================================*/
/*! CFrequencyCtrlSm

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrlSm::~CFrequencyCtrlSm()
{
}

void CFrequencyCtrlSm::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_SPCR_CMB, m_SpacecraftCtrl);
	DDX_Control(pDX, IDD_DLG_FREQ_CTRL_SML_TAB, m_SmlTabCtrl);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_PASST_DAY_DP, m_PassStartDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_PASST_TIME_DP, m_PassStartTime);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_PASET_DAY_DP, m_PassEndDay);
	DDX_DateTimeCtrl(pDX, IDC_FREQ_CTRL_SML_PASET_TIME_DP, m_PassEndTime);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_PASST_DAY_DP, m_PassStartDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_PASET_DAY_DP, m_PassEndDTCtrl);
	DDX_Control(pDX, IDC_FREQ_CTRL_SML_ANTFC_BTN, m_AntPredBtnCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrlSm, CDialogBase)
	ON_BN_CLICKED(IDC_FREQ_CTRL_SML_ANTFC_BTN, &CFrequencyCtrlSm::OnBnClickedFreqCtrlSmlAntfcBtn)
	ON_NOTIFY(TCN_SELCHANGE, IDD_DLG_FREQ_CTRL_SML_TAB, &CFrequencyCtrlSm::OnTcnSelchangeDlgFreqCtrlSmlTab)
	ON_CBN_SELCHANGE(IDC_FREQ_CTRL_SML_SPCR_CMB, &CFrequencyCtrlSm::OnCbnSelchangeFreqCtrlSmlSpcrCmb)
END_MESSAGE_MAP()


// CFrequencyCtrlSm ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CFrequencyCtrlSm

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSm::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �q�_�C�A���O�쐬
	m_TabTf.SetCallWnd(this);
	m_Tab3W.SetCallWnd(this);

	m_TabTf.Create(CFrequencyCtrlSmTf::IDD, &m_SmlTabCtrl);
	//m_TabFf.Create(CFrequencyCtrlSmRf::IDD, &m_SmlTabCtrl);
	m_Tab3W.Create(CFrequencyCtrlSm3W::IDD, &m_SmlTabCtrl);
	//m_TabCf.Create(CFrequencyCtrlSmCf::IDD, &m_SmlTabCtrl);

	// �q�_�C�A���O�ړ�
	CRect	rect;
	m_SmlTabCtrl.GetWindowRect(rect);
	m_SmlTabCtrl.AdjustRect(FALSE, rect);
	m_SmlTabCtrl.ScreenToClient(rect);
	rect.OffsetRect(0, 18);
	rect.bottom = rect.bottom - 18;


	m_TabTf.MoveWindow(&rect, FALSE);
	//m_TabFf.MoveWindow(&rect, FALSE);
	m_Tab3W.MoveWindow(&rect, FALSE);
	//m_TabCf.MoveWindow(&rect, FALSE);

	// �^�u�쐬
	m_SmlTabCtrl.InsertItem(0, _T("�n��Ǒ��M���g���ύX"));
	//m_SmlTabCtrl.InsertItem(1, _T("�n��ǎ�M���g���ύX"));
	m_SmlTabCtrl.InsertItem(1, _T("3WAY"));
	//m_SmlTabCtrl.InsertItem(3, _T("�R�}���h�T�u�L�����A���g���ύX"));

	// �����t�H�[�}�b�g�ύX
	m_PassStartDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_PassEndDTCtrl.SetFormat(_T("yyyy-MM-dd"));

	Initialize();

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

	OnTcnSelchangeDlgFreqCtrlSmlTab(0, 0);

	// �J�����g�̗\��l��ݒ肷��
	OnCbnSelchangeFreqCtrlSmlSpcrCmb();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CFrequencyCtrlSm

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrlSm::UpdateDlg()
{
	return TRUE;
}

/*============================================================================*/
/*! CFrequencyCtrlSm

-# �R���g���[���̏�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSm::Initialize()
{
	// �q�������X�g�̍쐬
	vector<CString>::iterator itr;
	m_SpacecraftCtrl.ResetContent();
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	for (itr = stNameList.begin(); itr != stNameList.end(); itr++){
		m_SpacecraftCtrl.AddString((*itr));
	}
	// ��荇�����擪�̉q���A�N�e�B�u�ɂ���
	m_SpacecraftCtrl.SetCurSel(0);

	// �J�����g�̗\��l��ݒ肷��
	OnCbnSelchangeFreqCtrlSmlSpcrCmb();
}


/*============================================================================*/
/*! CFrequencyCtrlSm

-# �^�u�I��ύX�ʒm

@param  pNMHDR	�FNMHDR�\���̃|�C���^�[�B
@param  pResult	�F���b�Z�[�W���������ꂽ�Ƃ����ʂ��i�[����R�[�h���� LRESULT �̕ϐ��ւ̃|�C���^�[�B
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CFrequencyCtrlSm::OnTcnSelchangeDlgFreqCtrlSmlTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel;
	sel = m_SmlTabCtrl.GetCurSel();

	m_TabTf.ShowWindow(sel == 0 ? SW_SHOW : SW_HIDE);
	//m_TabFf.ShowWindow(sel == 1 ? SW_SHOW : SW_HIDE);
	m_Tab3W.ShowWindow(sel == 1 ? SW_SHOW : SW_HIDE);
	//m_TabCf.ShowWindow(sel == 3 ? SW_SHOW : SW_HIDE);


//	*pResult = 0;
	if (pResult)
		*pResult = 0;
}


/*============================================================================*/
/*! CFrequencyCtrlSm

-# �A���e�i�\��l�{�^������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrlSm::OnBnClickedFreqCtrlSmlAntfcBtn()
{
	CString satellite;
	m_SpacecraftCtrl.GetWindowTextW(satellite);
	theApp.m_pMainWnd->SendMessage(eMessage_OpenForecastListDlg, (WPARAM)this, (LPARAM)&satellite);
}


void CFrequencyCtrlSm::OnCbnSelchangeFreqCtrlSmlSpcrCmb()
{
	UpdateData(TRUE);
	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	vector<stPredListData>& allPred = ((CFrequencyCtrl*)mCallWnd)->GetAllPredList();
	vector<stPredListData>& partPred = ((CFrequencyCtrl*)mCallWnd)->GetPartPredList();
	if (stNameList.size() == 0)
		return;
	int pos = m_SpacecraftCtrl.GetCurSel();
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
	CString temp = predfile;
	int idx = predfile.ReverseFind(_T('/'));
	predfile = predfile.Mid(idx + 1);
	idx = predfile.Find(_T("."));
	predfile = predfile.Mid(0, idx) + _T(".anp");
	m_AntPredBtnCtrl.SetWindowTextW(predfile);
	((CFrequencyCtrl*)mCallWnd)->GetTargetData().predAnp = predfile;
	mTargetPredFilePath = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation()) + satellite + _T("\\") + predfile;
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
LRESULT CFrequencyCtrlSm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == eMessage_UpdateForecastFilePath)
	{
		CString predfile = (*(CString*)lParam);
		predfile = predfile.Right(predfile.GetLength() - predfile.ReverseFind(_T('/')) - 1);

		int idx = predfile.Find(_T("."));
		predfile = predfile.Mid(0, idx) + _T(".anp");

		m_AntPredBtnCtrl.SetWindowTextW(predfile);
		CString satellite;
		m_SpacecraftCtrl.GetWindowTextW(satellite);
		mTargetPredFilePath = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation()) + satellite + _T("\\") + predfile;

		UpdateData(FALSE);
		return TRUE;
	}
	return CDialogBase::WindowProc(message, wParam, lParam);
}

/*============================================================================*/
/*! CFrequencyCtrlSm

-# SIM�f�[�^�̎擾

@param  data	�FSIM�f�[�^
@retval
*/
/*============================================================================*/
bool CFrequencyCtrlSm::GetSimData(stSimData& data)
{
	UpdateData(TRUE);

	CString satellite;
	vector<CString>& stNameList = ((CFrequencyCtrl*)mCallWnd)->GetSatelliteNameList();
	if (stNameList.size() == 0)
	{
		AfxMessageBox(_T("�q����������܂���B"), MB_OK | MB_ICONWARNING);
		return false;
	}

	CTime timeStart = CTime(m_PassStartDay.GetYear(), m_PassStartDay.GetMonth(), m_PassStartDay.GetDay(), m_PassStartTime.GetHour(), m_PassStartTime.GetMinute(), m_PassStartTime.GetSecond());
	CTime timeStop = CTime(m_PassEndDay.GetYear(), m_PassEndDay.GetMonth(), m_PassEndDay.GetDay(), m_PassEndTime.GetHour(), m_PassEndTime.GetMinute(), m_PassEndTime.GetSecond());
	time_t start_t = timeStart.GetTime();
	time_t end_t = timeStop.GetTime();
	if (start_t > end_t)
	{
		CString mes = _T("");
		CString strStartTime = timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString strEndTime = timeStop.Format(_T("%Y-%m-%d %H:%M:%S"));
		mes.Format(_T("�p�X�I���������p�X�J�n���������ߋ��̂��߁A�v�Z�ł��܂���B\n\n �p�X�J�n���� :%s\n �p�X�I������ :%s"), (LPCTSTR)strStartTime, (LPCTSTR)strEndTime);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		return false;
	}

	int pos = m_SpacecraftCtrl.GetCurSel();
	data.satname = stNameList[pos];
	m_AntPredBtnCtrl.GetWindowText(data.predname);

	data.passStart = start_t;
	data.passStop = end_t;
	data.predfilepath = mTargetPredFilePath;

	return true;
}
