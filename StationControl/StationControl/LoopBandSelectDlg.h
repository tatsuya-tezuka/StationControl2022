#pragma once
#include "afxwin.h"


// CLoopBandSelectDlg �_�C�A���O

class CLoopBandSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoopBandSelectDlg)

public:
	CLoopBandSelectDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CLoopBandSelectDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_LOOPBAND };

	//-----------------------------------------------------------------------------
	//	��`
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	CString		m_strLoopBand;

protected:
	vector<CString> m_LBCmbList;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetLBCmbList(vector<CString> list) { m_LBCmbList = list; }
	void SetLoopBand(CString str) { m_strLoopBand = str; }

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_LBCmbCtrl;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
