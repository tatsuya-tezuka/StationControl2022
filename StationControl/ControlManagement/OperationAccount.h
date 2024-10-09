#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"


// COperationAccount �_�C�A���O

class COperationAccount : public CDialogEx
{
	DECLARE_DYNAMIC(COperationAccount)

public:
	COperationAccount(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COperationAccount();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_OPERATION_ACCOUNT };

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
protected:
	CImageList			m_ImageList;
	vector<stAccount>	mAccountList;
public:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
protected:
	void		AutoSizeColumns(int col = -1);
	int			AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam=0);
	HIMAGELIST	CreateImageList(CDC* pdc);

public:

	//-----------------------------------------------------------------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx mList;
	afx_msg void OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
