#pragma once
#include "afxwin.h"
#include "RangePropagationView.h"
#include "afxcmn.h"

#define RNG_STATUS_NUM		12


typedef struct {
	UINT		id;				// �R���g���[��ID
	CString		nameK;			// �Ď���
	UINT		type;			// 0:������ 1:���l 2:�����_ 3:TIME
} structRngParam;

#ifdef _DEBUG_RNGPROPDUMMY
//----------------------------------------------------------------------
// �_�~�[�f�[�^
typedef struct {
	UINT		id;				// �R���g���[��ID
	CStringA		status;			// �_�~�[������
	double		val;			// �_�~�[�l
} structDummy;

static structDummy mRngDummy[RNG_STATUS_NUM] = {
	{ IDC_RANGEPROPASTATUSMON_EDIT1, ("1703279999.anp"), 0 },	// PRED FILE 
	{ IDC_RANGEPROPASTATUSMON_EDIT2, ("HAYABUSA2"), 0 },	// SAT NAME
	{ IDC_RANGEPROPASTATUSMON_EDIT3, (""), 0 },		// DATA TIME 
	{ IDC_RANGEPROPASTATUSMON_EDIT4, ("START"), 0 },	// RNG STATUS
	{ IDC_RANGEPROPASTATUSMON_EDIT5, (""), 20 },	// RNG NUM
	{ IDC_RANGEPROPASTATUSMON_EDIT6, ("TONE"), 0 },	// RNG FORMAT
	{ IDC_RANGEPROPASTATUSMON_EDIT7, (""), 0 },		// RNG SEND COUNT 
	{ IDC_RANGEPROPASTATUSMON_EDIT8, (""), 0 },	// RNG RCV COUNT
	{ IDC_RANGEPROPASTATUSMON_EDIT9, (""), 0 },		// NEXT RCV TIME 
//	{ IDC_RANGEPROPASTATUSMON_EDIT10, (""), 4506708810000 },	// RANGE 
	{ IDC_RANGEPROPASTATUSMON_EDIT10, (""), 450670.881 },	// RANGE 
	{ IDC_RANGEPROPASTATUSMON_EDIT11, (""), -1881 },	// RANGE RATE GetRngStatus
	{ IDC_RANGEPROPASTATUSMON_EDIT12, (""), 0 },		// RTT
};

static char mRngAreaDummy[][64] = {
	"X/X(�]��)", 
	"X/X(�Đ�)",
	"X/Ka(�]��)",
	"X/Ka(�Đ�)", 
};

#if 0
static structStepDummy mRngStepDummy[eRngStep_Num] = {
	{ _T("CTRL.PASS_FLAG"), _T(""), 1 },		        // Step1
	{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), 0 },		// Step2
	{ _T(""), _T(""), 0 },		        // Step3
	{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), 0 },	// Step4
	{ _T("SS-TCRRARR.RNG_TX_STS"), _T("A_CODE"), 0 },		// Step5
	{ _T(""), _T(""), 0 },		        // Step6
};
#endif
#endif

