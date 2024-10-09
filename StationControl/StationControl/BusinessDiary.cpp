/*============================================================================*/
/*! BusinessDiary.cpp

-# �����Ɩ������ꗗ���
*/
/*============================================================================*/
// BusinessDiary.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "BusinessDiary.h"
#include "afxdialogex.h"
#include "..\Utility\SshShell.h"
#include "BusinessDiaryFileNotify.h"
#include "WaitDialog.h"

#include "HookMsgBox.h"

#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")


#define	PRINT_FORM_CSV_FILE	_T("REP_FORM_CSV.txt")

const char* mEndTime[3] = { "07:59:59", "15:59:59", "23:59:59" };

//----------------------------------------------------------------------
// CEditInListCtrl

IMPLEMENT_DYNAMIC(CDiaryListCtrl, CListCtrl)

/*============================================================================*/
/*! CDiaryListCtrl

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CDiaryListCtrl::CDiaryListCtrl()
{
	mListNo = 0;
}

/*============================================================================*/
/*! CDiaryListCtrl

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CDiaryListCtrl::~CDiaryListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDiaryListCtrl, CListCtrl)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKA, 0, &CDiaryListCtrl::OnHdnDividerdblclick)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, &CDiaryListCtrl::OnHdnDividerdblclick)
	ON_NOTIFY(HDN_ENDTRACKA, 0, &CDiaryListCtrl::OnHdnEndtrack)
	ON_NOTIFY(HDN_ENDTRACKW, 0, &CDiaryListCtrl::OnHdnEndtrack)
END_MESSAGE_MAP()


/*============================================================================*/
/*! CDiaryListCtrl

-# �R���g���[���ʒm�̃��b�Z�[�W �}�b�v���������܂��B
   �w�b�_�[���Œ�ɂ���B

@param  wParam	�F���b�Z�[�W���R���g���[�����炻�̃��b�Z�[�W�𑗐M����R���g���[�������ʂ��܂��B
@param  lParam	�F�ʒm�R�[�h�ƒǉ������܂ޒʒm���b�Z�[�W (NMHDR) �̍\���̂ւ̃|�C���^�[�B
@param  pResult	�F���b�Z�[�W���������ꂽ�Ƃ����ʂ��i�[����R�[�h���� LRESULT �̕ϐ��ւ̃|�C���^�[�B
@retval ���b�Z�[�W�����������ꍇ��1��Ԃ��܂�; ����ȊO�̏ꍇ�� 0�B
*/
/*============================================================================*/
//BOOL CDiaryListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	HD_NOTIFY *phdn = (HD_NOTIFY *)lParam;
//	if (phdn->hdr.code == HDN_BEGINTRACK) {
//		// �J�������Œ�
////		if ((phdn->iItem == 0) || (phdn->iItem == 1) || (phdn->iItem == 2) || (phdn->iItem == 3)) 
//		{
//			*pResult = TRUE;
//			return(1);
//		}
//	}
//	return(0);
////	return CListCtrl::OnNotify(wParam, lParam, pResult);
//}
/*============================================================================*/
/*! �Ɩ�����

-# �J�����̎����T�C�Y����

@param	col		�J�����ʒu�̃J�����T�C�Y�������ɒ������܂�
@retval

*/
/*============================================================================*/
void CDiaryListCtrl::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	// create dummy
	int dummycol = pHeader->GetItemCount();
	InsertColumn(dummycol, _T("dummy"), LVCFMT_LEFT, 10);

	SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = GetColumnWidth(col);
		SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = GetColumnWidth(col);
		int wc = MAX(75, MAX(wc1, wc2));
		if (col == 0)
			wc = 50;
		SetColumnWidth(col, wc);
	}
	// delete dummy
	DeleteColumn(dummycol);
	SetRedraw(true);
}
/*============================================================================*/
/*! �Ɩ�����

-# �J�����̎����T�C�Y����

@param	col		�J�����ʒu�̃J�����T�C�Y�������ɒ������܂�
@retval

*/
/*============================================================================*/
void CDiaryListCtrl::ResetColumns(vector<int>& cols)
{
	//{ _T(""),                               _T("�i��C�j�����Ǐ]����"), _T("�����ݔ������"),   _T("") };
	//{ _T("�ԍ�"), _T("�^�p��"), _T("�V��"), _T("���i"), _T("����"),     _T("���i"), _T("����"), _T("�����"), _T("���ʐM���ԁi���j"), _T("�ʐM��") };

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	if (maxcol > cols.size())
		return;

	if (maxcol == 4 && cols.size() == 10){
		// ���C������T�u
		int width = cols[0] + cols[1] + cols[2];
		SetColumnWidth(0, width);
		width = cols[3] + cols[4];
		SetColumnWidth(1, width);
		width = cols[5] + cols[6];
		SetColumnWidth(2, width);
		width = cols[7] + cols[8] + cols[9];
		SetColumnWidth(3, width);
	}
	//if (maxcol == 10 && cols.size() == 4){
	//	// �T�u���烁�C��
	//	int width = cols[0];
	//	int wc = GetColumnWidth(2);
	//	if (width > wc){
	//		SetColumnWidth(2, 0);
	//		width -= wc;
	//		wc = GetColumnWidth(1);
	//		if (width > wc){
	//			SetColumnWidth(1, 0);
	//			width -= wc;
	//		}
	//		else{
	//		}
	//	}
	//	else{
	//		SetColumnWidth(2, wc - width);
	//	}
	//	width = cols[3] + cols[4];
	//	SetColumnWidth(1, width);
	//	width = cols[5] + cols[6];
	//	SetColumnWidth(2, width);
	//	width = cols[7] + cols[8] + cols[9];
	//	SetColumnWidth(3, width);
	//}
}

/*============================================================================*/
/*! �Ɩ�����

-# �Ɩ��������ڂ̓o�^

@param	nItem		�A�C�e���ԍ�
@param	nSubItem	�T�u�A�C�e���ԍ�
@param	strItem		�ݒ蕶����
@param	dwParam		���[�U�l
@retval

*/
/*============================================================================*/
int CDiaryListCtrl::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwParam;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return InsertItem(&lv);
	return SetItem(&lv);
}


void CDiaryListCtrl::OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	mHeaderSize.clear();
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	for (int col = 0; col < maxcol; col++){
		int wc = GetColumnWidth(col);
		mHeaderSize.push_back(wc);
	}
	GetParent()->PostMessage(WM_USER + 200, 0, mListNo);
	*pResult = 0;
}


void CDiaryListCtrl::OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	mHeaderSize.clear();
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	for (int col = 0; col < maxcol; col++){
		int wc = GetColumnWidth(col);
		mHeaderSize.push_back(wc);
	}
	GetParent()->PostMessage(WM_USER + 200, 0, mListNo);
	*pResult = 0;
}


// CBusinessDiary �_�C�A���O

IMPLEMENT_DYNAMIC(CBusinessDiary, CDialogBase)

/*============================================================================*/
/*! CBusinessDiary

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CBusinessDiary::CBusinessDiary(CWnd* pParent /*=NULL*/)
: CDialogBase(CBusinessDiary::IDD, pParent)
	, m_DayTime(0)
{
	mbEdit = FALSE;
}

