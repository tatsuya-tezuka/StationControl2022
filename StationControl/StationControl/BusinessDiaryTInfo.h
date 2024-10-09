#pragma once
#include "afxdtctl.h"
#include "afxwin.h"

#include "BusinessDiaryEdit.h"
#include "atltime.h"
#include "ATLComTime.h"

enum eRfFormatType{
	eRfFormatType_UDSC = 0,
	eRfFormatType_USC,
	eRfFormatType_MAX
};

// CBusinessDiaryTInfo �_�C�A���O

class CBusinessDiaryTInfo : public CDialogBase
{
	DECLARE_DYNAMIC(CBusinessDiaryTInfo)

public:
	CBusinessDiaryTInfo(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CBusinessDiaryTInfo();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_DIARY_TINFPERIOD };

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
	vector<CString>		mSpaceList;
	CTime				mStartOpeTime, mEndOpeTime;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetParameter(vector<CString> list, CTime st, CTime et) { mSpaceList = list; mStartOpeTime = st; mEndOpeTime = et; }

protected:
	void GetStartEndTimeinFile();
	void AddSpaceName(CString name);
	int GetRfFormatType();
	int GetRfSatellitePos();
	CString GetSatelliteName(CString& cbuf);
	BOOL CheckFormatRFInfoFile();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDiaryTinfperiodOutBtn();
	CDateTimeCtrl mStartDate;
	CDateTimeCtrl mEndDate;
	CComboBox mCbStation;
	CTime m_StartTime;
	CTime m_EndTime;
	CDateTimeCtrl mStartTime;
	CDateTimeCtrl mEndTime;
};
