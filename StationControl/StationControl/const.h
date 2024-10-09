//******************************************************************************
// @file	const.h
// @brief	��`�w�b�_�[
//******************************************************************************
#pragma once

using namespace std;

#ifdef _DEBUG
// 54m��64m���Ńf�o�b�O�m�F
//#define _DEBUG_KANSHI64M 1			// 64m�̕ߑ��󋵃X�e�[�^�X�̊Ď��E�����54m�Ŏg��
//#define _DEBUG_PIPEUSE64M 1			// 64m�̃p�C�v��54m�Ŏg��
//#define _DEBUG_PIPEUSE34M 1			// 34m�̃p�C�v��54m�Ŏg��
// ��M�f�[�^�Ɍ��ݎ������X�^���v
//#define	DemoTimeStamp		1
#endif

#define _GREAT_XSSPA_ISO	1			// 54m X-SSPA ISO�Ή�


static const CString mFaBrowserApp = _T("FAPanelBrowser.exe");
static const CString mTrendGraphApp = _T("TrendGraph.exe");

#define DSN_DBSHM_IPC		0xa901	/* ���ʃV�F�A�[�h������IPC�L�[�l */

// �J���[��`

#define COLOR_WHITE			RGB(255, 255, 255)
#define COLOR_LIGHTGLEY		RGB(211, 211, 211)
#define COLOR_LIGHTBLUE		RGB(173, 216, 230)
#define COLOR_MAGENTA		RGB(255, 0, 255)
#define COLOR_ORANGE		RGB(255, 165, 0)
#define COLOR_GREEN			RGB(80, 159, 0)
#define COLOR_YELLOW		RGB(255, 255, 0)
#define COLOR_BLUE			RGB(0, 176, 240)

#define COLOR_OPE_OPERATION		COLOR_LIGHTGLEY
#define COLOR_OPE_UPLINK		COLOR_YELLOW
#define COLOR_OPE_RNG			COLOR_ORANGE
#define COLOR_OPE_CMD			COLOR_BLUE
#define COLOR_OPE_TLMS			COLOR_LIGHTGLEY
#define COLOR_OPE_TLMX			COLOR_LIGHTGLEY
#define COLOR_OPE_TLMKa			COLOR_LIGHTGLEY
#define COLOR_OPE_INIT			COLOR_GREEN
#define COLOR_OPE_RDCK			COLOR_LIGHTBLUE
#define COLOR_OPE_ANTZENITH		COLOR_ORANGE
#define COLOR_OPE_ANTSDBY		COLOR_ORANGE

#define COLOR_REMOTE		RGB(0, 0, 128)		// �����[�g��I��
#define COLOR_REMOTES		RGB(255, 165, 0)	// �����[�g�I��
#define COLOR_D_REMOTE		RGB(192, 192, 255)	// �����[�g��I��(Disable)
#define COLOR_D_REMOTES		RGB(255, 237, 202)	// �����[�g�I��(Disable)
#define COLOR_LOCAL			RGB(0, 0, 0)		// ���[�J��
#define COLOR_CTRLIN		RGB(128, 128, 128)	// ���䒆
#define COLOR_ALARM			RGB(255, 0, 0)		// �A���[��

enum eViewType1{
	eViewType1_Trend,
	eViewType1_EquipmentStatus,
	eViewType1_OperationHist,
	eViewType1_ControlHist,
	eViewType1_EquipmentHist,
	eViewType1_EventHist,
	eViewType1_AlarmList,
	eViewType1_OperationSearch,
	eViewType1_ControlSearch,
	eViewType1_EquipmentSearch,
	eViewType1_EventSearch,
	eViewType1_Max,
	eViewType5_EqOperation1,
	eViewType5_EqOperation2,
	eViewType5_EqOperation3,
	eViewType5_EqOperation4,
	eViewType5_EqOperation5,
	eViewType5_OperationHist,
	eViewType5_ControlHist,
	eViewType5_EquipmentHist,
	eViewType5_EventHist,
	eViewType5_AlarmList,
	eViewType5_OperationSearch,
	eViewType5_ControlSearch,
	eViewType5_EquipmentSearch,
	eViewType5_EventSearch,
	eViewType5_Max,

	eViewTypeTool_TimeLine,

	eViewType_Max,
};

static const TCHAR* mViewTitle[]{
	_T("�ߑ��󋵃O���t"),
	_T("�ݔ����"),
	_T("�^�p����\��"),
	_T("���䗚��\��"),
	_T("�ݔ���ԗ���\��"),
	_T("�C�x���g����\��"),
	_T("�ݔ��A���[���󋵈ꗗ"),
	_T("�^�p����\��"),
	_T("���䗚��\��"),
	_T("�ݔ���ԗ���\��"),
	_T("�C�x���g����\��"),
	_T(""),
	_T("�ݔ��E�^�p��"),
	_T("�ݔ��E�^�p��"),
	_T("�ݔ��E�^�p��"),
	_T("�ݔ��E�^�p��"),
	_T("�ݔ��E�^�p��"),
	_T("�^�p����\��"),
	_T("���䗚��\��"),
	_T("�ݔ���ԗ���\��"),
	_T("�C�x���g����\��"),
	_T("�ݔ��A���[���󋵈ꗗ"),
	_T("�^�p����\��"),
	_T("���䗚��\��"),
	_T("�ݔ���ԗ���\��"),
	_T("�C�x���g����\��"),
	_T(""),
	_T("�^�p�v��^�C�����C���\��"),
	_T(""),
};

enum eExecuteMode{
	eExecuteMode_Control,
	eExecuteMode_Monitor,
};

enum eLimitedeMode{
	eLimitedMode_Disable,	//���䃂�[�h�ł��A�������łȂ�
	eLimitedMode_Enable,	//���䃂�[�h�ł��A������
};

enum eStationMode{
	eStation_1,
	eStation_5,
	eStation_10,
};
static const TCHAR* mStationModeString[]{
	_T("�P�ǃ��[�h"),
	_T("�S�ǃ��[�h"),
	_T("�S�ǃ��[�h"),
};

enum eStation{
	eStation_Usuda64,
	eStation_Usuda642,
	eStation_Uchinoura34,
	eStation_Uchinoura20,
	eStation_MAX,
};
static const TCHAR* mStationString[]{
	_T("�P�c64m��"),
	_T("�P�c54m��"),
	_T("���V�Y34m��"),
	_T("���V�Y20m��"),
};
static const TCHAR* mStationStringEx = _T("�}�g�ǉ^�p�ǐ�");
static const TCHAR* mHistoryStationString[]{
	_T("�P�c64m��"),
	_T("���V�Y34m��"),
	_T("���V�Y20m��"),
	_T("�P�c54m��"),
};
static const TCHAR* mStationSimpleString[]{
	_T("UDSC64"),
	_T("UDSC54"),
	_T("USC34"),
	_T("USC20"),
};
static const TCHAR* mCtrlNameString[]{
	_T("tzopr1"),
	_T("tzopr1"),
	_T("tzopr1"),
	_T("tzopr1"),
};
static const TCHAR* mNetworkDrive[]{
	_T("U:\\"),	// �P�c64m
	_T("X:\\"),	// �P�c54m
	_T("V:\\"),	// ���V�Y34m
	_T("W:\\"),	// ���V�Y20m
};
static const TCHAR* mHostNameUsuda = _T("u5opr1W1");	//�z�X�g�� �P�c�F����Ԋϑ���(�P�c64m�ǎɓ�)
static const COLORREF mStationColor[]{
	RGB(195, 195, 195),
	RGB(255, 193, 164),
	RGB(255, 195, 255),
	RGB(153, 217, 255),
};
static const TCHAR* mStationCtrlDir[]{
	_T("/sos/opr/data/udsc64/log/dump/"),
	_T("/sos/opr/data/udsc54/log/dump/"),
	_T("/sos/opr/data/usc34/log/dump/"),
	_T("/sos/opr/data/usc20/log/dump/"),
};

// �ǌŒ�I��ԍ�
enum eFixedSelectStation{
	eFixedSelectStation_Usuda64 = 10,
	eFixedSelectStation_Usuda642,
	eFixedSelectStation_Uchinoura34,
	eFixedSelectStation_Uchinoura20,
	eFixedSelectStation_MAX,
};

// �ǂ̕\����
static const UINT mStationViewOrder[]{
	eStation_Uchinoura20,
	eStation_Uchinoura34,
	eStation_Usuda64,
	eStation_Usuda642,
	eStation_MAX,
};
enum eStationCnv{
	eStationCnv_Uchinoura20,
	eStationCnv_Uchinoura34,
	eStationCnv_Usuda64,
	eStationCnv_Usuda642,
	eStationCnv_MAX,
};
static const UINT mStationViewCnv[]{
	eStationCnv_Usuda64,
	eStationCnv_Usuda642,
	eStationCnv_Uchinoura34,
	eStationCnv_Uchinoura20,
	eStationCnv_MAX,
};

// �擾��
enum eGetFrom{
	eGetFrom_ENP,
	eGetFrom_STATION,
};
static const CString mGetFromString[]{
	_T("ENP"),
	_T("�Ǌǐ�"),
	_T("STTN"),
};

// �^�p���
enum eOperationKind{
	eOperationKind_StationOccupancy,
	eOperationKind_Plan,
	eOperationKind_Vlbi,
	eOperationKind_Rdychk,
};
static const CString mOperationKindString[]{
	_T("�ǐ�L"),
	_T("���^�p"),
	_T("VLBI"),
	_T("���f�B�l�X�`�F�b�N"),
};

//=============================================================================
//	�ݔ��^�p��
//=============================================================================
// �ݔ��󋵍��ڒ�`
// 64m
#define MONI64M_ANT		"CTRL.ANT_BLOCK"
#define MONI64M_CMDOUT		"CTRL.CMDOUT_BLOCK"
#define MONI64M_CMDOUT3	"CTRL.CMDOUT3_BLOCK"
#define MONI64M_S_CAL		"CTRL.S-CAL_BLOCK"
#define MONI64M_S_DC		"CTRL.SS_DCSYN2_BLOCK"
#define MONI64M_S_RXRF		"CTRL.S_RXRF_BLOCK"
#define MONI64M_S_TLMIN	"CTRL.S_TLMIN_BLOCK"
#define MONI64M_S_TXFCONV	"CTRL.S_TXFCONV_BLOCK"
#define MONI64M_S_TXRF		"CTRL.S_TXRF_BLOCK"
#define MONI64M_S_UC		"CTRL.S_TXFCONV_BLOCK"
#define MONI64M_SS_TCRDEM	"CTRL.SS_TCRDEM_BLOCK"
#define MONI64M_SS_TCRMOD	"CTRL.SS_TCRMOD_BLOCK"
#define MONI64M_WEATHER	"CTRL.WEATHER_BLOCK"
#define MONI64M_X_CAL		"CTRL.X-CALOUT_BLOCK"
#define MONI64M_X_CMDOUT2	"CTRL.X_CMDOUT2_BLOCK"
#define MONI64M_X_DC		"CTRL.DC_BLOCK"
#define MONI64M_X_DCSYN	"CTRL.X-RXCONV_BLOCK"
#define MONI64M_X_HPA		"CTRL.X-KLY-TX1_BLOCK"
#define MONI64M_X_LNA		"CTRL.LNA3_BLOCK"
#define MONI64M_X_RX2		"CTRL.X_RX2_BLOCK"
#define MONI64M_X_RXRF		"CTRL.X-RXCONV_BLOCK"
#define MONI64M_X_TCRMOD	"CTRL.XX_TCRMOD_BLOCK"
#define MONI64M_X_TLMIN	"CTRL.X_PKTDEM_BLOCK"
#define MONI64M_X_TLMIN2	"CTRL.X_TLMIN2_BLOCK"
#define MONI64M_X_TXRF		"CTRL.X-TXIF_BLOCK"
#define MONI64M_XR_RARR	"CTRL.XX_RRNG_BLOCK"
#define MONI64M_XS_DC		"CTRL.XS_DC_BLOCK"
#define MONI64M_XX_RARR	"CTRL.XX_RARR_BLOCK"
#define MONI64M_XX_TCRDEM	"CTRL.DSNTCR_BLOCK"
#define MONI64M_XX_TCRMOD	"CTRL.DSNTCR_BLOCK"
#define MONI64M_XX_TCRRARR	"CTRL.DSNTCR_BLOCK"
#define MONI64M_GREEN		"COLOR_GREEN"
//#define MONI64M_szopr1	�Œ�(��)
//#define MONI64M_tzopr1	�Œ�(��)
//#define MONI64M_tzopr2	�Œ�(��)
//#define MONI64M_u6ctrl	�Œ�(��)
//#define MONI64M_u6opr1	�Œ�(��)

// 54m
#define MONI54M_ANT		"CTRL.ANT_BLOCK"
#define MONI54M_CMDOUT		"CTRL.CMDOUT_BLOCK"
#define MONI54M_CMDOUT3	"CTRL.CMDOUT3_BLOCK"
#define MONI54M_S_CAL		"CTRL.S-CAL_BLOCK"
#define MONI54M_S_DC		"CTRL.SS_DCSYN2_BLOCK"
#define MONI54M_S_RXRF		"CTRL.S_RXRF_BLOCK"
#define MONI54M_S_TLMIN	"CTRL.S_TLMIN_BLOCK"
#define MONI54M_S_TXFCONV	"CTRL.S_TXFCONV_BLOCK"
#define MONI54M_S_TXRF		"CTRL.S_TXRF_BLOCK"
#define MONI54M_S_UC		"CTRL.S_TXFCONV_BLOCK"
#define MONI54M_SS_TCRDEM	"CTRL.SS_TCRDEM_BLOCK"
#define MONI54M_SS_TCRMOD	"CTRL.SS_TCRMOD_BLOCK"
#define MONI54M_WEATHER	"CTRL.WEATHER_BLOCK"
#define MONI54M_X_CAL		"CTRL.X-CAL_BLOCK"
#define MONI54M_X_CMDOUT2	"CTRL.X_CMDOUT_BLOCK"
#define MONI54M_X_DC		"CTRL.X-DC_BLOCK"
#define MONI54M_X_DCSYN	"CTRL.X-RXCONV_BLOCK"
#define MONI54M_X_HPA		"CTRL.X-KLY-TX1_BLOCK"		// SSPA�̏ꍇ��CTRL.SSPA_BLOCK
#define MONI54M_SSPA		"CTRL.SSPA_BLOCK"
#define MONI54M_X_LNA		"CTRL.X-LNA_BLOCK"
#define MONI54M_X_RX2		"CTRL.X_RX2_BLOCK"
#define MONI54M_X_RXIF		"CTRL.X-RX_IF_BLOCK"
#define MONI54M_X_TCRMOD	"CTRL.XX_TCRMOD_BLOCK"
#define MONI54M_X_TLMIN	"CTRL.X_PKTDEM_BLOCK"
#define MONI54M_X_TLMIN2	"CTRL.XKa_TLMIN_BLOCK"
#define MONI54M_X_TXRF		"CTRL.X-TXIF_BLOCK"
#define MONI54M_XR_RARR	"CTRL.XX_RRNG_BLOCK"
#define MONI54M_XS_DC		"CTRL.XS_DC_BLOCK"
#define MONI54M_XX_RARR	"CTRL.XX_RARR_BLOCK"
#define MONI54M_XX_TCRDEM	"CTRL.DSNTCR_BLOCK"
#define MONI54M_XX_TCRMOD	"CTRL.DSNTCR_BLOCK"
#define MONI54M_XX_TCRRARR	"CTRL.DSNTCR_BLOCK"
#define MONI54M_GREEN		"COLOR_GREEN"

#define MONI54M_X_TXIF	"CTRL.X-TX_IF_BLOCK"		// X-TXIF(54m)
#define MONI54M_X_UC	"CTRL.X-UC_BLOCK"			// X-UC(54m)
#define MONI54M_DSN_MODEM	"CTRL.DSNTCR_BLOCK"		// DSN-MODEM(54m)
#define MONI54M_KA_LNA	"CTRL.Ka-LNA_BLOCK"			// Ka-LNA(54m)
#define MONI54M_KA_DC	"CTRL.Ka-DC_BLOCK"			// Ka-DC(54m)
#define MONI54M_REF_DIV	"CTRL.REF_DIV_BLOCK"		// REF-DIV(54m)	
#define MONI54M_X_TRANS	"CTRL.X-TRANS_BLOCK"		// X-TRANS(54m)	
#define MONI54M_TRANS_LO	"CTRL.TRANS_LO_BLOCK"	// TRANS-LO(54m)
#define MONI54M_KA_CAL	"CTRL.Ka-CAL_BLOCK"			// Ka-CAL(54m)
#define MONI54M_KA_TRANS	"CTRL.Ka-TRANS_BLOCK"	// Ka-TRANS(54m)
#define MONI54M_CVCF	"CTRL.CVCF_BLOCK"			// CVCF(54m)

