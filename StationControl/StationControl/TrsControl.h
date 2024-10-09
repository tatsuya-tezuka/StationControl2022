#pragma once
#include "TrsControlS20.h"
#include "TrsControlS34.h"
#include "TrsControlS64.h"
#include "TrsControlX.h"
#include "TrsControlXX.h"
#include "afxwin.h"


// CTrsControl �_�C�A���O

class CTrsControl : public CDialogBase
{
	DECLARE_DYNAMIC(CTrsControl)

public:
	CTrsControl(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTrsControl();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TRSCONTROL };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum {
		TYPE_NA,
		TYPE_64,
		TYPE_34,
	};
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	UINT m_SecelctType;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();
	void SetSelectType(UINT val) { m_SecelctType = val; }

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int m_BandRadio;
	afx_msg void OnBnClickedTrsctrlViewBtn();
	virtual BOOL OnInitDialog();
};
