/*============================================================================*/
/*! RdnsChkResult.cpp

-# �v�惌�f�B�l�X�`�F�b�N ���ʕ\�����
*/
/*============================================================================*/
// RdnsChkResult.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "RdnsChkResult.h"
#include "afxdialogex.h"

#define RES_TITLE				_T("�Z���f�f���O")
#define RES_FILE_AGCS			_T("AGC_S_RES.txt")
#define RES_FILE_AGCX			_T("AGC_X_RES.txt")
#define RES_FILE_AGCKA			_T("AGC_KA_RES.txt")
#define RES_FILE_RNG			_T("DELAY_RES.txt")
#define RES_FILE_TLM			_T("TLM_RES.txt")
#define RES_FILE_CMD			_T("CMD_RES.txt")

/////////////////////////////////////////////////////
// CCustomDlgProp
CResultViewDlgProp::CResultViewDlgProp(const CString& strName, const COleVariant& varValue, int type, CWnd* pParent) : CMFCPropertyGridProperty(strName, varValue)
{
	m_Type = type;
	m_pParent = pParent;
}

void CResultViewDlgProp::OnClickButton(CPoint /*point*/)
{
	CString dir = theApp.GetShareFilePath(eFileType_Calib_Result_Folder, theApp.GetSelectStation());
	CString strFilePath = _T("");

	// �ڍ׃r���[�\��

	if (m_Type == TYPE_AGC_S)
	{
		strFilePath = dir + RES_FILE_AGCS;
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[0].SetTitle(RES_TITLE);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[0].SetSelectFileName(strFilePath);
		if (((CRdnsChkResult*)m_pParent)->m_DetailDlg[0].GetSafeHwnd() != NULL)
		{
			((CRdnsChkResult*)m_pParent)->m_DetailDlg[0].DestroyWindow();
		}
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[0].Create(IDD_DIALOG_DETAILVIEW, m_pParent);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[0].ShowWindow(SW_SHOW);
	}
	else if (m_Type == TYPE_AGC_X)
	{
		strFilePath = dir + RES_FILE_AGCX;
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[1].SetTitle(RES_TITLE);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[1].SetSelectFileName(strFilePath);
		if (((CRdnsChkResult*)m_pParent)->m_DetailDlg[1].GetSafeHwnd() != NULL)
		{
			((CRdnsChkResult*)m_pParent)->m_DetailDlg[1].DestroyWindow();
		}
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[1].Create(IDD_DIALOG_DETAILVIEW, m_pParent);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[1].ShowWindow(SW_SHOW);
	}
	else if (m_Type == TYPE_RNG)
	{
		strFilePath = dir + RES_FILE_RNG;
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[2].SetTitle(RES_TITLE);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[2].SetSelectFileName(strFilePath);
		if (((CRdnsChkResult*)m_pParent)->m_DetailDlg[2].GetSafeHwnd() != NULL)
		{
			((CRdnsChkResult*)m_pParent)->m_DetailDlg[2].DestroyWindow();
		}
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[2].Create(IDD_DIALOG_DETAILVIEW, m_pParent);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[2].ShowWindow(SW_SHOW);
	}
	else if (m_Type == TYPE_TLM_S)
	{
		strFilePath = dir + RES_FILE_TLM;
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[3].SetTitle(RES_TITLE);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[3].SetSelectFileName(strFilePath);
		if (((CRdnsChkResult*)m_pParent)->m_DetailDlg[3].GetSafeHwnd() != NULL)
		{
			((CRdnsChkResult*)m_pParent)->m_DetailDlg[3].DestroyWindow();
		}
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[3].Create(IDD_DIALOG_DETAILVIEW, m_pParent);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[3].ShowWindow(SW_SHOW);
	}
	else if (m_Type == TYPE_CMD)
	{
		strFilePath = dir + RES_FILE_CMD;
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[4].SetTitle(RES_TITLE);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[4].SetSelectFileName(strFilePath);
		if (((CRdnsChkResult*)m_pParent)->m_DetailDlg[4].GetSafeHwnd() != NULL)
		{
			((CRdnsChkResult*)m_pParent)->m_DetailDlg[4].DestroyWindow();
		}
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[4].Create(IDD_DIALOG_DETAILVIEW, m_pParent);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[4].ShowWindow(SW_SHOW);
	}
	else if (m_Type == TYPE_AGC_KA)
	{
		strFilePath = dir + RES_FILE_AGCKA;
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[5].SetTitle(RES_TITLE);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[5].SetSelectFileName(strFilePath);
		if (((CRdnsChkResult*)m_pParent)->m_DetailDlg[5].GetSafeHwnd() != NULL)
		{
			((CRdnsChkResult*)m_pParent)->m_DetailDlg[5].DestroyWindow();
		}
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[5].Create(IDD_DIALOG_DETAILVIEW, m_pParent);
		((CRdnsChkResult*)m_pParent)->m_DetailDlg[5].ShowWindow(SW_SHOW);
	}
}



