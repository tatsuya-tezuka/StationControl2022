#pragma once

#include "const.h"
#include "CommBase.h"
#include "EngValCnv.h"
#include "ComboBoxEx2.h"

using namespace std;


// PLL BW�����`
#define SRCV_PLLBW				_T("SS-TCRDEM.CARR_LOOP")	/* SS-TCRDEM.CARR_LOOP */
#define XRCV_PLLBW				_T("DSNTCR.CARR_LOOP_BAND")	/* DSNTCR.CARR_LOOP_BAND */
#define XRCVOLD_PLLBW			_T("X-RX2.PLL_BW")			/* X-RX2.PLL_BW */

#define SRCV34_PLLBW			_T("SS-TCRDEM.CARR_LOOP")	/* SS-TCRDEM.CARR_LOOP */
#define XRCV34_PLLBW			_T("DSNTCR.CARR_LOOP_BAND")	/* DSNTCR.CARR_LOOP_BAND */
#define SCAP34_PLLBW			_T("S-ACQ_TRKRX.CARR_LOOP_BAND")	/* S-ACQ_TRKRX.CARR_LOOP_BAND */
#define XCAP34_PLLBW			_T("X-ACQ_TRKRX.CARR_LOOP_BAND")	/* X-ACQ_TRKRX.CARR_LOOP_BAND */
#define STRACK34_PLLBW			_T("S-TRKRX.CARR_LBW")		/* S-TRKRX.CARR_LBW */
#define XTRACK34_PLLBW			_T("X-TRKRX.CARR_LOOP_BAND")		/* X-TRKRX.CARR_LOOP_BAND */


#define RARRBAND_ID     0x03002e /* �q�`�q�q�g�p�ш���ł��邱�Ƃ̎��ʂh�c:2000-11-24*/
#define ENDOFSAT_ID     0x03ffff /* �X�̉q�����̒�`�̏I���������h�c */

enum{
	eShareFile_PredList=0x00000001,
	eShareFile_PlanList = 0x00000002,
};


#define	MAX_SIZE_PER_MMDATA		100		// �������}�b�v�f�[�^������̍ő呪��f�[�^��
#define	NUM_AROUND_OPEN_MMDATA	1		// �O��̃������}�b�v�f�[�^�t�@�C���I�[�v����


// �����W�`�����j�^��`
enum eRngTransMode {
	eRngTransMode_SS = 0,
	eRngTransMode_XX_XKA_54m = 0,
	eRngTransMode_XXOldPast = 1,
	eRngTransMode_XX = 1,
	eRngTransMode_XXOldRegene = 2,
	eRngTransMode_XXNew,
	eRngTransMode_Num
};

enum eRngGeneration {
	eRngGeneration_Past = 0,
	eRngGeneration_Regene,
};

enum eRngStep {
	eRngStep_1 = 0,
	eRngStep_2,
	eRngStep_3,
	eRngStep_4,
	eRngStep_5,
	eRngStep_6,
	eRngStep_Num,
};



typedef enum{
	TYPE_AGC_S,
	TYPE_AGC_X,
	TYPE_RNG,
	TYPE_TLM_S,
	TYPE_TLM_X,
	TYPE_CMD,
	TYPE_AGC_KA,
	TYPE_TLM_KA
}eType;

typedef enum{
	SHELL_CTRL,
	SHELL_CALIB,
}eShell;

typedef enum{
	STATE_NONE,
	STATE_PRE,
	STATE_WAIT,
	STATE_RUN,
	STATE_STP,
}eState;

static const int mRdnsRunLogItem = 100;

typedef struct{
	int		item;
	eType	type;
	eShell	ShellType;
	CString	satellite;
	CString Msg1;
	CString Msg2;
	CString MsgCancel;
	CString MonString;
	eState	state;
	int		time;
	UINT	station;
	CString RdnsItem;
}stSendCtrlMsg;

typedef struct{
	CString ctrl;
	CString loopband;
	int		time;
}stCalibPlan;

typedef struct {
	CString		nameK;			// �Ď���
	CString		value1;			// �Ď��l1
	CString		value2;			// �Ď��l2
} structRngStep;

