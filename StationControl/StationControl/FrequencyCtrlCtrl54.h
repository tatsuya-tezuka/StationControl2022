#pragma once

#include "FrequencyCtrlCtrl.h"
#include "afxwin.h"
#include "NumericEdit.h"

// �^�C�}��`
#define TID_DELTAF_FREQ		8
#define INTERVAL_DELTAF 	800 	//800ms����

// CFrequencyCtrlCtrl54 �_�C�A���O

// D/C COH���[�h �\���p
static const CString mDcCOHMode[] = {
	_T("X-COH(880/749)"),
	_T("X-INCOH"),
	_T("Ka-COH(3328/749)"),
	_T("Ka-COH(3344/749)"),
	_T("Ka-COH(3360/749)"),
	_T("Ka-INCOH"),
};
// D/C COH���[�h ���䖼
static const CString mDcCOHModeCtl[] = {
	_T("X-COH_880/749"),
	_T("X-INCOH"),
	_T("Ka-COH_3328/749"),
	_T("Ka-COH_3344/749"),
	_T("Ka-COH_3360/749"),
	_T("Ka-INCOH"),
};

class CFrequencyCtrlCtrl54 : public CFrequencyCtrlCtrl
{
	DECLARE_DYNAMIC(CFrequencyCtrlCtrl54)

public:
	CFrequencyCtrlCtrl54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlCtrl54();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQUENCYCTRLCTRL54 };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
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
	} stGetMonData;

	typedef struct{
		const CString		ctlname1;		// ���䖼
		const CString		ctlname2;		// ���䖼
		const CString		ctlname3;		// ���䖼
		const CString		ctlname4;		// ���䖼
		const CString		title;			// �^�C�g��
		UINT				control;		// �R���g���[��ID
		UINT				type;			// ��ʁF0:COMBO 1:EDIT 2:NONE
	} stSetCtlData54;

	enum{
		eControl_COH,
		eControl_DC_COH,
		eControl_UC,
		eControl_X_DC,
		eControl_Ka_DC,
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

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString m_DeltafCtrlFreqMoni;       //��f�i���j�^�j
	BOOL m_DeltafCtrlFreqCtrChkBtn;     //��f�i����j�`�F�b�N�{�b�N�X
	CButton m_DeltafCtrlChkBtnCtrl;
	CNumericEdit m_strDeltafCtrl;       //��f�i����j
	CButton m_DeltafCtrlFreqChkBtn;     //������g���`�F�b�N�{�b�N�X
	CNumericEdit m_strDeltafFreq;       //������g��
	CButton m_DeltafCtrlTimeChkBtn;     //���䎞�ԃ`�F�b�N�{�b�N�X
	CEdit m_strDeltafTime;              //���䎞��
	CString m_strPreDeltafTime;			//�O��̐��䎞��
	CButton m_DeltafStartBtn;           //�J�n
	CButton m_DeltafStopBtn;            //��~
	CString m_DeltafCtrlFreqEndTime;    //�I���\�莞��
	BOOL m_RecoveryCont;				//���J�o���[���t���O
	BOOL m_RngFlg;						//�����W�v�����t���O
	CEdit m_strXDC;
	CEdit m_strKaDC;
	CString m_strRecoverCont;
	CString m_strRngFlg;
	CComboBox m_DcCOHModeCtrl;
	BOOL m_strDeltafCtrlInit;			//��f�i����j�����l�ݒ�t���O(TRUE:�����l�ݒ��)

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

protected:
	void GetData();
	void SetData(int type);
	bool SetData(int type, int phase);
	BOOL CheckTimeSec(CString second);
	BOOL CheckFrequencyRate(CString freq);
	BOOL CheckFrequencyInput(CString freq);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedFreqCtrlStartBtn();
	afx_msg void OnBnClickedFreqCtrlStopBtn();
	afx_msg void OnBnClickedDeltafCtrlFreqCtrChk();
	afx_msg void OnBnClickedDeltafCtrlFreqChk();
	afx_msg void OnBnClickedDeltafCtrlTimeChk();
	BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedFreqCtrlCtrlSetUcBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlSetCmdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsTfBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsToBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpTdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpRdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDpCmdBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlScmBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlDcCOHBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlSetCmdTimeBtn();
	afx_msg void OnBnClickedFreqCtrlCtrlOfsTfBtn2();
	afx_msg void OnCbnSelchangeFreqCtrlDcCohCmb();
	afx_msg void OnEnChangeFreqCtrlCtrlDeltafCtrltimeEdit();
	afx_msg void OnEnChangeFreqCtrlCtrlOfsToEdit();
};