/////////////////////////////////////////////////////
// CMFCPropertyGridPropertyEx
CMFCPropertyGridPropertyEx::CMFCPropertyGridPropertyEx(const CString& strName, const CString& value, LPCTSTR lpszDescr, DWORD_PTR dwData)
	:CMFCPropertyGridProperty(strName, (variant_t)value, lpszDescr, dwData)
	, m_TextColor(RGB(0, 0, 0))
{
}

CMFCPropertyGridPropertyEx::~CMFCPropertyGridPropertyEx()
{
}

void CMFCPropertyGridPropertyEx::OnDrawValue(CDC* pDC, CRect rect)
{
	// ���ʂ�NG�̏ꍇ�A�����F��Ԃɂ���B
	COLORREF clr, old;
	CString value = GetValue();

	if (value.CompareNoCase(_T("NG")) == 0)
	{
		clr = RGB(255, 0, 0);
	}
	else
	{
		clr = RGB(0, 0, 0);
	}

	old = pDC->SetTextColor(clr);
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
	pDC->SetTextColor(old);
}

void CMFCPropertyGridPropertyEx::OnDrawName(CDC* pDC, CRect rect)
{
	// Disable�ׁ̈A�����F���D�F�B���ɂ���B
	COLORREF old = pDC->SetTextColor(RGB(0, 0, 0));
	CMFCPropertyGridProperty::OnDrawName(pDC, rect);
	pDC->SetTextColor(old);
}


// CRdnsChkResult �_�C�A���O

IMPLEMENT_DYNAMIC(CRdnsChkResult, CDialogEx)

CRdnsChkResult::CRdnsChkResult(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRdnsChkResult::IDD, pParent)
{

}

CRdnsChkResult::~CRdnsChkResult()
{
}

void CRdnsChkResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_PropListCtrl);
}


