#ifndef __CCommandLineInfoEx_
#define __CCommandLineInfoEx_	TRUE

/*
	FAPanelBrowser.exe [/TITLE:xxx] /XML:xxxxx
	/TITLE:�E�B���h�E�^�C�g��������i�ȗ�����XML�t�@�C�����j
	/XML:FA-PANEL�p��XML�t�@�C��
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
