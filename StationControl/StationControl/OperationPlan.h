#pragma once
#include "afxcmn.h"
#include "atltime.h"
#include "afxwin.h"
#include "OperationPlanTimeSet.h"
#include "OperationPlanGraph.h"
#include "CidSelect.h"
#include "SatelliteData.h"
#include "afxdtctl.h"
#include "DetailViewDlg.h"
#include "DateTimeCtrlNoToday.h"


#define BASE_OPE_NO		5201
#define MSG_CREATE		2

// COperationPlan �_�C�A���O

class COperationPlan : public CDialogBase
{
	DECLARE_DYNAMIC(COperationPlan)

public:
	COperationPlan(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COperationPlan();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_OPERATIONPLAN };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	typedef struct {
		double AZ;
		double EL;
	} stAzEl;

	typedef struct {
		CTime time;
		double AZ;
		double EL;
	} stAnpData;

protected:


	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	// ANT�҂������w���ʒm
	BOOL m_bAntStandby;
	// ANT�V���u���L��
	BOOL m_bAntZenith;
	// �^�p�I�v�V����-�����ݒ�
	BOOL m_bOpInit;

	// �A���e�i�\��l AOS
	CTime m_tAOS;
	CTime m_tAOSRf;
	CTime m_tAOSTLM;
	// �A���e�i�\��l LOS
	CTime m_tLOS;
	CTime m_tLOSRf;
	CTime m_tLOSTLM;

	// �^�p�J�n����
	CTime m_tOpeS;
	// �^�p�I������
	CTime m_tOpeE;

	// TLM S �J�n����
	CTime m_tTLMSS;
	// TLM S �I������
	CTime m_tTLMSE;

	// TLM X �J�n����
	CTime m_tTLMXS;
	// TLM X �I������
	CTime m_tTLMXE;

	// TLM Ka �J�n����
	CTime m_tTLMKaS;
	// TLM Ka �I������
	CTime m_tTLMKaE;

	// �^�p�I�v�V����-���M�o�̓^�C�v
	int m_OpTOutType;

	// x_tx (Ka�t���O)
	BOOL m_bXTX;

protected:
	long		m_l_my_ipc;			// IPC�L�[
	long		m_l_my_mid;			// ��M���b�Z�[�WID
	CString		m_SatelliteName;
	CString		m_PassID;
	CString		m_PassIDMask;
	stPlandata	m_PlanData;
	map<CString, int> m_RdyTimeAGCList;
	map<CString, int> m_RdyTimeRNGList;
	map<CString, int> m_RdyTimeTLMList;
	map<CString, int> m_RdyTimeCMDList;
	COprPlnData m_OprPlnDataOrg;
	BOOL		m_bNewPlan;
	vector<CString> m_RngEleList;

	int			m_IdxEqsat;
	DWORD		m_GetForm;

	vector<stAzEl>		m_SkyLineDataList;
	vector<double>		m_RFTripDataList;
	vector<stAnpData>	m_AnpDataList;
	stAnpData			m_AnpDataAos;
	stAnpData			m_AnpDataLos;
	stAnpData			m_AnpDataMax;

