#pragma once
#include "afxwin.h"
#include "atltime.h"
#include "EquipmentRecSelectEquipment.h"
#include "afxdtctl.h"


// CEquipmentRecDumpOutput ダイアログ

class CEquipmentRecDumpOutput : public CDialogBase
{
	DECLARE_DYNAMIC(CEquipmentRecDumpOutput)

public:
	CEquipmentRecDumpOutput(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEquipmentRecDumpOutput();

// ダイアログ データ
	enum { IDD = IDD_DLG_EQIPREC_DUMPOUTPUT };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CString> m_EquipNameListTemp;

protected:
	map<CString, vector<CString>> m_EquipMonNameList;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

	map<CString, vector<CString>>& GetEquipMonNameList() { return m_EquipMonNameList;}

protected:
	void	ResetList(CString name);
	void	ResetMonMapList();
	void	SetDlgData(CString mon);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 設備選択
	CComboBoxEx2 m_EquipmentSelCmb;
	// ファイル識別
	CString m_strFileId;
	// 出力開始時刻-日付
	CTime m_StartTimeD;
	// 出力開始時刻-時刻
	CTime m_StartTimeT;
	// 出力終了時刻-日付
	CTime m_EndTimeD;
	// 出力開始時刻-時刻
	CTime m_EndTimeT;
	// 検索頻度
	CEdit m_SearchTimeEdit;
	// ステータス出力方法
	CComboBox m_StatusOutputCmb;
	// 監視データリスト
	CListBox m_MonitoringDataList;
	// 直接入力
	CString m_strDirectEntry;
	afx_msg void OnBnClickedEqiprecDumpoutputFidSaveBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputFidReadBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputMntdUpBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputMntdDwnBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputMntdDelBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputDirectAddBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputExecBtn();
	afx_msg void OnMenuCloseClose();
	CDateTimeCtrl m_StartTimeDTCtrl;
	CDateTimeCtrl m_EndTimeDTCtrl;
	afx_msg void OnBnClickedEqiprecDumpoutputDirectSerchBtn();
	afx_msg void OnCbnSelchangeEqiprecDumpoutputEqipselCmb();
	afx_msg void OnEnChangeEqiprecDumpoutputFidEdit();
	CButton m_SaveBtnCtrl;
	CDateTimeCtrl m_StartTimeTTCtrl;
	CDateTimeCtrl m_EndTimeTTCtrl;
};
