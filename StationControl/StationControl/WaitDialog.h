#pragma once
#include "afxcmn.h"

static const DWORD mMessageDlgWaitCode = (WM_USER+1000);

//=============================================================================
//	�����҂��p�X���b�h�N���X
//=============================================================================
class CDlgWaitThread : public CWinThread
{
	DECLARE_DYNCREATE(CDlgWaitThread)

public:
	CDlgWaitThread();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd*			m_pParent;
	CString			m_strTitle;
	CString			m_strMessage;
	bool			m_bAbort;
	bool			m_bProgress;

	CEvent*			mEvent;
	CString			mEventName;
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetParent(CWnd* val) { m_pParent = val; }
	void	SetTitle(CString val) { m_strTitle = val; }
	void	SetMessage(CString val) { m_strMessage = val; }
	void	SetAbort(bool val) { m_bAbort = val; }
	void	SetProgress(bool val) { m_bProgress = val; }
	void	SetEventName(CString val) { mEventName = val; }
	CWnd*	GetParent() { return m_pParent; }
	CString	GetTitle() { return m_strTitle; }
	CString	GetMessage() { return m_strMessage; }
	bool	GetAbort() { return m_bAbort; }
	bool	GetProgress() { return m_bProgress; }
	CString	GetEventName() { return mEventName; }
	CEvent*	GetEvent() { return mEvent; }
protected:
	/* ------------------------------------------------------------------------------------ */

public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWaitThread)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CDlgWaitThread();

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWaitThread)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//=============================================================================
//	�����҂��p�R�}���h�N���X
//=============================================================================
class CDlgWaitCmd : public CCmdTarget
{
	DECLARE_DYNCREATE(CDlgWaitCmd)

	CDlgWaitCmd();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd*			m_pParent;
	CString			m_strTitle;
	CString			m_strMessage;
	bool			m_bAbort;
	bool			m_bProgress;
	CDlgWaitThread	*mThread;

	HANDLE			mEvent;
	CString			mEventName;
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	Open(CString strTitle, CString strMessage, bool bAbort, bool bProgress=false, CWnd* pParent = NULL);
	void	Close();

	void	SetParent(CWnd* val) { m_pParent = val; }
	void	SetTitle(CString val) { m_strTitle = val; }
	void	SetMessage(CString val) { m_strMessage = val; }
	void	SetAbort(bool val) { m_bAbort = val; }
	void	SetProgress(bool val) { m_bProgress = val; }
	void	SetEventName(CString val) { mEventName = val; }
	CWnd*	GetParent() { return m_pParent; }
	CString	GetTitle() { return m_strTitle; }
	CString	GetMessage() { return m_strMessage; }
	bool	GetAbort() { return m_bAbort; }
	bool	GetProgress() { return m_bProgress; }
	CString	GetEventName() { return mEventName; }
protected:
	/* ------------------------------------------------------------------------------------ */

	// ClassWizard generated virtual function overrides
	//}}AFX_VIRTUAL

public:
	virtual ~CDlgWaitCmd();

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWaitCmd)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//=============================================================================
//	�����҂��p�_�C�A���O�N���X
//=============================================================================
class CWaitDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CWaitDialog)

public:
	CWaitDialog(CString strTitle, CString strMessage, bool bAbort, bool bProgress=false, CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CWaitDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_WAIT };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd*			m_pParent;
	CString			m_strTitle;
	CString			m_strMessage;
	bool			m_bAbort;
	bool			m_bProgress;
	CDlgWaitThread	*mThread;
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetWaitThreadPtr(CDlgWaitThread* val) { mThread = val; }

	void	SetParent(CWnd* val) { m_pParent = val; }
	void	SetTitle(CString val) { m_strTitle = val; }
	void	SetMessage(CString val) { m_strMessage = val; }
	void	SetAbort(bool val) { m_bAbort = val; }
	void	SetProgress(bool val) { m_bProgress = val; }
	CWnd*	GetParent() { return m_pParent; }
	CString	GetTitle() { return m_strTitle; }
	CString	GetMessage() { return m_strMessage; }
	bool	GetAbort() { return m_bAbort; }
	bool	GetProgress() { return m_bProgress; }
protected:
	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel() { return; }
	virtual void OnOK() { return; }
	afx_msg void OnBnClickedButtonAbort();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CProgressCtrl m_Progress;
};
