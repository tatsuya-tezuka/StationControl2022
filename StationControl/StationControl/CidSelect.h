#pragma once
#include "afxwin.h"
#include "SatelliteData.h"


// CCidSelect �_�C�A���O

class CCidSelect : public CDialogBase
{
	DECLARE_DYNAMIC(CCidSelect)

public:
	CCidSelect(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CCidSelect();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_CIDSEL };

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
	CString		m_strSatelliteName;		// �q����
	CString		m_strCID;				// CID
	int			m_xtx;					// X�ё��M�ݔ��g�p�L��

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL UpdateDlg();

	void		SetSatelliteName(CString name) { m_strSatelliteName = name; };
	void		SetCID(CString cid) { m_strCID = cid; };
	stCIDParam	GetParameter();
	int			GetSendType(CString satname)
	{
		GetEqSatDB(satname);
		return m_xtx;
	}

protected:
	BOOL	InitDlg();
	BOOL	SetComboboxSatCIDParam(CString sat);
	BOOL	SetComboboxCIDParam(CString sat, CString cid);
	CString	CheckCid();
	void	GetEqSatDB(CString satname);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �q���I��
	CComboBoxEx2 m_SetalliteCmb;
	// CID�I��
	CComboBoxEx2 m_CidCmb;
	// �e�����g���r�b�g���[�g-S��
	CComboBoxEx2 m_TlmBrSCmb;
	// �e�����g���r�b�g���[�g-X��
	CComboBoxEx2 m_TlmBrXCmb;
	// �e�����g���r�b�g���[�g-X����
	CComboBoxEx2 m_TlmBrHXCmb;
	// �e�����g���r�b�g���[�g-Ka��
	CComboBoxEx2 m_TlmBrKaCmb;
	// �L�����A��M���[�v�o���h-S��
	CComboBoxEx2 m_CarrTLbSCmb;
	// �L�����A��M���[�v�o���h-X��
	CComboBoxEx2 m_CarrTLbXCmb;
	// �L�����A��M���[�v�o���h-Ka��
	CComboBoxEx2 m_CarrTLbKaCmb;
	// �����W�v���p�����[�^-���[�h
	CComboBoxEx2 m_RngPrmModeCmb;
	// �ϒ�����-S��
	CComboBoxEx2 m_MdlSCmb;
	// �ϒ�����-X��
	CComboBoxEx2 m_MdlXCmb;
	// �A���e�i�����ǔ�-�ڍs
	CComboBoxEx2 m_AntAutoCmb;
	// �R�}���h�r�b�g���[�g-S��
	CComboBoxEx2 m_CmdBrSCmb;
	// �R�}���h�r�b�g���[�g-X��
	CComboBoxEx2 m_CmdBrXCmb;
	// ���M�d��
	CComboBoxEx2 m_TPwrCmb;
	// �A�b�v�����N�|���p�����[�^-�`��
	CComboBoxEx2 m_UpLinkShapeCmb;
	// �R�}���h�ϒ�-�ϒ��x
	CComboBoxEx2 m_CmdMdlCmb;
	// �R�}���h�T�u�L�����A���g��-���g��
	CComboBoxEx2 m_CmdFreqCmb;
	// ���M�h�b�v��-�⏞
	CComboBoxEx2 m_TDoppCmb;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCidselSatelliteCmb();
	afx_msg void OnCbnSelchangeCidselCidCmb();
	afx_msg void OnCbnSelchangeCidselTlmbrSCmb();
	afx_msg void OnCbnSelchangeCidselTlmbrXCmb();
	afx_msg void OnCbnSelchangeCidselTlmbrHxCmb();
	afx_msg void OnCbnSelchangeCidselTlmbrKaCmb();
	afx_msg void OnCbnSelchangeCidselCmdbrSCmb();
	afx_msg void OnCbnSelchangeCidselCmdbrXCmb();
	afx_msg void OnCbnSelchangeCidselTpwrPwrCmb();
	afx_msg void OnCbnSelchangeCidselClbSCmb();
	afx_msg void OnCbnSelchangeCidselClbXCmb();
	afx_msg void OnCbnSelchangeCidselClbKaCmb();
	afx_msg void OnCbnSelchangeCidselUplnkShpCmb();
	afx_msg void OnCbnSelchangeCidselRprmSrCmb();
	afx_msg void OnCbnSelchangeCidselCmdmdlMdlCmb();
	afx_msg void OnCbnSelchangeCidselCmdsubfreqFreqCmb();
	afx_msg void OnCbnSelchangeCidselMmtdSCmb();
	afx_msg void OnCbnSelchangeCidselMmtdXCmb();
	afx_msg void OnCbnSelchangeCidselTdopCmpCmb();
	afx_msg void OnCbnSelchangeCidselAntatMigCmb();
	// �q���I��
	CString m_strSatelliteCmb;
	// CID�I��
	CString m_strCIDCmb;
	// �e�����g���r�b�g���[�g S��
	CString m_strTlmBRSCmb;
	// �e�����g���r�b�g���[�g X��
	CString m_strTlmBRXCmb;
	// �e�����g���r�b�g���[�g X����
	CString m_strTlmBRHXCmb;
	// �e�����g���r�b�g���[�g Ka��
	CString m_strTlmBRKaCmb;
	// �R�}���h�r�b�g���[�g S��
	CString m_strCmdBRSCmb;
	// �R�}���h�r�b�g���[�g X��
	CString m_strCmdBRXCmb;
	// ���M�d��
	CString m_strTPwrCmb;
	// �L�����A��M���[�v�o���h S��
	CString m_strCarrTLbSCmb;
	// �L�����A��M���[�v�o���h X��
	CString m_strCarrTLbXCmb;
	// �L�����A��M���[�v�o���h Ka��
	CString m_strCarrTLbKaCmb;
	// �A�b�v�����N�|���p�����[�^ �`��
	CString m_strUpLinkShapeCmb;
	// �����W�v���p�����[�^ ���[�h
	CString m_strRngPrmModeCmb;
	// �R�}���h�ϒ��x
	CString m_strCmdMdlCmb;
	// �R�}���h�T�u�L�����A���g��
	CString m_strCmdFreqCmb;
	// �ϒ����� S��
	CString m_strMdlSCmb;
	// �ϒ����� X��
	CString m_strMdlXCmb;
	// ���M�h�b�v���⏞
	CString m_strTDoppCmb;
	// �A���e�i�����ǔ�
	CString m_strAntAutoCmb;
	// �A���e�i�҂��󂯎w���̗L��
	CComboBoxEx2 m_AntStandbyCmb;
	CString m_strAntStandbyCmb;
	// �A���e�i�ҋ@�ʒu�w���̗L��
	CComboBoxEx2 m_AntStandbyPosCmb;
	CString m_strAntStandbyPosCmb;
	afx_msg void OnCbnSelchangeCidselAntstdorientedCmb();
	afx_msg void OnCbnSelchangeCidselAntstdposorientedCmb();
	afx_msg void OnBnClickedOk();
};
