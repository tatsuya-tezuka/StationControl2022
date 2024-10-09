#pragma once

#include "SpectrumVerticalSetCommonDlg.h"
#include "SpectrumView.h"

// CSpectrumVerticalSetDlg �_�C�A���O

class CSpectrumVerticalSetDlg : public CSpectrumVerticalSetCommonDlg
{
	DECLARE_DYNAMIC(CSpectrumVerticalSetDlg)

public:
	CSpectrumVerticalSetDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CSpectrumVerticalSetDlg(CWnd* pParent, CSpectrumView* pSpectrumView);
	virtual ~CSpectrumVerticalSetDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SPECTRUMVERTICALSET };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CSpectrumView* m_pSpectrumView;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

//	BOOL SetVerticalValues();
//	void SetEnableVAxis(BOOL bEnable);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeSpectrumverticalsetCombo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	/* ------------------------------------------------------------------------------------ */
};