static structRngStep mRngStep[eStation_MAX][eRngTransMode_Num][eRngStep_Num] = {
	// �P�c64m
	{
		// S/S
		{
			{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), _T("") },		        // Step1
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T(""), _T(""), _T("") },		        // Step3
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("SS-TCRRARR.RNG_TX_STS"), _T("A_CODE"), _T("A_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		// X/X�i���n�F�]���j
		{
			{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), _T("") },		        // Step1
			{ _T("XX-RARR.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("XX-RARR.RNG_CONT"), _T("START"), _T("") },		// Step3
			{ _T("XX-RARR.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("XX-RARR.TX_STATUS"), _T("A_CODE"), _T("A_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		// X/X�i���n�F�Đ��j
		{
			{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), _T("") },		        // Step1
			{ _T("XX-RRNG.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("XX-RRNG.RNG_CONT"), _T("START"), _T("") },		// Step3
			{ _T("XX-RRNG.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("XX-RRNGDATA.RNG_TX_STS"), _T("AB_CODE"), _T("AB_CODE") },		// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		// X/X�i�V�n�j
		{
			{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), _T("") },		        // Step1
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("DSNTCR.RNG_MODE"), _T("TURNAROUND"), _T("REGEN") },	// Step3
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("A_CODE"), _T("AB_CODE") },		// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},
	},

	// �P�c54m
	{
		// X/X, X/Ka����
		{
			{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), _T("") },		        // Step1
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("DSNTCR.RNG_MODE"), _T("TURNAROUND"), _T("REGEN") },	// Step3
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("A_CODE"), _T("AB_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},
		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		}

	},

	// ���V�Y34m
	{
		// S/S
		{
			{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), _T("") },		        // Step1
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T(""), _T(""), _T("") },		        // Step3
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("STOP") },	// Step4
			{ _T("SS-TCRRARR.RNG_TX_STS"), _T("A_CODE"), _T("A_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		// X/X
		{
			{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), _T("") },		        // Step1
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T("DSNTCR.RNG_MODE"), _T("TURNAROUND"), _T("REGEN") },	// Step3
			{ _T("DSNTCR.RNG_CONT"), _T("START"), _T("STOP") },		// Step4
			{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("A_CODE"), _T("AB_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		}
	},

	// ���V�Y20m
	{
		// S/S
		{
			{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), _T("") },		        // Step1
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("") },		// Step2
			{ _T(""), _T(""), _T("") },		        // Step3
			{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), _T("") },		// Step4
			{ _T("SS-TCRRARR.RNG_TX_STS"), _T("A_CODE"), _T("A_CODE") },	// Step5
			{ _T(""), _T(""), _T("") },		        // Step6
		},

		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		},
		{
			{ _T(""), _T(""), _T("") },
		}
	},

};

#ifdef _DEBUG_RNGPROPDUMMY
//----------------------------------------------------------------------------------------
// �_�~�[�f�[�^
typedef struct {
	CString		nameK;			// �Ď���
	CString		status;			// �_�~�[������
	double		val;			// �_�~�[�l
} structStepDummy;

static structStepDummy mRngStepDummy[5][eRngStep_Num] = {
	// S/S
	{
		{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), 1 },		    // Step1
		{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), 0 },		// Step2
		{ _T(""), _T(""), 0 },								// Step3
		{ _T("SS-TCRDEM.RNG_CONT"), _T("START"), 0 },		// Step4
		{ _T("SS-TCRRARR.RNG_TX_STS"), _T("OTHER"), 0 },	// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
	// X/X�i���n�F�]���j
	{
		{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), 1 },		    // Step1
		{ _T("XX-RARR.RNG_CONT"), _T("START"), 0 },			// Step2
		{ _T("XX-RARR.RNG_CONT"), _T("START"), 0 },			// Step3
		{ _T("XX-RARR.RNG_CONT"), _T("START"), 0 },			// Step4
		{ _T("XX-RARR.TX_STATUS"), _T("OTHER"), 0 },		// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
	// X/X�i���n�F�Đ��j
	{
		{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), 1 },		    // Step1
		{ _T("XX-RRNG.RNG_CONT"), _T("START"), 0 },			// Step2
		{ _T("XX-RRNG.RNG_CONT"), _T("START"), 0 },			// Step3
		{ _T("XX-RRNG.RNG_CONT"), _T("START"), 0 },			// Step4
		{ _T("XX-RRNGDATA.RNG_TX_STS"), _T("OTHER"), 0 },	// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
	// X/X�i���n�F�V�n�A�]���^�j
	{
		{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), 1 },		    // Step1
		{ _T("DSNTCR.RNG_CONT"), _T("START"), 0 },			// Step2
		{ _T("DSNTCR.RNG_MODE"), _T("TURNAROUND"), 0 },		// Step3
		{ _T("DSNTCR.RNG_CONT"), _T("START"), 0 },			// Step4
		{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("OTHER"), 0 },	// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
	// X/X�i���n�F�V�n�A�Đ��^�j
	{
		{ _T("CTRL.PASS_FLAG"), _T("�p�X��"), 1 },		    // Step1
		{ _T("DSNTCR.RNG_CONT"), _T("START"), 0 },			// Step2
		{ _T("DSNTCR.RNG_MODE"), _T("REGEN"), 0 },			// Step3
		{ _T("DSNTCR.RNG_CONT"), _T("START"), 0 },			// Step4
		{ _T("DSNTCR.RNG_TX_SEQUENCE"), _T("OTHER"), 0 },	// Step5
		{ _T(""), _T(""), 0 },								// Step6
	},
};

#endif


// �����W�`���N���X
class CRangePropagation
{
public:
	CRangePropagation();
	virtual ~CRangePropagation();
	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eRngStatus {
		eStop = 0,
		eStart
	};

	enum eRngFormat {
		eTone = 0
	};

	typedef struct {
		UINT	count;
		time_t	tStart;
		double	offset;
		double	pos;
		double  d2WayRange;
		double	rtt;
	} structPropData;

	static const UINT m_nMaxRngNum = 9999;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CTime m_dataTime;
	eRngStatus m_eRngStatus;
	UINT m_nRngNum;
	double m_2WayRange;
	double m_rtt;
	deque<structPropData>	m_queRngSnd;
	deque<structPropData>	m_queRngRcv;
	deque<structPropData>	m_queRngAll;

	UINT m_nSndCount;		// ���M��

	UINT m_nStation;		// ��
	UINT m_nTransMode;		// �ʐM���[�h
	UINT m_nGeneMode;		// �������[�h
	UINT m_nStep;			// �X�e�b�v
	CString m_strLastRngToneStatus;	// �O��̃����W�g�[���X�e�[�^�X

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	CTime GetDataTime()						{ return m_dataTime; }
	eRngStatus GetRngStatus()				{ return m_eRngStatus; }
	double GetRange()						{ return m_2WayRange / 2; }
	double GetRtt()							{ return m_rtt; }
	deque<structPropData>&	GetRngSndData()	{ return m_queRngSnd; }
	deque<structPropData>&	GetRngRcvData()	{ return m_queRngRcv; }
	deque<structPropData>&	GetRngAllData()	{ return m_queRngAll; }
	UINT GetTransMode()						{ return m_nTransMode; }
	UINT GetGeneMode()						{ return m_nGeneMode; }
	UINT GetRngNum()						{ return m_nRngNum; }
	UINT GetSndCount()						{ return m_nSndCount; }
	UINT GetStep()							{ return m_nStep; }

	void InitStatus();
	void InitStatus54();
	void UpdateStep();
	BOOL UpdateStepProc();
	BOOL UpdateStepProc54();
	void DetectRngTone();
	void InitRngTone();
	double Get2WayRng();
	void MoveRngTone();

	double GetKanshiDouble();
	CString GetKanshiString(int step = -1);

	void CreateDummy();

	/* ------------------------------------------------------------------------------------ */
};


// �T�}���v���p�e�B�N���X
class CSummaryProperties
{
public:
	CSummaryProperties();
	CSummaryProperties(UINT nViewID, UINT nStationID, UINT nParentID, CString strWndName);
	virtual ~CSummaryProperties();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT			m_nViewID;					// �r���[ID
	UINT			m_nStationID;				// ��ID
	UINT			m_nParentID;				// �e�E�B���h�EID
	CString			m_strWndName;				// �E�B���h�E��

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL GetViewNameList(vector<CString>& listViewName);
	UINT GetViewID()		{ return m_nViewID; }
	UINT GetStationID()		{ return m_nStationID; }
	UINT GetParentID()		{ return m_nParentID; }
	CString GetWndName()	{ return m_strWndName; }
	/* ------------------------------------------------------------------------------------ */
};


// RF�g���b�v�p��`�t�@�C���N���X
class CRFTripAngleFile
{
public:
	CRFTripAngleFile();
	virtual ~CRFTripAngleFile();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT m_nStation;								// �ǔԍ�
	CString m_strKanshiPower;						// ���M�d�͎擾�p�Ď���
	CString m_strKanshiMod;							// �ϒ��`���擾�p�Ď��� �ϒ��`���̔��肪�s�v�ȏꍇ�A* �Ƃ���
	CString m_strModSectionDef;						// �ʏ�Ƃ͈قȂ�ϒ��`����RF�g���b�v���w�肷��ꍇ�� �Ď��X�e�[�^�X���`����
													// ���̒�`�́A�ϒ��`���Z�N�V������ TRIP_XXXX �� XXXX �ɑΉ�����
	vector<CString> m_listPowerInfo;				// ���M�d��(STSNAME_POWER)�Ŏ擾�����X�e�[�^�X���`���郊�X�g
													// ���̏��ԂɁA�eRF�g���b�v�p�f�[�^���Ή�����B
#if 0
	map<CString, vector<Az_El>> m_mapRftripGene;	// RF�g���b�v�ėp�}�b�v
	map<CString, vector<Az_El>> m_mapRftripMod;		// RF�g���b�v�w��}�b�v
#else
	map<CString, vector<double>> m_mapRftripGene;	// RF�g���b�v�ėp�}�b�v
	map<CString, vector<double>> m_mapRftripMod;	// RF�g���b�v�w��}�b�v

#endif

	CString m_strSatellite;							// �q����

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
//	static CString GetRfTripFilePath(CString strPwr, UINT nStation);
//	static CString GetRfTripFilePath(UINT nStation);

	void SetStation(UINT nStation)		{ m_nStation = nStation; }
	BOOL ReadFile();						// RF�g���b�v�t�@�C���ǂݍ���
	BOOL ReadFile(CString& strSatellite);	// �w��q������RF�g���b�v�t�@�C���ǂݍ���
	BOOL ReadFile(UINT nStation, CString& strSatellite, CString& strPowerValue, CString& strModValue, vector<double>& listAzEl);
			// �w��̋ǁA�w��q������RF�g���b�v�t�@�C���ǂݍ���ŁA���͂̑��M�d�͒l�A�ϒ��`���l�ɉ�����AZ, EL���X�g��Ԃ�

	CString& GetKanshiPower()	{ return m_strKanshiPower; }
	CString& GetKanshiMod()		{ return m_strKanshiMod; }
	CString& GetModSectionDef()	{ return m_strModSectionDef; }
	vector<CString>& GetListPowerInfo()	{ return m_listPowerInfo; }

	BOOL GetAzEl(vector<double>& listAzEl);													// �Ď��̑��M�d�́A�ϒ��`���ɉ�����AZ, EL���X�g��Ԃ�
	BOOL GetAzEl(CString& strPowerValue, CString& strModValue, vector<double>& listAzEl);	// ���͂̑��M�d�͒l�A�ϒ��`���l�ɉ�����AZ, EL���X�g��Ԃ�
};

// ������[�e�B���e�B�N���X
class CPrintUtility
{
public:
	CPrintUtility();
	virtual ~CPrintUtility();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	static BOOL	PrintSjisFile(CString& filepath, CString title = _T(""));
	static BOOL PrintSjisFileList(vector<CString>& fileList, CString title);
	static BOOL	PrintUtf8File(CString& filepath, CString title = _T(""));
	static BOOL	PrintEucFile(CString& filepath, CString title = _T(""));
	static BOOL PrintEucFile(vector<CString>& listFilepath, CString title = _T(""));
	/* ------------------------------------------------------------------------------------ */
};

class CDetailPrint
{
public:
	CDetailPrint(CDC* dc);
	virtual ~CDetailPrint();
	BOOL	StartPrinting(bool bDiary=false);
	void	EndPrinting();
	void	SetTitle(CString& val) { mTitle = val; }
	void	PrintLine(const CString& str, BOOL bHeader = TRUE);
	void	PrintHeader();
	void	SetPrinterFont(bool bDiary = false);
	CFont*	CreateFont(int nPoints, int lfWeight, CString strFaceName=_T(""));
	int		GetPageNumber() { return mPageNumber; }
protected:
	CString	mTitle;
	CDC*	mDc;
	int		mPageVMargin;
	int		mPageHMargin;
	int		mPageHeight;
	int		mPageWidth;
	int		mSaveVMargin;
	int		mLineHeight;
	BOOL	mPageStarted;
	BOOL	mDocStarted;
	int		mPageNumber;

	CFont*	mFontHeader;
	CFont*	mFontSubHeader;
	CFont*	mFontBody;
};


// �t�@�C�����[�e�B���e�B�N���X
class CFileUtility
{
public:
	CFileUtility();
	virtual ~CFileUtility();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	static void DeleteTmpFlie(CString& filepath, int nDay);
	static BOOL	ExchangeFileUtf8toSjis(CString& srcFilePath, CString& dstFilePath);
	static BOOL	ExchangeFileEuctoSjis(CString& srcFilePath, CString& dstFilePath, BOOL bOverWrite = TRUE);
	static CString ConvertEuc2Sjis(char& szEuc);
	/* ------------------------------------------------------------------------------------ */
};


// DB�A�N�Z�X�N���X
/*
*******************************************************************************
* �ݔ�������  ���ʂh�c
*******************************************************************************
*/
/*
���ʂh�c�F�c�a�R���p�C���́A�X�̐��䍀�ږ��ɂ����̏���object��
���ďo�͂���ꍇ�ɁA���̎��ʂh�c�̏��������ɂȂ�тȂ����Ă�������
�i���䍀�ڌX�̒�`�̐擪�͕K��CTLNAME_ID�Ŏn�܂�A�Ō�́A�K��
ENDOFCTL_ID�ŏI���悤�ɂ��Ă��������B�j
*/
#define CTLNAME_ID      0x010001 /* �ݔ����䍀�ږ��̂ł��邱�Ƃ̎��ʂh�c */
#define PARAMTBL_ID     0x010002 /* �p�����[�^�ϊ��̒�`���Ӗ����鎯�ʂh�c */
#define ENDOFPARAM_ID   0x010003 /* �p�����[�^�ϊ��̒�`�I�����Ӗ����鎯�ʂh�c */
#define VAL_ID          0x010004 /* ���l�^�p�����[�^�̒�`���Ӗ����鎯�ʂh�c */
#define ENDOFVAL_ID     0x010005 /* ���l�^�p�����[�^�̒�`�I�����Ӗ����鎯�ʂh�c */
#define STS_ID          0x010006 /* �X�e�[�^�X�^�ϊ��̒�`���Ӗ����鎯�ʂh�c */
#define ENDOFSTS_ID     0x010007 /* �X�e�[�^�X�^�ϊ��̒�`�I�����Ӗ����鎯�ʂh�c */
#define FUNC_ID         0x010008 /* �֐��^�ϊ��̒�`���Ӗ����鎯�ʂh�c */
#define ENDOFFUNC_ID    0x01000a /* �֐��^�ϊ��̒�`�I�����Ӗ����鎯�ʂh�c */
#define NOPARAM_ID      0x01000b /* ���ϊ��^�ϊ��̒�`���Ӗ����鎯�ʂh�c */
#define ENDOFNOPARAM_ID 0x01000c /* ���ϊ��^�ϊ��̒�`�I�����Ӗ����鎯�ʂh�c */
#define SET_ID          0x01000d /* �ݒ�l���̒�`���Ӗ����鎯�ʂh�c */
#define ENDOFSET_ID     0x01000e /* �ݒ�l���̒�`�I�����Ӗ����鎯�ʂh�c */
#define PRECHKTBL_ID    0x01000f /* ����O���؂̊J�n���Ӗ����鎯�ʂh�c */
#define PRECHK_ID       0x010010 /* ����O���؂̌X�̌��؏����Ӗ����鎯�ʂh�c */
#define ENDOFPRECHK_ID  0x010011 /* ����O���؂̏I�����Ӗ����鎯�ʂh�c */
#define VERIFYTBL_ID    0x010012 /* �ƍ������Ӗ����鎯�ʂh�c */
#define VERIFY_ID       0x010013 /* �X�̏ƍ������Ӗ����鎯�ʂh�c */
#define ENDOFVFY_ID     0x010014 /* �ƍ����̏I�����Ӗ����鎯�ʂh�c */
#define AFTCTL_ID       0x010015 /* �����X�e�[�^�X�ύX�̒�`���Ӗ����鎯�ʂh�c */
#define ENDOFAFTCTL_ID  0x010016 /* �����X�e�[�^�X�ύX�̒�`�I�����Ӗ����鎯�ʂh�c */
#define ENDOFCTL_ID     0x01ffff /* �X�̃R�}���h�̒�`���̏I���������h�c */

/*
*******************************************************************************
* �q���ŗL���  ���ʂh�c
*******************************************************************************
*/

#define SATINF_ID       0x030001 /* �q���{�̏��ł��邱�Ƃ̎��ʂh�c */
#define TXFQ_ID         0x030002 /* �m�~�i�����M���g�����ł��邱�Ƃ̎��ʂh�c */
#define TXFQVAL_ID      0x030003 /* �m�~�i�����M���g���ݒ�l���ł��邱�Ƃ̎��ʂh�c */
#define ENDOFTXFQ_ID    0x030004 /* �m�~�i�����M���g�����I���ł��邱�Ƃ̎��ʂh�c */
#define RXFQ_ID         0x030005 /* �m�~�i����M���g�����ł��邱�Ƃ̎��ʂh�c */
#define RXFQVAL_ID      0x030006 /* �m�~�i����M���g���ݒ�l���ł��邱�Ƃ̎��ʂh�c */
#define ENDOFRXFQ_ID    0x030007 /* �m�~�i����M���g�����I���ł��邱�Ƃ̎��ʂh�c */
#define EIRP_ID         0x030008 /* �q���d�h�q�o���ł��邱�Ƃ̎��ʂh�c */
#define ENDOFEIRP_ID    0x030009 /* �q���d�h�q�o���I���ł��邱�Ƃ̎��ʂh�c */
#define SATTX_ID        0x03000a /* ���M�@���ł��邱�Ƃ̎��ʂh�c */
#define ENDOFSATTX_ID   0x03000b /* ���M�@���I���ł��邱�Ƃ̎��ʂh�c */
#define TXPWR_ID        0x03000c /* ���M�@�o�͏��ł��邱�Ƃ̎��ʂh�c */
#define ENDOFTXPWR_ID   0x03000d /* ���M�@�o�͏��I���ł��邱�Ƃ̎��ʂh�c */
#define SATANT_ID       0x03000e /* �q���A���e�i���ł��邱�Ƃ̎��ʂh�c */
#define EIRPVAL_ID      0x03000f /* �d�h�q�o�l���ł��邱�Ƃ̎��ʂh�c */
#define ENDOFSATANT_ID  0x030010 /* �q���A���e�i���I���ł��邱�Ƃ̎��ʂh�c */
#define EQUSE_ID        0x030011 /* �ݔ��g�p�w����ł��邱�Ƃ̎��ʂh�c */
#define EQUSENAME_ID    0x030012 /* �g�p�ݔ������ł��邱�Ƃ̎��ʂh�c */
#define ENDOFEQUSE_ID   0x030013 /* �ݔ��g�p�w����I���ł��邱�Ƃ̎��ʂh�c */
#define AGC_ID          0x030014 /* �`�f�b�Z���f�[�^�t�@�C�����̂ł��邱�Ƃ̎��ʂh�c */
#define RNG_ID          0x030015 /* �q�m�f�Ǔ��f�B���C�l�t�@�C�����̂ł��邱�Ƃ̎��ʂh�c */
#define PRD_ID          0x030016 /* �\��l�t�@�C���ʒu�ł��邱�Ƃ̎��ʂh�c */
#define MACDEF_ID       0x030017 /* �ŗL�}�N�����ł��邱�Ƃ̎��ʂh�c */
#define ENDOFMACDEF_ID  0x030018 /* �ŗL�}�N�����I���ł��邱�Ƃ̎��ʂh�c */
#define MAC_ID          0x030019 /* �}�N�������ł��邱�Ƃ̎��ʂh�c */
#define ENDOFMAC_ID     0x03001a /* �}�N�������I���ł��邱�Ƃ̎��ʂh�c */
#define CASE_ID         0x03001b /* �ݔ�����}�N���̃p�����[�^�ł��邱�Ƃ̎��ʂh�c */
#define BREAK_ID        0x03001c /* �ݔ�����}�N���̃p�����[�^�I���ł��邱�Ƃ̎��ʂh�c */
#define EXEC_ID         0x03001d /* �ݔ�������s���ł��邱�Ƃ̎��ʂh�c */
#define ENDOFEXEC_ID    0x03001e /* �ݔ�������s���ł��邱�Ƃ̎��ʂh�c */
#define TLMBR_ID        0x03001f /* �e�����g���r�b�g���[�g���ł��邱�Ƃ̎��ʂh�c */
#define TLMBRVAL_ID     0x030020 /* �e�����g���r�b�g���[�g�ݒ�l���ł��邱�Ƃ̎��ʂh�c */
#define ENDOFTLMBR_ID   0x030021 /* �e�����g���r�b�g���[�g���I���ł��邱�Ƃ̎��ʂh�c */
#define CMDBR_ID        0x030022 /* �R�}���h�r�b�g���[�g���ł��邱�Ƃ̎��ʂh�c */
#define CMDBRVAL_ID     0x030023 /* �R�}���h�r�b�g���[�g�ݒ�l���ł��邱�Ƃ̎��ʂh�c */
#define ENDOFCMDBR_ID   0x030024 /* �R�}���h�r�b�g���[�g���I���ł��邱�Ƃ̎��ʂh�c */
#define PLAN_ID         0x030025 /* �v��t�@�C���ʒu�ł��邱�Ƃ̎��ʂh�c */
#define PASLOG_ID       0x030026 /* �p�X�����t�@�C���ʒu�ł��邱�Ƃ̎��ʂh�c */
#define CATCH_ID        0x030027 /* �q���⑫���ł��邱�Ƃ̎��ʂh�c */
#define AGCRX_ID        0x030029 /* �`�f�b�Z���Ώێ�M�@���ł��邱�Ƃ̎��ʂh�c */
#define TOPANGL_ID      0x03002a /* �V���ʉߊĎ��p�x���ł��邱�Ƃ̎��ʂh�c */
#define XTX_ID          0x03002b /* �w�ё��M�ݔ��g�p�L�����ł��邱�Ƃ̎��ʂh�c */
#define XRXSEL_ID       0x03002c /* �w�ю�M�ݔ��Ώۏ��ł��邱�Ƃ̎��ʂh�c */
#define RAMPCNT_ID      0x03002d /* ���g�������v���䋖���ł��邱�Ƃ̎��ʂh�c */
#define RARRBAND_ID     0x03002e /* �q�`�q�q�g�p�ш���ł��邱�Ƃ̎��ʂh�c */


#define ENDOFSAT_ID     0x03ffff /* �X�̉q�����̒�`�̏I���������h�c */

//----------------
#define VECMAX			256		/* �c�a�����ő���i�p�����[�^�������ӁB�j */
#define	SATNAME_LEN		16		/* �q�����̍ő咷+1 */
#define	SATCTLNAME_LEN	24		/* �ڑ��q���ǐ����̍ő咷+1 */
#define	TXPWRSTR_LEN	24		/* �q�����M�@�o�͎��ʍő咷+1 */
#define	PRDPATH_LEN		128		/* �\��l�t�@�C���i�[�f�B���N�g�����̍ő咷+1 */
#define	PLANPATH_LEN	128		/* �v��t�@�C���i�[�f�B���N�g�����̍ő咷+1 */
#define	MACNAME_LEN		24		/* �ݔ�����}�N�����ő咷+1 */
#define	MACPARAM_LEN	24		/* �ݔ�����}�N���p�����[�^�ő咷+1 */
#define	EXECSTR_LEN		64		/* �}�N���p�ݔ�������s���ő咷+1 */

#define	CTLNAME_LEN		28								/* �ݔ����䖼�̍ő咷+1 */
#define	EQNAME_LEN		12								/* �ݔ����̍ő咷+1 */
#define	CTLID_LEN		( CTLNAME_LEN + EQNAME_LEN )	/* ���䍀��ID�ő咷+1 */
#define	OPTSTR_LEN		24								/* ���䍀�ڒ�` �t�����ő咷+1 */

//----------------
#define TXBAND_S_STR	"S"	/* �uS����ށv�w�蕶���� */
#define TXBAND_X_STR	"X"	/* �uX����ށv�w�蕶���� */

/* ------------------------------------------------------------------------------------ */
/* ��`                                                                                 */
/* ------------------------------------------------------------------------------------ */
struct res_t { /*���L��������̃e�[�u���\��*/
	long  l_id;
	long  l_len;
	char  buf[1];
};

struct mac_t {	/* �}�N������`�e�[�u��: �\�[�X��"mac"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=MAC_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct mac_t ) */
	char sz_macname[MACNAME_LEN];	/* �ݔ�����}�N���� */
};

struct endofmac_t {	/* �}�N������`�e�[�u���I��: �\�[�X��"endofmac"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=ENDOFMAC_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct endofmac_t ) */
};
struct case_t {	/* �}�N����`�e�[�u��: �\�[�X��"case"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=CASE_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct case_t ) */
	char sz_param[MACPARAM_LEN];	/* �ݔ�����}�N���p�����[�^ */
};

struct exec_t {	/* ���s�ݔ��������`�e�[�u��: �\�[�X��"exec"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=EXEC_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct exec_t ) */
	char sz_ctl[EXECSTR_LEN];	/* �}�N���p�ݔ�������s�� */
	long l_exofs;	/* ���s���ԃI�t�Z�b�g�l:�ȗ����͂O��ݒ肷�� */
	long l_rfu;	/* �p�f�B���O */
};

struct endofexec_t {	/* ���s�ݔ��������`�e�[�u���I��: */
	long l_id;	/* �f�[�^����: �萔=ENDOFEXEC_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct endofexec_t ) */
};


/*
*******************************************************************************
* �ݔ����䖼�̏��
*******************************************************************************
*/
struct ctlname_t {				/* ���̃e�[�u���͑S���䍀�ڂɂ��đ��݂��� */
	long l_id;					/* �f�[�^����: �萔=CTLNAME_ID�l */
	long l_len;					/* �f�[�^��  : sizeof( struct ctlname_t ) */
	long l_no;					/* ���䍀�ڔԍ��i�R���p�C�����V�[�P���V�����ɕt������B�j */
	long l_adr;					/* �ݒu�ʒu���:���j�b�g�A�h���X(3bit) */
	char sz_ctlname[CTLID_LEN];	/* ���䍀�ڂh�c: �ݔ���.���䍀�ږ� */
	char sz_optstr[OPTSTR_LEN];	/* �t����� */

	long l_eqno;				/* �ݔ��ԍ��i���P�j *//* Ver1.05 */
	long l_rfu;					/* �p�f�B���O *//* Ver1.05 */
};

/* ���L
���P�F�R���p�C�����ɁA���䍀�ڂh�c���ݔ������擾���A�ݔ����c�a��
�u�ݔ�����`�v���Q�Ƃ��A�u�ݔ��ԍ��v��ݒ肷��B
*/

/*
*******************************************************************************
* �ϊ�����`
*******************************************************************************
*/

struct paramtbl_t {		/* �ϊ����e�[�u��: �\�[�X��"param"�ɑ��� */
	long l_id;			/* �f�[�^����: �萔=PARAMTBL_ID�l */
	long l_len;			/* �f�[�^��  : sizeof( struct paramtbl_t ) */
	long l_paramno;		/* �p�����|�^�ԍ��i���a�P�j */
	long l_intype;		/* ���͌`��:�i���a�Q�j */
	long l_incolint;	/* ���͌���: �������i���a�S�j*/
	long l_incolflt;	/* ���͌���: �������i���a�S�j�i���a�T�j*/
	long l_incolexp;	/* ���͌���: �w�����i���a�S�j�i���a�T�j*/
	long l_cvttype;		/* �ϊ��`��:�i���a�R�j*/
	long l_cvtcolint;	/* �ϊ�����: �������i���a�S�j */
	long l_cvtcolflt;	/* �ϊ�����: �������i���a�S�j�i���a�T�j*/
	long l_cvtcolexp;	/* �ϊ�����: �w�����i���a�S�j�i���a�T�j*/
	long l_rfu;		/* �p�f�B���O */
};
struct endofparamtbl_t {/* �ϊ����e�[�u���I��: �\�[�X��"endofparam"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=ENDOFPARAM_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct endofparam_t ) */
};


/* ���L
���a�P�F�p�����[�^�͕������݉Ƃ���̂ŁA���̔ԍ���p���āu�扽�p�����[�^�v��
���ł��邩���`����B�i�P�`�j
���a�Q�F�\�[�X�㕶����ŕ\����Ă���`����ID�l�ɕϊ����đ������B
���a�R�F�\�[�X�㕶����ŕ\����Ă���`����ID�l�ɕϊ����đ������B
���a�S�F"n.mEe"�̌`���Ŏw�肳���(n)�𐮐����ɁA(m)���������ɁA(e)���w������
�ݒ肷��B�i"n"�y��"n.m"�̌`���̏ꍇ������B�j
���a�T�F�ȗ����ꂽ�ꍇ�A0��ݒ肷��B
*/

/*
���͌`���������`
*/
#define	IN_D_STR	"d"	/* �A�i���O�l�i�����t��10�i�����́j*/
#define	IN_I_STR	"i"	/* �A�i���O�l�i�����l���́B�擪��"0x"��������"0X"������16�i���j*/
#define	IN_U_STR	"u"	/* �A�i���O�l�i��������10�i�����́j*/
#define	IN_X_STR	"x"	/* �A�i���O�l�i16�i���������́B�擪��"0x"���邢��"0X"���t���Ă��t���Ȃ��Ă��ǂ��B�j*/
#define	IN_F_STR	"f"	/* �A�i���O�l�i���������_���́j*/
#define	IN_S_STR	"s"	/* ������ */
#define	IN_PRED_STR	"P"	/* �\��l�t�@�C�����{�q���� h10.10.30 by yajima */
#define	IN_TIME_STR	"T"	/* �����`��������(HH:MM:SS) */
#define	IN_DATE_STR	"D"	/* �ʎZ���`��������(DDD-HH:MM:SS) */
#define	IN_YEAR_STR	"Y"	/* ���t�`��������(YYYY-MM-DD) */
#define	IN_NONE_STR	""	/* �i�ȗ��j�p�����[�^�����B*/

/*
���͌`���h�c�l��`
*/
#define	IN_D_ID		1	/* �A�i���O�l�i�����t��10�i�����́j*/
#define	IN_I_ID		2	/* �A�i���O�l�i�����l���́B�擪��"0x"��������"0X"������16�i���j*/
#define	IN_U_ID		3	/* �A�i���O�l�i��������10�i�����́j*/
#define	IN_X_ID		4	/* �A�i���O�l�i16�i���������́B�擪��"0x"���邢��"0X"���t���Ă��t���Ȃ��Ă��ǂ��B�j*/
#define	IN_F_ID		5	/* �A�i���O�l�i���������_���́j*/
#define	IN_S_ID		6	/* ������ */
#define	IN_PRED_ID 10	/* �\��l�t�@�C�����{�q���� h10.10.30 by yajima */
#define	IN_TIME_ID	7	/* �����`��������(HH:MM:SS) */
#define	IN_DATE_ID	8	/* �ʎZ���`��������(DDD-HH:MM:SS) */
#define	IN_YEAR_ID	9	/* ���t�`��������(YYYY-MM-DD) */
#define	IN_NONE_ID	-1	/* �i�ȗ��j�p�����[�^�����B*/

/*
�ϊ��`���������`
*/
#define	CVT_B_STR	"B"	/* BCD�ϊ� */
#define	CVT_A_STR	"A"	/* �A�i���O�ϊ� */
#define	CVT_S_STR	"S"	/* �X�e�[�^�X�ϊ� */
#define	CVT_F_STR	"F"	/* �֐��ϊ� */
#define	CVT_NONE_STR	""	/* �i�ȗ��j���ϊ� */
#define	CVT_STR_STR	"Z"	/* ������ϊ� *//* Ver1.07 */
#define CVT_L_STR   "L" /* 64bit�����ϊ� *//* 2013-TRUBO */
#define CVT_U_STR   "U" /* 64bit�������ϊ� *//* 2013-TRUBO */

/*
�ϊ��`���h�c�l��`
*/
#define	CVT_B_ID	1	/* BCD�ϊ� */
#define	CVT_A_ID	2	/* �A�i���O�ϊ� */
#define	CVT_S_ID	3	/* �X�e�[�^�X�ϊ� */
#define	CVT_F_ID	4	/* �֐��ϊ� */
#define	CVT_NONE_ID	-1	/* �i�ȗ��j���ϊ� */
#define	CVT_STR_ID	5	/* ������ϊ� *//* Ver1.07 */
#define CVT_L_ID    6   /* 64bit�����ϊ� *//* 2013-TRUBO */
#define CVT_U_ID    7   /* 64bit�������ϊ� *//* 2013-TRUBO */

enum{
	eControlStatus,
	eControlNumeric,
	eControlDouble,
	eControlTime,
	eControlCommand,
	eControlNone,
};

typedef struct  {
	UINT		type;				// 0:������ 1:���l 2:�����_ 3:TIME
	char		buf[80];
	UINT		val;
	double		fval;
	__int64		tval;
	bool		ret;
} stControlValue;

typedef struct  {
	UINT		id;				// �R���g���[��ID
	CString		nameK;			// �Ď���
	CString		nameS;			// ���䖼
	UINT		type;			// 0:������ 1:���l 2:�����_ 3:TIME
} stControlParam;

typedef struct  {
	UINT		id;				// �R���g���[��ID
	CString		name;			// �Ď���
	UINT		type;			// 0:������ 1:���l 2:�����_ 3:TIME
} stMonitorParam;

/*
*******************************************************************************
* ���l�^�ϊ���`
*******************************************************************************
*/

struct val_t {	/* ���l�^�ϊ��e�[�u��: �\�[�X��"val"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=VAL_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct val_t ) */
	double d_min;	/* �����l */
	double d_max;	/* ����l */
	long double ld_min;	/* �����l *//* 2013-TURBO */
	long double ld_max;	/* ����l *//* 2013-TURBO */
};

struct endofval_t {	/* ���l�^�ϊ��e�[�u���I��: �\�[�X��"endofval"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=ENDOFVAL_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct endofval_t ) */
};

/*
*******************************************************************************
* �X�e�[�^�X�^�ϊ���`
*******************************************************************************
*/

struct sts_t {	/* �X�e�[�^�X�^�ϊ��e�[�u��: �\�[�X��"sts"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=STS_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct sts_t ) */
	char sz_sts[CHKVALSTR_LEN];	/* �X�e�[�^�X������ */
};

struct endofsts_t {	/* �X�e�[�^�X�^�ϊ��e�[�u���I��: �\�[�X��"endofsts"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=ENDOFSTS_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct endofsts_t ) */
};


/*
*******************************************************************************
* �X�̐��䍀�ڂɑ΂���ݔ�������̏I���������i�\�[�X���"end;"�ɑΉ�����j
*******************************************************************************
*/
struct endofctl_t {	/* ���̃e�[�u���͑S���䍀�ڂɂ��đ��݂��� */
	long l_id;	/* �f�[�^����: �萔=ENDOFCTL_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct endofctl_t ) */
};


/*
*******************************************************************************
* �q���{�̏���`
*******************************************************************************
*/

struct satinf_t {	/* ��`�e�[�u��: �\�[�X��"satinf"�ɑ��� */
	long	l_id;						/* �f�[�^����: �萔=SATINF_ID�l */
	long	l_len;						/* �f�[�^��  : sizeof( struct satinf_t ) */
	char	sz_satname[SATNAME_LEN];	/* �q���� */
	long	l_satno;					/* �q���ԍ� */
	long	l_useband;					/* �g�p���g���сi���`�P�j */
	long	l_level;					/* �g�p�ш�i���`�Q�j  */
	long	l_rfu;						/* �p�f�B���O */
	double	d_scoh;						/* �r�уR�[�q�[�����g���V�I:�i���`�R�j */
	double	d_xcoh;						/* �w�уR�[�q�[�����g���V�I:�i���`�R�j */
	long	l_brchg;					/* �����r�b�g���[�g�؂芷���̗L���i���`�S�j */
	long	l_cohmode;					/* �R�q�[�����g���[�h�̗L���i���`�T�j */
	char	sz_satctl[SATCTLNAME_LEN];	/* �ڑ��q���ǐ��� */
};

struct endofsatinf_t {	/* ��`�e�[�u���I��: �\�[�X��"end"�ɑ��� */
	long	l_id;	/* �f�[�^����: �萔=ENDOFSAT_ID�l */
	long	l_len;	/* �f�[�^��  : sizeof( struct endofsatinf_t ) */
};

/*
���g�p���g���с��F�F���g�p���g���т��ȉ��̕�����ɂĎg�p����B
"S":S����ނ̂�,"X":X����ނ̂�,
"S/X":S����ނ�X����ޗ���
*/
#define USEBAND_S_STR	"S"	/* �uS����ނ̂݁v�w�蕶���� */
#define USEBAND_X_STR	"X"	/* �uX����ނ̂݁v�w�蕶���� */
#define USEBAND_SX_STR	"S/X"	/* �uS����ނ�X����ޗ����v�w�蕶���� */

#define USEBAND_S_ID	1	/* �uS����ނ̂݁v�w��h�c�l */
#define USEBAND_X_ID	2	/* �uX����ނ̂݁v�w��h�c�l */
#define USEBAND_SX_ID	3	/* �uS����ނ�X����ޗ����v�w��h�c�l */
#define USEBAND_XX_ID	4	/* �uX/X����ނ̂݁v�w��h�c�l XUP �r�J 00/12/05 */

/*
���g�p�ш恄�F�F���g�p�ш��ȉ��̕�����ɂĎw�肷��B
"STDN"��������"DSN"
*/
#define USE_STDN_STR	"STDN"	/* �u�ߒn���q���v�w�蕶���� */
#define USE_DSN_STR		"DSN"	/* �u�[�F���q���v�w�蕶���� */

#define USE_STDN_ID	1	/* "STDN"�w��h�c�l */
#define USE_DSN_ID	2	/* "DSN"�w��h�c�l */


/*
�������r�b�g���[�g�؊����̗L����
�F�F�������r�b�g���[�g�؂芷�����s���ꍇ��"Y"��ݒ肷��B
�����r�b�g���[�g�؊������s��Ȃ��ꍇ��"N"��ݒ肷��B
*/
#define BRCHG_Y_STR	"Y"	/* "Y"�w�蕶���� */
#define BRCHG_N_STR	"N"	/* "N"�w�蕶���� */

#define BRCHG_Y_ID	1	/* "Y"�w��h�c�l */
#define BRCHG_N_ID	2	/* "N"�w��h�c�l */


/*
���R�q�[�����g���[�h�̗L����
�F�F���R�q�[�����g���[�h������ꍇ�́A"Y"��ݒ肷��B
�R�q�[�����g���[�h���Ȃ��ꍇ�́A"N"��ݒ肷��B
*/
#define COH_Y_STR	"Y"	/* "Y"�w�蕶���� */
#define COH_N_STR	"N"	/* "N"�w�蕶���� */

#define COH_Y_ID	1	/* "Y"�w��h�c�l */
#define COH_N_ID	2	/* "N"�w��h�c�l */

/*
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
* �m�q���d�h�q�o���n���M�@�o�͏���`
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
*/

struct txpwr_t {	/* ���M�@�o�͏���`�e�[�u��: �\�[�X��"txpwr"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=TXPWR_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct txpwr_t ) */
	char sz_txpwr[TXPWRSTR_LEN];	/* ���M�@�o�͎��� */
};

struct endoftxpwr_t {	/* ���M�@�o�͏���`�e�[�u���I��: �\�[�X��"endoftxpwr"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=ENDOFTXPWR_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct endoftxpwr_t ) */
};

/*
*******************************************************************************
* �\��l�t�@�C������`
*******************************************************************************
*/

struct prd_t {	/* �\��l�t�@�C������`�e�[�u��: �\�[�X��"prd"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=PRD_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct prd_t ) */
	char sz_prdpath[PRDPATH_LEN];	/* �\��l�t�@�C���i�[�f�B���N�g���� */
};


/*
*******************************************************************************
* �v��t�@�C������`
*******************************************************************************
*/

struct plan_t {	/* �v��t�@�C������`�e�[�u��: �\�[�X��"plan"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=PLAN_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct plan_t ) */
	char sz_planpath[PLANPATH_LEN];	/* �v��t�@�C���i�[�f�B���N�g���� */
};

/*
*******************************************************************************
* �w�ё��M�ݔ��g�p�L������`
*******************************************************************************
*/
struct xtx_t {	/* �w�ё��M�ݔ��g�p�L������`�e�[�u��: �\�[�X��"xtx"�ɑ��� */
	long l_id;	 /* �f�[�^����: �萔=XTX_ID�l */
	long l_len;	 /* �f�[�^��  : sizeof( struct xtx_t ) */
	long l_x_tx; /* �w�ё��M�ݔ��g�p�L��:XTX_Y_ID .or. XTX_N_ID */
	long l_rfu;  /* �p�f�B���O */
};

#define XTX_Y_STR	"Y"	/* "Y"�w�蕶���� */
#define XTX_N_STR	"N"	/* "N"�w�蕶���� */

#define XTX_Y_ID	1	/* "Y"�w��h�c�l */
#define XTX_N_ID	2	/* "N"�w��h�c�l */

/*
*******************************************************************************
* �w�ю�M�ݔ��Ώۏ���`
*******************************************************************************
*/
struct xrxsel_t {	/* �w�ю�M�ݔ��Ώۏ���`�e�[�u��: �\�[�X��"xrxsel"�ɑ��� */
	long l_id;	     /* �f�[�^����: �萔=XRXSEL_ID�l */
	long l_len;	     /* �f�[�^��  : sizeof( struct xrxsel_t ) */
	long l_x_rx_sel; /* �Ώێ�M�ݔ�:XRXSEL_RX1_ID .or. XRXSEL_RX2_ID*/
	long l_rfu;	     /* �p�f�B���O */
};

#define XRXSEL_RX1_STR	"RX1"	/* �uRX1�v�w�蕶���� */
#define XRXSEL_RX2_STR	"RX2"	/* �uRX2�v�w�蕶���� */

#define XRXSEL_RX1_ID	1	/* �uRX1�v�w��h�c�l */
#define XRXSEL_RX2_ID	2	/* �uRX2�v�w��h�c�l */

/*
*******************************************************************************
* ���g�������v���䋖����`
*******************************************************************************
*/
struct rampcnt_t {    /* ���g�������v���䋖����`�e�[�u��: �\�[�X��"rampcnt"�ɑ��� */
	long l_id;       /* �f�[�^����: �萔=RAMPCNT_ID�l */
	long l_len;       /* �f�[�^��  : sizeof( struct rampcnt_t ) */
	long l_s_tx;      /* �r�ё��M�@���g�������v����  �L���i���`�P�j */
	long l_x_tx;      /* �w�ё��M�@���g�������v���� �L���i���`�P�j */
	long l_s_rx;      /* �r�ю�M�@���g�������v����  �L���i���`�P�j */
	long l_x_rx;      /* �������w�ю�M�@���g�������v����  �L���i���`�P�j */
	long l_x_rx1;     /* �P�c�w�ю�M�@�P���g�������v����  �L���i���`�P�j */
	long l_x_rx2;     /* �P�c�w�ю�M�@�Q���g�������v����  �L���i���`�P�j */
	long l_sx_acqrx;  /* �������r�w�ѕߑ���M�@���g�������v����@�L���i���`�P�j */
	long l_x_qpskrx;  /* �w�э�����M�@���g�������v����  �L���i���`�P�j */
};

/* ���L
���`�P�F�\�[�X�㕶����ŕ\����Ă���`����ID�l�ɕϊ����đ������B
*/

#define RAMPCNT_Y_STR    "Y"    /* "Y"�w�蕶���� */
#define RAMPCNT_N_STR    "N"    /* "N"�w�蕶���� */

#define RAMPCNT_Y_ID    1    /* "Y"�w��h�c�l */
#define RAMPCNT_N_ID    2    /* "N"�w��h�c�l */

/*
*******************************************************************************
* �V���ʉߊĎ��p�x����`
*******************************************************************************
*/
struct topangl_t {	/* �V���ʉߊĎ��p�x����`�e�[�u��: �\�[�X��"topangl"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=TOPANGL_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct topangl_t ) */
	long l_in;  /* �V���ʉߓ��p(deg)�l */
	long l_out; /* �V���ʉߏo�p(deg)�l */
};

/*
*******************************************************************************
* �q���ߑ�����`
*******************************************************************************
*/

struct catch_t {	/* �q���⑫����`�e�[�u��: �\�[�X��"catch;...;.."�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=CATCH_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct catch_t ) */

	long   l_route;					/* �ߑ����[�g�ԍ�               �i�`�P�j */
	long   l_s3way;					/* �r�тR�v�`�x�̗L��           �i�`�P�j */
	long   l_x3way;					/* �w�тR�v�`�x�̗L��           �i�`�P�j */
	long   l_s_txsweep;				/* �r�ё��M�@�r�v�d�d�o�L��     �i�`�P�j */
	long   l_x_txsweep;				/* �w�ё��M�@�r�v�d�d�o�L��     �i�`�P�j */
	long   l_cmdrng;				/* �b�l�c�A�q�m�f�����ϒ��̗L�� �i�`�P�j */
	double d_s_rx_az_errv;			/* S-RX AZ      ���e�덷�d���l           */
	double d_s_rx_el_errv;			/* S-RX EL      ���e�덷�d���l           */
	long   l_s_rx_a_lockon;			/* S-RX Ach     LOCK-ON �����         */
	long   l_s_rx_a_lockoff;		/* S-RX Ach     LOCK-OFF�����         */
	long   l_s_rx_b_lockon;			/* S-RX Bch     LOCK-ON �����         */
	long   l_s_rx_b_lockoff;		/* S-RX Bch     LOCK-OFF�����         */
	double d_x_rx_az_errv;			/* X-RX AZ      ���e�덷�d���l           */
	double d_x_rx_el_errv;			/* X-RX EL      ���e�덷�d���l           */
	long   l_x_rx_a_lockon;			/* X-RX Ach     LOCK-ON �����         */
	long   l_x_rx_a_lockoff;		/* X-RX Ach     LOCK-OFF�����         */
	long   l_x_rx_b_lockon;			/* X-RX Bch     LOCK-ON �����         */
	long   l_x_rx_b_lockoff;		/* X-RX Bch     LOCK-OFF�����         */
	double d_sx_acqrx_az_errv;		/* SX-ACQRX AZ  ���e�덷�d���l           */
	double d_sx_acqrx_el_errv;		/* SX-ACQRX EL  ���e�덷�d���l           */
	long   l_sx_acqrx_a_lockon;		/* SX-ACQRX Ach LOCK-ON �����         */
	long   l_sx_acqrx_a_lockoff;	/* SX-ACQRX Ach LOCK-OFF�����         */
	long   l_sx_acqrx_b_lockon;		/* SX-ACQRX Bch LOCK-ON �����         */
	long   l_sx_acqrx_b_lockoff;	/* SX-ACQRX Bch LOCK-OFF�����         */
	double d_x_qpskrx_az_errv;		/* X-QPSKRX AZ  ���e�덷�d���l           */
	double d_x_qpskrx_el_errv;		/* X-QPSKRX EL  ���e�덷�d���l           */
	long   l_x_qpskrx_a_lockon;		/* X-QPSKRX Ach LOCK-ON �����         */
	long   l_x_qpskrx_a_lockoff;	/* X-QPSKRX Ach LOCK-OFF�����         */
	long   l_x_qpskrx_b_lockon;		/* X-QPSKRX Bch LOCK-ON �����         */
	long   l_x_qpskrx_b_lockoff;	/* X-QPSKRX Bch LOCK-OFF�����         */

	/*2000-11-22�g����:��ʃR���p�`�ɂȂ�悤�Ɉȉ��ɒǉ�*/
	double d_x_rx1_az_errv;			/* X-RX1 AZ      ���e�덷�d���l          */
	double d_x_rx1_el_errv;			/* X-RX1 EL      ���e�덷�d���l          */
	long   l_x_rx1_a_lockon;		/* X-RX1 Ach     LOCK-ON �����        */
	long   l_x_rx1_a_lockoff;		/* X-RX1 Ach     LOCK-OFF�����        */
	long   l_x_rx1_b_lockon;		/* X-RX1 Bch     LOCK-ON �����        */
	long   l_x_rx1_b_lockoff;		/* X-RX1 Bch     LOCK-OFF�����        */
	double d_x_rx2_az_errv;			/* X-RX2 AZ      ���e�덷�d���l          */
	double d_x_rx2_el_errv;			/* X-RX2 EL      ���e�덷�d���l          */
	long   l_x_rx2_a_lockon;		/* X-RX2 Ach     LOCK-ON �����        */
	long   l_x_rx2_a_lockoff;		/* X-RX2 Ach     LOCK-OFF�����        */
	long   l_x_rx2_b_lockon;		/* X-RX2 Bch     LOCK-ON �����        */
	long   l_x_rx2_b_lockoff;		/* X-RX2 Bch     LOCK-OFF�����        */
};
/* ���L
���`�P�F�\�[�X�㕶����ŕ\����Ă���`����ID�l�ɕϊ����đ������B
*/

/*
���r�тR�v�`�x�̗L��          ��
���w�тR�v�`�x�̗L��          ��
���r�ё��M�@�r�v�d�d�o�L��    ��
���w�ё��M�@�r�v�d�d�o�L��    ��
���b�l�c�A�q�m�f�����ϒ��̗L����
�F�F���L��̏ꍇ�́A"Y"��ݒ肷��B
�����̏ꍇ�́A"N"��ݒ肷��B
*/
#define CATCH_Y_STR	"Y"	/* "Y"�w�蕶���� */
#define CATCH_N_STR	"N"	/* "N"�w�蕶���� */

#define CATCH_Y_ID	1	/* "Y"�w��h�c�l */
#define CATCH_N_ID	2	/* "N"�w��h�c�l */


/*
*******************************************************************************
* �`�f�b�Z���Ώێ�M�@����`
*******************************************************************************
*/

struct agcrx_t {	/* �`�f�b�Z���Ώێ�M�@����`�e�[�u��: �\�[�X��"agcrx"�ɑ��� */
	long l_id;			/* �f�[�^����: �萔=AGCRX_ID�l */
	long l_len;			/* �f�[�^��  : sizeof( struct agcrx_t ) */
	long l_s_rx;		/* �r�ю��M�ݔ�  �L���i���`�P�j */
	long l_x_rx;		/* �w�ю��M�ݔ�  �L���i���`�P�j */
	long l_sx_acqrx_s;	/* �r�w�ѕ⑫�p��M�ݔ��@�r�с@�L���i���`�P�j */
	long l_sx_acqrx_x;	/* �r�w�ѕ⑫�p��M�ݔ��@�w�с@�L���i���`�P�j */
	long l_x_qpskrx;	/* �w�э����f�[�^�p��M�ݔ�  �L���i���`�P�j */

	/*2000-11-22�g����:�e�[�u���\������ʃR���p�`�ɂȂ�悤�Ɉȉ��ɒǉ�*/
	long l_x_rx1;		/* �P�c�w�ю�M�@�P  �L���i���`�P�j */
	long l_x_rx2;		/* �P�c�w�ю�M�@�Q  �L���i���`�P�j */

	long l_rfu;			/* �p�f�B���O */
};

struct agcrx54_t {	/* �`�f�b�Z���Ώێ�M�@����`�e�[�u��: �\�[�X��"agcrx"�ɑ��� */
	long l_id;			/* �f�[�^����: �萔=AGCRX_ID�l */
	long l_len;			/* �f�[�^��  : sizeof( struct agcrx54_t ) */
	long l_s_rx;		/* �r�ю��M�ݔ�  �L���i���`�P�j */
	long l_x_rx;		/* �w�ю��M�ݔ�  �L���i���`�P�j */
	long l_sx_acqrx_s;	/* �r�w�ѕ⑫�p��M�ݔ��@�r�с@�L���i���`�P�j */
	long l_sx_acqrx_x;	/* �r�w�ѕ⑫�p��M�ݔ��@�w�с@�L���i���`�P�j */
	long l_x_qpskrx;	/* �w�э����f�[�^�p��M�ݔ�  �L���i���`�P�j */
	long l_x_rx1;		/* �P�c�w�ю�M�@�P  �L���i���`�P�j */
	long l_x_rx2;		/* �P�c�w�ю�M�@�Q  �L���i���`�P�j */
	long l_x_dsntcr1;	/* X��DSN-MODEM1  �L���i���`�P�j */
	long l_ka_dsntcr1;	/* Ka��DSN-MODEM1  �L���i���`�P�j */
	long l_rfu;			/* �p�f�B���O */
};

/* ���L
���`�P�F�\�[�X�㕶����ŕ\����Ă���`����ID�l�ɕϊ����đ������B
*/

/*
���R�q�[�����g���[�h�̗L����
���r�ю��M�ݔ�  �L����
�@���w�ю��M�ݔ�  �L����
 �@���r�w�ѕ⑫�p��M�ݔ��@�r�с@�L����
  �@���r�w�ѕ⑫�p��M�ݔ��@�w�с@�L����
   �@���w�э����f�[�^�p��M�ݔ�  �L����
	�F�F���`�f�b�Z�����s���ꍇ�́A"Y"��ݒ肷��B
	�`�f�b�Z�����s��Ȃ��ꍇ�́A"N"��ݒ肷��B
	*/
#define AGCRX_Y_STR	"Y"	/* "Y"�w�蕶���� */
#define AGCRX_N_STR	"N"	/* "N"�w�蕶���� */

#define AGCRX_Y_ID	1	/* "Y"�w��h�c�l */
#define AGCRX_N_ID	2	/* "N"�w��h�c�l */


/*
*******************************************************************************
* �m�~�i����M���g������`
*******************************************************************************
*/

struct rxfq_t {	/* �m�~�i����M���g������`�e�[�u��: �\�[�X��"rxfq"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=RXFQ_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct rxfq_t ) */
	long l_valnum;	/* �ݒ�l���i���b�P�j */
	long l_rfu;	/* �p�f�B���O */
};

struct rxfqval_t {	/* �m�~�i����M���g���ݒ�l��`�e�[�u��:�i���b�Q�j  */
	long l_id;	/* �f�[�^����: �萔=RXFQVAL_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct rxfqval_t ) */
	long l_no;	/* �ݒ�l�ԍ��i���b�R�j */
	long l_band;	/* �o���h�w��i���b�S�j */
	double d_fq;	/* ��M���g���l[Hz] */
};

struct endofrxfq_t {	/* �m�~�i����M���g������`�e�[�u���I��: �\�[�X��rxfq�s��";"�ɑ��� */
	long l_id;	/* �f�[�^����: �萔=ENDOFRXFQ_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct endofrxfq_t ) */
};

/* ���L
���b�P�F�ݒ肳�ꂽ��M���g���̐�(���ɂ���rxfqval_t�̐�)
->�\�[�X��"rxfq"�����ʂƂ���ub_getln()�ɂ�蓾����B
���b�Q�F�{���ڂ́A�\�[�X��"rxfq arg1,arg2...,argN;"�ƋL�q�����
"arg1,...,argN"�ɑ�������B�i�A�[�M�������g�����m�ۂ���B�j
���b�R�F�{���ڂ́A�\�[�X��"rxfq arg1,arg2...,argN;"�ƋL�q�����
�A�[�M�������g�ɑ΂��A�{�ݒ�l���扽arg�ɑ������邩�������B�i�P�`�j
���b�S�F�\�[�X�㕶����ŕ\����Ă���`����ID�l�ɕϊ����đ������B
*/

/*
����M���g�����F�F���q���ɑ΂���A�Ή��o���h�̃m�~�i����M���g��[Hz]��
�u����ގw��=��M���g���v�̌`���Ŏw�肷��B
�o���h�w����@:'S'-S�o���h,'X'-X�o���h
*/

#define RXBAND_S_ID		1		/* �uS����ށv�w��h�c�l */
#define RXBAND_X_ID		2		/* �uX����ށv�w��h�c�l */
#define RXBAND_KA_ID	3		/* �uKa����ށv�w��h�c�l */


/*
*******************************************************************************
* �q�`�q�q�g�p�ш���
*******************************************************************************
*/
struct rarrband_t {	/* �q�`�q�q�g�p�ш����`�e�[�u��: �\�[�X��"rarrband"�ɑ��� */
	long l_id;	     /* �f�[�^����: �萔=RARRBAND_ID�l */
	long l_len;	     /* �f�[�^��  : sizeof( struct rarrband_t ) */
	long l_rarr_band; /* �g�p�ш�:*/
	long l_rfu;	      /* �p�f�B���O */
};

#define RARRBAND_S_STR		"S"    /* �uS�v�w�蕶���� */
#define RARRBAND_X_STR		"X"	   /* �uX�v�w�蕶���� */
#define RARRBAND_SX_STR		"S/X"  /* �uS/X�v�w�蕶���� */
#define RARRBAND_RX_STR		"RX"   /* �uRX�v�w�蕶���� *//* 2008-01-31 �Đ��^�����Ή��@�ǉ� */
#define RARRBAND_XKA_STR	"XKa"  /* �uXKa�v�w�蕶���� */
#define RARRBAND_RXKA_STR	"RXKa" /* �uRXKa�v�w�蕶���� */

#define RARRBAND_S_ID		1	/* �uS�v�w��h�c�l    */
#define RARRBAND_X_ID		2	/* �uX�v�w��h�c�l    */
#define RARRBAND_SX_ID		3	/* �uS/X�v�w��h�c�l  */
#define RARRBAND_RX_ID		4	/* �uRX�v�w��h�c�l   *//* 2008-01-31 �Đ��^�����Ή��@�ǉ� */
#define RARRBAND_XKA_ID		5	/* �uX/X�v�uX/Ka�v �w��h�c�l */
#define RARRBAND_RXKA_ID	6	/* �uX/X�v�uX/X�Đ��v�uX/Ka�v�uX/Ka�Đ��v�w��h�c�l */


#define ERR_RETURN -1


/*
*******************************************************************************
* �ݔ��Ď���� ���ʕ�����}�N����`
*******************************************************************************
*/
#define OBSNAME_STR       "obs"		/* �ݔ��Ď����̂ł��邱�Ƃ̎��ʕ����� */
#define IF_STR            "if"		/* ������`���Ӗ����鎯�ʕ����� */
#define ELSE_STR          "else"	/* ������`���Ӗ����鎯�ʕ����� */
#define ELSEIF_STR        "elseif"	/* ������`���Ӗ����鎯�ʕ����� */
#define ENDIF_STR         "endif"	/* ������`���Ӗ����鎯�ʕ����� */
#define EXT_STR           "ext"		/* �o�C�i�����o��`�i�������j���Ӗ����鎯�ʕ����� */
#define EXTS_STR          "exts"	/* �o�C�i�����o��`�i�����L�j���Ӗ����鎯�ʕ����� */
#define EXTBCD_STR        "extbcd"	/* �a�b�c���o��`�i�������j���Ӗ����鎯�ʕ����� */
#define EXTBCDS_STR       "extbcds"	/* �a�b�c���o��`�i�����L�j���Ӗ����鎯�ʕ����� */
#define EXTAVG_STR        "extavg"	/* ���o�f�[�^�̕��ρi�������j���Ӗ����鎯�ʕ����� */
#define EXTAVGS_STR       "extavgs"	/* ���o�f�[�^�̕��ρi�����L�j���Ӗ����鎯�ʕ����� */
#define EXTCHAR_STR       "extchar"	/* �����񒊏o�f�[�^���Ӗ����鎯�ʕ����� */
#define EXTCHARDEC_STR    "extchardec"/* ������^���l���o�f�[�^���Ӗ����鎯�ʕ����� */
#define EXTDBL_STR        "extdbl"	/* double���o�f�[�^���Ӗ����鎯�ʕ����� */
#define EXTFLT_STR        "extflt"	/* float���o�f�[�^���Ӗ����鎯�ʕ����� */
#define CATDATA_STR       "catdata"	/* ���̊Ď��f�[�^��A�����鎯�ʕ����� */
#define POL_STR           "pol"		/* �������ϊ���`���Ӗ����鎯�ʕ����� */
#define CARVTBL_STR       "carvtbl"	/* �J�[�u�ϊ���`���Ӗ����鎯�ʕ����� */
#define CAL_STR           "cal"	        /* �����ڌv�Z��`���Ӗ����鎯�ʕ����� */
#define TIMECVT_STR       "timecvt"	/* �����ϊ���`���Ӗ����鎯�ʕ����� */
#define STS_STR           "sts"		/* �X�e�[�^�X��`���Ӗ����鎯�ʕ����� */

#define EQMON_INF_STR     "inf"		/* �t������`���Ӗ����鎯�ʕ�����:2006-08-26 */

#define ENDOFCTL_STR      "end"		/* �X�̃R�}���h�̒�`���̏I�������������� */


#define	OBSNAME_ID		0x0101		/* ������`�h�c */
#define	IF_ID			0x0102		/* ������`�h�c */
#define	ELSE_ID			0x010d		/* ������`�h�c */
#define	ELSEIF_ID		0x010e		/* ������`�h�c */
#define	ENDIF_ID		0x010f		/* ������`�h�c */
#define	EXT_ID			0x0103		/* �o�C�i�����o�i���������j�h�c */
#define	EXTS_ID			0x0104		/* �o�C�i�����o�i�����L��j�h�c */
#define	EXTBCD_ID		0x0105		/* �a�b�c���o�i���������j�h�c */
#define	EXTBCDS_ID		0x0106		/* �a�b�c���o�i�����L��j�h�c */
#define	EXTAVG_ID		0x0107		/* ���o�f�[�^���ρi���������j�h�c */
#define	EXTAVGS_ID		0x0108		/* ���o�f�[�^���ρi�����L��j�h�c */
#define	EXTCHAR_ID		0x0110		/* �����񒊏o�h�c */
#define	EXTCHARDEC_ID	0x0113		/* ������^���l���o�h�c */
#define	EXTDBL_ID		0x0111		/* double���o�h�c */
#define	EXTFLT_ID		0x0114		/* float���o�h�c */
#define POL_ID			0x0109		/* �������ϊ���`�h�c */
/*#define CATDATA_ID		0x0112		*//* ���̊Ď��f�[�^��A������h�c */
#define CARVTBL_ID		0x010a		/* �J�[�u�ϊ���`�h�c */
#define TIMECVT_ID		0x010b		/* �����ϊ���`�h�c */
#define OBSSTS_ID		0x010c		/* �X�e�[�^�X��`�h�c */
#define CAL_ID			0x0112		/* �����ڌv�Z��`�h�c */

#define	EQMON_INF_ID	0x0121		/* "inf":�t������`���Ӗ�����h�c */

#define ENDOFOBS_ID 	-1			/* �X�̃R�}���h�̒�`�̏I���h�c */
#define L_OFFSET        6
#define CARV_MAX        32          /* �J�[�u�e�[�u���̍ő吔 */


class CDBAccess
{
public:
	CDBAccess();
	virtual ~CDBAccess();

public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	// �q���ŗL���DB
	char* m_eqsatdb_data;
	vector<CString> m_satnamelist;
	map<CString, __int64> m_satadrlist;

	// �ݔ�����DB
	char* m_eqctldb_data;
	vector<CString> m_ctlnamelist;
	map<CString, __int64> m_ctladrlist;

	// �Ď��f�[�^���ڃ��X�g
	char* m_eqmondb_data;
	vector<CString> m_monnamelist;

	// �Ď��f�[�^�i��M�Ď��f�[�^�j
	map<UINT, map<CString, CEngValCnv::obsname_t>> m_mondatalist;
//	vector<CEngValCnv::obsname_t> m_mondatalist[4];
//	CEngValCnv::obsname_t	mObsName[4][sizeof(CEngValCnv::obsname_t)];
//	deque<char*> mObList;

	// ���f�B�l�X�`�F�b�N����
	map<CString, int> m_rdychktimelist;
	// �v�掞��
	map<CString, int> m_plntimelist;

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	int eqsat_db_read();		// �q���ŗL���DB�擾
	int eqctl_db_read();		// �ݔ�����DB�擾
	int eqmon_db_read();		// �ݔ�����DB�擾
	int rdychk_read();			// ���f�B�l�X�`�F�b�N���Ԏ擾
	int plntime_read();			// �v�掞�Ԏ擾

	int db_read(char* db_file_nam, char *cp_loadadr_p);

	int ud_adr_to_sattbl(vector<CString>& snamelist);
	int ud_adr_to_ctltbl(vector<CString>& list);
	int ud_adr_to_montbl(vector<CString>& list);

	int ud_adr_to_resvec(char* base_p, long l_id, long l_limitid, long l_endoftblid, char* vec[], int i_argc);
	int ud_macname_to_ststbl(char *base_p, char *psz_macname, char *tbl[], char *vec[], int i_argc);
	int ud_macadr_to_ststbl(char *base_p, char *tbl[], char *vec[], int i_argc);
	int ud_macstsadr_to_exectbl(char *base_p, char *tbl[], char *vec[], int i_argc);
	int ud_macsts_to_exectbl(char *base_p, char *psz_param, char *tbl[], char *vec[], int i_argc);

	vector<CString>&		GetSatNameList() { return m_satnamelist; }
	map<CString, __int64>&	GetSatAdrList() { return m_satadrlist; }

	vector<CString>&		GetCtlNameList() { return m_ctlnamelist; }
	map<CString, __int64>&	GetCtlAdrList() { return m_ctladrlist; }

	vector<CString>&		GetMonNameList() { return m_monnamelist; }

	void SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data);
	char* GetObsname_t(UINT nStation, CString strWatch);

	int GetIdxSatelliteEqSatDB(CString str);

	map<CString, int>& GetRdyChkTimeList() { return m_rdychktimelist; }
	map<CString, int>& GetPlnTimeList() { return m_plntimelist; }

	int GetRdyChkTimeFile(CString str);
	int GetPlnTimeFile(CString str);

#ifdef MON_UT
	map<CString, CEngValCnv::obsname_t> mon_UT;
	void SetMonUT(vector<CEngValCnv::obsname_t>& data);
	map<CString, CEngValCnv::obsname_t> GetMonUT()	{ return mon_UT; }
	CEngValCnv::obsname_t mon_UT_time;
#endif

protected:

};



