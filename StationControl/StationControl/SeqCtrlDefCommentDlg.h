#pragma once


// CSeqCtrlDefCommentDlg ダイアログ

class CSeqCtrlDefCommentDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSeqCtrlDefCommentDlg)

public:
	CSeqCtrlDefCommentDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSeqCtrlDefCommentDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SEQCTRLDEFCOMMENT };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString	m_strComment;	// コメント

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:
	CString GetComment()	{ return m_strComment; }
	void SetComment(CString& str)		{ m_strComment = str; }

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	/* ------------------------------------------------------------------------------------ */
};
