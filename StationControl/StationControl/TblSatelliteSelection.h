#pragma once
#include "afxwin.h"
#include "const.h"
#include "TblFailSelection.h"

// CTblSatelliteSelection �_�C�A���O


class CTblSatelliteSelection : public CDialogEx
{
	DECLARE_DYNAMIC(CTblSatelliteSelection)

public:
	CTblSatelliteSelection(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTblSatelliteSelection();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TBL_SATSEL };

	//-----------------------------------------------------------------------------
	// ��`
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	// �����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTblSatselBtn();
	virtual BOOL OnInitDialog();
	// �q���I���R���{�{�b�N�X
	CComboBox m_SatCmb;
};
