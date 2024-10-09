#pragma once

// CAntStanbyOriented �_�C�A���O(54m)

class CAntStanbyOriented : public CDialogBase
{
	DECLARE_DYNAMIC(CAntStanbyOriented)

public:
	CAntStanbyOriented(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CAntStanbyOriented();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_ANTSTANBYORIENTED };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	UpdateSatellite(BOOL update);

protected:
	CButton		m_buttonAntForecast;		// �A���e�i�\��l�{�^���R���g���[��
	CString		m_strPassStart;				// �p�X�J�n����
	CString		m_strPassEnd;				// �p�X�I������
	CButton		m_buttonExec;				// ���s�{�^���R���g���[��
	CComboBox	m_SatelliteCmb;				// �T���@�R���{�{�b�N�X
	CString		m_satellilteName;			// �T���@��
	vector<CString> mSatelliteNameList;		// �T���@�ꗗ
	vector<stPredListData> mPartPredList;	// �\��l�ꗗ(�I�𒆒T���@)

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual BOOL OnInitDialog();
	void SetFileName(CString filepath);
	void GetKanshiData(CString fname);
	void UpdateDlg(CString fname);

protected:
	void GetSatelliteNameList();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeSatelliteCmb();
	afx_msg void OnBnClickedSatellitecaptureAntforecastButton();
	afx_msg void OnBnClickedAntstanbyorientedExecButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};
