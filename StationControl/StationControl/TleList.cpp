/*============================================================================*/
/*! TleList.cpp

-# �\��l�ꗗ��ʁ@TLE�ꗗ
*/
/*============================================================================*/
// TleList.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TleList.h"
#include "afxdialogex.h"

//----------------------------------------------------------------------
// �_�~�[�f�[�^

static const CString strSampleSatelliteName[5] = {
	_T("ALL"),
	_T("GCOM-C"),
	_T("ALOS-2"),
	_T("HAYABUSA2"),
	_T("123456789012345")
};

static const CTleList::structListItem sampleListItem[13] =
{
	// �q����,               ����,           EPOCH,                        �`����������               
	{ _T("GCOM-C"),          CTleList::eNOM, CTime::GetCurrentTime() + 1,  CTime::GetCurrentTime() + 2  },
	{ _T("GCOM-C"),          CTleList::eNOM, CTime::GetCurrentTime() + 3,  CTime::GetCurrentTime() + 4  },
	{ _T("GCOM-C"),          CTleList::eOFF, CTime::GetCurrentTime() + 5,  CTime::GetCurrentTime() + 6  },
	{ _T("ALOS-2"),          CTleList::eNOM, CTime::GetCurrentTime() + 7,  CTime::GetCurrentTime() + 8  },
	{ _T("ALOS-2"),          CTleList::eNOM, CTime::GetCurrentTime() + 9,  CTime::GetCurrentTime() + 10 },
	{ _T("ALOS-2"),          CTleList::eNOM, CTime::GetCurrentTime() + 11, CTime::GetCurrentTime() + 12 },
	{ _T("ALOS-2"),          CTleList::eOFF, CTime::GetCurrentTime() + 13, CTime::GetCurrentTime() + 14 },
	{ _T("HAYABUSA2"),       CTleList::eOFF, CTime::GetCurrentTime() + 15, CTime::GetCurrentTime() + 16 },
	{ _T("HAYABUSA2"),       CTleList::eOFF, CTime::GetCurrentTime() + 17, CTime::GetCurrentTime() + 18 },
	{ _T("123456789012345"), CTleList::eNOM, CTime::GetCurrentTime() + 19, CTime::GetCurrentTime() + 20 },
	{ _T("123456789012345"), CTleList::eNOM, CTime::GetCurrentTime() + 21, CTime::GetCurrentTime() + 22 },
	{ _T("123456789012345"), CTleList::eNOM, CTime::GetCurrentTime() + 23, CTime::GetCurrentTime() + 24 },
	{ _T("123456789012345"), CTleList::eNOM, CTime::GetCurrentTime() + 25, CTime::GetCurrentTime() + 26 },
};

static const CString strShikibetsu[2] = {
	_T("NOM"),
	_T("OFF"),
};


// CTleList �_�C�A���O

IMPLEMENT_DYNAMIC(CTleList, CDialogBase)

/*============================================================================*/
/*! CTleList

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CTleList::CTleList(CWnd* pParent /*=NULL*/)
	: CDialogBase(CTleList::IDD, pParent)
	, m_bUpperSort(TRUE)
	, m_nSelectedColumn(0)
{

}

