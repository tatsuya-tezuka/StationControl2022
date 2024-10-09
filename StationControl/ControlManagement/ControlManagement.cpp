
// ControlManagement.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "ControlManagement.h"
#include "ControlManagementDlg.h"
#include "ControlManagementInputPassword.h"

#include <lm.h>
#pragma comment( lib, "Netapi32.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CControlManagementApp

BEGIN_MESSAGE_MAP(CControlManagementApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CControlManagementApp �R���X�g���N�V����

CControlManagementApp::CControlManagementApp()
{
	// �ċN���}�l�[�W���[���T�|�[�g���܂�
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� CControlManagementApp �I�u�W�F�N�g�ł��B

CControlManagementApp theApp;


// CControlManagementApp ������

BOOL CControlManagementApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	InitializeApp();

	CControlManagementInputPassword man;
	if (man.DoModal() == IDCANCEL){
		return FALSE;
	}

	AfxEnableControlContainer();

	// �_�C�A���O�ɃV�F�� �c���[ �r���[�܂��̓V�F�� ���X�g �r���[ �R���g���[����
	// �܂܂�Ă���ꍇ�ɃV�F�� �}�l�[�W���[���쐬���܂��B
	CShellManager *pShellManager = new CShellManager;

	// MFC �R���g���[���Ńe�[�}��L���ɂ��邽�߂ɁA"Windows �l�C�e�B�u" �̃r�W���A�� �}�l�[�W���[���A�N�e�B�u��
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));

	CControlManagementDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <�L�����Z��> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "�x��: �_�C�A���O�̍쐬�Ɏ��s���܂����B�A�v���P�[�V�����͗\�������ɏI�����܂��B\n");
		TRACE(traceAppMsg, 0, "�x��: �_�C�A���O�� MFC �R���g���[�����g�p���Ă���ꍇ�A#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS ���w��ł��܂���B\n");
	}

	// ��ō쐬���ꂽ�V�F�� �}�l�[�W���[���폜���܂��B
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	//  �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# �A�v���P�[�V�����̏�����

@param
@retval

