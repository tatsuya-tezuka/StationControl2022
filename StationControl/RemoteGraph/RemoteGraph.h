// ���� MFC �T���v�� �\�[�X �R�[�h�ł́AMFC Microsoft Office Fluent ���[�U�[ �C���^�[�t�F�C�X 
// ("Fluent UI") �̎g�p���@�������܂��B���̃R�[�h�́AMFC C++ ���C�u���� �\�t�g�E�F�A�� 
// ��������Ă��� Microsoft Foundation Class ���t�@�����X����ъ֘A�d�q�h�L�������g��
// �⊮���邽�߂̎Q�l�����Ƃ��Ē񋟂���܂��B
// Fluent UI �𕡐��A�g�p�A�܂��͔z�z���邽�߂̃��C�Z���X�����͌ʂɗp�ӂ���Ă��܂��B
// Fluent UI ���C�Z���X �v���O�����̏ڍׂɂ��ẮAWeb �T�C�g
// http://go.microsoft.com/fwlink/?LinkId=238214 ���Q�Ƃ��Ă��������B
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// RemoteGraph.h : RemoteGraph �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��

#include "const.h"
#include "PropertiesWnd.h"
#include "SatelliteData.h"
#include "LogTraceEx.h"
#include "GraphException.h"
#include "DataCollection.h"
#include "CmdInf.h"

// CRemoteGraphApp:
// ���̃N���X�̎����ɂ��ẮARemoteGraph.cpp ���Q�Ƃ��Ă��������B
//

class CRemoteGraphApp : public CWinAppEx
{
public:
	CRemoteGraphApp();

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	CString&		GetAppPath()			{ return m_strAppPath; }
	CString&		GetAppInifileName()		{ return m_strAppInifileName; }
	CString&		GetAppDataPath()		{ return m_strAppDataPath; }
	CString&		GetAppLogPath()			{ return m_strAppLogPath; }
//	CString&		GetServerAddress()		{ return m_strServerAddress; }
//	CString&		GetServerFilePath()		{ return m_strServerFilePath; }
	CString&		GetFileServerName()		{ return m_strFileServerName; }

	CPropertiesData&	GetPropertiesData(int pos)
	{
		if (pos < 0 || pos >= MAX_VIEW)
			return m_propData[0];
		return m_propData[pos];
	}
	void				SetPropertiesData(int pos, CPropertiesData val) { m_propData[pos] = val; }
	void	WriteAllPropertiesData(CString szFilePath)
	{
		for( int i = 0; i < MAX_VIEW; i++ )
			m_propData[i].WritePropertiesData(szFilePath, i);
	}
	void	ReadAllPropertiesData(CString szFilePath)
	{
		for( int i = 0; i < MAX_VIEW; i++ )
			m_propData[i].ReadPropertiesData(szFilePath, i);
	}

	CDataCollection&	GetDataCollection()		{ return m_DataCollection; }

	void			OpenNewView()				{ OnFileNew(); }

//	bool		GetTrendScrollMode(int pos) { return m_TrendScrollMode[pos]; }
//	void		SetTrendScrollMode(int pos, bool val) { m_TrendScrollMode[pos] = val; }

	UINT			GetDefSelectStationMode()			{ return m_nDefSelectMode; }
	void			SetDefSelectStationMode(UINT val)	{ m_nDefSelectMode = val; }
	UINT			GetDefSelectStation()				{ return m_nDefSelectStation; }
	void			SetDefSelectStation(UINT val)		{ m_nDefSelectStation = val; }
	UINT			GetSelectStationMode()				{ return m_nSelectMode; }
	void			SetSelectStationMode(UINT val)		{ m_nSelectMode = val; }
	UINT			GetSelectStation()					{ return m_nSelectStation; }
	void			SetSelectStation(UINT val)			{ m_nSelectStation = val; }
	UINT			GetSelectEquipment()					{ return m_nSelectEquipment; }
	void			SetSelectEquipment(UINT val)			{ m_nSelectEquipment = val; }

	// ���A���^�C�����[�h�̑���̊J�n�����ƍŐV����(�擾�����f�[�^�̒��ōł��ŐV�̎���)
	CTime		GetRealTimeStart()	{ return m_RealTimeStart; }
	void		SetRealTimeStart(CTime& time)	{ m_RealTimeStart = time; }
	CTime		GetRealTimeLast()	{ return m_RealTimeLast; }
	void		SetRealTimeLast(CTime& time)	{ m_RealTimeLast = time; }
	CTimeSpan	GetRealTimeElapsed()	{ return m_RealTimeLast - m_RealTimeStart; }		// �J�n��������̌o�ߎ���
	unsigned __int64	GetRealTimeCount()	{ return GetRealTimeElapsed().GetTotalSeconds() + 1; }	// �J�n��������̎擾�_��

