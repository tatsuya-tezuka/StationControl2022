// DataSelectDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "DataSelectDlg.h"
#include "afxdialogex.h"
#include "WaitDialog.h"

// CDataSelectDlg ダイアログ

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


// CDataSelectDlg メッセージ ハンドラー

BOOL CDataSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDlgWaitCmd waitDlg;

	waitDlg.Open(_T("初期化"), _T("監視データのリストを読み込んでいます。\n\nしばらくお待ちください。"), false);

	m_btnAutoAssign.SetCheck(TRUE);

	_CreateControl();

	// 選択中の基地局を反映
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

	// 選択中の設備を反映
	m_nEquipment = theApp.GetSelectEquipment();
	UpdateData(FALSE);
	_ResetControl(1);

	waitDlg.Close();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CDataSelectDlg::_CreateControl()
{
	CWaitCursor wait;

	vector<CString> list;
	vector<CString>::iterator itr;

	// 基地局の作成
	list.clear();
	theApp.GetDataCollection().GetBaseStationList(list);
	m_cbStation.ResetContent();
	m_cbStation.AddString(_T("すべて"));
	for (itr = list.begin(); itr != list.end(); itr++){
		m_cbStation.AddString((*itr));
	}
	m_cbStation.SetCurSel(0);

	// 設備の作成
	list.clear();
	theApp.GetDataCollection().GetEquipmentList(_T(""), list);
	m_cbEquipment.ResetContent();
	m_cbEquipment.AddString(_T("すべて"));
	for (itr = list.begin(); itr != list.end(); itr++){
		m_cbEquipment.AddString((*itr));
	}
	m_cbEquipment.SetCurSel(0);

	// データの作成
	list.clear();
	theApp.GetDataCollection().GetDataNameList(_T(""), _T(""), list);
	m_listAll.ResetContent();
	for (itr = list.begin(); itr != list.end(); itr++){
		m_listAll.AddString((*itr));
	}

	// 既に登録されているデータ名称を右側リストに登録する
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
		// 基地局の変更
		list.clear();
		theApp.GetDataCollection().GetBaseStationList(list);

		if( ((m_nStation == 0) ? 0 : m_nStation - 1) < list.size() )
		{
			CString station = list[(m_nStation==0)?0:m_nStation - 1];
			// 設備の作成
			list.clear();
			theApp.GetDataCollection().GetEquipmentList((m_nStation == 0) ? _T("") : station, list);
			m_cbEquipment.ResetContent();
			m_cbEquipment.AddString(_T("すべて"));
			for (itr = list.begin(); itr != list.end(); itr++){
				m_cbEquipment.AddString((*itr));
			}
			m_cbEquipment.SetCurSel(0);
			// データの作成
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
		// 設備の変更
		list.clear();
		theApp.GetDataCollection().GetBaseStationList(list);

		if( ((m_nStation == 0) ? 0 : m_nStation - 1) < list.size() )
		{
			CString station = list[(m_nStation == 0) ? 0 : m_nStation - 1];
			list.clear();
			theApp.GetDataCollection().GetEquipmentList((m_nStation == 0) ? _T("") : station, list);
			CString equipment = list[(m_nEquipment == 0) ? 0 : m_nEquipment - 1];
			// データの作成
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

	// 現在の登録データ数取得
	int nCntPart = m_listPart.GetCount();

	// 追加される新規のデータ数取得
	int nCntNew = 0;
	int* ptr = (int*)aryListBoxSel.GetData();
	for (int i = 0; i < nCount; i++, ptr++){
		CString str;
		m_listAll.GetText(*ptr, str);
		int nIndex = m_listPart.FindStringExact(0, str);
		if (nIndex == LB_ERR)
			nCntNew++;
	}

	// 上限以上の場合エラー出力
	if (nCntPart + nCntNew > MAX_DATASELECT)
	{
		CString mes;
		mes.Format(_T("%d以上のデータ登録は出来ません。"), MAX_DATASELECT);
		MessageBox(mes, _T("Error"), MB_ICONERROR | MB_OK);
		if (nCntPart != MAX_DATASELECT && nCount != 1)
		{
			if (MessageBox(_T("選択されているデータを追加登録しますか？\n(選択されているデータで登録数が最大になるまで追加します。)"), _T("Information"), MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
			{
				return;
			}
		}
	}

	// データ登録
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

#if 0	// 一度に大量(80個)の項目を削除すると残ってしまうためインデックスで削除するように修正
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

	// 選択監視名リストを作成
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

	// ステータス文字列リストを作成
	theApp.GetDataCollection().CreateStatusList();

	theApp.GetDataCollection().RequestAddSignalList();

	// 選択の基地局／設備を更新
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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
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
