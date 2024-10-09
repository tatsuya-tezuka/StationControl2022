/*============================================================================*/
/*! OperationStartTimeChg.cpp

-# �^�p�J�n�����ύX���
*/
/*============================================================================*/
// OperationStartTimeChg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationStartTimeChg.h"
#include "afxdialogex.h"

#include "..\Utility\SshShell.h"

// COperationStartTimeChg �_�C�A���O

IMPLEMENT_DYNAMIC(COperationStartTimeChg, COperationTimeDialogBase)

/*============================================================================*/
/*! COperationStartTimeChg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
COperationStartTimeChg::COperationStartTimeChg(CWnd* pParent /*=NULL*/)
	: COperationTimeDialogBase(COperationStartTimeChg::IDD, pParent)
{

}

/*============================================================================*/
/*! COperationStartTimeChg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
COperationStartTimeChg::~COperationStartTimeChg()
{
}

void COperationStartTimeChg::DoDataExchange(CDataExchange* pDX)
{
	COperationTimeDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPERATIONSTARTTIMECHG_DATEPICKER, m_dateAfterCtrl);
	DDX_Control(pDX, IDC_OPERATIONSTARTTIMECHG_TIMEPICKER, m_timeAfterCtrl);
	DDX_Text(pDX, IDC_OPERATIONSTARTTIMECHG_DATEEDIT, m_strDateBefore);
	DDX_Text(pDX, IDC_OPERATIONSTARTTIMECHG_TIMEEDIT, m_strTimeBefore);
}


BEGIN_MESSAGE_MAP(COperationStartTimeChg, COperationTimeDialogBase)
	ON_BN_CLICKED(IDOK, &COperationStartTimeChg::OnBnClickedOk)
END_MESSAGE_MAP()


// COperationStartTimeChg ���b�Z�[�W �n���h���[



/*============================================================================*/
/*! COperationStartTimeChg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval TRUE�F���� / FALSE�F���s
*/
/*============================================================================*/
BOOL COperationStartTimeChg::OnInitDialog()
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

	SetBeforeTimeT(tStart);		// �J�n�����̃Z�b�g
	SetMinMaxTime(tStart, tEnd);

	COperationTimeDialogBase::OnInitDialog();

	SetType(COperationTimeDialogBase::TYPE_START);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void COperationStartTimeChg::OnBnClickedOk()
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
void COperationStartTimeChg::UpdatePlanData(UINT nStation, CString& strTime)
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

	// P-START�̏�������
	for (itr = plandata.begin(); itr != plandata.end(); itr++){
		cbuf = (*itr);

		// AUTO OFF
		if (UpdatePlanDataAutoOff(cbuf))
		{
		}
		else
		{
			// �w�b�_�[�s�́wP-START�x��T��
			if ((cbuf.Find(_T("# ")) >= 0) && (cbuf.Find(_T("P-START=")) >= 0)){
				CString header = _T("");
				CString item = _T("");
				int i = 1;
				while (AfxExtractSubString(str, cbuf, i++, ' ')){
					str.TrimLeft();
					str.TrimRight();
					if (str.Find(_T("P-START")) >= 0)
					{
						item.Format(_T("P-START=%s"), (LPCTSTR)strTime);
						header += _T(" ") + item;
						i++;	// HH:MM::SS
					}
					else if (str.Find(_T("P-END")) >= 0)
					{
						header += _T(" ") + str;
						AfxExtractSubString(str, cbuf, i++, ' ');	// HH:MM::SS
						header += _T(" ") + str;
					}
					else
					{
						header += _T(" ") + str;
					}
				}

				cbuf.Format(_T("#%s"), (LPCTSTR)header);
			}
			else if (cbuf.Find(_T("P-START")) >= 0){
				// �{���́wP-START�x��T��
				cbuf.Format(_T("%s P-START,,%s;\n"), (LPCTSTR)strTime, (LPCTSTR)str1);
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