*/
/*============================================================================*/
void CControlManagementApp::InitializeApp()
{
	// �e��t�@�C���E�t�H���_�̍쐬
	TCHAR	szFullPath[_MAX_PATH];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];

	if (::GetModuleFileName(NULL, szFullPath, _MAX_PATH) == 0)
	{
		return;
	}

	_wsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	m_strAppPath.Format(_T("%s%s"), szDrive, szDir);
	m_strAppInifileName.Format(_T("%s%sStationControl.ini"), szDrive, szDir);

	TCHAR buf[MAX_PATH];
	GetPrivateProfileString(SECTION_SYSTEM, KEY_FILESERVER_NAME, _T(""), buf, MAX_PATH, m_strAppInifileName);
	m_strFileServerName = buf;
	CString strssoc, strsttn, strtksc;
	if (m_strFileServerName.IsEmpty() == false){
		m_strFileServerName += _T("\\");
	}
	if (m_strFileServerName.IsEmpty() == true){
		m_strManagementPath.Format(_T("C:\\share\\%s\\"), (LPCTSTR)mManagementPath);
		strssoc.Format(_T("C:\\share\\%s\\%s\\"), (LPCTSTR)mManagementPath, (LPCTSTR)mManagementSsoc);
		strsttn.Format(_T("C:\\share\\%s\\%s\\"), (LPCTSTR)mManagementPath, (LPCTSTR)mManagementSttn);
		strtksc.Format(_T("C:\\share\\%s\\%s\\"), (LPCTSTR)mManagementPath, (LPCTSTR)mManagementTksc);
	}
	else{
		m_strManagementPath.Format(_T("\\\\%sshare\\%s\\"), (LPCTSTR)m_strFileServerName, (LPCTSTR)mManagementPath);
		strssoc.Format(_T("\\\\%sshare\\%s\\%s\\"), (LPCTSTR)m_strFileServerName, (LPCTSTR)mManagementPath, (LPCTSTR)mManagementSsoc);
		strsttn.Format(_T("\\\\%sshare\\%s\\%s\\"), (LPCTSTR)m_strFileServerName, (LPCTSTR)mManagementPath, (LPCTSTR)mManagementSttn);
		strtksc.Format(_T("\\\\%sshare\\%s\\%s\\"), (LPCTSTR)m_strFileServerName, (LPCTSTR)mManagementPath, (LPCTSTR)mManagementTksc);
	}

	// �t�H���_���Ȃ���������Ȃ��̂ō쐬����
	CreateDirectory(m_strManagementPath, NULL);	//�@�t�H���_�쐬
	//CreateDirectory(strssoc, NULL);	//�@�t�H���_�쐬
	//CreateDirectory(strsttn, NULL);	//�@�t�H���_�쐬
	//CreateDirectory(strtksc, NULL);	//�@�t�H���_�쐬


	// ���䌠���֘A�̃t�@�C�������݂��Ȃ��ꍇ�̓f�t�H���g�ݒ�ō쐬����
	int i;
	CFileAccess& ac = theApp.GetFileAccess();
	CString strDefaultPass = mDefaultPassword;
	strDefaultPass = theApp.ManEncodeDecode(strDefaultPass);
	CStringA strPass = CStringA(strDefaultPass);

	//=========================================================================
	//	�^�p�A�J�E���g�ݒ�t�@�C���̍쐬
	//=========================================================================
	vector<stAccount> mAcList;
	vector<stAccount> mAccountList;
	vector<stUserInfo> mUserList;
	theApp.GetLocalUserList(mUserList);
	if (ac.ReadOperationAccount(mAcList) == false){
		// �^�p�A�J�E���g�t�@�C�������݂��Ȃ��̂ŁA���[�J�����[�U�ŏ���������
		mAccountList.clear();
		vector<stUserInfo>::iterator itr;
		for (itr = mUserList.begin(); itr != mUserList.end(); itr++){
			stAccount data;
			data.user = (*itr).user;
			data.usersid = (*itr).usersid;
			data.usc20 = false;
			data.usc34 = false;
			data.udsc64 = false;
			data.xdsc54 = false;
			data.enable = true;
			data.mycomputer = true;
			data.dummy = 0;
			mAccountList.push_back(data);
		}
		ac.WriteOperationAccount(mAccountList);
	}
	//=========================================================================
	//	���䌠���Ǘ��p�p�X���[�h�t�@�C���̍쐬
	//=========================================================================
	CString mManPassword;
	if (ac.ReadManagementPassword(mManPassword) == false){
		// �p�X���[�h���ݒ肳��Ă��Ȃ��̂ŁA�f�t�H���g�p�X���[�h��ݒ肷��
		mManPassword = mDefaultPassword;
		mManPassword = theApp.ManEncodeDecode(mManPassword);
		ac.WriteManagementPassword(mManPassword);
	}
	//=========================================================================
	//	�p�X���[�h�E���䌠���Ǘ��t�@�C���̍쐬
	//=========================================================================
	stControl	mTControl[mMaxType];
	stControl	mKControl[mMaxType];
	stControl	mSControl[mMaxType];
	// TKSC
	for (i = 0; i < mMaxType; i++){
		if (ac.ReadSOSControl(mTControl[i], eTKSCusc20 + i) == false){
			// �t�@�C�������݂��Ȃ��̂Ńf�t�H���g��ݒ肷��
			// TKSC�͏펞�P
			memset(&mTControl[i], 0, sizeof(stControl));
			mTControl[i].flag = 1;
			memcpy((char*)mTControl[i].password, (char*)(const char*)strPass, mMaxPassword);
			sprintf_s(mTControl[i].lifetime, mMaxLifetime, "%I64d", INT64_MAX);
			ac.WriteSOSControl(mTControl[i], eTKSCusc20 + i, true);
		}
	}
	// �n���
	for (i = 0; i < mMaxType; i++){
		if (ac.ReadSOSControl(mKControl[i], eSOSusc20 + i) == false){
			// �t�@�C�������݂��Ȃ��̂Ńf�t�H���g��ݒ肷��
			memset(&mKControl[i], 0, sizeof(stControl));
			mKControl[i].flag = 0;
			memcpy((char*)mKControl[i].password, (char*)(const char*)strPass, mMaxPassword);
			sprintf_s(mKControl[i].lifetime, mMaxLifetime, "0");
			ac.WriteSOSControl(mKControl[i], eSOSusc20 + i);
		}
	}
	// ���͌��Ǌǐ�
	for (i = 0; i < mMaxType; i++){
		if (ac.ReadSSOCControl(mSControl[i], eSSOCusc20 + i) == false){
			// �t�@�C�������݂��Ȃ��̂Ńf�t�H���g��ݒ肷��
			memset(&mSControl[i], 0, sizeof(stControl));
			mSControl[i].flag = 0;
			memcpy((char*)mSControl[i].password, (char*)(const char*)strPass, mMaxPassword);
			sprintf_s(mSControl[i].lifetime, mMaxLifetime, "0");
			ac.WriteSSOCControl(mSControl[i], eSSOCusc20 + i);
		}
	}
	//=========================================================================
	//	���䌠���擾�󋵃t�@�C��
	//=========================================================================
	for (i = 0; i < mMaxType; i++){
		stLocalControl	tdata;
		if (ac.ReadLocalControl(tdata, eLOCALusc20 + i) == false){
			tdata.flag = 0;
			sprintf_s(tdata.lifetime, mMaxLifetime, "0");
			ac.WriteLocalControl(tdata, eLOCALusc20 + i);
		}
	}

}

