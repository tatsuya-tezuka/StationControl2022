#pragma once
#include "afxcmn.h"
#include "AgcIntgListCtrl.h"
#include "afxwin.h"

static const CString strStepComb[] = {
	_T("1step 毎"),
	_T("3step 毎"),
	_T("5step 毎"),
	_T("デフォルトに戻す"),
	_T("")
};


static const int STEP[] = {
	1,
	3,
	5,
};

enum{
	TYPE_DFLT,
	TYPE_CUST,
};

static const CString TypeFile[] = {
	_T("DFLT"),
	_T("CUST"),
};


// CRdnsAgcDetailStep ダイアログ

class CRdnsAgcDetailStep : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsAgcDetailStep)

public:
	CRdnsAgcDetailStep(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CRdnsAgcDetailStep();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDNS_AGCSTEP };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum {
		STEP_1,
		STEP_3,
		STEP_5,
		STEP_DEF,
	};

	typedef struct {
		CString att;		// ATT値
		CString calib;		// 校正値
		CString u_limit;	// 上限許容値
		CString l_limit;	// 下限許容値
		CString incr;		// ATT増分
		CString corr;		// 校正修正値
		CString wait;		// 校正待ち時間
		CString intg;		// 積分時間
		CString enable;		// 有効無効
		CString coment;		// コメント
	} stAgcTbl;

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
	CString	m_Station;
	CString m_Satellite;
	CString m_AgcItem;
	CString m_LoopBand;

protected:
	int m_CurrentStep;
	map<CString, stAgcTbl> m_AgcTblCustMap;
	map<CString, stAgcTbl> m_AgcTblDfltMap;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
protected:
	BOOL ReadAgcTblFile(int type, map<CString, stAgcTbl> &map);
	BOOL WriteAgcTblFile();
	
	void SetDefaultValue();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CAgcIntgListCtrl m_IntgListCtrl;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRdnsAgcStepIntgBtn();
	CEdit m_IntegEdit;
	CComboBox m_CalibCmb;
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeRdnsAgcStepPrfCmb();
	afx_msg void OnBnClickedOk();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