//#define MONI54M_szopr1	�Œ�(��)
//#define MONI54M_tzopr1	�Œ�(��)
//#define MONI54M_tzopr2	�Œ�(��)
//#define MONI54M_u6ctrl	�Œ�(��)
//#define MONI54M_u6opr1	�Œ�(��)

// 34m
#define MONI34M_ANT		"CTRL.ANT_BLOCK"
//#define MONI34M_CTRL_1	�Œ�(��)
//#define MONI34M_CTRL_2	�Œ�(��)
//#define MONI34M_CTRL_3	�Œ�(��)
//#define MONI34M_CTRL_4	�Œ�(��)
#define MONI34M_DSNTCR_DEM	"CTRL.DSNTCR_BLOCK"
#define MONI34M_DSNTCR_MOD	"CTRL.DSNTCR_BLOCK"
#define MONI34M_DSNTCR_RARR	"CTRL.DSNTCR_BLOCK"
//#define MONI34M_QD	�Œ�(��)
#define MONI34M_S_CAL		"CTRL.S_CAL_BLOCK"
#define MONI34M_S_CMDOUT	"CTRL.CMDOUT_BLOCK"
#define MONI34M_S_RXIFSW	"CTRL.S_RXIFSW_BLOCK"
#define MONI34M_S_RXSYN	"CTRL.S_RXSYN_BLOCK"
#define MONI34M_S_TLMIN	"CTRL.S_TLMIN_BLOCK"
#define MONI34M_S_TRKRX	"CTRL.S_RX_BLOCK"
#define MONI34M_S_TX		"CTRL.S_TX_BLOCK"
#define MONI34M_S_TXIFSW	"CTRL.S_TXIFSW_BLOCK"
#define MONI34M_S_TXSYN	"CTRL.S_TXSYN_BLOCK"
//#define MONI34M_S/SX�ǔ���1	�Œ�(��)
//#define MONI34M_S/SX�ǔ���2	�Œ�(��)
#define MONI34M_SS_TCRDEM	"CTRL.SS_TCRDEM_BLOCK"
#define MONI34M_SS_TCRMOD	"CTRL.SS_TCRMOD_BLOCK"
#define MONI34M_SS_TCRRARR	"CTRL.SS_TCRDEM_BLOCK"
//#define MONI34M_STS_1	�Œ�(��)
#define MONI34M_SX_ACQRX	"CTRL.S-ACQ_TRKRX_BLOCK"
#define MONI34M_SX_ACQRXSYN	"CTRL.SX_ACQRXSYN_BLOCK"
#define MONI34M_TIME		"CTRL.TIME_BLOCK"
#define MONI34M_TX_POL		"CTRL.SX_TXPW_BLOCK"
#define MONI34M_WEATHER	"CTRL.WEATHER_BLOCK"
#define MONI34M_X_CAL		"CTRL.X-CALOUT_BLOCK"
#define MONI34M_X_CMDOUT2	"CTRL.X_CMDOUT2_BLOCK"
#define MONI34M_X_HRX_DEM	"CTRL.X_QPSKRX_BLOCK"
#define MONI34M_X_HRX_MOD	"CTRL.X_QPSKRX_BLOCK"
#define MONI34M_X_HRXTLMIN	"CTRL.X_QPSKTLMIN_BLOCK"
#define MONI34M_X_IFSW		"CTRL.X-RF_IF_BLOCK"
#define MONI34M_X_RX		"CTRL.X_RX_BLOCK"
#define MONI34M_X_RXSYN	"CTRL.X-ACQ_TRKRX_BLOCK"
#define MONI34M_X_TRKRX		"CTRL.X-TRKRX_BLOCK"
#define MONI34M_X_TLMIN2	"CTRL.X_TLMIN2_BLOCK"
#define MONI34M_X_TX		"CTRL.X-UL_CNT_BLOCK"
#define MONI34M_X_TXSYN	"CTRL.X-UC_BLOCK"
//#define MONI34M_X/X�ǔ���	�Œ�(��)
//#define MONI34M_�u���b�N�}	�Œ�(��)
#define MONI34M_GREEN		"COLOR_GREEN"

// 20m
#define MONI20M_ANT		"CTRL.ANT_BLOCK"
#define MONI20M_CMDOUT1	"CTRL.CMDOUT1_BLOCK"
#define MONI20M_CMDOUT2	"CTRL.CMDOUT2_BLOCK"
//#define MONI20M_CTRL_1	�Œ�(��)
//#define MONI20M_CTRL_2	�Œ�(��)
//#define MONI20M_CTRL_3	�Œ�(��)
//#define MONI20M_QD	�Œ�(��)
//#define MONI20M_QD2	�Œ�(��)
#define MONI20M_S_ACQDET	"CTRL.S_ACQDET_BLOCK"
#define MONI20M_S_ACQTCRDEM	"CTRL.S_ACQTCRDEM_BLOCK"
#define MONI20M_S_CALRF		"CTRL.S_CALRF_BLOCK"
#define MONI20M_S_DCSYN		"CTRL.SS_DCSYN_BLOCK"
#define MONI20M_S_RXRF		"CTRL.S-RXRF_SUM_BLOCK"
#define MONI20M_S_TCRDEM	"CTRL.SS_TCRDEM_BLOCK"
#define MONI20M_S_TCRDET	"CTRL.SS_TCRDET_BLOCK"
#define MONI20M_S_TCRRARR	"CTRL.SS_TCRDEM_BLOCK"
#define MONI20M_S_TLMIN		"CTRL.S_TLMIN_BLOCK"
#define MONI20M_S_TXFCONV	"CTRL.S-RFOUT_BLOCK"
#define MONI20M_S_TXRF		"CTRL.S_TXRF_BLOCK"
#define MONI20M_S_UCSYN		"CTRL.S_UCSYN_BLOCK"
#define MONI20M_SS_TCRMOD	"CTRL.SS_TCRMOD_BLOCK"
#define MONI20M_SS_TRANSSYN	"CTRL.SS_TRANSSYN_BLOCK"
//#define MONI20M_STS_1	�Œ�(��)
#define MONI20M_SX_ACQDCSYN	"CTRL.S_ACQDCSYN_BLOCK"
#define MONI20M_SX_TCRMOD	"CTRL.SX_TCRMOD_BLOCK"
#define MONI20M_SX_TRANSSYN	"CTRL.SX_TRANSSYN_BLOCK"
#define MONI20M_WEATHER		"CTRL.WEATHER_BLOCK"
#define MONI20M_X_CALRF_UC	"CTRL.X-CALRFOUT_BLOCK"
#define MONI20M_X_DCSYN		"CTRL.XS_DCSYN_BLOCK"
#define MONI20M_X_HRX_DEM	"CTRL.X_QPSKRXDEM_BLOCK"
#define MONI20M_X_HRX_MOD	"CTRL.X_QPSKRXDEM_BLOCK"
#define MONI20M_X_HRXTLMIN	"CTRL.X_QPSKTLMIN_BLOCK"
#define MONI20M_X_TCRDEM	"CTRL.SX_TCRDEM_BLOCK"
#define MONI20M_X_TCRDET	"CTRL.SX_TCRDET_BLOCK"
#define MONI20M_X_TCRRARR	"CTRL.SX_TCRRARR_BLOCK"
#define MONI20M_X_TLMDEC	"CTRL.X_TLMDEC_BLOCK"
#define MONI20M_X_TLMIN		"CTRL.X_TLMIN_BLOCK"
#define MONI20M_X_TLMSIM	"CTRL.X_TLMSIM_BLOCK"
//#define MONI20M_�u���b�N�}	�Œ�(��)
//#define MONI20M_�ǔ���	�Œ�(��)
//#define MONI20M_�^�p��	�Œ�(��)
#define MONI20M_GREEN		"COLOR_GREEN"

#define MONIEQUIPMENT_GREEN		"COLOR_GREEN"

static const CString mEquipmentStatusString[4][9][6] = {
	{
		// 64m
		{ _T("ANT"), _T("S-TXRF"), _T("S-TXFCONV"), _T("S-UC"), _T("SS-TCRMOD"), _T("CMDOUT") },
		{ _T(""), _T("S-RXRF"), _T(""), _T("S-DC"), _T("SS-TCRDEM"), _T("S-TLMIN") },
		{ _T("X-HPA"), _T("X-TXRF"), _T(""), _T(""), _T("X-TCRMOD"), _T("CMDOUT3") },
		{ _T("X-LNA"), _T("X-RXRF"), _T("X-DC"), _T("X-RX2"), _T("X-DCSYN"), _T("X-TLMIN") },
		{ _T(""), _T(""), _T(""), _T(""), _T("XX-RARR"), _T("XR-RARR") },
		{ _T(""), _T(""), _T("XS-DC"), _T("XX-TCRMOD"), _T("XX-TCRDEM"), _T("X-CMDOUT2") },
		{ _T(""), _T(""), _T(""), _T(""), _T("XX-TCRRARR"), _T("X-TLMIN2") },
		{ _T("S-CAL"), _T("X-CAL"), _T("WEATHER"), _T(""), _T(""), _T("") },
		{ _T("u6ctrl"), _T("u6opr1"), _T("szopr1"), _T("tzopr1"), _T("tzopr2"), _T("") },
	},
	{
		// 54m
		{ _T("ANT"), _T("X-SSPA"), _T("X-TXIF"), _T("X-UC"), _T(""), _T("X-CMDOUT") },
		{ _T(""), _T(""), _T(""), _T(""), _T(""), _T("") },
		{ _T("X-LNA"), _T("X-RXIF"), _T("X-DC"), _T("DSN-MODEM"), _T(""), _T("XKa-TLMIN") },
		{ _T("Ka-LNA"), _T("Ka-DC"), _T(""), _T(""), _T(""), _T("") },
		{ _T(""), _T(""), _T(""), _T(""), _T("REF-DIV"), _T("") },
		{ _T("X-CAL"), _T("X-TRANS"), _T("TRANS-LO"), _T(""), _T(""), _T("") },
		{ _T("Ka-CAL"), _T("Ka-TRANS"), _T(""), _T(""), _T(""), _T("") },
		{ _T(""), _T(""), _T(""), _T(""), _T("WEATHER"), _T("CVCF") },
		{ _T("u5ctrl"), _T("u5opr1"), _T("szopr4"), _T("tzopr1"), _T(""), _T("") },
	},
	{
		// 34
		{ _T("ANT"), _T("S-TX"), _T("S-TXSYN"), _T("S-TXIFSW"), _T("SS-TCRMOD"), _T("S-CMDOUT") },
		{ _T("TX-POL"), _T("S-TRKRX"), _T(""), _T("S-RXIFSW"), _T("SS-TCRDEM"), _T("S-TLMIN") },
		{ _T(""), _T("S-ACQRX"), _T(""), _T(""), _T("SS-TCRRARR"), _T("") },
		{ _T("X-TX"), _T("X-RF_IF"), _T("X-UC"), _T(""), _T("DSNTCR-MOD"), _T("X-CMDOUT2") },
		{ _T("X-RX"), _T("X-ACQRX"), _T("X-TRXRX"), _T(""), _T("DSNTCR-DEM"), _T("X-TLMIN2") },
		{ _T(""), _T(""), _T("X-HRXDEM"), _T("X-HRXMOD"), _T("X-HRXTLMIN"), _T("") },
		{ _T("S-CAL"), _T("X-CAL"), _T(""), _T(""), _T("DSNTOR-RARR"), _T("") },
		{ _T("S/SX�ǔ���1"), _T("S/SX�ǔ���2"), _T("X/X�ǔ���"), _T("STS-1"), _T("TIME"), _T("WEATHER") },
		{ _T("QD"), _T("CTRL-1"), _T("CTRL-2"), _T("CTRL-3"), _T("CTRL-4"), _T("�u���b�N�}") },
	},
	{
		// 20m
		{ _T("ANT"), _T("S-TXRF"), _T("S-TXFCONV"), _T("S-USCYN"), _T("SS-TCRMOD"), _T("CMDOUT1") },
		{ _T(""), _T(""), _T(""), _T(""), _T("SX-TCRMOD"), _T("CMDOUT2") },
		{ _T("S-RXRF"), _T("S-DCSYN"), _T("S-TCRDET"), _T(""), _T("S-TCRDEM"), _T("S-TLMIN") },
		{ _T(""), _T("SX-ACQDCSYN"), _T("S-ACQDET"), _T("S-ACQTCRDEM"), _T("S-TCRRARR"), _T("") },
		{ _T(""), _T("X-DCSYN"), _T("X-TCRDET"), _T("X-TCRRARR"), _T("X-TCRDEM"), _T("X-TLMIN") },
		{ _T(""), _T(""), _T(""), _T("X-HRXDEM"), _T("X-HRXMOD"), _T("X-HRTLMIN") },
		{ _T("S-CALRF"), _T("X-CALRF U/C"), _T("SS-TRANSSYN"), _T("SX-TRANSSYN"), _T("X-TLMDEC"), _T("X-TLMSIM") },
		{ _T("CTRL-1"), _T("CTRL-2"), _T("CTRL-3"), _T(""), _T(""), _T("WEATHER") },
		{ _T("�ǔ���"), _T("�^�p��"), _T("QD"), _T("QD2"), _T("STS-1"), _T("�u���b�N�}") },
	},
};


static const CString mEquipmentStatusStatusTag[4][9][6] = {
	{
		// 64m
		{ CString(MONI64M_ANT), CString(MONI64M_S_TXRF), CString(MONI64M_S_TXFCONV), CString(MONI64M_S_UC), CString(MONI64M_SS_TCRMOD), CString(MONI64M_CMDOUT) },
		{ CString(""), CString(MONI64M_S_RXRF), CString(""), CString(MONI64M_S_DC), CString(MONI64M_SS_TCRDEM), CString(MONI64M_S_TLMIN) },
		{ CString(MONI64M_X_HPA), CString(MONI64M_X_TXRF), CString(""), CString(""), CString(MONI64M_X_TCRMOD), CString(MONI64M_CMDOUT3) },
		{ CString(MONI64M_X_LNA), CString(MONI64M_X_RXRF), CString(MONI64M_X_DC), CString(MONI64M_X_RX2), CString(MONI64M_X_DCSYN), CString(MONI64M_X_TLMIN) },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI64M_XX_RARR), CString(MONI64M_XR_RARR) },
		{ CString(""), CString(""), CString(MONI64M_XS_DC), CString(MONI64M_XX_TCRMOD), CString(MONI64M_XX_TCRDEM), CString(MONI64M_X_CMDOUT2) },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI64M_XX_TCRRARR), CString(MONI64M_X_TLMIN2) },
		{ CString(MONI64M_S_CAL), CString(MONI64M_X_CAL), CString(MONI64M_WEATHER), CString(""), CString(""), CString("") },
		{ CString(MONI64M_GREEN), CString(MONI64M_GREEN), CString(MONI64M_GREEN), CString(MONI64M_GREEN), CString(MONI64M_GREEN), CString("") },
	},
	{
		// 54m
		{ CString(MONI54M_ANT), CString(MONI54M_SSPA), CString(MONI54M_X_TXIF), CString(MONI54M_X_UC), CString(""), CString(MONI54M_X_CMDOUT2) },
		{ CString(""), CString(""), CString(""), CString(""), CString(""), CString("") },
		{ CString(MONI54M_X_LNA), CString(MONI54M_X_RXIF), CString(MONI54M_X_DC), CString(MONI54M_DSN_MODEM), CString(""), CString(MONI54M_X_TLMIN2) },
		{ CString(MONI54M_KA_LNA), CString(MONI54M_KA_DC), CString(""), CString(""), CString(""), CString("") },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI54M_REF_DIV), CString("") },
		{ CString(MONI54M_X_CAL), CString(MONI54M_X_TRANS), CString(MONI54M_TRANS_LO), CString(""), CString(""), CString("") },
		{ CString(MONI54M_KA_CAL), CString(MONI54M_KA_TRANS), CString(""), CString(""), CString(""), CString("") },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI54M_WEATHER), CString(MONI54M_CVCF) },
		{ CString(MONI54M_GREEN), CString(MONI54M_GREEN), CString(MONI54M_GREEN), CString(MONI54M_GREEN), CString(""), CString("") },
	},
	{
		// 34m
		{ CString(MONI34M_ANT), CString(MONI34M_S_TX), CString(MONI34M_S_TXSYN), CString(MONI34M_S_TXIFSW), CString(MONI34M_SS_TCRMOD), CString(MONI34M_S_CMDOUT) },
		{ CString(MONI34M_TX_POL), CString(MONI34M_S_TRKRX), CString(MONI34M_S_RXSYN), CString(MONI34M_S_RXIFSW), CString(MONI34M_SS_TCRDEM), CString(MONI34M_S_TLMIN) },
		{ CString(""), CString(MONI34M_SX_ACQRX), CString(""), CString(""), CString(MONI34M_SS_TCRRARR), CString("") },
		{ CString(MONI34M_X_TX), CString(MONI34M_X_IFSW), CString(MONI34M_X_TXSYN), CString(""), CString(MONI34M_DSNTCR_MOD), CString(MONI34M_X_CMDOUT2) },
		{ CString(MONI34M_X_RX), CString(MONI34M_X_RXSYN), CString(MONI34M_X_TRKRX), CString(""), CString(MONI34M_DSNTCR_DEM), CString(MONI34M_X_TLMIN2) },
		{ CString(""), CString(""), CString(MONI34M_X_HRX_DEM), CString(MONI34M_X_HRX_MOD), CString(MONI34M_X_HRXTLMIN), CString("") },
		{ CString(MONI34M_S_CAL), CString(MONI34M_X_CAL), CString(""), CString(""), CString(MONI34M_DSNTCR_RARR), CString("") },
		{ CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_TIME), CString(MONI34M_WEATHER) },
		{ CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN), CString(MONI34M_GREEN) },
	},
	{
		// 20m
		{ CString(MONI20M_ANT), CString(MONI20M_S_TXRF), CString(MONI20M_S_TXFCONV), CString(MONI20M_S_UCSYN), CString(MONI20M_SS_TCRMOD), CString(MONI20M_CMDOUT1) },
		{ CString(""), CString(""), CString(""), CString(""), CString(MONI20M_SX_TCRMOD), CString(MONI20M_CMDOUT2) },
		{ CString(MONI20M_S_RXRF), CString(MONI20M_S_DCSYN), CString(MONI20M_S_TCRDET), CString(""), CString(MONI20M_S_TCRDEM), CString(MONI20M_S_TLMIN) },
		{ CString(""), CString(MONI20M_SX_ACQDCSYN), CString(MONI20M_S_ACQDET), CString(MONI20M_S_ACQTCRDEM), CString(MONI20M_S_TCRRARR), CString("") },
		{ CString(""), CString(MONI20M_X_DCSYN), CString(MONI20M_X_TCRDET), CString(MONI20M_X_TCRRARR), CString(MONI20M_X_TCRDEM), CString(MONI20M_X_TLMIN) },
		{ CString(""), CString(""), CString(""), CString(MONI20M_X_HRX_DEM), CString(MONI20M_X_HRX_MOD), CString(MONI20M_X_HRXTLMIN) },
		{ CString(MONI20M_S_CALRF), CString(MONI20M_X_CALRF_UC), CString(MONI20M_SS_TRANSSYN), CString(MONI20M_SX_TRANSSYN), CString(MONI20M_X_TLMDEC), CString(MONI20M_X_TLMSIM) },
		{ CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(""), CString(""), CString(MONI20M_WEATHER) },
		{ CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN), CString(MONI20M_GREEN) },
	},
};