/*============================================================================*/
/*! ���䌠���Ǘ�

-# �p�X���[�h�̃G���R�[�h,�f�R�[�h

@param
@retval

*/
/*============================================================================*/
CString CControlManagementApp::ManEncodeDecode(CString str)
{
	// 8�����Œ�
	char tobuf[mMaxPassword + 1];
	char frombuf[mMaxPassword + 1];
	sprintf_s((char*)tobuf, mMaxPassword + 1, "%s", (LPCSTR)CStringA(str));
	memset(frombuf, 0, mMaxPassword + 1);

	short* toptr = (short*)tobuf;
	short* fromptr = (short*)frombuf;

	for (int i = 0; i < mMaxPassword / 2; i++){
		*fromptr = htons(*toptr);
		toptr++;
		fromptr++;
	}
	CString ret = CString(frombuf);
	return ret;
}

#include <sddl.h>

/*============================================================================*/
/*! ���䌠���Ǘ�

-# ���[�J�����[�U�ꗗ�̎擾

@param
@retval

*/
/*============================================================================*/
void CControlManagementApp::GetLocalUserList(vector<stUserInfo>& userList)
{
	LPUSER_INFO_1 pBuf = NULL;
	LPUSER_INFO_1 pTmpBuf;
	DWORD dwLevel = 1;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;

	userList.clear();

	do
	{
		nStatus = NetUserEnum((LPCWSTR)pszServerName,
			dwLevel,
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				// �G���g���[�����J��Ԃ�
				for (i = 0; (i < dwEntriesRead); i++)
				{
					if (pTmpBuf == NULL)
					{
						break;
					}
					TRACE("\t-- %s\n", CStringA(pTmpBuf->usri1_name));
					if (!(pTmpBuf->usri1_flags & UF_ACCOUNTDISABLE) && pTmpBuf->usri1_priv != USER_PRIV_GUEST){
						CString str = CString(pTmpBuf->usri1_name);
						stUserInfo info;
						info.user = str;
						getUserSid(str, info.usersid);
						userList.push_back(info);
					}

					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	while (nStatus == ERROR_MORE_DATA); // end do
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
}

bool CControlManagementApp::getUserSid(CString& strUserName, CString& strSid)
{
	// ���[�J���A�J�E���g��SID���擾����
	DWORD			cbDomain = 0;
	DWORD			cbUserSID = 0;
	SID_NAME_USE	snuType;
	TCHAR			*pszDomain = NULL;
	LPVOID			pUserSID = NULL;
	BOOL ret;

	strSid.Empty();

	// �ŏ��̎��s�Ŋe��T�C�Y���擾����
	ret = ::LookupAccountName(NULL, (TCHAR*)(const TCHAR*)strUserName, pUserSID, &cbUserSID, pszDomain, &cbDomain, &snuType);
	if (ret == FALSE){
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER){
			return false;
		}
	}
	pUserSID = (PSID)malloc(cbUserSID);
	if (pUserSID == NULL){
		// �������m�ێ��s
		return false;
	}

	pszDomain = (TCHAR*)::malloc(cbDomain*sizeof(TCHAR));
	if (NULL == pszDomain){
		// �������m�ێ��s
		free(pUserSID);
		return false;
	}
	memset(pszDomain, 0, cbDomain * sizeof(TCHAR));

	// SID�擾
	ret = ::LookupAccountName(NULL, (TCHAR*)(const TCHAR*)strUserName, pUserSID, &cbUserSID, pszDomain, &cbDomain, &snuType);
	if (ret == FALSE){
		free(pUserSID);
		free(pszDomain);
		return false;
	}

	// �����܂ł����SID�͐���Ɏ擾�ł���
	PSID_IDENTIFIER_AUTHORITY psia;
	DWORD dwSidSize, dwSubAuthorities;

	if (!IsValidSid(pUserSID)){
		free(pUserSID);
		free(pszDomain);
		return false;
	}

	// �R���s���[�^���̈�̃N���A
	free(pszDomain);

	psia = GetSidIdentifierAuthority(pUserSID);
	dwSubAuthorities = *GetSidSubAuthorityCount(pUserSID);
	// S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
	dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);

	//LPTSTR pSID = NULL;
	//DWORD size = dwSidSize;
	//pSID = (LPTSTR)malloc(size);
	//if (pSID){
	//	memset(pSID, 0, size);
	//}
	//else{
	//	free(pUserSID);
	//	free(pszDomain);
	//	return false;
	//}

	CString temp;
	strSid.Empty();
	temp.Format(_T("S-%lu-"), SID_REVISION);
	strSid += temp;
	//dwSidSize = swprintf_s(pSID, dwSidSize * 2, _T("S-%lu-"), SID_REVISION);


	if ((psia->Value[0] != 0) || (psia->Value[1] != 0))
	{
		temp.Format(_T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
			(USHORT)psia->Value[0],
			(USHORT)psia->Value[1],
			(USHORT)psia->Value[2],
			(USHORT)psia->Value[3],
			(USHORT)psia->Value[4],
			(USHORT)psia->Value[5]);
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + lstrlen(pSID),
		//	size,
		//	_T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
		//	(USHORT)psia->Value[0],
		//	(USHORT)psia->Value[1],
		//	(USHORT)psia->Value[2],
		//	(USHORT)psia->Value[3],
		//	(USHORT)psia->Value[4],
		//	(USHORT)psia->Value[5]);
	}
	else
	{
		temp.Format(_T("%lu"),
			(ULONG)(psia->Value[5]) +
			(ULONG)(psia->Value[4] << 8) +
			(ULONG)(psia->Value[3] << 16) +
			(ULONG)(psia->Value[2] << 24));
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + lstrlen(pSID),
		//	size,
		//	_T("%lu"),
		//	(ULONG)(psia->Value[5]) +
		//	(ULONG)(psia->Value[4] << 8) +
		//	(ULONG)(psia->Value[3] << 16) +
		//	(ULONG)(psia->Value[2] << 24));
	}

	// Add SID subauthorities to the string
	for (DWORD dwCounter = 0; dwCounter < dwSubAuthorities; dwCounter++)
	{
		temp.Format(_T("-%lu"), *GetSidSubAuthority(pUserSID, dwCounter));
		strSid += temp;
		//dwSidSize += swprintf_s(pSID + dwSidSize,
		//	size,
		//	_T("-%lu"),
		//	*GetSidSubAuthority(pUserSID, dwCounter));
	}

	free(pUserSID);

	return true;
}

void CControlManagementApp::getLastError()
{
	LPVOID lpMsgBuf;
	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // ����̌���
		(LPTSTR)&lpMsgBuf,
		0,
		NULL) == 0)
	{
		return;
	}

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION);
	// �o�b�t�@���������B
	LocalFree(lpMsgBuf);
}


