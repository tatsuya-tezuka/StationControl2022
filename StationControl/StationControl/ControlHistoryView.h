#pragma once

#include "HistroyView.h"

// CControlHistoryView �r���[

class CControlHistoryView : public CHistroyView
{
	DECLARE_DYNCREATE(CControlHistoryView)

protected:
	CControlHistoryView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CControlHistoryView();

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
	BOOL bLost;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	virtual void	AddHistroy();
	virtual void	AddHistroy(stHistoryData& data, bool bDataAdd = true);
protected:
	/* ------------------------------------------------------------------------------------ */

	// ����
public:

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
};