	CString				m_strRngMesMode;
	BOOL		m_bForceClosed;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	BOOL	IsConflictTimeInList(CListCtrl& list, CTime cSTime, CTime cETime, int idx = -1);
	BOOL	IsProximityTime(CTime cSTime, CTime cETime, int type, int idx);
	void	SetSatelliteName(CString str) { m_SatelliteName = str; }
	void	SetPassId(CString str)	{ m_PassID = str; }
	void	SetGetForm(DWORD form)	{ m_GetForm = form; }
	void	SetType(UINT type)		{
		if (type == 1)
		{
			m_bNewPlan = TRUE;
		}
		else
		{
			m_bNewPlan = FALSE;
		}
	}
	//virtual BOOL UpdateDlg();
	BOOL	UpdateOperationPlanDlg();
	void	ClearForceClosed()		{ m_bForceClosed = FALSE; }

protected:
	int		rev_env_set();
	void	GetAntFrcTime(CTime &cAos, CTime &cLos);		// �A���e�i�\��l�擾
	CString ChgPlanIDFormat(CString str);
	BOOL	SetPlanData();
	BOOL	GetPlanData(CString satellite, CString planid, COprPlnData& pd);
	BOOL	SetPlanDataMap(CString satellite, CString planid, COprPlnData data);
	BOOL	CheckPlanDataDlg();
	BOOL	GetPredTime(CString pc_satname, CString pc_pass, CTime& pc_aos_time, CTime& pc_los_time, stPlandata& stpd);
	BOOL	GetPlanList();
	BOOL	GetSatelliteInfoData(CString satname);
	BOOL	GetPlanFile(CString satname, CString pass, DWORD lParam);
	BOOL	PutPlanFile(CString path, CString file);
	BOOL	AccessPlanData();
//	int		ud_plan_rw(long l_mode, CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFormFile20(CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFormFile34(CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFormFile64(CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFormFile54(CString sz_satname, CString sz_passno, stPlandata& stpd);
	int		ReadPlanFile20(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw);
	int		ReadPlanFile34(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw);
	int		ReadPlanFile64(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw);
	int		ReadPlanFile54(CString sz_satname, CString sz_passno, stPlandata& stpd, DWORD dw);
	int 	CreatePlanFile(BOOL bRegist = TRUE);
	int		WritePlanFile20(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist = TRUE);
	int		WritePlanFile34(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist = TRUE);
	int		WritePlanFile64(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist = TRUE);
	int		WritePlanFile54(CString sz_satname, CString sz_passno, stPlandata& stpd, BOOL bRegist = TRUE);
	CString	ExchageRngTypePlanFile(CString str);
	CString	ExchageRngTypePlanFile54(CString str);
	CString	GetOpeMode();
	CString	GetOpeMode54();
	void	GetOpeModeAddString(CString &str, CString mode);
	void	CalcRdyTime20();
	void	CalcRdyTime34();
	void	CalcRdyTime64();
	void	CalcRdyTime54();
	BOOL	InitDlg();
	CString GetFullPath2FileName(CString str);
	CString GetFilePath();
	BOOL	GetTimeInList(CListCtrl& list, int idx, CTime& cSTime, CTime& cETime);
	int		GetRdyTime20(CString str);
	int		GetRdyTime34(CString str);
	int		GetRdyTime64(CString str);
	int		GetRdyTime54(CString str);
	int		GetPlnTime54(CString str);
	BOOL	SetPlanDataDlg(int nStation, CString sz_passno, stPlandata& stpd);

	int		data_get_from_db20();
	int		data_get_from_db34();
	int		data_get_from_db64();
	int		data_get_from_db54();

	int		GetIndexEqSatDBData(CString str);

	BOOL	IsInPlanDayTime(CTime cSTime, CTime cETime);
	void	GetAnpData();

	BOOL	GetSkylineData();
	BOOL	GetELforSkylineData(int type, double &el);
	void	GetIntersectionDateSkyline(CTime &sTime, CTime &eTime);

	BOOL	GetRFTripData();
	BOOL	GetELforRFTripData(int type, double &el);
	void	GetIntersectionDateRFTrip(CTime &sTime, CTime &eTime);

	void	InvalidOerationGraph();

