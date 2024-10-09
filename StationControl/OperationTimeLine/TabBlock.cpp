// TabBlock.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "TabBlock.h"
#include "afxdialogex.h"


// CTabBlock �_�C�A���O

IMPLEMENT_DYNCREATE(CTabBlock, CDialogEx)

CTabBlock::CTabBlock(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabBlock::IDD, pParent)
{

}

CTabBlock::~CTabBlock()
{
}

void CTabBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FAPANEL1, m_FaBlock);
}


BEGIN_MESSAGE_MAP(CTabBlock, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTabBlock ���b�Z�[�W �n���h���[


BOOL CTabBlock::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString fname;
	fname.Format(_T("%s%s"), (LPCTSTR)theApp.GetAppFaPanelPath(), mFapanelBlockFile[theApp.GetSelectStation()]);
	m_FaBlock.SetFileName(fname);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CTabBlock::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_FaBlock.GetSafeHwnd()){
		CRect rect;
		GetClientRect(rect);
		m_FaBlock.MoveWindow(rect);
	}
}
