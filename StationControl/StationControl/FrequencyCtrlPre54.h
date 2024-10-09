#pragma once

#include "FrequencyCtrlPre.h"

// CFrequencyCtrlPre54 ダイアログ

class CFrequencyCtrlPre54 : public CFrequencyCtrlPre
{
	DECLARE_DYNAMIC(CFrequencyCtrlPre54)

public:
	CFrequencyCtrlPre54(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFrequencyCtrlPre54();

// ダイアログ データ
	enum { IDD = IDD_DLG_FREQ_CTRL_PRESET54 };

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
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
