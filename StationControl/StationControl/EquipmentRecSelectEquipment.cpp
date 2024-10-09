/*============================================================================*/
/*! EquipmentRecSelectEquipment.cpp

-# �ݔ��L�^ �_���v�o�͑Ώېݔ��I�����
*/
/*============================================================================*/
// EquipmentRecSelectEquipment.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentRecSelectEquipment.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"


// CEquipmentRecSelectEquipment �_�C�A���O

IMPLEMENT_DYNAMIC(CEquipmentRecSelectEquipment, CDialogBase)

CEquipmentRecSelectEquipment::CEquipmentRecSelectEquipment(CWnd* pParent /*=NULL*/)
: CDialogBase(CEquipmentRecSelectEquipment::IDD, pParent)
{
	m_pParent = pParent;
}

CEquipmentRecSelectEquipment::~CEquipmentRecSelectEquipment()
{
}

void CEquipmentRecSelectEquipment::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EQIPREC_SELEQIP_LIST, m_EquipList);
}


BEGIN_MESSAGE_MAP(CEquipmentRecSelectEquipment, CDialogBase)
	ON_BN_CLICKED(IDC_EQIPREC_SELEQIP_OUTPUT_BTN, &CEquipmentRecSelectEquipment::OnBnClickedEqiprecSeleqipOutputBtn)
END_MESSAGE_MAP()


// CEquipmentRecSelectEquipment ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CEquipmentRecSelectEquipment

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CEquipmentRecSelectEquipment::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	for (int i = 0; i < m_EquipNameList.size(); ++i)
	{
		CString eqip = m_EquipNameList[i];
		m_EquipList.InsertString(-1, eqip);
	}

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

#include "CommBase.h"

