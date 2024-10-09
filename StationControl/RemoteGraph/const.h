#pragma once
//#pragma pack(1)

using namespace std;

//=============================================================================
//	�r���h�I�v�V����
//=============================================================================

//#define	_TEST_MAXDATA			1				// �ŏI�e�[�u����3���ԂƂ��Ď���
//#define	_TEST_LEAP				1				// ���邤�b�������ɔ��������Ď���

#ifdef	_TEST_MAXDATA
#define	HighSpeedDemo		1					// �����f��
#endif	// 	_TEST_MAXDATA

#ifndef _PIPE_COLLECTION
//#define	HighSpeedDemo		1					// �����f��
#endif	// _PIPE_COLLECTION

#ifdef	_PIPE_COLLECTION
#ifdef	_DEBUG
//#define	DemoTimeStamp		1					// �f���p�C�v�ʐM(��M�f�[�^�Ɍ��ݎ������X�^���v)
//#define	DemoStepData			1					// �f���K�i�g�`�f�[�^�ʐM
//#define	DemoSaigen			1					// �c���ڐ���\���s���Č�
//#define DemoSin			1						// �f��Sin�g�`
#endif	// _DEBUG
#endif	// _PIPE_COLLECTION

//=============================================================================
//	�萔
//=============================================================================
#define MAX_TDATA	10
#define MAX_VIEW	 8
#define MAX_VSCALE	 2
#define MAX_HSCALE	 2

#define	DREAL_NAN_HIGH		0x7ff80000
#define	DREAL_NAN_LOW		0x00000000
#define	DREAL_ERROR_HIGH	0xffffffff
#define	DREAL_ERROR_LOW		0xfffffffe

#define MAX_OUTPUT_LIST		100

#define	MAX_USEDATA_DAYS	200					// �A�v���������񈳏k�f�[�^�̍ő����(�P�ʁF��)
#define	DATA_BLOCK_UNIT		8					// �A�v���������񈳏k�f�[�^�̃u���b�N�P��(�P�ʁF����)
#define	DATA_BLOCK_RANGE	1					// �A�v���������񈳏k�f�[�^�̃u���b�N�͈�(�P�ʁF��)
#define	MAX_HOLDDATA_TIME	(DATA_BLOCK_UNIT + (DATA_BLOCK_UNIT * (DATA_BLOCK_RANGE * 2)))	// ��������ɕێ�����񈳏k�f�[�^�̍ő厞��(�P�ʁF����)
#define MAX_HOLDDATA_SIZE	(3600 * MAX_HOLDDATA_TIME)										// ��������ɕێ�����񈳏k�f�[�^�̍ő�f�[�^��(���b��)
#define MAX_ALLDATA_SIZE	(3600 * 24 * MAX_USEDATA_DAYS)									// �A�v���������񈳏k�f�[�^�̍ő�f�[�^��(���b��)
#define MAX_COMPDATA_SIZE	(3600 + 1)			// ���k�f�[�^�ێ��T�C�Y�i�E�B���h�E�E�[�ʒu�̃f�[�^�\���̂��߁A+1����j
#define MAX_DATASELECT		80
#define	MAX_LIMIT_HISTORY	10					// ���~�b�g����

#ifdef	_TEST_MAXDATA
#define	MAX_SIZE_PER_MMDATA		3600	// �������}�b�v�f�[�^������̍ő呪��f�[�^��(�P���ԕ��̕b����60�~60)
										// �P�t�@�C���̃T�C�Y�F3600�~40�o�C�g��144000�o�C�g(��140KB)
#else	// 	_TEST_MAXDATA
#define	MAX_SIZE_PER_MMDATA		86400	// �������}�b�v�f�[�^������̍ő呪��f�[�^��(�P�����̕b����60�~60�~24)
										// �P�t�@�C���̃T�C�Y�F86400�~40�o�C�g��3456000�o�C�g(��3MB)
#endif	// 	_TEST_MAXDATA

#define VECMAX			256		/* �c�a�����ő���i�p�����[�^�������ӁB�j */
#define	OBSNAME_ID		0x0101		/* ������`�h�c */

#define	CTLNAME_LEN	28	/* �ݔ����䖼�̍ő咷+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12	/* �ݔ����̍ő咷+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* ���䍀��ID�ő咷+1 */

#define	FILE_VERSION		101				// INI�t�@�C���̃o�[�W����

#define	DISK_FREE_SPACE		10				// �f�B�X�N�̋󂫋��e�e��

#define	REALTIME_FNAME		_T("__tmp_RealTime")
#define	ANALYSIS_FNAME		_T("__tmp_Analysis")

