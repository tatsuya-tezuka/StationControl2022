#pragma once

#include <vector>
using namespace std;

#include "MFCEqTabCtrl.h"
#include "TabAntennaView34.h"
#include "TabXsendView.h"
#include "TabXrecvView.h"
#include "TabXmeasView.h"
#include "EqStatusGridView.h"
#include "ForecastRadarGraphView.h"
#include "SummaryGraphCtrl.h"
#include "SummaryPropertiesDlg.h"

// CEquipmentStatusView �t�H�[�� �r���[

class CEquipmentStatusView34 : public CFormView
{
	DECLARE_DYNCREATE(CEquipmentStatusView34)

public:
	CEquipmentStatusView34();   // �W���R���X�g���N�^�[
	virtual ~CEquipmentStatusView34();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_EQSTATUS34 };

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
	stTabInfo			mTabList[eTabMax];
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
	void	createTab34();
	void	createTab20();
	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CStatic mLocAntenna;
	CStatic mLocXsend;
	CStatic mLocXrecvNew;
	CStatic mLocXrecvOld;
	CStatic mLocXmeas;
	CStatic mLocStatus;
	CStatic mLocTarget;
	CStatic mLocGraph1;
	CStatic mLocGraph2;
	CStatic mLocGraph3;
	CStatic mLocGraph4;
	virtual void OnInitialUpdate();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // EquipmentStatusView.cpp �̃f�o�b�O �o�[�W����
inline CStationControlDoc* CEquipmentStatusView34::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
