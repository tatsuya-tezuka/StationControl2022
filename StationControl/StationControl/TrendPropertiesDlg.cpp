/*============================================================================*/
/*! TrendPropertiesDlg.cpp

-# �ǔ��󋵃O���t�ݒ���
*/
/*============================================================================*/
// TrendPropertiesDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TrendPropertiesDlg.h"
#include "afxdialogex.h"
#include "HookMsgBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTrendPropertiesDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CTrendPropertiesDlg, CPropertiesCommonDlg)

/*============================================================================*/
/*! CTrendPropertiesDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CTrendPropertiesDlg::CTrendPropertiesDlg(CWnd* pParent /*=NULL*/)
: CPropertiesCommonDlg(CTrendPropertiesDlg::IDD, pParent)
{
	m_PropertiesType = ePropertiesType_Trend;
	m_nListMax = MAX_TDATA;
	m_bChgGraphPropDlg = FALSE;
	m_bModified = FALSE;
}

/*============================================================================*/
/*! CTrendPropertiesDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@param	nViewID�F�O���t�r���[ID
@param	listViewName�F�O���t�r���[�����X�g
@param	nPropertiesID�F�ݒ�ID�iSatelliteData�N���X���̔z��C���f�b�N�XID�j
@param  nStaionID�F�q��ID
@param  nParentID�F�e�E�B���h�EID
@param  nPropertiesType�F�ݒ�^�C�v�i�g�����h/�T�}���j
@param  strWndName�F�E�B���h�E�^�C�g��
@param	pSummaryProp�F�T�}���v���p�e�B
@retval �Ȃ�
*/
/*============================================================================*/
CTrendPropertiesDlg::CTrendPropertiesDlg(CWnd* pParent, UINT nViewID, vector<CString>& listViewName, UINT nPropertiesID, UINT nStaionID, UINT nParentID /*= 0*/, 
	UINT nPropertiesType /*= ePropertiesType_Trend*/, CString strWndName /*= _T("")*/, CWnd* pSummaryProp /*= NULL*/, BOOL bChgDlg /*= FALSE*/)
	: CPropertiesCommonDlg(CTrendPropertiesDlg::IDD, pParent, nViewID, listViewName, nPropertiesID)
{
	m_nStationID = nStaionID;
	m_nParentID = nParentID;
	m_PropertiesType = nPropertiesType;
	m_strWndName = strWndName;
	if (m_PropertiesType == ePropertiesType_Trend)
	{
		m_nListMax = MAX_TDATA;
	}
	else
	{
		m_nListMax = MAX_SUMMARYTDATA;
	}
	m_pSummaryProp = pSummaryProp;
	m_bChgDlg = bChgDlg;
	m_bModified = FALSE;
}

