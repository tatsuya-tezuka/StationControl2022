#pragma once

#include "GraphWindow.h"
#include "StationControlDoc.h"

// CTrendGraphView �r���[

class CTrendGraphView : public CView
{
	DECLARE_DYNCREATE(CTrendGraphView)

public:
	CTrendGraphView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CTrendGraphView();

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
	CGraphWindow		m_Graph[eTrendWndType_Num];	// �e�O���t
	eTrendWndSetType	m_nWndTypeSet;				// �E�B���h�E�^�C�v�Z�b�g
	BOOL				m_bIsInitialized;			// �������ς�
	time_t				m_MostOldTime1st;			// ����̂����Ƃ��Â�����
	time_t				m_MostOldTime;				// �����Ƃ��Â�����
	time_t				m_MostNewTime;				// �����Ƃ��V��������

	BOOL				m_bInit;					// ������

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	// ����
public:
	CStationControlDoc* GetDocument() const;
	void SetWndTypeSet(eTrendWndSetType type)	{ m_nWndTypeSet = type; }
	eTrendWndSetType GetWndTypeSet()			{ return m_nWndTypeSet; }
	void GetGraphWndStartEnd(int& nStart, int& nEnd, BOOL& bEnaRR);
	void InitTrendWnd(eTrendWndSetType type);
	BOOL GetViewNameList(vector<CString>& listViewName);
	BOOL GetViewAllNameList(vector<CString>& listViewName);
	void AddViewParam();
	UINT ExchgPropertiesID2ViewID(UINT nPropertiesID);
	time_t	GetMostOldTime()	{ return m_MostOldTime; }
	time_t	GetMostNewTime()	{ return m_MostNewTime; }

	// ���C�A�E�g�ǂݍ��݁��ۑ��i�ݒ���j
	bool	Load(CArchive& ar);
	bool	Save(CArchive& ar);

protected:
	void UpdateGraph(BOOL bAll = FALSE);
	void SetGraphTime(int nStation);

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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	/* ------------------------------------------------------------------------------------ */
};

#ifndef _DEBUG  // StationControlView.cpp �̃f�o�b�O �o�[�W����
inline CStationControlDoc* CTrendGraphView::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
