/*============================================================================*/
/*! SummaryPropertiesDlg.cpp

-# �T�}���O���t�ݒ���
*/
/*============================================================================*/
// SummaryPropertiesDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SummaryPropertiesDlg.h"
#include "MFCEqTabCtrl.h"
#include "afxdialogex.h"


/////////////////////////////////////////////////////////////////////////////
// CSummaryPropertiesDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CSummaryPropertiesDlg, CDialogBase)

/*============================================================================*/
/*! CTrendPropertiesDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSummaryPropertiesDlg::CSummaryPropertiesDlg(CWnd* pParent /*=NULL*/)
: CDialogBase(CSummaryPropertiesDlg::IDD, pParent)
{
}


/*============================================================================*/
/*! CSummaryPropertiesDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSummaryPropertiesDlg::~CSummaryPropertiesDlg()
{
}

void CSummaryPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_SUMMARYGRAPTH_PROP_COMBO, m_wndObjectCombo);
//	DDX_Control(pDX, IDC_SUMMARYGRAPTH_PROP_BUTTON, m_btnSetting);
}


BEGIN_MESSAGE_MAP(CSummaryPropertiesDlg, CDialogEx)
//	ON_WM_CREATE()
//	ON_CBN_SELCHANGE(IDC_SUMMARYGRAPTH_PROP_COMBO, &CSummaryPropertiesDlg::OnComboBoxSelChange)
//	ON_BN_CLICKED(IDC_SUMMARYGRAPTH_PROP_BUTTON, &CSummaryPropertiesDlg::OnBnClickedSummarygrapthPropButton)
END_MESSAGE_MAP()


// CSummaryPropertiesDlg ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSummaryPropertiesDlg

-# �_�C�A���O�������ɌĂ΂��

@param	pParentWnd�F�e�E�C���h�E
@param  nIDTemplate�FID
@param	nViewID�F�r���[ID
@param	nStaionID�F��ID
@param	nParentID�F�e�E�B���h�EID
@retval 0�F�������� / 1�F�������s
*/
/*============================================================================*/
BOOL CSummaryPropertiesDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nViewID, UINT nStaionID, UINT nParentID /*= 0*/)
{
//	m_nStationID = nStaionID;
//	m_nViewID = nViewID;
//	m_nParentID = nParentID;
//	GetViewNameList(m_listViewName);
	return CDialogBase::Create(nIDTemplate, pParentWnd);
}


/*============================================================================*/
/*! CSummaryPropertiesDlg

-# �_�C�A���O�������ɌĂ΂��

@param  lpCreateStruct�F�쐬����I�u�W�F�N�g�Ɋւ�����
@retval 0�F�������� / 1�F�������s
*/
/*============================================================================*/
int CSummaryPropertiesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBase::OnCreate(lpCreateStruct) == -1)
		return -1;

//	CRect rectDummy;
//	rectDummy.SetRectEmpty();
//
//	// �R���{ �{�b�N�X�̍쐬:
//	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
//
//	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_PROPERTIES_CB))
//	{
//		TRACE0("�v���p�e�B �R���{ �{�b�N�X���쐬�ł��܂���ł���\n");
//		return -1;      // �쐬�ł��Ȃ��ꍇ
//	}
//
//	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

	return 0;
}


/*============================================================================*/
/*! CTrendGraphView

-# �O���t�r���[�����X�g�̎擾

@param	listViewName�F�r���[�����X�g
@retval	TRUE:�擾���� / FALSE:�擾���s

*/
/*============================================================================*/
BOOL CSummaryPropertiesDlg::GetViewNameList(vector<CString>& listViewName)
{
	CString str;

	if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_XSPGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_SAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_XAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	return TRUE;
}