//=============================================================================
//	�v���p�e�B
//=============================================================================
//#define	AXIS_MINMAX_RANGE		1000000000			// �c���̍ő啝
//#define	AXIS_MINMAX_VALUE		999999999.999999	// �c���̍ő�l
#define	AXIS_MINMAX_RANGE		10000000000000			// �c���̍ő啝
#define	AXIS_MINMAX_VALUE		9999999999999.0			// �c���̍ő�l
#define	AXIS_SCALE_MIN			2					// �ڐ���̕������̍ŏ��l
#define	AXIS_SCALE_MAX			10					// �ڐ���̕������̍ő�l
//#define	LIMIT_MINMAX_RANGE		1000000000			// ���~�b�g�̍ő啝
//#define	LIMIT_MINMAX_VALUE		999999999.999999	// ���~�b�g�̍ő�l
#define	LIMIT_MINMAX_RANGE		10000000000000			// ���~�b�g�̍ő啝
#define	LIMIT_MINMAX_VALUE		9999999999999.0			// ���~�b�g�̍ő�l

//=============================================================================
//	INI�t�@�C��
//=============================================================================
const CString SECTION_SYSTEM	= _T("SYSTEM");
const CString KEY_LOGLEVEL		= _T("LogLevel");		// ���O���x��
const CString KEY_SERVER		= _T("Server");			// �T�[�o�[�A�h���X
const CString KEY_SERVERPATH	= _T("ServerPath");		// �T�[�o�[�f�[�^�i�[�p�X
const CString KEY_FILEVERSION	= _T("FileVersion");	// �t�@�C���o�[�W����
const CString KEY_DATANAMEOUT	= _T("DataNameOut");	// �f�[�^�����o��
const CString KEY_CHECKDISKFREESPACE = _T("CheckDiskFreeSpace");	// �f�B�X�N�󂫗e�ʃ`�F�b�N

const CString SECTION_DATALIST	= _T("DATALIST");
const CString KEY_DATANAMENUM	= _T("DataNameNumber");	// �Ď����̓o�^��
const CString KEY_DATANAME		= _T("DataName%d");		// �Ď���

const CString SECTION_PROPDATA	= _T("PROPDATA");
const CString KEY_REALTIME		= _T("Realtime");
const CString KEY_VSCL_AUTO		= _T("VScaleAuto");
const CString KEY_VSCL_LMAX		= _T("VScaleLMax");
const CString KEY_VSCL_LMIN		= _T("VScaleLMin");
const CString KEY_VSCL_RMAX		= _T("VScaleRMax");
const CString KEY_VSCL_RMIN		= _T("VScaleRMin");
const CString KEY_VSCL_ITVL		= _T("VScaleItvl");
const CString KEY_HSCL_NUM		= _T("HScaleNumber%d");
const CString KEY_DATA_NAME		= _T("DataName%d");
const CString KEY_DATA_GNAME	= _T("DataGraphName%d");
const CString KEY_DATA_UNIT		= _T("DataUnit%d");
const CString KEY_GRAPH_VMODE	= _T("GraphViewMode%d");
const CString KEY_GRAPH_DMODE	= _T("GraphDrawMode%d");
const CString KEY_GRAPH_COL		= _T("GraphColor%d");
const CString KEY_GRAPH_DOTSZ	= _T("GraphDotSize%d");
const CString KEY_GRAPH_VSKIND	= _T("GraphVScaleKind%d");
const CString KEY_COM_STR		= _T("CommentString%d");
const CString KEY_COM_DISP		= _T("CommentDisplay%d");
const CString KEY_LMT_WAR_U_V	= _T("LimitWarningUpperValid%d");
const CString KEY_LMT_WAR_U		= _T("LimitWarningUpper%d");
const CString KEY_LMT_WAR_L_V	= _T("LimitWarningLowerValid%d");
const CString KEY_LMT_WAR_L		= _T("LimitWarningLower%d");
const CString KEY_LMT_ALM_U_V	= _T("LimitAlarmUpperValid%d");
const CString KEY_LMT_ALM_U		= _T("LimitAlarmUpper%d");
const CString KEY_LMT_ALM_L_V	= _T("LimitAlarmLowerValid%d");
const CString KEY_LMT_ALM_L		= _T("LimitAlarmLower%d");

enum eStationMode{
	eStation_1,
	eStation_5,
	eStation_10,
};

enum eStation{
	eStation_Usuda64,
	eStation_Usuda642,
	eStation_Uchinoura34,
	eStation_Uchinoura20,
	eStation_MAX,
};

static const TCHAR* mStationFolder[]{
	_T("udsc64"),
	_T("udsc54"),
	_T("usc34"),
	_T("usc20"),
};

static const TCHAR* mStationSimpleString[]{
	_T("UDSC64"),
	_T("UDSC54"),
	_T("USC34"),
	_T("USC20"),
};

