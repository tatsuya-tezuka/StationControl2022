#pragma once

using namespace std;

const UINT mMaxPassword = 8;
const UINT mMaxDate = 20;
const UINT mMaxLifetime = 20;
const UINT mMaxType = 4;
//=============================================================================
//	設定ファイル
//=============================================================================
const CString mManagementPath = _T("auth");
const CString mManagementSsoc = _T("ssoc");
const CString mManagementSttn = _T("sttn");
const CString mManagementTksc = _T("tksc");
const CString mDefaultPassword = _T("1qaZxsw2");
enum{
	eOpeAccount,
	eManPass,
	eLOCALusc20,
	eLOCALusc34,
	eLOCALudsc64,
	eLOCALxdsc54,
	eTKSCusc20,
	eTKSCusc34,
	eTKSCudsc64,
	eTKSCxdsc54,
	eSOSusc20,
	eSOSusc34,
	eSOSudsc64,
	eSOSxdsc54,
	eSSOCusc20,
	eSSOCusc34,
	eSSOCudsc64,
	eSSOCxdsc54,
};
static const TCHAR* mManFileName[] = {
	_T("opeaccount.ctl"),
	_T("management.ctl"),
	_T("auth_opattr.usc20.local"),
	_T("auth_opattr.usc34.local"),
	_T("auth_opattr.udsc64.local"),
	_T("auth_opattr.udsc54.local"),
	_T("auth_opattr.usc20.tksc"),
	_T("auth_opattr.usc34.tksc"),
	_T("auth_opattr.udsc64.tksc"),
	_T("auth_opattr.udsc54.tksc"),
	_T("auth_opattr.usc20.sttn"),
	_T("auth_opattr.usc34.sttn"),
	_T("auth_opattr.udsc64.sttn"),
	_T("auth_opattr.udsc54.sttn"),
	_T("auth_opattr.usc20.ssoc"),
	_T("auth_opattr.usc34.ssoc"),
	_T("auth_opattr.udsc64.ssoc"),
	_T("auth_opattr.udsc54.ssoc"),
};

enum{
	eClientSttn,		// 地上局
	eClientSsoc,		// 相模原局管制
	eClientTksc,		// 筑波局管制
};

static const CString mLocalHeader = _T("#!HEAD AUTHORITY-SCHEM LOCAL");
static const CString mLocalHeaderUpdate = _T("Updated:");
static const CString mLocalItemOpflag = _T("auth-opflag = ");
static const CString mLocalItemLatestdate = _T("auth-latestdate = ");
static const CString mLocalItemLifetime = _T("auth-lifetime = ");
static const CString mHeader = _T("#!HEAD: AUTHORITY-SCHEM MANAGEMENT FILE");
static const CString mItemOpflag = _T("auth-opflag = ");
static const CString mItemPassword = _T("auth-password = ");
static const CString mItemLatestdate = _T("auth-latestdate = ");
static const CString mItemLifetime = _T("auth-lifetime = ");

// CFileAccess コマンド ターゲット
typedef struct  {
	CString	user;
	CString	usersid;
	bool	usc20;
	bool	usc34;
	bool	udsc64;
	bool	xdsc54;
	bool	enable;
	bool	mycomputer;
	int		dummy;
} stAccount;

typedef struct  {
	CString	user;
	CString	usersid;
} stUserInfo;

typedef struct{
	bool	flag;
	char	latestdate[mMaxDate+1];
	char	lifetime[mMaxLifetime+1];
} stLocalControl;

typedef struct{
	bool	flag;
	char	password[mMaxPassword+1];
	char	latestdate[mMaxDate+1];
	char	lifetime[mMaxLifetime+1];
} stControl;

class CFileAccess
{
public:
	CFileAccess();
	virtual ~CFileAccess();

	//-----------------------------------------------------------------------------
	//	メンバ変数
	//-----------------------------------------------------------------------------
protected:
	int		mClientTerminal;
public:

	//-----------------------------------------------------------------------------
	//	メンバ関数
	//-----------------------------------------------------------------------------
protected:
	bool	ExecShell(int kind, int type);

public:
	int		GetClientTerminal();
	void	SetClientTerminal(int val);
	bool	ReadManagementPassword(CString& val);
	bool	WriteManagementPassword(CString val);
	bool	ReadOperationAccount(vector<stAccount>& val);
	bool	WriteOperationAccount(vector<stAccount> val);
	bool	ReadLocalControl(stLocalControl& val, int type);
	bool	WriteLocalControl(stLocalControl val, int type);
	bool	ReadTKSCControl(stControl& val, int type);
	bool	WriteTKSCControl(stControl val, int type);
	bool	ReadSOSControl(stControl& val, int type);
	bool	WriteSOSControl(stControl val, int type, bool bDefault=false);
	bool	ReadSSOCControl(stControl& val, int type);
	bool	WriteSSOCControl(stControl val, int type);

	//-----------------------------------------------------------------------------
};