// �Z�b�V�������N���X
class CSessionInfo
{
public:
	CSessionInfo();
	virtual ~CSessionInfo();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eStatus {
		eUnknown = 0,
		eGettingNow,
		eError,
		eDisconnect,
		eConnect
	};

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	UINT m_sessionStatus[eSessionType_Num];	// �Z�b�V�������

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	UINT GetSessionStatus(UINT pos)					{ return m_sessionStatus[pos]; }
	void SetSessionStatus(UINT pos, UINT eStatus)	
	{ 
		if (pos < eSessionType_Num)
		{
			m_sessionStatus[pos] = eStatus;
		}
		else
		{
			for (int i = 0; i < eSessionType_Num; i++)
			{
				m_sessionStatus[i] = eStatus;
			}
		}
	}
//	void UpdateSessionStatus();
	void ReadStatusFile();
	int GetSessionIdx(CString& str);
	void SetSessionStatus(CString& strSession, CString& strStatus);
};

// �g�����h�f�[�^�N���X
class CTrendData
{
public:
	CTrendData();
	virtual ~CTrendData();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	/*  �\��l�e�[�u���\���� */

	typedef struct {
		CString strSigName;			// �M����
		time_t	t;					// ����
		double	value;				// �l
	} structForecastValue;

	typedef struct	{
		unsigned char  uca_time[PRED_TIME_LEN];             /* ����(BCD:YYYYMMDDHHMMSS)     */
		double d_predict_az;                                /* �\�����ʊp                   */
		double d_predict_el;                                /* �\���p                     */
		double d_predict_uplink_range;                      /* �\���A�b�v�����N�����W       */
		double d_predict_downlink_range;                    /* �\���_�E�������N�����W       */
		double d_predict_uplink_range_rate;                 /* �\���A�b�v�����N�����W���[�g */
		double d_predict_downlink_range_rate;               /* �\���_�E�������N�����W���[�g */
		double d_predict_2way_rng;                          /* �\���Q�v�`�x�����W           *//* <<2013-TURBO>> �ǉ� */
		double d_predict_2way_range_rate;                   /* �\���Q�v�`�x�����W���[�g     *//* <<2013-TURBO>> �ǉ� */
		double d_predict_3way_range_rate;                   /* �\���R�v�`�x�����W���[�g     *//* 2017/12/16 �ǉ� */
		char   ca_predict_s_tx_freq[PRED_FREQ_LEN];         /* �\�����M���g��               */
		char   ca_predict_s_rx_1way_freq[PRED_FREQ_LEN];    /* �r�тP�v�`�x�\����M���g��   */
		char   ca_predict_x_rx_1way_freq[PRED_FREQ_LEN];    /* �w�тP�v�`�x�\����M���g��   */
		char   ca_predict_s_rx_2way_freq[PRED_FREQ_LEN];    /* �r�тQ�v�`�x�\����M���g��   */
		char   ca_predict_x_rx_2way_freq[PRED_FREQ_LEN];    /* �w�тQ�v�`�x�\����M���g��   */
		char   ca_predict_s_rx_1way_ramprate[PRED_RAMP_LEN];/* �r�тP�v�`�x�����v���[�g     */
		char   ca_predict_x_rx_1way_ramprate[PRED_RAMP_LEN];/* �w�тP�v�`�x�����v���[�g     */
		char   ca_predict_s_rx_2way_ramprate[PRED_RAMP_LEN];/* �r�тQ�v�`�x�����v���[�g     */
		char   ca_predict_x_rx_2way_ramprate[PRED_RAMP_LEN];/* �w�тQ�v�`�x�����v���[�g     */
		char   ca_dummy[PRED_DUMY_LEN];              /* �W�o�C�g���E���킹�p�_�~�[          */
	} one_predata_t;

	typedef struct	{
		one_predata_t	predata[MAX_PRED_SIZE];
	}predata_SHM_t;                                 /* �\��l�e�[�u���\����                 */

	typedef struct	{
		long	l_id;
		long	l_size;
		one_predata_t	predata[MAX_PRED_SIZE];
	}predata_t;                                     /* �\��l�e�[�u���\����(�Q�Ɨp)         */

