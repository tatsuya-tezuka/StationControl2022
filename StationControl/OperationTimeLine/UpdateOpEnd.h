#pragma once
#include "afxdtctl.h"


// CUpdateOpEnd �_�C�A���O

class CUpdateOpEnd : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateOpEnd)

public:
	CUpdateOpEnd(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CUpdateOpEnd();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_UPDATEOPEND };

	CTime	mUpdateTime;
	CTime	mDefTime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDateTimeCtrl m_Date;
	CDateTimeCtrl m_Time;
	virtual void OnOK();
	CString m_strTime;
};
