#pragma once

#include "MFCEqTabCtrl.h"
#include "TabBlock.h"
#include "TabAntenna.h"

enum{
	eTabBlock,
	//eTabAntenna,

	eTabMax,
};

typedef struct  {
	UINT			id;
	CRuntimeClass*	pset;
	CString			name;
} stTabItem;
static const stTabItem mTabItem[] = {
	{ IDD_TAB_BLOCK, RUNTIME_CLASS(CTabBlock), _T("FA-Panel"), },
	{ IDD_TAB_ANTENNA, RUNTIME_CLASS(CTabAntenna), _T("�A���e�i"), },
};

// CEquipmentSystem �t�H�[�� �r���[

class CEquipmentSystem : public CFormView
{
	DECLARE_DYNCREATE(CEquipmentSystem)

protected:
	CEquipmentSystem();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CEquipmentSystem();

public:
	enum { IDD = IDD_DIALOG_EQSYSTEM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	COperationTimeLineDoc* GetDocument() const;
	CDialogEx *mpDlg;

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
	SIZE				mSizeTotal, mSizePage, mSizeLine;
	int					mSaveCx, mSaveCy;
	CMFCEqTabCtrl		mTabCtrl;
	vector<CWnd*>		mControlList;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	void	createTab();

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // OperationTimeLineView.cpp �̃f�o�b�O �o�[�W����
inline COperationTimeLineDoc* CEquipmentSystem::GetDocument() const
{
	return reinterpret_cast<COperationTimeLineDoc*>(m_pDocument);
}
#endif