	/*** ���u���\���f�[�^���e�[�u�� ***/
	struct   uni_inf_t {
		int      uni_on;                      /* �\���ΏۗL��(1:ON/0:OFF)   */
//		double   d_end_time;                  /* �ŏI�i�[�f�[�^��������     */
		long     l_end;                       /* �ŏI�i�[�f�[�^�ʒu�m�n     */
		double   d_last_time;                 /* �ŏI�`��f�[�^��������     */
		struct   shmdata_t*  	shmdata_adr;    /* ���u���~�σf�[�^�A�h���X   */
		unsigned long  		uni_color;      /* �O���t�\���F               */
		char     name[50];					/* ���u���i�Ď����ږ��j       */
	};

	/*** �O���t�\���p�ݒ�l�e�[�u�� ***/
	struct   grdsp_inf_t {
		int      i_dsp_type;                  /* �����i�āj�^�g�����h�\��   */
		int      i_dsp_mode;                  /* ���A���^�ۑ��f�[�^�\���N�� */
		int      i_dsp_mode2;                 /* �ۑ��f�[�^�����\���^�\��l�I��\�� */
		int      i_dsp_flg;					/* �\���؂�ւ��i�v�^�s�v�j   */
		int      i_unyou_mode;				/* �^�p�󋵃��[�h�i����j     */
		int      i_unyou_mode_s;				/* �^�p�󋵃��[�h�i�O��j     */
		int      i_band;						/* �g�p������g����           */
		int	   i_yohouti_umu;				/* �\��l�t�@�C���L��(V2-1)   */
		int      i_yohouti_get;               /* �\��l�t�@�C���擾��(V2-5) */
		int      i_mondat_NGcnt;              /* ����p�Ď��f�[�^�����J�E���^(98.08.25) */

		/**** ����̏� (V2-2) *******/
		char     ca_eisei[100];               /* �q����					  */
		char     ca_fname[1024];	         /* �\��l�t�@�C���p�X��       */
		char     ca_ftime[30];                /* �\��l�t�@�C���X�V����     */
		char     ca_stime[30];                /* �p�X�J�n����               */
		/**** �O��̏� (V2-2) *******/
		char     ca_eisei_s[100];             /* �q����					  */
		char     ca_fname_s[1024];       /* �\��l�t�@�C���p�X��       */
		char     ca_ftime_s[30];              /* �\��l�t�@�C���X�V����     */
		char     ca_stime_s[30];              /* �p�X�J�n����               */

		/** �ۑ��f�[�^�\���N�����̑Ώ� **/
		char     ca_eisei_def[100];           /* �q����		              */
		char     ca_fname_def[1024];     /* �\��l�t�@�C���p�X��       */

		/**** �^�p���p�X�̕\���Ώ� ****/
		double   d_AOS_time;                  /* �\��l�i�`�n�r�����j       */
		double   d_LOS_time;                  /* �\��l�i�k�n�r�����j       */
		int      i_OFF_timeSig;               /* �\��l�i�����I�t�Z�b�g�����j*/
		double   d_OFF_time;                  /* �\��l�i�����I�t�Z�b�g�l�j */
		double   d_OFF_freq;                  /* �\��l�i���g���I�t�Z�b�g�l�j*/
		double   d_Xstattime;                 /* �����i�J�n�����j           */
		double   d_Xstoptime;                 /* �����i�I�������j           */
		double   d_Xtime;                     /* �����i�\�����F���ԁj       */
		double   d_RNG_Ymin;                  /* �q�m�f�c���i�ŏ��l�j       */
		double   d_RNG_Ymax;                  /* �q�m�f�c���i�ő�l�j       */
		double   d_RNG_Yval;                  /* �q�m�f�c���i�\�����F�l�j   */
		double   d_RR_Ymin;                   /* �q�q�c���i�ŏ��l�j         */
		double   d_RR_Ymax;                   /* �q�q�c���i�ő�l�j         */
		double   d_RR_Yval;                   /* �q�q�c���i�\�����F�l�j     */
		int      i_YRNG;						/* �q�m�f�O���t�i�x���W�j���� */
		int      i_YRR;						/* �q�q�O���t  �i�x���W�j���� */

		//	unsigned long  pixmap_color;			/* �s�b�N�X�}�b�v�w�i�F       */
		//	unsigned long  grph_color;			/* �O���t�̈�w�i�F           */
		//	unsigned long  frem_color;            /* �O���t�g�\���F             */
		//	unsigned long  grid_color;            /* �O���t�r���\���F           */
		//	unsigned long  strtit_color;          /* �^�C�g�������\���F         */
		//	unsigned long  strcon_color;          /* �Œ荀�ڕ����\���F         */
		//	unsigned long  strvar_color;          /* �ύ��ڕ����\���F         */
		//	unsigned long  over_color;			/* �O���t�`��F(�͈̓I�[�o�[) */
		//	unsigned long  yohouti_color;			/* �O���t�`��F(�\��l) 	  */

		//	int	   i_non_pid_num;				/* �ۑ��f�[�^�N�����v���Z�X�� */
		//	int	   i_pid[NON_PID_NUM];			/* �ۑ��f�[�^�\���v���Z�XPID  */

		int      end_id;                      /* �ŐV�f�[�^���u[�ʒu]       */
		struct   uni_inf_t  uni_inf[GRNUM_RNG_MAX]; /* ���u���\���f�[�^��� */
	};

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	// �\��l�e�[�u���\���̃|�C���^
	predata_t	m_gst_predata;
	struct grdsp_inf_t   m_gst_dspinf;    /* �O���t�\��������e�[�u��     */
	long	m_gla_prd_cont[2];				/* �\��l�i�[����             */
	time_t	m_glla_time[2][86400];			/* �\��l����                 */
	double	m_gda_dwRNG[2][86400];			/* �_�E�������N�����W         */
	double	m_gda_dwRR[2][86400];			/* �_�E�������N�����W���[�g   */

	// �\��l
	time_t	m_lla_time[MAX_PRED_SIZE];						/* ����(UNIX�����F�ʎZ�b)		*/
	double	m_da_predict_az[MAX_PRED_SIZE];					/* �\�����ʊp					*/
	double	m_da_predict_el[MAX_PRED_SIZE];					/* �\���p						*/
	double	m_da_predict_uplink_range[MAX_PRED_SIZE];		/* �\���A�b�v�����N�����W		*/
	double	m_da_predict_downlink_range[MAX_PRED_SIZE];		/* �\���_�E�������N�����W		*/
	double	m_da_predict_uplink_range_rate[MAX_PRED_SIZE];	/* �\���A�b�v�����N�����W���[�g	*/
	double	m_da_predict_downlink_range_rate[MAX_PRED_SIZE];	/* �\���_�E�������N�����W���[�g	*/
	double	m_da_predict_2way_rng[MAX_PRED_SIZE];	        /* �\���Q�v�`�x�_�E�������N�����W		*//* <<2013-TURBO>> */
	double	m_da_predict_2way_range_rate[MAX_PRED_SIZE];	/* �\���Q�v�`�x�_�E�������N�����W���[�g	*//* <<2013-TURBO>> */
	double	m_da_predict_s_tx_freq[MAX_PRED_SIZE];			/* �\�����M���g��				*/
	double	m_da_predict_s_rx_1way_freq[MAX_PRED_SIZE];		/* �r�тP�v�`�x�\����M���g��	*/
	double	m_da_predict_x_rx_1way_freq[MAX_PRED_SIZE];		/* �w�тP�v�`�x�\����M���g��	*/
	double	m_da_predict_s_rx_2way_freq[MAX_PRED_SIZE];		/* �r�тQ�v�`�x�\����M���g��	*/
	double	m_da_predict_x_rx_2way_freq[MAX_PRED_SIZE];		/* �w�тQ�v�`�x�\����M���g��	*/
	double	m_da_predict_s_rx_1way_ramprate[MAX_PRED_SIZE];	/* �r�тP�v�`�x�����v���[�g		*/
	double	m_da_predict_x_rx_1way_ramprate[MAX_PRED_SIZE];	/* �w�тP�v�`�x�����v���[�g		*/
	double	m_da_predict_s_rx_2way_ramprate[MAX_PRED_SIZE];	/* �r�тQ�v�`�x�����v���[�g		*/
	double	m_da_predict_x_rx_2way_ramprate[MAX_PRED_SIZE];	/* �w�тQ�v�`�x�����v���[�g		*/

	// �\��l�t�@�C��
	CString		m_yohoFilePath[eStation_MAX];				/* �\��l�t�@�C���p�X */
	FILETIME	m_yohoFileTime[eStation_MAX];				/* �\��l�t�@�C������ */

	// �\��l�A��������
	map<time_t, double>		m_2WayRngMap;					

	map<time_t, double>		m_UpDownLinkRangeMap;
	map<time_t, double>		m_UpDownLinkRateMap;
	long m_PredItemCount;

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	// �\��l�t�@�C�����擾���[�`��
	int gryohouti(int i_get_type, UINT nStation = eStation_MAX);
	int gryohouti2(int i_prot_mode, UINT nStation = eStation_MAX);
	int load_pred_data(char *szp_file_name, double *d_aos, double *d_los, bool* pNewFormat=NULL);
	void asc19_to_bcd7_time(unsigned char* psz_asc19_time, unsigned char* psz_bcd7_time);
	void bcd7_to_asc19_time(unsigned char* psz_bcd7_time, unsigned char* psz_asc19_time);

	double	GetUpDownLinkRange(time_t t, int type);
	map<time_t, double>&	GetUpDownLinkRangeMap() { return m_UpDownLinkRangeMap; }
	double GetLinearInterpolationRange(time_t t1, double dRng1, time_t t2, double dRng2, time_t t);
	double	GetUpDownLinkRate(time_t t, int type);
	map<time_t, double>&	GetUpDownLinkRateMap() { return m_UpDownLinkRateMap; }
	double GetLinearInterpolationRate(time_t t1, double dRng1, time_t t2, double dRng2, time_t t);

//	double lb_rttcal(predata_t* stp_predict, double d_pred_t, int i_cal_flag, double* dp_range, char* psz_offset_time, double d_offset_freq);
//	double lb_rttcal_rr(predata_t* stp_predict, double d_pred_t, int i_cal_flag, double* dp_range_rate, char* psz_offset_time, double d_offset_freq);
//	int lb_prdcal(one_predata_t* st_predict, predata_t* gpst_predict, double d_pred_t, int i_sx, char* szp_time_offset, double d_freq_offset);
//	void lb_ramp0(one_predata_t* st_predict, int i_sx);
//	int lb_prdcal_hit(int i, one_predata_t* st_predict, predata_t* gpst_predict, double d_pred_t, int i_sx, double d_freq_offset);
//	double lb_lagrange(int i_dynamic_counter, int n, double t, time_t i_in_time[], double d_input_parameter[]);
	double InterpolatePrd(double t, __int64* idx, CSpaceData* data);
	double CalcLagrange(int i_dynamic_counter, int n, double t, time_t i_in_time[], double d_input_parameter[]);
	
	void AddLinearInterpolation(time_t t1, double dRng1, time_t t2, double dRng2);

	map<time_t, double>& Get2WayRngMap()	{ return m_2WayRngMap; }
	double GetLinearInterpolation(time_t t1, double dRng1, time_t t2, double dRng2, time_t t);
	double Get2WayRng(time_t t);

	CString GetAnpFileName(UINT nStation)	{ 
		if (nStation < 0 || nStation >= eStation_MAX)
			return _T("");

		CString str = m_yohoFilePath[nStation].Right(m_yohoFilePath[nStation].GetLength() - m_yohoFilePath[nStation].ReverseFind(_T('/')) - 1);
		str = str.Right(str.GetLength() - str.ReverseFind(_T('\\')) - 1);
		return str;
	}
};


// �T�}���ϑ��f�[�^���Z�b�g
typedef struct {
	CString dataName;	// �f�[�^����
	CString dataGName;	// �f�[�^����(�O���t�p��)
} stSummaryDataName;

typedef struct {
	int			wndIdx;						// �E�B���h�[�ԍ�
	TCHAR		wndName[256];				// �E�B���h�[����
	LPARAM		wnd;						// CView
	int			nGraphIdx;					// �O���t�ԍ�

	bool		isRealTime;					// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
	CTime		startTime;					// �J�n����
	stVScale	trandVScale;				// �c���X�P�[��[0:���A1:�E]
	HScale		trandHScale[MAX_HSCALE];	// �����X�P�[��[0:�g�����h�O���t�r���[1�A1:�g�����h�O���t�r���[2]

	TCHAR		dataName[MAX_TDATA][256];	// �f�[�^����
	TCHAR		dataGName[MAX_TDATA][256];	// �f�[�^����(�O���t�p��)
	TCHAR		dataUnit[MAX_TDATA][256];	// �P��
	int			nDecDidits[MAX_TDATA];		// �����_�ȉ��̌���
	bool		isViewMode[MAX_TDATA];		// �\��On/Off
	DrawMode	drawMode[MAX_TDATA];		// �`�惂�[�h�iLine�ADots�AL&D�j
	COLORREF	color[MAX_TDATA];			// �\���F
	int			lineWidth[MAX_TDATA];		// ����
	DotSize		dotSize[MAX_TDATA];			// �h�b�g�T�C�Y
	VScaleKind	vscaleKind[MAX_TDATA];		// �c���X�P�[���I���i�E/���j
	TCHAR		comment[MAX_TDATA][256];	// �R�����g
	bool		isComment[MAX_TDATA];		// �R�����g�\��
	bool		isAlarm[MAX_TDATA];			// �A���[���\��
	double		alarm1Major[MAX_TDATA];		// �A���[��1 ���
	double		alarm1Minor[MAX_TDATA];		// �A���[��1 ���� 
	double		alarm2Major[MAX_TDATA];		// �A���[��2 ���
	double		alarm2Minor[MAX_TDATA];		// �A���[��2 ����
} stTrendPropiesMemData;


// �g�����h�v���p�e�B�f�[�^�N���X
class CPropertiesData
{
public:
	CPropertiesData()	{ m_bSetted = FALSE; };
	virtual ~CPropertiesData(){};

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	int			wndIdx;						// �E�B���h�[�ԍ�
	CString		wndName;					// �E�B���h�[����
	LPARAM		wnd;						// CView
	int			nGraphIdx;					// �O���t�ԍ�

	bool		isRealTime;					// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
	CTime		startTime;					// �J�n����
	stVScale	trandVScale;				// �c���X�P�[��[0:���A1:�E]
	HScale		trandHScale[MAX_HSCALE];	// �����X�P�[��[0:�g�����h�O���t�r���[1�A1:�g�����h�O���t�r���[2]

	CString		dataName[MAX_TDATA];		// �f�[�^����
	CString		dataGName[MAX_TDATA];		// �f�[�^����(�O���t�p��)
	CString		dataUnit[MAX_TDATA];		// �P��
	int			nDecDidits[MAX_TDATA];		// �����_�ȉ��̌���
	bool		isViewMode[MAX_TDATA];		// �\��On/Off
	DrawMode	drawMode[MAX_TDATA];		// �`�惂�[�h�iLine�ADots�AL&D�j
	COLORREF	color[MAX_TDATA];			// �\���F
	int			lineWidth[MAX_TDATA];		// ����
	DotSize		dotSize[MAX_TDATA];			// �h�b�g�T�C�Y
	VScaleKind	vscaleKind[MAX_TDATA];		// �c���X�P�[���I���i�E/���j
	CString		comment[MAX_TDATA];			// �R�����g
	bool		isComment[MAX_TDATA];		// �R�����g�\��
	bool		isAlarm[MAX_TDATA];			// �A���[���\��
	double		alarm1Major[MAX_TDATA];		// �A���[��1 ���
	double		alarm1Minor[MAX_TDATA];		// �A���[��1 ���� 
	double		alarm2Major[MAX_TDATA];		// �A���[��2 ���
	double		alarm2Minor[MAX_TDATA];		// �A���[��2 ����

	BOOL		m_bSetted;					// �ݒ�ς�

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	int getHScal(CString str) {
		for (int i = 0; i < sizeof(strHScale) / sizeof(strHScale[0]); ++i)
		{
			if (str == strHScale[i])
			{
				return i;
			}
		};
		return -1;
	};
};


// �X�y�N�g�����O���t�\���f�[�^�N���X
class CSpectrumViewData
{
public:
	CSpectrumViewData();
	~CSpectrumViewData();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct {
		CString    sz_sts;
		CString    sz_low;
		CString    sz_upp;
		double  d_low;
		double  d_upp;
	} BandWidth_t;

	typedef struct {
		CString		strHeader;					/* �X�y�N�g�����t�@�C�����ʃw�b�_ */
		BOOL		bAutoScale;					/* �I�[�g�X�P�[�� */
		long        gl_y_grid_max;              /* Y���O���b�h�ő�l */
		long        gl_y_grid_min;              /* Y���O���b�h�ŏ��l */
		long        gl_y_grid_val;              /* Y���O���b�h�Ԋu�l */
		double      gd_y_offset_val;            /* Y���I�t�Z�b�g�l */
	} struct_VScale;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	double		m_gda_spectrum_data[SPECTRUM_NUM];		// �X�y�N�g�����f�[�^
	double		m_gd_spectrum_peak;						// �X�y�N�g�����s�[�N�l
	CString		m_gsz_time_tag;							// �f�[�^����
	CString		m_gsz_bandw_sts;						// �o���h��
	CString		m_gsz_moni_point;						// �Ď��|�C���g
	double		m_gd_freq_ofs;							// ���g���I�t�Z�b�g			
	double		m_gd_center_freq;						// ���S���g��

	long        m_gl_moni_point_sel;			/* ���j�^�[�|�C���g�I�� */
	long        m_gl_bw_sel = 0;                /* �ш敝�ݒ�I�� */
	double      m_gd_freq_ofs_in;               /* ���S���g���I�t�Z�b�g���� */
	double		m_gd_peak_level;				/* �s�[�N���x�� */
	double      m_gd_peak_freq;                 /* �s�[�N���g�� */

//	long        m_gl_y_grid_min;                /* Y���O���b�h�ŏ��l */
//	long        m_gl_y_grid_max;                /* Y���O���b�h�ő�l */
//	long        m_gl_y_grid_val;                /* Y���O���b�h�Ԋu�l */

//	double      m_gd_y_offset_val;              /* Y���I�t�Z�b�g�l */
	double      m_gd_y_offset_att;              /* Y��ATT�l */

	int			m_nDummyOffset;					/* �_�~�[�I�t�Z�b�g */
	double		m_dDummyVOffset;				/* �_�~�[�c���I�t�Z�b�g */

//	BOOL		m_bAutoScale;					/* �I�[�g�X�P�[�� */
	struct_VScale	m_stVScale;					/* �c���ݒ� */

//	UINT		m_dummy_moni_point;
//	UINT		m_dummy_bandw;
//	double		m_dummy_freq_ofs;
//	double		m_dummy_center_freq;

	UINT		m_nStation;						/* �ǔԍ� */
	int			m_nRetSpectrumData;				/* �X�y�N�g�����f�[�^�擾���� */

protected:

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	long GetVerticalMax()	{ return m_stVScale.gl_y_grid_max; }
	void SetVerticalMax(long max)	{ m_stVScale.gl_y_grid_max = max; }
	long GetVerticalMin()	{ return m_stVScale.gl_y_grid_min; }
	void SetVerticalMin(long min)	{ m_stVScale.gl_y_grid_min = min; }
	long GetVerticalGrid()	{ return m_stVScale.gl_y_grid_val; }
	void SetVerticalGrid(long grid)	{ m_stVScale.gl_y_grid_val = grid; }
	long GetAutoScale()		{ return m_stVScale.bAutoScale; }
	void SetAutoScale(BOOL autoscale)	{ m_stVScale.bAutoScale = autoscale; }
	struct_VScale& GetVScale()	{ return m_stVScale; }

	void SetStation(UINT nStation)	{ m_nStation = nStation; }

	void InitializeData(UINT nGraphType);
	int FileRead(FILE* fp, UINT nStationID, UINT nViewID, UINT nGraphType);
	int FileWrite(FILE* fp, UINT nStationID, UINT nViewID, UINT nGraphType);
	int MonOffsetGet();
	int GetSpectrumData();
	void CreateDummyData();
	void CreateDummyOffset();

	int GetRetSpectrumData()	{ return m_nRetSpectrumData; }

protected:
};


// �ǔ����[�_�[����Ď��f�[�^�N���X
class CTrackingRadarCtrlMonData
{
public:
	CTrackingRadarCtrlMonData();
	~CTrackingRadarCtrlMonData();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */

public:
	// ON/OFF
	enum {
		eOFF = 0,
		eON,
		eCOMP
	} eONOFF;

	// �^�p��
	enum {
		eUNYOU_OUT = 0,						// �^�p�󋵁i��^�p���
		eUNYOU_START,						// �^�p�󋵁i�^�p�J�n30���ȓ��O�j
		eUNYOU_IN,							// �^�p�󋵁i�^�p���j   
		eUNYOU_TIME = 604800				// �^�p�؂�ւ��J�n���ԁi�V���j
	} eUNYOU;

	enum {
		eDSP_OFF = 0,						// �\���؂�ւ��s�v  
		eDSP_ON								// �\���؂�ւ��v
	} eDSP;

	enum {
		eYOHOUTI_NASI = 0,					// �\��l�t�@�C���Ȃ� 
		eYOHOUTI_ARI						// �\��l�t�@�C���L��
	} eYOHOUTI;

	enum {
		eYOHOGET_MATI = 1,					// �\��l�t�@�C���擾�҂�
		eYOHOGET_SUMI,						// �\��l�t�@�C���擾�ς�
		eYOHOGET_OK							// �\��l�t�@�C���擾�\
	} eYOHOGET;

	// �O���t�\���p�ݒ�l�e�[�u��
	struct gtdsp_inf_t {
		int		i_dsp_type;					// �\����ԁi�����^�g�����h�j
		int		i_dsp_flg;   				// �\���؂�ւ��i�v�^�s�v�j  
		int		i_unyou_mode;				// �^�p�󋵃��[�h�i����j    
		int		i_unyou_mode_s;				// �^�p�󋵃��[�h�i�O��j     
		int		i_yohouti_umu;				// �\��l�t�@�C���L��   
		int		i_yohouti_get;              // �\��l�t�@�C���擾��
		int		i_mondat_NGcnt;             // ����p�Ď��f�[�^�����J�E���^

		// ����̏�
		char	ca_stime[30];	            // �p�X�J�n����          
		char	ca_etime[30];	            // �p�X�I������         
		TCHAR	ca_fpath[1024];             // �\��l�t�@�C���p�X��
		char	ca_ftime[30];				// �\��l�t�@�C���X�V����
		char	ca_plnno[30];	            // �v��ԍ�      
		char	ca_eisei[100];              // �q����
		char	ca_AOS_time[30];            // �`�n�r����
		char	ca_AOS_AZ[30];              // �`�n�r �`�y�l
		char	ca_AOS_EL[30];              // �`�n�r �d�k�l 
		char	ca_LOS_time[30];            // �k�n�r����
		char	ca_LOS_AZ[30];              // �k�n�r �`�y�l
		char	ca_LOS_EL[30];              // �k�n�r �d�k�l
		char	ca_MEL_time[30];            // �l�`�w �d�k����
		char	ca_MEL_AZ[30];              // �l�`�w �d�k �`�y�l
		char	ca_MEL_EL[30];              // �l�`�w �d�k �d�k�l
		char	ca_visible[10];	            // �u�h�r�h�a�k�d

		// �O��̏�
		TCHAR	ca_fpath_s[1024];           // �\��l�t�@�C���p�X��
		char	ca_stime_s[30];	            // �p�X�J�n����
		char	ca_ftime_s[30];				// �\��l�t�@�C���X�V����

		COLORREF	pixmap_color;			// �s�b�N�X�}�b�v�w�i�F
		COLORREF	grph_color;				// �O���t�̈�w�i�F
		COLORREF	frem_color;				// �O���t�g�\���F
		COLORREF	frem_color2;			// �O���t�g�\���F2
		COLORREF	e80_frem_color;			// EL80�O���t�g�\���F
		COLORREF	grid_color;				// �O���t�r���\���F
		COLORREF	grid_color2;			// �O���t�r���\���F2
		COLORREF	strtit_color;			// �^�C�g�������\���F
		COLORREF	strcon_color;			// �Œ荀�ڕ����\���F
		COLORREF	strvar_color;			// �ύ��ڕ����\���F
		COLORREF	aos_yoho_color;			// �O���t�`��F(�\��laos)
		COLORREF	los_yoho_color;			// �O���t�`��F(�\��llos)
		COLORREF	yoho_color;				// �O���t�`��F(�\��l�ʏ�)
		COLORREF	trnd_color;	  			// �O���t�`��F(�����l)
		COLORREF	skyl_color;    			// �O���t�`��F(�X�J�C���C��)
		COLORREF	rftr_color;	   			// �O���t�`��F(�q�e�g���b�v)
		COLORREF	over_color;				// �O���t�`��F(�͈̓I�[�o�[)

//		double		d_end_time;				// �ŏI�i�[�f�[�^��������
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	long		m_l_inflg;				/* �Ď��f�[�^�L�����             */

	gtdsp_inf_t	m_gst_dspinf;		// �O���t�\���ݒ�e�[�u��
	long		m_gpla_shmadr[GRNUM_PRS_MAX + 1]; // ���L�������i�[�A�h���X�i�[�̈�
	int			m_i_into_flag;		// �p�X�J�n�R�O���ȏ�O����R�O���ȓ��ɓ��������ǂ����̃t���O
	long long	m_l_latest_time;	// �O��̃J�E���g�_�E���l
	int			m_gi_cmp1;
	int			m_gi_cmp2;