/*============================================================================*/
/*! CTrendPropertiesDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CTrendPropertiesDlg::~CTrendPropertiesDlg()
{
}

void CTrendPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertiesCommonDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTrendPropertiesDlg, CPropertiesCommonDlg)
	ON_WM_CREATE()
	ON_COMMAND(ID_EXPAND_ALL, &CPropertiesCommonDlg::OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, &CPropertiesCommonDlg::OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, &CPropertiesCommonDlg::OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, &CPropertiesCommonDlg::OnUpdateSortProperties)
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELCHANGE(ID_PROPERTIES_CB, &CTrendPropertiesDlg::OnComboBoxSelChange)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CTrendPropertiesDlg::OnPropertyChanged)
	ON_BN_CLICKED(IDOK, &CTrendPropertiesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTrendPropertiesDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrendPropertiesDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �_�C�A���O�������ɌĂ΂��

@param  lpCreateStruct�F�쐬����I�u�W�F�N�g�Ɋւ�����
@retval 0�F�������� / 1�F�������s
*/
/*============================================================================*/
int CTrendPropertiesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_strWndName.IsEmpty())
	{
		CString strTitle;
		strTitle.Format(_T("%s�ݒ�"), (LPCTSTR)m_strWndName);
		SetWindowText(strTitle);
	}

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �R���{ �{�b�N�X�̍쐬:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_PROPERTIES_CB))
	{
		TRACE0("�v���p�e�B �R���{ �{�b�N�X���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect(&rectCombo);
	m_nComboHeight = rectCombo.Height();

	// �v���p�e�B�V�[�g�쐬
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rectDummy, this, 2))
	{
		TRACE0("�v���p�e�B �O���b�h���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	InitPropList();

	if (m_PropertiesType == ePropertiesType_Trend)
	{
		// �q���ߑ��O���t�̓R���{�{�b�N�X�؂�ւ��ňقȂ�O���t��ݒ�
		for (int i = 0; i < m_listViewName.size(); i++)
		{
			SetNewPropertyData(i, m_listViewName[i], (LPARAM)GetParent());
		}
		// �q���ߑ��O���t�ݒ�
		m_wndObjectCombo.SetCurSel(m_nViewID);
		m_bModified = FALSE;
	}
	else
	{
		// �T�}���O���t�̓R���{�{�b�N�X�̓��X�g�S�ăZ�b�g���Ă������A�v���p�e�B�ݒ�̓J�����g�̂�
		for (int i = 0; i < m_listViewName.size(); i++)
		{
			m_wndObjectCombo.InsertString(-1, m_listViewName[i]);
			m_wndObjectCombo.SetCurSel(0);
		}

		// �v���p�e�B�ύX
		ChangePropaties();
		// �ύX��ʒm
//		SendPropertiesChange();

		// �T�}���ݒ�
		CPropertiesData& prop = GetPropertiesData(m_nPropertiesID);
		m_wndObjectCombo.SetCurSel(prop.nGraphIdx);

		ShowProperties();
		m_bModified = TRUE;
	}

	AdjustLayout();

	UpdatePropatyDataList();

	// �v���p�e�B���o�b�N�A�b�v����
	BackupPropertiesData();

	return 0;
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# ���C�A�E�g�𒲐�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrendPropertiesDlg::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient, rectCombo;

	GetClientRect(rectClient);
	rectClient.bottom -= mOKButtonAreaHeight;
	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyTlb = 4;
	int cyCmb = rectCombo.Size().cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B���X�g������������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrendPropertiesDlg::InitPropList()
{
	int i = 0;
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties(0);	// �v���p�e�B�ύX�㑾���\������

	//------------------------------------------------------
	// �\���ݒ�
	CMFCPropertyGridProperty* pPropView = new CMFCPropertyGridProperty(_T("�\���ݒ�"));

	// �\���^�C�v
	//	CMFCPropertyGridProperty* pPropViewMode = new CMFCPropertyGridProperty(_T("�\���^�C�v"), (variant_t)initPropData.ViewMode, _T("���A���^�C��/�m�����A���^�C��"), idViewMode);
	CMFCPropertyGridProperty* pPropViewMode = new CMFCPropertyGridProperty(_T("�\���^�C�v"), (variant_t)initPropData.ViewMode, _T("���A���^�C��"), idViewMode);
	pPropViewMode->AddOption(_T("���A���^�C��"));
	//	pPropViewMode->AddOption(_T("�m�����A���^�C��"));
	pPropViewMode->AllowEdit(FALSE);
	pPropView->AddSubItem(pPropViewMode);

//	// �J�n����
//	CCustomDlgProp* pPropViewDate = new CCustomDlgProp(_T("�J�n����"), (variant_t)_T(""), _T("�f�[�^�\���J�n����"), idViewDate);
//	pPropView->AddSubItem(pPropViewDate);
//	pPropViewDate->Enable(FALSE);
//	pPropViewDate->SetParent(this);

	m_wndPropList.AddProperty(pPropView);

	//------------------------------------------------------
	// �c���ݒ�
	CMFCPropertyGridProperty* pPropV = new CMFCPropertyGridProperty(_T("�c���ݒ�"));

	// �c���ݒ�-�\���X�P�[��(��)
	CMFCPropertyGridProperty* pPropVL = new CMFCPropertyGridProperty(_T("�\���X�P�[��"));
	pPropV->AddSubItem(pPropVL);

	// �c���ݒ�-�\���X�P�[��(��)-�I�[�g�X�P�[��
	CMFCPropertyGridProperty* pPropVLA = new CMFCPropertyGridProperty(_T("�I�[�g�X�P�[��"), (variant_t)initPropData.Auto, _T("�I�[�g�X�P�[����ON/OFF"), idVLAxisAutoRange);
	pPropVLA->AddOption(_T("On"));
	pPropVLA->AddOption(_T("Off"));
	pPropVLA->AllowEdit(FALSE);
	pPropVL->AddSubItem(pPropVLA);

	// �c���ݒ�-�\���X�P�[��(��)-�ő�l
	CMFCPropertyGridProperty* pPropVLMax = new CMFCPropertyGridProperty(_T("�ő�l"), initPropData.VScalLMax, _T("�ő�l"), idVLAxisMaxRange);
	//	pPropVLMax->EnableSpinControl(TRUE, -100, 100);
	pPropVLMax->Enable(FALSE);
	pPropVL->AddSubItem(pPropVLMax);

	// �c���ݒ�-�\���X�P�[��(��)-�ŏ��l
	CMFCPropertyGridProperty* pPropVLMin = new CMFCPropertyGridProperty(_T("�ŏ��l"), initPropData.VScalLMin, _T("�ŏ��l"), idVLAxisMinRange);
	//	pPropVLMin->EnableSpinControl(TRUE, -100, 100);
	pPropVLMin->Enable(FALSE);
	pPropVL->AddSubItem(pPropVLMin);

	// �c���ݒ�-�ڐ���̊Ԋu
	CMFCPropertyGridProperty* pPropVM = new CMFCPropertyGridProperty(_T("�ڐ���̊Ԋu"), initPropData.VSCalItvl, _T("�ڐ���̊Ԋu"), idVAxisScaleInterval);
	pPropVM->EnableSpinControl(TRUE, AXIS_SCALE_MIN, AXIS_SCALE_MAX);
	pPropV->AddSubItem(pPropVM);

	m_wndPropList.AddProperty(pPropV);


	//------------------------------------------------------
	// �����ݒ�
	CMFCPropertyGridProperty* pPropH = new CMFCPropertyGridProperty(_T("�����ݒ�"));

	// �����ݒ�-�\���X�P�[��(��)
	CMFCPropertyGridProperty* pPropHSU = new CMFCPropertyGridProperty(_T("�\���X�P�[��"), (variant_t)initPropData.HScaleU, _T("�g�����h�O���t�r���[1�̕\���X�P�[��"), idHUAxisScale);
	for (i = 0; strHScale[i] != _T(""); ++i)
	{
		pPropHSU->AddOption(strHScale[i]);
	}
	pPropHSU->AllowEdit(FALSE);
	pPropH->AddSubItem(pPropHSU);

	m_wndPropList.AddProperty(pPropH);

	//-------------------------------------------------------------------------------------------------
	// �f�[�^�ݒ�
	for (int i = 0; i < m_nListMax; ++i){
		// �f�[�^X
		CString str;
		str.Format(_T("�f�[�^%02d"), i + 1);
		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(str);

		// �f�[�^�I��
		CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("�f�[�^�I��"), _T(""), _T("�f�[�^�̑I��"), ((idMax - idAxisMax)*i) + idSelectItem);
		pItem->AddOption(_T(""));
		pItem->AllowEdit(FALSE);
		if (theApp.GetServiceMode() != eServiceMode_oldremote)
		{
			// �������[�g�����[�h�Ŗ����ꍇ�A�Ď��̕ύX��s�ɂ���
			pItem->Enable(FALSE);
		}
		pGroup->AddSubItem(pItem);

		// ����
		str.Format(_T("Data%02d"), i + 1);
		pItem = new CMFCPropertyGridProperty(_T("����"), str, _T("�f�[�^����"), ((idMax - idAxisMax)*i) + idItemName);
		pGroup->AddSubItem(pItem);

		// �P��
		pItem = new CMFCPropertyGridProperty(_T("�P��"), (COleVariant)_T("100"), _T("�f�[�^�̒P��"), ((idMax - idAxisMax)*i) + idItemUnit);
		pItem->Enable(TRUE);
		pGroup->AddSubItem(pItem);

		// �\��
		pItem = new CMFCPropertyGridProperty(_T("�\��"), (variant_t)initPropData.Display, _T("�\����ON/OFF"), ((idMax - idAxisMax)*i) + idDisplay);
		pItem->AddOption(_T("On"));
		pItem->AddOption(_T("Off"));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// �`�惂�[�h
		pItem = new CMFCPropertyGridProperty(_T("�`�惂�[�h"), (variant_t)initPropData.DrawKind, _T("Line/Dots/L&D"), ((idMax - idAxisMax)*i) + idDrawKind);
		pItem->AddOption(_T("Line"));
		pItem->AddOption(_T("Dots"));
		pItem->AddOption(_T("L&D"));
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

		// �\���F
		CMFCPropertyGridColorProperty* pGColorProp = new CMFCPropertyGridColorProperty(_T("�\���F"), 0, NULL, _T("�\���F"), ((idMax - idAxisMax)*i) + idDrawColor);
		pGColorProp->EnableAutomaticButton(_T("����l"), colordef[i]);
		pGColorProp->EnableOtherButton(_T("���̑�..."));
		pGColorProp->SetColor(colordef[i]);

		pGroup->AddSubItem(pGColorProp);

		// ����
		pItem = new CMFCPropertyGridProperty(_T("����"), initPropData.lineWidth, _T("1�`10"), ((idMax - idAxisMax)*i) + idLineWidth);
		pItem->EnableSpinControl(TRUE, 1, 10);
		pItem->Enable(true);
		pItem->AllowEdit(TRUE);
		pGroup->AddSubItem(pItem);

		// �h�b�g�T�C�Y
		pItem = new CMFCPropertyGridProperty(_T("�h�b�g�T�C�Y"), (variant_t)initPropData.DotSize, _T("Large/Medium/Small"), ((idMax - idAxisMax)*i) + idDotSize);
		pItem->AddOption(_T("Small"));
		pItem->AddOption(_T("Medium"));
		pItem->AddOption(_T("Large"));
		pItem->Enable(false);
		pItem->AllowEdit(FALSE);
		pGroup->AddSubItem(pItem);

//		//------------------------------------------------------
//		// �R�����g
//		CMFCPropertyGridProperty* pGroupCm = new CMFCPropertyGridProperty(_T("�R�����g"));
//
//		// �R�����g
//		pItem = new CMFCPropertyGridProperty(_T("�R�����g"), (COleVariant)_T(""), _T("�O���t��̃f�[�^�R�����g"), ((idMax - idAxisMax)*i) + idComment);
//		pGroupCm->AddSubItem(pItem);
//
//		// �R�����g�\��
//		pItem = new CMFCPropertyGridProperty(_T("�\��"), (variant_t)_T("Off"), _T("�R�����g��ON/OFF"), ((idMax - idAxisMax)*i) + idCommentEnable);
//		pItem->AddOption(_T("On"));
//		pItem->AddOption(_T("Off"));
//		pItem->AllowEdit(FALSE);
//		pGroupCm->AddSubItem(pItem);
//
//		pGroupCm->Expand(FALSE);
//		pGroup->AddSubItem(pGroupCm);
//
//		//------------------------------------------------------
//		// �A���[��
//		CMFCPropertyGridProperty* pGroupAl = new CMFCPropertyGridProperty(_T("�A���[��"));
//
//		// �A���[�����\��
//		pItem = new CMFCPropertyGridProperty(_T("�\��"), (variant_t)_T("Off"), _T("�A���[�����\����ON/OFF"), ((idMax - idAxisMax)*i) + idAlarmEnable);
//		pItem->AddOption(_T("On"));
//		pItem->AddOption(_T("Off"));
//		pItem->AllowEdit(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		// �A���[��1 ���
//		pItem = new CMFCPropertyGridProperty(_T("�A���[��1���"), initPropData.Alarm1Major, _T("�A���[���l1���"), ((idMax - idAxisMax)*i) + idAlarm1Major);
//		pItem->Enable(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		// �A���[��1 ����
//		pItem = new CMFCPropertyGridProperty(_T("�A���[��1����"), initPropData.Alarm1Minor, _T("�A���[��1����"), ((idMax - idAxisMax)*i) + idAlarm1Minor);
//		pItem->Enable(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		// �A���[��2 ���
//		pItem = new CMFCPropertyGridProperty(_T("�A���[��2���"), initPropData.Alarm2Major, _T("�A���[��2���"), ((idMax - idAxisMax)*i) + idAlarm2Major);
//		pItem->Enable(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		// �A���[��2 ����
//		pItem = new CMFCPropertyGridProperty(_T("�A���[��2����"), initPropData.Alarm2Minor, _T("�A���[��2����"), ((idMax - idAxisMax)*i) + idAlarm2Minor);
//		pItem->Enable(FALSE);
//		pGroupAl->AddSubItem(pItem);
//
//		pGroupAl->Expand(FALSE);
//
//		pGroup->AddSubItem(pGroupAl);

		m_wndPropList.AddProperty(pGroup);
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# ��ʑI��ύX
��ʑI�����ύX���ꂽ���ɁA�v���p�e�B�����̉�ʗp�ɍX�V����B

@param	bUpdate�F�v���p�e�B�A�b�v�f�[�g�t���O
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::ChangePropaties(BOOL bUpdate)
{
	CString str = _T("");

	// �v���p�e�B�f�[�^�擾
	CPropertiesData& prop = GetPropertiesData(m_nPropertiesID);

	// �\���ݒ�
	CMFCPropertyGridProperty* pView = m_wndPropList.GetProperty(idViewGr);

	// �\���ݒ�-�\���^�C�v
	CMFCPropertyGridProperty* pViewMode = pView->GetSubItem(idViewGrMode);
	str = pViewMode->GetName();
	if (prop.isRealTime)
	{
		pViewMode->SetValue(_T("���A���^�C��"));
	}
//	else
//	{
//		pViewMode->SetValue(_T("�m�����A���^�C��"));
//	}

	// �c���ݒ�
	CMFCPropertyGridProperty* pVAxis = m_wndPropList.GetProperty(idVAxisGr);

	// �c���ݒ�-�\���X�P�[��(��)-�I�[�g�X�P�[��
	CMFCPropertyGridProperty* pVAxisLAuto = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrAuto);
	if (prop.trandVScale.Auto)
	{
		pVAxisLAuto->SetValue(_T("On"));
	}
	else
	{
		pVAxisLAuto->SetValue(_T("Off"));
	}

	// �c���ݒ�-�\���X�P�[��(��)-�ő�l
	CMFCPropertyGridProperty* pVAxisLMax = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrMax);
	pVAxisLMax->SetValue((double)prop.trandVScale.VScalLMax);

	// �c���ݒ�-�\���X�P�[��(��)-�ŏ��l
	CMFCPropertyGridProperty* pVAxisLMin = pVAxis->GetSubItem(idVAxisGrScalLGr)->GetSubItem(idVAxisGrScalLGrMin);
	pVAxisLMin->SetValue((double)prop.trandVScale.VScalLMin);

	if (prop.trandVScale.Auto)
	{
		pVAxisLMax->Enable(FALSE);
		pVAxisLMin->Enable(FALSE);
	}
	else
	{
		pVAxisLMax->Enable(TRUE);
		pVAxisLMin->Enable(TRUE);
	}
	// �c���ݒ�-�ڐ���̊Ԋu
	CMFCPropertyGridProperty* pVAxisScaleItvl = pVAxis->GetSubItem(idVAxisGrScalInvl);
	pVAxisScaleItvl->SetValue((long)prop.trandVScale.VSCalItvl);


	// �����ݒ�
	CMFCPropertyGridProperty* pHAxis = m_wndPropList.GetProperty(idHAxisGr);

	// �����ݒ�-�\���X�P�[��(��)
	CMFCPropertyGridProperty* pHAxisScaleU = pHAxis->GetSubItem(idHAxisGrScaleU);
	pHAxisScaleU->SetValue((variant_t)(strHScale[prop.trandHScale[0]]));

	// �f�[�^
	for (int i = 0; i < m_nListMax; ++i)
	{
		CMFCPropertyGridProperty* pData = m_wndPropList.GetProperty(idDataGr + i);

		// �f�[�^����
		CMFCPropertyGridProperty* pItem = pData->GetSubItem(idDataGrDataSelect);
		pItem->SetValue(prop.dataName[i]);
		str = pItem->GetName();

		// �f�[�^���́i�O���t�p�j
		pItem = pData->GetSubItem(idDataGrName);
		pItem->SetValue(prop.dataGName[i]);
		str = pItem->GetName();

		// �P��
		pItem = pData->GetSubItem(idDataGrItemUnit);
		pItem->SetValue(prop.dataUnit[i]);
		str = pItem->GetName();

		// �\��
		pItem = pData->GetSubItem(idDataGrDisplay);
		str = pItem->GetName();
		if (prop.isViewMode[i])
		{
			pItem->SetValue(_T("On"));
		}
		else
		{
			pItem->SetValue(_T("Off"));
		}

		// �`�惂�[�h
		pItem = pData->GetSubItem(idDataGrDrawKind);
		str = pItem->GetName();
		if (prop.drawMode[i] == Line)
		{
			pItem->SetValue(_T("Line"));
		}
		else if (prop.drawMode[i] == Dots)
		{
			pItem->SetValue(_T("Dots"));
		}
		else if (prop.drawMode[i] == LineAndDots)
		{
			pItem->SetValue(_T("L&D"));
		}

		// �\���F
		CMFCPropertyGridColorProperty* pGColorProp = (CMFCPropertyGridColorProperty*)pData->GetSubItem(idDataGrColor);
		pGColorProp->SetColor(prop.color[i]);
		str = pGColorProp->GetName();

		// ����
		CMFCPropertyGridProperty* pGLineWidth = pData->GetSubItem(idDataGrLineWidth);
		pGLineWidth->SetValue((long)prop.lineWidth[i]);

		// �h�b�g�T�C�Y
		pItem = pData->GetSubItem(idDataGrDotSiz);
		str = pItem->GetName();
		if (prop.dotSize[i] == Small)
		{
			pItem->SetValue(_T("Small"));
		}
		else if (prop.dotSize[i] == Medium)
		{
			pItem->SetValue(_T("Medium"));
		}
		else if (prop.dotSize[i] == Large)
		{
			pItem->SetValue(_T("Large"));
		}

		if (prop.drawMode[i] == Line)
		{
			// �񊈐�
			pItem->Enable(FALSE);
		}
		else
		{
			// ����
			pItem->Enable(TRUE);
		}

//		// �R�����g
//		CMFCPropertyGridProperty* pComment = pData->GetSubItem(idDataGrComment);
//
//		pItem = pComment->GetSubItem(idDataGrCommentStr);
//		str = pItem->GetName();
//		pItem->SetValue(prop.comment[i]);
//
//		// �R�����g�\��
//		pItem = pComment->GetSubItem(idDataGrCommentEnable);
//		if (prop.isComment)
//		{
//			pItem->SetValue(_T("On"));
//		}
//		else
//		{
//			pItem->SetValue(_T("Off"));
//		}
//
//		// �A���[���\��
//		CMFCPropertyGridProperty* pAlarm = pData->GetSubItem(idDataGrAlarm);
//		pItem = pAlarm->GetSubItem(idDataGrAlarmEnable);
//		if (prop.isAlarm)
//		{
//			pItem->SetValue(_T("On"));
//		}
//		else
//		{
//			pItem->SetValue(_T("Off"));
//		}
//
//		// �A���[���l1 ���
//		pItem = pAlarm->GetSubItem(idDataGrAlarm1Major);
//		pItem->SetValue((double)prop.alarm1Major[i]);
//
//		// �A���[���l1 ����
//		pItem = pAlarm->GetSubItem(idDataGrAlarm1Minor);
//		pItem->SetValue((double)prop.alarm1Minor[i]);
//
//		// �A���[���l2 ���
//		pItem = pAlarm->GetSubItem(idDataGrAlarm2Major);
//		pItem->SetValue((double)prop.alarm2Major[i]);
//
//		// �A���[���l2 ����
//		pItem = pAlarm->GetSubItem(idDataGrAlarm2Minor);
//		pItem->SetValue((double)prop.alarm2Minor[i]);
	}

	// �v���p�e�B�f�[�^�Z�b�g
	if (bUpdate)
	{
		SetPropertiesData(m_nPropertiesID, prop);
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �f�[�^�I��p���X�g�X�V
�f�[�^�I��p���X�g���X�V����B

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::UpdatePropatyDataList()
{
	map<DWORD, CString>& data = theApp.GetDataCollection().GetDataNameList(theApp.GetSelectStation());
	vector<stSelectData>::iterator itr;

	m_DataNameList.RemoveAll();
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		m_DataNameList.AddTail((*itr).second);
	}

	for (int i = 0; i < m_nListMax; ++i)
	{
		// �f�[�^�I���R���g���[���擾
		CMFCPropertyGridProperty* pData = m_wndPropList.GetProperty(idDataGr + i);
		CMFCPropertyGridProperty* pItem = pData->GetSubItem(idDataGrDataSelect);

		// �R���{�{�b�N�X������
		CString sName = _T("");
		sName = pItem->GetValue().bstrVal;
		pItem->RemoveAllOptions();
		pItem->AddOption(_T(""));

		BOOL bFind = FALSE;
		POSITION pos = m_DataNameList.GetHeadPosition();
		while (pos != NULL)
		{
			// �f�[�^�I�����X�g�č\�z
			CString name = m_DataNameList.GetNext(pos);
			pItem->AddOption(name);

			if (!bFind && (name == sName))
			{
				bFind = TRUE;
			}
		}

		// ���ݑI�𒆂̃f�[�^�����X�g�ɖ����̂ŁA�\�����폜����B
		if (!bFind)
		{
			pItem->SetValue(_T(""));
		}
	}

#ifdef _NOCOMM
	// �v���p�e�B�ύX
	ChangePropaties();
#endif


}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B�f�[�^���擾����

@param	pos�F�r���[�ʒu
@retval	�v���p�e�B�f�[�^

*/
/*============================================================================*/
CPropertiesData&  CTrendPropertiesDlg::GetPropertiesData(int pos)
{
	if (m_PropertiesType == ePropertiesType_Trend)
		return theApp.GetSatelliteData().GetTrendPropertiesData(pos);
	else
		return theApp.GetSatelliteData().GetSummaryPropertiesData(m_nStationID, pos);
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B�f�[�^���Z�b�g����

@param	pos�F�r���[�ʒu
@param	val�F�v���p�e�B�f�[�^
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::SetPropertiesData(int pos, CPropertiesData& val)
{
	if (m_PropertiesType == ePropertiesType_Trend)
		theApp.GetSatelliteData().SetTrendPropertiesData(pos, val);
	else
		theApp.GetSatelliteData().SetSummaryPropertiesData(m_nStationID, pos, val);
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B�ύX���O���t�ɒʒm����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::SendPropertiesChange()
{
	if (m_PropertiesType == ePropertiesType_Trend)
		// �ύX��ʒm
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)m_nViewID);
	else
		// �ύX��ʒm
		theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_nViewID);
}

/*============================================================================*/
/*! CTrendPropertiesDlg

-# OK�{�^���N���b�N���ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::OnBnClickedOk()
{
	// �m�F�_�C�A���O�\��
	CString title;
	GetWindowText(title);

	if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("���s���܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;

//	// �g�����h�ݒ��ۑ�����
//	theApp.GetSatelliteData().TrendPropWriteData(m_nStationID);

	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	m_wndPropList.RemoveAll();

	CDialogBase::OnOK();
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �L�����Z���{�^���N���b�N���ɌĂ΂��

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::OnBnClickedCancel()
{
	if (!m_bChgGraphPropDlg)
		// �o�b�N�A�b�v�����v���p�e�B�����ɖ߂�
		RestorePropertiesData();

	m_wndPropList.RemoveAll();

	CDialogBase::OnCancel();
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B�f�[�^���o�b�N�A�b�v����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::BackupPropertiesData()
{
	// �v���p�e�B�f�[�^��߂�
	if (m_PropertiesType == ePropertiesType_Trend)
	{
		// �v���p�e�B�f�[�^�擾B
		for (int i = 0; i < eTrendWndType_Num; i++)
		{
			m_backupPropertiesData[i] = GetPropertiesData(i);
		}
	}
	else
	{
		// �v���p�e�B�f�[�^�擾
		for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
		{
			m_backupSummaryPropertiesData[i] = GetPropertiesData(i);
		}
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B�f�[�^�𕜌�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::RestorePropertiesData()
{
	// �v���p�e�B�f�[�^�𕜌�����
	if (m_PropertiesType == ePropertiesType_Trend)
	{
		for (int i = 0; i < eTrendWndType_Num; i++)
		{
			SetPropertiesData(i, m_backupPropertiesData[i]);
			if (m_bModified)
			{
				// �ύX��ʒm
				theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 0, (LPARAM)i);
			}
		}
	}
	else
	{
		for (int i = 0; i < MAX_SUMMARY_VIEW; i++)
		{
			SetPropertiesData(i, m_backupSummaryPropertiesData[i]);
			if (m_bModified)
			{
				// �ύX��ʒm
				theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)i);
			}
		}
	}
	m_bModified = FALSE;
}

/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B�\�����A�N�e�B�u�E�B���h�E�p�ɕύX
�\������Ă���E�B���h�E���ύX���ꂽ��A�v���p�e�B�[�\����ύX����B

@param	pos�F�O���t�r���[�ԍ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTrendPropertiesDlg::ChangeView(int pos)
{
	CPropertiesData& prop = GetPropertiesData(pos);
	m_wndObjectCombo.SelectString(0, prop.wndName);
	m_nPropertiesID = pos;

	// �v���p�e�B�ύX
	ChangePropaties();
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �R���{�{�b�N�X�I��ύX���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrendPropertiesDlg::OnComboBoxSelChange()
{
	// �g�����h�̎�
	if (m_PropertiesType == ePropertiesType_Trend)
	{
		int pos = -1;
		CString name;
		m_wndObjectCombo.GetWindowTextW(name);
		for (int i = 0; i < eTrendWndType_Num; ++i)
		{
			CPropertiesData& prop = GetPropertiesData(i);

			if (name == prop.wndName)
			{
				pos = prop.nGraphIdx;
				break;
			}
		}

		if (pos == -1)
			return;

		// �R���{�Ŏw�肵���r���[�ɐ؂�ւ���
		ChangeView(pos);
	}
	// �T�}���̎�
	else
	{
		m_bModified = TRUE;
		CPropertiesData& prop = GetPropertiesData(m_nPropertiesID);

		int nCurSel = m_wndObjectCombo.GetCurSel();

		// �X�y�N�g�����I�����́A�ݒ�_�C�A���O��ς���B
		if (nCurSel == eSummaryWndSetType_XSP && m_nStationID != eStation_Uchinoura20 ||
			nCurSel == eSummaryWndSetType_KASP && m_nStationID == eStation_Usuda642)
		{
			CString title;
			GetWindowText(title);
			if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�X�y�N�g�����ݒ�E�B���h�E�ɐ؂�ւ��܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
				return;

			prop.nGraphIdx = nCurSel;

			CRect rect;
			GetWindowRect(&rect);
			theApp.SetWindowTmpPoint(rect.left, rect.top);

			// �g�����h�O���t�؂�ւ��ʒm
			theApp.m_pMainWnd->PostMessage(eMessage_InitSummaryWnd, (WPARAM)m_nParentID, (LPARAM)m_nViewID);
			// �T�}���X�y�N�g�����v���p�e�B�_�C�A���O���J���ʒm
			theApp.m_pMainWnd->PostMessage(eMessage_OpenSummarySpectrumSetDlgChg, (WPARAM)m_pSummaryProp, (LPARAM)this);

			// �v���p�e�B�f�[�^�Z�b�g
			SetPropertiesData(m_nPropertiesID, prop);
			ShowProperties();

			// �v���p�e�B�ύX
			ChangePropaties();
			// �v���p�e�B�ύX�ʒm
			SendPropertiesChange();
//			// �v���p�e�B�ۑ�
//			theApp.GetSatelliteData().TrendPropWriteData(m_nStationID);

			// �g�����h�ݒ�_�C�A���O�����
			m_bChgGraphPropDlg = TRUE;
			PostMessage(WM_CLOSE);
		}
		// �g�����h�̏ꍇ�̓v���p�e�B�ύX
		else
		{
			prop.nGraphIdx = nCurSel;
			for (int i = 0; i < MAX_SUMMARYTDATA; i++)
			{
				prop.dataName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSigName;
				prop.dataGName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSimpleSigName;
				prop.dataUnit[i] = mListSummarySig[m_nStationID][nCurSel][i].strUnit;
			}

			// �g�����h�O���t�؂�ւ��ʒm
			theApp.m_pMainWnd->PostMessage(eMessage_InitSummaryWnd, (WPARAM)m_nParentID, (LPARAM)m_nViewID);
			// �v���p�e�B�f�[�^�Z�b�g
			SetPropertiesData(m_nPropertiesID, prop);
			ShowProperties();

			// �v���p�e�B�ύX
			ChangePropaties();
			// �v���p�e�B�ύX�ʒm
			SendPropertiesChange();
		}
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B��\������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrendPropertiesDlg::ShowProperties()
{
	int nSel = m_wndObjectCombo.GetCurSel();
	// �~�j�X�y�N�g�����̏ꍇ
	if (nSel == eSummaryWndSetType_XSP && m_nStationID != eStation_Uchinoura20 ||
		nSel == eSummaryWndSetType_KASP && m_nStationID == eStation_Usuda642)
	{
		// ��\��
		m_wndPropList.ShowWindow(SW_HIDE);
	}
	// ����ȊO�̏ꍇ
	else
	{
		// �\��
		m_wndPropList.ShowWindow(SW_SHOW);
	}
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �E�B���h�E����鎞�ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTrendPropertiesDlg::OnClose()
{
	if (!m_bChgGraphPropDlg)
	{
		CString title;
		GetWindowText(title);

		if (IDCANCEL == MessageBoxHooked(this->m_hWnd, _T("�I�����܂��B\n��낵���ł����H"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
			return;
	}

	CDialogEx::OnClose();
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �_�C�A���O���������ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CTrendPropertiesDlg::OnInitDialog()
{
	CPropertiesCommonDlg::OnInitDialog();

	if (m_bChgDlg)
	{
		CPoint pt = theApp.GetWindowTmpPoint();
		SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
	else
	{
		CenterWindowEx();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CTrendPropertiesDlg

-# �v���p�e�B�ύX���ɌĂ΂��

@param  wParam
@param  lParam
@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CTrendPropertiesDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	m_bModified = TRUE;

	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;

	CString		str = _T("");
	COLORREF	color = 0;
	double		dval = 0;
	long		lval = 0;
	int			ival = 0;

	int pos = 0;
	int data = (int)pProp->GetData();

	if (data > idAxisMax)
	{
		pos = ((int)pProp->GetData() - idAxisMax) / (idMax - idAxisMax);
		data = (((int)pProp->GetData() - idAxisMax) % (idMax - idAxisMax)) + idAxisMax;
	}

	CString item = pProp->GetName();
	CPropertiesData& prop = GetPropertiesData(m_nPropertiesID);

	switch (data){
	case idViewMode:	// �\���^�C�v
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("���A���^�C��")) == 0)
		{
			prop.isRealTime = true;
			CCustomDlgProp* pStartDate = (CCustomDlgProp*)pProp->GetParent()->GetSubItem(idViewGrDate);
			pStartDate->Enable(FALSE);
		}
		break;

	case idVLAxisAutoRange:	// �c�����I�[�g�X�P�[��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.trandVScale.Auto = true;

			// �ő�l�A�ŏ��l��񊈐��ɂ���B
			CMFCPropertyGridProperty* pMax = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMax);
			pMax->Enable(FALSE);
			CMFCPropertyGridProperty* pMin = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMin);
			pMin->Enable(FALSE);
		}
		else
		{
			prop.trandVScale.Auto = false;

			// �ő�l�A�ŏ��l�������ɂ���B
			CMFCPropertyGridProperty* pMax = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMax);
			pMax->Enable(TRUE);
			CMFCPropertyGridProperty* pMin = pProp->GetParent()->GetSubItem(idVAxisGrScalLGrMin);
			pMin->Enable(TRUE);
		}
		break;

	case idVLAxisMaxRange:	// �c�����ő�l
		dval = pProp->GetValue().dblVal;
		if (dval > 10000000000000)
		{
			dval = 9999999999999.999999;
			pProp->SetValue(dval);
		}
		else if (dval < -10000000000000)
		{
			dval = -9999999999999.999999;
			pProp->SetValue(dval);
		}
		prop.trandVScale.VScalLMax = dval;
		break;

	case idVLAxisMinRange:	// �c�����ŏ��l
		dval = pProp->GetValue().dblVal;
		if (dval > 10000000000000)
		{
			dval = 9999999999999.999999;
			pProp->SetValue(dval);
		}
		else if (dval < -10000000000000)
		{
			dval = -9999999999999.999999;
			pProp->SetValue(dval);
		}
		prop.trandVScale.VScalLMin = dval;
		break;

	case idVAxisScaleInterval:	// �c���ڐ���̊Ԋu
		lval = pProp->GetValue().lVal;
		if (lval < AXIS_SCALE_MIN)
		{
			lval = AXIS_SCALE_MIN;
			pProp->SetValue(lval);
			pProp->Redraw();
		}
		else if (lval > AXIS_SCALE_MAX)
		{
			lval = AXIS_SCALE_MAX;
			pProp->SetValue(lval);
			pProp->Redraw();
		}
		prop.trandVScale.VSCalItvl = lval;
		break;

	case idHUAxisScale:		// ������X�P�[��
		{
			str = pProp->GetValue().bstrVal;
			HScale hscale = (HScale)prop.getHScal(str);
			// �g�����h�̎�
			if (m_PropertiesType == ePropertiesType_Trend)
			{
				// �����X�P�[�����ύX�̏ꍇ
				if (prop.trandHScale[0] != hscale)
				{
					// ���v���p�e�B�̃X�P�[���ύX
					prop.trandHScale[0] = hscale;
					// �S�Ẵv���p�e�B�̉��������킹�ĕύX
					for (int i = 0; i < eTrendWndType_Num; ++i)
					{
						if (i == m_nPropertiesID)
							continue;
	
						CPropertiesData& propOther = GetPropertiesData(i);
						propOther.trandHScale[0] = hscale;
					}
				}
			}
			else
			{
				// ���v���p�e�B�̃X�P�[���ύX
				prop.trandHScale[0] = hscale;
			}
		}
		break;

	case idHLAxisScale:		// �������X�P�[��
		str = pProp->GetValue().bstrVal;
		prop.trandHScale[1] = (HScale)prop.getHScal(str);
		break;

	case idSelectItem:	// �f�[�^�I��
		str = pProp->GetValue().bstrVal;
		prop.dataName[pos] = str;
		break;

	case idItemName:	// ����
		str = pProp->GetValue().bstrVal;
		prop.dataGName[pos] = str;
		break;

	case idDisplay:	// �\��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.isViewMode[pos] = true;
		}
		else
		{
			prop.isViewMode[pos] = false;
		}
		break;

	case idDrawKind:	// �`�惂�[�h
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("Line")) == 0)
		{
			prop.drawMode[pos] = Line;

			// �����ύX��
			CMFCPropertyGridProperty* pLineProp = pProp->GetParent()->GetSubItem(idDataGrLineWidth);
			pLineProp->Enable(TRUE);
			// �h�b�g�T�C�Y �ύX�s��
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(FALSE);
		}
		else if (str.Compare(_T("Dots")) == 0)
		{
			prop.drawMode[pos] = Dots;

			// �����ύX�s��
			CMFCPropertyGridProperty* pLineProp = pProp->GetParent()->GetSubItem(idDataGrLineWidth);
			pLineProp->Enable(FALSE);
			// �h�b�g�T�C�Y �ύX��
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(TRUE);
		}
		else if (str.Compare(_T("L&D")) == 0)
		{
			prop.drawMode[pos] = LineAndDots;

			// �����ύX��
			CMFCPropertyGridProperty* pLineProp = pProp->GetParent()->GetSubItem(idDataGrLineWidth);
			pLineProp->Enable(TRUE);
			// �h�b�g�T�C�Y �ύX��
			CMFCPropertyGridProperty* pDotProp = pProp->GetParent()->GetSubItem(idDataGrDotSiz);
			pDotProp->Enable(TRUE);
		}

		break;

	case idDrawColor:	// �\���F
	{
		CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*)pProp;
		color = pColor->GetColor();
		if (color == -1)
		{
			prop.color[pos] = colordef[pos];
			pColor->SetColor(prop.color[pos]);
		}
		else
		{
			prop.color[pos] = color;
		}
	}
	break;

	case idLineWidth:	// ����
		lval = pProp->GetValue().lVal;
		if (lval > 10)
		{
			lval = 10;
			pProp->SetValue(lval);
		}
		else if (lval < 1)
		{
			lval = 1;
			pProp->SetValue(lval);
		}
		prop.lineWidth[pos] = lval;
		break;

	case idDotSize:		// �h�b�g�T�C�Y
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("Small")) == 0)
		{
			prop.dotSize[pos] = Small;
		}
		else if (str.Compare(_T("Medium")) == 0)
		{
			prop.dotSize[pos] = Medium;
		}
		else if (str.Compare(_T("Large")) == 0)
		{
			prop.dotSize[pos] = Large;
		}
		break;

	case idVScale:		// �����X�P�[���I��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("��")) == 0)
		{
			prop.vscaleKind[pos] = LScale;
		}
		else
		{
			prop.vscaleKind[pos] = RScale;
		}
		break;

	case idComment:			// �R�����g
		str = pProp->GetValue().bstrVal;
		prop.comment[pos] = str;
		break;

	case idCommentEnable:	// �R�����g�\��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.isComment[pos] = true;
		}
		else
		{
			prop.isComment[pos] = false;
		}
		break;

	case idAlarmEnable:		// �A���[���\��
		str = pProp->GetValue().bstrVal;
		if (str.Compare(_T("On")) == 0)
		{
			prop.isAlarm[pos] = true;
		}
		else
		{
			prop.isAlarm[pos] = false;
		}
		break;
	}

	// �v���p�e�B�f�[�^�Z�b�g
	SetPropertiesData(m_nPropertiesID, prop);

	// �ύX��ʒm
	SendPropertiesChange();

	return 0;
}
