#pragma once


// CPauseMsgInputDlg �_�C�A���O

class CPauseMsgInputDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CPauseMsgInputDlg)

public:
	CPauseMsgInputDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CPauseMsgInputDlg();


	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_PAUSEMSGINPUT };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_str;
	CEdit m_cedit;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	CString GetString();
	void SetString(CString str)		{ m_str = str; }

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
