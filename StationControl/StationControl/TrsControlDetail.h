#pragma once


// CTrsControlDetail �_�C�A���O

class CTrsControlDetail : public CDialogEx
{
	DECLARE_DYNAMIC(CTrsControlDetail)

public:
	CTrsControlDetail(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTrsControlDetail();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TRSCTRL_DETAIL };

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
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// ���䖼
	CString m_strCtrlName;
	int m_CtrlValue;
};
