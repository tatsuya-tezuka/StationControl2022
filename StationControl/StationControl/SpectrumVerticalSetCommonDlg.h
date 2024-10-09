#pragma once

#define SPECTRUM_VMAX 0
#define SPECTRUM_VMIN -999

// CSpectrumVerticalSetCommonDlg �_�C�A���O

class CSpectrumVerticalSetCommonDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSpectrumVerticalSetCommonDlg)

public:
	CSpectrumVerticalSetCommonDlg(UINT IDD = 0, CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSpectrumVerticalSetCommonDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const int m_VMax = SPECTRUM_VMAX;
	static const int m_VMin = SPECTRUM_VMIN;
	static const int m_VMaxGrid = m_VMax - m_VMin;

	enum eColor{
		eBlack = 0,
		eOrange,
		eBlue,
		eRed,
		eGreen,
		eColorNum
	};

	COLORREF m_colText[eColorNum];

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CColorComboBoxEx m_CCBoxAutoScaleOnOff;
	CMFCMaskedEdit m_CEditMax;
	CMFCMaskedEdit m_CEditMin;
	CMFCMaskedEdit m_CEditGrid;

	BOOL m_bLastAutoScale;
	long m_gl_last_y_grid_max;
	long m_gl_last_y_grid_min;
	long m_gl_last_y_grid_val;

	UINT		m_nViewID;				// �r���[ID
	UINT		m_nStationID;			// ��ID
	UINT		m_nGraphType;			// �O���t�^�C�v

	COLORREF m_colEditMax;
	COLORREF m_colEditMin;
	COLORREF m_colEditGrid;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

public:
	BOOL InitializeDlg(UINT nStationID, UINT nViewID, UINT nGraphType);
	CSpectrumViewData* GetSpectrumViewData();
	void CtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
	DECLARE_MESSAGE_MAP()

	BOOL SetVerticalValues();
	void SetEnableVAxis(BOOL bEnable);

public:
	afx_msg void OnCbnSelchangeSpectrumverticalsetCombo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeVMax();
	afx_msg void OnEnChangeVMin();
	afx_msg void OnEnChangeVGrid();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	/* ------------------------------------------------------------------------------------ */
};
