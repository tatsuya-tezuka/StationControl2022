
// TrendServerDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "TrendServer.h"
#include "TrendServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTrendServerDlg �_�C�A���O



CTrendServerDlg::CTrendServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrendServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTrendServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATION, mListStation);
	DDX_Control(pDX, IDC_LIST_EQUIPMENT, mListEquipment);
	DDX_Control(pDX, IDC_LIST_SIGNAL, mListSignal);
	DDX_Control(pDX, IDC_RICHEDIT_MESSAGE, mMessage);
}

BEGIN_MESSAGE_MAP(CTrendServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTrendServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CTrendServerDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


/*============================================================================*/
/*! �T�[�o�[

-# ����T�[�o�[�̊J�n

@param

@retval

*/
/*============================================================================*/
bool CTrendServerDlg::_StartServer()
{
	vector<stDataList>& datalist = m_DataServer.GetDataList();
	// �f�[�^���X�g�̓o�^
	int cntStation = mListStation.GetItemCount();
	int cntEquipment = mListEquipment.GetItemCount();
	int cntSignal = mListSignal.GetItemCount();

	datalist.clear();
	int i, j, k;
	CString str;
	for (i = 0; i < cntStation; i++){
		if (mListStation.GetCheck(i) == FALSE)
			continue;
		for (j = 0; j < cntEquipment; j++){
			if (mListEquipment.GetCheck(j) == FALSE)
				continue;
			for (k = 0; k < cntSignal; k++){
				if (mListSignal.GetCheck(k) == FALSE)
					continue;
				str = mListStation.GetItemText(i, 0);
				str += _T(".");
				str += mListEquipment.GetItemText(j, 0);
				str += _T(".");
				str += mListSignal.GetItemText(k, 0);
				stDataList val;
				CStringA name(str);
				sprintf_s((char*)val.name, _MAX_NAME, "%s", name);
				CStringA unit(mDataUnitString[k]);
				sprintf_s((char*)val.unit, _MAX_UNIT, "%s", unit);
				datalist.push_back(val);
			}
		}
	}

	bool bSuccess = m_DataServer.Create(CCommServer::mDataPort);
	if (bSuccess){
		m_DataServer.SetMessageWnd(this);
		char buf[MAX_PATH];
		CString str;
		m_DataServer.GetLocalAddress(buf, MAX_PATH);
		str.Format(_T("=== Start Server : %s(%d)"), CString(buf), CCommServer::mDataPort);
		_AppendMessage(str);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_EQUIPMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_SIGNAL)->EnableWindow(FALSE);
	}

	return bSuccess;
}

/*============================================================================*/
/*! �T�[�o�[

-# ����T�[�o�[�̏I��

@param

@retval

*/
/*============================================================================*/
bool CTrendServerDlg::_StopServer()
{
	bool bSuccess = m_DataServer.Release();
	if (bSuccess){
		_AppendMessage(_T("=== Stop Server"));
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_EQUIPMENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_SIGNAL)->EnableWindow(TRUE);
	}
	return true;
}

/*============================================================================*/
/*! �T�[�o�[

-# ���X�g�R���g���[���ւ̍��ڒǉ�

@param

@retval

*/
/*============================================================================*/
int CTrendServerDlg::_AddItem(CListCtrl& list, int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
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
		return list.InsertItem(&lv);
	return list.SetItem(&lv);
}
/*============================================================================*/
/*! �T�[�o�[

-# ���b�Z�[�W����ʂɏo�͂���

@param	msg		���b�Z�[�W
@param	level	�\�����x��
@param	color	���b�Z�[�W�F

@retval

*/
/*============================================================================*/
int CTrendServerDlg::_AppendMessage(CString msg, COLORREF color/* = RGB(0, 0, 0)*/)
{
	int nOldLines = 0, nNewLines = 0, nScroll = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	if (mMessage.GetSafeHwnd() == NULL)
		return 0;


	nOldLines = mMessage.GetLineCount();

	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;
	cf.crTextColor = color;

	nInsertionPoint = mMessage.GetWindowTextLength();
	mMessage.SetSel(nInsertionPoint, -1);

	mMessage.SetSelectionCharFormat(cf);

	msg += _T("\n");
	mMessage.ReplaceSel(msg);

	nNewLines = mMessage.GetLineCount();

	nScroll = nNewLines - nOldLines;
	mMessage.LineScroll(nScroll);

	return 0;
}


// CTrendServerDlg ���b�Z�[�W �n���h���[

BOOL CTrendServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	int pos;
	int i;
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(mListStation.m_hWnd);
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES;
	// ��n�ǂ̍쐬
	ListView_SetExtendedListViewStyle(mListStation.m_hWnd, dwStyle);
	for (i = 0; i<sizeof(mBaseStationString) / sizeof(mBaseStationString[0]); i++){
		pos = _AddItem(mListStation, i, 0, mBaseStationString[i], i);
		mListStation.SetCheck(pos);
	}
	// �ݔ��̍쐬
	ListView_SetExtendedListViewStyle(mListEquipment.m_hWnd, dwStyle);
	for (i = 0; i<sizeof(mEquipmentString) / sizeof(mEquipmentString[0]); i++){
		pos = _AddItem(mListEquipment, i, 0, mEquipmentString[i], i);
		mListEquipment.SetCheck(pos);
	}
	// ��n�ǂ̍쐬
	ListView_SetExtendedListViewStyle(mListSignal.m_hWnd, dwStyle);
	for (i = 0; i<sizeof(mDataNameString) / sizeof(mDataNameString[0]); i++){
		pos = _AddItem(mListSignal, i, 0, mDataNameString[i], i);
		mListSignal.SetCheck(pos);
	}

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CTrendServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CTrendServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CTrendServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTrendServerDlg::OnBnClickedButtonStart()
{
	_StartServer();
}


void CTrendServerDlg::OnBnClickedButtonStop()
{
	_StopServer();
}


LRESULT CTrendServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ClientConnection:
	{
		CADDRLIST::iterator itr;
		itr = m_DataServer.GetClientAddrList().find((ULONG)lParam);
		if (itr != m_DataServer.GetClientAddrList().end()){
			CString str;
			str.Format(_T("# Connect Client : %s"), CString((*itr).second.ipClient));
			_AppendMessage(str);
		}
	}
	break;
	case	eMessage_ClientDisConnection:
	{
		CADDRLIST::iterator itr;
		itr = m_DataServer.GetClientAddrList().find((ULONG)lParam);
		if (itr != m_DataServer.GetClientAddrList().end()){
			CString str;
			str.Format(_T("# DisConnect Client : %s"), CString((*itr).second.ipClient));
			_AppendMessage(str);
		}
	}
	break;
	case	eMessage_ClientCommand:
	{
		CADDRLIST::iterator itr;
		itr = m_DataServer.GetClientAddrList().find((ULONG)lParam);
		if (itr != m_DataServer.GetClientAddrList().end()){
			CString str;
			str.Format(_T("- (%s) : Command(%d)"), CString((*itr).second.ipClient), wParam);
			_AppendMessage(str);
		}
	}
	break;
	default:
		return CDialogEx::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
