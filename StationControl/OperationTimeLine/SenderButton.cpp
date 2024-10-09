// SenderButton.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "OperationTimeLine.h"
#include "SenderButton.h"

HWND CSenderButton::m_hWnd = NULL;

// CSenderButton

IMPLEMENT_DYNAMIC(CSenderButton, CMFCMenuButton)

CSenderButton::CSenderButton()
{

}

CSenderButton::~CSenderButton()
{
	mPopMenu.DestroyMenu();
	mMainMenu.DestroyMenu();
}


BEGIN_MESSAGE_MAP(CSenderButton, CMFCMenuButton)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(BN_CLICKED, &CSenderButton::OnBnClicked)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CSenderButton ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! ���M�����u�I���R���g���[��

-# ���M�����u�I���R���g�g�[���̍쐬

@param

@retval

*/
/*============================================================================*/
void CSenderButton::Create()
{
	CMFCButton::EnableWindowsTheming(TRUE);
//	m_bOSMenu = FALSE;
	m_bOSMenu = TRUE;
	m_bStayPressed = FALSE;

	mMainMenu.CreateMenu();
	mPopMenu.CreatePopupMenu();
	//SizeToContent();
	int i;
	for (i = 0; i < sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]); ++i) {
		if ((theApp.GetSelectStation() == eStation_Uchinoura34 || theApp.GetSelectStation() == eStation_Uchinoura20) && i >= eSenderItem3){
			// 34m , 20m �͋ǉ^�p�ǐ����u�Ɛݔ����䑕�u�̂�
			continue;
		}
		mData[i].id = mSenderMessage + i;
		mData[i].name = mHistEquipmentString[i];
		mData[i].chack = true;
		mData[i].cancel = false;
		mPopMenu.AppendMenu(MF_STRING, mSenderMessage + i, mHistEquipmentString[i]);
		mPopMenu.CheckMenuItem(i, MF_BYPOSITION | MF_CHECKED);
	}
	mData[i].id = mSenderMessage + i;
	mData[i].name = _T("�L�����Z��");
	mData[i].chack = false;
	mData[i].cancel = true;
	mPopMenu.AppendMenu(MF_STRING, mSenderMessage + i, _T("�L�����Z��"));

	mMainMenu.AppendMenu(MF_POPUP, (UINT_PTR)mPopMenu.m_hMenu, _T("dummy"));
	m_hMenu = mMainMenu.GetSubMenu(0)->GetSafeHmenu();

	SetTitle();
}

/*============================================================================*/
/*! ���M�����u�I���R���g���[��

-# ���M�����u�I���C�x���g

@param

@retval

*/
/*============================================================================*/
void CSenderButton::OnBnClicked()
{
	if (m_hWnd != this->GetSafeHwnd())
	{
		// �Ⴄ�{�^���̏ꍇ�͉������Ȃ�
		return;
	}

	for (int i = 0; i < eSenderItemMax+1; ++i) {
		if (mData[i].id == m_nMenuResult){
			if (mData[i].cancel == true)
				return;
			mData[i].chack = (mData[i].chack) ? false : true;
			UINT cflag = (mData[i].chack) ? MF_CHECKED : MF_UNCHECKED;
			mPopMenu.CheckMenuItem(i, MF_BYPOSITION | cflag);
			break;
		}
	}
	SetTitle();
}

