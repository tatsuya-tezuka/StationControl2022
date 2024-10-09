// EquipmentCtrlStatus.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "EquipmentCtrlStatus.h"
#include "afxdialogex.h"


// CEquipmentCtrlStatus ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentCtrlStatus, CDialogEx)

CEquipmentCtrlStatus::CEquipmentCtrlStatus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEquipmentCtrlStatus::IDD, pParent)
	, m_strContName(_T(""))
	, m_strWindowTitle(_T(""))
{
	mControlName.Empty();
	mCurrentName.Empty();
	mStatusList.clear();
}

CEquipmentCtrlStatus::~CEquipmentCtrlStatus()
{
}

void CEquipmentCtrlStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbStatus);
	DDX_Text(pDX, IDC_EQUIPCTRL_NUM_EQIPT_STATIC, m_strContName);
}


BEGIN_MESSAGE_MAP(CEquipmentCtrlStatus, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEquipmentCtrlStatus::OnBnClickedOk)
END_MESSAGE_MAP()


// CEquipmentCtrlStatus メッセージ ハンドラー


BOOL CEquipmentCtrlStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_strWindowTitle.IsEmpty())
		SetWindowText(m_strWindowTitle);

	vector<CString>::iterator itr;
	for (itr = mStatusList.begin(); itr != mStatusList.end(); itr++){
		m_cbStatus.AddString((*itr));
	}
	
	int item = m_cbStatus.FindStringExact(-1, mCurrentName);
	m_cbStatus.SetCurSel(item);

	m_strContName = mControlName;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CEquipmentCtrlStatus::OnBnClickedOk()
{
	UpdateData(TRUE);

	int item = m_cbStatus.GetCurSel();
	if (item < 0)
	{
		CString title;
		GetWindowText(title);
		MessageBox(_T("ステータス値を選択してください。"), title, MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	mCurrentName = mStatusList[item];

	CDialogEx::OnOK();
}