/*============================================================================*/
/*! CSshShell

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSshShell::CSshShell()
{
}


/*============================================================================*/
/*! CSshShell

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSshShell::~CSshShell()
{
}
/*============================================================================*/
/*! CSshShell

-# �T�[�o��SSH�ڑ����A�V�F�������s����

@param  stParam�FSSH�\����
@param	bWaitComplete�FTRUE:�V�F���I����҂i�f�t�H���g�j / FALSE:�V�F���I����҂��Ȃ�
@retval �V�F�����s����(0�F�����A���̑��F���s�j
*/
/*============================================================================*/
int CSshShell::ExecShell(structSshParam& stParam, BOOL bWaitComplete /*= TRUE*/)
{
	CWaitCursor wait;

	int ret = -1;

	CString strSshBatch;
	strSshBatch.Format(_T("%s %s@%s ./%s %s \"%s\""), (LPCTSTR)stParam.strBatchPath, (LPCTSTR)stParam.strUserName, (LPCTSTR)stParam.strServerName, SVSTART_SH, (LPCTSTR)stParam.strRemShellName, (LPCTSTR)stParam.strRemShellParam);
	TRACE("%s\n", CStringA(strSshBatch));

	if (stParam.strBatchPath.IsEmpty() ||
		stParam.strUserName.IsEmpty() ||
		stParam.strServerName.IsEmpty() ||
		stParam.strRemShellName.IsEmpty() ||
		stParam.strRemShellParam.IsEmpty())
		return ret;

	// �V�����v���Z�X�p�̍\���̂��N������
	STARTUPINFO    stStartUpInfo;
	// �����o�[�̑����͗\�񂳂�Ă��邩���������̂ŁA���ׂẴ����o�[��
	// 0 �ɐݒ肵�Ă���A�K�v�ȃ����o�[���������Z�b�g����
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);			// �\���̂̃T�C�Yls
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow �����o�[���g��
	//	stStartUpInfo.wShowWindow = SW_SHOWDEFAULT;		// �f�t�H���g�̒l
	stStartUpInfo.wShowWindow = SW_HIDE;			// ��ʂ͔�\��

	// �V�����v���Z�X��ŏ����󂯎��v���Z�X�\����
	PROCESS_INFORMATION stProcessInfo;

	// �V�����v���Z�X�����ƁA�v���Z�X�̓v���C�}���X���b�h�����A
	// �A�v���P�[�V�������N������
	if (CreateProcess(NULL, (LPTSTR)strSshBatch.GetBuffer(), NULL, NULL, FALSE,
		/*CREATE_SUSPENDED*/NORMAL_PRIORITY_CLASS, NULL,
		NULL, &stStartUpInfo, &stProcessInfo) == 0)
	{
		strSshBatch.ReleaseBuffer();
		return -1;
	}
	strSshBatch.ReleaseBuffer();

	// �V�F���I����҂ꍇ
	if (bWaitComplete)
	{
		// �����[�g�V�F���I���܂őҋ@
		if (WaitForSingleObject(stProcessInfo.hProcess, SSH_WAITTIME) == WAIT_OBJECT_0)
		{
			DWORD rc;
			GetExitCodeProcess(stProcessInfo.hProcess, &rc);
			ret = (int)rc;
		}
		else
		{
		}
	}
	// �V�F���I����҂��Ȃ��ꍇ
	else
	{
		ret = 0;
	}

	CloseHandle(stProcessInfo.hThread);
	CloseHandle(stProcessInfo.hProcess);

	return ret;
}
