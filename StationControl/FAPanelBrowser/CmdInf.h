#ifndef __CCommandLineInfoEx_
#define __CCommandLineInfoEx_	TRUE

/*
	FAPanelBrowser.exe [/TITLE:xxx] /XML:xxxxx
	/TITLE:ウィンドウタイトル文字列（省略時はXMLファイル名）
	/XML:FA-PANEL用のXMLファイル
*/

class	CCommandLineInfoEx : public CCommandLineInfo
{
public:
	CCommandLineInfoEx(void);
public: // Over ride
	virtual void	ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
public: // member
	CString		mTitleString;
	CString		mPanelXmlName;
};

#endif	// __CCommandLineInfoEx_
