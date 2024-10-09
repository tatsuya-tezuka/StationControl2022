/*============================================================================*/
/*! ChangeControlDlg.cpp

-# 制御権利変更
*/
/*============================================================================*/
// ChangeControlDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ChangeControlDlg.h"
#include "afxdialogex.h"


// CChangeControlDlg ダイアログ

IMPLEMENT_DYNAMIC(CChangeControlDlg, CDialogEx)

CChangeControlDlg::CChangeControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangeControlDlg::IDD, pParent)
{
	m_Status = 1;
	for (int i = 0; i < mMaxTimer; i++){
		m_Timer[i] = 0;
	}
}

CChangeControlDlg::~CChangeControlDlg()
{
}

void CChangeControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Status);
	DDX_Text(pDX, IDC_EDIT1, m_Timer[0]);
	DDX_Text(pDX, IDC_EDIT2, m_Timer[1]);
	DDX_Text(pDX, IDC_EDIT3, m_Timer[2]);
	DDX_Text(pDX, IDC_EDIT4, m_Timer[3]);
	DDX_Text(pDX, IDC_EDIT5, m_Timer[4]);
	DDX_Text(pDX, IDC_EDIT6, m_Timer[5]);
	DDX_Text(pDX, IDC_EDIT7, m_Timer[6]);
	DDX_Text(pDX, IDC_EDIT8, m_Timer[7]);
}


