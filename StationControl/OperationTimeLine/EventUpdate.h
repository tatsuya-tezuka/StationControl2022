#pragma once
#include "afxdtctl.h"

static const CString mEventUpdateText[] = {
	_T("�A�b�v�����N�̓�����ύX���܂�"),
	_T("CMD�̓�����ύX���܂�"),
	_T("RNG�̓�����ύX���܂�"),
};

enum{
	eEventUpdateModeUplink,
	eEventUpdateModeCmd,
	eEventUpdateModeRng,
};

// CEventUpdate �_�C�A���O

class CEventUpdate : public CDialogEx
{
	DECLARE_DYNAMIC(CEventUpdate)

public:
	CEventUpdate(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEventUpdate();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_EVENTUPDATE };

	int		mMode;
	CTime	mStartTime, mEndTime;
	CTime	mDefStartTime, mDefEndTime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CString m_strText;
	CString m_strStart;
	CString m_strEnd;
	CDateTimeCtrl m_StartDate;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_EndDate;
	CDateTimeCtrl m_EndTime;
};
