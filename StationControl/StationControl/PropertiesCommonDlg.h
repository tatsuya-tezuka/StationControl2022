#pragma once

#include "afxwin.h"
#include "TabCtrlEx.h"

#define ID_PROPERTIES_CB			(WM_USER +311)
#define ID_PROPERTIES_TAB           (WM_USER +500)
#define ID_PROPERTIES_LIST          (WM_USER +501)

// CCustomDlgProp
class CCustomDlgProp : public CMFCPropertyGridProperty
{
public:
	CCustomDlgProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData);

	void SetParent(CWnd* pWnd) { m_Parent = pWnd; }
	CWnd* GetParent() { return m_Parent; }

protected:
	CWnd*	m_Parent;

	virtual BOOL HasButton() const { return TRUE; }
	virtual void OnClickButton(CPoint point);
};

// CPropertiesToolBar
class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


// CPropertiesCommonDlg �_�C�A���O

class CPropertiesCommonDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CPropertiesCommonDlg)

public:
	CPropertiesCommonDlg(UINT IDD, CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CPropertiesCommonDlg(UINT IDD, CWnd* pParent, UINT nViewID, vector<CString>& listViewName, UINT nPropertiesID);
	virtual ~CPropertiesCommonDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TRENDPROPERTIES };

	// OnPropertyChanged �p
	enum
	{
		idViewMode,
		idViewDate,
		idVLAxisAutoRange,
		idVLAxisMaxRange,
		idVLAxisMinRange,
		idVRAxisMaxRange,
		idVRAxisMinRange,
		idVAxisScaleInterval,
		idHUAxisScale,
		idHLAxisScale,
		idAxisMax,
	};
	enum
	{
		idSelectItem = idAxisMax,
		idItemName,
		idItemUnit,
		idDisplay,
		idDrawKind,
		idDrawColor,
		idLineWidth,
		idDotSize,
		idVScale,
		idComment,
		idCommentEnable,
		idAlarmEnable,
		idAlarm1Major,
		idAlarm1Minor,
		idAlarm2Major,
		idAlarm2Minor,
		idMax,
	};

	// �f�[�^�Z�b�g �p�@>>>
	enum
	{
		idViewGr,				// �\���ݒ�
		idVAxisGr,				// �����ݒ�
		idHAxisGr,				// �c���ݒ�
		idDataGr,				// �f�[�^x
	};	// Top Group

	enum
	{
		idViewGrMode,			// �\���^�C�v
		idViewGrDate			// �J�n����
	};	// idViewGr

	enum
	{
		idVAxisGrScalLGr,		// �c���ݒ�-�\���X�P�[��(��)
		//		idVAxisGrScalRGr,		// �c���ݒ�-�\���X�P�[��(�E)
		idVAxisGrScalInvl,		// �c���ݒ�-�ڐ���̊Ԋu
	};	// idVAxisGr

	enum
	{
		idVAxisGrScalLGrAuto,	// �c���ݒ�-�\���X�P�[��(��)-�I�[�g�X�P�[��
		idVAxisGrScalLGrMax,	// �c���ݒ�-�\���X�P�[��(��)-�ő�l
		idVAxisGrScalLGrMin,	// �c���ݒ�-�\���X�P�[��(��)-�ŏ��l
	};	// idVAxisGrScalLGr

	enum
	{
		idVAxisGrScalRGrMax,	// �c���ݒ�-�\���X�P�[��(�E)-�ő�l
		idVAxisGrScalRGrMin,	// �c���ݒ�-�\���X�P�[��(�E)-�ŏ��l
	};	// idVAxisGrScalRGr

	enum
	{
		idHAxisGrScaleU,		// �c���ݒ�-�\���X�P�[��(��)
		idHAxisGrScaleL,		// �c���ݒ�-�\���X�P�[��(��)
	};	// 	idHAxisGr

	enum
	{
		idDataGrDataSelect,		// �f�[�^-�f�[�^�I��
		idDataGrName,			// �f�[�^-����
		idDataGrItemUnit,		// �f�[�^-�P��
		idDataGrDisplay,		// �f�[�^-�\��
		idDataGrDrawKind,		// �f�[�^-�`�惂�[�h
		idDataGrColor,			// �f�[�^-�\���F
		idDataGrLineWidth,		// ����
		idDataGrDotSiz,			// �f�[�^-�h�b�g�T�C�Y
		//		idDataGrSelScale,		// �f�[�^-�c���X�P�[���I��
		idDataGrComment,		// �f�[�^-�R�����g
		idDataGrAlarm,			// �f�[�^-�A���[��
	};	// 	idDataGr

	enum
	{
		idDataGrCommentStr,		// �R�����g-�R�����g
		idDataGrCommentEnable,	// �R�����g-�\��
	};//idDataGrComment

	enum
	{
		idDataGrAlarmEnable,	// �A���[��-�\��
		idDataGrAlarm1Major,	// �A���[��1-���
		idDataGrAlarm1Minor,	// �A���[��1-����
		idDataGrAlarm2Major,	// �A���[��2-���
		idDataGrAlarm2Minor,	// �A���[��2-����
	};//idDataGrAlarm

	static const int mOKButtonAreaHeight = 40;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CFont					m_fntPropList;
	CComboBox				m_wndObjectCombo;
	//	CPropertiesToolBar		m_wndToolBar;
	CMFCPropertyGridCtrl	m_wndPropList;
	//	CTabCtrlEx				m_wndTabs;
	//	CListCtrlEx				m_wndList;

	int						m_nViewID;
	int						m_nPropertiesID;
	int						m_nParentID;
	CStringList				m_DataNameList;
	CString					m_StartDate;

	int						m_nComboHeight;

	vector<CString>			m_listViewName;

	CPropertiesData			m_backupPropertiesData[eTrendWndType_Num];
	CPropertiesData			m_backupSummaryPropertiesData[MAX_SUMMARY_VIEW];

	int						m_nListMax;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual void AdjustLayout() = 0;

	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	void	SetNewPropertyData(int pos, CString name, LPARAM wnd);
	virtual void ChangeView(int pos) = 0;
	void	CloseView(int pos);
	virtual void ChangePropaties(BOOL bUpdate = TRUE) = 0;
	virtual void UpdatePropatyDataList() = 0;
	void	ExchangeViewCombo(BOOL *vBiew, CString name);

	void	SetStartDate(CString str) { m_StartDate = str; }
	CString	GetStartDate() { return m_StartDate; }

	CString		GetDataSelectNameAt(int idx);

protected:
	int			GetDrawMode(CString str);

	virtual void InitPropList() = 0;
	virtual CPropertiesData& GetPropertiesData(int pos) = 0;
	virtual void SetPropertiesData(int pos, CPropertiesData& val) = 0;
	virtual void SendPropertiesChange() = 0;

	void SetPropListFont();

	//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
//	afx_msg void OnComboBoxSelChange();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	/* ------------------------------------------------------------------------------------ */
};
