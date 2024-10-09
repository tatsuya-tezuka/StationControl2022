/*============================================================================*/
/*! BusinessDiaryEdit.cpp

-# 無線業務日誌編集画面
*/
/*============================================================================*/
// BusinessDiaryEdit.cpp : 実装ファイル
//
#include "stdafx.h"
#include "StationControl.h"
#include "BusinessDiaryEdit.h"
#include "afxdialogex.h"
#include "BusinessDiary.h"
#include "HookMsgBox.h"


// CBusinessDiaryEdit ダイアログ

IMPLEMENT_DYNAMIC(CBusinessDiaryEdit, CDialogBase)

/*============================================================================*/
/*! CBusinessDiaryEdit

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CBusinessDiaryEdit::CBusinessDiaryEdit(CWnd* pParent /*=NULL*/)
: CDialogBase(CBusinessDiaryEdit::IDD, pParent)
	, m_OperationDate(0)
	, m_strOperationBackNo(_T(""))
	, m_strOperationNextNo(_T(""))
	, m_strStation1(_T(""))
	, m_strComTimeH1(_T(""))
	, m_strComTimeM1(_T(""))
	, m_strComCount1(_T(""))
	, m_strStation2(_T(""))
	, m_strComTimeH2(_T(""))
	, m_strComTimeM2(_T(""))
	, m_strComCount2(_T(""))
	, m_strStation3(_T(""))
	, m_strComTimeH3(_T(""))
	, m_strComTimeM3(_T(""))
	, m_strComCount3(_T(""))
	, m_strCause(_T(""))
	, m_strResult(_T(""))
	, m_strNotices(_T(""))
{
	m_pParent = pParent;
}