/*============================================================================*/
/*! CTleList

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CTleList::~CTleList()
{
}

void CTleList::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TLELIST_COMBO, m_CCBoxSatelliteName);
	DDX_Control(pDX, IDC_TLELIST_LIST1, m_listTle);
}


BEGIN_MESSAGE_MAP(CTleList, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_RC2, &CTleList::OnMenuClose)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CTleList::OnHdnItemclickTlelistList1)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON2, &CTleList::OnBnClickedTlelistButton2)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON1, &CTleList::OnBnClickedTlelistButton1)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON3, &CTleList::OnBnClickedTlelistButton3)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON4, &CTleList::OnBnClickedTlelistButton4)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON5, &CTleList::OnBnClickedTlelistButton5)
	ON_BN_CLICKED(IDC_TLELIST_BUTTON6, &CTleList::OnBnClickedTlelistButton6)
	ON_CBN_SELCHANGE(IDC_TLELIST_COMBO, &CTleList::OnCbnSelchangeTlelistCombo)
END_MESSAGE_MAP()


/*============================================================================*/
/*! CTleList

-# ���j���[����̃t�@�C���N���[�Y���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CTleList::OnMenuClose()
{
	OnOK();
}

// CTleList ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! CTleList

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CTleList::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// �T���v���f�[�^����
	CreateSampleData();

	// �R���{�{�b�N�X������
	InitComboBox();

	// ���X�g������
	if (!InitList())
		return FALSE;

	// ���X�g�A�C�e���}��
	if (!InsertListItem())
		return FALSE;

	//��ɃX�N���[���o�[��\��������
	m_listTle.EnableScrollBarCtrl(SB_VERT, TRUE);

	UpdateData(FALSE);

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! CTleList

-# �T���v���f�[�^�ݒ�

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTleList::CreateSampleData()
{
	const int itemNum = sizeof(sampleListItem) / sizeof(sampleListItem[0]);

	for (int i = 0; i < itemNum; i++)
	{
		m_dequeItem.push_back(sampleListItem[i]);
	}
}


/*============================================================================*/
/*! CTleList

-# �R���{�{�b�N�X������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CTleList::InitComboBox()
{
	int sz = sizeof(strSampleSatelliteName) / sizeof(strSampleSatelliteName[0]);
	for (int i = 0; i < sz; ++i)
	{
		m_CCBoxSatelliteName.InsertString(-1, strSampleSatelliteName[i]);
	}
	m_CCBoxSatelliteName.SetCurSel(0);
}


/*============================================================================*/
/*! CTleList

-# ���X�g�R���g���[��������

@param	�Ȃ�
@retval	TRUE:���������� / FALSE:���������s

*/
/*============================================================================*/
BOOL CTleList::InitList()
{
	LVCOLUMN lvc;
	TCHAR szMsg[256];

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_TLELIST_SATELLITENAME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 0;
	lvc.pszText = szMsg;
	lvc.cx = m_nSatelliteNameWidth;
	if (m_listTle.InsertColumn(0, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_TLELIST_SHIKIBETSU, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 1;
	lvc.pszText = szMsg;
	lvc.cx = m_nSikibetuWidth;
	if (m_listTle.InsertColumn(1, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_TLELIST_EPOCH, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 2;
	lvc.pszText = szMsg;
	lvc.cx = m_epochTimeWidth;
	if (m_listTle.InsertColumn(2, &lvc) < 0)
		return FALSE;

	memset(szMsg, 0, sizeof(szMsg) / sizeof(szMsg[0]));
	if (!LoadString(GetModuleHandle(NULL), IDS_TLELIST_PROPPROCTIME, szMsg, sizeof(szMsg) / sizeof(szMsg[0])))
		return FALSE;
	lvc.iSubItem = 3;
	lvc.pszText = szMsg;
	lvc.cx = m_propProcTimeWidth;
	if (m_listTle.InsertColumn(3, &lvc) < 0)
		return FALSE;

	m_listTle.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;
}

/*============================================================================*/
/*! CTleList

-# ���X�g�R���g���[���ɃA�C�e���}��

@param	�Ȃ�
@retval	TRUE:�}������ / FALSE:�}�����s

*/
/*============================================================================*/
BOOL CTleList::InsertListItem()
{
	SetSatelliteName();

	m_listTle.DeleteAllItems();

	LVITEM lvi;
	CString str;
	int i, index;

	for (i = 0, index = 0; i < (int)m_dequeItem.size(); i++)
	{
		// �q������ALL�܂��͈�v�̂Ƃ��Ƀ��X�g�ɒǉ�
		if (!(m_strSatelliteName.CompareNoCase(_T("All")) == 0 ||
			m_strSatelliteName.CompareNoCase(m_dequeItem[i].m_satelliteName) == 0))
			continue;

		lvi.mask = LVIF_TEXT;

		lvi.iItem = i;
		lvi.iSubItem = 0;
		str.Format(m_dequeItem[i].m_satelliteName);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if ((index = m_listTle.InsertItem(&lvi)) < 0)
			return FALSE;

		lvi.iItem = index;
		lvi.iSubItem = 1;
		str.Format(strShikibetsu[m_dequeItem[i].m_shikibetsu]);
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listTle.SetItem(&lvi) < 0)
			return FALSE;

		lvi.iItem = index;
		lvi.iSubItem = 2;
		str = m_dequeItem[i].m_epochTime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listTle.SetItem(&lvi) < 0)
			return FALSE;

		lvi.iItem = index;
		lvi.iSubItem = 3;
		str = m_dequeItem[i].m_propProcTime.Format("%Y-%m-%d %H:%M:%S");
		lvi.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		if (m_listTle.SetItem(&lvi) < 0)
			return FALSE;
	}

	for (int i = 0; i < m_listTle.GetItemCount(); i++) {
		m_listTle.SetItemData(i, i);
	}

	return TRUE;
}

/*============================================================================*/
/*! CTleList

-# ���X�g�R���g���[���̃A�C�e�����X�V����

@param	�Ȃ�
@retval	TRUE:�X�V���� / FALSE:�X�V���s

*/
/*============================================================================*/
BOOL CTleList::UpdateListItem()
{
	int nItemCount = m_listTle.GetItemCount();
	CRect rect;
	m_listTle.GetItemRect(0, &rect, LVIR_BOUNDS);
	int index = m_listTle.GetTopIndex();
	int offset = rect.Height() * index;
	CSize cs;
	cs.cx = 0;
	cs.cy = offset;

	// �A�C�e���}��
	InsertListItem();

	// �A�C�e�������ς���Ă��Ȃ��ꍇ�́A�L�^�����X�N���[���ʒu�ɃZ�b�g����
	if (nItemCount == m_listTle.GetItemCount())
	{
		if (offset)
		{
			m_listTle.Scroll(cs);
		}
	}

	return TRUE;
}


/*============================================================================*/
/*! CTleList

-# ���X�g�R���g���[���\�[�g���̃R�[���o�b�N�֐�

@param	lParam1�F		��r���A�C�e��
@param	lParam2�F		��r��A�C�e��
@param	lParamSort�F	�E�B���h�E�n���h��
@retval 0: ��v, -1:��r���̏��Ԃ��Ⴂ, 1:��r���̏��Ԃ�����

*/
/*============================================================================*/
int CALLBACK CTleList::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTleList* dlg = (CTleList*)lParamSort;
	return dlg->CompareFunc(lParam1, lParam2);

}

/*============================================================================*/
/*! CTleList

-# ���X�g�R���g���[���\�[�g���̔�r�֐�

@param	lParam1�F		��r���A�C�e��
@param	lParam2�F		��r��A�C�e��
@retval 0: ��v, -1:��r���̏��Ԃ��Ⴂ, 1:��r���̏��Ԃ�����

*/
/*============================================================================*/
int CTleList::CompareFunc(LPARAM lParam1, LPARAM lParam2)
{
	CString str[2];
	LPARAM param[2] = { lParam1, lParam2 };

	// ���X�g�R���g���[�������r����A�C�e���擾
	for (int i = 0; i < 2; ++i)
	{
		LVFINDINFO fi = { 0 };
		fi.flags = LVFI_PARAM;
		fi.lParam = param[i];
		str[i] = m_listTle.GetItemText(m_listTle.FindItem(&fi), m_nSelectedColumn);
	}

	// ��r����
	int ret = 0;
	if (m_bUpperSort)
	{
		ret = str[0].CompareNoCase(str[1]);
	}
	else
	{
		ret = str[1].CompareNoCase(str[0]);
	}

	return ret;
}

/*============================================================================*/
/*! CTleList

-# �q�����擾

@param	�Ȃ�
@retval	�q����

*/
/*============================================================================*/
CString CTleList::GetSatelliteName()
{
	CString str;
	UpdateData(TRUE);
	int nIndex = m_CCBoxSatelliteName.GetCurSel();
	if (nIndex < 0)
		return str;
	m_CCBoxSatelliteName.GetLBText(nIndex, str);

	return str;
}

/*============================================================================*/
/*! CTleList

-# �q�������ύX����Ă���ꍇ�ɉq�������Z�b�g

@param	�Ȃ�
@retval TRUE: �ύX����Ă���B FALSE: �ύX����Ă��Ȃ��B

*/
/*============================================================================*/
BOOL CTleList::SetSatelliteName()
{
	CString str = GetSatelliteName();
	// �q�������ύX���ꂽ�ꍇ
	if (m_strSatelliteName.CompareNoCase(str) != 0)
	{
		// �q�������Z�b�g����
		m_strSatelliteName = str;
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
/*! CTleList

-# �I������Ă���TLE�ꗗ���폜����

@param	�Ȃ�
@retval TRUE: �폜�����B FALSE: �폜���Ȃ��B

*/
/*============================================================================*/
BOOL CTleList::DeleteListItem()
{
	POSITION pos;
	int nItem;
	int number;
	deque<structListItem>::iterator itr;
	BOOL ret = FALSE;

	while (TRUE)
	{
		pos = m_listTle.GetFirstSelectedItemPosition();
		if (!pos)
			break;

		nItem = m_listTle.GetNextSelectedItem(pos);
		m_listTle.DeleteItem(nItem);
		for (itr = m_dequeItem.begin(), number = 0; itr != m_dequeItem.end(); itr++, number++)
		{
			if (number == nItem){
				m_dequeItem.erase(itr);
				ret = TRUE;
				break;
			}
		}
	}

	UpdateData(FALSE);
	UpdateWindow();

	return ret;
}


/*============================================================================*/
/*! CTleList

-# ���X�g�R���g���[���̍��ڃN���b�N���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CTleList::OnHdnItemclickTlelistList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (phdr == NULL)
		return;

	m_nSelectedColumn = phdr->iItem;

	for (int i = 0; i < m_listTle.GetItemCount(); i++) {
		m_listTle.SetItemData(i, i);
	}
	m_listTle.SortItems(&CTleList::CompareFunc, (LPARAM)this);

	m_bUpperSort = !m_bUpperSort;

	*pResult = 0;
}


/*============================================================================*/
/*! CTleList

-# �`�d���s�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CTleList

-# �폜�{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton2()
{
	// TLE�ꗗ����A�C�e�����폜����
	DeleteListItem();
}


/*============================================================================*/
/*! CTleList

-# ��荞�݃{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton3()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CTleList

-# �ڍו\���{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton4()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CTleList

-# �ڍ׈���{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton5()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CTleList

-# �ꗗ����{�^���������ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CTleList::OnBnClickedTlelistButton6()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


/*============================================================================*/
/*! CTleList

-# �R���{�{�b�N�X�I��ύX���ɌĂ΂��B

@param  �Ȃ�
@retval �Ȃ�

*/
/*============================================================================*/
void CTleList::OnCbnSelchangeTlelistCombo()
{
	// �ĕ`��
	UpdateListItem();
}

