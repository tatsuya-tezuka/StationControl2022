/*============================================================================*/
/*! FrequencyCtrl54.cpp

-# ���g��������(54m)
*/
/*============================================================================*/
// FrequencyCtrl54.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "FrequencyCtrl54.h"
#include "afxdialogex.h"


// CFrequencyCtrl54 �_�C�A���O

IMPLEMENT_DYNAMIC(CFrequencyCtrl54, CFrequencyCtrl)

CFrequencyCtrl54::CFrequencyCtrl54(CWnd* pParent /*=NULL*/)
: CFrequencyCtrl(CFrequencyCtrl54::IDD, pParent)
{

}

CFrequencyCtrl54::~CFrequencyCtrl54()
{
}

void CFrequencyCtrl54::DoDataExchange(CDataExchange* pDX)
{
	CFrequencyCtrl::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FREQ_CTRL_TAB, m_FreqTabCtrl);
}


BEGIN_MESSAGE_MAP(CFrequencyCtrl54, CFrequencyCtrl)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FREQ_CTRL_TAB, &CFrequencyCtrl54::OnTcnSelchangeFreqCtrlTab)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CFrequencyCtrl::OnMenuClose)
END_MESSAGE_MAP()


// CFrequencyCtrl54 ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CFrequencyCtrl54

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CFrequencyCtrl54::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �q�_�C�A���O�쐬
	m_TabMon.SetCallWnd(this);
	m_TabCtrl.SetCallWnd(this);
	m_TabPre.SetCallWnd(this);
	m_TabSm.SetCallWnd(this);
	m_TabFcChg.SetCallWnd(this);
	m_TabMon.Create(CFrequencyCtrlMon54::IDD, &m_FreqTabCtrl);
	m_TabCtrl.Create(CFrequencyCtrlCtrl54::IDD, &m_FreqTabCtrl);
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

-# �^�u�I��ύX�ʒm

@param  pNMHDR	�FNMHDR�\���̃|�C���^�[�B
@param  pResult	�F���b�Z�[�W���������ꂽ�Ƃ����ʂ��i�[����R�[�h���� LRESULT �̕ϐ��ւ̃|�C���^�[�B
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
void CFrequencyCtrl54::OnTcnSelchangeFreqCtrlTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int        sel;
	sel = m_FreqTabCtrl.GetCurSel();

	// �Ď����[�h���͐擪�́u���g�����j�^�v�����I���ł��Ȃ�
	// ���䃂�[�h�ł��A�����������l
	if( (theApp.GetExecuteMode() == eExecuteMode_Monitor) ||
		(theApp.GetLimitedMode() == eLimitedMode_Enable) ){
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

LRESULT CFrequencyCtrl54::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
