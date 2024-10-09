#pragma once


// CControlAcquisition �_�C�A���O

class CControlAcquisition : public CDialogEx
{
	DECLARE_DYNAMIC(CControlAcquisition)

public:
	CControlAcquisition(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlAcquisition();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CONTROL_ACQUISITION };

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
	CString		mGetPassword;
	__int64		mGetTimer;
	int			mClientTerminal;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	ResetControlFlag();
	int		GetClientTerminal()	{ return mClientTerminal; }

protected:
	int		GetControlType(int target);
	bool	CheckControl(int target, stControl& datast, stLocalControl& local);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedMode();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CString m_strText;
	CString m_strPassword;
};
