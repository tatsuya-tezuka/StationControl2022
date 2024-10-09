// BusinessDiaryFileNotify.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "BusinessDiaryFileNotify.h"
#include "afxdialogex.h"


// CBusinessDiaryFileNotify �_�C�A���O

IMPLEMENT_DYNAMIC(CBusinessDiaryFileNotify, CDialogEx)

CBusinessDiaryFileNotify::CBusinessDiaryFileNotify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBusinessDiaryFileNotify::IDD, pParent)
{
	mpThread = NULL;
}

CBusinessDiaryFileNotify::~CBusinessDiaryFileNotify()
{
}

void CBusinessDiaryFileNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBusinessDiaryFileNotify, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CBusinessDiaryFileNotify::OnBnClickedCancel)
END_MESSAGE_MAP()


// CBusinessDiaryFileNotify ���b�Z�[�W �n���h���[

static bool	mNotifyThreadEnd = false;

/*============================================================================*/
/*! �Ď��f�[�^

-# �Ď��f�[�^�t�@�C������ǂݍ��܂ꂽ�f�[�^���Ď�����X���b�h

@param
@retval

*/
/*============================================================================*/
UINT ThreadNotifyProc(LPVOID pParam)
{
	CBusinessDiaryFileNotify *pMain = (CBusinessDiaryFileNotify *)pParam;

	long count = 0;
	while (!mNotifyThreadEnd){

		if (pMain->IsBusinessDiaryFile() == true){
			pMain->PostMessage(WM_USER + 100, 0, 0);
		}
		Sleep(100);
	}

	return 0;
}

BOOL CBusinessDiaryFileNotify::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (IsBusinessDiaryFile() == true){
		EndDialog(IDOK);
	}
	else{
		mNotifyThreadEnd = false;
		mpThread = AfxBeginThread(ThreadNotifyProc, this);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CBusinessDiaryFileNotify::OnBnClickedCancel()
{
	if (mpThread != NULL){
		mNotifyThreadEnd = true;
		WaitForSingleObject(mpThread->m_hThread, 10000);
		mpThread = NULL;
	}

	CDialogEx::OnCancel();
}


bool CBusinessDiaryFileNotify::IsBusinessDiaryFile()
{
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString filename = theApp.GetShareFilePath(eFileType_Diary_File, nStation);

	// �w�肳�ꂽ�t�@�C�����Ɉ�v����t�@�C�����A�f�B���N�g�����Ō�������
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(filename, &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return false;
	}
	FindClose(hFindFile);

	return true;
}



LRESULT CBusinessDiaryFileNotify::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == (WM_USER + 100)){
		mNotifyThreadEnd = true;
		WaitForSingleObject(mpThread->m_hThread, 10000);
		mpThread = NULL;
		EndDialog(IDOK);
		return TRUE;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
