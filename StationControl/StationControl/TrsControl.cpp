/*============================================================================*/
/*! TrsControl.cpp

-# 送信制御選択画面
*/
/*============================================================================*/
// TrsControl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrsControl.h"
#include "afxdialogex.h"



// CTrsControl ダイアログ

IMPLEMENT_DYNAMIC(CTrsControl, CDialogBase)

/*============================================================================*/
/*! CTrsControl

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CTrsControl::CTrsControl(CWnd* pParent /*=NULL*/)
: CDialogBase(CTrsControl::IDD, pParent)
	, m_BandRadio(0)
{

}

/*============================================================================*/
/*! CTrsControl

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CTrsControl::~CTrsControl()
{
}

void CTrsControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TRSCTRL_S_RDO, m_BandRadio);
}


BEGIN_MESSAGE_MAP(CTrsControl, CDialogBase)
	ON_BN_CLICKED(IDC_TRSCTRL_VIEW_BTN, &CTrsControl::OnBnClickedTrsctrlViewBtn)
END_MESSAGE_MAP()


// CTrsControl メッセージ ハンドラー


/*============================================================================*/
/*! CTrsControl

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CTrsControl::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CTrsControl::UpdateDlg()
{
	if (m_SecelctType == TYPE_34)
	{
		GetDlgItem(IDC_TRSCTRL_X_RDO)->EnableWindow(FALSE);
	}

	CenterWindowEx();
	return TRUE;
}

/*============================================================================*/
/*! CTrsControl

-# 表示ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CTrsControl::OnBnClickedTrsctrlViewBtn()
{
	UpdateData();
	int rdo = m_BandRadio;

	if (rdo == 0)
	{
		UINT sta = theApp.GetSelectStation();

		if (sta == eStation_Usuda64)
		{
			CTrsControlS64* pDlg = (CTrsControlS64*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S64);
			if (pDlg != NULL){
				pDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				pDlg = new CTrsControlS64();
				if (pDlg != NULL)
				{
					if (pDlg->Create(IDD_DLG_TRSCTRL_S64, this))
					{
						pDlg->ShowWindow(SW_SHOW);
						theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S64);
					}
				}
			}
		}
		else if(sta == eStation_Usuda642)
		{
			CTrsControlS34* pDlg = (CTrsControlS34*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S34);
			if (pDlg != NULL){
				pDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				pDlg = new CTrsControlS34();
				if (pDlg != NULL)
				{
					if (pDlg->Create(IDD_DLG_TRSCTRL_S34, this))
					{
						pDlg->ShowWindow(SW_SHOW);
						theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S34);
					}
				}
			}
		}
		else if(sta == eStation_Uchinoura34)
		{
			CTrsControlS34* pDlg = (CTrsControlS34*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S34);
			if (pDlg != NULL){
				pDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				pDlg = new CTrsControlS34();
				if (pDlg != NULL)
				{
					if (pDlg->Create(IDD_DLG_TRSCTRL_S34, this))
					{
						pDlg->ShowWindow(SW_SHOW);
						theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_S34);
					}
				}
			}
		}
		else if (sta == eStation_Uchinoura20)
		{
			CTrsControlS20* pDlg20 = (CTrsControlS20*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S20);
			if (pDlg20 != NULL){
				pDlg20->ShowWindow(SW_SHOW);
			}
			else
			{
				pDlg20 = new CTrsControlS20();
				if (pDlg20 != NULL)
				{
					if (pDlg20->Create(IDD_DLG_TRSCTRL_S20, this))
					{
						pDlg20->ShowWindow(SW_SHOW);
						theApp.AddWindowHandle((LPARAM)pDlg20, IDD_DLG_TRSCTRL_S20);
					}
				}
			}
		}
	


#if 0
		CTrsControlS34* pDlg34 = (CTrsControlS34*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S34);
		if (pDlg34 != NULL){
			pDlg34->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg34 = new CTrsControlS34();
			if (pDlg34 != NULL)
			{
				if (pDlg34->Create(IDD_DLG_TRSCTRL_S34, this))
				{
					pDlg34->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg34, IDD_DLG_TRSCTRL_S34);
				}
			}
		}


		CTrsControlS64* pDlg64 = (CTrsControlS64*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_S64);
		if (pDlg64 != NULL){
			pDlg64->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg64 = new CTrsControlS64();
			if (pDlg64 != NULL)
			{
				if (pDlg64->Create(IDD_DLG_TRSCTRL_S64, this))
				{
					pDlg64->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg64, IDD_DLG_TRSCTRL_S64);
				}
			}
		}
#endif

	}
	else if (rdo == 1)
	{
		CTrsControlX* pDlg = (CTrsControlX*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_X);
		if (pDlg != NULL){
			pDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg = new CTrsControlX();
			if (pDlg != NULL)
			{
				if (pDlg->Create(IDD_DLG_TRSCTRL_X, this))
				{
					pDlg->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_X);
				}
			}
		}
	}
	else if (rdo == 2)
	{
		CTrsControlXX* pDlg = (CTrsControlXX*)theApp.GetWindowHandle(IDD_DLG_TRSCTRL_XX);
		if (pDlg != NULL){
			pDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg = new CTrsControlXX();
			if (pDlg != NULL)
			{
				if (pDlg->Create(IDD_DLG_TRSCTRL_XX, this))
				{
					pDlg->ShowWindow(SW_SHOW);
					theApp.AddWindowHandle((LPARAM)pDlg, IDD_DLG_TRSCTRL_XX);
				}
			}
		}
	}

	OnOK();
}


