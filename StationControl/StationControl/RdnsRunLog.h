#pragma once
#include "afxwin.h"


// CRdnsRunLog �_�C�A���O

class CRdnsRunLog : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsRunLog)

public:
	CRdnsRunLog(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CRdnsRunLog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_RDNS_RUNLOG };

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
	int m_left;
	int m_top;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void setPos(int left, int top) {
		m_left = left; m_top = top;
	}

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	CListBox m_RunLogList;
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
