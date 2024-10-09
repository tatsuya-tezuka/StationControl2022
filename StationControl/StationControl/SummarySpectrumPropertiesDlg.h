#pragma once

#include "SpectrumCommonDlg.h"
#include "SpectrumVerticalSetCommonDlg.h"
#include "SpectrumView.h"

// CSummarySpectrumPropertiesDlg ダイアログ

class CSummarySpectrumPropertiesDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSummarySpectrumPropertiesDlg)

public:
	CSummarySpectrumPropertiesDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	CSummarySpectrumPropertiesDlg(CWnd* pParent, UINT nViewID, vector<CString> listViewName, UINT nStaionID, 
		UINT nParentID = 0, CString strWndName = _T(""), CWnd* pSummaryProp = NULL, BOOL bChgDlg = FALSE);
	virtual ~CSummarySpectrumPropertiesDlg();


	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SUMMARYSPECTRUMPROPERTIES };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CSpectrumCommonDlg	m_spectrumSet;
	CSpectrumVerticalSetCommonDlg	m_verticalSet;

	int			m_ActiveView;			// ビューID
	int			m_nParentID;			// 親ウィンドウID
	UINT		m_nStationID;			// 局ID
	CString		m_strWndName;			// ウィンドウ名
	vector<CString>		m_listViewName;	// グラフビュー名リスト
	CWnd* m_pSummaryProp;				// サマリプロパティウィンドウポインタ

	BOOL m_bChgGraphPropDlg;			// グラフプロパティダイアログ変更

	CComboBox				m_wndObjectCombo;	// グラフ選択コンボボックス

	BOOL m_bChgDlg;						// ダイアログ種類変更
	CPoint m_ptWnd;						// ウィンドウ位置

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void ResetCenterFreqOffset();
	int ExchangeFreqUnit(double& src, double& dst);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeSummaryspectrumVerticalsetCombo();
	afx_msg void OnBnClickedSummaryspectrumGridpropExecButton();
	afx_msg void OnBnClickedSummaryspectrumGridpropResetButton();
	afx_msg void OnBnClickedSummaryspectrumviewCenterButton();
	afx_msg void OnBnClickedSummaryspectrumCtrlExecButton();
	afx_msg void OnBnClickedSummaryspectrumCtrlResetButton();
	afx_msg void OnBnClickedSummaryspectrumInputattExecButton();
	afx_msg void OnBnClickedSummaryspectrumInputattResetButton();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDeltaposSummaryspectrumviewCenterfreqoffsetSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSummaryspectrumInputexchgspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSummaryspectrumAttspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeSummaryspectrumviewYmaxEdit();
	afx_msg void OnEnChangeSummaryspectrumviewYminEdit();
	afx_msg void OnEnChangeSummaryspectrumviewYgridEdit();
	afx_msg void OnEnChangeSummaryspectrumviewCenterfreqoffsetEdit();
	afx_msg void OnEnChangeSummaryspectrumInputexchgedit();
	afx_msg void OnEnChangeSummaryspectrumAttedit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeSummarygrapthSelectgraphCombo();
	afx_msg void OnCbnSelchangeSummaryspectrumviewMonpointCombo();
	afx_msg void OnCbnSelchangeSummaryspectrumviewBandwidthCombo();
	afx_msg void OnMenuClose();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/* ------------------------------------------------------------------------------------ */
};