/*============================================================================*/
/*! ���M�����u�I���R���g���[��

-# ���M�����u�I����Ԃɂ���ă{�^���^�C�g����ݒ肷��

@param

@retval

*/
/*============================================================================*/
void CSenderButton::SetTitle()
{
	CString title = _T("");
	CString simpletitle = _T("");
	UINT count = 0;
	for (int i = 0; i < eSenderItemMax; ++i) {
		if (mData[i].chack == true){
			if (simpletitle.IsEmpty() == false){
				simpletitle += _T("+");
			}
			simpletitle += mHistEquipmentStringSimple[i];
			title = mHistEquipmentString[i];
			count++;
		}
	}
	if (theApp.GetSelectStation() == eStation_Usuda64 ||
		theApp.GetSelectStation() == eStation_Usuda642){
		// 64m , 54m
		if (count == 0 || count == eSenderItemMax)
			SetWindowText(_T("���ׂ�"));
		else if (count == 1){
			SetWindowText(title);
		}
		else{
			SetWindowText(simpletitle);
		}
	}
	else{
		// 34m , 20m
		if (count == 0 || count == 2)
			SetWindowText(_T("���ׂ�"));
		else if (count == 1){
			SetWindowText(title);
		}
		else{
			SetWindowText(simpletitle);
		}
	}
}

/*============================================================================*/
/*! ���M�����u�I���R���g���[��

-# ���M�����u�I����Ԃ̎擾

@param

@retval

*/
/*============================================================================*/
UINT CSenderButton::GetStatus()
{
	UINT status = 0;
	for (int i = 0; i < eSenderItemMax; ++i) {
		if (mData[i].chack == true){
			status |= 1 << i;
		}
	}
	if (status == 0)
		status = 0x0f;
	return status;
}

/*============================================================================*/
/*! ���M�����u�I���R���g���[��

-# ���M�����u�I����Ԃ̐ݒ�

@param

@retval

*/
/*============================================================================*/
void CSenderButton::SetStatus(UINT val)
{
	if (theApp.GetSelectStation() == eStation_Usuda64 ||
		theApp.GetSelectStation() == eStation_Usuda642){
		for (int i = 0; i < eSenderItemMax; ++i) {
			if (val == 0 || (val&(1 << i))){
				// �`�F�b�N���
				mPopMenu.CheckMenuItem(i, MF_BYPOSITION | MF_CHECKED);
				mData[i].chack = true;
			}
			else{
				mPopMenu.CheckMenuItem(i, MF_BYPOSITION | MF_UNCHECKED);
				mData[i].chack = false;
			}
		}
	}
	else{
		for (int i = 0; i < 2; ++i) {
			if (val == 0 || (val&(1 << i))){
				// �`�F�b�N���
				mPopMenu.CheckMenuItem(i, MF_BYPOSITION | MF_CHECKED);
				mData[i].chack = true;
			}
			else{
				mPopMenu.CheckMenuItem(i, MF_BYPOSITION | MF_UNCHECKED);
				mData[i].chack = false;
			}
		}
	}
	SetTitle();
}






// CColorCheckButton

IMPLEMENT_DYNAMIC(CColorCheckButton, CButton)

CColorCheckButton::CColorCheckButton()
{

}

CColorCheckButton::~CColorCheckButton()
{
}


BEGIN_MESSAGE_MAP(CColorCheckButton, CButton)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CColorCheckButton::OnNMCustomdraw)
END_MESSAGE_MAP()



// CColorCheckButton ���b�Z�[�W �n���h���[




void CColorCheckButton::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	long nState = pNMCD->uItemState;
	CDC *pDC = CDC::FromHandle(pNMCD->hdc);
	CRect rcItem = pNMCD->rc;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	//UINT nFormat = DT_CENTER | DT_WORDBREAK;

	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREERASE:
		*pResult = CDRF_NOTIFYPOSTERASE;
		break;

	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYPOSTPAINT;
		break;

	case CDDS_POSTPAINT:
	{
		COLORREF color;
		int nCheck = GetCheck();
		color = mStationColor[m_nStation];

		CString str;
		GetWindowText(str);
		CRect rect = rcItem;
		pDC->SetBkMode(TRANSPARENT);
		rect.left += 16;
		pDC->FillSolidRect(rect, color);
		pDC->DrawText(str, rect, nFormat);

		*pResult = CDRF_DODEFAULT;
	}
	break;
	default:
		break;
	}
}


void CSenderButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_hWnd = this->GetSafeHwnd();

	CMFCMenuButton::OnLButtonDown(nFlags, point);
}

