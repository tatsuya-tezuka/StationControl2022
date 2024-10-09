// EquipmentCtrlTime.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrlTime.h"
#include "afxdialogex.h"


// CEquipmentCtrlTime �_�C�A���O

IMPLEMENT_DYNAMIC(CEquipmentCtrlTime, CDialogEx)

CEquipmentCtrlTime::CEquipmentCtrlTime(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEquipmentCtrlTime::IDD, pParent)
	, m_strControlName(_T(""))
	, m_strWindowTitle(_T(""))
	, m_time(0)
	, m_nTimeType(eTimeType_MMSS)
{
	mTimeString.Empty();
}

CEquipmentCtrlTime::~CEquipmentCtrlTime()
{
}

void CEquipmentCtrlTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPT_STATIC, m_strControlName);
	DDX_Control(pDX, IDC_MFCMASKEDEDIT_CTRLREGIST, m_CMaskedEdit);
}


BEGIN_MESSAGE_MAP(CEquipmentCtrlTime, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrlTime::OnBnClickedOk)
//	ON_EN_KILLFOCUS(IDC_MFCMASKEDEDIT_CTRLREGIST, &CEquipmentCtrlTime::OnEnKillfocusMfcmaskededitCtrlregist)
END_MESSAGE_MAP()


// CEquipmentCtrlTime ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CEquipmentCtrlTime

