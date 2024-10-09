#pragma once


// CSelectLNADlg �_�C�A���O

class CSelectLNADlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSelectLNADlg)

public:
	CSelectLNADlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSelectLNADlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SELECTLNA };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum
	{
		eLNA1 = 1,
		eLNA2,
	};

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	UINT m_nSelLNA;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetLNA(UINT lna)
	{
		if (lna < eLNA1 || lna > eLNA2)
			m_nSelLNA = eLNA1;
		else
			m_nSelLNA = lna;
	}

protected:
	void EnableExecButton();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedSelectLna1();
	afx_msg void OnBnClickedSelectLna2();
};
