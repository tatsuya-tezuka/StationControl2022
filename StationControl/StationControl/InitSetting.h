#pragma once


// CInitSetting �_�C�A���O

class CInitSetting : public CDialogBase
{
	DECLARE_DYNAMIC(CInitSetting)

public:
	CInitSetting(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CInitSetting();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_INITSETTING };

	// �����ݒ�
	typedef struct {
		DWORD initID;					// �����ݒ�ID
		CString initName;				// �����ݒ�Ώۖ�
	} structInitSetting;

	typedef struct {
		DWORD id;									// �q��ID
		vector<structInitSetting> listInitSetting;	// �����ݒ�
	} structSatelliteInitSetting;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CComboBox m_CCBoxSatelliteName;								// �q�����R���{�{�b�N�X
	CButton m_CButonSelCid;										// CID�I���{�^��
	CListBox m_listInitSetting;									// �����ݒ胊�X�g�{�b�N�X
	CString m_strSatelliteName;									// �q����
	CButton m_chkAllSelect;										// �S�I��

	vector<CString> m_listSatellite;							// �q�����X�g
	map<CString, vector<CString>> m_listInitMacr;				// �����ݒ�}�N����`���X�g

	CArray<int, int> m_aryListBoxSel;							// �I�����X�g�{�b�N�X
	CString m_satSel;											// �I���q��
	int m_aryListBoxPos;										// ���X�g�{�b�N�X�ʒu

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	BOOL InitDlg();
	void RestListBox(CString sat);
	void RestCIDBtn(CString sat);

public:
	virtual BOOL UpdateDlg();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeInitsettingCombo1();
	afx_msg void OnBnClickedInitsettingCidButton();
	afx_msg void OnBnClickedInitsetingCheck();
	BOOL m_CidChkBox;
	afx_msg void OnBnClickedCidcheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	/* ------------------------------------------------------------------------------------ */
};