BEGIN_MESSAGE_MAP(CChangeControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CChangeControlDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CChangeControlDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CChangeControlDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CChangeControlDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CChangeControlDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CChangeControlDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CChangeControlDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CChangeControlDlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CChangeControlDlg::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &CChangeControlDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &CChangeControlDlg::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO12, &CChangeControlDlg::OnBnClickedRadio12)
	ON_BN_CLICKED(IDC_RADIO13, &CChangeControlDlg::OnBnClickedRadio13)
	ON_BN_CLICKED(IDC_RADIO14, &CChangeControlDlg::OnBnClickedRadio14)
	ON_BN_CLICKED(IDC_RADIO15, &CChangeControlDlg::OnBnClickedRadio15)
	ON_BN_CLICKED(IDOK, &CChangeControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChangeControlDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChangeControlDlg メッセージ ハンドラー


BOOL CChangeControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFileAccess& ac = theApp.GetFileAccess();
	int i;
	CString strDefaultPass = mDefaultPassword;
	strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
	CStringA strPass = CStringA(strDefaultPass);
	// 地上局
	for (i = 0; i < mMaxType; i++){
		if (ac.ReadSOSControl(mKControl[i], eSOSusc20 + i) == false){
			// ファイルが存在しないのでデフォルトを設定する
			mKControl[i].flag = 0;
			memcpy((char*)mKControl[i].password, (char*)(const char*)strPass, mMaxPassword);
			sprintf_s(mKControl[i].lifetime, mMaxLifetime, "0");
			ac.WriteSOSControl(mKControl[i], eSOSusc20 + i);
		}
		else{
		}
	}
	// 相模原局管制
	for (i = 0; i < mMaxType; i++){
		if (ac.ReadSSOCControl(mSControl[i], eSSOCusc20 + i) == false){
			// ファイルが存在しないのでデフォルトを設定する
			mSControl[i].flag = 0;
			memcpy((char*)mSControl[i].password, (char*)(const char*)strPass, mMaxPassword);
			sprintf_s(mSControl[i].lifetime, mMaxLifetime, "0");
			ac.WriteSSOCControl(mSControl[i], eSSOCusc20 + i);
		}
		else{
		}
	}
	UpdateData(FALSE);

	EnableControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CChangeControlDlg::EnableControl()
{
	GetDlgItem(IDC_EDIT1)->EnableWindow((m_Status == 0) ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow((m_Status == 2) ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow((m_Status == 4) ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT4)->EnableWindow((m_Status == 6) ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT5)->EnableWindow((m_Status == 8) ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT6)->EnableWindow((m_Status == 10) ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT7)->EnableWindow((m_Status == 12) ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT8)->EnableWindow((m_Status == 14) ? TRUE : FALSE);
}

void CChangeControlDlg::OnBnClickedRadio1()
{
	UpdateData(TRUE);
	//mKControl[0].flag = 1;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio2()
{
	UpdateData(TRUE);
	//mKControl[0].flag = 0;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio3()
{
	UpdateData(TRUE);
	//mKControl[1].flag = 1;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio4()
{
	UpdateData(TRUE);
	//mKControl[1].flag = 0;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio5()
{
	UpdateData(TRUE);
	//mKControl[2].flag = 1;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio6()
{
	UpdateData(TRUE);
	//mKControl[2].flag = 0;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio7()
{
	UpdateData(TRUE);
	//mKControl[3].flag = 1;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio8()
{
	UpdateData(TRUE);
	//mKControl[3].flag = 0;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio9()
{
	UpdateData(TRUE);
	//mSControl[0].flag = 1;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio10()
{
	UpdateData(TRUE);
	//mSControl[0].flag = 0;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio11()
{
	UpdateData(TRUE);
	//mSControl[1].flag = 1;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio12()
{
	UpdateData(TRUE);
	//mSControl[1].flag = 0;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio13()
{
	UpdateData(TRUE);
	//mSControl[2].flag = 1;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio14()
{
	UpdateData(TRUE);
	//mSControl[2].flag = 0;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio15()
{
	UpdateData(TRUE);
	//mSControl[3].flag = 1;
	EnableControl();
}
void CChangeControlDlg::OnBnClickedRadio16()
{
	UpdateData(TRUE);
	//mSControl[3].flag = 0;
	EnableControl();
}


void CChangeControlDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	CFileAccess& ac = theApp.GetFileAccess();
	int i = 0;

	SYSTEMTIME st_UTC;
	GetSystemTime(&st_UTC);
	CTime time = CTime(st_UTC);
	__int64 curTime = time.GetTime();

	if (m_Status < 8){
		// 地上局
		if (m_Status < 2){
			if (m_Status == 0){
				mKControl[0].flag = 1;
				sprintf_s(mKControl[0].lifetime, mMaxLifetime, "%d", (m_Timer[0] * 60));
			}
			else{
				mKControl[0].flag = 0;
				sprintf_s(mKControl[0].lifetime, mMaxLifetime, "%d", 0);
			}
			ac.WriteSOSControl(mKControl[0], eSOSusc20 + 0);
		}
		else if (m_Status < 4){
			if (m_Status == 2){
				mKControl[1].flag = 1;
				sprintf_s(mKControl[1].lifetime, mMaxLifetime, "%d", (m_Timer[1] * 60));
			}
			else{
				mKControl[1].flag = 0;
				sprintf_s(mKControl[1].lifetime, mMaxLifetime, "%d", 0);
			}
			ac.WriteSOSControl(mKControl[1], eSOSusc20 + 1);
		}
		else if (m_Status < 6){
			if (m_Status == 4){
				mKControl[2].flag = 1;
				sprintf_s(mKControl[2].lifetime, mMaxLifetime, "%d", (m_Timer[2] * 60));
			}
			else{
				mKControl[2].flag = 0;
				sprintf_s(mKControl[2].lifetime, mMaxLifetime, "%d", 0);
			}
			ac.WriteSOSControl(mKControl[2], eSOSusc20 + 2);
		}
		else if (m_Status < 8){
			if (m_Status == 6){
				mKControl[3].flag = 1;
				sprintf_s(mKControl[3].lifetime, mMaxLifetime, "%d", (m_Timer[3] * 60));
			}
			else{
				mKControl[3].flag = 0;
				sprintf_s(mKControl[3].lifetime, mMaxLifetime, "%d", 0);
			}
			ac.WriteSOSControl(mKControl[3], eSOSusc20 + 3);
		}
	}
	else{
		// 相模原局管制
		if (m_Status < 10){
			if (m_Status == 8){
				mSControl[0].flag = 1;
				sprintf_s(mSControl[0].lifetime, mMaxLifetime, "%d", (m_Timer[4] * 60));
			}
			else{
				mSControl[0].flag = 0;
				sprintf_s(mSControl[0].lifetime, mMaxLifetime, "%d", 0);
			}
			ac.WriteSSOCControl(mSControl[0], eSSOCusc20 + 0);
		}
		else if (m_Status < 12){
			if (m_Status == 10){
				mSControl[1].flag = 1;
				sprintf_s(mSControl[1].lifetime, mMaxLifetime, "%d", (m_Timer[5] * 60));
			}
			else{
				mSControl[1].flag = 0;
				sprintf_s(mSControl[1].lifetime, mMaxLifetime, "%d", 0);
			}
			ac.WriteSSOCControl(mSControl[1], eSSOCusc20 + 1);
		}
		else if (m_Status < 14){
			if (m_Status == 12){
				mSControl[2].flag = 1;
				sprintf_s(mSControl[2].lifetime, mMaxLifetime, "%d", (m_Timer[6] * 60));
			}
			else{
				mSControl[2].flag = 0;
				sprintf_s(mSControl[2].lifetime, mMaxLifetime, "%d", 0);
			}
			ac.WriteSSOCControl(mSControl[2], eSSOCusc20 + 2);
		}
		else if (m_Status < 16){
			if (m_Status == 14){
				mSControl[3].flag = 1;
				sprintf_s(mSControl[3].lifetime, mMaxLifetime, "%d", (m_Timer[7] * 60));
			}
			else{
				mSControl[3].flag = 0;
				sprintf_s(mSControl[3].lifetime, mMaxLifetime, "%d", 0);
			}
			ac.WriteSSOCControl(mSControl[3], eSSOCusc20 + 3);
		}
	}

	CDialogEx::OnOK();
}


void CChangeControlDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