	void	InitPlanData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuClose();
	afx_msg void OnBnClickedOpeplnBasicprmUlAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmUlDelBtn();
	afx_msg void OnBnClickedOpeplnBasicprmRngAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmRngDelBtn();
	afx_msg void OnBnClickedOpeplnBasicprmCmdAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmCmdDelBtn();
	afx_msg void OnBnClickedOpeplnDetailBtn();
	afx_msg void OnBnClickedOpeplnPrintBtn();
	afx_msg void OnBnClickedOpeplnEntryBtn();
	afx_msg void OnBnClickedOpeplnBasicprmUlEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmRngEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmCmdAddBtn2();
	afx_msg void OnBnClickedOpeplnBasicprmCmdEditBtn();
	afx_msg void OnBnClickedOpeplnRdnsAgcCb();
	afx_msg void OnBnClickedOpeplnRdnsRngCb();
	afx_msg void OnBnClickedOpeplnRdnsTlmCb();
	afx_msg void OnBnClickedOpeplnRdnsCmdCb();
	afx_msg void OnCbnSelchangeOpeplnOpeopInitsetCmb();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOpeplnBrCidBtn();
	afx_msg void OnBnClickedOpeplnBasicprmUlCheck();
	afx_msg void OnBnClickedOpeplnBasicprmRngCheck();
	afx_msg void OnBnClickedOpeplnBasicprmCmdCheck();
	afx_msg void OnBnClickedOpeplnBasicprmTlmsCheck();
	afx_msg void OnBnClickedOpeplnBasicprmTlmxCheck();
	afx_msg void OnBnClickedOpeplnBasicprmTlmkaCheck();
	afx_msg void OnLvnItemchangedOpeplnStltList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkOpeplnStltList(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_SatelliteList;
	virtual BOOL OnInitDialog();
	// �^�p�J�n�����iDay�j
	CTime m_OpeStartDay;
	// �^�p�J�n�����iTime�j
	CTime m_OpeStartTime;
	// �^�p�I�������iDay�j
	CTime m_OpeEndDay;
	// �^�p�I�������iTime�j
	CTime m_OpeEndTime;
	// �A�b�v�����N�J�n�I���������X�g
	CListCtrl m_UpLinkList;
	// RNG�J�n�I���������X�g
	CListCtrl m_RngList;
	// CMD�J�n�I������
	CListCtrl m_CmdList;
	// �^�p�I�v�V����-�����ݒ�
	CComboBoxEx2 m_OpInitCmb;
	// �^�p�I�v�V����-��M�@�h�b�v���⏞
	CComboBoxEx2 m_OpRDPCmb;
	// ���f�B�l�X�`�F�b�N-AGC�Z��
	BOOL m_AGCChkValue;
	// ���f�B�l�X�`�F�b�N-RNG�Z��
	BOOL m_RNGChkValue;
	// ���f�B�l�X�`�F�b�N-TLM�]��
	BOOL m_TLMChkValue;
	// ���f�B�l�X�`�F�b�N-CMD�`��
	BOOL m_CMDChkValue;
	COperationPlanGraph m_OperationGraph;
	// ��{�p�����[�^-�v��ID
	CString m_strPlanID;
	CMFCMaskedEdit m_PlanIDEdit;
	// ��{�p�����[�^-�A�b�v�����N
	BOOL m_UpLinkChkBox;
	// ��{�p�����[�^-RNG
	BOOL m_RngChkBox;
	// ��{�p�����[�^-CMD
	BOOL m_CmdChkBox;
	// ��{�p�����[�^-TLMS
	BOOL m_TLMSChkBox;
	// ��{�p�����[�^ TLMX�`�F�b�N�{�b�N�X
	CButton m_TLMSCtrl;
	// ��{�p�����[�^-TLMX
	BOOL m_TLMXChkBox;
	// ��{�p�����[�^-TLMKa
	BOOL m_TLMKaChkBox;
	// ��{�p�����[�^-TLMKa�`�F�b�N�{�b�N�X
	CButton m_TLMKaCtrl;
	// �r�b�g���[�g-TLM S
	CString m_strBRTLMS;
	// �r�b�g���[�g-TLM X
	CString m_strBRTLMX;
	// �r�b�g���[�g-TLM Ka
	CString m_strBRTLMKa;	//��ʓ��o�͗p
	// �r�b�g���[�g-����TLM
	CString m_strBRHTLM;
	// �r�b�g���[�g-CMD S
	CString m_strBRCMDS;
	// �r�b�g���[�g-CMD X
	CString m_strBRCMDX;
	// �^�p�J�n�����iDay�j�R���g���[��
	CDateTimeCtrlNoToday m_OpeStartDayCtrl;
	// �^�p�J�n�����iTime�j�R���g���[��
	CDateTimeCtrl m_OpeStartTimeCtrl;
	// �^�p�I�������iDay�j�R���g���[��
	CDateTimeCtrlNoToday m_OpeEndDayCtrl;
	// �^�p�I�������iTime�j�R���g���[��
	CDateTimeCtrl m_OpeEndTimeCtrl;
	// �q����
	CString m_strSatellite;
	// �\��lAOS�iDAY�j
	CTime m_PredAosDay;
	// �\��lAOS�iTime�j
	CTime m_PredAosTime;
	// �\��lLOS�iDAY�j
	CTime m_PredLosDay;
	// �\��lLOS�iTime�j
	CTime m_PredLosTime;
	// �\��lAOS�R���g���[��
	CDateTimeCtrlNoToday m_PredAosDayCtrl;
	// �\��lLOS�R���g���[��
	CDateTimeCtrlNoToday m_PredLosDayCtrl;

	// �ڍו\�����
	CDetailViewDlg m_DetailDlg;			
	// �\��l�t�@�C����
	CString m_PredFileName;
	// CID�I���{�^���R���g���[��
	CButton m_CidBtnCtrl;
	// �r�b�g���[�g-CMD X
	CEdit m_CmdXEditCtrl;
	// TLM S �������X�g
	CListCtrl m_TlmSList;
	// TLM X �������X�g
	CListCtrl m_TlmXList;
	// TLM Ka �������X�g
	CListCtrl m_TlmKaList;
	// TLM S �ǉ��{�^��
	CButton m_TlmSAddBtn;
	// TLM S �ҏW�{�^��
	CButton m_TlmSEditBtn;
	// TLM S �폜�{�^��
	CButton m_TlmSDelBtn;
	afx_msg void OnBnClickedOpeplnBasicprmTlmsAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmsEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmsDelBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmxAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmxEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmxDelBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmkaAddBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmkaEditBtn();
	afx_msg void OnBnClickedOpeplnBasicprmTlmkaDelBtn();
	// TLM Ka �ǉ��{�^��
	CButton m_TlmKaAddBtn;
	// TLM Ka �ҏW�{�^��
	CButton m_TlmKaEditBtn;
	// TLM Ka �폜�{�^��
	CButton m_TlmKaDelBtn;
	// TLM ����
	CEdit m_TlmXHEditCtrl;
	// �r�b�g���[�g-CMD S
	CEdit m_CmdSEditCtrl;
	// �^�p�I�v�V����-���M�o��
	CString m_TrsPowerStr;
	// �^�p�I�v�V����-���M�@�h�b�v���⏞
	CString m_OpTDPStr;
	// ANT�҂������w���ʒm
	CString m_AntStandbyStr;
	// ANT�V���u���L��
	CString m_AntZenithStr;
	CDateTimeCtrl m_PredAosTimeCtrl;
	CDateTimeCtrl m_PredLosTimeCtrl;
	afx_msg void OnClose();
	// TLM X
	CButton m_TLMXCtrl;
	CButton m_TlmXAddBtn;
	CButton m_TlmXEditBtn;
	CButton m_TlmXDelBtn;
	afx_msg void OnDtnDatetimechangeOpeplnBasicprmOpeSdayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeOpeplnBasicprmOpeStimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeOpeplnBasicprmOpeEdayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeOpeplnBasicprmOpeEtimeDp(NMHDR *pNMHDR, LRESULT *pResult);
	// ��{�p�����[�^-�A�b�v�����N�`�F�b�N�{�b�N�X
	CButton m_UplinkChkBtnCtrl;
	// ��{�p�����[�^-RNG�`�F�b�N�{�b�N�X
	CButton m_RngChkBtnCtrl;
	// ��{�p�����[�^-CMD�`�F�b�N�{�b�N�X
	CButton m_CmdChkBtnCtrl;
};
