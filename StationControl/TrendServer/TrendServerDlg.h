
// TrendServerDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

#include "CommBase.h"
#include "afxcmn.h"


// CTrendServerDlg �_�C�A���O
class CTrendServerDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CTrendServerDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TRENDSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
	static const UINT mMaxConnection = 10;
public:
protected:
	CCommServer	m_DataServer;						// �ڑ��T�[�o�[
	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
protected:
	bool	_StartServer();
	bool	_StopServer();
	int		_AddItem(CListCtrl& list, int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);
	int		_AppendMessage(CString msg, COLORREF color=RGB(0,0,0));

	//-----------------------------------------------------------------------------
	//	�_�~�[�f�[�^
	//-----------------------------------------------------------------------------
	int				m_angle[80];

// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	CListCtrl mListStation;
	CListCtrl mListEquipment;
	CListCtrl mListSignal;
	CRichEditCtrl mMessage;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