/*============================================================================*/
/*! CBusinessDiary

-# �f�X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CBusinessDiary::~CBusinessDiary()
{
}

void CBusinessDiary::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DIARY_STLT_CMB, m_SatelliteCmb);
	DDX_DateTimeCtrl(pDX, IDC_DIARY_STLT_DP, m_DayTime);
	DDX_Control(pDX, IDC_DIARY_LIST, m_DiaryList);
	DDX_Control(pDX, IDC_DIARY_LIST2, m_DiaryList2);
	DDX_Control(pDX, IDC_DIARY_STLT_DP, mDate);
}


BEGIN_MESSAGE_MAP(CBusinessDiary, CDialogBase)
	ON_BN_CLICKED(IDC_DIARY_EDIT_BTN, &CBusinessDiary::OnBnClickedDiaryEditBtn)
	ON_BN_CLICKED(IDC_DIARY_PRNT_BTN, &CBusinessDiary::OnBnClickedDiaryPrntBtn)
	ON_BN_CLICKED(IDC_DIARY_CSV_BTN, &CBusinessDiary::OnBnClickedDiaryCsvBtn)
	ON_BN_CLICKED(IDC_DIARY_SINFO_BTN, &CBusinessDiary::OnBnClickedDiarySinfoBtn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DIARY_STLT_DP, &CBusinessDiary::OnDtnDatetimechangeDiaryStltDp)
	ON_CBN_SELCHANGE(IDC_DIARY_STLT_CMB, &CBusinessDiary::OnCbnSelchangeDiaryStltCmb)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CBusinessDiary::OnMenuCloseClose)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DIARY_LIST, &CBusinessDiary::OnLvnItemchangedDiaryList)
END_MESSAGE_MAP()


// CBusinessDiary ���b�Z�[�W �n���h���[
/*============================================================================*/
/*! CBusinessDiary

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CBusinessDiary::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �p�X�����m�F
	if (IsPass() == TRUE)
	{
		// �p�X���͊J���Ȃ�
		EndDialog(IDCANCEL);
		return TRUE;  // return TRUE unless you set the focus to a control
	}

	// �����Ɩ������擾
#ifndef _DEBUG
//	if (Shell_GetRadiolog() == FALSE)
//	{
//		EndDialog(IDCANCEL);
//		return TRUE;  // return TRUE unless you set the focus to a control
//	}

	Shell_GetRadiolog();	// �t�@�C�������݂��Ȃ��P�[�X�����肦�邽�߁A�V�F���̖߂�͌��Ȃ�
#endif

	// RF�t�@�C�������������m�F
	if (CheckFormatRFFile() == FALSE)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	// RF���t�@�C���̍ŏI�s��ON�����邩�m�F
	if (CheckRFInfoFile() == TRUE)
	{
		EndDialog(IDCANCEL);
		return TRUE;  // return TRUE unless you set the focus to a control
	}

#if 0
	// �����Ɩ������t�@�C���m�F
	CBusinessDiaryFileNotify dlg;
	if (dlg.DoModal() == IDCANCEL){
		EndDialog(IDCANCEL);
		return TRUE;  // return TRUE unless you set the focus to a control
	}
#endif

	//-----------------------------------------------------
	// ���t�t�H�[�}�b�g�ύX
	mDate.SetFormat(_T("yyyy-MM-dd"));

	//-----------------------------------------------------
	// DiaryList2(��ԏ�̃w�b�_)
	DWORD dwStyle2 = m_DiaryList2.GetExtendedStyle();
	dwStyle2 |= LVS_EX_GRIDLINES;
	m_DiaryList2.SetExtendedStyle(dwStyle2);

	LVCOLUMN    lvc2;
	TCHAR       caption2[][32] = { _T(""), _T("�i��C�j�����Ǐ]����"), _T("�����ݔ������"), _T("") };
	int			csize2[] = { 200, 170, 170, 260 };
	const int   clmNum2 = sizeof caption2 / sizeof caption2[0];

	for (int i = 0; i < clmNum2; i++)
	{
		lvc2.iSubItem = i;
		lvc2.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc2.fmt = LVCFMT_CENTER;
		lvc2.pszText = caption2[i];
		lvc2.cchTextMax = (int)wcslen(caption2[i]);
		lvc2.cx = csize2[i];
		m_DiaryList2.InsertColumn(i, &lvc2);
	}

	//-----------------------------------------------------
	// DiaryList
	DWORD dwStyle = m_DiaryList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_DiaryList.SetExtendedStyle(dwStyle);

	LVCOLUMN    lvc;
	TCHAR       caption[][32] = { _T("�ԍ�"), _T("�^�p��"), _T("�V��"), _T("���i"), _T("����"), _T("���i"), _T("����"), _T("�����"), _T("���ʐM���ԁi���j"), _T("�ʐM��") };
	int			csize[] = { 50, 100, 50, 70, 100, 70, 100, 100, 100, 60};
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;

	for (int i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;
		if (i == 8 || i == 9)
		{
			lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvc.fmt = LVCFMT_RIGHT;

		}
		else
		{
			lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvc.fmt = LVCFMT_CENTER;
		}
		lvc.pszText = caption[i];
		lvc.cchTextMax = (int)wcslen(caption[i]);
		lvc.cx = csize[i];
		m_DiaryList.InsertColumn(i, &lvc);
	}

	CreateDialyList();
	m_DiaryList.AutoSizeColumns();
	if (mSpaceList.size() > 1){
		// �q���R���{�{�b�N�X�ւ̓o�^
		m_SatelliteCmb.ResetContent();
		vector<CString>::iterator itr;
		for (itr = mSpaceList.begin(); itr != mSpaceList.end(); itr++){
			m_SatelliteCmb.AddString((*itr));
		}
		m_SatelliteCmb.SetCurSel(0);

		// �J�����_�R���g���[���̃J�����g�ݒ�
		SYSTEMTIME date;
		memset(&date, 0, sizeof(SYSTEMTIME));
		CString str;

		if (mStartOpeTime.IsEmpty() == false)
		{
			AfxExtractSubString(str, mStartOpeTime, 0, '-');
			date.wYear = _wtoi(str);
			AfxExtractSubString(str, mStartOpeTime, 1, '-');
			date.wMonth = _wtoi(str);
			AfxExtractSubString(str, mStartOpeTime, 2, '-');
			date.wDay = _wtoi(str);
			mDate.SetTime(date);
		}

		// �����t�H�[�}�b�g�ύX
		mDate.SetFormat(_T("yyyy-MM-dd"));
		CenterWindowEx();
	}

	m_DiaryList.mListNo = 0;
	m_DiaryList2.mListNo = 1;

	// ���C������T�u
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_DiaryList.GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	m_DiaryList.mHeaderSize.clear();
	for (int col = 0; col < maxcol; col++){
		int wc = m_DiaryList.GetColumnWidth(col);
		m_DiaryList.mHeaderSize.push_back(wc);
	}
	m_DiaryList2.ResetColumns(m_DiaryList.mHeaderSize);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CBusinessDiary

-# ���O�_�C�A���O�\������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CBusinessDiary::UpdateDlg()
{
	return TRUE;
}


/*============================================================================*/
/*! CBusinessDiary

-# �f�[�^���X�g�쐬

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::CreateDialyList()
{
	mSpaceList.clear();
	mSpaceList.push_back(mAllText);
	mStartOpeTime.Empty();

	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;
	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, nStation);

	FILE *fp=NULL;

//	mDialyDataList.clear();
	vector<stDiary>	dialyDataList;
	char szDmy1[16] = { 0 };
	char szDmy2[16] = { 0 };
	char szDate[16] = { 0 };
	char szTime[16] = { 0 };
	char szCnt[16] = { 0 };
	char szDmy3[16] = { 0 };
	char szDmy4[16] = { 0 };

	TRY
	{
		// �����t�@�C���̃I�[�v��
		if (_wfopen_s(&fp, filename, _T("r")) != 0){
			// �����t�@�C�����Ȃ������ꍇ�A�w�b�_�݂̂̃t�@�C�����쐬����
			if (_wfopen_s(&fp, filename, _T("w")) != 0){
				char	szBuf[256];
				strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), "1990-01-01 00:00:00");
				sprintf_s(szBuf, "Dmy1 Dmy2 %s %d Dmy3 Dmy4\n", m_szMakeDate, 0);
				fputs(szBuf, fp);
			}
		}
		else
		{
			char szline[2048];
			// �w�b�_�[��������ƃ��R�[�h���擾
			fgets(szline, static_cast<unsigned>(_countof(szline)), fp);

			sscanf_s(szline, "%s %s %s %s %s %s %s\n"
				, szDmy1, static_cast<unsigned>(_countof(szDmy1))
				, szDmy2, static_cast<unsigned>(_countof(szDmy2))
				, szDate, static_cast<unsigned>(_countof(szDate))
				, szTime, static_cast<unsigned>(_countof(szTime))
				, szCnt, static_cast<unsigned>(_countof(szCnt))
				, szDmy3, static_cast<unsigned>(_countof(szDmy3))
				, szDmy4, static_cast<unsigned>(_countof(szDmy4)));

			memset(m_szMakeDate, '\0', sizeof(m_szMakeDate));
			strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), szDate);
			strcat_s(m_szMakeDate, sizeof(m_szMakeDate), " ");
			strcat_s(m_szMakeDate, sizeof(m_szMakeDate), szTime);

			mStrRecCount = CString(szCnt);

			CString str, cbuf;
			BOOL bRead = TRUE;
			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
				cbuf = CServerFile::ConvertUnicode(szline);
				cbuf = cbuf.Mid(0, cbuf.GetLength() - 1);	// ���s�R�[�h�폜
				int i = 0;
				stDiary temp;
#if 1

				// �ԍ�
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
#if 0
					if (str == _T("-"))
					{
						continue;
					}
#endif
					temp.no = str;
				}

				// �^�p��
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
//					if (mStartOpeTime.IsEmpty() == true)
//						mStartOpeTime = str;
//					mEndOpeTime = str;
					temp.date = str;
				}
#else

				while (AfxExtractSubString(str, cbuf, i++, ',')){
					switch (i - 1){
					case	0:	// �ԍ�
						temp.no = str;
						break;
					case	1:	// �^�p��
						if (mStartOpeTime.IsEmpty() == true)
							mStartOpeTime = str;
						mEndOpeTime = str;
						temp.date = str;
						break;
					}

					if (i == 2)
						break;
				}
#endif

#if 1
				for (int u = 0; u < 3; ++u)
				{
					// �V��
					temp.data[u].weather = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].weather = str;
					}

					// ���i
					temp.data[u].capa1 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].capa1 = str;
					}

					// ����
					temp.data[u].name1 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].name1 = str;
					}

					// ���i
					temp.data[u].capa2 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].capa2 = str;
					}

					// ����
					temp.data[u].name2 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].name2 = str;
					}

					// ���i
					temp.data[u].capa3 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].capa3 = str;
					}

					// ����
					temp.data[u].name3 = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].name3 = str;
					}

					// �����
					temp.data[u].station = _T("-");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						AddSpaceName(str);
						temp.data[u].station = str;
					}

					// ���ʐM����
					temp.data[u].time = _T("0");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].time = str;
					}

					// �ʐM��
					temp.data[u].count = _T("0");
					if (AfxExtractSubString(str, cbuf, i++, ','))
					{
						temp.data[u].count = str;
					}
				}
#else
				int k = 0;
				while (AfxExtractSubString(str, cbuf, i++, ','))
				{
					switch ((i - 3) % 10){
					case	0:	// �V��
						temp.data[k].weather = str;
						break;
					case	1:	// ���i
						temp.data[k].capa1 = str;
						break;
					case	2:	// ����
						temp.data[k].name1 = str;
						break;
					case	3:	// ���i
						temp.data[k].capa2 = str;
						break;
					case	4:	// ����
						temp.data[k].name2 = str;
						break;
					case	5:	// ���i
						temp.data[k].capa3 = str;
						break;
					case	6:	// ����
						temp.data[k].name3 = str;
						break;
					case	7:	// �����
						AddSpaceName(str);
						temp.data[k].station = str;
						break;
					case	8:	// ���ʐM����
						temp.data[k].time = str;
						break;
					case	9:	// �ʐM��
						temp.data[k].count = str;
						k++;
						break;
					default:
						break;
					}

					if (i >= 32)
					{
						break;
					}
				}
#endif

#if 1
				// ���l1
				temp.info1 = _T("");
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
					temp.info1 = str;
				}

				// ���l2
				temp.info2 = _T("");
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
					temp.info2 = str;
				}

				// ���l3
				temp.info3 = _T("");
				if (AfxExtractSubString(str, cbuf, i++, ','))
				{
					temp.info3 = str;
				}

#else
				temp.info1 = _T("");
				temp.info2 = _T("");
				temp.info3 = _T("");

				while (AfxExtractSubString(str, cbuf, i++, ','))
				{
					switch (i - 1){
					case	32:	// ���l1
						temp.info1 = str;
						break;
					case	33:	// ���l2
						temp.info2 = str;
						break;
					case	34:	// ���l3
						temp.info3 = str;
						break;
					}
				}
#endif
//				mDialyDataList.push_back(temp);
				dialyDataList.push_back(temp);
			}
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if(fp != NULL)
		fclose(fp);

	// ���ݎ����̎��ԑт�ݒ肷��
	GetTimeConf(NULL);
	// RF���t�@�C���ǂݍ���
	ReadRFInfoFile();
	// �����Ɩ������쐬�^�X�V����
	MakeReport(dialyDataList);
	// �����Ɩ��������t�@�C���ɏ�������
	WriteDiaryData();

	if (mDialyDataList.size() > 0)
	{
		mStartOpeTime = mDialyDataList[0].date;
		mEndOpeTime = mDialyDataList[mDialyDataList.size() - 1].date;
	}

	// ���X�g�\��
	DisplayList(mAllText);
}

/*============================================================================*/
/*! CBusinessDiary

-# ���X�g�\��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::DisplayList(CString name)
{
	CWaitCursor wait;

	m_DiaryList.SetRedraw(false);
	m_DiaryList.DeleteAllItems();

	mDialyDataListTmp.clear();

	// ���X�g�\��
	vector<stDiary>::iterator itr;
	int item = 0;
	DWORD nExistTime[3];
	int nTime;
	int nCount;

	for (itr = mDialyDataList.begin(); itr != mDialyDataList.end(); itr++)
	{
		stDiary st = (*itr);
		if (name != mAllText)
		{
			if (name != st.data[0].station && name != st.data[1].station && name != st.data[2].station)
				continue;
		}
		int subitem = 0;

		// �ԍ���"-"�̏ꍇ�̓��X�g���Ȃ�
		if (st.no == _T("-"))
		{
			continue;
		}

		for (int nTimeIdx = 0; nTimeIdx < 3; nTimeIdx++)
		{
			nExistTime[nTimeIdx] = 0;
		}

		nTime = 0;
		nCount = 0;
		// �q�����������Ă��郌�R�[�h��\������
		for (int nTimeIdx = 0; nTimeIdx < 3; nTimeIdx++)
		{
			// ���ԂƉ񐔂͍��v��\������
			nTime += _ttoi(st.data[nTimeIdx].time);
			nCount += _ttoi(st.data[nTimeIdx].count);

			if (st.data[nTimeIdx].station != _T("") && st.data[nTimeIdx].station != _T("-"))
			{
				// �q��������
				nExistTime[nTimeIdx] |= 1 << 3;
			}
			if (st.data[nTimeIdx].time != _T("0") && st.data[nTimeIdx].count != _T("0"))
			{
				// �ʐM���ԂƒʐM�񐔂�0�łȂ�
				nExistTime[nTimeIdx] |= 1 << 2;
			}
			if (st.data[nTimeIdx].weather.Find(_T("-")) < 0)
			{
				// �V���񂪂���
				nExistTime[nTimeIdx] |= 1 << 1;
			}
			if ((st.data[nTimeIdx].capa1 != _T("") && st.data[nTimeIdx].capa1 != _T("-") && st.data[nTimeIdx].capa1 != _T("------")) ||
				(st.data[nTimeIdx].name1 != _T("") && st.data[nTimeIdx].name1 != _T("-") && st.data[nTimeIdx].name1 != _T("--------------")) ||
				(st.data[nTimeIdx].capa2 != _T("") && st.data[nTimeIdx].capa2 != _T("-") && st.data[nTimeIdx].capa2 != _T("------")) ||
				(st.data[nTimeIdx].name2 != _T("") && st.data[nTimeIdx].name2 != _T("-") && st.data[nTimeIdx].name2 != _T("--------------")))
			{
				// ���i�A������񂪂���
				nExistTime[nTimeIdx] |= 1 << 0;
			}
		}

		DWORD nCurTime = nExistTime[0];
		int nCurIdx = 0;		// �����l�͎��ԑ�1
		for (int nTimeIdx = 0; nTimeIdx < 3; nTimeIdx++)
		{
			// �D��x���������ԑт�T��
			if (nCurTime < nExistTime[nTimeIdx])
			{
				nCurTime = nExistTime[nTimeIdx];
				nCurIdx = nTimeIdx;
			}
		}

		m_DiaryList.AddItem(item, subitem++, st.no);
		m_DiaryList.AddItem(item, subitem++, st.date);
		stMember dst;
		ExchgNoneData2Long(st.data[nCurIdx], dst);
		m_DiaryList.AddItem(item, subitem++, dst.weather);
		m_DiaryList.AddItem(item, subitem++, dst.capa1);
		m_DiaryList.AddItem(item, subitem++, dst.name1);
		m_DiaryList.AddItem(item, subitem++, dst.capa2);
		m_DiaryList.AddItem(item, subitem++, dst.name2);
		m_DiaryList.AddItem(item, subitem++, dst.station);

		CString strTime, strCount;
		strTime.Format(_T("%d"), nTime);
		strCount.Format(_T("%d"), nCount);
		m_DiaryList.AddItem(item, subitem++, strTime);
		m_DiaryList.AddItem(item, subitem++, strCount);
		mDialyDataListTmp.push_back(st);
		item++;
	}

	if (m_DiaryList.GetItemCount() > 0){
		m_DiaryList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	m_DiaryList.SetRedraw(true);
}


/*============================================================================*/
/*! CBusinessDiary

-# �e���̃f�[�^�����܂���'-'������'-'�A��������ɕϊ�����

@param  src	�ϊ��O�f�[�^('-')
@param  dst	�ϊ���f�[�^('------')�ȂǁBNULL���͕ϊ��O�f�[�^�ɓ����
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::ExchgNoneData2Long(stMember& src, stMember& dst)
{
	// weather
	if (src.weather == _T("") || src.weather == _T("-"))
	{
		dst.weather = _T("---");
	}
	else if (&src != &dst)
	{
		dst.weather = src.weather;
	}

	// capa1
	if (src.capa1 == _T("") || src.capa1 == _T("-"))
	{
		dst.capa1 = ("------");
	}
	else if (&src != &dst)
	{
		dst.capa1 = src.capa1;
	}

	// name1
	if (src.name1 == _T("") || src.name1 == _T("-"))
	{
		dst.name1 = _T("--------------");
	}
	else if (&src != &dst)
	{
		dst.name1 = src.name1;
	}

	// capa2
	if (src.capa2 == _T("") || src.capa2 == _T("-"))
	{
		dst.capa2 = ("------");
	}
	else if (&src != &dst)
	{
		dst.capa2 = src.capa2;
	}

	// name2
	if (src.name2 == _T("") || src.name2 == _T("-"))
	{
		dst.name2 = _T("--------------");
	}
	else if (&src != &dst)
	{
		dst.name2 = src.name2;
	}

	// capa3
	if (src.capa3 == _T("") || src.capa3 == _T("-"))
	{
		dst.capa3 = ("------");
	}
	else if (&src != &dst)
	{
		dst.capa3 = src.capa3;
	}

	// name3
	if (src.name3 == _T("") || src.name3 == _T("-"))
	{
		dst.name3 = _T("--------------");
	}
	else if (&src != &dst)
	{
		dst.name3 = src.name3;
	}

	// station
	if (src.station == _T("") || src.station == _T("-"))
	{
		dst.station = _T("------");
	}
	else if (&src != &dst)
	{
		dst.station = src.station;
	}

	// time
	if (src.time == _T("") || src.time == _T("-"))
	{
		dst.time = _T("0");
	}
	else if (&src != &dst)
	{
		dst.time = src.time;
	}

	// count
	if (src.count == _T("") || src.count == _T("-"))
	{
		dst.count = _T("0");
	}
	else if (&src != &dst)
	{
		dst.count = src.count;
	}
}

/*============================================================================*/
/*! CBusinessDiary

-# �e���̃f�[�^�����܂���'-'�A���������'-'�����ɕϊ�����

@param  src	�ϊ���f�[�^('------')�Ȃ�
@param  dst	�ϊ��O�f�[�^('-')
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::ExchgNoneData2Short(stMember& src, stMember& dst)
{
	// weather
	if (src.weather == _T("") || src.weather == _T("---"))
	{
		dst.weather = _T("-");
	}
	else if (&src != &dst)
	{
		dst.weather = src.weather;
	}

	// capa1
	if (src.capa1 == _T("") || src.capa1 == ("------"))
	{
		dst.capa1 = ("-");
	}
	else if (&src != &dst)
	{
		dst.capa1 = src.capa1;
	}

	// name1
	if (src.name1 == _T("") || src.name1 == _T("--------------"))
	{
		dst.name1 = _T("-");
	}
	else if (&src != &dst)
	{
		dst.name1 = src.name1;
	}

	// capa2
	if (src.capa2 == _T("") || src.capa2 == ("------"))
	{
		dst.capa2 = ("-");
	}
	else if (&src != &dst)
	{
		dst.capa2 = src.capa2;
	}

	// name2
	if (src.name2 == _T("") || src.name2 == _T("--------------"))
	{
		dst.name2 = _T("-");
	}
	else if (&src != &dst)
	{
		dst.name2 = src.name2;
	}

	// capa3
	if (src.capa3 == _T("") || src.capa3 == ("------"))
	{
		dst.capa3 = ("-");
	}
	else if (&src != &dst)
	{
		dst.capa3 = src.capa3;
	}

	// name3
	if (src.name3 == _T("") || src.name3 == _T("--------------"))
	{
		dst.name3 = _T("-");
	}
	else if (&src != &dst)
	{
		dst.name3 = src.name3;
	}

	// station
	if (src.station == _T("") || src.station == _T("------"))
	{
		dst.station = _T("-");
	}
	else if (&src != &dst)
	{
		dst.station = src.station;
	}

	// time
	if (src.time == _T("") || src.time == _T("-"))
	{
		dst.time = _T("0");
	}
	else if (&src != &dst)
	{
		dst.time = src.time;
	}

	// count
	if (src.count == _T("") || src.count == _T("-"))
	{
		dst.count = _T("0");
	}
	else if (&src != &dst)
	{
		dst.count = src.count;
	}
}

/*============================================================================*/
/*! CBusinessDiary

-# �q�����̓o�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::AddSpaceName(CString name)
{
	if (name == _T("-"))
		return;
	vector<CString>::iterator itr;
	for (itr = mSpaceList.begin(); itr != mSpaceList.end(); itr++){
		if ((*itr) == name)
			return;
	}
	mSpaceList.push_back(name);
}

/*============================================================================*/
/*! CBusinessDiary

-# �q�����̓o�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::JumpDate()
{
	SYSTEMTIME date;
	memset(&date, 0, sizeof(SYSTEMTIME));
	mDate.GetTime(&date);
	CTime ctime(date);
	CString str = ctime.Format("%Y-%m-%d");

	int count = m_DiaryList.GetItemCount();
	for (int i = 0; i < count; i++){
		CString text = m_DiaryList.GetItemText(i, 1);
		if (text == str){
			CRect rc;
			m_DiaryList.GetItemRect(0, &rc, LVIR_BOUNDS);// �s����
			int index = m_DiaryList.GetTopIndex();// ���ݍs
			if (i == index)
				return;
			int offset = rc.Height() * (i - index);// �s�փI�t�Z�b�g�v�Z
			CSize cs;
			cs.cx = 0;
			cs.cy = offset;
			if (offset) {
				m_DiaryList.Scroll(cs);
			}
			return;
		}
		else{
			DWORD t1 = _wtoi(text.Left(4)) << 16;
			t1 |= _wtoi(text.Mid(5, 2)) << 8;
			t1 |= _wtoi(text.Right(2));
			DWORD t2 = (date.wYear << 16) | (date.wMonth << 8) | date.wDay;
			if (t1 > t2){
				CRect rc;
				m_DiaryList.GetItemRect(0, &rc, LVIR_BOUNDS);// �s����
				int index = m_DiaryList.GetTopIndex();// ���ݍs
				int offset = rc.Height() * ((i-1) - index);// �s�փI�t�Z�b�g�v�Z
				CSize cs;
				cs.cx = 0;
				cs.cy = offset;
				if (offset) {
					m_DiaryList.Scroll(cs);
				}
				return;
			}
		}
	}
	m_DiaryList.EnsureVisible(count-1, FALSE);
}

/*============================================================================*/
/*! CBusinessDiary

-# �ҏW�{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::OnBnClickedDiaryEditBtn()
{
	// �q�����擾
	CString str;
	m_SatelliteCmb.GetLBText(m_SatelliteCmb.GetCurSel(), str);

	// �I������Ă�������擾
//	int idx = m_DiaryList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);

	int lidx = 0;
	int no = 0;
	int idx = -1;
	int count = 0;
	while ((idx = m_DiaryList.GetNextItem(idx, LVNI_ALL | LVNI_SELECTED)) != -1)
	{
		if (count == 0)
		{
			lidx = idx;
			CString strNo =	m_DiaryList.GetItemText(lidx, 0);
			no = atoi(CStringA(strNo));
		}
		else
		{
			m_DiaryList.SetItemState(idx, 0, LVIS_SELECTED);
		}
		count++;
	}

	// �ҏW�_�C�A���O�\��
	CBusinessDiaryEdit dlg(this);
	dlg.SetSatellite(str);
	dlg.SetListPos(no);

	if (dlg.DoModal() != IDCANCEL)
	{
		CWaitCursor wait;
//		DisplayList(str);
		CreateDialyList();
	}
}


/*============================================================================*/
/*! CBusinessDiary

-# ����{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::OnBnClickedDiaryPrntBtn()
{
	CBusinessDiaryPrint dlg(this);

	SYSTEMTIME date;
	memset(&date, 0, sizeof(SYSTEMTIME));
	date.wYear = _wtoi(mStartOpeTime.Left(4));
	date.wMonth = _wtoi(mStartOpeTime.Mid(5, 2));
	date.wDay = _wtoi(mStartOpeTime.Right(2));
	CTime st = CTime(date);
	date.wYear = _wtoi(mEndOpeTime.Left(4));
	date.wMonth = _wtoi(mEndOpeTime.Mid(5, 2));
	date.wDay = _wtoi(mEndOpeTime.Right(2));
	CTime et = CTime(date);

	dlg.SetParameter(mSpaceList, st, et);
	dlg.DoModal();
}


/*============================================================================*/
/*! CBusinessDiary

-# CSV�o�̓{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::OnBnClickedDiaryCsvBtn()
{
	UpdateData(TRUE);

	CTime ctime;
	mDate.GetTime(ctime);
	DWORD date = ctime.GetYear() << 16 | ctime.GetMonth() << 8 | ctime.GetDay();

	int i = m_SatelliteCmb.GetCurSel();
	CString mStation;
	m_SatelliteCmb.GetLBText(i, mStation);

#if 1
	FILE *fpform;
	FILE *fpcsv;

	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString formDir = theApp.GetShareFilePath(eFileType_Diary_Form_Folder, nStation);
	CString csvDir = theApp.GetShareFilePath(eFileType_Diary_CSVOUT_Folder, nStation);

	int satCnt = 2;
	if(mStation == mAllText)
	{
		satCnt = m_SatelliteCmb.GetCount();
	}

	CString satname;
	char szRead[2048];
	char szWrite[2048];
	CString cbuf;
	CStringA wbuf;

	vector<CStringA> csvFormList;
	bool bOutput = false;

	for (int i = 1; i < satCnt; ++i)
	{
		csvFormList.clear();

		// �q�����擾
		if (mStation == mAllText)
		{
			m_SatelliteCmb.GetLBText(i, satname);
		}
		else
		{
			m_SatelliteCmb.GetLBText(m_SatelliteCmb.GetCurSel(), satname);
		}

		CString strSatDir = _T("");

		// �q���ԍ��擾
		int satidx = GetIndexEqSatDBData(satname);
		if (satidx == -1)
		{
			strSatDir = NOT_SAT_DIR;
		}
		else
		{
			strSatDir = satname.MakeLower();
		}

		// CSV�o�̓t�H�[�}�b�g�t�@�C���̃t�H���_�����邩�m�F
		CString strCSVFormFolder;
		strCSVFormFolder.Format(_T("%s%s\\"), (LPCTSTR)formDir, (LPCTSTR)strSatDir);
		if (PathIsDirectory(strCSVFormFolder) == FALSE)
		{
			MakeSureDirectoryPathExists((PCSTR)CStringA(strCSVFormFolder));
		}

		// CSV�o�̓t�H�[�}�b�g�t�@�C���擾
		CString strCSVForm;
		strCSVForm.Format(_T("%s%s\\%s"), (LPCTSTR)formDir, (LPCTSTR)strSatDir, (LPCTSTR)PRINT_FORM_CSV_FILE);

		if (_wfopen_s(&fpform, strCSVForm, _T("r")) != 0)
		{
			CString mes;
			mes.Format(_T("�ȉ��̃t�@�C����������܂���ł����B\n%s"), (LPCTSTR)strCSVForm);
			AfxMessageBox(mes, MB_OK | MB_ICONWARNING);
			continue;
		}

		while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fpform) != NULL)
		{
			cbuf = CServerFile::ConvertUnicode(szRead);
			csvFormList.push_back(CStringA(cbuf));
		}
		fclose(fpform);


		// ���[��������

		stDiary st;
		int PrintYear = 0;
		int oldPrintYear = 0;
		int PrintMonth = 0;
		int oldPrintMonth = 0;
		int PrintDay;

		BOOL bOpen = FALSE;

		fpcsv = NULL;

		int dcnt = (int)mDialyDataListTmp.size();
		for (int d = 0; d < dcnt; ++d)
		{
			st = mDialyDataListTmp[d];
			if ( (satname.CompareNoCase(st.data[0].station) == 0) 
				|| (satname.CompareNoCase(st.data[1].station) == 0)
				|| (satname.CompareNoCase(st.data[2].station) == 0))
			{
				COleDateTime oleTime;
				oleTime.ParseDateTime(st.date);
				CTime cTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

				// �N����
				PrintYear = cTime.GetYear();
				PrintMonth = cTime.GetMonth();
				PrintDay = cTime.GetDay();

				// �j��
				CString strDayOfWeek = DefDayOfWeek[cTime.GetDayOfWeek() - 1];

				if (!((PrintYear == oldPrintYear) && (PrintMonth == oldPrintMonth)))
				{
					if (fpcsv != NULL)
					{
						fclose(fpcsv);
						bOpen = FALSE;
					}

					CString strCsvFile;
					CString folder;
					folder.Format(_T("%s\\%s\\"), (LPCTSTR)csvDir, (LPCTSTR)strSatDir);
					if (PathFileExists(folder) == FALSE)	// �o�͉q���t�H���_�������ꍇ�쐬����B
					{
//						CreateDirectory(folder, NULL);
						MakeSureDirectoryPathExists((PCSTR)CStringA(folder));
					}

					if (satidx == -1)
					{
						strCsvFile.Format(_T("%s\\%s\\REP_%04d_%02d_%s.csv"), (LPCTSTR)csvDir, (LPCTSTR)strSatDir, PrintYear, PrintMonth, (LPCTSTR)satname);
					}
					else
					{
						strCsvFile.Format(_T("%s\\%s\\REP_%04d_%02d.csv"), (LPCTSTR)csvDir, (LPCTSTR)strSatDir, PrintYear, PrintMonth);
					}

					// CSV�o�̓t�@�C���I�[�v��
					if ((_wfopen_s(&fpcsv, strCsvFile, _T("w")) == 0) && (fpcsv != NULL))
					{
						// �w�b�_�[����������
						sprintf_s(szWrite, "%s", (LPCSTR)csvFormList[0]);
						fputs(szWrite, fpcsv);

						sprintf_s(szWrite, "%s", (LPCSTR)csvFormList[1]);
						fputs(szWrite, fpcsv);

						sprintf_s(szWrite, csvFormList[2], (PrintYear - 1988));
						fputs(szWrite, fpcsv);

						sprintf_s(szWrite, csvFormList[3], PrintMonth);
						fputs(szWrite, fpcsv);

						sprintf_s(szWrite, "%s", (LPCSTR)csvFormList[4]);
						fputs(szWrite, fpcsv);

						bOpen = TRUE;
						bOutput = true;
					}
				}

				oldPrintYear = PrintYear;
				oldPrintMonth = PrintMonth;

				if (bOpen == TRUE)
				{
					for (int phaseT = 0; phaseT < 3; phaseT++)
					{
						// �e���̃f�[�^�����܂���'-'������'-'�A��������ɕϊ�����
						ExchgNoneData2Long(st.data[phaseT], st.data[phaseT]);
					}

					CString ref;
					AfxExtractSubString(ref, st.info3, 0, '\\');
					sprintf_s(szWrite, csvFormList[5], PrintDay, CStringA(strDayOfWeek), CStringA(st.data[0].weather), CStringA(st.data[0].capa1), CStringA(st.data[0].name1),
						CStringA(st.data[0].capa2), CStringA(st.data[0].name2), CStringA(st.data[0].station), _wtoi(st.data[0].time), _wtoi(st.data[0].count),
						CStringA(st.info1), CStringA(st.info2), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 1, '\\');
					sprintf_s(szWrite, csvFormList[6], CStringA(st.data[0].capa3), CStringA(st.data[0].name3), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 2, '\\');
					sprintf_s(szWrite, csvFormList[7], CStringA(st.data[1].weather), CStringA(st.data[1].capa1), CStringA(st.data[1].name1),
						CStringA(st.data[1].capa2), CStringA(st.data[1].name2), CStringA(st.data[1].station), _wtoi(st.data[1].time), _wtoi(st.data[1].count), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 3, '\\');
					sprintf_s(szWrite, csvFormList[8], CStringA(st.data[1].capa3), CStringA(st.data[1].name3), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 4, '\\');
					sprintf_s(szWrite, csvFormList[9], CStringA(st.data[2].weather), CStringA(st.data[2].capa1), CStringA(st.data[2].name1),
						CStringA(st.data[2].capa2), CStringA(st.data[2].name2), CStringA(st.data[2].station), _wtoi(st.data[2].time), _wtoi(st.data[2].count), CStringA(ref));
					fputs(szWrite, fpcsv);

					AfxExtractSubString(ref, st.info3, 5, '\\');
					sprintf_s(szWrite, csvFormList[10], CStringA(st.data[2].capa3), CStringA(st.data[2].name3), CStringA(ref));
					fputs(szWrite, fpcsv);
				}
			}
		}
		if(fpcsv != NULL)
		{
			fclose(fpcsv);
		}
	}

	if (bOutput)
	{
		AfxMessageBox(_T("CSV�t�@�C�����o�͂��܂����B"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox(_T("�o�͑Ώۂ�������܂���ł����B"), MB_OK | MB_ICONEXCLAMATION);
	}

#else
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString folder = theApp.GetShareFilePath(eFileType_Diary_CSVOUT_Folder, nStation);
	const TCHAR BASED_CODE szFilter[] = _T("CSV File(*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("csv"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	dlg.m_ofn.lpstrInitialDir = folder;
	if (dlg.DoModal() != IDOK)
		return;

	CString filenameout = dlg.GetPathName();

	FILE *fpout = NULL;

	TRY
	{
		// CSV�t�@�C���̃I�[�v��
		if (_wfopen_s(&fpout, filenameout, _T("w, ccs=UTF-8")) != 0){
			return;
		}

		CHeaderCtrl* pHeader = (CHeaderCtrl*)m_DiaryList.GetDlgItem(0);
		int maxcol = pHeader->GetItemCount();
		int count = m_DiaryList.GetItemCount();
		CString cbuf;
		CString text;
		for (int i = 0; i < count; i++){
			cbuf.Empty();
			for (int j = 0; j < maxcol; j++){
				text = m_DiaryList.GetItemText(i, j);
				if (cbuf.IsEmpty() == false){
					cbuf += _T(",");
				}
				cbuf += text;
			}
			cbuf += _T("\n");
			// �o�͂���
			fputws(cbuf, fpout);
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

		if (fpout != NULL)
			fclose(fpout);


#endif

}


/*============================================================================*/
/*! CBusinessDiary

-# ���M���{�^����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::OnBnClickedDiarySinfoBtn()
{
	CBusinessDiaryTInfo dlg;
#if 0
	SYSTEMTIME date;
	memset(&date, 0, sizeof(SYSTEMTIME));
	date.wYear = _wtoi(mStartOpeTime.Left(4));
	date.wMonth = _wtoi(mStartOpeTime.Mid(5, 2));
	date.wDay = _wtoi(mStartOpeTime.Right(2));
	CTime st = CTime(date);
	date.wYear = _wtoi(mEndOpeTime.Left(4));
	date.wMonth = _wtoi(mEndOpeTime.Mid(5, 2));
	date.wDay = _wtoi(mEndOpeTime.Right(2));
	CTime et = CTime(date);

	dlg.SetParameter(mSpaceList, st, et);
#endif
	dlg.DoModal();
}




void CBusinessDiary::OnDtnDatetimechangeDiaryStltDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	//JumpDate();
	PostMessage(WM_USER + 100, 0, 0);

	*pResult = 0;
}


void CBusinessDiary::OnCbnSelchangeDiaryStltCmb()
{
	UpdateData(TRUE);

	int i = m_SatelliteCmb.GetCurSel();
	CString str;
	m_SatelliteCmb.GetLBText(i, str);
	DisplayList(str);
}


/*============================================================================*/
/*! CBusinessDiary

-# �t�@�C��-���郁�j���[����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::OnMenuCloseClose()
{
	CWaitCursor wait;
	// �����Ɩ��������t�@�C���ɏ�������
	WriteDiaryData();
	// �����Ɩ������o�^�v��
	Shell_RegistRadiolog();

	OnOK();
}


LRESULT CBusinessDiary::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == (WM_USER + 100)){
		JumpDate();
		return TRUE;
	}
	if (message == (WM_USER + 200)){
		if (lParam == 0){
			// ���C������T�u
			m_DiaryList2.ResetColumns(m_DiaryList.mHeaderSize);
		}
		if (lParam == 1){
			// �T�u���烁�C��
			m_DiaryList.ResetColumns(m_DiaryList2.mHeaderSize);
		}
		return TRUE;
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}

void CBusinessDiary::SelectListRow(int no)
{
	int sidx = -1;
	int idx;
	while ((sidx = m_DiaryList.GetNextItem(sidx, LVNI_ALL | LVNI_SELECTED)) != -1)
	{
		m_DiaryList.SetItemState(sidx, 0, LVIS_SELECTED);
	}

	int count = m_DiaryList.GetItemCount();
	for (idx = 0; idx < count; idx++)
	{
		CString strNo = m_DiaryList.GetItemText(idx, 0);
		if (strNo == _T("-"))
		{
			continue;
		}
		int searchNo = atoi(CStringA(strNo));
		if (searchNo == no)
		{
			break;
		}
	}

	m_DiaryList.SetItemState(idx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_DiaryList.EnsureVisible(idx, FALSE);
}

/*============================================================================*/
/*! CBusinessDiary

-# �����Ɩ������擾�V�F��������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CBusinessDiary::Shell_GetRadiolog()
{
	BOOL ret = TRUE;
	CString arg = _T("");

#if 0
	// �t�@�C�����폜����B
	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, theApp.GetSelectStation());
	if (PathFileExistsW(filename) == TRUE)
	{
		DeleteFileW(filename);
	}
#endif

	// �V�F�����s
	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = GET_RADIOLOG_SH;

	arg.Format(_T("%s"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower());

	stParam.strRemShellParam.Format(arg);

	CDlgWaitCmd wait;
	wait.Open(_T("�����Ɩ�����"), _T("�����Ɩ������擾���B\n���΂炭���҂����������B"), false, true);
	int err = CSshShell::ExecShell(stParam);
	wait.Close();

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("�����Ɩ������擾"), _T("CBusinessDiary"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err == 0)
	{
		ret = TRUE;
	}
	else
	{
		AfxMessageBox(_T("�����Ɩ������擾�Ɏ��s���܂����B"), MB_OK | MB_ICONWARNING);
		ret = FALSE;
	}

	return ret;
}

/*============================================================================*/
/*! CBusinessDiary

-# �����Ɩ������o�^�v���V�F��������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CBusinessDiary::Shell_RegistRadiolog()
{
	CString arg = _T("");

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = theApp.GetSshBatchPath();
	stParam.strUserName = theApp.GetSshUserName();
	stParam.strServerName = theApp.GetSshServerName();
	stParam.strKeyFilePath = theApp.GetSshKeyPath();
	stParam.strRemShellName = REGIST_RADIOLOG_SH;

	arg.Format(_T("%s"), (LPCTSTR)CString(mStationSimpleString[theApp.GetSelectStation()]).MakeLower());

	stParam.strRemShellParam.Format(arg);

	int err = CSshShell::ExecShell(stParam);

	{
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: ExecShell(%d)"), __FILEW__, __LINE__, err);
		CLogTraceEx::Write(_T(""), _T("�����Ɩ������o�^�v��"), _T("CBusinessDiary"), _T("debug"), strDebug, nLogEx::debug);
	}

	if (err != 0)
	{
		AfxMessageBox(_T("�����Ɩ������o�^�Ɏ��s���܂����B"), MB_OK | MB_ICONWARNING);
	}

	return TRUE;
}

/*============================================================================*/
/*! CBusinessDiary

-# �p�X���m�F

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CBusinessDiary::IsPass()
{
	char	sz_status[256] = {0};

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	// �p�X���͖����ǋƖ������͊J���Ȃ�
	UINT sta = theApp.GetSelectStation();
	memset(sz_status, (char)NULL, sizeof(sz_status));

	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, CString(CTRL_PASS_INFO_NO));
	if (obs_adr != NULL)
	{
		strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
	}

	if ((strcmp(sz_status, CT_PASS_INFO_INIT) == 0) 			// �����ݒ�
		|| (strcmp(sz_status, CT_PASS_INFO_1WAY) == 0) 			// �P�v�`�x�ߑ���
		|| (strcmp(sz_status, CT_PASS_INFO_2WAY) == 0)			// �Q�v�`�x�ߑ���
		|| (strcmp(sz_status, CT_PASS_INFO_PASS_START1) == 0)	// �p�X�J�n(1WAY)
		|| (strcmp(sz_status, CT_PASS_INFO_PASS_START2) == 0)	// �p�X�J�n(2WAY)
		|| (strcmp(sz_status, CT_PASS_INFO_POL) == 0)			// �V���ʉߊĎ��̈��
		|| (strcmp(sz_status, CT_PASS_INFO_POSN) == 0)			// ANT POSN MODE �ڍs�p���o
		|| (strcmp(sz_status, CT_PASS_INFO_EXTEND) == 0)		// �p�X����
		)
	{
		CString title = _T("");
		GetWindowText(title);

		MessageBox(_T("�p�X���͖����Ɩ�������\���ł��܂���B"), title, MB_OK | MB_ICONWARNING);
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
/*! CBusinessDiary

-# RF���t�@�C���̍ŏI�s��"ON"�̗L�����m�F

@param  �Ȃ�
@retval ON �L�� TRUE or �G���[�� / ON ���� FALSE;
*/
/*============================================================================*/
BOOL CBusinessDiary::CheckRFInfoFile()
{
	CString filename = theApp.GetShareFilePath(eFileType_Diary_Rf_File, theApp.GetSelectStation());

	BOOL ret = FALSE;
	FILE *fp = NULL;

	CString title = _T("");
	GetWindowText(title);

	TRY
	{
		// RF���t�@�C���̃I�[�v��
		if ((_wfopen_s(&fp, filename, _T("r")) == 0) && (fp != NULL))
		{
			char szline[2048];
			CString cbuf = _T("");

			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
			{
				cbuf = CServerFile::ConvertUnicode(szline);
			}

			if (cbuf.Find(_T("ON")) != -1)
			{
				MessageBox(_T("�����Ɩ������ҏW�֎~���ԑт̂��߁A\n�����Ɩ������ꗗ��ʂ��I�����܂��B"), title, MB_OK | MB_ICONWARNING);
				ret = TRUE;
			}

			if (cbuf == _T(""))
			{
				MessageBox(_T("�����Ɩ��������ҏW�o���鎞�ԑт��m�F�o���Ȃ����߁A\n�����Ɩ������ꗗ��ʂ��I�����܂��B"), title, MB_OK | MB_ICONWARNING);
				ret = TRUE;
			}
		}
		else
		{
			MessageBox(_T("�����Ɩ��������ҏW�o���鎞�ԑт��m�F�o���Ȃ����߁A\n�����Ɩ������ꗗ��ʂ��I�����܂��B"), title, MB_OK | MB_ICONWARNING);
			ret = TRUE;
		}

	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);

	return ret;
}

