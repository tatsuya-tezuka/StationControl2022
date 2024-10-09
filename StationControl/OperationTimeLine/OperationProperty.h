#pragma once
#include "afxwin.h"
#include "CidSelect.h"

using namespace timeline;

#pragma pack(1)
// 初期プロパティ
typedef struct
{
	int		leapyear;	// うるう年（★衛星に関係ない）
	int		leapsec;	// うるう秒（★衛星に関係ない）
	double	gain;		// 地上局ANT利得（★衛星に関係ない）
	int		autocalc;	// 予想受信レベル自動計算選択
	double	reclevels;	// 予想受信レベル（固定値）
	double	reclevelx;	// 予想受信レベル（固定値）
	double	eirp;		// 衛星送信EIRP
	bool	tlmflag;	// TLMサブキャリア
	double	tlm;		// TLMサブキャリア
	bool	cmdflag;	// CMDサブキャリア
	double	cmd;		// CMDサブキャリア
} stInitProperty;

// 運用プロパティ
typedef struct
{
	int		event_autocalc;	// イベント日時の自動計算
	double	even_sunset;	// 天頂通過角度入り
	double	event_sunrise;	// 天頂通過角度明け
	double	mg_ULon;		// マージン角度U/L ON
	double	mg_ULoff;		// マージン角度U/L OFF
	double	mg_DL;			// マージン角度D/L
	DWORD	of_CMD;			// U/L終了前の時刻オフセット CMD運用終了
	DWORD	of_RNG;			// U/L終了前の時刻オフセット RNG運用終了
	char	cid[30];		// デフォルトCID
	int		opespace;		// 衛星運用CMD/RNG同時変調
	stInitProperty	init;	// 初期プロパティ
} stOpeProperty;
#pragma pack()

static const char* mSeparator = "#";	// 区切り文字
static const char* mOpePropFileName = "Property.dat";	// ファイル名
static const char* mOpePropFileTemp = "~~~Property.dat";	// ファイル名
static const char* mOpePropFileNameBackup = "Property.";	// ファイル名

enum eIdentifier{
	eLEAP_YEAR,
	eLEAP_SEC,
	eGAIN,
	eEVENT_CALC,
	eZENITH_IN,
	eZENITH_OUT,
	eMARGIN_UP_ON,
	eMARGIN_UP_OFF,
	eMARGIN_DOWN,
	eOFFSET_CMD_END,
	eOFFSET_RNG_END,
	eDEFAULT_CID,
	eSAME_TIME_MOD,
	eEXPECT_LEVEL,
	eEXPECT_LEVEL_FIXED_S,
	eEXPECT_LEVEL_FIXED_X,
	eEIRP,
	eSUBSARR_TLM,
	eSUBSARR_CMD,
	eIdentifierMax,
};
static const CStringA mIdentifier[] = {
	"LEAP_YEAR",
	"LEAP_SEC",
	"GAIN",
	"EVENT_CALC",
	"ZENITH_IN",
	"ZENITH_OUT",
	"MARGIN_UP_ON",
	"MARGIN_UP_OFF",
	"MARGIN_DOWN",
	"OFFSET_CMD_END",
	"OFFSET_RNG_END",
	"DEFAULT_CID",
	"SAME_TIME_MOD",
	"EXPECT_LEVEL",
	"EXPECT_LEVEL_FIXED_S",
	"EXPECT_LEVEL_FIXED_X",
	"EIRP",
	"SUBSARR_TLM",
	"SUBSARR_CMD",
};

static const CString mStatusString[] = {
	_T("OFF"),
	_T("ON"),
};

static const CString mAutoCalString[] = {
	_T("FIXED"),
	_T("AUTO"),
};

static const CString mLeapSecString[] = {
	_T("OFF"),
	_T("1"),
	_T("4"),
	_T("7"),
	_T("10"),
};

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <string>
#include <typeinfo>
using namespace std;


//=============================================================================
// CFloatEdit
//=============================================================================
//! 小数点入力可能エディットコントロール
class CFloatEdit : public CEdit
{
	DECLARE_DYNAMIC(CFloatEdit)
	// コンストラクション
public:
	CFloatEdit();

