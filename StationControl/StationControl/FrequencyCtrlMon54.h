#pragma once
#include "afxdtctl.h"
#include "atltime.h"
#include "FrequencyCtrlMon.h"

// CFrequencyCtrlMon54 �_�C�A���O


class CFrequencyCtrlMon54 : public CFrequencyCtrlMon
{
	DECLARE_DYNAMIC(CFrequencyCtrlMon54)

public:
	CFrequencyCtrlMon54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlMon54();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_MONITOR54 };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

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
	void SetData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strKaDC;
	CString m_strDCCOHMode;
};
