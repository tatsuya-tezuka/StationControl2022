#pragma once


// CSessionStatusUDSC64m �_�C�A���O

class CSessionStatusUDSC64m : public CDialogEx
{
	DECLARE_DYNAMIC(CSessionStatusUDSC64m)

public:
	CSessionStatusUDSC64m(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSessionStatusUDSC64m();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SESSIONSTATUSUDSC64M };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CEdit m_ceditMonitor;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSessionstatusMonitorButton();
	afx_msg void OnBnClickedSessionstatusCtrlButton();
	afx_msg void OnBnClickedSessionstatusNotifyButton();
	afx_msg void OnBnClickedSessionstatusHistoryButton();
	/* ------------------------------------------------------------------------------------ */
};
