#pragma once
#include "atltime.h"
#include "afxdtctl.h"


// CFrequencyCtrlSmRf �_�C�A���O

class CFrequencyCtrlSmRf : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSmRf)

public:
	CFrequencyCtrlSmRf(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlSmRf();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_SML_RF };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �n��ǎ�M���g��
	CString m_strRFreq;
	afx_msg void OnBnClickedFreqCtrlSmlRfDpcalcBtn();
	afx_msg void OnBnClickedFreqCtrlSmlRfDpviewBtn();
	afx_msg void OnBnClickedFreqCtrlSmlRfCalcBtn();
	// �w�莞���iDay�j
	CTime m_SpecDay;
	// �w�莞���iTime�j
	CTime m_SpecTime;
	CDateTimeCtrl m_SpecDayDTCtrl;
	virtual BOOL OnInitDialog();
};
