#pragma once
#include "afxwin.h"
#include "NumericEdit.h"


// CEquipmetCtrlNum ダイアログ

class CEquipmentCtrlNum : public CDialogEx
{
	DECLARE_DYNAMIC(CEquipmentCtrlNum)

public:
	CEquipmentCtrlNum(CWnd* pParent = NULL);   // 標準コンストラクター
	CEquipmentCtrlNum(UINT IDD, CWnd* pParent = NULL);	// 継承用コンストラクター
	virtual ~CEquipmentCtrlNum();

// ダイアログ データ
	enum { IDD = IDD_DLG_EQUIPCTRL_NUM };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
	CString	m_RangeCur;
protected:
	long	m_Type;
	double	m_RangeMin;
	double	m_RangeMax;
	CString m_strWindowTitle;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void SetRang(CString cur, double min, double max) { m_RangeCur = cur;  m_RangeMin = min; m_RangeMax = max; }
	void SetEquipment(CString eqip, long type) { m_strEquipment = eqip; m_Type = type; }
	void SetRangeText(CString range) { m_strRange = range; }
	void SetWindowTitle(CString str)	{ m_strWindowTitle = str; }

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
protected:
	CString m_strEquipment;
	CString m_strRange;
public:
	CString m_strVal;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CNumericEdit m_CtrlValueEdit;
};
