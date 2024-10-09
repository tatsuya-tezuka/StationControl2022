#pragma once


// CInputOccupancy �_�C�A���O

class CInputOccupancy : public CDialogEx
{
	DECLARE_DYNAMIC(CInputOccupancy)

public:
	CInputOccupancy(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CInputOccupancy();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_STATION_OCCUPANCY };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CTime		mStartTime, mEndTime;
	CTime		mTimeLineTime;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetStartTime(CTime val) { mStartTime = val; }
	void	SetEndTime(CTime val) { mEndTime = val; }
	void	SetTimeLineTime(CTime time) { mTimeLineTime = time; }

protected:
	void	GetPlanID(CString strDay, CString &id);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	CDateTimeCtrl m_StartDate;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_StopDate;
	CDateTimeCtrl m_StopTime;
	CString m_strComment;
};
