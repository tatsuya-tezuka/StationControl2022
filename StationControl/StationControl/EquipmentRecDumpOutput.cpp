/*============================================================================*/
/*! EquipmentRecDumpOutput.cpp

-# �ݔ��L�^ �Ď��f�[�^�_���v���
*/
/*============================================================================*/
// EquipmentRecDumpOutput.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentRecDumpOutput.h"
#include "afxdialogex.h"
#include "MonSearchDlg.h"
#include "HookMsgBox.h"

// CEquipmentRecDumpOutput �_�C�A���O

IMPLEMENT_DYNAMIC(CEquipmentRecDumpOutput, CDialogBase)

CEquipmentRecDumpOutput::CEquipmentRecDumpOutput(CWnd* pParent /*=NULL*/)
: CDialogBase(CEquipmentRecDumpOutput::IDD, pParent)
	, m_strFileId(_T(""))
	, m_StartTimeD(0)
	, m_StartTimeT(0)
	, m_EndTimeD(0)
	, m_EndTimeT(0)
	, m_strDirectEntry(_T(""))
{

}

CEquipmentRecDumpOutput::~CEquipmentRecDumpOutput()
{
}

void CEquipmentRecDumpOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_EQIPSEL_CMB, m_EquipmentSelCmb);
	DDX_Text(pDX, IDC_EQIPREC_DUMPOUTPUT_FID_EDIT, m_strFileId);
	DDX_DateTimeCtrl(pDX, IDC_EQIPREC_DUMPOUTPUT_OSTIME_DAY_DP, m_StartTimeD);
	DDX_DateTimeCtrl(pDX, IDC_EQIPREC_DUMPOUTPUT_OSTIME_TIME_DP, m_StartTimeT);
	DDX_DateTimeCtrl(pDX, IDC_EQIPREC_DUMPOUTPUT_OETIME_DAY_DP, m_EndTimeD);
	DDX_DateTimeCtrl(pDX, IDC_EQIPREC_DUMPOUTPUT_OETIME_TIME_DP, m_EndTimeT);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_SEARCH_EDIT, m_SearchTimeEdit);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_STATUS_CMB, m_StatusOutputCmb);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_MNTD_LIST, m_MonitoringDataList);
	DDX_Text(pDX, IDC_EQIPREC_DUMPOUTPUT_DIRECT_EDIT, m_strDirectEntry);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_OSTIME_DAY_DP, m_StartTimeDTCtrl);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_OETIME_DAY_DP, m_EndTimeDTCtrl);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_FID_SAVE_BTN, m_SaveBtnCtrl);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_OSTIME_TIME_DP, m_StartTimeTTCtrl);
	DDX_Control(pDX, IDC_EQIPREC_DUMPOUTPUT_OETIME_TIME_DP, m_EndTimeTTCtrl);
}


BEGIN_MESSAGE_MAP(CEquipmentRecDumpOutput, CDialogBase)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_FID_SAVE_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputFidSaveBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_FID_READ_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputFidReadBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_MNTD_UP_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdUpBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_MNTD_DWN_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdDwnBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_MNTD_DEL_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdDelBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_DIRECT_ADD_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputDirectAddBtn)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_EXEC_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputExecBtn)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CEquipmentRecDumpOutput::OnMenuCloseClose)
	ON_BN_CLICKED(IDC_EQIPREC_DUMPOUTPUT_DIRECT_SERCH_BTN, &CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputDirectSerchBtn)
	ON_CBN_SELCHANGE(IDC_EQIPREC_DUMPOUTPUT_EQIPSEL_CMB, &CEquipmentRecDumpOutput::OnCbnSelchangeEqiprecDumpoutputEqipselCmb)
	ON_EN_CHANGE(IDC_EQIPREC_DUMPOUTPUT_FID_EDIT, &CEquipmentRecDumpOutput::OnEnChangeEqiprecDumpoutputFidEdit)
END_MESSAGE_MAP()