-# ���������ɌĂяo�����B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
BOOL CEquipmentCtrlTime::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CTime time;

	if (!m_strWindowTitle.IsEmpty())
		SetWindowText(m_strWindowTitle);

	switch (m_nTimeType)
	{
	case eTimeType_MSS:
		m_CMaskedEdit.EnableMask(_T("*****"), _T("_____"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890+-: "));
		if (mTimeString.IsEmpty())
		{
			// ����������
			m_time = 0;
			UpdateOffsetTime();
		}
		else
		{
			// �����񂩂�l�擾
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	case eTimeType_MMSS:
		m_CMaskedEdit.EnableMask(_T("******"), _T("______"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890+-: "));
		if (mTimeString.IsEmpty())
		{
			// ����������
			m_time = 0;
			UpdateOffsetTime();
		}
		else
		{
			// �����񂩂�l�擾
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	case eTimeType_HHMMSS:
		m_CMaskedEdit.EnableMask(_T("*********"), _T("_________"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890+-: "));
		if (mTimeString.IsEmpty())
		{
			// ����������
			m_time = 0;
			UpdateOffsetTime();
		}
		else
		{
			// �����񂩂�l�擾
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	case eTimeType_DDDHHMMSS:
		m_CMaskedEdit.EnableMask(_T("ddd dd:dd:dd"), _T("___ __:__:__"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890"));
		if (mTimeString.IsEmpty())
		{
			// ����������
			theApp.GetCurrentUtcTime(time);
			m_time = time.GetTime();
			UpdateOffsetTime();
		}
		else
		{
			// �����񂩂�l�擾
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	case eTimeType_YYYYmmdd:
		m_CMaskedEdit.EnableMask(_T("dddd-dd-dd"), _T("____-__-__"), _T(' '));
		m_CMaskedEdit.SetValidChars(_T("01234567890"));
		if (mTimeString.IsEmpty())
		{
			// ����������
			theApp.GetCurrentUtcTime(time);
			m_time = time.GetTime();
			UpdateOffsetTime();
		}
		else
		{
			// �����񂩂�l�擾
			if (GetOffsetTimeFromEditCtrl(mTimeString))
			{
				m_CMaskedEdit.SetWindowTextW(mTimeString);
			}
		}
		break;

	default:
		break;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CEquipmentCtrlTime

-# ���s���ɌĂяo�����B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CEquipmentCtrlTime::OnBnClickedOk()
{
	UpdateData();
	m_CMaskedEdit.GetWindowText(mTimeString);

	if (GetOffsetTimeFromEditCtrl(mTimeString))
		CDialogEx::OnOK();
}


/*============================================================================*/
/*! CEquipmentCtrlTime

-# �����I�t�Z�b�g�̎������X�V����B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CEquipmentCtrlTime::UpdateOffsetTime()
{
	CString str, strSign;

	if (m_time > 0)
	{
		strSign = _T("+");
	}
	else if (m_time < 0)
	{
		strSign = _T("-");
		m_time *= -1;
	}
	else
	{
		strSign = _T(" ");
	}

	CTime t(m_time);
	CString strTime;

	switch (m_nTimeType)
	{
	case eTimeType_MSS:
		str.Format(_T("%s%01d%02d"), (LPCTSTR)strSign, t.GetMinute(), t.GetSecond());
		break;

	case eTimeType_MMSS:
		strTime = t.Format(_T("%M:%S"));
		str.Format(_T("%s%s"), (LPCTSTR)strSign, (LPCTSTR)strTime);
		break;

	case eTimeType_HHMMSS:
		strTime = t.Format(_T("%H:%M:%S"));
		str.Format(_T("%s%s"), (LPCTSTR)strSign, (LPCTSTR)strTime);
		break;

	case eTimeType_DDDHHMMSS:
		str = t.Format(_T("%j %H:%M:%S"));
		break;

	case eTimeType_YYYYmmdd:
		str = t.Format(_T("%Y-%m-%d"));
		break;

	default:
		break;
	}

	m_CMaskedEdit.SetWindowTextW(str);
}


/*============================================================================*/
/*! CEquipmentCtrlTime

-# �G�f�B�b�g�R���g���[������I�t�Z�b�g�������擾

@param  str�F�I�t�Z�b�g����������
@param	bAutoCorrect�FTRUE�F�͈͊O���ɍő�lor�ŏ��l�Ɏ����C�� / FALSE�F�����C���Ȃ�
@retval TRUE�F�͈͓��������񐮌`����Ă��Ȃ� / FALSE�F�͈͊O�������͕����񐮌`���ꂽ

*/
/*============================================================================*/
BOOL CEquipmentCtrlTime::GetOffsetTimeFromEditCtrl(CString& str, BOOL bAutoCorrect /*= FALSE*/)
{
	CString strTmp = str;

	// �����𒊏o
	int nSign = 0;
	if (strTmp.Left(1) == _T('+'))
	{
		nSign = 1;
		strTmp.TrimLeft(_T('+'));
	}
	else if (strTmp.Left(1) == _T('-'))
	{
		nSign = -1;
		strTmp.TrimLeft(_T('-'));
	}
	else if (strTmp.Left(1) == _T(' '))
	{
		nSign = 1;
		strTmp.TrimLeft(_T(' '));
	}

	BOOL ret = TRUE;

	int nMM = 0;
	int nSS = 0;
	int nHH = 0;
	int nDDD = 0;
	int nYYYY = 0;
	int nmm = 0;
	int ndd = 0;
	int t_total = 0;
	int nMMTmp = 0;


	// DDDHHMMSS�̏ꍇ
	if (m_nTimeType == eTimeType_DDDHHMMSS)
	{
		CString str1;
		CString str2;

		CString strDDD;

		if (AfxExtractSubString(str1, strTmp, 0, ' '))
		{
			(void)AfxExtractSubString(str2, strTmp, 0, ' ');
		}

		if (!strTmp.IsEmpty())
		{
			if (!str1.IsEmpty())
			{
				if (!str2.IsEmpty())
				{
					// ���Ԃƕ��ƕb
					strTmp = str2;
				}
			}
			else
			{
				// ���ɂ�
				strDDD = str1;
			}
		}

		nDDD = _ttoi(strDDD);

		if (nDDD >= m_timeDDDMax)
		{
			// ���Ԃ��傫������
			if (bAutoCorrect)
			{
				nDDD = m_timeDDDMax - 1;
			}
			else
			{
				ret = FALSE;
			}
		}
	}

	// MSS�܂���MMSS�܂���HHMMSS�̏ꍇ
	if (m_nTimeType == eTimeType_MSS || m_nTimeType == eTimeType_MMSS || m_nTimeType == eTimeType_HHMMSS)
	{
		CString str1;
		CString str2;
		CString str3;

		CString strHH;
		CString strMM;
		CString strSS;

		if (AfxExtractSubString(str1, strTmp, 0, ':'))
		{
			if (AfxExtractSubString(str2, strTmp, 1, ':'))
			{
				(void)AfxExtractSubString(str3, strTmp, 2, ':');
			}
		}

		if (!strTmp.IsEmpty())
		{
			if (!str1.IsEmpty())
			{
				if (!str2.IsEmpty())
				{
					if (!str3.IsEmpty())
					{
						// ���Ԃƕ��ƕb
						strHH = str1;
						strMM = str2;
						strSS = str3;
					}
					else
					{
						// ���ƕb
						strMM = str1;
						strSS = str2;
					}
				}
				else
				{
					// ���̂�
					strSS = str1;
				}
			}
			else
			{
				// �b�̂�
				strSS = str1;
			}
		}

		nHH = _ttoi(strHH);
		nMM = _ttoi(strMM);
		nSS = _ttoi(strSS);

		t_total = nHH * m_timeMMMax + nMM * m_timeSSMax + nSS;

		switch (m_nTimeType)
		{
		case eTimeType_MSS:
			if (t_total >= m_timeMMMax * m_timeMMax)
			{
				if (bAutoCorrect)
					t_total = m_timeMMMax * m_timeMMax - 1;
				else
					ret = FALSE;
			}
			nMM = t_total / m_timeSSMax;
			nSS = t_total % m_timeSSMax;
			break;

		case eTimeType_MMSS:
			if (t_total >= m_timeMMMax * m_timeSSMax)
			{
				if (bAutoCorrect)
					t_total = m_timeMMMax * m_timeSSMax - 1;
				else
					ret = FALSE;
			}
			nMM = t_total / m_timeSSMax;
			nSS = t_total % m_timeSSMax;
			break;

		case eTimeType_HHMMSS:
			if (t_total >= m_timeHHMax * m_timeMMMax * m_timeSSMax)
			{
				if (bAutoCorrect)
					t_total = m_timeHHMax * m_timeMMMax * m_timeSSMax - 1;
				else
					ret = FALSE;
			}
			nHH = t_total / m_timeMMMax;
			nMMTmp = t_total % m_timeMMMax;
			nMM = nMMTmp / m_timeSSMax;
			nSS = t_total % m_timeSSMax;
			break;
		}
	}

	if (m_nTimeType == eTimeType_YYYYmmdd)
	{
		// �E����Q����
		CString strdd = strTmp.Right(2);
		// �c�肪���ȏ�
		int nLength = strTmp.GetLength();
		CString strOvermm = strTmp.Left(nLength - 2);
		CString strmm = strOvermm.Right(2);
		ndd = _ttoi(strdd);
		nmm = _ttoi(strmm);

		if (ndd >= m_timeddMax)
		{
			// �����傫������
			if (bAutoCorrect)
			{
				ndd = m_timeddMax - 1;
			}
			else
			{
				ret = FALSE;
			}
		}
		if (nmm >= m_timemmMax)
		{
			// �����傫������
			if (bAutoCorrect)
			{
				nmm = m_timemmMax - 1;
			}
			else
			{
				ret = FALSE;
			}
		}
		// �c�肪�N
		nLength = strOvermm.GetLength();
		CString strOverYYYY = strOvermm.Left(nLength - 2);
		CString strYYYY = strOvermm.Right(4);
		nYYYY = _ttoi(strYYYY);
	}

	// �蓮�C���̏ꍇ
	if (bAutoCorrect == FALSE && ret == FALSE)
	{
		// �G���[�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_OFFSETTIME_OUTOFRANGE_ERR) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
		//# �G���[�Ȃ̂ŃG�f�B�b�g�R���g���[���̓��e��S�I�����ăt�H�[�J�X�ݒ肷��
		m_CMaskedEdit.SetSel(0, -1);
		m_CMaskedEdit.SetFocus();
	}

	switch (m_nTimeType)
	{
	case eTimeType_MSS:
		strTmp.Format(_T("%d:%02d"), nMM, nSS);
		break;

	case eTimeType_MMSS:
		strTmp.Format(_T("%02d:%02d"), nMM, nSS);
		break;

	case eTimeType_HHMMSS:
		strTmp.Format(_T("%02d:%02d:%02d"), nHH, nMM, nSS);
		break;

	case eTimeType_DDDHHMMSS:
		strTmp.Format(_T("%03d %02d:%02d:%02d"), nDDD, nHH, nMM, nSS);
		break;

	case eTimeType_YYYYmmdd:
		strTmp.Format(_T("%04d-%02d-%02d"), nYYYY, nmm, ndd);
		break;

	default:
		break;
	}

	CString strText;
	if (nSign > 0)
	{
		strText.Format(_T("+%s"), (LPCTSTR)strTmp);
	}
	else if (nSign < 0)
	{
		strText.Format(_T("-%s"), (LPCTSTR)strTmp);
	}
	else if (m_nTimeType == eTimeType_MSS || m_nTimeType == eTimeType_MMSS || m_nTimeType == eTimeType_HHMMSS)
	{
		strText.Format(_T("+%s"), (LPCTSTR)strTmp);
	}
	else
	{
		strText = strTmp;
	}

	strTmp = strText;

	// �����񐮌`��
	if (strTmp.CompareNoCase(str) != 0)
	{
		if (ret)
		{
			// �l���͈͓��̏ꍇ�A���`������\�����Ȃ���
			m_CMaskedEdit.SetWindowTextW(strTmp);
			UpdateData(FALSE);
		}
		else
		{
			// �l���͈͊O�̏ꍇ�A���͕��������̂܂ܕ\������
		}

		ret = FALSE;
	}
	
	return ret;
}


BOOL CEquipmentCtrlTime::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_RETURN) {
		//# �G�f�B�b�g�R���g���[���̏ꍇ�AENTER�L�[����M������TAB�L�[�ɕύX����
		HWND p = m_CMaskedEdit.m_hWnd;
		if (p == pMsg->hwnd){
			pMsg->wParam = VK_TAB;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//void CEquipmentCtrlTime::OnEnKillfocusMfcmaskededitCtrlregist()
//{
//	//# �t�H�[�J�X���������ꂽ��A�����I�t�Z�b�g���t�H�[�}�b�g�`�F�b�N����
//	m_CMaskedEdit.GetWindowText(mTimeString);
//	GetOffsetTimeFromEditCtrl(mTimeString);
//}
