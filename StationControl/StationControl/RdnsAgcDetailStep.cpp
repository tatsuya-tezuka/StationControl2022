/*============================================================================*/
/*! RdnsAgcDetailStep.cpp

-# �v�惌�f�B�l�X�`�F�b�N-AGC�Z���X�e�b�v��/�ϕ����Ԑݒ���
*/
/*============================================================================*/
// RdnsAgcDetailStep.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsAgcDetailStep.h"
#include "afxdialogex.h"

// CRdnsAgcDetailStep �_�C�A���O

IMPLEMENT_DYNAMIC(CRdnsAgcDetailStep, CDialogBase)

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# �R���X�g���N�^

@param  pParent	�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CRdnsAgcDetailStep::CRdnsAgcDetailStep(CWnd* pParent /*=NULL*/)
: CDialogBase(CRdnsAgcDetailStep::IDD, pParent)
{
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CRdnsAgcDetailStep::~CRdnsAgcDetailStep()
{
}

void CRdnsAgcDetailStep::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDNS_AGC_STEP_LIST, m_IntgListCtrl);
	DDX_Control(pDX, IDC_RDNS_AGC_STEP_INTG_EDIT, m_IntegEdit);
	DDX_Control(pDX, IDC_RDNS_AGC_STEP_PRF_CMB, m_CalibCmb);
}


BEGIN_MESSAGE_MAP(CRdnsAgcDetailStep, CDialogBase)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RDNS_AGC_STEP_INTG_BTN, &CRdnsAgcDetailStep::OnBnClickedRdnsAgcStepIntgBtn)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_RDNS_AGC_STEP_PRF_CMB, &CRdnsAgcDetailStep::OnCbnSelchangeRdnsAgcStepPrfCmb)
	ON_BN_CLICKED(IDOK, &CRdnsAgcDetailStep::OnBnClickedOk)
END_MESSAGE_MAP()


// CRdnsAgcDetailStep ���b�Z�[�W �n���h���[


int CRdnsAgcDetailStep::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


