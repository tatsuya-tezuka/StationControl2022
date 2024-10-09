#pragma once


// CInputVlbi ダイアログ

class CInputVlbi : public CDialogEx
{
	DECLARE_DYNAMIC(CInputVlbi)

public:
	CInputVlbi(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CInputVlbi();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_STATION_VLBI };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CTime		mStartTime, mEndTime;
	CTime		mTimeLineTime;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetStartTime(CTime val) { mStartTime = val; }
	void	SetEndTime(CTime val) { mEndTime = val; }
	void	SetTimeLineTime(CTime time) { mTimeLineTime = time; }

protected:
	void	GetPlanID(CString strDay, CString &id);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	CDateTimeCtrl m_StartDate;
	CDateTimeCtrl m_StartTime;
	CDateTimeCtrl m_StopDate;
	CDateTimeCtrl m_StopTime;
};
