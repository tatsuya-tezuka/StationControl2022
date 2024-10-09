#pragma once
#include "RdnsAgcDetailStep.h"

// CRdnsAgcDetail �_�C�A���O

class CRdnsAgcDetail : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsAgcDetail)

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	CString	m_station;
	CString m_satellite;
	CString m_agcItem;

protected:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetLoopBand(CString strLB);
protected:

public:
	CRdnsAgcDetail(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CRdnsAgcDetail();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_RDNS_AGCDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int m_nLoopBand;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
