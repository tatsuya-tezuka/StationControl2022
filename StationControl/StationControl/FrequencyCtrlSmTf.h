#pragma once
#include "atltime.h"
#include "afxdtctl.h"

enum{
	eDoppOn,
	eDoppOffCenter,
	eDoppOffNomi,
	eDoppOffAos,
};

// ���M�h�b�v��
static const CString mSimTxDopp[] = {
	_T("���I�⏞"),
	_T("�ÓI�⏞(�Z���^�[)"),
	_T("�ÓI�⏞(�m�~�i��)"),
	_T("�ÓI�⏞(AOS�x�X�g���b�N)"),
};
// �R�}���h�L�����A
static const CString mSimCmdCarr[] = {
	_T("���I�⏞"),
	_T("�ÓI�⏞(�Z���^�[)"),
	_T("�ÓI�⏞(�m�~�i��)"),
	//_T("�ÓI�⏞(AOS�x�X�g���b�N)"),
};

// CFrequencyCtrlSmTf �_�C�A���O

class CFrequencyCtrlSmTf : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSmTf)

public:
	CFrequencyCtrlSmTf(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlSmTf();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_SML_TF };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mItemRows = 4;
	static const UINT mItemCols = 4;
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	CWnd*		mCallWnd;
	CTrendData	mTrendData;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }

protected:
	bool	_getTxFqVal(char* pbuf);
	bool	_getRxFqVal(char* pbuf);
	bool	_getCmdFqVal(char* pbuf);
	bool	_getCmdFqValEx(CString pbuf);
	void	GetEqstatDbValue(CString& strst, int type);
	double	CalculatioFreq(time_t time, int type, int timetype, bool bPassUse = true);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �n��Ǒ��M���g��
	CString m_strTFreq;
	CString m_strRFreq;
	// �R�}���h�L�����A���g��
	CString m_strCFreq;
	// �h�b�v���⏞-�n��Ǒ��M�h�b�v��
	CComboBox m_TDopplerCtrl;
	// �h�b�v���⏞-�R�}���h�L�����A�h�b�v��
	CComboBox m_CDopplerCtrl;

	afx_msg void OnBnClickedFreqCtrlSmlTfCalcBtn();
	// �w�莞���iDay�j
	CTime m_SpecDay;
	// �w�莞���iTime�j
	CTime m_SpecTime;
	CDateTimeCtrl m_SpecDayDTCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFreqCtrlSmlTfNominalBtn();
	afx_msg void OnBnClickedFreqCtrlSmlCfNominalBtn();
	afx_msg void OnBnClickedFreqCtrlSmlTfcfCalcBtn();
	CString m_strSimValue[mItemRows][mItemCols];
};