	CTime		GetRealTimeWriteData(int index)	{ return m_RealTimeWriteData[index]; }
	void		SetRealTimeWriteData(int index, CTime& time)	{ m_RealTimeWriteData[index] = time; }
	CTime		GetRealTimeWriteMax()	{ return m_RealTimeWriteMax; }
	void		SetRealTimeWriteMax(CTime& time)	{ m_RealTimeWriteMax = time; }

	UINT		GetStartFile()	{ return m_StartFile; }
	void		SetStartFile(UINT value)	{ m_StartFile = value; }

	CString		GetShareFilePath(UINT fileType, UINT nStation = eStation_MAX);

	void	DeleteFileFromDataFolder(int nView);
	void	DeleteFileFromDataFolderAll();

	void		SetAnalysisMigration(int view)	{ m_bAnalysisMigration |= (1 << view); }
	void		ClrAnalysisMigration(int view)	{ m_bAnalysisMigration &= ~(1 << view); }
	bool		IsAnalysisMigration(int view)	{ return (m_bAnalysisMigration & (1 << view)) ? true : false; }
	bool		IsAnalysisMigrationAll()		{ return (m_bAnalysisMigration != 0x0) ? true : false; }

	bool		IsDiskFreeSpace()				{ return m_bDiskFreeSpace; }
	void		SetDiskFreeSpace(bool flg)		{ m_bDiskFreeSpace = flg; }
	bool		IsCheckDiskFreeSpace()			{ return m_bCheckDiskFreeSpace; }

	DWORD		GetHostIpInfo();

	UINT		GetLogLevel()	{ return m_nLogLevel; }

	CString		AddFileName(CString sSrcPath, CString sAddFileName)
	{
		TCHAR	szDir[_MAX_DIR];
		TCHAR	szDrive[_MAX_DRIVE];
		TCHAR	szFname[_MAX_FNAME];
		TCHAR	szExt[_MAX_EXT];

		_wsplitpath_s(sSrcPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT);

		CString	sResult;
		sResult.Format(_T("%s%s%s%s%s"), szDrive, szDir, szFname, (LPCTSTR)sAddFileName, szExt);

		return sResult;
	}

	deque<PIPEMONITORACK>&	GetMessageTemp(int nDispStation)		{ return mMessageTemp[nDispStation]; }

protected:
	void	_InitializeAppl();

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
	deque<PIPEMONITORACK>	mMessageTemp[eStation_MAX];

protected:
	CString		m_strAppPath;						// �A�v���P�[�V�����p�X��
	CString		m_strAppInifileName;				// �A�v���P�[�V����INI�t�@�C����
	CString		m_strAppDataPath;					// �A�v���P�[�V����DATA�p�X��
	CString		m_strAppLogPath;					// �A�v���P�[�V����LOG�p�X��
	UINT		m_nLogLevel;
//	CString		m_strServerAddress;					// �T�[�o�[�A�h���X������i192.168.1.1�j
//	CString		m_strServerFilePath;				// �T�[�o�[�t�@�C���p�X������i\\server\\data�j
	CString		m_strFileServerName;				// �t�@�C���T�[�o�[����

	CPropertiesData	m_propData[MAX_VIEW];
	CDataCollection	m_DataCollection;

	CTime		m_RealTimeStart;
	CTime		m_RealTimeLast;

	// �f�[�^�̎�ށF�f�[�^�o�^����Ă���f�[�^���i�[
	// �f�[�^�̏��ԁF�f�[�^�o�^�̏��ԂɊi�[
	CTime		m_RealTimeWriteData[MAX_VIEW * MAX_TDATA];
	CTime		m_RealTimeWriteMax;

	UINT		m_StartFile;

//	bool		m_TrendScrollMode[MAX_VIEW];

	CCommandLineInfoEx	m_cmdInfo;
	
	UINT		m_nDefSelectMode;					// �f�t�H���g�\���ǃ��[�h�i0:�P�� 1:5�� 2:10�ǁj
	UINT		m_nDefSelectStation;				// �f�t�H���g1�ǃ��[�h���̋ǁi1:�P�c64m�� 2:�P�c54m�� 3:���V�Y34�� 4:���V�Y20�ǁj
	UINT		m_nSelectMode;						// �\���ǃ��[�h�i0:�P�� 1:5�� 2:10�ǁj
	UINT		m_nSelectStation;					// 1�ǃ��[�h���̋ǁi1:�P�c64m 2:�P�c54m�� 3:���V�Y34�� 4:���V�Y20�ǁj
	UINT		m_nSelectEquipment;					// �f�[�^�o�^�̑I��ݔ�

	DWORD		m_bAnalysisMigration;				// ��͈ڍs�t���O

	bool	m_bDiskFreeSpace;						// �f�B�X�N�󂫗e�ʃt���O
	bool	m_bCheckDiskFreeSpace;					// �f�B�X�N�󂫗e�ʃ`�F�b�N

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRemoteGraphApp theApp;
