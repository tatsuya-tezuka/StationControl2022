#pragma once
#include "afxwin.h"

// CDownlinkCapture �_�C�A���O

class CDownlinkCapture : public CDialogBase
{
	DECLARE_DYNAMIC(CDownlinkCapture)

public:
	CDownlinkCapture(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CDownlinkCapture(UINT IDD, CWnd* pParent = NULL);
	virtual ~CDownlinkCapture();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_DOWNLINKCAPTURE };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString				m_strRcvFreqMode;		// ��M���g�����[�h������
	CString				m_strCtrlMode;			// ���䃂�[�h������
	CComboBox			m_CCBoxRcvFreqMode;		// ��M���g�����[�h�R���{�{�b�N�X
	CComboBox			m_CCBoxCtrlMode;		// ���䃂�[�h�R���{�{�b�N�X
	eRcvFreqModeType	m_rcvFreqMode;			// ��M���g�����[�h�i�Ď��j
	eCtrlModeType		m_ctrlMode;				// ���䃂�[�h�i�Ď��j

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL CreateDlgItems();								// �_�C�A���O�A�C�e���𐶐�
	BOOL CreateDlgRcvFreqMode();						// ��M���g�����[�h�_�C�A���O�A�C�e���𐶐�
	BOOL CreateDlgCtrlMode();							// ���䃂�[�h�_�C�A���O�A�C�e���𐶐�

	CString GetRcvFreqModeComboStr();					// �R���{�{�b�N�X�̎�M���g�����[�h��������擾����
	void SetRcvFreqMode(char szRcvFreqMode[]);			// ��M���g�����[�h��ݒ肷��

	CString GetCtrlModeComboStr();						// �R���{�{�b�N�X�̐��䃂�[�h��������擾����
	void SetCtrlMode(char szCtrlMode[]);				// ���䃂�[�h��ݒ肷��
	void SetCtrlModeNull();								// ���䃂�[�h��NULL�ݒ肷��

	BOOL UpdateKanshiData();							// �Ď��f�[�^�X�V
	unsigned char GetRcvFreqModNotifyData();			// ��M���g�����[�h�ʒm�f�[�^�擾
//	unsigned char GetCtrlModNotifyData();				// ���䃂�[�h�ʒm�f�[�^�擾
	unsigned char GetAutoFreqData();					// �����̏ꍇ�̎��g���f�[�^
	unsigned char GetManualFreqData();					// �蓮�̏ꍇ�̎��g���f�[�^
	unsigned char GetWayBinary();						// Way���[�h�̃o�C�i���l�擾
	unsigned char GetDownLinkBinary();

	UINT GetRcvFreqMode()	{ return (UINT)m_rcvFreqMode; }
	UINT GetCtrlMode()		{ return (UINT)m_ctrlMode; }
	unsigned char GetCtrlModeFromCombo();
	virtual void ShowRcvFreqMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDownlinkcaptureRcvfreqmodeButton();
	afx_msg void OnBnClickedDownlinkcaptureCtrlmodeButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	/* ------------------------------------------------------------------------------------ */
	afx_msg void OnCbnSelchangeDownlinkcaptureCtrlmodeCombo();
};
