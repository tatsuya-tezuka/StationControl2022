#pragma once

#include "../StationControl/ComboBoxEx2.h"

namespace timeline{

// CID選択画面データ管理クラス

typedef struct {	// 衛星毎、CID選択項目
	vector<CString>	CID;				// CID
	vector<CString>	TlmBitRateS;		// テレメトリビットレート S帯
	vector<CString>	TlmBitRateX;		// テレメトリビットレート X帯
	vector<CString>	TlmBitRateHX;		// テレメトリビットレート X高速
	vector<CString>	TlmBitRateKa;		// テレメトリビットレート Ka帯
	vector<CString>	CmdBitRateS;		// コマンドビットレート S帯
	vector<CString>	CmdBitRateX;		// コマンドビットレート X帯
	vector<CString>	TrsPower;			// 送信電力
	vector<CString>	UpLinkSweepShape;	// アップリンク掃引パラメータ 形状
	vector<CString>	RngMesMode;			// レンジ計測パラメータ モード
	vector<CString>	CarrLoopBandS;		// キャリア受信ループバンド S帯
	vector<CString>	CarrLoopBandX;		// キャリア受信ループバンド X帯
	vector<CString>	CarrLoopBandKa;		// キャリア受信ループバンド Ka帯
	vector<CString>	CmdMod;				// コマンド変調度
	vector<CString>	CmdSubCarrFreq;		// コマンドサブキャリア周波数
	vector<CString>	TransDoppler;		// 送信ドップラ
	vector<CString>	AntAutoTrack;		// アンテナ自動追尾移行の有無
	vector<CString>	Modulation;			// 変調方式
	vector<CString>	ModulationS;		// 変調方式 S帯
	vector<CString>	ModulationX;		// 変調方式 X帯
	vector<CString>	AntAosPosn;			// アンテナ待ち受け指向の有無
	vector<CString>	AntWaitPoint;		// アンテナ待機位置の有無
}stCIDPrmEle;

typedef struct {	// CID毎パラメータ
	CString	CID;				// CID
	CString	TlmBitRateS;		// テレメトリビットレート S帯
	CString	TlmBitRateX;		// テレメトリビットレート X帯
	CString	TlmBitRateHX;		// テレメトリビットレート X高速
	CString	TlmBitRateKa;		// テレメトリビットレート Ka帯
	CString	CmdBitRateS;		// コマンドビットレート S帯
	CString	CmdBitRateX;		// コマンドビットレート X帯
	CString	TrsPower;			// 送信電力
	CString	CarrLoopBandS;		// キャリア受信ループバンド S帯
	CString	CarrLoopBandX;		// キャリア受信ループバンド X帯
	CString	CarrLoopBandKa;		// キャリア受信ループバンド Ka帯
	CString	UpLinkSweepShape;	// アップリンク掃引パラメータ 形状
	CString	UpLinkSweepWidth;	// アップリンク掃引パラメータ 幅
	CString	UpLinkSweepTime;	// アップリンク掃引パラメータ 時間
	CString	RngMesMode;			// レンジ計測パラメータ モード
	CString	RngMesSmplRate;		// レンジ計測パラメータ サンプリングレート
	CString	RngMesSmplItgTime;	// レンジ計測パラメータ 積分時間
	CString	RngMesSmplMod;		// レンジ計測パラメータ 変調度
	CString	Modulation;			// 変調方式 S帯
	CString	ModulationS;		// 変調方式 S帯
	CString	ModulationX;		// 変調方式 X帯
	CString	CmdMod;				// コマンド変調度
	CString	CmdSubCarrFreq;		// コマンドサブキャリア周波数
	CString	TransDoppler;		// 送信ドップラ
	CString	AntAutoTrack;		// アンテナ自動追尾
	CString	AntAosPosn;			// アンテナ待ち受け指向
	CString	AntWaitPoint;		// アンテナ待機位置指向
}stCIDParam;

typedef struct {	// CID毎パラメータ
	BOOL	CID;				// CID
	BOOL	TlmBitRateS;		// テレメトリビットレート S帯
	BOOL	TlmBitRateX;		// テレメトリビットレート X帯
	BOOL	TlmBitRateHX;		// テレメトリビットレート X高速
	BOOL	TlmBitRateKa;		// テレメトリビットレート Ka帯
	BOOL	CmdBitRateS;		// コマンドビットレート S帯
	BOOL	CmdBitRateX;		// コマンドビットレート X帯
	BOOL	TrsPower;			// 送信電力
	BOOL	CarrLoopBandS;		// キャリア受信ループバンド S帯
	BOOL	CarrLoopBandX;		// キャリア受信ループバンド X帯
	BOOL	CarrLoopBandKa;		// キャリア受信ループバンド Ka帯
	BOOL	UpLinkSweepShape;	// アップリンク掃引パラメータ 形状
	BOOL	UpLinkSweepWidth;	// アップリンク掃引パラメータ 幅
	BOOL	UpLinkSweepTime;	// アップリンク掃引パラメータ 時間
	BOOL	RngMesMode;			// レンジ計測パラメータ モード
	BOOL	Modulation;			// 変調方式 S帯
	BOOL	ModulationS;		// 変調方式 S帯
	BOOL	ModulationX;		// 変調方式 X帯
	BOOL	CmdMod;				// コマンド変調度
	BOOL	CmdSubCarrFreq;		// コマンドサブキャリア周波数
	BOOL	TransDoppler;		// 送信ドップラ
	BOOL	AntAutoTrack;		// アンテナ自動追尾
	BOOL	AntAosPosn;			// アンテナ待ち受け指向
	BOOL	AntWaitPoint;		// アンテナ待機位置指向
}stCIDPrmFlg;

#define CID_INVALID		_T("---------")

class CCIDSlctData
{
public:
	CCIDSlctData();
	~CCIDSlctData();

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
	vector<CString>							m_SatelliteNameList;	// 衛星名リスト
	map<CString, stCIDPrmEle>				m_CidElementList;		// 衛星毎CIDパラメータ要素
	map<CString, stCIDPrmFlg>				m_CidPrmFlgList;		// 衛星毎CIDパラメータ有効/無効フラグ
	map<CString, map<CString, stCIDParam>>	m_CidParamList;		// 衛星毎CID毎パラメータ設定値

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
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

// CCidSelect ダイアログ

class CCidSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CCidSelect)

public:
	CCidSelect(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CCidSelect();

	// ダイアログ データ
	enum { IDD = IDD_DLG_CIDSEL };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CCIDSlctData	m_CIDSlctData;		// CID選択画面データ
protected:
	CString		m_strSatelliteName;		// 衛星名
	CString		m_strCID;				// CID
	int			m_xtx;					// X帯送信設備使用有無

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL UpdateDlg();

	void		SetSatelliteName(CString name) { m_strSatelliteName = name; };
	void		SetCID(CString cid) { m_strCID = cid; };
	stCIDParam	GetParameter();
	void		SetTxtMode(int val) { m_xtx = val; }

protected:
public:
	BOOL	InitDlg();
	BOOL	SetComboboxSatCIDParam(CString sat);
	BOOL	SetComboboxCIDParam(CString sat, CString cid);
	CString	CheckCid();
	//void	GetEqSatDB(CString satname);
	BOOL	ReadCIDFile();
	void	AddDataVector(vector<CString> &v, CString str);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	// 衛星選択
	CComboBoxEx2 m_SetalliteCmb;
	// CID選択
	CComboBoxEx2 m_CidCmb;
	// テレメトリビットレート-S帯
	CComboBoxEx2 m_TlmBrSCmb;
	// テレメトリビットレート-X帯
	CComboBoxEx2 m_TlmBrXCmb;
	// テレメトリビットレート-X高速
	CComboBoxEx2 m_TlmBrHXCmb;
	// テレメトリビットレート-Ka帯
	CComboBoxEx2 m_TlmBrKaCmb;
	// キャリア受信ループバンド-S帯
	CComboBoxEx2 m_CarrTLbSCmb;
	// キャリア受信ループバンド-X帯
	CComboBoxEx2 m_CarrTLbXCmb;
	// キャリア受信ループバンド-Ka帯
	CComboBoxEx2 m_CarrTLbKaCmb;
	// レンジ計測パラメータ-モード
	CComboBoxEx2 m_RngPrmModeCmb;
	// 変調方式-S帯
	CComboBoxEx2 m_MdlSCmb;
	// 変調方式-X帯
	CComboBoxEx2 m_MdlXCmb;
	// アンテナ自動追尾-移行
	CComboBoxEx2 m_AntAutoCmb;
	// コマンドビットレート-S帯
	CComboBoxEx2 m_CmdBrSCmb;
	// コマンドビットレート-X帯
	CComboBoxEx2 m_CmdBrXCmb;
	// 送信電力
	CComboBoxEx2 m_TPwrCmb;
	// アップリンク掃引パラメータ-形状
	CComboBoxEx2 m_UpLinkShapeCmb;
	// コマンド変調-変調度
	CComboBoxEx2 m_CmdMdlCmb;
	// コマンドサブキャリア周波数-周波数
	CComboBoxEx2 m_CmdFreqCmb;
	// 送信ドップラ-補償
	CComboBoxEx2 m_TDoppCmb;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCidselSatelliteCmb();
	afx_msg void OnCbnSelchangeCidselCidCmb();
	afx_msg void OnCbnSelchangeCidselTlmbrSCmb();
	afx_msg void OnCbnSelchangeCidselTlmbrXCmb();
	afx_msg void OnCbnSelchangeCidselTlmbrHxCmb();
	afx_msg void OnCbnSelchangeCidselTlmbrKaCmb();
	afx_msg void OnCbnSelchangeCidselCmdbrSCmb();
	afx_msg void OnCbnSelchangeCidselCmdbrXCmb();
	afx_msg void OnCbnSelchangeCidselTpwrPwrCmb();
	afx_msg void OnCbnSelchangeCidselClbSCmb();
	afx_msg void OnCbnSelchangeCidselClbXCmb();
	afx_msg void OnCbnSelchangeCidselClbKaCmb();
	afx_msg void OnCbnSelchangeCidselUplnkShpCmb();
	afx_msg void OnCbnSelchangeCidselRprmSrCmb();
	afx_msg void OnCbnSelchangeCidselCmdmdlMdlCmb();
	afx_msg void OnCbnSelchangeCidselCmdsubfreqFreqCmb();
	afx_msg void OnCbnSelchangeCidselMmtdSCmb();
	afx_msg void OnCbnSelchangeCidselMmtdXCmb();
	afx_msg void OnCbnSelchangeCidselTdopCmpCmb();
	afx_msg void OnCbnSelchangeCidselAntatMigCmb();
	// 衛星選択
	CString m_strSatelliteCmb;
	// CID選択
	CString m_strCIDCmb;
	// テレメトリビットレート S帯
	CString m_strTlmBRSCmb;
	// テレメトリビットレート X帯
	CString m_strTlmBRXCmb;
	// テレメトリビットレート X高速
	CString m_strTlmBRHXCmb;
	// テレメトリビットレート Ka帯
	CString m_strTlmBRKaCmb;
	// コマンドビットレート S帯
	CString m_strCmdBRSCmb;
	// コマンドビットレート X帯
	CString m_strCmdBRXCmb;
	// 送信電力
	CString m_strTPwrCmb;
	// キャリア受信ループバンド S帯
	CString m_strCarrTLbSCmb;
	// キャリア受信ループバンド X帯
	CString m_strCarrTLbXCmb;
	// キャリア受信ループバンド Ka帯
	CString m_strCarrTLbKaCmb;
	// アップリンク掃引パラメータ 形状
	CString m_strUpLinkShapeCmb;
	// レンジ計測パラメータ モード
	CString m_strRngPrmModeCmb;
	// コマンド変調度
	CString m_strCmdMdlCmb;
	// コマンドサブキャリア周波数
	CString m_strCmdFreqCmb;
	// 変調方式 S帯
	CString m_strMdlSCmb;
	// 変調方式 X帯
	CString m_strMdlXCmb;
	// 送信ドップラ補償
	CString m_strTDoppCmb;
	// アンテナ自動追尾
	CString m_strAntAutoCmb;
	// アンテナ待ち受け指向の有無
	CComboBoxEx2 m_AntStandbyCmb;
	CString m_strAntStandbyCmb;
	// アンテナ待機位置指向の有無
	CComboBoxEx2 m_AntStandbyPosCmb;
	CString m_strAntStandbyPosCmb;
	afx_msg void OnCbnSelchangeCidselAntstdorientedCmb();
	afx_msg void OnCbnSelchangeCidselAntstdposorientedCmb();
	afx_msg void OnBnClickedOk();
};

}; //namespace timeline