BEGIN_MESSAGE_MAP(CRdnsChkResult, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRdnsChkResult::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRdnsChkResult ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CRdnsChkResult

-# �_�C�A���O����������

@param  �Ȃ�
@retval ���̓t�H�[�J�X�ݒ�
*/
/*============================================================================*/
BOOL CRdnsChkResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �V�X�e���R�}���hSC_CLOSE�̎g�p�֎~
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu)
	{
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	}

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	HDITEM hdi;
	hdi.cxy = 140;
	hdi.mask = HDI_WIDTH;
	m_PropListCtrl.GetHeaderCtrl().SetItem(0, &hdi);
	m_PropListCtrl.EnableHeaderCtrl(TRUE, _T("����"), _T("����"));
	m_PropListCtrl.EnableDescriptionArea(FALSE);
	m_PropListCtrl.SetVSDotNetLook(FALSE);

	int lsize = (int)m_RdnsChkList.size();

	vector<CString> AgcSItemList;
	vector<CString> AgcXItemList;
	vector<CString> AgcKaItemList;
	vector<CString> RndItemList;
	vector<CString> TlmSItemList;
	vector<CString> TlmXItemList;
	vector<CString> TlmKaItemList;
	vector<CString> CmdXItemList;
	for (int i = 0; i < lsize; ++i)
	{
		stSendCtrlMsg st = m_RdnsChkList[i];

		if (st.type == TYPE_AGC_S)
		{
			AgcSItemList.push_back(st.RdnsItem);
		}
		else if (st.type == TYPE_AGC_X)
		{
			AgcXItemList.push_back(st.RdnsItem);
		}
		else if (st.type == TYPE_AGC_KA)
		{
			AgcKaItemList.push_back(st.RdnsItem);
		}
		else if (st.type == TYPE_RNG)
		{
			RndItemList.push_back(st.RdnsItem);
		}
		else if (st.type == TYPE_TLM_S)
		{
			TlmSItemList.push_back(st.RdnsItem);
		}
		else if (st.type == TYPE_TLM_X)
		{
			TlmXItemList.push_back(st.RdnsItem);
		}
		else if (st.type == TYPE_TLM_KA)
		{
			TlmKaItemList.push_back(st.RdnsItem);
		}
		else if (st.type == TYPE_CMD)
		{
			CmdXItemList.push_back(st.RdnsItem);
		}
	}

	int size1, size2, size3;
	int totalCnt = 0;
	CMFCPropertyGridPropertyEx* pItem;
	CResultViewDlgProp *pResultDlg;

	m_ItemPropList.clear();
	m_ResultBtnList.clear();

	// AGC�Z��
	size1 = (int)AgcSItemList.size();
	size2 = (int)AgcXItemList.size();
	size3 = (int)AgcKaItemList.size();

	if (size1 > 0 || size2 > 0 || size3 > 0)
	{
		CMFCPropertyGridProperty* pAGC = new CMFCPropertyGridProperty(_T("AGC�Z��"));

		// S��
		for (int i = 0; i < size1; ++i)
		{
			pItem = new CMFCPropertyGridPropertyEx(AgcSItemList[i], (variant_t)_T(""), _T(""), totalCnt++);
			pItem->AllowEdit(FALSE);
			pItem->Enable(FALSE);
			pAGC->AddSubItem(pItem);
			m_ItemPropList[AgcSItemList[i]] = pItem;
		}

		if (size1 > 0)
		{
			// ���O�\���{�^��
			pResultDlg = new CResultViewDlgProp(_T(""), _T(""), TYPE_AGC_S, this);
			pResultDlg->AllowEdit(FALSE);
			pResultDlg->Enable(FALSE);
			pAGC->AddSubItem(pResultDlg);
			m_ResultBtnList[TYPE_AGC_S] = pResultDlg;
		}

		// X��
		for (int i = 0; i < size2; ++i)
		{
			pItem = new CMFCPropertyGridPropertyEx(AgcXItemList[i], (variant_t)_T(""), _T(""), totalCnt++);
			pItem->AllowEdit(FALSE);
			pItem->Enable(FALSE);
			pAGC->AddSubItem(pItem);
			m_ItemPropList[AgcXItemList[i]] = pItem;
		}

		if (size2 > 0)
		{
			// ���O�\���{�^��
			pResultDlg = new CResultViewDlgProp(_T(""), _T(""), TYPE_AGC_X, this);
			pResultDlg->AllowEdit(FALSE);
			pResultDlg->Enable(FALSE);
			pAGC->AddSubItem(pResultDlg);
			m_ResultBtnList[TYPE_AGC_X] = pResultDlg;
		}

		// Ka��
		for (int i = 0; i < size3; ++i)
		{
			pItem = new CMFCPropertyGridPropertyEx(AgcKaItemList[i], (variant_t)_T(""), _T(""), totalCnt++);
			pItem->AllowEdit(FALSE);
			pItem->Enable(FALSE);
			pAGC->AddSubItem(pItem);
			m_ItemPropList[AgcKaItemList[i]] = pItem;
		}

		if (size3 > 0)
		{
			// ���O�\���{�^��
			pResultDlg = new CResultViewDlgProp(_T(""), _T(""), TYPE_AGC_KA, this);
			pResultDlg->AllowEdit(FALSE);
			pResultDlg->Enable(FALSE);
			pAGC->AddSubItem(pResultDlg);
			m_ResultBtnList[TYPE_AGC_KA] = pResultDlg;
		}

		m_PropListCtrl.AddProperty(pAGC);
	}

	// �Ǔ��f�B���C�l�v��
	size1 = (int)RndItemList.size();

	if (size1 > 0)
	{
		CMFCPropertyGridProperty* pRNG = new CMFCPropertyGridProperty(_T("�Ǔ��f�B���C�l�v��"));

		for (int i = 0; i < size1; ++i)
		{
			pItem = new CMFCPropertyGridPropertyEx(RndItemList[i], (variant_t)_T(""), _T(""), totalCnt++);
			pItem->AllowEdit(FALSE);
			pItem->Enable(FALSE);
			pRNG->AddSubItem(pItem);
			m_ItemPropList[RndItemList[i]] = pItem;
		}

		// ���O�\���{�^��
		pResultDlg = new CResultViewDlgProp(_T(""), _T(""), TYPE_RNG, this);
		pResultDlg->AllowEdit(FALSE);
		pResultDlg->Enable(FALSE);
		pRNG->AddSubItem(pResultDlg);
		m_ResultBtnList[TYPE_RNG] = pResultDlg;

		m_PropListCtrl.AddProperty(pRNG);
	}

	// �e�����g���`���`�F�b�N
	size1 = (int)TlmSItemList.size();
	size2 = (int)TlmXItemList.size();
	size3 = (int)TlmKaItemList.size();

	if (size1 > 0 || size2 > 0 || size3 > 0)
	{
		CMFCPropertyGridProperty* pTLM = new CMFCPropertyGridProperty(_T("�e�����g���`���`�F�b�N"));

		for (int i = 0; i < size1; ++i)
		{
			pItem = new CMFCPropertyGridPropertyEx(TlmSItemList[i], (variant_t)_T(""), _T(""), totalCnt++);
			pItem->AllowEdit(FALSE);
			pItem->Enable(FALSE);
			pTLM->AddSubItem(pItem);
			m_ItemPropList[TlmSItemList[i]] = pItem;
		}

		for (int i = 0; i < size2; ++i)
		{
			pItem = new CMFCPropertyGridPropertyEx(TlmXItemList[i], (variant_t)_T(""), _T(""), totalCnt++);
			pItem->AllowEdit(FALSE);
			pItem->Enable(FALSE);
			pTLM->AddSubItem(pItem);
			m_ItemPropList[TlmXItemList[i]] = pItem;
		}

		for (int i = 0; i < size3; ++i)
		{
			pItem = new CMFCPropertyGridPropertyEx(TlmKaItemList[i], (variant_t)_T(""), _T(""), totalCnt++);
			pItem->AllowEdit(FALSE);
			pItem->Enable(FALSE);
			pTLM->AddSubItem(pItem);
			m_ItemPropList[TlmKaItemList[i]] = pItem;
		}

		// ���O�\���{�^��
		pResultDlg = new CResultViewDlgProp(_T(""), _T(""), TYPE_TLM_S, this);
		pResultDlg->AllowEdit(FALSE);
		pResultDlg->Enable(FALSE);
		pTLM->AddSubItem(pResultDlg);
		m_ResultBtnList[TYPE_TLM_S] = pResultDlg;

		m_PropListCtrl.AddProperty(pTLM);
	}

	// �R�}���h�`���`�F�b�N
	size1 = (int)CmdXItemList.size();

	if (size1 > 0)
	{
		CMFCPropertyGridProperty* pCMD = new CMFCPropertyGridProperty(_T("�R�}���h�`���`�F�b�N"));

		for (int i = 0; i < size1; ++i)
		{
			pItem = new CMFCPropertyGridPropertyEx(CmdXItemList[i], (variant_t)_T(""), _T(""), totalCnt++);
			pItem->AllowEdit(FALSE);
			pItem->Enable(FALSE);
			pCMD->AddSubItem(pItem);
			m_ItemPropList[CmdXItemList[i]] = pItem;
		}

		// ���O�\���{�^��
		pResultDlg = new CResultViewDlgProp(_T(""), _T(""), TYPE_CMD, this);
		pResultDlg->AllowEdit(FALSE);
		pResultDlg->Enable(FALSE);
		pCMD->AddSubItem(pResultDlg);
		m_ResultBtnList[TYPE_CMD] = pResultDlg;

		m_PropListCtrl.AddProperty(pCMD);
	}

	// OK�{�^���񊈐���
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


