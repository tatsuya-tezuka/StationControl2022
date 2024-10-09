#pragma once

#include "AntStanbyOriented.h"
#include "DownlinkCapture.h"
#include "UplinkCtrl.h"

// CSatelliteCapture ダイアログ

class CSatelliteCapture : public CDialogBase
{
	DECLARE_DYNAMIC(CSatelliteCapture)

public:
	CSatelliteCapture(CWnd* pParent = NULL);   // 標準コンストラクター
	CSatelliteCapture(UINT IDD, CWnd* pParent = NULL);
	virtual ~CSatelliteCapture();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SATELLITECAPTURE };

	// 画面設定
	const int nTabLeftOffset = 15;		// タブウィンドウのLeftオフセット
	const int nTabTopOffset = 35;		// タブウィンドウのTopオフセット

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CTabCtrl			m_tabCtrl;						// タブコントロール
//	CAntStanbyOriented	m_antStanbyOriented;			// ANT待ち受け指向
	CDownlinkCapture	m_downlinkCapture;				// ダウンリンク捕捉
	CUplinkCtrl			m_uplinkCtrl;					// アップリンク制御

	UINT				m_CmdMode;						// CMD制御設定
	UINT				m_RngMode;						// RNG制御設定

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	void Update();
	void set_cur_cap();
	UINT GetRcvFreqMode()				{ return m_downlinkCapture.GetRcvFreqMode(); }
	UINT GetCtrlMode()					{ return m_downlinkCapture.GetCtrlMode(); }
	UINT GetRfTripMonitor()				{ return m_uplinkCtrl.GetRfTripMonitor(); }
	unsigned char GetAutoFreqData()		{ return m_downlinkCapture.GetAutoFreqData(); }
	unsigned char GetManualFreqData()	{ return m_downlinkCapture.GetManualFreqData(); }
	unsigned char GetWayBinary()		{ return m_downlinkCapture.GetWayBinary(); }
	unsigned char GetUpLinkBinary()		{ return m_uplinkCtrl.GetUpLinkBinary(); }
	unsigned char GetDownLinkBinary()	{ return m_downlinkCapture.GetDownLinkBinary(); }
	unsigned char GetCmdBinary();
	unsigned char GetRngBinary();

	BOOL UpdateKanshiData();
	void SetRngMode(char szCtrlMode[]);
	void SetCmdMode(char szCtrlMode[]);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnTcnSelchangeSatellitecaptureTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/* ------------------------------------------------------------------------------------ */
};