/*============================================================================*/
/*! CBusinessDiary

-# RF�t�@�C���̃t�H�[�}�b�g�`�F�b�N

@param  �Ȃ�
@retval TRUE�F���� / FALSE�F�ُ�
*/
/*============================================================================*/
BOOL CBusinessDiary::CheckFormatRFFile()
{
	CString filename = theApp.GetShareFilePath(eFileType_Diary_Rf_File, theApp.GetSelectStation());

	BOOL ret = TRUE;
	FILE *fp = NULL;

	CString title = _T("");
	GetWindowText(title);

	TRY
	{
		// RF���t�@�C���̃I�[�v��
		if ((_wfopen_s(&fp, filename, _T("r")) == 0) && (fp != NULL))
		{
			char szline[2048];
			CString cbuf = _T("");
			CString ccanma = _T("");
			CString cbufchk = _T("");

			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL && ret == TRUE)
			{
				cbuf = CServerFile::ConvertUnicode(szline);
				// �J���}�̐��`�F�b�N
				ccanma = cbuf;
				if (ccanma.Remove(_T(',')) != 4)
				{
					ret = FALSE;
				}
				// �������`�F�b�N
				for (int i = 0; i < 5 && ret == TRUE; i++)
				{
					AfxExtractSubString(cbufchk, cbuf, i, ',');
					if (cbufchk.GetLength() > 64)
					{
						ret = FALSE;
					}
				}
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format(_T("RF�t�@�C�����J���Ȃ����߁A�����Ɩ������ꗗ��ʂ��I�����܂��B\n%s"), filename);
			MessageBox(strMsg, title, MB_OK | MB_ICONWARNING);
			return FALSE;
		}

	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);

	if (ret == FALSE)
	{
		CString strMsg;
		strMsg.Format(_T("RF�t�@�C���t�H�[�}�b�g���قȂ邽�߁A�����Ɩ������ꗗ��ʂ��I�����܂��B\n%s"), filename);
		MessageBox(strMsg, title, MB_OK | MB_ICONWARNING);
	}

	return ret;
}