BOOL CRdnsChkResult::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	return CDialogEx::Create(nIDTemplate, pParentWnd);
}

// OK�{�^��
void CRdnsChkResult::OnBnClickedOk()
{
//	DestroyWindow();
	CDialogEx::OnOK();
}


// �~�{�^��
void CRdnsChkResult::OnClose()
{
//	DestroyWindow();
	CDialogEx::OnClose();
}


void CRdnsChkResult::PostNcDestroy()
{
	CloseLogDlg();
	delete this;
	CDialogEx::PostNcDestroy();
}


void CRdnsChkResult::CloseLogDlg()
{
	for (int i = 0; i < 6; ++i)
	{
		if (m_DetailDlg[i].GetSafeHwnd() != NULL)
		{
			m_DetailDlg[i].DestroyWindow();
		}
	}
}


LRESULT CRdnsChkResult::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case eRdnsChk_LogOpen:	// ���O�\���\�ʒm
		{
			// �Y���{�^����Enable
			int type = (int)lParam;
			m_ResultBtnList[type]->Enable(TRUE);
			m_ResultBtnList[type]->SetValue(_T("���O�\��"));
		}
		break;
	case eRdnsChk_Result:			// �Z���f�f���ʒʒm
		{
			CString item = (*(CString*)wParam);
			CString res = (*(CString*)lParam);
			auto itr = m_ItemPropList.find(item);
			if (itr != m_ItemPropList.end())
			{
				m_ItemPropList[item]->SetValue(res);
			}
		}
		break;
	case eRdnsChk_End:			// �Z���f�f�I���ʒm
		{
			// OK�{�^��������
			GetDlgItem(IDOK)->EnableWindow(TRUE);

			// X�{�^��������
			CMenu* pSysMenu = GetSystemMenu(FALSE);
			if (pSysMenu)
			{
				pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
			}
		}
		break;
	default:
		return CDialogEx::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}

