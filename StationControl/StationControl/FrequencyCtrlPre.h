#pragma once
#include "afxwin.h"
#include "atltime.h"
#include "afxdtctl.h"
#include "NumericEdit.h"

// ���M���g���擾��
typedef struct{
	const CString		sendname;		// ����於��
	UINT				sendid;			// �����ID
} stSendFreq;
static const stSendFreq mSenderFreq[eStation_MAX][2] = {
	{
		{ _T("USC34m"), eStation_Uchinoura34 },
		{ _T("UDSC54m"), eStation_Usuda642 }
	},
	{
		{ _T("UDSC64m"), eStation_Usuda64 },
		{ _T("USC34m"), eStation_Uchinoura34 }
	},
	{
		{ _T("UDSC64m"), eStation_Usuda64 },
		{ _T("UDSC54m"), eStation_Usuda642 }
	},
	{
		{ _T(""), eStation_Usuda64 },
		{ _T(""), eStation_Usuda64 }
	},
};

// CFrequencyCtrlPre �_�C�A���O

class CFrequencyCtrlPre : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlPre)

public:
	CFrequencyCtrlPre(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CFrequencyCtrlPre(UINT IDD, CWnd* pParent = NULL);
	virtual ~CFrequencyCtrlPre();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_PRESET };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
public:
	enum{
		ePre_1WAY,		// 1WAY�^�p(�e�����P�Ɖ^�p)
		ePre_2WAY1,		// 2WAY�^�p(COH�A���I)
		ePre_2WAY2,		// 2WAY�^�p(COH�A�Z���^�[)
		ePre_2WAY3,		// 2WAY�^�p(INCOH�A���I)
		ePre_2WAY4,		// 2WAY�^�p(INCOH�A�Z���^�[)
		ePre_3WAY,		// 3WAY
	};
	typedef struct{
		const CString		ctlname;		// ���䖼
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
	void	SetData(int type);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPre1way();
	afx_msg void OnBnClickedButtonPre2way1();
	afx_msg void OnBnClickedButtonPre2way2();
	afx_msg void OnBnClickedButtonPre2way3();
	afx_msg void OnBnClickedButtonPre2way4();
	afx_msg void OnBnClickedButtonPre3way();
	CString m_strSendOp;
	CNumericEdit m_SendOpEdit;
	CComboBox m_cbSendGet;
	int m_nSendGet;
	afx_msg void OnBnClickedButtonGet();
};