void CBusinessDiary::WriteDiaryData()
{
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;
	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, nStation);

	FILE *fp = NULL;

	if (mbEdit)
	{
		// �����Ɩ������X�V���Ƀo�b�N�A�b�v�t�@�C���쐬
		SYSTEMTIME time;
		GetSystemTime(&time);
		CTime ctime(time);
		CString temp = ctime.Format("%Y%m%d%H%M%S");
		CString backfile;
		backfile.Format(_T("%s.%s"), (LPCTSTR)filename, (LPCTSTR)temp);
		CopyFile(filename, backfile, FALSE);
		mbEdit = FALSE;
	}

	CStringA cbuf;

	TRY
	{
		// �����t�@�C���̃I�[�v��
		if ((_wfopen_s(&fp, filename, _T("wb")) != 0) || (fp == NULL))
		{
			return;
		}

		TCHAR szline[2048];
		wsprintf(szline, _T("Dmy1 Dmy2 %s %s Dmy3 Dmy4\n"), (LPCTSTR)CString(m_szMakeDate), (LPCTSTR)mStrRecCount);
		cbuf = CServerFile::ConvertEuc(szline);
		// �w�b�_�[��������
		fputs(cbuf, fp);

		int count = (int)mDialyDataList.size();
		for (int i = 0; i < count; ++i)
		{
			wsprintf(szline, _T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n"),
				(LPCTSTR)mDialyDataList[i].no,
				(LPCTSTR)mDialyDataList[i].date,
				(LPCTSTR)mDialyDataList[i].data[0].weather,
				(LPCTSTR)mDialyDataList[i].data[0].capa1,
				(LPCTSTR)mDialyDataList[i].data[0].name1,
				(LPCTSTR)mDialyDataList[i].data[0].capa2,
				(LPCTSTR)mDialyDataList[i].data[0].name2,
				(LPCTSTR)mDialyDataList[i].data[0].capa3,
				(LPCTSTR)mDialyDataList[i].data[0].name3,
				(LPCTSTR)mDialyDataList[i].data[0].station,
				(LPCTSTR)mDialyDataList[i].data[0].time,
				(LPCTSTR)mDialyDataList[i].data[0].count,
				(LPCTSTR)mDialyDataList[i].data[1].weather,
				(LPCTSTR)mDialyDataList[i].data[1].capa1,
				(LPCTSTR)mDialyDataList[i].data[1].name1,
				(LPCTSTR)mDialyDataList[i].data[1].capa2,
				(LPCTSTR)mDialyDataList[i].data[1].name2,
				(LPCTSTR)mDialyDataList[i].data[1].capa3,
				(LPCTSTR)mDialyDataList[i].data[1].name3,
				(LPCTSTR)mDialyDataList[i].data[1].station,
				(LPCTSTR)mDialyDataList[i].data[1].time,
				(LPCTSTR)mDialyDataList[i].data[1].count,
				(LPCTSTR)mDialyDataList[i].data[2].weather,
				(LPCTSTR)mDialyDataList[i].data[2].capa1,
				(LPCTSTR)mDialyDataList[i].data[2].name1,
				(LPCTSTR)mDialyDataList[i].data[2].capa2,
				(LPCTSTR)mDialyDataList[i].data[2].name2,
				(LPCTSTR)mDialyDataList[i].data[2].capa3,
				(LPCTSTR)mDialyDataList[i].data[2].name3,
				(LPCTSTR)mDialyDataList[i].data[2].station,
				(LPCTSTR)mDialyDataList[i].data[2].time,
				(LPCTSTR)mDialyDataList[i].data[2].count,
				(LPCTSTR)mDialyDataList[i].info1,
				(LPCTSTR)mDialyDataList[i].info2,
				(LPCTSTR)mDialyDataList[i].info3);
			cbuf = CServerFile::ConvertEuc(szline);
			fputs(cbuf, fp);
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);
}


