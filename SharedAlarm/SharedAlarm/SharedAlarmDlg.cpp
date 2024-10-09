
// SharedAlarmDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "SharedAlarm.h"
#include "SharedAlarmDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCriticalSection	CSharedAlarmDlg::mUpdateSection;

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


// CSharedAlarmDlg �_�C�A���O



CSharedAlarmDlg::CSharedAlarmDlg(int mode, CString alarmFile /*= _T("")*/, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSharedAlarmDlg::IDD, pParent)
	, m_nClient(0)
	, m_strUpdateTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mAlarmFile = alarmFile;
	m_nClient = mode;
	mStation = -1;
	ClearStation();
}

void CSharedAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ALARM, mAlarmFile);
	DDX_Radio(pDX, IDC_RADIO_TYPE1, m_nClient);
	DDX_Control(pDX, IDC_LIST1, mList);
	DDX_Check(pDX, IDC_CHECK1, m_bStation[0]);
	DDX_Check(pDX, IDC_CHECK2, m_bStation[1]);
	DDX_Check(pDX, IDC_CHECK3, m_bStation[2]);
	DDX_Check(pDX, IDC_CHECK4, m_bStation[3]);
	DDX_Control(pDX, IDC_CHECK1, m_ctlSta64);
	DDX_Control(pDX, IDC_CHECK2, m_ctlSta34);
	DDX_Control(pDX, IDC_CHECK3, m_ctlSta20);
	DDX_Control(pDX, IDC_CHECK4, m_ctlSta54);
	DDX_Text(pDX, IDC_STATIC_UPDATETIME, m_strUpdateTime);
}

BEGIN_MESSAGE_MAP(CSharedAlarmDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ALARM, &CSharedAlarmDlg::OnBnClickedButtonAlarm)
	ON_BN_CLICKED(IDC_RADIO_TYPE1, &CSharedAlarmDlg::OnBnClickedRadioType1)
	ON_BN_CLICKED(IDC_RADIO_TYPE2, &CSharedAlarmDlg::OnBnClickedRadioType2)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSharedAlarmDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECV, &CSharedAlarmDlg::OnBnClickedButtonRecv)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, &CSharedAlarmDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CSharedAlarmDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CSharedAlarmDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CSharedAlarmDlg::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CSharedAlarmDlg ���b�Z�[�W �n���h���[

BOOL CSharedAlarmDlg::OnInitDialog()
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

	DWORD  dwStyle = ListView_GetExtendedListViewStyle(mList.m_hWnd);
	//  �g���X�^�C���̃Z�b�g
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  �V�����g���X�^�C����ݒ�
	ListView_SetExtendedListViewStyle(mList.m_hWnd, dwStyle);

	const TCHAR* mListHeader[] = {
		_T("��"),
		_T("���M��"),
		_T("�X�e�[�^�X"),
		_T("�Ď����ږ�"),
		_T("�Ď�����"),
	};
	for (int index = 0; index < sizeof(mListHeader) / sizeof(mListHeader[0]); index++){
		mList.InsertColumn(index, mListHeader[index], LVCFMT_LEFT, 200);
	}

	UpdateControl();

	if (m_nClient == 0){
		m_ctlSta64.EnableWindow(TRUE);
		m_ctlSta34.EnableWindow(TRUE);
		m_ctlSta20.EnableWindow(TRUE);
		m_ctlSta54.EnableWindow(TRUE);
		mAlarmFile.Empty();
		SetTimer(1, 1000, NULL);
	}
	else{
		m_ctlSta64.EnableWindow(FALSE);
		m_ctlSta34.EnableWindow(FALSE);
		m_ctlSta20.EnableWindow(FALSE);
		m_ctlSta54.EnableWindow(FALSE);
		if (mAlarmFile.IsEmpty() == false){
			// �A���[���t�@�C���ǂݍ���
			ReadAlarmFile();
		}
	}

	if (m_nClient == 0){
		SetTimer(1, 1000, NULL);
	}

	AutoSizeColumns();

	UpdateData(FALSE);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

int CSharedAlarmDlg::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwAll/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwAll;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return mList.InsertItem(&lv);
	return mList.SetItem(&lv);
}

#define	MINCOLWIDTH		30
template <class T> inline const T& MAX(const T& t1, const T& t2) {
	return t1 < t2 ? t2 : t1;
}
void CSharedAlarmDlg::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)mList.GetDlgItem(0);
	// create dummy
	int dummycol = pHeader->GetItemCount();
	mList.InsertColumn(dummycol, _T("dummy"), LVCFMT_LEFT, 10);

	mList.SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		mList.SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = mList.GetColumnWidth(col);
		mList.SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = mList.GetColumnWidth(col);
		int wc = MAX(MINCOLWIDTH, MAX(wc1, wc2));
		mList.SetColumnWidth(col, wc);
	}
	// delete dummy
	mList.DeleteColumn(dummycol);
	mList.SetRedraw(true);
}

void CSharedAlarmDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSharedAlarmDlg::OnPaint()
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
HCURSOR CSharedAlarmDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSharedAlarmDlg::OnBnClickedButtonAlarm()
{
	static TCHAR BASED_CODE szFilter[] = _T("Alarm Files|*.csv|All Files|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

	mAlarmFile = dlg.GetPathName();
	UpdateData(FALSE);

	if (mAlarmFile.IsEmpty() == true){
		//MessageBox(_T("�A���[���t�@�C�����I������Ă��܂���"), _T(""), MB_OK | MB_ICONERROR);
		return;
	}

	// �A���[���t�@�C���ǂݍ���
	ReadAlarmFile();
}

void CSharedAlarmDlg::UpdateControl()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_BUTTON_ALARM)->EnableWindow((m_nClient == 1));
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow((m_nClient == 1));
	GetDlgItem(IDC_BUTTON_RECV)->EnableWindow((m_nClient == 0));
}

void CSharedAlarmDlg::UpdateAlarmList(vector<CShareAlarmIf::stAlarmItem>& list)
{
	mList.SetRedraw(false);

	mList.DeleteAllItems();

	vector<CShareAlarmIf::stAlarmItem>::iterator itr;
	int item = 0;
	CString str;
	CString time = _T("");
	COleDateTime oleTime, oleTemp;
	for (itr = list.begin(); itr != list.end(); itr++){
		if ((*itr).line.no == 0)
			continue;
		if ((*itr).line.no == (DWORD)-1)
		{
			// �������݂̂̏ꍇ
			str = CString((*itr).updatetime);
			str.Replace('-', '/');
			oleTemp.ParseDateTime(str);
			if (oleTemp > oleTime){
				time = CString((*itr).updatetime);
				oleTime = oleTemp;
			}
			continue;
		}
		str = CString((*itr).updatetime);
		str.Replace('-', '/');
		oleTemp.ParseDateTime(str);
		if (oleTemp > oleTime){
			time = CString((*itr).updatetime);
			oleTime = oleTemp;
		}
		str.Format(_T("%s"), mAlarmStationString[(*itr).line.station]);
		AddItem(item, 0, str);
		str.Format(_T("%s"), (LPCTSTR)mAlarmEquipmentString[(*itr).line.sender]);
		AddItem(item, 1, str);
		str.Format(_T("%s"), mAlarmStatusString[(*itr).line.status]);
		AddItem(item, 2, str);
		str.Format(_T("%s"), (LPCTSTR)CString((*itr).line.monname));
		AddItem(item, 3, str);
		str.Format(_T("%s"), (LPCTSTR)CString((*itr).line.monval));
		AddItem(item, 4, str);
		item++;
	}

	AutoSizeColumns();

	m_strUpdateTime = time;

	mList.SetRedraw(true);
}

void CSharedAlarmDlg::ReadAlarmFile()
{
	mAlarmList.clear();
	int nStation = mShareAlarmIf.ConvertAlarmData(mAlarmFile, mAlarmList);
	if (nStation < 0){
		//MessageBox(_T("���L�������̓o�^�Ɏ��s���܂���"), _T(""), MB_OK | MB_ICONERROR);
		return;
	}

	// �ꗗ�\���X�V
	UpdateAlarmList(mAlarmList);
	// ���L����������
	CreateShareMemUpdateDisplay(nStation);
}