// �t�@�C���^�C�v
enum eFileType{
	// �f�[�^�x�[�X
	eFileType_DB_Eqctl_File,
	eFileType_DB_Eqdsp_File,
	eFileType_DB_Eqinf_File,
	eFileType_DB_Eqmon_File,
	eFileType_DB_Eqsat_File,

	// �ݔ��Ď��f�[�^�i���f�[�^�j�t�H���_
	eFileType_Equipment_Folder,

	// �v�搗�`�t�H���_
	eFileType_Form_Folder,

	// �v��t�H���_
	eFileType_Plan_Folder,

	// �v��o�̓t�H���_
	eFileType_PlanOut_Folder,

	// �v��ꗗ�t�@�C��
	eFileType_PlanList_File,

	// �\��l�t�H���_
	eFileType_Pred_Folder,

	// �\��l�ꗗ�t�@�C��
	eFileType_PredList_File,

	// CID�t�H���_
	eFileType_CID_Folder,

	// �Ɩ�����/�p�X�����t�H���_
	eFileType_Diary_Folder,

	// �Ɩ������t�@�C��
	eFileType_Diary_File,

	// �Ɩ��������M���t�@�C��
	eFileType_Diary_File_Info,

	// RF���t�@�C��
	eFileType_RfInfo_File,

	// �^�p�����t�H���_
	eFileType_OpHistory_Folder,

	// ���䗚���t�H���_
	eFileType_CtrlHistory_Folder,

	// �ݔ������t�H���_
	eFileType_EquipHistory_Folder,

	// ���~�b�g�G���[�t�H���_
	eFileType_Limit_Folder,

	// �C�x���g�����t�H���_
	eFileType_EventHistory_Folder,

	// �A���e�i�ݔ��쓮�����t�H���_
	eFileType_AntEquipDriveHistory_Folder,

	// �A���e�i�ݔ��^�p�����t�H���_
	eFileType_AntEquipOpHistory_Folder,

	// RF�g���b�v�t�H���_
	eFileType_RFTrip_Folder,

	// RF�g���b�v�t�@�C��
	eFileType_RFTrip_25w_File,
	eFileType_RFTrip_100w_File,
	eFileType_RFTrip_200w_File,
	eFileType_RFTrip_1kw_File,
	eFileType_RFTrip_2kw_File,
	eFileType_RFTrip_20kw_File,

	// �X�J�C���C���t�H���_
	eFileType_Skyline_Folder,

	// �X�J�C���C���t�@�C��
	eFileType_Skyline_File,

	// �ǔ��\��l�t�H���_
	eFileType_Yoho_Folder,

	// �ǔ��\��l�t�@�C��
	eFileType_Yoho_File,

	// ���������`�t�H���_
	eFileType_SeqCtrl_Folder,

	// ���f�B�l�X�`�F�b�N���ԃt�@�C��
	eFileType_Rdychk_File,

	// �����[�g�V�F���t�H���_
	eFileType_Remsh,

	// �v���p�e�B�t�@�C��
	eFileType_Property,

	// �ǃt�H���_
	eFileType_Station_Folder,

	// �Ď��f�[�^�_���v�E��`�t�H���_
	eFileType_MonDump_Def,

	// �Ď��f�[�^�_���v�E�o�̓t�H���_
	eFileType_MonDump_Ftp,

	// AGC�Z���e�[�u���t�H���_
	eFileType_AGCCalib_Folder,

	// �t�@�C���E�t�H���_��`��
	eFileType_Max
};

