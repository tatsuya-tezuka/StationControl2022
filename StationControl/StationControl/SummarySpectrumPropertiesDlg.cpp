/*============================================================================*/
/*! SummarySpectrumPropertiesDlg.cpp

-# �T�}���X�y�N�g�����ݒ���
*/
/*============================================================================*/
// SummarySpectrumPropertiesDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SummarySpectrumPropertiesDlg.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"

extern CString spectrum_gpsz_moni_point_str[3];
extern CString spectrum_gpsz_bandw_str[23];
extern CString spectrum_gsz_dbm_str[1];
extern CSpectrumViewData::BandWidth_t spectrum_gast_bandw[];

extern COLORREF Spectrum_colText[5];
extern CString Spectrum_strFreqUnit[3];
extern CString gsz_center_str[1];

// CSummarySpectrumPropertiesDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSummarySpectrumPropertiesDlg, CDialogBase)

/*============================================================================*/
/*! SummarySpectrumPropertiesDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSummarySpectrumPropertiesDlg::CSummarySpectrumPropertiesDlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CSummarySpectrumPropertiesDlg::IDD, pParent)
{

}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@param	nViewID�F�O���t�r���[ID
@param	listViewName�F�O���t�r���[�����X�g
@param  nStaionID�F�q��ID
@param  nParentID�F�e�E�B���h�EID
@param  strWndName�F�E�B���h�E�^�C�g��
@param	pSummaryProp�F�T�}���v���p�e�B
@retval �Ȃ�
*/
/*============================================================================*/
CSummarySpectrumPropertiesDlg::CSummarySpectrumPropertiesDlg(CWnd* pParent, UINT nViewID, vector<CString> listViewName, UINT nStaionID, 
	UINT nParentID /*= 0*/, CString strWndName /*= _T("")*/, CWnd* pSummaryProp /*= NULL*/, BOOL bChgDlg /*= FALSE*/)
	: CDialogBase(CSummarySpectrumPropertiesDlg::IDD, pParent)
	, m_ActiveView(nViewID)
	, m_listViewName(listViewName)
	, m_nStationID(nStaionID)
	, m_nParentID(nParentID)
	, m_strWndName(strWndName)
	, m_pSummaryProp(pSummaryProp)
	, m_bChgGraphPropDlg(FALSE)
	, m_bChgDlg(bChgDlg)
{
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSummarySpectrumPropertiesDlg::~CSummarySpectrumPropertiesDlg()
{
}


void CSummarySpectrumPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_MONPOINT_COMBO, m_spectrumSet.m_CCBoxMonitorPoint);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_BANDWIDTH_COMBO, m_spectrumSet.m_CCBoxBandWidth);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_CENTERFREQOFFSET_EDIT, m_spectrumSet.m_CEditCenterFreqOffset);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_CENTERFREQOFFSET_SPIN, m_spectrumSet.m_CSpinCenterFreqOffset);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_INPUTEXCHGEDIT, m_spectrumSet.m_CEditInputExchg);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_ATTEDIT, m_spectrumSet.m_CEditAtt);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_INPUTEXCHGSPIN, m_spectrumSet.m_CSpinInputExchg);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_ATTSPIN, m_spectrumSet.m_CSpinAtt);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUM_VERTICALSET_COMBO, m_verticalSet.m_CCBoxAutoScaleOnOff);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_YMAX_EDIT, m_verticalSet.m_CEditMax);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_YMIN_EDIT, m_verticalSet.m_CEditMin);
	DDX_Control(pDX, IDC_SUMMARYSPECTRUMVIEW_YGRID_EDIT, m_verticalSet.m_CEditGrid);
	DDX_Control(pDX, IDC_SUMMARYGRAPTH_SELECTGRAPH_COMBO, m_wndObjectCombo);
}


