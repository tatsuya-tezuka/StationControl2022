/*============================================================================*/
/*! OperationStartTimeChg.cpp

-# �^�p�I�������ύX���
*/
/*============================================================================*/
// OperationStartTimeChg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationEndTimeChg.h"
#include "afxdialogex.h"

#include "..\Utility\SshShell.h"

// COperationEndTimeChg �_�C�A���O

IMPLEMENT_DYNAMIC(COperationEndTimeChg, COperationTimeDialogBase)

/*============================================================================*/
/*! COperationEndTimeChg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
COperationEndTimeChg::COperationEndTimeChg(CWnd* pParent /*=NULL*/)
	: COperationTimeDialogBase(COperationEndTimeChg::IDD, pParent)
{

}

/*============================================================================*/
/*! COperationEndTimeChg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
COperationEndTimeChg::~COperationEndTimeChg()
{
}

void COperationEndTimeChg::DoDataExchange(CDataExchange* pDX)
{
	COperationTimeDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPERATIONENDTIMECHG_DATEPICKER, m_dateAfterCtrl);
	DDX_Control(pDX, IDC_OPERATIONENDTIMECHG_TIMEPICKER, m_timeAfterCtrl);
	DDX_Text(pDX, IDC_OPERATIONENDTIMECHG_DATEEDIT, m_strDateBefore);
	DDX_Text(pDX, IDC_OPERATIONENDTIMECHG_TIMEEDIT, m_strTimeBefore);
}


BEGIN_MESSAGE_MAP(COperationEndTimeChg, COperationTimeDialogBase)
	ON_BN_CLICKED(IDOK, &COperationEndTimeChg::OnBnClickedOk)
END_MESSAGE_MAP()


// COperationEndTimeChg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! COperationEndTimeChg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval TRUE�F���� / FALSE�F���s
*/
/*============================================================================*/
BOOL COperationEndTimeChg::OnInitDialog()
{
	CString satellite, planid, getform;
	LPARAM val3;
	UINT val4;
	CTime val5;
	theApp.GetTimeLinePlan(satellite, planid, val3, val4, val5);
	if (val3 == 0)
		getform = _T("�Ǌǐ�");
	else
		getform = _T("ENP");

	// �ǔԎ擾
	UINT nStation = theApp.GetSelectStation();
	time_t	tStart = 0;
	time_t	tEnd = 0;

	// �v��ꗗ���X�g����ǁA�q�����A�v��ID�A�쐬����v�ŒT��
	vector<stPlanListData>& listPlan = theApp.GetSatelliteData().GetPlanList();
	for (int i = 0; i < listPlan.size(); i++)
	{
		if (listPlan[i].strStationName.CompareNoCase(mStationString[nStation]) == 0 &&
			listPlan[i].strSatelliteName.CompareNoCase(satellite) == 0 &&
			listPlan[i].strPassId.CompareNoCase(planid) == 0 &&
			listPlan[i].strGetFrom.CompareNoCase(getform) == 0)
		{
			// �������擾���ă_�C�A���O�ɃZ�b�g
			tStart = listPlan[i].tPassStart;
			tEnd = listPlan[i].tPassEnd;
			break;
		}
	}

	SetBeforeTimeT(tEnd);		// �I�������̃Z�b�g
	SetMinMaxTime(tStart, LONG64_MAX);

	COperationTimeDialogBase::OnInitDialog();

	SetType(COperationTimeDialogBase::TYPE_END);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


//void COperationEndTimeChg::OnBnClickedOk()
//{
//	SYSTEMTIME date, time;
//	// �N������ύX����
//	if (m_dateAfterCtrl.GetTime(&date) == GDT_VALID && m_timeAfterCtrl.GetTime(&time) == GDT_VALID)
//	{
//		m_timeAfter.wYear = date.wYear;
//		m_timeAfter.wMonth = date.wMonth;
//		m_timeAfter.wDayOfWeek = date.wDayOfWeek;
//		m_timeAfter.wDay = date.wDay;
//		m_timeAfter.wHour = time.wHour;
//		m_timeAfter.wMinute = time.wMinute;
//		m_timeAfter.wSecond = time.wSecond;
//		m_timeAfter.wMilliseconds = time.wMilliseconds;
//
//		CString strTime;
//		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), m_timeAfter.wYear, m_timeAfter.wMonth, m_timeAfter.wDay, m_timeAfter.wHour, m_timeAfter.wMinute, m_timeAfter.wSecond);
//
//		// �v��ύX�v��
//		CSshShell::structSshParam stParam;
//		stParam.strBatchPath = theApp.GetSshBatchPath();
//		stParam.strUserName = theApp.GetSshUserName();
//		stParam.strServerName = theApp.GetSshServerName();
//		stParam.strKeyFilePath = theApp.GetSshKeyPath();
//		stParam.strRemShellName = SEND_NOTICE_SH;
//		CString arg;
//
//		int nStation = theApp.GetSelectStation();
//		arg.Format(_T("%s %d %d,%s"),
//			(LPCTSTR)CString(mStationSimpleString[nStation]).MakeLower(),	// ��
//			INFO_ID_REQ_PLN_MO,												// �v��ύX�v��
//			m_Type,															// �J�n�����ύX�F1 or �I�������ύX�F2
//			(LPCTSTR)strTime);														// ����
//
//		stParam.strRemShellParam.Format(arg);
//
////		int err = CSshShell::ExecShell(stParam, FALSE);		// �V�F���҂����Ȃ�
//		int err = CSshShell::ExecShell(stParam);		// �V�F���҂�����
//		{
//			CString strDebug;
//			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
//			CLogTraceEx::Write(_T(""), _T("CForecastList"), _T("OnMenuResend"), _T("debug"), strDebug, nLogEx::debug);
//		}
//
//		if (err != 0) // �ُ�I��
//		{
//		}
//		else // ����I��
//		{
//			// �v��X�V�ʒm�𑗂�
//			UpdatePlanData(nStation, strTime);
//		}
//	}
//
//	CDialogBase::OnOK();
//
//	// �E�B���h�E�����
//	CDialogBase::OnOK();
//}
void COperationEndTimeChg::OnBnClickedOk()
{
	// �E�B���h�E�����
	COperationTimeDialogBase::OnBnClickedOk();
}

/*============================================================================*/
/*! COperationEndTimeChg

-# �v��t�@�C���X�V

@param  �Ȃ�
@retval TRUE�F���� / FALSE�F���s
*/
/*============================================================================*/
void COperationEndTimeChg::UpdatePlanData(UINT nStation, CString& strTime)
{
	CString str1, str2;
	LPARAM val3;
	UINT val4;
	CTime val5;

	// �v��擾
	theApp.GetTimeLinePlan(str1, str2, val3, val4, val5);

	CString fname;
	fname.Format(_T("%s%s\\%s\\%s.pln"), (LPCTSTR)theApp.GetShareFilePath(eFileType_Plan_Folder, nStation), (val3 == 0) ? _T("sttn") : _T("enp"), (LPCTSTR)str1, (LPCTSTR)str2);
	fname.Replace(_T("/"), _T("\\"));

	CString ftpfname;
	ftpfname.Format(_T("%s%s\\%s.pln"), (LPCTSTR)theApp.GetShareFilePath(eFileType_PlanOut_Folder, nStation), (LPCTSTR)str1, (LPCTSTR)str2);
	ftpfname.Replace(_T("/"), _T("\\"));


	CGraphMutex::Lock(eFile);
	// ���ݑI������Ă���v��t�@�C������C�ǂ�
	FILE *file = NULL;
	if (_wfopen_s(&file, fname, _T("r")) != 0 || file == NULL){
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("COperationEndTimeChg"), _T("UpdatePlanData"), _T("Plan File Open(R) Error"), fname, nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		CGraphMutex::Unlock(eFile);
		return;
	}

	vector<CString>	plandata;
	vector<CString>::iterator itr;
	CString cbuf, str;
	CStringA abuf;

	TRY
	{
		char sz_line[512];
		while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), file) != NULL){
			cbuf = CServerFile::ConvertUnicode(sz_line);
			plandata.push_back(cbuf);
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

		fclose(file);

	FILE *fileeuc = NULL;
	if (_wfopen_s(&fileeuc, ftpfname, _T("wb")) != 0 || fileeuc == NULL){
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("UpdatePlanData"), _T("Plan File Open(W) Error"), ftpfname, nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		CGraphMutex::Unlock(eFile);
		return;
	}

	// P-END�̏�������
	for (itr = plandata.begin(); itr != plandata.end(); itr++){
		cbuf = (*itr);

		// AUTO OFF
		if (UpdatePlanDataAutoOff(cbuf))
		{
		}
		else
		{
			// �w�b�_�[�s�́wP-END�x��T��
			if ((cbuf.Find(_T("# ")) >= 0) && (cbuf.Find(_T("P-END=")) >= 0)){
				CString header = _T("");
				CString item = _T("");
				int i = 1;
				while (AfxExtractSubString(str, cbuf, i++, ' ')){
					str.TrimLeft();
					str.TrimRight();
					if (str.Find(_T("P-START")) >= 0)
					{
						header += _T(" ") + str;
						AfxExtractSubString(str, cbuf, i++, ' ');	// HH:MM::SS
						header += _T(" ") + str;
					}
					else if (str.Find(_T("P-END")) >= 0)
					{
						item.Format(_T("P-END=%s"), (LPCTSTR)strTime);
						header += _T(" ") + item;
						i++;	// HH:MM::SS
					}
					else
					{
						header += _T(" ") + str;
					}
				}
				cbuf.Format(_T("#%s\n"), (LPCTSTR)header);
			}
			else if (cbuf.Find(_T("P-END")) >= 0){
				// �{���́wP-END�x��T��
				cbuf.Format(_T("%s P-END,,%s;\n"), (LPCTSTR)strTime, (LPCTSTR)str1);
			}
		}
		abuf = CServerFile::ConvertEuc((LPTSTR)(LPCTSTR)cbuf);
		fputs(abuf, fileeuc);
	}

	fclose(fileeuc);

	// �v��o�^�v��
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = REGIST_PLAN_SH;
	CString arg;
	CString temp = str1;
	arg.Format(_T("%s renew %s sttn %s.pln"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)temp.MakeLower(), (LPCTSTR)str2);

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);
	if (err != 0)
	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("CTimeLineView"), _T("UpdatePlanData"), _T(""), strDebug, nLogEx::error);
	}

	CGraphMutex::Unlock(eFile);
}
