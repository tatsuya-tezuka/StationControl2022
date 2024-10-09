// CMonSearchDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "MonSearchDlg.h"
#include "afxdialogex.h"


// CMonSearchDlg ダイアログ

IMPLEMENT_DYNAMIC(CMonSearchDlg, CDialogEx)

CMonSearchDlg::CMonSearchDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMonSearchDlg::IDD, pParent)
, m_strMonName(_T(""))
{

}

CMonSearchDlg::~CMonSearchDlg()
{
}

void CMonSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MONNAME_EDIT, m_strMonName);
	DDX_Control(pDX, IDC_MONNAME_LIST, m_MonNameList);
}


BEGIN_MESSAGE_MAP(CMonSearchDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MONNAME_EDIT, &CMonSearchDlg::OnEnChangeMonnameEdit)
	ON_LBN_DBLCLK(IDC_MONNAME_LIST, &CMonSearchDlg::OnLbnDblclkMonnameList)
	ON_BN_CLICKED(IDOK, &CMonSearchDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMonSerachDlg メッセージ ハンドラー


/*============================================================================*/
/*! CMonSearchDlg

-# ダイアログ初期化処理

@param  なし
@retval 入力フォーカス設定
*/
/*============================================================================*/
BOOL CMonSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>& mList = dba.GetMonNameList();
	int cnt = (int)mList.size();
	for (int i = 0; i < cnt; ++i)
	{
		m_MonNameList.InsertString(-1, mList[i]);
	}

	ResetList(m_strMonName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CMonSearchDlg

-# エディット入力処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CMonSearchDlg::OnEnChangeMonnameEdit()
{
	UpdateData();
//	TRACE("%s\n", CStringA(m_strMonName));
	ResetList(m_strMonName);
}

/*============================================================================*/
/*! CMonSearchDlg

-# リスト更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CMonSearchDlg::ResetList(CString name)
{
	m_MonNameList.SetRedraw(FALSE);
	m_MonNameList.ResetContent();

	CSatelliteData& sd = theApp.GetSatelliteData();
	CDBAccess& dba = sd.GetDBAccessCls();

	vector<CString>& mList = dba.GetMonNameList();
	int cnt = (int)mList.size();
	CString str;
	name.MakeLower();

	if (name.Find(_T("*")) < 0){
		for (int i = 0; i < cnt; ++i)
		{
			if (name.IsEmpty() == false){
				str = mList[i];
				str.MakeLower();
				if (str.Find(name) == 0){
					m_MonNameList.InsertString(-1, mList[i]);
				}
			}
			else{
				m_MonNameList.InsertString(-1, mList[i]);
			}
		}
	}
	else
	{
		// ワイルドカード検索
		for (int i = 0; i < cnt; ++i)
		{
			if (name.IsEmpty() == true){
				m_MonNameList.InsertString(-1, mList[i]);
				continue;
			}

			name.MakeLower();
			TCHAR* pwild = (TCHAR*)(const TCHAR*)name;

			CString str = mList[i];
			str.MakeLower();
			TCHAR* pstr = (TCHAR*)(const TCHAR*)str;

			if (WildCard(pwild, pstr) == true){
				// 見つかった
				m_MonNameList.InsertString(-1, mList[i]);
			}
		}
	}

	m_MonNameList.SetRedraw(TRUE);
}

/*============================================================================*/
/*! CMonSearchDlg

-# リスト更新処理（ワイルドカード検索）

@param  なし
@retval なし
*/
/*============================================================================*/
bool CMonSearchDlg::WildCard(TCHAR* pwild, TCHAR* pstr)
{
	enum State {
		Exact,
		AnyRepeat,
	};

	const TCHAR *s = pstr;
	const TCHAR *w = pwild;
	const TCHAR *q = 0;
	int state = 0;

	bool match = true;
	while (match && *w)
	{
		if (*w == '*')
		{
			state = AnyRepeat;
			q = w + 1;
		}
		else
			state = Exact;

		if (*s == 0)
			break;

		switch (state)
		{
		case Exact:
			match = *s == *w;
			s++;
			w++;
			break;

		case AnyRepeat:
			match = true;
			s++;

			if (*s == *q)
				w++;
			break;
		}
	}

	if (state == AnyRepeat)
		return (*s == *q);
	else
		return match && (*s == *w);
}


/*============================================================================*/
/*! CMonSearchDlg

-# リスト更新処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CMonSearchDlg::OnLbnDblclkMonnameList()
{
#if 1
	return;
#else
	int nIdx = m_MonNameList.GetCurSel();
	if (nIdx == LB_ERR)
		return;

	CString str;
	m_MonNameList.GetText(nIdx, str);

	m_strMonName = str;

	UpdateData(FALSE);

	ResetList(m_strMonName);
#endif
}


/*============================================================================*/
/*! CMonSearchDlg

-# 実行ボタン処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CMonSearchDlg::OnBnClickedOk()
{
	int nIdx = m_MonNameList.GetCurSel();
	if (nIdx == LB_ERR)
		return;

	CString str;
	m_MonNameList.GetText(nIdx, str);

	m_MonNameSearch = str;

	CDialogEx::OnOK();
}
