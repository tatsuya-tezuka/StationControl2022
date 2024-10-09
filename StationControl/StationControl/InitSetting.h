#pragma once


// CInitSetting ダイアログ

class CInitSetting : public CDialogBase
{
	DECLARE_DYNAMIC(CInitSetting)

public:
	CInitSetting(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CInitSetting();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_INITSETTING };

	// 初期設定
	typedef struct {
		DWORD initID;					// 初期設定ID
		CString initName;				// 初期設定対象名
	} structInitSetting;

	typedef struct {
		DWORD id;									// 衛星ID
		vector<structInitSetting> listInitSetting;	// 初期設定
	} structSatelliteInitSetting;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CComboBox m_CCBoxSatelliteName;								// 衛星名コンボボックス
	CButton m_CButonSelCid;										// CID選択ボタン
	CListBox m_listInitSetting;									// 初期設定リストボックス
	CString m_strSatelliteName;									// 衛星名
	CButton m_chkAllSelect;										// 全選択

	vector<CString> m_listSatellite;							// 衛星リスト
	map<CString, vector<CString>> m_listInitMacr;				// 初期設定マクロ定義リスト

	CArray<int, int> m_aryListBoxSel;							// 選択リストボックス
	CString m_satSel;											// 選択衛星
	int m_aryListBoxPos;										// リストボックス位置

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL InitDlg();
	void RestListBox(CString sat);
	void RestCIDBtn(CString sat);

public:
	virtual BOOL UpdateDlg();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeInitsettingCombo1();
	afx_msg void OnBnClickedInitsettingCidButton();
	afx_msg void OnBnClickedInitsetingCheck();
	BOOL m_CidChkBox;
	afx_msg void OnBnClickedCidcheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	/* ------------------------------------------------------------------------------------ */
};
