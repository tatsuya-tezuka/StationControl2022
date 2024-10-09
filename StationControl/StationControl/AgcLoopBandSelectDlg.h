#pragma once
#include "afxwin.h"


// AgcLoopBandSelectDlg �_�C�A���O

class AgcLoopBandSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AgcLoopBandSelectDlg)

public:
	AgcLoopBandSelectDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~AgcLoopBandSelectDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_LOOPBAND2 };


	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	CString		m_strLoopBand;

protected:
	vector<CString> m_LBCmbList;

	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetLBCmbList(vector<CString> list) { m_LBCmbList = list; }
	void SetLoopBand(CString str) { m_strLoopBand = str; }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_LoopBandCmb;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
