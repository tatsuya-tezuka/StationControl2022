/*============================================================================*/
/*! PropertiesCommonDlg.cpp

-# �q���ߑ��O���t�A�T�}���O���t���ʐݒ���
*/
/*============================================================================*/
// PropertiesCommonDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "PropertiesCommonDlg.h"
#include "afxdialogex.h"


/////////////////////////////////////////////////////////////////////////////
// CCustomDlgProp

CCustomDlgProp::CCustomDlgProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData) : CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{
}

void CCustomDlgProp::OnClickButton(CPoint /*point*/)
{
}

// CPropertiesCommonDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CPropertiesCommonDlg, CDialogBase)

/*============================================================================*/
/*! CPropertiesCommonDlg

-# �R���X�g���N�^

@param	IDD�FIDD
@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CPropertiesCommonDlg::CPropertiesCommonDlg(UINT IDD, CWnd* pParent /*=NULL*/)
: CDialogBase(IDD, pParent)
{
	m_nComboHeight = 0;
	m_nViewID = 0;
	m_nPropertiesID = 0;
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@param	nViewID�F�O���t�r���[ID
@param	strViewName�F�O���t�r���[��
@retval �Ȃ�
*/
/*============================================================================*/
CPropertiesCommonDlg::CPropertiesCommonDlg(UINT IDD, CWnd* pParent, UINT nViewID, vector<CString>& listViewName, UINT nPropertiesID)
	: CDialogBase(IDD, pParent)
	, m_nViewID(nViewID)
	, m_listViewName(listViewName)
	, m_nPropertiesID(nPropertiesID)
	, m_nComboHeight(0)
{
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CPropertiesCommonDlg::~CPropertiesCommonDlg()
{
}

void CPropertiesCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropertiesCommonDlg, CDialogBase)
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


// CPropertiesCommonDlg ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CPropertiesCommonDlg

-# �v���p�e�B��S�W�J���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# �v���p�e�B�̑S�W�J���X�V���ɌĂ΂��

@param  CCmdUI�F���[�U�C���^�[�t�F�C�X�A�C�e��
@retval �Ȃ�
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# �v���p�e�B���\�[�g���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# �v���p�e�B�̃\�[�g���X�V���ɌĂ΂��

@param  CCmdUI�F���[�U�C���^�[�t�F�C�X�A�C�e��
@retval �Ȃ�
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# �t�H�[�J�X���ɌĂ΂��

@param  pOldWnd�F���̓t�H�[�J�X��������CWnd�I�u�W�F�N�g
@retval �Ȃ�
*/
/*============================================================================*/
void CPropertiesCommonDlg::OnSetFocus(CWnd* pOldWnd)
{
	m_wndPropList.SetFocus();
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# �v���p�e�B���X�g�Ƀt�H���g��ݒ肷��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CPropertiesCommonDlg::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
	//	m_wndTabs.SetFont(&m_fntPropList);
	//	m_wndList.SetFont(&m_fntPropList);
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# �v���p�e�B�ύX���ɌĂ΂��

@param  wParam
@param  lParam
@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CPropertiesCommonDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
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
		str = pProp->GetValue().bstrVal;
		prop.trandHScale[0] = (HScale)prop.getHScal(str);
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

/*============================================================================*/
/*! CPropertiesCommonDlg

-# �v���p�e�B�E�B���h�E�f�[�^�V�K�쐬
�O���t�E�B���h�E���쐬���ꂽ���ɁA�v���p�e�B�f�[�^���쐬����B

@param	pos�F�O���t�r���[�ԍ�
@param	name�F�O���t�r���[��
@param	wnd�F�E�B���h�E�n���h��
@retval	�Ȃ�

*/
/*============================================================================*/
void CPropertiesCommonDlg::SetNewPropertyData(int pos, CString name, LPARAM wnd)
{
	m_wndObjectCombo.InsertString(-1, name);
	m_wndObjectCombo.SetCurSel(0);

	// �v���p�e�B�ύX
	ChangePropaties();

//	// �ύX��ʒm
//	SendPropertiesChange();

	return;
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# �v���p�e�B�E�B���h�E�f�[�^�폜
�E�B���h�E���I�����ꂽ���ɁA�v���p�e�B��ʑI������폜����B

@param	pos�F�O���t�r���[�ԍ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CPropertiesCommonDlg::CloseView(int pos)
{
	// �h��ʑI���h�p���X�g�X�V
	CPropertiesData& prop = GetPropertiesData(pos);
	m_wndObjectCombo.DeleteString(m_wndObjectCombo.FindString(0, prop.wndName));
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# �f�[�^���X�g���w�肳�ꂽ�C���f�b�N�X�̃f�[�^�����擾����

@param	idx�F�C���f�b�N�X
@retval	�f�[�^��

*/
/*============================================================================*/
CString CPropertiesCommonDlg::GetDataSelectNameAt(int idx)
{
	CString strData = _T("");
	BOOL bFind = FALSE;
	POSITION pos = m_DataNameList.GetHeadPosition();

	int cnt = 0;
	while (pos != NULL)
	{
		CString name = m_DataNameList.GetNext(pos);

		if (idx == cnt)
		{
			strData = name;
			break;
		}
		cnt++;
	}
	return strData;
}

/*============================================================================*/
/*! CPropertiesCommonDlg

-# ��ʑI���R���{�{�b�N�X���X�g�Đݒ�

@param	vBiew	�F�\�����
@param	name	�F�\������Ă���O���t��
@retval	�Ȃ�

*/
/*============================================================================*/
void CPropertiesCommonDlg::ExchangeViewCombo(BOOL *vBiew, CString name)
{
	// ��ʑI�� �A�C�e���č쐬
	int nCmb = m_wndObjectCombo.GetCount();
	m_wndObjectCombo.ResetContent();

	nCmb = m_wndObjectCombo.GetCount();


	CString strView = _T("");
	for (int i = 0; i < MAX_VIEW; ++i)
	{
		CPropertiesData& prop = GetPropertiesData(i);
		if (vBiew[i])
		{
			m_wndObjectCombo.AddString(prop.wndName);
		}
	}

	m_wndObjectCombo.SelectString(0, name);
	m_wndObjectCombo.UpdateWindow();
}


/*============================================================================*/
/*! CPropertiesCommonDlg

-# �ȈՐݒ�^�u�A�`�惂�[�h�擾
�`�惂�[�h��String����enum�l�ɕϊ�����B

@param	�`�惂�[�h
@retval	�`�惂�[�h�ԍ�

*/
/*============================================================================*/
int CPropertiesCommonDlg::GetDrawMode(CString str)
{
	for (int i = 0; strDrawMode[i] != _T(""); ++i)
	{
		if (str == strDrawMode[i])
		{
			return i;
		}
	}
	return -1;
}




