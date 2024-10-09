
// OperationTimeLineView.h : COperationTimeLineView �N���X�̃C���^�[�t�F�C�X
//

#pragma once


class COperationTimeLineView : public CView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	COperationTimeLineView();
	DECLARE_DYNCREATE(COperationTimeLineView)

// ����
public:
	COperationTimeLineDoc* GetDocument() const;

// ����
public:

// �I�[�o�[���C�h
public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~COperationTimeLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // OperationTimeLineView.cpp �̃f�o�b�O �o�[�W����
inline COperationTimeLineDoc* COperationTimeLineView::GetDocument() const
   { return reinterpret_cast<COperationTimeLineDoc*>(m_pDocument); }
#endif

