// EngValConvDLL.h : EngValConvDLL.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CEngValConvDLLApp
// ���̃N���X�̎����Ɋւ��Ă� EngValConvDLL.cpp ���Q�Ƃ��Ă��������B
//

class CEngValConvDLLApp : public CWinApp
{
public:
	CEngValConvDLLApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
