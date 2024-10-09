#pragma once


// CCtrlSearchDlg �_�C�A���O

class CCtrlSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCtrlSearchDlg)

public:
	CCtrlSearchDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CCtrlSearchDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_CTRLSEARCH };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_strCtrlName;
	CString m_strCtrlNameSearch;
	CString m_strCtrlValSearch;
	CListBox m_CtrlNameList;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	ResetList(CString name);
	bool	WildCard(TCHAR* pwild, TCHAR* pstr);
	CString& GetCtrlNameSearch()	{ return m_strCtrlNameSearch; }
	CString& GetCtrlValeSearch()	{ return m_strCtrlValSearch; }
	void SetCtrlName(CString& str)				{ m_strCtrlName = str; }
	void SetValName(CString& str)				{ m_strCtrlValSearch = str; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeCtrlnameEdit();
	afx_msg void OnLbnDblclkCtrlnameList();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeCtrlnameList();
	/* ------------------------------------------------------------------------------------ */
};
