
// StationControlDoc.h : CStationControlDoc クラスのインターフェイス
//


#pragma once


class CStationControlDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CStationControlDoc();
	DECLARE_DYNCREATE(CStationControlDoc)

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	UINT	m_nViewType;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetTitleEx(LPCTSTR lpszTitle);
	void	SetTitleEx(UINT id);
	void	SetTitleEx();
	CString	GetTitleEx() { return mViewTitle[m_nViewType]; };
	void	SetViewType(UINT val)	{ m_nViewType = val; }
protected:
	/* ------------------------------------------------------------------------------------ */

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 実装
public:
	virtual ~CStationControlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 検索ハンドラーの検索コンテンツを設定するヘルパー関数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};
