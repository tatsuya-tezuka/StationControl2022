#pragma once
#include "afxwin.h"
#include "DownlinkCapture.h"

// CDownlinkCapture54 �_�C�A���O

class CDownlinkCapture54 : public CDownlinkCapture
{
	DECLARE_DYNAMIC(CDownlinkCapture54)

public:
	CDownlinkCapture54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CDownlinkCapture54();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_DOWNLINKCAPTURE54 };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString				m_strBandSelect;				// X��<=>Ka�ѐ؂�ւ�������
	CComboBox			m_CCBoxBandSelect;				// X��<=>Ka�ѐ؂�ւ��R���{�{�b�N�X
	eBandSelectType		m_bandSelect;					// X��<=>Ka�ѐ؂�ւ�

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL CreateDlgItems();								// �_�C�A���O�A�C�e���𐶐�
	BOOL CreateDlgBandSelect();    						// X��/Ka�ёI���_�C�A���O�A�C�e���𐶐�
	BOOL UpdateKanshiData();							// �Ď��f�[�^�X�V
	void ShowRcvFreqMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDownlinkcaptureBandSelectButton();
};
