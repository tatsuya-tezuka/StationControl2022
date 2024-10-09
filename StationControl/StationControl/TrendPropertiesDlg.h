#pragma once

#include "PropertiesCommonDlg.h"


// CTrendPropertiesDlg �_�C�A���O

class CTrendPropertiesDlg : public CPropertiesCommonDlg
{
	DECLARE_DYNAMIC(CTrendPropertiesDlg)

public:
	CTrendPropertiesDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CTrendPropertiesDlg(CWnd* pParent, UINT nViewID, vector<CString>& listViewName, UINT nPropertiesID, UINT nStaionID, UINT nParentID = 0, UINT nPropertiesType = ePropertiesType_Trend,
		CString strWndName = _T(""), CWnd* pSummaryProp = NULL, BOOL bChgDlg = FALSE);
	virtual ~CTrendPropertiesDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TRENDPROPERTIES };

	enum ePropertiesType
	{
		ePropertiesType_Trend = 0,		// �g�����h�E�v���p�e�B
		ePropertiesType_Summary,		// �T�}���E�v���p�e�B
	};

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT m_PropertiesType;				// �v���p�e�B�E�^�C�v
	UINT m_nStationID;					// ��ID
	CString m_strWndName;				// �E�B���h�E��
	BOOL m_bChgGraphPropDlg;			// �O���t�v���p�e�B�_�C�A���O�ύX
	CWnd* m_pSummaryProp;				// �T�}���v���p�e�B�E�B���h�E�|�C���^
	BOOL m_bChgDlg;						// �_�C�A���O��ޕύX
	CPoint m_ptWnd;						// �E�B���h�E�ʒu
	BOOL m_bModified;					// �ύX�t���O

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void AdjustLayout();
	virtual void ChangePropaties(BOOL bUpdate = TRUE);
	virtual void UpdatePropatyDataList();
	virtual void ChangeView(int pos);

protected:
	virtual void InitPropList();
	virtual CPropertiesData& GetPropertiesData(int pos);
	virtual void SetPropertiesData(int pos, CPropertiesData& val);
	virtual void SendPropertiesChange();

	void BackupPropertiesData();
	void RestorePropertiesData();

	void ShowProperties();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnComboBoxSelChange();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
