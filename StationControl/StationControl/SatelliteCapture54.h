#pragma once

#include "SatelliteCapture.h"
#include "AntStanbyOriented.h"
#include "DownlinkCapture.h"
#include "UplinkCtrl.h"

// CSatelliteCapture54 ダイアログ

class CSatelliteCapture54 : public CSatelliteCapture
{
	DECLARE_DYNAMIC(CSatelliteCapture54)

public:
	CSatelliteCapture54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSatelliteCapture54();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// ダイアログ データ
	enum { IDD = IDD_DLG_SATELLITECAPTURE54 };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CAntStanbyOriented	m_antStanbyOriented;			// ANT待ち受け指向
	CDownlinkCapture	m_downlinkCapture;				// ダウンリンク捕捉
	CUplinkCtrl			m_uplinkCtrl;					// アップリンク制御

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnTcnSelchangeSatellitecaptureTab1(NMHDR *pNMHDR, LRESULT *pResult);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/* ------------------------------------------------------------------------------------ */
};
