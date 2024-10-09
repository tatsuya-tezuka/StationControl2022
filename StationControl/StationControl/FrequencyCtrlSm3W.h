#pragma once
#include "atltime.h"
#include "afxdtctl.h"
#include "afxwin.h"

// 送信周波数取得先
typedef struct{
	const CString		sendname;		// 相手先名称
	UINT				sendid;			// 相手先ID
} stSimSendFreq;
static const stSimSendFreq mSimSenderFreq[eStation_MAX][2] = {
	{
		{ _T("USC34m"), eStation_Uchinoura34 },
		{ _T("UDSC54m"), eStation_Usuda642 }
	},
	{
		{ _T("UDSC64m"), eStation_Usuda64 },
		{ _T("USC34m"), eStation_Uchinoura34 }
	},
	{
		{ _T("UDSC64m"), eStation_Usuda64 },
		{ _T("UDSC54m"), eStation_Usuda642 }
	},
	{
		{ _T(""), eStation_Usuda64 },
		{ _T(""), eStation_Usuda64 }
	},
};

// CFrequencyCtrlSm3W ダイアログ

class CFrequencyCtrlSm3W : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSm3W)

public:
	CFrequencyCtrlSm3W(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlSm3W();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_SML_3W };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mItemRows = 4;
	static const UINT mItemCols = 4;
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	CWnd*		mCallWnd;
	CTrendData	mTrendData;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	double	CalculatioFreq(time_t time, int type, int timetype, bool bPassUse = true);

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 3WAY用他局送信周波数
	CString m_str3WAYFreq;
	afx_msg void OnBnClickedFreqCtrlSml3wCalcBtn();
	// 指定時刻（Day）
	CTime m_SpecDay;
	// 指定時刻（Time）
	CTime m_SpecTime;
	CDateTimeCtrl m_SpecDayDTCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFreqCtrlSml3wCalcBtn2();
	CComboBox m_cbSimSendGet;
	CString m_strSimValue[mItemRows][mItemCols];
	afx_msg void OnBnClickedButtonGet();
};
