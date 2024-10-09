#pragma once

// CAntStanbyOriented ダイアログ(54m)

class CAntStanbyOriented : public CDialogBase
{
	DECLARE_DYNAMIC(CAntStanbyOriented)

public:
	CAntStanbyOriented(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CAntStanbyOriented();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_ANTSTANBYORIENTED };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	UpdateSatellite(BOOL update);

protected:
	CButton		m_buttonAntForecast;		// アンテナ予報値ボタンコントロール
	CString		m_strPassStart;				// パス開始時刻
	CString		m_strPassEnd;				// パス終了時刻
	CButton		m_buttonExec;				// 実行ボタンコントロール
	CComboBox	m_SatelliteCmb;				// 探査機コンボボックス
	CString		m_satellilteName;			// 探査機名
	vector<CString> mSatelliteNameList;		// 探査機一覧
	vector<stPredListData> mPartPredList;	// 予報値一覧(選択中探査機)

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL OnInitDialog();
	void SetFileName(CString filepath);
	void GetKanshiData(CString fname);
	void UpdateDlg(CString fname);

protected:
	void GetSatelliteNameList();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeSatelliteCmb();
	afx_msg void OnBnClickedSatellitecaptureAntforecastButton();
	afx_msg void OnBnClickedAntstanbyorientedExecButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
