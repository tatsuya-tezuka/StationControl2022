// DataSelectDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "DataSelectDlg.h"
#include "afxdialogex.h"
#include "WaitDialog.h"

// CDataSelectDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CDataSelectDlg, CDialogEx)

CDataSelectDlg::CDataSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataSelectDlg::IDD, pParent)
	, m_nStation(0)
	, m_nEquipment(0)
{
	m_bAutoAssign = true;
}

CDataSelectDlg::~CDataSelectDlg()
{
}

void CDataSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STATION, m_cbStation);
	DDX_CBIndex(pDX, IDC_COMBO_STATION, m_nStation);
	DDX_Control(pDX, IDC_COMBO_EQUIPMENT, m_cbEquipment);
	DDX_CBIndex(pDX, IDC_COMBO_EQUIPMENT, m_nEquipment);
	DDX_Control(pDX, IDC_LIST1, m_listAll);
	DDX_Control(pDX, IDC_LIST2, m_listPart);
	DDX_Control(pDX, IDC_CHECK_AUTO_ASSIGN, m_btnAutoAssign);
}


BEGIN_MESSAGE_MAP(CDataSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDataSelectDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDataSelectDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CDataSelectDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CDataSelectDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDOK, &CDataSelectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDataSelectDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_STATION, &CDataSelectDlg::OnCbnSelchangeComboStation)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENT, &CDataSelectDlg::OnCbnSelchangeComboEquipment)
	ON_LBN_SELCHANGE(IDC_LIST2, &CDataSelectDlg::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// CDataSelectDlg ���b�Z�[�W �n���h���[

BOOL CDataSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDlgWaitCmd waitDlg;

	waitDlg.Open(_T("������"), _T("�Ď��f�[�^�̃��X�g��ǂݍ���ł��܂��B\n\n���΂炭���҂����������B"), false);

	m_btnAutoAssign.SetCheck(TRUE);

	_CreateControl();

	// �I�𒆂̊�n�ǂ𔽉f
//	m_nStation = (theApp.GetSelectStationMode() == eStation_1) ? (theApp.GetSelectStation() + 1) : 0;
	m_nStation = 0;
	if( (theApp.GetSelectStationMode() == eStation_1) && (theApp.GetSelectStation() < eStation_MAX) )
	{
		CString	sInitStr = mStationSimpleString[theApp.GetSelectStation()];
		for( int i = 1; i < m_cbStation.GetCount(); i++ )
		{
			CString	sFindStr;
			m_cbStation.GetLBText(i, sFindStr);
			if( sInitStr == sFindStr )
			{
				m_nStation = i;

				break;
			}
		}
	}
	UpdateData(FALSE);
	_ResetControl(0);

	// �I�𒆂̐ݔ��𔽉f
	m_nEquipment = theApp.GetSelectEquipment();
	UpdateData(FALSE);
	_ResetControl(1);

	waitDlg.Close();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CDataSelectDlg::_CreateControl()
{
	CWaitCursor wait;

	vector<CString> list;
	vector<CString>::iterator itr;

	// ��n�ǂ̍쐬
	list.clear();
	theApp.GetDataCollection().GetBaseStationList(list);
	m_cbStation.ResetContent();
	m_cbStation.AddString(_T("���ׂ�"));
	for (itr = list.begin(); itr != list.end(); itr++){
		m_cbStation.AddString((*itr));
	}
	m_cbStation.SetCurSel(0);

	// �ݔ��̍쐬
	list.clear();
	theApp.GetDataCollection().GetEquipmentList(_T(""), list);
	m_cbEquipment.ResetContent();
	m_cbEquipment.AddString(_T("���ׂ�"));
	for (itr = list.begin(); itr != list.end(); itr++){
		m_cbEquipment.AddString((*itr));
	}
	m_cbEquipment.SetCurSel(0);

	// �f�[�^�̍쐬
	list.clear();
	theApp.GetDataCollection().GetDataNameList(_T(""), _T(""), list);
	m_listAll.ResetContent();
	for (itr = list.begin(); itr != list.end(); itr++){
		m_listAll.AddString((*itr));
	}

	// ���ɓo�^����Ă���f�[�^���̂��E�����X�g�ɓo�^����
	m_listPart.ResetContent();
	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itrlist;
	for (itrlist = datalist.begin(); itrlist != datalist.end(); itrlist++){
		m_listPart.AddString((*itrlist).name);
	}
}

void CDataSelectDlg::_ResetControl(int type)
{
	CWaitCursor wait;

	vector<CString> list;
	vector<CString>::iterator itr;

	if (type == 0){
		// ��n�ǂ̕ύX
		list.clear();
		theApp.GetDataCollection().GetBaseStationList(list);

		if( ((m_nStation == 0) ? 0 : m_nStation - 1) < list.size() )
		{
			CString station = list[(m_nStation==0)?0:m_nStation - 1];
			// �ݔ��̍쐬
			list.clear();
			theApp.GetDataCollection().GetEquipmentList((m_nStation == 0) ? _T("") : station, list);
			m_cbEquipment.ResetContent();
			m_cbEquipment.AddString(_T("���ׂ�"));
			for (itr = list.begin(); itr != list.end(); itr++){
				m_cbEquipment.AddString((*itr));
			}
			m_cbEquipment.SetCurSel(0);
			// �f�[�^�̍쐬
			list.clear();
			theApp.GetDataCollection().GetDataNameList((m_nStation == 0) ? _T("") : station, _T(""), list);
			m_listAll.ResetContent();
			for (itr = list.begin(); itr != list.end(); itr++){
				m_listAll.AddString((*itr));
			}
		}
		else
			m_listAll.ResetContent();
	}
	if (type == 1){
		// �ݔ��̕ύX
		list.clear();
		theApp.GetDataCollection().GetBaseStationList(list);

		if( ((m_nStation == 0) ? 0 : m_nStation - 1) < list.size() )
		{
			CString station = list[(m_nStation == 0) ? 0 : m_nStation - 1];
			list.clear();
			theApp.GetDataCollection().GetEquipmentList((m_nStation == 0) ? _T("") : station, list);
			CString equipment = list[(m_nEquipment == 0) ? 0 : m_nEquipment - 1];
			// �f�[�^�̍쐬
			list.clear();
			theApp.GetDataCollection().GetDataNameList((m_nStation == 0) ? _T("") : station, (m_nEquipment == 0) ? _T("") : equipment, list);
			m_listAll.ResetContent();
			for (itr = list.begin(); itr != list.end(); itr++){
				m_listAll.AddString((*itr));
			}
		}
		else
			m_listAll.ResetContent();
	}
}

void CDataSelectDlg::OnBnClickedButtonAdd()
{
#if 1
	int nCount = m_listAll.GetSelCount();
	CArray<int, int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_listAll.GetSelItems(nCount, aryListBoxSel.GetData());

	// ���݂̓o�^�f�[�^���擾
	int nCntPart = m_listPart.GetCount();

	// �ǉ������V�K�̃f�[�^���擾
	int nCntNew = 0;
	int* ptr = (int*)aryListBoxSel.GetData();
	for (int i = 0; i < nCount; i++, ptr++){
		CString str;
		m_listAll.GetText(*ptr, str);
		int nIndex = m_listPart.FindStringExact(0, str);
		if (nIndex == LB_ERR)
			nCntNew++;
	}

	// ����ȏ�̏ꍇ�G���[�o��
	if (nCntPart + nCntNew > MAX_DATASELECT)
	{
		CString mes;
		mes.Format(_T("%d�ȏ�̃f�[�^�o�^�͏o���܂���B"), MAX_DATASELECT);
		MessageBox(mes, _T("Error"), MB_ICONERROR | MB_OK);
		if (nCntPart != MAX_DATASELECT && nCount != 1)
		{
			if (MessageBox(_T("�I������Ă���f�[�^��ǉ��o�^���܂����H\n(�I������Ă���f�[�^�œo�^�����ő�ɂȂ�܂Œǉ����܂��B)"), _T("Information"), MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
			{
				return;
			}
		}
	}

	// �f�[�^�o�^
	aryListBoxSel.SetSize(nCount);
	m_listAll.GetSelItems(nCount, aryListBoxSel.GetData());

	int nCntOrg = 0;
	ptr = (int*)aryListBoxSel.GetData();
	//m_listPart.ResetContent();
	for (int i = 0; i < nCount; i++, ptr++){
		if (nCntPart + nCntOrg >= MAX_DATASELECT)
		{
			break;
		}
		CString str;
		m_listAll.GetText(*ptr, str);
		int nIndex = m_listPart.FindStringExact(0, str);
		if (nIndex == LB_ERR)
		{
			m_listPart.AddString(str);
			nCntOrg++;
		}
	}
#else
	int nCount = m_listAll.GetSelCount();
	CArray<int, int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_listAll.GetSelItems(nCount, aryListBoxSel.GetData());

	int* ptr = (int*)aryListBoxSel.GetData();
	//m_listPart.ResetContent();
	for (int i = 0; i < nCount; i++, ptr++){
		CString str;
		m_listAll.GetText(*ptr, str);
		int nIndex = m_listPart.FindStringExact(0, str);
		if (nIndex == LB_ERR)
			m_listPart.AddString(str);
	}
#endif
}


void CDataSelectDlg::OnBnClickedButtonDel()
{
	int nCount = m_listPart.GetSelCount();
	CArray<int, int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_listPart.GetSelItems(nCount, aryListBoxSel.GetData());

#if 0	// ��x�ɑ��(80��)�̍��ڂ��폜����Ǝc���Ă��܂����߃C���f�b�N�X�ō폜����悤�ɏC��
	vector<CString> list;
	vector<CString>::iterator itr;
	int* ptr = (int*)aryListBoxSel.GetData();
	for (int i = 0; i < nCount; i++, ptr++){
		CString str;
		m_listPart.GetText(*ptr, str);
		list.push_back(str);
	}

	for (itr = list.begin(); itr != list.end(); itr++){
		int nIndex = m_listPart.FindStringExact(0, (*itr));
		if (nIndex != LB_ERR)
			m_listPart.DeleteString(nIndex);
	}
#else
	int	*ptr = (int*)aryListBoxSel.GetData();
	for( int i = nCount - 1; i >= 0; i-- )
		m_listPart.DeleteString(ptr[i]);
#endif
}


void CDataSelectDlg::OnBnClickedButtonUp()
{
	int nIndex = m_listPart.GetCurSel();
	if (nIndex == 0)
		return;
	CString str;
	m_listPart.GetText(nIndex, str);
	m_listPart.DeleteString(nIndex);
	nIndex = m_listPart.InsertString(nIndex - 1, str);
	m_listPart.SetSel(nIndex);
}


void CDataSelectDlg::OnBnClickedButtonDown()
{
	int nIndex = m_listPart.GetCurSel();
	if (nIndex == m_listPart.GetCount()-1)
		return;
	CString str;
	m_listPart.GetText(nIndex, str);
	m_listPart.DeleteString(nIndex);
	nIndex = m_listPart.InsertString(nIndex + 1, str);
	m_listPart.SetSel(nIndex);
}


void CDataSelectDlg::OnBnClickedOk()
{
	CWaitCursor wait;

	UpdateData(TRUE);

	vector<stSelectData>&	datalist = theApp.GetDataCollection().GetSelectDataList();
	map<CString, DWORD>&	name2id	= theApp.GetDataCollection().GetSelName2Id();
	map<DWORD, CString>&	id2name	= theApp.GetDataCollection().GetSelId2Name();
	map<DWORD, int>&		id2index = theApp.GetDataCollection().GetSelId2Index();

	datalist.clear();
	name2id.clear();
	id2name.clear();
	id2index.clear();

	// �I���Ď������X�g���쐬
	int nCount = m_listPart.GetCount();
	for (int i = 0; i < nCount; i++){
		stSelectData data;
		m_listPart.GetText(i, data.name);
		data.id = theApp.GetDataCollection().ConvNameToId(data.name);
		datalist.push_back(data);
		name2id.insert(map<CString, DWORD>::value_type(data.name, data.id));
		id2name.insert(map<DWORD, CString>::value_type(data.id, data.name));
		id2index.insert(map<DWORD, int>::value_type(data.id, i));
	}

	// �X�e�[�^�X�����񃊃X�g���쐬
	theApp.GetDataCollection().CreateStatusList();

	theApp.GetDataCollection().RequestAddSignalList();

	// �I���̊�n�ǁ^�ݔ����X�V
	if( m_nStation == 0 )
		theApp.SetSelectStationMode(eStation_5);
	else
	{
		theApp.SetSelectStationMode(eStation_1);

		CString	sEndStr;
		m_cbStation.GetLBText(m_nStation, sEndStr);
		for( int i = 0; i < eStation_MAX; i++ )
		{
			if( sEndStr == mStationSimpleString[i] )
			{
				theApp.SetSelectStation(i);

				break;
			}
		}
	}
	theApp.SetSelectEquipment(m_nEquipment);

	m_bAutoAssign = (m_btnAutoAssign.GetCheck() != 0) ? true : false;

	CDialogEx::OnOK();
}


void CDataSelectDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}


void CDataSelectDlg::OnCbnSelchangeComboStation()
{
	UpdateData(TRUE);
	_ResetControl(0);
}


void CDataSelectDlg::OnCbnSelchangeComboEquipment()
{
	UpdateData(TRUE);
	_ResetControl(1);
}


void CDataSelectDlg::OnLbnSelchangeList2()
{
	int nCount = m_listPart.GetSelCount();
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow((nCount == 1) ? TRUE : FALSE);
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow((nCount == 1) ? TRUE : FALSE);
}
