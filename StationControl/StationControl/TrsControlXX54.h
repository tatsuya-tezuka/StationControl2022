#pragma once

#include "TrsControlXX.h"
#include "afxwin.h"
#include "NumericEdit.h"

// �^�C�}��`
#define TID_DELTAF_TRS		7
#define INTERVAL_DELTAF 	800 	//800ms����

// CTrsControlXX54 �_�C�A���O

class CTrsControlXX54 : public CTrsControlXX
{
	DECLARE_DYNAMIC(CTrsControlXX54)

public:
	CTrsControlXX54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTrsControlXX54();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TRSCTRL_XX54 };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
public:
	enum{
		eINT,
		eDOUBLE,
		eSTRING,
	};
	typedef struct{
		const CString		monname;		// �Ď���
		CString&			controlval;		// �R���g���[���l
		const CString		format;			// �t�H�[�}�b�g
		UINT				type;			// ��ʁF0:INT 1:FLOAT 2:DOUBLE 3:STRING
	} stSetMonData;

protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	CGroupBoxEx m_grDeltafRecovery;
	CGroupBoxEx m_grDelfafTimeRate;

protected:
	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

protected:
	void SetData();
	BOOL CheckTimeSec(CString second);
	BOOL CheckFrequencyRate(CString freq);
	BOOL CheckFrequencyInput(CString freq);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	/* ------------------------------------------------------------------------------------ */
	CString m_DeltafCtrlFreqMoni;       //��f�iMON�j
	BOOL m_DeltafCtrlFreqCtrChkBtn;     //��f�iCTRL�j�`�F�b�N�{�b�N�X
	CButton m_DeltafCtrlChkBtnCtrl;
	CNumericEdit m_strDeltafCtrl;       //��f�iCTRL�j
	CButton m_DeltafCtrlTimeChkBtn;     //���䎞�ԃ`�F�b�N�{�b�N�X
	CEdit m_strDeltafTime;              //���䎞��
	CString m_strPreDeltafTime;			//�O��̐��䎞��
	CButton m_DeltafCtrlFreqChkBtn;     //������g���`�F�b�N�{�b�N�X
	CNumericEdit m_strDeltafFreq;       //������g��
	CGroupBoxEx m_grRecovery;
	CColorButtonEx m_RecoveryOffBtn;    //OFF
	CColorButtonEx m_RecoveryOnBtn;     //ON
	CString m_RecoveryOffOn;
	CString m_strRngFlg;
	CString m_DeltafCtrlFreqEndTime;    //�I���\�莞��
	BOOL m_RecoveryCont;				//���J�o���[���t���O
	BOOL m_RngFlg;						//�����W�v�����t���O
	BOOL m_strDeltafCtrlInit;			//��f�iCTRL�j�����l�ݒ�t���O(TRUE:�����l�ݒ��)

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTrxxctrlRfOffBtn();
	afx_msg void OnBnClickedTrxxctrlRfOnBtn();
	afx_msg void OnBnClickedTrxxctrlNouseOffBtn();
	afx_msg void OnBnClickedTrxxctrlNouseOnBtn();
	afx_msg void OnBnClickedTrxxctrlSweetStopBtn();
	afx_msg void OnBnClickedTrxxctrlSweetIncBtn();
	afx_msg void OnBnClickedTrxxctrlSweetIncdecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetDecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetDecincBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubStopBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubIncBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubIncdecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubDecBtn();
	afx_msg void OnBnClickedTrxxctrlSweetSubDecincBtn();
	afx_msg void OnBnClickedTrxxctrlXxcohCohBtn();
	afx_msg void OnBnClickedTrxxctrlXxcohIncohBtn();
	afx_msg void OnBnClickedTrxxctrlRngingStopBtn();
	afx_msg void OnBnClickedTrxxctrlRngingStartBtn();
	afx_msg void OnBnClickedTrxxctrlRgntypeStopBtn();
	afx_msg void OnBnClickedTrxxctrlRgntypeStartBtn();
	afx_msg void OnBnClickedStaticDeltafRecoveryBox();
	afx_msg void OnBnClickedRecoveryOffBtn();
	afx_msg void OnBnClickedRecoveryOnBtn();
	afx_msg void OnBnClickedDeltafCtrlFreqCtrChk();
	afx_msg void OnBnClickedDeltafCtrlTimeChk();
	afx_msg void OnBnClickedDeltafCtrlFreqChk();
	afx_msg void OnEnChangeTrxxctrlDeltafTimeEdit();
	afx_msg void OnBnClickedTrxxctrlHoldOffBtn();
	afx_msg void OnBnClickedTrxxctrlHoldOnBtn();
};
