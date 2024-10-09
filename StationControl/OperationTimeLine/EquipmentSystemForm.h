#pragma once

#include "fapanel1.h"


// CEquipmentSystemForm �t�H�[�� �r���[

class CEquipmentSystemForm : public CFormView
{
	DECLARE_DYNCREATE(CEquipmentSystemForm)

protected:
	CEquipmentSystemForm();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CFapanel1 m_FaBlock;
};



#ifndef _DEBUG  // OperationTimeLineView.cpp �̃f�o�b�O �o�[�W����
inline COperationTimeLineDoc* CEquipmentSystemForm::GetDocument() const
{
	return reinterpret_cast<COperationTimeLineDoc*>(m_pDocument);
}
#endif

