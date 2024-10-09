#pragma once

#include "fapanel1.h"


// CEquipmentSystemForm フォーム ビュー

class CEquipmentSystemForm : public CFormView
{
	DECLARE_DYNCREATE(CEquipmentSystemForm)

protected:
	CEquipmentSystemForm();           // 動的生成で使用される protected コンストラクター
	virtual ~CEquipmentSystemForm();

public:
	enum { IDD = IDD_TAB_BLOCK };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	COperationTimeLineDoc* GetDocument() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CFapanel1 m_FaBlock;
};



#ifndef _DEBUG  // OperationTimeLineView.cpp のデバッグ バージョン
inline COperationTimeLineDoc* CEquipmentSystemForm::GetDocument() const
{
	return reinterpret_cast<COperationTimeLineDoc*>(m_pDocument);
}
#endif

