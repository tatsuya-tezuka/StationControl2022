/*============================================================================*/
/*! SeqCrlDefFileDlg.cpp

-# 逐次制御定義ファイル選択画面
*/
/*============================================================================*/
// SeqCrlDefFileDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "SeqCtrlDefFileDlg.h"
#include "afxdialogex.h"
#include "HookMsgbox.h"

// CSeqCtrlDefFileDlg ダイアログ

IMPLEMENT_DYNAMIC(CSeqCtrlDefFileDlg, CDialogBase)

/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# コンストラクタ

@param  pParent：親ウィンドウ
@retval なし
*/
/*============================================================================*/
CSeqCtrlDefFileDlg::CSeqCtrlDefFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSeqCtrlDefFileDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# デストラクタ

@param  なし
@retval なし
*/
/*============================================================================*/
CSeqCtrlDefFileDlg::~CSeqCtrlDefFileDlg()
{
}

void CSeqCtrlDefFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEQCTRLDEFLIST, m_listBox);
}


BEGIN_MESSAGE_MAP(CSeqCtrlDefFileDlg, CDialogBase)
	ON_BN_CLICKED(IDC_SEQCTRLDEFFILE_DEL, &CSeqCtrlDefFileDlg::OnBnClickedSeqctrldeffileDel)
	ON_BN_CLICKED(IDOK, &CSeqCtrlDefFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSeqCtrlDefFileDlg メッセージ ハンドラー


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# 逐次制御定義ファイル削除

@param  なし
@retval なし
*/
/*============================================================================*/
void CSeqCtrlDefFileDlg::OnBnClickedSeqctrldeffileDel()
{
	int nSel = m_listBox.GetCurSel();
	if (nSel == LB_ERR)
	{
		// 選択されていなかった
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("ファイルが選択されていません。"), title, MB_OK | MB_ICONWARNING);
		return;
	}

	CString strFilePath;
	strFilePath.Format(_T("%s%s"), (LPCTSTR)m_strFolderPath, (LPCTSTR)m_listFileName[nSel]);
	CString strMsg;
	strMsg.Format(_T("%s\n%s"), _T("以下のファイルを削除します。よろしいですか？"), (LPCTSTR)strFilePath);
	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, _T("削除"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// ファイル削除
		if (DeleteFile(strFilePath) == 0 || DeleteFile(strFilePath + COMMENT_EXT) == 0)
		{
			CString title = _T("");
			GetWindowText(title);
			MessageBox(_T("ファイルが削除できませんでした。"), title, MB_OK | MB_ICONWARNING);
		}
	}

	// リストボックスを更新する
	(void)UpdateListBox();
}


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# 逐次制御定義ファイル読み込み

@param  なし
@retval なし
*/
/*============================================================================*/
void CSeqCtrlDefFileDlg::OnBnClickedOk()
{
	int nSel = m_listBox.GetCurSel();
	if (nSel == LB_ERR)
	{
		// 選択されていなかった
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("ファイルが選択されていません。"), title, MB_OK | MB_ICONWARNING);
		return;
	}

	m_strFileName = m_listFileName[nSel];

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# 初期化時に呼び出される

@param  なし
@retval TRUE:成功 / FALSE:失敗
*/
/*============================================================================*/
BOOL CSeqCtrlDefFileDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// リストボックス更新
	(void)UpdateListBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# リストボックス更新時に呼び出される

@param  なし
@retval TRUE:成功 / FALSE:失敗
*/
/*============================================================================*/
BOOL CSeqCtrlDefFileDlg::UpdateListBox()
{
	// フォルダパスをセット
	m_strFolderPath = theApp.GetShareFilePath(eFileType_SeqCtrlDef_Folder, theApp.GetSelectStation());

	m_strFileName = _T("");
	m_listBox.ResetContent();
	m_listFileName.clear();

	// フォルダからファイル一覧を取得

	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(m_strFolderPath + "*", &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return FALSE;
	}

	map<CString, CString> mapComment;

	do
	{
		// ファイルの場合、リストに追加
		if ((ffdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strFileName(ffdata.cFileName);
			CString strName = strFileName;
			if (strName.Find(COMMENT_EXT) >= 0)
			{
				// コメントファイルの場合
				CString strFilePath;
				strFilePath.Format(_T("%s%s"), (LPCTSTR)m_strFolderPath, (LPCTSTR)strFileName);
				CString strComment;
				if (ReadCommentFile(strFilePath, strComment))
				{
//					strName.TrimRight(COMMENT_EXT);
//					int leng = strName.GetLength();
//					int dot = strName.ReverseFind(_T('.'));
//					strName = strName.Left(leng - dot);

					strName = strName.Left(strName.ReverseFind(_T('.')));
//					strName.TrimRight(TXT_EXT);
					mapComment[strName] = strComment;
				}
			}
			else if (strName.Find(TXT_EXT) >= 0)
			{
				// 定義ファイルの場合
//				strName = strFileName;
//				strName = strName.Left(strName.GetLength() - strName.ReverseFind(_T('.')) - 1);
//				strName.TrimRight(TXT_EXT);
				m_listFileName.push_back(strFileName);
			}
		}
	} while (FindNextFile(hFindFile, &ffdata));

	FindClose(hFindFile);

	int nMax = 0;
	int nSize;
	CDC* pDC = m_listBox.GetDC();
	CFont* pOldFont = pDC->SelectObject(m_listBox.GetFont());
	// マップリストからファイル名が一致するコメントを探して、リストボックスを作る
	for (int i = 0; i < m_listFileName.size(); i++)
	{
		CString strFileName = m_listFileName[i];
		auto itr = mapComment.find(strFileName);
		CString strFileNameAndComment;
		CString strNoExt = strFileName;

		strNoExt = strNoExt.TrimRight(TXT_EXT);
		if (itr != mapComment.end())
		{
			// リストボックスにファイル名とコメントを追加
			strFileNameAndComment.Format(_T("%s : %s"), (LPCTSTR)strNoExt, (LPCTSTR)mapComment[strFileName]);
		}
		else
		{
			// リストボックスにファイル名を追加
			strFileNameAndComment.Format(_T("%s : "), (LPCTSTR)strNoExt);
		}
		m_listBox.InsertString(-1, strFileNameAndComment);

		nSize = pDC->GetTextExtent(strFileNameAndComment).cx;
		if (nMax < nSize)
			nMax = nSize;
	}

	m_listBox.SetHorizontalExtent(nMax + 5);
	ReleaseDC(pDC);

	ASSERT(m_listFileName.size() == m_listBox.GetCount());

	return TRUE;
}


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# コメントファイルからコメントを読み込む

@param  strFilePath	ファイルパス文字列
@param  strComment	コメント文字列
@retval TRUE:成功 / FALSE:失敗
*/
/*============================================================================*/
BOOL CSeqCtrlDefFileDlg::ReadCommentFile(CString& strFilePath, CString& strComment)
{
	BOOL ret = FALSE;
	FILE *fp;

	TRY
	{
		if (_wfopen_s(&fp, strFilePath, _T("r")) != 0 || fp == NULL)
		{
			return ret;
		}
	
		char szline[2048];
		// 一行目のみ読む
		if (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL)
		{
			strComment = CServerFile::ConvertUnicode(szline);
			ret = TRUE;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	fclose(fp);

	return ret;
}
