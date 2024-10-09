#ifndef __CCommandLineInfoEx_
#define __CCommandLineInfoEx_	TRUE

/*
	OperationTimeLine.exe /HWND:xxxxxxxx [/DEBUG]
	/HWND:�Ăяo�����E�B���h�E�n���h��
	/DEBUG:�f�o�b�O���[�h
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