	deque<Az_El>	m_queAzElSkyline;			// �X�J�C���C���f�[�^�L���[
	deque<Forecast_Az_El>	m_queAzElForecast;	// �\��l�f�[�^�L���[
	deque<Az_El> m_queAzElActual;				// �����l�f�[�^�L���[

	int			m_i_unyou;				/* �p�X�^�p�� 				  */
//	char        m_ca_wtime[30];			/* �p�X�^�p�J�n�܂ł̎��� 		  */
//	long long	m_d_time;				/* �p�X�^�p�J�n�܂ł̎��� 		  */
//	char		m_ca_stime[30];			/* �p�X�J�n����                   */
//	char		m_ca_etime[30];			/* �p�X�I������                   */
//	TCHAR		m_ca_fpath[1024];		/* �\��l�t�@�C�����i�t���p�X�j   */
//	char		m_ca_ftime[30];			/* �\��l�t�@�C���X�V����		  */
//	char		m_ca_plnno[30];			/* �v��ԍ�                       */
//	char		m_ca_eisei[100];		/* �q����       			      */
//	char		m_ca_AOS_time[20];		/* �`�n�r����                     */
//	char		m_ca_AOS_AZ[9];			/* �`�n�r�� �`�y�l                */
//	char		m_ca_AOS_EL[8];			/* �`�n�r�� �d�k�l                */
//	char		m_ca_LOS_time[20];		/* �k�n�r����                     */
//	char		m_ca_LOS_AZ[9];			/* �k�n�r�� �`�y�l                */
//	char		m_ca_LOS_EL[8];			/* �k�n�r�� �d�k�l                */
//	char		m_ca_MEL_time[20];		/* �l�`�w�|�d�k����               */
//	char		m_ca_MEL_AZ[9];			/* �l�`�w�|�d�k�� �`�y�l          */
//	char		m_ca_MEL_EL[8];			/* �l�`�w�|�d�k�� �d�k�l          */
//	char		m_ca_VISIBLE[9];		/* �u�h�r�h�a�k�d����             */
	char        m_ca_yohouget[30];		/* �\��l�t�@�C���擾��         */

	int			m_nViewID;				/* �r���[ID */
	int			m_nStationID;			/* ��ID */

	CString		m_yohoFilePath;			/* �\��l�t�@�C���p�X */
	FILETIME	m_yohoFileTime;			/* �\��l�t�@�C������ */

	bool		m_bPassEnd;				/* �p�X�I�� */

protected:

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
//	void SetPredFilePath(TCHAR* fpath)  { wcscpy_s(m_ca_fpath, 1024, fpath); }
	void SetViewID(int nID)		{ m_nViewID = nID; }
	void SetStationID(int nID)	{ m_nStationID = nID; }
	void UpdateData();
//	int Gtunyou();
	int GetKanshiData();
//	int GetFileData();
	void Gtinit();
	void Gtgetskyl();
	void Gtgetrftr();
	int Gtgetyoho();
	void ClearQeue();

