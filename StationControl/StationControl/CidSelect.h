#pragma once
#include "afxwin.h"
#include "SatelliteData.h"


// CCidSelect ダイアログ

class CCidSelect : public CDialogBase
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
	int			GetSendType(CString satname)
	{
		GetEqSatDB(satname);
		return m_xtx;
	}

protected:
	BOOL	InitDlg();
	BOOL	SetComboboxSatCIDParam(CString sat);
	BOOL	SetComboboxCIDParam(CString sat, CString cid);
	CString	CheckCid();
	void	GetEqSatDB(CString satname);


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
