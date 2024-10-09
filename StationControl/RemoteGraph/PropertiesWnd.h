// ���� MFC �T���v�� �\�[�X �R�[�h�ł́AMFC Microsoft Office Fluent ���[�U�[ �C���^�[�t�F�C�X 
// ("Fluent UI") �̎g�p���@�������܂��B���̃R�[�h�́AMFC C++ ���C�u���� �\�t�g�E�F�A�� 
// ��������Ă��� Microsoft Foundation Class ���t�@�����X����ъ֘A�d�q�h�L�������g��
// �⊮���邽�߂̎Q�l�����Ƃ��Ē񋟂���܂��B
// Fluent UI �𕡐��A�g�p�A�܂��͔z�z���邽�߂̃��C�Z���X�����͌ʂɗp�ӂ���Ă��܂��B
// Fluent UI ���C�Z���X �v���O�����̏ڍׂɂ��ẮAWeb �T�C�g
// http://go.microsoft.com/fwlink/?LinkId=238214 ���Q�Ƃ��Ă��������B
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#pragma once

//#include "const.h"
#include "ListCtrlEx.h"
#include "TabCtrlEx.h"
#include "StartDateDlg.h"

// CPropertiesDataItem
class CPropertiesDataItem
{
public:
	static const int SIZE_DATANAME = 80;
	static const int SIZE_DATAGNAME = 80;
	static const int SIZE_DATAUNIT = 10;

	CPropertiesDataItem()
	{
		wmemset(dataName, 0, SIZE_DATANAME);
		wmemset(dataGName, 0, SIZE_DATAGNAME);
		wmemset(dataUnit, 0, SIZE_DATAUNIT);
		isViewMode = false;
		drawMode = Line;
		color = RGB(0, 0, 0);
		dotSize = Small;
		vscaleKind = LScale;
		isComment = false;
		comment.Empty();
		isLimitWarningUpper = false;
		limitWarningUpper = 0.0;
		isLimitWarningLower = false;
		limitWarningLower = 0.0;
		isLimitAlarmUpper = false;
		limitAlarmUpper = 0.0;
		isLimitAlarmLower = false;
		limitAlarmLower = 0.0;
	};
	virtual ~CPropertiesDataItem(){};

public:
	TCHAR		dataName[SIZE_DATANAME];			// �f�[�^����
	TCHAR		dataGName[SIZE_DATAGNAME];			// �f�[�^����(�O���t�p��)
	TCHAR		dataUnit[SIZE_DATAUNIT];			// �P��
	bool		isViewMode;			// �\��On/Off
	DrawMode	drawMode;			// �`�惂�[�h�iLine�ADots�AL&D�j
	COLORREF	color;				// �\���F
	DotSize		dotSize;			// �h�b�g�T�C�Y
	VScaleKind	vscaleKind;			// �c���X�P�[���I���i�E/���j
	bool		isComment;			// �R�����g�\��
	CString		comment;			// �R�����g
	bool		isLimitWarningUpper;	// ���[�j���O���(�\��)
	double		limitWarningUpper;		// ���[�j���O���
	bool		isLimitWarningLower;	// ���[�j���O����(�\��)
	double		limitWarningLower;		// ���[�j���O���� 
	bool		isLimitAlarmUpper;		// �A���[�����(�\��)
	double		limitAlarmUpper;		// �A���[�����
	bool		isLimitAlarmLower;		// �A���[������(�\��)
	double		limitAlarmLower;		// �A���[������
};

// CPropertiesData
class CPropertiesData
{
public:
	CPropertiesData(){};
	virtual ~CPropertiesData(){};

public:
	int			wndIdx;						// �E�B���h�[�ԍ�
	CString		wndName;					// �E�B���h�[����
	LPARAM		wnd;						// CView

	bool		isRealTime;					// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
	//	CTime		startTime;					// �J�n����
	stVScale	trandVScale;				// �c���X�P�[��[0:���A1:�E]
	HScale		trandHScale[MAX_HSCALE];	// �����X�P�[��[0:�g�����h�O���t�r���[1�A1:�g�����h�O���t�r���[2]

	CPropertiesDataItem item[MAX_TDATA];

public:
	int getHScal(CString str) {
		for (int i = 0; i < sizeof(strHScale) / sizeof(strHScale[0]); ++i)
		{
			if (str == strHScale[i])
			{
				return i;
			}
		};
		return -1;
	};