// �t�@�C���E�t�H���_�p�X
static const TCHAR* mSvFileFolder[] =
{
	// �f�[�^�x�[�X�t�@�C��
	_T("ctrl\\db\\ld\\eqctl.db.obj"),
	_T("ctrl\\db\\ld\\eqdsp.db.obj"),
	_T("ctrl\\db\\ld\\eqinf.db.obj"),
	_T("ctrl\\db\\ld\\eqmon.db.obj"),
	_T("ctrl\\db\\ld\\eqsat.db.obj"),

	// �ݔ��Ď��f�[�^�i���f�[�^�j�t�H���_
	_T("log\\mon\\"),

	// �v�搗�`�t�H���_
	_T("ctrl\\plan\\form\\"),

	// �v��t�H���_
	_T("ctrl\\plan\\exec\\"),

	// �v��o�̓t�H���_
	_T("ctrl\\plan\\ftp\\"),

	// �v��ꗗ�t�@�C��
	_T("ctrl\\plan\\exec\\plan_list.txt"),

	// �\��l�t�H���_
	_T("ctrl\\prd\\"),

	// �\��l�ꗗ�t�@�C��
	_T("ctrl\\prd\\pred_list.txt"),

	// CID�t�H���_
	_T("ctrl\\db\\cid\\"),

	// �Ɩ�����/�p�X�����t�H���_
	_T("ctrl\\log\\pass\\"),

	// �Ɩ������t�@�C��
	_T("ctrl\\log\\pass\\radio_report.txt"),

	// �Ɩ��������M���t�@�C��
	_T("ctrl\\log\\pass\\rf_info_report.txt"),

	// RF���t�@�C��
	_T("ctrl\\log\\pass\\rarf_report.txt"),

	// �^�p�����t�H���_(�t�@�C�����FOperation_YYYYMMDD.txt)
	_T("log\\opr\\"),

	// ���䗚���t�H���_(�t�@�C�����FContrl_YYYYMMDD.txt)
	_T("log\\ctrl\\"),

	// �ݔ������t�H���_(�t�@�C�����FEquipment_YYYYMMDD.txt)
	_T("log\\moni\\"),

	// ���~�b�g�G���[�t�H���_
	_T("log\\limit\\"),

	// �C�x���g�����t�H���_(�t�@�C�����FEvent_YYYYMMDD.txt)
	_T("log\\event\\"),

	// �A���e�i�ݔ��쓮�����t�H���_
	_T("ctrl\\log\\ant\\"),

	// �A���e�i�ݔ��^�p�����t�H���_
	_T("ctrl\\log\\ant\\"),

	// RF�g���b�v�t�H���_
	_T("cfg\\etc\\"),

	// RF�g���b�v�t�@�C��
	_T("cfg\\etc\\ct_rftrip_25w.dat"),
	_T("cfg\\etc\\ct_rftrip_100w.dat"),
	_T("cfg\\etc\\ct_rftrip_200w.dat"),
	_T("cfg\\etc\\ct_rftrip_1kw.dat"),
	_T("cfg\\etc\\ct_rftrip_2kw.dat"),
	_T("cfg\\etc\\ct_rftrip_20kw.dat"),

	// �X�J�C���C���t�H���_
	_T("cfg\\etc\\"),

	// �X�J�C���C���t�@�C��
	_T("cfg\\etc\\ct_skyline.dat"),

	// �ǔ��\��l�t�H���_
	_T("cfg\\etc\\grph_yofile\\"),

	// �ǔ��\��l�t�@�C��
	_T("cfg\\etc\\grph_yofile\\opgt_yofile"),

	// ���������`�t�H���_
	_T("cfg\\etc\\cont\\def\\"),

	// ���f�B�l�X�`�F�b�N���ԃt�@�C��
	_T("cfg\\etc\\RDY_TIME.txt"),

	// �����[�g�V�F���t�H���_
	_T(""),

	// �v���p�e�B�t�@�C��
	_T(""),

	// �ǃt�H���_
	_T(""),

	// �Ď��f�[�^�_���v�E��`�t�H���_
	_T("log\\dump\\cfg\\"),

	// �Ď��f�[�^�_���v�E�o�̓t�H���_
	_T("log\\dump\\"),

	// AGC�Z���e�[�u���t�H���_
	_T("ctrl\\agc\\"),

	// �t�@�C���E�t�H���_��`��
	_T(""),
};

