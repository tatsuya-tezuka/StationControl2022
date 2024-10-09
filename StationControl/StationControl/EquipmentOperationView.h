#pragma once

#include "GridCtrl.h"
#include "TrackingCtrl.h"
#include "SummaryGraphCtrl.h"
#include "ForecastRadarGraphView.h"

// CEquipmentOperationView �r���[

class CEquipmentOperationView : public CView
{
	DECLARE_DYNCREATE(CEquipmentOperationView)

protected:
	CEquipmentOperationView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CEquipmentOperationView();

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
	CGridCtrl			mGridEquipment;
	CGridCtrl			mGridOperation;
	CTrackingCtrl		mTrack;
	CForecastRadarGraphView*	mpTrackView;
	CSummaryGraphCtrl	mSummary[mSummaryGraphMax];
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */

	// ����
public:
	CStationControlDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // StationControlView.cpp �̃f�o�b�O �o�[�W����
inline CStationControlDoc* CEquipmentOperationView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
