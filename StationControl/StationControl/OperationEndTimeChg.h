#pragma once

#include "OperationTimeDialogBase.h"

// COperationEndTimeChg �_�C�A���O

class COperationEndTimeChg : public COperationTimeDialogBase
{
	DECLARE_DYNAMIC(COperationEndTimeChg)

public:
	COperationEndTimeChg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COperationEndTimeChg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_OPERATIONENDTIMECHG };

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
