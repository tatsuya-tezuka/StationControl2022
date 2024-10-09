#pragma once
#include "afxwin.h"

#ifdef _DEBUG_KANSHI64M
// 64m�Ď�
static const stControlParam mKarecvParam54[] = {
	{ IDC_BUTTON_DCCTRL, _T("S70-DC.SET_FREQ"), _T("S70-DC.SET_FREQ"), eControlDouble },		// D/C CTRL��
	{ IDC_BUTTON_RX, _T("DSNTCR.DOWN_REF_FREQ"), _T("DSNTCR.DOWN_REF_FREQ"), eControlDouble },		// RX��
};
#else
// 54m�Ď�
static const stControlParam mKarecvParam54[] = {
	{ IDC_BUTTON_DCCTRL, _T("X-DC.Ka-RX_FREQUENCY"), _T("X-DC.Ka-RX_FREQUENCY"), eControlDouble },		// D/C CTRL��
	{ IDC_BUTTON_RX, _T("DSNTCR.DOWN_REF_FREQ"), _T("DSNTCR.DOWN_REF_FREQ"), eControlDouble },		// RX��
};
#endif

// CTabKarecvView54 �_�C�A���O

class CTabKarecvView54 : public CDialogEx
{
	DECLARE_DYNCREATE(CTabKarecvView54)

public:
	CTabKarecvView54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTabKarecvView54();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_TAB_KARECV54 };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	ControlParamExecute(UINT pos);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

protected:
	void	UpdateStatus();
	void	InitStatus();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDcctrl();
	afx_msg void OnBnClickedButtonRx();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
