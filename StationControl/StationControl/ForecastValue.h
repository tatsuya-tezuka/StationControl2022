#pragma once

#include "ForecastValueGroupView.h"
#include "DateTimeCtrlEx.h"
#include "ListCtrlForecast.h"

// CForecastValue �_�C�A���O

class CForecastValue : public CDialogBase
{
	DECLARE_DYNAMIC(CForecastValue)

public:
	CForecastValue(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CForecastValue();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FORECASTVALUE };

	enum eGenSen {
		eAnt = 0,
		eTle
	};

	enum eShikibetsu {
		eNOM = 0,
		eOFF
	};

	// ���X�g���ڂ̕\����
	static const int m_nStationNameWidth =			100;	// �q�������ڕ�
	static const int m_nSatelliteNameWidth =		120;	// �q�������ڕ�
	static const int m_nGensenWidth =				 40;	// ���򍀖ڕ�
	static const int m_nSikibetuWidth =				 40;	// ���ʍ��ڕ�
	static const int m_nPassIDWidth =				 80;	// �p�XID���ڕ�
	static const int m_nDateWidth =					120;	// DATE TIME���ڕ�
//	static const int m_nTimeWidth =					 60;	// DATE TIME���ڕ�
	static const int m_nAZWidth =					 60;	// AZ EL���ڕ�
	static const int m_nOffsetWidth	=				 60;	// OFFSET���ڕ�
	static const int m_nFormatWidth =				 60;	// FORMAT���ڕ�
	static const int m_nGetStateWidth =				 50;	// �擾���ڕ�

	typedef struct {
		UINT m_upper;
		UINT m_lower;
	} stPassId;

	typedef struct {
		CString			m_stationName;			// �ǖ�
		CString			m_satelliteName;		// �q����
//		eGenSen			m_gensen;				// ����
//		eShikibetsu		m_shikibetsu;			// ����
//		stPassId		m_passId;				// �p�XID
		CString			m_passId;				// �p�XID
		CTime			m_aosTime;				// DATE TIME (AOS)
		double			m_aosAz;				// AZ (AOS)
		double			m_aosEl;				// EL (AOS)
		CTime			m_losTime;				// DATE TIME (LOS)
		double			m_losAz;				// AZ (LOS)
		double			m_losEl;				// EL (LOS)
		CTime			m_maxelTime;			// DATE TIME (MAX EL)
		double			m_maxelAz;				// AZ (MAX EL)
		double			m_maxelEl;				// EL (MAX EL)
//		double			m_offset;				// OFFSET
		time_t			m_offset;				// OFFSET
		CString			m_format;				// FORMAT
		CString			m_getState;				// �擾
		CString			m_filepath;				// �\��l�t�@�C���p�X
	} structListItem;

	typedef struct {
		int x1;
		int x2;
	} structGroupXPos;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
//	CListCtrl m_listForecastValue;		// �\��l���X�g�R���g���[��
//	CListCtrlEx m_listForecastValue;		// �\��l���X�g�R���g���[��
	CListCtrlForecast m_listForecastValue;		// �\��l���X�g�R���g���[��
	CStatic m_viewArea;					// �\��l�O���[�v�r���[�G���A
	CForecastValueGroupView* m_pView;	// �\��l�O���[�v�r���[
	BOOL m_bUpateView;					// �\��l�O���[�v�r���[�̍X�V
	BOOL m_bUpperSort;					// �����\�[�g
	int m_nSelectedColumn;				// �I�����ꂽ�J����
	CString m_strStationName;			// �ǖ�
	int m_nStationIndex;				// �ǃC���f�b�N�X
	CString m_strSatelliteName;			// �q����
//	deque<structListItem> m_dequeItem;	// �A�C�e���L���[
//	CEdit m_editCtrl;					// �G�f�B�b�g�R���g���[��
//	CDateTimeCtrl m_timeCtrl;			// �����R���g���[��
//	CDateTimeCtrlEx m_timeCtrl;			// �����R���g���[��
	int m_nPosEditCtrl;					// �G�f�B�b�g�R���g���[���ʒu
	time_t m_offset;					// OFFSET
	CString	m_strSelectedFilePath;		// �I�����ꂽ�t�@�C���p�X

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL InitList();
	BOOL CreateGroupView();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int CompareFunc(LPARAM lParam1, LPARAM lParam2);
	BOOL SetStationName();
	BOOL SetStationIndex();
	BOOL SetSatelliteName();
	void CreateSampleData();
//	int GetPassIdIndex(CString strPassId, deque<structListItem>& list);

public:
	void GetAosXPos(structGroupXPos& stXPos);
	void GetLosXPos(structGroupXPos& stXPos);
	void GetMaxElXPos(structGroupXPos& stXPos);
	BOOL InsertListItem();
	BOOL UpdateListItem();
//	BOOL DeleteListItem();
	BOOL GetSelectedForecastFilePath(CString& strFilePath, BOOL bChk = FALSE);
//	BOOL GetSelectedForecastStationNameAndFilePath(CString& strStationName, CString& strFilePath, BOOL bChk = FALSE);
	BOOL GetSelectedForecastStationNameAndFilePath(CString& strStationName, CString& strFilePath, int& nFormat, BOOL bChk = FALSE);
	BOOL GetSelectedForecastStationNameAndFullFilePath(CString& strStationName, CString& strSatelliteName, CString& strPassId, CString& strFilePath, BOOL bChk = FALSE);
	BOOL GetSelectedForecastStationNameAndFullFilePath(vector<stPredListData>& listPred, BOOL bChk = FALSE);
	BOOL GetSelectedForecastOffsetTime(time_t& offsetTime);
	BOOL GetStationNameList(vector<CString>& listName);
	BOOL GetSatelliteNameList(vector<CString>& listName);

	void InitRadarGraphView();
	CForecastValueGroupView* GetGroupView()	{ return m_pView; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnLvnItemchangedForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHdnItemclickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLvnBeginScrollForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnEndtrackForecastvalueList1(NMHDR *pNMHDR, LRESULT *pResult);
	/* ------------------------------------------------------------------------------------ */
};
