#pragma once

#include "SatelliteCapture.h"
#include "AntStanbyOriented.h"
#include "DownlinkCapture.h"
#include "UplinkCtrl.h"

// CSatelliteCapture54 �_�C�A���O

class CSatelliteCapture54 : public CSatelliteCapture
{
	DECLARE_DYNAMIC(CSatelliteCapture54)

public:
	CSatelliteCapture54(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSatelliteCapture54();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SATELLITECAPTURE54 };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CAntStanbyOriented	m_antStanbyOriented;			// ANT�҂��󂯎w��
	CDownlinkCapture	m_downlinkCapture;				// �_�E�������N�ߑ�
	CUplinkCtrl			m_uplinkCtrl;					// �A�b�v�����N����

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnTcnSelchangeSatellitecaptureTab1(NMHDR *pNMHDR, LRESULT *pResult);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/* ------------------------------------------------------------------------------------ */
};
