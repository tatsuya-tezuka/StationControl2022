#ifndef __CCommandLineInfoEx_
#define __CCommandLineInfoEx_	TRUE

class	CCommandLineInfoEx : public CCommandLineInfo
{
public:
	CCommandLineInfoEx(void);
public: // Over ride
	virtual void	ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
public: // member
	bool		mDebug;
	CString		mAlarmFile;
};

#endif	// __CCommandLineInfoEx_
