#ifndef __CCommandLineInfoEx_
#define __CCommandLineInfoEx_	TRUE

/*
	OperationTimeLine.exe /HWND:xxxxxxxx [/DEBUG]
	/HWND:呼び出し元ウィンドウハンドル
	/DEBUG:デバッグモード
*/

class	CCommandLineInfoEx : public CCommandLineInfo
{
public:
	CCommandLineInfoEx(void);
public: // Over ride
	virtual void	ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
public: // member
	HWND		mCallWindow;
	UINT		mSelectStation;
	UINT		mExecuteMode;
	bool		mDebug;

	unsigned long	HexToDec(CString strHex);
};

#endif	// __CCommandLineInfoEx_
