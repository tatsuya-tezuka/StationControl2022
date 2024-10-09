#pragma once
#include "atltime.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "DateTimeCtrlNoToday.h"

// COperationPlanTimeSet ダイアログ

class COperationPlanTimeSet : public CDialogBase
{
	DECLARE_DYNAMIC(COperationPlanTimeSet)

public:
	COperationPlanTimeSet(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~COperationPlanTimeSet();

// ダイアログ データ
	enum { IDD = IDD_DLG_OPERATIONPLAN_TIMESET };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum {
		TYPE_UL,
		TYPE_RNG,
		TYPE_CMD,
		TYPE_TLMS,
		TYPE_TLMX,
		TYPE_TLMKa
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CTime m_sTime;
	CTime m_eTime;

	CTime m_Range1;
	CTime m_Range2;

	int m_OpeType;
	vector<CString> m_OpeTypeEle;

protected:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void SetDefTime(CTime startTime, CTime endTime) { m_sTime = startTime; m_eTime = endTime; }
	void SetRangeTime(CTime range1, CTime range2) { m_Range1 = range1; m_Range2 = range2; }
	void SetParam(int opetype, vector<CString> ele, CString kind = _T("")) { m_OpeType = opetype; m_OpeTypeEle = ele; m_OpeTypeKind = kind; }

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpeplnTimeRegBtn();
	// 開始日時（Day）
	CTime m_StartDay;
	// 開始日時（Time）
	CTime m_StartTime;
	// 終了日時（Day）
	CTime m_EndDay;
	// 終了日時（Time）
	CTime m_EndTime;
	virtual BOOL OnInitDialog();
	// 開始日時コントロール
	CDateTimeCtrlNoToday m_StartDayCtrl;
	CDateTimeCtrl m_StartTimeCtrl;
	// 終了日時コントロール
	CDateTimeCtrlNoToday m_EndDayCtrl;
	CDateTimeCtrl m_EndTimeCtrl;
	CStatic m_KindStaticCtrl;
	CComboBox m_KindCmbCtrl;
	CString m_OpeTypeKind;
};
