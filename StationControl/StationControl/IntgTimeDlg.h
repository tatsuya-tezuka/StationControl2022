#pragma once


// CIntgTimeDlg �_�C�A���O

class CIntgTimeDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CIntgTimeDlg)

public:
	CIntgTimeDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CIntgTimeDlg();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_INTGTIME };

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	int m_nIntgTime;
	CEdit m_cedit;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetIntgTime(int time)	{ m_nIntgTime = time; }
	int GetIntgTime()			{ return m_nIntgTime; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	/* ------------------------------------------------------------------------------------ */
};
