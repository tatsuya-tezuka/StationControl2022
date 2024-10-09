/*============================================================================*/
/*! OperationAccount.cpp

-# �^�p�A�J�E���g�ݒ�
*/
/*============================================================================*/
// OperationAccount.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "OperationAccount.h"
#include "afxdialogex.h"


// COperationAccount �_�C�A���O

IMPLEMENT_DYNAMIC(COperationAccount, CDialogEx)

COperationAccount::COperationAccount(CWnd* pParent /*=NULL*/)
	: CDialogEx(COperationAccount::IDD, pParent)
{

}

COperationAccount::~COperationAccount()
{
}

void COperationAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mList);
}


BEGIN_MESSAGE_MAP(COperationAccount, CDialogEx)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, &COperationAccount::OnHdnDividerdblclickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &COperationAccount::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COperationAccount::OnBnClickedButton2)
END_MESSAGE_MAP()


// COperationAccount ���b�Z�[�W �n���h���[


BOOL COperationAccount::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD  dwStyle = ListView_GetExtendedListViewStyle(mList.m_hWnd);
	//  �g���X�^�C���̃Z�b�g
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= /*LVS_EX_CHECKBOXES | */LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 0x00010000/*LVS_EX_DOUBLEBUFFER*/;
	//  �V�����g���X�^�C����ݒ�
	ListView_SetExtendedListViewStyle(mList.m_hWnd, dwStyle);

	//=========================================================================
	// �R���g���[���Ŏg�p����C���[�W���X�g�̍쐬
	CDC* pdc = GetDC();
	HIMAGELIST hImageList = CreateImageList(pdc);
	m_ImageList.Attach(hImageList);
	mList.SetImageList(&m_ImageList, TVSIL_STATE);
	ReleaseDC(pdc);

	const CString mHeader[] = {
		_T("���[�U��"), _T("���V�Y20m��"), _T("���V�Y34m��"), _T("�P�c64m��"), _T("�P�c54m��"),
	};
	for (int i = 0; i<sizeof(mHeader) / sizeof(mHeader[0]); i++){
		mList.InsertColumn(i, mHeader[i], LVCFMT_LEFT, 100);
	}

	vector<stUserInfo> mUserList;
	theApp.GetLocalUserList(mUserList);
	vector<stAccount> mAcList;

	bool	bUpdateFile = false;

	CFileAccess& ac = theApp.GetFileAccess();
	if (ac.ReadOperationAccount(mAcList) == false){
		// �^�p�A�J�E���g�t�@�C�������݂��Ȃ��̂ŁA���[�J�����[�U�ŏ���������
		mAccountList.clear();
		vector<stUserInfo>::iterator itr;
		for (itr = mUserList.begin(); itr != mUserList.end(); itr++){
			stAccount data;
			data.user = (*itr).user;
			data.usersid = (*itr).usersid;
			data.usc20 = false;
			data.usc34 = false;
			data.udsc64 = false;
			data.xdsc54 = false;
			data.enable = true;
			data.mycomputer = true;
			data.dummy = 0;
			mAccountList.push_back(data);
		}
	}
	else{
		// ���łɉ^�p�A�J�E���g�����݂����̂ŁA���[�J�����[�U�Ƃ̐������`�F�b�N���s��
		vector<stAccount>::iterator itrdata;
		vector<stUserInfo>::iterator itr;
		mAccountList.clear();
		// �t�@�C�����̃f�[�^��o�^����
		for (itrdata = mAcList.begin(); itrdata != mAcList.end(); itrdata++){
			for (itr = mUserList.begin(); itr != mUserList.end(); itr++){
				if ((*itr).usersid.Mid(0, (*itr).usersid.GetLength() - 5) != (*itrdata).usersid.Mid(0, (*itr).usersid.GetLength() - 5)){
					break;
				}
				if ((*itr).usersid == (*itrdata).usersid)
					break;
			}
			if (itr == mUserList.end()){
				// �t�@�C�����̃��[�U��Windows��ɑ��݂��Ȃ��̂�
				// �o�^���Ȃ�
				bUpdateFile = true;
				continue;
			}
			stAccount data;
			data.user = (*itrdata).user;
			data.usersid = (*itrdata).usersid;
			data.usc20 = (*itrdata).usc20;
			data.usc34 = (*itrdata).usc34;
			data.udsc64 = (*itrdata).udsc64;
			data.xdsc54 = (*itrdata).xdsc54;
			data.enable = (*itrdata).enable;
			data.mycomputer = false;
			data.dummy = (*itrdata).dummy;
			mAccountList.push_back(data);
		}

		for (itr = mUserList.begin(); itr != mUserList.end(); itr++){
			for (itrdata = mAccountList.begin(); itrdata != mAccountList.end(); itrdata++){
				if ((*itr).user == (*itrdata).user && (*itr).usersid == (*itrdata).usersid){
					// ���[�J�����[�U�����݂���
					(*itrdata).mycomputer = true;
					break;
				}
			}
			if (itrdata == mAccountList.end()){
				// �V�������[�U�����݂����̂ŏ��������Ēǉ�����
				stAccount data;
				data.user = (*itr).user;
				data.usersid = (*itr).usersid;
				data.usc20 = false;
				data.usc34 = false;
				data.udsc64 = false;
				data.xdsc54 = false;
				data.enable = true;
				data.mycomputer = true;
				data.dummy = 0;
				mAccountList.push_back(data);
			}
		}
	}

	if (bUpdateFile == true){
		// Windows���烆�[�U���폜���ꂽ�̂ŁA�A�J�E���g�t�@�C��������폜����
		CFileAccess& ac = theApp.GetFileAccess();
		ac.WriteOperationAccount(mAccountList);
	}

	vector<stAccount>::iterator itr;
	int pos = 0;
	int index;
	for (index=0,itr = mAccountList.begin(); itr != mAccountList.end(); index++,itr++){
		//LPARAM lParam = ((*itr).usc20 << 24) | ((*itr).usc34 << 16) | ((*itr).udsc64 << 8) | ((*itr).xdsc54 << 0);
		if ((*itr).enable == false)
			continue;
		LPARAM lParam = (((*itr).mycomputer<<24) | (index<<16) | (*itr).usc20 << 3) | ((*itr).usc34 << 2) | ((*itr).udsc64 << 1) | ((*itr).xdsc54 << 0);
		AddItem(pos, 0, (*itr).user, (DWORD)lParam);
		AddItem(pos, 1, _T(""));
		AddItem(pos, 2, _T(""));
		AddItem(pos, 3, _T(""));
		AddItem(pos, 4, _T(""));
		pos++;
	}

	AutoSizeColumns();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