/*============================================================================*/
/*! CTrendGraphView

-# �O���t�r���[�����X�g�̎擾�i�g�����h�̂݁j

@param	listViewName�F�r���[�����X�g
@retval	TRUE:�擾���� / FALSE:�擾���s

*/
/*============================================================================*/
BOOL CSummaryPropertiesDlg::GetViewTrendNameList(vector<CString>& listViewName)
{
	CString str;

	if (str.LoadString(IDS_TREND_RNGGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_RRGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_SAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	if (str.LoadString(IDS_TREND_XAGCGRAPH) == FALSE)
		return FALSE;
	listViewName.push_back(str);

	return TRUE;
}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# �v���p�e�B�f�[�^���擾����
//
//@param	pos�F�r���[�ʒu
//@retval	�v���p�e�B�f�[�^
//
//*/
///*============================================================================*/
//CPropertiesData&  CSummaryPropertiesDlg::GetPropertiesData(int pos)
//{
//	return theApp.GetSatelliteData().GetSummaryPropertiesData(m_nStationID, pos);
//}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# �v���p�e�B�f�[�^���Z�b�g����
//
//@param	pos�F�r���[�ʒu
//@param	val�F�v���p�e�B�f�[�^
//@retval	�Ȃ�
//
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::SetPropertiesData(int pos, CPropertiesData& val)
//{
////	theApp.GetSatelliteData().SetSummaryPropertiesData(m_nStationID, pos, val);
//}

///*============================================================================*/
///*! CTrendPropertiesDlg
//
//-# �v���p�e�B�ύX���O���t�ɒʒm����
//
//@param	�Ȃ�
//@retval	�Ȃ�
//
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::SendPropertiesChange()
//{
////	// �ύX��ʒm
////	theApp.m_pMainWnd->PostMessage(eMessage_SummaryPropetyChange, (WPARAM)m_nParentID, (LPARAM)m_nViewID);
//}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# �R���{�{�b�N�X�I�����X�V����
//
//@param  �Ȃ�
//@retval �Ȃ�
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::UpdateComboBoxSelect()
//{
////	CPropertiesData& prop = GetPropertiesData(m_nViewID);
////	m_wndObjectCombo.SetCurSel(prop.nGraphIdx);
////	UpdateData(FALSE);
//}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# �R���{�{�b�N�X�I��ύX���ɌĂ΂��
//
//@param  �Ȃ�
//@retval �Ȃ�
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::OnComboBoxSelChange()
//{
//	CPropertiesData& prop = GetPropertiesData(m_nViewID);
//
//	int nCurSel = m_wndObjectCombo.GetCurSel();
//
//	for (int i = 0; i < MAX_SUMMARYTDATA; i++)
//	{
//		prop.dataName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSigName;
//		prop.dataGName[i] = mListSummarySig[m_nStationID][nCurSel][i].strSimpleSigName;
//		prop.dataUnit[i] = mListSummarySig[m_nStationID][nCurSel][i].strUnit;
//	}
//	prop.wndName = m_listViewName[nCurSel];
//	prop.nGraphIdx = nCurSel;
//	SetPropertiesData(m_nViewID, prop);
//
//	// �v���p�e�B�ύX�ʒm
//	SendPropertiesChange();
//}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# �ݒ�{�^���������ɌĂ΂��
//
//@param  �Ȃ�
//@retval �Ȃ�
//*/
///*============================================================================*/
//void CSummaryPropertiesDlg::OnBnClickedSummarygrapthPropButton()
//{
//	// �T�}���v���p�e�B�_�C�A���O���J���ʒm
//	theApp.m_pMainWnd->PostMessage(eMessage_OpenSummaryTrendSetDlg, (WPARAM)m_nViewID, (LPARAM)this);
//
//	CMFCEqTabCtrl* pWnd = (CMFCEqTabCtrl*)GetParent();
//	if (pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
//	{
//		pWnd->SetActiveTab(0);
//	}
//}


/*============================================================================*/
/*! CSummaryPropertiesDlg

-# ���������ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CSummaryPropertiesDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

//	for (int i = 0; i < m_listViewName.size(); i++)
//	{
//		m_wndObjectCombo.InsertString(-1, m_listViewName[i]);
//	}
//	CPropertiesData& prop = GetPropertiesData(m_nViewID);
//	m_wndObjectCombo.SetCurSel(prop.nGraphIdx);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


///*============================================================================*/
///*! CSummaryPropertiesDlg
//
//-# �T�}���R���{�{�b�N�X�X�V���ɌĂ΂��
//
//@param  �Ȃ�
//@retval �Ȃ�
//*/
///*============================================================================*/
//LRESULT CSummaryPropertiesDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message){
//
//	case eMessage_UpdateSummaryTabCombo:
//	{
//		// �T�}���v���p�e�B�R���{�ύX�ʒm
//		UpdateComboBoxSelect();
//		break;
//	}
//
//	default:
//		return CDialogBase::WindowProc(message, wParam, lParam);
//	}
//
//	return TRUE;
//}