/*============================================================================*/
/*! CBusinessDiaryEdit

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CBusinessDiaryEdit::~CBusinessDiaryEdit()
{
}

void CBusinessDiaryEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DIARY_EDIT_OPEDAY_DP, m_OperationDate);
	DDX_Text(pDX, IDC_DIARY_EDIT_BACK_EDIT_EDIT, m_strOperationBackNo);
	DDX_Text(pDX, IDC_DIARY_EDIT_NEXT_EDIT, m_strOperationNextNo);
	DDX_Control(pDX, IDC_DIARY_EDIT_1_WEATHER_CMB, m_Weather1Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_1_CAPA_CMB, m_EmpCapa1Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_1_NAME_CMB, m_EmpName1Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_1_CAPA21_CMB, m_OpeCapa11Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_1_NAME21_CMB, m_OpeName11Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_1_CAPA22_CMB, m_OpeCapa12Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_1_NAME22_CMB, m_OpeName12Cmb);
	DDX_Text(pDX, IDC_DIARY_EDIT_1_STATION_EDIT, m_strStation1);
	DDX_Text(pDX, IDC_DIARY_EDIT_1_HOUR_EDIT, m_strComTimeH1);
	DDX_Text(pDX, IDC_DIARY_EDIT_1_MINUTE_EDIT, m_strComTimeM1);
	DDX_Text(pDX, IDC_DIARY_EDIT_1_COUNT_EDIT, m_strComCount1);
	DDX_Control(pDX, IDC_DIARY_EDIT_2_WEATHER_CMB, m_Weather2Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_2_CAPA_CMB, m_EmpCapa2Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_2_NAME_CMB, m_EmpName2Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_2_CAPA21_CMB, m_OpeCapa21Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_2_NAME21_CMB, m_OpeName21Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_2_CAPA22_CMB, m_OpeCapa22Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_2_NAME22_CMB, m_OpeName22Cmb);
	DDX_Text(pDX, IDC_DIARY_EDIT_2_STATION_EDIT, m_strStation2);
	DDX_Text(pDX, IDC_DIARY_EDIT_2_HOUR_EDIT, m_strComTimeH2);
	DDX_Text(pDX, IDC_DIARY_EDIT_2_MINUTE_EDIT, m_strComTimeM2);
	DDX_Text(pDX, IDC_DIARY_EDIT_2_COUNT_EDIT, m_strComCount2);
	DDX_Control(pDX, IDC_DIARY_EDIT_3_WEATHER_CMB, m_Weather3Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_3_CAPA_CMB, m_EmpCapa3Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_3_NAME_CMB, m_EmpName3Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_3_CAPA21_CMB, m_OpeCapa31Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_3_NAME21_CMB, m_OpeName31Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_3_CAPA22_CMB, m_OpeCapa32Cmb);
	DDX_Control(pDX, IDC_DIARY_EDIT_3_NAME22_CMB, m_OpeName32Cmb);
	DDX_Text(pDX, IDC_DIARY_EDIT_3_STATION_EDIT, m_strStation3);
	DDX_Text(pDX, IDC_DIARY_EDIT_3_HOUR_EDIT, m_strComTimeH3);
	DDX_Text(pDX, IDC_DIARY_EDIT_3_MINUTE_EDIT, m_strComTimeM3);
	DDX_Text(pDX, IDC_DIARY_EDIT_3_COUNT_EDIT, m_strComCount3);
	DDX_Text(pDX, IDC_DIARY_EDIT_CAUSE_EDIT, m_strCause);
	DDX_Text(pDX, IDC_DIARY_EDIT_RESULT_EDIT, m_strResult);
	DDX_Text(pDX, IDC_DIARY_EDIT_NOTICES_EDIT, m_strNotices);
	DDX_Control(pDX, IDC_DIARY_EDIT_OPEDAY_DP, mDate);
	DDX_Control(pDX, IDC_DIARY_EDIT_NEXT_EDIT, m_NextEditCtrl);
}


BEGIN_MESSAGE_MAP(CBusinessDiaryEdit, CDialogBase)
	ON_BN_CLICKED(IDC_DIARY_EDIT_NOTICES_ENTRY_BTN, &CBusinessDiaryEdit::OnBnClickedDiaryEditNoticesEntryBtn)
	ON_BN_CLICKED(IDC_DIARY_EDIT_BACK_BTN, &CBusinessDiaryEdit::OnBnClickedDiaryEditBackBtn)
	ON_BN_CLICKED(IDC_DIARY_EDIT_NEXT_BTN, &CBusinessDiaryEdit::OnBnClickedDiaryEditNextBtn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DIARY_EDIT_OPEDAY_DP, &CBusinessDiaryEdit::OnDtnDatetimechangeDiaryEditOpedayDp)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_1_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1WeatherCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_1_CAPA_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1CapaCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_1_NAME_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1NameCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_1_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Capa21Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_1_NAME21_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Name21Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_1_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Capa22Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_1_NAME22_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Name22Cmb)
	ON_EN_CHANGE(IDC_DIARY_EDIT_1_STATION_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1StationEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_1_HOUR_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1HourEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_1_MINUTE_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1MinuteEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_1_COUNT_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1CountEdit)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_2_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit2WeatherCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_2_CAPA_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1CapaCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_2_NAME_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1NameCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_2_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Capa21Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_2_NAME21_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Name21Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_2_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Capa22Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_2_NAME22_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Name22Cmb)
	ON_EN_CHANGE(IDC_DIARY_EDIT_2_STATION_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1StationEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_2_HOUR_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1HourEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_2_MINUTE_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1MinuteEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_2_COUNT_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1CountEdit)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_3_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit3WeatherCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_3_CAPA_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1CapaCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_3_NAME_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1NameCmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_3_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Capa21Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_3_NAME21_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Name21Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_3_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Capa22Cmb)
	ON_CBN_SELCHANGE(IDC_DIARY_EDIT_3_NAME22_CMB, &CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Name22Cmb)
	ON_EN_CHANGE(IDC_DIARY_EDIT_3_STATION_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1StationEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_3_HOUR_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1HourEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_3_MINUTE_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1MinuteEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_3_COUNT_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEdit1CountEdit)
	ON_BN_CLICKED(IDOK, &CBusinessDiaryEdit::OnBnClickedOk)
	ON_EN_CHANGE(IDC_DIARY_EDIT_CAUSE_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEditCauseEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_RESULT_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEditResultEdit)
	ON_EN_CHANGE(IDC_DIARY_EDIT_NOTICES_EDIT, &CBusinessDiaryEdit::OnEnChangeDiaryEditNoticesEdit)
	ON_COMMAND(ID_MENU_CLOSE_CLOSE, &CBusinessDiaryEdit::OnMenuCloseClose)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_1_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1WeatherCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_1_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1WeatherCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_1_CAPA_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1CapaCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_1_CAPA_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1CapaCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_1_NAME_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1NameCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_1_NAME_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1NameCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_1_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1Capa21Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_1_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1Capa21Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_1_NAME21_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1Name21Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_1_NAME21_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1Name21Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_1_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1Capa22Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_1_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1Capa22Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_1_NAME22_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1Name22Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_1_NAME22_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1Name22Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_2_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2WeatherCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_2_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2WeatherCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_2_CAPA_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2CapaCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_2_CAPA_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2CapaCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_2_NAME_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2NameCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_2_NAME_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2NameCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_2_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2Capa21Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_2_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2Capa21Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_2_NAME21_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2Name21Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_2_NAME21_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2Name21Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_2_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2Capa22Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_2_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2Capa22Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_2_NAME22_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2Name22Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_2_NAME22_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2Name22Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_3_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3WeatherCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_3_WEATHER_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3WeatherCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_3_CAPA_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3CapaCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_3_CAPA_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3CapaCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_3_NAME_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3NameCmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_3_NAME_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3NameCmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_3_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3Capa21Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_3_CAPA21_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3Capa21Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_3_NAME21_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3Name21Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_3_NAME21_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3Name21Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_3_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3Capa22Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_3_CAPA22_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3Capa22Cmb)
	ON_CBN_EDITCHANGE(IDC_DIARY_EDIT_3_NAME22_CMB, &CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3Name22Cmb)
	ON_CBN_KILLFOCUS(IDC_DIARY_EDIT_3_NAME22_CMB, &CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3Name22Cmb)
END_MESSAGE_MAP()


// CBusinessDiaryEdit メッセージ ハンドラー


/*============================================================================*/
/*! CBusinessDiaryEdit

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CBusinessDiaryEdit::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CenterWindowEx();
//	ReadDiaryData();
	CreateControl();
	DisplayData();
	mUpdate = false;
	mWriteData = false;

	// 日時フォーマット変更
	mDate.SetFormat(_T("yyyy-MM-dd"));
	
	m_NextEditCtrl.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 登録ボタン 押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnBnClickedDiaryEditNoticesEntryBtn()
{
	if (mUpdate == false)
		return;

	UpdateData(TRUE);

	vector<stDiary> &DiaryData = ((CBusinessDiary*)m_pParent)->GetDialyDataList();
	int idx = ((CBusinessDiary*)m_pParent)->GetDiaryListIndex(mListPos);

	CString str, strEntsuTime1, strEntsuTime2, strEntsuTime3;
	BOOL bDelFlg[3] = { 0 };
	int i = 0;
	int h, m;

	// 時間帯1
	// 延通時間と回数が削除対象か確認
	h = _wtoi(m_strComTimeH1);
	m = _wtoi(m_strComTimeM1);
	strEntsuTime1.Format(_T("%d"), h * 60 + m);
	if (strEntsuTime1 == _T("0") && m_strComCount1 == _T("0"))
	{
		// 時間と回数が0ならば時間帯1は削除対象
		bDelFlg[0] = TRUE;
	}

	// 時間帯2
	// 延通時間と回数が削除対象か確認
	h = _wtoi(m_strComTimeH2);
	m = _wtoi(m_strComTimeM2);
	strEntsuTime2.Format(_T("%d"), h * 60 + m);
	if (strEntsuTime2 == _T("0") && m_strComCount2 == _T("0"))
	{
		// 時間と回数が0ならば時間帯2は削除対象
		bDelFlg[1] = TRUE;
	}

	// 時間帯3
	// 延通時間と回数が削除対象か確認
	h = _wtoi(m_strComTimeH3);
	m = _wtoi(m_strComTimeM3);
	strEntsuTime3.Format(_T("%d"), h * 60 + m);
	if (strEntsuTime3 == _T("0") && m_strComCount3 == _T("0"))
	{
		// 時間と回数が0ならば時間帯3は削除対象
		bDelFlg[2] = TRUE;
	}

	// 3つの時間帯の削除フラグ確認
	if (bDelFlg[0] == TRUE && bDelFlg[1] == TRUE && bDelFlg[2] == TRUE)
	{
		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("項目削除しますか？"), _T("無線業務日誌削除確認"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			// 削除キャンセルのため、時間と回数を元に戻す
			UINT time;
			time = _wtoi(DiaryData[idx].data[0].time);
			m_strComTimeH1.Format(_T("%d"), time / 60);
			m_strComTimeM1.Format(_T("%d"), time % 60);
			m_strComCount1 = DiaryData[idx].data[0].count;

			time = _wtoi(DiaryData[idx].data[1].time);
			m_strComTimeH2.Format(_T("%d"), time / 60);
			m_strComTimeM2.Format(_T("%d"), time % 60);
			m_strComCount2 = DiaryData[idx].data[1].count;

			time = _wtoi(DiaryData[idx].data[2].time);
			m_strComTimeH3.Format(_T("%d"), time / 60);
			m_strComTimeM3.Format(_T("%d"), time % 60);
			m_strComCount3 = DiaryData[idx].data[2].count;

			UpdateData(FALSE);

			return;
		}

		// 削除のため、削除マークセット
		DiaryData[idx].no = _T("-");
		CString strCnt = ((CBusinessDiary*)m_pParent)->GetRecCountStr();
		int cnt = _wtoi(strCnt) - 1;
		strCnt.Format(_T("%d"), cnt);
		((CBusinessDiary*)m_pParent)->SetRecCountStr(strCnt);
	}

	// コントローラの設定値取得
	stMember	src, dst;
	// 時間帯1
	{
		// 天候
		m_Weather1Cmb.GetWindowTextW(src.weather);
		// 資格1
		m_EmpCapa1Cmb.GetWindowTextW(src.capa1);
		// 氏名1
		m_EmpName1Cmb.GetWindowTextW(src.name1);
		// 資格2
		m_OpeCapa11Cmb.GetWindowTextW(src.capa2);
		// 氏名2
		m_OpeName11Cmb.GetWindowTextW(src.name2);
		// 資格3
		m_OpeCapa12Cmb.GetWindowTextW(src.capa3);
		// 氏名3
		m_OpeName12Cmb.GetWindowTextW(src.name3);
		// 相手局
		src.station = m_strStation1;
		// 延通時間
		src.time = strEntsuTime1;
		// 通信回数
		src.count = m_strComCount1;
		// 各項のデータ無しまたは'-'連続文字列を'-'文字に変換する
		((CBusinessDiary*)m_pParent)->ExchgNoneData2Short(src, DiaryData[idx].data[i]);
	}

	// 時間帯2
	i = 1;
	{
		// 天候
		m_Weather2Cmb.GetWindowTextW(src.weather);
		// 資格1
		m_EmpCapa2Cmb.GetWindowTextW(src.capa1);
		// 氏名1
		m_EmpName2Cmb.GetWindowTextW(src.name1);
		// 資格2
		m_OpeCapa21Cmb.GetWindowTextW(src.capa2);
		// 氏名2
		m_OpeName21Cmb.GetWindowTextW(src.name2);
		// 資格3
		m_OpeCapa22Cmb.GetWindowTextW(src.capa3);
		// 氏名3
		m_OpeName22Cmb.GetWindowTextW(src.name3);
		// 相手局
		src.station = m_strStation2;
		// 延通時間
		src.time = strEntsuTime2;
		// 通信回数
		src.count = m_strComCount2;
		// 各項のデータ無しまたは'-'連続文字列を'-'文字に変換する
		((CBusinessDiary*)m_pParent)->ExchgNoneData2Short(src, DiaryData[idx].data[i]);
	}

	// 時間帯3
	i = 2;
	{
		// 天候
		m_Weather3Cmb.GetWindowTextW(src.weather);
		// 資格1
		m_EmpCapa3Cmb.GetWindowTextW(src.capa1);
		// 氏名1
		m_EmpName3Cmb.GetWindowTextW(src.name1);
		// 資格2
		m_OpeCapa31Cmb.GetWindowTextW(src.capa2);
		// 氏名2
		m_OpeName31Cmb.GetWindowTextW(src.name2);
		// 資格3
		m_OpeCapa32Cmb.GetWindowTextW(src.capa3);
		// 氏名3
		m_OpeName32Cmb.GetWindowTextW(src.name3);
		// 相手局
		src.station = m_strStation3;
		// 延通時間
		src.time = strEntsuTime3;
		// 通信回数
		src.count = m_strComCount3;
		// 各項のデータ無しまたは'-'連続文字列を'-'文字に変換する
		((CBusinessDiary*)m_pParent)->ExchgNoneData2Short(src, DiaryData[idx].data[i]);
	}

	// 空電、通信、受信感度減衰機器の故障と原因、処置
	CString temp;
	temp = m_strCause;
	temp.Replace(_T("\r\n"), _T("\\"));
	if (temp == _T(""))
	{
		temp = _T("-");
	}
	DiaryData[idx].info1 = temp;

	// 実験方法、経過及び結果等
	temp = m_strResult;
	temp.Replace(_T("\r\n"), _T("\\"));
	if (temp == _T(""))
	{
		temp = _T("-");
	}
	DiaryData[idx].info2 = temp;

	// 特記事項、参考情報
	temp = m_strNotices;
	temp.Replace(_T("\r\n"), _T("\\"));
	if (temp == _T(""))
	{
		temp = _T("-");
	}
	DiaryData[idx].info3 = temp;

	mWriteData = true;
	mUpdate = false;

	CWaitCursor wait;
	WriteDiaryData();

	// 無線業務日誌登録要求
	((CBusinessDiary*)m_pParent)->Shell_RegistRadiolog();

//	// 変更フラグセット
//	((CBusinessDiary*)m_pParent)->SetEditFlg(TRUE);

	// 無線業務日誌更新
	((CBusinessDiary*)m_pParent)->CreateDialyList();
	int nCount = ((CBusinessDiary*)m_pParent)->m_DiaryList.GetItemCount();
	if (nCount == 0)
	{
		// 無線業務日誌レコードが全部削除されたため編集画面を閉じる
		OnOK();
	}
	else
	{
		CString strLastNo = ((CBusinessDiary*)m_pParent)->m_DiaryList.GetItemText(nCount - 1, 0);
		int nLastNo = atoi(CStringA(strLastNo));
		if (mListPos > nLastNo)
		{
			// 無線業務日誌一覧の末端を削除したので番号を更新する
			mListPos = nLastNo;
			DisplayData();
			((CBusinessDiary*)m_pParent)->SelectListRow(mListPos);
		}
		else
		{
			// 番号は同じで再表示
			DisplayData();
			((CBusinessDiary*)m_pParent)->SelectListRow(mListPos);
		}
	}
}

void CBusinessDiaryEdit::ReadDiaryData()
{
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;
	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, nStation);

	FILE *fp = NULL;

	mDataList.clear();
	mPos = 0;
	char szDmy1[16] = { 0 };
	char szDmy2[16] = { 0 };
	char szDate[16] = { 0 };
	char szTime[16] = { 0 };
	char szCnt[16] = { 0 };
	char szDmy3[16] = { 0 };
	char szDmy4[16] = { 0 };

	TRY
	{
		// 日誌ファイルのオープン
		if ((_wfopen_s(&fp, filename, _T("r")) != 0) || (fp == NULL))
		{
			return;
		}
		char szline[2048];
		// ヘッダー読み飛ばし
		fgets(szline, static_cast<unsigned>(_countof(szline)), fp);
		sscanf_s(szline, "%s %s %s %s %s %s %s\n"
			, szDmy1, static_cast<unsigned>(_countof(szDmy1))
			, szDmy2, static_cast<unsigned>(_countof(szDmy2))
			, szDate, static_cast<unsigned>(_countof(szDate))
			, szTime, static_cast<unsigned>(_countof(szTime))
			, szCnt, static_cast<unsigned>(_countof(szCnt))
			, szDmy3, static_cast<unsigned>(_countof(szDmy3))
			, szDmy4, static_cast<unsigned>(_countof(szDmy4)));

		mRecCount = szCnt;
#if 0
		CString str, cbuf;
		BOOL bRead = TRUE;
		while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL){
			sscanf_s(szline, "%s\n", szline, sizeof(szline));
			cbuf = CServerFile::ConvertUnicode(szline);
			mDataList.push_back(cbuf);
		}
#endif
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);
}

void CBusinessDiaryEdit::OnBnClickedOk()
{
//#if 0
//	if (mWriteData == true){
//		WriteDiaryData();
//	}
//#endif
	EndDialog(IDOK);
}

void CBusinessDiaryEdit::WriteDiaryData()
{
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;
	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, nStation);

	FILE *fp = NULL;

	SYSTEMTIME time;
	GetSystemTime(&time);
	CTime ctime(time);
	CString temp = ctime.Format("%Y%m%d%H%M%S");
	CString backfile;
	backfile.Format(_T("%s.%s"), (LPCTSTR)filename, (LPCTSTR)temp);
	CopyFile(filename, backfile, FALSE);
	CStringA cbuf;

	vector<stDiary> &DiaryData = ((CBusinessDiary*)m_pParent)->GetDialyDataList();

	TRY
	{
		// 日誌ファイルのオープン
		if ((_wfopen_s(&fp, filename, _T("wb")) != 0) || (fp == NULL))
		{
			return;
		}
		TCHAR szline[2048];
		wsprintf(szline, _T("Dmy1 Dmy2 %s %s Dmy3 Dmy4\n"), (LPCTSTR)CString(((CBusinessDiary*)m_pParent)->m_szMakeDate), (LPCTSTR)((CBusinessDiary*)m_pParent)->GetRecCountStr());
		cbuf = CServerFile::ConvertEuc(szline);
		// ヘッダー書き込み
		fputs(cbuf, fp);

		int count = (int)DiaryData.size();
		int num = 0;
		for (int i = 0; i < count; ++i)
		{
			CString strCnt = DiaryData[i].no;
			if (strCnt != _T("-"))
			{
				num++;
				strCnt.Format(_T("%d"), num);
			}
			wsprintf(szline, _T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n"),
				(LPCTSTR)strCnt,
				(LPCTSTR)DiaryData[i].date,
				(LPCTSTR)DiaryData[i].data[0].weather,
				(LPCTSTR)DiaryData[i].data[0].capa1,
				(LPCTSTR)DiaryData[i].data[0].name1,
				(LPCTSTR)DiaryData[i].data[0].capa2,
				(LPCTSTR)DiaryData[i].data[0].name2,
				(LPCTSTR)DiaryData[i].data[0].capa3,
				(LPCTSTR)DiaryData[i].data[0].name3,
				(LPCTSTR)DiaryData[i].data[0].station,
				(LPCTSTR)DiaryData[i].data[0].time,
				(LPCTSTR)DiaryData[i].data[0].count,
				(LPCTSTR)DiaryData[i].data[1].weather,
				(LPCTSTR)DiaryData[i].data[1].capa1,
				(LPCTSTR)DiaryData[i].data[1].name1,
				(LPCTSTR)DiaryData[i].data[1].capa2,
				(LPCTSTR)DiaryData[i].data[1].name2,
				(LPCTSTR)DiaryData[i].data[1].capa3,
				(LPCTSTR)DiaryData[i].data[1].name3,
				(LPCTSTR)DiaryData[i].data[1].station,
				(LPCTSTR)DiaryData[i].data[1].time,
				(LPCTSTR)DiaryData[i].data[1].count,
				(LPCTSTR)DiaryData[i].data[2].weather,
				(LPCTSTR)DiaryData[i].data[2].capa1,
				(LPCTSTR)DiaryData[i].data[2].name1,
				(LPCTSTR)DiaryData[i].data[2].capa2,
				(LPCTSTR)DiaryData[i].data[2].name2,
				(LPCTSTR)DiaryData[i].data[2].capa3,
				(LPCTSTR)DiaryData[i].data[2].name3,
				(LPCTSTR)DiaryData[i].data[2].station,
				(LPCTSTR)DiaryData[i].data[2].time,
				(LPCTSTR)DiaryData[i].data[2].count,
				(LPCTSTR)DiaryData[i].info1,
				(LPCTSTR)DiaryData[i].info2,
				(LPCTSTR)DiaryData[i].info3);
			cbuf = CServerFile::ConvertEuc(szline);
			fputs(cbuf, fp);
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);
}

void CBusinessDiaryEdit::CreateControl()
{
	m_Weather1Cmb.ResetContent();
	m_Weather2Cmb.ResetContent();
	m_Weather3Cmb.ResetContent();
	m_EmpCapa1Cmb.ResetContent();
	m_EmpCapa2Cmb.ResetContent();
	m_EmpCapa3Cmb.ResetContent();
	m_EmpName1Cmb.ResetContent();
	m_EmpName2Cmb.ResetContent();
	m_EmpName3Cmb.ResetContent();
	m_OpeCapa11Cmb.ResetContent();
	m_OpeCapa12Cmb.ResetContent();
	m_OpeCapa21Cmb.ResetContent();
	m_OpeCapa22Cmb.ResetContent();
	m_OpeCapa31Cmb.ResetContent();
	m_OpeCapa32Cmb.ResetContent();
	m_OpeName11Cmb.ResetContent();
	m_OpeName12Cmb.ResetContent();
	m_OpeName21Cmb.ResetContent();
	m_OpeName22Cmb.ResetContent();
	m_OpeName31Cmb.ResetContent();
	m_OpeName32Cmb.ResetContent();

//#if 0
//	m_Weather1Cmb.AddString(_T("---"));
//	m_Weather2Cmb.AddString(_T("---"));
//	m_Weather3Cmb.AddString(_T("---"));
//	m_EmpCapa1Cmb.AddString(_T("---"));
//	m_EmpCapa2Cmb.AddString(_T("---"));
//	m_EmpCapa3Cmb.AddString(_T("---"));
//	m_OpeCapa11Cmb.AddString(_T("---"));
//	m_OpeCapa12Cmb.AddString(_T("---"));
//	m_OpeCapa21Cmb.AddString(_T("---"));
//	m_OpeCapa22Cmb.AddString(_T("---"));
//	m_OpeCapa31Cmb.AddString(_T("---"));
//	m_OpeCapa32Cmb.AddString(_T("---"));
//	m_EmpName1Cmb.AddString(_T("---"));
//	m_EmpName2Cmb.AddString(_T("---"));
//	m_EmpName3Cmb.AddString(_T("---"));
//	m_OpeName11Cmb.AddString(_T("---"));
//	m_OpeName12Cmb.AddString(_T("---"));
//	m_OpeName21Cmb.AddString(_T("---"));
//	m_OpeName22Cmb.AddString(_T("---"));
//	m_OpeName31Cmb.AddString(_T("---"));
//	m_OpeName32Cmb.AddString(_T("---"));
//#else
	m_Weather1Cmb.AddString(_T("---"));
	m_Weather2Cmb.AddString(_T("---"));
	m_Weather3Cmb.AddString(_T("---"));
	m_EmpCapa1Cmb.AddString(_T("------"));
	m_EmpCapa2Cmb.AddString(_T("------"));
	m_EmpCapa3Cmb.AddString(_T("------"));
	m_OpeCapa11Cmb.AddString(_T("------"));
	m_OpeCapa12Cmb.AddString(_T("------"));
	m_OpeCapa21Cmb.AddString(_T("------"));
	m_OpeCapa22Cmb.AddString(_T("------"));
	m_OpeCapa31Cmb.AddString(_T("------"));
	m_OpeCapa32Cmb.AddString(_T("------"));
	m_EmpName1Cmb.AddString(_T("--------------"));
	m_EmpName2Cmb.AddString(_T("--------------"));
	m_EmpName3Cmb.AddString(_T("--------------"));
	m_OpeName11Cmb.AddString(_T("--------------"));
	m_OpeName12Cmb.AddString(_T("--------------"));
	m_OpeName21Cmb.AddString(_T("--------------"));
	m_OpeName22Cmb.AddString(_T("--------------"));
	m_OpeName31Cmb.AddString(_T("--------------"));
	m_OpeName32Cmb.AddString(_T("--------------"));
//#endif
	
//#if 1
	int i, n;
	CString str;
	vector<stDiary> &DiaryData = ((CBusinessDiary*)m_pParent)->GetDialyDataList();
	int count = (int)DiaryData.size();
	for (i = 0; i < count; ++i)
	{
		// 番号が"-"の場合はリストしない
		if (DiaryData[i].no == _T("-"))
		{
			continue;
		}

//		// 通信時間と通信回数が0の場合はリストしない
//		if ((DiaryData[i].data[0].time == _T("0") && DiaryData[i].data[0].count == _T("0"))
//			&& (DiaryData[i].data[1].time == _T("0") && DiaryData[i].data[1].count == _T("0"))
//			&& (DiaryData[i].data[2].time == _T("0") && DiaryData[i].data[2].count == _T("0")))
//		{
//			continue;
//		}

		for (n = 0; n < 3; ++n)
		{
			// 天候
			str = DiaryData[i].data[n].weather;
			if (str != _T("") && str != _T("-") && CB_ERR == m_Weather1Cmb.FindStringExact(-1, str))
			{
				m_Weather1Cmb.AddString(str);
				m_Weather2Cmb.AddString(str);
				m_Weather3Cmb.AddString(str);
			}

			// 資格1
			str = DiaryData[i].data[n].capa1;
			if (str != _T("") && str != _T("-") && CB_ERR == m_EmpCapa1Cmb.FindStringExact(-1, str))
			{
				m_EmpCapa1Cmb.AddString(str);
				m_EmpCapa2Cmb.AddString(str);
				m_EmpCapa3Cmb.AddString(str);
				m_OpeCapa11Cmb.AddString(str);
				m_OpeCapa12Cmb.AddString(str);
				m_OpeCapa21Cmb.AddString(str);
				m_OpeCapa22Cmb.AddString(str);
				m_OpeCapa31Cmb.AddString(str);
				m_OpeCapa32Cmb.AddString(str);
			}

			// 資格2
			str = DiaryData[i].data[n].capa2;
			if (str != _T("") && str != _T("-") && CB_ERR == m_EmpCapa1Cmb.FindStringExact(-1, str))
			{
				m_EmpCapa1Cmb.AddString(str);
				m_EmpCapa2Cmb.AddString(str);
				m_EmpCapa3Cmb.AddString(str);
				m_OpeCapa11Cmb.AddString(str);
				m_OpeCapa12Cmb.AddString(str);
				m_OpeCapa21Cmb.AddString(str);
				m_OpeCapa22Cmb.AddString(str);
				m_OpeCapa31Cmb.AddString(str);
				m_OpeCapa32Cmb.AddString(str);
			}

			// 資格3
			str = DiaryData[i].data[n].capa3;
			if (str != _T("") && str != _T("-") && CB_ERR == m_EmpCapa1Cmb.FindStringExact(-1, str))
			{
				m_EmpCapa1Cmb.AddString(str);
				m_EmpCapa2Cmb.AddString(str);
				m_EmpCapa3Cmb.AddString(str);
				m_OpeCapa11Cmb.AddString(str);
				m_OpeCapa12Cmb.AddString(str);
				m_OpeCapa21Cmb.AddString(str);
				m_OpeCapa22Cmb.AddString(str);
				m_OpeCapa31Cmb.AddString(str);
				m_OpeCapa32Cmb.AddString(str);
			}

			// 氏名1
			str = DiaryData[i].data[n].name1;
			if (str != _T("") && str != _T("-") && CB_ERR == m_EmpName1Cmb.FindStringExact(-1, str))
			{
				m_EmpName1Cmb.AddString(str);
				m_EmpName2Cmb.AddString(str);
				m_EmpName3Cmb.AddString(str);
				m_OpeName11Cmb.AddString(str);
				m_OpeName12Cmb.AddString(str);
				m_OpeName21Cmb.AddString(str);
				m_OpeName22Cmb.AddString(str);
				m_OpeName31Cmb.AddString(str);
				m_OpeName32Cmb.AddString(str);
			}

			// 氏名2
			str = DiaryData[i].data[n].name2;
			if (str != _T("") && str != _T("-") && CB_ERR == m_EmpName1Cmb.FindStringExact(-1, str))
			{
				m_EmpName1Cmb.AddString(str);
				m_EmpName2Cmb.AddString(str);
				m_EmpName3Cmb.AddString(str);
				m_OpeName11Cmb.AddString(str);
				m_OpeName12Cmb.AddString(str);
				m_OpeName21Cmb.AddString(str);
				m_OpeName22Cmb.AddString(str);
				m_OpeName31Cmb.AddString(str);
				m_OpeName32Cmb.AddString(str);
			}

			// 氏名3
			str = DiaryData[i].data[n].name3;
			if (str != _T("") && str != _T("-") && CB_ERR == m_EmpName1Cmb.FindStringExact(-1, str))
			{
				m_EmpName1Cmb.AddString(str);
				m_EmpName2Cmb.AddString(str);
				m_EmpName3Cmb.AddString(str);
				m_OpeName11Cmb.AddString(str);
				m_OpeName12Cmb.AddString(str);
				m_OpeName21Cmb.AddString(str);
				m_OpeName22Cmb.AddString(str);
				m_OpeName31Cmb.AddString(str);
				m_OpeName32Cmb.AddString(str);
			}
		}
	}


//#else
//	CString str, temp;
//	vector<CString>::iterator itr;
//	for (itr = mDataList.begin(); itr != mDataList.end(); itr++){
//		int i = 0;
//		int pos = 0;
//		while (AfxExtractSubString(str, (*itr), i++, ',')){
//			if (str.Left(1) == _T("-"))
//				continue;
//			switch (i - 1){
//			case	2:	// 天候
//			case	12:	// 天候
//			case	22:	// 天候
//				for (pos = 0; pos < m_Weather1Cmb.GetCount(); pos++){
//					m_Weather1Cmb.GetLBText(pos, temp);
//					if (temp == str)
//						break;
//				}
//				if (pos >= m_Weather1Cmb.GetCount()){
//					m_Weather1Cmb.AddString(str);
//					m_Weather2Cmb.AddString(str);
//					m_Weather3Cmb.AddString(str);
//				}
//				break;
//			case	3:	// 資格
//			case	5:	// 資格
//			case	7:	// 資格
//			case	13:	// 資格
//			case	15:	// 資格
//			case	17:	// 資格
//			case	23:	// 資格
//			case	25:	// 資格
//			case	27:	// 資格
//				for (pos = 0; pos < m_EmpCapa1Cmb.GetCount(); pos++){
//					m_EmpCapa1Cmb.GetLBText(pos, temp);
//					if (temp == str)
//						break;
//				}
//				if (pos >= m_EmpCapa1Cmb.GetCount()){
//					m_EmpCapa1Cmb.AddString(str);
//					m_EmpCapa2Cmb.AddString(str);
//					m_EmpCapa3Cmb.AddString(str);
//					m_OpeCapa11Cmb.AddString(str);
//					m_OpeCapa12Cmb.AddString(str);
//					m_OpeCapa21Cmb.AddString(str);
//					m_OpeCapa22Cmb.AddString(str);
//					m_OpeCapa31Cmb.AddString(str);
//					m_OpeCapa32Cmb.AddString(str);
//				}
//				break;
//			case	4:	// 氏名
//			case	6:	// 氏名
//			case	8:	// 氏名
//			case	14:	// 氏名
//			case	16:	// 氏名
//			case	18:	// 氏名
//			case	24:	// 氏名
//			case	26:	// 氏名
//			case	28:	// 氏名
//				for (pos = 0; pos < m_EmpName1Cmb.GetCount(); pos++){
//					m_EmpName1Cmb.GetLBText(pos, temp);
//					if (temp == str)
//						break;
//				}
//				if (pos >= m_EmpName1Cmb.GetCount()){
//					m_EmpName1Cmb.AddString(str);
//					m_EmpName2Cmb.AddString(str);
//					m_EmpName3Cmb.AddString(str);
//					m_OpeName11Cmb.AddString(str);
//					m_OpeName12Cmb.AddString(str);
//					m_OpeName21Cmb.AddString(str);
//					m_OpeName22Cmb.AddString(str);
//					m_OpeName31Cmb.AddString(str);
//					m_OpeName32Cmb.AddString(str);
//				}
//				break;
//			default:
//				break;
//			}
//		}
//	}
//#endif

	m_Weather1Cmb.SetCurSel(0);
	m_Weather2Cmb.SetCurSel(0);
	m_Weather3Cmb.SetCurSel(0);
	m_EmpCapa1Cmb.SetCurSel(0);
	m_EmpCapa2Cmb.SetCurSel(0);
	m_EmpCapa3Cmb.SetCurSel(0);
	m_EmpName1Cmb.SetCurSel(0);
	m_EmpName2Cmb.SetCurSel(0);
	m_EmpName3Cmb.SetCurSel(0);
	m_OpeCapa11Cmb.SetCurSel(0);
	m_OpeCapa12Cmb.SetCurSel(0);
	m_OpeCapa21Cmb.SetCurSel(0);
	m_OpeCapa22Cmb.SetCurSel(0);
	m_OpeCapa31Cmb.SetCurSel(0);
	m_OpeCapa32Cmb.SetCurSel(0);
	m_OpeName11Cmb.SetCurSel(0);
	m_OpeName12Cmb.SetCurSel(0);
	m_OpeName21Cmb.SetCurSel(0);
	m_OpeName22Cmb.SetCurSel(0);
	m_OpeName31Cmb.SetCurSel(0);
	m_OpeName32Cmb.SetCurSel(0);
}

void CBusinessDiaryEdit::OnBnClickedDiaryEditBackBtn()
{
#if 1
	int item = mListPos;
	vector<stDiary> &DiaryData = ((CBusinessDiary*)m_pParent)->GetDialyDataList();
	int baseIdx = ((CBusinessDiary*)m_pParent)->GetDiaryListIndex(mListPos);
	int count = (int)DiaryData.size();

	BOOL bFind = FALSE;
	for (int i = baseIdx; i >= 0; --i)
	{
		if (mSatellite != mAllText)
		{
			if (mSatellite != DiaryData[i].data[0].station && mSatellite != DiaryData[i].data[1].station && mSatellite != DiaryData[i].data[2].station)
			{
				continue;
			}
		}

		// 番号が"-"の場合はリストしない
		if (DiaryData[i].no == _T("-"))
		{
			continue;
		}

//		// 通信時間と通信回数が0の場合はリストしない
//		if ((DiaryData[i].data[0].time == _T("0") && DiaryData[i].data[0].count == _T("0"))
//			&& (DiaryData[i].data[1].time == _T("0") && DiaryData[i].data[1].count == _T("0"))
//			&& (DiaryData[i].data[2].time == _T("0") && DiaryData[i].data[2].count == _T("0")))
//		{
//			continue;
//		}

		item = atoi(CStringA(DiaryData[i].no));	
		if (item < mListPos)
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{
		mListPos = item;
		DisplayData();
		((CBusinessDiary*)m_pParent)->SelectListRow(mListPos);
	}
#else
	if (mPos == 0)
		return;
	mPos--;
	DisplayData();
#endif
}

void CBusinessDiaryEdit::OnBnClickedDiaryEditNextBtn()
{
#if 1
	int item = mListPos;
	vector<stDiary> &DiaryData = ((CBusinessDiary*)m_pParent)->GetDialyDataList();
	int baseIdx = ((CBusinessDiary*)m_pParent)->GetDiaryListIndex(mListPos);
	int count = (int)DiaryData.size();

	BOOL bFind = FALSE;
	for (int i = baseIdx; i < count; ++i)
	{
		if (mSatellite != mAllText)
		{
			if (mSatellite != DiaryData[i].data[0].station && mSatellite != DiaryData[i].data[1].station && mSatellite != DiaryData[i].data[2].station)
			{
				continue;
			}
		}

		// 番号が"-"の場合はリストしない
		if (DiaryData[i].no == _T("-"))
		{
			continue;
		}

//		// 通信時間と通信回数が0の場合はリストしない
//		if ((DiaryData[i].data[0].time == _T("0") && DiaryData[i].data[0].count == _T("0"))
//			&& (DiaryData[i].data[1].time == _T("0") && DiaryData[i].data[1].count == _T("0"))
//			&& (DiaryData[i].data[2].time == _T("0") && DiaryData[i].data[2].count == _T("0")))
//		{
//			continue;
//		}

		item = atoi(CStringA(DiaryData[i].no));
		if (item > mListPos)
		{
			bFind = TRUE;
			break;
		}
	}

	if(bFind)
	{
		mListPos = item;
		DisplayData();
		((CBusinessDiary*)m_pParent)->SelectListRow(mListPos);
	}

#else
	if (mPos == mDataList.size())
		return;
	mPos++;
	DisplayData();
#endif
}

void CBusinessDiaryEdit::DisplayData()
{
	UpdateData(TRUE);

//#if 1
	CString str;
	int item;
	vector<stDiary> &DiaryData = ((CBusinessDiary*)m_pParent)->GetDialyDataList();
	vector<stDiary>::iterator itr;
	for (itr = DiaryData.begin(); itr != DiaryData.end(); itr++){
		stDiary st = (*itr);
		if (mSatellite != mAllText)
		{
			if (mSatellite != (*itr).data[0].station && mSatellite != (*itr).data[1].station && mSatellite != (*itr).data[2].station)
			{
				continue;
			}
		}

		// 番号が"-"の場合はリストしない
		if (st.no == _T("-"))
		{
			continue;
		}

////#if 0
////		// 通信時間と通信回数が0の場合はリストしない
////		if ((st.data[0].time == _T("0") && st.data[0].count == _T("0"))
////			&& (st.data[1].time == _T("0") && st.data[1].count == _T("0"))
////			&& (st.data[2].time == _T("0") && st.data[2].count == _T("0")))
////#else
//		// 天候情報なしで、かつ通信時間と通信回数が0の場合はリストしない
//		if (   st.data[0].weather.Find(_T("-")) >= 0
//			&& st.data[1].weather.Find(_T("-")) >= 0
//			&& st.data[2].weather.Find(_T("-")) >= 0
//			&& (st.data[0].time == _T("0") && st.data[0].count == _T("0"))
//			&& (st.data[1].time == _T("0") && st.data[1].count == _T("0"))
//			&& (st.data[2].time == _T("0") && st.data[2].count == _T("0")))
//
////#endif
//		{
//			continue;
//		}

		UINT time = 0;
		item = atoi(CStringA(st.no));
		if (item == mListPos)
		{
			// 運用日
			COleDateTime oleTime;
			oleTime.ParseDateTime(st.date);
			m_OperationDate = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), 0, 0, 0);

			// インデックス
			m_strOperationBackNo = (*itr).no;

			//------------------
			// 天候
			if (st.data[0].weather == _T("-") || st.data[0].weather == _T(""))
			{
				m_Weather1Cmb.SelectStringEx(-1, _T("---"));
			}
			else
			{
				m_Weather1Cmb.SelectStringEx(-1, st.data[0].weather);
			}
			OnCbnSelchangeDiaryEdit1WeatherCmb();

			// 資格1
			if (st.data[0].capa1 == _T("-") || st.data[0].capa1 == _T(""))
			{
				m_EmpCapa1Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_EmpCapa1Cmb.SelectStringEx(-1, st.data[0].capa1);
			}

			// 氏名1
			if (st.data[0].name1 == _T("-") || st.data[0].name1 == _T(""))
			{
				m_EmpName1Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_EmpName1Cmb.SelectStringEx(-1, st.data[0].name1);
			}

			// 資格2
			if (st.data[0].capa2 == _T("-") || st.data[0].capa2 == _T(""))
			{
				m_OpeCapa11Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_OpeCapa11Cmb.SelectStringEx(-1, st.data[0].capa2);
			}

			// 氏名2
			if (st.data[0].name2 == _T("-") || st.data[0].name2 == _T(""))
			{
				m_OpeName11Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_OpeName11Cmb.SelectStringEx(-1, st.data[0].name2);
			}

			// 資格3
			if (st.data[0].capa3 == _T("-") || st.data[0].capa3 == _T(""))
			{
				m_OpeCapa12Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_OpeCapa12Cmb.SelectStringEx(-1, st.data[0].capa3);
			}

			// 氏名3
			if (st.data[0].name3 == _T("-") || st.data[0].name3 == _T(""))
			{
				m_OpeName12Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_OpeName12Cmb.SelectStringEx(-1, st.data[0].name3);
			}

			// 相手局
			if (st.data[0].station != _T("-"))
			{
				m_strStation1 = st.data[0].station;
			}
			else
			{
				m_strStation1 = _T("");
			}

			// 時間
			if (st.data[0].time != _T("-"))
			{
				time = _wtoi(st.data[0].time);
				m_strComTimeH1.Format(_T("%d"), time / 60);
				m_strComTimeM1.Format(_T("%d"), time % 60);
			}

			// 回数
			if (st.data[0].count != _T("-"))
			{
				m_strComCount1 = st.data[0].count;
			}

			//------------------
			// 天候
			if (st.data[1].weather == _T("-") || st.data[1].weather == _T(""))
			{
				m_Weather2Cmb.SelectStringEx(-1, _T("---"));
			}
			else
			{
				m_Weather2Cmb.SelectStringEx(-1, st.data[1].weather);
			}
			OnCbnSelchangeDiaryEdit2WeatherCmb();

			// 資格1
			if (st.data[1].capa1 == _T("-") || st.data[1].capa1 == _T(""))
			{
				m_EmpCapa2Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_EmpCapa2Cmb.SelectStringEx(-1, st.data[1].capa1);
			}

			// 氏名1
			if (st.data[1].name1 == _T("-") || st.data[1].name1 == _T(""))
			{
				m_EmpName2Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_EmpName2Cmb.SelectStringEx(-1, st.data[1].name1);
			}

			// 資格2
			if (st.data[1].capa2 == _T("-") || st.data[1].capa2 == _T(""))
			{
				m_OpeCapa21Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_OpeCapa21Cmb.SelectStringEx(-1, st.data[1].capa2);
			}

			// 氏名2
			if (st.data[1].name2 == _T("-") || st.data[1].name2 == _T(""))
			{
				m_OpeName21Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_OpeName21Cmb.SelectStringEx(-1, st.data[1].name2);
			}

			// 資格3
			if (st.data[1].capa3 == _T("-") || st.data[1].capa3 == _T(""))
			{
				m_OpeCapa22Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_OpeCapa22Cmb.SelectStringEx(-1, st.data[1].capa3);
			}

			// 氏名3
			if (st.data[1].name3 == _T("-") || st.data[1].name3 == _T(""))
			{
				m_OpeName22Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_OpeName22Cmb.SelectStringEx(-1, st.data[1].name3);
			}

			// 相手局
			if (st.data[1].station != _T("-"))
			{
				m_strStation2 = st.data[1].station;
			}
			else
			{
				m_strStation2 = _T("");
			}

			// 時間
			if (st.data[1].time != _T("-"))
			{
				time = _wtoi(st.data[1].time);
				m_strComTimeH2.Format(_T("%d"), time / 60);
				m_strComTimeM2.Format(_T("%d"), time % 60);
			}

			// 回数
			if (st.data[1].count != _T("-"))
			{
				m_strComCount2 = st.data[1].count;
			}

			//------------------
			// 天候
			if (st.data[2].weather == _T("-") || st.data[2].weather == _T(""))
			{
				m_Weather3Cmb.SelectStringEx(-1, _T("---"));
			}
			else
			{
				m_Weather3Cmb.SelectStringEx(-1, st.data[2].weather);
			}
			OnCbnSelchangeDiaryEdit3WeatherCmb();

			// 資格1
			if (st.data[2].capa1 == _T("-") || st.data[2].capa1 == _T(""))
			{
				m_EmpCapa3Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_EmpCapa3Cmb.SelectStringEx(-1, st.data[2].capa1);
			}

			// 氏名1
			if (st.data[2].name1 == _T("-") || st.data[2].name1 == _T(""))
			{
				m_EmpName3Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_EmpName3Cmb.SelectStringEx(-1, st.data[2].name1);
			}

			// 資格2
			if (st.data[2].capa2 == _T("-") || st.data[2].capa2 == _T(""))
			{
				m_OpeCapa31Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_OpeCapa31Cmb.SelectStringEx(-1, st.data[2].capa2);
			}

			// 氏名2
			if (st.data[2].name2 == _T("-") || st.data[2].name2 == _T(""))
			{
				m_OpeName31Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_OpeName31Cmb.SelectStringEx(-1, st.data[2].name2);
			}

			// 資格3
			if (st.data[2].capa3 == _T("-") || st.data[2].capa3 == _T(""))
			{
				m_OpeCapa32Cmb.SelectStringEx(-1, _T("------"));
			}
			else
			{
				m_OpeCapa32Cmb.SelectStringEx(-1, st.data[2].capa3);
			}

			// 氏名3
			if (st.data[2].name3 == _T("-") || st.data[2].name3 == _T(""))
			{
				m_OpeName32Cmb.SelectStringEx(-1, _T("--------------"));
			}
			else
			{
				m_OpeName32Cmb.SelectStringEx(-1, st.data[2].name3);
			}

			// 相手局
			if (st.data[2].station != _T("-"))
			{
				m_strStation3 = st.data[2].station;
			}
			else
			{
				m_strStation3 = _T("");
			}

			// 時間
			if (st.data[2].time != _T("-"))
			{
				time = _wtoi(st.data[2].time);
				m_strComTimeH3.Format(_T("%d"), time / 60);
				m_strComTimeM3.Format(_T("%d"), time % 60);
			}

			// 回数
			if (st.data[2].count != _T("-"))
			{
				m_strComCount3 = st.data[2].count;
			}

			//------------------
			// 備考1
			str = st.info1;
			if (str != _T("-"))
			{
				str.Replace(_T("\\"), _T("\r\n"));
				m_strCause = str;
			}

			// 備考2
			str = st.info2;
			if (str != _T("-"))
			{
				str.Replace(_T("\\"), _T("\r\n"));
				m_strResult = str;
			}

			// 備考3
			str = st.info3;
			if (str != _T("-"))
			{
				str.Replace(_T("\\"), _T("\r\n"));
				m_strNotices = str;
			}
		}
	}
//#else
//
//	if (mDataList.size() == 0)
//		return;
//
//	SYSTEMTIME date;
//	CString str, temp;
//	int i = 0;
//	int pos;
//	while (AfxExtractSubString(str, mDataList[mPos], i++, ',')){
//		if (str.Left(1) == _T("-"))
//			continue;
//		if (str == _T("\n"))
//			continue;
//		switch (i - 1){
//		case	1:	// 運用日
//			memset(&date, 0, sizeof(SYSTEMTIME));
//			date.wYear = _wtoi(str.Left(4));
//			date.wMonth = _wtoi(str.Mid(5, 2));
//			date.wDay = _wtoi(str.Right(2));
//			m_OperationDate = CTime(date);
//			mDate.SetTime(&m_OperationDate);
//			break;
//		case	2:	// 天候
//		case	12:	// 天候
//		case	22:	// 天候
//			pos = i - 1;
//			if (pos == 2)	m_Weather1Cmb.SelectStringEx(-1, str);
//			if (pos == 12)	m_Weather2Cmb.SelectStringEx(-1, str);
//			if (pos == 22)	m_Weather3Cmb.SelectStringEx(-1, str);
//			break;
//		case	3:	// 資格
//		case	5:	// 資格
//		case	7:	// 資格
//		case	13:	// 資格
//		case	15:	// 資格
//		case	17:	// 資格
//		case	23:	// 資格
//		case	25:	// 資格
//		case	27:	// 資格
//			pos = i - 1;
//			if (pos == 3)	m_EmpCapa1Cmb.SelectStringEx(-1, str);
//			if (pos == 5)	m_OpeCapa11Cmb.SelectStringEx(-1, str);
//			if (pos == 7)	m_OpeCapa12Cmb.SelectStringEx(-1, str);
//			if (pos == 13)	m_EmpCapa2Cmb.SelectStringEx(-1, str);
//			if (pos == 15)	m_OpeCapa21Cmb.SelectStringEx(-1, str);
//			if (pos == 17)	m_OpeCapa22Cmb.SelectStringEx(-1, str);
//			if (pos == 23)	m_EmpCapa3Cmb.SelectStringEx(-1, str);
//			if (pos == 25)	m_OpeCapa31Cmb.SelectStringEx(-1, str);
//			if (pos == 27)	m_OpeCapa32Cmb.SelectStringEx(-1, str);
//			break;
//		case	4:	// 氏名
//		case	6:	// 氏名
//		case	8:	// 氏名
//		case	14:	// 氏名
//		case	16:	// 氏名
//		case	18:	// 氏名
//		case	24:	// 氏名
//		case	26:	// 氏名
//		case	28:	// 氏名
//			pos = i - 1;
//			if (pos == 4)	m_EmpName1Cmb.SelectStringEx(-1, str);
//			if (pos == 6)	m_OpeName11Cmb.SelectStringEx(-1, str);
//			if (pos == 8)	m_OpeName12Cmb.SelectStringEx(-1, str);
//			if (pos == 14)	m_EmpName2Cmb.SelectStringEx(-1, str);
//			if (pos == 16)	m_OpeName21Cmb.SelectStringEx(-1, str);
//			if (pos == 18)	m_OpeName22Cmb.SelectStringEx(-1, str);
//			if (pos == 24)	m_EmpName3Cmb.SelectStringEx(-1, str);
//			if (pos == 26)	m_OpeName31Cmb.SelectStringEx(-1, str);
//			if (pos == 28)	m_OpeName32Cmb.SelectStringEx(-1, str);
//			break;
//		case	9:	// 相手局
//		case	19:	// 相手局
//		case	29:	// 相手局
//			pos = i - 1;
//			if (pos == 9)	m_strStation1 = str;
//			if (pos == 19)	m_strStation2 = str;
//			if (pos == 29)	m_strStation3 = str;
//			break;
//		case	10:	// 時間
//		case	20:	// 時間
//		case	30:	// 時間
//		{
//			pos = i - 1;
//			UINT time = _wtoi(str);
//			if (pos == 10){
//				m_strComTimeH1.Format(_T("%d"), time / 60);
//				m_strComTimeM1.Format(_T("%d"), time % 60);
//			}
//			if (pos == 20){
//				m_strComTimeH2.Format(_T("%d"), time / 60);
//				m_strComTimeM2.Format(_T("%d"), time % 60);
//			}
//			if (pos == 30){
//				m_strComTimeH3.Format(_T("%d"), time / 60);
//				m_strComTimeM3.Format(_T("%d"), time % 60);
//			}
//			break;
//		}
//		case	11:	// 通信回数
//		case	21:	// 通信回数
//		case	31:	// 通信回数
//			pos = i - 1;
//			if (pos == 11)	m_strComCount1 = str;
//			if (pos == 21)	m_strComCount2 = str;
//			if (pos == 31)	m_strComCount3 = str;
//			break;
//		case	32:	// 故障原因
//			str.Replace(_T("\\"), _T("\r\n"));
//			m_strCause = str;
//			break;
//		case	33:	// 実験方法
//			str.Replace(_T("\\"), _T("\r\n"));
//			m_strResult = str;
//			break;
//		case	34:	// 特記事項
//			str.Replace(_T("\\"), _T("\r\n"));
//			m_strNotices = str;
//			break;
//		default:
//			break;
//		}
//	}
//#endif
	UpdateData(FALSE);
}


void CBusinessDiaryEdit::OnDtnDatetimechangeDiaryEditOpedayDp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	mUpdate = true;
	*pResult = 0;
}


void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1WeatherCmb()
{
	mUpdate = true;

	int sel = m_Weather1Cmb.GetCurSel();
	m_EmpCapa1Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_EmpName1Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeCapa11Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeName11Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeCapa12Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeName12Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_1_STATION_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_1_HOUR_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_1_MINUTE_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_1_COUNT_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
}

void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit2WeatherCmb()
{
	mUpdate = true;

	int sel = m_Weather2Cmb.GetCurSel();
	m_EmpCapa2Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_EmpName2Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeCapa21Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeName21Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeCapa22Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeName22Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_2_STATION_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_2_HOUR_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_2_MINUTE_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_2_COUNT_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
}

void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit3WeatherCmb()
{
	mUpdate = true;

	int sel = m_Weather3Cmb.GetCurSel();
	m_EmpCapa3Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_EmpName3Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeCapa31Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeName31Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeCapa32Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	m_OpeName32Cmb.EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_3_STATION_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_3_HOUR_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_3_MINUTE_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_3_COUNT_EDIT)->EnableWindow(sel == 0 ? FALSE : TRUE);
}


void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1CapaCmb()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1NameCmb()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Capa21Cmb()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Name21Cmb()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Capa22Cmb()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnCbnSelchangeDiaryEdit1Name22Cmb()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnEnChangeDiaryEdit1StationEdit()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnEnChangeDiaryEdit1HourEdit()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnEnChangeDiaryEdit1MinuteEdit()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnEnChangeDiaryEdit1CountEdit()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnEnChangeDiaryEditCauseEdit()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnEnChangeDiaryEditResultEdit()
{
	mUpdate = true;
}


void CBusinessDiaryEdit::OnEnChangeDiaryEditNoticesEdit()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# ファイル-閉じるメニュー処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnMenuCloseClose()
{
//#if 0
//	if (mWriteData == true){
//		WriteDiaryData();
//	}
//#endif
	OnOK();
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 天候1 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1WeatherCmb()
{
	CString str;
	mUpdate = true;

	// 文字列取得
	m_Weather1Cmb.GetWindowTextW(str);

	m_EmpCapa1Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_EmpName1Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeCapa11Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeName11Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeCapa12Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeName12Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_1_STATION_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_1_HOUR_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_1_MINUTE_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_1_COUNT_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 天候1 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1WeatherCmb()
{
	CString str;
	m_Weather1Cmb.GetWindowTextW(str);
	if (CB_ERR == m_Weather1Cmb.FindStringExact(-1, str))
	{
		m_Weather1Cmb.AddString(str);
		m_Weather2Cmb.AddString(str);
		m_Weather3Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格1-1 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1CapaCmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格1-1 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1CapaCmb()
{
	CString str;
	m_EmpCapa1Cmb.GetWindowTextW(str);
	if (CB_ERR == m_EmpCapa1Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名1-1 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1NameCmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名1-1 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1NameCmb()
{
	CString str;
	m_EmpName1Cmb.GetWindowTextW(str);
	if (CB_ERR == m_EmpName1Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格1-2 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1Capa21Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格1-2 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1Capa21Cmb()
{
	CString str;
	m_OpeCapa11Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeCapa11Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名1-2 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1Name21Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名1-2 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1Name21Cmb()
{
	CString str;
	m_OpeName11Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeName11Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格1-3 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1Capa22Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格1-3 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1Capa22Cmb()
{
	CString str;
	m_OpeCapa12Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeCapa12Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名1-3 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit1Name22Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名1-3 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit1Name22Cmb()
{
	CString str;
	m_OpeName12Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeName12Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 天候2 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2WeatherCmb()
{
	CString str;
	mUpdate = true;

	// 文字列取得
	m_Weather2Cmb.GetWindowTextW(str);

	m_EmpCapa2Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_EmpName2Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeCapa21Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeName21Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeCapa22Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeName22Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_2_STATION_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_2_HOUR_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_2_MINUTE_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_2_COUNT_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 天候2 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2WeatherCmb()
{
	CString str;
	m_Weather2Cmb.GetWindowTextW(str);
	if (CB_ERR == m_Weather2Cmb.FindStringExact(-1, str))
	{
		m_Weather1Cmb.AddString(str);
		m_Weather2Cmb.AddString(str);
		m_Weather3Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格2-1 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2CapaCmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格2-1 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2CapaCmb()
{
	CString str;
	m_EmpCapa2Cmb.GetWindowTextW(str);
	if (CB_ERR == m_EmpCapa2Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名2-1 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2NameCmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名2-1 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2NameCmb()
{
	CString str;
	m_EmpName2Cmb.GetWindowTextW(str);
	if (CB_ERR == m_EmpName2Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格2-2 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2Capa21Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格2-2 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2Capa21Cmb()
{
	CString str;
	m_OpeCapa21Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeCapa21Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名2-2 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2Name21Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名2-2 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2Name21Cmb()
{
	CString str;
	m_OpeName21Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeName21Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格2-3 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2Capa22Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格2-3 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2Capa22Cmb()
{
	CString str;
	m_OpeCapa22Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeCapa22Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名2-3 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit2Name22Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名2-3 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit2Name22Cmb()
{
	CString str;
	m_OpeName22Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeName22Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 天候3 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3WeatherCmb()
{
	CString str;
	mUpdate = true;

	// 文字列取得
	m_Weather3Cmb.GetWindowTextW(str);

	m_EmpCapa3Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_EmpName3Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeCapa31Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeName31Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeCapa32Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	m_OpeName32Cmb.EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_3_STATION_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_3_HOUR_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_3_MINUTE_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
	GetDlgItem(IDC_DIARY_EDIT_3_COUNT_EDIT)->EnableWindow(str == _T("---") ? FALSE : TRUE);
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 天候3 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3WeatherCmb()
{
	CString str;
	m_Weather3Cmb.GetWindowTextW(str);
	if (CB_ERR == m_Weather3Cmb.FindStringExact(-1, str))
	{
		m_Weather1Cmb.AddString(str);
		m_Weather2Cmb.AddString(str);
		m_Weather3Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格3-1 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3CapaCmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格3-1 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3CapaCmb()
{
	CString str;
	m_EmpCapa3Cmb.GetWindowTextW(str);
	if (CB_ERR == m_EmpCapa3Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名3-1 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3NameCmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名3-1 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3NameCmb()
{
	CString str;
	m_EmpName3Cmb.GetWindowTextW(str);
	if (CB_ERR == m_EmpName3Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格3-2 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3Capa21Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格3-2 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3Capa21Cmb()
{
	CString str;
	m_OpeCapa31Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeCapa31Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名3-2 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3Name21Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名3-2 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3Name21Cmb()
{
	CString str;
	m_OpeName31Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeName31Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格3-3 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3Capa22Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 資格3-3 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3Capa22Cmb()
{
	CString str;
	m_OpeCapa32Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeCapa32Cmb.FindStringExact(-1, str))
	{
		m_EmpCapa1Cmb.AddString(str);
		m_OpeCapa11Cmb.AddString(str);
		m_OpeCapa12Cmb.AddString(str);
		m_EmpCapa2Cmb.AddString(str);
		m_OpeCapa21Cmb.AddString(str);
		m_OpeCapa22Cmb.AddString(str);
		m_EmpCapa3Cmb.AddString(str);
		m_OpeCapa31Cmb.AddString(str);
		m_OpeCapa32Cmb.AddString(str);
	}
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名3-3 コンボボックス- エディット変更通知

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnEditchangeDiaryEdit3Name22Cmb()
{
	mUpdate = true;
}


/*============================================================================*/
/*! CBusinessDiaryEdit

-# 氏名3-3 コンボボックス- Kill Forcus

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryEdit::OnCbnKillfocusDiaryEdit3Name22Cmb()
{
	CString str;
	m_OpeName32Cmb.GetWindowTextW(str);
	if (CB_ERR == m_OpeName32Cmb.FindStringExact(-1, str))
	{
		m_EmpName1Cmb.AddString(str);
		m_OpeName11Cmb.AddString(str);
		m_OpeName12Cmb.AddString(str);
		m_EmpName2Cmb.AddString(str);
		m_OpeName21Cmb.AddString(str);
		m_OpeName22Cmb.AddString(str);
		m_EmpName3Cmb.AddString(str);
		m_OpeName31Cmb.AddString(str);
		m_OpeName32Cmb.AddString(str);
	}
}

