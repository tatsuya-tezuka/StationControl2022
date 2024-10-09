#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "atltime.h"


// CEquipmentCtrlBitRate ダイアログ

class CEquipmentCtrlBitRate : public CDialogEx
{
	DECLARE_DYNAMIC(CEquipmentCtrlBitRate)

public:
	CEquipmentCtrlBitRate(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEquipmentCtrlBitRate();

// ダイアログ データ
	enum { IDD = IDD_DLG_EQUIPCTRL_BITRATE };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct {
		CString satellite;
		CString bitrate;
		BOOL	bTime;
		CTime	time;
	}stBitRate;

protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:

protected:
	BOOL	m_bCmdBitRate;
	CString m_strBitRateCtrl;
	stBitRate m_stBitRate;
	CString m_strWindowTitle;

	map<CString, vector<CString>> m_SatBitRateMap;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void SetCmdBitRateFlg(BOOL flg) { m_bCmdBitRate = flg; }
	void SetBitRateCtrl(CString str) { m_strBitRateCtrl = str; }
	stBitRate GetBitRateSetting() { return m_stBitRate; }
	void SetWindowTitle(CString str)	{ m_strWindowTitle = str; }

protected:
	void GetBitRateList(CString satname);
	void GetBitRateListSat();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 衛星名
	CComboBox m_SatCmb;
	CString m_strSatCmb;
	// ビットレートリスト
	CListBox m_BitRateListBox;
	// 日時指定（DAY）
	CDateTimeCtrl m_DataTimeDayCtrl;
	CTime m_DataTimeDay;
	// 日時指定（Time）
	CDateTimeCtrl m_DateTimeTimeCtrl;
	CTime m_DataTimeTime;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	// 日時指定チェックボックス
	CButton m_SpecifyDateChkBtn;
	afx_msg void OnCbnSelchangeDlgEquipctrlBitrateSatCmb();
	afx_msg void OnBnClickedOk();
};
