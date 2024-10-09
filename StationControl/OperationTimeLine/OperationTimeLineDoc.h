
// OperationTimeLineDoc.h : COperationTimeLineDoc クラスのインターフェイス
//


#pragma once


class COperationTimeLineDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	COperationTimeLineDoc();
	DECLARE_DYNCREATE(COperationTimeLineDoc)

// 属性
public:

// 操作
public:

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
	CString		m_strDefaultTitle;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetTitleEx(LPCTSTR lpszTitle);
	void	ResetTitleEx();
	CString	GetTitleEx() { return m_strDefaultTitle; }
protected:
	/* ------------------------------------------------------------------------------------ */

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
	virtual ~COperationTimeLineDoc();
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
