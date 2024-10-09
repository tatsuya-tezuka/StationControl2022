#pragma once
#include "afxwin.h"


// CFrequencyCtrlCtrl �_�C�A���O

// �T���@���[�h
static const CString mSatMode[] = {
	_T("INCOH"),
	_T("COH-880/749"),
	_T("COH-3344/749"),
	_T("COH-3328/749"),
	_T("COH-3360/749"),
};
// ���M�h�b�v��
static const CString mTxDopp[] = {
	_T("���I�⏞"),
	_T("�ÓI�⏞(�Z���^�[)"),
	_T("�ÓI�⏞(�m�~�i��)"),
	_T("�ÓI�⏞(AOS�x�X�g���b�N)"),
};
// ��M�h�b�v��
static const CString mRxDopp[] = {
	_T("1WAY"),
	_T("2WAY"),
};
// �R�}���h�L�����A
static const CString mCmdCarr[] = {
	_T("���I�⏞"),
	_T("�ÓI�⏞(�Z���^�[)"),
	_T("�ÓI�⏞(�m�~�i��)"),
	_T("�ÓI�⏞(MOD ON�x�X�g���b�N)"),
};
static const CString mCmdCtrl[] = {
	_T("ON"),
	_T("AVE"),
	_T("NOMINAL"),
	_T("STT"),
};

class CFrequencyCtrlCtrl : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlCtrl)

public:
	CFrequencyCtrlCtrl(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CFrequencyCtrlCtrl(UINT IDD, CWnd* pParent = NULL);
	virtual ~CFrequencyCtrlCtrl();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_CTRL };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum{
		eControl_COH,
		eControl_UC,
		eControl_DC,
		eControl_CS,
		eControl_TIMELOCK,
		eControl_TDP,
		eControl_RDP,
		eControl_CDP,
		eControl_TOFFSET,
		eControl_ROFFSET,
		eControl_TIMEOFFSET,
		eControl_ALLSET,
	};
	enum{
		eCOMBO,
		eEDIT,
		eNONE,
	};
	enum{
		ePhase_chk,
		ePhase_send,
		ePhase_end,
	};
	typedef struct{
		const CString		ctlname1;		// ���䖼
		const CString		ctlname2;		// ���䖼
		UINT				control;		// �R���g���[��ID
		UINT				type;			// ��ʁF0:COMBO 1:EDIT 2:NONE
	} stSetCtlData;

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
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	virtual BOOL UpdateDlg();

protected:
	void SetData(int type);
	bool CheckCtrlValRange(CString& strCtrl, CString& strVal, CString& strTitle);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �ݒ���g��-U/C
	CString m_strUC;
	// �ݒ���g��-D/C
	CString m_strDC;
	// �ݒ���g��-�R�}���h�L�����A
	CString m_strCmdCareer;
	// �I�t�Z�b�g-���M���g���I�t�Z�b�g
	CString m_strTFreqOffset;
	// �I�t�Z�b�g-�����I�t�Z�b�g
	CString m_strTimeOffset;
	CString m_strTimeOffsetPre;
	// �h�b�v���⏞-���M�h�b�v��
	CComboBox m_TDopplerCtrl;
	// �h�b�v���⏞-��M�h�b�v��
	CComboBox m_RDopplerCtrl;
	// �h�b�v���⏞-�R�}���h�L�����A
	CComboBox m_DopplerCmdCareerCtrl;
	// �T���@COH���[�h
	CComboBox m_SpacecraftCOHModeCtrl;
	afx_msg void OnBnClickedFreqCtrlCtrlSetUcBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlSetCmdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsTfBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsToBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpTdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpRdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpCmdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlScmBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFreqCtrlCtrlSetCmdTimeBtn();
	CString m_strRFreqOffset;
	afx_msg void OnBnClickedFreqCtrlCtrlOfsTfBtn2();
	afx_msg void OnEnChangeFreqCtrlCtrlOfsToEdit();
};
