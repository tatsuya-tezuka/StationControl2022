#pragma once

#include "ListCtrlForecast.h"

// CForecastOperationPlan �_�C�A���O

class CForecastOperationPlan : public CDialogBase
{
	DECLARE_DYNAMIC(CForecastOperationPlan)

public:
	CForecastOperationPlan(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CForecastOperationPlan();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FORECASTOPERATIONPLAN };

	enum eOperationType {
		eOp		 = 0,
		eRdnChk 
	};

	enum eExecResult {
		eOK = 0,
		eNG
	};

	// ���X�g���ڂ̕\����
	static const int m_nStationNameWidth = 100;			// �ǖ����ڕ�
	static const int m_nSatelliteNameWidth = 120;		// �q�������ڕ�
	static const int m_nPassIdWidth = 80;				// �p�XID���ڕ�
	static const int m_createTypeWidth = 60;			// �쐬���ڕ�
	static const int m_operationTypeWidth = 100;		// �^�p��ʍ��ڕ�
	static const int m_prePassStartTimeWidth = 120;		// �v���p�X�J�n�������ڕ�
	static const int m_passStartTimeWidth = 120;		// �p�X�J�n�������ڕ�
	static const int m_passEndTimeWidth = 120;			// �p�X�I���������ڕ�
//	static const int m_execResultWidth = 60;			// ���s���ʍ��ڕ�

	typedef struct {
		CString			m_stationName;			// �ǖ�
		CString			m_satelliteName;		// �q����
//		UINT			m_nPlanId;				// �v��ID
		CString			m_passId;				// �p�XID
		CString			m_createBy;				// �쐬
//		eOperationType  m_operationType;		// �^�p���
		CString			m_operationType;		// �^�p���
		CTime			m_prePassStartTime;		// �v���p�X�J�n����
		CTime			m_passStartTime;		// �p�X�J�n����
		CTime			m_passEndTime;			// �p�X�I������
		CString			m_filepath;				// �v��t�@�C���p�X
//		eExecResult		m_execResult;			// ���s����
//		CString			m_execResult;			// ���s����
	} structListItem;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CListCtrlForecast m_listOperationPlan;		// �^�p�v�惊�X�g�R���g���[��
	BOOL m_bUpperSort;					// �����\�[�g
	int m_nSelectedColumn;				// �I�����ꂽ�J����
	CString m_strStationName;			// �ǖ�
	CString m_strSatelliteName;			// �q����
	int m_nStationIndex;				// �ǃC���f�b�N�X
//	deque<structListItem> m_dequeItem;	// �A�C�e���L���[
//	vector<stPlanListData>	m_listPlanList;		// �v��ꗗ�f�[�^
	CString m_strSelectedStation;		// �I�����ꂽ��
	CString m_strSelectedSatellite;		// �I�����ꂽ�q��
	CString m_strSelectedPassId;		// �I�����ꂽ�v��ID
	CString m_strSelectedGetFrom;		// �I�����ꂽ�쐬��
	CString	m_strSelectedFile;			// �I�����ꂽ�t�@�C��

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL InitList();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int CompareFunc(LPARAM lParam1, LPARAM lParam2);
	BOOL SetStationName();
	BOOL SetStationIndex();
	BOOL SetSatelliteName();
	void CreateSampleData();
	void GetPlanList();

public:
	BOOL InsertListItem();
	BOOL UpdateListItem();
	BOOL GetSelectedPredFilePath(CString& strFilePath, BOOL bChk = FALSE);
	BOOL GetSelectedPlanID(CString& strPlanID, __int64& tPassStart, __int64& tPassEnd, BOOL bChk = FALSE);
	BOOL GetSelectedStationNameAndPredFilePath(CString& strStationName, CString& strSatelliteName, CString& strPassId, CString& strGetFrom, CString& strFilePath, BOOL bChk = FALSE);
	BOOL GetSelectedStationNameAndPlanFilePath(CString& strStationName, CString& strSatelliteName, CString& strFilePath, CString& strGetFrom, BOOL bChk = FALSE);
	BOOL GetSelectedStationNameAndPlanFilePath(vector<stPlanListData>& listPlan, BOOL bChk = FALSE);
	BOOL IsUsedPlan(CString& strStationName, CString& strSatelliteName, CString& strPredPassId);
	void InitRadarGraphView();
	BOOL GetStationNameList(vector<CString>& listName);
	BOOL GetSatelliteNameList(vector<CString>& listName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHdnItemclickForecastoperationplanList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickForecastoperationplanList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	/* ------------------------------------------------------------------------------------ */
};
