#pragma once

#include "EquipmentCtrlNum.h"

// CEquipmentCtrlSameNum ダイアログ

class CEquipmentCtrlSameNum : public CEquipmentCtrlNum
{
	DECLARE_DYNAMIC(CEquipmentCtrlSameNum)

public:
	CEquipmentCtrlSameNum(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEquipmentCtrlSameNum();

// ダイアログ データ
	enum { IDD = IDD_DLG_EQUIPCTRL_SAME_NUM };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString m_strEquipment2;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL OnInitDialog();
	void SetEquipment2(CString eqip) { m_strEquipment2 = eqip; }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	/* ------------------------------------------------------------------------------------ */
};
