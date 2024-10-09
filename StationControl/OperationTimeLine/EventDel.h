#pragma once


static const CString mEventDelText[] = {
	_T("�A�b�v�����N�ԍ� %d ���폜���܂�\n��낵���ł����H"),
	_T("CMD�ԍ� %d ���폜���܂�\n��낵���ł����H"),
	_T("RNG�ԍ� %d ���폜���܂�\n��낵���ł����H"),
};

enum{
	eEventDelModeUplink,
	eEventDelModeCmd,
	eEventDelModeRng,
};

// CEventDel �_�C�A���O

class CEventDel : public CDialogEx
{
	DECLARE_DYNAMIC(CEventDel)

public:
	CEventDel(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEventDel();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_EVENTDEL };

	int		mMode;
	int		mTargetNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
