#pragma once
#include "TabAntennaView.h"
#include "TabSmeasView.h"
#include "TabXmeasView.h"
#include "TabXmeasOldView.h"
#include "TabXremeasOldView.h"
#include "TabSsendView.h"
#include "TabXsendView.h"
#include "TabXsendOldView.h"
#include "TabSrecvView.h"
#include "TabXrecvView.h"
#include "TabXrecvOldView.h"
#include "TabOperationView.h"
#include "TabAntennaView34.h"
#include "TabSsendView34.h"
#include "TabXsendView34.h"
#include "TabSrecvView34.h"
#include "TabXrecvView34.h"
#include "TabSmeasView34.h"
#include "TabScaptView34.h"
#include "TabXcaptView34.h"
#include "TabXmeasView34.h"
#include "TabStrackView34.h"
#include "TabXhspdView34.h"
#include "TabXtrackView34.h"
#include "TabAntennaView20.h"
#include "TabSsendView20.h"
#include "TabSrecvView20.h"
#include "TabXrecvView20.h"
#include "TabSmeasView20.h"
#include "TabScaptView20.h"
#include "TabXhspdView20.h"
#include "TabXmeasView20.h"
#include "TabStrackView20.h"
#include "TabAntennaView54.h"
#include "TabXsendView54.h"
#include "TabXrecvView54.h"
#include "TabKarecvView54.h"
#include "TabDsnModemView54.h"
#include "TabXmeasView54.h"

enum{
	eTabAntenna,
	eTabXsend,
	eTabXrecvNew,
	eTabXrecvOld,
	eTabXmeas,
	eTabStatus,
	eTabTarget,
	eTabGraph1,
	eTabGraph2,
	eTabGraph3,
	eTabGraph4,

	eTabMax,
};

enum{
	eTab54Antenna,
	eTab54Xsend,
	eTab54Xrecv,
	eTab54Karecv,
	eTab54Dsnmodem,
	eTab54Xmeas,
	eTab54Status,
	eTab54Target,
	eTab54Graph1,
	eTab54Graph2,
	eTab54Graph3,
	eTab54Graph4,

	eTab54Max,
};

enum{
	eTab5Status,
	eTab5Operation,
	eTab5Target,
	eTab5Graph1,
	eTab5Graph2,
	eTab5Graph3,
	eTab5Graph4,

	eTab5Max,
};

static const UINT mTabLocationID[] = {
	IDC_ANTENNA_LOC,
	IDC_XSEND_LOC,
	IDC_XRECVNEW_LOC,
	IDC_XRECVOLD_LOC,
	IDC_XMEAS_LOC,
	IDC_STATUS_LOC,
	IDC_TARGET_LOC,
	IDC_GRAPH1_LOC,
	IDC_GRAPH2_LOC,
	IDC_GRAPH3_LOC,
	IDC_GRAPH4_LOC,
};

static const UINT mTabLocationID54[] = {
	IDC_ANTENNA_LOC,
	IDC_XSEND_LOC,
	IDC_XRECV_LOC,
	IDC_KARECV_LOC,
	IDC_DSNMODEM_LOC,
	IDC_XMEAS_LOC,
	IDC_STATUS_LOC,
	IDC_TARGET_LOC,
	IDC_GRAPH1_LOC,
	IDC_GRAPH2_LOC,
	IDC_GRAPH3_LOC,
	IDC_GRAPH4_LOC,
};

static const UINT mTab5LocationID[] = {
	IDC_STATUS_LOC,
	IDC_OPERATION_LOC,
	IDC_TARGET_LOC,
	IDC_GRAPH1_LOC,
	IDC_GRAPH2_LOC,
	IDC_GRAPH3_LOC,
	IDC_GRAPH4_LOC,
};

#define	MAX_TABITEM	10

enum{
	eViewType_Dialog,
	eViewType_StatusGridView,
	eViewType_TrackView,
	eViewType_SummaryGraph,
	eViewType_SummaryGraphProperties,
	eViewType_Window,
	eViewType_Form,
	eViewType_Cntrol,
};

