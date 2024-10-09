
// FAPanelBrowserView.h : CFAPanelBrowserView �N���X�̃C���^�[�t�F�C�X
//

#pragma once


class CFAPanelBrowserView : public CView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CFAPanelBrowserView();
	DECLARE_DYNCREATE(CFAPanelBrowserView)

// ����
public:
	CFAPanelBrowserDoc* GetDocument() const;

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
	virtual ~CFAPanelBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // FAPanelBrowserView.cpp �̃f�o�b�O �o�[�W����
inline CFAPanelBrowserDoc* CFAPanelBrowserView::GetDocument() const
   { return reinterpret_cast<CFAPanelBrowserDoc*>(m_pDocument); }
#endif