	void GetAzEl(char* pc_rec, Az_El* az_el);
	void GetForecastAzEl(int i_type, char* pc_rec, Forecast_Az_El* az_el, int i_roop_cnt);

protected:

};


// �Z���f�f��ʃf�[�^�Ǘ��N���X

#define SS_TCR_LBW	_T("SS-TCRDEM.CARR_LOOP")
#define XX_TCR_LBW	_T("DSNTCR.CARR_LOOP_BAND")
#define X_RX2_LBW	_T("X-RX2.PLL_BW")
#define S_TRKRX_LBW	_T("S-TRKRX.CARR_LBW")
#define S_ACQRX_LBW	_T("S-ACQ_TRKRX.CARR_LOOP_BAND")
#define X_TRKRX_LBW	_T("X-TRKRX.CARR_LOOP_BAND")
#define X_ACQRX		_T("X-ACQ_TRKRX.CARR_LOOP_BAND")

class CRdnsChkData
{
public:
	CRdnsChkData();
	~CRdnsChkData();

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

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	double BitrateStrToDouble(CString str);

protected:

};


// �v�旧�ĉ�ʃf�[�^�Ǘ��N���X

typedef struct  {
	CTime		start;		// �J�n����
	CTime		end;		// �I������
} stTime;	// �J�n�I�������\����

typedef struct  {
	CString		start;		// �J�n����
	CString		end;		// �I������
} stTimeStr;	// �J�n�I�������\����

enum {
	STRON_OFF,
	STRON_ON,
};
static const CString strON[] = {
	_T("����"),
	_T("�L��"),
	_T(""),
};

enum {
	ARINASI_ARI,
	ARINASI_NASI,
};
static const CString strAriNasi[] = {
	_T("����"),
	_T("�Ȃ�"),
	_T(""),
};

// ANT�V���w���L��
enum {
	ANTZENITH_ARI_SWTOW,
	ANTZENITH_ARI,
	ANTZENITH_NASI,
};
static const CString strANTZenith[] = {
	_T("����(STOW����)"),
	_T("����(STOW�Ȃ�)"),
	_T("�Ȃ�"),
	_T(""),
};

// ���M�@�h�b�v���⏞
enum {
	TXDOPPLER_ARI,
	TXDOPPLER_NASI_AVE,
	TXDOPPLER_NASI_STT,
	TXDOPPLER_NASI_NOM,
};
static const CString strTxDoppler[] = {
	_T("�L��"),
	_T("�����i�Z���^�[�j"),
	_T("�����i�x�X�g���b�N�j"),
	_T("�����i�m�~�i���j"),
	_T(""),
};

static const CString strTxDoppler2[] = {
	_T("���I�⏞"),
	_T("�p�X�Z���^�["),
	_T("AOS�x�X�g���b�N"),
	_T("�m�~�i��"),
	_T(""),
};

// ��M�M�@�h�b�v���⏞
enum {
	RXDOPPLER_SX,
	RXDOPPLER_S,
	RXDOPPLER_X,
	RXDOPPLER_NASI,
};
static const CString strRxDoppler[] = {
	_T("S/X"),
	_T("S"),
	_T("X"),
	_T("����"),
	_T(""),
};

static const CString strRxDoppler54[] = {
	_T("�L��"),
	_T("����"),
	_T(""),
	_T(""),
	_T(""),
};


// ���M�o��
enum {
	TRSPWR_HPA_10KW,
	TRSPWR_SSPA_1KW,
	TRSPWR_SSPA_100W,
	TRSPWR_SSPA_25W
};
static const CString strTrsPwr20[] = {
	_T("HPA_10KW"),
	_T("SSPA_1KW"),
	_T("SSPA_100W"),
	_T("SSPA_25W"),
	_T(""),
};

enum {
	TRSPWRX_X,
	TRSPWRX_S,
};

enum {
	TRSPWRX_HPA20kw,
	TRSPWRX_HPA2kw,
	TRSPWRX_HPA200w,
	TRSPWRX_SSPA100w,
	TRSPWRX_SSPA10w,
};
static const CString strTrsPwrX34[] = {
	_T("HPA-20kw"),
	_T("HPA-2kw"),
	_T("HPA-200w"),
	_T("SSPA-100w"),
	_T("SSPA-10w"),
	_T(""),
};

enum {
	TRSPWRS_HPA10kw,
	TRSPWRS_SSPA1kw,
	TRSPWRS_SSPA100w,
	TRSPWRS_SSPA25w,
	TRSPWRS_SSPA75w,
};
static const CString strTrsPwrS34[] = {
	_T("HPA-10kw"),
	_T("SSPA-1kw"),
	_T("SSPA-100w"),
	_T("SSPA-25w"),
	_T("SSPA-75w"),
	_T(""),
};

enum {
	TRSPWR_20kw,
	TRSPWR_2kw,
	TRSPWR_200w
};
static const CString strTrsPwr64[] = {
	_T("20kw"),
	_T("2kw"),
	_T("200w"),
	_T(""),
};

// strTrsPwr54[]�́A�֐�(COprPlnData::GetOpTOutIdx)�����g�p�Ȃ̂Œǉ��s�v

// eqdb_lib.h >>>
#define	SATNAME_LEN		16	/* �q�����̍ő咷+1 */
#define	PRDPATH_LEN		128	/* �\��l�t�@�C���i�[�f�B���N�g�����̍ő咷+1 */
#define	PLANPATH_LEN	128	/* �v��t�@�C���i�[�f�B���N�g�����̍ő咷+1 */
#define	SATTXSTR_LEN	24	/* �q�����M�@���ʍő咷+1 */
#define	SATANTSTR_LEN	24	/* �q���A���e�i���ʖ��̍ő咷+1 */
// <<< eqdb_lib.h

// opr_def.h >>>
#define TBLMAX			40	/*�ꎞ�I�Ƀe�[�u���ő吔���`���Ă����B��ō폜���邱��*/
// <<< opr_def.h

// opr_plan.h >>>
struct txpwr_inf_t{
	char *sz_txpwr_val;								/* ���M�p���[ */
	char *satant_sval[20];							/* �A���e�i */
	int i_s_snd_level;								/* S-BAND���M���x�����ڐ� */
	int i_x_snd_level;								/* X-BAND���M���x�����ڐ� */
};

struct sat_inf_t{									/* �q���{�̏��T�� */
	char sz_satnames[SATNAME_LEN];					/* �q���� */
	long l_sat_no;									/* �q���ԍ� */
	long l_use_band;								/* �g�p���g���� */
	long l_level;									/* �g�p�ш� */
	double d_s_coh;									/* s�[band�R�q�[�����g���V�I */
	double d_x_coh;									/* x�[band�R�q�[�����g���V�I */
	double nominal_tx[256];							/* �m�~�i�����M���g�� */
	double nominal_rx[256];							/* �m�~�i����M���g�� */
	char *cmd_tbl[TBLMAX];							/* s/x�[band�R�}���h�r�b�g���[�g�e�[�u�� */
	char *x_cmd_tbl[TBLMAX];						/* x�[band�R�}���h�r�b�g���[�g�e�[�u�� */
	int rxbands[TBLMAX];							/* �m�~�i����M���g���o���h�� */
	/*	char *lev_s_tbl[TBLMAX];       */			/* s�[band�q���ʐM���x�� */
	/*	char *lev_x_tbl[TBLMAX];       */			/* x�[band�q���ʐM���x�� */
	char *br_s_tbl[TBLMAX];							/* s�[band�e�����r�b�g���[�g */
	char *br_x_tbl[TBLMAX];							/* x�[band�e�����r�b�g���[�g */
	char *br_qpsk_tbl[TBLMAX];						/* x�[band�e�����r�b�g���[�g */
	char sz_pred_dir[PRDPATH_LEN];					/* �\��l�t�@�C���f�B���N�g�� */
	char sz_plan_dir[PLANPATH_LEN];					/* �v��t�@�C���f�B���N�g�� */
	char sz_sattx_val[SATTXSTR_LEN];				/* */
	struct txpwr_inf_t s_txpwr_sval[20];			/* */
	struct txpwr_inf_t x_txpwr_sval[20];			/* */
	int i_cmd_bitrate;								/* S/X-BAND�R�}���h�r�b�g���[�g���ڐ� */
	int i_x_cmd_bitrate;							/* X-BAND�R�}���h�r�b�g���[�g���ڐ� */
	int i_bitrate_s;								/* S-BAND�e�����r�b�g���[�g���ڐ� */
	int i_bitrate_x;								/* X-BAND�e�����r�b�g���[�g���ڐ� */
	int i_bitrate_x2;								/* X-BAND�e�����r�b�g���[�g���ڐ� */
	int i_bitrate_ka;								/* Ka-BAND�e�����r�b�g���[�g���ڐ� */
	int i_bitrate_qpsk;								/* X-BAND�e�����r�b�g���[�g���ڐ� */
	int i_s_power;									/* S-BAND���M�p���[���ڐ� */
	int i_x_power;									/* X-BAND���M�p���[���ڐ� */
	int	i_x_tx_sel;									/* �w�ё��M�ݔ��L�� */
	int	i_x_rx_sel;									/* �w�ю�M�ݔ����� */
	int	i_s_tx_ramp;								/* �r�ё��M�ݔ������v����L�� */
	int	i_x_tx_ramp;								/* �w�ё��M�ݔ������v����L�� */
	int	i_s_rx_ramp;								/* �r�ю�M�ݔ������v����L�� */
	int	i_x_rx_ramp;								/* �w�ю�M�ݔ������v����L���i��������p�j */
	int	i_x_rx1_ramp;								/* �w�ю�M�ݔ��P�����v����L���i�P�c��p�j */
	int	i_x_rx2_ramp;								/* �w�ю�M�ݔ��Q�����v����L���i�P�c��p�j */
	int	i_sx_acqrx_ramp;							/* �r�w�ѕߑ���M�ݔ������v����L���i��������p�j */
	int	i_x_qpskrx_ramp;							/* �w�э�����M�ݔ������v����L�� */
	int	i_acq_route;								/* �⑫���[�g */
	int	i_pl_sel;									/* ACQ/MAIN����L�� */
	int	i_top_mon;									/* �V���ʉߊĎ��L�� */
	int	i_ent_angle;								/* �V���ʉߊĎ����p */
	int	i_ext_angle;								/* �V���ʉߊĎ��o�p */
	int	i_s_rx;										/* �`�f�b�Z���Ώێ�M�@���i�r�ю��M�ݔ��j */
	int	i_x_rx;										/* �`�f�b�Z���Ώێ�M�@���i�w�ю��M�ݔ��j��������p */
	int	i_x_rx1;									/* �`�f�b�Z���Ώێ�M�@���i�w�ю��M�ݔ��P�j�P�c��p */
	int	i_x_rx2;									/* �`�f�b�Z���Ώێ�M�@���i�w�ю��M�ݔ��Q�j�P�c��p */
	int	i_x_dsntcr1;								/* �`�f�b�Z���Ώێ�M�@���iX��DSN-MODEM1�jGREAT��p */
	int	i_ka_dsntcr1;								/* �`�f�b�Z���Ώێ�M�@���iKa��DSN-MODEM1�jGREAT��p */
	int	i_sx_acqrx_s;								/* �`�f�b�Z���Ώێ�M�@���i�r�w�ѕ⑫�p��M�ݔ��@�r�сj��������p */
	int	i_sx_acqrx_x;								/* �`�f�b�Z���Ώێ�M�@���i�r�w�ѕ⑫�p��M�ݔ��@�w�сj��������p */
	int	i_x_qpskrx;									/* �`�f�b�Z���Ώێ�M�@���i�w�э����f�[�^�p��M�ݔ��j */
	int i_rarr_band;								/* �q�`�q�q�g�p�ш��� */
	int	i_ss_tcr;									/* �`�f�b�Z���Ώێ�M�@���i�r�r�|�s�b�q�c�d�l�j */
	int i_rxfq_ka;									/* �m�~�i����M���g����� Ka �L�� */
	int i_rxfq_x;									/* �m�~�i����M���g����� X �L�� */
	int i_rxfq_s;									/* �m�~�i����M���g����� S �L�� */
};
// <<< opr_plan.h

// eqsat_db.h >>>
struct sattx_t {									/* �q�����M�@����`�e�[�u��: �\�[�X��"sattx"�ɑ��� */
	long l_id;										/* �f�[�^����: �萔=SATTX_ID�l */
	long l_len;										/* �f�[�^��  : sizeof( struct sattx_t ) */
	char sz_sattx[SATTXSTR_LEN];					/* ���M�@���� */
};
struct satant_t {									/* �q���A���e�i����`�e�[�u��: �\�[�X��"satant"�ɑ��� */
	long l_id;										/* �f�[�^����: �萔=SATANT_ID�l */
	long l_len;										/* �f�[�^��  : sizeof( struct satant_t ) */
	char sz_satant[SATANTSTR_LEN];					/* �q���A���e�i���� */
	long l_valnum;									/* �ݒ�l���i���e�P�j */
};
// <<< eqsat_db.h


#define RDY_AGC_OLD				_T("CTRL.RDYCHK_AGC_OLD")		/* RDYCHK AGC */
#define RDY_RNG_OLD				_T("CTRL.RDYCHK_RNG_OLD")		/* RDYCHK RNG */
#define INIT					_T("INIT")						/* INIT */
#define BIT_S					_T("BITRATE-S")					/* TLM(S) */
#define BIT_X					_T("BITRATE-X")					/* TLM(X) */
#define BIT_X2					_T("BITRATE-X2")				/* TLM(X)2 */
#define BIT_KA					_T("BITRATE-KA")				/* TLM(Ka) */
#define C_BIT					_T("CMD-BITRATE")				/* COMMAND BITRATE(S) */
#define C_BIT2					_T("CMD2-BITRATE")				/* COMMAND BITRATE */
#define C_BIT3					_T("CMD3-BITRATE")
#define C_BIT4					_T("CMD4-BITRATE")				/* COMMAND BITRATE */
#define X_CMD2_BITRATE_TXT		_T("CMD4-BITRATE")

#define X_CMD2_BITRATE			_T("XCMD-BITRATE2")
#define S_ACQ_BITRATE			_T("BITRATE-S_ACQ")
#define X_ACQ_BITRATE			_T("BITRATE-X_ACQ")
#define S_TRKRX_BITRATE			_T("BITRATE-S_TRK")
#define S_ACQT_BITRATE			_T("S-ACQTCRDEM.BIT_RATE")
#define X_HRX_DATARATE			_T("X-HRX.DEMO_DATA_RATE_CONT")
#define SS_BITRATE				_T("SS-TCRDET.BIT_RATE")


#define C_MOD					_T("CMD_MOD_ON")				/* CMD_MOD */
#define R_MOD					_T("RNG_MOD_ON")				/* RNG_MOD */
#define R_BAND					_T("RNG_BAND")					/* RNG_BAND */
#define R_START					_T("R-START")					/* RNG_START */
#define R_END					_T("R-END")						/* RNG_END */
#define T_OFF					_T("OFFSET")					/* OFFSET */
#define P_START					_T("P-START")					/* P-START */
#define P_END					_T("P-END")						/* P-END */
#define A_FLAG					_T("AOS_FLAG")					/* AOS_FLAG */
#define L_FLAG					_T("LOS_FLAG")					/* LOS_FLAG */
#define AUTO_ACQ				_T("AUTO_ACQ")					/* AUTO_ACQ */
#define BIT_XQPSK				_T("BITRATE-XQPSK")				/* XQPSK(X)*/
#define BIT_XQPSK20				_T("BITRATE-XHRX")				/* XQPSK(X)*/
#define BIT_XHRX				_T("BITRATE-XHRX")				/* XHRX */

#define BIT_TRKRX				_T("BITRATE-X")

#define T_ANT					_T("TERM_ANT_MODE")				/* T_ANT_MODE */
#define X_TX_RAMP				_T("TX_RAMP")					/* X_TX_RAMP_MODE */
#define RX_RAMP					_T("RX_RAMP")					/* RX_RAMP_MODE */
#define ACQ_ROUTE				_T("ACQ_ROUTE")					/* ACQ_ROUTE */
#define FALSE_LOCK				_T("FALSE_LOCK")				/* FALSE_LOCK_MODE */
#define POL_CHK					_T("POL_CHK")					/* POL_CHK_MODE */
#define POL_ENT					_T("POL_ENT")					/* POL_ENT */
#define POL_EXIT				_T("POL_EXIT")					/* POL_EXIT */
#define	MODECHK					_T("CTRL.MODECHK")				/* MODECHK */
#define	PREDGET					_T("PREDGET")					/* PREDGET */
#define LEV_S					_T("LEVEL-S")					/* COMINICATION LEVEL(S)*/
#define LEV_X					_T("LEVEL-X")					/* COMINICATION LEVEL(X)*/
#define DELAY_SEL_TXT			_T("RNG_TYPE")					/* �Đ������Ή� */
#define DELAY_SEL_CONT_TXT		_T("XX-RRNG.DIST_RNG_VIDEO")	/* �Đ������Ή� */
#define S_TX_PWR_SET_TXT		_T("CTRL.S_TX_RF_PWR")			/* �w�тg�o�`���ݑΉ� */
#define X_TX_PWR_SET_TXT		_T("CTRL.X_TX_RF_PWR")			/* �w�тg�o�`���ݑΉ� */
#if 1
#define	X_MOD_SEL_TXT			_T("CTRL.DSN_TCRMOD_SEL")		/* �w�тg�o�`���ݑΉ� */
#else
#define	X_MOD_SEL_TXT			_T("CTRL.X_MOD_SEL")			/* �w�тg�o�`���ݑΉ� */
#endif
#define C_BIT_X					_T("XCMD-BITRATE")				/* COMMAND BITRATE(X) */
#define C_BIT_X2				_T("XCMD-BITRATE2")				/* COMMAND BITRATE(X) */
#define RDY_AGC					_T("CTRL.RDYCHK_AGC")			/* RDYCHK AGC */
#define RDY_RNG					_T("CTRL.RDYCHK_RNG")			/* RDYCHK RNG */
#define RDY_TLM					_T("CTRL.RDYCHK_TLM")			/* RDYCHK TLM */
#define RDY_CMD					_T("CTRL.RDYCHK_CMD")			/* RDYCHK CMD */
#define SYS_ANT					_T("SYSTEM.ant_waitpoint&")		/* REM ANT */
#define	CTRL_IINFO				_T("CTRL.INITINFO")				/* CTRL.INITINFO */
#define	RANGE_BAND				_T("RANGE_BAND")				/* S��RNG�v���ш� */
#define	ANT_STOW				_T("ANT_STOW")					/* ANT�V���w���L�� */
#define	S_TLMIN_CTRL_TXT		_T("STLMIN.TLM_START")			/* S��BBE�����Ή� */
#define	CTRL_S_RNG_ROUTE		_T("CTRL.S_RNG_ROUTE")			/* S��BBE�����Ή� */
#define	DELAY_SEL_CONT_DSN_TXT	_T("DSNTCR.RNG_MODE")			
#define DSN_TX_AID_ON_TXT		_T("DSNTCR.TX_CARR_DOPP_PRE")	
#define DSN_RX_AID_ON_TXT		_T("DSNTCR.RX_CARR_DOPP_PRE")	
#define S_TX_INIT_TXT			_T("INIT.S-TX")
#define X_TX_INIT_TXT			_T("INIT.X-TX")
#define INIT_CID				_T("INIT.CID")
#define X_ROUTE_DEF_TXT			_T("INIT.DSNTCR-DEM")
#define	X_TXSYN_TXT				_T("X-TXSYN.")
#define DSN_PREDGET_TXT			_T("XX-DSNTCRFC.PREDGET")			
#define	SS_TCRMOD_TXT			_T("SS-TCRMOD.")
#define	S_DCSYN_TXT				_T("SS-DCSYN1.")
#define	S_RXSYN_TXT				_T("SS-TCRDEM.")
#define	X_RXSYN_TXT				_T("X-RXSYN.")
#define	X_RX1SYN_TXT			_T("SX-DPL.")
#define	X_RX2SYN_TXT			_T("X-RX2SYN.")
#define	X_QPSKSYN_TXT			_T("X-QPSKRXSYN.")
#define S_TX_PWR_SET_64_TXT		_T("S-TXRF.IF_RF_PWR")
//#define X_TX_PWR_SET_64_TXT		_T("X-TX.HPA_RF_PWR")
#define X_TX_PWR_SET_64_TXT		_T("X-UL_CNT.RF_PWR_200/2k/20kW")
#define X_TX_PWR_SET_54_TXT		_T("X-SSPA.TX_POWER_MODE")
#define CID_TXT					_T("CID")
#define ST_NAME					_T("ST_NAME")
#define PRI						_T("PRI")
#define OPE_MODE				_T("OPE_MODE")

#define DOWN_START				_T("CTRL.DOWN_START")
#define DOWN_END				_T("CTRL.DOWN_END")
#define UP_START				_T("CTRL.UP_START")
#define UP_END					_T("CTRL.UP_END")
#define RNG_START				_T("CTRL.RNG_START")
#define RNG_END					_T("CTRL.RNG_END")
#define RNG_SS					_T("SS")
#define RNG_SX					_T("SX")
#define RNG_XX					_T("XX")
#define RNG_XKa					_T("XKa")
#define CMD_START				_T("CTRL.CMD_START")
#define CMD_END					_T("CTRL.CMD_END")
#define CMD_S					_T("S")
#define CMD_X					_T("X")
#define TLM_START				_T("CTRL.TLM_START")
#define TLM_END					_T("CTRL.TLM_END")
#define TLM_S					_T("S")
#define TLM_X					_T("X")
#define TLM_KA					_T("Ka")
#define AutoGenerated			_T("AUTO")

#define BIT_S_TRK				_T("BITRATE-S_TRK")
#define BIT_S_ACQ				_T("BITRATE-S_ACQ")
#define BIT_X_ACQ				_T("BITRATE-X_ACQ")
#define BIT_Ka					_T("BITRATE-Ka")	/* eqsat.db(�}�N��)�ACID */

#define PROP_READ				_T("CTRL.PROP_READ")


//char *sz_delay_sel_prm[] = { _T("REPLAY"), _T("NORMAL"), NULL };
//char *sz_delay_sel_dsn_prm[] = { _T("REGEN"), _T("TURNAROUND"), NULL };

#define SAT_DATA     15
#define START_OFFSET 120.0

static const CString sz_c_ele20[] = {
	RDY_AGC_OLD,				// 0
	RDY_RNG_OLD,				// 1
	INIT,						// 2
	BIT_S,						// 3
	BIT_X,						// 4
	C_BIT,						// 5
	C_MOD,						// 6
	R_MOD,						// 7
	R_BAND,						// 8
	R_START,					// 9
	R_END,						// 10
	T_OFF,						// 11
	P_START,					// 12
	P_END,						// 13
	A_FLAG,						// 14
	L_FLAG,						// 15
	AUTO_ACQ,					// 16
	BIT_XQPSK,					// 17
	T_ANT,						// 18
	X_TX_RAMP,					// 19
	RX_RAMP,					// 20
	ACQ_ROUTE,					// 21
	FALSE_LOCK,					// 22
	POL_CHK,					// 23
	POL_ENT,					// 24
	POL_EXIT,					// 25
	MODECHK,					// 26
	PREDGET,					// 27
	LEV_S,						// 28
	LEV_X,						// 29
	DELAY_SEL_TXT,				// 30
	DELAY_SEL_CONT_TXT,			// 31
	S_TX_PWR_SET_TXT,			// 32
	X_TX_PWR_SET_TXT,			// 33
	X_MOD_SEL_TXT,				// 34
	_T("*"),					// 35
	RDY_AGC,					// 36
	RDY_RNG,					// 37
	RDY_TLM,					// 38
	RDY_CMD,					// 39
	SYS_ANT,					// 40
	CTRL_IINFO,					// 41
	_T("*"),					// 42
	ANT_STOW,					// 43
	S_TLMIN_CTRL_TXT,			// 44
	CTRL_S_RNG_ROUTE,			// 45
	DELAY_SEL_CONT_DSN_TXT,		// 46
	DSN_TX_AID_ON_TXT,			// 47
	DSN_RX_AID_ON_TXT,			// 48
	CID_TXT,					// 49
	ST_NAME,					// 50
	PRI,						// 51
	OPE_MODE,					// 52
	DOWN_START,					// 53
	DOWN_END,					// 54
	UP_START,					// 55
	UP_END,						// 56
	RNG_START,					// 57
	RNG_END,					// 58
	CMD_START,					// 59
	CMD_END,					// 60
	TLM_START,					// 61
	TLM_END,					// 62
	AutoGenerated,				// 63
	INIT_CID,					// 64
	PROP_READ,					// 65
	_T(""),
};

static const CString sz_c_ele34[] = {
	RDY_AGC_OLD,				// 0
	RDY_RNG_OLD,				// 1
	INIT,						// 2
	BIT_S,						// 3
	BIT_X,						// 4
	C_BIT,						// 5
	C_MOD,						// 6
	R_MOD,						// 7
	R_BAND,						// 8
	R_START,					// 9
	R_END,						// 10
	T_OFF,						// 11
	P_START,					// 12
	P_END,						// 13
	A_FLAG,						// 14
	L_FLAG,						// 15
	AUTO_ACQ,					// 16
	BIT_XHRX,					// 17
	T_ANT,						// 18
	X_TX_RAMP,					// 19
	RX_RAMP,					// 20
	ACQ_ROUTE,					// 21
	FALSE_LOCK,					// 22
	POL_CHK,					// 23
	POL_ENT,					// 24
	POL_EXIT,					// 25
	MODECHK,					// 26
	PREDGET,					// 27
	LEV_S,						// 28
	LEV_X,						// 29
	DELAY_SEL_TXT,				// 30
	DELAY_SEL_CONT_TXT,			// 31
	S_TX_PWR_SET_TXT,			// 32
	X_TX_PWR_SET_TXT,			// 33
	X_MOD_SEL_TXT,				// 34
	C_BIT_X2,					// 35
	RDY_AGC,					// 36
	RDY_RNG,					// 37
	RDY_TLM,					// 38
	RDY_CMD,					// 39
	SYS_ANT,					// 40
	CTRL_IINFO,					// 41
	RANGE_BAND,					// 42
	ANT_STOW,					// 43
	S_TLMIN_CTRL_TXT,			// 44
	CTRL_S_RNG_ROUTE,			// 45
	DELAY_SEL_CONT_DSN_TXT,		// 46
	DSN_TX_AID_ON_TXT,			// 47
	DSN_RX_AID_ON_TXT,			// 48
	S_TX_INIT_TXT,				// 49
	X_TX_INIT_TXT,				// 50
	CID_TXT,					// 51
	ST_NAME,					// 52
	PRI,						// 53
	OPE_MODE,					// 54
	DOWN_START,					// 55
	DOWN_END,					// 56
	UP_START,					// 57
	UP_END,						// 58
	RNG_START,					// 59
	RNG_END,					// 60
	CMD_START,					// 61
	CMD_END,					// 62
	TLM_START,					// 63
	TLM_END,					// 64
	AutoGenerated,				// 65
	INIT_CID,					// 66
	BIT_S_TRK,					// 67
	BIT_S_ACQ,					// 68
	BIT_X2,						// 69
	BIT_X_ACQ,					// 70
	PROP_READ,					// 71
	_T(""),
};

static const CString sz_c_ele64[] = {
	RDY_AGC_OLD,				// 0
	RDY_RNG_OLD,				// 1
	INIT,						// 2
	BIT_S,						// 3
	BIT_X,						// 4
	C_BIT,						// 5
	C_MOD,						// 6
	R_MOD,						// 7
	R_BAND,						// 8
	R_START,					// 9
	R_END,						// 10
	T_OFF,						// 11
	P_START,					// 12
	P_END,						// 13
	A_FLAG,						// 14
	L_FLAG,						// 15
	AUTO_ACQ,					// 16
	BIT_XQPSK,					// 17
	T_ANT,						// 18
	X_TX_RAMP,					// 19
	RX_RAMP,					// 20
	ACQ_ROUTE,					// 21
	FALSE_LOCK,					// 22
	POL_CHK,					// 23
	POL_ENT,					// 24
	POL_EXIT,					// 25
	MODECHK,					// 26
	PREDGET,					// 27
	LEV_S,						// 28
	LEV_X,						// 29
	CTRL_IINFO,					// 30
	S_TX_PWR_SET_64_TXT,		// 31
	X_TX_PWR_SET_64_TXT,		// 32
	DELAY_SEL_TXT,				// 33
	DELAY_SEL_CONT_TXT,			// 34
	X_MOD_SEL_TXT,				// 35
	C_BIT3,						// 36
	RDY_AGC,					// 37
	RDY_RNG,					// 38
	RDY_TLM,					// 39
	RDY_CMD,					// 40
	SYS_ANT,					// 41
	DELAY_SEL_CONT_DSN_TXT,		// 42
	X_CMD2_BITRATE_TXT,			// 43
	DSN_TX_AID_ON_TXT,			// 44
	DSN_RX_AID_ON_TXT,			// 45
	ANT_STOW,					// 46
	CID_TXT,					// 47
	ST_NAME,					// 48
	PRI,						// 49
	OPE_MODE,					// 50
	DOWN_START,					// 51
	DOWN_END,					// 52
	UP_START,					// 53
	UP_END,						// 54
	RNG_START,					// 55
	RNG_END,					// 56
	CMD_START,					// 57
	CMD_END,					// 58
	TLM_START,					// 59
	TLM_END,					// 60
	AutoGenerated,				// 61
	INIT_CID,					// 62
	PROP_READ,					// 63
	_T(""),
};

static const CString sz_c_ele54[] = {
	RDY_AGC_OLD,				// 0
	RDY_RNG_OLD,				// 1
	INIT,						// 2
	BIT_KA,						// 3	BITRATE-KA
	BIT_X,						// 4
	C_BIT,						// 5
	C_MOD,						// 6
	R_MOD,						// 7
	R_BAND,						// 8
	R_START,					// 9
	R_END,						// 10
	T_OFF,						// 11
	P_START,					// 12
	P_END,						// 13
	A_FLAG,						// 14
	L_FLAG,						// 15
	AUTO_ACQ,					// 16
	BIT_XQPSK,					// 17
	T_ANT,						// 18
	X_TX_RAMP,					// 19
	RX_RAMP,					// 20
	ACQ_ROUTE,					// 21
	FALSE_LOCK,					// 22
	POL_CHK,					// 23
	POL_ENT,					// 24
	POL_EXIT,					// 25
	MODECHK,					// 26
	PREDGET,					// 27
	LEV_S,						// 28
	LEV_X,						// 29
	CTRL_IINFO,					// 30
	S_TX_PWR_SET_64_TXT,		// 31
	X_TX_PWR_SET_54_TXT,		// 32	SSPA
	DELAY_SEL_TXT,				// 33
	DELAY_SEL_CONT_TXT,			// 34
	X_MOD_SEL_TXT,				// 35
	C_BIT3,						// 36
	RDY_AGC,					// 37
	RDY_RNG,					// 38
	RDY_TLM,					// 39
	RDY_CMD,					// 40
	SYS_ANT,					// 41
	DELAY_SEL_CONT_DSN_TXT,		// 42
	X_CMD2_BITRATE_TXT,			// 43
	DSN_TX_AID_ON_TXT,			// 44
	DSN_RX_AID_ON_TXT,			// 45
	ANT_STOW,					// 46
	CID_TXT,					// 47
	ST_NAME,					// 48
	PRI,						// 49
	OPE_MODE,					// 50
	DOWN_START,					// 51
	DOWN_END,					// 52
	UP_START,					// 53
	UP_END,						// 54
	RNG_START,					// 55
	RNG_END,					// 56
	CMD_START,					// 57
	CMD_END,					// 58
	TLM_START,					// 59
	TLM_END,					// 60
	AutoGenerated,				// 61
	INIT_CID,					// 62
	PROP_READ,					// 63
	_T(""),
};

#define STR_SX_ACQRX_S			_T("SX-ACQRX_S")
#define STR_SX_ACQRX_X			_T("SX-ACQRX_X")
#define STR_SS_TCR				_T("SS-TCR")
#define STR_S_RX				_T("S-RX")
#define STR_S_RX_A				_T("S-RX_A")
#define STR_X_RX2_A				_T("X-RX2_A")
#define STR_SS_TCRDET			_T("SS-TCRDET")
#define STR_1					_T("1")
#define STR_2					_T("2")
#define STR_3					_T("3")
#define STR_4					_T("4")

#define STR_SS_TCR_AB			_T("SS-TCR_AB")
#define STR_X_RX				_T("X-RX")
#define STR_X_QPSKRX_A			_T("X-QPSKRX_A")
#define STR_DSN_MODEM_A			_T("DSN-MODEM_A")
#define STR_DSN_MODEM_X			_T("DSN-MODEM_X_A")
#define STR_DSN_MODEM_KA		_T("DSN-MODEM_KA_A")
#define STR_SS					_T("SS")
#define STR_SX					_T("SX")
#define STR_XX					_T("XX")
#define STR_RX					_T("RX")
#define STR_DXN					_T("DXN")
#define STR_DXR					_T("DXR")
#define STR_DKAN				_T("DKAN")
#define STR_DKAR				_T("DKAR")
#define STR_S					_T("S")
#define STR_X					_T("X")
#define STR_X2					_T("X2")
#define STR_KA					_T("KA")
#define STR_QPSK				_T("QPSK")
#define STR_ANT					_T("ANT")
#define STR_S_RX_AB				_T("S-RX_AB")
#define CTRL_4WAY				_T("CTRL_4WAY")	
#define CTRL_OLD				_T("_OLD")
#define OPE_TYPE				_T("OPE_TYPE")


static const CString sz_sx[] = {
	_T("S"),
	_T("X"),
	_T("S/X"),
	_T(""),
};

static const CString sz_on[] = { 
	_T("OFF"),
	_T("ON"),
	_T(""),
};
static const CString sz_auto[] = {
	_T("MAN"),
	_T("AUTO"),
	_T("")
};
static const CString sz_term_ant[] = {
	_T("POSN"),
	_T("NA"),
	_T("")
};
static const CString sz_term_ant64[] = {
	_T("PROG"),
	_T("MAN"),
	_T("STOW"),
	_T("NA"),
	_T("POS-1"),
	_T("POS-2"),
	_T("POS-3"),
	_T("POS-4"),
	_T("")
};
static const CString sz_rdychk[] = {
	_T("OFF"),
	_T("AGC"),
	_T("RNG"),
	_T("AGC+RNG"),
	_T("")
};
static const CString sz_on_off[] = { 
	_T("ON"),
	_T("OFF"),
	_T("")
};
static const CString sz_tx_ramp[] = {
	_T("ON"),
	_T("AVE"),
	_T("STT"),
	_T("NOMINAL"),
	_T("")
};
static const CString sz_rx_ramp[] = {
	_T("SX"),
	_T("S"),
	_T("X"),
	_T("OFF"),
	_T("")
};
static const CString sz_rx_ramp54[] = {
	_T("X"),	//��M�h�b�v���⏞ �L��
	_T("OFF"),	//��M�h�b�v���⏞ ����
	_T("")
	_T("")
	_T("")
};
static const CString sz_delay_sel[] = {
	_T("REP"),
	_T("NOR"),
	_T("")
};
static const CString sz_delay_sel_btn[] = {
	_T("�Đ��^"),
	_T("�]���^"),
	_T("")
};
static const CString sz_delay_sel_prm[] = {
	_T("REPLAY"),
	_T("NORMAL"),
	_T("")
};
static const CString sz_delay_sel_dsn_prm[] = {
	_T("REGEN"),
	_T("TURNAROUND"),
	_T("")
};

// CID >>>
static const CString sz_tx_pwr_sel_cid20[] = {
	_T("HPA-10kw"),
	_T("SSPA-1kw"),
	_T("SSPA-100w"),
	_T("SSPA-25w"),
	_T("HPA-1kw"),
	_T("HPA-100w"),
	_T("")
};

static const CString sz_s_tx_pwr_sel_cid34[] = {
	_T("HPA-10kw"),
	_T("SSPA-1kw"),
	_T("SSPA-100w"),
	_T("SSPA-25w"),
	_T("SSPA-75w"),
	_T("HPA-1kw"),
	_T("HPA-100w"),
	_T("")
};

static const CString sz_x_tx_pwr_sel_cid34[] = {
	_T("20kw"),
	_T("2kw"),
	_T("200w"),
	_T("100w"),
	_T("10w"),
	_T("")
};

static const CString sz_s_tx_pwr_sel_cid64[] = {
	_T("20kw"),
	_T("2kw"),
	_T("200w"),
	_T("")
};

// sz_s_tx_pwr_sel_cid54[]�́A���g�p�Ȃ̂Œǉ��s�v

static const CString sz_x_tx_pwr_sel_cid64[] = {
	_T("HPA1-20kw"),
	_T("HPA2-20kw"),
	_T("HPA1-2kw"),
	_T("HPA2-2kw"),
	_T("HPA1-200w"),
	_T("HPA2-200w"),
	_T("")
};

static const CString sz_x_tx_pwr_sel_cid54[] = {
	_T("40kW"),
#ifdef _GREAT_XSSPA_ISO
	_T("19.5kW"),
#else
	_T("20kW"),
#endif // _GREAT_XSSPA_ISO
	_T("2kW"),
	_T("200W"),
	_T("")
};
// <<< CID

static const CString sz_tx_pwr_sel20[] = {
	_T("HPA_10KW"),
	_T("SSPA_1KW"),
	_T("SSPA_100W"),
	_T("SSPA_25W"),
	_T("HPA-1KW"),
	_T("HPA-100W"),
	_T("")
};

static const CString sz_s_tx_pwr_sel[] = {
	_T("HPA_10kw"),
	_T("SSPA_1kw"),
	_T("SSPA_100w"),
	_T("SSPA_25w"),
	_T("SSPA_75w"),
	_T("HPA-1kw"),
	_T("HPA-100w"),
	_T("")
};
static const CString sz_x_tx_pwr_sel[] = {
	_T("HPA_20kw"),
	_T("HPA_2kw"),
	_T("HPA_200w"),
	_T("SSPA_100w"),
	_T("SSPA_10w"),
	_T("")
};
static CString sz_tx_pwr_sel64[] = {
	_T("20kw"),
	_T("2kw"),
	_T("200w"),
	_T("")
};
static CString sz_tx_pwr_sel54[] = {
	_T("40kW"),
#ifdef _GREAT_XSSPA_ISO
	_T("19.5kW"),
#else
	_T("20kW"),
#endif // _GREAT_XSSPA_ISO
	_T("2kW"),
	_T("200W"),
	_T("")
};
static const CString sz_s_tx_uc_sel[] = {
	_T("U/C1"),
	_T("U/C2"),
	_T("")
};
static const CString sz_mod_sel[] = {
	_T("DSN-MODEM"),
	_T("TCRMOD"),
	_T("")
};
static const CString sz_range_band[] = {
	_T("SS"),
	_T("SX"),
	_T("")
};
static const CString sz_range_band_ctrl[] = {
	_T("S/S"),
	_T("S/X"),
	_T("")
};
static const CString sz_ant_stow[] = {
	_T("ON"),
	_T("OF"),
	_T("NA"),
	_T("")
};

static const CString sz_ope_type[] = {
	_T("NORM"),
	_T("RECV"),
	_T("")
};

#define PLAN_GETFORM _T("�Ǌǐ�")
#define PLAN_TYPE	 _T("���^�p")


struct stPlandata {
	CString		sz_filepath;		// �v��t�@�C���t���p�X
	CString		sz_ftpfilepath;		// �v��t�@�C���o�̓t���p�X
	CString		sz_start;			// �p�X�J�n���� "YYYY-MM-DD HH:MM:SS"[IN/OUT]
	CString		sz_end;				// �p�X�I������ "YYYY-MM-DD HH:MM:SS"[IN/OUT]
	CString		sz_mes;				// �d���`�F�b�N���b�Z�[�W[OUT]
	CString		sz_s_bitrate;		// �r�� ���M���x�� High/Mid/Low/OFF  [IN/OUT]
	CString		sz_x_bitrate;		// �w�� ���M���x�� High/Mid/Low/OFF  [IN/OUT]
	CString		sz_ka_bitrate;		// Ka�� �r�b�g���[�g
	CString		sz_x_qpsk;			// �w��QPSK ���M���x�� High/Mid/Low/OFF  [IN/OUT]
	CString		sz_cmd_bit_rate;	// CMD S BIT RATE High/Mid/Low/OFF  [IN/OUT]
	CString		sz_x_cmd_bit_rate;	// CMD X BIT RATE High/Mid/Low/OFF  [IN/OUT]
	long		l_cmd_mod;			// CMD MOD 0:�蓮,1:���� [IN/OUT]
	long		l_rng_mod;			// RNG MOD 0:�蓮,1:���� [IN/OUT]
	CString		sz_rng_band;		// RNG�g�p�o���h S/X/SX  [IN/OUT]
	CString		sz_rng_start;		// RNG�J�n����   MM:SS   [IN/OUT]
	CString		sz_rng_end;			// RNG�I������   HH:MM:SS[IN/OUT]
	long		l_rdychk;			// ���f�B�l�X�`�F�b�N 0:����,1:AGC,2:RNG,3:AGC+RNG[IN/OUT]
	long		l_init;				// �����ݒ� 0:����,1:�L�� [IN/OUT]
	long		l_auto_acq;			// �����ߑ� 0:����,1:�L�� [IN/OUT]
	long		l_offset;			// �����I�t�Z�b�g�ւ̒Ǐ] 0:����,1:�L�� [IN/OUT]
	long		l_a_flag;			// AOS FLAG 0:OFF,1:ON   [IN/OUT]
	long		l_l_flag;			// LOS FLAG 0:OFF,1:ON   [IN/OUT]
	long		l_term_ant;			// �p�X�I�����`�m�s���[�h 0:POSN, 1:N/A       [IN/OUT]
	long		l_tx_ramp;			// ���M�ݔ������v����L���i0:ON, 1:�Z���^�[, 2:�x�X�g���b�N, 3:�m�~�i���j
	CString		sz_tx_ramp;			// ���M�ݔ������v����L���i0:ON, 1:�Z���^�[, 2:�x�X�g���b�N, 3:�m�~�i���j������
	long		l_rx_ramp;			// ��M�ݔ������v����L���i0:S/X, 1:S, 2:X, 3:�Ȃ��j 54m(0:�L�� 1:����)
	long		l_acq_route;		// �⑫���[�g�i�O�`�V�j
	long		l_pl_sel;			// ACQ/MAIN����L���i0:ON, 1:OFF�j
	long		l_top_mon;			// �V���ʉߊĎ��L���i0:ON, 1:OFF�j
	long		l_ent_angle;		// �V���ʉߓ��p
	long		l_ext_angle;		// �V���ʉߏo�p
	CString		sz_level_s;			// �r�щq�����M���x��    [IN/OUT]
	CString		sz_level_x;			// �w�щq�����M���x��    [IN/OUT]
	CString		sz_ant_s;			// �r�уA���e�i�I�����  [IN/OUT]
	CString		sz_ant_x;			// �w�уA���e�i�I�����  [IN/OUT]
	CString		sz_pred_path;		// �\��l�t�@�C���p�X��
	long		l_offset_val;		// �����I�t�Z�b�g�l [IN]
	long		l_delay_sel;		// �Ǔ��f�B���C�l�I�� 0:�Đ��^,1:�]���^ [IN/OUT]
	long		l_tx_pwr_sel;		// ���M�o�͑I��
	CString 	sz_tx_pwr_sel;		// ���M�o�͑I��(������)
	long		l_mod_sel;			// �w�ѕϒ����u�I�� 0:MOD,1:TCRMOD [IN/OUT]
	long		l_tx_uc_sel;		// �r�ё��M���g���ϊ��ՑI�� 0:U/C1,1:U/C2 [IN/OUT]
	long		l_antwait_flg;		// �A���e�i�҂��󂯎w���ʒm
	CString		sz_antwait_flg;		// �A���e�i�҂��󂯎w���ʒm ������
	long		l_ant_time;			// �A���e�i�҂��󂯎w���ʒm����
	long		l_rdy_time;			// ���f�B�������`�F�b�N�J�n����
	long		l_agc_index;		// ���f�B�l�X�`�F�b�N AGC
	time_t		l_agc_time;
	long		l_agc_s_time;
	long		l_agc_x_time;
	long		l_rng_index;		// ���f�B�l�X�`�F�b�N RNG
	time_t		l_rng_time;
	long		l_rng_xx_time;
	long		l_rng_rx_time;
	long		l_tlm_index;		// ���f�B�l�X�`�F�b�N TLM
	time_t		l_tlm_time;
	long		l_tlm_s_time;
	long		l_tlm_x_time;
	long		l_cmd_index;		// ���f�B�l�X�`�F�b�N CMD
	time_t		l_cmd_time;
	long		l_range_band;
	long		l_ant_stow;
	CString		sz_ant_stow;
	long		l_route_mode;		// X�ё���M���[�g����
	long		l_tx_mode;			// ���M�o�́@0:S-TX�A1�FX-TX

	long				l_uplink;				// �A�b�v�����N �L��
	vector<CString>		sz_uplink_s_timelst;	// �A�b�v�����N �������X�g
	vector<CString>		sz_uplink_e_timelst;	// �A�b�v�����N �������X�g
	long				l_rng;					// RNG �L��
	vector<CString>		sz_rng_s_timelst;		// RNG �������X�g
	vector<CString>		sz_rng_e_timelst;		// RNG �������X�g
	vector<CString>		sz_rng_kindlst;			// RNG ��ʃ��X�g
	long				l_cmd;					// CMD �L��
	vector<CString>		sz_cmd_s_timelst;		// CMD �������X�g
	vector<CString>		sz_cmd_e_timelst;		// CMD �������X�g
	CString				sz_cmd_type;
	long				l_tlms;					// TLM S �L��
	vector<CString>		sz_tlms_s_timelst;		// TLM S �������X�g
	vector<CString>		sz_tlms_e_timelst;		// TLM S �������X�g
	long				l_tlmx;					// TLM X �L��
	vector<CString>		sz_tlmx_s_timelst;		// TLM X �������X�g
	vector<CString>		sz_tlmx_e_timelst;		// TLM X �������X�g
	long				l_tlmka;				// TLM Ka �L��
	vector<CString>		sz_tlmka_s_timelst;		// TLM Ka �������X�g
	vector<CString>		sz_tlmka_e_timelst;		// TLM Ka �������X�g
	CString		sz_cid;							// CID
	CString		sz_station;						// �ǖ�
	long		l_selene_4way;					// �S�v�`�x�^�p�L��
	CString		sz_priority;					// ��]��
	CString		sz_ope_mode;					// �^�p���[�h
	CString		sz_auto;						// �������[�h

	__int64		l_antwait_time;					// �A���e�i�҂��󂯎w������
	__int64		l_antinit_time;					// �A���e�i����������
	__int64		l_initmin_time;					// �ݔ������ݒ�ŏ�����

