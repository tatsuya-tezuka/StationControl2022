#pragma once

#include "DataCollection.h"
#include "GraphWindow.h"
#include "SpectrumGraphView.h"

// CSummaryGraphCtrl

class CSummaryGraphCtrl : public CWnd
{
	DECLARE_DYNAMIC(CSummaryGraphCtrl)

public:
	CSummaryGraphCtrl();
	virtual ~CSummaryGraphCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
protected:
	CWnd*		mParent;
	UINT		mID;								// �r���[ID
	UINT		mStationIndex;						// �ǃC���f�b�N�X
	BOOL		mInitOver;
	CRect		mRect;
	CGraphWindow	m_Graph;						// �O���t�r���[
	CSpectrumGraphView*	m_pSpectrumView;			// �X�y�N�g�����r���[
	eSummaryWndSetType	m_nWndTypeSet;				// �E�B���h�E�^�C�v�Z�b�g
	int			m_nListMax;							// ���ڐ��ő�

	time_t				m_MostOldTime1st;			// ����̂����Ƃ��Â�����
	time_t				m_MostOldTime;				// �����Ƃ��Â�����
	time_t				m_MostNewTime;				// �����Ƃ��V��������

public:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	time_t	GetMostOldTime()	{ return m_MostOldTime; }
	time_t	GetMostNewTime()	{ return m_MostNewTime; }
	void UpdateGraph(BOOL bAll = FALSE);

	// ���C�A�E�g�ǂݍ��݁��ۑ��i�ݒ���j
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);

protected:
	BOOL	RegisterClass();

	void SetWndTypeSet(eSummaryWndSetType type)	{ m_nWndTypeSet = type; }
	eSummaryWndSetType GetWndTypeSet()			{ return m_nWndTypeSet; }
	void InitTrendWnd(eSummaryWndSetType type);
	void InitSpectrumWnd(UINT nViewID);
	BOOL GetViewNameList(vector<CString>& listViewName);
	void AddViewParam();
	void SetGraphTime(int nStation);

public:
	BOOL	Create(CWnd *pParent, CRect rect, UINT nID, UINT nStaionID, UINT nParentID = 0);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/* ------------------------------------------------------------------------------------ */
};


