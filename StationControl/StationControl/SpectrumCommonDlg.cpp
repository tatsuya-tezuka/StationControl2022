/*============================================================================*/
/*! CSpectrumCommonDlg.cpp

-# �X�y�N�g�����ݒ苤�ʉ��
*/
/*============================================================================*/
// SpectrumCommonDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SpectrumCommonDlg.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"
#include "..\Utility\SshShell.h"

extern CString spectrum_gpsz_moni_point_str[3];
extern CString spectrum_gpsz_bandw_str[23];
extern CString spectrum_gsz_dbm_str[1];
extern CSpectrumViewData::BandWidth_t spectrum_gast_bandw[];

extern COLORREF Spectrum_colText[5];
extern CString Spectrum_strFreqUnit[3];
extern CString gsz_center_str[1];

// CSpectrumCommonDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSpectrumCommonDlg, CDialogBase)

/*============================================================================*/
/*! CSpectrumCommonDlg

-# �R���X�g���N�^

@param	IDD�FIDD
@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSpectrumCommonDlg::CSpectrumCommonDlg(UINT IDD, CWnd* pParent /*=NULL*/)
: CDialogBase(IDD, pParent)
, m_strPeakLevelUnit(_T(""))
, m_strPeakFreqUnit(_T(""))
, m_strHLValue(_T(""))
, m_strHCValue(_T(""))
, m_strHRValue(_T(""))
, m_strDataTime(_T(""))
, m_strPeakLevel(_T(""))
, m_strPeakFreq(_T(""))
, m_pView(NULL)
, m_bMPModified(FALSE)
, m_bBWModified(FALSE)
, m_bCFOfstModified(FALSE)
, m_bMPMListOpen(FALSE)
, m_bBWMListOpen(FALSE)
, m_bCFOfstMListOpen(FALSE)
{

}

/*============================================================================*/
/*! CSpectrumCommonDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSpectrumCommonDlg::~CSpectrumCommonDlg()
{
}

void CSpectrumCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpectrumCommonDlg, CDialogBase)
END_MESSAGE_MAP()


// CSpectrumCommonDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �_�C�A���O����������

@param  nStationID�F��ID
@param  nViewID�F�r���[ID
@param  nGraphType�F�O���t�^�C�v
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CSpectrumCommonDlg::InitializeDlg(UINT nStationID, UINT nViewID, UINT nGraphType)
{
	m_nStationID = nStationID;
	m_nViewID = nViewID;
	m_nGraphType = nGraphType;

	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return FALSE;
	}

	for (int i = 0; i < eColorNum; i++)
	{
		m_colText[i] = Spectrum_colText[i];
	}

	// �ݒ菉����
	pCSectrumData->m_gl_moni_point_sel = SPECTRUM_MONIP_70MIF_SEL;
	pCSectrumData->m_gl_bw_sel = 0;
	pCSectrumData->m_gd_freq_ofs_in = 0.0;
	pCSectrumData->m_gd_peak_freq = 0.0;
	pCSectrumData->m_gd_peak_level = 0.0;

#ifdef _ENGVALCNV_TREND
	pCSectrumData->GetSpectrumData();
#else
	// �_�~�[�f�[�^����
	pCSectrumData->m_gsz_time_tag = _T("123 12:34:56");
	pCSectrumData->m_gsz_bandw_sts = _T("32MHz");
	pCSectrumData->m_gsz_moni_point = _T("70M_IF");
	pCSectrumData->m_gd_freq_ofs = 0.0;
	pCSectrumData->m_gd_center_freq = 0.0;

	pCSectrumData->m_nDummyOffset = 0;

	// �_�~�[�I�t�Z�b�g����
	pCSectrumData->CreateDummyOffset();
#endif /* _ENGVALCNV_TREND */

	//	// �G�f�B�b�g�}�X�N
	m_CEditCenterFreqOffset.DisableMask();
	m_CEditCenterFreqOffset.SetValidChars(_T("0123456789,.-"));
	m_CEditInputExchg.DisableMask();
	m_CEditInputExchg.SetValidChars(_T("0123456789,.-"));
	m_CEditAtt.DisableMask();
	m_CEditAtt.SetValidChars(_T("0123456789,.-"));

	CString str;
	// ���͊��Z�ݒ�
	str = CSatelliteData::AddCanma(pCSectrumData->m_stVScale.gd_y_offset_val, 2);
	m_CEditInputExchg.SetWindowTextW(str);
	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_y_offset_att, 2);
	m_CEditAtt.SetWindowTextW(str);
	// �F�ݒ�
