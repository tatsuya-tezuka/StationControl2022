#pragma once

#include "FrequencyCtrlPre.h"

// CFrequencyCtrlPre54 �_�C�A���O

class CFrequencyCtrlPre54 : public CFrequencyCtrlPre
{
	DECLARE_DYNAMIC(CFrequencyCtrlPre54)

public:
	CFrequencyCtrlPre54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlPre54();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_PRESET54 };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
