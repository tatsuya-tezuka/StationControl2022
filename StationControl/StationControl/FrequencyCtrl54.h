#pragma once

#include "FrequencyCtrl.h"
#include "FrequencyCtrlMon54.h"
#include "FrequencyCtrlCtrl54.h"
#include "FrequencyCtrlPre54.h"

// CFrequencyCtrl54 �_�C�A���O

class CFrequencyCtrl54 : public CFrequencyCtrl
{
	DECLARE_DYNAMIC(CFrequencyCtrl54)

public:
	CFrequencyCtrl54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrl54();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL54 };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
private:
	CFrequencyCtrlMon54 		m_TabMon;
	CFrequencyCtrlCtrl54		m_TabCtrl;
	CFrequencyCtrlPre			m_TabPre;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeFreqCtrlTab(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	/* ------------------------------------------------------------------------------------ */
};