	// アトリビュート
public:
	UINT	m_nDecimal;
	UINT	m_nDelta;
	float	m_MinValue, m_MaxValue;
	BOOL	m_bUsePlusChar;
	BOOL	m_bUseMinusChar;
	enum { VALID = 0x00, OUT_OF_RANGE = 0x01, INVALID_CHAR = 0x02, MINUS_PLUS = 0x03 };

	// オペレーション
public:
	void	SetDecimal(UINT len);
	UINT	GetDecimal()	{ return m_nDecimal; };
	void	SetDelta(UINT val)	{ m_nDelta = val; }
	BOOL	CheckDecimal();
	BOOL	CheckDelta();
	void	SetMinMax(float min, float max) { m_MinValue = min; m_MaxValue = max; }
	void	GetMinMax(float& min, float& max) { min = m_MinValue; max = m_MaxValue; }
	float	GetValue() const;
	BOOL	SetValue(float val);
	void	SetUsePlusMinusChar(BOOL bUsePlusCar = TRUE, BOOL bUseMinusChar = TRUE)	{
		m_bUsePlusChar = bUsePlusCar;
		m_bUseMinusChar = bUseMinusChar;
	}

	// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFloatEdit)
	//}}AFX_VIRTUAL

	// インプリメンテーション
public:
	virtual ~CFloatEdit();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CFloatEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


// COperationProperty ダイアログ

class COperationProperty : public CDialogEx
{
	DECLARE_DYNAMIC(COperationProperty)

public:
	COperationProperty(CWnd* pParent);   // 標準コンストラクター
	virtual ~COperationProperty();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OPEPROP };

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
	CWnd*			mpTimeLine;
	stOpeProperty	mOpeProp;				// 運用プロパティ
	stOpeProperty	mSaveOpeProp;			// 運用プロパティ
	CString			mOpeProp_Station;		// 対象衛星名
	CString			mOpeProp_Equipment;		// 対象局運用：enp,sttn
	int				mOpeProp_TxtMode;		// Ｘ帯送信設備使用有無情報取得

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetOpeProp_Station(CString val)		{ mOpeProp_Station = val; }
	void	SetOpeProp_Equipment(CString val)	{ mOpeProp_Equipment = val; }
	void	SetOpeProp_TxtMode(int val) { mOpeProp_TxtMode = val; }
	bool	ReadOpeProp() { return _ReadOpeProp(); }
protected:
	void	_InitializeControl();
	void	_UpdateControl();
	void	_SetData(stOpeProperty& data);
	bool	_CheckData();
	bool	_ReadOpeProp();
	bool	_ReadCommon(stOpeProperty* pdata, CString& buf);
	bool	_ReadSpace(stOpeProperty* pdata, CString& buf);
	int		_GetIdentifier(CString& buf);
	int		_GetLeapSecNo(CString& buf);
	CString	_GetLeapSecString(int val);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPropreset();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_nOptionEvent;
//	DWORD m_nPropEventSunset;
//	DWORD m_nPropEventSunrise;
//	DWORD m_nPropMarginUlon;
//	DWORD m_nPropMarginUloff;
//	DWORD m_nPropMarginDl;
	CString m_strPropEventSunset;
	CString m_strPropEventSunrise;
	CString m_strPropMarginUlon;
	CString m_strPropMarginUloff;
	CString m_strPropMarginDl;
	DWORD m_nPropUlCmd;
	DWORD m_nPropUlRng;
	int m_nSmdRng;
//	CEdit m_edPropEventSunset;
//	CEdit m_edPropEventSunrise;
//	CEdit m_edPropMarginUlon;
//	CEdit m_edPropMarginUloff;
//	CEdit m_edPropMarginDl;
	CFloatEdit m_edPropEventSunset;
	CFloatEdit m_edPropEventSunrise;
	CFloatEdit m_edPropMarginUlon;
	CFloatEdit m_edPropMarginUloff;
	CFloatEdit m_edPropMarginDl;
	CEdit m_edPropUlCmd;
	CEdit m_edPropUlRng;
	int m_nLeapYear;
	int m_nLeapSec;
	int m_nAutoCalc;
	CFloatEdit m_edEirp;
	CFloatEdit m_edExpect;
	CString m_strEirp;
	CString m_strExpect;
	afx_msg void OnBnClickedButtonPropcid();
	afx_msg void OnCbnSelchangeComboReclevel();
};
