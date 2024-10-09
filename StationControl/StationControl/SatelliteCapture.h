#pragma once

#include "AntStanbyOriented.h"
#include "DownlinkCapture.h"
#include "UplinkCtrl.h"

// CSatelliteCapture �_�C�A���O

class CSatelliteCapture : public CDialogBase
{
	DECLARE_DYNAMIC(CSatelliteCapture)

public:
	CSatelliteCapture(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CSatelliteCapture(UINT IDD, CWnd* pParent = NULL);
	virtual ~CSatelliteCapture();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SATELLITECAPTURE };

	// ��ʐݒ�
	const int nTabLeftOffset = 15;		// �^�u�E�B���h�E��Left�I�t�Z�b�g
	const int nTabTopOffset = 35;		// �^�u�E�B���h�E��Top�I�t�Z�b�g

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CTabCtrl			m_tabCtrl;						// �^�u�R���g���[��
//	CAntStanbyOriented	m_antStanbyOriented;			// ANT�҂��󂯎w��
	CDownlinkCapture	m_downlinkCapture;				// �_�E�������N�ߑ�
	CUplinkCtrl			m_uplinkCtrl;					// �A�b�v�����N����

	UINT				m_CmdMode;						// CMD����ݒ�
	UINT				m_RngMode;						// RNG����ݒ�

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	void Update();
	void set_cur_cap();
	UINT GetRcvFreqMode()				{ return m_downlinkCapture.GetRcvFreqMode(); }
	UINT GetCtrlMode()					{ return m_downlinkCapture.GetCtrlMode(); }
	UINT GetRfTripMonitor()				{ return m_uplinkCtrl.GetRfTripMonitor(); }
	unsigned char GetAutoFreqData()		{ return m_downlinkCapture.GetAutoFreqData(); }
	unsigned char GetManualFreqData()	{ return m_downlinkCapture.GetManualFreqData(); }
	unsigned char GetWayBinary()		{ return m_downlinkCapture.GetWayBinary(); }
	unsigned char GetUpLinkBinary()		{ return m_uplinkCtrl.GetUpLinkBinary(); }
	unsigned char GetDownLinkBinary()	{ return m_downlinkCapture.GetDownLinkBinary(); }
	unsigned char GetCmdBinary();
	unsigned char GetRngBinary();

	BOOL UpdateKanshiData();
	void SetRngMode(char szCtrlMode[]);
	void SetCmdMode(char szCtrlMode[]);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnTcnSelchangeSatellitecaptureTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/* ------------------------------------------------------------------------------------ */
};
