#pragma once

#include "HistroyView.h"

// CControlSearchView �r���[

class CControlSearchView : public CHistroyView
{
	DECLARE_DYNCREATE(CControlSearchView)

protected:
	CControlSearchView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CControlSearchView();

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
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	AddHistroy();
	void	AddHistroy(stHistoryData& data, bool bDataAdd = true);
protected:
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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


