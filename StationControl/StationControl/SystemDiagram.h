#pragma once


// CSystemDiagram �r���[

class CSystemDiagram : public CView
{
	DECLARE_DYNCREATE(CSystemDiagram)

protected:
	CSystemDiagram();           // ���I�����Ŏg�p����� protected �R���X�g���N�^�[
	virtual ~CSystemDiagram();

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
protected:
	/* ------------------------------------------------------------------------------------ */

	// ����
public:
	CStationControlDoc* GetDocument() const;

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
};

#ifndef _DEBUG  // StationControlView.cpp �̃f�o�b�O �o�[�W����
inline CStationControlDoc* CSystemDiagram::GetDocument() const
{
	return reinterpret_cast<CStationControlDoc*>(m_pDocument);
}
#endif
