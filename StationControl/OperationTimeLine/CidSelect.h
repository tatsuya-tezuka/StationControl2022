#pragma once

#include "../StationControl/ComboBoxEx2.h"

namespace timeline{

// CID�I����ʃf�[�^�Ǘ��N���X

typedef struct {	// �q�����ACID�I������
	vector<CString>	CID;				// CID
	vector<CString>	TlmBitRateS;		// �e�����g���r�b�g���[�g S��
	vector<CString>	TlmBitRateX;		// �e�����g���r�b�g���[�g X��
	vector<CString>	TlmBitRateHX;		// �e�����g���r�b�g���[�g X����
	vector<CString>	TlmBitRateKa;		// �e�����g���r�b�g���[�g Ka��
	vector<CString>	CmdBitRateS;		// �R�}���h�r�b�g���[�g S��
	vector<CString>	CmdBitRateX;		// �R�}���h�r�b�g���[�g X��
	vector<CString>	TrsPower;			// ���M�d��
	vector<CString>	UpLinkSweepShape;	// �A�b�v�����N�|���p�����[�^ �`��
	vector<CString>	RngMesMode;			// �����W�v���p�����[�^ ���[�h
	vector<CString>	CarrLoopBandS;		// �L�����A��M���[�v�o���h S��
	vector<CString>	CarrLoopBandX;		// �L�����A��M���[�v�o���h X��
	vector<CString>	CarrLoopBandKa;		// �L�����A��M���[�v�o���h Ka��
	vector<CString>	CmdMod;				// �R�}���h�ϒ��x
	vector<CString>	CmdSubCarrFreq;		// �R�}���h�T�u�L�����A���g��
	vector<CString>	TransDoppler;		// ���M�h�b�v��
	vector<CString>	AntAutoTrack;		// �A���e�i�����ǔ��ڍs�̗L��
	vector<CString>	Modulation;			// �ϒ�����
	vector<CString>	ModulationS;		// �ϒ����� S��
	vector<CString>	ModulationX;		// �ϒ����� X��
	vector<CString>	AntAosPosn;			// �A���e�i�҂��󂯎w���̗L��
	vector<CString>	AntWaitPoint;		// �A���e�i�ҋ@�ʒu�̗L��
}stCIDPrmEle;

typedef struct {	// CID���p�����[�^
	CString	CID;				// CID
	CString	TlmBitRateS;		// �e�����g���r�b�g���[�g S��
	CString	TlmBitRateX;		// �e�����g���r�b�g���[�g X��
	CString	TlmBitRateHX;		// �e�����g���r�b�g���[�g X����
	CString	TlmBitRateKa;		// �e�����g���r�b�g���[�g Ka��
	CString	CmdBitRateS;		// �R�}���h�r�b�g���[�g S��
	CString	CmdBitRateX;		// �R�}���h�r�b�g���[�g X��
	CString	TrsPower;			// ���M�d��
	CString	CarrLoopBandS;		// �L�����A��M���[�v�o���h S��
	CString	CarrLoopBandX;		// �L�����A��M���[�v�o���h X��
	CString	CarrLoopBandKa;		// �L�����A��M���[�v�o���h Ka��
	CString	UpLinkSweepShape;	// �A�b�v�����N�|���p�����[�^ �`��
	CString	UpLinkSweepWidth;	// �A�b�v�����N�|���p�����[�^ ��
	CString	UpLinkSweepTime;	// �A�b�v�����N�|���p�����[�^ ����
	CString	RngMesMode;			// �����W�v���p�����[�^ ���[�h
	CString	RngMesSmplRate;		// �����W�v���p�����[�^ �T���v�����O���[�g
	CString	RngMesSmplItgTime;	// �����W�v���p�����[�^ �ϕ�����
	CString	RngMesSmplMod;		// �����W�v���p�����[�^ �ϒ��x
	CString	Modulation;			// �ϒ����� S��
	CString	ModulationS;		// �ϒ����� S��
	CString	ModulationX;		// �ϒ����� X��
	CString	CmdMod;				// �R�}���h�ϒ��x
	CString	CmdSubCarrFreq;		// �R�}���h�T�u�L�����A���g��
	CString	TransDoppler;		// ���M�h�b�v��
	CString	AntAutoTrack;		// �A���e�i�����ǔ�
	CString	AntAosPosn;			// �A���e�i�҂��󂯎w��
	CString	AntWaitPoint;		// �A���e�i�ҋ@�ʒu�w��
}stCIDParam;

typedef struct {	// CID���p�����[�^
	BOOL	CID;				// CID
	BOOL	TlmBitRateS;		// �e�����g���r�b�g���[�g S��
	BOOL	TlmBitRateX;		// �e�����g���r�b�g���[�g X��
	BOOL	TlmBitRateHX;		// �e�����g���r�b�g���[�g X����
	BOOL	TlmBitRateKa;		// �e�����g���r�b�g���[�g Ka��
	BOOL	CmdBitRateS;		// �R�}���h�r�b�g���[�g S��
	BOOL	CmdBitRateX;		// �R�}���h�r�b�g���[�g X��
	BOOL	TrsPower;			// ���M�d��
	BOOL	CarrLoopBandS;		// �L�����A��M���[�v�o���h S��
	BOOL	CarrLoopBandX;		// �L�����A��M���[�v�o���h X��
	BOOL	CarrLoopBandKa;		// �L�����A��M���[�v�o���h Ka��
	BOOL	UpLinkSweepShape;	// �A�b�v�����N�|���p�����[�^ �`��
	BOOL	UpLinkSweepWidth;	// �A�b�v�����N�|���p�����[�^ ��
	BOOL	UpLinkSweepTime;	// �A�b�v�����N�|���p�����[�^ ����
	BOOL	RngMesMode;			// �����W�v���p�����[�^ ���[�h
	BOOL	Modulation;			// �ϒ����� S��
	BOOL	ModulationS;		// �ϒ����� S��
	BOOL	ModulationX;		// �ϒ����� X��
	BOOL	CmdMod;				// �R�}���h�ϒ��x
	BOOL	CmdSubCarrFreq;		// �R�}���h�T�u�L�����A���g��
	BOOL	TransDoppler;		// ���M�h�b�v��
	BOOL	AntAutoTrack;		// �A���e�i�����ǔ�
	BOOL	AntAosPosn;			// �A���e�i�҂��󂯎w��
	BOOL	AntWaitPoint;		// �A���e�i�ҋ@�ʒu�w��
}stCIDPrmFlg;

#define CID_INVALID		_T("---------")

class CCIDSlctData
{
public:
	CCIDSlctData();
	~CCIDSlctData();

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
	vector<CString>							m_SatelliteNameList;	// �q�������X�g
	map<CString, stCIDPrmEle>				m_CidElementList;		// �q����CID�p�����[�^�v�f
	map<CString, stCIDPrmFlg>				m_CidPrmFlgList;		// �q����CID�p�����[�^�L��/�����t���O
	map<CString, map<CString, stCIDParam>>	m_CidParamList;		// �q����CID���p�����[�^�ݒ�l

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CString>&						GetSatelliteNameList() { return m_SatelliteNameList; }
	vector<CString>							GetCidList(CString sat);
	map<CString, stCIDPrmEle>&				GetCidElementList() { return m_CidElementList; }
	map<CString, map<CString, stCIDParam>>&	GetCidParamList() { return m_CidParamList; }
	map<CString, stCIDPrmFlg>&				GetCidPrmFlgList()	{ return m_CidPrmFlgList; }
	static stCIDParam						TrimInvalidString(stCIDParam stCid);

protected:

};

// CCidSelect �_�C�A���O

class CCidSelect : public CDialogEx
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
	CCIDSlctData	m_CIDSlctData;		// CID�I����ʃf�[�^
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
	void		SetTxtMode(int val) { m_xtx = val; }

protected:
public:
	BOOL	InitDlg();
	BOOL	SetComboboxSatCIDParam(CString sat);
	BOOL	SetComboboxCIDParam(CString sat, CString cid);
	CString	CheckCid();
	//void	GetEqSatDB(CString satname);
	BOOL	ReadCIDFile();
	void	AddDataVector(vector<CString> &v, CString str);


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

}; //namespace timeline