// �f�o�b�O�p���[�J���t�@�C���E�t�H���_�p�X
static const TCHAR* mLocalFileFolder[] =
{
	// �f�[�^�x�[�X�t�@�C��
	_T("ctrl\\db\\ld\\eqctl.db.obj"),
	_T("ctrl\\db\\ld\\eqdsp.db.obj"),
	_T("ctrl\\db\\ld\\eqinf.db.obj"),
	_T("ctrl\\db\\ld\\eqmon.db.obj"),
	_T("ctrl\\db\\ld\\eqsat.db.obj"),

	// �ݔ��Ď��f�[�^�i���f�[�^�j�t�H���_
	_T("log\\mon\\"),

	// �v�搗�`�t�H���_
	_T("ctrl\\plan\\form\\"),

	// �v��t�H���_
	_T("ctrl\\plan\\exec\\"),

	// �v��o�̓t�H���_
	_T("ctrl\\plan\\ftp\\"),

	// �v��ꗗ�t�@�C��
	_T("ctrl\\plan\\exec\\plan_list.txt"),

	// �\��l�t�H���_
	_T("ctrl\\prd\\"),

	// �\��l�ꗗ�t�@�C��
	_T("ctrl\\prd\\pred_list.txt"),

	// CID�t�H���_
	_T("ctrl\\db\\cid\\"),

	// �Ɩ�����/�p�X�����t�H���_
	_T("ctrl\\log\\pass\\"),

	// �Ɩ������t�@�C��
	_T("ctrl\\log\\pass\\radio_report.txt"),

	// �Ɩ��������M���t�@�C��
	_T("ctrl\\log\\pass\\rf_info_report.txt"),

	// RF���t�@�C��
	_T("ctrl\\log\\pass\\rarf_report.txt"),

	// �^�p�����t�H���_(�t�@�C�����FOperation_YYYYMMDD.txt)
	_T("log\\opr\\"),

	// ���䗚���t�H���_(�t�@�C�����FContrl_YYYYMMDD.txt)
	_T("log\\ctrl\\"),

	// �ݔ������t�H���_(�t�@�C�����FEquipment_YYYYMMDD.txt)
	_T("log\\moni\\"),

	// ���~�b�g�G���[�t�H���_
	_T("log\\limit\\"),

	// �C�x���g�����t�H���_(�t�@�C�����FEvent_YYYYMMDD.txt)
	_T("log\\event\\"),

	// �A���e�i�ݔ��쓮�����t�H���_
	_T("ctrl\\log\\ant\\"),

	// �A���e�i�ݔ��^�p�����t�H���_
	_T("ctrl\\log\\ant\\"),

	// RF�g���b�v�t�H���_
	_T("cfg\\etc\\"),

	// RF�g���b�v�t�@�C��
	_T("cfg\\etc\\ct_rftrip_25w.dat"),
	_T("cfg\\etc\\ct_rftrip_100w.dat"),
	_T("cfg\\etc\\ct_rftrip_200w.dat"),
	_T("cfg\\etc\\ct_rftrip_1kw.dat"),
	_T("cfg\\etc\\ct_rftrip_2kw.dat"),
	_T("cfg\\etc\\ct_rftrip_20kw.dat"),

	// �X�J�C���C���t�H���_
	_T("cfg\\etc\\"),

	// �X�J�C���C���t�@�C��
	_T("cfg\\etc\\ct_skyline.dat"),

	// �ǔ��\��l�t�H���_
	_T("cfg\\etc\\grph_yofile\\"),

	// �ǔ��\��l�t�@�C��
	_T("cfg\\etc\\grph_yofile\\opgt_yofile"),

	// ���������`�t�H���_
	_T("cfg\\etc\\cont\\def\\"),

	// ���f�B�l�X�`�F�b�N���ԃt�@�C��
	_T("cfg\\etc\\RDY_TIME.txt"),

	// �����[�g�V�F���t�H���_
	_T(""),

	// �v���p�e�B�t�@�C��
	_T(""),

	// �ǃt�H���_
	_T(""),

	// �Ď��f�[�^�_���v�E��`�t�H���_
	_T("log\\dump\\cfg\\"),

	// �Ď��f�[�^�_���v�E�o�̓t�H���_
	_T("log\\dump\\"),

	// AGC�Z���e�[�u���t�H���_
	_T("ctrl\\agc\\"),

	// �t�@�C���E�t�H���_��`��
	_T(""),
};

enum eMessagex{
	// �E�B���h�E�n
	eMessage_ChildActivate = (WM_USER + 100),
	eMessage_ChildDestroy,
	eMessage_ChangeActiveTab,
	eMessage_InitializeApplication,
	eMessage_UpdateGraph,
	eMessage_AnalysisKeyEvent,
	eMessage_UpdateZoomRectangle,
	eMessage_UpdateCursorLine,
	eMessage_MoveZoomRectangle,
	eMessage_CreateZoomRectangle,
	eMessage_MoveCursorLine,
	eMessage_CreateCursorLine,
	eMessage_CommentChange,
	eMessage_DifferChange,
//	eMessage_CloseWaitDialog,

	// �X���b�h�n
	eMessage_ThreadNormal,
	eMessage_ThreadError,
	eMessage_ThreadFail,
	eMessage_ThreadException,

	// �v���p�e�B�n
	eMessage_PropetyChange,
	eMessage_PropetyCmbChange,
	eMessage_AutoClear,
};

enum {
	grapf1,
	grapf2,
	grapf3,
	grapf4,
	grapf5,
	grapf6,
	grapf7,
	grapf8,
	grapf9,

	grapfmax,
};

typedef enum {
	Line,
	Dots,
	LineAndDots,
}DrawMode;

typedef enum {
	Small,
	Medium,
	Large,
}DotSize;

typedef enum {
	second,
	minute,
	hour,
}AxisUnit;

typedef enum {
	LScale,
	RScale,
	VScaleKind_Max
}VScaleKind;

typedef enum {
	LimitWarning,	// ���[�j���O
	LimitAlarm,		// �A���[��
	LimitKind_Max
}LimitKind;

typedef enum {
	LimitUpper,		// ���
	LimitLower,		// ����
}LimitRange;

typedef enum {
	LimitAbove,		// �`������
	LimitBelow,		// �`��艺���
}LimitStatus;

typedef struct {
	CTime	time;
	ULONG	state;
} stLimitState;

