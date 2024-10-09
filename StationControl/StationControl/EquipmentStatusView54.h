#pragma once
#include "afxwin.h"

#include <vector>
using namespace std;

#include "MFCEqTabCtrl.h"
#include "TabAntennaView54.h"
#include "TabXsendView54.h"
#include "TabXrecvView54.h"
#include "TabkarecvView54.h"
#include "TabDsnModemView54.h"
#include "TabXmeasView54.h"
#include "EqStatusGridView.h"
#include "ForecastRadarGraphView.h"
#include "SummaryGraphCtrl.h"
#include "SummaryPropertiesDlg.h"


// CEquipmentStatusView54 �t�H�[�� �r���[

class CEquipmentStatusView54 : public CFormView
{
	DECLARE_DYNCREATE(CEquipmentStatusView54)

protected:
	CEquipmentStatusView54();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CEquipmentStatusView54();

public:
	enum { IDD = IDD_DIALOG_EQSTATUS54 };

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
	stTabInfo			mTabList[eTab54Max];
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CStatic mLocAntenna;
	CStatic mLocXsend;
	CStatic mLocXrecv;
	CStatic mLocKarecv;
	CStatic mLocDsnmodem;
	CStatic mLocXmeas;
	CStatic mLocStatus;
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
inline CStationControlDoc* CEquipmentStatusView54::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif



