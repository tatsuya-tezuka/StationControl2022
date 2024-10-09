
// OperationTimeLine.h : OperationTimeLine �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��

#include "CmdInf.h"

#include "GraphException.h"

#include "../StationControl/SatelliteData.h"
#include "../StationControl/DataCollection.h"
#include "../utility/StationPipe.h"
#include "LogTraceEx.h"


enum eTimeViewType{
	eTimeViewType_OperatioTimeline,
	eTimeViewType_EquipmentSystem,
	eTimeViewType_Event,
	eTimeViewType_LimitError,
	eTimeViewType_Max,
};

static const TCHAR* mTimeViewTitle[]{
	_T("�p�X��"),
	_T("�ݔ��n��"),
	_T("�C�x���g����\��"),
	_T("���~�b�g�G���[����\��"),
	_T(""),
};

enum eTimeLineStation{
	eTimeLineStation_Usuda64,
	eTimeLineStation_Usuda642,
	eTimeLineStation_Uchinoura34,
	eTimeLineStation_Uchinoura20,
	TimeLineeStation_MAX,
};
static const COLORREF mTimeLineStationColor[]{
	RGB(195, 195, 195),
	RGB(255, 193, 164),
	RGB(255, 195, 255),
	RGB(153, 217, 255),
};

// COperationTimeLineApp:
// ���̃N���X�̎����ɂ��ẮAOperationTimeLine.cpp ���Q�Ƃ��Ă��������B
//


class COperationTimeLineApp : public CWinAppEx
{
public:
	COperationTimeLineApp();

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CMultiDocTemplate* m_pDocTemplate[eTimeViewType_Max];
	CString		m_strAppPath;						// �A�v���P�[�V�����p�X��
	CString		m_strAppInifileName;				// �A�v���P�[�V����INI�t�@�C����
	CString		m_strAppDataPath;					// �A�v���P�[�V����DATA�p�X��
	CString		m_strAppLogPath;					// �A�v���P�[�V����LOG�p�X��
	CString		m_strAppFaPanelPath;				// FAPANEL�p�X��
	CString		m_strServerName;					// �T�[�o�[����
	CString		m_strSpaceName;						// �Ώۉq����
	UINT		m_nLogLevel;
	CString		m_strSshServerName;					// SSH�T�[�o�[����
	CString		m_strSshUserName;					// SSH���[�U����
	CString		m_strSshKeyPath;					// SSH�L�[�p�X
	CString		m_strBatchPath;						// SSH�o�b�`�t�@�C���p�X 
	UINT		m_nMessageThread;					// 0:�X���b�h�Ȃ��@1:�X���b�h����

	UINT		m_nDrawUpdateInterval;

	UINT		m_nSelectStation;					// 1�ǃ��[�h���̋ǁi1:�P�c64m�� 2:�P�c54m�� 3:���V�Y34�� 4:���V�Y20�ǁj
	UINT		m_nExecuteMode;						// 0:���䃂�[�h 1:�Ď����[�h
	UINT		m_nLimitedMode;						// ���������䃂�[�h
	HWND		m_hCallWnd;							// �Ăяo���E�B���h�E�n���h��

	UINT		mServiceMode;						// �T�[�r�X���[�h
	DWORD		m_nHistID54;						// 54m �������ʔԍ�

	// �����f�[�^
	deque<stHistoryData>	mEquipmentHistoryTemp;		// �ݔ��󋵗����f�[�^�T�[�o��M�p�̃��[�N
	deque<stHistoryData>	mEquipmentHistoryData;	// �ݔ��󋵗����f�[�^�i�T�u��ʁj
	deque<stHistoryData>	mEventHistoryTemp;			// �C�x���g�����f�[�^�T�[�o��M�p�̃��[�N
	deque<stHistoryData>	mEventHistoryData;		// �C�x���g�����f�[�^�i�T�u��ʁj

	UINT		m_nHcopyType;

	deque<PIPEMONITORACK>	mMessageTemp[eStation_MAX];

