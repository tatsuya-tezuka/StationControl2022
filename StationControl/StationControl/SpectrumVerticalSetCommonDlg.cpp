/*============================================================================*/
/*! SpectrumVerticalSetCommonDlg.cpp

-# �X�y�N�g�����c���ݒ苤�ʉ��
*/
/*============================================================================*/
// SpectrumVerticalSetCommonDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SpectrumVerticalSetCommonDlg.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"

extern COLORREF Spectrum_colText[5];

// CSpectrumVerticalSetCommonDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSpectrumVerticalSetCommonDlg, CDialogBase)

/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# �R���X�g���N�^

@param	IDD�FIDD
@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSpectrumVerticalSetCommonDlg::CSpectrumVerticalSetCommonDlg(UINT IDD, CWnd* pParent /*=NULL*/)
: CDialogBase(IDD, pParent)
{

}

/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSpectrumVerticalSetCommonDlg::~CSpectrumVerticalSetCommonDlg()
{
}

void CSpectrumVerticalSetCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpectrumVerticalSetCommonDlg, CDialogBase)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSpectrumVerticalSetCommonDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# �_�C�A���O����������

@param  nStationID�F��ID
@param  nViewID�F�r���[ID
@param  nGraphType�F�O���t�^�C�v
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CSpectrumVerticalSetCommonDlg::InitializeDlg(UINT nStationID, UINT nViewID, UINT nGraphType)
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

	// �R���{�{�b�N�X������
	m_bLastAutoScale = pCSectrumData->GetAutoScale();

	if (m_bLastAutoScale)
		m_CCBoxAutoScaleOnOff.SetCurSel(SPECTRUM_AUTOSCALE_ON);
	else
		m_CCBoxAutoScaleOnOff.SetCurSel(SPECTRUM_AUTOSCALE_OFF);
	m_CCBoxAutoScaleOnOff.SetSelectedTextColor(m_colText[eBlack]);

	// �G�f�B�b�g�{�b�N�X, �X�s���R���g���[��������
	m_CEditMax.DisableMask();
	m_CEditMax.SetValidChars(_T("0123456789,-"));
	m_CEditMin.DisableMask();
	m_CEditMin.SetValidChars(_T("0123456789,-"));
	m_CEditGrid.DisableMask();
	m_CEditGrid.SetValidChars(_T("0123456789,"));

	CString str;

	m_gl_last_y_grid_max = pCSectrumData->GetVerticalMax();
	str.Format(_T("%d"), m_gl_last_y_grid_max);
	m_CEditMax.SetWindowTextW(str);

	m_gl_last_y_grid_min = pCSectrumData->GetVerticalMin();
	str.Format(_T("%d"), m_gl_last_y_grid_min);
	m_CEditMin.SetWindowTextW(str);

	m_gl_last_y_grid_val = pCSectrumData->GetVerticalGrid();
	str.Format(_T("%d"), m_gl_last_y_grid_val);
	m_CEditGrid.SetWindowTextW(str);

	SetEnableVAxis(!m_bLastAutoScale);

	// �F�ݒ�
	m_colEditMax = m_colText[eBlack];
	m_colEditMin = m_colText[eBlack];
	m_colEditGrid = m_colText[eBlack];

	return TRUE;
}

