#pragma once
#include "afxwin.h"
#include "CidSelect.h"

using namespace timeline;

#pragma pack(1)
// �����v���p�e�B
typedef struct
{
	int		leapyear;	// ���邤�N�i���q���Ɋ֌W�Ȃ��j
	int		leapsec;	// ���邤�b�i���q���Ɋ֌W�Ȃ��j
	double	gain;		// �n���ANT�����i���q���Ɋ֌W�Ȃ��j
	int		autocalc;	// �\�z��M���x�������v�Z�I��
	double	reclevels;	// �\�z��M���x���i�Œ�l�j
	double	reclevelx;	// �\�z��M���x���i�Œ�l�j
	double	eirp;		// �q�����MEIRP
	bool	tlmflag;	// TLM�T�u�L�����A
	double	tlm;		// TLM�T�u�L�����A
	bool	cmdflag;	// CMD�T�u�L�����A
	double	cmd;		// CMD�T�u�L�����A
} stInitProperty;

// �^�p�v���p�e�B
typedef struct
{
	int		event_autocalc;	// �C�x���g�����̎����v�Z
	double	even_sunset;	// �V���ʉߊp�x����
	double	event_sunrise;	// �V���ʉߊp�x����
	double	mg_ULon;		// �}�[�W���p�xU/L ON
	double	mg_ULoff;		// �}�[�W���p�xU/L OFF
	double	mg_DL;			// �}�[�W���p�xD/L
	DWORD	of_CMD;			// U/L�I���O�̎����I�t�Z�b�g CMD�^�p�I��
	DWORD	of_RNG;			// U/L�I���O�̎����I�t�Z�b�g RNG�^�p�I��
	char	cid[30];		// �f�t�H���gCID
	int		opespace;		// �q���^�pCMD/RNG�����ϒ�
	stInitProperty	init;	// �����v���p�e�B
} stOpeProperty;
#pragma pack()

static const char* mSeparator = "#";	// ��؂蕶��
static const char* mOpePropFileName = "Property.dat";	// �t�@�C����
static const char* mOpePropFileTemp = "~~~Property.dat";	// �t�@�C����
static const char* mOpePropFileNameBackup = "Property.";	// �t�@�C����

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
//! �����_���͉\�G�f�B�b�g�R���g���[��
class CFloatEdit : public CEdit
{
	DECLARE_DYNAMIC(CFloatEdit)
	// �R���X�g���N�V����
public:
	CFloatEdit();

	// �A�g���r���[�g
public:
	UINT	m_nDecimal;
	UINT	m_nDelta;
	float	m_MinValue, m_MaxValue;
	BOOL	m_bUsePlusChar;
	BOOL	m_bUseMinusChar;
	enum { VALID = 0x00, OUT_OF_RANGE = 0x01, INVALID_CHAR = 0x02, MINUS_PLUS = 0x03 };

	// �I�y���[�V����
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

	// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFloatEdit)
	//}}AFX_VIRTUAL

	// �C���v�������e�[�V����
public:
	virtual ~CFloatEdit();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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


// COperationProperty �_�C�A���O

class COperationProperty : public CDialogEx
{
	DECLARE_DYNAMIC(COperationProperty)

public:
	COperationProperty(CWnd* pParent);   // �W���R���X�g���N�^�[
	virtual ~COperationProperty();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_OPEPROP };

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
	CWnd*			mpTimeLine;
	stOpeProperty	mOpeProp;				// �^�p�v���p�e�B
	stOpeProperty	mSaveOpeProp;			// �^�p�v���p�e�B
	CString			mOpeProp_Station;		// �Ώۉq����
	CString			mOpeProp_Equipment;		// �Ώۋǉ^�p�Fenp,sttn
	int				mOpeProp_TxtMode;		// �w�ё��M�ݔ��g�p�L�����擾

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
