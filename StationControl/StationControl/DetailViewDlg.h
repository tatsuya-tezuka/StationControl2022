#pragma once
#include "afxcmn.h"


enum{
	DVANCHORE_LEFTTOP = 0x0000,	// ����͌Œ肳����
	DVANCHORE_RIGHT = 0x0001,	// �E���͌Œ肳����
	DVANCHORE_BOTTOM = 0x0002,	// �����͌Œ肳����
	DVRESIZE_HOR = 0x0004,	// ���̃��T�C�Y���\�ɂ���
	DVRESIZE_VER = 0x0008,	// �c�̃��T�C�Y���\�ɂ���
	DVRESIZE_BOTH = (DVRESIZE_HOR | DVRESIZE_VER),
};

class CColorListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CColorListCtrl)

public:
	CColorListCtrl();
	virtual ~CColorListCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};



// CDetailViewDlg �_�C�A���O

class CDetailViewDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CDetailViewDlg)

public:
	CDetailViewDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CDetailViewDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_DETAILVIEW };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	int					mSaveCx, mSaveCy;
	vector<DWORD>		mControl;
	CString				mTitleText;
	CString				mSelectFileName;
	CFont				mListFont;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetTitle(CString val) { mTitleText = val; }
	void	SetSelectFileName(CString val) { mSelectFileName = val; }
protected:
	void	AutoSizeColumns(int col = -1);
	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam=0);
	void	SetControlInfo(WORD id, WORD anchore);
	bool	ReadFile(CString filename);
	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void OnCancel(){};

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSelectFile;
	CColorListCtrl m_ListFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void PostNcDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuClose();
	afx_msg void OnClose();
};