// CEquipmentRecDumpOutput ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CEquipmentRecDumpOutput::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �_�C�A���O�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CEquipmentRecDumpOutput::UpdateDlg()
{
	// ���ݎ����擾
	SYSTEMTIME timeSys;
	GetSystemTime(&timeSys);
	CTime time = CTime(timeSys);


	// �o�͊J�n����
	m_StartTimeDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_StartTimeTTCtrl.SetFormat(_T("HH:mm:ss"));
	CTimeSpan    cTimeSpan(0, 1, 0, 0);

	m_StartTimeD = time - cTimeSpan;
	m_StartTimeT = time - cTimeSpan;

	// �o�͏I������
	m_EndTimeDTCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_EndTimeTTCtrl.SetFormat(_T("HH:mm:ss"));
	m_EndTimeD = time;
	m_EndTimeT = time;

	// �������x
	m_SearchTimeEdit.SetWindowTextW(_T("0"));

	// �X�e�[�^�X�o�͕��@
	m_StatusOutputCmb.ResetContent();
	m_StatusOutputCmb.InsertString(-1, _T("����"));
	m_StatusOutputCmb.InsertString(-1, _T("���l"));
	m_StatusOutputCmb.SetCurSel(0);

	// �ۑ��{�^��Disable
	m_SaveBtnCtrl.EnableWindow(FALSE);

	// �ݔ��I��
	m_EquipmentSelCmb.ResetContent();

	// �t�@�C������
	m_strFileId = _T("");

	// �Ď��f�[�^
	m_MonitoringDataList.ResetContent();

	// �f�[�^���X�g
	m_EquipMonNameList.clear();

	UpdateData(FALSE);

	OnBnClickedEqiprecDumpoutputFidReadBtn();

	return TRUE;
}

