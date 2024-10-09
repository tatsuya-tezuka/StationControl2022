/*============================================================================*/
/*! OperationPlanTimeSet.cpp

-# 計画立案画面-日時設定画面
*/
/*============================================================================*/
// OperationPlanTimeSet.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationPlanTimeSet.h"
#include "afxdialogex.h"


// COperationPlanTimeSet ダイアログ

IMPLEMENT_DYNAMIC(COperationPlanTimeSet, CDialogBase)

/*============================================================================*/
/*! COperationPlanTimeSet

-# コンストラクタ

@param  pParent	：親ウィンドウ
@retval なし
*/
/*============================================================================*/
COperationPlanTimeSet::COperationPlanTimeSet(CWnd* pParent /*=NULL*/)
: CDialogBase(COperationPlanTimeSet::IDD, pParent)
	, m_StartDay(0)
	, m_StartTime(0)
	, m_EndDay(0)
	, m_EndTime(0)
	, m_OpeTypeKind(_T(""))
{
	m_OpeType = -1;
	m_OpeTypeEle.clear();
}

/*============================================================================*/
/*! COperationPlanTimeSet

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
COperationPlanTimeSet::~COperationPlanTimeSet()
{
}

void COperationPlanTimeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_TIME_SDAY_DP, m_StartDay);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_TIME_STIME_DP, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_TIME_EDAY_DP, m_EndDay);
	DDX_DateTimeCtrl(pDX, IDC_OPEPLN_TIME_ETIME_DP, m_EndTime);
	DDX_Control(pDX, IDC_OPEPLN_TIME_SDAY_DP, m_StartDayCtrl);
	DDX_Control(pDX, IDC_OPEPLN_TIME_EDAY_DP, m_EndDayCtrl);
	DDX_Control(pDX, IDC_OPEPLN_TIME_STIME_DP, m_StartTimeCtrl);
	DDX_Control(pDX, IDC_OPEPLN_TIME_ETIME_DP, m_EndTimeCtrl);
	DDX_Control(pDX, IDC_OPEPLN_KIND_STATIC, m_KindStaticCtrl);
	DDX_Control(pDX, IDC_OPEPLN_KIND_CMB, m_KindCmbCtrl);
	DDX_CBString(pDX, IDC_OPEPLN_KIND_CMB, m_OpeTypeKind);
}


BEGIN_MESSAGE_MAP(COperationPlanTimeSet, CDialogBase)
	ON_BN_CLICKED(IDC_OPEPLN_TIME_REG_BTN, &COperationPlanTimeSet::OnBnClickedOpeplnTimeRegBtn)
END_MESSAGE_MAP()


// COperationPlanTimeSet メッセージ ハンドラー

/*============================================================================*/
/*! COperationPlanTimeSet

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL COperationPlanTimeSet::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_StartDay	= m_sTime;
	m_StartTime = m_sTime;
	m_EndDay	= m_eTime;
	m_EndTime	= m_eTime;

	m_StartDayCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_StartTimeCtrl.SetFormat(_T("HH:mm:ss"));
	m_EndDayCtrl.SetFormat(_T("yyyy-MM-dd"));
	m_EndTimeCtrl.SetFormat(_T("HH:mm:ss"));

	// 範囲
	m_StartDayCtrl.SetRange(&m_Range1, &m_Range2);
//	m_StartTimeCtrl.SetRange(&m_Range1, &m_Range2);
	m_EndDayCtrl.SetRange(&m_Range1, &m_Range2);
//	m_EndTimeCtrl.SetRange(&m_Range1, &m_Range2);

	if (m_OpeType == TYPE_RNG)
	{
		int size = (int)m_OpeTypeEle.size();
		for (int i = 0; i < size; ++i)
		{
			m_KindCmbCtrl.InsertString(-1, m_OpeTypeEle[i]);
		}
		m_KindCmbCtrl.SetCurSel(0);
	}
	else
	{
		m_KindStaticCtrl.ShowWindow(FALSE);
		m_KindCmbCtrl.ShowWindow(FALSE);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! COperationPlanTimeSet

-# 登録ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlanTimeSet::OnBnClickedOpeplnTimeRegBtn()
{
	UpdateData(TRUE);
	CTime sd = m_StartDay;
	CTime st = m_StartTime;
	CTime ed = m_EndDay;
	CTime et = m_EndTime;

	CTime cSTime = CTime(sd.GetYear(), sd.GetMonth(), sd.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
	CTime cETime = CTime(ed.GetYear(), ed.GetMonth(), ed.GetDay(), et.GetHour(), et.GetMinute(), et.GetSecond());

	if (cSTime.GetTime() > cETime.GetTime())
	{
		AfxMessageBox(_T("終了日時が開始日時より過去です。\n日時を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	CString str;
	m_KindCmbCtrl.GetWindowTextW(str);
	m_OpeTypeKind = str;

	OnOK();
}