enum eHistory{
	eHistory_Operation,
	eHistory_Control,
	eHistory_Equipment,
	eHistory_Event,
	eHistory_Alarm,
	eHistory_SearchOperation,
	eHistory_SearchControl,
	eHistory_SearchEquipment,
	eHistory_SearchEvent,
	eHistory_SearchLimit,
	eHistory_SearchAlarm,
	eHistory_Max,
};

// �������
#define	MINCOLWIDTH		80
template <class T> inline const T& MAX(const T& t1, const T& t2) {
	return t1 < t2 ? t2 : t1;
}
#pragma pack(1)
typedef struct {
	WORD	year;			// �N
	BYTE	month;			// ��
	BYTE	day;			// ��
	BYTE	hour;			// ��
	BYTE	minute;			// ��
	BYTE	second;			// �b
	BYTE	station;		// ��
	BYTE	equipment;		// ���M��
	BYTE	imstatus;		// �ݔ��d�v�x
	BYTE	status;			// �X�e�[�^�X
	DWORD	message;		// ���b�Z�[�WID
	TCHAR	msgstring[1024];
	UINT	searchtype;
} stHistoryData;

typedef struct {
	bool	filter;
	WORD	station;
	BYTE	sender[eStation_MAX];
	BYTE	imstatus;
	BYTE	status;
	DWORD	message;
	DWORD	message2;
	SYSTEMTIME stime;
	SYSTEMTIME ptime;
	UINT	searchtype;
} stFilterData;
#pragma pack()

enum eFilterItem{
	eFilterItem_Station,
	eFilterItem_Time,
	eFilterItem_Equipment,
	eFilterItem_Status,
	eFilterItem_MessageId,
	eFilterItem_Message,
};

enum eFilterItemEq{
	eFilterItemEq_Station,
	eFilterItemEq_Time,
	eFilterItemEq_Equipment,
	eFilterItemEq_ImStatus,
	eFilterItemEq_Status,
	eFilterItemEq_MessageId,
	eFilterItemEq_Message,
};

enum eSearchtem{
	eSearchitem_Station,
	eSearchitem_Time,
	eSearchitem_Equipment,
	eSearchitem_ImStatus,
	eSearchitem_Status,
	eSearchitem_MessageId,
	eSearchitem_Message,
	eSearchitem_Max,
};

enum eFilterItemEvent{
	eFilterItemEvent_Station,
	eFilterItemEvent_Time,
	eFilterItemEvent_Status,
	eFilterItemEvent_MessageId,
	eFilterItemEvent_Message,
};

enum eDirectWindowID{
	eCtrl_AntEmg = (WM_USER+1000),
	eCtrl_SndStop,
	eCtrl_SnapShot,
	eCtrl_Sound,
	eCtrl_Mute,
	eCtrl_Help,
	eGridCtrl_Equipment = 510001,
	eGridCtrl_Operation,
	eTrackingCtrl,
	eSummaryCtrl,
	eSummaryCtrl2,
	eSummaryCtrl3,
	eSummaryCtrl4,
	eSummaryGraph,
	eSummaryGraph2,
	eSummaryGraph3,
	eSummaryGraph4,
	eTrendCtrl,
	eOperationHistory,
	eOperationSearch,
	eControlHistory,
	eControlSearch,
	eEquipmentHistory,
	eEquipmentSearch,
	eEventHistory,
	eEventSearch,
	eAlarmList,
};

// AGC�Z�� ���[�v�o���h
enum {
	AGCLB_1kHz,
	AGCLB_300Hz,
	AGCLB_100Hz,
	AGCLB_30Hz,
	AGCLB_10Hz,
	AGCLB_5p6Hz,
	AGCLB_3Hz,
	AGCLB_1p8Hz,
	AGCLB_1Hz,
	AGCLB_0p56Hz,
	AGCLB_0p3Hz,
	AGCLB_0p18Hz,
	AGCLB_01Hz,
	AGCLB_CURRENT_LBW,
};

static const CString strAGCLoopBand[] = {
	_T("1kHz"),
	_T("300Hz"),
	_T("100Hz"),
	_T("30Hz"),
	_T("10Hz"),
	_T("5.6Hz"),
	_T("3Hz"),
	_T("1.8Hz"),
	_T("1Hz"),
	_T("0.56Hz"),
	_T("0.3Hz"),
	_T("0.18Hz"),
	_T("0.1Hz"),
	_T("CURRENT_LBW"),
	_T(""),
};

#define CURRENT_LBW _T("CURRENT_LBW")

//=============================================================================
//	INI�t�@�C��
//=============================================================================
const CString SECTION_SYSTEM = _T("SYSTEM");
const CString KEY_STATIONMAX = _T("StationMax");	// �ő�ǐ�
const CString KEY_FILESERVER_NAME = _T("FileServerName");	// �t�@�C���T�[�o�[����
const CString KEY_LOGLEVEL = _T("LogLevel");		// ���O���x��
const CString SECTION_DEBUG = _T("DEBUG");
const CString KEY_TIMERATE = _T("TimeRate");

const CString KEY_SSHUSER_NAME = _T("SshUserName");		// SSH���[�U����
const CString KEY_SSHSERVER_NAME = _T("SshServerName");	// SSH�T�[�o����
const CString KEY_SSHKEY_PATH = _T("SshKeyFilePath");	// SSH�L�[�t�@�C���p�X����
const CString KEY_SSHBATCH_PATH = _T("SshBatchPath");	// SSH�o�b�`�t�@�C���p�X����
const CString KEY_EVENTTONE_PATH = _T("EventTonePath");	// �C�x���g���t�@�C���i�[�p�X

const CString KEY_DRAWUPDATEINTERVAL = _T("DrawUpdateInterval");	// �`��X�V�Ԋu
const CString KEY_MESSAGETHREAD = _T("MessageThread");	// ���b�Z�[�W�X���b�h

const CString KEY_PREDDIFF_ENABLE = _T("PredDiffEnable");	// PRED DIFF�v�Z�L������(1:�L��/0:����)
const CString KEY_PREDDIFF_MIN = _T("PredDiffMin");		// �ŏ�PRED DIFF�Ԋu(54m)

const CString KEY_HISTID54 = _T("HistoryID54");			// 54m�̗������ʔԍ�

const CString KEY_SERVICE_MODE = _T("ServiceMode");		// �T�[�r�X���[�h�iadmin, sttn, ssoc, user)

static const TCHAR* mServiceModeString[]{
	_T("user"),		// ���[�U���[�h�E�p�X���[�h�v
	_T("admin"),	// �Ǘ��҃��[�h�E�p�X���[�h�s�v
	_T("sttn"),		// �n��ǃ��[�h�E�p�X���[�h�s�v
	_T("ssoc"),		// ���͌����[�h�E�p�X���[�h�s�v
	_T("limited"),	// �������[�h�E�p�X���[�h�v
	_T("unlimited"),	// �������[�h�����E�p�X���[�h�v
	_T("oldremote"),	// �������[�g���̃��[�h
};
enum eServiceMode{
	eServiceMode_user,
	eServiceMode_admin,
	eServiceMode_sttn,
	eServiceMode_ssoc,
	eServiceMode_limited,
	eServiceMode_unlimited,
	eServiceMode_oldremote,
	eServiceMode_max,
};

const CString APP_TIMELINE = _T("OperationTimeLine.exe");	// �^�p�^�C�����C���A�v������
//=============================================================================
//	�t���[��
//=============================================================================
#define OFFSET_EMGBUTTON_RIGHT	50
#define MAX_EMGBUTTON_WIDTH		120

//=============================================================================
//	�g�����h�O���t
//=============================================================================
#define MAX_TDATA	13
#define MAX_SUMMARYTDATA	10
#define MAX_VIEW	 8
#define MAX_VSCALE	 2
#define MAX_HSCALE	 2

#define	DREAL_NAN_HIGH		0x7ff80000
#define	DREAL_NAN_LOW		0x00000000
#define	DREAL_ERROR_HIGH	0xffffffff
#define	DREAL_ERROR_LOW		0xfffffffe

#define MAX_OUTPUT_LIST		100

#define MAX_SPACEDATA_SIZE	(3600 * 24)	// �q�����f�[�^�ێ��T�C�Y�i1���j
//#define MAX_SPACEDATA_SIZE	(300)	// �q�����f�[�^�ێ��T�C�Y����m�F�e�X�g

//#define MAX_PPDATA_SIZE	(12 + 1)		// ����m�F�e�X�g
//#define MAX_PPDATA_SIZE	(120 + 1)		// ����m�F�e�X�g
#define MAX_PPDATA_SIZE		(3600 + 1)		// PP���k�f�[�^�ێ��T�C�Y�i�E�B���h�E�E�[�ʒu�̃f�[�^�\���̂��߁A+1����j
#define MAX_DATASELECT		80

#define MAX_SUMMARY_VIEW	4

enum eTrendWndType{
	eTrendWndType_AGC = 0,
	eTrendWndType_RNG,
	eTrendWndType_RR,
	eTrendWndType_EL,
	eTrendWndType_Num
};

enum eTrendWndSetType{
	eTrendWndSetType_AGC = 0,
	eTrendWndSetType_RNG_RR,
	eTrendWndSetType_AGC_RNG_EL,
	eTrendWndSetType_Num
};

enum eSummaryWndSetType{
	eSummaryWndSetType_RNG = 0,
	eSummaryWndSetType_RR,
	eSummaryWndSetType_XSP,
	eSummaryWndSetType_KASP,
	eSummaryWndSetType_SAGC,
	eSummaryWndSetType_XAGC,
	eSummaryWndSetType_Num
};

enum eMessagex{
	// �E�B���h�E�n
	eMessage_ChildActivate = (WM_USER + 100),
	eMessage_ChildDestroy,
	eMessage_DirectPrint,
	eMessage_WindowClose,

	// �X���b�h�n
	eMessage_ThreadNormal,
	eMessage_ThreadError,
	eMessage_ThreadFail,
	eMessage_ThreadException,
	eMessage_ThreadEvent,

	// �v���p�e�B�n
	eMessage_PropetyChange,
	eMessage_SummaryPropetyChange,
	eMessage_PropetyCmbChange,
	eMessage_AutoClear,

	// �^�p�^�C�����C��
	eMessage_TimeLineHandle = (WM_USER + 6000),	// �E�B���h�E�n���h��
	eMessage_TimeLineFinish,					// �A�v���I��
	eMessage_TimeLinePlan,						// �v�旧��
	eMessage_TimeLineForcast,					// �\��l�ꗗ
	eMessage_TimeLineEmg,						// �A���e�i������~
	eMessage_TimeLineStop,						// ���M�@��~

	// �ݔ����
	eMessage_EqStatusControl = (WM_USER + 7000),	// �R���g���[���n���h��

	// �g�����h�O���t
	eMessage_InitTrendWnd = (WM_USER + 8000),		// �g�����h�O���t�E�B���h�E������
	eMessage_OpenTrendSetDlg,						// �g�����h�O���t�ݒ�E�B���h�E�Ăяo��
	eMessage_OpenSummaryTrendSetDlg,				// �T�}���g�����h�O���t�ݒ�E�B���h�E�Ăяo��
	eMessage_OpenSummaryTrendSetDlgChg,				// �T�}���g�����h�O���t�ݒ�E�B���h�E�Ăяo���i�_�C�A���O���X�y�N�g�����ݒ肩��g�����h�ݒ�ɕύX�j
	eMessage_UpdateSummaryTabCombo,					// �T�}���O���t�^�u�R���{�X�V�ʒm
	eMessage_UpdateForecastData,					// �\��l�f�[�^�X�V�ʒm
	eMessage_UpdateEngValData,						// �H�w�l�ϊ��f�[�^�X�V�ʒm
	eMessage_NewEngValData,							// �V�����H�w�l�ϊ��f�[�^���󂯂��ʒm

	// �X�y�N�g�����O���t
	eMessage_OpenSpectrumSetDlg = (WM_USER + 8100),		// �X�y�N�g�����ݒ�E�B���h�E�Ăяo��
	eMessage_OpenSummarySpectrumSetDlg,					// �T�}���X�y�N�g�����O���t�ݒ�E�B���h�E�Ăяo��
	eMessage_OpenSummarySpectrumSetDlgChg,				// �T�}���X�y�N�g�����O���t�ݒ�E�B���h�E�Ăяo���i�_�C�A���O���g�����h�ݒ肩��X�y�N�g�����ݒ�ɕύX�j

	// �\��l�ꗗ
	eMessage_OpenForecastListDlg = (WM_USER + 8200),	// �\��l�ꗗ�Ăяo��
	eMessage_UpdateForecastFilePath,					// �\��l�t�@�C���p�X�X�V

	// �T�}���O���t
	eMessage_InitSummaryWnd = (WM_USER + 8300),				// �T�}���O���t�E�B���h�E������

	// ���L�t�@�C��
	eMessage_UpdateShareFile = (WM_USER + 8400),			// ���L�t�@�C���̍X�V

	// SSH
	eMessage_CompleteSsh = (WM_USER + 8500),				// SSH����

	// �Z�b�V�������
	eMessage_UpdateSessionStatus = (WM_USER + 8600),		// �Z�b�V������ԍX�V

	// ��������
	eMessage_UpdateSeqCtrlPos = (WM_USER + 8700),			// ��������̍s�ʒu�X�V
	eMessage_PauseCompltete,								// �|�[�Y����
	eMessage_WarnSeqCtrlSetDefFile,							// ��������̒�`�t�@�C���w�莸�s���[�j���O
	eMessage_WarnSeqCtrlExec,								// ����������s���s���[�j���O
	eMessage_WarnSeqCtrlPause,								// ��������̃|�[�Y���s���[�j���O
	eMessage_WarnSeqCtrlSelLinePos,							// ��������̍s�w�莸�s���[�j���O
	eMessage_WarnSeqCtrlEnd,								// ��������̒��f���s���[�j���O