/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# �c���̃G�f�B�b�g�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
BOOL CSpectrumVerticalSetCommonDlg::SetVerticalValues()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return FALSE;
	}

	if (m_CEditMax.GetSafeHwnd() == NULL ||
		m_CEditMin.GetSafeHwnd() == NULL ||
		m_CEditGrid.GetSafeHwnd() == NULL)
		return FALSE;

	if (this->GetSafeHwnd() != NULL)
		UpdateData(TRUE);

	CString str;
	long l_y_grid_max, l_y_grid_min, l_y_grid_val;

	// �O���b�h�ő�l�`�F�b�N
	m_CEditMax.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	l_y_grid_max = _ttol(str);

	if (l_y_grid_max > m_VMax)
	{
		// �G���[�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_MAX_ILLEGAL) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// �O���b�h�ŏ��l�`�F�b�N
	m_CEditMin.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	l_y_grid_min = _ttol(str);

	if (l_y_grid_min < m_VMin)
	{
		// �G���[�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_MIN_ILLEGAL) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else if (l_y_grid_max <= l_y_grid_min)
	{
		// �G���[�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_MIN_TOO_LARGE) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// �O���b�h�l�`�F�b�N
	m_CEditGrid.GetWindowText(str);
	str.Trim();
	str.Remove(_T(','));
	l_y_grid_val = _ttol(str);

	if (l_y_grid_val < 1)
	{
		// �G���[�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_VALUE_TOO_SMALL) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else if (l_y_grid_max - l_y_grid_min < l_y_grid_val)
	{
		// �G���[�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_YGRID_VALUE_TOO_LARGE) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// �m�F�_�C�A���O�\��
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("���s���܂��B\n��낵���ł����H"), _T("�O���b�h�ݒ�"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return FALSE;
	}

	pCSectrumData->SetVerticalMax(l_y_grid_max);
	pCSectrumData->SetVerticalMin(l_y_grid_min);
	pCSectrumData->SetVerticalGrid(l_y_grid_val);

	return TRUE;
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# �c���ݒ�̗L�������؂�ւ����ɌĂ΂��

@param  bEnable�FTRUE:�L�� / FALSE:����
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::SetEnableVAxis(BOOL bEnable)
{
	m_CEditMax.EnableWindow(bEnable);
	m_CEditMin.EnableWindow(bEnable);
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# �X�y�N�g�����ݒ�f�[�^�擾

@param	�Ȃ�
@retval	�X�y�N�g�����ݒ�f�[�^

*/
/*============================================================================*/
CSpectrumViewData* CSpectrumVerticalSetCommonDlg::GetSpectrumViewData()
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
/*! CSpectrumVerticalSetCommonDlg

-# �I�[�g�X�P�[���I��ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnCbnSelchangeSpectrumverticalsetCombo()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	int nCurSel = m_CCBoxAutoScaleOnOff.GetCurSel();
//	m_CCBoxAutoScaleOnOff.SetSelectedTextColor(m_colText[eOrange]);
	BOOL	bAutoScale;
	if (nCurSel == SPECTRUM_AUTOSCALE_ON)
		bAutoScale = TRUE;
	else
		bAutoScale = FALSE;

	pCSectrumData->SetAutoScale(bAutoScale);

	SetEnableVAxis(!bAutoScale);
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# OK�{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnBnClickedOk()
{
	OnCbnSelchangeSpectrumverticalsetCombo();
	(void)SetVerticalValues();

	m_CCBoxAutoScaleOnOff.SetSelectedTextColor(m_colText[eBlack]);
	m_colEditMax = m_colText[eBlack];
	m_colEditMin = m_colText[eBlack];
	m_colEditGrid = m_colText[eBlack];
}


/*============================================================================*/
/*! CSpectrumVerticalSetCommonDlg

-# �L�����Z���{�^���N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnBnClickedCancel()
{
	CSpectrumViewData* pCSectrumData = GetSpectrumViewData();
	if (pCSectrumData == NULL)
	{
		return;
	}

	pCSectrumData->SetAutoScale(m_bLastAutoScale);

	pCSectrumData->SetVerticalMax(m_gl_last_y_grid_max);
	pCSectrumData->SetVerticalMin(m_gl_last_y_grid_min);
	pCSectrumData->SetVerticalGrid(m_gl_last_y_grid_val);

	m_CCBoxAutoScaleOnOff.SetSelectedTextColor(m_colText[eBlack]);
	m_colEditMax = m_colText[eBlack];
	m_colEditMin = m_colText[eBlack];
	m_colEditGrid = m_colText[eBlack];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �ő�l�̃G�f�B�b�g�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnEnChangeVMax()
{
//	m_colEditMax = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �ŏ��l�̃G�f�B�b�g�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnEnChangeVMin()
{
//	m_colEditMin = m_colText[eOrange];
}


/*============================================================================*/
/*! CSpectrumCommonDlg

-# �O���b�h�l�̃G�f�B�b�g�ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CSpectrumVerticalSetCommonDlg::OnEnChangeVGrid()
{
//	m_colEditGrid = m_colText[eOrange];
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
HBRUSH CSpectrumVerticalSetCommonDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CSpectrumVerticalSetCommonDlg::CtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (*pWnd == m_CEditMax)
		{
			pDC->SetTextColor(m_colEditMax);
		}
		else if (*pWnd == m_CEditMin)
		{
			pDC->SetTextColor(m_colEditMin);
		}
		else if (*pWnd == m_CEditGrid)
		{
			pDC->SetTextColor(m_colEditGrid);
		}
	}
}

