/*============================================================================*/
/*! OperationPlanTimeOffsetSetting.cpp

-# �����I�t�Z�b�g�ݒ���
*/
/*============================================================================*/
// OperationPlanTimeOffsetSetting.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationPlanTimeOffsetSet.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"

// COperationPlanTimeOffsetSet �_�C�A���O

IMPLEMENT_DYNAMIC(COperationPlanTimeOffsetSet, CDialogBase)

/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
COperationPlanTimeOffsetSet::COperationPlanTimeOffsetSet(CWnd* pParent /*=NULL*/)
	: CDialogBase(COperationPlanTimeOffsetSet::IDD, pParent)
	, m_time(0)
{
	m_Type = -1;
}

/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
COperationPlanTimeOffsetSet::~COperationPlanTimeOffsetSet()
{
}

void COperationPlanTimeOffsetSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OFFSETTIME_MFCMASKEDEDIT, m_CMaskedEdit);
	DDX_Control(pDX, IDC_OFFSETTIME_SPIN, m_CSpin);
}


BEGIN_MESSAGE_MAP(COperationPlanTimeOffsetSet, CDialogBase)
	ON_BN_CLICKED(IDC_OPERATIONPLANTIMEOFFSETSET_BUTTON1, &COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton1)
	ON_BN_CLICKED(IDC_OPERATIONPLANTIMEOFFSETSET_BUTTON2, &COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton2)
	ON_BN_CLICKED(IDC_OPERATIONPLANTIMEOFFSETSET_BUTTON3, &COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_OFFSETTIME_SPIN, &COperationPlanTimeOffsetSet::OnDeltaposOffsettimeSpin)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// COperationPlanTimeOffsetSet ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL COperationPlanTimeOffsetSet::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_CMaskedEdit.EnableMask(_T("*d:dd"), _T("__:__"), _T(' '));
	m_CMaskedEdit.SetValidChars(_T("01234567890+-"));

	m_CSpin.SetRange32(0, (int)(m_timeMax - m_timeMin));

	UpdateOffsetTime();

	m_CSpin.SetPos32((int)m_time + m_timeMax);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �����I�t�Z�b�g�̎������X�V����B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::UpdateOffsetTime()
{
	CString str, strSign;
	time_t t = m_time;

	if (t >= 0)
	{
		strSign = _T("+");
	}
	else
	{
		strSign = _T("-");
		t *= -1;
	}
	str.Format(_T("%s%01d:%02d"), (LPCTSTR)strSign, (int)(t / 60), (int)(t % 60));
	m_CMaskedEdit.SetWindowTextW(str);

	UpdateData();
	UpdateWindow();
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �G�f�B�b�g�R���g���[������I�t�Z�b�g�������擾

@param  offsetTime�F�I�t�Z�b�g�����o��
@param	bAutoCorrect�FTRUE�F�͈͊O���ɍő�lor�ŏ��l�Ɏ����C�� / FALSE�F�����C���Ȃ�
@retval TRUE�F�͈͓������� / FALSE�F�͈͊O������

*/
/*============================================================================*/
BOOL COperationPlanTimeOffsetSet::GetOffsetTimeFromEditCtrl(time_t& offsetTime, BOOL bAutoCorrect /*= FALSE*/)
{
	UpdateData(TRUE);

	CString str;
	m_CMaskedEdit.GetWindowTextW(str);

	// �����𒊏o
	int nSign = 1;
	if (str.Left(1) == _T('+'))
	{
		nSign = 1;
		str.Remove(_T('+'));
	}
	else if (str.Left(1) == _T('-'))
	{
		nSign = -1;
		str.Remove(_T('-'));
	}

	// �E����Q���b
	CString strSS = str.Right(2);
	// �c�肪��
	int nLength = str.GetLength();
	CString strMM = str.Left(nLength - 2);
	int nMM = _ttoi(strMM);
	int nSS = _ttoi(strSS);
	BOOL bErr = FALSE;

	if (nMM > m_timeMMMax)
	{
		// �����傫������
		if (bAutoCorrect)
		{
			nMM = m_timeMMMax;
		}
		else
		{
			bErr = TRUE;
		}
	}
	if (nSS >= m_timeSSMax)
	{
		// �b���傫������
		if (bAutoCorrect)
		{
			nSS = m_timeSSMax;
		}
		else
		{
			bErr = TRUE;
		}
	}

	if (bErr == FALSE)
	{
		time_t nMMSS = nSign * (nMM * 60 + nSS);
		if (nMMSS > m_timeMax)
		{
			// ���ƕb�̍��v���傫������
			if (bAutoCorrect)
			{
				nMMSS = m_timeMax;
			}
		}
		else if (nMMSS < m_timeMin)
		{
			// ���ƕb�̍��v������������
			if (bAutoCorrect)
			{
				nMMSS = m_timeMin;
			}
		}
		else
		{
			offsetTime = nMMSS;
			return TRUE;
		}
	}

	// �蓮�C���̏ꍇ
	if (bAutoCorrect == FALSE)
	{
		// �G���[�_�C�A���O�\��
		CString strTitle;
		CString strMsg;
		if (strTitle.LoadString(IDS_CHECK_TITLE) == FALSE)
			return FALSE;
		if (strMsg.LoadString(IDS_OFFSETTIME_OUTOFRANGE_ERR) == FALSE)
			return FALSE;
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONERROR);
	}
	
	return FALSE;
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# OK�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton1()
{
	time_t nMMSS;
	// �I�t�Z�b�g�����擾
	if (GetOffsetTimeFromEditCtrl(nMMSS))
	{
		// �����I�t�Z�b�g���T�[�o�ɑ��M����
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strRemShellName = SEND_NOTICE_SH;

		CString arg;
		//		m_CMaskedEdit.GetWindowTextW(arg);
		CString strSign;
		if (nMMSS >= 0)
		{
			strSign = _T("+");
		}
		else
		{
			strSign = _T("-");
		}

		if (m_Type == TYPE_TIMELINE)
		{
			arg.Format(_T("%s %d %s%02d:%02d"), (LPCTSTR)m_strStationName, INFO_ID_REQ_TIMOFS, (LPCTSTR)strSign, (int)abs(nMMSS / 60), (int)abs(nMMSS % 60));
		}
		else
		{
			arg.Format(_T("%s %d %s,%s%02d:%02d"), (LPCTSTR)m_strStationName, INFO_ID_REQ_TIMOFS, (LPCTSTR)m_strFullFilePath, (LPCTSTR)strSign, (int)abs(nMMSS / 60), (int)abs(nMMSS % 60));
		}
		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam, FALSE);		// �V�F���҂����Ȃ�
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("COperationPlanTimeOffsetSet"), _T("OnBnClickedOperationplantimeoffsetsetButton1"), _T("debug"), strDebug, nLogEx::debug);
		}

		// �E�B���h�E�����
		CDialogBase::OnOK();
	}
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �����I�t�Z�b�g�����{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton2()
{
	// �����I�t�Z�b�g���N���A����
	m_time = 0;
	UpdateOffsetTime();
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �I���{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnBnClickedOperationplantimeoffsetsetButton3()
{
// test >>>
//	CString strTest;
//strTest.Format(_T("%02d:%02d"), m_timeOffset.wMinute, m_timeOffset.wSecond);
//	AfxMessageBox(strTest);
// test <<<

	// ���������ɕ���
	PostMessage(WM_CLOSE);
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �I�t�Z�b�g�����̃X�s���R���g���[���ύX���ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnDeltaposOffsettimeSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if (m_CSpin.GetSafeHwnd() == NULL)
	{
		return;
	}

	// �I�t�Z�b�g�����擾���A�X�s���ʒu�ɃZ�b�g
	(void)GetOffsetTimeFromEditCtrl(m_time, TRUE);
	int t = (int)m_time + m_timeMax;
	m_CSpin.SetPos32(t);

	*pResult = 0;
}


/*============================================================================*/
/*! COperationPlanTimeOffsetSet

-# �c�X�N���[���o�[�N���b�N���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�

*/
/*============================================================================*/
void COperationPlanTimeOffsetSet::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == SB_THUMBPOSITION)
	{
		if ((m_CSpin.m_hWnd == pScrollBar->m_hWnd))
		{
			CString str;
			m_time = (time_t)((int)nPos - m_timeMax);
			UpdateOffsetTime();
		}
	}

	CDialogBase::OnVScroll(nSBCode, nPos, pScrollBar);
}
