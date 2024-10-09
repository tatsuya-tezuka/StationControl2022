
// OperationTimeLineDoc.h : COperationTimeLineDoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once


class COperationTimeLineDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	COperationTimeLineDoc();
	DECLARE_DYNCREATE(COperationTimeLineDoc)

// ����
public:

// ����
public:

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
	CString		m_strDefaultTitle;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetTitleEx(LPCTSTR lpszTitle);
	void	ResetTitleEx();
	CString	GetTitleEx() { return m_strDefaultTitle; }
protected:
	/* ------------------------------------------------------------------------------------ */

// �I�[�o�[���C�h
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ����
public:
	virtual ~COperationTimeLineDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �����n���h���[�̌����R���e���c��ݒ肷��w���p�[�֐�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};