	// ����
	eMessage_History,

	// �Z���f�f
	eMessage_RdnsChkTimeout = (WM_USER + 8900),				// �Z���f�f�^�C���A�E�g
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
}VScaleKind;

typedef struct {
	bool	Auto;
	double	VScalLMax;
	double	VScalLMin;
	double	VScalRMax;
	double	VScalRMin;
	long	VSCalItvl;
} stVScale;

typedef enum {
	m1,
	m5,
	m10,
	m20,
	m30,
	m60,
	h2,
	h4,
	h8,
	h12,
	h16,
	h20,
	h24,
//	autoscale,
	yohochiscale,
}HScale;

static const CString strHScale[] = {
	_T("1��"),
	_T("5��"),
	_T("10��"),
	_T("20��"),
	_T("30��"),
	_T("60��"),
	_T("2����"),
	_T("4����"),
	_T("8����"),
	_T("12����"),
	_T("16����"),
	_T("20����"),
	_T("24����"),
//	_T("�I�[�g"),
	_T("�\��l�ɍ��킹��"),
	_T(""),
};
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
	RGB(128, 128, 128),	// gray
	RGB(0, 0, 128),		// navy
	RGB(128, 0, 0),		// maroon
	RGB(128, 0, 128),	// purple
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
	long		lineWidth;
	CString		DotSize;
	CString		SelVScale;
	CString		dataUnit;
	double		Alarm1Major;
	double		Alarm1Minor;
	double		Alarm2Major;
	double		Alarm2Minor;
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
	_T("60��"),
	_T("���A�����[�h"),
	_T("On"),
	_T("Line"),
	1,
	_T("Small"),
	_T("��"),
	_T(""),
	0,
	0,
	0,
	0,
};

static const stPropData initSummaryPropData = {
	_T("On"),
	_T(""),
	100,
	-100,
	50,
	-50,
	4,
	_T("24����"),
	_T("60��"),
	_T("���A�����[�h"),
	_T("On"),
	_T("Line"),
	1,
	_T("Small"),
	_T("��"),
	_T(""),
	0,
	0,
	0,
	0,
};

typedef struct
{
	CEvent*		pTermThreadEvent;	// �X���b�h�C�x���g���I��
	CWnd*		pWnd;				// �E�B���h�E�n���h��
	bool		bPassStatus;		// �p�X�󋵉�ʃt���O
	UINT		nStation;			// �I���ǔԍ�
	int			nExecuteMode;		// ���䃂�[�h�A�Ď����[�h
} stThreadInfo;


//=============================================================================
//	�Ď��f�[�^
//=============================================================================

typedef struct {
	double	max;
	double	min;
	__int64	count;
} stPPPair;

typedef struct  {
	DWORD		idx;		// �C���f�b�N�X
	SYSTEMTIME	systime;	// ����
	//DWORD		dataid;		// �f�[�^ID
	//TCHAR		name[80];	// �f�[�^����
	//DWORD		kind;		// �f�[�^���
	double		value;		// �f�[�^
	BYTE		status[8];	// �X�e�[�^�X
	stPPPair	ppPair;		// PP���k
	TCHAR		res[16];
} stSpaceData;

typedef struct {
	DWORD	id;			// �f�[�^ID
	CString	name;		// �f�[�^����
} stSelectData;

enum eStatus{
	eStatus_None,		// �f�[�^����
	eStatus_Enable,		// �f�[�^�L��
	eStatus_Disable,	// �f�[�^�����i�Ⴆ�΃f�[�^�������j
};


class CSpaceData
{
public:
	CSpaceData(){};
	virtual ~CSpaceData(){};

public:
	DWORD			dataid;		// �f�[�^ID
	CString			dataname;	// �f�[�^��
	DWORD			kind;		// �f�[�^���
	int				srcType;	// 0:�I�����C���f�[�^ / 1:�t�@�C���f�[�^
	__int64			totalSize;	// �L���[�폜�O�̃g�[�^���T�C�Y
	deque<stSpaceData>	value;	// �g�����h�f�[�^�L���[
};

typedef struct {
	DWORD		dataid;		// �f�[�^ID
	SYSTEMTIME	systime;	// ����
} stDataIdTime;


//=============================================================================
//	�Z�b�V����
//=============================================================================
enum eSessionType
{
	eSessionType_Monitor = 0,	// �Ď��Z�b�V����
	eSessionType_Ctrl,			// ����Z�b�V����
	eSessionType_Notify,		// �ʒm�Z�b�V����
	eSessionType_History,		// �����Z�b�V����
	eSessionType_Num			// �Z�b�V������
};

//=============================================================================
//	��M���g�����[�h
//=============================================================================
enum eRcvFreqModeType
{
	eRcvFreqModeType_1Way = 0,	// 1WAY
	eRcvFreqModeType_2Way,		// 2WAY
//	eRcvFreqModeType_3Way,		// 3WAY
	eRcvFreqModeType_Dup,		// DUP
	eRcvFreqModeType_Other,		// Other
	eRcvFreqModeType_Num,		// ���[�h��
};

//=============================================================================
//	���䃂�[�h
//=============================================================================
enum eCtrlModeType
{
	eCtrlModeType_Manual = 0,	// �蓮
	eCtrlModeType_Auto,			// ����
	eCtrlModeType_Null,			// NULL
	eCtrlModeType_Num,			// ���[�h��
};

//=============================================================================
//	X��<=>Ka�ѐ؂�ւ�
//=============================================================================
enum eBandSelectType
{
	eBandSelectType_X = 0,		// X��
	eBandSelectType_Ka,			// Ka��
	eBandSelectType_Null,		// NULL
	eBandSelectType_Num,		// ���[�h��
};

//=============================================================================
//	RF TRIP�Ď�
//=============================================================================
enum eRfTripMonitorType
{
	eRfTripMonitorType_Manual = 0,	// �蓮
	eRfTripMonitorType_Auto,		// ����
	eRfTripMonitorType_Null,		// NULL
	eRfTripMonitorType_Num,			// ���[�h��
};

//=============================================================================
//	�ǔ��󋵃O���t
//=============================================================================
/*--------------*/
/* �^�p�󋵊֌W */
/*--------------*/
#define  UNYOU_OUT        0         /* �^�p�󋵁i��^�p��ԁj         */
#define  UNYOU_START      1         /* �^�p�󋵁i�^�p�J�n30���ȓ��O�j */
#define  UNYOU_IN         2         /* �^�p�󋵁i�^�p���j             */
#define  UNYOU_TIME  604800         /* �^�p�؂�ւ��J�n���ԁi�V���j   */
#define  DSP_ON           1			/* �\���؂�ւ��v                 */
#define  DSP_OFF          0			/* �\���؂�ւ��s�v               */
#define  YOHOUTI_ARI      1			/* �\��l�t�@�C���L��             */
#define  YOHOUTI_NASI     0			/* �\��l�t�@�C���Ȃ�             */
#define  YOHOGET_MATI     1         /* �\��l�t�@�C���擾�҂�(V2-5)   */
#define  YOHOGET_SUMI     2         /* �\��l�t�@�C���擾�ς�(V2-5)   */
#define  YOHOGET_OK       3         /* �\��l�t�@�C���擾�\(V2-5)   */

#define  INFLG_OFF	    0							// �Ď��f�[�^����
#define  INFLG_ON	    1				            // �Ď��f�[�^�L��

#define  UNYOU_FLAG		"CTRL.PASS_FLAG"			/* �^�p�p�X��ԃt���O		  */
#define  TILL_TIME 		"CTRL.PASS_CNTDOWN(DEC)"	/* �^�p�p�X�J�n�܂ł̎���	  */
#define  START_TIME		"CTRL.START_TIME"    		/* �p�X�J�n����               */
#define  STOP_TIME		"CTRL.END_TIME"     		/* �p�X�I������               */
#define  PREDFILE_NAME	"CTRL.PRED_FILE"			/* �\��l�t�@�C���p�X��       */
#define  PREDFILE_TIME	"CTRL.MAKE_PRED_TIME" 		/* �\��l�t�@�C���쐬����     */
#define  PLAN_NO		"CTRL.PASS_NO"				/* �v��ԍ�                   */
#define  EISEI_NAME		"CTRL.SAT_NAME"				/* �q����                     */
#define  AOS_TIME		"CTRL.PRED_AOS_TIME"		/* �`�n�r����                 */
#define  AOS_AZ			"CTRL.AZ_AT_AOS"			/* �`�n�r �`�y�l              */
#define  AOS_EL			"CTRL.EL_AT_AOS"			/* �`�n�r �d�k�l              */
#define  LOS_TIME		"CTRL.PRED_LOS_TIME"		/* �k�n�r����                 */
#define  LOS_AZ			"CTRL.AZ_AT_LOS"			/* �k�n�r �`�y�l              */
#define  LOS_EL			"CTRL.EL_AT_LOS"			/* �k�n�r �d�k�l              */
#define  MAXEL_TIME		"CTRL.PRED_MAX_EL_TIME"		/* �l�`�w �d�k����            */
#define  MAXEL_AZ		"CTRL.AZ_AT_MAX_EL"			/* �l�`�w �d�k �`�y�l         */
#define  MAXEL_EL		"CTRL.EL_AT_MAX_EL"			/* �l�`�w �d�k �d�k�l         */
#define  VISIBLE		"CTRL.VISIBLE"				/* �u�h�r�h�a�k�d(HH:MM)      */
#define  YOHOU_GET      "CTRL.PRED_COND"            /* �\��l�擾���         	  */
#define  REAL_AZ		"ANT.REAL_AZ"				/* AZ�����l */
#define  REAL_EL		"ANT.REAL_EL"				/* EL�����l */
#define  CTRL_MON_TIME	"CTRL.MON_TIME"				/* �ǔ��̌��݂̊Ď����� */

//=============================================================================
//	�O���t�p�f�[�^��
//=============================================================================
#define	 GRDATA_ID_MAX   10		/* �O���t�Ď����ڍő吔               */
#define	 GRNUM_AGC_MAX   10		/* �`�f�b�O���t�Ď����ڐ�             */
#define	 GRNUM_RNG_MAX   8		/* �q�m�f�C�q�q�O���t�Ď����ڐ�		�@*/
#define	 GRNUM_PRS_MAX   2		/* �ǔ��󋵃O���t�Ď����ڐ�           */

#define  GRDATA_NUM_MAX  86400	/* �O���t�f�[�^�l�`�w����             */
#define  GRDATA_NUM_AGC  86400	/* �`�f�b�O���t�f�[�^����             */
#define  GRDATA_NUM_RNG  86400	/* �q�m�f�C�q�q�O���t�f�[�^����       */
#define  GRDATA_NUM_PRS  86400	/* �ǔ��󋵃O���t�f�[�^����           */
#define  GRDATA_NUM_AGC_SH  864000	/* �`�f�b�O���t�f�[�^����(�Z����) */

// �ǔ��O���t�^�C�v
enum eRadarViewType
{
	eRadarViewType_Station = 0,
	eRadarViewType_ForecastList,
	eRadarViewType_OperationPlan,
};

struct Az_El {
	double d_AZ;						// AZ�f�[�^
	double d_EL;						// EL�f�[�^
	double d_time;						// ����
};

struct Forecast_Az_El {
	double	d_AZ;
	double	d_EL;
	double  d_aos;
	double	d_time;
	int		i_roop_cnt;
};

//=============================================================================
//	�O���t�\���ۑ��f�[�^�i�[�\��
//=============================================================================
struct   shmdata_t {            /* �O���t�\���ۑ��f�[�^�i�[�\��       */
	long   l_id;				/* �f�[�^�h�c(�̈敪���g�p�j          */
	long   l_size;				/* �f�[�^�h�c���̈�T�C�Y             */
	long   l_end;				/* �O��ŏI�i�[�f�[�^�ʒu[]           */
	double d_endtime;			/* �O��ŏI�i�[�f�[�^����             */
	double d_data[GRDATA_NUM_MAX];/* �g�����h�f�[�^�i�[�G���A(���b�j    */
};

/* �Z�����p�f�[�^�i�[�G���A */
struct   shmdata_sh_t {         /* �O���t�\���ۑ��f�[�^�i�[�\��       */
	long   l_id;				/* �f�[�^�h�c(�̈敪���g�p�j          */
	long   l_size;				/* �f�[�^�h�c���̈�T�C�Y             */
	long   l_end;				/* �O��ŏI�i�[�f�[�^�ʒu[]           */
	double d_endtime;			/* �O��ŏI�i�[�f�[�^����             */
	double d_data[GRDATA_NUM_AGC_SH];/* �g�����h�f�[�^�i�[�G���A(���b�j */
};

//=============================================================================
//	�X�y�N�g�����O���t�\����`
//=============================================================================
#define SPECTRUM_NUM        ( 1024 )
#define SPECTRUM_NUM_MIN    ( 0 )
#define SPECTRUM_NUM_MAX    ( 1023 )
#define SPECTRUM_INTERVAL        ( 300 ) /* �^�C���A�E�g�R�[���o�b�N�Ăяo���Ԋu[msec] */
#define SPECTRUM_MONIP_70MIF_SEL ( 0 )
#define SPECTRUM_MONIP_CL_SEL    ( 1 )
#define SPECTRUM_MONIP_CLA_SEL   ( 2 )
#define SPECTRUM_EXEC_CMD        _T("opctproc")	/* ������s�R�}���h�� */
/* �X�y�N�g�����t�@�C���ݒ� */
#define SPECTRUM_FILE_AUTO_SCALE	_T("AutoScale")     /* �I�[�g�X�P�[������ */	
#define SPECTRUM_FILE_GRID_MIN   _T("GridMin")		/* �O���b�h�ŏ��l���� */
#define SPECTRUM_FILE_GRID_MAX   _T("GridMax")       /* �O���b�h�ő�l���� */
#define SPECTRUM_FILE_GRID_VAL   _T("GridVal")       /* �O���b�h�l���� */
#define SPECTRUM_FILE_GRID_OFS   _T("Offset")        /* �O���b�h�I�t�Z�b�g�l���� */
#define SPECTRUM_PEAK_DEF   ( -160.0 )
// ���W�A��
#define	SPECTRUM_RADCV	(3.1415926538/180.0)
// ����`
#define SPECTRUM_MAXDIGIT	(1 + 9 + 2 + 1)		// �}�C�i�X +  ������ + �J���} + ������I��� 

#define SPECTRUM_FREQ_OFS_MIN  ( double )( -4000000.000 )
#define SPECTRUM_FREQ_OFS_MAX  ( double )(  4000000.000 )

#define SPECTRUM_INPUTMAX 100000000
#define SPECTRUM_INPUTMIN -100000000
#define SPECTRUM_AUTOSCALE_ON	0
#define SPECTRUM_AUTOSCALE_OFF	1

// �t�@�C���w�b�_
#define SPECTRUM_HEADER		("!#HEAD")

enum eSpectrumGraphType{
	eSpectrumGraphType_Normal = 0,
	eSpectrumGraphType_Sammary
};

// �Ď��f�[�^�֘A��`
#define MON_TIME_TAG_NAME		"DSNTCR_SPE.TIME_TAG"
#define MON_MONI_POINT_NAME     "DSNTCR_SPE.SPEC_MONI_POINT"
#define MON_MEASURE_BW_NAME		"DSNTCR_SPE.SPEC_MEASURE_BW"
#define MON_SPECTRUM_NAME		"DSNTCR_SPE.SPECTRUM_%d"
#define MON_FREQ_OFFSET_NAME    "DSNTCR_SPE.SPEC_FRE_OFFSET"
#define CTL_MONI_POINT_NAME     "DSNTCR.SPEC_MONI_POINT"
#define CTL_MEASURE_BW_NAME     "DSNTCR.SPEC_MEASURE_BW"
#define CTL_FREQ_OFFSET_NAME    "DSNTCR.SPEC_FRE_OFFSET"
#define MON_CENTER_FREQ_NAME    "DSNTCR_SPE.SPEC_CENTER_FREQ"

//=============================================================================
//	�q���ߑ��O���t��`
//=============================================================================
// �q���ߑ��O���t�p�̋�.�ݔ�.���ږ��ƊȈՕ\�����̃Z�b�g
typedef struct {
	int		nSigType;				// �M���^�C�v
	CString strSigName;				// ��.�ݔ�.���ږ�
	CString strSimpleSigName;		// �ȈՕ\����
	CString strUnit;				// �P�ʕ\����
	int		nIntDidits;				// ����������
	int		nDecDidits;				// �����_�ȉ�����
} structSatelliteCaptureSig;

// �q���ߑ��O���t�̐M�����^�C�v
enum eSigSrcType
{
	eSigSrcType_EngExchange = 0,	// �H�w�l�ϊ��f�[�^
	eSigSrcType_File,				// �t�@�C���f�[�^
	eSigSrcType_Num
};