#define	MAKE_LIMIT_STATE(k, r, s)	(((s) << 16) | ((r) << 8) | ((k) << 0))
#define	GET_LIMIT_KIND(state)		(((state) >>  0) & 0xff)
#define	GET_LIMIT_RANGE(state)		(((state) >>  8) & 0xff)
#define	GET_LIMIT_STATUS(state)		(((state) >> 16) & 0xff)

typedef enum {
	DifferNone,		// �Ȃ�
	DifferSameData,	// ����f�[�^
	DifferSameTime	// ���ꎞ��
}DifferMode;

#define	DIFFER_ADD	10
#define	DIFFER_TEXT	50

typedef struct {
	bool	Auto;
	double	VScalLMax;
	double	VScalLMin;
	double	VScalRMax;
	double	VScalRMin;
	long	VSCalItvl;
} stVScale;

#ifdef	_TEST_MAXDATA
typedef enum {
	m1,
	m10,
	m30,
	h1,
	h3,
	autoscale
}HScale;

static const CString strHScale[] = {
	_T("1��"),
	_T("10��"),
	_T("30��"),
	_T("1����"),
	_T("3����"),
	_T("�I�[�g"),
	_T(""),
};
#else	// 	_TEST_MAXDATA
typedef enum {
	m1,
	m10,
	m30,
	h1,
	h3,
	h6,
	h12,
	h24,
	h48,
	h96,
	h120,
	h240,
	h480,
	h960,
	h1200,
	h2400,
	h4800,
	autoscale
}HScale;

static const CString strHScale[] = {
	_T("1��"),
	_T("10��"),
	_T("30��"),
	_T("1����"),
	_T("3����"),
	_T("6����"),
	_T("12����"),
	_T("24����"),
	_T("48����"),
	_T("96����"),
	_T("120����"),
	_T("240����"),
	_T("480����"),
	_T("960����"),
	_T("1200����"),
	_T("2400����"),
	_T("4800����"),
	_T("�I�[�g"),
	_T(""),
};
#endif	// 	_TEST_MAXDATA

static const CString strDrawMode[] = {
	_T("Line"),
	_T("Dots"),
	_T("L&D"),
	_T(""),
};

static COLORREF colordef[] =
{
	RGB(0, 0, 255),		// blue
	RGB(255, 0, 0),		// red
	RGB(255, 0, 255),	// magenta
	RGB(0, 255, 0),		// green
	RGB(0, 255, 255),	// cyan
	RGB(255, 255, 0),	// yellow
	RGB(255, 255, 255),	// white
	RGB(255, 165, 0),	// orange
	RGB(165, 42, 42),	// brown
	RGB(50, 205, 50),	// lime
	NULL
};

typedef struct {
	CString		Auto;
	CString		startDate;
	double		VScalLMax;
	double		VScalLMin;
	double		VScalRMax;
	double		VScalRMin;
	long		VSCalItvl;
	CString		HScaleU;
	CString		HScaleL;
	CString		ViewMode;
	CString		Display;
	CString		DrawKind;
	CString		DotSize;
	CString		SelVScale;
	CString		dataUnit;
	double		LimitWarningUpper;
	double		LimitWarningLower;
	double		LimitAlarmUpper;
	double		LimitAlarmLower;
} stPropData;

static const stPropData initPropData = {
	_T("On"),
	_T(""),
	100,
	-100,
	50,
	-50,
	5,
	_T("24����"),
	_T("1����"),
	_T("���A�����[�h"),
	_T("On"),
	_T("Line"),
	_T("Small"),
	_T("��"),
	_T(""),
	0,
	0,
	0,
	0,
};

//=============================================================================
//	�Ď��f�[�^
//=============================================================================

enum eSpaceDataStatus
{
	SDS_INVALID	= 0x0000,

	SDS_VALID	= 0x0001,

	SDS_LEAP	= 0x0002,
	SDS_LEAPMIN	= 0x0004,
	SDS_LEAPMAX	= 0x0008
};

#pragma pack(1)
typedef struct {
	USHORT	cnt;
	double	max;
	double	min;
} stMMPair;

typedef struct  {
	SYSTEMTIME	systime;	// ����
	USHORT		status;		// �X�e�[�^�X
	stMMPair	mmPair;		// �s�[�N���k
} stSpaceData;
#pragma pack()

typedef struct {
	unsigned __int64	start_pos;	// �`��ʒu
	unsigned __int64	data_num;	// �`��_��
	unsigned __int64	update_pos;	// �X�V�J�n�ʒu
	bool				change_flg;	// 100%��90%�ւ̕ύX�t���O
} stDrawInfo;

typedef struct {
	DWORD	id;			// �f�[�^ID
	CString	name;		// �f�[�^����
	bool	valid;		// �f�[�^�擾�Ώۃt���O
} stSelectData;

