/*============================================================================*/
/*! ColorStringComboBox.cpp

-# �����F��ύX����R���{�{�b�N�X
*/
/*============================================================================*/
// ColorStringComboBox.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ColorStringComboBox.h"


// CColorStringComboBox

IMPLEMENT_DYNAMIC(CColorStringComboBox, CComboBox)

/*============================================================================*/
/*! ColorStringComboBox

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CColorStringComboBox::CColorStringComboBox()
: m_colText(RGB(0, 0, 0))
//: m_colSelectedText(RGB(0, 0, 0))
//, m_colOtherText(RGB(0, 0, 0))
{

}

/*============================================================================*/
/*! ColorStringComboBox

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CColorStringComboBox::~CColorStringComboBox()
{
}


BEGIN_MESSAGE_MAP(CColorStringComboBox, CComboBox)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CColorStringComboBox ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! ColorStringComboBox

-# �I�[�i�[�`�掞�ɌĂ΂��

@param  nIDCtl
@param  lpDrawItemStruct
@retval �Ȃ�
*/
/*============================================================================*/
void CColorStringComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC	cDC;
	CString	sText;

	if (!cDC.Attach(lpDrawItemStruct->hDC))
		return;

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		cDC.SetTextColor(m_colText);
		cDC.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		cDC.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		cDC.SetTextColor(m_colText);
		cDC.SetBkColor(GetSysColor(COLOR_3DFACE));
		cDC.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW));
	}

	if (lpDrawItemStruct->itemID != -1)
	{
		this->GetLBText(lpDrawItemStruct->itemID, sText);

		cDC.TextOut(3, lpDrawItemStruct->rcItem.top += 2, sText);
	}

	cDC.Detach();
}


BOOL CColorStringComboBox::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	return CComboBox::OnChildNotify(message, wParam, lParam, pLResult);
}