bool CSharedAlarmDlg::CreateShareMem(int nStation)
{
	if (mStation == nStation && mShareAlarmIf.IsValidMap() == true)
		return true;

	mStation = nStation;

	mShareAlarmIf.Delete();
	if (m_nClient == 0){
		return mShareAlarmIf.Create(true, nStation);
	}
	else{
		return mShareAlarmIf.Create(false, nStation);
	}
}

bool CSharedAlarmDlg::CreateShareMemUpdateDisplay(int nStation)
{
	bool ret = CreateShareMem(nStation);
	if (ret) 
	{
		SetStation(nStation);
	}
	else
	{
		ClearStation();
	}
	UpdateData(FALSE);

	return ret;
}


void CSharedAlarmDlg::OnBnClickedRadioType1()
{
	mStation = GetStation();

	if (mStation < 0 && mStation >= CShareAlarmIf::mAlarmStationMax)
		return;

	mShareAlarmIf.Create(true, mStation);
	UpdateControl();
}


void CSharedAlarmDlg::OnBnClickedRadioType2()
{
	mStation = GetStation();

	if (mStation < 0 && mStation >= CShareAlarmIf::mAlarmStationMax)
		return;

	mShareAlarmIf.Create(false, mStation);
	UpdateControl();
}


void CSharedAlarmDlg::OnBnClickedButtonSend()
{
	if (mStation < 0 && mStation >= CShareAlarmIf::mAlarmStationMax)
		return;

	CreateShareMem(mStation);
	mShareAlarmIf.Send((CShareAlarmIf::stAlarmItem*)&mAlarmList.at(0), mAlarmList.size());
}


void CSharedAlarmDlg::OnBnClickedButtonRecv()
{
	mUpdateSection.Lock();
	vector<CShareAlarmIf::stAlarmItem> temp(0);
	temp.clear();
	temp.resize(CShareAlarmIf::mAlarmItemMax);
	bool ret = mShareAlarmIf.Recv((CShareAlarmIf::stAlarmItem*)&temp.at(0), temp.size());
	if (ret == true){
		mAlarmList.swap(temp);
		mAlarmList.resize(CShareAlarmIf::mAlarmItemMax);
		UpdateAlarmList(mAlarmList);
		UpdateData(FALSE);
	}
	mUpdateSection.Unlock();
}


void CSharedAlarmDlg::OnTimer(UINT_PTR nIDEvent)
{
	mUpdateSection.Lock();
	vector<CShareAlarmIf::stAlarmItem> temp(0);
	temp.clear();
	temp.resize(CShareAlarmIf::mAlarmItemMax);
	bool ret = mShareAlarmIf.Recv((CShareAlarmIf::stAlarmItem*)&temp.at(0), temp.size());
	if (ret == true){
		mAlarmList.swap(temp);
		mAlarmList.resize(CShareAlarmIf::mAlarmItemMax);
		UpdateAlarmList(mAlarmList);
		UpdateData(FALSE);
	}
	mUpdateSection.Unlock();

	CDialogEx::OnTimer(nIDEvent);
}


void CSharedAlarmDlg::OnClose()
{
	KillTimer(1);

	CDialogEx::OnClose();
}

int CSharedAlarmDlg::GetStation()
{
	for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++){
		if (m_bStation[i] == TRUE){
			return i;
		}
	}
	return -1;
}

void CSharedAlarmDlg::SetStation(int nStation)
{
	for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++){
		if (i == nStation){
			m_bStation[i] = TRUE;
		}
		else
		{
			m_bStation[i] = FALSE;
		}
	}
}

void CSharedAlarmDlg::ClearStation()
{
	for (int i = 0; i < CShareAlarmIf::mAlarmStationMax; i++){
		m_bStation[i] = FALSE;
	}
}

void CSharedAlarmDlg::OnBnClickedCheck1()
{
	// ���L����������
	CreateShareMemUpdateDisplay(0);
}


void CSharedAlarmDlg::OnBnClickedCheck2()
{
	// ���L����������
	CreateShareMemUpdateDisplay(1);
}


void CSharedAlarmDlg::OnBnClickedCheck3()
{
	// ���L����������
	CreateShareMemUpdateDisplay(2);
}


void CSharedAlarmDlg::OnBnClickedCheck4()
{
	// ���L����������
	CreateShareMemUpdateDisplay(3);
}
