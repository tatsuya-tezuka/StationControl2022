#pragma once

#include "EquipmentCtrlNum.h"

// CEquipmentCtrlSameNum �_�C�A���O

class CEquipmentCtrlSameNum : public CEquipmentCtrlNum
{
	DECLARE_DYNAMIC(CEquipmentCtrlSameNum)

public:
	CEquipmentCtrlSameNum(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEquipmentCtrlSameNum();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQUIPCTRL_SAME_NUM };

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
	CString m_strEquipment2;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL OnInitDialog();
	void SetEquipment2(CString eqip) { m_strEquipment2 = eqip; }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	/* ------------------------------------------------------------------------------------ */
};
