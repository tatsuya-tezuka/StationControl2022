#pragma once
#include "afxcmn.h"
#include "ForecastValue.h"
#include "ForecastOperationPlan.h"
#include "ForecastRadarGraphView.h"
#include "DetailViewDlg.h"

// CForecastList �_�C�A���O

class CForecastList : public CDialogBase
{
	DECLARE_DYNAMIC(CForecastList)

public:
	CForecastList(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CForecastList();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FORECASTLIST };

	// ��ʐݒ�
	const int nTabLeftOffset =					15;		// �^�u�E�B���h�E��Left�I�t�Z�b�g
	const int nTabTopOffset =					65;		// �^�u�E�B���h�E��Top�I�t�Z�b�g
	const int nTabBotOffset =					15;		// �^�u�E�B���h�E��Bot�I�t�Z�b�g

	// �\��l�擾���[�h
	enum ePredGetType{
		ePredGetType_Auto = 0,
		ePredGetType_Manual,
		ePredGetType_Stop,
		ePredGetType_Other,
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	CTabCtrl m_tabCtrl;					// �^�u�R���g���[��
	CForecastValue m_forcastValue;		// �\��l
	CForecastOperationPlan m_operationPlan;		// �^�p�v��
	CComboBox m_CCBoxSatelliteName;		// �q�����R���{�{�b�N�X
	CComboBox m_CCBoxStationName;		// �ǖ��R���{�{�b�N�X
	CStatic m_viewArea;					// �\��l���[�_�[�O���t�r���[�G���A
	CForecastRadarGraphView* m_pView;	// �\��l���[�_�[�O���t�r���[
	CButton m_CButonDelete;				// �폜�{�^��
	CButton m_CButonOffsetTime;			// �����I�t�Z�b�g�{�^��
	BOOL m_bUseSelectBtn;				// �I���{�^�����g�p
	CString m_SelectedForecastFilePath; // �I�����ꂽ�\��l�̃t�@�C���p�X
	CDetailViewDlg mDetailDlg;			// �ڍו\�����
	vector<CString> m_listStationName;	// �ǖ����X�g
	vector<CString> m_listSatelliteName;	// �q�������X�g
	CWnd* m_pCallWnd;					// �Ăяo���E�B���h�E
	CString m_strAntFilePath;			// �\��l�t�@�C���p�X
	BOOL m_bCloseUnCheck;				// �����`�F�b�N

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void InitCombobox();
//	void InitComboAndList();
	CString GetSatelliteName();
	CString GetStationName();
	CString GetSimpleStationName();
	int GetStationIndex();
	int GetStationCount();
	void SetFocusToOffsetTimeBtn();
	CTabCtrl& GetTabCtrl()	{ return m_tabCtrl; }
	void SelectTabAndSatellite(int sel, CString& strSatelliteName);
	void SelectTabAndInitCombo(int sel);
	CButton& GetCButonOffsetTime()	{ return m_CButonOffsetTime; }
	void SetUseSelectBtn(BOOL bSel)			{ m_bUseSelectBtn = bSel; }
	int ChangeSimpleStationNameToIndex(CString stationName);
	CForecastRadarGraphView* GetView()	{ return m_pView; }
	void ViewUnSelectedMessage();
	void SetSatelliteName(CString& strStationName);
	void SetCallWnd(CWnd* pWnd)			{ m_pCallWnd = pWnd; }
	static int GetStationIndex(CString& strStationName);
	BOOL PrintList(int nStation, CString& fname);
	BOOL IsForecastList() {
		if (m_tabCtrl.GetCurSel() == eForecastListType_Forecast){
			return TRUE;
		}
		else{
			return FALSE;
		}
	}
	BOOL IsOperationList() {
		if (m_tabCtrl.GetCurSel() == eForecastListType_Operation){
			return TRUE;
		}
		else{
			return FALSE;
		}
	}

protected:
	void SelectForcastlist(int sel, CString& strSatelliteName);
	void SelectForcastlistAndInitCombo(int sel);
	UINT GetPredGetMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnMenuSetAutoMode();
	afx_msg void OnMenuSetManualModel();
	afx_msg void OnMenuGetManual ();
	afx_msg void OnMenuResend();
	afx_msg void OnTcnSelchangeForcastlistTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeForcastlistCombo();
	afx_msg void OnBnClickedForecastlistButton1();
	afx_msg void OnBnClickedForecastlistButton2();
	afx_msg void OnBnClickedForecastlistButton3();
	afx_msg void OnBnClickedForecastlistButton4();
	afx_msg void OnBnClickedForecastlistButton5();
	afx_msg void OnBnClickedForecastlistButton6();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeForcastlistStationCombo();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	/* ------------------------------------------------------------------------------------ */
};