#define	MINCOLWIDTH		80
template <class T> inline const T& MAX(const T& t1, const T& t2) {
	return t1 < t2 ? t2 : t1;
}
/*============================================================================*/
/*! �g�����X�g�R���g���[��

-# ���X�g�r���[�̃J�����������ݒ�

@param	col	�����������s���J�����ԍ��i-1�̏ꍇ�͑S�ẴJ������ΏۂɎ����������s���j

@retval void
*/
/*============================================================================*/
void COperationAccount::AutoSizeColumns(int col /*=-1*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)mList.GetDlgItem(0);

	mList.SetRedraw(false);
	int dummycol = pHeader->GetItemCount();
	mList.InsertColumn(dummycol, _T("dummy"), LVCFMT_LEFT, 10);

	HDITEM hdi = { 0 };
	hdi.mask = HDI_FORMAT | HDI_IMAGE;
	pHeader->GetItem(0, &hdi);

	int nCol = col;
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? pHeader->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++){
		mList.SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = mList.GetColumnWidth(col);
		mList.SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = mList.GetColumnWidth(col);
		int wc = MAX(MINCOLWIDTH, MAX(wc1, wc2));
		mList.SetColumnWidth(col, wc);
	}
	mList.DeleteColumn(dummycol);
	mList.SetRedraw(true);
}
/*============================================================================*/
/*! �g�����X�g�R���g���[��

-# ���X�g�r���[�ւ̍��ڐݒ�

@param	nItem		�A�C�e���ԍ�
@param	nSubItem	�T�u�A�C�e���ԍ�
@param	strItem		�A�C�e��������
@param	dwParam		���[�U���

@retval int
*/
/*============================================================================*/
int COperationAccount::AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam/*=0*/)
{
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.mask = LVIF_TEXT;
	lv.iItem = nItem;
	lv.iSubItem = nSubItem;
	if (nSubItem == 0){
		lv.mask |= LVIF_PARAM;
		lv.lParam = dwParam;
	}
	lv.pszText = (LPTSTR)(LPCTSTR)strItem;
	lv.cchTextMax = strItem.GetLength();

	if (nSubItem == 0)
		return mList.InsertItem(&lv);
	return mList.SetItem(&lv);
}


void COperationAccount::OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	AutoSizeColumns();
	*pResult = 0;
}

