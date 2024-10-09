#pragma once
#include "afxwin.h"
#include "atltime.h"
#include "EquipmentRecSelectEquipment.h"
#include "afxdtctl.h"


// CEquipmentRecDumpOutput �_�C�A���O

class CEquipmentRecDumpOutput : public CDialogBase
{
	DECLARE_DYNAMIC(CEquipmentRecDumpOutput)

public:
	CEquipmentRecDumpOutput(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEquipmentRecDumpOutput();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQIPREC_DUMPOUTPUT };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CString> m_EquipNameListTemp;

protected:
	map<CString, vector<CString>> m_EquipMonNameList;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	virtual BOOL UpdateDlg();

	map<CString, vector<CString>>& GetEquipMonNameList() { return m_EquipMonNameList;}

protected:
	void	ResetList(CString name);
	void	ResetMonMapList();
	void	SetDlgData(CString mon);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// �ݔ��I��
	CComboBoxEx2 m_EquipmentSelCmb;
	// �t�@�C������
	CString m_strFileId;
	// �o�͊J�n����-���t
	CTime m_StartTimeD;
	// �o�͊J�n����-����
	CTime m_StartTimeT;
	// �o�͏I������-���t
	CTime m_EndTimeD;
	// �o�͊J�n����-����
	CTime m_EndTimeT;
	// �����p�x
	CEdit m_SearchTimeEdit;
	// �X�e�[�^�X�o�͕��@
	CComboBox m_StatusOutputCmb;
	// �Ď��f�[�^���X�g
	CListBox m_MonitoringDataList;
	// ���ړ���
	CString m_strDirectEntry;
	afx_msg void OnBnClickedEqiprecDumpoutputFidSaveBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputFidReadBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputMntdUpBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputMntdDwnBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputMntdDelBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputDirectAddBtn();
	afx_msg void OnBnClickedEqiprecDumpoutputExecBtn();
	afx_msg void OnMenuCloseClose();
	CDateTimeCtrl m_StartTimeDTCtrl;
	CDateTimeCtrl m_EndTimeDTCtrl;
	afx_msg void OnBnClickedEqiprecDumpoutputDirectSerchBtn();
	afx_msg void OnCbnSelchangeEqiprecDumpoutputEqipselCmb();
	afx_msg void OnEnChangeEqiprecDumpoutputFidEdit();
	CButton m_SaveBtnCtrl;
	CDateTimeCtrl m_StartTimeTTCtrl;
	CDateTimeCtrl m_EndTimeTTCtrl;
};
