#pragma once

#include "HistoryList.h"
#include "AlarmFilterDlg.h"

// CAlarmListView �r���[

enum eAlarmFileTye{
	eAlarmFileType_Station,
	eAlarmFileType_Equipment,
	eAlarmFileType_ImStatus,
	eAlarmFileType_Status,
};

class CAlarmListView : public CView
{
	DECLARE_DYNCREATE(CAlarmListView)

protected:
	CAlarmListView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CAlarmListView();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static CAlarmListView*	Instance();
protected:
	static CAlarmListView*	m_pInstance;
	static CCriticalSection	mSection;
	UINT				m_nTabItem;
	CHistoryList		m_History;
	CMFCTabCtrlEx		m_TabBar;
	CAlarmFilterDlg		m_AlarmFilterDlg;
	stFilterData		mFilterData;
	deque<stHistoryData>	mHistoryData;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CStationControlDoc* GetDocument() const;
	static void CALLBACK messageProc(stFilterData& param, UINT type);
	void	CloseChildWindow(MSG* pMsg)
	{
		m_TabBar.CloseChildWindow(pMsg);
	}
	void	SetDisplayFilter(stFilterData& data);
	bool	IsDisplay(int nFilterType, DWORD val, UINT nStation);
	virtual void	AddHistroy(){};
	virtual void	AddHistroy(stHistoryData& data, bool bDataAdd = true){};
	virtual void	ClearHistroy(){ mHistoryData.clear(); };
protected:
	virtual void	CreateAlarmList();
	virtual void	AutoSizeColumns(int col = -1);
	virtual int		AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0);
	/* ------------------------------------------------------------------------------------ */
	bool	_GetStation(stHistoryData& hdata, CString cbuf);
	bool	_GetEquipment(stHistoryData& hdata, CString cbuf);
	bool	_GetImStatus(stHistoryData& hdata, CString cbuf);
	bool	_GetStatus(stHistoryData& hdata, CString cbuf);
	/* ------------------------------------------------------------------------------------ */

public:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // StationControlView.cpp �̃f�o�b�O �o�[�W����
inline CStationControlDoc* CAlarmListView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