// �q���ߑ��O���t�Ď����ږ�
// �P�c64m��
// AGC
#define UDSC64_AGC_SS_TCR_SIG		_T("SS-TCRAGC.AGC_A_AVG")
#define UDSC64_AGC_X_RX2_SIG		_T("X-RX2AGC.AGC_A_AVG")
#define UDSC64_AGC_XX_TCR_SIG		_T("DSNTCR_AGAV.AGC_A_AVG")
#define UDSC64_AGC_XRX2_INTEG_SIG	_T("X-RX2AGC.AGC_A_TKSC")
#define UDSC64_AGC_XXTCR_INTEG_SIG	_T("DSNTCR_AGAV.AGC_A_TKSC")

// RNG
#define UDSC64_RNG_SS_SIG			_T("CTRL.AMB_SS_DATA_KM")
#define UDSC64_RNG_XX_SIG			_T("CTRL.AMB_XX_DATA_KM")
#define UDSC64_RNG_XX_REPLAY_SIG	_T("CTRL.AMB_RX_DATA_KM")
#define UDSC64_RNG_XX_TCR_SIG		_T("CTRL.AMB_XX_DSN_DATA_KM")
#define UDSC64_RNG_PRED_SIG			_T("RNG(PRED)")

// RR
#define UDSC64_RR_SS_SIG			_T("CTRL.DIST_SS_DATA")
#define UDSC64_RR_XX_SIG			_T("CTRL.DIST_XX_DATA")
#define UDSC64_RR_XX_TCR_SIG		_T("CTRL.DIST_XX_DSN_DATA")
#define UDSC64_RR_PRED_SIG			_T("RR(PRED)")

// EL
#define UDSC64_EL_EL_SIG			_T("ANT.REAL_EL")
#define UDSC64_EL_EL_PRED_SIG		_T("EL(PRED)")
#define UDSC64_EL_SKYLINE_SIG		_T("SKYLINE")
#define UDSC64_EL_RF_TRIP_SIG		_T("RF-TRIP")


// �P�c54m��
// AGC
#define XDSC54_AGC_SS_TCR_SIG		_T("SS-TCRAGC.AGC_A_AVG")
#define XDSC54_AGC_X_RX2_SIG		_T("X-RX2AGC.AGC_A_AVG")
#define XDSC54_AGC_XX_TCR_SIG		_T("DSNTCR_AGAV.AGC_A_AVG")
#define XDSC54_AGC_XRX2_INTEG_SIG	_T("X-RX2AGC.AGC_A_TKSC")
#define XDSC54_AGC_XXTCR_INTEG_SIG	_T("DSNTCR_AGAV.AGC_A_TKSC")
//#define XDSC54_AGC_DSNMODEM_SIG		_T("DSNTCR_AGAV.AGC_A_AVG")	
#define XDSC54_AGC_DSNMODEM_SIG		_T("DSNTCR_AGSH.AGC_A_AVG")
#define XDSC54_AGC_DSNMODEM_INTEG_SIG	_T("DSNTCR_AGAV.AGC_A_TKSC")		// DataCollection�ŋǃG���A�Œu������

// RNG
#define XDSC54_RNG_SS_SIG			_T("CTRL.AMB_SS_DATA_KM")
#define XDSC54_RNG_XX_SIG			_T("CTRL.AMB_XX_DATA_KM")
#define XDSC54_RNG_XX_REPLAY_SIG	_T("CTRL.AMB_RX_DATA_KM")
#define XDSC54_RNG_XX_TCR_SIG		_T("CTRL.AMB_XX_DSN_DATA_KM")
#define XDSC54_RNG_DSNMODEM_X_SIG	_T("CTRL.AMB_XX_DSN_DATA_KM")
#define XDSC54_RNG_DSNMODEM_KA_SIG	_T("CTRL.AMB_XKa_DSN_DATA_KM")
#define XDSC54_RNG_PRED_SIG			_T("RNG(PRED)")

// RR
#define XDSC54_RR_SS_SIG			_T("CTRL.DIST_SS_DATA")
#define XDSC54_RR_XX_SIG			_T("CTRL.DIST_XX_DATA")
#define XDSC54_RR_XX_TCR_SIG		_T("CTRL.DIST_XX_DSN_DATA")
#define XDSC54_RR_DSNMODEM_X_SIG	_T("CTRL.DIST_XX_DSN_DATA")
#define XDSC54_RR_DSNMODEM_KA_SIG	_T("CTRL.DIST_XKa_DSN_DATA")
#define XDSC54_RR_PRED_SIG			_T("RR(PRED)")

// EL
#define XDSC54_EL_EL_SIG			_T("ANT.REAL_EL")
#define XDSC54_EL_EL_PRED_SIG		_T("EL(PRED)")
#define XDSC54_EL_SKYLINE_SIG		_T("SKYLINE")
#define XDSC54_EL_RF_TRIP_SIG		_T("RF-TRIP")


// ���V�Y34m��
// AGC
#define USC34_AGC_SS_TCR_A_SIG		_T("SS-TCRAGC.AGC_A_AVG")
#define USC34_AGC_SS_TCR_B_SIG		_T("SS-TCRAGC.AGC_B_AVG")
#define USC34_AGC_S_TRKRX_A_SIG		_T("S-RXAGC.AGC_A_AVG")
#define USC34_AGC_S_TRKRX_B_SIG		_T("S-RXAGC.AGC_B_AVG")
#define USC34_AGC_X_RX_A_SIG		_T("X-TRKRXAGC.AGC_A_AVG")
#define USC34_AGC_X_RX_B_SIG		_T("X-TRKRXAGC.AGC_B_AVG")
#define USC34_AGC_S_ACQRX_A_SIG		_T("S-ACQRXAGC.AGC_A_AVG")
#define USC34_AGC_S_ACQRX_B_SIG		_T("S-ACQRXAGC.AGC_B_AVG")
#define USC34_AGC_X_ACQRX_A_SIG		_T("X-ACQRXAGC.AGC_A_AVG")
#define USC34_AGC_X_ACQRX_B_SIG		_T("X-ACQRXAGC.AGC_B_AVG")
#define USC34_AGC_X_HRX_SIG			_T("X-HRXAGC.AGC_A_AVG")
#define USC34_AGC_SHXX_TCR_SIG		_T("DSNTCR_AGSH.AGC_A_AVG")
#define USC34_AGC_INTEG_SIG			_T("DSNTCR_AGAV.AGC_A_TKSC")

// RNG
#define USC34_RNG_SS_SIG			_T("CTRL.AMB_SS_DATA_KM")
#define USC34_RNG_XX_SIG			_T("CTRL.AMB_XX_DSN_DATA_KM")
#define USC34_RNG_PRED_SIG			_T("RNG(PRED)")

// RR
#define USC34_RR_SS_SIG				_T("CTRL.DIST_SS_DATA")
#define USC34_RR_XX_SIG				_T("CTRL.DIST_XX_DSN_DATA")
#define USC34_RR_PRED_SIG			_T("RR(PRED)")

// EL
#define USC34_EL_EL_SIG				_T("ANT.REAL_EL")
#define USC34_EL_EL_PRED_SIG		_T("EL(PRED)")
#define USC34_EL_SKYLINE_SIG		_T("SKYLINE")
#define USC34_EL_RF_TRIP_SIG		_T("RF-TRIP")


// ���V�Y20m��
// AGC
#define USC20_AGC_SS_TCR_A_SIG		_T("SS-TCRAGC.AGC_A_AVG")
#define USC20_AGC_SS_TCR_B_SIG		_T("SS-TCRAGC.AGC_B_AVG")
#define USC20_AGC_S_DET_A_SIG		_T("SS-TCRDTAGC.AGC_A_AVG")
#define USC20_AGC_S_DET_B_SIG		_T("SS-TCRDTAGC.AGC_B_AVG")
#define USC20_AGC_SX_TCR_A_SIG		_T("SX-TCRAGC.AGC_A_AVG")
#define USC20_AGC_SX_TCR_B_SIG		_T("SX-TCRAGC.AGC_B_AVG")
#define USC20_AGC_S_ACQTCR_A_SIG	_T("S-ACQTCRAGC.AGC_A_AVG")
#define USC20_AGC_S_ACQTCR_B_SIG	_T("S-ACQTCRAGC.AGC_B_AVG")
#define USC20_AGC_X_HRX_SIG			_T("X-HRXAGC.AGC_A_AVG")

// RNG
#define USC20_RNG_SS_SIG			_T("CTRL.AMB_SS_DATA_KM")
#define USC20_RNG_PRED_SIG			_T("RNG(PRED)")

// RR
#define USC20_RR_SS_SIG				_T("CTRL.DIST_SS_DATA")
#define USC20_RR_PRED_SIG			_T("RR(PRED)")

// EL
#define USC20_EL_EL_SIG				_T("ANT.REAL_EL")
#define USC20_EL_EL_PRED_SIG		_T("EL(PRED)")
#define USC20_EL_SKYLINE_SIG		_T("SKYLINE")
#define USC20_EL_RF_TRIP_SIG		_T("RF-TRIP")


// �q���ߑ��O���t�Ď��\����
// �P�c64m��
// AGC
#define UDSC64_AGC_SS_TCR_NAME		_T("SS-TCR")
#define UDSC64_AGC_X_RX2_NAME		_T("X-RX2")
#define UDSC64_AGC_XX_TCR_NAME		_T("XX-TCR")
#define UDSC64_AGC_XRX2_INTEG_NAME	_T("INTEG")
//#define UDSC64_AGC_XXTCR_INTEG_NAME	_T("XX-TCR(INTEG)")
#define UDSC64_AGC_XXTCR_INTEG_NAME	_T("INTEG")
//#define UDSC64_AGC_INTEG_NAME		_T("INTEG")

// RNG
#define UDSC64_RNG_SS_NAME			_T("S/S")
#define UDSC64_RNG_XX_NAME			_T("X/X")
#define UDSC64_RNG_XX_REPLAY_NAME	_T("X/X�Đ��^")
#define UDSC64_RNG_XX_TCR_NAME		_T("XX-TCR")
#define UDSC64_RNG_PRED_NAME		_T("PRED")

// RR
#define UDSC64_RR_SS_NAME			_T("S/S")
#define UDSC64_RR_XX_NAME			_T("X/X")
#define UDSC64_RR_XX_TCR_NAME		_T("XX-TCR")
#define UDSC64_RR_PRED_NAME			_T("PRED")

// EL
#define UDSC64_EL_EL_NAME			_T("EL")
#define UDSC64_EL_EL_PRED_NAME		_T("EL(PRED)")
#define UDSC64_EL_SKYLINE_NAME		_T("SKYLINE")
#define UDSC64_EL_RF_TRIP_NAME		_T("RF-TRIP")


// �P�c54m��
// AGC
#define XDSC54_AGC_SS_TCR_NAME		_T("SS-TCR")
#define XDSC54_AGC_X_RX2_NAME		_T("X-RX2")
#define XDSC54_AGC_XX_TCR_NAME		_T("XX-TCR")
#define XDSC54_AGC_DSNMODEM_NAME		_T("DSN-MODEM")
//#define XDSC54_AGC_XRX2_INTEG_NAME	_T("X-RX2(INTEG)")
//#define XDSC54_AGC_XXTCR_INTEG_NAME	_T("XX-TCR(INTEG)")
#define XDSC54_AGC_XXTCR_INTEG_NAME	_T("INTEG")
#define XDSC54_AGC_DSNMODEM_INTEG_NAME	_T("INTEG")
//#define XDSC54_AGC_INTEG_NAME		_T("INTEG")

// RNG
#define XDSC54_RNG_SS_NAME			_T("S/S")
#define XDSC54_RNG_XX_NAME			_T("X/X")
#define XDSC54_RNG_XX_REPLAY_NAME	_T("X/X�Đ��^")
#define XDSC54_RNG_XX_TCR_NAME		_T("XX-TCR")
#define XDSC54_RNG_DSNMODEM_X_NAME	_T("DSN-MODEM(X)")
#define XDSC54_RNG_DSNMODEM_KA_NAME	_T("DSN-MODEM(Ka)")
#define XDSC54_RNG_PRED_NAME		_T("PRED")

// RR
#define XDSC54_RR_SS_NAME			_T("S/S")
#define XDSC54_RR_XX_NAME			_T("X/X")
#define XDSC54_RR_XX_TCR_NAME		_T("XX-TCR")
#define XDSC54_RR_DSNMODEM_X_NAME	_T("DSN-MODEM(X)")
#define XDSC54_RR_DSNMODEM_KA_NAME	_T("DSN-MODEM(Ka)")
#define XDSC54_RR_PRED_NAME			_T("PRED")

// EL
#define XDSC54_EL_EL_NAME			_T("EL")
#define XDSC54_EL_EL_PRED_NAME		_T("EL(PRED)")
#define XDSC54_EL_SKYLINE_NAME		_T("SKYLINE")
#define XDSC54_EL_RF_TRIP_NAME		_T("RF-TRIP")


// ���V�Y34m��
// AGC
#define USC34_AGC_SS_TCR_A_NAME		_T("SS-TCR(A)")
#define USC34_AGC_SS_TCR_B_NAME		_T("SS-TCR(B)")
#define USC34_AGC_S_TRKRX_A_NAME	_T("S-TRKRX(A)")
#define USC34_AGC_S_TRKRX_B_NAME	_T("S-TRKRX(B)")
#define USC34_AGC_X_RX_A_NAME		_T("X-TRKRX(A)")
#define USC34_AGC_X_RX_B_NAME		_T("X-TRKRX(B)")
#define USC34_AGC_S_ACQRX_A_NAME	_T("S-ACQRX(A)")
#define USC34_AGC_S_ACQRX_B_NAME	_T("S-ACQRX(B)")
#define USC34_AGC_X_ACQRX_A_NAME	_T("X-ACQRX(A)")
#define USC34_AGC_X_ACQRX_B_NAME	_T("X-ACQRX(B)")
#define USC34_AGC_X_HRX_NAME		_T("X-HRX")
#define USC34_AGC_SHXX_TCR_NAME		_T("XX-TCR")
#define USC34_AGC_INTEG_NAME		_T("INTEG")

// RNG
#define USC34_RNG_SS_NAME			_T("S/S")
#define USC34_RNG_XX_NAME			_T("X/X")
#define USC34_RNG_PRED_NAME			_T("PRED")

// RR
#define USC34_RR_SS_NAME			_T("S/S")
#define USC34_RR_XX_NAME			_T("X/X")
#define USC34_RR_PRED_NAME			_T("PRED")

// EL
#define USC34_EL_EL_NAME			_T("EL")
#define USC34_EL_EL_PRED_NAME		_T("EL(PRED)")
#define USC34_EL_SKYLINE_NAME		_T("SKYLINE")
#define USC34_EL_RF_TRIP_NAME		_T("RF-TRIP")


// ���V�Y20m��
// AGC
#define USC20_AGC_SS_TCR_A_NAME		_T("SS-TCR(A)")
#define USC20_AGC_SS_TCR_B_NAME		_T("SS-TCR(B)")
#define USC20_AGC_S_DET_A_NAME		_T("S-DET(A)")
#define USC20_AGC_S_DET_B_NAME		_T("S-DET(B)")
#define USC20_AGC_SX_TCR_A_NAME		_T("SX-TCR(A)")
#define USC20_AGC_SX_TCR_B_NAME		_T("SX-TCR(B)")
#define USC20_AGC_S_ACQTCR_A_NAME	_T("S-ACQTCR(A)")
#define USC20_AGC_S_ACQTCR_B_NAME	_T("S-ACQTCR(B)")
#define USC20_AGC_X_HRX_NAME		_T("X-HRX")

// RNG
#define USC20_RNG_SS_NAME			_T("S/S")
#define USC20_RNG_PRED_NAME			_T("PRED")

// RR
#define USC20_RR_SS_NAME			_T("S/S")
#define USC20_RR_PRED_NAME			_T("PRED")

// EL
#define USC20_EL_EL_NAME			_T("EL")
#define USC20_EL_EL_PRED_NAME		_T("EL(PRED)")
#define USC20_EL_SKYLINE_NAME		_T("SKYLINE")
#define USC20_EL_RF_TRIP_NAME		_T("RF-TRIP")


// �T�}���O���t�Ď����ږ�
// �P�c64m��
// �q���ߑ��Ɋ܂܂�Ă���

// �P�c54m��
// �q���ߑ��Ɋ܂܂�Ă���

// ���V�Y34m��
// S-AGC
#define USC34_SAGC_TRKRXAGC_A_SIG	_T("S-ACQ_TRKRXAGC.AGC_A_AVG")
#define USC34_SAGC_TRKRXAGC_B_SIG	_T("S-ACQ_TRKRXAGC.AGC_B_AVG")