/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �w�ۑ��x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputFidSaveBtn()
{
	UpdateData();

	FILE	*fp;
	TCHAR	sz_buff[512];
	CStringA cbuf;

	CString folder = theApp.GetShareFilePath(eFileType_MonDump_Def, theApp.GetSelectStation());

	CString		filter("�_���v��`�t�@�C�� (*.txt)|*.txt||");
	CFileDialog	dlg(FALSE, NULL, m_strFileId, OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, filter);
	dlg.m_ofn.lpstrTitle = _T("�_���v��`�t�@�C���I��");
	dlg.m_ofn.lpstrInitialDir = folder;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CString file = dlg.GetPathName();
	if (file.Find(_T(".txt")) == -1)
	{
		file = file + _T(".txt");
	}

	if ((_wfopen_s(&fp, file, _T("wb")) != 0) ||( fp == NULL))
	{
		CString mes;
		mes.Format(_T("�_���v��`�t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	CString str;
	for (int i = 0; i < m_EquipmentSelCmb.GetCount(); ++i)
	{
		m_EquipmentSelCmb.GetLBText(i, str);
		vector<CString> &monlist = m_EquipMonNameList[str];
		for (int i = 0; i < monlist.size(); ++i)
		{
			CString mon = monlist[i];
			wsprintf(sz_buff, _T("%s\n"), (LPCTSTR)mon);
			cbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(cbuf, fp);
		}
	}
	fclose(fp);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �w�Ǎ��x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputFidReadBtn()
{
	FILE	*fp;

	CString folder = theApp.GetShareFilePath(eFileType_MonDump_Def, theApp.GetSelectStation());

	CString	filter("�_���v��`�t�@�C�� (*.txt)|*.txt||");
	CFileDialog	dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, filter);
	dlg.m_ofn.lpstrTitle = _T("�_���v��`�t�@�C���I��");
	dlg.m_ofn.lpstrInitialDir = folder;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CString file = dlg.GetPathName();

	if ((_wfopen_s(&fp, file, _T("r")) != 0) || (fp == NULL))
	{
		CString mes;
		mes.Format(_T("�_���v��`�t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	m_EquipmentSelCmb.ResetContent();
	m_MonitoringDataList.ResetContent();
	m_EquipMonNameList.clear();

	char sz_line[512];
	CString cbuf;
	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(sz_line);
		cbuf = cbuf.Mid(0, cbuf.GetLength() - 1);	// ���s�R�[�h�폜
		SetDlgData(CString(cbuf));
	}
	fclose(fp);


	// �ݔ��I���X�V
	m_EquipmentSelCmb.SetCurSel(0);
	
	// �Ď��f�[�^�X�V
	CString equip;
	m_EquipmentSelCmb.GetLBText(0, equip);
	ResetList(equip);

	// �t�@�C�����ʍX�V
	m_strFileId = dlg.GetFileTitle();
	m_SaveBtnCtrl.EnableWindow(TRUE);

	UpdateData(FALSE);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �Ď��f�[�^���X�g�Đݒ�

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::ResetMonMapList()
{
	CString str;
	CString equip;
	m_EquipmentSelCmb.GetWindowTextW(equip);

	vector<CString> monlist;
	int idx = m_MonitoringDataList.GetCount();
	if (idx == 0)
	{
		m_EquipMonNameList.erase(equip);
		m_EquipmentSelCmb.DeleteString(m_EquipmentSelCmb.FindStringExact(-1, equip));
	}
	else
	{
		for (int i = 0; i < idx; ++i)
		{
			m_MonitoringDataList.GetText(i, str);
			monlist.push_back(str);
		}
		m_EquipMonNameList[equip].swap(monlist);
	}
}
	
/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �w1�s��ցx�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdUpBtn()
{
	int nIndex = m_MonitoringDataList.GetCurSel();
	if (nIndex == -1 || nIndex == 0)
		return;

	CString str;
	m_MonitoringDataList.GetText(nIndex, str);
	m_MonitoringDataList.DeleteString(nIndex);
	nIndex = m_MonitoringDataList.InsertString(nIndex - 1, str);
	m_MonitoringDataList.SetCurSel(nIndex);

	ResetMonMapList();
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �w1�s���ցx�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdDwnBtn()
{
	int nIndex = m_MonitoringDataList.GetCurSel();
	if (nIndex == m_MonitoringDataList.GetCount() - 1)
		return;

	CString str;
	m_MonitoringDataList.GetText(nIndex, str);
	m_MonitoringDataList.DeleteString(nIndex);
	nIndex = m_MonitoringDataList.InsertString(nIndex + 1, str);
	m_MonitoringDataList.SetCurSel(nIndex);

	ResetMonMapList();
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �w1�s�폜�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputMntdDelBtn()
{
	int nIndex = m_MonitoringDataList.GetCurSel();
	m_MonitoringDataList.DeleteString(nIndex);

	ResetMonMapList();
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# ���X�g�X�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::SetDlgData(CString mon)
{
	CString equip = _T("");
	int idx = mon.Find(_T("."));
	equip = mon.Mid(0, idx);
	if (m_EquipmentSelCmb.FindStringExact(-1, equip) == CB_ERR)
	{
		m_EquipmentSelCmb.InsertString(-1, equip);
	}
	m_EquipmentSelCmb.SelectStringEx(-1, equip);

	// �Ď����X�g�ɒǉ�
	if (m_EquipMonNameList.count(equip) == 0)
	{
		vector<CString> monlist;
		monlist.push_back(mon);
		m_EquipMonNameList[equip] = monlist;
	}
	else
	{
		BOOL bFind = FALSE;
		vector<CString> &monlist = m_EquipMonNameList[equip];
		for (int i = 0; i < monlist.size(); ++i)
		{
			if (monlist[i] == mon)
			{
				AfxMessageBox(_T("�ǉ����悤�Ƃ����Ď����͓o�^�ς݂ł��B"), MB_OK | MB_ICONWARNING);
				bFind = TRUE;
				break;
			}
		}

		if (bFind == FALSE)
		{
			monlist.push_back(mon);
		}
	}
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �w�ǉ��x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputDirectAddBtn()
{
	UpdateData();

	// ���͂��ꂽ�Ď��f�[�^��DB�����ꍇ�́A���b�Z�[�W�o��
	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();

	BOOL bFind = FALSE;
	vector<CString>& mList = dba.GetMonNameList();
	int cnt = (int)mList.size();
	for (int i = 0; i < cnt; ++i)
	{
		if (m_strDirectEntry == mList[i])
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind == FALSE)
	{
		AfxMessageBox(_T("���͂��ꂽ�Ď������s���ł��B"), MB_OK | MB_ICONWARNING);
		return;
	}

	// �ݔ��I���R���{�{�b�N�X�ɓo�^
	SetDlgData(m_strDirectEntry);

	CString equip = _T("");
	int idx = m_strDirectEntry.Find(_T("."));
	equip = m_strDirectEntry.Mid(0, idx);

	ResetList(equip);

	m_strDirectEntry = _T("");
	UpdateData(FALSE);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �w���s�x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputExecBtn()
{
	UpdateData(TRUE);

	// �����`�F�b�N
	// �o�͊J�n����
	CTime dumpStart(m_StartTimeD.GetYear(), m_StartTimeD.GetMonth(), m_StartTimeD.GetDay(), m_StartTimeT.GetHour(), m_StartTimeT.GetMinute(), m_StartTimeT.GetSecond());
	// �o�͏I������
	CTime dumpEnd(m_EndTimeD.GetYear(), m_EndTimeD.GetMonth(), m_EndTimeD.GetDay(), m_EndTimeT.GetHour(), m_EndTimeT.GetMinute(), m_EndTimeT.GetSecond());
	if (dumpStart.GetTime() > dumpEnd.GetTime())
	{
		CString mes = _T("");
		CString strStartTime = dumpStart.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString strEndTime = dumpEnd.Format(_T("%Y-%m-%d %H:%M:%S"));
		mes.Format(_T("�o�͏I���������o�͊J�n���������ߋ��̂��߁A�Ď��_���v�o�͂ł��܂���B\n\n �o�͊J�n���� :%s\n �o�͏I������ :%s"), (LPCTSTR)strStartTime, (LPCTSTR)strEndTime);
		AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
		return;
	}

	vector<CString> list;
	CString str;
	for (int i = 0; i < m_EquipmentSelCmb.GetCount(); ++i)
	{
		m_EquipmentSelCmb.GetLBText(i, str);
		list.push_back(str);
	}

	CEquipmentRecSelectEquipment dlg(this);
	dlg.m_EquipNameList.clear();
	dlg.m_EquipMonNameList.clear();
	dlg.m_EquipNameList = list;
	dlg.m_EquipMonNameList = m_EquipMonNameList;
	dlg.DoModal();
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �t�@�C��-���郁�j���[����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnMenuCloseClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �w�����x�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnBnClickedEqiprecDumpoutputDirectSerchBtn()
{
	UpdateData();

	CMonSearchDlg dlg;
	dlg.m_strMonName = m_strDirectEntry;
	if (IDOK == dlg.DoModal())
	{
		m_strDirectEntry = dlg.m_MonNameSearch;
		UpdateData(FALSE);
	}
}

/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# ���X�g�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::ResetList(CString name)
{
//	m_strDirectEntry

	m_MonitoringDataList.SetRedraw(FALSE);
	m_MonitoringDataList.ResetContent();

#if 1
	vector<CString> &monlist = m_EquipMonNameList[name];
	for(int i = 0; i < monlist.size(); ++i)
	{
		m_MonitoringDataList.InsertString(-1, monlist[i]);
	}
	m_MonitoringDataList.SetRedraw(TRUE);

#else
	int cnt = (int)m_EquipNameListTemp.size();
	CString str;
	name.MakeLower();
	for (int i = 0; i < cnt; ++i)
	{
		if (name.IsEmpty() == false)
		{
			str = m_EquipNameListTemp[i];
			str.MakeLower();
			if (str.Find(name) == 0)
			{
				m_MonitoringDataList.InsertString(-1, m_EquipNameListTemp[i]);
			}
		}
		else
		{
			m_MonitoringDataList.InsertString(-1, m_EquipNameListTemp[i]);
		}
	}
	m_MonitoringDataList.SetRedraw(TRUE);
#endif
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �ݔ��I���R���{�{�b�N�X�ύX����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnCbnSelchangeEqiprecDumpoutputEqipselCmb()
{
	CString str;

	int idx = m_EquipmentSelCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}
	m_EquipmentSelCmb.GetLBText(idx, str);
	ResetList(str);
}


/*============================================================================*/
/*! CEquipmentRecDumpOutput

-# �t�@�C������ ���͏���

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CEquipmentRecDumpOutput::OnEnChangeEqiprecDumpoutputFidEdit()
{
	UpdateData();

	if (m_strFileId.GetLength() != 0)
	{
		m_SaveBtnCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_SaveBtnCtrl.EnableWindow(FALSE);
	}

	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A���̃R���g���[����
	// ���̒ʒm�𑗐M����ɂ́ACDialogBase::OnInitDialog() �֐����I�[�o�[���C�h���A
	// CRichEditCtrl().SetEventMask() ��
	// OR ��Ԃ� ENM_CHANGE �t���O���}�X�N�ɓ���ČĂяo���K�v������܂��B

	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����Ă��������B
}
