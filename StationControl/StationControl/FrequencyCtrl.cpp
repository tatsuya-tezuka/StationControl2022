/*============================================================================*/
/*! FrequencyCtrl.cpp

-# ���g��������
*/
/*============================================================================*/
// FrequencyCtrl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrl.h"
#include "afxdialogex.h"


// CFrequencyCtrl �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrl, CDialogBase)

/*============================================================================*/
/*! CFrequencyCtrl

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrl::CFrequencyCtrl(CWnd* pParent /*=NULL*/)
: CDialogBase(CFrequencyCtrl::IDD, pParent)
{
	mTargetData.satellite.Empty();
	mTargetData.predAnp.Empty();
	mTargetData.txDopp = 0;
	mTargetData.timeStart = 0;
	mTargetData.timeStop = 0;

	Initialize(false);
}

/*============================================================================*/
/*! CFrequencyCtrl

-# �R���X�g���N�^

@param	IDD�FIDD
@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrl::CFrequencyCtrl(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CFrequencyCtrl

-# �f�X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CFrequencyCtrl::~CFrequencyCtrl()
{
}

/*============================================================================*/
/*! CForecastOperationPlan

-# �v��ꗗ����q�������X�g���擾����

@param	listName�F�q�������X�g
@retval TRUE: �Ԃ��B FALSE: �Ԃ��Ȃ��B

*/
/*============================================================================*/
BOOL CFrequencyCtrl::GetSatelliteNameList(vector<CString>& listName)
{
	CGraphMutex::Lock(eFile);
	vector<stPredListData>::iterator itr;
	vector<CString>::iterator itrName;

	listName.clear();

	for (itr = mAllPredList.begin(); itr != mAllPredList.end(); itr++)
	{
		// �q���������ɉq�������X�g�ɂ��邩���ׂ�
		itrName = find(listName.begin(), listName.end(), (*itr).strSatellite);
		if (itrName == listName.end())
		{
			// �����ꍇ�͋ǖ����X�g�ɒǉ�
			listName.push_back((*itr).strSatellite);
		}
	}
	CGraphMutex::Unlock(eFile);

	if (listName.empty() == TRUE)
		return FALSE;

	return TRUE;
}

void CFrequencyCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FREQ_CTRL_TAB, m_FreqTabCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrl, CDialogBase)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FREQ_CTRL_TAB, &CFrequencyCtrl::OnTcnSelchangeFreqCtrlTab)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CFrequencyCtrl::OnMenuClose)
END_MESSAGE_MAP()


// CFrequencyCtrl ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CFrequencyCtrl

-# �t�@�C��-���郁�j���[����

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrl::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CFrequencyCtrl

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrl::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �q�_�C�A���O�쐬
	m_TabMon.SetCallWnd(this);
	m_TabCtrl.SetCallWnd(this);
	m_TabPre.SetCallWnd(this);
	m_TabSm.SetCallWnd(this);
	m_TabFcChg.SetCallWnd(this);
	m_TabMon.Create(CFrequencyCtrlMon::IDD, &m_FreqTabCtrl);
	m_TabCtrl.Create(CFrequencyCtrlCtrl::IDD, &m_FreqTabCtrl);
	m_TabPre.Create(CFrequencyCtrlPre::IDD, &m_FreqTabCtrl);
	m_TabSm.Create(CFrequencyCtrlSm::IDD, &m_FreqTabCtrl);
	m_TabFcChg.Create(CFrequencyCtrlFcChg::IDD, &m_FreqTabCtrl);

	// �q�_�C�A���O�ړ�
	CRect	rect;
	m_FreqTabCtrl.GetWindowRect(rect);
	m_FreqTabCtrl.AdjustRect(FALSE, rect);
	m_FreqTabCtrl.ScreenToClient(rect);
	rect.OffsetRect(0, 18);
	rect.bottom = rect.bottom - 18;


	m_TabMon.MoveWindow(&rect, FALSE);
	m_TabCtrl.MoveWindow(&rect, FALSE);
	m_TabPre.MoveWindow(&rect, FALSE);
	m_TabSm.MoveWindow(&rect, FALSE);
	m_TabFcChg.MoveWindow(&rect, FALSE);

	// �^�u�쐬
	m_FreqTabCtrl.InsertItem(0, _T("���g�����j�^"));
	m_FreqTabCtrl.InsertItem(1, _T("�ʐݒ�"));
	m_FreqTabCtrl.InsertItem(2, _T("�ꊇ�ݒ�"));
	m_FreqTabCtrl.InsertItem(3, _T("�V�~�����[�V����"));
	m_FreqTabCtrl.InsertItem(4, _T("�\��l�؂�ւ�"));

	// �����\��
	OnTcnSelchangeFreqCtrlTab(0, 0);
	
	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CFrequencyCtrl

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CFrequencyCtrl::UpdateDlg()
{
	return TRUE;
}


