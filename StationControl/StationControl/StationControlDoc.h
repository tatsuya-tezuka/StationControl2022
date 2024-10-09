
// StationControlDoc.h : CStationControlDoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once


class CStationControlDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CStationControlDoc();
	DECLARE_DYNCREATE(CStationControlDoc)

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
	UINT	m_nViewType;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetTitleEx(LPCTSTR lpszTitle);
	void	SetTitleEx(UINT id);
	void	SetTitleEx();
	CString	GetTitleEx() { return mViewTitle[m_nViewType]; };
	void	SetViewType(UINT val)	{ m_nViewType = val; }
protected:
	/* ------------------------------------------------------------------------------------ */

// ����
public:

// ����
public:

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
	virtual ~CStationControlDoc();
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
