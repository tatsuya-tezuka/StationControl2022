/*============================================================================*/
/*! SpectrumView.cpp

-# �X�y�N�g�����\�����
*/
/*============================================================================*/
// SpectrumView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SpectrumView.h"
#include "afxdialogex.h"
#include "SpectrumVerticalSetDlg.h"


extern CString spectrum_gpsz_moni_point_str[3];
extern CString spectrum_gpsz_bandw_str[23];
extern CString spectrum_gsz_dbm_str[1];
extern CSpectrumViewData::BandWidth_t spectrum_gast_bandw[];

extern COLORREF Spectrum_colText[5];
extern CString Spectrum_strFreqUnit[3];
extern CString gsz_center_str[1];


// CSpectrumView �_�C�A���O

IMPLEMENT_DYNAMIC(CSpectrumView, CSpectrumCommonDlg)

/*============================================================================*/
/*! CSpectrumView

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSpectrumView::CSpectrumView(CWnd* pParent /*=NULL*/)
: CSpectrumCommonDlg(CSpectrumView::IDD, pParent)
{

}

/*============================================================================*/
/*! CSpectrumView

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSpectrumView::~CSpectrumView()
{
}

void CSpectrumView::DoDataExchange(CDataExchange* pDX)
{
	CSpectrumCommonDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SPECTRUMVIEW_EDIT12, m_strPeakLevelUnit);
	DDX_Text(pDX, IDC_SPECTRUMVIEW_EDIT13, m_strPeakFreqUnit);
	DDX_Text(pDX, IDC_SPECTRUMVIEW_EDIT8, m_strHLValue);
	DDX_Text(pDX, IDC_SPECTRUMVIEW_EDIT9, m_strHCValue);
	DDX_Text(pDX, IDC_SPECTRUMVIEW_EDIT10, m_strHRValue);
	DDX_Text(pDX, IDC_SPECTRUMVIEW_EDIT4, m_strDataTime);
	DDX_Text(pDX, IDC_SPECTRUMVIEW_EDIT5, m_strPeakLevel);
	DDX_Text(pDX, IDC_SPECTRUMVIEW_EDIT6, m_strPeakFreq);
	DDX_Control(pDX, IDC_SPECTRUMVIEW_COMBO1, m_CCBoxMonitorPoint);
	DDX_Control(pDX, IDC_SPECTRUMVIEW_COMBO2, m_CCBoxBandWidth);
	DDX_Control(pDX, IDC_SPECTRUMVIEW_EDIT7, m_CEditCenterFreqOffset);
	DDX_Control(pDX, IDC_SPECTRUMVIEW_SPIN4, m_CSpinCenterFreqOffset);
	DDX_Control(pDX, IDC_SPECTRUMVIEW_BUTTON1, m_CButonReset);
	DDX_Control(pDX, IDC_STATIC_SPECTRUMVIEW_AREA, m_viewArea);
	DDX_Control(pDX, IDC_SPECTRUM_INPUTEXCHGEDIT, m_CEditInputExchg);
	DDX_Control(pDX, IDC_SPECTRUM_ATTEDIT, m_CEditAtt);
	DDX_Control(pDX, IDC_SPECTRUM_INPUTEXCHGSPIN, m_CSpinInputExchg);
	DDX_Control(pDX, IDC_SPECTRUM_ATTSPIN, m_CSpinAtt);
}


BEGIN_MESSAGE_MAP(CSpectrumView, CSpectrumCommonDlg)
	ON_STN_CLICKED(IDC_STATIC_SPECTRUMVIEW_AREA, &CSpectrumView::OnStnClickedStaticSpectrumviewArea)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPECTRUMVIEW_SPIN4, &CSpectrumView::OnDeltaposSpectrumviewSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPECTRUM_INPUTEXCHGSPIN, &CSpectrumView::OnDeltaposSpectrumInputexchgspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPECTRUM_ATTSPIN, &CSpectrumView::OnDeltaposSpectrumAttspin)
	ON_WM_VSCROLL()
//	ON_EN_CHANGE(IDC_SPECTRUMVIEW_EDIT1, &CSpectrumView::OnEnChangeSpectrumviewEdit1)
//	ON_EN_CHANGE(IDC_SPECTRUMVIEW_EDIT2, &CSpectrumView::OnEnChangeSpectrumviewEdit2)
//	ON_EN_CHANGE(IDC_SPECTRUMVIEW_EDIT3, &CSpectrumView::OnEnChangeSpectrumviewEdit3)
	ON_EN_CHANGE(IDC_SPECTRUMVIEW_EDIT7, &CSpectrumView::OnEnChangeSpectrumviewEdit7)
	ON_EN_CHANGE(IDC_SPECTRUM_INPUTEXCHGEDIT, &CSpectrumView::OnEnChangeSpectrumInputexchgedit)
	ON_EN_CHANGE(IDC_SPECTRUM_ATTEDIT, &CSpectrumView::OnEnChangeSpectrumAttedit)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CSpectrumView::OnMenuClose)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SPECTRUM_EXEC_BUTTON, &CSpectrumView::OnBnClickedSpectrumExecButton)
	ON_BN_CLICKED(IDC_SPECTRUMVIEW_BUTTON1, &CSpectrumView::OnBnClickedSpectrumviewButton1)
	ON_BN_CLICKED(IDC_SPECTRUM_INPUTEXCHG_EXEC_BUTTON, &CSpectrumView::OnBnClickedSpectrumInputexchgExecButton)
	ON_BN_CLICKED(IDC_SPECTRUM_RESET_BUTTON, &CSpectrumView::OnBnClickedSpectrumResetButton)
	ON_BN_CLICKED(IDC_SPECTRUM_INPUTEXCHG_RESET_BUTTON, &CSpectrumView::OnBnClickedSpectrumInputexchgResetButton)
	ON_CBN_SELCHANGE(IDC_SPECTRUMVIEW_COMBO1, &CSpectrumView::OnCbnSelchangeSpectrumviewCombo1)
	ON_CBN_SELCHANGE(IDC_SPECTRUMVIEW_COMBO2, &CSpectrumView::OnCbnSelchangeSpectrumviewCombo2)
	ON_BN_CLICKED(IDC_SPECTRUMVERTICALSET_BUTTON, &CSpectrumView::OnBnClickedSpectrumverticalsetButton)
	ON_CBN_DROPDOWN(IDC_SPECTRUMVIEW_COMBO1, &CSpectrumView::OnCbnDropdownSpectrumviewCombo1)
	ON_CBN_CLOSEUP(IDC_SPECTRUMVIEW_COMBO1, &CSpectrumView::OnCbnCloseupSpectrumviewCombo1)
	ON_CBN_DROPDOWN(IDC_SPECTRUMVIEW_COMBO2, &CSpectrumView::OnCbnDropdownSpectrumviewCombo2)
	ON_CBN_CLOSEUP(IDC_SPECTRUMVIEW_COMBO2, &CSpectrumView::OnCbnCloseupSpectrumviewCombo2)
END_MESSAGE_MAP()


// CSpectrumView ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSpectrumView

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CSpectrumView::OnInitDialog()
{
	CSpectrumCommonDlg::OnInitDialog();

	if (!InitializeDlg(theApp.GetSelectStation(), 0, eSpectrumGraphType_Normal))
		return FALSE;

	// �r���[�̍쐬
	CRect rect;
	m_viewArea.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRuntimeClass* pClass = RUNTIME_CLASS(CSpectrumGraphView);
	if (pClass == NULL)
	{
		return FALSE;
	}

	m_pView = (CSpectrumGraphView*)(pClass->CreateObject());
	if (m_pView != NULL)
	{
		m_pView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 0, NULL);
		m_pView->InitializeView(theApp.GetSelectStation(), 0, eSpectrumGraphType_Normal);
		m_pView->SendMessage(WM_INITIALUPDATE);
	}

	// �Ď����[�h�̏ꍇ�͔񊈐�
	if (theApp.GetExecuteMode() != eExecuteMode_Control){
		GetDlgItem(IDC_SPECTRUM_EXEC_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPECTRUMVIEW_BUTTON1)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	UpdateWindow();

	// �^�C�}�o�^
	SetTimer(1, SPECTRUM_INTERVAL, NULL);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CSpectrumView

-# �X�y�N�g�����`��̈�N���b�N���ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumView::OnStnClickedStaticSpectrumviewArea()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CSpectrumView

-# �^�C�}�C�x���g�������ɌĂяo�����

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumView::OnTimer(UINT_PTR nIDEvent)
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	/* �Ď��f�[�^�擾 */
	if (UpdateCtrl() == TRUE && m_pView != NULL) {
		// �r���[�`��
		m_pView->Invalidate();

		// X���l�\��
		m_strHLValue = spectrum_gast_bandw[m_nBandWidthCurSel].sz_low;
		m_strHRValue = spectrum_gast_bandw[m_nBandWidthCurSel].sz_upp;
		double d_freq;
		int n_freq_unit = ExchangeFreqUnit(pCSectrumData->m_gd_center_freq, d_freq);
		CString str = CSatelliteData::AddCanma(d_freq, 3);
		m_strHCValue.Format(_T("%s%s"), (LPCTSTR)str, (LPCTSTR)Spectrum_strFreqUnit[n_freq_unit]);

		// �����A�s�[�N���x���A�s�[�N���g���\��
		m_strDataTime = pCSectrumData->m_gsz_time_tag;
		m_strPeakLevel = CSatelliteData::AddCanma(pCSectrumData->m_gd_peak_level, 2);
		n_freq_unit = ExchangeFreqUnit(pCSectrumData->m_gd_peak_freq, d_freq);
		m_strPeakFreq = CSatelliteData::AddCanma(d_freq, 3);
		m_strPeakFreqUnit = Spectrum_strFreqUnit[n_freq_unit];

		UpdateData(FALSE);
	}

	CSpectrumCommonDlg::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSpectrumView

-# �E�B���h�E�j�����ɌĂяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumView::OnDestroy()
{
	CSpectrumCommonDlg::OnDestroy();

	KillTimer(1);
}


/*============================================================================*/
/*! CSpectrumView

-# �ݒ�{�^���������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumView::OnBnClickedSpectrumverticalsetButton()
{
	// �X�y�N�g�����ݒ�_�C�A���O���J���ʒm
	theApp.m_pMainWnd->PostMessage(eMessage_OpenSpectrumSetDlg, (WPARAM)this, NULL);
}

