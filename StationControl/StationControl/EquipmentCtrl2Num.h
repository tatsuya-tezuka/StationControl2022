#pragma once
#include "afxwin.h"
#include "NumericEdit.h"


// CEquipmentCtrl2Num �_�C�A���O

class CEquipmentCtrl2Num : public CDialogEx
{
	DECLARE_DYNAMIC(CEquipmentCtrl2Num)

public:
	CEquipmentCtrl2Num(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEquipmentCtrl2Num();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQUIPMENTCTRL2NUM };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	const static UINT m_nCtrlNumMax = 2;

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	stEquipment2Ctrl m_stEquipmentCtrl[m_nCtrlNumMax];
	CString m_strWindowTitle;

	CString m_strEquipment1;
	CString m_strRange1;
	CString m_strVal1;
	CNumericEdit m_CtrlValueEdit1;
	CString m_strEquipment2;
	CString m_strRange2;
	CString m_strVal2;
	CNumericEdit m_CtrlValueEdit2;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetEquipment(UINT idx, stEquipment2Ctrl& stEquipmentCtrl)
	{
		m_stEquipmentCtrl[idx].strEquipment = stEquipmentCtrl.strEquipment;
		m_stEquipmentCtrl[idx].nType = stEquipmentCtrl.nType;
		m_stEquipmentCtrl[idx].strRang = stEquipmentCtrl.strRang;
		m_stEquipmentCtrl[idx].dRangeCur = stEquipmentCtrl.dRangeCur;
		m_stEquipmentCtrl[idx].dRangeMin = stEquipmentCtrl.dRangeMin;
		m_stEquipmentCtrl[idx].dRangeMax = stEquipmentCtrl.dRangeMax;
		m_stEquipmentCtrl[idx].l_incolflt = stEquipmentCtrl.l_incolflt;

		if (idx == 0)
		{
			m_strEquipment1 = stEquipmentCtrl.strEquipment;
			m_strRange1 = stEquipmentCtrl.strRang;
			m_strVal1 = stEquipmentCtrl.dRangeCur;
			m_CtrlValueEdit1.SetRoundPlaceValue(stEquipmentCtrl.l_incolflt);
		}
		else
		{
			m_strEquipment2 = stEquipmentCtrl.strEquipment;
			m_strRange2 = stEquipmentCtrl.strRang;
			m_strVal2 = stEquipmentCtrl.dRangeCur;
			m_CtrlValueEdit2.SetRoundPlaceValue(stEquipmentCtrl.l_incolflt);
		}
	}

	CString& GetEquipmentVal(UINT idx)
	{
		if (idx < m_nCtrlNumMax)
			return m_stEquipmentCtrl[idx].dRangeCur;
		else 
			return m_stEquipmentCtrl[0].dRangeCur;
	}

	void SetWindowTitle(CString str)	{ m_strWindowTitle = str; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	/* ------------------------------------------------------------------------------------ */
};