typedef struct {
	CString	name;		// �f�[�^����
	stSpaceData	data;	// �f�[�^
} stQueData;

enum eStatus{
	eStatus_None,		// �f�[�^����
	eStatus_Enable,		// �f�[�^�L��
	eStatus_Disable,	// �f�[�^�����i�Ⴆ�΃f�[�^�������j
};

class CSpaceData
{
public:
	CSpaceData()
	{
		value.clear();
		value_for_copy.clear();
		data_max = -DBL_MAX;
		data_min = DBL_MAX;
	};
	virtual ~CSpaceData(){};

public:
	DWORD			dataid;		// �f�[�^ID
	DWORD			kind;		// �f�[�^���
	double			data_max;	// �ő�l
	double			data_min;	// �ŏ��l
	__int64			totalSize;	// �L���[�폜�O�̃g�[�^���T�C�Y
	deque<stSpaceData>	value;	// ���g�����h�f�[�^�L���[
	vector<stSpaceData>	value_for_copy;	// �R�s�[�p�g�����h�f�[�^
};

typedef struct {
	DWORD		dataid;		// �f�[�^ID
	SYSTEMTIME	systime;	// ����
} stDataIdTime;

//=============================================================================
//	�X�P�[�����
//=============================================================================

typedef struct {
	unsigned short		cmp_rate;		// ���k��
	unsigned __int64	all_data;		// �S�_��
	unsigned __int64	hld_data;		// �ێ��_��
	unsigned long		dsp_data;		// �\���_��
	bool				flg_auto;		// AUTO�p�X�P�[��
} stScaleInfo;

#ifdef	_TEST_MAXDATA
const stScaleInfo	tblRealScaleInfo[]={
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	  61,	false	},	// �P��
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	 601,	false	},	// �P�O��
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	1801,	false	},	// �R�O��
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	3601,	false	},	// �P����
	{	   3,	MAX_ALLDATA_SIZE /    3	/* 5760000*/,	3600,	3601,	false	},	// �R����
};
#else	// _TEST_MAXDATA
const stScaleInfo	tblRealScaleInfo[]={
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	  61,	false	},	// �P��
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	 601,	false	},	// �P�O��
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	1801,	false	},	// �R�O��
	{	   1,	MAX_ALLDATA_SIZE		/*17280000*/,	3601,	3601,	false	},	// �P����
	{	   3,	MAX_ALLDATA_SIZE /    3	/* 5760000*/,	3601,	3601,	false	},	// �R����
	{	   6,	MAX_ALLDATA_SIZE /    6	/* 2880000*/,	3601,	3601,	false	},	// �U����
	{	  12,	MAX_ALLDATA_SIZE /   12	/* 1440000*/,	3601,	3601,	false	},	// �P�Q����
	{	  24,	MAX_ALLDATA_SIZE /   24	/*  720000*/,	3601,	3601,	false	},	// �Q�S����
	{	  48,	MAX_ALLDATA_SIZE /   48	/*  360000*/,	3601,	3601,	true	},	// �S�W����
	{	  96,	MAX_ALLDATA_SIZE /   96	/*  180000*/,	3601,	3601,	true	},	// �X�U����
	{	 120,	MAX_ALLDATA_SIZE /  120	/*  144000*/,	3601,	3601,	true	},	// �P�Q�O����
	{	 240,	MAX_ALLDATA_SIZE /  240	/*   72000*/,	3601,	3601,	true	},	// �Q�S�O����
	{	 480,	MAX_ALLDATA_SIZE /  480	/*   36000*/,	3601,	3601,	true	},	// �S�W�O����
	{	 960,	MAX_ALLDATA_SIZE /  960	/*   18000*/,	3601,	3601,	true	},	// �X�U�O����
	{	1200,	MAX_ALLDATA_SIZE / 1200	/*   14400*/,	3601,	3601,	true	},	// �P�Q�O�O����
	{	2400,	MAX_ALLDATA_SIZE / 2400	/*    7200*/,	3601,	3601,	true	},	// �Q�S�O�O����
	{	4800,	MAX_ALLDATA_SIZE / 4800	/*    3600*/,	3600,	3601,	true	}	// �S�W�O�O����
};
#endif	// _TEST_MAXDATA

#define	NUM_REAL_SCALE_INFO_TABLE		(sizeof(tblRealScaleInfo) / sizeof(tblRealScaleInfo[0]))

/*
�ݔ�������c�a
*/

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
#define CARVTBL_ID		0x010a		/* �J�[�u�ϊ���`�h�c */
#define TIMECVT_ID		0x010b		/* �����ϊ���`�h�c */
#define OBSSTS_ID		0x010c		/* �X�e�[�^�X��`�h�c */
#define CAL_ID			0x0112		/* �����ڌv�Z��`�h�c */
#define	EQMON_INF_ID	0x0121		/* "inf":�t������`���Ӗ�����h�c */
#define ENDOFOBS_ID 	-1			/* �X�̃R�}���h�̒�`�̏I���h�c */
#define L_OFFSET        6

