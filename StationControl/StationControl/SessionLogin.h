#pragma once


// CSessionLogin �_�C�A���O

class CSessionLogin : public CDialogBase
{
	DECLARE_DYNAMIC(CSessionLogin)

public:
	CSessionLogin(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSessionLogin();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SESSIONLOGIN };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_passwd;					// �p�X���[�h
	eSessionType m_sessionType;			// �Z�b�V����

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetSessionType(eSessionType sessionType)	{ m_sessionType = sessionType; }
	eSessionType GetSessionType()	{ return m_sessionType; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	/* ------------------------------------------------------------------------------------ */
};
