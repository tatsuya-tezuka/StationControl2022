#pragma once


// CCommentInputDlg �_�C�A���O

class CCommentInputDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CCommentInputDlg)

public:
	CCommentInputDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CCommentInputDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_COMMENTINPUT };

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