// X-AGC
#define USC34_XAGC_ACQ_TRKRXAGC_A_SIG	_T("X-ACQ_TRKRXAGC.AGC_A_AVG")
#define USC34_XAGC_ACQ_TRKRXAGC_B_SIG	_T("X-ACQ_TRKRXAGC.AGC_B_AVG")
#define USC34_XAGC_TRKRXAGC_A_SIG	_T("X-TRKRXAGC.AGC_A_AVG")
#define USC34_XAGC_TRKRXAGC_B_SIG	_T("X-TRKRXAGC.AGC_B_AVG")
#define USC34_AGC_XX_TCR_SIG		_T("DSNTCR_AGAV.AGC_A_AVG")

// ���V�Y20m��
// �q���ߑ��Ɋ܂܂�Ă���


// �ϕ��ʒm���b�Z�[�W
//#define NOTIFY_INTEG_MSG			_T("V-EQU.AGC_INTEG_TKSC")
#define NOTIFY_INTEG_MSG			_T("V-EQU.AGC_INTEG_")

#define LGR_SMP_NUM       4         /* �\��l��ԏ����T���v�����O��             */

//=============================================================================
//	�\��l�ꗗ
//=============================================================================
// �\��l�ꗗ�^�C�v
enum eForecastListType {
	eForecastListType_Forecast = 0,
	eForecastListType_Operation
};

//=============================================================================
//	�\��l, �q���ߑ��O���t
//=============================================================================
#define     V_C       (double)(299792458)       /* �����x (m/sec)                       */
#define     BAND_SS             (int)1          /* �r�^�r�@        ��ΏۂƂ���         */
#define     BAND_SX             (int)2          /* �r�^�w�@        ��ΏۂƂ���         */
#define     BAND_SSSX           (int)3          /* �r�^�r�{�r�^�w�@��ΏۂƂ���         */
#define     BAND_XX             (int)4          /* �w�^�w          ��ΏۂƂ���         */
#define     BAND_RX             (int)5          /* �w�^�w(�Đ��^)  ��ΏۂƂ���         *//* 2008-02-27 �Đ������Ή��@�ǉ� */
#define     BAND_DX_T           (int)6          /* �w�^�w          ��ΏۂƂ���         *//* <<2013-TURBO>> DSN-MODEM�Ή� */
#define     BAND_DX_R           (int)7          /* �w�^�w(�Đ��^)  ��ΏۂƂ���         *//* <<2013-TURBO>> DSN-MODEM�Ή� */

#define     RTT_BASE_UP         (int)1          /* �A�b�v�����N�����W���x�[�X��RTT�v�Z  */
#define     RTT_BASE_DOWN       (int)2          /* �_�E�������N�����W���x�[�X��RTT�v�Z  */

#define     INTERVAL_NEXT_PASS  (int)10         /* ANT�V���w�����{�𔻒f���鎞�ԁi���j  */

#define	SAMPLING_NUM		4     /* ��Ԃ����̃f�[�^�_���i�K��������ݒ肷�邱�Ɓj */
#define     OPT_DMY             (long)(0x0001)  /* �����o�͌`��:�g�p����                */
#define     FNC_DMY             ""              /* �@�\����    :�g�p����                */
#define     TYP_DMY             ""              /* �`������    :�g�p����                */

#define	PRED_TIME_LEN               8           /* �\��l�����^�O�o�C�g��               */
#define	PRED_FREQ_LEN               18          /* �\�����g���o�C�g��                   */
#define	PRED_RAMP_LEN               14          /* �����v���[�g�o�C�g��                 */
#define	PRED_DUMY_LEN               6           /* �_�~�[�f�[�^�o�C�g��                 */
#define	MAX_PRED_SIZE             60*60*24          /* �\��l�e�[�u���T�C�Y�i�b�j           */
#define	END_OF_TABLE        "***** END ****"    /*                                      */

// �\�����
#define  TYPE_NEW		0							// �v���Z�X�N�����̏����\��
#define  TYPE_TRND		1							// �����\����̃g�����h�\��

#define YOHOUTI_ARI		 1			/* �\��l�t�@�C���L��			  */
#define YOHOUTI_NASI	 0			/* �\��l�t�@�C���Ȃ�			  */
#define YOHOGET_MATI     1         /* �\��l�t�@�C���擾�҂�(V2-5)   */
#define YOHOGET_SUMI     2         /* �\��l�t�@�C���擾�ς�(V2-5)   */
#define YOHOGET_OK       3         /* �\��l�t�@�C���擾�\(V2-5)   */
#define GET_REAL         1         /* ���A���\�����ł�            �\��l�擾�v��(V2-5) */
#define GET_SET          2         /* �\��l�I���u�ݒ�v����      �\��l�擾�v��(V2-5) */
#define GET_DEF          3         /* �\��l�I���u�f�t�H���g�v���̗\��l�擾�v��(V2-5) */
#define COPY_ALL         0         /* �`����(�S��)          (V2-3) */
#define COPY_RIGHT       1         /* �`����(�ŉE�O���b�h)  (V2-3) */
#define YOHOU_RNG        0         /* �q�m�f�O���t�̗\��l�`��(V2-3) */
#define YOHOU_RR         1         /* �q�q  �O���t�̗\��l�`��(V2-3) */

//=============================================================================
//	�q���ߑ�����
//=============================================================================
#define CTRL_P_CAP_ANT   "CTRL.CAP_ANT"     /* ANT�Ҏ�w������                            */
#define CTRL_P_CAP_INIT  "CTRL.CAP_INIT"    /* �����ߑ�����                               */
#define CTRL_P_CAP_NORMAL "CTRL.CAP_NORMAL" /* ���ߑ�����                               */
#define CTRL_P_CAP_RX    "CTRL.CAP_RX_STR"      /* RX�_�E�������N�ߑ�����                     */
#define CTRL_P_CAP_UPLINK "CTRL.CAP_UPLINK_STR" /* �A�b�v�����N����                           */
#define CTRL_P_CAP_WAY   "CTRL.CAP_1WAY/2WAY_SET"	/* 1Way/2Way�ݒ�l                            */
													/* "2WAY","1WAY"                              */
#define CTRL_P_PLAN_FILE "CTRL.PLAN_FILE"   /* �W�J�ς݌v��t�@�C����                     */

#define CTRL_P_CAP_CMD	"CTRL.CAP_CMD_STR"	/* CMD���� */
#define CTRL_P_CAP_RNG	"CTRL.CAP_RNG_STR"	/* RNG���� */

enum eSatellieMode {
	eSatellieMode_AutoManual = 0,	/* �����ߑ�/�蓮�ߑ���� */
	eSatellieMode_AntStandBy,		/* ANT�҂��󂯎w����� */
	eSatellieMode_CMD,				/* CMD����ݒ� */
	eSatellieMode_RNG,				/* RNG����ݒ� */
	eSatellieMode_DownLink,			/* �_�E�������N����ݒ� */
	eSatellieMode_UpLink,			/* �A�b�v�����N����ݒ� */
	eSatellieMode_Way,				/* 1Way/2Way/Dup�ݒ� */
};


//=============================================================================
//	�t�H���_��`
//=============================================================================
// �t�@�C���p�X
//#define  YOLIST_FILE	_T("CONFIG\\opr\\etc\\pred_list.txt")		// �\��l�ꗗ�t�@�C���f�t�H���g�p�X
//#define  YOHOUTI_PATH	_T("CONFIG\\opr\\etc\\grph_yofile\\")		// �\��l�t�@�C���i�[�p�X
#define  YOHOUTI_NAME	_T("opgt_yofile")				// �\��l�t�@�C����
//#define  SKYLINE_FILE	_T("CONFIG\\opr\\etc\\ct_skyline.dat")	// �X�J�C���C���e�h�k�d��
//#define  RFTRIP_FILE	_T("CONFIG\\opr\\etc\\ct_rftrip.dat")		// �q�e�g���b�v�e�h�k�d��

//#define SPECTRUM_FILE	_T("DATA\\opr\\etc\\opgsproc.dat")	// �X�y�N�g�����e�h�k�d��
#define SPECTRUM_FILE	_T("opgsproc")	// �X�y�N�g�����e�h�k�d��
#define SPECTRUM_FILE_TYPE	_T("dat")	// �X�y�N�g�����e�h�k�d�g���q

#define TRENDPROP_FILE	_T("trendprop")	// �g�����h�ݒ�e�h�k�d��
#define TRENDPROP_FILE_TYPE	_T("dat")	// �g�����h�ݒ�e�h�k�d�g���q

#define DATA_DIR			_T("CONFIG")							// �f�[�^�p�X
#define PRED_LIST_FILE		_T("CONFIG\\prd\\pred_list.txt")		// �\��l�ꗗ�t�@�C���p�X
#define PRED_FILE_DIR		_T("CONFIG\\prd\\")						// �\��l�t�@�C���f�B���N�g��
#define PLAN_LIST_FILE		_T("CONFIG\\plan\\exec\\plan_list.txt")	// �v��l�ꗗ�t�@�C���p�X
#define PLAN_FORM_FILE_DIR	_T("CONFIG\\plan\\form\\")				// �v��l���`�t�@�C���f�B���N�g��
#define PLAN_FILE_DIR		_T("CONFIG\\plan\\exec\\")				// �v��l�t�@�C���f�B���N�g��
#define CID_FILE_DIR		_T("CONFIG\\cid\\")						// CID�t�@�C���f�B���N�g��
#if 1
#define CID_FILE			_T("cid.csv")							// CID�t�@�C����
#define CID_EXT				_T(".cid")								// CID�t�@�C����
#else
#define CID_FILE			_T("cid.txt")							// CID�t�@�C����
#endif

#define PRED_FILE_TYPE		_T("prd")								// �\��l�t�@�C���g���q
#define ANP_FILE_TYPE		_T("anp")								// �A���e�i�\��l�t�@�C���g���q
#define PLAN_FILE_TYPE		_T("pln")								// �v��t�@�C���g���q

#define TILL_TIME			"CTRL.PASS_CNTDOWN(DEC)"    /* �^�p�p�X�J�n�܂ł̎��� 			*/
#define PRED_FILE_NAME		"CTRL.PRED_FILE"            /* �\��l�t�@�C���p�X��   			*/
#define PRED_FILE_TIME		"CTRL.MAKE_PRED_TIME"       /* �\��l�t�@�C���쐬���� 			*/
#define EISEI_NAME			"CTRL.SAT_NAME"             /* �q����                 			*/
#define VEQU_EISEI_NAME		"V-EQU.BBE_SATNAME"         /* VEQU �q����             			*/
#define BAND_TYPE			"CTRL.BAND"                 /* �g�p���g���уt���O     			*/
#define PASS_START_TIME		"CTRL.START_TIME"           /* �p�X�J�n����           			*/
#define YOHOU_GET			"CTRL.PRED_COND"            /* �\��l�擾���          			*/
#define PRED_VISIBLE		"CTRL.VISIBLE"              /* �\��l������          			*/

#define PRED_GET_MODE		"CTRL.PRED_MODE"			/* �\��l�擾���[�h */
#define AUTO_STR			"AUTO"						/* �������[�h������ */
#define MANUAL_STR			"MANUAL"					/* �蓮���[�h������ */
#define STOP_STR			"STOP"						/* ��~���[�h������ */

// �p�X��
#define CTRL_PASS_INFO_NO			"CTRL.PASS_INFO"			// �p�X�i�s���
#define	CT_PASS_INFO_INIT			"�����ݒ�"					// �����ݒ�
#define	CT_PASS_INFO_1WAY			"�q���ߑ�(1WAY)"			// �P�v�`�x�ߑ���
#define	CT_PASS_INFO_2WAY			"�q���ߑ�(2WAY)"			// �Q�v�`�x�ߑ���
#define	CT_PASS_INFO_PASS_START1	"�^�p��(1WAY)"				// �p�X�J�n(1WAY)
#define	CT_PASS_INFO_PASS_START2	"�^�p��(2WAY)"				// �p�X�J�n(2WAY)
#define	CT_PASS_INFO_POL			"�V���ʉߊĎ��̈��"		// �V���ʉߊĎ��̈��
#define	CT_PASS_INFO_POSN			"ANT POSN MODE �ڍs�p���o"	// ANT POSN MODE �ڍs�p���o
#define	CT_PASS_INFO_EXTEND			"�^�p��(��������)"			// �p�X����


//=============================================================================
//	�ʒm�f�[�^���ʒl��`
//=============================================================================
#define		INFO_ID_DAT_INSEQU	(unsigned char)(0x01)	/* �ݔ��Ď��f�[�^						*/
#define		INFO_ID_DAT_CTLEQU	(unsigned char)(0x02)	/* �ݔ�����f�[�^						*/
#define		INFO_ID_ANS_CTLEQU	(unsigned char)(0x03)	/* �ݔ�����A���T�f�[�^ �e�ݔ��Ƃ�IF�p	*/
#define		INFO_ID_DAT_HISOPE	(unsigned char)(0x04)	/* �^�p�����f�[�^						*/
#define		INFO_ID_REQ_CHKREV	(unsigned char)(0x06)	/* ���r�W�����`�F�b�N�v��				*/
#define		INFO_ID_REQ_CHNGDB	(unsigned char)(0x07)	/* �f�[�^�x�[�X�X�V�v��					*/
#define		INFO_ID_CMD_SNDPRE	(unsigned char)(0x08)	/* �\��l���o�w��						*/
#define		INFO_ID_ANS_SNDPRE	(unsigned char)(0x09)	/* �\��l���o�A���T						*/
#define		INFO_ID_NOT_CPLPRE	(unsigned char)(0x0a)	/* �\��l���o�����ʒm					*/
#define		INFO_ID_NOT_CHGPST	(unsigned char)(0x0b)	/* �v��J�n�����ύX�ʒm					*/
#define		INFO_ID_NOT_CHGPED	(unsigned char)(0x0c)	/* �v��I�������ύX�ʒm					*/
#define		INFO_ID_ANS_CHGPST	(unsigned char)(0x0d)	/* �v��J�n�����ύX�A���T				*/
#define		INFO_ID_ANS_CHGPED	(unsigned char)(0x0e)	/* �v��I�������ύX�A���T				*/
#define		INFO_ID_NOT_PLNINF	(unsigned char)(0x0f)	/* �v��^�p���ʒm						*/

#define		INFO_ID_REQ_REGSDB	(unsigned char)(0x10)	/* �f�[�^�x�[�X�o�^�v��					*/
#define		INFO_ID_NOT_CPLRDB	(unsigned char)(0x11)	/* �f�[�^�x�[�X�o�^�����ʒm				*/
#define		INFO_ID_NOT_INIEQU	(unsigned char)(0x13)	/* �ݔ������ݒ�ʒm						*/
#define		INFO_ID_NOT_CHGPAS	(unsigned char)(0x14)	/* �p�X�ύX�ʒm							*/
#define		INFO_ID_NOT_STPPAS	(unsigned char)(0x15)	/* �p�X�����I���ʒm						*/
#define		INFO_ID_NOT_CHGBIT	(unsigned char)(0x16)	/* �r�b�g���[�g�ύX�ʒm					*/
/* 98.05.18 �폜
#define		INFO_ID_ANS_CTLEQ2	(unsigned char)(0x17) */ /* �ݔ�����A���T�f�[�^�i�q�s�Ő����j	*/
#define		INFO_ID_DAT_RNGDAT	(unsigned char)(0x31)	/* �q�m�f�f�[�^							*/
#define		INFO_ID_DAT_RRDATA	(unsigned char)(0x32)	/* �q  �q�f�[�^							*/

#define		INFO_ID_INF_INIEQU	(unsigned char)(0x40)	/* �ݔ������ݒ�ʒm�f�[�^�i�V�j			*/
#define		INFO_ID_RTN_INIEQU	(unsigned char)(0x41)	/* �ݔ������ݒ艞���f�[�^�i�V�j			*/
#define		INFO_ID_INF_CHGPAS	(unsigned char)(0x42)	/* �p�X�����ύX�ʒm�f�[�^�i�V�j			*/
#define		INFO_ID_RTN_CHGPAS	(unsigned char)(0x43)	/* �p�X�����ύX�����f�[�^�i�V�j			*/
#define		INFO_ID_INF_STPPAS	(unsigned char)(0x44)	/* �p�X�����I���ʒm�f�[�^�i�V�j			*/
#define		INFO_ID_RTN_STPPAS	(unsigned char)(0x45)	/* �p�X�����I�������f�[�^�i�V�j			*/
#define		INFO_ID_INF_STTCAL	(unsigned char)(0x46)	/* �Z���f�f�J�n�ʒm�f�[�^				*/
#define		INFO_ID_RTN_STTCAL	(unsigned char)(0x47)	/* �Z���f�f�J�n�����f�[�^				*/
#define		INFO_ID_INF_STPCAL	(unsigned char)(0x48)	/* �Z���f�f�I���ʒm�f�[�^				*/
#define		INFO_ID_RTN_STPCAL	(unsigned char)(0x49)	/* �Z���f�f�I�������f�[�^				*/
#define		INFO_ID_INF_LOCPTN	(unsigned char)(0x4a)	/* �R�����[�V����LOCK ON�p�^�[�����茋�ʒʒm�f�[�^*/

