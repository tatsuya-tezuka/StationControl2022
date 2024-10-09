#pragma once


// CControlRegistDlg �_�C�A���O

class CControlRegistDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CControlRegistDlg)

public:
	CControlRegistDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CControlRegistDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_CONTROLREGIST };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_strCtrl;			// ���䖼
	CString m_strVal;			// ����l
	CString m_strValDisp;		// ����l�i�\���p�j
	CEdit m_cedit;				// ���䖼�G�f�B�b�g�{�b�N�X

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	CString GetString();
	void SetCtrlString(CString str)		{ m_strCtrl = str; }
	void SetValString(CString str)		{ m_strVal = str; }

public:
	afx_msg void OnBnClickedButtonCtrlregistSearch();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
