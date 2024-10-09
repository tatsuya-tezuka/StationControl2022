#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "atltime.h"


// CEquipmentCtrlBitRate �_�C�A���O

class CEquipmentCtrlBitRate : public CDialogEx
{
	DECLARE_DYNAMIC(CEquipmentCtrlBitRate)

public:
	CEquipmentCtrlBitRate(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEquipmentCtrlBitRate();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQUIPCTRL_BITRATE };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct {
		CString satellite;
		CString bitrate;
		BOOL	bTime;
		CTime	time;
	}stBitRate;

protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:

protected:
	BOOL	m_bCmdBitRate;
	CString m_strBitRateCtrl;
	stBitRate m_stBitRate;
	CString m_strWindowTitle;

	map<CString, vector<CString>> m_SatBitRateMap;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetCmdBitRateFlg(BOOL flg) { m_bCmdBitRate = flg; }
	void SetBitRateCtrl(CString str) { m_strBitRateCtrl = str; }
	stBitRate GetBitRateSetting() { return m_stBitRate; }
	void SetWindowTitle(CString str)	{ m_strWindowTitle = str; }

protected:
	void GetBitRateList(CString satname);
	void GetBitRateListSat();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �q����
	CComboBox m_SatCmb;
	CString m_strSatCmb;
	// �r�b�g���[�g���X�g
	CListBox m_BitRateListBox;
	// �����w��iDAY�j
	CDateTimeCtrl m_DataTimeDayCtrl;
	CTime m_DataTimeDay;
	// �����w��iTime�j
	CDateTimeCtrl m_DateTimeTimeCtrl;
	CTime m_DataTimeTime;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	// �����w��`�F�b�N�{�b�N�X
	CButton m_SpecifyDateChkBtn;
	afx_msg void OnCbnSelchangeDlgEquipctrlBitrateSatCmb();
	afx_msg void OnBnClickedOk();
};
