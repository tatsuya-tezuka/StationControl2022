#pragma once


// CSeqCtrlDefCommentDlg �_�C�A���O

class CSeqCtrlDefCommentDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSeqCtrlDefCommentDlg)

public:
	CSeqCtrlDefCommentDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSeqCtrlDefCommentDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SEQCTRLDEFCOMMENT };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString	m_strComment;	// �R�����g

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

public:
	CString GetComment()	{ return m_strComment; }
	void SetComment(CString& str)		{ m_strComment = str; }

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	/* ------------------------------------------------------------------------------------ */
};
