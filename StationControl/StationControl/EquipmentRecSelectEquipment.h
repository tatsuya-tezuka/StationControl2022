#pragma once
#include "afxwin.h"
#include "EquipmentRecDumpOutput.h"

// CEquipmentRecSelectEquipment ダイアログ


#define COMMENT_DEV			_T("/* ダンプ対象装置名 */")
#define COMMENTSTART_TIME   _T("/* ダンプ開始時刻 */")
#define COMMENTSTOP_TIME    _T("/* ダンプ終了時刻 */")
#define COMMENTWATCH        _T("/* ダンプ対象監視項目 */")

class CEquipmentRecSelectEquipment : public CDialogBase
{
	DECLARE_DYNAMIC(CEquipmentRecSelectEquipment)

public:
	CEquipmentRecSelectEquipment(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEquipmentRecSelectEquipment();

// ダイアログ データ
	enum { IDD = IDD_DLG_EQIPREC_SELEQIP };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CString> m_EquipNameList;
	map<CString, vector<CString>> m_EquipMonNameList;

protected:
	CWnd* m_pParent;



	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:

protected:
	CString GetTotalDay(CTime time);
	CString GetMonName(CString mon);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_EquipList;
	afx_msg void OnBnClickedEqiprecSeleqipOutputBtn();
};
