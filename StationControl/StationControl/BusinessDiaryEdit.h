#pragma once
#include "atltime.h"
#include "afxwin.h"
#include "afxdtctl.h"

typedef struct  {
	CString		weather;
	CString		capa1;
	CString		name1;
	CString		capa2;
	CString		name2;
	CString		capa3;
	CString		name3;
	CString		station;
	CString		time;
	CString		count;
} stMember;

typedef struct  {
	CString		no;
	CString		date;
	stMember	data[3];
	CString		info1;
	CString		info2;
	CString		info3;
} stDiary;

static const CString mAllText = _T("ALL");

// CBusinessDiaryEdit �_�C�A���O

class CBusinessDiaryEdit : public CDialogBase
{
	DECLARE_DYNAMIC(CBusinessDiaryEdit)

public:
	CBusinessDiaryEdit(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CBusinessDiaryEdit();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_DIARY_EDIT };

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
	CWnd* m_pParent;

	vector<CString>	mDataList;
	UINT			mPos;
	bool			mUpdate;
	bool			mWriteData;
	CString			mRecCount;

	CString			mSatellite;
	int				mListPos;

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void	SetSatellite(CString str) { mSatellite = str; };
	void	SetListPos(int pos) { mListPos = pos; };

protected:
	void	ReadDiaryData();
	void	WriteDiaryData();
	void	DisplayData();
	void	CreateControl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �^�p��
	CTime m_OperationDate;
	// �^�p��-BACK�ԍ�
	CString m_strOperationBackNo;
	// �^�p��-NEXT�ԍ�
	CString m_strOperationNextNo;
	// 1�A�V��
	CComboBoxEx2 m_Weather1Cmb;
	// 1�A�����Ǐ]���Ҏ��i
	CComboBoxEx2 m_EmpCapa1Cmb;
	// 1�A�����Ǐ]���Ҏ���
	CComboBoxEx2 m_EmpName1Cmb;
	// 1�A�����Ǒ���Ҏ��i1
	CComboBoxEx2 m_OpeCapa11Cmb;
	// 1�A�����Ǒ���Ҏ���1
	CComboBoxEx2 m_OpeName11Cmb;
	// 1�A�����Ǒ���Ҏ��i2
	CComboBoxEx2 m_OpeCapa12Cmb;
	// 1�A�����Ǒ���Ҏ���2
	CComboBoxEx2 m_OpeName12Cmb;
	// 1�A�����
	CString m_strStation1;
	// 1�A���ʐM���ԁA����
	CString m_strComTimeH1;
	// 1�A���ʐM���ԁA��
	CString m_strComTimeM1;
	// 1�A�ʐM��
	CString m_strComCount1;
	// 2�A�V��
	CComboBoxEx2 m_Weather2Cmb;
	// 2�A�����Ǐ]���Ҏ��i
	CComboBoxEx2 m_EmpCapa2Cmb;
	// 2�A�����Ǐ]���Ҏ���
	CComboBoxEx2 m_EmpName2Cmb;
	// 2�A�����Ǒ���Ҏ��i1
	CComboBoxEx2 m_OpeCapa21Cmb;
	// 2�A�����Ǒ���Ҏ���1
	CComboBoxEx2 m_OpeName21Cmb;
	// 2�A�����Ǒ���Ҏ��i2
	CComboBoxEx2 m_OpeCapa22Cmb;
	// 2�A�����Ǒ���Ҏ���2
	CComboBoxEx2 m_OpeName22Cmb;
	// 2�A�����
	CString m_strStation2;
	// 2�A���ʐM���ԁA����
	CString m_strComTimeH2;
	// 2�A���ʐM���ԁA��
	CString m_strComTimeM2;
	// 2�A�ʐM��
	CString m_strComCount2;
	// 3�A�V��
	CComboBoxEx2 m_Weather3Cmb;
	// 3�A�����Ǐ]���Ҏ��i
	CComboBoxEx2 m_EmpCapa3Cmb;
	// 3�A�����Ǐ]���Ҏ���
	CComboBoxEx2 m_EmpName3Cmb;
	// 3�A�����Ǒ���Ҏ��i1
	CComboBoxEx2 m_OpeCapa31Cmb;
	// 3�A�����Ǒ���Ҏ���1
	CComboBoxEx2 m_OpeName31Cmb;
	// 3�A�����Ǒ���Ҏ��i2
	CComboBoxEx2 m_OpeCapa32Cmb;
	// 3�A�����Ǒ���Ҏ���2
	CComboBoxEx2 m_OpeName32Cmb;
	// 3�A�����
	CString m_strStation3;
	// 3�A���ʐM���ԁA����
	CString m_strComTimeH3;
	// 3�A���ʐM���ԁA��
	CString m_strComTimeM3;
	// 3�A�ʐM��
	CString m_strComCount3;
	// �̏�ƌ����A���u
	CString m_strCause;
	// �������@�A�o�ߋy�ь��ʓ�
	CString m_strResult;
	// ���L�����A�Q�l���
	CString m_strNotices;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDiaryEditNoticesEntryBtn();
	afx_msg void OnBnClickedDiaryEditBackBtn();
	afx_msg void OnBnClickedDiaryEditNextBtn();
	CDateTimeCtrl mDate;
	afx_msg void OnDtnDatetimechangeDiaryEditOpedayDp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeDiaryEdit1WeatherCmb();
	afx_msg void OnCbnSelchangeDiaryEdit2WeatherCmb();
	afx_msg void OnCbnSelchangeDiaryEdit3WeatherCmb();
	afx_msg void OnCbnSelchangeDiaryEdit1CapaCmb();
	afx_msg void OnCbnSelchangeDiaryEdit1NameCmb();
	afx_msg void OnCbnSelchangeDiaryEdit1Capa21Cmb();
	afx_msg void OnCbnSelchangeDiaryEdit1Name21Cmb();
	afx_msg void OnCbnSelchangeDiaryEdit1Capa22Cmb();
	afx_msg void OnCbnSelchangeDiaryEdit1Name22Cmb();
	afx_msg void OnEnChangeDiaryEdit1StationEdit();
	afx_msg void OnEnChangeDiaryEdit1HourEdit();
	afx_msg void OnEnChangeDiaryEdit1MinuteEdit();
	afx_msg void OnEnChangeDiaryEdit1CountEdit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeDiaryEditCauseEdit();
	afx_msg void OnEnChangeDiaryEditResultEdit();
	afx_msg void OnEnChangeDiaryEditNoticesEdit();
	afx_msg void OnMenuCloseClose();
	afx_msg void OnCbnEditchangeDiaryEdit1WeatherCmb();
	afx_msg void OnCbnKillfocusDiaryEdit1WeatherCmb();
	afx_msg void OnCbnEditchangeDiaryEdit1CapaCmb();
	afx_msg void OnCbnKillfocusDiaryEdit1CapaCmb();
	afx_msg void OnCbnEditchangeDiaryEdit1NameCmb();
	afx_msg void OnCbnKillfocusDiaryEdit1NameCmb();
	afx_msg void OnCbnEditchangeDiaryEdit1Capa21Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit1Capa21Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit1Name21Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit1Name21Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit1Capa22Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit1Capa22Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit1Name22Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit1Name22Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit2WeatherCmb();
	afx_msg void OnCbnKillfocusDiaryEdit2WeatherCmb();
	afx_msg void OnCbnEditchangeDiaryEdit2CapaCmb();
	afx_msg void OnCbnKillfocusDiaryEdit2CapaCmb();
	afx_msg void OnCbnEditchangeDiaryEdit2NameCmb();
	afx_msg void OnCbnKillfocusDiaryEdit2NameCmb();
	afx_msg void OnCbnEditchangeDiaryEdit2Capa21Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit2Capa21Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit2Name21Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit2Name21Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit2Capa22Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit2Capa22Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit2Name22Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit2Name22Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit3WeatherCmb();
	afx_msg void OnCbnKillfocusDiaryEdit3WeatherCmb();
	afx_msg void OnCbnEditchangeDiaryEdit3CapaCmb();
	afx_msg void OnCbnKillfocusDiaryEdit3CapaCmb();
	afx_msg void OnCbnEditchangeDiaryEdit3NameCmb();
	afx_msg void OnCbnKillfocusDiaryEdit3NameCmb();
	afx_msg void OnCbnEditchangeDiaryEdit3Capa21Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit3Capa21Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit3Name21Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit3Name21Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit3Capa22Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit3Capa22Cmb();
	afx_msg void OnCbnEditchangeDiaryEdit3Name22Cmb();
	afx_msg void OnCbnKillfocusDiaryEdit3Name22Cmb();
	CEdit m_NextEditCtrl;
};
