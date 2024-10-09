/*============================================================================*/
/*! ComboBoxEx2.cpp

-# �R���{�{�b�N�X�g���N���X
-#
-# �w�肵����������R���{�{�b�N�X���猟�����A���S��v�������ڂ̑I���@�\����������B
-# CComboBox::SelectString�̓R���{�{�b�N�X�̍��ڂ̐擪����̕�����v�̂��߁A
-# ���ڂ̊��S��v�����邽�߂ɂ́A�{�N���X�Ŏ������Ă���SelectStringEx���g�p���邱�ƁB
*/
/*============================================================================*/
// ComboBoxEx2.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ComboBoxEx2.h"


// CComboBoxEx2

IMPLEMENT_DYNAMIC(CComboBoxEx2, CComboBox)

CComboBoxEx2::CComboBoxEx2()
{

}

CComboBoxEx2::~CComboBoxEx2()
{
}


BEGIN_MESSAGE_MAP(CComboBoxEx2, CComboBox)
END_MESSAGE_MAP()



// CComboBoxEx2 ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! ComboBoxEx2

-# �w�肵����������R���{�{�b�N�X���猟�����A���S��v�������ڂ�I������B

@param  nStartAfter	�����J�n�ʒu
@param	lpszString	����������
@retval ��v���ڂ�0����n�܂�C���f�b�N�X
*/
/*============================================================================*/
int CComboBoxEx2::SelectStringEx(int nStartAfter, LPCTSTR lpszString)
{
	int idx = FindStringExact(nStartAfter, lpszString);
	SetCurSel(idx);

	return idx;
}
