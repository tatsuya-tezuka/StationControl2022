#pragma once


// CSessionStatusTab �_�C�A���O

class CSessionStatusTab : public CDialogBase
{
	DECLARE_DYNAMIC(CSessionStatusTab)

public:
	CSessionStatusTab(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSessionStatusTab();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SESSIONSTATUSTAB };

	COLORREF	m_colorText = RGB(0, 0, 0);		// �e�L�X�g�̐F
	COLORREF	m_colorBkDisConnect = RGB(255, 0, 0);	// �w�i�̐ؒf�F
	COLORREF	m_colorBkConnect = RGB(0, 128, 0);		// �w�i�̐ڑ��F
	COLORREF	m_colorBkInvalid = RGB(128, 128, 128);	// �w�i�̖����F
	COLORREF	m_colorTextInv = RGB(255, 255, 255);			// �e�L�X�g�̔��]�F

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CColorStatic m_csMonitorStatus;		// �Ď��Z�b�V������ԕ\��
	CColorStatic m_csCtrlStatus;		// ����Z�b�V������ԃG�f�B�b�g�R���g���[��
	CColorStatic m_csNotifyStatus;		// �ʒm�Z�b�V������ԃG�f�B�b�g�R���g���[��
	CColorStatic m_csHistoryStatus;		// �����Z�b�V������ԃG�f�B�b�g�R���g���[��

	CButton m_buttonMonitor;			// �Ď��Z�b�V������ԃ{�^���R���g���[��			
	CButton m_buttonCtrl;				// ����Z�b�V������ԃ{�^���R���g���[��
	CButton m_buttonNotify;				// �ʒm�Z�b�V������ԃ{�^���R���g���[��
	CButton m_buttonHistory;			// �����Z�b�V������ԃ{�^���R���g���[��

	CFont		m_txtFont;				// �e�L�X�g�t�H���g

	CString m_stationName;				// �ǖ�
	UINT m_nStationID;					// �ǔԍ�

	CStatic	m_staticCtrlStr;			// ����Z�b�V����������
	UINT m_sessionStatus[eSessionType_Num];	// �Z�b�V�������

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetStationName(CString stationName)	{ m_stationName = stationName; }
	CString GetStationName()					{ return m_stationName; }
	void SetStationID(UINT nStationID)			{ m_nStationID = nStationID; }

	void GetSessionStatus(BOOL bWaitComplete = TRUE);
	void UpdateSessionDisplay();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	void DispMonitorStatus();
	void DispCtrlStatus();
	void DispNotifyStatus();
	void DispHistoryStatus();

	void DispMonitorUnknownStatus();
	void DispMonitorGettingNowStatus();
	void DispMonitorErrorStatus();
	void DispMonitorConStatus();
	void DispMonitorDisConStatus();
	void DispCtrlUnknownStatus();
	void DispCtrlGettingNowStatus();
	void DispCtrlErrorStatus();
	void DispCtrlConStatus();
	void DispCtrlDisConStatus();
	void DispNotifyUnknownStatus();
	void DispNotifyGettingNowStatus();
	void DispNotifyErrorStatus();
	void DispNotifyConStatus();
	void DispNotifyDisConStatus();
	void DispHistoryUnknownStatus();
	void DispHistoryGettingNowStatus();
	void DispHistoryErrorStatus();
	void DispHistoryConStatus();
	void DispHistoryDisConStatus();

	BOOL DisConnectMonitorSession();
	BOOL DisConnectCtrlSession();
	BOOL DisConnectNotifySession();
	BOOL DisConnectHistorySession();

	BOOL ConnectMonitorSession();
	BOOL ConnectCtrlSession();
	BOOL ConnectNotifySession();
	BOOL ConnectHistorySession();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedSessionstatusMonitorButton();
	afx_msg void OnBnClickedSessionstatusCtrlButton();
	afx_msg void OnBnClickedSessionstatusNotifyButton();
	afx_msg void OnBnClickedSessionstatusHistoryButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	/* ------------------------------------------------------------------------------------ */
};