#define	CTLNAME_LEN	28	/* �ݔ����䖼�̍ő咷+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12	/* �ݔ����̍ő咷+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* ���䍀��ID�ő咷+1 */
#define	CHKVALDBL_NUM	2	/* ���ؒl�F�����^�̏ꍇ�̔z�� */
#define	CHKVALSTR_LEN	( CHKVALDBL_NUM * ( sizeof(double) / sizeof(char)  ))
//#define CHKVALSTR_LEN   ( 6 * CHKVALDBL_NUM * ( sizeof(double) /sizeof(char)  ))

/*
*******************************************************************************
* �ݔ��Ď����̏��
*******************************************************************************
*/
struct obsname_t {	/* ���̃e�[�u���͑S�Ď����ڂɂ��đ��݂��� */
	long   l_id;	/* �f�[�^����: �萔=OBSNAME_ID�l */
	long   l_len;	/* �f�[�^��  : sizeof( struct obsname_t ) */
	long   l_no;	/* �Ď����ڔԍ��i�R���p�C�����V�[�P���V�����ɕt������B�j */
	char   sz_obsname[CTLID_LEN]; 	/* �Ď����ڂh�c: �ݔ���.�Ď����ږ� */
	long   l_inflg; /* �f�[�^�L���t���O 1=�L��  ���P */
	long   l_grmd;  /* �X�e�[�^�X�t���O 1=�L��,2=���������� ���P */
	long   l_data;  /* �ϊ����ʁi�����j         ���P */
	double d_data;  /* �ϊ����ʁi�����j         ���P */
	char   sz_sts_name[64]; /* �ϊ����ʃX�e�[�^�X������ */

	/* �ȉ���64bit�������x�Ή��g���̈� */

	long double ld_data;/* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x ���P */

	long   l_bitinfo;   /* �����r�b�g��:�������o�f�[�^�̃r�b�g��:1..64
						==0:�������o�ł͂Ȃ�   ���P */
	long   l__rfu__;    /* ���g�p�Fpadding�o�C�g */

	long   l_cnvResType;	/* �ϊ����ʃ^�C�v�i�r�b�g���сj */
};

/*
*******************************************************************************
* ���o��`���
*******************************************************************************
*/
typedef struct pwb_tbl_t {	/* ���o��`�e�[�u����ɑ��݂���B */
	long l_pid;	/* ���u�ԍ� */
	long l_word;	/* ���[�h�ʒu */
	long l_s_bit;  /* �J�n�r�b�g�ʒu */
	long l_e_bit;  /* �I���r�b�g�ʒu */
} PWB_T;

/*
*******************************************************************************
* �e���o��`���
*******************************************************************************
*/
struct ext_tbl_t{	/* �e���o��`��� */
	long l_id;	/* �f�[�^����:
				EXT_ID or EXTS_ID or
				EXTBCD_ID or EXTBCDS_ID or
				EXTAVG_ID or EXTAVGS_ID or
				EXTCHAR_ID or EXTCHARDEC or
				EXTDBL or EXTFLT */
	long l_len;	/* �f�[�^��  : sizeof( struct ext_tbl_t ) */
	long l_pw_cnt;	/* ���o��`�� */
	long l_rsv;	/* �T�C�Y���� */
	pwb_tbl_t s_pwb[10];/* ���o��`��� */
};

/*
*******************************************************************************
* �X�e�[�^�X�ƍ���`
*******************************************************************************
*/
typedef struct chksts_tbl_t {	/* �X�e�[�^�X�ƍ���`�e�[�u�� */
	long l_val;	/* ���ڒl */
	long l_rsv;	/* �T�C�Y���� */
	char sz_sts[CHKVALSTR_LEN];/* �X�e�[�^�X������ */
} C_STS;

/*
*******************************************************************************
* �X�e�[�^�X���
*******************************************************************************
*/
struct obssts_tbl_t {	/* �X�e�[�^�X���e�[�u�� */
	long l_id;	/* �f�[�^����: �萔=OBSSTS_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct obssts_tbl_t ) */
	long l_sts_cnt; /* �X�e�[�^�X���ڐ� */
	long l_rsv;	/* �T�C�Y���� */
	char sz_i_sts[CHKVALSTR_LEN];/* �����l�X�e�[�^�X������ */
	chksts_tbl_t chk_t[32]; /* �X�e�[�^�X�ϊ����� */
};

struct res_t { /*���L��������̃e�[�u���\��*/
	long  l_id;
	long  l_len;
	char  buf[1];
};

