#pragma once

#include "SessionStatusTab.h"

// CSessionStatus �_�C�A���O

class CSessionStatus : public CDialogBase
{
	DECLARE_DYNAMIC(CSessionStatus)

public:
	CSessionStatus(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSessionStatus();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SESSIONSTATUS };

	// ��ʐݒ�
	const int m_nTabLeftOffset = 15;			// �^�u�E�B���h�E��Left�I�t�Z�b�g
	const int m_nTabTopOffset = 35;				// �^�u�E�B���h�E��Top�I�t�Z�b�g
	static const int m_nIntervalTime = 10000;	// �Ď��C���^�[�o���^�C�}

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CTabCtrl m_tabCtrl;									// �^�u�R���g���[��
	CSessionStatusTab	m_station[eSessionType_Num];	// ��
	UINT	m_nStation;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void AddStation(UINT nIndex, CString stationName);	// �ǖ���ǉ�����
	void SetStation(UINT nStation)	{ m_nStation = nStation; }
	UINT GetStation()				{ return m_nStation; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

protected:
	void GetSessionStatus(BOOL bWaitComplete = TRUE, BOOL bForce = TRUE);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnTcnSelchangeSessionstatusTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