/*============================================================================*/
/*! CBusinessDiary

-# �q���ŗLDB���擾�����f�[�^���Y������q���̃C���f�b�N�X���擾����

@param	str :�q����
@retval	�C���f�b�N�X
*/
/*============================================================================*/
int CBusinessDiary::GetIndexEqSatDBData(CString str)
{
	int idx = -1;

	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();
	vector<CString> &saNameList = dba.GetSatNameList();

	int cnt = (int)saNameList.size();

	if (cnt == 0)
	{
		// �T�C�Y0�̓f�[�^�x�[�X�I�[�v���Ɏ��s���Ă���B
		return -1;
	}

	for (int i = 0; i < cnt; ++i)
	{
		CString satname = saNameList[i];
		if (satname.CompareNoCase(str) == 0)
		{
			idx = i;
			break;
		}
	}

	return idx;
}


/*============================================================================*/
/*! CBusinessDiary

-# �����Ɩ������ԍ����烊�X�g�C���f�b�N�X�ʒu���擾����

@param  �����Ɩ������ԍ�
@retval �����Ɩ������C���f�b�N�X
*/
/*============================================================================*/
int CBusinessDiary::GetDiaryListIndex(int no)
{
	int idx;
	int searchNo;

	// �ԍ����疳���Ɩ������̃C���f�b�N�X�𒲂ׂ�
	for (idx = 0; idx < mDialyDataList.size(); idx++)
	{
		if (mDialyDataList[idx].no == _T("-"))
		{
			// -�͉������Ȃ�
			continue;
		}

		searchNo = atoi(CStringA(mDialyDataList[idx].no));

		if (searchNo == no)
		{
			break;
		}
	}

	return idx;
}

