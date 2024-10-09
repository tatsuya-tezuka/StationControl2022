#pragma once


// CUplinkCtrl �_�C�A���O

class CUplinkCtrl : public CDialogBase
{
	DECLARE_DYNAMIC(CUplinkCtrl)

public:
	CUplinkCtrl(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CUplinkCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_UPLINKCTRL };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CString				m_strRfTripMonitor;		// RF TRIP�Ď�������
	CString				m_strRealAz;			// REAL AZ������
	CString				m_strAzSkyLIne;			// AZ������SKYLINE������
	CComboBox			m_CCBoxRfTripMonitor;	// RF TRIP�Ď��R���{�{�b�N�X
	eRfTripMonitorType	m_rfTripMonitor;		// RF TRIP�Ď�

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL CreateDlgItems();									// �_�C�A���O�A�C�e���𐶐�

	CString GetRfTripMonitorStr();							// �R���{�{�b�N�X��RF TRIP�Ď���������擾����
	void SetRfTripMonitor(char szRfTripMode[]);				// RF TRIP�Ď���ݒ肷��
	void SetRfTripMonitorNull();							// RF TRIP�Ď���NULL�ݒ肷��
	BOOL UpdateKanshiData();
	UINT GetRfTripMonitor()		{ return (UINT)m_rfTripMonitor; }
	unsigned char GetUpLinkBinary();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedUplinkctrlRftripmonitorButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	/* ------------------------------------------------------------------------------------ */
};
