#pragma once


// CTleList �_�C�A���O

class CTleList : public CDialogBase
{
	DECLARE_DYNAMIC(CTleList)

public:
	CTleList(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTleList();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_TLELIST };

	enum eShikibetsu {
		eNOM = 0,
		eOFF
	};

	// ���X�g���ڂ̕\����
	static const int m_nSatelliteNameWidth = 100;		// �q�������ڕ�
	static const int m_nSikibetuWidth = 40;				// ���ʍ��ڕ�
	static const int m_epochTimeWidth = 120;			// �v���p�X�J�n�������ڕ�
	static const int m_propProcTimeWidth = 120;			// �p�X�J�n�������ڕ�

	typedef struct {
		CString			m_satelliteName;		// �q����
		eShikibetsu		m_shikibetsu;			// ����
		CTime			m_epochTime;			// EPOCH
		CTime			m_propProcTime;			// �`����������
	} structListItem;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CListCtrl m_listTle;				// TLE���X�g�R���g���[��
	BOOL m_bUpperSort;					// �����\�[�g
	int m_nSelectedColumn;				// �I�����ꂽ�J����
	CString m_strSatelliteName;			// �q����
	deque<structListItem> m_dequeItem;	// �A�C�e���L���[
	CComboBox m_CCBoxSatelliteName;		// �q�����R���{�{�b�N�X

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL InitList();
	void InitComboBox();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int CompareFunc(LPARAM lParam1, LPARAM lParam2);
	BOOL SetSatelliteName();
	void CreateSampleData();

public:
	CString GetSatelliteName();
	BOOL InsertListItem();
	BOOL UpdateListItem();
	BOOL DeleteListItem();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuClose();
	afx_msg void OnHdnItemclickTlelistList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTlelistButton1();
	afx_msg void OnBnClickedTlelistButton2();
	afx_msg void OnBnClickedTlelistButton3();
	afx_msg void OnBnClickedTlelistButton4();
	afx_msg void OnBnClickedTlelistButton5();
	afx_msg void OnBnClickedTlelistButton6();
	afx_msg void OnCbnSelchangeTlelistCombo();
	/* ------------------------------------------------------------------------------------ */
};
