#pragma once

#include <list>
#include <map>
#include <vector>
#include "const.h"

using namespace std;

static const CString mBaseStationString[] = {
	_T("UDSC64"),
	_T("USC20"),
	_T("USC34"),
	_T("UDSC54"),
};
static const CString mEquipmentString[] = {
	_T("ANT"),
	_T("BITRATE"),
	_T("CMD"),
	_T("CTRL"),
};
static const CString mDataNameString[] = {
	_T("AZ_DRIVE"),
	_T("AZ_OFFSET"),
	_T("DRIVE_MODE"),
	_T("EL_DRIVE"),
	_T("EL_OFFSET"),
};
static const CString mDataUnitString[] = {
	_T("Hz"),
	_T("V"),
	_T("A"),
	_T("W"),
	_T("℃"),
};


// 衛星捕捉グラフ用の局.設備.項目名
static structSatelliteCaptureSig mListSatelliteCaptureSig[eStation_MAX][eTrendWndType_Num][MAX_TDATA + 1] = {
	// 臼田64m
	{
		// AGC
		{
			// SS-TCR
			{
				eSigSrcType_EngExchange,
				UDSC64_AGC_SS_TCR_SIG,
				UDSC64_AGC_SS_TCR_NAME,
				_T("dBm"),
				3,
				2
			},
			// X-RX2
			{
				eSigSrcType_EngExchange,
				UDSC64_AGC_X_RX2_SIG,
				UDSC64_AGC_X_RX2_NAME,
				_T("dBm"),
				3,
				2
			},
			// X-RX2 INTEG
			{
				eSigSrcType_EngExchange,
				UDSC64_AGC_XRX2_INTEG_SIG,
				UDSC64_AGC_XRX2_INTEG_NAME,
				_T("dBm"),
				3,
				2
			},
			// XX-TCR
			{
				eSigSrcType_EngExchange,
				UDSC64_AGC_XX_TCR_SIG,
				UDSC64_AGC_XX_TCR_NAME,
				_T("dBm"),
				3,
				2
			},
			// XX-TCR INTEG
			{
				eSigSrcType_EngExchange,
				UDSC64_AGC_XXTCR_INTEG_SIG,
				UDSC64_AGC_XXTCR_INTEG_NAME,
				_T("dBm"),
				3,
				2
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RNG
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				UDSC64_RNG_SS_SIG,
				UDSC64_RNG_SS_NAME,
				_T("km"),
				13,
				//++ 20181023 UUN-18-074確認用 >>>
//#ifndef _DEBUG_STEP_TEST_WAVE
				0
//#else
//				6
//#endif
			},
			// X/X
			{
				eSigSrcType_EngExchange,
				UDSC64_RNG_XX_SIG,
				UDSC64_RNG_XX_NAME,
				_T("km"),
				13,
				//++ 20181023 UUN-18-074確認用 >>>
//#ifndef _DEBUG_STEP_TEST_WAVE
				0
//#else
//				6
//#endif
			},
			// X/X再生型
			{
				eSigSrcType_EngExchange,
				UDSC64_RNG_XX_REPLAY_SIG,
				UDSC64_RNG_XX_REPLAY_NAME,
				_T("km"),
				13,
				//++ 20181023 UUN-18-074確認用 >>>
//#ifndef _DEBUG_STEP_TEST_WAVE
				0
//#else
//				6
//#endif
			},
			// XX-TCR
			{
				eSigSrcType_EngExchange,
				UDSC64_RNG_XX_TCR_SIG,
				UDSC64_RNG_XX_TCR_NAME,
				_T("km"),
				13,
//++ 20181023 UUN-18-074確認用 >>>
//#ifndef _DEBUG_STEP_TEST_WAVE
				0
//#else
//				6
//#endif
			},
			// RNG(PRED) 予報値
			{
				eSigSrcType_File,
				UDSC64_RNG_PRED_SIG,
				UDSC64_RNG_PRED_NAME,
				_T("km"),
				13,
				//++ 20181023 UUN-18-074確認用 >>>
//#ifndef _DEBUG_STEP_TEST_WAVE
				0
//#else
//				6
//#endif
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RR
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				UDSC64_RR_SS_SIG,
				UDSC64_RR_SS_NAME,
				_T("m/s"),
				13,
				0
			},
			// X/X
			{
				eSigSrcType_EngExchange,
				UDSC64_RR_XX_SIG,
				UDSC64_RR_XX_NAME,
				_T("m/s"),
				13,
				0
			},
			// X/X-TCR
			{
				eSigSrcType_EngExchange,
				UDSC64_RR_XX_TCR_SIG,
				UDSC64_RR_XX_TCR_NAME,
				_T("m/s"),
				13,
				0
			},
			// RR(PRED) 予報値
			{
				eSigSrcType_File,
				UDSC64_RR_PRED_SIG,
				UDSC64_RR_PRED_NAME,
				_T("m/s"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// EL
		{
			// EL
			{
				eSigSrcType_EngExchange,
				UDSC64_EL_EL_SIG,
				UDSC64_EL_EL_NAME,
				_T("deg"),
				3,
				3
			},
			// EL(PRED) 予報値
			{
				eSigSrcType_File,
				UDSC64_EL_EL_PRED_SIG,
				UDSC64_EL_EL_PRED_NAME,
				_T("deg"),
				3,
				3
			},
			// SKYLINE
			{
				eSigSrcType_File,
				UDSC64_EL_SKYLINE_SIG,
				UDSC64_EL_SKYLINE_NAME,
				_T("deg"),
				3,
				3
			},
			// RF-TRIP
			{
				eSigSrcType_File,
				UDSC64_EL_RF_TRIP_SIG,
				UDSC64_EL_RF_TRIP_NAME,
				_T("deg"),
				3,
				3
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
	},

	// 臼田54m
	{
		// AGC
		{
			// DSN-MODEM
			{
				eSigSrcType_EngExchange,
				XDSC54_AGC_DSNMODEM_SIG,
				XDSC54_AGC_DSNMODEM_NAME,
				_T("dBm"),
				3,
				2
			},
			// DSN-MODEM INTG
			{
				eSigSrcType_EngExchange,
				XDSC54_AGC_DSNMODEM_INTEG_SIG,
				XDSC54_AGC_DSNMODEM_INTEG_NAME,
				_T("dBm"),
				3,
				2
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RNG
		{
			// DSN-MODEM(X)
			{
				eSigSrcType_EngExchange,
				XDSC54_RNG_DSNMODEM_X_SIG,
				XDSC54_RNG_DSNMODEM_X_NAME,
				_T("km"),
				13,
				0
			},
			// DSN-MODEM(Ka)
			{
				eSigSrcType_EngExchange,
				XDSC54_RNG_DSNMODEM_KA_SIG,
				XDSC54_RNG_DSNMODEM_KA_NAME,
				_T("km"),
				13,
				0
			},
			// RNG(PRED) 予報値
			{
				eSigSrcType_File,
				XDSC54_RNG_PRED_SIG,
				XDSC54_RNG_PRED_NAME,
				_T("km"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RR
		{
			// DSN-MODEM(X)
			{
				eSigSrcType_EngExchange,
				XDSC54_RR_DSNMODEM_X_SIG,
				XDSC54_RR_DSNMODEM_X_NAME,
				_T("m/s"),
				13,
				0
			},
			// DSN-MODEM(Ka)
			{
				eSigSrcType_EngExchange,
				XDSC54_RR_DSNMODEM_KA_SIG,
				XDSC54_RR_DSNMODEM_KA_NAME,
				_T("m/s"),
				13,
				0
			},
			// RR(PRED) 予報値
			{
				eSigSrcType_File,
				XDSC54_RR_PRED_SIG,
				XDSC54_RR_PRED_NAME,
				_T("m/s"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// EL
		{
			// EL
			{
				eSigSrcType_EngExchange,
				XDSC54_EL_EL_SIG,
				XDSC54_EL_EL_NAME,
				_T("deg"),
				3,
				3
			},
			// EL(PRED) 予報値
			{
				eSigSrcType_File,
				XDSC54_EL_EL_PRED_SIG,
				XDSC54_EL_EL_PRED_NAME,
				_T("deg"),
				3,
				3
			},
			// SKYLINE
			{
				eSigSrcType_File,
				XDSC54_EL_SKYLINE_SIG,
				XDSC54_EL_SKYLINE_NAME,
				_T("deg"),
				3,
				3
			},
			// RF-TRIP
			{
				eSigSrcType_File,
				XDSC54_EL_RF_TRIP_SIG,
				XDSC54_EL_RF_TRIP_NAME,
				_T("deg"),
				3,
				3
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
	},

	// 内之浦34m
	{
		// AGC
		{
			// SS-TCR(A)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_SS_TCR_A_SIG,
				USC34_AGC_SS_TCR_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// SS-TCR(B)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_SS_TCR_B_SIG,
				USC34_AGC_SS_TCR_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// S-TRKRX(A)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_S_TRKRX_A_SIG,
				USC34_AGC_S_TRKRX_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// S-TRXRX(B)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_S_TRKRX_B_SIG,
				USC34_AGC_S_TRKRX_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// X-RX(A)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_X_RX_A_SIG,
				USC34_AGC_X_RX_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// X-RX(B)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_X_RX_B_SIG,
				USC34_AGC_X_RX_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// S-ACQRX(A)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_S_ACQRX_A_SIG,
				USC34_AGC_S_ACQRX_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// S-ACQRX(B)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_S_ACQRX_B_SIG,
				USC34_AGC_S_ACQRX_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// X-ACQRX(B)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_X_ACQRX_A_SIG,
				USC34_AGC_X_ACQRX_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// X-ACQRX(B)
			{
				eSigSrcType_EngExchange,
				USC34_AGC_X_ACQRX_B_SIG,
				USC34_AGC_X_ACQRX_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// X-HRX
			{
				eSigSrcType_EngExchange,
				USC34_AGC_X_HRX_SIG,
				USC34_AGC_X_HRX_NAME,
				_T("dBm"),
				3,
				2
			},
			// XX-TCR
			{
				eSigSrcType_EngExchange,
				USC34_AGC_SHXX_TCR_SIG,
				USC34_AGC_SHXX_TCR_NAME,
				_T("dBm"),
				3,
				2
			},
			// INTG
			{
				eSigSrcType_EngExchange,
				USC34_AGC_INTEG_SIG,
				USC34_AGC_INTEG_NAME,
				_T("dBm"),
				3,
				2
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RNG
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				USC34_RNG_SS_SIG,
				USC34_RNG_SS_NAME,
				_T("km"),
				13,
				0
			},
			// S/X
			{
				eSigSrcType_EngExchange,
				USC34_RNG_XX_SIG,
				USC34_RNG_XX_NAME,
				_T("km"),
				13,
				0
			},
			// PRED 予報値
			{
				eSigSrcType_File,
				USC34_RNG_PRED_SIG,
				USC34_RNG_PRED_NAME,
				_T("km"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RR
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				USC34_RR_SS_SIG,
				USC34_RR_SS_NAME,
				_T("m/s"),
				13,
				0
			},
			// X/X
			{
				eSigSrcType_EngExchange,
				USC34_RR_XX_SIG,
				USC34_RR_XX_NAME,
				_T("m/s"),
				13,
				0
			},
			// RR(PRED) 予報値
			{
				eSigSrcType_File,
				USC34_RR_PRED_SIG,
				USC34_RR_PRED_NAME,
				_T("m/s"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// EL
		{
			// EL
			{
				eSigSrcType_EngExchange,
				USC34_EL_EL_SIG,
				USC34_EL_EL_NAME,
				_T("deg"),
				3,
				3
			},
			// EL(PRED) 予報値
			{
				eSigSrcType_File,
				USC34_EL_EL_PRED_SIG,
				USC34_EL_EL_PRED_NAME,
				_T("deg"),
				3,
				3
			},
			// SKYLINE
			{
				eSigSrcType_File,
				USC34_EL_SKYLINE_SIG,
				USC34_EL_SKYLINE_NAME,
				_T("deg"),
				3,
				3
			},
			// RF-TRIP
			{
				eSigSrcType_File,
				USC34_EL_RF_TRIP_SIG,
				USC34_EL_RF_TRIP_NAME,
				_T("deg"),
				3,
				3
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
	},

	// 内之浦20m
	{
		// AGC
		{
			// SS-TCR(A)
			{
				eSigSrcType_EngExchange,
				USC20_AGC_SS_TCR_A_SIG,
				USC20_AGC_SS_TCR_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// SS-TCR(B)
			{
				eSigSrcType_EngExchange,
				USC20_AGC_SS_TCR_B_SIG,
				USC20_AGC_SS_TCR_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// S-DET(A)
			{
				eSigSrcType_EngExchange,
				USC20_AGC_S_DET_A_SIG,
				USC20_AGC_S_DET_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// S-DET(B)
			{
				eSigSrcType_EngExchange,
				USC20_AGC_S_DET_B_SIG,
				USC20_AGC_S_DET_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// SX-TCR(A)
			{
				eSigSrcType_EngExchange,
				USC20_AGC_SX_TCR_A_SIG,
				USC20_AGC_SX_TCR_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// SX-TCR(B)
			{
				eSigSrcType_EngExchange,
				USC20_AGC_SX_TCR_B_SIG,
				USC20_AGC_SX_TCR_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// S-ACQTCR(A)
			{
				eSigSrcType_EngExchange,
				USC20_AGC_S_ACQTCR_A_SIG,
				USC20_AGC_S_ACQTCR_A_NAME,
				_T("dBm"),
				3,
				2
			},
			// S-ACQTCR(B)
			{
				eSigSrcType_EngExchange,
				USC20_AGC_S_ACQTCR_B_SIG,
				USC20_AGC_S_ACQTCR_B_NAME,
				_T("dBm"),
				3,
				2
			},
			// X-HRX
			{
				eSigSrcType_EngExchange,
				USC20_AGC_X_HRX_SIG,
				USC20_AGC_X_HRX_NAME,
				_T("dBm"),
				3,
				2
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RNG
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				USC20_RNG_SS_SIG,
				USC20_RNG_SS_NAME,
				_T("km"),
				13,
				0
			},
			// PRED 予報値
			{
				eSigSrcType_File,
				USC20_RNG_PRED_SIG,
				USC20_RNG_PRED_NAME,
				_T("km"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RR
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				USC20_RR_SS_SIG,
				USC20_RR_SS_NAME,
				_T("m/s"),
				13,
				0
			},
			// RR(PRED) 予報値
			{
				eSigSrcType_File,
				USC20_RR_PRED_SIG,
				USC20_RR_PRED_NAME,
				_T("m/s"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// EL
		{
			// EL
			{
				eSigSrcType_EngExchange,
				USC20_EL_EL_SIG,
				USC20_EL_EL_NAME,
				_T("deg"),
				3,
				3
			},
			// EL(PRED) 予報値
			{
				eSigSrcType_File,
				USC20_EL_EL_PRED_SIG,
				USC20_EL_EL_PRED_NAME,
				_T("deg"),
				3,
				3
			},
			// SKYLINE
			{
				eSigSrcType_File,
				USC20_EL_SKYLINE_SIG,
				USC20_EL_SKYLINE_NAME,
				_T("deg"),
				3,
				3
			},
			// RF-TRIP
			{
				eSigSrcType_File,
				USC20_EL_RF_TRIP_SIG,
				USC20_EL_RF_TRIP_NAME,
				_T("deg"),
				3,
				3
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
	},
};


static const structSatelliteCaptureSig mListSummarySig[eStation_MAX][eSummaryWndSetType_Num][MAX_TDATA + 1] = {
	// 臼田64m
	{
		// RNG
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				UDSC64_RNG_SS_SIG,
				UDSC64_RNG_SS_NAME,
				_T("km"),
				13,
				0
			},
			// X/X
			{
				eSigSrcType_EngExchange,
				UDSC64_RNG_XX_TCR_SIG,
				UDSC64_RNG_XX_TCR_NAME,
				_T("km"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RR
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				UDSC64_RR_SS_SIG,
				UDSC64_RR_SS_NAME,
				_T("m/s"),
				13,
				0
			},
			// X/X
			{
				eSigSrcType_EngExchange,
				UDSC64_RR_XX_SIG,
				UDSC64_RR_XX_NAME,
				_T("m/s"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// XSP
		{
			{
				eSigSrcType_EngExchange,
				_T("DSNTCR_SPE.SPECTRUM"),
				_T(""),
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// SAGC
		{
			{
				eSigSrcType_EngExchange,
				UDSC64_AGC_SS_TCR_SIG,
				UDSC64_AGC_SS_TCR_NAME,
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// XAGC
		{
			{
				eSigSrcType_EngExchange,
				UDSC64_AGC_XX_TCR_SIG,
				UDSC64_AGC_XX_TCR_NAME,
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
	},

	// 臼田54m
	{
		// RNG
		{
			// X/X
			{
				eSigSrcType_EngExchange,
				XDSC54_RNG_DSNMODEM_X_SIG,
				XDSC54_RNG_DSNMODEM_X_NAME,
				_T("km"),
				13,
				0
			},
			// X/Ka
			{
				eSigSrcType_EngExchange,
				XDSC54_RNG_DSNMODEM_KA_SIG,
				XDSC54_RNG_DSNMODEM_KA_NAME,
				_T("km"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RR
		{
			// X/X
			{
				eSigSrcType_EngExchange,
				XDSC54_RR_DSNMODEM_X_SIG,
				XDSC54_RR_DSNMODEM_X_NAME,
				_T("m/s"),
				13,
				0
			},
			// X/Ka
			{
				eSigSrcType_EngExchange,
				XDSC54_RR_DSNMODEM_KA_SIG,
				XDSC54_RR_DSNMODEM_KA_NAME,
				_T("m/s"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// XSP
		{
			{
				eSigSrcType_EngExchange,
				_T("DSNTCR_SPE.SPECTRUM"),
				_T(""),
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// KaSP
		{
			{
				eSigSrcType_EngExchange,
				_T("DSNTCR_SPE.SPECTRUM"),
				_T(""),
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// XAGC
		{
			{
				eSigSrcType_EngExchange,
				XDSC54_AGC_XX_TCR_SIG,
				XDSC54_AGC_DSNMODEM_NAME,
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// KaAGC
		{
			{
				eSigSrcType_EngExchange,
				XDSC54_AGC_XX_TCR_SIG,
				XDSC54_AGC_DSNMODEM_NAME,
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
	},

	// 内之浦34m
	{
		// RNG
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				USC34_RNG_SS_SIG,
				USC34_RNG_SS_NAME,
				_T("km"),
				13,
				0
			},
			// X/X
			{
				eSigSrcType_EngExchange,
				USC34_RNG_XX_SIG,
				USC34_RNG_XX_NAME,
				_T("km"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RR
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				USC34_RR_SS_SIG,
				USC34_RR_SS_NAME,
				_T("m/s"),
				13,
				0
			},
			// X/X
			{
				eSigSrcType_EngExchange,
				USC34_RR_XX_SIG,
				USC34_RR_XX_NAME,
				_T("m/s"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// XSP
		{
			{
				eSigSrcType_EngExchange,
				_T("DSNTCR_SPE.SPECTRUM"),
				_T(""),
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// SAGC
		{
			{
				eSigSrcType_EngExchange,
				USC34_AGC_SS_TCR_A_SIG,
				USC34_AGC_SS_TCR_A_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_AGC_SS_TCR_B_SIG,
				USC34_AGC_SS_TCR_B_NAME,
				_T("dBm"),
				13,
				0
			},
				{
				eSigSrcType_EngExchange,
				USC34_SAGC_TRKRXAGC_A_SIG,
				_T(""),
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_SAGC_TRKRXAGC_B_SIG,
				_T(""),
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_AGC_S_TRKRX_A_SIG,
				USC34_AGC_S_TRKRX_A_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_AGC_S_TRKRX_B_SIG,
				USC34_AGC_S_TRKRX_B_NAME,
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// XAGC
		{
			{
				eSigSrcType_EngExchange,
				USC34_XAGC_ACQ_TRKRXAGC_A_SIG,
				_T(""),
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_XAGC_ACQ_TRKRXAGC_B_SIG,
				_T(""),
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_XAGC_TRKRXAGC_A_SIG,
				_T(""),
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_XAGC_TRKRXAGC_B_SIG,
				_T(""),
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_AGC_XX_TCR_SIG,
				_T(""),
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC34_AGC_X_HRX_SIG,
				USC34_AGC_X_HRX_NAME,
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
	},

	// 内之浦20m
	{
		// RNG
		{
			// S/S
			{
				eSigSrcType_EngExchange,
				USC20_RNG_SS_SIG,
				USC20_RNG_SS_NAME,
				_T("km"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// RR
		{
			{
				eSigSrcType_EngExchange,
				USC20_RR_SS_SIG,
				USC20_RR_SS_NAME,
				_T("m/s"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},

//		// XSP
//		{
//			// NULL
//			{
//				0,
//				_T(""),
//				_T(""),
//				_T(""),
//				0,
//				0
//			},
//		},

		// SAGC
		{
			{
				eSigSrcType_EngExchange,
				USC20_AGC_SS_TCR_A_SIG,
				USC20_AGC_SS_TCR_A_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC20_AGC_SS_TCR_B_SIG,
				USC20_AGC_SS_TCR_B_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC20_AGC_S_ACQTCR_A_SIG,
				USC20_AGC_S_ACQTCR_A_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC20_AGC_S_ACQTCR_B_SIG,
				USC20_AGC_S_ACQTCR_B_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC20_AGC_S_DET_A_SIG,
				USC20_AGC_S_DET_A_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC20_AGC_S_DET_B_SIG,
				USC20_AGC_S_DET_B_NAME,
				_T("dBm"),
				13,
				0
			},

			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
		// XAGC
		{
			{
				eSigSrcType_EngExchange,
				USC20_AGC_SX_TCR_A_SIG,
				USC20_AGC_SX_TCR_A_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC20_AGC_SX_TCR_B_SIG,
				USC20_AGC_SX_TCR_B_NAME,
				_T("dBm"),
				13,
				0
			},
			{
				eSigSrcType_EngExchange,
				USC20_AGC_X_HRX_SIG,
				USC20_AGC_X_HRX_NAME,
				_T("dBm"),
				13,
				0
			},
			// NULL
			{
				0,
				_T(""),
				_T(""),
				_T(""),
				0,
				0
			},
		},
	}
};


// 衛星捕捉グラフ用の予報値、スカイライン、RFトリップ名のデータ数
#define GRAPH_YOHOSIGNUM	5
// 衛星捕捉グラフ用の予報値、スカイライン、RFトリップ名
static CString mListYohoSig[eStation_MAX][GRAPH_YOHOSIGNUM] = {
	// 臼田64m
	{
		UDSC64_RNG_PRED_SIG,
		UDSC64_RR_PRED_SIG,
		UDSC64_EL_EL_PRED_SIG,
		UDSC64_EL_SKYLINE_SIG,
		UDSC64_EL_RF_TRIP_SIG,
	},
	// 臼田54m
	{
		XDSC54_RNG_PRED_SIG,
		XDSC54_RR_PRED_SIG,
		XDSC54_EL_EL_PRED_SIG,
		XDSC54_EL_SKYLINE_SIG,
		XDSC54_EL_RF_TRIP_SIG,
	},
	// 内之浦34m
	{
		USC34_RNG_PRED_SIG,
		USC34_RR_PRED_SIG,
		USC34_EL_EL_PRED_SIG,
		USC34_EL_SKYLINE_SIG,
		USC34_EL_RF_TRIP_SIG,
	},
	// 内之浦20m
	{
		USC20_RNG_PRED_SIG,
		USC20_RR_PRED_SIG,
		USC20_EL_EL_PRED_SIG,
		USC20_EL_SKYLINE_SIG,
		USC20_EL_RF_TRIP_SIG,
	}
};


//=============================================================================
//	監視データ
//=============================================================================
#pragma pack(1)
#define _MAX_NAME	64
#define _MAX_UNIT	8

enum{
	eCommand_ReqSignal = 0x01,
	eCommand_ReqStart = 0x02,
	eCommand_ReqStop = 0x03,
	eCommand_ReqDataList = 0x04,
	eCommand_ReqGetData = 0x05,
	eCommand_SendData = 0x10,
};

enum{
	eMessage_ClientConnection = (WM_USER + 1000),
	eMessage_ClientDisConnection,
	eMessage_ClientCommand,
};

//=============================================================================
//	ヘッダー
//=============================================================================
typedef struct  {
	DWORD		size;				// ヘッダーを含むデータサイズ
	BYTE		command;			// コマンドID
	BYTE		reserved[3];		// 予約
} stCommandHeader;

//=============================================================================
//	監視データ登録要求コマンド（eCommand_ReqSignal）
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// コマンドヘッダー
	BYTE			name[_MAX_NAME];// 監視データ名称（複数有り）
} stReqSignal;
typedef struct  {
	BYTE			name[_MAX_NAME];// 監視データ名称（複数有り）
} stReqSignalName;

//=============================================================================
//	監視データ送信開始要求コマンド（eCommand_ReqStart）
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// コマンドヘッダー
} stReqStart;

//=============================================================================
//	監視データ送信停止要求コマンド（eCommand_ReqStop）
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// コマンドヘッダー
} stReqStop;

//=============================================================================
//	監視データ名一覧取得要求コマンド（eCommand_ReqDataList）
//=============================================================================
typedef struct  {
	stCommandHeader	header;			// コマンドヘッダー
} stReqDataList;

//=============================================================================
//	監視データ送信コマンド（eCommand_SendData）
//=============================================================================
typedef struct  {
	WORD			wYear;				// 年
	WORD			wDay;				// １年間の通算日
	WORD			wHour;				// 時間
	WORD			wMinute;			// 分
	WORD			wSecond;			// 秒
	WORD			wMilliseconds;		// ミリ秒
} stServerTime;
typedef struct  {
	stServerTime	systime;			// 時刻
	BYTE			name[_MAX_NAME];	// データ名称
	DWORD			kind;				// データ種別
	double			value;				// データ
	BYTE			status[8];			// ステータス
	BYTE			reserved[32];
} stServerItemData;
typedef struct  {
	stCommandHeader		header;				// コマンドヘッダー
	stServerItemData	item;				// アイテム
} stServerData;

//=============================================================================
//	応答
//=============================================================================
typedef struct  {
	DWORD		size;				// ヘッダーを含むデータサイズ
	BYTE		command;			// コマンドID
	BYTE		status;				// Ack/Nack
	BYTE		reserved[2];		// 予約
} stAckNack;
typedef struct  {
	BYTE		name[_MAX_NAME];	// 信号名称
	BYTE		unit[_MAX_UNIT];	// 信号単位
} stDataList;
typedef struct  {
	stAckNack	header;				// コマンドヘッダー
	stDataList	data[1];			// 信号リスト
} stSendDataList;
//=============================================================================
typedef struct  {
	BOOL	start;
	BOOL	requestData;
	HANDLE	thread;
	ULONG	ipClient;
	SOCKET	sockClient;
} stThreadData;
//=============================================================================
#pragma pack()


// CCommBase コマンド ターゲット

class CCommBase
{
public:
	CCommBase();
	virtual ~CCommBase();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mCtrlPort = 50001;
	static const UINT mDataPort = 50002;
	static const DWORD mDefaultTimeout = 100L;
	static const DWORD mDefaultCommTimeout = 1000L;
	static const DWORD mMaxHostName = MAX_PATH;
//	static const DWORD mMaxData = 80;
#if 1
	static const DWORD mMaxData = 61 + 32;
#else
	static const DWORD mMaxData = 54;
#endif
	static const DWORD mMaxBufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData)*mMaxData);

//++ 20181023 UUN-18-074確認用 >>>
#ifndef _DEBUG_STEP_TEST_WAVE
	static const DWORD mUdsc64BufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData)*13);
#else
	static const DWORD mUdsc64BufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData) * 1);
#endif /* _DEBUG_STEP_TEST_WAVE */
//++ 20181023 UUN-18-074確認用 <<<
	static const DWORD mXdsc54MaxBufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData)*8);
	static const DWORD mUsc34BufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData)*(18 + 7));
	static const DWORD mUsc20BufferSize = sizeof(stCommandHeader) + (sizeof(stServerItemData) * 12);

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	bool			mThreadStop;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static int		WaitSelect(SOCKET fd, long lMilliSec);
	static bool		SendData(SOCKET fd, char* pcDataBuf, unsigned int uiDataSize);
	static bool		RecvData(SOCKET fd, char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
	bool			GetThreadStop()	{ return mThreadStop; }

	static void		ConvertTime(const stServerTime* unixTime, SYSTEMTIME* winTime);
	static void		ConvertTime(const SYSTEMTIME* winTime, stServerTime* unixTime);

	static bool		GetLocalAddress(char* strAddress, UINT nSize);
	static bool		GetLocalName(char* strName, UINT nSize);
	static USHORT	GetPortNumber(char* strServiceName);
	static ULONG	GetIPAddress(char* strHostName);

protected:

};


typedef struct{
	SOCKET	sockClient;
	char	ipClient[20];
} stClientSockInfo;

typedef std::map<ULONG, stClientSockInfo> CADDRLIST;

// CCommServer コマンド ターゲット

class CCommServer : public CCommBase
{
public:
	CCommServer();
	virtual ~CCommServer();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetMessageWnd(CWnd* val) { mMessageWnd = val; }
	map< ULONG, vector<stReqSignalName> >&	GetSignalList() { return mSignalList; }
	vector<stDataList>&	GetDataList() { return mDataList; }
	map<UINT, stThreadData>&	GetThreadData() { return mThreadClientData; }

protected:
	map< ULONG, vector<stReqSignalName> >	mSignalList;
	vector<stDataList>		mDataList;
	CWnd*					mMessageWnd;
	map<UINT, stThreadData>		mThreadClientData;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SOCKET			mSock;
	HANDLE			mThread;
	CADDRLIST		mClientSockAddrList;
	map<ULONG, DWORD>	mThreadList;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	Create(int iPort);
	bool	Release();
	ULONG	WaitAccept(long lTimeOut);
	bool	SendData(char* pcDataBuf, unsigned int uiDataSize);
	bool	RecvData(char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
	void	Wait();
	void	Client(DWORD threadid);
	CADDRLIST& GetClientAddrList() { return mClientSockAddrList; }
	bool	SendGetDataSignal(SOCKET sockClient, LPBYTE lpData, DWORD dwSize);

	/* ------------------------------------------------------------------------------------ */
	/* コマンド                                                                             */
	/* ------------------------------------------------------------------------------------ */
	bool	RequestSignal(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestSignalStart(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestSignalStop(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);
	bool	RequestDataList(SOCKET sockClient, stCommandHeader& header);
	bool	RequestGetDataSignal(ULONG ipClient, SOCKET sockClient, stCommandHeader& header);

	DWORD	GetThreadId(ULONG ipClient)
	{
		map<ULONG, DWORD>::iterator itr = mThreadList.find(ipClient);
		if (itr == mThreadList.end())
			return 0;

		return (*itr).second;
	}
	ULONG	GetThreadClient(DWORD thread)
	{
		map<ULONG, DWORD>::iterator itr;
		for (itr = mThreadList.begin(); itr != mThreadList.end(); itr++){
			if ((*itr).second == thread)
				return (*itr).first;
		}

		return 0;
	}
	void	ResetThreadId(ULONG ipClient)
	{
		map<ULONG, DWORD>::iterator itr = mThreadList.find(ipClient);
		if (itr == mThreadList.end())
			return;

		(*itr).second = 0xffffffff;
	}
	void	Clear()
	{
		map<ULONG, DWORD>::iterator itr;
		bool ret = true;
		while (ret)
		{
			ret = false;
			for (itr = mThreadList.begin(); itr != mThreadList.end(); itr++){
				if ((*itr).second == 0xffffffff){
					mThreadList.erase(itr);
					ret = false;
					break;
				}
			}
		}
	}
protected:
	static UINT WINAPI WaitClientThread(LPVOID pParam);
	static UINT WINAPI ClientCommThread(LPVOID pParam);
	static UINT WINAPI ClientDataThread(LPVOID pParam);
};


// CCommClient コマンド ターゲット

class CCommClient : public CCommBase
{
public:
	CCommClient();
	virtual ~CCommClient();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	SOCKET			mSock;
	bool			mOpen;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	IsOpen()	{ return mOpen; }

	bool	Open(int iPort, char* pServerIP);
	bool	Close();
	bool	SendData(char* pcDataBuf, unsigned int uiDataSize);
	bool	RecvData(char* pcDataBuf, unsigned int* puiSize, long lTimeOut);
protected:

};



