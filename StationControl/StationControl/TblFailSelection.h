#pragma once
#include "afxwin.h"
#include "const.h"


// CTblFailSelection �_�C�A���O

class CTblFailSelection : public CDialogEx
{
	DECLARE_DYNAMIC(CTblFailSelection)

public:
	CTblFailSelection(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTblFailSelection();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TBL_FILESEL };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// �t�@�C�����X�g
	CListBox m_FileList;
	// �I���t�@�C��Edit
	CEdit m_FileEdit;
	// �I���t�@�C��
	CString m_strFile;
	afx_msg void OnBnClickedButton1();
};
