#pragma once
#include "afxcmn.h"
#include "FrequencyCtrlMon.h"
#include "FrequencyCtrlCtrl.h"
#include "FrequencyCtrlPre.h"
#include "FrequencyCtrlSm.h"
#include "FrequencyCtrlFcChg.h"

// CFrequencyCtrl ダイアログ

class CFrequencyCtrl : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrl)

public:
	CFrequencyCtrl(CWnd* pParent = NULL);   // 標準コンストラクター
	CFrequencyCtrl(UINT IDD, CWnd* pParent = NULL);
	virtual ~CFrequencyCtrl();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct {
		TCHAR	szTime[15];
		TCHAR	szAZ[9];
		TCHAR	szEL[9];
		TCHAR	szUpRNG[18];
		TCHAR	szDwRNG[18];
		TCHAR	szUpRR[12];
		TCHAR	szDwRR[12];
		TCHAR	sz2wayRNG[18];
		TCHAR	sz2wayRR[12];
		TCHAR	sz3wayRNG[18];
		TCHAR	sz3wayRR[12];
	} stAnpData;
	typedef struct {
		CString	satellite;
		CString	predAnp;
		UINT	txDopp;
		__time64_t	timeStart;
		__time64_t	timeStop;
	} stTargetData;
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
protected:
	vector<CFrequencyCtrl::stAnpData> m_AnpDataList;
	vector<CString> mSatelliteNameList;
	vector<stPredListData> mAllPredList;
	vector<stPredListData> mPartPredList;
	stTargetData	mTargetData;
	
public:
	CFrequencyCtrlMon		m_TabMon;
	CFrequencyCtrlCtrl		m_TabCtrl;
	CFrequencyCtrlPre		m_TabPre;
	CFrequencyCtrlSm		m_TabSm;
	CFrequencyCtrlFcChg		m_TabFcChg;

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

	void	Initialize(bool bResetControl=true);

	void	GetAnpData(CString satName, CString anpName);
	vector<CFrequencyCtrl::stAnpData>& GetAnpDataList() { return m_AnpDataList; }
	vector<CString>& GetSatelliteNameList() { return mSatelliteNameList; }
	vector<stPredListData>& GetAllPredList() { return mAllPredList; }
	vector<stPredListData>& GetPartPredList() { return mPartPredList; }

	stTargetData& GetTargetData() { return mTargetData; }
	void SetTargetData(stTargetData& data) { mTargetData = data; }

protected:
	BOOL	GetSatelliteNameList(vector<CString>& listName);


protected:


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_FreqTabCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeFreqCtrlTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
