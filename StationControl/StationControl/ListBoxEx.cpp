/*============================================================================*/
/*! ListBoxEx.cpp

-# CListBox�̊g���N���X�i�X�N���[���o�[�N���b�N��e�E�B���h�E�ɒʒm�j
*/
/*============================================================================*/
// ListBoxEx.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ListBoxEx.h"


// CListBoxEx

IMPLEMENT_DYNAMIC(CListBoxEx, CListBox)

/*============================================================================*/
/*! CListBoxEx

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CListBoxEx::CListBoxEx()
{

}

/*============================================================================*/
/*! CListBoxEx

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CListBoxEx ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CListBoxEx

-# �X�N���[���o�[�N���b�N���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CListBoxEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);

	CWnd* pWnd = GetParent();
	if (pWnd != NULL && pWnd->GetSafeHwnd())
	{
		pWnd->PostMessage(eCListBoxMsg_VScroll, (WPARAM)this, 0);
	}
}
