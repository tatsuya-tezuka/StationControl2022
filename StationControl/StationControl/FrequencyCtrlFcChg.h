#pragma once
#include "afxwin.h"
#include "atltime.h"
#include "afxdtctl.h"


// CFrequencyCtrlFcChg �_�C�A���O

static const CString mCtrlFcChgTxDopp[] = {
	_T("���I�⏞"),
	_T("�ÓI�⏞(�Z���^�[)"),
	_T("�ÓI�⏞(�m�~�i��)"),
	_T("�ÓI�⏞(AOS�x�X�g���b�N)"),
	_T("")
};

class CFrequencyCtrlFcChg : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlFcChg)

public:
	CFrequencyCtrlFcChg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlFcChg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_FCCHG };

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
	CWnd*		mCallWnd;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void	Initialize();
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	virtual BOOL UpdateDlg();

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �T���@
	CComboBox m_SpacecraftCmb;
	// ���M�h�b�v���⏞
	CComboBox m_TDoppCmb;
	// �p�X�J�n�����iDay�j
	CTime m_PassStartDay;
	// �p�X�J�n�����iTime�j
	CTime m_PassStartTime;
	// �p�X�I�������iDay�j
	CTime m_PassEndDay;
	// �p�X�I�������iTime�j
	CTime m_PassEndTime;
	CDateTimeCtrl m_PassStartDTCtrl;
	CDateTimeCtrl m_PassEndDTCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFreqCtrlFcchgAntfcBtn();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CButton m_AntPredBtnCtrl;
	afx_msg void OnBnClickedFreqCtrlFcchgBtn();
	afx_msg void OnBnClickedFreqCtrlOpeBtn();
	afx_msg void OnCbnSelchangeFreqCtrlFcchgSpcrCmb();
	afx_msg void OnCbnSelchangeFreqCtrlFcchgTdpCmb();
	int m_nTxDopp;
	afx_msg void OnDtnDatetimechangeFreqCtrlFcchgPasstDayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeFreqCtrlFcchgPasstTimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeFreqCtrlFcchgPasetDayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeFreqCtrlFcchgPasetTimeDp(NMHDR *pNMHDR, LRESULT *pResult);
};