BEGIN_MESSAGE_MAP(CSummarySpectrumPropertiesDlg, CDialogBase)
	ON_CBN_SELCHANGE(IDC_SUMMARYSPECTRUM_VERTICALSET_COMBO, &CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumVerticalsetCombo)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_GRIDPROP_EXEC_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumGridpropExecButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_GRIDPROP_RESET_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumGridpropResetButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUMVIEW_CENTER_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumviewCenterButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_CTRL_EXEC_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumCtrlExecButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_CTRL_RESET_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumCtrlResetButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_INPUTATT_EXEC_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumInputattExecButton)
	ON_BN_CLICKED(IDC_SUMMARYSPECTRUM_INPUTATT_RESET_BUTTON, &CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumInputattResetButton)
	ON_BN_CLICKED(IDOK, &CSummarySpectrumPropertiesDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SUMMARYSPECTRUMVIEW_CENTERFREQOFFSET_SPIN, &CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumviewCenterfreqoffsetSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SUMMARYSPECTRUM_INPUTEXCHGSPIN, &CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumInputexchgspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SUMMARYSPECTRUM_ATTSPIN, &CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumAttspin)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUMVIEW_YMAX_EDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYmaxEdit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUMVIEW_YMIN_EDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYminEdit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUMVIEW_YGRID_EDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYgridEdit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUMVIEW_CENTERFREQOFFSET_EDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewCenterfreqoffsetEdit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUM_INPUTEXCHGEDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumInputexchgedit)
	ON_EN_CHANGE(IDC_SUMMARYSPECTRUM_ATTEDIT, &CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumAttedit)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_SUMMARYGRAPTH_SELECTGRAPH_COMBO, &CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummarygrapthSelectgraphCombo)
	ON_CBN_SELCHANGE(IDC_SUMMARYSPECTRUMVIEW_MONPOINT_COMBO, &CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumviewMonpointCombo)
	ON_CBN_SELCHANGE(IDC_SUMMARYSPECTRUMVIEW_BANDWIDTH_COMBO, &CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumviewBandwidthCombo)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CSummarySpectrumPropertiesDlg::OnMenuClose)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSummarySpectrumPropertiesDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �I�[�g�X�P�[���I��ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumVerticalsetCombo()
{
	m_verticalSet.OnCbnSelchangeSpectrumverticalsetCombo();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// �O���t�ĕ`��ʒm
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}

/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �c���O���b�h���s�{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumGridpropExecButton()
{
	m_verticalSet.OnBnClickedOk();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// �O���t�ĕ`��ʒm
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# �c���O���b�h�L�����Z���{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumGridpropResetButton()
{
	m_verticalSet.OnBnClickedCancel();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// �O���t�ĕ`��ʒm
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �Z���^�[�{�^���������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumviewCenterButton()
{
	m_spectrumSet.OnBnClickedSpectrumviewButton1();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// �ĕ`��́A�^�C�}�[�����ŊĎ��f�[�^�X�V��ɍs��
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���s�{�^���������ɂ�΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumCtrlExecButton()
{
	m_spectrumSet.OnBnClickedSpectrumExecButton();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// �ĕ`��́A�^�C�}�[�����ŊĎ��f�[�^�X�V��ɍs��
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���Z�b�g�{�^���������ɂ�΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumCtrlResetButton()
{
	m_spectrumSet.OnBnClickedSpectrumResetButton();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// �ĕ`��́A�^�C�}�[�����ŊĎ��f�[�^�X�V��ɍs��
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���͒[���Z���s�{�^���������ɂ�΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumInputattExecButton()
{
	m_spectrumSet.OnBnClickedSpectrumInputexchgExecButton();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// �O���t�ĕ`��ʒm
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���͒[���Z���Z�b�g�{�^���������ɂ�΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedSummaryspectrumInputattResetButton()
{
	m_spectrumSet.OnBnClickedSpectrumInputexchgResetButton();
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

	// �O���t�ĕ`��ʒm
	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���j���[����̃t�@�C���N���[�Y���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnBnClickedOk()
{
	if (!m_bChgGraphPropDlg)
	{
		CString title;
		GetWindowText(title);

		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	OnOK();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CSummarySpectrumPropertiesDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	if (!m_strWndName.IsEmpty())
	{
		CString strTitle;
		strTitle.Format(_T("%s�ݒ�"), (LPCTSTR)m_strWndName);
		SetWindowText(strTitle);
	}

	m_spectrumSet.InitializeDlg(m_nStationID, m_ActiveView, eSpectrumGraphType_Sammary);
	m_verticalSet.InitializeDlg(m_nStationID, m_ActiveView, eSpectrumGraphType_Sammary);

	// �O���t�I���R���{�{�b�N�X������
	for (int i = 0; i < m_listViewName.size(); i++)
	{
		m_wndObjectCombo.InsertString(-1, m_listViewName[i]);
	}
//	m_wndObjectCombo.SetCurSel(eSummaryWndSetType_XSP);
	CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(m_nStationID, m_ActiveView);
	m_wndObjectCombo.SetCurSel(prop.nGraphIdx);

	// �Ď����[�h�̏ꍇ�͔񊈐�
	if (theApp.GetExecuteMode() != eExecuteMode_Control){
		GetDlgItem(IDC_SUMMARYSPECTRUM_CTRL_EXEC_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SUMMARYSPECTRUMVIEW_CENTER_BUTTON)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	UpdateWindow();

	// �^�C�}�o�^
	SetTimer(1, SPECTRUM_INTERVAL, NULL);

	if (m_bChgDlg)
	{
		CPoint pt = theApp.GetWindowTmpPoint();
		SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
	else
	{
		CenterWindowEx();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# CENTER FREQ OFFSET�X�s���R���g���[���ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumviewCenterfreqoffsetSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_spectrumSet.OnDeltaposSpectrumviewSpin4(pNMHDR, pResult);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���͒[���Z�X�s���R���g���[���ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumInputexchgspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_spectrumSet.OnDeltaposSpectrumInputexchgspin(pNMHDR, pResult);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ATT�l�X�s���R���g���[���ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnDeltaposSummaryspectrumAttspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_spectrumSet.OnDeltaposSpectrumAttspin(pNMHDR, pResult);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �c�X�N���[���o�[�N���b�N���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_spectrumSet.VScroll(nSBCode, nPos, pScrollBar);

	CDialogBase::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYmaxEdit()
{
	m_verticalSet.OnEnChangeVMax();
}


void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYminEdit()
{
	m_verticalSet.OnEnChangeVMin();
}


void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewYgridEdit()
{
	m_verticalSet.OnEnChangeVGrid();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���S���g���I�t�Z�b�g�̃G�f�B�b�g�ύX���ɌĂ΂��

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumviewCenterfreqoffsetEdit()
{
	m_spectrumSet.OnEnChangeSpectrumviewEdit7();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���͒[���Z�l�̃G�f�B�b�g�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumInputexchgedit()
{
	m_spectrumSet.OnEnChangeSpectrumInputexchgedit();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ATT�l�̃G�f�B�b�g�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnEnChangeSummaryspectrumAttedit()
{
	m_spectrumSet.OnEnChangeSpectrumAttedit();
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ��u���V�̃n���h����Ԃ�

@param	pDC�F�q�E�B���h�E�̃f�B�X�v���C�R���e�L�X�g�̃|�C���^
@param	pWnd�F�F���i�[���ꂽ�R���g���[���̃|�C���^
@param	nCtrlColor�F�R���g���[���̎��
@retval	�R���g���[���̔w�i��h��Ԃ��u���V�̃n���h��

*/
/*============================================================================*/
HBRUSH CSummarySpectrumPropertiesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	m_spectrumSet.CtlColor(pDC, pWnd, nCtlColor);
	m_verticalSet.CtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �O���t�I���R���{�{�b�N�X�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummarygrapthSelectgraphCombo()
{
	CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(m_nStationID, m_ActiveView);

	int nCurSel = m_wndObjectCombo.GetCurSel();

	if (nCurSel == prop.nGraphIdx)
	{
		// �������Ȃ�
		return;
	}

	// �~�j�g�����h�I�����́A�ݒ�_�C�A���O��ς���B
	if (nCurSel != eSummaryWndSetType_XSP && nCurSel != eSummaryWndSetType_KASP)
	{
		CString title;
		GetWindowText(title);
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�g�����h�ݒ�E�B���h�E�ɐ؂�ւ��܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;

		// �v���p�e�B���e���g�����h�ɕύX����
		prop.nGraphIdx = nCurSel;
		for (int i = 0; i < MAX_SUMMARYTDATA; i++)
		{
			prop.dataName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSigName;
			prop.dataGName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSimpleSigName;
			prop.dataUnit[i] = mListSummarySig[m_nStationID][nCurSel][i].strUnit;
		}
		// �v���p�e�B�f�[�^�Z�b�g
		theApp.GetSatelliteData().SetSummaryPropertiesData(m_nStationID, m_ActiveView, prop);

		CRect rect;
		GetWindowRect(&rect);
		theApp.SetWindowTmpPoint(rect.left, rect.top);

		// �g�����h�O���t�؂�ւ��ʒm
		theApp.m_pMainWnd->PostMessage(eMessage_InitSummaryWnd, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
		// �T�}���g�����h�v���p�e�B�_�C�A���O���J���ʒm
		theApp.m_pMainWnd->PostMessage(eMessage_OpenSummaryTrendSetDlgChg, (WPARAM)m_pSummaryProp, (LPARAM)this);
	}
	else
	{
		// �X�y�N�g�����̏ꍇ
		prop.nGraphIdx = nCurSel;
		// �v���p�e�B�f�[�^�Z�b�g
		theApp.GetSatelliteData().SetSummaryPropertiesData(m_nStationID, m_ActiveView, prop);

		CRect rect;
		GetWindowRect(&rect);
		theApp.SetWindowTmpPoint(rect.left, rect.top);

		// �g�����h�O���t�؂�ւ��ʒm
		theApp.m_pMainWnd->PostMessage(eMessage_InitSummaryWnd, (WPARAM)m_nParentID, (LPARAM)m_ActiveView);
		// �T�}���X�y�N�g�����v���p�e�B�_�C�A���O���J���ʒm
		theApp.m_pMainWnd->PostMessage(eMessage_OpenSummarySpectrumSetDlgChg, (WPARAM)m_pSummaryProp, (LPARAM)this);
	}
	// �g�����h�ݒ�_�C�A���O�����
	m_bChgGraphPropDlg = TRUE;
	PostMessage(WM_CLOSE);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# MONITOR POINT�R���{�{�b�N�X���I��ύX���ꂽ�Ƃ��ɌĂ΂��

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumviewMonpointCombo()
{
	m_spectrumSet.OnCbnSelchangeSpectrumviewCombo1();
	if (this->GetSafeHwnd())
	{
		m_spectrumSet.m_CCBoxMonitorPoint.Invalidate();
	}
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# BAND WIDTH�R���{�{�b�N�X���I��ύX���ꂽ�Ƃ��ɌĂ΂��

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnCbnSelchangeSummaryspectrumviewBandwidthCombo()
{
	m_spectrumSet.OnCbnSelchangeSpectrumviewCombo2();
	if (this->GetSafeHwnd())
	{
		m_spectrumSet.m_CCBoxBandWidth.Invalidate();
	}
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# ���j���[����̃t�@�C���N���[�Y���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CSummarySpectrumPropertiesDlg

-# �E�B���h�E����鎞�ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnClose()
{
	if (!m_bChgGraphPropDlg)
	{
		CString title;
		GetWindowText(title);

		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	CDialogEx::OnClose();
}


/*============================================================================*/
/*! CSpectrumView

-# �^�C�}�C�x���g�������ɌĂяo�����

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnTimer(UINT_PTR nIDEvent)
{
	// ����X�V
	m_spectrumSet.UpdateCtrl();

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSpectrumView

-# �E�B���h�E�j�����ɌĂяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSummarySpectrumPropertiesDlg::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}
