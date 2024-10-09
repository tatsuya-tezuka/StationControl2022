#pragma once

#include <vector>
#include "GridButton.h"

using namespace std;

// CEqStatusGridView �r���[

class CEqStatusGridView : public CScrollView
{
	DECLARE_DYNCREATE(CEqStatusGridView)

protected:
	CEqStatusGridView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CEqStatusGridView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
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
	vector<CGridButton*>	mButtonList;
	vector<CString>			mStatusTagList;
	UINT					mStation;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	void	UpdateStatus();

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual void OnInitialUpdate();     // �\�z�� 1 ���

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPushButton(UINT nID);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


