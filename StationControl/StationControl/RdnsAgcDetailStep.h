#pragma once
#include "afxcmn.h"
#include "AgcIntgListCtrl.h"
#include "afxwin.h"

static const CString strStepComb[] = {
	_T("1step ��"),
	_T("3step ��"),
	_T("5step ��"),
	_T("�f�t�H���g�ɖ߂�"),
	_T("")
};


static const int STEP[] = {
	1,
	3,
	5,
};

enum{
	TYPE_DFLT,
	TYPE_CUST,
};

static const CString TypeFile[] = {
	_T("DFLT"),
	_T("CUST"),
};


// CRdnsAgcDetailStep �_�C�A���O

class CRdnsAgcDetailStep : public CDialogBase
{
	DECLARE_DYNAMIC(CRdnsAgcDetailStep)

public:
	CRdnsAgcDetailStep(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CRdnsAgcDetailStep();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_RDNS_AGCSTEP };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum {
		STEP_1,
		STEP_3,
		STEP_5,
		STEP_DEF,
	};

	typedef struct {
		CString att;		// ATT�l
		CString calib;		// �Z���l
		CString u_limit;	// ������e�l
		CString l_limit;	// �������e�l
		CString incr;		// ATT����
		CString corr;		// �Z���C���l
		CString wait;		// �Z���҂�����
		CString intg;		// �ϕ�����
		CString enable;		// �L������
		CString coment;		// �R�����g
	} stAgcTbl;

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	CString	m_Station;
	CString m_Satellite;
	CString m_AgcItem;
	CString m_LoopBand;

protected:
	int m_CurrentStep;
	map<CString, stAgcTbl> m_AgcTblCustMap;
	map<CString, stAgcTbl> m_AgcTblDfltMap;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
protected:
	BOOL ReadAgcTblFile(int type, map<CString, stAgcTbl> &map);
	BOOL WriteAgcTblFile();
	
	void SetDefaultValue();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CAgcIntgListCtrl m_IntgListCtrl;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRdnsAgcStepIntgBtn();
	CEdit m_IntegEdit;
	CComboBox m_CalibCmb;
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeRdnsAgcStepPrfCmb();
	afx_msg void OnBnClickedOk();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