/*============================================================================*/
/*! CBusinessDiary

-# ���ԑяI�������擾����	  
   
@param  ����
@retval ���ԑєԍ�
*/
/*============================================================================*/
int CBusinessDiary::GetTimeConf(char *pszDate)
{
	int         i;
	time_t      lTime;
	struct tm   stTm;
	time_t      iTimeConf;
	char        szTimeConf[64];
	char        szBeforeTimeConf[64];

	char 		szNowTmpDate[64];

	memset(&stTm, 0, sizeof(stTm));
	memset(szTimeConf, '\0', sizeof(szTimeConf));
	memset(szNowTmpDate, '\0', sizeof(szNowTmpDate));

	if (pszDate == NULL) {
		time(&lTime);
//#ifdef _DEBUG		// �R�������ԃG���A�e�X�g
//		lTime = lTime + 16 * 3600;
//#endif
		if (gmtime_s(&stTm, &lTime) != 0)
		{
			return 0;
		}
		sprintf_s(szTimeConf, sizeof(szTimeConf), "%d-%02d-%02d %02d:%02d:%02d", stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday, stTm.tm_hour, stTm.tm_min, stTm.tm_sec);
		lTime = (time_t)CSatelliteData::ub_asc_to_dtime(szTimeConf);
	}
	else {
		lTime = (time_t)CSatelliteData::ub_asc_to_dtime(pszDate);
		if (localtime_s(&stTm, &lTime) != 0)
		{
			return 0;
		}
	}

	// ���ԑ�
	memset(szTimeConf, '\0', sizeof(szTimeConf));
	for (i = 0; i < 3; i++) {
		sprintf_s(szTimeConf, sizeof(szTimeConf), "%d-%02d-%02d %s", stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday, mEndTime[i]);
		iTimeConf = (time_t)CSatelliteData::ub_asc_to_dtime(szTimeConf);
		if (lTime < iTimeConf) {
			// ���ݎ������I��莞�������O�̏ꍇ
			if (i == 0) {
				// ��ꎞ�ԂȂ�ΑO���̎����I���ɂ���
				if (pszDate == NULL) {
					sprintf_s(szTimeConf, sizeof(szTimeConf), "%d-%02d-%02d %s", stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday, mEndTime[2]);

					/* #2010-�v�Z�@����2010_ctr CTR:HRTS-C-10-001-001�Ή� �� */
					/* �ʎZ�b������߂��� */
					iTimeConf = (time_t)CSatelliteData::ub_asc_to_dtime(szTimeConf);
					iTimeConf -= 60 * 60 * 24;
					CSatelliteData::ub_dtime_to_asc((double)iTimeConf, szNowTmpDate);	/* ut ��������YYYY-MM-DD HH:MM:SS.ssss������ɕϊ� */
					memset(m_szNowDate, '\0', sizeof(m_szNowDate));
					strncpy_s(m_szNowDate, sizeof(m_szNowDate), szNowTmpDate, 19);	/* �b�̏����_���폜 */
					/* #2010-�v�Z�@����2010_ctr CTR:HRTS-C-10-001-001�Ή� �� */
				}
			}
			else {
				if (pszDate == NULL) {
					strcpy_s(m_szNowDate, sizeof(m_szNowDate), szBeforeTimeConf);
				}
			}
			break;
		}
		strcpy_s(szBeforeTimeConf, sizeof(szBeforeTimeConf), szTimeConf);
	}
	return(i);
}