	map<CString, CWnd*>	m_pDlgMap;					// �_�C�A���O�}�b�v

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString&		GetAppPath()			{ return m_strAppPath; }
	CString&		GetAppInifileName()		{ return m_strAppInifileName; }
	CString&		GetAppDataPath()		{ return m_strAppDataPath; }
	CString&		GetAppLogPath()			{ return m_strAppLogPath; }
	CString&		GetAppFaPanelPath()		{ return m_strAppFaPanelPath; }
	CString&		GetServerName()			{ return m_strServerName; }
	CString&		GetSshServerName()		{ return m_strSshServerName; }
	CString&		GetSshUserName()		{ return m_strSshUserName; }
	CString&		GetSshKeyPath()			{ return m_strSshKeyPath; }
	CString&		GetSshBatchPath()		{ return m_strBatchPath; }

	CString&		GetTargetSpaceName()				{ return m_strSpaceName; }
	void			SetTargetSpaceName(CString& val)	{ m_strSpaceName = val; }

	UINT			GetDrawUpdateInterval(bool bRead);
	UINT			GetMessageThread() { return m_nMessageThread; }

	void			SetHcopyType(UINT val)	{ m_nHcopyType = val; }
	UINT			GetHcopyType()			{ return m_nHcopyType; }

	UINT			GetSelectStation()					{ return m_nSelectStation; }
	void			SetSelectStation(UINT val)			{ m_nSelectStation = val; }

	UINT			GetExecuteMode()					{ return m_nExecuteMode; }
	UINT			GetLimitedMode()					{ return m_nLimitedMode; }

	HWND			GetCallWnd()						{ return m_hCallWnd; }

	CString	GetShareFilePath(UINT fileType, UINT nStation = eStation_MAX);

	void	OpenView(UINT val);
	void	CloseView(UINT val);
	CView*	GetView(UINT val);

	deque<stHistoryData>&	GetEquipmentHistoryData()		{ return mEquipmentHistoryData; }
	deque<stHistoryData>&	GetEventHistoryData()			{ return mEventHistoryData; }

	void	SendControl(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete = TRUE);
	void	SendControlNoUI(CString val1, CString val2, CString val3, CString val4, BOOL bWaitComplete = TRUE);

	int		GetPipeStationIndex()
	{
		if (m_nSelectStation == 0) return 0;
		if (m_nSelectStation == 1) return 3;
		if (m_nSelectStation == 2) return 1;
		if (m_nSelectStation == 3) return 2;
		return 0;
	};

	map<CString, CWnd*>	GetDlgMap()	{ return m_pDlgMap; }
	CWnd*	GetDlgMap(CString strClass)	{ return m_pDlgMap[strClass]; }
	void	SetDlgMap(CString strClass, CWnd* pWnd)
	{
		m_pDlgMap[strClass] = pWnd;
	}
	void	DeleteDlgMap(CString strClass)
	{
		m_pDlgMap[strClass] = NULL;
	}

protected:
	BOOL	CreateDocTemplate();


	// �Ď��f�[�^�������f�[�^
public:
	void	SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data);
	char*	GetObsname_t(UINT nStation, CString strWatch);
	DWORD	GetHostIpInfo();

	deque<PIPEMONITORACK>&	GetMessageTemp(int nDispStation)		{ return mMessageTemp[nDispStation]; }

	CString	UTF8_3CharsToSJis(char *pUtf8Str);
	CString ConvUtf8Unicode(std::string& src);
	void	CreateDataToHistory(PIPEMONITORACK* pbuf);
	UINT	AddHistory(UINT message, WPARAM wParam, LPARAM lParam);

	bool	IsMessageEvent(TCHAR* pmsg);
	bool	IsMessageLimit(DWORD msgno);
	void	SetLimitedMode();

#ifdef MON_UT
	map<CString, CEngValCnv::obsname_t> mon_UT;
	void SetMonUT(vector<CEngValCnv::obsname_t>& data);
#endif


protected:
	// �Ď��f�[�^�i��M�Ď��f�[�^�j
	map<UINT, map<CString, CEngValCnv::obsname_t>> m_mondatalist;




// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	CCommandLineInfoEx	m_cmdInfo;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern COperationTimeLineApp theApp;
