#pragma once


// CComboBoxEx2

class CComboBoxEx2 : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxEx2)

public:
	CComboBoxEx2();
	virtual ~CComboBoxEx2();

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

	/* ------------------------------------------------------------------------------------ */
	/*	メンバ関数
	/* ------------------------------------------------------------------------------------ */
public:
	int SelectStringEx(int nStartAfter, LPCTSTR lpszString);

protected:
	DECLARE_MESSAGE_MAP()
};


