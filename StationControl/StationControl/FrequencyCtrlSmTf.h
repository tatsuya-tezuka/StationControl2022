#pragma once
#include "atltime.h"
#include "afxdtctl.h"

enum{
	eDoppOn,
	eDoppOffCenter,
	eDoppOffNomi,
	eDoppOffAos,
};

// 送信ドップラ
static const CString mSimTxDopp[] = {
	_T("動的補償"),
	_T("静的補償(センター)"),
	_T("静的補償(ノミナル)"),
	_T("静的補償(AOSベストロック)"),
};
// コマンドキャリア
static const CString mSimCmdCarr[] = {
	_T("動的補償"),
	_T("静的補償(センター)"),
	_T("静的補償(ノミナル)"),
	//_T("静的補償(AOSベストロック)"),
};

// CFrequencyCtrlSmTf ダイアログ

class CFrequencyCtrlSmTf : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSmTf)

public:
	CFrequencyCtrlSmTf(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlSmTf();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_SML_TF };

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

protected:
	bool	_getTxFqVal(char* pbuf);
	bool	_getRxFqVal(char* pbuf);
	bool	_getCmdFqVal(char* pbuf);
	bool	_getCmdFqValEx(CString pbuf);
	void	GetEqstatDbValue(CString& strst, int type);
	double	CalculatioFreq(time_t time, int type, int timetype, bool bPassUse = true);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 地上局送信周波数
	CString m_strTFreq;
	CString m_strRFreq;
	// コマンドキャリア周波数
	CString m_strCFreq;
	// ドップラ補償-地上局送信ドップラ
	CComboBox m_TDopplerCtrl;
	// ドップラ補償-コマンドキャリアドップラ
	CComboBox m_CDopplerCtrl;

	afx_msg void OnBnClickedFreqCtrlSmlTfCalcBtn();
	// 指定時刻（Day）
	CTime m_SpecDay;
	// 指定時刻（Time）
	CTime m_SpecTime;
	CDateTimeCtrl m_SpecDayDTCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFreqCtrlSmlTfNominalBtn();
	afx_msg void OnBnClickedFreqCtrlSmlCfNominalBtn();
	afx_msg void OnBnClickedFreqCtrlSmlTfcfCalcBtn();
	CString m_strSimValue[mItemRows][mItemCols];
};
