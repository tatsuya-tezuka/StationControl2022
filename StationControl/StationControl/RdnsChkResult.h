#pragma once
#include "afxpropertygridctrl.h"
#include "DetailViewDlg.h"


// メッセージ
enum{
	eRdnsChk_LogOpen = (WM_USER + 200),
	eRdnsChk_Result,
	eRdnsChk_End,
};


// CResultViewDlgProp
class CResultViewDlgProp : public CMFCPropertyGridProperty
{
public:
	CResultViewDlgProp(const CString& strName, const COleVariant& varValue, int type, CWnd* pParent);

protected:
	int m_Type;
	CWnd* m_pParent;

protected:
	virtual BOOL HasButton() const { return TRUE; }
	virtual void OnClickButton(CPoint point);
};

// CMFCPropertyGridPropertyEx
class CMFCPropertyGridPropertyEx : public CMFCPropertyGridProperty
{
public:
	CMFCPropertyGridPropertyEx(const CString& strName, const CString& value, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);
	virtual ~CMFCPropertyGridPropertyEx();
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	virtual void OnDrawName(CDC* pDC, CRect rect);

	void SetTextColor(COLORREF color) { m_TextColor = color; }

protected:
	COLORREF m_TextColor;

};



// CRdnsChkResult ダイアログ

class CRdnsChkResult : public CDialogEx
{
	DECLARE_DYNAMIC(CRdnsChkResult)

public:
	CRdnsChkResult(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CRdnsChkResult();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDNSCHK_RESULT };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
public:
	CDetailViewDlg m_DetailDlg[6];

protected:
	vector<stSendCtrlMsg>	m_RdnsChkList;
	map<int, CResultViewDlgProp*> m_ResultBtnList;
	map<CString, CMFCPropertyGridPropertyEx*> m_ItemPropList;


	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
public:
	void SetRdnsCheckList(vector<stSendCtrlMsg>& list) { m_RdnsChkList = list; };
protected:
	void CloseLogDlg();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CMFCPropertyGridCtrl m_PropListCtrl;
	virtual BOOL OnInitDialog();
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	afx_msg void OnBnClickedOk();
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
};


