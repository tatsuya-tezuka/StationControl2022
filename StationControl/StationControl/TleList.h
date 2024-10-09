#pragma once


// CTleList ダイアログ

class CTleList : public CDialogBase
{
	DECLARE_DYNAMIC(CTleList)

public:
	CTleList(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTleList();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_TLELIST };

	enum eShikibetsu {
		eNOM = 0,
		eOFF
	};

	// リスト項目の表示幅
	static const int m_nSatelliteNameWidth = 100;		// 衛星名項目幅
	static const int m_nSikibetuWidth = 40;				// 識別項目幅
	static const int m_epochTimeWidth = 120;			// プリパス開始日時項目幅
	static const int m_propProcTimeWidth = 120;			// パス開始日時項目幅

	typedef struct {
		CString			m_satelliteName;		// 衛星名
		eShikibetsu		m_shikibetsu;			// 識別
		CTime			m_epochTime;			// EPOCH
		CTime			m_propProcTime;			// 伝搬処理日時
	} structListItem;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CListCtrl m_listTle;				// TLEリストコントロール
	BOOL m_bUpperSort;					// 昇順ソート
	int m_nSelectedColumn;				// 選択されたカラム
	CString m_strSatelliteName;			// 衛星名
	deque<structListItem> m_dequeItem;	// アイテムキュー
	CComboBox m_CCBoxSatelliteName;		// 衛星名コンボボックス

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL InitList();
	void InitComboBox();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int CompareFunc(LPARAM lParam1, LPARAM lParam2);
	BOOL SetSatelliteName();
	void CreateSampleData();

public:
	CString GetSatelliteName();
	BOOL InsertListItem();
	BOOL UpdateListItem();
	BOOL DeleteListItem();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnHdnItemclickTlelistList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTlelistButton1();
	afx_msg void OnBnClickedTlelistButton2();
	afx_msg void OnBnClickedTlelistButton3();
	afx_msg void OnBnClickedTlelistButton4();
	afx_msg void OnBnClickedTlelistButton5();
	afx_msg void OnBnClickedTlelistButton6();
	afx_msg void OnCbnSelchangeTlelistCombo();
	/* ------------------------------------------------------------------------------------ */
};
