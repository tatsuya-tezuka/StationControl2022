#pragma once

#define COMMENT_EXT		(_T(".comment"))
#define TXT_EXT			(_T(".txt"))

// CSeqCtrlDefFileDlg �_�C�A���O

class CSeqCtrlDefFileDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSeqCtrlDefFileDlg)

public:
	CSeqCtrlDefFileDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSeqCtrlDefFileDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SEQCTRLDEFFILE };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_strFolderPath;
	CString m_strFileName;
	CListBox m_listBox;
	vector<CString> m_listFileName;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

public:
	CString GetFileName()		{ 
		CString str = m_strFileName;
		str.TrimRight(TXT_EXT);
		return str;
	}					// ��`�t�@�C�����擾
	CString GetFilePath()		{ return m_strFolderPath + m_strFileName; }	// ��`�t�@�C���p�X�擾

protected:
	BOOL UpdateListBox();
	BOOL ReadCommentFile(CString& strFilePath, CString& strComment);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSeqctrldeffileDel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
