#pragma once
#include "afxwin.h"


// CEquipmentRecSelectFile �_�C�A���O

class CEquipmentRecSelectFile : public CDialogBase
{
	DECLARE_DYNAMIC(CEquipmentRecSelectFile)

public:
	CEquipmentRecSelectFile(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEquipmentRecSelectFile();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQIPREC_SELFILE };

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
	CListBox m_FileList;
	afx_msg void OnBnClickedEqiprecSelfileReadBtn();
	afx_msg void OnBnClickedEqiprecSelfileDelBtn();
};
