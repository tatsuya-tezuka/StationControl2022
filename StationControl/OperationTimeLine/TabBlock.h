#pragma once
#include "fapanel1.h"


// CTabBlock �_�C�A���O

class CTabBlock : public CDialogEx
{
	DECLARE_DYNCREATE(CTabBlock)

public:
	CTabBlock(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTabBlock();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TAB_BLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CFapanel1 m_FaBlock;
};
