#pragma once

#define COMMENT_EXT		(_T(".comment"))
#define TXT_EXT			(_T(".txt"))

// CSeqCtrlDefFileDlg ダイアログ

class CSeqCtrlDefFileDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSeqCtrlDefFileDlg)

public:
	CSeqCtrlDefFileDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSeqCtrlDefFileDlg();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SEQCTRLDEFFILE };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_strFolderPath;
	CString m_strFileName;
	CListBox m_listBox;
	vector<CString> m_listFileName;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:
	CString GetFileName()		{ 
		CString str = m_strFileName;
		str.TrimRight(TXT_EXT);
		return str;
	}					// 定義ファイル名取得
	CString GetFilePath()		{ return m_strFolderPath + m_strFileName; }	// 定義ファイルパス取得

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
