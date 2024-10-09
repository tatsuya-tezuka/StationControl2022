#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "FrequencyCtrlSmTf.h"
#include "FrequencyCtrlSmRf.h"
#include "FrequencyCtrlSm3W.h"
#include "FrequencyCtrlSmcf.h"
#include "atltime.h"
#include "afxdtctl.h"


// CFrequencyCtrlSm �_�C�A���O

class CFrequencyCtrlSm : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlSm)

public:
	CFrequencyCtrlSm(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CFrequencyCtrlSm();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_SML };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct{
		CString		satname;		// �q����
		CString		predname;		// �\��l��
		CString		predfilepath;	// �\��l�t�@�C���p�X��
		time_t		passStart;		// �p�X�J�n����
		time_t		passStop;		// �p�X�I������
	} stSimData;
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	CWnd*		mCallWnd;
	CString		mTargetPredFilePath;
private:
	CFrequencyCtrlSmTf	m_TabTf;
	CFrequencyCtrlSmRf	m_TabFf;	// (���g�p�j
	CFrequencyCtrlSm3W	m_Tab3W;
	CFrequencyCtrlSmCf	m_TabCf;	// (���g�p�j


	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void	Initialize();
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	bool	GetSimData(stSimData& data);
	virtual BOOL UpdateDlg();

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �T���@
	CComboBox m_SpacecraftCtrl;
	afx_msg void OnBnClickedFreqCtrlSmlAntfcBtn();
	CTabCtrl m_SmlTabCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeDlgFreqCtrlSmlTab(NMHDR *pNMHDR, LRESULT *pResult);
	// �p�X�J�n�����iDay�j
	CTime m_PassStartDay;
	// �p�X�J�n�����iTime�j
	CTime m_PassStartTime;
	// �p�X�I�������iDay�j
	CTime m_PassEndDay;
	// �p�X�I�������iTime�j
	CTime m_PassEndTime;
	CDateTimeCtrl m_PassStartDTCtrl;
	CDateTimeCtrl m_PassEndDTCtrl;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CButton m_AntPredBtnCtrl;
	afx_msg void OnCbnSelchangeFreqCtrlSmlSpcrCmb();
};