/*============================================================================*/
/*! CFrequencyCtrl

-# �R���g���[���̏�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrl::Initialize(bool bResetControl/* = true*/)
{
	mAllPredList = theApp.GetSatelliteData().GetPredList();
	GetSatelliteNameList(mSatelliteNameList);
	if (bResetControl == true){
		m_TabSm.Initialize();
		m_TabFcChg.Initialize();
	}
}

/*============================================================================*/
/*! CFrequencyCtrl

-# �^�u�I��ύX�ʒm

@param  pNMHDR	�FNMHDR�\���̃|�C���^�[�B
@param  pResult	�F���b�Z�[�W���������ꂽ�Ƃ����ʂ��i�[����R�[�h���� LRESULT �̕ϐ��ւ̃|�C���^�[�B
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CFrequencyCtrl::OnTcnSelchangeFreqCtrlTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int        sel;
	sel = m_FreqTabCtrl.GetCurSel();

	// �Ď����[�h���͐擪�́u���g�����j�^�v�����I���ł��Ȃ�
	if (theApp.GetExecuteMode() != eExecuteMode_Control){
		m_TabMon.ShowWindow(SW_SHOW);
		m_TabCtrl.ShowWindow(SW_HIDE);
		m_TabPre.ShowWindow(SW_HIDE);
		m_TabSm.ShowWindow(SW_HIDE);
		m_TabFcChg.ShowWindow(SW_HIDE);
		m_FreqTabCtrl.SetCurSel(0);
	}
	else{
		m_TabMon.ShowWindow(sel == 0 ? SW_SHOW : SW_HIDE);
		m_TabCtrl.ShowWindow(sel == 1 ? SW_SHOW : SW_HIDE);
		m_TabPre.ShowWindow(sel == 2 ? SW_SHOW : SW_HIDE);
		m_TabSm.ShowWindow(sel == 3 ? SW_SHOW : SW_HIDE);
		m_TabFcChg.ShowWindow(sel == 4 ? SW_SHOW : SW_HIDE);
	}


//	*pResult = 0;
	if (pResult)
		*pResult = 0;
}


/*============================================================================*/
/*! CFrequencyCtrl

-# �\��l�t�@�C��(anp)����f�[�^�擾

@param	satName		�q����
@param	anpName		�\��lANP�t�@�C����
@retval	�Ȃ�
*/
/*============================================================================*/
void CFrequencyCtrl::GetAnpData(CString satName, CString anpName)
{
	CString anpPath;
	FILE *fp;

	char	szLine[512];
	TCHAR	szBuff[512];

	COleDateTime oleTime;
	CTime tmpTime;

	m_AnpDataList.clear();

	if (anpName == _T("") || satName == _T("")){
		return;
	}

	// �\��l�t�H���_�擾
	CString dir = theApp.GetShareFilePath(eFileType_Pred_Folder, theApp.GetSelectStation());

	// �\��l�t�@�C���t���p�X�擾
	anpPath.Format(_T("%s%s\\%s"), (LPCTSTR)dir, (LPCTSTR)satName, (LPCTSTR)anpName);

	// �\��l�t�@�C�����A�����AAZ�AEL�擾
	if ((_wfopen_s(&fp, anpPath, _T("r")) != 0) || (fp == NULL)){
		return;
	}


	// �w�b�_�[���ǂݔ�΂�
	for (int i = 0; i < 5; ++i){
		fgets(szLine, static_cast<unsigned>(_countof(szLine)), fp);
	}

	while (fgets(szLine, static_cast<unsigned>(_countof(szLine)), fp) != NULL){
		CFrequencyCtrl::stAnpData data = { 0 };
		wsprintf(szBuff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(szLine));
		swscanf_s(szBuff, _T("%s %s %s %s %s %s %s"),
			data.szTime, static_cast<TCHAR>(_countof(data.szTime)),
			data.szAZ, static_cast<TCHAR>(_countof(data.szAZ)),
			data.szEL, static_cast<TCHAR>(_countof(data.szEL)),
			data.szUpRNG, static_cast<TCHAR>(_countof(data.szUpRNG)),
			data.szDwRNG, static_cast<TCHAR>(_countof(data.szDwRNG)),
			data.szUpRR, static_cast<TCHAR>(_countof(data.szUpRR)),
			data.szDwRR, static_cast<TCHAR>(_countof(data.szDwRR)),
			data.sz2wayRNG, static_cast<TCHAR>(_countof(data.sz2wayRNG)),
			data.sz2wayRR, static_cast<TCHAR>(_countof(data.sz2wayRR)));

		m_AnpDataList.push_back(data);
	}
	fclose(fp);
}


LRESULT CFrequencyCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_UpdateShareFile:
		Initialize();
		break;

	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
