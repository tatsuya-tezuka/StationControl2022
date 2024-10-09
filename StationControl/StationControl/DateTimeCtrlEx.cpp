/*============================================================================*/
/*! DateTimeCtrlEx.cpp

-# CDateTimeCtrl�̊g���N���X
-# �t�H�[�J�X���������ɁA�e�E�B���h�E�ɒʒm����
*/
/*============================================================================*/
// DateTimeCtrlEx.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "DateTimeCtrlEx.h"


// CDateTimeCtrlEx

IMPLEMENT_DYNAMIC(CDateTimeCtrlEx, CDateTimeCtrl)

/*============================================================================*/
/*! CDateTimeCtrlEx

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CDateTimeCtrlEx::CDateTimeCtrlEx()
: m_pPostToWnd(NULL)
{

}

/*============================================================================*/
/*! CDateTimeCtrlEx

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CDateTimeCtrlEx::~CDateTimeCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CDateTimeCtrlEx, CDateTimeCtrl)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CDateTimeCtrlEx ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CDateTimeCtrlEx

-# �t���[�����[�N�����̓t�H�[�J�X���������O�ɌĂ΂��

@param  pNewWnd�F���̓t�H�[�J�X���󂯎��E�B���h�E�ւ̃|�C���^�[
@retval �Ȃ�
*/
/*============================================================================*/
void CDateTimeCtrlEx::OnKillFocus(CWnd* pNewWnd)
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);

	// m_pPostToWnd�ɒʒm��̃E�B���h�E�|�C���^���Z�b�g����Ă���ꍇ�́Am_pPostToWnd�ɒʒm����
	if (m_pPostToWnd != NULL)
	{
		// �t�H�[�J�X�������ʒm������
		m_pPostToWnd->PostMessage(WM_KILLFOCUS, (WPARAM)(this->GetSafeHwnd()), NULL);
	}
	// ����ȊO�́A�e�E�B���h�E�ɒʒm����
	else
	{
		CWnd* pWnd = GetParent();
		if (pWnd != NULL)
		{
			// �t�H�[�J�X�������ʒm������
			pWnd->PostMessage(WM_KILLFOCUS, (WPARAM)(this->GetSafeHwnd()), NULL);
		}
	}
}