	CString		sz_plan_type;					// �v��^�C�v
};

#define SOS_HEADER_ID	_T("#!HEAD:")	/* �t�@�C���Ǘ����� */

// ud_plan_rw ���[�h
enum {
	READ_MODE,
	WRITE_MODE,
	PRINT_MODE,
	DEL_MODE,
	FORM_MODE,
};

#define XTX_Y_ID 1	/* "Y"�w��h�c�l */
#define XTX_N_ID 2  /* "N"�w��h�c�l */

class COprPlnData
{
public:
	COprPlnData(){};
	~COprPlnData(){};

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
#if 0	// �R�s�[�R���X�g���N�^
	COprPlnData &operator=(const COprPlnData &other)
	{
		// ���g�̑���`�F�b�N
		if (this != &other) {
			m_strPlanID = other.m_strPlanID;
		}
		return *this;
	}
#endif
	void saveTime(CArchive& ar, stTime& val)
	{
		ar << val.start.GetTime();
		ar << val.end.GetTime();
	}
	void Save(CArchive& ar)
	{
		UINT i, size;
		ar << m_strPredID;
		ar << m_strPlanID;
		saveTime(ar, m_OpetDay);
		ar << m_UpLinkChk;
		ar << m_UpLinkCnt;
		size = (UINT)m_ULDay.size();
		ar << size;
		for (i = 0; i<size; i++){
			saveTime(ar, m_ULDay[i]);
		}
		ar << m_RngChk;
		ar << m_RngCnt;
		size = (UINT)m_RngDayList.size();
		ar << size;
		for (i = 0; i<size; i++){
			saveTime(ar, m_RngDayList[i]);
		}
		ar << m_CmdChk;
		ar << m_CmdCnt;
		size = (UINT)m_CmdDayList.size();
		ar << size;
		for (i = 0; i<size; i++){
			saveTime(ar, m_CmdDayList[i]);
		}
		ar << m_TlmSChk;
		saveTime(ar, m_TlmSDay);
		ar << m_TlmXChk;
		saveTime(ar, m_TlmXDay);
		ar << m_TlmKaChk;
		saveTime(ar, m_TlmKaDay);
		ar << m_AntStandby;
		ar << m_strAntStandbyTime;
		ar << m_AntZenith;
		ar << m_OpInit;
		ar << m_OpTDP;
		ar << m_OpRDP;
		ar << m_OpTOutType;
		ar << m_OpTOut;
		ar << m_AgcChkValue;
		ar << m_RndChkValue;
		ar << m_TlmChkValue;
		ar << m_CmdChkValue;
		ar << m_strCID;
		ar << m_strBRTLMS;
		ar << m_strBRTLMX;
		ar << m_strBRTLMKa;
		ar << m_strBRHTLM;
		ar << m_strBRCMDS;
		ar << m_strBRCMDX;
		saveTime(ar, m_AntFcstValue);
	}
	void loadTime(CArchive& ar, stTime& val)
	{
		__time64_t time;
		ar >> time;
		val.start = CTime(time);
		ar >> time;
		val.end = CTime(time);
	}
	void Load(CArchive& ar)
	{
		UINT i, size;
		ar >> m_strPredID;
		ar >> m_strPlanID;
		loadTime(ar, m_OpetDay);
		ar >> m_UpLinkChk;
		ar >> m_UpLinkCnt;
		ar >> size;
		m_ULDay.clear();
		for (i = 0; i<size; i++){
			stTime val;
			loadTime(ar, val);
			m_ULDay.push_back(val);
		}
		ar >> m_RngChk;
		ar >> m_RngCnt;
		ar >> size;
		m_RngDayList.clear();
		for (i = 0; i<size; i++){
			stTime val;
			loadTime(ar, val);
			m_RngDayList.push_back(val);
		}
		ar >> m_CmdChk;
		ar >> m_CmdCnt;
		ar >> size;
		m_CmdDayList.clear();
		for (i = 0; i<size; i++){
			stTime val;
			loadTime(ar, val);
			m_CmdDayList.push_back(val);
		}
		ar >> m_TlmSChk;
		loadTime(ar, m_TlmSDay);
		ar >> m_TlmXChk;
		loadTime(ar, m_TlmXDay);
		ar >> m_TlmKaChk;
		loadTime(ar, m_TlmKaDay);
		ar >> m_AntStandby;
		ar >> m_strAntStandbyTime;
		ar >> m_AntZenith;
		ar >> m_OpInit;
		ar >> m_OpTDP;
		ar >> m_OpRDP;
		ar >> m_OpTOutType;
		ar >> m_OpTOut;
		ar >> m_AgcChkValue;
		ar >> m_RndChkValue;
		ar >> m_TlmChkValue;
		ar >> m_CmdChkValue;
		ar >> m_strCID;
		ar >> m_strBRTLMS;
		ar >> m_strBRTLMX;
		ar >> m_strBRTLMKa;
		ar >> m_strBRHTLM;
		ar >> m_strBRCMDS;
		ar >> m_strBRCMDX;
		loadTime(ar, m_AntFcstValue);
	}
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString			m_strPlanFilePath;	// �v��t�@�C���t���p�X
	CString			m_strFtpPlanFilePath;	// FTP�v��t�@�C���t���p�X
	CString			m_strSatellite;		// �q����
	CString			m_strPredID;		// �\��lID
	CString			m_strPlanID;		// ��{�p�����[�^-�v��ID
	stTime			m_OpetDay;			// ��{�p�����[�^-�^�p�J�n�I������
	BOOL			m_UpLinkChk;		// ��{�p�����[�^-�A�b�v�����N �L��/�����t���O
	int				m_UpLinkCnt;		// ��{�p�����[�^-�A�b�v�����N �ݒ萔
	vector<stTime>	m_ULDay;			// ��{�p�����[�^-�A�b�v�����N �J�n�I���������X�g
	BOOL			m_RngChk;			// ��{�p�����[�^-RNG �L��/�����t���O
	int				m_RngCnt;			// ��{�p�����[�^-RNG �ݒ萔
	vector<stTime>	m_RngDayList;		// ��{�p�����[�^-RNG �J�n�I���������X�g
	vector<CString>	m_RngTypeList;		// ��{�p�����[�^-RNG ��ʃ��X�g
	BOOL			m_CmdChk;			// ��{�p�����[�^-CMD �L��/�����t���O
	int				m_CmdCnt;			// ��{�p�����[�^-CMD �ݒ萔
	vector<stTime>	m_CmdDayList;		// ��{�p�����[�^-CMD �J�n�I������
	BOOL			m_TlmSChk;			// ��{�p�����[�^-TLM S �L��/�����t���O
	stTime			m_TlmSDay;			// ��{�p�����[�^-TLM S �J�n�I������
	vector<stTime>	m_TlmSDayList;		// ��{�p�����[�^-TLM S �J�n�I������
	BOOL			m_TlmXChk;			// ��{�p�����[�^-TLM X �L��/�����t���O
	stTime			m_TlmXDay;			// ��{�p�����[�^-TLM X  �J�n�I������
	vector<stTime>	m_TlmXDayList;		// ��{�p�����[�^-TLM X  �J�n�I������
	BOOL			m_TlmKaChk;			// ��{�p�����[�^-TLM Ka �L��/�����t���O
	stTime			m_TlmKaDay;			// ��{�p�����[�^-TLM Ka �J�n�I������
	vector<stTime>	m_TlmKaDayList;		// ��{�p�����[�^-TLM Ka �J�n�I������

	int				m_AntStandby;		// ANT�҂������w���ʒm
	CString			m_strAntStandby;	// ANT�҂������w���ʒm ������
	CString			m_strAntStandbyTime;// ANT�҂��󂯎w���ʒm ����	
	int				m_AntZenith;		// ANT�V���u���L��
	CString			m_strAntZenith;		// ANT�V���u���L�� ������

	int				m_OpInit;			// �^�p�I�v�V����-�����ݒ�
	int				m_OpTDP;			// �^�p�I�v�V����-���M�@�h�b�v���⏞
	CString			m_OpTDPStr;			// �^�p�I�v�V����-���M�@�h�b�v���⏞ ������
	int				m_OpRDP;			// �^�p�I�v�V����-��M�@�h�b�v���⏞
	int				m_OpTOutType;		// �^�p�I�v�V����-���M�o�̓^�C�v
	int				m_OpTOut;			// �^�p�I�v�V����-���M�d��
	CString			m_OpTOutstr;		// �^�p�I�v�V����-���M�d�͕�����
	CString			m_OpPriority;		// ��]��

	BOOL			m_AgcChkValue;		// ���f�B�l�X�`�F�b�N-AGC�Z��
	BOOL			m_RndChkValue;		// ���f�B�l�X�`�F�b�N-RNG�Z��
	BOOL			m_TlmChkValue;		// ���f�B�l�X�`�F�b�N-TLM�]��
	BOOL			m_CmdChkValue;		// ���f�B�l�X�`�F�b�N-CMD�`��

	CString			m_strCID;			// CID�i"CIDXXXXXXX"�j
	CString			m_strBRTLMS;		// �r�b�g���[�g-TLM S
	CString			m_strBRTLMX;		// �r�b�g���[�g-TLM X
	CString			m_strBRTLMKa;		// �r�b�g���[�g-TLM Ka	(�v��t�@�C�����o�͗p)
	CString			m_strBRHTLM;		// �r�b�g���[�g-����TLM
	CString			m_strBRCMDS;		// �r�b�g���[�g-CMD S
	CString			m_strBRCMDX;		// �r�b�g���[�g-CMD X

	stTime			m_AntFcstValue;		// �A���e�i�\��l (AOS,LOS)
	stTime			m_InitPassTime;		// ����������
	stTime			m_AntWaitTime;		// ANT�҂��󂯎w��
	stTime			m_PrePassTime;		// �v���p�X�i�^�p�v��O�̏����j����

	bool			mGetForm;			// true:enp false:sttn

	CString			m_strOpeMode;		// �^�p���[�h
	CString			m_strAuto;			// ��������

	CString			m_strPlanType;		// �v��^�C�v

	CTime			m_AgcTime;			// ���f�B�l�X�`�F�b�NAGC����
	CTime			m_RngTime;			// ���f�B�l�X�`�F�b�NRNG����
	CTime			m_TlmTime;			// ���f�B�l�X�`�F�b�NTLM����
	CTime			m_CmdTime;			// ���f�B�l�X�`�F�b�NCMD����

protected:

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */

public:
	static int WritePlanFile(CString sz_satname, CString sz_passno, stPlandata stpd, CString comment=_T(""));
	static int RegistPlanNew(CString sz_satname, CString sz_passno, BOOL bNew = TRUE);
	static int WriteRdyChkPlanFile(CString sz_satname, long satno, CString sz_passno, stPlandata stpd, vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist);
	static int ReadRdyChkPlanFile(CString sz_satname, CString sz_passno, stPlandata& stpd, vector<stCalibPlan>& agclist, vector<stCalibPlan>& tlmlist, vector<stCalibPlan>& cmdlist);

public:
	static int GetAntStandbyIdx(CString str);
	static int GetAntZenithIdx(CString str);
	static int GetOpInitIdx(CString str);
	static int GetOpTDPIdx(CString str);
	static int GetOpTDPIdx2(CString str);
	static int GetOpRDPIdx(CString str);
	static int GetOpTOutIdx(CString str, int type=0);

protected:

};


// ���g�������ʃf�[�^�Ǘ��N���X

// �^�C�}��`
#define TID_FREQMON			1
#define INTERVAL_FREQMON	800

// �ݒ���g��-U/C
static const CString FreqCtrlFreqUC = _T("X-UC.FREQUENCY");
static const CString FreqCtrlFreqUCCtrl = _T("X-UC.FREQUENCY");

// �ݒ���g��-D/C
static const CString FreqCtrlFreqDC = _T("S70-DC.MON_FREQ");
static const CString FreqCtrlFreqDCCtrl = _T("S70-DC.MON_FREQ");

// �ݒ���g��-�R�}���h�T�u�L�����A
static const CString FreqCtrlFreqSC = _T("DSNTCR.TX_SC_FREQ");
static const CString FreqCtrlFreqSCCtrl = _T("DSNTCR.TX_SC_FREQ");

// �I�t�Z�b�g-���M���g���I�t�Z�b�g
static const CString FreqCtrlOffSetTx = _T("XX-DSNTCRFC.FREQ_OFFSET");
static const CString FreqCtrlOffSetTxCtrl = _T("XX-DSNTCRFC.FREQ_OFFSET");

// �I�t�Z�b�g-�����I�t�Z�b�g
static const CString FreqCtrlOffSetTime = _T("XX-DSNTCRFC.TIME_OFFSET");
static const CString FreqCtrlOffSetTimeCtrl = _T("XX-DSNTCRFC.TIME_OFFSET");

// �h�b�v���⏞-���M�h�b�v��
static const CString FreqCtrlDopplerTx = _T("CTRL.UP_FREQ_MODE");
static const CString FreqCtrlDopplerTxCtrl = _T("CTRL.UP_FREQ_MODE");

// �h�b�v���⏞-��M�h�b�v��
static const CString FreqCtrlDopplerRx = _T("XX-DSNTCRFC.DOPP_MODE");
static const CString FreqCtrlDopplerRxCtrl = _T("XX-DSNTCRFC.DOPP_MODE");

// �h�b�v���⏞-�R�}���h�T�u�L�����A
static const CString FreqCtrlDopplerSC[] = {
	_T("CTRL.SC_AID_MODE"),	// �P�c64m��
	_T(""),					// �P�c54m��
	_T("CTRL.SC_AID_MODE"),	// ���V�Y34m��
	_T("CTRL.SC_AID_MODE")	// ���V�Y20m��
};
static const CString FreqCtrlDopplerSCCtrl[] = {
	_T("CTRL.SC_AID_MODE"),	// �P�c64m��
	_T(""),					// �P�c54m��
	_T("CTRL.SC_AID_MODE"),	// ���V�Y34m��
	_T("CTRL.SC_AID_MODE")	// ���V�Y20m��
};

// �T���@COH���[�h
static const CString FreqCtrlCohMode = _T("DSNTCR.TRP_RATIO");


// ���M�h�b�v��
enum{
	TXDOPP_A,
	TXDOPP_N_C,
	TXDOPP_N_T,
};
static const CString szTxDopp[] = {
	_T("����"),
	_T("�Ȃ�(�Z���^�[)"),
	_T("�Ȃ�(�����w��)"),
	_T("")
};
// ��M�h�b�v��
enum{
	RXDOPP_1WAY,
	RXDOPP_2WAY,
	RXDOPP_3WAY,
};
static const CString szRxDopp[] = {
	_T("���I�⏞(1WAY)"),
	_T("���I�⏞(2WAY)"),
	_T("���I�⏞(3WAY)"),
	_T("")
};
// �R�}���h�L�����A
static const CString szCmdCarr[] = {
	_T("���I�⏞"),
	_T("�ÓI�⏞(�Z���^�[)"),
	_T("�ÓI�⏞(�m�~�i��)"),
	_T("�ÓI�⏞(AOS�x�X�g���b�N)"),
	_T("")
};
// �T���@���[�h
static const CString szSatMode[] = {
	_T("INCOH"),
	_T("COH-880/749"), 
	_T("COH-3344/749"),
	_T("COH-3328/749"),
	_T("COH-3360/749"),
	_T("")
};
// 3WAY�p���Ǒ��M���g��
enum {
	OTHER_SAT_34,
	OTHER_SAT_64,
	OTHER_SAT_54,
};
static const CString sz3WayOtherSta[] = {
	_T("34m"),
	_T("64m"),
	_T("54m"),
	_T("")
};



class CFreqCtrlData
{
public:
	CFreqCtrlData();
	~CFreqCtrlData();

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

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
protected:

};


// ���M�����ʃf�[�^�Ǘ��N���X
#define	CTLNAME_LEN	28								/* �ݔ����䖼�̍ő咷+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12								/* �ݔ����̍ő咷+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* ���䍀��ID�ő咷+1 */
//struct obsname_t
//{
//	long	l_id;					/* �f�[�^����: �萔=OBSNAME_ID�l */
//	long	l_len;					/* �f�[�^��  : sizeof( struct obsname_t ) */
//	long	l_no;					/* �Ď����ڔԍ��i�R���p�C�����V�[�P���V�����ɕt������B�j */
//	char	sz_obsname[CTLID_LEN]; 	/* �Ď����ڂh�c: �ݔ���.�Ď����ږ� */
//	long	l_inflg;				/* �f�[�^�L���t���O 1=�L��  ���P */
//	long	l_grmd;					/* �X�e�[�^�X�t���O 1=�L��,2=���������� ���P */
//	long	l_data;					/* �ϊ����ʁi�����j         ���P */
//	double	d_data;					/* �ϊ����ʁi�����j         ���P */
//	char	sz_sts_name[64];		/* �ϊ����ʃX�e�[�^�X������ */
//
//	__int64	i64_data;				/* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x ���P */
//	long	l_bitinfo;				/* �����r�b�g��:�������o�f�[�^�̃r�b�g��:1..64�A==0:�������o�ł͂Ȃ�   ���P */
//	long	l__rfu__;				/* ���g�p�Fpadding�o�C�g */
//};

#define CTRL_SWEEP_NA        (long)0      /* 1shot-sweep */
#define CTRL_SWEEP_BEFORE    (long)1      /* 1shot-sweep */
#define CTRL_SWEEP_AFTER     (long)2      /* 1shot-sweep */

#define SWEEP_MODE_NON	0
#define SWEEP_MODE_SAW	1
#define SWEEP_MODE_TRI	2

#define TID_CONDITION	1
#define TID_COUNTDOWN	2

#define INTERVAL_CONDITION		800
#define INTERVAL_COUNTDOWN		1000

#define RNG_TYPE_NOR	1
#define RNG_TYPE_REP	2
#define RNG_TYPE_NON	-1

#define MON_STR_PASS     "�p�X��"			// �p�X�^�p���t���O �Ď��p�����[�^
#define MON_STR_NOPASS   "�p�X�O"			// �p�X�^�p���t���O �Ď��p�����[�^

class CTrnCtrlData
{
public:
	CTrnCtrlData();
	~CTrnCtrlData();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CEngValCnv::obsname_t	m_obs[100];	// �_�~�[

protected:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:

protected:

};


// �����Ɩ�������ʃf�[�^�Ǘ��N���X
class CBsnsDiaryData
{
public:
	CBsnsDiaryData();
	~CBsnsDiaryData();

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

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
protected:

};


// CID�I����ʃf�[�^�Ǘ��N���X

typedef struct {	// �q�����ACID�I������
	vector<CString>	CID;				// CID
	vector<CString>	TlmBitRateS;		// �e�����g���r�b�g���[�g S��
	vector<CString>	TlmBitRateX;		// �e�����g���r�b�g���[�g X��
	vector<CString>	TlmBitRateHX;		// �e�����g���r�b�g���[�g X����
	vector<CString>	TlmBitRateKa;		// �e�����g���r�b�g���[�g Ka��
	vector<CString>	CmdBitRateS;		// �R�}���h�r�b�g���[�g S��
	vector<CString>	CmdBitRateX;		// �R�}���h�r�b�g���[�g X��
	vector<CString>	TrsPower;			// ���M�d��
	vector<CString>	UpLinkSweepShape;	// �A�b�v�����N�|���p�����[�^ �`��
	vector<CString>	RngMesMode;			// �����W�v���p�����[�^ ���[�h
	vector<CString>	CarrLoopBandS;		// �L�����A��M���[�v�o���h S��
	vector<CString>	CarrLoopBandX;		// �L�����A��M���[�v�o���h X��
	vector<CString>	CarrLoopBandKa;		// �L�����A��M���[�v�o���h Ka��
	vector<CString>	CmdMod;				// �R�}���h�ϒ��x
	vector<CString>	CmdSubCarrFreq;		// �R�}���h�T�u�L�����A���g��
	vector<CString>	TransDoppler;		// ���M�h�b�v��
	vector<CString>	AntAutoTrack;		// �A���e�i�����ǔ��ڍs�̗L��
	vector<CString>	Modulation;			// �ϒ�����
	vector<CString>	ModulationS;		// �ϒ����� S��
	vector<CString>	ModulationX;		// �ϒ����� X��
	vector<CString>	AntAosPosn;			// �A���e�i�҂��󂯎w���̗L��
	vector<CString>	AntWaitPoint;		// �A���e�i�ҋ@�ʒu�̗L��
}stCIDPrmEle;

typedef struct {	// CID���p�����[�^
	CString	CID;				// CID
	CString	TlmBitRateS;		// �e�����g���r�b�g���[�g S��
	CString	TlmBitRateX;		// �e�����g���r�b�g���[�g X��
	CString	TlmBitRateHX;		// �e�����g���r�b�g���[�g X����
	CString	TlmBitRateKa;		// �e�����g���r�b�g���[�g Ka��
	CString	CmdBitRateS;		// �R�}���h�r�b�g���[�g S��
	CString	CmdBitRateX;		// �R�}���h�r�b�g���[�g X��
	CString	TrsPower;			// ���M�d��
	CString	CarrLoopBandS;		// �L�����A��M���[�v�o���h S��
	CString	CarrLoopBandX;		// �L�����A��M���[�v�o���h X��
	CString	CarrLoopBandKa;		// �L�����A��M���[�v�o���h Ka��
	CString	UpLinkSweepShape;	// �A�b�v�����N�|���p�����[�^ �`��
	CString	UpLinkSweepWidth;	// �A�b�v�����N�|���p�����[�^ ��
	CString	UpLinkSweepTime;	// �A�b�v�����N�|���p�����[�^ ����
	CString	RngMesMode;			// �����W�v���p�����[�^ ���[�h
	CString	RngMesSmplRate;		// �����W�v���p�����[�^ �T���v�����O���[�g
	CString	RngMesSmplItgTime;	// �����W�v���p�����[�^ �ϕ�����
	CString	RngMesSmplMod;		// �����W�v���p�����[�^ �ϒ��x
	CString	Modulation;			// �ϒ����� S��
	CString	ModulationS;		// �ϒ����� S��
	CString	ModulationX;		// �ϒ����� X��
	CString	CmdMod;				// �R�}���h�ϒ��x
	CString	CmdSubCarrFreq;		// �R�}���h�T�u�L�����A���g��
	CString	TransDoppler;		// ���M�h�b�v��
	CString	AntAutoTrack;		// �A���e�i�����ǔ�
	CString	AntAosPosn;			// �A���e�i�҂��󂯎w��
	CString	AntWaitPoint;		// �A���e�i�ҋ@�ʒu�w��
}stCIDParam;

typedef struct {	// CID���p�����[�^
	BOOL	CID;				// CID
	BOOL	TlmBitRateS;		// �e�����g���r�b�g���[�g S��
	BOOL	TlmBitRateX;		// �e�����g���r�b�g���[�g X��
	BOOL	TlmBitRateHX;		// �e�����g���r�b�g���[�g X����
	BOOL	TlmBitRateKa;		// �e�����g���r�b�g���[�g Ka��
	BOOL	CmdBitRateS;		// �R�}���h�r�b�g���[�g S��
	BOOL	CmdBitRateX;		// �R�}���h�r�b�g���[�g X��
	BOOL	TrsPower;			// ���M�d��
	BOOL	CarrLoopBandS;		// �L�����A��M���[�v�o���h S��
	BOOL	CarrLoopBandX;		// �L�����A��M���[�v�o���h X��
	BOOL	CarrLoopBandKa;		// �L�����A��M���[�v�o���h Ka��
	BOOL	UpLinkSweepShape;	// �A�b�v�����N�|���p�����[�^ �`��
	BOOL	UpLinkSweepWidth;	// �A�b�v�����N�|���p�����[�^ ��
	BOOL	UpLinkSweepTime;	// �A�b�v�����N�|���p�����[�^ ����
	BOOL	RngMesMode;			// �����W�v���p�����[�^ ���[�h
	BOOL	Modulation;			// �ϒ����� S��
	BOOL	ModulationS;		// �ϒ����� S��
	BOOL	ModulationX;		// �ϒ����� X��
	BOOL	CmdMod;				// �R�}���h�ϒ��x
	BOOL	CmdSubCarrFreq;		// �R�}���h�T�u�L�����A���g��
	BOOL	TransDoppler;		// ���M�h�b�v��
	BOOL	AntAutoTrack;		// �A���e�i�����ǔ�
	BOOL	AntAosPosn;			// �A���e�i�҂��󂯎w��
	BOOL	AntWaitPoint;		// �A���e�i�ҋ@�ʒu�w��
}stCIDPrmFlg;

#define CID_INVALID		_T("---------")

class CCIDSlctData
{
public:
	CCIDSlctData();
	~CCIDSlctData();

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
	vector<CString>							m_SatelliteNameList;	// �q�������X�g
	map<CString, stCIDPrmEle>				m_CidElementList;		// �q����CID�p�����[�^�v�f
	map<CString, stCIDPrmFlg>				m_CidPrmFlgList;		// �q����CID�p�����[�^�L��/�����t���O
	map<CString, map<CString, stCIDParam>>	m_CidParamList;			// �q����CID���p�����[�^�ݒ�l

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CString>&						GetSatelliteNameList() { return m_SatelliteNameList; }
	vector<CString>							GetCidList(CString sat);
	map<CString, stCIDPrmEle>&				GetCidElementList() { return m_CidElementList; }
	map<CString, map<CString, stCIDParam>>&	GetCidParamList() { return m_CidParamList; }
	map<CString, stCIDPrmFlg>&				GetCidPrmFlgList()	{ return m_CidPrmFlgList; }
	static stCIDParam						TrimInvalidString(stCIDParam stCid);

protected:

};



// �g�����h�v���p�e�B�������}�b�v�f�[�^���Ǘ�����N���X
class CTrendPropertiesMemMapData
{
public:
	CTrendPropertiesMemMapData(void);
	~CTrendPropertiesMemMapData(void);