/*============================================================================*/
/*! CBusinessDiary

-# �����Ɩ������쐬�^�X�V����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiary::MakeReport(vector<stDiary>& dialyDataList)
{
	time_t	iNowTime, iDateLimit, iRfSrchDate, iOnTime;
	char	szSrchDate[24];
	char    szOnDate[24], szOffDate[24];
	char	szCurDay[24];
	char    szBeforeDay[24];
	char    szRecord[512];
	int     iNo;
	size_t	pos = 0;
	bool	bAborRftList = false;

	vector<stSatRfInfo> satRfInfoList;

	memset(szSrchDate, '\0', sizeof(szSrchDate));
	memset(szOnDate, '\0', sizeof(szOnDate));
	memset(szOffDate, '\0', sizeof(szOffDate));
	memset(szCurDay, '\0', sizeof(szCurDay));
	memset(szBeforeDay, '\0', sizeof(szBeforeDay));
	memset(szRecord, '\0', sizeof(szRecord));
	satRfInfoList.clear();

	iNowTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szNowDate);	/* ���݂̓��t��ʎZ�b�ɕϊ� */
	iDateLimit = iNowTime - ((mDelSpan + 1)*(60 * 60 * 24));			/* ���݂̓��t����ۑ��Ԍ�̒ʎZ�b���Z�o */

	iNo = 0;

	// �L���s���J�E���g���Ȃ����A�ԍ��U��Ȃ���
	for (int i = 0; i < dialyDataList.size(); i++)
	{
		// �c���f�[�^�ł���
		if (dialyDataList[i].no != _T("-"))
		{
			iNo++;
			dialyDataList[i].no.Format(_T("%d"), iNo);		// �ԍ��U��Ȃ���
		}
	}

	if (m_RfInfoList.size() > 0)
	{
		SetEditFlg(TRUE);								// RF����ǉ��̂��߁A�����X�V

		// �V����RF���𖳐��Ɩ������ɒǉ�
		strncpy_s(szBeforeDay, sizeof(szBeforeDay), m_RfInfoList[0].szDate, 10);
		for (pos = 0; pos < m_RfInfoList.size(); pos++)
		{
			// �Ⴄ���t�ɂȂ�����
			memset(szCurDay, '\0', sizeof(szCurDay));
			strncpy_s(szCurDay, sizeof(szCurDay), m_RfInfoList[pos].szDate, 10);
			if (strcmp(szCurDay, szBeforeDay) && strlen(szOnDate) && strlen(szOffDate) &&
				CSatelliteData::ub_asc_to_dtime(szOnDate) <= CSatelliteData::ub_asc_to_dtime(szOffDate))
			{
				// �Ⴄ���t�ɕς������A�����Ɩ������ɔ��f����
				for (int i = 0; i < satRfInfoList.size(); i++)
				{
					iNo++;

					stDiary temp;
					temp.no.Format(_T("%d"), iNo);
					temp.date.Format(_T("%s"), (LPCTSTR)CString(szBeforeDay));
					for (int u = 0; u < 3; ++u)
					{
						temp.data[u].weather = _T("-");
						temp.data[u].capa1 = _T("-");
						temp.data[u].name1 = _T("-");
						temp.data[u].capa2 = _T("-");
						temp.data[u].name2 = _T("-");
						temp.data[u].capa3 = _T("-");
						temp.data[u].name3 = _T("-");

						time_t	iaMin = satRfInfoList[i].iaTime[u] / 60;	// �b�𕪂ɂ���
						if (satRfInfoList[i].iaTime[u] % 60)
						{
							iaMin++;
						}

						if (iaMin > 0)
						{
							temp.data[u].station = CString(satRfInfoList[i].szaSatName);
						}
						else
						{
							temp.data[u].station = _T("-");
						}
						temp.data[u].time.Format(_T("%I64d"), iaMin);
						temp.data[u].count.Format(_T("%d"), satRfInfoList[i].iaCnt[u]);
					}
					temp.info1 = _T("");
					temp.info2 = _T("");
					temp.info3 = _T("");

					dialyDataList.push_back(temp);
				}

				// ���̓��t�ɂȂ邽�ߏ���������
				memset(szBeforeDay, '\0', sizeof(szBeforeDay));
				strcpy_s(szBeforeDay, sizeof(szBeforeDay), szCurDay);
				memset(szOnDate, '\0', sizeof(szOnDate));
				memset(szOffDate, '\0', sizeof(szOffDate));
				satRfInfoList.clear();
			}

			// ON���ǂݍ���
			if (m_RfInfoList[pos].bOn)
			{
				strcpy_s(szOnDate, sizeof(szOnDate), m_RfInfoList[pos].szDate);
				iOnTime = (time_t)CSatelliteData::ub_asc_to_dtime(szOnDate);
				if (iNowTime < iOnTime)
				{
					bAborRftList = true;
					break;
				}
			}
			// OFF���ǂݍ���
			else
			{
				strcpy_s(szOffDate, sizeof(szOffDate), m_RfInfoList[pos].szDate);
				SetRfInfo(m_RfInfoList[pos].szSatName, m_RfInfoList[pos].szColim, m_RfInfoList[pos].szFreq, szOnDate, szOffDate, satRfInfoList);
			}
		}

		// �Ō�̓��t��RF��������ɔ��f����
		if (strlen(szBeforeDay))
		{
			for (int i = 0; i < satRfInfoList.size(); i++)
			{
				iNo++;

				stDiary temp;
				temp.no.Format(_T("%d"), iNo);
				temp.date.Format(_T("%s"), (LPCTSTR)CString(szBeforeDay));
				for (int u = 0; u < 3; ++u)
				{
					temp.data[u].weather = _T("-");
					temp.data[u].capa1 = _T("-");
					temp.data[u].name1 = _T("-");
					temp.data[u].capa2 = _T("-");
					temp.data[u].name2 = _T("-");
					temp.data[u].capa3 = _T("-");
					temp.data[u].name3 = _T("-");

					time_t	iaMin = satRfInfoList[i].iaTime[u] / 60;	// �b�𕪂ɂ���
					if (satRfInfoList[i].iaTime[u] % 60)
					{
						iaMin++;
					}

					if (iaMin > 0)
					{
						temp.data[u].station = CString(satRfInfoList[i].szaSatName);
					}
					else
					{
						temp.data[u].station = _T("-");
					}
					temp.data[u].time.Format(_T("%I64d"), iaMin);
					temp.data[u].count.Format(_T("%d"), satRfInfoList[i].iaCnt[u]);
				}
				temp.info1 = _T("-");
				temp.info2 = _T("-");
				temp.info3 = _T("-");

				dialyDataList.push_back(temp);
			}
		}

		// �X�V���Ԃ̍X�V
		size_t	size = m_RfInfoList.size();
		size_t	lastpos;
		bool	bUpdateMakeDate = false;
		if (bAborRftList == false)
		{
			// �V����RF����S���ǉ����̃��X�g�ʒu
			lastpos = size - 1;
			bUpdateMakeDate = true;
		}
		else if (pos > 0 && pos < size)
		{
			// �V����RF����r���܂Œǉ����̃��X�g�ʒu
			lastpos = pos - 1;
			bUpdateMakeDate = true;
		}

		if (bUpdateMakeDate == true && lastpos >= 0 && lastpos < size)
		{
			time_t	iOffTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_RfInfoList[lastpos].szDate);
			time_t	iHeaderTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szMakeDate);
			if (iHeaderTime < iNowTime)
			{
				if (iOffTime <= iNowTime)
				{
					strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), m_szNowDate);
				}
				else
				{
					// �Ζ��ׂ��̏ꍇ�́ARF���t�@�C���̍Ō��OFF�����ɂ���
					strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), m_RfInfoList[lastpos].szDate);
				}
			}
		}
	}

	mDialyDataList.clear();								// ���X�g�������������
	iNo = 0;

	// �ۑ����ԓ��̃f�[�^�̂ݎc��
	for (int i = 0; i < dialyDataList.size(); i++)
	{
		strcpy_s(szSrchDate, sizeof(szSrchDate), CStringA(dialyDataList[i].date));
		strcat_s(szSrchDate, sizeof(szSrchDate), " ");
		strcat_s(szSrchDate, sizeof(szSrchDate), "00:00:00");

		iRfSrchDate = (time_t)CSatelliteData::ub_asc_to_dtime(szSrchDate);	/* ���t��ʎZ�b�ɕϊ� */
		if (iRfSrchDate > iDateLimit)
		{
			// �c���f�[�^�ł���
			if (dialyDataList[i].no != _T("-"))
			{
				iNo++;
				dialyDataList[i].no.Format(_T("%d"), iNo);		// �ԍ��U��Ȃ���
			}
			mDialyDataList.push_back(dialyDataList[i]);
		}
		else
		{
			// �폜�f�[�^�ł���
			SetEditFlg(TRUE);									// �����X�V
		}
	}

	// �L�����R�[�h���X�V
	mStrRecCount.Format(_T("%d"), iNo);
}

