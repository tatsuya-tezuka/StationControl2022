#pragma once

#include "OperationTimeDialogBase.h"

// COperationStartTimeChg �_�C�A���O

class COperationStartTimeChg : public COperationTimeDialogBase
{
	DECLARE_DYNAMIC(COperationStartTimeChg)

public:
	COperationStartTimeChg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COperationStartTimeChg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_OPERATIONSTARTTIMECHG };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	void UpdatePlanData(UINT nStation, CString& strTime);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
