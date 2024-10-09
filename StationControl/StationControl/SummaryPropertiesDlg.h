#pragma once

#include "PropertiesCommonDlg.h"

// CSummaryPropertiesDlg �_�C�A���O

class CSummaryPropertiesDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSummaryPropertiesDlg)

public:
	CSummaryPropertiesDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSummaryPropertiesDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_SUMMARYPROPERTIES };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
//	UINT			m_nStationID;				// ��ID
//	UINT			m_nViewID;					// �A�N�e�B�u�r���[ID
//	UINT			m_nParentID;				// �e�E�B���h�EID
//	vector<CString>	m_listViewName;				// ���X�g�r���[��
//	CComboBox		m_wndObjectCombo;			// �O���t�̃R���{�{�b�N�X
//	CButton			m_btnSetting;				// �ݒ�{�^��

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static BOOL GetViewNameList(vector<CString>& listViewName);
	static BOOL GetViewTrendNameList(vector<CString>& listViewName);
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nViewID, UINT nStaionID, UINT nParentID = 0);
//	UINT GetStationID()		{ return m_nStationID; }
//	UINT GetParentID()		{ return m_nParentID; }
//	int GeCurComboSel()		{ return m_wndObjectCombo.GetCurSel(); }

protected:
//	CPropertiesData&  GetPropertiesData(int pos);
//	void SetPropertiesData(int pos, CPropertiesData& val);
//	void UpdateComboBoxSelect();
//	void SendPropertiesChange();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnComboBoxSelChange();
//	afx_msg void OnBnClickedSummarygrapthPropButton();
	virtual BOOL OnInitDialog();
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