/*============================================================================*/
/*! �ݒ胊�X�g

-# �c���[�R���g���[���Ŏg�p����C���[�W���X�g�̍쐬

@param		pdc			�f�o�C�X�R���e�L�X�g

@retval	HIMAGELIST	�C���[�W���X�g
*/
/*============================================================================*/
HIMAGELIST COperationAccount::CreateImageList(CDC* pdc)
{
	typedef struct {
		UINT	nType;
		UINT	nStateId;
		UINT	nState;
	} stImageInfo;
	// �c���[�R���g���[���ł̓`�F�b�N�{�b�N�X�����g�p���Ȃ��̂Ń��W�I�{�^���̍쐬�͍s��Ȃ�
	static	const stImageInfo	mImageInfo[] = {
		{ 0, 0, 0 },
		{ BP_CHECKBOX, CBS_UNCHECKEDNORMAL, DFCS_BUTTONCHECK | DFCS_FLAT },
		{ BP_CHECKBOX, CBS_CHECKEDNORMAL, DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT },
		{ BP_CHECKBOX, CBS_MIXEDNORMAL, DFCS_BUTTON3STATE | DFCS_CHECKED | DFCS_FLAT },
	};

	int mBmpWidth, mBmpHeight;
	mBmpWidth = 16;
	mBmpHeight = 16;

	HIMAGELIST hImageList = 0;
	const int nBmpWidth = mBmpWidth;
	const int nBmpHeight = mBmpHeight;
	const int nImages = sizeof(mImageInfo) / sizeof(mImageInfo[0]);
	CDC memDC;
	if (memDC.CreateCompatibleDC(pdc) == FALSE)
		return NULL;
	CBitmap bmp;
	if (bmp.CreateCompatibleBitmap(pdc, nBmpWidth * nImages, nBmpHeight) == FALSE){
		memDC.DeleteDC();
		return NULL;
	}
	hImageList = ImageList_Create(nBmpWidth, nBmpHeight, ILC_COLOR32, nImages, 1);
	if (hImageList == NULL){
		bmp.DeleteObject();
		memDC.DeleteDC();
		return NULL;
	}

	RECT rectImage1 = { 0, 0, mBmpWidth - 1, mBmpHeight - 1 };

	HTHEME hTheme = OpenThemeData(NULL, L"BUTTON");
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);
	memDC.FillSolidRect(0, 0, nBmpWidth*nImages, nBmpHeight, GetSysColor(COLOR_WINDOW));

	int nImageWidth = nBmpWidth - 2;
	int nImageHeight = nBmpHeight - 2;
	int nImageLeft = (nBmpWidth - nImageWidth) / 2;
	int nImageTop = (nBmpHeight - nImageHeight) / 2;

	RECT rectImage = { nImageLeft,
		nImageTop,
		nImageLeft + nImageWidth,
		nImageTop + nImageHeight };

	for (int i = 0; i<nImages; i++){
		memDC.FillSolidRect(0, 0, mBmpWidth, mBmpHeight, GetSysColor(COLOR_WINDOW));
		if (mImageInfo[i].nType == 0){
		}
		else{
			if (hTheme){
				rectImage1.left = rectImage.left - 1;
				rectImage1.right = rectImage1.left + nBmpWidth - 1;
				DrawThemeBackground(hTheme, memDC.m_hDC, mImageInfo[i].nType, mImageInfo[i].nStateId, &rectImage1, NULL);
				DrawThemeEdge(hTheme, memDC.m_hDC, mImageInfo[i].nType, mImageInfo[i].nStateId, &rectImage1, 0, BF_ADJUST | BF_RECT, NULL);
			}
			else{
				memDC.DrawFrameControl(&rectImage1, DFC_BUTTON, mImageInfo[i].nState);
			}
		}
		rectImage.left += nBmpWidth;
		rectImage.right += nBmpWidth;
	}
	if (hTheme){
		CloseThemeData(hTheme);
	}
	memDC.SelectObject(pOldBmp);

	// �C���[�W���X�g�ւ̒ǉ�
	ImageList_Add(hImageList, (HBITMAP)bmp.m_hObject, 0);

	bmp.DeleteObject();
	memDC.DeleteDC();
	return hImageList;
}


void COperationAccount::OnBnClickedButton1()
{
	vector<stAccount>::iterator itr;
	int pos = 0;
	for (itr = mAccountList.begin(); itr != mAccountList.end(); itr++){
		if ((*itr).enable == false)
			continue;
		LPARAM lParam = mList.GetItemData(pos);
		(*itr).usc20 = ((lParam >> 3) & 0x01) ? true : false;
		(*itr).usc34 = ((lParam >> 2) & 0x01) ? true : false;
		(*itr).udsc64 = ((lParam >> 1) & 0x01) ? true : false;
		(*itr).xdsc54 = ((lParam >> 0) & 0x01) ? true : false;
		pos++;
	}
	CFileAccess& ac = theApp.GetFileAccess();
	ac.WriteOperationAccount(mAccountList);

	vector<stAccount> mAcList;
	ac.ReadOperationAccount(mAcList);
}

void COperationAccount::OnBnClickedButton2()
{
	// �o�^�폜
	// �I������Ă��郆�[�U���폜����
	CString msg;
	UINT uSelectedCount = mList.GetSelectedCount();
	int  nItem = -1;
	nItem = mList.GetNextItem(nItem, LVNI_SELECTED);
	CString user = mList.GetItemText(nItem, 0);
	msg.Format(_T("�I�����ꂽ���[�U[%s]���^�p�A�J�E���g����폜���܂��B\n��낵���ł����B"), (LPCTSTR)user);
	if (MessageBox(msg, 0, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
		return;

	// �폜����
	LPARAM pos = mList.GetItemData(nItem);
	pos = (pos >> 16) & 0xff;
	mAccountList[pos].enable = false;
	mList.DeleteItem(nItem);
}