#define		INFO_ID_ANS_CTRLEQ	(unsigned char)(0x50)	/* ����A���T�f�[�^�ݔ����䑕�u�������� */

#define		INFO_ID_REQ_MNUGET	(unsigned char)(0x60)	/* �蓮�擾���[�h�v��					*/
#define		INFO_ID_ANS_MNUGET	(unsigned char)(0x61)	/* �蓮�擾���[�h����					*/
#define		INFO_ID_REQ_AUTGET	(unsigned char)(0x62)	/* �����擾���[�h�v��					*/
#define		INFO_ID_ANS_AUTGET	(unsigned char)(0x63)	/* �����擾���[�h����					*/
#define		INFO_ID_REQ_STPPRC	(unsigned char)(0x64)	/* �������f�v��							*/
#define		INFO_ID_ANS_STPPRC	(unsigned char)(0x65)	/* �������f����							*/
#define		INFO_ID_REQ_RSMPRC	(unsigned char)(0x66)	/* �����ĊJ�v��							*/
#define		INFO_ID_ANS_RSMPRC	(unsigned char)(0x67)	/* �����ĊJ����							*/
#define		INFO_ID_REQ_GETPRE	(unsigned char)(0x68)	/* �\��l�擾�v��						*/
#define		INFO_ID_NOT_GETPRE	(unsigned char)(0x69)	/* �\��l�擾���ʒʒm					*/
#define		INFO_ID_REQ_DELPRE	(unsigned char)(0x6a)	/* �\��l�폜�v��						*/
#define		INFO_ID_NOT_DELPRE	(unsigned char)(0x6b)	/* �\��l�폜���ʒʒm					*/
#define		INFO_ID_REQ_CHGHST	(unsigned char)(0x6c)	/* �\��l�擾�Ώۑ��u�w��v��			*/
#define		INFO_ID_ANS_CHGHST	(unsigned char)(0x6d)	/* �\��l�擾�Ώۑ��u�w�艞��			*/
#define		INFO_ID_REQ_TIMOFS	(unsigned char)(0x6e)	/* �����I�t�Z�b�g�ݒ�v��				*/
#define		INFO_ID_NOT_TIMOFS	(unsigned char)(0x6f)	/* �����I�t�Z�b�g�ݒ茋�ʒʒm			*/

#define		INFO_ID_REQ_PRIVIL	(unsigned char)(0x80)	/* �v�旧�Đ��䌠�v��					*/
#define		INFO_ID_ANS_PRIVIL	(unsigned char)(0x81)	/* �v�旧�Đ��䌠����					*/
#define		INFO_ID_REQ_PRI_RE	(unsigned char)(0x82)	/* �v�旧�Đ��䌠�J���v��				*/
#define		INFO_ID_ANS_PRI_RE	(unsigned char)(0x83)	/* �v�旧�Đ��䌠�J������				*/
#define		INFO_ID_REQ_PLN_UD	(unsigned char)(0x84)	/* �v��X�V�v��							*/
#define		INFO_ID_ANS_PLN_NE	(unsigned char)(0x85)	/* �v��X�V���ʉ����i�V�K�o�^�j			*/
#define		INFO_ID_ANS_PLN_DE	(unsigned char)(0x86)	/* �v��X�V���ʉ����i�폜�j				*/
#define		INFO_ID_ANS_PLN_UD	(unsigned char)(0x87)	/* �v��X�V���ʉ����i�X�V�j				*/
#define		INFO_ID_REQ_CHGOFS	(unsigned char)(0x88)	/* �����I�t�Z�b�g�ύX�v��				*/
#define		INFO_ID_ANS_CHGOFS	(unsigned char)(0x89)	/* �����I�t�Z�b�g�ύX�����ʒm			*/
#define		INFO_ID_REQ_PLN_MO	(unsigned char)(0x8a)	/* �v��ύX�v��							*/
#define		INFO_ID_ANS_CHGSTM	(unsigned char)(0x8b)	/* �v��ύX���ʉ���	�i�v��J�n�����ύX�j*/
#define		INFO_ID_ANS_CHGETM	(unsigned char)(0x8c)	/* �v��ύX���ʉ���	�i�v��I�������ύX�j*/
#define		INFO_ID_ANS_TERMIN	(unsigned char)(0x8d)	/* �v��ύX���ʉ���	�i�����I���j		*/
#define		INFO_ID_NOT_PREDUD	(unsigned char)(0x8e)	/* �\��l�X�V�ʒm						*/

#define		INFO_ID_DAT_SSRARR	(unsigned char)(0x90)	/* �r�^�r��  �ǐՃf�[�^					*/
#define		INFO_ID_DAT_SXRARR	(unsigned char)(0x91)	/* �r�^�w��  �ǐՃf�[�^					*/
#define		INFO_ID_DAT_XXRARR	(unsigned char)(0x92)	/* �w�^�w��  �ǐՃf�[�^					*/
#define		INFO_ID_DAT_FREQI 	(unsigned char)(0x93)	/* ���M���g����� (���ݒ�@�r�J) XUP	*/

#define		INFO_ID_REQ_ACQCAN	(unsigned char)(0xa0)	/* �����⑫���~�v��						*/
#define		INFO_ID_ANS_ACQCAN	(unsigned char)(0xa1)	/* �����⑫���~���ʉ���					*/
#define		INFO_ID_REQ_ACQMOD	(unsigned char)(0xa2)	/* �q���⑫���[�h�ݒ�v��				*/
#define		INFO_ID_ANS_ACQMOD	(unsigned char)(0xa3)	/* �q���⑫���[�h�ݒ茋�ʉ���			*/
#define		INFO_ID_NOT_RNGMON	(unsigned char)(0xb0)	/* �����W�`�����j�^�ʒm�f�[�^			*/
#define		INFO_ID_REQ_STPAUS	(unsigned char)(0xb8)	/* S�ђǐՃf�[�^���M��~�v��			*/
#define		INFO_ID_REQ_STRSND	(unsigned char)(0xb9)	/* S�ђǐՃf�[�^���M�ĊJ�v��			*/

#define		INFO_ID_REQ_INTEG	(unsigned char)(0xc1)	/* �ϕ����Ԏw��v��						*/


//=============================================================================
//	���L�t�H���_
//=============================================================================
static const TCHAR* mStationFolder[]{
	_T("udsc64"),
	_T("udsc54"),
	_T("usc34"),
	_T("usc20"),
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

	// ���M���t�@�C��
	eFileType_Diary_RFInfo_File,

	// ���M�t�@�C��
	eFileType_Diary_Rf_File,

	// �Ɩ������t�H�[�}�b�g�t�H���_
	eFileType_Diary_Form_Folder,

	// �Ɩ�����CSV�o�̓t�H���_
	eFileType_Diary_CSVOUT_Folder,

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
	eFileType_SeqCtrlDef_Folder,

	// ��������e���|�����t�H���_
	eFileType_SeqCtrlTmp_Folder,

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

	// �Ǔ��f�B���C�l�e�[�u���t�H���_
	eFileType_Delay_Folder,

	// �Z���f�f���ʃt�H���_
	eFileType_Calib_Result_Folder,

	// �^�p�v���p�e�B�t�H���_
	eFileType_OperationProp_Folder,

	// �v�掞�ԃt�@�C��
	eFileType_PlnTime_File,

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

	// ���M���t�@�C��
	_T("ctrl\\log\\pass\\rf_info_report.txt"),

	// ���M�t�@�C��
	_T("ctrl\\log\\pass\\rf_report.txt"),

	// �Ɩ������t�H�[�}�b�g�t�H���_
	_T("ctrl\\log\\pass\\form\\"),

	// �Ɩ�����CSV�o�̓t�H���_
	_T("ctrl\\log\\pass\\csv\\"),

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

	// ��������e���|�����t�H���_
	_T("cfg\\etc\\cont\\tmp\\"),

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

	// �Ǔ��f�B���C�l�e�[�u���t�H���_
	_T("ctrl\\delay\\"),

	// �Z���f�f���ʃt�H���_
	_T("ctrl\\log\\calib\\"),

	// �^�p�v���p�e�B�t�H���_
	_T("ctrl\\cfg\\etc\\"),

	// �v�掞�ԃt�@�C��
	_T("cfg\\etc\\PLN_TIME.txt"),

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
	_T("ctrl\\log\\mon\\"),

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

	// ���M���t�@�C��
	_T("ctrl\\log\\pass\\rf_info_report.txt"),

	// ���M�t�@�C��
	_T("ctrl\\log\\pass\\rf_report.txt"),

	// �Ɩ������t�H�[�}�b�g�t�H���_
	_T("ctrl\\log\\pass\\form\\"),

	// �Ɩ�����CSV�o�̓t�H���_
	_T("ctrl\\log\\pass\\csv"),

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

	// ��������e���|�����t�H���_
	_T("cfg\\etc\\cont\\tmp\\"),

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

	// �Ǔ��f�B���C�l�e�[�u���t�H���_
	_T("ctrl\\delay\\"),

	// �Z���f�f���ʃt�H���_
	_T("ctrl\\log\\calib\\"),

	// �^�p�v���p�e�B�t�H���_
	_T("ctrl\\cfg\\etc\\"),

	// �v�掞�ԃt�@�C��
	_T("cfg\\etc\\PLN_TIME.txt"),

	// �t�@�C���E�t�H���_��`��
	_T(""),
};

// �T�[�o���L�t�H���_
static const TCHAR* mSvShareFolder = _T("CONFIG\\share\\");


//=============================================================================
//	RF�g���b�v�p��`
//=============================================================================
// ���M�d�͎��ʍ���
#define XTX_HPA_SEL_SIG	_T("X-TX.HPA_SEL")
#define HPA1_RES		_T("HPA-1")
#define HPA2_RES		_T("HPA-2")

#define	XTX_HPA_RF_PWR_SIG	_T("X-TX.HPA_RF_PWR")
#define	XTX_HPA2_RF_PWR_SIG	_T("X-TX.HPA2_RF_PWR")

#define STX_RF_POWER_SEL	_T("S-TX.RF_POWER_SEL")
#define XTX_HPA_RF_PWR_CONT	_T("X-TX.HPA_RF_PWR_CONT")

#define STXRF_RF_POWER_SEL	_T("S-TXRF.RF_POWER_SEL")

#define PWR_25W			_T("25w")
#define PWR_100W		_T("100w")
#define PWR_200W		_T("200w")
#define PWR_1KW			_T("1kw")
#define PWR_2KW			_T("2kw")
#define PWR_20KW		_T("20kw")

//=============================================================================
//	RF�g���b�v�p��`�t�@�C��
//=============================================================================
#define RFTRIP_FILENAME					_T("ct_rftrip_%s.dat")		// RF�g���b�v�p��`�t�@�C����
#define RFTRIP_SECTION_INFO				_T("INFO")					// INFO�Z�N�V����
#define RFTRIP_KEY_KANSHI_POWER			_T("STSNAME_POWER")			// ���M�d�͎擾�p�Ď���
#define RFTRIP_KEY_KANSHI_MOD			_T("STSNAME_MOD")			// �ϒ��`���擾�p�Ď���
#define RFTRIP_KEY_MOD_SECTION_DEF		_T("STSDATA_MOD")			// �ʏ�Ƃ͈قȂ�ϒ��`����RF�g���b�v���w�肷��ꍇ�� �Ď��X�e�[�^�X��`��
#define RFTRIP_KEY_POWERINFO			_T("POWERINFO")				// ���M�d��(STSNAME_POWER)�Ŏ擾�����X�e�[�^�X
#define RFTRIP_SECTION_RFTRIP_GENE		_T("TRIP_*")				// �ϒ��`�� �u�ʏ�v�Z�N�V����
#define RFTRIP_SECTION_RFTRIP_MOD		_T("TRIP_")					// �ϒ��`�� �u�w��v�Z�N�V����

//=============================================================================
//	FA-Panel
//=============================================================================
static const TCHAR* mFapanelBlockFile[] =
{
	_T("RemoteCtrlTrm_UDSC64m.xml"),	// �P�c64
	_T("RemoteCtrlTrm_UDSC54m.xml"),	// �P�c54
	_T("RemoteCtrlTrm_USC34m.xml"),		// ���V�Y34
	_T("RemoteCtrlTrm_USC20m.xml"),		// ���V�Y20
};

#define FA_AUTH_FILE			_T("auth.txt")					// FA�p���䌠���t�@�C��

//=============================================================================
//	�����[�g�V�F����`
//=============================================================================
//#define SERVER_LOGINNAME		_T("sos")						// �T�[�o�[���O�C�����i�b��j
//#define SSHBATCHFILENAME		_T("remsh.bat")					// SSH�N���o�b�`�t�@�C��
#define SVSTART_SH				_T("start.sh")					// �T�[�o���V�F��
#define GET_RADIOLOG_SH			_T("GET_RADIOLOG.sh")			// �����Ɩ������擾
#define REGIST_RADIOLOG_SH		_T("REGIST_RADIOLOG.sh")		// �����Ɩ������o�^�v��
#define REGIST_OPEPROPERTY_SH	_T("REGIST_OPEPROPERTY.sh")		// �^�p�v���p�e�B�o�^�v��
#define REGIST_PLAN_SH			_T("REGIST_PLAN.ksh")			// �v��o�^�v��(2017.03.24 sh->ksh)
#define SEND_NOTICE_SH			_T("SEND_NOTICE.sh")			// �ʒm
#define SEND_CONTROL_SH			_T("SEND_CONTROL_UI.sh")		// �ݔ�����(UI)
#define SEND_CONTROL_NOUI_SH	_T("SEND_CONTROL.sh")			// �ݔ�����
#define AGCTBL_LOAD_SH			_T("agctbl_load.ksh")			// AGC�e�[�u�����[�h
#define RSYNC_SH				_T("RSYNC.sh")					// �t�@�C���]��
#define EMERGENCY_ANT_SH		_T("EMERGENCY_STOP_ANT.sh")		// �ً}��~�i�A���e�i�j
#define EMERGENCY_TX_SH			_T("EMERGENCY_STOP_TX.sh")		// �ً}��~�i���M�@�j
#define MONDUMP_SH				_T("MON_DUMP_PROC.sh")			// �Ď��f�[�^�_���v
#define AGCCALIB_GET_SH			_T("GET_CALIB.sh")				// �Z���f�f�pAGC�Z���t�@�C���擾
#define AGCCALIB_REQ_SH			_T("REQUEST_CALIB.sh")			// �Z���f�f���s�v��
#define CON_SDTP_SH				_T("CON_SDTP.sh")				// SDTP�Z�b�V�����ڑ�
#define DIS_SDTP_SH				_T("DIS_SDTP.sh")				// SDTP�Z�b�V�����ؒf
#define STAT_SDTP_SH			_T("STAT_SDTP.sh")				// SDTP�Z�b�V�����ڑ��󋵊m�F
#define SEND_CRM_SH				_T("SEND_CRM.sh")				// ���䌠���Ǘ��t�@�C���]��
#define FTP_GET_SH				_T("FTPGET.sh")					// FTP GET
#define FTP_PUT_SH				_T("FTPPUT.sh")					// FTP PUT
#define SEQ_CTRL_SH				_T("opccCtrl.sh")				// ��������
#define OPTION_AGCTBL_LOAD_SH	_T("agctbl_load_all.ksh")		// AGC�e�[�u�����[�h(�I�v�V�������j���[)
#define AGCTBL_LNASEL_SH		_T("AGCTBL_LNASEL")				// LNA1, LNA2�ؑ�

enum eRemShellType {
	eRemShellType_GET_RADIOLOG_SH = 0,
	eRemShellType_REGIST_RADIOLOG_SH,
	eRemShellType_REGIST_OPEPROPERTY_SH,
	eRemShellType_REGIST_PLAN_SH,
	eRemShellType_SEND_NOTICE_SH,
	eRemShellType_SEND_CONTROL_SH,
	eRemShellType_RSYNC_SH,
	eRemShellType_EMERGENCY_ANT_SH,
	eRemShellType_EMERGENCY_TX_SH,
	eRemShellType_MONDUMP_SH,
	eRemShellType_AGCCALIB_GET_SH,
	eRemShellType_AGCCALIB_REQ_SH,
	eRemShellType_CON_SDTP_SH,
	eRemShellType_DIS_SDTP_SH,
	eRemShellType_STAT_SDTP_SH,
	eRemShellType_SEND_CRM_SH,
	eRemShellType_FTP_GET_SH,
	eRemShellType_FTP_PUT_SH,
};

