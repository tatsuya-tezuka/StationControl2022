#pragma once


// CDateTimeCtrlEx

class CDateTimeCtrlEx : public CDateTimeCtrl
{
	DECLARE_DYNAMIC(CDateTimeCtrlEx)

public:
	CDateTimeCtrlEx();
	virtual ~CDateTimeCtrlEx();

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
	CWnd* m_pPostToWnd;			// �ʒm��E�B���h�E

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void SetPostToWnd(CWnd* pWnd)	{ m_pPostToWnd = pWnd; }
	CWnd* GetPostToWnd()			{ return m_pPostToWnd; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	/* ------------------------------------------------------------------------------------ */
};


