#pragma once
#include "afxcmn.h"
#include "FrequencyCtrlMon.h"
#include "FrequencyCtrlCtrl.h"
#include "FrequencyCtrlPre.h"
#include "FrequencyCtrlSm.h"
#include "FrequencyCtrlFcChg.h"

// CFrequencyCtrl �_�C�A���O

class CFrequencyCtrl : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrl)

public:
	CFrequencyCtrl(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CFrequencyCtrl(UINT IDD, CWnd* pParent = NULL);
	virtual ~CFrequencyCtrl();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
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
	//	�����o�ϐ�
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
	//	�����o�֐�
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


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_FreqTabCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeFreqCtrlTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
