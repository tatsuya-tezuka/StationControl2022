
// ControlManagementDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ControlManagementDlg.h"
#include "afxdialogex.h"

#include "ControlManagementChangePasswordDlg.h"
#include "OperationAccount.h"
#include "ChangeControlDlg.h"
#include "ControlChangePasswordDlg.h"
#include "CancelControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CControlManagementDlg �_�C�A���O



CControlManagementDlg::CControlManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlManagementDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CControlManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlManagementDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CControlManagementDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CControlManagementDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CControlManagementDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CControlManagementDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CControlManagementDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDCANCEL, &CControlManagementDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CControlManagementDlg ���b�Z�[�W �n���h���[

BOOL CControlManagementDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CControlManagementDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CControlManagementDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CControlManagementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# ���䌠���ύX

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton1()
{
	CChangeControlDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# �}�g�ǉ^�p�ǐ��[���̐��䌠������

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton2()
{
	CCancelControlDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# �^�p�A�J�E���g�ݒ�

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton3()
{
	COperationAccount dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# ���䌠���擾�p�p�X���[�h�ύX

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton4()
{
	CControlChangePasswordDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# �Ǘ��p�X���[�h�ύX

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedButton5()
{
	CControlManagementChangePasswordDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# �I��

@param
@retval

*/
/*============================================================================*/
void CControlManagementDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}