	DWORD GetPropertiesDataCnt() { return m_nPropertiesDataCnt; }


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
	CString	m_strMMDataFile;			// �������}�b�v�f�[�^�t�@�C����
	HANDLE	m_hMMDataFile;				// �������}�b�v�f�[�^�t�@�C���n���h��
	HANDLE	m_hMMDataObject;			// �������}�b�v�f�[�^�t�@�C���}�b�s���O�I�u�W�F�N�g�n���h��
	LPVOID	m_pMMDataMapView;			// �������}�b�v�f�[�^�t�@�C���}�b�v�r���[
	vector<LPVOID>	m_listMMDataMap;	// �������}�b�v�f�[�^�}�b�v���X�g
	DWORD	m_nPropertiesDataCnt;		// �v���p�e�B�f�[�^��

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
protected:
	void	ClearParameter()
	{
		m_strMMDataFile.Empty();
		m_hMMDataFile = INVALID_HANDLE_VALUE;
		m_hMMDataObject = NULL;
		m_pMMDataMapView = NULL;
		m_listMMDataMap.clear();
	}

	bool	MappingUpdateMMDataFile(DWORD nMMDataMapView);

public:
	void	SetMMDataFileName(CString strMMDataFile)	{ m_strMMDataFile = strMMDataFile; }

	bool	IsOpenMMDataFile()	{ return (m_hMMDataFile != INVALID_HANDLE_VALUE) ? true : false; }

public:
	bool	OpenMMDataFile(bool flgRead = true);
	void	CloseMMDataFile();

	bool	WriteMMDataFile(CPropertiesData& propertiesData);
	bool	ReadMMDataFile(DWORD idx, CPropertiesData& propertiesData);
	/* ------------------------------------------------------------------------------------ */
};

#if 0
// �g�����h�v���p�e�B�������}�b�v�f�[�^���X�g���Ǘ�����N���X
class CTrendPropertiesMemMapList
{
public:
	CTrendPropertiesMemMapList(void);
	~CTrendPropertiesMemMapList(void);


	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum eMMListStatus
	{
		eMMListStatus_None = 0,
		eMMListStatus_WriteOpen = 1,
		eMMListStatus_ReadOpen = 2
	};

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	eMMListStatus		m_eMMListStatus;		// �������}�b�v�f�[�^���X�g�X�e�[�^�X
	vector<CTrendPropertiesMemMapData>	m_listMMData;			// �f�[�^�p�̃������}�b�v�f�[�^���X�g
	map<CString, CTrendPropertiesMemMapData>	m_MapMMData;	// �f�[�^�p�̃������}�b�v�f�[�^���X�g

	unsigned __int64	m_countData;			// �ۑ������f�[�^��
	CString				m_strMMInfoFile;		// �������}�b�v���t�@�C����

protected:
	CString				m_strBaseName;			// �x�[�X�t�@�C����
	unsigned __int64	m_posRWPos;				// �f�[�^�̓ǂݏ����f�[�^�ʒu(0�`Prime�f�[�^��-1�F0�`18,446,744,073,709,551,615)

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
protected:
	void	ClearParameter()
	{
		m_eMMListStatus = eMMListStatus_None;

		m_listMMData.clear();

		m_countData = 0;
		m_strMMInfoFile.Empty();

		m_strBaseName.Empty();
		m_posRWPos = 0;
	}

	void	SaveMMInfoFile()
	{
		int a = 0;
	}

	void	LoadMMInfoFile()
	{
		int a = 0;
	}

public:
	UINT	GetMaxSizePerMMData()	{ return MAX_SIZE_PER_MMDATA; }

	eMMListStatus	GetMemMapListStatus()	{ return m_eMMListStatus; }
	UINT	GetFileCount()	{ return (UINT)((m_countData + (GetMaxSizePerMMData() - 1)) / GetMaxSizePerMMData()); }
	unsigned __int64	GetDataCount()	{ return m_countData; }
	CString	GetMMInfoFileName()	{ return m_strMMInfoFile; }

public:
	bool	DeleteMemMapList();

	bool	WriteInitialization(CString strFileName);
	bool	WriteFinalization();
	bool	WriteData(CString strFileName, vector<CPropertiesData>& propertiesdataList);

	bool	ReadInitialization(CString strFileName);
	bool	ReadFinalization();
	bool	ReadData(CString strFileName, vector<CPropertiesData>& propertiesdataList);
	/* ------------------------------------------------------------------------------------ */
};
#endif

// �\��l���X�g�Ǘ��N���X
typedef struct {
	CString	strStationName;						// �ǖ�
	CString strSatellite;						// �q����
	CString strPassId;							// �p�XID
	__int64 tAOS;								// AOS ����
	double  dAosAz;								// AOS AZ
	double  dAosEl;								// AOS EL
	__int64 tLOS;								// LOS ����
	double  dLosAz;								// LOS AZ
	double  dLosEl;								// LOS EL
	__int64 tMaxEl;								// Max EL ����
	double  dMaxAz;								// Max AZ
	double  dMaxEl;								// Max EL
	CString strPredPath;						// �\��l�i�[�p�X
	long	tOffset;							// �I�t�Z�b�g����
	CString strFormat;							// �^�C�v
	CString strGetFrom;							// �擾��
} stPredListData;


// �v�惊�X�g�Ǘ��N���X
typedef struct {
	CString	strStationName;						// �ǖ�
	CString	strSatelliteName;					// �q����
	CString	strPassId;							// �p�XID
	__int64	tPrePassStart;						// �v���p�X�J�n����
	__int64	tPassStart;							// �p�X�J�n����
	__int64	tPassEnd;							// �p�X�I������
	CString	strPredName;						// �\��l�t�@�C����
	CString strPlanName;						// �v��t�@�C��
	long	lValidFlag;							// �`�F�b�N���ʃt���O
	long	lOfsFlag;							// �����I�t�Z�b�g�Ǐ]�����t���O
	long	tOffset;							// �����I�t�Z�b�g
	CString	strGetFrom;							// �擾��
	CString	strPlanType;						// �v����
	CString strResult;							// ���s����
	CString strMaintComment;					// MAINT�p�R�����g
	CString strPostPass;						// �|�X�g�p�X�i�`�m�s�V���w���j
} stPlanListData;


// �z�X�g���u����`
#define STTN_OPR_HOST  "k3opr1\n"		// USC �ǉ^�p�ǐ����u�z�X�g��
#define SSOC_OPR_HOST  "szopr2\n"		// SSOC�ǉ^�p�ǐ����u�z�X�g��

#define	DEV_NO_STTN_OPR		0xe1		// �o��Ǒ��ǉ^�p�ǐ����u
#define	DEV_NO_SSOC_OPR		0xe2		// ���͌����ǉ^�p�ǐ����u

#define DATA_ID_POS 6                   // �f�[�^��ʈʒu
#define SEND_ID_POS 4                   // �ǉ^�p�ǐ����u�p�v�����@�\���ʈʒu
#define REAL_DATA_POS 12                // ���M���f�[�^�J�n�ʒu
#define DATA_NO_USE_CHAR NULL           // ���M�f�[�^���g�p�����ݒ�l
#define LF_CHAR 0x0a                    // LF�����R�[�h

// op_ipc.h >>>
#define MSG_ALLOC       1
#define MAX_SDTP		(32*1024)
#define SDTP_INFO		24
#define SDTP_HED		32
#define SDTP_INFO_FIELD 12
#define DATA_PDU_K		0x010020b0		// �f�[�^�o�c�t
// <<< op_ipc.h

// dns_def.h >>>
#define	DEV_NO_EQU_CTRL		0xc0					// �ݔ����䑕�u
#define	INFO_ID_DAT_CTLEQU	(unsigned char)(0x02)	// �ݔ�����f�[�^
#define	INFO_SENDID_STTN    0x10					// �o�摤�ǉ^�p�ǐ����u���@�\
#define	INFO_SENDID_SSOC    0x11					// ���͌��ǉ^�p�ǐ����u���@�\
#define	INFO_NO_SEND		0x00					// ���M���Ȃ�
// <<< dns_def.h

// sdtp.h >>>
#define SD_OPR_CONTROL		0x0007		// UDSC64 & KSC34 �ݔ�����
// <<< sdtp.h

// optcmesg.h >>>
#define DEFAULT_SEND_IPC   0x8403		// ���M�p
// <<< optcmesg.h

struct  sdtp_msg_t {					// sdtp ���b�Z�[�W�e�[�u��
//	long        l_unix_msgtype;			// ���b�Z�[�W�^�C�v(64)
	long long   l_unix_msgtype;			// ���b�Z�[�W�^�C�v(64)
	long        l_len;					// ���M�o�C�g��
	long        l_objtype;				// �v���h�c
	long        l_blkcnt;				// �u���b�N�J�E���g
	long        l_send_ipc;				// ���M�����b�Z�[�W�L���[�h�c
	long        l_recv_ipc;				// ��M�����b�Z�[�W�L���[�h�c
	unsigned char uca_SdtpDummy[26];	// ��M�����b�Z�[�W�L���[�h�c
	unsigned char uca_SdtpPDULen[4];	// PDU��
	unsigned char uca_SdtpRecNum[2];	// ���R�[�h��
	unsigned char uca_buff[MAX_SDTP];	// �f�[�^�o�b�t�@
};

/*                                   1   2   3   4   5   6   7   8   9  10  11  12 */
static const long l_chkday[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#pragma pack(1)
// �����v���p�e�B
typedef struct
{
	int		leapyear;	// ���邤�N�i���q���Ɋ֌W�Ȃ��j
	int		leapsec;	// ���邤�b�i���q���Ɋ֌W�Ȃ��j
	double	gain;		// �n���ANT�����i���q���Ɋ֌W�Ȃ��j
	int		autocalc;	// �\�z��M���x�������v�Z�I��
	double	reclevels;	// �\�z��M���x���i�Œ�l�j
	double	reclevelx;	// �\�z��M���x���i�Œ�l�j
	double	eirp;		// �q�����MEIRP
	bool	tlmflag;	// TLM�T�u�L�����A
	double	tlm;		// TLM�T�u�L�����A
	bool	cmdflag;	// CMD�T�u�L�����A
	double	cmd;		// CMD�T�u�L�����A
} stInitProp;

// �^�p�v���p�e�B
typedef struct
{
	int		event_autocalc;	// �C�x���g�����̎����v�Z
	DWORD	even_sunset;	// �V���ʉߊp�x����
	DWORD	event_sunrise;	// �V���ʉߊp�x����
	DWORD	mg_ULon;		// �}�[�W���p�xU/L ON
	DWORD	mg_ULoff;		// �}�[�W���p�xU/L OFF
	DWORD	mg_DL;			// �}�[�W���p�xD/L
	DWORD	of_CMD;			// U/L�I���O�̎����I�t�Z�b�g CMD�^�p�I��
	DWORD	of_RNG;			// U/L�I���O�̎����I�t�Z�b�g RNG�^�p�I��
	char	cid[30];		// �f�t�H���gCID
	int		opespace;		// �q���^�pCMD/RNG�����ϒ�
	stInitProp	init;	// �����v���p�e�B
} stOpeProp;
#pragma pack()

/* ------------------------------------------------------------------------------------ */
/* �ǐ��f�[�^�Ǘ��N���X
/* ------------------------------------------------------------------------------------ */
class CSatelliteData
{
public:
	CSatelliteData();
	~CSatelliteData();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	union   umoji {                         /* ������擪2�����̃o�b�t�@ */
		char   c_moji[2];
		short  s_moji;
	};


	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	//	vector<CRdnsChkData>		m_listRdnsChkData;													// �Z���f�f��ʃf�[�^
	CRdnsChkData				m_CRdnsChkData;														// �Z���f�f��ʃf�[�^
	map<CString, vector<COprPlnData>> m_listOprPlnData;													// �v�旧�ĉ�ʃf�[�^
	CFreqCtrlData				m_listFreqCtrlData;														// ���g�������ʃf�[�^
	CTrnCtrlData				m_TrnCtrlData;															// ���M�����ʃf�[�^
	CBsnsDiaryData				m_listBsnsDiaryData;													// �����Ɩ�������ʃf�[�^
	CCIDSlctData				m_CIDSlctData;															// CID�I����ʃf�[�^
	CTrackingRadarCtrlMonData	m_arrayTrackingRadarCtrlMonData[eStation_MAX + 2];						// �ǔ����[�_�[����Ď��f�[�^�z��i�T�}��x4��, �\��l�ꗗ�A�v��ꗗ �j
//	CTrackingRadarCtrlMonData	m_dummyTrackingRadarCtrlMonData[eStation_MAX + 2];						// �ǔ����[�_�[����Ď��_�~�[�f�[�^�i�T�}��x4��, �\��l�ꗗ�A�v��ꗗ �j
	CSpectrumViewData			m_spectrumData;															// �X�y�N�g�����f�[�^
	CSpectrumViewData			m_arraySummarySpectrumData[eStation_MAX][MAX_SUMMARY_VIEW];				// �X�y�N�g�����f�[�^�i�T�}��x4�� x �r���[���p�Ӂj
	CPropertiesData				m_arrayTrendPropertiesData[eTrendWndType_Num];							// �g�����h�v���p�e�B�f�[�^�z��i�O���t��ސ��p�Ӂj
	CPropertiesData				m_arraySummaryPropertiesData[eStation_MAX][MAX_SUMMARY_VIEW];			// �T�}���v���p�e�B�f�[�^�z��i�T�}��x4�� x �r���[���p�Ӂj
	vector<stPredListData>		m_listPredList;															// �\��l�ꗗ�f�[�^
	vector<stPlanListData>	m_listPlanList;																// �v��ꗗ�f�[�^
	CString						m_strSelectedForecastFilePath;											// �I�������\��l�t�@�C����
	CTrendData					m_trendData;															// �q���ߑ��f�[�^
	CSessionInfo				m_sessionInfo[eStation_MAX];											// �Z�b�V�������
	CRangePropagation			m_rngPropagation;														// RNG�`��

	struct sat_inf_t satinfo;																			// �q���{�̏��e�[�u��
	struct sat_inf_t m_satinfo[TBLMAX];																	// �q���{�̏��e�[�u��

	CTime						mGetSharePredFileTime[eStation_MAX];
	CTime						mGetSharePlanFileTime[eStation_MAX];

	FILETIME					m_SessionFileTime;														// �Z�b�V�����t�@�C������

	CRFTripAngleFile			m_arrayRFTripData[eStation_MAX];										// RF�g���b�v�f�[�^

//	CTrendPropertiesMemMapList	m_trendPropMemMapList;													// �g�����h�v���p�e�B�������}�b�v�f�[�^���X�g

protected:
	sdtp_msg_t	m_st_sdtp_msg;			// SDTP�ʐM�p�\����

	CDBAccess	m_dbaccess;				// DB �A�N�Z�X�N���X

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	static double ub_asc_to_dtime(char* sz_time);
	static int ub_asc_to_asctime(char sz_input_time[], char sz_output_time[]);
	static int ub_asc_to_asctime(TCHAR sz_input_time[], TCHAR sz_output_time[]);
	static void ub_dtime_to_asc(double d_uttime, char* sz_time);
//	static int ua_strmatch(char* string, char* pattern);
//	static long ub_getln(FILE* fp, long* lp_line, char* argv[], long argc);
//	static int ub_fgetconf(TCHAR* sz_filename, char* keyword, char* formatstring, 
//							char* arg1 = NULL, char* arg2 = NULL, char* arg3 = NULL, char* arg4 = NULL, char* arg5 = NULL, char* arg6 = NULL, char* arg7 = NULL, char* arg8 = NULL);

	static int MovetHeaderInFile(FILE* fp, TCHAR* strId);
	static int GetConfFromFile(FILE* fp, TCHAR* strId, long* val);
	static int GetConfFromFile(FILE* fp, TCHAR* strId, double* val);

	CPropertiesData&	GetTrendPropertiesData(int pos)
	{
		if (pos < 0 || pos >= eTrendWndType_Num)
			return m_arrayTrendPropertiesData[0];
		return m_arrayTrendPropertiesData[pos];
	}

	void				SetTrendPropertiesData(int pos, CPropertiesData val)	{ m_arrayTrendPropertiesData[pos] = val; }

	CPropertiesData&	GetSummaryPropertiesData(int nStationIndex, int pos)
	{
		if (nStationIndex < 0 || nStationIndex >= eStation_MAX || pos < 0 || pos >= MAX_SUMMARY_VIEW)
			return m_arraySummaryPropertiesData[0][0];
		return m_arraySummaryPropertiesData[nStationIndex][pos];
	}

	void				SetSummaryPropertiesData(int nStationIndex, int pos, CPropertiesData val) { m_arraySummaryPropertiesData[nStationIndex][pos] = val; }

	void				CreateSummaryPropertiesData();

	CSpectrumViewData& GetSpectrumViewData()	{ return m_spectrumData; }

	CSpectrumViewData& GetSummarySpectrumViewData(int nStationIndex, int pos)
	{
		if (pos < 0 || pos >= eStation_MAX)
			return m_arraySummarySpectrumData[0][0];
		return m_arraySummarySpectrumData[nStationIndex][pos];
	}

	void SetSummarySpectrumStation()
	{
		for (int i = 0; i < eStation_MAX; i++)
		{
			for (int j = 0; j < MAX_SUMMARY_VIEW; j++)
			{
				CSpectrumViewData& spectrum = GetSummarySpectrumViewData(i, j);
				spectrum.SetStation(i);
			}
		}
	}

//	static CString AddCanma(double pos, int pointKeta = 0);
	static CString AddCanma(double pos, int pointKeta, int singFlag = 0);
	void SpectrumInitialize();
	void SpectrumInitialize(UINT nStationID);
	int SpectrumFileRead(UINT nStationID);
	int SpectrumFileWrite(UINT nStationID);

//	CTrendPropertiesMemMapList& GetTrendPropertiesMemMapList()	{ return m_trendPropMemMapList; }
	void TrendPropInitialize();

	bool	TrendPropWriteData(UINT nStationID);

	bool	TrendPropReadData(UINT nStationID);

	CTrendData& GetTrendData()		{ return m_trendData; }

//	BOOL	SetPredListDataList();
//	BOOL	SetPredList_Utf8();
//	BOOL	SetPredList_Utf8(UINT nStation);
	BOOL	SetPredList_Euc();
	BOOL	SetPredList_Euc(UINT nStation);
	BOOL	IsPredListFileUpdated(UINT nStation);
	static CTime	StrTimeToCTime(CString strTime);

	vector<stPredListData>&	GetPredList() { return m_listPredList; }				// �\��l�ꗗ�f�[�^

//	BOOL SetPlanList();
//	BOOL SetPlanList_Utf8();
//	BOOL SetPlanList_Utf8(UINT nStation);
	BOOL SetPlanList_Euc();
	BOOL SetPlanList_Euc(UINT nStation);
	BOOL IsPlanListFileUpdated(UINT nStation);

	vector<stPlanListData>&	GetPlanList() { return m_listPlanList; }				// �v��ꗗ�f�[�^�擾
	BOOL IsUsePredFile(CString& strFilePath);

	CString opctproc(CString strCtrl, CString strParam, CString val = _T(""));
	void opctproc2(CString strCtrl, CString strParam);

	void SendControlSelect(CString strCtrl, stControlValue* pval, CString strCtrl2 = _T(""));
	int RegistControlSelect(CString strCtrl, stControlValue* pval);
	int param_chk(CString strCtrl, stControlValue* pval = NULL, CString strAddStatus = _T(""), BOOL bSeqCtrl = FALSE, CString strCtrl2 = _T(""));
	int param_chk2(CString strCtrl, stControlValue* pval, CString strCtrl2, stControlValue* pval2);
	bool GetEquipmentParam(CString &strCtrl, struct paramtbl_t *param_adr[], struct val_t *val_adr[], stEquipment2Ctrl *stEquip, double fval);
	int get_paramlist(CString strCtrl, vector<CString> &list);

	BOOL IsPllBwCtrl(CString strCtrl);

	BOOL IsBitRateCtrl(CString strCtrl);
	BOOL IsCmdBitRateCtrl(CString strCtrl);
	CString CtrlBitRate(CString strCtrl, stControlValue* pval = NULL, BOOL bSeqCtrl = FALSE);

	CTrackingRadarCtrlMonData& GetTrackingRadarCtrlMonData(int pos)
	{ 
		if (pos < 0 || pos >= eStation_MAX + 2)
			return m_arrayTrackingRadarCtrlMonData[0];
		return m_arrayTrackingRadarCtrlMonData[pos];
	}

	CRFTripAngleFile& GetRFTripAngleData(int pos)
	{
		if (pos < 0 || pos >= eStation_MAX)
			return m_arrayRFTripData[0];
		return m_arrayRFTripData[pos];
	}
	void ReadRFTripFile();

	UINT	GetSimpleStationID(CString name);
	UINT	GetStationID(CString name);

	BOOL ReadCIDFile();

	void	GetShareFile(UINT val=0xffffffff);

	inline const bool CompareData(stPredListData& val1, stPredListData& val2)
	{
		if (val1.strStationName != val2.strStationName)	return true;
		if (val1.strSatellite != val2.strSatellite)	return true;
		if (val1.strPassId != val2.strPassId)	return true;
		if (val1.tAOS != val2.tAOS)	return true;
		if ((__int64)(val1.dAosAz*100.0) != (__int64)(val2.dAosAz*100.0))	return true;
		if ((__int64)(val1.dAosEl*100.0) != (__int64)(val2.dAosEl*100.0))	return true;
		if (val1.tLOS != val2.tLOS)	return true;
		if ((__int64)(val1.dLosAz*100.0) != (__int64)(val2.dLosAz*100.0))	return true;
		if ((__int64)(val1.dLosEl*100.0) != (__int64)(val2.dLosEl*100.0))	return true;
		if (val1.dLosEl != val2.dLosEl)	return true;
		if (val1.tMaxEl != val2.tMaxEl)	return true;
		if ((__int64)(val1.dMaxAz*100.0) != (__int64)(val2.dMaxAz*100.0))	return true;
		if ((__int64)(val1.dMaxEl*100.0) != (__int64)(val2.dMaxEl*100.0))	return true;
		if (val1.strPredPath != val2.strPredPath)	return true;
		if (val1.tOffset != val2.tOffset)	return true;
		if (val1.strFormat != val2.strFormat)	return true;
		if (val1.strGetFrom != val2.strGetFrom)	return true;
		return false;
	}
	inline const bool CompareData(stPlanListData& val1, stPlanListData& val2)
	{
		if (val1.strStationName != val2.strStationName)	return true;
		if (val1.strSatelliteName != val2.strSatelliteName)	return true;
		if (val1.strPassId != val2.strPassId)	return true;
		if (val1.tPrePassStart != val2.tPrePassStart)	return true;
		if (val1.tPassStart != val2.tPassStart)	return true;
		if (val1.tPassEnd != val2.tPassEnd)	return true;
		if (val1.strPredName != val2.strPredName)	return true;
		if (val1.lValidFlag != val2.lValidFlag)	return true;
		if (val1.lOfsFlag != val2.lOfsFlag)	return true;
		if (val1.tOffset != val2.tOffset)	return true;
		if (val1.strGetFrom != val2.strGetFrom)	return true;
		if (val1.strPlanType != val2.strPlanType)	return true;
		if (val1.strResult != val2.strResult)	return true;
		return false;
	}

	CSessionInfo& GetSessionInfo(int pos)
	{
		if (pos < 0 || pos >= eStation_MAX)
			return m_sessionInfo[0];
		return m_sessionInfo[pos];
	}
	BOOL ReadSessionStatusFile();
	void DeleteSessionStatusFile();

	CRangePropagation& GetRangePropagation()	{ return m_rngPropagation; }

	int stn_tbl_access();
	CDBAccess& GetDBAccessCls() { return m_dbaccess; }
	int opctcheck(struct paramtbl_t *param_adr, struct val_t *val_adr, char sz_indata[], int i_signflag);
	int uc_valchk(char *s, int *i_p, double *d_p, int l_opt);

	// ���C�A�E�g�ǂݍ��݁��ۑ��i�ݒ���j
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);

	// �v���p�e�B�t�@�C���Ǎ�
	int ReadPropertyFile(CString strSat, stOpeProp &stProp);

	// �v��t�@�C�����[�o�͗p�e���|�����t�@�C���쐬
	typedef struct {
		CString strDate;
		CTime	tDate;
		CString strEventName;
		CString strCtrl;
		CString strPrm;
	}stPlnoutDetail;
	CString	CreatePlnPrintFile(CString fname);

	static void ExcelOutPut(HWND hwnd, CString param);
	static int ExcelOutPutProc(HWND hwnd, CString param, BOOL bWaitComplete = TRUE);

protected:
	void AddDataVector(vector<CString> &v, CString str);
};


/* ------------------------------------------------------------------------------------ */
/* StationControl��OperationTimeline�Ƃ̊Ď��f�[�^
/* ------------------------------------------------------------------------------------ */
class CMonitorDataTemp
{
public:
	CMonitorDataTemp(){};
	~CMonitorDataTemp(){};

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:


	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CEngValCnv::obs_tbl_res_t>	mData;

protected:

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:

protected:

};