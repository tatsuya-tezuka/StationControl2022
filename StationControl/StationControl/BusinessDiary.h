#pragma once
#include "afxwin.h"
#include "atltime.h"
#include "afxcmn.h"
#include "BusinessDiaryTInfo.h"
#include "BusinessDiaryEdit.h"
#include "BusinessDiaryPrint.h"
#include "afxdtctl.h"

#define	NOT_SAT_DIR			_T("Other")

static const CString DefDayOfWeek[] = {
	_T("��"),
	_T("��"),
	_T("��"),
	_T("��"),
	_T("��"),
	_T("��"),
	_T("�y"),
};



// CBusinessDiary �_�C�A���O
class CDiaryListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CDiaryListCtrl)

public:
	CDiaryListCtrl();
	virtual ~CDiaryListCtrl();

	void	AutoSizeColumns(int col=-1);
	void	ResetColumns(vector<int>& cols);
	int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);

	int		mListNo;
	vector<int>		mHeaderSize;

public:
	DECLARE_MESSAGE_MAP()
public:
	//virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult);
};
	
// CBusinessDiary �_�C�A���O

class CBusinessDiary : public CDialogBase
{
	DECLARE_DYNAMIC(CBusinessDiary)

public:
	CBusinessDiary(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CBusinessDiary();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_DIARY };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	typedef struct
	{
		char	szDate[24];		// ����
		bool	bOn;			// ON/OFF
		char	szFreq[24];		// ���g��
		char	szSatName[24];	// �q����
		char	szColim[24];	// COLIM
	} stRfInfo;					// RF���

	typedef struct
	{
		char	szaSatName[24];	// �q����
		time_t	iaTime[3];		// ���v����
		int		iaCnt[3];		// ���v��
	} stSatRfInfo;				// �q����RF���							

	const int	mDelSpan = 400;	// �ۑ�����

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	vector<CString>		mSpaceList;
	CString				mStartOpeTime, mEndOpeTime;
	vector<stDiary>		mDialyDataList;
	vector<stDiary>		mDialyDataListTmp;
	CString 			mStrRecCount;

	BOOL				mbEdit;

public:
	char				m_szMakeDate[64];
	char				m_szNowDate[64];
	vector<stRfInfo>			m_RfInfoList;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	virtual BOOL	UpdateDlg();
	void	CreateDialyList();
	void	AddSpaceName(CString name);
	void	JumpDate();
	void	DisplayList(CString name);
	void	SelectListRow(int no);
	CString GetRecCountStr() { return mStrRecCount; }
	void	SetRecCountStr(CString str) { mStrRecCount = str; }
	vector<stDiary>& GetDialyDataList() { return mDialyDataList; }

	BOOL Shell_GetRadiolog();
	BOOL Shell_RegistRadiolog();

	void	SetEditFlg(BOOL flg) { mbEdit = flg; }

	int		GetIndexEqSatDBData(CString str);

	int		GetDiaryListIndex(int no);

	int		GetTimeConf(char *pszDate);
	void	MakeReport(vector<stDiary>& dialyDataList);
	void	SetRfInfo(char *pszSatName, char *pszColim, char *pszFreq, char *pszOnDate, char *pszOffDate, vector<stSatRfInfo>& satRfInfoList);
	bool	ReadRFInfoFile();
	void	ExchgNoneData2Long(stMember& src, stMember& dst);
	void	ExchgNoneData2Short(stMember& src, stMember& dst);

protected:
	BOOL	IsPass();
	BOOL	CheckRFInfoFile();
	BOOL	CheckFormatRFFile();
	void	WriteDiaryData();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_SatelliteCmb;
	CTime m_DayTime;
	CDiaryListCtrl m_DiaryList;
	afx_msg void OnBnClickedDiaryEditBtn();
	afx_msg void OnBnClickedDiaryPrntBtn();
	afx_msg void OnBnClickedDiaryCsvBtn();
	afx_msg void OnBnClickedDiarySinfoBtn();
	virtual BOOL OnInitDialog();
	CDiaryListCtrl m_DiaryList2;
	CDateTimeCtrl mDate;
	afx_msg void OnDtnDatetimechangeDiaryStltDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeDiaryStltCmb();
	afx_msg void OnMenuCloseClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnLvnItemchangedDiaryList(NMHDR *pNMHDR, LRESULT *pResult);
};