#define SSH_WAITTIME			45000							// SSH�҂�����(ms)
#define CYOHO_WAITTIME			20000							// ���[�҂�����(ms)

#define STAT_SDTP_FILENAME		_T("remsh_res.txt")				// �Z�b�V������Ԃ̌��ʊi�[�t�@�C��
#define STAT_SDTP_LOCKFILENAME	_T("remsh_res.loc")				// �Z�b�V������Ԃ̌��ʃ��b�N�t�@�C��

// �Z�b�V�����^�C�v������
static const TCHAR* msStrSessionType[] =
{
	_T("mon"),	// �Ď�
	_T("ctl"),	// ����
	_T("nti"),	// �ʒm
	_T("his"),	// ����
};

//=============================================================================
//	���������`
//=============================================================================
#define SEQCTRL_LINEPOS_FILE	_T("serialcont.EXEC")
#define SEQCTRL_PAUSE_FILE		_T("serialcont.PAUSE")
#define SEQCTRL_END_LINE		-9999

//=============================================================================
// TKSC,SSOC�z�X�g��
enum eHostName{
	eHost_sttn = 3,	// �n���(64m, 54m, 34m, 20m)
	eHost_ssoc,		// ���͌���
	eHost_tksc,		// �}�g��
};
// ���z�X�g���͎��ۂ̃z�X�g���̐擪�T����
static const TCHAR* mHostNameString[]{
	_T("u6opr"),	//�P�c64m�Ǔ�
	_T("u5opr"),	//�P�c54m�Ǔ�
	_T("u3opr"),	//���V�Y34m�Ǔ�
	_T("u2opr"),	//���V�Y20m�Ǔ�
	_T("szopr"),
	_T("tzopr"),
};

static const TCHAR* mAreaNameString[]{
	_T("STTN"),		// �n���
	_T("SSOC"),		// ���͌���
	_T("TKSC"),		// �}�g��
};

//=============================================================================
//	�Ď�����`
//=============================================================================
#define CTRL_P_SAT	_T("V-EQU.BBE_SATNAME")

#define RNG_DENOMINATOR_NUM	_T("V-EQU.RNG_NUM")		// RNG�̕��ꐔ

//=============================================================================
//	�Z���f�f �����I���e�[�u��
//=============================================================================
typedef struct
{
	CString param;
	CString ctrl;
}stRdnsCancelst;

//----------------
// 64m
// AGC S��
static const stRdnsCancelst m_RdnsCancel64AgcSTbl[] =
{
	{ _T("S-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL S-RX_A,%s;"), },
	{ _T(""),					_T(""), },
};
// AGC X��
static const stRdnsCancelst m_RdnsCancel64AgcXTbl[] =
{
	{ _T("X-RX2"),				_T("CTRL.RDYCHK_AGC_CANCEL X-RX2_A,%s;"), },
	{ _T("XX-TCR"),				_T("CTRL.RDYCHK_AGC_CANCEL X2-TCR_A,%s;"), },
	{ _T(""), _T(""), },
};
// RNG
static const stRdnsCancelst m_RdnsCancel64RngTbl[] =
{
	{ _T("S/S"),				_T("CTRL.RDYCHK_RNG_CANCEL SS,%s;"), },
	{ _T("S/X"),				_T("CTRL.RDYCHK_RNG_CANCEL SX,%s;"), },
	{ _T("X/X"),				_T("CTRL.RDYCHK_RNG_CANCEL XX,%s;"), },
	{ _T("X/X(�Đ��^)"),		_T("CTRL.RDYCHK_RNG_CANCEL RX,%s;"), },
	{ _T("X/X_DSN"),			_T("CTRL.RDYCHK_RNG_CANCEL DX,%s;"), },
	{ _T("X/X_DSN(�Đ��^)"),	_T("CTRL.RDYCHK_RNG_CANCEL DR,%s;"), },
	{ _T(""), _T(""), },
};
// TLM S��
static const stRdnsCancelst m_RdnsCancel64TlmSTbl[] =
{
	{ _T("SS-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL S,%s;"), },
	{ _T(""), _T(""), },
};
// TLM X��
static const stRdnsCancelst m_RdnsCancel64TlmXTbl[] =
{
	{ _T("SX-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL X,%s;"), },
	{ _T("XX-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL X2,%s;"), },
	{ _T(""), _T(""), },
};
// CMD
static const stRdnsCancelst m_RdnsCancel64CmdTbl[] =
{
	{ _T("CMDOUT2"),			_T("CTRL.RDYCHK_CMD_CANCEL 2,%s;"), },
	{ _T("CMDOUT3"),			_T("CTRL.RDYCHK_CMD_CANCEL 3,%s;"), },
	{ _T("X-CMDOUT2"),			_T("CTRL.RDYCHK_CMD_CANCEL 4,%s;"), },
	{ _T(""), _T(""), },
};

//----------------
// 54m
// AGC X��
static const stRdnsCancelst m_RdnsCancel54AgcXTbl[] =
{
	{ _T("DSN_TCR"), _T("CTRL.RDYCHK_AGC_CANCEL DSN-MODEM_X_A,%s;"), },
	{ _T(""), _T(""), },
};
// AGC Ka��
static const stRdnsCancelst m_RdnsCancel54AgcKaTbl[] =
{
	{ _T("DSN_TCR"), _T("CTRL.RDYCHK_AGC_CANCEL DSN-MODEM_KA_A,%s;"), },
	{ _T(""), _T(""), },
};
// RNG
static const stRdnsCancelst m_RdnsCancel54RngTbl[] =
{
	{ _T("X/X(�]��)"), _T("CTRL.RDYCHK_RNG_CANCEL DXN,%s;"), },
	{ _T("X/X(�Đ�)"), _T("CTRL.RDYCHK_RNG_CANCEL DXR,%s;"), },
	{ _T("X/Ka(�]��)"), _T("CTRL.RDYCHK_RNG_CANCEL DKAN,%s;"), },
	{ _T("X/Ka(�Đ�)"), _T("CTRL.RDYCHK_RNG_CANCEL DKAR,%s;"), },
	{ _T(""), _T(""), },
};
// TLM X��
static const stRdnsCancelst m_RdnsCancel54TlmXTbl[] =
{
	{ _T("DSN_TCR"), _T("CTRL.RDYCHK_TLM_CANCEL X,%s;"), },
	{ _T(""), _T(""), },
};
// TLM Ka��
static const stRdnsCancelst m_RdnsCancel54TlmKaTbl[] =
{
	{ _T("DSN_TCR"), _T("CTRL.RDYCHK_TLM_CANCEL KA,%s;"), },
	{ _T(""), _T(""), },
};
// CMD
static const stRdnsCancelst m_RdnsCancel54CmdTbl[] =
{
	{ _T("X-CMDOUT2"), _T("CTRL.RDYCHK_CMD_CANCEL 1,%s;"), },
	{ _T(""), _T(""), },
};

//----------------
// 34m
// AGC S��
static const stRdnsCancelst m_RdnsCancel34AgcSTbl[] =
{
	{ _T("SS-TCR"),				_T("CTRL.RDYCHK_AGC_CANCEL SS-TCR_AB,%s;"), },
	{ _T("S-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL S-TRKRX_AB,%s;"), },
	{ _T("SX-ACQRX_S"),			_T("CTRL.RDYCHK_AGC_CANCEL S-ACQRX_AB,%s;"), },
	{ _T("SX-ACQRX_X"),			_T("CTRL.RDYCHK_AGC_CANCEL X-ACQRX_AB,%s;"), },
	{ _T(""), _T(""), },
};
// AGC X��
static const stRdnsCancelst m_RdnsCancel34AgcXTbl[] =
{
	{ _T("X-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL X-TRKRX_AB,%s;"), },
	{ _T("X-ACQRX"),			_T("CTRL.RDYCHK_AGC_CANCEL X-ACQRX_AB,%s;"), },
	{ _T("X-QPSKRX"),			_T("CTRL.RDYCHK_AGC_CANCEL X-QPSKRX_A,%s;"), },
	{ _T("XX-TCR"),				_T("CTRL.RDYCHK_AGC_CANCEL X2-TCR_A,%s;"), },
	{ _T(""), _T(""), },
};
// RNG
static const stRdnsCancelst m_RdnsCancel34RngTbl[] =
{
	{ _T("S/S"),				_T("CTRL.RDYCHK_RNG_CANCEL SS,%s;"), },
	{ _T("S/X"),				_T("CTRL.RDYCHK_RNG_CANCEL SX,%s;"), },
	{ _T("X/X_DSN"),			_T("CTRL.RDYCHK_RNG_CANCEL DX,%s;"), },
	{ _T("X/X_DSN(�Đ��^)"),	_T("CTRL.RDYCHK_RNG_CANCEL DR,%s;"), },
	{ _T(""), _T(""), },
};
// TLM S��
static const stRdnsCancelst m_RdnsCancel34TlmSTbl[] =
{
	{ _T("SS-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL S,%s;"), },
	{ _T(""), _T(""), },
};
// TLM X��
static const stRdnsCancelst m_RdnsCancel34TlmXTbl[] =
{
	{ _T("XX-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL X2,%s;"), },
	{ _T("X-QPSKRX"),			_T("CTRL.RDYCHK_TLM_CANCEL QPSK,%s;"), },
	{ _T(""), _T(""), },
};
// CMD
static const stRdnsCancelst m_RdnsCancel34CmdTbl[] =
{
	{ _T("S-CMDOUT"),			_T("CTRL.RDYCHK_CMD_CANCEL 1,%s;"), },
	{ _T("X-CMDOUT2"),			_T("CTRL.RDYCHK_CMD_CANCEL 3,%s;"), },
	{ _T(""), _T(""), },
};

//----------------
// 20m
// AGC S��
static const stRdnsCancelst m_RdnsCancel20AgcSTbl[] =
{
	{ _T("SX-ACQRX_S"),			_T("CTRL.RDYCHK_AGC_CANCEL SX-ACQRX_S_A,%s;"), },
	{ _T("S-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL S-RX_AB,%s;"), },
	{ _T("SS-TCRDET"),			_T("CTRL.RDYCHK_AGC_CANCEL SS-TCRDET_AB,%s;"), },
	{ _T(""), _T(""), },
};
// AGC X��
static const stRdnsCancelst m_RdnsCancel20AgcXTbl[] =
{
	{ _T("X-RX"),				_T("CTRL.RDYCHK_AGC_CANCEL X-RX_AB,%s;"), },
	{ _T("X-QPSKRX"),			_T("CTRL.RDYCHK_AGC_CANCEL X-QPSKRX_A,%s;"), },
	{ _T(""), _T(""), },
};
// RNG
static const stRdnsCancelst m_RdnsCancel20RngTbl[] =
{
	{ _T("S/S"),				_T("CTRL.RDYCHK_RNG_CANCEL SS,%s;"), },
	{ _T("S/X"),				_T("CTRL.RDYCHK_RNG_CANCEL SX,%s;"), },
	{ _T(""), _T(""), },
};
// TLM S��
static const stRdnsCancelst m_RdnsCancel20TlmSTbl[] =
{
	{ _T("SS-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL S,%s;"), },
	{ _T(""), _T(""), },
};
// TLM X��
static const stRdnsCancelst m_RdnsCancel20TlmXTbl[] =
{
	{ _T("SX-TCR"),				_T("CTRL.RDYCHK_TLM_CANCEL X,%s;"), },
	{ _T("X-QPSKRX"),			_T("CTRL.RDYCHK_TLM_CANCEL QPSK,%s;"), },
	{ _T(""), _T(""), },
};
// CMD
static const stRdnsCancelst m_RdnsCancel20CmdTbl[] =
{
	{ _T("CMDOUT1"),			_T("CTRL.RDYCHK_CMD_CANCEL 1,%s;"), },
	{ _T(""), _T(""), },
};

//=============================================================================
//	�戵��������
//=============================================================================
#define DOCUMENT_FILENAME		_T("StationControl.pdf")

//=============================================================================
//	���[�A�v����
//=============================================================================
#define CYOHYO_APPNAME			_T("Excel���[�쐬.exe")

//=============================================================================
//	�}�g�ǉ^�p�ǐ����u���O
//=============================================================================
enum eTkscLog {
	eTkscLog_Ope = 100,
	eTkscLog_Ctl = 101,
};

//=============================================================================
//	�v���p�e�B
//=============================================================================
#define	AXIS_SCALE_MIN			2					// �ڐ���̕������̍ŏ��l
#define	AXIS_SCALE_MAX			10					// �ڐ���̕������̍ő�l

//=============================================================================
//	�q���I���e�[�u���^�C�v
//=============================================================================
enum eSatSelTblType {
	eSatSelTblType_AgcTblRead,
	eSatSelTblType_AgcTblCopy,
	eSatSelTbltype_AgcTblRecovery,
	eSatSelTbltype_SatDlyTblcopy,
	eSatSelTbltype_SatDlyTblRecovery,
};

static const TCHAR* mSatSelTblTitleString[]{
	_T("AGC�Z���e�[�u���Ǎ��E�q���I��"),
	_T("AGC�Z���e�[�u���t�@�C���R�s�[�E�q���I��"),
	_T("AGC�Z���e�[�u���t�@�C�������E�q���I��"),
	_T("�Ǔ��f�B���C�l�e�[�u���t�@�C���R�s�[�E�q���I��"),
	_T("�Ǔ��f�B���C�l�e�[�u�������E�q���I��"),
};

static const TCHAR* mSatSelTblMsgString[]{
	_T("AGC�Z���e�[�u���t�@�C���ǂݍ��ݑΏۂ̉q����I�����Ă��������B"),
	_T("AGC�Z���e�[�u���t�@�C���R�s�[�Ώۂ̉q����I�����Ă��������B"),
	_T("AGC�Z���e�[�u���t�@�C�������Ώۂ̉q����I�����Ă��������B"),
	_T("�Ǔ��f�B���C�l�e�[�u���t�@�C���R�s�[�Ώۂ̉q����I�����Ă��������B"),
	_T("�Ǔ��f�B���C�l�e�[�u�������Ώۂ̉q����I�����Ă��������B"),
};


//=============================================================================
//	�ݔ��ڍא���
//=============================================================================
typedef struct
{
	CString strEquipment;
	long	nType;
	CString strRang;
	CString	dRangeCur;
	double	dRangeMin;
	double	dRangeMax;
	long	l_incolflt;	/* ���͌���: �������i���a�S�j�i���a�T�j*/
} stEquipment2Ctrl;		// �ݔ��ڍא���Q

#ifdef _DEBUG_KANSHI64M
// 64m�Ď�	���̐��䖼
#define RA_OFFSET	_T("ANT.AZ_OFFSET")
#define DEC_OFFSET	_T("ANT.EL_OFFSET")
#else
// 54m�Ď�
#define RA_OFFSET	_T("ANT.RA_OFFSET")
#define DEC_OFFSET	_T("ANT.DEC_OFFSET")
#endif

//=============================================================================
//	PRED DIFF
//=============================================================================
typedef struct
{
	double	t;			// ����
	double	data;		// �f�[�^
} stPredData;

#define DEFAULT_PREDDIFFMIN	_T("10")					// �f�t�H���g�̍ŏ�PRED DIFF�Ԋu(54m)

//=============================================================================
//	�������ʔԍ�(54m)
//=============================================================================
//#define DEFAULT_HISTID54	_T("0x010a010a")			// 54m �������ʔԍ��f�t�H���g�l
#define DEFAULT_HISTID54	_T("0x010a0110")			// 54m �������ʔԍ��f�t�H���g�l


//=============================================================================
//	X�я]���AX�эĐ��AKa�я]���AKa�эĐ��ؑ�
//=============================================================================
enum eXmeasBandType {
	eXmeasBandType_X,
	eXmeasBandType_Ka,
	eXmeasBandType_Num,
};

enum eXmeasRngMode {
	eXmeasRngMode_Turnaround,
	eXmeasRngMode_Regen,
	eXmeasRngMode_Num,
};

static const TCHAR* mBandTypeString[eXmeasBandType_Num] = {
	_T("X"),
	_T("Ka"),
};

static const TCHAR* mRngModeString[eXmeasRngMode_Num] = {
	_T("TURNAROUND"),
	_T("REGEN"),
};

#define JUDGE_BANDTYPE_STR		_T("CTRL.BAND")
#define JUDGE_RNGMODE_STR		_T("DSNTCR.RNG_MODE")

//=============================================================================
//	�Z���f�f�v��@�^�C���A�E�g����(�b)
//=============================================================================
#define RDNSCHKPLAN_FORCESTOP_TIMEOUT	30
