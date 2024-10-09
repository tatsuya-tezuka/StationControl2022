#pragma once


// CDialogBase ダイアログ

class CDialogBase : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogBase)

public:
	CDialogBase(UINT IDD, CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CDialogBase();

protected:
	UINT	m_IDD;
	UINT	m_nSubCode;
	bool	m_modal;

public:
	UINT	GetIDD() { return m_IDD; }
	virtual void	CenterWindowEx(bool bCloseEnable = false);
	virtual BOOL	UpdateDlg();

	void LoadRect(CArchive& ar, RECT& rect)
	{
		ar >> rect.left;
		ar >> rect.top;
		ar >> rect.right;
		ar >> rect.bottom;
	}
	void LoadPoint(CArchive& ar, POINT& point) const
	{
		ar >> point.x;
		ar >> point.y;
	}
	bool Load(CArchive& ar)
	{
		WINDOWPLACEMENT wp;
		memset(&wp, 0, sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);

		try
		{
			ar >> wp.flags;
			ar >> wp.showCmd;
			ar >> m_nSubCode;
			LoadPoint(ar, wp.ptMinPosition);
			LoadPoint(ar, wp.ptMaxPosition);
			LoadRect(ar, wp.rcNormalPosition);
			SetWindowPlacement(&wp);
		}
		catch (CArchiveException* e)
		{
			e->Delete();
			return false;
		}
		return true;
	}

	void SaveRect(CArchive& ar, RECT& rect) const
	{
		ar << rect.left;
		ar << rect.top;
		ar << rect.right;
		ar << rect.bottom;
	}
	void SavePoint(CArchive& ar, POINT& point) const
	{
		ar << point.x;
		ar << point.y;
	}
	bool Save(CArchive& ar) const
	{
		WINDOWPLACEMENT wp;
		memset(&wp, 0, sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(&wp);

		try
		{
			ar << wp.flags;
//			ar << wp.showCmd;
			ar << (UINT)((m_bClosedByEndDialog == TRUE) ? SW_HIDE : SW_SHOWNORMAL);
			ar << m_nSubCode;
			SavePoint(ar, wp.ptMinPosition);
			SavePoint(ar, wp.ptMaxPosition);
			SaveRect(ar, wp.rcNormalPosition);
		}
		catch (CArchiveException* e)
		{
			e->Delete();
			return false;
		}
		return true;
	}

	void SetWindowSubCode(UINT nSubCode)
	{
		m_nSubCode = nSubCode;
	}

	UINT GetWindowSubCode()		{ return m_nSubCode;  }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	virtual INT_PTR DoModal();
};
