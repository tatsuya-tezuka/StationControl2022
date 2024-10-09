#pragma once
#include "afxpropertygridctrl.h"
#include "DetailViewDlg.h"


// ���b�Z�[�W
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



// CRdnsChkResult �_�C�A���O

class CRdnsChkResult : public CDialogEx
{
	DECLARE_DYNAMIC(CRdnsChkResult)

public:
	CRdnsChkResult(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CRdnsChkResult();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_RDNSCHK_RESULT };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	CDetailViewDlg m_DetailDlg[6];

protected:
	vector<stSendCtrlMsg>	m_RdnsChkList;
	map<int, CResultViewDlgProp*> m_ResultBtnList;
	map<CString, CMFCPropertyGridPropertyEx*> m_ItemPropList;


	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetRdnsCheckList(vector<stSendCtrlMsg>& list) { m_RdnsChkList = list; };
protected:
	void CloseLogDlg();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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


