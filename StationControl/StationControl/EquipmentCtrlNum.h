#pragma once
#include "afxwin.h"
#include "NumericEdit.h"


// CEquipmetCtrlNum �_�C�A���O

class CEquipmentCtrlNum : public CDialogEx
{
	DECLARE_DYNAMIC(CEquipmentCtrlNum)

public:
	CEquipmentCtrlNum(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CEquipmentCtrlNum(UINT IDD, CWnd* pParent = NULL);	// �p���p�R���X�g���N�^�[
	virtual ~CEquipmentCtrlNum();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQUIPCTRL_NUM };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	CString	m_RangeCur;
protected:
	long	m_Type;
	double	m_RangeMin;
	double	m_RangeMax;
	CString m_strWindowTitle;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetRang(CString cur, double min, double max) { m_RangeCur = cur;  m_RangeMin = min; m_RangeMax = max; }
	void SetEquipment(CString eqip, long type) { m_strEquipment = eqip; m_Type = type; }
	void SetRangeText(CString range) { m_strRange = range; }
	void SetWindowTitle(CString str)	{ m_strWindowTitle = str; }

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
protected:
	CString m_strEquipment;
	CString m_strRange;
public:
	CString m_strVal;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CNumericEdit m_CtrlValueEdit;
};