/*============================================================================*/
/*! CBusinessDiary

-# RF ON/OFF���Ԑݒ菈��

@param  
@retval 
*/
/*============================================================================*/
void CBusinessDiary::SetRfInfo(char *pszSatName, char *pszColim, char *pszFreq, char *pszOnDate, char *pszOffDate, vector<stSatRfInfo>& satRfInfoList)
{
	int     iOnTimeConf;
	time_t	iTime;

	iOnTimeConf = GetTimeConf(pszOnDate);
	iTime = (time_t)CSatelliteData::ub_asc_to_dtime(pszOffDate) - (time_t)CSatelliteData::ub_asc_to_dtime(pszOnDate);

	char	szSatName[24];
	memset(szSatName, '\0', sizeof(szSatName));

	if ((pszSatName[0] == ' ') || (strcmp(pszSatName, "NA") == 0))
	{
		strcpy_s(szSatName, sizeof(szSatName), pszFreq);
	}
	else
	{
		strcpy_s(szSatName, sizeof(szSatName), pszSatName);
	}

	// ���X�g�ɉq�������łɂ���΂��̃f�[�^���g���A�Ȃ���Βǉ�
	for (int i = 0; i < satRfInfoList.size(); i++)
	{
		if (strcmp(satRfInfoList[i].szaSatName, szSatName) == 0)
		{
			// �q�������������߁A���ԂƉ񐔂����Z
			satRfInfoList[i].iaTime[iOnTimeConf] += iTime;
			satRfInfoList[i].iaCnt[iOnTimeConf]++;
			return;
		}
	}

	// �V�����q���Ǝ��ԁA�񐔂�ǉ�
	stSatRfInfo rfInfo;
	memset(rfInfo.szaSatName, '\0', sizeof(rfInfo.szaSatName));
	for (int i = 0; i < 3; i++)
	{
		rfInfo.iaTime[i] = 0;
		rfInfo.iaCnt[i] = 0;
	}
	strcpy_s(rfInfo.szaSatName, sizeof(rfInfo.szaSatName), szSatName);
	rfInfo.iaTime[iOnTimeConf] = iTime;
	rfInfo.iaCnt[iOnTimeConf] = 1;
	satRfInfoList.push_back(rfInfo);
}

/*============================================================================*/
/*! CBusinessDiary

-# RF���t�@�C����ǂݍ���Ń��X�g�ɓW�J

@param  �Ȃ�
@retval true:�ǉ����� / false:�ǉ����Ȃ�����
*/
/*============================================================================*/
bool CBusinessDiary::ReadRFInfoFile()
{
	CString filename = theApp.GetShareFilePath(eFileType_Diary_Rf_File, theApp.GetSelectStation());
	FILE *fp = NULL;
	bool	bAppend = false;

	m_RfInfoList.clear();

	TRY
	{
		// RF���t�@�C���̃I�[�v��
		if ((_wfopen_s(&fp, filename, _T("r")) == 0) && (fp != NULL))
		{
			time_t     iOnTime, iAllReadyTime;

			iAllReadyTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szMakeDate);

			char szline[2048];
			CString cbuf = _T("");
			CString str;
			CStringA strA;
			stRfInfo rfInfo;
			memset(&rfInfo, 0, sizeof(rfInfo));

			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
			{
				cbuf = CServerFile::ConvertUnicode(szline);
				cbuf.TrimRight(_T("\n"));

				if (AfxExtractSubString(str, cbuf, 0, ','))
				{
					// ����
					strcpy_s(rfInfo.szDate, sizeof(rfInfo.szDate), CStringA(str));
				}

				if (AfxExtractSubString(str, cbuf, 1, ','))
				{
					// ON/OFF
					if (str == _T("ON"))
					{
						rfInfo.bOn = true;
					}
					else
					{
						rfInfo.bOn = false;
					}
				}

				if (AfxExtractSubString(str, cbuf, 2, ','))
				{
					// ���g��
					strcpy_s(rfInfo.szFreq, sizeof(rfInfo.szFreq), CStringA(str));
				}

				if (AfxExtractSubString(str, cbuf, 3, ','))
				{
					// �q����
					strcpy_s(rfInfo.szSatName, sizeof(rfInfo.szSatName), CStringA(str));
				}

				if (AfxExtractSubString(str, cbuf, 4, ','))
				{
					// COLIM
					strcpy_s(rfInfo.szColim, sizeof(rfInfo.szColim), CStringA(str));
				}

				// ��������
				if (bAppend == false && rfInfo.bOn == true)
				{
					iOnTime = (time_t)CSatelliteData::ub_asc_to_dtime(rfInfo.szDate);
					if (iOnTime > iAllReadyTime)
					{
						bAppend = true;
					}
				}

				if (bAppend)
				{
					// �V����RF����ǉ�
					m_RfInfoList.push_back(rfInfo);
					// �V�����q����������Βǉ�
					AddSpaceName(CString(rfInfo.szSatName));
				}
			}

#if 0
			// �X�V���Ԃ̍X�V
			size_t size = m_RfInfoList.size();
			if (size > 0)
			{
				time_t	iNowTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szNowDate);
				time_t	iOffTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_RfInfoList[size - 1].szDate);
				time_t	iHeaderTime = (time_t)CSatelliteData::ub_asc_to_dtime(m_szMakeDate);
				if (iHeaderTime < iNowTime)
				{
					if (iOffTime <= iNowTime)
					{
						strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), m_szNowDate);
					}
					else
					{
						// �Ζ��ׂ��̏ꍇ�́ARF���t�@�C���̍Ō��OFF�����ɂ���
						strcpy_s(m_szMakeDate, sizeof(m_szMakeDate), m_RfInfoList[size - 1].szDate);
					}
				}
			}
#endif
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

		if (fp != NULL)
			fclose(fp);

	return bAppend;
}


void CBusinessDiary::OnClose()
{
//	CDialogBase::OnClose();
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

	CWaitCursor wait;
	// �����Ɩ��������t�@�C���ɏ�������
	WriteDiaryData();
	// �����Ɩ������o�^�v��
	Shell_RegistRadiolog();

	OnOK();
}


void CBusinessDiary::OnLvnItemchangedDiaryList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// ADD 2018.06.04 >>>
	GetDlgItem(IDC_DIARY_EDIT_BTN)->EnableWindow((m_DiaryList.GetSelectedCount() == 0)?FALSE:TRUE);
	// ADD 2018.06.04 <<<

	*pResult = 0;
}