	void	WritePropertiesData(CString szFilePath, int pos);
	void	ReadPropertiesData(CString szFilePath, int pos);
};

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
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// �R���X�g���N�V����
public:
	CPropertiesWnd();

	void AdjustLayout();

// ����
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	void	SetNewPropertyData(int pos, LPARAM wnd);
	void	UpdateSimplicityList()
	{
		// �ȈՃ��X�g��ύX
		if( m_wndTabs.GetCurSel() == 1 )
			OnTcnSelchangeTab(0, 0);
	}
	void	ChangeView(int pos);
	void	CloseView(int pos);
	void	ChangePropaties(BOOL bUpdate = TRUE);
	void	UpdatePropatyDataList(BOOL bDataName = TRUE, BOOL bAutoAssign = TRUE);
	void	ExchangeViewCombo(BOOL *vBiew, CString name);

	void	SetStartDate(CString str) { m_StartDate = str; }
	CString	GetStartDate() { return m_StartDate; }
	BOOL	GetAutoScaleEnable(int pos) { return m_AutoScale[pos]; }

	void	ChangeViewMode();
	void	SetViewMode(CPropertiesData& prop, CString str);

protected:
	CString		GetDataSelectNameAt(int idx);
	int			GetDrawMode(CString str);

protected:
	CFont					m_fntPropList;
	CComboBox				m_wndObjectCombo;
	CPropertiesToolBar		m_wndToolBar;
	CMFCPropertyGridCtrl	m_wndPropList;
	CTabCtrlEx				m_wndTabs;
	CListCtrlEx				m_wndList;

	int						m_ActiveView;
	CStringList				m_DataNameList;
	CString					m_StartDate;
	BOOL					m_AutoScale[MAX_VIEW];

	// OnPropertyChanged �p
	enum
	{
		idViewMode,
//		idViewDate,
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
		idDotSize,
		idVScale,
		idCommentEnable,
		idComment,
		idLimitWarningUpperEnable,
		idLimitWarningUpper,
		idLimitWarningLowerEnable,
		idLimitWarningLower,
		idLimitAlarmUpperEnable,
		idLimitAlarmUpper,
		idLimitAlarmLowerEnable,
		idLimitAlarmLower,
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
//		idViewGrDate			// �J�n����
	};	// idViewGr


	enum 
	{
		idVAxisGrScalLGr,		// �����ݒ�-�\���X�P�[��(��)
		idVAxisGrScalRGr,		// �����ݒ�-�\���X�P�[��(�E)
		idVAxisGrScalInvl,		// �����ݒ�-�ڐ���̊Ԋu
	};	// idVAxisGr

	enum
	{
		idVAxisGrScalLGrAuto,	// �����ݒ�-�\���X�P�[��(��)-�I�[�g�X�P�[��
		idVAxisGrScalLGrMax,	// �����ݒ�-�\���X�P�[��(��)-�ő�l
		idVAxisGrScalLGrMin,	// �����ݒ�-�\���X�P�[��(��)-�ŏ��l
	};	// idVAxisGrScalLGr

	enum
	{
		idVAxisGrScalRGrMax,	// �����ݒ�-�\���X�P�[��(�E)-�ő�l
		idVAxisGrScalRGrMin,	// �����ݒ�-�\���X�P�[��(�E)-�ŏ��l
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
		idDataGrDotSiz,			// �f�[�^-�h�b�g�T�C�Y
		idDataGrSelScale,		// �f�[�^-�c���X�P�[���I��
		idDataGrComment,		// �f�[�^-�R�����g
		idDataGrLimit,			// �f�[�^-���~�b�g
	};	// 	idDataGr

	enum
	{
		idDataGrCommentEnable,	// �R�����g-�\��
		idDataGrCommentStr,		// �R�����g-�R�����g
	};//idDataGrComment

	enum
	{
		idDataGrLimitWarningUpperEnable,	// ���[�j���O-���(�\��)
		idDataGrLimitWarningUpper,			// ���[�j���O-���
		idDataGrLimitWarningLowerEnable,	// ���[�j���O-����(�\��)
		idDataGrLimitWarningLower,			// ���[�j���O-����
		idDataGrLimitAlarmUpperEnable,		// �A���[��-���(�\��)
		idDataGrLimitAlarmUpper,			// �A���[��-���
		idDataGrLimitAlarmLowerEnable,		// �A���[��-����(�\��)
		idDataGrLimitAlarmLower,			// �A���[��-����
	};//idDataGrLimit

// ����
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnComboBoxSelChange();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

	int m_nComboHeight;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

