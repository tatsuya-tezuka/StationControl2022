// OperationTimeDialogBase.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationTimeDialogBase.h"
#include "afxdialogex.h"

#include "..\Utility\SshShell.h"

// COperationTimeDialogBase �_�C�A���O

IMPLEMENT_DYNAMIC(COperationTimeDialogBase, CDialogBase)

COperationTimeDialogBase::COperationTimeDialogBase(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialogBase(IDD, pParent)
	, m_strTimeBefore(_T(""))
{
	GetSystemTime(&m_timeBefore);
}

COperationTimeDialogBase::~COperationTimeDialogBase()
{
}

void COperationTimeDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COperationTimeDialogBase, CDialogBase)
END_MESSAGE_MAP()


// COperationTimeDialogBase ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! COperationTimeDialogBase

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL COperationTimeDialogBase::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CTime ctime = m_timeBefore;
	m_strDateBefore = ctime.Format("%Y-%m-%d");
	m_strTimeBefore = ctime.Format("%H:%M:%S");
	m_dateAfterCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_timeAfterCtrl.SetFormat(_T("HH:mm:ss"));

	m_dateAfterCtrl.SetTime(&m_timeBefore);
	m_timeAfterCtrl.SetTime(&m_timeBefore);

	UpdateData(FALSE);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! COperationTimeDialogBase

-# OK�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void COperationTimeDialogBase::OnBnClickedOk()
{
	SYSTEMTIME date, time;
	// �N������ύX����
	if (m_dateAfterCtrl.GetTime(&date) == GDT_VALID && m_timeAfterCtrl.GetTime(&time) == GDT_VALID)
	{
		m_timeAfter.wYear = date.wYear;
		m_timeAfter.wMonth = date.wMonth;
		m_timeAfter.wDayOfWeek = date.wDayOfWeek;
		m_timeAfter.wDay = date.wDay;
		m_timeAfter.wHour = time.wHour;
		m_timeAfter.wMinute = time.wMinute;
		m_timeAfter.wSecond = time.wSecond;
		m_timeAfter.wMilliseconds = time.wMilliseconds;

		CString strTime;
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), m_timeAfter.wYear, m_timeAfter.wMonth, m_timeAfter.wDay, m_timeAfter.wHour, m_timeAfter.wMinute, m_timeAfter.wSecond);

		char sz_time[40];
		char sz_asctime[40];
		sprintf_s(sz_time, 40, "%s", (LPCSTR)CStringA(strTime));
		CSatelliteData::ub_asc_to_asctime(sz_time, sz_asctime);
		double tNew = CSatelliteData::ub_asc_to_dtime(sz_asctime);

		if (m_Type == TYPE_START)	// �J�n�����ύX�̏ꍇ
		{
			if (tNew < (double)m_tMin)
			{
				CString strTitle;
				GetWindowText(strTitle);
				MessageBox(_T("�J�n�����𑁂߂邱�Ƃ͏o���܂���B"), strTitle, MB_OK | MB_ICONWARNING);
				return;
			}
			if (tNew > (double)m_tMax)
			{
				CString strTitle;
				GetWindowText(strTitle);
				MessageBox(_T("�J�n�������I����������ɂ͏o���܂���B"), strTitle, MB_OK | MB_ICONWARNING);
				return;
			}
		}
		else if (m_Type == TYPE_END)	// �I�������ύX�̏ꍇ
		{
			if (tNew < (double)m_tMin)
			{
				CString strTitle;
				GetWindowText(strTitle);
				MessageBox(_T("�I���������J�n������葁�߂邱�Ƃ͏o���܂���B"), strTitle, MB_OK | MB_ICONWARNING);
				return;
			}
		}

		// �v��ύX�v��
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = SEND_NOTICE_SH;
		CString arg;

		int nStation = theApp.GetSelectStation();
		arg.Format(_T("%s %d %d,%s"),
			(LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(),	// ��
			INFO_ID_REQ_PLN_MO,												// �v��ύX�v��
			m_Type,															// �J�n�����ύX�F1 or �I�������ύX�F2
			(LPCTSTR)strTime);												// ����

		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam);		// �V�F���҂�����
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuResend"), _T("debug"), strDebug, nLogEx::debug);
		}

		if (err != 0) // �ُ�I��
		{
		}
		else // ����I��
		{
			// �v��X�V�ʒm�𑗂�
			UpdatePlanData(nStation, strTime);
		}
	}

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! COperationTimeDialogBase

-# �v��t�@�C����AUTO OFF����

@param	cbuf	�v��t�@�C���̕�����
@retval TRUE:AUTO��OFF�ɒu�������� / FALSE:AUTO��u�������ĂȂ�
*/
/*============================================================================*/
BOOL COperationTimeDialogBase::UpdatePlanDataAutoOff(CString& cbuf)
{
	// �uAUTO=�v��T��
	const CString strSearchString = _T("AUTO=");
	if (cbuf.Find(strSearchString) >= 0){
		// �uAUTO=�v���݂������̂�OFF�ɂ��邽�߁A�uAUTO=�v���E��������
		CString str[20];
		int count = 0;
		int pos = 0;
		int i = 0;
		while (AfxExtractSubString(str[i], cbuf, i, ' ')){
			str[i].TrimLeft();
			str[i].TrimRight();
			if (str[i].Find(strSearchString) >= 0){
				pos = i;
			}
			i++;
		}
		count = i;
		for (i = 0; i < count; i++){
			if (i == 0){
				cbuf = str[i];
			}
			else{
				cbuf += _T(" ");
				if (i == pos){
					cbuf += strSearchString;
				}
				else{
					cbuf += str[i];
				}
			}
		}
		cbuf += _T("\n");
		return TRUE;
	}

	return FALSE;
}
