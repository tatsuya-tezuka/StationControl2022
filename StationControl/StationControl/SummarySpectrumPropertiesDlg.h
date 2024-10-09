#pragma once

#include "SpectrumCommonDlg.h"
#include "SpectrumVerticalSetCommonDlg.h"
#include "SpectrumView.h"

// CSummarySpectrumPropertiesDlg �_�C�A���O

class CSummarySpectrumPropertiesDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSummarySpectrumPropertiesDlg)

public:
	CSummarySpectrumPropertiesDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CSummarySpectrumPropertiesDlg(CWnd* pParent, UINT nViewID, vector<CString> listViewName, UINT nStaionID, 
		UINT nParentID = 0, CString strWndName = _T(""), CWnd* pSummaryProp = NULL, BOOL bChgDlg = FALSE);
	virtual ~CSummarySpectrumPropertiesDlg();


	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SUMMARYSPECTRUMPROPERTIES };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CSpectrumCommonDlg	m_spectrumSet;
	CSpectrumVerticalSetCommonDlg	m_verticalSet;

	int			m_ActiveView;			// �r���[ID
	int			m_nParentID;			// �e�E�B���h�EID
	UINT		m_nStationID;			// ��ID
	CString		m_strWndName;			// �E�B���h�E��
	vector<CString>		m_listViewName;	// �O���t�r���[�����X�g
	CWnd* m_pSummaryProp;				// �T�}���v���p�e�B�E�B���h�E�|�C���^

	BOOL m_bChgGraphPropDlg;			// �O���t�v���p�e�B�_�C�A���O�ύX

	CComboBox				m_wndObjectCombo;	// �O���t�I���R���{�{�b�N�X

	BOOL m_bChgDlg;						// �_�C�A���O��ޕύX
	CPoint m_ptWnd;						// �E�B���h�E�ʒu

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void ResetCenterFreqOffset();
	int ExchangeFreqUnit(double& src, double& dst);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
