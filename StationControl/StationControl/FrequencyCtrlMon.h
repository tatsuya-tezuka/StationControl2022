#pragma once
#include "afxdtctl.h"
#include "atltime.h"

// CFrequencyCtrlMon ダイアログ


class CFrequencyCtrlMon : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlMon)

public:
	CFrequencyCtrlMon(CWnd* pParent = NULL);   // 標準コンストラクター
	CFrequencyCtrlMon(UINT IDD, CWnd* pParent = NULL);
	virtual ~CFrequencyCtrlMon();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_MONITOR };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum{
		eINT,
		eDOUBLE,
		eSTRING,
	};
	typedef struct{
		const CString		monname;		// 監視名
		CString&			controlval;		// コントロール値
		const CString		format;			// フォーマット
		UINT				type;			// 種別：0:INT 1:FLOAT 2:DOUBLE 3:STRING
	} stSetMonData;
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	CTrendData	mTrendData;
	CWnd*		mCallWnd;
	CString		mTargetSatellite;
	CString		mTargetPredAnp;
	double		mTxFreqVal;		// 送信周波数値[Hz]
	double		mRxFreqVal[2];	// (S,X)受信周波数値[Hz]
	long		mTXTType;		// 0=S帯 1=X帯

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	virtual BOOL UpdateDlg();

protected:
	void SetData();
	bool _getTxFqVal(char* pbuf);
	bool _getRxFqVal(char* pbuf);
	bool _getXTXTypeVal(char* pbuf);
	bool _convertPredFileName(CString& monPredFileName, CString& satName, CString& anpName);

	void GetEqstatDbValue(CString& strst);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 設定周波数-U/C
	CString m_strUC;
	// 設定周波数-D/C
	CString m_strDC;
	// 設定周波数-コマンドキャリア
	CString m_strCmdCareer;
	// オフセット-送信周波数オフセット
	CString m_strTFreqOffset;
	// オフセット-時刻オフセット
	CString m_strTimeOffset;
	// 予報値情報-RTT（Up Link）
	CString m_strForecastRTTUp;
	// 予報値情報-RTT（Down Link）
	CString m_strForecastRTTDw;
	// ドップラ補償-送信ドップラ
	CString m_strTDoppler;
	// ドップラ補償-受信ドップラ
	CString m_strRDoppler;
	// ドップラ補償-コマンドキャリア
	CString m_strDopplerCmdCareer;
	// 探査機COHモード
	CString m_strSpacecraftCOHMode;
	// 予測周波数（TX)
	CString m_strPreFreqTx;
	// 予測周波数（RX)
	CString m_strPreFreqRx;
	// 予測周波数（SC)
	CString m_strPreFreqSc;

	virtual BOOL OnInitDialog();
	CDateTimeCtrl m_PredTimeDayCtrl;
	CTime m_tPredDay;
	CTime m_tPredTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strRFreqOffset;
};
