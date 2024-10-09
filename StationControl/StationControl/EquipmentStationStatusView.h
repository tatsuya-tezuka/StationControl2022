#pragma once

#include "MFCEqTabCtrl.h"
#include "EqStatusGridView.h"
#include "TabOperationView.h"
#include "ForecastRadarGraphView.h"
#include "SummaryGraphCtrl.h"
#include "SummaryPropertiesDlg.h"


// CEquipmentStationStatusView �t�H�[�� �r���[

class CEquipmentStationStatusView : public CFormView
{
	DECLARE_DYNCREATE(CEquipmentStationStatusView)

protected:
	CEquipmentStationStatusView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CEquipmentStationStatusView();

public:
	enum { IDD = IDD_DIALOG_EQSTATIONSTATUS };

	// ����
public:
	CStationControlDoc* GetDocument() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static	const UINT mSummaryGraphMax = 4;
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CFont				mTabFont;
	SIZE				mSizeTotal, mSizePage, mSizeLine;
	stTabInfo			mTabList[eTab5Max];
	CEqStatusGridView*	mEqStatusView;
	CForecastRadarGraphView*	mTrackView;
	CSummaryGraphCtrl	mSummary[mSummaryGraphMax];
	CSummaryPropertiesDlg	mSummaryProperties[mSummaryGraphMax];
	vector<CWnd*>		mControlList;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	// ���C�A�E�g�ǂݍ��݁��ۑ��i�ݒ���j
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);
protected:
	void	createTab();
	int	GetViewID(CView* pView);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CStatic mLocStatus;
	CStatic mLocOperation;
	CStatic mLocTarget;
	CStatic mLocGraph1;
	CStatic mLocGraph2;
	CStatic mLocGraph3;
	CStatic mLocGraph4;
	virtual void OnInitialUpdate();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};

#ifndef _DEBUG  // EquipmentStatusView.cpp �̃f�o�b�O �o�[�W����
inline CStationControlDoc* CEquipmentStationStatusView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
