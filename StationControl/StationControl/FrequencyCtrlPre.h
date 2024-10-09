#pragma once
#include "afxwin.h"
#include "atltime.h"
#include "afxdtctl.h"
#include "NumericEdit.h"

// 送信周波数取得先
typedef struct{
	const CString		sendname;		// 相手先名称
	UINT				sendid;			// 相手先ID
} stSendFreq;
static const stSendFreq mSenderFreq[eStation_MAX][2] = {
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

// CFrequencyCtrlPre ダイアログ

class CFrequencyCtrlPre : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlPre)

public:
	CFrequencyCtrlPre(CWnd* pParent = NULL);   // 標準コンストラクター
	CFrequencyCtrlPre(UINT IDD, CWnd* pParent = NULL);
	virtual ~CFrequencyCtrlPre();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_PRESET };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
public:
	enum{
		ePre_1WAY,		// 1WAY運用(テレメ単独運用)
		ePre_2WAY1,		// 2WAY運用(COH、動的)
		ePre_2WAY2,		// 2WAY運用(COH、センター)
		ePre_2WAY3,		// 2WAY運用(INCOH、動的)
		ePre_2WAY4,		// 2WAY運用(INCOH、センター)
		ePre_3WAY,		// 3WAY
	};
	typedef struct{
		const CString		ctlname;		// 制御名
	} stSetCtlData;
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	CWnd*		mCallWnd;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	virtual BOOL UpdateDlg();

protected:
	void	SetData(int type);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPre1way();
	afx_msg void OnBnClickedButtonPre2way1();
	afx_msg void OnBnClickedButtonPre2way2();
	afx_msg void OnBnClickedButtonPre2way3();
	afx_msg void OnBnClickedButtonPre2way4();
	afx_msg void OnBnClickedButtonPre3way();
	CString m_strSendOp;
	CNumericEdit m_SendOpEdit;
	CComboBox m_cbSendGet;
	int m_nSendGet;
	afx_msg void OnBnClickedButtonGet();
};
