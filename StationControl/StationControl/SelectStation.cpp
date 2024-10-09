// SelectStation.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SelectStation.h"
#include "afxdialogex.h"


// CSelectStation ダイアログ

IMPLEMENT_DYNAMIC(CSelectStation, CDialogEx)

CSelectStation::CSelectStation(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectStation::IDD, pParent)
	, m_nStation(0)
	, m_nStationBefore(0)
	, m_bFixedStation(false)
{
	mEnableStation = 0x0f;
}

CSelectStation::~CSelectStation()
{
}

void CSelectStation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nStation);
	// ★表示インデックスを内之浦20m, 内之浦34m, 臼田64m, 臼田54mの順にする
	DDX_Control(pDX, IDC_RADIO1, m_btStation[3]);
	DDX_Control(pDX, IDC_RADIO2, m_btStation[2]);
	DDX_Control(pDX, IDC_RADIO3, m_btStation[0]);
	DDX_Control(pDX, IDC_RADIO4, m_btStation[1]);
}


BEGIN_MESSAGE_MAP(CSelectStation, CDialogEx)
END_MESSAGE_MAP()


// CSelectStation メッセージ ハンドラー


BOOL CSelectStation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// システムコマンドSC_CLOSEの使用禁止
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu)
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
		m_btStation[i].SetWindowText(mStationString[i]);
	}

	BYTE	byteFixedStation;
	if (m_bFixedStation)
	{
		// 局固定
		byteFixedStation = 1 << m_nStationBefore;
	}
	else
	{
		// 局非固定
		byteFixedStation = 0x0f;
	}

	// 局番を表示順へ変換
	m_nStation = mStationViewCnv[m_nStationBefore];

	// 局選択の活性非活性
	bool bEnableStation = false;
	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
		if (byteFixedStation & mEnableStation & (1 << i)){
			bEnableStation = true;
		}
		else{
			m_btStation[i].EnableWindow(FALSE);
		}
	}

	// 局非固定時に局選択ダイアログの上の局から、選択可能なデフォルト局を検索する
	if (m_bFixedStation == false){
		BYTE chgOdrEnaStation = ChgOrderEnebleStation(mEnableStation);
		for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
			if (chgOdrEnaStation & (1 << i)){
			}
			else{
				if (m_nStation == i)
					m_nStation++;
			}
		}
	}

	if (bEnableStation == false)
	{
		// 選択可能な局がない場合
		EndDialog(IDABORT);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

