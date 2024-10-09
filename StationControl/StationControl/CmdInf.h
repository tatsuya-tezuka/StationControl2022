#ifndef __CCommandLineInfoEx_
#define __CCommandLineInfoEx_	TRUE

/*
	StationControl.exe /MODE:xxx /STATION:xxxxx
	/MODE:[{ 1 | 2 | 3 }]
		1=1局モード
		2(default)=5局モード
		3=10局モード※
	/STATION:[{ SELECT | 1 | 2 | 3 | 4 }]
		SELECT(default)=局選択画面を表示
		1=臼田64m局
		2=臼田54m局
		3=内之浦32m局
		4=内之浦20m局
*/

class	CCommandLineInfoEx : public CCommandLineInfo
{
public:
	CCommandLineInfoEx(void);
public: // Over ride
	virtual void	ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
public: // member
	UINT		mStationMode;
	UINT		mStation;
	UINT		mIgnore;
	UINT		mLBStation;
	CString		mLBCtrl;
};

#endif	// __CCommandLineInfoEx_
