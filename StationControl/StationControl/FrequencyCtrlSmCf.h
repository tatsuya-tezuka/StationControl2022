#pragma once
#include "atltime.h"
#include "afxdtctl.h"


// CFrequencyCtrlSmCf �_�C�A���O

class CFrequencyCtrlSmCf : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSmCf)

public:
	CFrequencyCtrlSmCf(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlSmCf();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_SML_CF };

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
	// �R�}���h�T�u�L�����A���g��
	CString m_strCmdFreq;
	afx_msg void OnBnClickedFreqCtrlSmlCfDpcalcBtn();
	afx_msg void OnBnClickedFreqCtrlSmlCfDpviewBtn();
	afx_msg void OnBnClickedFreqCtrlSmlCfCalcBtn();
	// �w�莞���iDay�j
	CTime m_SpecDay;
	// �w�莞���iTime�j
	CTime m_SpecTime;
	CDateTimeCtrl m_SpecDayDTCtrl;
	virtual BOOL OnInitDialog();
};