static structRngParam mRngParam[eStation_MAX][eRngTransMode_Num][RNG_STATUS_NUM] = {
	// �P�c64m
	{
		// S/S
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"),            eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"),             eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""),                          eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("SS-TCRDEM.RNG_CONT"),        eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"),             eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T("SS-TCRDEM.RNG_MODE"),        eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""),                          eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.SS_RNG_GOT_CNT"),       eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""),                          eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_SS_DATA_KM"),      eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_SS_DATA"),        eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""),                         eControlNone },		// RTT
		},

		// X/X�i���n�F�]���j
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"),            eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"),             eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""),                          eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("XX-RARR.RNG_CONT"),          eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"),             eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T(""),                          eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""),                          eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.XX_RNG_GOT_CNT"),       eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""),                          eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_XX_DATA_KM"),      eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_XX_DATA"),        eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""),                         eControlNone },		// RTT
		},

		// X/X�i���n�F�Đ��j
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"),            eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"),             eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""),                          eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("XX-RRNG.RNG_CONT"),          eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"),             eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T(""),                          eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""),                          eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.RX_RNG_GOT_CNT"),       eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""),                          eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_RX_DATA_KM"),      eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_XX_DATA"),        eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""),                         eControlNone },		// RTT
		},

		// X/X�i�V�n�j
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"),            eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"),             eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""),                          eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("DSNTCR.RNG_CONT"),           eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"),             eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T(""),                          eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""),                          eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.RX_RNG_GOT_CNT"),       eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""),                          eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_XX_DSN_DATA_KM"),  eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_XX_DSN_DATA"),    eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""),                         eControlNone },		// RTT
		},
	},

	// �P�c54m
	{
		// X/X
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"),            eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"),             eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""),                          eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("DSNTCR.RNG_CONT"),           eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"),             eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T(""),                          eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""),                          eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.RNG_AREA"),             eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""),                          eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_XX_DSN_DATA_KM"),  eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_XX_DSN_DATA"),    eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""),                         eControlNone },		// RTT
		},

		// X/Ka
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"), eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"), eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""), eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("DSNTCR.RNG_CONT"), eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"), eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T(""), eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""), eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.RNG_AREA"), eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""), eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_XKa_DSN_DATA_KM"), eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_XKa_DSN_DATA"), eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""), eControlNone },		// RTT
		},

		{
			{ 0, _T(""), eControlNone },
		},

		{
			{ 0, _T(""), eControlNone },
		}
	},

	// ���V�Y34m
	{
		// S/S
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"),            eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"),             eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""),                          eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("SS-TCRDEM.RNG_CONT"),        eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"),             eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T("SS-TCRDEM.RNG_MODE"),        eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""),                          eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.SS_RNG_GOT_CNT"),       eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""),                          eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_SS_DATA_KM"),      eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_SS_DATA"),        eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""),                         eControlNone },		// RTT
		},

		// X/X
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"),            eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"),             eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""),                          eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("DSNTCR.RNG_CONT"),           eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"),             eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T(""),                          eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""),                          eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.RX_RNG_GOT_CNT"),            eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""),                          eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_XX_DSN_DATA_KM"),  eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_XX_DSN_DATA"),    eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""),                         eControlNone },		// RTT
		},

		{
			{ 0, _T(""), eControlNone },
		},
		{
			{ 0, _T(""), eControlNone },
		}
	},

	// ���V�Y20m
	{
		// S/S
		{
			{ IDC_RANGEPROPASTATUSMON_EDIT1, _T("CTRL.PRED_FILE"),            eControlStatus },	// PRED FILE 
			{ IDC_RANGEPROPASTATUSMON_EDIT2, _T("CTRL.SAT_NAME"),             eControlStatus },	// SAT NAME
			{ IDC_RANGEPROPASTATUSMON_EDIT3, _T(""),                          eControlNone },		// DATA TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT4, _T("SS-TCRDEM.RNG_CONT"),        eControlStatus },	// RNG STATUS
			{ IDC_RANGEPROPASTATUSMON_EDIT5, _T("V-EQU.RNG_NUM"),             eControlDouble },	// RNG NUM
			{ IDC_RANGEPROPASTATUSMON_EDIT6, _T("SS-TCRDEM.RNG_MODE"),        eControlStatus },	// RNG FORMAT
			{ IDC_RANGEPROPASTATUSMON_EDIT7, _T(""),                          eControlNone },		// RNG SEND COUNT 
			{ IDC_RANGEPROPASTATUSMON_EDIT8, _T("CTRL.SS_RNG_GOT_CNT"),       eControlDouble },	// RNG RCV COUNT
			{ IDC_RANGEPROPASTATUSMON_EDIT9, _T(""),                          eControlNone },		// NEXT RCV TIME 
			{ IDC_RANGEPROPASTATUSMON_EDIT10, _T("CTRL.AMB_SS_DATA_KM"),      eControlDouble },	// RANGE 
			{ IDC_RANGEPROPASTATUSMON_EDIT11, _T("CTRL.DIST_SS_DATA"),        eControlDouble },	// RANGE RATE 
			{ IDC_RANGEPROPASTATUSMON_EDIT12, _T(""),                         eControlNone },		// RTT
		},

		{
			{ 0, _T(""), eControlNone },
		},
		{
			{ 0, _T(""), eControlNone },
		},
		{
			{ 0, _T(""), eControlNone },
		}
	},
};


// CRangePropagationStatusMonitor �_�C�A���O

class CRangePropagationStatusMonitor : public CDialogBase
{
	DECLARE_DYNAMIC(CRangePropagationStatusMonitor)

public:
	CRangePropagationStatusMonitor(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CRangePropagationStatusMonitor();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_RANGEPROPAGATIONSTATUSMONITOR };

	static const int m_nNoWidth = 100;
	static const int m_nRngNumWidth = 200;
	static const int m_nTxTimeWidth = 150;
	static const int m_nScTimeWidth = 150;
	static const int m_nRxTimeWidth = 150;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CEdit m_CEditDetailMon;
	CStatic m_viewArea;
	CRangePropagationView* m_pView;
	CListCtrl m_detailList;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL InitList();
	BOOL InsertListItem();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	void UpdateStatus();
	void UpdateStatus54();
	void InitStatus();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/* ------------------------------------------------------------------------------------ */
};
