#pragma once
#include "atltime.h"
#include "afxdtctl.h"
#include "afxwin.h"

// ���M���g���擾��
typedef struct{
	const CString		sendname;		// ����於��
	UINT				sendid;			// �����ID
} stSimSendFreq;
static const stSimSendFreq mSimSenderFreq[eStation_MAX][2] = {
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

// CFrequencyCtrlSm3W �_�C�A���O

class CFrequencyCtrlSm3W : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSm3W)

public:
	CFrequencyCtrlSm3W(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlSm3W();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_SML_3W };

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
	double	CalculatioFreq(time_t time, int type, int timetype, bool bPassUse = true);

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// 3WAY�p���Ǒ��M���g��
	CString m_str3WAYFreq;
	afx_msg void OnBnClickedFreqCtrlSml3wCalcBtn();
	// �w�莞���iDay�j
	CTime m_SpecDay;
	// �w�莞���iTime�j
	CTime m_SpecTime;
	CDateTimeCtrl m_SpecDayDTCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFreqCtrlSml3wCalcBtn2();
	CComboBox m_cbSimSendGet;
	CString m_strSimValue[mItemRows][mItemCols];
	afx_msg void OnBnClickedButtonGet();
};