/*============================================================================*/
/*! CRdnsAgcDetailStep

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CRdnsAgcDetailStep::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// AGC�Z���e�[�u���t�@�C���ǂݍ���
	m_AgcTblDfltMap.clear();
	if (ReadAgcTblFile(TYPE_DFLT, m_AgcTblDfltMap) == FALSE)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	m_AgcTblCustMap.clear();
	if (ReadAgcTblFile(TYPE_CUST, m_AgcTblCustMap) == FALSE)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	// �Z���l�擾
	for (int i = 0; strStepComb[i] != _T(""); ++i)
	{
		m_CalibCmb.InsertString(-1, strStepComb[i]);
	}
	m_CalibCmb.SetCurSel(0);	// �f�t�H���g��1step

	// �ϕ�����

	// List
	DWORD dwStyle = m_IntgListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	m_IntgListCtrl.SetExtendedStyle(dwStyle);

	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("�v�����x��[dBm]"), _T("�f�t�H���g"), _T("�J�X�^�}�C�Y"), _T("�ϕ�����[s]") };
	int			csize[] = {95, 70, 70, 80};
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;

	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem	= i;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt			= LVCFMT_CENTER;
		lvc.pszText		= caption[i];
		lvc.cchTextMax = (int)wcslen(caption[i]);
		lvc.cx			= csize[i];
		m_IntgListCtrl.InsertColumn(i, &lvc);
	}

//#if 1
	int r = 0;
	LVITEM       lvi;
	stAgcTbl stCust;
	stAgcTbl stDflt;
	memset(&lvi, 0, sizeof(lvi));
	for (auto itr = m_AgcTblDfltMap.begin(); itr != m_AgcTblDfltMap.end(); ++itr)
	{
		CString att = itr->first;
		stDflt = itr->second;

		// �v�����x��
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvi.lParam = (LPARAM)r;
		lvi.iItem		= r;
		lvi.iSubItem	= 0;
		lvi.pszText		= stDflt.att.GetBuffer();
		m_IntgListCtrl.InsertItem(&lvi);

		// �f�t�H���g
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = r;
		lvi.iSubItem = 1;
		lvi.pszText = (LPWSTR)_T("");
		m_IntgListCtrl.SetItem(&lvi);

		// �J�X�^��
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.iItem = r;
		lvi.iSubItem = 2;
		lvi.pszText = (LPWSTR)_T("");
		m_IntgListCtrl.SetItem(&lvi);

		// �ϕ�����
		stCust = m_AgcTblCustMap[att];
		lvi.mask = LVIF_TEXT;
		lvi.iItem = r;
		lvi.iSubItem = 3;
		lvi.pszText = stCust.intg.GetBuffer();
		//lvi.cchTextMax = stCust.intg.GetLength();
		m_IntgListCtrl.SetItem(&lvi);

		++r;
	}

	// ���X�g���`�F�b�N�{�b�N�X�쐬
	m_IntgListCtrl.CreateCheckBox();

	r = 0;
	for (auto itr = m_AgcTblDfltMap.begin(); itr != m_AgcTblDfltMap.end(); ++itr)
	{
		CString att = itr->first;
		stDflt = itr->second;

		// �f�t�H���g
		m_IntgListCtrl.SetItemData(r, 0);
		LPARAM param = 0;
		if (stDflt.enable == _T("ON"))
		{
			param |= 0x01;
		}

		// �J�X�^�}�C�Y
		stCust = m_AgcTblCustMap[att];
		if (stCust.enable == _T("ON"))
		{
			param |= 0x02;
		}
		m_IntgListCtrl.SetItemData(r, param);

		++r;
	}



//#else
//
//	LVITEM       lvi;
//	CString      str;
//	for (int r = 0; r < 21; ++r)
//	{
//		// �v�����x��
//		lvi.lParam		= (LPARAM)r;
//		lvi.iItem		= r;
//		lvi.iSubItem	= 0;
//		lvi.mask		= LVIF_TEXT;
//		str.Format(_T("%d"), -100 - r );
//		lvi.pszText		= str.GetBuffer();
//		m_IntgListCtrl.InsertItem(&lvi);
//
//		// �ϕ�����
//		lvi.lParam = (LPARAM)r;
//		lvi.iItem = r;
//		lvi.iSubItem = 3;
//		lvi.mask = LVIF_TEXT;
//		if (r < 13)
//		{
//			str.Format(_T("%d"), 1);
//		}
//		else if (r >= 13 && r < 20)
//		{
//			str.Format(_T("%d"), 3);
//		}
//		else
//		{
//			str.Format(_T("%d"), 5);
//		}
//		lvi.pszText = str.GetBuffer();
//		m_IntgListCtrl.SetItem(&lvi);
//	}
//
//	m_IntgListCtrl.CreateCheckBoxColumn(1, 21, FALSE);
//	m_IntgListCtrl.CreateCheckBoxColumn(2, 21);
//
//	// �`�F�b�N�{�b�N�X�Ƀ`�F�b�N
//	OnCbnSelchangeRdnsAgcStepPrfCmb();
//#endif
	CenterWindowEx();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# �W�v�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsAgcDetailStep::OnBnClickedRdnsAgcStepIntgBtn()
{
	int count = m_IntgListCtrl.GetItemCount();

	CString str = _T("");
	int custTime = 0;
	// �J�X�^���̐ϕ����ԍ��v���v�Z
	for (int i = 0; i < count; ++i)
	{
		LPARAM param = m_IntgListCtrl.GetItemData(i);
		if (param & 0x02)
		{
			// �L���Ȑϕ����Ԃ����Z
			str = m_IntgListCtrl.GetItemText(i, 3);	// �J�X�^���ϕ����Ԏ擾
			custTime += _wtoi(str);
		}
	}

	// �f�t�H���g�̐ϕ����ԍ��v���v�Z
	int deftTime = 0;
	for (auto itr = m_AgcTblDfltMap.begin(); itr != m_AgcTblDfltMap.end(); ++itr)
	{
		stAgcTbl st = itr->second;
		if (st.enable == _T("ON"))
		{
			// �L���Ȑϕ����Ԃ����Z
			deftTime += _wtoi(st.intg);	// ATT�l�ɑΉ�����f�t�H���g�ϕ��l�擾
		}
	}

	str.Format(_T("%d"), custTime - deftTime);
	m_IntegEdit.SetWindowTextW(str);
}


/*============================================================================*/
/*! CRdnsAgcDetailStep

-# �ݒ�{�^������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsAgcDetailStep::OnBnClickedOk()
{
	// CUST�t�@�C���쐬
	WriteAgcTblFile();

//#if 0
//	// �t�@�C���R�s�[�i"_CUST"�����j
//	CString f_cust, f_lb;
//	CString folder = theApp.GetShareFilePath(eFileType_AGCCalib_Folder, theApp.GetSelectStation());
//	f_cust.Format(_T("%s%s\\agc_%s.tbl_%s_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand, (LPCTSTR)TypeFile[TYPE_CUST]);
//	f_lb.Format(_T("%s%s\\agc_%s.tbl_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand);
//
//	CopyFile(f_cust, f_lb, FALSE);
//#endif

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CRdnsAgcDetailStep

-# ����{�^������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsAgcDetailStep::OnClose()
{
	CDialogBase::OnClose();
}


/*============================================================================*/
/*! CRdnsAgcDetailStep

-# �Z���l�擾�R���{�{�b�N�X�ύX����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsAgcDetailStep::OnCbnSelchangeRdnsAgcStepPrfCmb()
{
	CString str;

	int idx = m_CalibCmb.GetCurSel();
	if (idx == -1)
	{
		return;
	}

	if ( (idx == STEP_1 )
		|| (idx == STEP_3)
		|| (idx == STEP_5)
		)
	{
		m_IntgListCtrl.CheckBoxCheckStep(2, STEP[idx]);
		m_CurrentStep = idx;
	}
	else if (idx == STEP_DEF)
	{
		SetDefaultValue();
	}
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# ���X�g���ڂ�DFLT��Ԃɂ���B

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CRdnsAgcDetailStep::SetDefaultValue()
{
	int row = 0;
	for (auto itr = m_AgcTblDfltMap.begin(); itr != m_AgcTblDfltMap.end(); ++itr)
	{
		CString att = itr->first;
		stAgcTbl st = itr->second;

		// �J�X�^�}�C�Y���f�t�H���g�l�ɂ���B
		LPARAM param = m_IntgListCtrl.GetItemData(row);
		if (st.enable == _T("ON"))
		{
			param = (param & ~0x02) | 0x02;
		}
		else
		{
			param = param & ~0x02;
		}
		m_IntgListCtrl.SetItemData(row, param);

		// �ϕ����Ԃ��f�t�H���g�l�ɂ���B
		m_IntgListCtrl.SetItemText(row, 3, st.intg);

		++row;
	}
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# AGC�Z���e�[�u���t�@�C���ǂݍ���

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CRdnsAgcDetailStep::ReadAgcTblFile(int type, map<CString, stAgcTbl> &map)
{
	FILE *fp = NULL;

	CString file;
	TCHAR sz_tmp1[20];
	TCHAR sz_tmp2[20];

	char sz_line[512];
	
	CString cbuf;
	CString str, tag;
	stAgcTbl st;

	CString folder = theApp.GetShareFilePath(eFileType_AGCCalib_Folder, theApp.GetSelectStation());
	file.Format(_T("%s%s\\agc_%s.tbl_%s_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand, (LPCTSTR)TypeFile[type]);

	if (_wfopen_s(&fp, file, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("AGC�Z���e�[�u���t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)file);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	while (fgets(sz_line, static_cast<unsigned>(_countof(sz_line)), fp) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(sz_line);
		if (cbuf.Mid(0, 1) == '#' || cbuf.Mid(0, 1) == '\n')
		{
			continue;
		}

		// TAG
		AfxExtractSubString(str, cbuf, 0, ',');

		swscanf_s(str.GetBuffer(), _T("%s %s"),
			sz_tmp1, static_cast<unsigned>(_countof(sz_tmp1)),
			sz_tmp2, static_cast<unsigned>(_countof(sz_tmp2)));

		tag = CString(sz_tmp1);
		if (tag.Find(_T("MAIN_CAL")) == -1)
		{
			continue;
		}

		st.att = CString(sz_tmp2);

		// �Z���l
		AfxExtractSubString(str, cbuf, 1, ',');
		str.TrimLeft(_T(" "));
		st.calib = str;

		// ������e�l
		AfxExtractSubString(str, cbuf, 2, ',');
		str.TrimLeft(_T(" "));
		st.u_limit = str;

		// �������e�l
		AfxExtractSubString(str, cbuf, 3, ',');
		str.TrimLeft(_T(" "));
		st.l_limit = str;

		// ATT����
		AfxExtractSubString(str, cbuf, 4, ',');
		str.TrimLeft(_T(" "));
		st.incr = str;

		// �Z���C���l
		AfxExtractSubString(str, cbuf, 5, ',');
		str.TrimLeft(_T(" "));
		st.corr = str;

		// �Z���҂�����
		AfxExtractSubString(str, cbuf, 6, ',');
		str.TrimLeft(_T(" "));
		st.wait = str;

		// �ϕ�����
		AfxExtractSubString(str, cbuf, 7, ',');
		str.TrimLeft(_T(" "));
		st.intg = str;

		// �L������
		AfxExtractSubString(str, cbuf, 8, ',');
		str.TrimLeft(_T(" "));
		int idx = str.Find(_T(";"));
		st.enable = str.Mid(0, idx);

		// �R�����g
		st.coment = str.Mid(idx+1);

		map[st.att] = st;
	}

	fclose(fp);

	return TRUE;
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# AGC�Z���e�[�u���t�@�C���ǂݍ���

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CRdnsAgcDetailStep::WriteAgcTblFile()
{
	FILE *fp_cust = NULL;
	FILE *fp_dflt = NULL;

	char sz_rline[512];
	CString rbuf;
	TCHAR sz_buff[512];
	TCHAR sz_tmp1[20];
	TCHAR sz_tmp2[20];

	CStringA wbuf;
	TCHAR sz_wline[2048];


	CString f_cust;
	CString f_dflt;


	CString folder = theApp.GetShareFilePath(eFileType_AGCCalib_Folder, theApp.GetSelectStation());
	f_cust.Format(_T("%s%s\\agc_%s.tbl_%s_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand, (LPCTSTR)TypeFile[TYPE_CUST]);
	f_dflt.Format(_T("%s%s\\agc_%s.tbl_%s_%s"), (LPCTSTR)folder, (LPCTSTR)m_Satellite, (LPCTSTR)m_AgcItem, (LPCTSTR)m_LoopBand, (LPCTSTR)TypeFile[TYPE_DFLT]);

	if (_wfopen_s(&fp_dflt, f_dflt, _T("r")) != 0)
	{
		CString mes;
		mes.Format(_T("AGC�Z���e�[�u���t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)f_dflt);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (_wfopen_s(&fp_cust, f_cust, _T("wb")) != 0)
	{
		CString mes;
		mes.Format(_T("AGC�Z���e�[�u���t�@�C�����J�����Ƃ��ł��܂���B\n%s"), (LPCTSTR)f_cust);
		AfxMessageBox(mes, MB_OK | MB_ICONEXCLAMATION);
		fclose(fp_dflt);
		return FALSE;
	}

	SYSTEMTIME time;
	GetSystemTime(&time);
	CTime ctime(time);
	CString strTime;
	strTime = ctime.Format("%Y-%m-%d %H:%M:%S");

	while (fgets(sz_rline, static_cast<unsigned>(_countof(sz_rline)), fp_dflt) != NULL)
	{
		rbuf = CServerFile::ConvertUnicode(sz_rline);
		wsprintf(sz_buff, _T("%s"), (LPCTSTR)CServerFile::ConvertUnicode(sz_rline));

		if (!wcsncmp(sz_buff, _T("#!HEAD:"), 7))
		{
			wsprintf(sz_wline, _T("#!HEAD: AGCTBL %s ---------- -- %s\n")
				, (LPCTSTR)strTime
				, (LPCTSTR)m_Satellite);
			wbuf = CServerFile::ConvertEuc(sz_wline);
			fputs(wbuf, fp_cust);
			continue;
		}
		else if (sz_buff[0] == '#' || sz_buff[0] == '\n')
		{
			wbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(wbuf, fp_cust);
			continue;
		}

		CString str;
		// TAG
		AfxExtractSubString(str, rbuf, 0, ',');

		swscanf_s(str.GetBuffer(), _T("%s %s"),
			sz_tmp1, static_cast<unsigned>(_countof(sz_tmp1)),
			sz_tmp2, static_cast<unsigned>(_countof(sz_tmp2)));

		CString tag = CString(sz_tmp1);
		if (tag.Find(_T("MAIN_CAL")) == -1)
		{
			wbuf = CServerFile::ConvertEuc(sz_buff);
			fputs(wbuf, fp_cust);
			continue;
		}

		CString dAtt = CString(sz_tmp2);

		int size = m_IntgListCtrl.GetItemCount();
		for (int i = 0; i < size; ++i)
		{
			CString att = m_IntgListCtrl.GetItemText(i, 0);
			if (att == dAtt)
			{
				stAgcTbl &stCust = m_AgcTblCustMap[att];

				// �J�X�^��
				LPARAM param = m_IntgListCtrl.GetItemData(i);
				CString enable = _T("OFF");
				if (param & 0x02)
				{
					enable = _T("ON");
				}

				// �ϕ�����
				CString iteg = m_IntgListCtrl.GetItemText(i, 3);

				wsprintf(sz_wline, _T("%s %5s, %8s, %10s, %10s, %7s, %10s, %10s, %8s, %8s;%s")
					, (LPCTSTR)tag
					, (LPCTSTR)att
					, (LPCTSTR)stCust.calib
					, (LPCTSTR)stCust.u_limit
					, (LPCTSTR)stCust.l_limit
					, (LPCTSTR)stCust.incr
					, (LPCTSTR)stCust.corr
					, (LPCTSTR)stCust.wait
					, (LPCTSTR)iteg
					, (LPCTSTR)enable
					, (LPCTSTR)stCust.coment);
				wbuf = CServerFile::ConvertEuc(sz_wline);
				fputs(wbuf, fp_cust);
			}
		}

	}

	fclose(fp_dflt);
	fclose(fp_cust);

	return TRUE;
}

/*============================================================================*/
/*! CRdnsAgcDetailStep

-# AGC�X�e�b�v/�ϕ����Ԃ̓��͔͈͒l�̃��b�Z�[�W�\���B

@param  message	�F���b�Z�[�W�̎��ʎq
@param  wParam	�F���b�Z�[�W�̍ŏ��̃p�����[�^
@param  lParam	�F���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
@retval ��������
*/
/*============================================================================*/
LRESULT CRdnsAgcDetailStep::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){

	case	eMessage_AgcStepOutOFRange:
		AfxMessageBox(_T("�ϕ����Ԃ�1�`300[�b]����͂��Ă��������B"), MB_OK | MB_ICONEXCLAMATION);
		break;

	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