/*============================================================================*/
/*! CEquipmentRecSelectEquipment

-# �ʎZ���̎擾

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CString CEquipmentRecSelectEquipment::GetTotalDay(CTime time)
{
	CString str = _T("");

	// ���ݎ������狁�߂Ă���̂ň������Ԃ��狁�߂�悤�ɏC��
	//tm t;
	//memset(&t, 0, sizeof(t));
	//time.GetGmtTm(&t);
	//str.Format(_T("%d, %02d:%02d:%02d"), t.tm_yday, time.GetHour(), time.GetMinute(), time.GetSecond());

	stServerItemData data;
	memset(&data, 0, sizeof(stServerItemData));
	SYSTEMTIME st;
	time.GetAsSystemTime(st);
	CCommBase::ConvertTime(&st, &data.systime);

	str.Format(_T("%d, %02d:%02d:%02d"), data.systime.wDay, time.GetHour(), time.GetMinute(), time.GetSecond());
	return str;
}


/*============================================================================*/
/*! CEquipmentRecSelectEquipment

-# �Ď��f�[�^������ݔ������폜

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CString CEquipmentRecSelectEquipment::GetMonName(CString mon)
{
	CString str = _T("");

	int idx = mon.Find(_T("."));
	str = mon.Mid(idx + 1);
	return str;
}


/*============================================================================*/
/*! CEquipmentRecSelectEquipment

-# �o�̓{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecSelectEquipment::OnBnClickedEqiprecSeleqipOutputBtn()
{
	FILE *fp;

	// �I�����ڎ擾
	int nCount = m_EquipList.GetSelCount();
	if (nCount <= 0)
	{
		AfxMessageBox(_T("�_���v�o�͑Ώېݔ����I������Ă��܂���B"), MB_OK | MB_ICONEXCLAMATION);
	}

	CArray<int, int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_EquipList.GetSelItems(nCount, aryListBoxSel.GetData());


	CEquipmentRecDumpOutput* pParent = ((CEquipmentRecDumpOutput*)m_pParent);
	map<CString, vector<CString>> &EquipMonNameList = pParent->GetEquipMonNameList();

	// �_���v�J�n����
	CTime dumpStart(pParent->m_StartTimeD.GetYear(), pParent->m_StartTimeD.GetMonth(), pParent->m_StartTimeD.GetDay(), pParent->m_StartTimeT.GetHour(), pParent->m_StartTimeT.GetMinute(), pParent->m_StartTimeT.GetSecond());
	CString strDumpStart = GetTotalDay(dumpStart);

	// �_���v�I������
	CTime dumpEnd(pParent->m_EndTimeD.GetYear(), pParent->m_EndTimeD.GetMonth(), pParent->m_EndTimeD.GetDay(), pParent->m_EndTimeT.GetHour(), pParent->m_EndTimeT.GetMinute(), pParent->m_EndTimeT.GetSecond());
	CString strDumpEnd = GetTotalDay(dumpEnd);

	CString strEquip;
	CString filename;
	CString fullpath;
	TCHAR szwline[512];
	CStringA cbuf;


	CString strStation = mStationSimpleString[theApp.GetSelectStation()];
	CString ctrDir = mStationCtrlDir[theApp.GetSelectStation()];
	CString defDir = theApp.GetShareFilePath(eFileType_MonDump_Ftp, theApp.GetSelectStation());
	CString dir, pathTmp, ctrFulPath;

#if 1
	dir = defDir;
#else
	while (1)
	{
		dir = _T("");
		theApp.GetShellManager()->BrowseForFolder(dir, this, defDir, _T("�o�̓t�H���_��I�����Ă��������B"));

		if (dir == _T(""))
		{
			dir = defDir;
		}

		int idx = dir.Find(strStation.MakeLower());

		if (idx == -1)
		{
			CString msg;
			msg.Format(_T("%s�ȉ��̃t�H���_���w�肵�Ă��������B"), (LPCTSTR)strStation);
			AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION);
			continue;
		}

		idx += strStation.GetLength();
		pathTmp = dir.Mid(idx);
		pathTmp.Replace(_T('\\'), _T('/'));
		break;
	}
#endif


	for (int i = 0; i < nCount; ++i)
	{
		int idx = aryListBoxSel.GetAt(i);
		m_EquipList.GetText(idx, strEquip);

		filename.Format(_T("mon_dump_%s.txt"), (LPCTSTR)strEquip);
		fullpath.Format(_T("%smon_dump_%s.txt"), (LPCTSTR)dir, (LPCTSTR)strEquip);
		ctrFulPath = ctrDir + pathTmp + filename;

		if ((_wfopen_s(&fp, fullpath, _T("wb")) != 0) || (fp == NULL))
		{
			CString mes;
			mes.Format(_T("�_���v���s�t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)fullpath);
			AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		// �_���v�Ώۑ��u��
		wsprintf(szwline, _T("DEV             %s; %s\n"), (LPCTSTR)strEquip, (LPCTSTR)COMMENT_DEV);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp);

		// �_���v�J�n����
		wsprintf(szwline, _T("START_TIME      %s; %s\n"), (LPCTSTR)strDumpStart, (LPCTSTR)COMMENTSTART_TIME);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp);

		// �_���v�I������
		wsprintf(szwline, _T("STOP_TIME       %s; %s\n"), (LPCTSTR)strDumpEnd, (LPCTSTR)COMMENTSTOP_TIME);
		cbuf = CServerFile::ConvertEuc(szwline);
		fputs(cbuf, fp);

		vector<CString> &monList = EquipMonNameList[strEquip];
		for (int m = 0; m < monList.size(); ++m)
		{
			CString mon = monList[m];

			// �_���v�ΏۊĎ�����
			wsprintf(szwline, _T("WATCH           %s; %s\n"), (LPCTSTR)GetMonName(mon), (LPCTSTR)COMMENTWATCH);
			cbuf = CServerFile::ConvertEuc(szwline);
			fputs(cbuf, fp);
		}
		fclose(fp);

		// -----------------------------------------------------------------
		// �V�F���ďo

		// �������x
		CString seido;
		pParent->m_SearchTimeEdit.GetWindowTextW(seido);

		// �X�e�[�^�X
		int nStatus = pParent->m_StatusOutputCmb.GetCurSel();

		CString arg;
		CSshShell::structSshParam stParam;
		stParam.strBatchPath = theApp.GetSshBatchPath();
		stParam.strUserName = theApp.GetSshUserName();
		stParam.strServerName = theApp.GetSshServerName();
		stParam.strKeyFilePath = theApp.GetSshKeyPath();
		stParam.strRemShellName = MONDUMP_SH;

//		arg.Format(_T("%s %s %s %d"), CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), filename, seido, nStatus);
		arg.Format(_T("%s %s %s %d"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower(), (LPCTSTR)ctrFulPath, (LPCTSTR)seido, nStatus);
		stParam.strRemShellParam.Format(arg);

		int err = CSshShell::ExecShell(stParam);
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
			CLogTraceEx::Write(_T(""), _T("CEquipmentRecSelectEquipment"), _T("MON DUMP"), _T("debug"), strDebug, nLogEx::debug);
		}
	}
}
