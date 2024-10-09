#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "SpectrumGraphView.h"
#include "ColorStringComboBox.h"
#include "..\Utility\ColorControl.h"
#include "SatelliteData.h"

// CSpectrumCommonDlg ダイアログ

class CSpectrumCommonDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSpectrumCommonDlg)

public:
	CSpectrumCommonDlg(UINT IDD = 0, CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSpectrumCommonDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eDBUnit{
		edBm = 0,
	};

	enum eFreqUnit{
		eHz = 0,
		ekHz,
		eMHz
	};

	enum eColor{
		eBlack = 0,
		eOrange,
		eBlue,
		eRed,
		eGreen,
		eColorNum
	};

	COLORREF m_colText[eColorNum];

	static const int m_Grid = 10;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString m_strPeakLevelUnit;
	CString m_strPeakFreqUnit;
	CString m_strHLValue;
	CString m_strHCValue;
	CString m_strHRValue;
	CMFCMaskedEdit m_CEditCenterFreqOffset;
	CMFCMaskedEdit m_CEditInputExchg;
	CMFCMaskedEdit m_CEditAtt;
	CSpinButtonCtrl m_CSpinCenterFreqOffset;
	CSpinButtonCtrl m_CSpinInputExchg;
	CSpinButtonCtrl m_CSpinAtt;
	CString m_strDataTime;
	CString m_strPeakLevel;
	CString m_strPeakFreq;
	CColorComboBoxEx m_CCBoxMonitorPoint;
	CColorComboBoxEx m_CCBoxBandWidth;

	CButton m_CButonReset;
	CString m_strCenterFreqOffset;
	CStatic m_viewArea;

	CSpectrumGraphView* m_pView;

//	COLORREF m_colEditMax;
//	COLORREF m_colEditMin;
//	COLORREF m_colEditGrid;
	COLORREF m_colEditCenterFreqOffset;
	COLORREF m_colEditInputExchg;
	COLORREF m_colEditAtt;

	int m_nMonitorPointCurSel;
	int m_nBandWidthCurSel;

	UINT		m_nViewID;				// ビューID
	UINT		m_nStationID;			// 局ID
	UINT		m_nGraphType;			// グラフタイプ

	BOOL m_bMPModified;
	BOOL m_bBWModified;
	BOOL m_bCFOfstModified;
	BOOL m_bMPMListOpen;
	BOOL m_bBWMListOpen;
	BOOL m_bCFOfstMListOpen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL InitializeDlg(UINT nStationID, UINT nActiveView, UINT nGraphType);
	void CtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void VScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSpectrumViewData* GetSpectrumViewData();

	// レイアウト読み込み＆保存（設定情報）
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);

	BOOL UpdateCtrl();
	void ClearAutoVMaxMin()
	{
		if (m_pView != NULL)
		{
			m_pView->ClearAutoVMaxMin();
		}
	}

protected:
	void ResetCenterFreqOffset();
	int ExchangeFreqUnit(double& src, double& dst);
//	int FileRead();
	int SendShell(CString& strCtrl, CString& strVal);
	CString ExchgCtrlValSpecFreOffset54m(double freq);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDeltaposSpectrumviewSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSpectrumviewButton1();
	afx_msg void OnDeltaposSpectrumviewSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpectrumviewSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpectrumviewSpin3(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnEnChangeSpectrumviewEdit1();
//	afx_msg void OnEnChangeSpectrumviewEdit2();
//	afx_msg void OnEnChangeSpectrumviewEdit3();
	afx_msg void OnEnChangeSpectrumviewEdit7();
	afx_msg void OnMenuClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedSpectrumExecButton();
	afx_msg void OnBnClickedSpectrumResetButton();
	afx_msg void OnBnClickedSpectrumInputexchgExecButton();
	afx_msg void OnBnClickedSpectrumInputexchgResetButton();
	afx_msg void OnDeltaposSpectrumInputexchgspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpectrumAttspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeSpectrumInputexchgedit();
	afx_msg void OnEnChangeSpectrumAttedit();
	afx_msg void OnCbnSelchangeSpectrumviewCombo1();
	afx_msg void OnCbnDropdownSpectrumviewCombo1();
	afx_msg void OnCbnCloseupSpectrumviewCombo1();
	afx_msg void OnCbnSelchangeSpectrumviewCombo2();
	afx_msg void OnCbnDropdownSpectrumviewCombo2();
	afx_msg void OnCbnCloseupSpectrumviewCombo2();
	/* ------------------------------------------------------------------------------------ */
};