//	m_colEditMax = m_colText[eBlack];
//	m_colEditMin = m_colText[eBlack];
//	m_colEditGrid = m_colText[eBlack];
	m_colEditInputExchg = m_colText[eBlack];
	m_colEditAtt = m_colText[eBlack];

	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
	m_CEditCenterFreqOffset.SetWindowText(str);
	m_colEditCenterFreqOffset = m_colText[eBlack];

	// �X�s���R���g���[��������
	m_CSpinCenterFreqOffset.SetRange32((int)(SPECTRUM_FREQ_OFS_MIN), (int)(SPECTRUM_FREQ_OFS_MAX));
	m_CSpinInputExchg.SetRange32(SPECTRUM_INPUTMIN, SPECTRUM_INPUTMAX);
	m_CSpinAtt.SetRange32(SPECTRUM_INPUTMIN, SPECTRUM_INPUTMAX);

	// �R���{�{�b�N�X������
	for (int i = 0; i < sizeof(spectrum_gpsz_moni_point_str) / sizeof(spectrum_gpsz_moni_point_str[0]); i++)
	{
		m_CCBoxMonitorPoint.InsertString(-1, spectrum_gpsz_moni_point_str[i]);
	}
	m_CCBoxMonitorPoint.SetCurSel(SPECTRUM_MONIP_70MIF_SEL);
	m_nMonitorPointCurSel = m_CCBoxMonitorPoint.GetCurSel();
	m_CCBoxMonitorPoint.SetSelectedTextColor(m_colText[eBlack]);
	for (int i = 0; i < sizeof(spectrum_gpsz_bandw_str) / sizeof(spectrum_gpsz_bandw_str[0]); i++)
	{
		m_CCBoxBandWidth.InsertString(-1, spectrum_gpsz_bandw_str[i]);
	}
	m_CCBoxBandWidth.SetCurSel(0);
	m_nBandWidthCurSel = m_CCBoxBandWidth.GetCurSel();
	m_CCBoxBandWidth.SetSelectedTextColor(m_colText[eBlack]);

	m_strPeakLevelUnit = spectrum_gsz_dbm_str[edBm];
	m_strPeakFreqUnit = Spectrum_strFreqUnit[eMHz];
	m_strHLValue = spectrum_gast_bandw[0].sz_low;
	m_strHCValue = gsz_center_str[0];
	m_strHRValue = spectrum_gast_bandw[0].sz_upp;

	m_bMPModified = FALSE;
	m_bBWModified = FALSE;
	m_bCFOfstModified = FALSE;
	m_bMPMListOpen = FALSE;
	m_bBWMListOpen = FALSE;
	m_bCFOfstMListOpen = FALSE;

	return TRUE;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# CENTER FREQ OFFSET�̃X�s���R���g���[���ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnDeltaposSpectrumviewSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// ���S���g���I�t�Z�b�g�l��ύX����
	if (m_CSpinCenterFreqOffset.GetSafeHwnd() == NULL)
	{
		return;
	}

	CString str;
	m_CEditCenterFreqOffset.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	double d_freq = _ttof(str);

	if (d_freq < SPECTRUM_FREQ_OFS_MIN)
	{
		d_freq = SPECTRUM_FREQ_OFS_MIN;
	}
	else if (d_freq > SPECTRUM_FREQ_OFS_MAX)
	{
		d_freq = SPECTRUM_FREQ_OFS_MAX;
	}

	//	m_CSpinCenterFreqOffset.SetPos32((int)round(d_freq * 100.0));
	m_CSpinCenterFreqOffset.SetPos32((int)round(d_freq));

	*pResult = 0;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �c�X�N���[���o�[�N���b�N���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	VScroll(nSBCode, nPos, pScrollBar);

	CDialogBase::OnVScroll(nSBCode, nPos, pScrollBar);
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �c�X�N���[���o�[�N���b�N���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::VScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == SB_THUMBPOSITION)
	{
		if ((m_CSpinCenterFreqOffset.m_hWnd == pScrollBar->m_hWnd))
		{
			CString str;
			int val = m_CSpinCenterFreqOffset.GetPos32();
			str = CSatelliteData::AddCanma((double)val, 3);
			m_CEditCenterFreqOffset.SetWindowText(str);
		}
		else if ((m_CSpinInputExchg.m_hWnd == pScrollBar->m_hWnd))
		{
			CString str;
			int val = m_CSpinInputExchg.GetPos32();
			str = CSatelliteData::AddCanma((double)val, 2);
			m_CEditInputExchg.SetWindowText(str);
		}
		else if ((m_CSpinAtt.m_hWnd == pScrollBar->m_hWnd))
		{
			CString str;
			int val = m_CSpinAtt.GetPos32();
			str = CSatelliteData::AddCanma((double)val, 2);
			m_CEditAtt.SetWindowText(str);
		}
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �Z���^�[�{�^���������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumviewButton1()
{
	CString strTitle;
	CString strMsg;
	if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
		return;
	if (strMsg.LoadString(IDS_CHECK_CTRL_MSG) == FALSE)
		return;
	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	// ���S�I�t�Z�b�g�����Z�b�g����
	ResetCenterFreqOffset();
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���S���g���I�t�Z�b�g�l���擾����

@param	�Ȃ�
@retval	�O���b�h�l

*/
/*============================================================================*/
void CSpectrumCommonDlg::ResetCenterFreqOffset()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;
	m_CCBoxMonitorPoint.GetLBText(m_nMonitorPointCurSel, str);
	double d_freq;
	if (pCSectrumData->m_gsz_moni_point.Compare(str) == 0)
	{
		if (pCSectrumData->m_gd_peak_freq != 0.0){
#if(1)
			d_freq = pCSectrumData->m_gd_peak_freq - pCSectrumData->m_gd_center_freq;
#else
			d_freq = m_gd_peak_freq - 70000000.0;
#endif
		}
		else {
			d_freq = 0.0;
		}
	}
	else {
		d_freq = pCSectrumData->m_gd_peak_freq;
	}
	d_freq = (double)((long long)(d_freq *  (double)1000.0)) / (double)1000.0;
//	pCSectrumData->m_gd_freq_ofs_in = d_freq;
//	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
//	m_CEditCenterFreqOffset.SetWindowText(str);
//	m_colEditCenterFreqOffset = m_colText[eBlack];

	// �����ő��u�ɕύX����FreqOffset�̐���𑗂�
	CString strVal;
	if (m_nStationID == eStation_Usuda642)
	{
		// 54m�̏ꍇ
		strVal = ExchgCtrlValSpecFreOffset54m(d_freq);
	}
	else
	{
		// 54m�ȊO�̏ꍇ
		strVal.Format(_T("%.3f"), d_freq);
	}
	if (SendShell(CString(CTL_FREQ_OFFSET_NAME), strVal) == 0)
	{
		m_bCFOfstModified = FALSE;
		UpdateCtrl();
	}

//	// �_�~�[�I�t�Z�b�g����
//	pCSectrumData->CreateDummyOffset();
//
//	// ���Z�b�g�����I�t�Z�b�g��0�ɖ߂�
//	pCSectrumData->m_gd_freq_ofs_in = 0.0;
//	CString str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
//	m_CEditCenterFreqOffset.SetWindowText(str);
//	m_colEditCenterFreqOffset = m_colText[eBlack];
	
	if (this->GetSafeHwnd())
	{
		Invalidate();
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���g���l�̒P�ʕϊ�

@param	src�F�ϊ��O�̎��g���l
@param	dst�F�ϊ���̎��g���l
@retval	�P�� 0�FHz / 1�FKHz / 2�FMHz

*/
/*============================================================================*/
int CSpectrumCommonDlg::ExchangeFreqUnit(double& src, double& dst)
{
	dst = src;
	for (int i = 0; i <= eMHz; i++)
	{
		if (dst > 1000.0)
		{
			dst = dst / 1000.0;
		}
		else
		{
			return i;
		}
	}

	return eMHz;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �V�F�����M

@param	strCtrl�F�ݔ�������
@param	strVal�F���M�f�[�^������
@retval	�Ȃ�

*/
/*============================================================================*/
int CSpectrumCommonDlg::SendShell(CString& strCtrl, CString& strVal)
{
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = SEND_CONTROL_SH;
	stParam.strRemShellParam.Format(_T("%s %s %s"), (LPCTSTR)CString(mStationSimpleString[m_nStationID]).MakeLower(), (LPCTSTR)strCtrl, (LPCTSTR)strVal);

	int err = CSshShell::ExecShell(stParam);
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CSpectrumCommonDlg"), _T("SendShell"), _T("debug"), strDebug, nLogEx::debug);
	}

	return err;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �X�y�N�g�����ݒ�f�[�^�擾

@param	�Ȃ�
@retval	�X�y�N�g�����ݒ�f�[�^

*/
/*============================================================================*/
CSpectrumViewData* CSpectrumCommonDlg::GetSpectrumViewData()
{
	CSpectrumViewData* pCSectrumData;
	if (m_nGraphType == eSpectrumGraphType_Normal)
	{
		pCSectrumData = &theApp.GetSatelliteData().GetSpectrumViewData();
	}
	else
	{
		pCSectrumData = &theApp.GetSatelliteData().GetSummarySpectrumViewData(m_nStationID, m_nViewID);
	}

	return pCSectrumData;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���S���g���I�t�Z�b�g�̃G�f�B�b�g�ύX���ɌĂ΂��

@param	�Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnEnChangeSpectrumviewEdit7()
{
	// �X�V����
	m_bCFOfstModified = TRUE;
	m_colEditCenterFreqOffset = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���͒[���Z�l�̃G�f�B�b�g�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnEnChangeSpectrumInputexchgedit()
{
	m_colEditInputExchg = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ATT�l�̃G�f�B�b�g�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnEnChangeSpectrumAttedit()
{
	m_colEditAtt = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���j���[����̃t�@�C���N���[�Y���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSpectrumCommonDlg::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}



/*============================================================================*/
/*! CSpectrumCommonDlg

-# �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ��u���V�̃n���h����Ԃ�

@param	pDC�F�q�E�B���h�E�̃f�B�X�v���C�R���e�L�X�g�̃|�C���^
@param	pWnd�F�F���i�[���ꂽ�R���g���[���̃|�C���^
@param	nCtrlColor�F�R���g���[���̎��
@retval	�R���g���[���̔w�i��h��Ԃ��u���V�̃n���h��

*/
/*============================================================================*/
HBRUSH CSpectrumCommonDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	CtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ�

@param	pDC�F�q�E�B���h�E�̃f�B�X�v���C�R���e�L�X�g�̃|�C���^
@param	pWnd�F�F���i�[���ꂽ�R���g���[���̃|�C���^
@param	nCtrlColor�F�R���g���[���̎��
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::CtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (*pWnd == m_CEditCenterFreqOffset)
		{
			pDC->SetTextColor(m_colEditCenterFreqOffset);
		}
		else if (*pWnd == m_CEditInputExchg)
		{
			pDC->SetTextColor(m_colEditInputExchg);
		}
		else if (*pWnd == m_CEditAtt)
		{
			pDC->SetTextColor(m_colEditAtt);
		}
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���s�{�^���������ɂ�΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumExecButton()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;
	long l_y_grid_max, l_y_grid_min, l_y_grid_val;
	double d_freq;

	if (m_CCBoxMonitorPoint.GetSafeHwnd() == NULL ||
		m_CCBoxBandWidth.GetSafeHwnd() == NULL ||
		m_CEditCenterFreqOffset.GetSafeHwnd() == NULL)
	{
		return;
	}

	l_y_grid_max = 0;
	l_y_grid_min = 0;
	l_y_grid_val = 0;

	// ���S���g���I�t�Z�b�g�`�F�b�N
	m_CEditCenterFreqOffset.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	d_freq = _ttof(str);

	if ((d_freq < SPECTRUM_FREQ_OFS_MIN) || (d_freq > SPECTRUM_FREQ_OFS_MAX))
	{
		// �G���[�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_SPECTRUMVIEW_CENTERFREQOFFSET_ERR) == FALSE)
			return;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return;
	}

	// �m�F�_�C�A���O�\��
	{
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_CHECK_CTRL_MSG) == FALSE)
			return;
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, strMsg, strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	// �ݒ�l�̔��f
	d_freq = (double)((long long)(d_freq *  (double)1000.0)) / (double)1000.0;

	int nMonitorPointCurSel = m_CCBoxMonitorPoint.GetCurSel();
	int nBandWidthCurSel = m_CCBoxBandWidth.GetCurSel();

//ifdef _DEBUG
//	pCSectrumData->m_dummy_moni_point = nMonitorPointCurSel;
//	pCSectrumData->m_dummy_bandw = nBandWidthCurSel;
//	pCSectrumData->m_dummy_freq_ofs = d_freq;
//	pCSectrumData->m_dummy_center_freq = d_freq;
//endif

	// MonitorPoint�̐���𑗂�
	if (SendShell(CString(CTL_MONI_POINT_NAME), spectrum_gpsz_moni_point_str[nMonitorPointCurSel]) == 0)
	{
		m_bMPModified = FALSE;
		UpdateCtrl();
	}

	// BandWidth�̐���𑗂�
	if (SendShell(CString(CTL_MEASURE_BW_NAME), spectrum_gast_bandw[nBandWidthCurSel].sz_sts) == 0)
	{
		m_bBWModified = FALSE;
		UpdateCtrl();
	}

	// FreqOffset�̐���𑗂�
	CString strVal;
	if (m_nStationID == eStation_Usuda642)
	{
		// 54m�̏ꍇ
		strVal = ExchgCtrlValSpecFreOffset54m(d_freq);
	}
	else
	{
		// 54m�ȊO�̏ꍇ
		strVal.Format(_T("%.3f"), d_freq);
	}
	if (SendShell(CString(CTL_FREQ_OFFSET_NAME), strVal) == 0)
	{ 
		m_bCFOfstModified = FALSE;
		UpdateCtrl();
	}

//#ifdef _DEBUG
//	// �_�~�[�I�t�Z�b�g����
//	pCSectrumData->CreateDummyOffset();
//#endif

	if (this->GetSafeHwnd())
	{
		Invalidate();
	}

//	// �t�@�C���ۑ�
//	theApp.GetSatelliteData().SpectrumFileWrite(m_nStationID);
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# 54m�̃X�y�N�g�������g���I�t�Z�b�g�l�𐧌䕶����ɕϊ�����

@param	freq�F���g��
@retval	���䕶����

*/
/*============================================================================*/
CString CSpectrumCommonDlg::ExchgCtrlValSpecFreOffset54m(double freq)
{
	CString strVal;

	// bit40�ɕ���
	int signVal;
	double d_freqAbs = fabs(freq);
	if (freq >= 0)
	{
		signVal = 1;
	}
	else
	{
		signVal = 0;
	}

	// bit12�`39��������
	int intVal = (int)d_freqAbs;

	// bit0�`11��������
	double decVal = d_freqAbs - (double)intVal;
	// �����_�ȉ��R���̂ݗL��
	decVal *= 1000;

	strVal.Format(_T("0x%01d%07d%03d"), signVal, intVal, (int)round(decVal));

	return strVal;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���Z�b�g�{�^���������ɂ�΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumResetButton()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;
	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
	m_CEditCenterFreqOffset.SetWindowText(str);
	m_colEditCenterFreqOffset = m_colText[eBlack];

	m_CCBoxMonitorPoint.SetCurSel(m_nMonitorPointCurSel);
	m_CCBoxMonitorPoint.SetSelectedTextColor(m_colText[eBlack]);

	m_CCBoxBandWidth.SetCurSel(m_nBandWidthCurSel);
	m_CCBoxBandWidth.SetSelectedTextColor(m_colText[eBlack]);

	m_bMPModified = FALSE;
	m_bBWModified = FALSE;
	m_bCFOfstModified = FALSE;
	m_bMPMListOpen = FALSE;
	m_bBWMListOpen = FALSE;
	m_bCFOfstMListOpen = FALSE;

	if (this->GetSafeHwnd())
	{
		Invalidate();
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���͒[���Z���s�{�^���������ɂ�΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumInputexchgExecButton()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;
	double d_y_offset_val, d_y_offset_att;

	// Y���I�t�Z�b�g�l�`�F�b�N
	m_CEditInputExchg.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	d_y_offset_val = _ttof(str);

	if (d_y_offset_val > (double)SPECTRUM_INPUTMAX || d_y_offset_val < (double)SPECTRUM_INPUTMIN)
	{
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_YOFFSET_VALUE_ILLEGAL) == FALSE)
			return;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return;
	}

	// ATT�l�`�F�b�N
	m_CEditAtt.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	d_y_offset_att = _ttof(str);

	if (d_y_offset_att > (double)SPECTRUM_INPUTMAX || d_y_offset_att < (double)SPECTRUM_INPUTMIN)
	{
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return;
		if (strMsg.LoadString(IDS_ATT_VALUE_ILLEGAL) == FALSE)
			return;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return;
	}

	// �m�F�_�C�A���O�\��
	{
		CString strTitle;
		if (strTitle.LoadString(IDS_INPUTEXCHG) == FALSE)
			return;
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("���s���܂��B\n��낵���ł����H"), strTitle, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	// �ݒ�l�̔��f
	d_y_offset_val = (double)((long long)(d_y_offset_val *  (double)100.0)) / (double)100.0;
	pCSectrumData->m_stVScale.gd_y_offset_val = d_y_offset_val;
	d_y_offset_att = (double)((long long)(d_y_offset_att *  (double)100.0)) / (double)100.0;
	pCSectrumData->m_gd_y_offset_att = d_y_offset_att;

	str = CSatelliteData::AddCanma(pCSectrumData->m_stVScale.gd_y_offset_val, 2);
	m_CEditInputExchg.SetWindowText(str);
	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_y_offset_att, 2);
	m_CEditAtt.SetWindowText(str);

	m_colEditInputExchg = m_colText[eBlack];
	m_colEditAtt = m_colText[eBlack];

	if (this->GetSafeHwnd())
	{
		ClearAutoVMaxMin();
		Invalidate();
	}

//	// �t�@�C���ۑ�
//	theApp.GetSatelliteData().SpectrumFileWrite(m_nStationID);
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���͒[���Z���Z�b�g�{�^���������ɂ�΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnBnClickedSpectrumInputexchgResetButton()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	CString str;

	str = CSatelliteData::AddCanma(pCSectrumData->m_stVScale.gd_y_offset_val, 2);
	m_CEditInputExchg.SetWindowText(str);
	m_colEditInputExchg = m_colText[eBlack];

	str = CSatelliteData::AddCanma(pCSectrumData->m_gd_y_offset_att, 2);
	m_CEditAtt.SetWindowText(str);
	m_colEditAtt = m_colText[eBlack];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���͒[���Z�X�s���R���g���[���ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnDeltaposSpectrumInputexchgspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// ���͒[���Z�l��ύX����
	if (m_CSpinInputExchg.GetSafeHwnd() == NULL)
	{
		return;
	}

	CString str;
	m_CEditInputExchg.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	double val = _ttof(str);

	m_CSpinInputExchg.SetPos32((int)round(val));

	*pResult = 0;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ATT�l�X�s���R���g���[���ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnDeltaposSpectrumAttspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// ATT�l��ύX����
	if (m_CSpinAtt.GetSafeHwnd() == NULL)
	{
		return;
	}

	CString str;
	m_CEditAtt.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	double val = _ttof(str);

	m_CSpinAtt.SetPos32((int)round(val));

	*pResult = 0;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# MONITOR POINT�R���{�{�b�N�X���I��ύX���ꂽ�Ƃ��ɌĂ΂��

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnSelchangeSpectrumviewCombo1()
{
	// �X�V����
	m_bMPModified = TRUE;
	m_CCBoxMonitorPoint.SetSelectedTextColor(m_colText[eOrange]);

	if (this->GetSafeHwnd())
	{
		m_CCBoxMonitorPoint.Invalidate();
	}
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# BAND WIDTH�R���{�{�b�N�X���I��ύX���ꂽ�Ƃ��ɌĂ΂��

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnSelchangeSpectrumviewCombo2()
{
	// �X�V����
	m_bBWModified = TRUE;
	m_CCBoxBandWidth.SetSelectedTextColor(m_colText[eOrange]);

	if (this->GetSafeHwnd())
	{
		m_CCBoxBandWidth.Invalidate();
	}
}

/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���C�A�E�g�ǂݍ��݁i�ݒ�ǂݍ��݁j

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
bool CSpectrumCommonDlg::Load(CArchive& ar)
{
	return true;
}

/*============================================================================*/
/*! CSpectrumCommonDlg

-# ���C�A�E�g�ۑ��i�ݒ�ۑ��j

@param  �Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
bool CSpectrumCommonDlg::Save(CArchive& ar)
{
	return true;
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# ����X�V

@param  �Ȃ�
@retval	TRUE:�Ď����䂪�擾�ł����^FALSE:����Ď����擾�ł��Ȃ�����

*/
/*============================================================================*/
BOOL CSpectrumCommonDlg::UpdateCtrl()
{
	BOOL ret = FALSE;

	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return ret;
	}

	/* �Ď��f�[�^�擾 */
	if (pCSectrumData->GetSpectrumData() != ERR_RETURN)
	{
		CString str;

		// MonitorPoint�����[�U���ύX���Ă��Ȃ��ꍇ
//		if (m_CCBoxMonitorPoint.GetSelectedTextColor() == m_colText[eBlack])
		if (m_bMPModified == FALSE && m_bMPMListOpen == FALSE)
		{
			for (int i = 0; i < sizeof(spectrum_gpsz_moni_point_str) / sizeof(spectrum_gpsz_moni_point_str[0]); i++)
			{
				// �Ď��������r
				if (pCSectrumData->m_gsz_moni_point.Compare(spectrum_gpsz_moni_point_str[i]) == 0)
				{
					// �C���f�b�N�X�ʒu�ɍ��ق�����ꍇ�`��
					if (m_nMonitorPointCurSel != i)
					{
						// �Ď��l��\��
						m_nMonitorPointCurSel = i;
						m_CCBoxMonitorPoint.SetCurSel(i);
						m_CCBoxMonitorPoint.SetSelectedTextColor(m_colText[eBlack]);
						m_CCBoxMonitorPoint.Invalidate();
					}
					break;
				}
			}
		}

		// BandWidth�����[�U���ύX���Ă��Ȃ��ꍇ
//		if (m_CCBoxBandWidth.GetSelectedTextColor() == m_colText[eBlack])
		if (m_bBWModified == FALSE && m_bBWMListOpen == FALSE)
		{
			for (int i = 0; spectrum_gast_bandw[i].sz_sts[0] != NULL; i++)
			{
				// �Ď��������r
				if (pCSectrumData->m_gsz_bandw_sts.Compare(spectrum_gast_bandw[i].sz_sts) == 0)
				{
					// �C���f�b�N�X�ʒu�ɍ��ق�����ꍇ�`��
					if (m_nBandWidthCurSel != i)
					{
						// �Ď��l��\��
						m_nBandWidthCurSel = i;
						m_CCBoxBandWidth.SetCurSel(i);
						m_CCBoxBandWidth.SetSelectedTextColor(m_colText[eBlack]);
						m_CCBoxBandWidth.Invalidate();
					}
					break;
				}
			}
		}

		// FreqOffset�����[�U���ύX���Ă��Ȃ��ꍇ
//		if (m_colEditCenterFreqOffset == m_colText[eBlack])
		if (m_bCFOfstModified == FALSE)
		{
			// �Ď��l��\��
			str = CSatelliteData::AddCanma(pCSectrumData->m_gd_freq_ofs_in, 3);
			CString strWndtxt;
			m_CEditCenterFreqOffset.GetWindowText(strWndtxt);
			// ��ʂ̕�����ƍ��ق�����ꍇ�`��
			bool bUpdate = false;
			if (strWndtxt.Compare(str) != 0)
			{
				m_CEditCenterFreqOffset.SetWindowText(str);
				bUpdate = true;
			}
			if (m_colEditCenterFreqOffset != m_colText[eBlack])
			{
				m_colEditCenterFreqOffset = m_colText[eBlack];
				bUpdate = true;
			}
			m_bCFOfstModified = FALSE;
			if (bUpdate)
			{
				m_CEditCenterFreqOffset.Invalidate();
			}
		}

		ret = TRUE;
	}

	return ret;
}


/*============================================================================*/
/*! CSpectrumView

-# Monitor Point�̃v���_�E�����X�g���J�����Ƃ��ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnDropdownSpectrumviewCombo1()
{
	// ���X�g���J���Ă���
	m_bMPMListOpen = TRUE;
}


/*============================================================================*/
/*! CSpectrumView

-# Monitor Point�̃v���_�E�����X�g��������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnCloseupSpectrumviewCombo1()
{
	// ���X�g�����Ă���
	m_bMPMListOpen = FALSE;
}


/*============================================================================*/
/*! CSpectrumView

-# Band Width�̃v���_�E�����X�g���J�����Ƃ��ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnDropdownSpectrumviewCombo2()
{
	// ���X�g���J���Ă���
	m_bBWMListOpen = TRUE;
}


/*============================================================================*/
/*! CSpectrumView

-# Band Width�̃v���_�E�����X�g��������ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSpectrumCommonDlg::OnCbnCloseupSpectrumviewCombo2()
{
	// ���X�g�����Ă���
	m_bBWMListOpen = FALSE;
}


