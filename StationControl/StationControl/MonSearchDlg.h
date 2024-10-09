#pragma once
#include "afxwin.h"


// CMonSerchDlg �_�C�A���O

class CMonSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonSearchDlg)

public:
	CMonSearchDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CMonSearchDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_MON_SEARCH };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString m_MonNameSearch;

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	void	ResetList(CString name);
	bool	WildCard(TCHAR* pwild, TCHAR* pstr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strMonName;
	CListBox m_MonNameList;
	afx_msg void OnEnChangeMonnameEdit();
	afx_msg void OnLbnDblclkMonnameList();
	afx_msg void OnBnClickedOk();
};
