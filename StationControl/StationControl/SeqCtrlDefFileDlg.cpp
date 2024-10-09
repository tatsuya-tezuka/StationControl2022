/*============================================================================*/
/*! SeqCrlDefFileDlg.cpp

-# ���������`�t�@�C���I�����
*/
/*============================================================================*/
// SeqCrlDefFileDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SeqCtrlDefFileDlg.h"
#include "afxdialogex.h"
#include "HookMsgbox.h"

// CSeqCtrlDefFileDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSeqCtrlDefFileDlg, CDialogBase)

/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSeqCtrlDefFileDlg::CSeqCtrlDefFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSeqCtrlDefFileDlg::IDD, pParent)
{

}

/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
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


// CSeqCtrlDefFileDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# ���������`�t�@�C���폜

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSeqCtrlDefFileDlg::OnBnClickedSeqctrldeffileDel()
{
	int nSel = m_listBox.GetCurSel();
	if (nSel == LB_ERR)
	{
		// �I������Ă��Ȃ�����
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("�t�@�C�����I������Ă��܂���B"), title, MB_OK | MB_ICONWARNING);
		return;
	}

	CString strFilePath;
	strFilePath.Format(_T("%s%s"), (LPCTSTR)m_strFolderPath, (LPCTSTR)m_listFileName[nSel]);
	CString strMsg;
	strMsg.Format(_T("%s\n%s"), _T("�ȉ��̃t�@�C�����폜���܂��B��낵���ł����H"), (LPCTSTR)strFilePath);
	if (IDOK == MessageBoxHooked(this->m_hWnd, strMsg, _T("�폜"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		// �t�@�C���폜
		if (DeleteFile(strFilePath) == 0 || DeleteFile(strFilePath + COMMENT_EXT) == 0)
		{
			CString title = _T("");
			GetWindowText(title);
			MessageBox(_T("�t�@�C�����폜�ł��܂���ł����B"), title, MB_OK | MB_ICONWARNING);
		}
	}

	// ���X�g�{�b�N�X���X�V����
	(void)UpdateListBox();
}


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# ���������`�t�@�C���ǂݍ���

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSeqCtrlDefFileDlg::OnBnClickedOk()
{
	int nSel = m_listBox.GetCurSel();
	if (nSel == LB_ERR)
	{
		// �I������Ă��Ȃ�����
		CString title = _T("");
		GetWindowText(title);
		MessageBox(_T("�t�@�C�����I������Ă��܂���B"), title, MB_OK | MB_ICONWARNING);
		return;
	}

	m_strFileName = m_listFileName[nSel];

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# ���������ɌĂяo�����

@param  �Ȃ�
@retval TRUE:���� / FALSE:���s
*/
/*============================================================================*/
BOOL CSeqCtrlDefFileDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// ���X�g�{�b�N�X�X�V
	(void)UpdateListBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CSeqCtrlDefFileDlg

-# ���X�g�{�b�N�X�X�V���ɌĂяo�����

@param  �Ȃ�
@retval TRUE:���� / FALSE:���s
*/
/*============================================================================*/
BOOL CSeqCtrlDefFileDlg::UpdateListBox()
{
	// �t�H���_�p�X���Z�b�g
	m_strFolderPath = theApp.GetShareFilePath(eFileType_SeqCtrlDef_Folder, theApp.GetSelectStation());

	m_strFileName = _T("");
	m_listBox.ResetContent();
	m_listFileName.clear();

	// �t�H���_����t�@�C���ꗗ���擾

	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(m_strFolderPath + "*", &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return FALSE;
	}

	map<CString, CString> mapComment;

	do
	{
		// �t�@�C���̏ꍇ�A���X�g�ɒǉ�
		if ((ffdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strFileName(ffdata.cFileName);
			CString strName = strFileName;
			if (strName.Find(COMMENT_EXT) >= 0)
			{
				// �R�����g�t�@�C���̏ꍇ
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
				// ��`�t�@�C���̏ꍇ
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
	// �}�b�v���X�g����t�@�C��������v����R�����g��T���āA���X�g�{�b�N�X�����
	for (int i = 0; i < m_listFileName.size(); i++)
	{
		CString strFileName = m_listFileName[i];
		auto itr = mapComment.find(strFileName);
		CString strFileNameAndComment;
		CString strNoExt = strFileName;

		strNoExt = strNoExt.TrimRight(TXT_EXT);
		if (itr != mapComment.end())
		{
			// ���X�g�{�b�N�X�Ƀt�@�C�����ƃR�����g��ǉ�
			strFileNameAndComment.Format(_T("%s : %s"), (LPCTSTR)strNoExt, (LPCTSTR)mapComment[strFileName]);
		}
		else
		{
			// ���X�g�{�b�N�X�Ƀt�@�C������ǉ�
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

-# �R�����g�t�@�C������R�����g��ǂݍ���

@param  strFilePath	�t�@�C���p�X������
@param  strComment	�R�����g������
@retval TRUE:���� / FALSE:���s
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
		// ��s�ڂ̂ݓǂ�
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
