#pragma once
#include "afxdtctl.h"
#include "afxmaskededit.h"
#include "afxwin.h"


// CEquipmentCtrlTime �_�C�A���O

class CEquipmentCtrlTime : public CDialogEx
{
	DECLARE_DYNAMIC(CEquipmentCtrlTime)

public:
	CEquipmentCtrlTime(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEquipmentCtrlTime();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQUIPCTRL_TIME };

	enum eTimeType {
		eTimeType_MSS = 0,
		eTimeType_MMSS,
		eTimeType_HHMMSS,
		eTimeType_DDDHHMMSS,
		eTimeType_YYYYmmdd
	};

	static const int m_timeMMax = 10;				// �ő�l(���j
	static const int m_timeMMMax = 60;				// �ő�l(���j
	static const int m_timeSSMax = 60;				// �ő�l(�b�j
	static const int m_timeHHMax = 24;				// �ő�l(���j
	static const int m_timeDDDMax = 365;			// �ő�l(�ώZ���j
	static const int m_timeddMax = 31;				// �ő�l(���j
	static const int m_timemmMax = 12;				// �ő�l(���j

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString			mTimeString;
	CString			m_strWindowTitle;
	time_t			m_time;
	CMFCMaskedEdit	m_CMaskedEdit;
	UINT			m_nTimeType;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

public:
	void SetTimeType(UINT type)		{ m_nTimeType = type; }
	void SetTime(time_t time)		{ m_time = time; }
	time_t GetTime()				{ return m_time; }
	void SetTimeString(CString& str)	{ mTimeString = str; }
	CString GetTimeString()			{ return mTimeString.Trim(_T(" ")); }

protected:
	void UpdateOffsetTime();
	BOOL GetOffsetTimeFromEditCtrl(CString& str, BOOL bAutoCorrect = FALSE);

	DECLARE_MESSAGE_MAP()
public:
	void SetWindowTitle(CString str)	{ m_strWindowTitle = str; }
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	CString m_strTimeOffset;
	CString m_strControlName;
	/* ------------------------------------------------------------------------------------ */
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnEnKillfocusMfcmaskededitCtrlregist();
};
