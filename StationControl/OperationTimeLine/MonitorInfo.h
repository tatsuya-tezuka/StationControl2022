/*============================================================================*/
// �Ď����e�[�u��
//   CDBAccess::GetObsname_t()�Ŏ擾�G���[�ƂȂ�Ď����e�X�g�p�Ɏ���
/*============================================================================*/
// MonitorInfo.h : �����t�@�C��

#pragma once

#include "afxwin.h"
#include "atltime.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxdialogex.h"

#ifdef MON_UT

// CMonitorInfo ���X�g
class CMonitorListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMonitorListCtrl)

public:
	CMonitorListCtrl();
	virtual ~CMonitorListCtrl();

	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);

public:
	DECLARE_MESSAGE_MAP()

};

// CMonitorInfo �_�C�A���O
class CMonitorInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorInfo)

public:
	CMonitorInfo(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CMonitorInfo();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_MON_UT };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString m_FileName;
	CMonitorListCtrl m_MonitorList;
	std::vector<CEngValCnv::obsname_t> m_obsname_list;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	virtual BOOL	UpdateDlg();
	void	DisplayList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMonUt();
	afx_msg void OnUpdateMonUt(CCmdUI *pCmdUI);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMonUtReadBtn();
	afx_msg void OnBnClickedMonUtRenewBtn();
};

#endif
