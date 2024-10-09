#ifndef __CCommandLineInfoEx_
#define __CCommandLineInfoEx_	TRUE

/*
	StationControl.exe /MODE:xxx /STATION:xxxxx
	/MODE:[{ 1 | 2 | 3 }]
		1=1�ǃ��[�h
		2(default)=5�ǃ��[�h
		3=10�ǃ��[�h��
	/STATION:[{ SELECT | 1 | 2 | 3 | 4 }]
		SELECT(default)=�ǑI����ʂ�\��
		1=�P�c64m��
		2=�P�c54m��
		3=���V�Y32m��
		4=���V�Y20m��
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
