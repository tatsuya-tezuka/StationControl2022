#pragma once
#include "SpectrumCommonDlg.h"

// CSpectrumView �_�C�A���O

class CSpectrumView : public CSpectrumCommonDlg
{
	DECLARE_DYNAMIC(CSpectrumView)

public:
	CSpectrumView(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSpectrumView();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SPECTRUMVIEW };


protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStnClickedStaticSpectrumviewArea();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSpectrumverticalsetButton();
	/* ------------------------------------------------------------------------------------ */
};
