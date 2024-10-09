#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "BusinessDiaryEdit.h"


// CBusinessDiaryPrint �_�C�A���O

class CBusinessDiaryPrint : public CDialogBase
{
	DECLARE_DYNAMIC(CBusinessDiaryPrint)

public:
	CBusinessDiaryPrint(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CBusinessDiaryPrint();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_DIARY_PRNTPERIOD };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	typedef struct {
		CString file;
		CString dir;
	}stPrintFile;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd*				m_pParent;
	vector<CString>		mSpaceList;
	CTime				mStartOpeTime, mEndOpeTime;
	vector<CString>		mPrintList;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void	SetParameter(vector<CString> list, CTime st, CTime et) { mSpaceList = list; mStartOpeTime = st; mEndOpeTime = et; }
	void	WriteCoverAndHeader(CString formDir, CString satname, int iYear, int iMonth, int iSDay, int iEDay, int iAllTime);
	void	WriteCoverCsv(FILE *fp, CString formDir, CString satname, int iYear, int iMonth, int iSDay, int iEDay, int iAllTime);
	void	CreatePrintList(CString formDir, CString satname, int iYear, int iMonth);

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDateTimeCtrl mStartDate;
	CDateTimeCtrl mEndDate;
	CComboBox mCbStation;
	BOOL mSendCount;
	afx_msg void OnBnClickedDiaryPrintperiodPrntBtn();
};