typedef struct  {
	UINT			type;
	UINT			id;
	CRuntimeClass*	pset;
	CString			name;
} stTabItem;
static const stTabItem mTabItemID[eTabMax+1][MAX_TABITEM] = {
	{
		{ eViewType_Dialog, IDD_TAB_ANTENNA, RUNTIME_CLASS(CTabAntennaView), _T("�A���e�i"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SSEND, RUNTIME_CLASS(CTabSsendView), _T("S�ё��M"), },
		{ eViewType_Dialog, IDD_TAB_XSEND, RUNTIME_CLASS(CTabXsendView), _T("X�ё��M�i�V�n�j"), },
		{ eViewType_Dialog, IDD_TAB_XSENDOLD, RUNTIME_CLASS(CTabXsendOldView), _T("X�ё��M�i���n�j"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SRECV, RUNTIME_CLASS(CTabSrecvView), _T("S�ю�M"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_XRECV, RUNTIME_CLASS(CTabXrecvView), _T("X�ю�M�i�V�n�j"), },
		{ eViewType_Dialog, IDD_TAB_XRECVOLD, RUNTIME_CLASS(CTabXrecvOldView), _T("X�ю�M�i���n�j"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SMEAS, RUNTIME_CLASS(CTabSmeasView), _T("S�ё���"), },
		{ eViewType_Dialog, IDD_TAB_XMEAS, RUNTIME_CLASS(CTabXmeasView), _T("X�ё����i�V�n�j"), },
		{ eViewType_Dialog, IDD_TAB_XMEASOLD, RUNTIME_CLASS(CTabXmeasOldView), _T("X�ё����i���n�j"), },
		{ eViewType_Dialog, IDD_TAB_XREMEASOLD, RUNTIME_CLASS(CTabXremeasOldView), _T("X�эĐ������i���n�j"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_StatusGridView, 1, 0,	_T("�ݔ����"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_TrackView, 1, 0,	_T("�ǔ��O���t"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 1, 0,	_T("�O���t-�P"), },
		{ eViewType_SummaryGraphProperties, 1, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 2, 0,	_T("�O���t-�Q"), },
		{ eViewType_SummaryGraphProperties, 2, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 3, 0,	_T("�O���t-�R"), },
		{ eViewType_SummaryGraphProperties, 3, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 4, 0,	_T("�O���t-�S"), },
		{ eViewType_SummaryGraphProperties, 4, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ 0, 0, 0,	_T(""), },
	},
};
static const stTabItem mTabItemID54[eTab54Max + 1][MAX_TABITEM] = {
	{
		{ eViewType_Dialog, IDD_TAB_ANTENNA54, RUNTIME_CLASS(CTabAntennaView54), _T("�A���e�i"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_XSEND54, RUNTIME_CLASS(CTabXsendView54), _T("X�ё��M"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_XRECV54, RUNTIME_CLASS(CTabXrecvView54), _T("X�ю�M"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_KARECV54, RUNTIME_CLASS(CTabKarecvView54), _T("Ka�ю�M"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_DSNMODEM54, RUNTIME_CLASS(CTabDsnModemView54), _T("DSN-MODEM"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_XMEAS54, RUNTIME_CLASS(CTabXmeasView54), _T("����"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_StatusGridView, 1, 0, _T("�ݔ����"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_TrackView, 1, 0, _T("�ǔ��O���t"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 1, 0, _T("�O���t-�P"), },
		{ eViewType_SummaryGraphProperties, 1, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 2, 0, _T("�O���t-�Q"), },
		{ eViewType_SummaryGraphProperties, 2, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 3, 0, _T("�O���t-�R"), },
		{ eViewType_SummaryGraphProperties, 3, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 4, 0, _T("�O���t-�S"), },
		{ eViewType_SummaryGraphProperties, 4, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ 0, 0, 0, _T(""), },
	},
};
static const stTabItem mTabItemID34[eTabMax + 1][MAX_TABITEM] = {
	{
		{ eViewType_Dialog, IDD_TAB_ANTENNA_U34, RUNTIME_CLASS(CTabAntennaView34), _T("�A���e�i"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SSEND34, RUNTIME_CLASS(CTabSsendView34), _T("S�ё��M"), },
		{ eViewType_Dialog, IDD_TAB_XSEND34, RUNTIME_CLASS(CTabXsendView34), _T("X�ё��M"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SRECV34, RUNTIME_CLASS(CTabSrecvView34), _T("S�ю�M"), },
		{ eViewType_Dialog, IDD_TAB_SCAPT34, RUNTIME_CLASS(CTabScaptView34), _T("S�ѕߑ�"), },
		{ eViewType_Dialog, IDD_TAB_STRACK34, RUNTIME_CLASS(CTabStrackView34), _T("S�ђǔ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_XRECV34, RUNTIME_CLASS(CTabXrecvView34), _T("X�ю�M"), },
		{ eViewType_Dialog, IDD_TAB_XCAPT34, RUNTIME_CLASS(CTabXcaptView34), _T("X�ѕߑ�"), },
		{ eViewType_Dialog, IDD_TAB_XHSPD34, RUNTIME_CLASS(CTabXhspdView34), _T("X�э���"), },
		{ eViewType_Dialog, IDD_TAB_XTRACK34, RUNTIME_CLASS(CTabXtrackView34), _T("X�ђǔ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SMEAS34, RUNTIME_CLASS(CTabSmeasView34), _T("SS�ё���"), },
		{ eViewType_Dialog, IDD_TAB_XMEAS34, RUNTIME_CLASS(CTabXmeasView34), _T("XX�ё���"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_StatusGridView, 1, 0, _T("�ݔ���"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_TrackView, 1, 0, _T("�ǔ��O���t"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 1, 0, _T("�O���t�P"), },
		{ eViewType_SummaryGraphProperties, 1, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 2, 0, _T("�O���t�Q"), },
		{ eViewType_SummaryGraphProperties, 2, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 3, 0, _T("�O���t�R"), },
		{ eViewType_SummaryGraphProperties, 3, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 4, 0, _T("�O���t�S"), },
		{ eViewType_SummaryGraphProperties, 4, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ 0, 0, 0, _T(""), },
	},
};
static const stTabItem mTabItemID20[eTabMax + 1][MAX_TABITEM] = {
	{
		{ eViewType_Dialog, IDD_TAB_ANTENNA20, RUNTIME_CLASS(CTabAntennaView20), _T("�A���e�i"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SSEND20, RUNTIME_CLASS(CTabSsendView20), _T("S�ё��M"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SRECV20, RUNTIME_CLASS(CTabSrecvView20), _T("S�ю�M"), },
		{ eViewType_Dialog, IDD_TAB_SCAPT20, RUNTIME_CLASS(CTabScaptView20), _T("S�ѕߑ�"), },
		{ eViewType_Dialog, IDD_TAB_STRACK20, RUNTIME_CLASS(CTabStrackView20), _T("S�ђǔ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_XRECV20, RUNTIME_CLASS(CTabXrecvView20), _T("X�ю�M"), },
		{ eViewType_Dialog, IDD_TAB_XHSPD20, RUNTIME_CLASS(CTabXhspdView20), _T("X�э���"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_SMEAS20, RUNTIME_CLASS(CTabSmeasView20), _T("SS�ё���"), },
		{ eViewType_Dialog, IDD_TAB_XMEAS20, RUNTIME_CLASS(CTabXmeasView20), _T("SX�ё���"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_StatusGridView, 1, 0, _T("�ݔ���"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_TrackView, 1, 0, _T("�ǔ��O���t"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 1, 0, _T("�O���t�P"), },
		{ eViewType_SummaryGraphProperties, 1, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 2, 0, _T("�O���t�Q"), },
		{ eViewType_SummaryGraphProperties, 2, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 3, 0, _T("�O���t�R"), },
		{ eViewType_SummaryGraphProperties, 3, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 4, 0, _T("�O���t�S"), },
		{ eViewType_SummaryGraphProperties, 4, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ 0, 0, 0, _T(""), },
	},
};

static const stTabItem mTab5ItemID[eTab5Max + 1][MAX_TABITEM] = {
	{
		{ eViewType_StatusGridView, 1, 0, _T("�ݔ����"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_Dialog, IDD_TAB_OPERATION, RUNTIME_CLASS(CTabOperationView), _T("�^�p��"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_TrackView, 1, 0, _T("�ǔ��O���t"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 1, 0, _T("�O���t-�P"), },
		{ eViewType_SummaryGraphProperties, 1, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 2, 0, _T("�O���t-�Q"), },
		{ eViewType_SummaryGraphProperties, 2, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 3, 0, _T("�O���t-�R"), },
		{ eViewType_SummaryGraphProperties, 3, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ eViewType_SummaryGraph, 4, 0, _T("�O���t-�S"), },
		{ eViewType_SummaryGraphProperties, 4, 0, _T("�ݒ�"), },
		{ 0, 0, 0, _T(""), },
	},
	{
		{ 0, 0, 0, _T(""), },
	},
};

// CMFCEqTabCtrl

class CMFCEqTabCtrl : public CMFCTabCtrl
{
	DECLARE_DYNAMIC(CMFCEqTabCtrl)

public:
	CMFCEqTabCtrl();
	virtual ~CMFCEqTabCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eTabType
	{
		eTabType_Normal = 0,		/* �ʏ�^�u */
		eTabType_SummaryGraph,		/* �T�}���O���t */
	};

protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT	m_nTabType;
	CSummaryProperties* m_pProp;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	Initialize();
	void	SetTabType(UINT nTabType)	{ m_nTabType = nTabType; }
	void	SetProperties(CSummaryProperties* prop)	{ m_pProp = prop; }
	CSummaryProperties*	GetProperties()				{ return m_pProp; }

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/* ------------------------------------------------------------------------------------ */
};

// �ݔ���
typedef struct  {
	CMFCEqTabCtrl	tab;
	UINT			itemCount;
	CDialogEx		dlg[MAX_TABITEM];
	CView*			pview[MAX_TABITEM];
	CWnd*			pwnd[MAX_TABITEM];
} stTabInfo;

