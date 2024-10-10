/*============================================================================*/
/*! EngValCnv.cpp

-# �H�w�l�ϊ�
*/
/*============================================================================*/

#include "stdafx.h"
#include "EngValCnv.h"
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <iomanip>

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

CEngValCnvDll::CEngValCnvDll(char* db_file_name)
{
	mCallBack = NULL;
	db_data = NULL;
	db_data2 = NULL;
	try{
		HANDLE hFile = CreateFileA(db_file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		mFileSize = GetFileSize(hFile, NULL) + 100;
		CloseHandle(hFile);
		db_data = new char[mFileSize];
		db_data2 = new char[mFileSize];
		//DB���i�[�̈��������
		memset(db_data, 0, mFileSize);
		memset(db_data2, 0, mFileSize);

		//DB�t�@�C����ǂ�ł���
		if (DbRead(db_file_name, db_data) != 0){
			db_status = 1;
			return;
		}

		memcpy_s(db_data2, mFileSize, db_data, mFileSize);

		// DB�e�[�u���쐬
		CreateObsTbl();

		db_status = 0;
	}
	catch (...){
		//���s
		db_status = 1;
	}

	m_lastCnvAfterType = 0;

	sprintf_s(mEqMonDBFileName, _MAX_PATH, "%s", db_file_name);
}

CEngValCnvDll::CEngValCnvDll()
{
	memset(mEqMonDBFileName, 0, _MAX_PATH);
	db_data = NULL;
	db_data2 = NULL;
}

CEngValCnvDll::~CEngValCnvDll()
{
	data_list.clear();

	if (db_data != NULL)
		delete[] db_data;
	if (db_data2 != NULL)
		delete[] db_data2;

	mMonTimeLDData.clear();
}

/*
	������EngValConv.DLL�̎g�p���@������

	�@CEngValCnvDll�̒�`
	  CEngValCnvDll engcnv;
	�A�Ď�DB�t�@�C�����̐ݒ�
	  CString val = _T("C:\\share\\udsc64\\ctrl\\db\ld\\eqmon.db.obj");
	  engcnv.SetEqMonDbFileName(val);
	�B�Ď��~�σf�[�^�p�X�̐ݒ�
	  val = _T("C:\\share\\udsc64\\log\\mon\\");
	  engcnv.SetMonDataPath(val);
	�C�Ď��f�[�^�̎擾
	  TL_TIME timeTarget={0,2017,5,1,0,0};
	  map< CString, vector<obs_tbl_res_t> > outDataList;
	  engcnv.GetMonitorData(timeTarget, outDataList);
*/

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �Ď�DB�ueqmon.db.obj�v�̃t�@�C���p�X���iC:\share\udsc64\ctrl\db\ld\eqmon.db.obj�j���w�肷��

@param	val		�Ď�DB�t�@�C����

@retval

*/
/*============================================================================*/
void CEngValCnvDll::SetEqMonDbFileName(TCHAR* val)
{
	mDbName = CString(val);
	sprintf_s(mEqMonDBFileName, _MAX_PATH, "%s", (LPCSTR)CStringA(val));

	if (db_data != NULL)
		delete[] db_data;
	if (db_data2 != NULL)
		delete[] db_data2;

	try{
		HANDLE hFile = CreateFileA(mEqMonDBFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		mFileSize = GetFileSize(hFile, NULL) + 100;
		CloseHandle(hFile);
		db_data = new char[mFileSize];
		db_data2 = new char[mFileSize];
		//DB���i�[�̈��������
		memset(db_data, 0, mFileSize);
		memset(db_data2, 0, mFileSize);

		//DB�t�@�C����ǂ�ł���
		if (DbRead(mEqMonDBFileName, db_data) != 0){
			db_status = 1;
			return;
		}

		memcpy_s(db_data2, mFileSize, db_data, mFileSize);

		db_status = 0;
	}
	catch (...){
		//���s
		db_status = 1;
	}

	m_lastCnvAfterType = 0;
}

/*
---------------------------------------------------------------------
$name = { �Ď����ږ��e�[�u���쐬 }
$outline = {
�e�[�u���쐬
}
$comment = {
}
---------------------------------------------------------------------
*/
/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �Ď����ږ��e�[�u���쐬

@param
@retval

*/
/*============================================================================*/
int CEngValCnvDll::CreateObsTbl()
{
	struct xobsname_t *obs_adrs;
	struct xext_tbl_t *ext_adr;
	char			 *mon_adr;
	long			l_rc;

	memcpy_s(db_data, mFileSize, db_data2, mFileSize);

	/*----- �e�Ď����ڃf�[�^�擾���� -----*/
	mon_adr = db_data;
	mon_adr += (sizeof(long) * 2);

	xobs_tbl_t st;
	obs_map.clear();

	try{
		int l = 0;
		int t = 0;
		while (1){
			obs_adrs = (struct xobsname_t *)mon_adr;

			long ed_id = ds_htonl(obs_adrs->l_id);
			long ed_len = ds_htonl(obs_adrs->l_len);

			t++;

			if (ed_id == 0) {
				break;
			}
			if (ed_id == OBSNAME_ID)
			{
				st.obs_adr = obs_adrs;
				l_rc = ud_ext_adr(st.obs_adr->sz_obsname, obs_adrs, &ext_adr);
				if (l_rc == 0) {
					st.l_dataid = ds_htonl(ext_adr->s_pwb[0].l_pid);;

					long ed_max = ds_htonl(ext_adr->l_pw_cnt);
					for (int m = 1; m < ed_max; m++) {
						if (st.l_dataid != ds_htonl(ext_adr->s_pwb[m].l_pid)) {
							st.l_dataid = -1;
							break;
						}
					}
				}
				else {
					st.l_dataid = -2;
					st.obs_adr->l_no = -1;
				}

				st.l_offset = (long)(mon_adr - db_data);
				st.obs_adr->l_inflg = 0;
				l++;
				if (l >= OBS_MAX) {
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: Error over OBS_MAX(%d)"), __FILEW__, __LINE__, l);
						//						CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
					}
					return(-1);
				}

				obs_map[st.l_dataid].push_back(st);
			}
			mon_adr += ed_len;
		}
	}
	catch (...){
		TRACE("@@@ CalcFromFile Exception\n");
	}

	return 0;
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �Ď��~�σt�@�C�����i�[����Ă���p�X���iC:\share\udsc64\log\mon\�j���w�肷��

@param	val		�Ď��~�σt�@�C���p�X���i�I�[\\�j

@retval

*/
/*============================================================================*/
void CEngValCnvDll::SetMonDataPath(TCHAR* val)
{
	swprintf_s(mMonDataPathName, _MAX_PATH, _T("%s"), val);
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �w����Ԃ̊Ď��f�[�^�t�@�C����ǂݍ��݁A�w�肳�ꂽ�Ď����ɑΉ�����f�[�^���擾����

@param	startTime	�J�n����
@param	endTime		�I������
@param	outDataList	�Ď��f�[�^���X�g(����)�iANT.AZ_DRIVE,ANT.REAL_AZ�E�E�E�j�L�[	(�o��)�Ď��f�[�^

@retval	bool

  */
/*============================================================================*/
bool CEngValCnvDll::GetMonitorData(xTL_TIME& timeTarget, map< CString, vector<xobs_tbl_res_t> >& outDataList)
{
	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	vector<CString> filelist;
	filelist.clear();

	xTL_TIME timeT = timeTarget;

	// �Ď��~�σf�[�^�̑Ώۃp�X���擾����

	// �J�n�I����������Ώۓ��t�t�H���_�����߂�
	switch (timeTarget.bMode){
	case	eTimeDirect:
	{
		if (ConvertDirectRangeTime(timeTarget, timeT, true) == false)
			return false;
		GetTimeRangePath(timeT.sttime, timeT.sptime, filelist);
		break;
	}
	case	eTimeRange:
		GetTimeRangePath(timeT.sttime, timeT.sptime, filelist);
		break;
	default:
		return false;
	}

	if (filelist.size() == 0)
		return false;

	// �u�Ď��~�σt�@�C���p�X���v�{�u�J�n�I���������狁�߂��t�H���_�v���f�[�^�t�@�C�������擾����
	vector<CString>::iterator itr;
	CString fname;
	vector<CString> datafilelist;
	datafilelist.clear();
	for (itr = filelist.begin(); itr != filelist.end(); itr++){
		fname.Format(_T("%s%s\\"), mMonDataPathName, (LPCTSTR)(*itr));
		GetDataFileList(fname, datafilelist);
	}

	// �Ď��~�σt�@�C�������܂����̂ōH�w�l�ϊ����s��
	char file[_MAX_PATH];
	for (itr = datafilelist.begin(); itr != datafilelist.end(); itr++){
		sprintf_s(file, _MAX_PATH, "%s", (LPCSTR)CStringA((*itr)));
		if (CalcFromFileEx(file, outDataList) < 0)
			continue;
		
		// �H�w�l�ϊ��������ʂ���A�Ώۂ̊Ď����̂𔲂��o��
		vector<xobs_tbl_res_t>& data = GetAllVal();
		vector<xobs_tbl_res_t>::iterator itrd;
		for (itrd = data.begin(); itrd != data.end(); itrd++){

			// �Ώۂ̊Ď��������݂��邩�`�F�b�N����
			if (IsObsName((*itrd).obs_adr.sz_obsname, outDataList) == false){
				continue;
			}

			CStringA str((*itrd).obs_adr.sz_obsname);
			CString wstr = CString(str);
			map< CString, vector<xobs_tbl_res_t> >::iterator itrn;
			itrn = outDataList.find(wstr);

			// ���Ԕ͈͂̊m�F
			if (timeTarget.bMode == eTimeRange){
				if (timeTarget.sttime > (*itrd).ld_time_val || timeTarget.sptime < (*itrd).ld_time_val){
					// �͈͊O
					continue;
				}
			}

			(*itrn).second.push_back((*itrd));
			//CTime tm = CTime((*itrd).ld_time_val);
			//CString strtm = tm.Format("%Y-%m-%d  %H:%M:%S");
			//TRACE("==%s\n", CStringA(strtm));
		}
	}

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### GetMonitorData = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
	timeEndPeriod(1);

	return true;
}

bool CEngValCnvDll::GetMonitorDataEx(xTL_TIME* timeTarget)
{
	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	xTL_TIME timeT = *timeTarget;

	// �Ď��~�σf�[�^�̑Ώۃt�@�C���p�X���m�F����
	if (mMonDataFileList.size() == 0)
		return false;

	DWORD dwElapsed = 0;

	// �Ď��~�σt�@�C�������܂����̂ōH�w�l�ϊ����s��
	char file[_MAX_PATH];
	vector<CString>::iterator itr;
	for (itr = mMonDataFileList.begin(); itr != mMonDataFileList.end(); itr++){
		sprintf_s(file, _MAX_PATH, "%s", (LPCSTR)CStringA((*itr)));
		if (CalcFromFileEx(file, mOutDataList) < 0)
			continue;

		dwElapsed = (timeGetTime() - dwStart);
		TRACE("### CalcFromFileEx = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
		dwStart = timeGetTime();

		// �H�w�l�ϊ��������ʂ���A�Ώۂ̊Ď����̂𔲂��o��
		vector<xobs_tbl_res_t>& data = GetAllVal();
		vector<xobs_tbl_res_t>::iterator itrd;
		for (itrd = data.begin(); itrd != data.end(); itrd++){

			// ���ȉ��̏�����CalcFromFileEx�֐����ōs��
			// �Ώۂ̊Ď��������݂��邩�`�F�b�N����
			//if (IsObsName((*itrd).obs_adr.sz_obsname, mOutDataList) == false){
			//	continue;
			//}

			CStringA str((*itrd).obs_adr.sz_obsname);
			CString wstr = CString(str);
			map< CString, vector<xobs_tbl_res_t> >::iterator itrn;
			itrn = mOutDataList.find(wstr);

			// ���Ԕ͈͂̊m�F
			if (timeTarget->bMode == eTimeRange){
				if (timeTarget->sttime > (*itrd).ld_time_val || timeTarget->sptime < (*itrd).ld_time_val){
					// �͈͊O
					continue;
				}
			}

			(*itrn).second.push_back((*itrd));
			//CTime tm = CTime((*itrd).ld_time_val);
			//CString strtm = tm.Format("%Y-%m-%d  %H:%M:%S");
			//TRACE("==%s\n", CStringA(strtm));
		}

		dwElapsed = (timeGetTime() - dwStart);
		TRACE("### FileRead = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
		dwStart = timeGetTime();
	}

	timeEndPeriod(1);

	return true;
}

void CEngValCnvDll::ClearMonitorName()
{
	mOutDataList.clear();
}

void CEngValCnvDll::AddMonitorName(TCHAR* name)
{
	CString tag(name);
	vector<xobs_tbl_res_t> val;
	map< CString, vector<xobs_tbl_res_t> >::iterator itr = mOutDataList.find(tag);
	if (itr == mOutDataList.end())
		mOutDataList.insert(map< CString, vector<xobs_tbl_res_t> >::value_type(tag, val));
}

void CEngValCnvDll::ClearDataFileList()
{
	mMonDataFileList.clear();
}

void CEngValCnvDll::AddDataFileList(TCHAR* val)
{
	TCHAR buf[_MAX_PATH];
	swprintf_s(buf, _MAX_PATH, _T("%s"), val);
	mMonDataFileList.push_back(CString(buf));
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �����R�[�h�ϊ�

@param	src		�ϊ��O������
@param	dest	�ԊҌ㕶����

@retval	bool

*/
/*============================================================================*/
bool CEngValCnvDll::ConvertStatusChar(char* stsname, int len)
{
	char src[64];
	char dest[64];
	memcpy(src, stsname, 64);
	memset(dest, 0, 64);

#ifdef _ALLCODE
	int srcpos = 0, destpos = 0;
	bool error = false;
	CStringA str;
	while (srcpos < len){
		if (src[srcpos] == 0x00){
			dest[destpos++] = src[srcpos++];
			break;
		}
		str = UTF8_3CharsToSJis(&src[srcpos]);
		if (str == ""){
			error = true;
			break;
			//memcpy(&dest[destpos], (char*)(const char*)str, 1);
			//srcpos += 1;
			//destpos += 1;
		}
		else{
			memcpy(&dest[destpos], (char*)(const char*)str, 2);
			srcpos += 3;
			destpos += 2;
		}
	}
	if (error == true){
		srcpos = 0, destpos = 0;
		error = false;
		while (srcpos < len){
			if (src[srcpos] == 0x00){
				dest[destpos++] = src[srcpos++];
				break;
			}
			str = EUCToSJis(&src[srcpos]);
			if (str == ""){
				if (srcpos == 0){
					// �擪������EUC�ł͂Ȃ��̂ŃG���[�Ƃ���
					error = true;
					break;
				}
				memcpy(&dest[destpos], &src[srcpos], 1);
				srcpos += 1;
				destpos += 1;
			}
			else{
				memcpy(&dest[destpos], (char*)(const char*)str, 2);
				srcpos += 2;
				destpos += 2;
			}
		}
	}
	if (error == false){
		memcpy(stsname, dest, 64);
		CStringA str = CStringA(stsname);
		str.TrimLeft();
		str.TrimRight();
		memset(stsname, 0, 64);
		memcpy(stsname, (char*)(const char*)str, str.GetLength());
	}
#else

#ifdef _DEBUG
	static bool bTest = false;
	const char mon[32] = {
		(char)0x56, (char)0x4C, (char)0x42, (char)0x49, (char)0xB1, (char)0xBF, (char)0xCD, (char)0xD1,
		(char)0xBB, (char)0xFE, (char)0xB4, (char)0xD6, (char)0x20, (char)0x20, (char)0x20, (char)0x20,
		(char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20,
		(char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x20, (char)0x00,
	};

	if (bTest == true)
		memcpy(src, mon, 32);
#endif

	// �����R�[�h��EUC�����`�F�b�N����
	int srcpos = 0, destpos = 0;
	UINT nConvMode = 1; // EUC�t���O�𗧂Ă�
	while (1){
		if (src[srcpos] == 0x00){
			dest[destpos++] = src[srcpos++];
			break;
		}
		if (src[srcpos] >= 0x00 && src[srcpos] <= 0x7F){
			// ASCII�R�[�h
			dest[destpos++] = src[srcpos++];
		}
		else{
			if ((UCHAR)src[srcpos] >= 0xA1 && (UCHAR)src[srcpos] <= 0xFE &&
				(UCHAR)src[srcpos + 1] >= 0xA1 && (UCHAR)src[srcpos + 1] <= 0xFE){
				// EUC�̂Q�o�C�g����
				srcpos += 2;
				destpos += 2;
			}
			else{
				// EUC2�o�C�g�����ł͂Ȃ�
				nConvMode = 2;
				break;
			}
		}
	}

	memset(dest, 0, 64);

	srcpos = 0;
	destpos = 0;
	bool error = false;
	CStringA str;
	while (1){
		if (src[srcpos] == 0x00){
			dest[destpos++] = src[srcpos++];
			break;
		}
		if (src[srcpos] >= 0x00 && src[srcpos] <= 0x7F){
			// ASCII�R�[�h
			dest[destpos++] = src[srcpos++];
		}
		else{
			if (nConvMode == 1){
				// EUC�̂Q�o�C�g����
				str = EUCToSJis(&src[srcpos]);
				if (str == ""){
					error = true;
					break;
				}
				else{
					memcpy(&dest[destpos], (char*)(const char*)str, 2);
					srcpos += 2;
					destpos += 2;
				}
			}
			if (nConvMode == 2){
				// UTF-8
				str = UTF8_3CharsToSJis(&src[srcpos]);
				if (str == ""){
					error = true;
					break;
				}
				else{
					memcpy(&dest[destpos], (char*)(const char*)str, 2);
					srcpos += 3;
					destpos += 2;
				}
			}
		}
	}
	if (error == false){
		memcpy(stsname, dest, 64);
		str = CStringA(stsname);
		str.TrimLeft();
		str.TrimRight();
		memset(stsname, 0, 64);
		memcpy(stsname, (char*)(const char*)str, str.GetLength());
	}
#endif
	return !error;
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# ���ړ��t����͈͓��t�ւ̕ϊ�

@param	dtime		���ړ��t
@param	rtime		�͈͓��t
@param	timezero	true�̏ꍇ�ϊ���̎����b�~���b���[���ɂ���

@retval	bool

*/
/*============================================================================*/
bool CEngValCnvDll::ConvertDirectRangeTime(xTL_TIME& dtime, xTL_TIME& rtime, bool timezero/*=true*/)
{
	CTimeSpan daySpan(1, 0, 0, 0);
	SYSTEMTIME sttime, sptime;
	memset(&sttime, 0, sizeof(SYSTEMTIME));
	memset(&sptime, 0, sizeof(SYSTEMTIME));
	if (dtime.wYear < 1970 || dtime.wYear > 3000)
		return false;
	sttime.wYear = dtime.wYear;
	sptime.wYear = dtime.wYear;
	if (dtime.wMonth < 0 || dtime.wMonth > 12)
		return false;
	sttime.wMonth = dtime.wMonth;
	sptime.wMonth = dtime.wMonth;
	if (dtime.wMonth == 0){
		sttime.wMonth = 1;
		sptime.wMonth = 12;
	}
	if (dtime.wDay < 0 || dtime.wDay > 31)
		return false;
	sttime.wDay = dtime.wDay;
	sptime.wDay = dtime.wDay;
	if (dtime.wDay == 0){
		sttime.wDay = 1;
		sptime.wDay = 1;
		if (sptime.wMonth == 12){
			sptime.wYear++;
			sptime.wMonth = 1;
		}
		else{
			sptime.wMonth++;
		}
		CTime tm(sptime);
		tm -= daySpan;
		if (tm.GetAsSystemTime(sptime) == false){
			memset(&sptime, 0, sizeof(SYSTEMTIME));
		}
	}

	if (timezero == false){
		sptime.wHour = 23;
		sptime.wMinute = 59;
		sptime.wSecond = 59;
		sptime.wMilliseconds = 999;
	}

	CTime st = CTime(sttime);
	CTime sp = CTime(sptime);

	rtime.sttime = st.GetTime();
	rtime.sptime = sp.GetTime();

	return true;
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �w����Ԃ̊Ď��f�[�^�t�@�C����ǂݍ��݁A�w�肳�ꂽ�Ď����ɑΉ�����f�[�^���擾����

@param	sttime		�J�n����
@param	sptime		�I������
@param	fileList	�t�@�C���p�X�ꗗ�i�o�́j

@retval	void

*/
/*============================================================================*/
void CEngValCnvDll::GetTimeRangePath(time_t sttime, time_t sptime, vector<CString>& fileList)
{
	fileList.clear();

	CTimeSpan daySpan(1, 0, 0, 0);
	CTime time;
	CString str;
	for (time_t tm = sttime; tm <= sptime;){
		// ���t������t�p�X�����߂�
		time = CTime(tm);
		str = time.Format("%Y-%m-%d");

		fileList.push_back(str);

		time += daySpan;
		tm = time.GetTime();
	}
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �w��t�H���_�̃t�@�C���ꗗ���擾����

@param	path	�t�H���_��
@param	outFileList	�Ď��t�@�C�����X�g(�o��)

@retval

*/
/*============================================================================*/
void CEngValCnvDll::GetDataFileList(CString& path, vector<CString>& outFileList)
{
	// �Ώۃt�H���_���̃t�@�C�������擾����
	// �w�肳�ꂽ�t�@�C�����Ɉ�v����t�@�C�����A�f�B���N�g�����Ō�������
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(path+_T("*"), &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return;
	}

	UINT item = 0;
	do{
		TRY
		{
			CString fname;
			if (ffdata.cFileName[0] == '\0')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '.')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '[')
			{
				AfxThrowUserException();
			}

			// �t�@�C�����擾
			if (ffdata.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				CString name = CString(ffdata.cFileName);
				if (name.Find(_T(".")) >= 0){
					continue;
				}
				if (name.Left(3).CompareNoCase(_T("NON")) == 0){
					continue;
				}
				outFileList.push_back(path + name);
			}
		}
		CATCH(CUserException, e)
		{
		}
		AND_CATCH(CException, e)
		{
		}
		END_CATCH
	} while (FindNextFile(hFindFile, &ffdata));

	FindClose(hFindFile);
}
/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# UTF8�R�[�h��SJIS�ɕϊ�

@param	pUtf8Str			UTF-8

@retval	CString
*/
/*============================================================================*/
CStringA CEngValCnvDll::UTF8_3CharsToSJis(char *pUtf8Str)
{
	wchar_t pUtfMsg[4];

	if (pUtf8Str[0] >= 0x00 && pUtf8Str[0] <= 0x7f){
		// 1�o�C�g����
		return "";
	}

	// UTF8 �̂R�o�C�g�����C�h�L�����N�^�֍���
	pUtfMsg[0] = ((pUtf8Str[0] & 0x0f) << 12)
		| ((pUtf8Str[1] & 0x3f) << 6)
		| (pUtf8Str[2] & 0x3f);
	pUtfMsg[1] = 0x0000;

	size_t wLen = 0;
	errno_t err = 0;

	//���P�[���w��
	char* back = setlocale(LC_ALL, "japanese");
	//�ϊ�
	char	sjis[10];
	err = wcstombs_s(&wLen, sjis, 10, pUtfMsg, _TRUNCATE);
#ifndef _DEBUG
	setlocale(LC_ALL, back);
#endif
	if (err != 0)
		return "";

	CStringA str(sjis);
	return str;
}
/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# EUC�R�[�h��SJIS�ɕϊ�

@param	pEucStr			EUC

@retval	CString
*/
/*============================================================================*/
CStringA CEngValCnvDll::EUCToSJis(char *pEucStr)
{
	// EUC�̂Q�o�C�g�������̃`�F�b�N
	if ((UCHAR)pEucStr[0] >= 0xA1 && (UCHAR)pEucStr[0] <= 0xFE &&
		(UCHAR)pEucStr[1] >= 0xA1 && (UCHAR)pEucStr[1] <= 0xFE){
		// �����R�[�h�͂Q�o�C�g����
	}
	else{
		return "";
	}

	char	sjis[10];

	// EUC-JIS
	sjis[0] = pEucStr[0] & ~0x80;
	sjis[1] = pEucStr[1] & ~0x80;
	sjis[2] = '\0';
	// JIS-SJIS
	if (sjis[0] & 0x01){
		sjis[0] >>= 1;
		if (sjis[0] < 0x2F)
			sjis[0] += 0x71;
		else
			sjis[0] -= 0x4F;

		if (sjis[1] > 0x5F)
			sjis[1] += 0x20;
		else
			sjis[1] += 0x1F;
	}
	else{
		sjis[0] >>= 1;
		if (sjis[0] < 0x2F)
			sjis[0] += 0x70;
		else
			sjis[0] -= 0x50;
		sjis[1] += 0x7E;
	}
	CStringA str(sjis);

	return str;
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �Ď����ږ�

@param	path	�t�H���_��
@param	outFileList	�Ď��t�@�C�����X�g(�o��)

@retval

*/
/*============================================================================*/
bool CEngValCnvDll::IsObsName(char* obsname, map< CString, vector<xobs_tbl_res_t> >& outDataList)
{
	CStringA str(obsname);
	CString wstr = CString(str);
	map< CString, vector<xobs_tbl_res_t> >::iterator itrn;
	itrn = outDataList.find(wstr);
	if (itrn == outDataList.end()){
		// �Ď��������݂��Ȃ��̂ŉ������Ȃ�
		return false;
	}
	return true;
}






int CEngValCnvDll::DbRead(char* db_file_nam, char *cp_loadadr_p)
{
	int errno;
	struct stat stbuf;
	int    i_readbyte;
	long   l_alldatasize = 0, l_datasize;
	char   sz_work[256] = { 0 };

	unsigned long ul_top_pos; /*�t�@�C���̐擪���烁�������[�h���e�̐擪�܂ł̃o�C�g��*/
	long   l_idnum = 0;
	FILE *fp;
	char *fname = db_file_nam;

	/*�t�@�C�����Ƃ݂Ȃ�*/
	if (stat(fname, &stbuf) == -1){
		return(500);
	}

	l_datasize = stbuf.st_size;


	if (l_idnum == 0) {
		l_alldatasize += l_datasize;
	}
	else {
		l_alldatasize += (l_datasize * l_idnum);
	}

	errno_t ans;
	/*cp_loadadr_p = cp_loadadr;*/
	if (((ans = fopen_s(&fp, fname, "rb")) != 0) || (fp == NULL)){
		return(500);
	}
	if (fread(sz_work, sizeof(char), sizeof(sz_work), fp) == -1){/*#!HEAD:�����邩�ǂ����ǂ�ł݂�*/
		fclose(fp); /*�t�@�C����close����*/
		return(500);
	}
	if (!strncmp(sz_work, "#!HEAD:", 7)){ /*#!HEAD:��ǂݔ�΂�*/
		if ((ul_top_pos = (unsigned long)strchr(sz_work, '\n')) == NULL){
			fclose(fp); /*�t�@�C����close����*/
			return(500);
		}
		else{
			ul_top_pos = (ul_top_pos - (unsigned long)&sz_work[0]) + 1;
		}
	}
	else{ /*�t�@�C���̐擪����S���e��ǂݍ���*/
		ul_top_pos = 0; /*�t�@�C���̐擪���烁�����Ƀ��[�h������e�̐擪�܂ł̃o�C�g��*/
	}
	if (fseek(fp, ul_top_pos, SEEK_SET) == -1){
		fclose(fp); /*�t�@�C����close����*/
		return(500);
	}
	for (;;){
		i_readbyte = (int)fread(cp_loadadr_p, sizeof(char), (l_datasize - ul_top_pos), fp);
		if (i_readbyte <= 0){ break; }
		if (i_readbyte == (l_datasize - ul_top_pos)){ break; }
		l_datasize -= i_readbyte;

		cp_loadadr_p = cp_loadadr_p + i_readbyte;
	}
	cp_loadadr_p = cp_loadadr_p + (l_datasize - ul_top_pos);
	l_alldatasize -= ul_top_pos; /*#!HEAD:�̕����̃o�C�g����␳*/
	fclose(fp); /*�t�@�C����close����*/

	return 0;
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �Ď��f�[�^�t�@�C����ǂݍ���
�@�@�����Ӂ��Ăяo�����ƂŖ߂�l��TL_TLMDATA*�A�h���X���폜���邱��

  @param	fp	�t�@�C���|�C���^
  @retval	TL_TLMDATA* �Ď��f�[�^�|�C���^

  */
/*============================================================================*/
void* CEngValCnvDll::MonRead(FILE **fp)
{
	size_t i_readbyte2;

	xTL_INFO info;
	xTL_TLMDATA* pdata;

	// �ŏ��Ɂu���t�B�[���h�v��ǂݍ���Body�T�C�Y���擾����
	i_readbyte2 = fread(&info, sizeof(xTL_INFO), 1, *fp);
	if (i_readbyte2 <= 0){
		return NULL;
	}

	//�G���f�B�A���ϊ�
#if 1 //1=big-endianness 
	info.l_reclen = htonl(info.l_reclen);
#else
	info.l_reclen = info.l_reclen;
#endif

	// Body�T�C�Y���킩�����̂ŁA�T�C�Y�����������m�ۂ��āu�t�����v�{�u�e�����g����M�f�[�^�v���擾����
	pdata = (xTL_TLMDATA*)::malloc(sizeof(xTL_INFO) + info.l_reclen);
	if (pdata == NULL)
		return NULL;

	memcpy(&(pdata->info), &info, sizeof(xTL_INFO));
	i_readbyte2 = fread(&pdata->addinfo.uc_ver, info.l_reclen, 1, *fp);
	if (i_readbyte2 <= 0)
	{
		free(pdata);
		return NULL;
	}

	return (void*)pdata;
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �Ď��~�σt�@�C������H�w�l�ϊ����s��(�g�����h�O���t�p)

@param	nama_data_file	�Ď��~�σt�@�C����
@param	outDataList		�ΏۊĎ����ꗗ
@retval	int

*/
/*============================================================================*/
int CEngValCnvDll::CalcFromFileEx(char* nama_data_file, map< CString, vector<xobs_tbl_res_t> >& outDataList)
{
	struct xobsname_t *obs_adrs;
	struct xext_tbl_t *ext_adr;
	char      *mon_adr;
	long      l_rc;

	long      l_dataid;
	int       l_inflg;
	int       l_grmd;
	double    d_data;
	long double ld_data;/* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
	long      l_bitinfo;   /* �����r�b�g��:�������o�f�[�^�̃r�b�g�� */
	char      sz_work[STR_BUF_SIZE];
	std::vector<xobs_tbl_res_t>::iterator itr;


	if (db_status != 0){
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: Error db_status(%d)"), __FILEW__, __LINE__, db_status);
//			CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
		}
		return -1;
	}

	memcpy_s(db_data, mFileSize, db_data2, mFileSize);

	/*----- �e�Ď����ڃf�[�^�擾���� -----*/
	mon_adr = db_data;
	mon_adr += (sizeof(long) * 2);

	data_list.clear();

	FILE *fp = NULL;
	bool bRead = true;
	int ret = 0;

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();
	const CStringA mStdMonTime = "MON_TIME";

	try{
		int l = 0;
		while (1){
			obs_adrs = (struct xobsname_t *)mon_adr;

			long ed_id = ds_htonl(obs_adrs->l_id);
			long ed_len = ds_htonl(obs_adrs->l_len);

			if (ed_id == 0) {
				break;
			}

			// ���Ԃ��擾���邽�߂ɁuMON_TIME�v�͕K�v�Ȃ̂ŁA
			// �ݔ����́uMON_TIME�v�͓o�^����
			CStringA str = CStringA(obs_adrs->sz_obsname);
			if (str.Find(mStdMonTime) < 0){
				// �ΏۊĎ����ږ������݂��邩�̃`�F�b�N
				if (IsObsName(obs_adrs->sz_obsname, outDataList) == false){
					mon_adr += ed_len;
					continue;
				}
			}

			if (ed_id == OBSNAME_ID)
			{
				obs_tbl[l].obs_adr = obs_adrs;
				l_rc = ud_ext_adr(obs_tbl[l].obs_adr->sz_obsname, obs_adrs, &ext_adr);
				if (l_rc == 0) {
					obs_tbl[l].l_dataid = ds_htonl(ext_adr->s_pwb[0].l_pid);
					long ed_max = ds_htonl(ext_adr->l_pw_cnt);
					for (int m = 1; m < ed_max; m++) {
						if (obs_tbl[l].l_dataid != ds_htonl(ext_adr->s_pwb[m].l_pid)) {
							obs_tbl[l].l_dataid = -1;
							break;
						}
					}
				}
				else {
					obs_tbl[l].l_dataid = -2;
					obs_tbl[l].obs_adr->l_no = -1;
				}

				obs_tbl[l].l_offset = (long)(mon_adr - db_data);
				obs_tbl[l].obs_adr->l_inflg = 0;
				l++;
				if (l >= OBS_MAX) {
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: Error over OBS_MAX(%d)"), __FILEW__, __LINE__, l);
//						CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
					}
					return(-1);
				}
			}
			mon_adr += ed_len;
		}
		obs_tbl[l].obs_adr = NULL;
		obs_tbl[l].l_offset = NULL;


		/*--------------------------------------------------------*/
		/*    �Ď��f�[�^�ʒm                                      */
		/*--------------------------------------------------------*/
		int offset = 0;
		unsigned char* buff;
		long cnt = 0;
		char* mon_time_name = "MON_TIME";
		char   sz_work2[256] = { 0 };
		unsigned long ul_top_pos;

		// �Ď��f�[�^�̃I�[�v��
		if ((fopen_s(&fp, nama_data_file, "rb")) != 0){
			bRead = false;
		}
		// �u#!HEAD:�v�����݂���ꍇ�͓ǂݎ̂Ă�
		if ((fp == NULL) || (fread(sz_work2, sizeof(char), sizeof(sz_work2), fp) == -1)){/*#!HEAD:�����邩�ǂ����ǂ�ł݂�*/
			// �ǂݍ��݃G���[�Ȃ̂ŉ������Ȃ�
			if (fp != NULL)
			{
				fclose(fp); /*�t�@�C����close����*/
			}
			bRead = false;
		}
		else{
			// �u#!HEAD:�v�����݂����̂œǂݎ̂Ă�
			if (!strncmp(sz_work2, "#!HEAD:", 7)){ /*#!HEAD:��ǂݔ�΂�*/
				if ((ul_top_pos = (unsigned long)strchr(sz_work2, '\n')) == NULL){
					fclose(fp); /*�t�@�C����close����*/
					bRead = false;
				}
				else{
					// �Ď��f�[�^�̐擪�ʒu
					ul_top_pos = (ul_top_pos - (unsigned long)&sz_work2[0]) + 1;
				}
			}
			else{ /*�t�@�C���̐擪����S���e��ǂݍ���*/
				// �t�@�C���擪����Ď��f�[�^
				ul_top_pos = 0; /*�t�@�C���̐擪���烁�����Ƀ��[�h������e�̐擪�܂ł̃o�C�g��*/
			}
		}
		if (bRead == true){
			// �Ď��f�[�^�ʒu�܂ŃV�[�N����
			if (fseek(fp, ul_top_pos, SEEK_SET) == -1){
				fclose(fp); /*�t�@�C����close����*/
				bRead = false;
			}
		}

		bool bFirst = true;
		// �t�@�C��END�܂ňȉ����J��Ԃ�
		while (bRead)
		{
			// �uTL_TLMDATA�v�P�ʂŃf�[�^��ǂݍ���
			xTL_TLMDATA* pdata;
			pdata = (xTL_TLMDATA*)MonRead(&fp);
			if (pdata == NULL)
			{
				// �t�@�C���̏I�[���邢�͓ǂݍ��݃G���[�Ȃ̂ŏ������I��
				fclose(fp); /*�t�@�C����close����*/
				bRead = false;
				break;
			}

			//======================================================================
			//	��������ǂݍ��񂾊Ď��f�[�^���f�[�^���X�g�Ɋi�[����
			//======================================================================
			l_dataid = pdata->addinfo.uca_op_data_id[0];
			buff = (unsigned char *)&pdata->addinfo.uca_timetag;

//			long double time_val = 0;
			bool leapSec = false; // ���邤�b�t���O

			for (l = 0; obs_tbl[l].obs_adr != NULL; l++) {

				if (l_dataid == obs_tbl[l].l_dataid || obs_tbl[l].l_dataid == -1) {

					// ���ۂ̍H�w�l�ϊ�
					memset(sz_work, 0, STR_BUF_SIZE);
					l_rc = ud_montbl_data64(db_data, l_dataid, buff,
						obs_tbl[l].l_offset,
						&l_inflg,
						&l_grmd,
						&d_data,
						sz_work,
						&ld_data,
						&l_bitinfo);

					if (l_rc == 0 && l_inflg == 1) {
						obs_tbl[l].l_grp_cnt = cnt;
						obs_tbl[l].obs_adr->l_id = ds_htonl(obs_tbl[l].obs_adr->l_id);
						obs_tbl[l].obs_adr->l_len = ds_htonl(obs_tbl[l].obs_adr->l_len);
						obs_tbl[l].obs_adr->l_no = ds_htonl(obs_tbl[l].obs_adr->l_no);
						obs_tbl[l].obs_adr->l_inflg = l_inflg;
						obs_tbl[l].obs_adr->l_grmd = l_grmd;
						obs_tbl[l].obs_adr->d_data = d_data;
						obs_tbl[l].obs_adr->ld_data = ld_data;  /* 64bit���x�g�� */
						obs_tbl[l].obs_adr->l_bitinfo = l_bitinfo; /* 64bit���x�g�� */
						obs_tbl[l].obs_adr->l_cnvResType = GetCnvResType();
						strcpy_s(obs_tbl[l].obs_adr->sz_sts_name, sizeof(obs_tbl[l].obs_adr->sz_sts_name), sz_work);

						//���ʂ��i�[
						xobs_tbl_res_t res;
						res.l_dataid = obs_tbl[l].l_dataid;
						res.l_grp_cnt = obs_tbl[l].l_grp_cnt;
						res.l_offset = obs_tbl[l].l_offset;
						res.l_dataid = obs_tbl[l].l_dataid;
						memcpy_s(&res.obs_adr, sizeof(xobsname_t), obs_tbl[l].obs_adr, sizeof(xobsname_t));

						CString	tmp_obsname = CString(res.obs_adr.sz_obsname);
						CString	tmp_watch = tmp_obsname.Left(tmp_obsname.Find('.'));

						// ����O���[�v�𓯈ꎞ�Ԃɐݒ肷��
						char* ans = strstr(res.obs_adr.sz_obsname, mon_time_name);
						if (ans != NULL){
#if 0
							time_val = res.obs_adr.ld_data;
#else
							GetAndUpdateMonTime(tmp_watch, res.obs_adr.ld_data);
#endif
							// ���邤�b�̊m�F
							CString str = CString(res.obs_adr.sz_sts_name);
							if (str.Right(2) == _T("60")){
								leapSec = true;
							}
						}

#if 0
						res.ld_time_val = time_val;
#else
						res.ld_time_val = GetAndUpdateMonTime(tmp_watch);
#endif

						if (leapSec == true){
							res.obs_adr.l_cnvResType |= 1 << eCnvResType_TimeLeapSec;
						}

						if (bFirst == true){
#if 0
							// ���񂾂��A���ł�data_list�ɓo�^����Ă���f�[�^�𓯈ꎞ�Ԃɐݒ肷��
							vector<xobs_tbl_res_t>::iterator itr;
							for (itr = data_list.begin(); itr != data_list.end(); itr++){
								(*itr).ld_time_val = time_val;
							}
#endif
							bFirst = false;
						}

						// �ΏۊĎ����ږ������݂��邩�̃`�F�b�N
						if (IsObsName(res.obs_adr.sz_obsname, outDataList) == true){
							// �ϊ����ʃX�e�[�^�X������̃R�[�h�ύX�iUTF-8->SJIS EUC-JI->SJIS�j
							if (outDataList.size() == 0)	// setlocal�ŗ����邽��
								ConvertStatusChar(res.obs_adr.sz_sts_name, 64);

							// �f�[�^���X�g�Ɋi�[
							if (mCallBack == NULL){
								data_list.push_back(res);
							}
							else{
								CString obsname = CString(res.obs_adr.sz_obsname);
								TCHAR buf[40];
								swprintf_s(buf, 40, _T("%s"), (LPCTSTR)obsname);
								mCallBack(buf, (time_t)res.ld_time_val, &res, leapSec);
							}
						}
					}
				}
			} //for (l = 0; obs_tbl[l].obs_adr != NULL; l++) {

			//�O���[�v�J�E���g�A�b�v
			cnt++;

			// �������폜
			delete pdata;

		} //while (bRead)

		ret = 0;
	}
	catch (...){
		TRACE("@@@ CalcFromFileEx Exception\n");
		if (bRead == true){
			fclose(fp);
		}
		ret = -1;
	}

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### CalcFromFileEx(%s) = %.3lf(sec)\n", nama_data_file, (double)((double)dwElapsed / 1000.0));

 	timeEndPeriod(1);

	return ret;
}

/*============================================================================*/
/*! �Ď��f�[�^�A�N�Z�X

-# �Ď��~�σt�@�C������H�w�l�ϊ����s��

@param	nama_data_file	�Ď��~�σt�@�C����
@param	outDataList		�ΏۊĎ����ꗗ
@retval	int

*/
/*============================================================================*/
int CEngValCnvDll::CalcFromFile(char* nama_data_file)
{
	struct xobsname_t *obs_adrs;
	struct xext_tbl_t *ext_adr;
	char      *mon_adr;
	long      l_rc;

	long      l_dataid;
	int       l_inflg;
	int       l_grmd;
	double    d_data;
	long double ld_data;/* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
	long      l_bitinfo;   /* �����r�b�g��:�������o�f�[�^�̃r�b�g�� */
	char      sz_work[STR_BUF_SIZE];
	std::vector<xobs_tbl_res_t>::iterator itr;


	if (db_status != 0){
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: Error db_status(%d)"), __FILEW__, __LINE__, db_status);
			//			CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
		}
		return -1;
	}

	memcpy_s(db_data, mFileSize, db_data2, mFileSize);

	/*----- �e�Ď����ڃf�[�^�擾���� -----*/
	mon_adr = db_data;
	mon_adr += (sizeof(long) * 2);

	data_list.clear();

	FILE *fp = NULL;
	bool bRead = true;
	int ret = 0;

	// �ŏ��^�C�}����\��ݒ�(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	try{
		int l = 0;
		int t = 0;
		while (1){
			obs_adrs = (struct xobsname_t *)mon_adr;

			long ed_id = ds_htonl(obs_adrs->l_id);
			long ed_len = ds_htonl(obs_adrs->l_len);

			t++;

			if (ed_id == 0) {
				break;
			}
			if (ed_id == OBSNAME_ID)
			{
				obs_tbl[l].obs_adr = obs_adrs;
				l_rc = ud_ext_adr(obs_tbl[l].obs_adr->sz_obsname, obs_adrs, &ext_adr);
				if (l_rc == 0) {
					obs_tbl[l].l_dataid = ds_htonl(ext_adr->s_pwb[0].l_pid);
					long ed_max = ds_htonl(ext_adr->l_pw_cnt);
					for (int m = 1; m < ed_max; m++) {
						if (obs_tbl[l].l_dataid != ds_htonl(ext_adr->s_pwb[m].l_pid)) {
							obs_tbl[l].l_dataid = -1;
							break;
						}
					}
				}
				else {
					obs_tbl[l].l_dataid = -2;
					obs_tbl[l].obs_adr->l_no = -1;
				}

				obs_tbl[l].l_offset = (long)(mon_adr - db_data);
				obs_tbl[l].obs_adr->l_inflg = 0;
				l++;
				if (l >= OBS_MAX) {
					{
						CString strDebug;
						strDebug.Format(_T("%s(%d) :: Error over OBS_MAX(%d)"), __FILEW__, __LINE__, l);
						//						CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
					}
					return(-1);
				}
			}
			mon_adr += ed_len;
		}
		obs_tbl[l].obs_adr = NULL;
		obs_tbl[l].l_offset = NULL;


		/*--------------------------------------------------------*/
		/*    �Ď��f�[�^�ʒm                                      */
		/*--------------------------------------------------------*/
		int offset = 0;
		unsigned char* buff;
		long cnt = 0;
		char* mon_time_name = "MON_TIME";
		char   sz_work2[256] = { 0 };
		unsigned long ul_top_pos;

		// �Ď��f�[�^�̃I�[�v��
		if ((fopen_s(&fp, nama_data_file, "rb")) != 0){
			bRead = false;
		}
		// �u#!HEAD:�v�����݂���ꍇ�͓ǂݎ̂Ă�
		if ((fp == NULL) || (fread(sz_work2, sizeof(char), sizeof(sz_work2), fp) == -1)){/*#!HEAD:�����邩�ǂ����ǂ�ł݂�*/
			// �ǂݍ��݃G���[�Ȃ̂ŉ������Ȃ�
			if (fp != NULL)
			{
				fclose(fp); /*�t�@�C����close����*/
			}
			bRead = false;
		}
		else{
			// �u#!HEAD:�v�����݂����̂œǂݎ̂Ă�
			if (!strncmp(sz_work2, "#!HEAD:", 7)){ /*#!HEAD:��ǂݔ�΂�*/
				if ((ul_top_pos = (unsigned long)strchr(sz_work2, '\n')) == NULL){
					if (fp != NULL)
					{
						fclose(fp); /*�t�@�C����close����*/
					}
					bRead = false;
				}
				else{
					// �Ď��f�[�^�̐擪�ʒu
					ul_top_pos = (ul_top_pos - (unsigned long)&sz_work2[0]) + 1;
				}
			}
			else{ /*�t�@�C���̐擪����S���e��ǂݍ���*/
				// �t�@�C���擪����Ď��f�[�^
				ul_top_pos = 0; /*�t�@�C���̐擪���烁�����Ƀ��[�h������e�̐擪�܂ł̃o�C�g��*/
			}
		}
		if (bRead == true){
			// �Ď��f�[�^�ʒu�܂ŃV�[�N����
			if (fseek(fp, ul_top_pos, SEEK_SET) == -1){
				fclose(fp); /*�t�@�C����close����*/
				bRead = false;
			}
		}

		bool bFirst = true;
		// �t�@�C��END�܂ňȉ����J��Ԃ�
		while (bRead)
		{
			// �uTL_TLMDATA�v�P�ʂŃf�[�^��ǂݍ���
			xTL_TLMDATA* pdata;
			pdata = (xTL_TLMDATA*)MonRead(&fp);
			if (pdata == NULL)
			{
				TRACE("### CalcFromFile(%s) Error\n", nama_data_file);
				// �t�@�C���̏I�[���邢�͓ǂݍ��݃G���[�Ȃ̂ŏ������I��
				fclose(fp); /*�t�@�C����close����*/
				bRead = false;
				break;
			}

			//======================================================================
			//	��������ǂݍ��񂾊Ď��f�[�^���f�[�^���X�g�Ɋi�[����
			//======================================================================
			l_dataid = pdata->addinfo.uca_op_data_id[0];
			buff = (unsigned char *)&pdata->addinfo.uca_timetag;

			long double time_val = 0;
			bool leapSec = false; // ���邤�b�t���O

			for (l = 0; obs_tbl[l].obs_adr != NULL; l++) {
				if (l_dataid == obs_tbl[l].l_dataid || obs_tbl[l].l_dataid == -1) {
					memset(sz_work, 0, STR_BUF_SIZE);
					l_rc = ud_montbl_data64(db_data, l_dataid, buff,
						obs_tbl[l].l_offset,
						&l_inflg,
						&l_grmd,
						&d_data,
						sz_work,
						&ld_data,
						&l_bitinfo);

					if (l_rc == 0 && l_inflg == 1) {
						obs_tbl[l].l_grp_cnt = cnt;
						obs_tbl[l].obs_adr->l_id = ds_htonl(obs_tbl[l].obs_adr->l_id);
						obs_tbl[l].obs_adr->l_len = ds_htonl(obs_tbl[l].obs_adr->l_len);
						obs_tbl[l].obs_adr->l_no = ds_htonl(obs_tbl[l].obs_adr->l_no);
						obs_tbl[l].obs_adr->l_inflg = l_inflg;
						obs_tbl[l].obs_adr->l_grmd = l_grmd;
						obs_tbl[l].obs_adr->d_data = d_data;
						obs_tbl[l].obs_adr->ld_data = ld_data;  /* 64bit���x�g�� */
						obs_tbl[l].obs_adr->l_bitinfo = l_bitinfo; /* 64bit���x�g�� */
						obs_tbl[l].obs_adr->l_cnvResType = GetCnvResType();
						strcpy_s(obs_tbl[l].obs_adr->sz_sts_name, sizeof(obs_tbl[l].obs_adr->sz_sts_name), sz_work);

						//���ʂ��i�[
						xobs_tbl_res_t res;
						res.l_dataid = obs_tbl[l].l_dataid;
						res.l_grp_cnt = obs_tbl[l].l_grp_cnt;
						res.l_offset = obs_tbl[l].l_offset;
						res.l_dataid = obs_tbl[l].l_dataid;
						memcpy_s(&res.obs_adr, sizeof(xobsname_t), obs_tbl[l].obs_adr, sizeof(xobsname_t));

						// ����O���[�v�𓯈ꎞ�Ԃɐݒ肷��
						char* ans = strstr(res.obs_adr.sz_obsname, mon_time_name);
						if (ans != NULL){
							time_val = res.obs_adr.ld_data;
							// ���邤�b�̊m�F
							CString str = CString(res.obs_adr.sz_sts_name);
							if (str.Right(2) == _T("60")){
								leapSec = true;
							}
						}
						res.ld_time_val = time_val;
						if (leapSec == true){
							res.obs_adr.l_cnvResType |= 1 << eCnvResType_TimeLeapSec;
						}

						if (bFirst == true){
							// ���񂾂��A���ł�data_list�ɓo�^����Ă���f�[�^�𓯈ꎞ�Ԃɐݒ肷��
							vector<xobs_tbl_res_t>::iterator itr;
							for (itr = data_list.begin(); itr != data_list.end(); itr++){
								(*itr).ld_time_val = time_val;
								if (leapSec == true){
									res.obs_adr.l_cnvResType |= 1 << eCnvResType_TimeLeapSec;
								}
							}
							bFirst = false;
						}

						// �ϊ����ʃX�e�[�^�X������̃R�[�h�ύX�iUTF-8->SJIS EUC-JI->SJIS�j
						ConvertStatusChar(res.obs_adr.sz_sts_name, 64);

						// �f�[�^���X�g�Ɋi�[
						data_list.push_back(res);

					}
				}
			} //for (l = 0; obs_tbl[l].obs_adr != NULL; l++) {

			//�O���[�v�J�E���g�A�b�v
			cnt++;

			// �������폜
			delete pdata;

		} //while (bRead)

		ret = 0;
	}
	catch (...){
		TRACE("@@@ CalcFromFile Exception\n");
		if (bRead == true){
			fclose(fp);
		}
		ret = -1;
	}

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### CalcFromFile(%s) = %.3lf(sec)\n", nama_data_file, (double)((double)dwElapsed / 1000.0));

	timeEndPeriod(1);

	return ret;
}

int CEngValCnvDll::CalcFromMsg(xsdtp_msg_ex_t* msg)
{
	char      *mon_adr;
	long      l_dsu;

	int       l_inflg;
	int       l_grmd;
	double    d_data;
	long double ld_data;/* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
	long      l_bitinfo;   /* �����r�b�g��:�������o�f�[�^�̃r�b�g�� */
	char      sz_work[STR_BUF_SIZE];
	long      l_rc;
	std::vector<xobs_tbl_res_t>::iterator itr;


	if (db_status != 0){
		{
			CString strDebug;
			strDebug.Format(_T("%s(%d) :: Error db_status(%d)"), __FILEW__, __LINE__, db_status);
//			CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
		}
		return -1;
	}

	// �������A������DB�ɒ��ڒl����������ł���炵���̂ŁA���������Ȃ��B
	//memcpy_s(db_data, mFileSize, db_data2, mFileSize);

	/*----- �e�Ď����ڃf�[�^�擾���� -----*/
	mon_adr = db_data;
	mon_adr += (sizeof(long) * 2);

	data_list.clear();

	try{
		int l = 0;
		int t = 0;
		bool bFirst = true;

		/*--------------------------------------------------------*/
		/*    �Ď��f�[�^�ʒm                                      */
		/*--------------------------------------------------------*/
		//		if (msg->l_objtype == DATA_PDU)
		{
			l_dsu = msg->uca_SdtpRecNum[0] * 256 + msg->uca_SdtpRecNum[1];

			int j = 0;
			long k = 0;
			long cnt = 0;
			char* mon_time_name = "MON_TIME";

//			long double time_val = 0;
			for (int i = 0; i < l_dsu; i++) {
				long l_dataid = msg->uca_buff[j + 1];

				vector<xobs_tbl_t> &list = obs_map[l_dataid];
				for (int l = 0; l < (int)list.size(); ++l) {
					xobs_tbl_t st = list[l];
					if (l_dataid == st.l_dataid || st.l_dataid == -1) {

						memset(sz_work, 0, STR_BUF_SIZE);
						l_rc = ud_montbl_data64(db_data, l_dataid, &msg->uca_buff[j + 12],
							st.l_offset,
							&l_inflg,
							&l_grmd,
							&d_data,
							sz_work,
							&ld_data,
							&l_bitinfo);

						if (l_rc == 0 && l_inflg == 1) {
							st.l_grp_cnt = cnt;
							st.obs_adr->l_id = ds_htonl(st.obs_adr->l_id);
							st.obs_adr->l_len = ds_htonl(st.obs_adr->l_len);
							st.obs_adr->l_no = ds_htonl(st.obs_adr->l_no);
							st.obs_adr->l_inflg = l_inflg;
							st.obs_adr->l_grmd = l_grmd;
							st.obs_adr->d_data = d_data;
							st.obs_adr->ld_data = ld_data;  /* 64bit���x�g�� */
							st.obs_adr->l_bitinfo = l_bitinfo; /* 64bit���x�g�� */
							st.obs_adr->l_cnvResType = GetCnvResType();
							strcpy_s(st.obs_adr->sz_sts_name, sizeof(st.obs_adr->sz_sts_name), sz_work);


							//���ʂ��i�[
							xobs_tbl_res_t res;
							res.l_dataid = st.l_dataid;
							res.l_grp_cnt = st.l_grp_cnt;
							res.l_offset = st.l_offset;
//							res.l_cnvResType = GetCnvResType();
							memcpy_s(&res.obs_adr, sizeof(xobsname_t), st.obs_adr, sizeof(xobsname_t));

							CString	tmp_obsname = CString(res.obs_adr.sz_obsname);
							CString	tmp_watch = tmp_obsname.Left(tmp_obsname.Find('.'));

							// ����O���[�v�𓯈ꎞ�Ԃɐݒ肷��
							char* ans = strstr(res.obs_adr.sz_obsname, mon_time_name);
							if (ans != NULL){
#if 0
								time_val = res.obs_adr.ld_data;
#else
								GetAndUpdateMonTime(tmp_watch, res.obs_adr.ld_data);
#endif
							}

#if 0
							res.ld_time_val = time_val;
#else
							res.ld_time_val = GetAndUpdateMonTime(tmp_watch);
#endif

							if (bFirst == true){
#if 0
								// ���񂾂��A���ł�data_list�ɓo�^����Ă���f�[�^�𓯈ꎞ�Ԃɐݒ肷��
								vector<xobs_tbl_res_t>::iterator itr;
								for (itr = data_list.begin(); itr != data_list.end(); itr++){
									(*itr).ld_time_val = time_val;
								}
#endif
								bFirst = false;
							}

							// �ϊ����ʃX�e�[�^�X������̃R�[�h�ύX�iUTF-8->SJIS EUC-JI->SJIS�j
							ConvertStatusChar(res.obs_adr.sz_sts_name, 64);

							data_list.push_back(res);
						}
					}
				}

				////MONTIME�������ē����O���[�v��Item�Ɋi�[����
				//long double time_val = 0;
				//for (itr = data_list.begin(); itr != data_list.end(); itr++){
				//	CEngValCnvDll::obs_tbl_res_t d = *itr;
				//	if (d.l_grp_cnt == cnt){
				//		char* ans = strstr(d.obs_adr.sz_obsname, mon_time_name);
				//		if (ans != NULL){
				//			time_val = d.obs_adr.ld_data;
				//			break;
				//		}
				//	}
				//}
				//for (itr = data_list.begin(); itr != data_list.end(); itr++){
				//	CEngValCnvDll::obs_tbl_res_t* d = &*itr;
				//	if (d->l_grp_cnt == cnt){
				//		d->ld_time_val = time_val;
				//	}
				//}

				memcpy(&k, &msg->uca_buff[j + 8], sizeof(long));
				k = ntohl(k);
				if (k > MAX_SDTP / 2)
				{
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: break uca buff size k(%d)"), __FILEW__, __LINE__, k);
//					CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
					break;
				}
				j += (k + 12);
				cnt++;
			}

		}

		return 0;
	}
	catch (CMemoryException* e)
	{
		e->Delete();
		CString strDebug;
		strDebug.Format(_T("%s(%d) :: CMemoryException!"), __FILEW__, __LINE__);
//		CLogTraceEx::Write(_T(""), _T("CEnvValCnv"), _T("CalcFromMsg"), _T("debug"), strDebug, nLogEx::debug);
		return -1;
	}
	catch (...){
		return -1;
	}
}



int CEngValCnvDll::ud_montbl_data64(char    *shmadr, int l_dataid, unsigned char ca_data[], int l_offset,
	int *l_inflg, int *l_grmd, double *d_data, char sz_sts_name[],
	long double *ldp_data, long *lp_bitinfo)
{
	char   *mon_adr;
	struct xobsname_t *obs_adr;     /* �Ď����ڒ�`��� */
	struct xobsname_t *obs_ex_adr;  /* �Ď����ڒ�`��� */
	struct xif_tbl_t  *if_adr;      /* ������`��� */
	struct xcal_tbl_t *cal_adr;     /* �����ڌv�Z��� */
	struct xext_tbl_t *ext_adr;     /* �e���o��`��� */
	struct xpol_tbl_t *pol_adr;     /* ��������`��`�e�[�u�� */
	struct xcarv_tbl_t *carv_adr;   /* �Z���_�J�[�u��`�e�[�u�� */
	struct xtimecvt_tbl_t *time_adr; /* �����ϊ���`�e�[�u�� */
	struct xobssts_tbl_t *obssts_adr;/* �X�e�[�^�X���e�[�u�� */

	int     l_if_inflg;    /* �Ď����ڃf�[�^�L���t���O 1:�L�� [OUT] */
	int     l_if_grmd;     /* �X�e�[�^�X�t���O 1:�X�e�[�^�X���� [OUT] */
	double  d_if_data;     /* �H�w�l�f�[�^ [OUT] */
	char    sz_work[256];
	char    *ca_adr;
	long    l_flg;
	long    l_ifflg[20];

	long    l_rc;
	long    i;
	long    j;
	long    k;
	long    l;
	long    l_dataf[10];

	long double         ld_dataf;
	long long           ll_data = 0;
	unsigned long long  ull_data = 0;

	double  d_dataf[10];
	float   f_data;
	long    l_data;

	/*----- ���L�������̃A�h���X�f�d�s -----*/
	for (i = 0; i < 20; i++) {
		l_ifflg[i] = 0;
	}

	//obs_adr = (struct obsname_t *)(shmadr[EQMON_DB_SHM_ID] + l_offset);
	//�ݔ��Ď����̏�� from db
	obs_adr = (struct xobsname_t *)(shmadr + l_offset);
	if (ds_htonl(obs_adr->l_id) != OBSNAME_ID) {
		return -1;
	}

	*l_inflg = 0;    /* �Ď����ڃf�[�^�L���t���O�𖳌� */
	*l_grmd = 0;
	*d_data = 0.0;

	*ldp_data = 0.0;  /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
	*lp_bitinfo = 0;  /* �����r�b�g��:�������o�f�[�^�̃r�b�g�� */

	mon_adr = (char *)obs_adr;
	mon_adr += ds_htonl(obs_adr->l_len);

	m_lastCnvAfterType = 0;

	while (1) {

		obssts_adr = (struct xobssts_tbl_t *)mon_adr;

		//
		long ed_id = ds_htonl(obssts_adr->l_id);
		long d_len = ds_htonl(obssts_adr->l_len);

		if (ed_id == ENDOFOBS_ID) {
			return 0;
		}

		if (l_ifflg[0] != 0 && l_ifflg[0] != 2) {
			if (ed_id != IF_ID &&
				ed_id != ELSE_ID &&
				ed_id != ELSEIF_ID &&
				ed_id != ENDIF_ID) {
				mon_adr += d_len;
				continue;
			}
		}

		switch (ed_id) {
			/*-----------------------------------------------------------*/
			/*              ���o�f�[�^��`�������̃f�[�^����             */
			/*-----------------------------------------------------------*/
		case EXT_ID:
		case EXTS_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;

			/* d_dataf[0]  = 0.0 ; EXT/EXTS�ł͎g��Ȃ� */
			/* l_data = 0 ; EXT/EXTS�ł͎g��Ȃ� */

			ld_dataf = 0.0; /* long double�^���̃A�L�������[�^ */

			for (i = 0; i < ds_htonl(ext_adr->l_pw_cnt); i++) { /* ���o��`�� */

				if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {

					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
					j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
					j++; /* j:�r�b�g�� */

					(*lp_bitinfo) += j; /* �����r�b�g��:�������o�f�[�^�̃r�b�g�� */

					if (ds_htonl(ext_adr->l_pw_cnt) == 1){ /* ���o��`���P�F�ʏ� */

						if (ds_htonl(ext_adr->l_id) == EXTS_ID){ /* �����t�� */

							ua_ldbitfieldll((unsigned long long*)&ll_data, &ca_data[L_OFFSET],
								ds_htonl(ext_adr->s_pwb[i].l_word),
								ds_htonl(ext_adr->s_pwb[i].l_s_bit), j, 1);
							ld_dataf = (long double)ll_data;

						}
						else{ /* �����Ȃ� */

							ua_ldbitfieldll(&ull_data, &ca_data[L_OFFSET],
								ds_htonl(ext_adr->s_pwb[i].l_word),
								ds_htonl(ext_adr->s_pwb[i].l_s_bit), j, 0);
							ld_dataf = (long double)ull_data;
						}

					}
					else{ /* �r�b�g�𕡐�������W�߂ĂP�ɂ��� */

						if (i == 0 && ds_htonl(ext_adr->l_id) == EXTS_ID) { /* �������̐擪 */

							ua_ldbitfieldll((unsigned long long *)&ll_data, &ca_data[L_OFFSET],
								ds_htonl(ext_adr->s_pwb[i].l_word),
								ds_htonl(ext_adr->s_pwb[i].l_s_bit), j, 1);
							ld_dataf = (long double)ll_data;

						}
						else { /* �����Ȃ��A�܂��͕����t���̂Q�� */

							ua_ldbitfieldll(&ull_data, &ca_data[L_OFFSET],
								ds_htonl(ext_adr->s_pwb[i].l_word),
								ds_htonl(ext_adr->s_pwb[i].l_s_bit), j, 0);

							ld_dataf *= (long double)((((long long)1) << j)); /* ��ʃr�b�g�� */

							if (ld_dataf >= 0.0) {
								ld_dataf += ull_data;
							}
							else {
								ld_dataf -= ull_data;
							}

						}

					}/* end if �r�b�g�𕡐�������W�߂�ꍇ */
				}/* end if l_dataid */
			}/* end for i */

			(*ldp_data) += ld_dataf;  /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */

			(*d_data) = (double)(*ldp_data); /* �]���ǂ����double�l */
			m_lastCnvAfterType |= 1 << eCnvResType_LongDouble;
			break;

			/*-----------------------------------------------------------*/
			/*              ���o�f�[�^��`�������̃f�[�^����             */
			/*-----------------------------------------------------------*/
		case EXTAVG_ID:
		case EXTAVGS_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			for (i = 0; i < ds_htonl(ext_adr->l_pw_cnt); i++) {
				if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {
					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
					j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
					j++;
					ua_ldbitfield((unsigned long *)&l_dataf[i], &ca_data[L_OFFSET],
						ds_htonl(ext_adr->s_pwb[i].l_word),
						ds_htonl(ext_adr->s_pwb[i].l_s_bit), j,
						ds_htonl(ext_adr->l_id) - EXTAVG_ID);
					l_data += l_dataf[i];
				}
			}
			(*d_data) += ((double)l_data / (double)(ds_htonl(ext_adr->l_pw_cnt)));

			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;

			/*-----------------------------------------------------------*/
			/*              ���o�f�[�^�idouble)�̃f�[�^����              */
			/*-----------------------------------------------------------*/
		case EXTDBL_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			if (l_dataid == ds_htonl(ext_adr->s_pwb[0].l_pid)) {
				*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
				memcpy(&d_dataf[0], &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], sizeof(double));
#ifdef _CUT
				(*d_data) += d_dataf[0];
				(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
#else
				(*d_data) += dswap2(d_dataf[0]);
				(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
#endif
				m_lastCnvAfterType |= 1 << eCnvResType_Double;
			}
			break;

			/*-----------------------------------------------------------*/
			/*              ���o�f�[�^�ifloat)�̃f�[�^����               */
			/*-----------------------------------------------------------*/
		case EXTFLT_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			if (l_dataid == ds_htonl(ext_adr->s_pwb[0].l_pid)) {
				*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
				memcpy(&f_data, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], sizeof(float));
#ifdef _CUT
				(*d_data) += f_data;
				(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
#else
				(*d_data) += fswap(f_data);
				(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
#endif
				m_lastCnvAfterType |= 1 << eCnvResType_Double;
			}
			break;

			/*-----------------------------------------------------------*/
			/*              �����񒊏o�f�[�^�̃f�[�^����                 */
			/*-----------------------------------------------------------*/
		case EXTCHAR_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			if (l_dataid == ds_htonl(ext_adr->s_pwb[0].l_pid)) {
				j = ds_htonl(ext_adr->s_pwb[0].l_e_bit) - ds_htonl(ext_adr->s_pwb[0].l_s_bit);
				j++;
				j /= 8;
				if (j < sizeof(obs_adr->sz_sts_name)) {
					memcpy(sz_sts_name, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], j);
					sz_sts_name[j] = '\0';
					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
					*l_grmd = 2;
					m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
				}
				else if (strlen((char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)]) < sizeof(obs_adr->sz_sts_name)) {
					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
					*l_grmd = 2;
					m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
					//strcpy(sz_sts_name, (char *)&ca_data[L_OFFSET + ext_adr->s_pwb[0].l_word]);
					strcpy_s(sz_sts_name, STR_BUF_SIZE, (char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)]);
				}
				else {
					ca_adr = strrchr((char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], '/');
					if (ca_adr == NULL || strlen(ca_adr) > sizeof(obs_adr->sz_sts_name)) {
						memcpy(sz_sts_name, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[0].l_word)], sizeof(obs_adr->sz_sts_name));
						*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
						*l_grmd = 2;
						m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
						sz_sts_name[sizeof(obs_adr->sz_sts_name) - 1] = '\0';
					}
					else {
						ca_adr++;
						*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
						*l_grmd = 2;
						m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
						strcpy_s(sz_sts_name, STR_BUF_SIZE, ca_adr);
					}
				}
				/*
				printf ( "%s = %s \n" , obs_adr->sz_obsname , &ca_data[L_OFFSET+ext_adr->s_pwb[0].l_word] ) ;
				printf ( "%s = %s \n" , obs_adr->sz_obsname , sz_sts_name ) ;
				*/
			}
			break;

			/*-----------------------------------------------------------*/
			/*              ������^���l���o�f�[�^�̃f�[�^����           */
			/*-----------------------------------------------------------*/
		case EXTCHARDEC_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			l_data = 0;
			i = 0;
			if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {
				j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
				j++;
				j /= 8;
				if (j < sizeof(obs_adr->sz_sts_name)) {
					memcpy(sz_sts_name, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[i].l_word)], j);
					sz_sts_name[j] = '\0';
					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
					*l_grmd = 1;
					m_lastCnvAfterType |= 1 << eCnvResType_StatusStr;
				}
				else if (strlen((char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[i].l_word)]) < sizeof(obs_adr->sz_sts_name)) {
					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
					*l_grmd = 1;
					m_lastCnvAfterType |= 1 << eCnvResType_StatusStr;
					strcpy_s(sz_sts_name, STR_BUF_SIZE, (char *)&ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[i].l_word)]);
				}
				else {
					j = sizeof(obs_adr->sz_sts_name) - 1;
					memcpy(sz_sts_name, &ca_data[L_OFFSET + ds_htonl(ext_adr->s_pwb[i].l_word)], j);
					sz_sts_name[j] = '\0';
					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
					*l_grmd = 1;
					m_lastCnvAfterType |= 1 << eCnvResType_StatusStr;
				}
				l_rc = uc_valchk(sz_sts_name, (int *)&l_data, &d_dataf[0], NULL);
				if (l_rc == 1) {
					(*d_data) += d_dataf[0];
					(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
					m_lastCnvAfterType |= 1 << eCnvResType_Double;
				}
			}
			/*
			printf ( "%s = %s \n" , obs_adr->sz_obsname , &ca_data[L_OFFSET+ext_adr->s_pwb[i].l_word] ) ;
			printf ( "%s = %s \n" , obs_adr->sz_obsname , sz_sts_name ) ;
			*/
			break;

			/*-----------------------------------------------------------*/
			/*              �������̃f�[�^����                           */
			/*-----------------------------------------------------------*/
		case IF_ID:
			if (l_ifflg[0] != 0) {
				memmove(&l_ifflg[1], &l_ifflg[0], sizeof(int) * 19);
				if (l_ifflg[1] != 0 && l_ifflg[1] != 2) {
					l_ifflg[0] = 3;
					break;
				}
			}
			//break;//��������聚����
		case ELSEIF_ID:
			if (ed_id == ELSEIF_ID) {
				if (l_ifflg[0] != 1) {
					l_ifflg[0] = 3;
					break;
				}
			}
			if_adr = (struct xif_tbl_t  *)mon_adr;

			if (if_adr->l_offset == NULL) {
				l_rc = ud_sts_flg(if_adr->sz_obsname, NULL, shmadr, 0,
					&l_if_grmd, &if_adr->l_offset, NULL);
				if (l_rc != 0) {
					if_adr->l_offset = -1;
					l_rc = 0;
				}
			}

			if (ds_htonl(if_adr->l_offset) != -1) {
				l_rc = ud_montbl_data(shmadr, l_dataid, ca_data, if_adr->l_offset, &l_if_inflg, &l_if_grmd, &d_if_data, sz_work);
				if (l_rc != 0 || l_if_inflg == 0) {
					obs_ex_adr = (struct xobsname_t *)(shmadr + if_adr->l_offset);
					l_if_inflg = ds_htonl(obs_ex_adr->l_inflg);
					l_if_grmd = ds_htonl(obs_ex_adr->l_grmd);
					d_if_data = dswap(obs_ex_adr->d_data);
					strcpy_s(sz_work, sizeof(sz_work), obs_ex_adr->sz_sts_name);
				}
			}
			else {
				l_if_inflg = 0;
			}
			l_ifflg[0] = 1;
			if (l_rc == 0 && l_if_inflg != 0) {

				switch (ds_htonl(if_adr->l_flag)) {
				case 0:
					if (d_if_data == dswap(if_adr->d_val)) {
						l_ifflg[0] = 2;
					}
					break;
				case 1:
					if (d_if_data != dswap(if_adr->d_val)) {
						l_ifflg[0] = 2;
					}
					break;
				case 2:
					if (d_if_data < dswap(if_adr->d_val)) {
						l_ifflg[0] = 2;
					}
					break;
				case 3:
					if (d_if_data > dswap(if_adr->d_val)) {
						l_ifflg[0] = 2;
					}
					break;
				}
			}
			break;
			/*-----------------------------------------------------------*/
			/*              ELSE���̃f�[�^����                           */
			/*-----------------------------------------------------------*/
		case ELSE_ID:
			if (l_ifflg[0] != 0) {
				l_ifflg[0] ++;
			}
			break;
			/*-----------------------------------------------------------*/
			/*              ENDIF���̃f�[�^����                           */
			/*-----------------------------------------------------------*/
		case ENDIF_ID:
			if (l_ifflg[1] != 0) {
				memmove(&l_ifflg[0], &l_ifflg[1], sizeof(int) * 19);
			}
			else {
				l_ifflg[0] = 0;
			}
			break;
			/*-----------------------------------------------------------*/
			/*              �����ڌv�Z��񏈗�                           */
			/*-----------------------------------------------------------*/
		case CAL_ID:
			if (*l_inflg == 1) {
				cal_adr = (struct xcal_tbl_t  *)mon_adr;

				if (cal_adr->l_offset == NULL) {
					l_rc = ud_sts_flg(cal_adr->sz_obsname, NULL, shmadr, 0,
						&l_if_grmd, &cal_adr->l_offset, NULL);
					if (l_rc != 0) {
						cal_adr->l_offset = -1;
					}
				}

				if (ds_htonl(cal_adr->l_offset) != -1) {
					l_rc = ud_montbl_data(shmadr, l_dataid, ca_data, cal_adr->l_offset,
						&l_if_inflg, &l_if_grmd, &d_if_data, sz_work);
					if (l_rc != 0 || l_if_inflg == 0) {
						obs_ex_adr = (struct xobsname_t *)(shmadr + cal_adr->l_offset);
						l_if_inflg = ds_htonl(obs_ex_adr->l_inflg);//�����ϊ�����Ȃ���������
						l_if_grmd = ds_htonl(obs_ex_adr->l_grmd);//�����ϊ�����Ȃ���������
						d_if_data = dswap(obs_ex_adr->d_data);
						strcpy_s(sz_work, sizeof(sz_work), obs_ex_adr->sz_sts_name);

					}
					/*
					if ( strcmp ( obs_adr->sz_obsname , "CTRL.LINE_11" ) == NULL ) {
					printf ( "l_rc = %d\n" , l_rc ) ;
					printf ( "l_if_inflg = %d\n" , l_if_inflg ) ;
					printf ( "d_if_data = %lg\n" , d_if_data ) ;
					printf ( "obs_ex_adr->sz_obsname = %s\n" , obs_ex_adr->sz_obsname ) ;
					}
					*/
				}
				else {
					l_if_inflg = 0;
				}
				/*
				if ( strcmp ( obs_adr->sz_obsname , "CTRL.LINE_11" ) == NULL ) {
				printf ( "cal_adr->l_flag = %d , d_data = %lg , d_if_data = %lg\n" , cal_adr->l_flag , *d_data ,  d_if_data ) ;
				}
				*/
				if (l_rc == 0 && l_if_inflg != 0) {
					switch (ds_htonl(cal_adr->l_flag)) {
					case 0:
						(*d_data) += d_if_data;
						break;
					case 1:
						(*d_data) -= d_if_data;
						break;
					case 2:
						(*d_data) *= d_if_data;
						break;
					case 3:
						(*d_data) /= d_if_data;
						break;
					}
					(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
					m_lastCnvAfterType |= 1 << eCnvResType_Double;
				}
				else {
					*l_inflg = 0;
				}
			}
			break;
			/*-----------------------------------------------------------*/
			/*            BCD���o�f�[�^��`�������̃f�[�^����             */
			/*-----------------------------------------------------------*/
		case EXTBCD_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			d_dataf[1] = 0.0;

			for (i = 0; i < ds_htonl(ext_adr->l_pw_cnt); i++) {

				if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {
					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */

					j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
					j++;
					ub_bcdext(&d_dataf[i], &ca_data[L_OFFSET],
						ds_htonl(ext_adr->s_pwb[i].l_word),
						ds_htonl(ext_adr->s_pwb[i].l_s_bit), j);
				}
			}
			*d_data += (d_dataf[0] * pow(10.0, d_dataf[1]));
			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
			/*-----------------------------------------------------------*/
			/*            BCD���o�f�[�^��`�������̃f�[�^����            */
			/*-----------------------------------------------------------*/
		case EXTBCDS_ID:
			ext_adr = (struct xext_tbl_t *)mon_adr;
			d_dataf[1] = 0.0;
			k = 0;
			l = 0;
			for (i = 0; i < ds_htonl(ext_adr->l_pw_cnt); i++) {
				if (l_dataid == ds_htonl(ext_adr->s_pwb[i].l_pid)) {
					*l_inflg = 1;    /* �Ď����ڃf�[�^�L���t���O��L�� */
					j = ds_htonl(ext_adr->s_pwb[i].l_e_bit) - ds_htonl(ext_adr->s_pwb[i].l_s_bit);
					j++;
					if ((i == 0 && ds_htonl(ext_adr->l_pw_cnt) != 1) ||
						(i == 2 && ds_htonl(ext_adr->l_pw_cnt) == 4)) {
						ua_ldbitfield((unsigned long *)&l_dataf[k], &ca_data[L_OFFSET],
							ds_htonl(ext_adr->s_pwb[i].l_word),
							ds_htonl(ext_adr->s_pwb[i].l_s_bit),
							j, 0);
						k++;
					}
					else {
						ub_bcdext(&d_dataf[l], &ca_data[L_OFFSET],
							ds_htonl(ext_adr->s_pwb[i].l_word),
							ds_htonl(ext_adr->s_pwb[i].l_s_bit), j);
						l++;
					}
				}
			}
			for (i = 0; i < k; i++) {
				if (l_dataf[i] != 0) {
					d_dataf[i] = -d_dataf[i];
				}
			}
			*d_data += (d_dataf[0] * pow(10.0, d_dataf[1]));
			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
			/*-----------------------------------------------------------*/
			/*            �������f�[�^�ϊ�����                           */
			/*-----------------------------------------------------------*/
		case POL_ID:
			pol_adr = (struct xpol_tbl_t *)mon_adr;
			//d_dataf[i] = 0.0;���������_�F�����R�[�h�̂܂܂��ƃX�^�b�N���j�󂳂�遚����
			d_dataf[0] = 0.0;
			for (i = 0; i < ds_htonl(pol_adr->l_val_cnt); i++) {
				d_dataf[i] = 1.0;
				for (j = i + 1; j < ds_htonl(pol_adr->l_val_cnt); j++) {
					d_dataf[i] *= (*d_data);
				}
				d_dataf[i] *= dswap(pol_adr->d_val[i]);
			}
			*d_data = 0.0;
			for (i = 0; i < ds_htonl(pol_adr->l_val_cnt); i++) {
				(*d_data) += d_dataf[i];
			}
			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;

			/*-----------------------------------------------------------*/
			/*            �J�[�u�ϊ�����                                 */
			/*-----------------------------------------------------------*/
		case CARVTBL_ID:
			carv_adr = (struct xcarv_tbl_t *)mon_adr;
			for (i = 0; i < ds_htonl(carv_adr->l_carv_cnt) - 1; i++) {
				if (*d_data < dswap(carv_adr->d_carv[i][0])) {
					if (i == 0) {
						*d_data = dswap(carv_adr->d_carv[i][1]);
					}
					else {
						d_dataf[0] = (*d_data - dswap(carv_adr->d_carv[i][0])) /
							(dswap(carv_adr->d_carv[i + 1][0]) - dswap(carv_adr->d_carv[i][0]));
						d_dataf[1] = (dswap(carv_adr->d_carv[i + 1][1]) - dswap(carv_adr->d_carv[i][1]));
						*d_data = d_dataf[0] * d_dataf[1] + dswap(carv_adr->d_carv[i][1]);
					}
					break;
				}
			}
			if (i == ds_htonl(carv_adr->l_carv_cnt) - 1) {
				*d_data = dswap(carv_adr->d_carv[i - 1][1]);
			}
			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
			/*-----------------------------------------------------------*/
			/*            �����f�[�^�ϊ�����                             */
			/*-----------------------------------------------------------*/
		case TIMECVT_ID:
			time_adr = (struct xtimecvt_tbl_t *)mon_adr;
			*l_grmd = 2;
			m_lastCnvAfterType |= 1 << eCnvResType_TimeStr;
			if (ds_htonl(time_adr->l_cvtno) == 1) {
				ub_dtime_to_asc(*d_data, sz_work);
				sz_work[19] = '\0';
				//strcpy(sz_sts_name, sz_work);
				strcpy_s(sz_sts_name, STR_BUF_SIZE, sz_work);
			}
			else {
				if (d_dataf[0] < 0.0) {
					d_dataf[0] = -(*d_data);
					l_flg = 1;
				}
				else {
					d_dataf[0] = *d_data;
					l_flg = 0;
				}

				for (i = 0; i < 3; i++) {
					l_dataf[i] = (long)fmod(d_dataf[0], 100.0);
					d_dataf[0] = floor(d_dataf[0] / 100.0);
				}

				if (ds_htonl(time_adr->l_cvtno) == 2) {
					l_dataf[i] = (long)d_dataf[0];
					sprintf_s(sz_sts_name, STR_BUF_SIZE, "%03d %02d:%02d:%02d", l_dataf[3],
						l_dataf[2], l_dataf[1], l_dataf[0]);

					*d_data = ua_ddd_to_dtime(&sz_sts_name[0],
						&sz_sts_name[4],
						&sz_sts_name[7],
						&sz_sts_name[10]);
					(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
					m_lastCnvAfterType |= 1 << eCnvResType_Double;
					break;
				}
				else if (ds_htonl(time_adr->l_cvtno) == 3) {
					sprintf_s(sz_sts_name, STR_BUF_SIZE, "%02d:%02d:%02d",
						l_dataf[2], l_dataf[1], l_dataf[0]);
				}
				else if (ds_htonl(time_adr->l_cvtno) == 4) {
					if (l_flg == 0) {
						sprintf_s(sz_sts_name, STR_BUF_SIZE, "+%02d:%02d",
							l_dataf[1], l_dataf[0]);
					}
					else {
						sprintf_s(sz_sts_name, STR_BUF_SIZE, "-%02d:%02d",
							l_dataf[1], l_dataf[0]);
					}
				}
				else if (ds_htonl(time_adr->l_cvtno) == 5) {
					(*d_data) = ub_asc_to_dtime(sz_sts_name);
				}
#ifdef _GREAT_ENGVAL
				/* �yGREAT�ztimecvt 6 �Ή� */
				else if (ds_htonl(time_adr->l_cvtno) == 6) {
					if (l_flg == 0) {
						sprintf_s(sz_sts_name, STR_BUF_SIZE, "+%01d:%02d",
							l_dataf[1], l_dataf[0]);
					}
					else {
						sprintf_s(sz_sts_name, STR_BUF_SIZE, "-%01d:%02d",
							l_dataf[1], l_dataf[0]);
					}
				}
#endif /* _GREAT_ENGVAL */
			}
			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
			/*-----------------------------------------------------------*/
			/*            �X�e�[�^�X�ϊ�����                             */
			/*-----------------------------------------------------------*/
		case OBSSTS_ID:
			obssts_adr = (struct xobssts_tbl_t *)mon_adr;
			l_data = (long)(*d_data);
			*l_grmd = 1;
			m_lastCnvAfterType |= 1 << eCnvResType_StatusStr;
			//strcpy(sz_sts_name, obssts_adr->sz_i_sts);
			strcpy_s(sz_sts_name, STR_BUF_SIZE, obssts_adr->sz_i_sts);
			for (i = 0; i < ds_htonl(obssts_adr->l_sts_cnt); i++) {
				if (l_data == ds_htonl(obssts_adr->chk_t[i].l_val)) {
					strcpy_s(sz_sts_name, STR_BUF_SIZE, obssts_adr->chk_t[i].sz_sts);
					break;
				}
			}
			break;

#ifdef _GREAT_ENGVAL
			/* GREAT �H�w�l�ϊ��̉��C */
			/*-----------------------------------------------------------*/
			/*            ��p�ΐ�(log10)�ϊ�����                        */
			/*-----------------------------------------------------------*/
		case LOG10_ID:
			*d_data = log10(*d_data);
			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;

			/*-----------------------------------------------------------*/
			/*            ��p�ΐ�(log10)�t�ϊ�����                      */
			/*-----------------------------------------------------------*/
		case EXP10_ID:
			*d_data = pow(10.0, *d_data);
			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;

			/*-----------------------------------------------------------*/
			/*            �t����(arctan)�ϊ�����                         */
			/*-----------------------------------------------------------*/
		case ARCTAN_ID:
			*d_data = atan(*d_data);
			(*ldp_data) = (*d_data); /* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x */
			m_lastCnvAfterType |= 1 << eCnvResType_Double;
			break;
#endif /* _GREAT_ENGVAL */

		}

		mon_adr += d_len;
	}
}


/*

���ʃ��[�`��ua_ldbitfield()�́A�R�Q�r�b�g�f�[�^(long)�𒊏o����d�l�Ȃ̂�
���o���ʂ��A64bit-integer(long long)�ɂȂ�悤�ɍ���Ă��܂���B

long long �l�𒊏o�������̂ł���΁A�ȉ��̂悤�ɂ��Ă��������B

unsigned long long ull; //���ꂪ�A���߂���long long�l

unsigned long ula_work[2];

ua_ldbitfield( &ula_work[0], inputbuf
, l_wordno, ul_bitno, ul_bitlen, l_signflg );
#ifdef I8086
ull = (((unsigned long long)( (ula_work[1] ))<<32)
|  ((unsigned long long)( (ula_work[0] ));
#else
ull = (((unsigned long long)( (ula_work[0] ))<<32)
|  ((unsigned long long)( (ula_work[1] ));
#endif
ull = ull >> (64 - ul_bitlen);

�܂��́A�ȉ��́Aua_ldbitfieldll()���g�p���Ă��������B
*/
/*<a name="ua_ldbitfieldll"></a>
@function : ua_ldbitfieldll()
@name     : �r�b�g�t�B�[���h���o��64bit(long long)�o�[�W����
@���藚�� : 2007-02-15 Ver0.0 1st coding
@�����T�v : ua_ldbitfield()��64bit(long long)�o�[�W����
@
@  1. �o�C�g�ʒu�A�r�b�g�ʒu�A�r�b�g�����w�肵�ăr�b�g�t�B�[���h�𒊏o����
@
@    �u�r�b�g�ʒu�v�́AMSB ��0 �r�b�g�Ƃ���I�N�e�b�g���r�b�g�ԍ�(0..7)���w�肷��B
@        �i�r�b�g�ʒu���A8 �Ȃ�Ύ��̃I�N�e�b�g���� MSB ���w�肵���Ӗ��ɂȂ�j
@
@       0 1 2 3 4 5 6 7
@      |+-+-+-+-+-+-+-+|
@      |MSB         LSB|
@      |---------------|
@
@  2. ���o���ɕ�������/�Ȃ����w��\�i�����t���f�[�^�̏ꍇ�͂Q�̕␔�\��
@     �Ƃ݂Ȃ��ŏ�ʃr�b�g�𕄍��g���i64�r�b�g�̕����t���f�[�^�Ɂj����j
@
@     W0�̃r�b�g4����8�r�b�g�����Ȃ��Œ��o����ꍇ
@
@       ���o��
@           W0      W1      W2      W3
@           01234567012345670123456701234567
@               11111111
@               --------
@
@       ���o����(64bit�l)
@           b0..........................................................b63
@           000000000000000000000000000000000000000000000000000000011111111
@                                                                  --------
@�⑫     :
@         ���o�r�b�g����65�r�b�g�ȏ�̃r�b�g�����w�肵�Ă�
@         64�r�b�g�̒��o���w�肵�����̂ƌ��Ȃ��܂��B
*/
//unsigned long long *pull_outbuf;/*output: ���o���ʂ��i�[����64�r�b�g�̃o�b�t�@ */
//unsigned char inputbuf[]; /*input:  ���o���o�b�t�@�̐擪�iW0�j�A�h���X*/
//long l_wordno;            /*input:  ���o����o�C�g�ԍ�    �F0..n      */
//unsigned long ul_bitno;   /*input:  ���o�r�b�g�ԍ�        �F0..n      */
//unsigned long ul_bitlen;  /*input:  ���o�r�b�g��          �F1..64     */
//long l_signflg;           /*input:  �����w��F�i0:�����Ȃ�
//						  1:���o�㕄���g������j*/

void CEngValCnvDll::ua_ldbitfieldll(unsigned long long *pull_outbuf, unsigned char inputbuf[]
	, long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen, long l_signflg)
{
	unsigned long long ull; /*���ꂪ�A���߂���long long�l*/
	long long           ll; /*���ꂪ�A���߂���long long�l*/

	unsigned long ula_work[2];

	ua_ldbitfield(&ula_work[0], inputbuf
		, l_wordno, ul_bitno, ul_bitlen, l_signflg);

	if (ul_bitlen > 32){

		//#ifdef I8086
		ull = (((unsigned long long)((ula_work[1])) << 32))
			| ((unsigned long long)((ula_work[0])));

		if (l_signflg == 0){
			ull = ull >> (64 - ul_bitlen);
			*pull_outbuf = ull;
		}
		else{
			ll = ull;
			ll = ll >> (64 - (long)ul_bitlen);
			*pull_outbuf = ll;
		}

	}
	else{
		if (l_signflg == 0){
			*pull_outbuf = ula_work[0];
		}
		else{
			*pull_outbuf = (long long)((long)ula_work[0]);
		}
	}
}


/*<a name="ua_ldbitfield"></a>
@function : ua_ldbitfield()
@name     : �r�b�g�t�B�[���h���o
@���藚�� : 97-10-23 Ver0.0 1st coding
@�����T�v :
@  1. �o�C�g�ʒu�A�r�b�g�ʒu�A�r�b�g�����w�肵�ăr�b�g�t�B�[���h�𒊏o����
@
@    �u�r�b�g�ʒu�v�́AMSB ��0 �r�b�g�Ƃ���I�N�e�b�g���r�b�g�ԍ�(0..7)���w�肷��B
@        �i�r�b�g�ʒu���A8 �Ȃ�Ύ��̃I�N�e�b�g���� MSB ���w�肵���Ӗ��ɂȂ�j
@
@       0 1 2 3 4 5 6 7
@      |+-+-+-+-+-+-+-+|
@      |MSB         LSB|
@      |---------------|
@
@  2. ���o���ɕ�������/�Ȃ����w��\�i�����t���f�[�^�̏ꍇ�͂Q�̕␔�\��
@     �Ƃ݂Ȃ��ŏ�ʃr�b�g�𕄍��g���i32�r�b�g�̕����t���f�[�^�Ɂj����j
@
@     W0�̃r�b�g4����8�r�b�g�����Ȃ��Œ��o����ꍇ
@
@       ���o��
@           W0      W1      W2      W3
@           01234567012345670123456701234567
@               11111111
@               --------
@
@       ���o����(32bit�l)
@           b0...........................b31
@           00000000000000000000000011111111
@                                   --------
@
@�߂�l   :  �Ȃ�
@�⑫     :
@  1. 32�r�b�g�̃f�[�^�𒊏o�w�肳�ꂽ�ꍇ�́A�����g���w��́A����
@     ���āA32�r�b�g�f�[�^���̂��̂𒊏o���ʂƂ���B
@  2. 33�r�b�g�ȏ�̃f�[�^���w�肳�ꂽ�ꍇ���A�����g���w��́A����
@     �����B
@  3. 33�r�b�g�ȏ�̃f�[�^���w�肳�ꂽ�ꍇ�́A�r�b�g�A���C�����g��
@     �Ƃ�Ă���(�r�b�g�ԍ�==0)�ꍇ�ƁA�Ƃ�Ă��Ȃ�(�r�b�g�ԍ�>0)
@     �ꍇ�Œ��o���ʂ��i�[������@���قȂ�A�ȉ��̂悤�ɂȂ�B
@
@     �r�b�g�ԍ�==0�̏ꍇ:
@         ���o���ʂ��i�[����o�b�t�@�ɂ́A���o���f�[�^����K��64�r�b�g
@         �̃f�[�^���]�������B
@
@     �r�b�g�ԍ�>0�̏ꍇ:
@         ���o���ʂ��i�[����o�b�t�@�ɂ́A���o���f�[�^���璊�o�r�b�g��
@         �̃f�[�^���]�������B���̂Ƃ��A���o���ʂ��i�[����o�b�t�@��
@         ���̃r�b�g�͕ύX����Ȃ��i���o�r�b�g���݂̂̃r�b�g���ݒ肳��
@         ��j�B
@         �܂��A���o�r�b�g����65�r�b�g�ȏ�̃r�b�g�����w�肵���ꍇ�ł�
@         64�r�b�g�܂ł̓]�������s��Ȃ��B
@
@   4. 64bit�����l(long long)�𒊏o���ʂƂ��ē������ꍇ�́A
@      �r�b�g�t�B�[���h���o��64bit(long long)�o�[�W����( ua_ldbitfieldll() )
@      ���p�ӂ���Ă��܂�
*/

//unsigned long *pul_outbuf;/*output: ���o���ʂ��i�[����32�r�b�g�܂���
//							  64�r�b�g�̃o�b�t�@                */
//unsigned char inputbuf[]; /*input:  ���o���o�b�t�@�̐擪�iW0�j�A�h���X*/
//long l_wordno;            /*input:  ���o����o�C�g�ԍ�    �F0..n      */
//unsigned long ul_bitno;   /*input:  ���o�r�b�g�ԍ�        �F0..n      */
//unsigned long ul_bitlen;  /*input:  ���o�r�b�g��          �F1..64     */
//long l_signflg;           /*input:  �����w��F�i0:�����Ȃ�
//						  1:���o�㕄���g������j*/

void CEngValCnvDll::ua_ldbitfield(unsigned long *pul_outbuf, unsigned char inputbuf[]
	, long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen, long l_signflg)
{
	unsigned long ul_w;
	//unsigned char *p;
	//int i; 
	long l_w;
	register  unsigned long ul_w_r;
	register           long  l_w_r;

	l_wordno = l_wordno + (ul_bitno >> 3); /* �o�C�g�ʒu�����߂� */
	ul_bitno = ul_bitno & LOW3BIT;           /* �o�C�g���̃r�b�g�ԍ������߂� */

	if (ul_bitlen <= 24){       /*24�r�b�g�܂ł̃f�[�^*/
		ul_w_r = ((unsigned long)inputbuf[l_wordno] << 24)
			| ((unsigned long)inputbuf[l_wordno + 1] << 16)
			| ((unsigned long)inputbuf[l_wordno + 2] << 8)
			| (unsigned long)inputbuf[l_wordno + 3];
		ul_w_r = ul_w_r << (ul_bitno); /*�擪�r�b�g�����[�h�̐擪��sift:(93-11-05)*/


		if (l_signflg == 0){/*0:then abs data*/
			*pul_outbuf = ul_w_r >> (32 - ul_bitlen);/*�ŉ��ʃr�b�g�����[�h��LSB��sift*/
		}
		else{                                 /*�ŉ��ʃr�b�g�����[�h��LSB�ɎZ�psift*/
			l_w_r = (long)ul_w_r >> (32 - (long)ul_bitlen);
			*pul_outbuf = l_w_r;
		}

	}
	else if ((ul_bitlen == 32) && (ul_bitno == 0)){

		/*32�r�b�g�f�[�^�Ńr�b�g�A���C�����g�����Ă���ꍇ*/

		ul_w_r = ((unsigned long)inputbuf[l_wordno] << 24)
			| ((unsigned long)inputbuf[l_wordno + 1] << 16)
			| ((unsigned long)inputbuf[l_wordno + 2] << 8)
			| (unsigned long)inputbuf[l_wordno + 3];
		*pul_outbuf = ul_w_r;

		/*
		#### 8086�p���o�菇 ###############################################
		*/
	}
	else if (ul_bitlen >= 32){

		/*8086:32�r�b�g�ȏ�f�[�^�Ńr�b�g�A���C�����g�����Ă��Ȃ��ꍇ*/

		if (ul_bitlen >64){ ul_bitlen = 64; }
		ua_insbitfield((unsigned char *)pul_outbuf, 0L, ul_bitlen, (char *)&inputbuf[l_wordno], ul_bitno);

		if (ul_bitlen == 32){
			(*pul_outbuf) = ntohl(*pul_outbuf); /*long�l�ŃX�g�A*/
		}
		else{
			ul_w_r = ntohl(*pul_outbuf);        /*double�l�Ƃ݂ă��[�h�t���ŃX�g�A*/
			(*pul_outbuf) = ntohl(*(pul_outbuf + 1));
			(*(pul_outbuf + 1)) = ul_w_r;
		}


	}
	else{    /*25�r�b�g����31�r�b�g�܂ł̃f�[�^*/

		/*�{���́A25�r�b�g����64�r�b�g�܂ł������Œ��o���ėǂ��̂ł��邪
		32bit/64bit�f�[�^���r�b�g���E����n�܂邱�Ƃ͖����̂ōœK��
		�̂��ߏ�̂悤�ȏ����Ƃ����B
		*/

		ul_w = 0;
		ua_insbitfield((unsigned char *)&ul_w, 0L, ul_bitlen, (char *)&inputbuf[l_wordno], ul_bitno); /*(93-11-05)*/
		ul_w = ntohl(ul_w); /* mem1234 -> reg4321 -> reg1234 */

		if (l_signflg == 0){/*0:then abs data*/
			*pul_outbuf = (unsigned long)ul_w >> (32 - ul_bitlen);/*�ŉ��ʃr�b�g�����[�h��LSB��sift*/
		}
		else{                                 /*�ŉ��ʃr�b�g�����[�h��LSB�ɎZ�psift*/
			l_w = (long)ul_w >> (32 - (long)ul_bitlen);
			*pul_outbuf = l_w;
		}
	}
}


/*<a name="ua_stbitfield"></a>
@function : ua_stbitfield()
@name     : �r�b�g�t�B�[���h�ւ̒l�̃X�g�A(ua_ldbitfield�̋t)
@���藚�� : 2011-05-15 Ver0.0 1st coding
@�����T�v :
@
@  1. ���l���A�r�b�g�t�B�[���h�փX�g�A����B�r�b�g�t�B�[���h����
@    �o�C�g�ʒu�A�r�b�g�ʒu�A�r�b�g���͈����Ŏw�肷��B
@
@    �u�r�b�g�ʒu�v�́AMSB ��0 �r�b�g�Ƃ���I�N�e�b�g���r�b�g�ԍ�(0..7)���w�肷��B
@        �i�r�b�g�ʒu���A8 �Ȃ�Ύ��̃I�N�e�b�g���� MSB ���w�肵���Ӗ��ɂȂ�j
@
@       0 1 2 3 4 5 6 7
@      |+-+-+-+-+-+-+-+|
@      |MSB         LSB|
@      |---------------|
@
@
@     �X�g�A���������l�i�X�g�A����r�b�g����LSB������K�v�r�b�g�����)
@
@           b0...........................b31(�����Ŏw�肵��32bit�l)
@           00000000000000000000000011111111
@                                   --------
@                                   <-8bit->
@
@     �X�g�A��r�b�g�t�B�[���h�iW0�̃r�b�g4����8�r�b�g�X�g�A�j
@
@           W0      W1      W2      W3
@           01234567012345670123456701234567
@               11111111
@               --------
@               <-8bit->
@                 ����8�r�b�g�̂ݒu����������i���̃r�b�g�͕ω����Ȃ��j
@
@�⑫     :
*/

//unsigned long ul_input;   /*input: �X�g�A���������l
//							  */
//unsigned char outputbuf[];/*output: �X�g�A��r�b�g�t�B�[���h�̐擪�iW0�j�A�h���X*/
//long l_wordno;            /*input:  �X�g�A��o�C�g�ԍ�    �F0..n      */
//unsigned long ul_bitno;   /*input:  �X�g�A��r�b�g�ԍ�    �F0..n      */
//unsigned long ul_bitlen;  /*input:  �X�g�A��r�b�g��      �F1..32     */

void CEngValCnvDll::ua_stbitfield(unsigned long ul_input, unsigned char outputbuf[],
	long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen)
{
	unsigned long ul_work;

	ul_work = htonl(ul_input << (32 - ul_bitlen));                       /*���Ɋ񂹂�*/
	ua_insbitfield(&outputbuf[l_wordno], ul_bitno, ul_bitlen, (char *)&ul_work, 0); /*�擪�r�b�g=b0����ins*/
}


/*<a name="ua_insbitfield"></a>
@function : ua_insbitfield()
@name     : �r�b�g�t�B�[���h�C���T�[�g����
@���藚�� : 97-10-23 Ver0.0 1st coding
@�����T�v :
@  1. �w�肳�ꂽ��̔C�ӂ̃r�b�g�t�B�[���h��C�ӂ̃r�b�g�t�B�[���h��
@     �u��������i�r�b�g�P�ʂ̃X�g�A�j
@
@     ���f�[�^�̃r�b�g 4 ���� 4 �r�b�g���C���T�[�g��̃r�b�g 6 ����}��
@
@       �C���T�[�g���錳�f�[�^
@           b0....b7
@           xxxx1111
@               ----
@
@       �C���T�[�g��
@           W0      W1      W2      W3
@           01234567012345670123456701234567
@           yyyyyy1111yyyyyyyyyyyyyyyyyyyyyy
@                 ----
@                 ���̂S�r�b�g�̂ݒu����������i���̃r�b�g�͕ω����Ȃ��j
@
@�߂�l   :  �Ȃ�
@�⑫     :
*/

//char  *tgtadr;   /*input,output:�C���T�[�g��擪�o�C�g�A�h���X*/
//long  l_inspos;  /*input:�C���T�[�g��r�b�g�ʒu:0..n          */
//long  l_inslen;  /*input:�C���T�[�g�r�b�g��    :1..n          */
//char  *suadr;    /*input:�C���T�[�g���錳�f�[�^�̐擪�o�C�g��
//				 �A�h���X                             */
//long  l_extpos;  /*input:�C���T�[�g���錳�f�[�^�̃r�b�g�ʒu
//				 :0..7 MSB���r�b�g0,LSB���r�b�g7�Ƃ���*/
void CEngValCnvDll::ua_insbitfield(unsigned char *tgtadr, long l_inspos, long l_inslen, char  *suadr, long l_extpos)
{
	register unsigned long ul_tgt, ul_su, ul_hi, ul_mid, ul_low;
	register long l_bytepos;

	while (l_inslen > 24){
		ua_insbitfield(tgtadr, l_inspos, 24L, suadr, l_extpos);
		tgtadr += 3; suadr += 3; l_inslen -= 24;
	}
	l_bytepos = l_inspos >> 3;       /*���ʂR�ޯ�mask(div 8)*/
	l_inspos = l_inspos & LOW3BIT;  /*���ʂR�ޯĂ̂ݎ��o��(mod 8)*/

	/*�f�B�X�g�l�[�V�������R�Q�r�b�g�l��register��load����*/

	ul_tgt = ((unsigned long)*((unsigned char *)(tgtadr + l_bytepos)) << 24)
		| ((unsigned long)*((unsigned char *)(tgtadr + l_bytepos + 1)) << 16)
		| ((unsigned long)*((unsigned char *)(tgtadr + l_bytepos + 2)) << 8)
		| (unsigned long)*((unsigned char *)(tgtadr + l_bytepos + 3));

	/*�\�[�X���R�Q�r�b�g��register��load����*/

	ul_su = ((unsigned long)*((unsigned char *)(suadr)) << 24)
		| ((unsigned long)*((unsigned char *)(suadr + 1)) << 16)
		| ((unsigned long)*((unsigned char *)(suadr + 2)) << 8)
		| (unsigned long)*((unsigned char *)(suadr + 3));


	if (l_inspos == 0){
		ul_mid = ul_su << l_extpos;
		ul_mid = ul_mid >> (32 - l_inslen);            /*LSB�Ɋ񂹂�:    0000mm */
		ul_mid = ul_mid << (32 - l_inslen);                           /* 00mm00 */
		ul_low = ul_tgt << l_inslen;
		ul_low = ul_low >> l_inslen;               /*���bitfield��ر:  0000LL */
		ul_tgt = ul_mid | ul_low;                   /*�ޯĂ�ϰ��:         hhmmLL */
	}
	else{
		ul_hi = ul_tgt >> (32 - l_inspos);      /*inspos����ʂ��ޯĂ̂ݎc��*/
		ul_hi = ul_hi << (32 - l_inspos);                          /* hh0000 */
		ul_mid = ul_su << l_extpos;
		ul_mid = ul_mid >> (32 - l_inslen);            /*LSB�Ɋ񂹂�:    0000mm */
		ul_mid = ul_mid << (32 - (l_inspos + l_inslen));                /* 00mm00 */
		ul_low = ul_tgt << (l_inspos + l_inslen);
		ul_low = ul_low >> (l_inspos + l_inslen); /*���bitfield��ر:  0000LL */
		ul_tgt = ul_hi | ul_mid | ul_low;           /*�ޯĂ�ϰ��:         hhmmLL */
	}


	/*�r�b�g�}�[�W���ʁi�R�Q�r�b�g�j���f�B�X�g�l�[�V�������ɃX�g�A*/

	*((unsigned char *)(tgtadr + l_bytepos)) = ul_tgt >> 24;
	*((unsigned char *)(tgtadr + l_bytepos + 1)) = (unsigned char)(ul_tgt >> 16);
	*((unsigned char *)(tgtadr + l_bytepos + 2)) = (unsigned char)(ul_tgt >> 8);
	*((unsigned char *)(tgtadr + l_bytepos + 3)) = (unsigned char)ul_tgt;
}



/*
---------------------------------------------------------------------
$name = { �Ď��e�[�u���f�[�^���o�֐� }
$outline = {
�w��̊Ď��f�[�^�o�b�t�@��蒊�o���������ڃe�[�u���̃f�[�^��
���o����B-->���o���邾���ł͂Ȃ��H�w�l�ϊ����s���Ă���B
}
$comment = {
}
---------------------------------------------------------------------
*/
//char    *shmadr[];  /* DB�e�[�u���A�h���X uc_get_shm_adr ()�őI��ꂽ�A�h���X�̔z��[IN] */
//int     l_dataid;   /* �Ď����ڃf�[�^�̑��u�ԍ� [IN] */
//unsigned char ca_data[];  /* �Ď����ڃf�[�^�̃f�[�^�t�B�[���h [IN] */
//int     l_offset;   /* �Ď����ڃf�[�^�̃I�t�Z�b�g�ʒu [IN] */
//int     *l_inflg;    /* �Ď����ڃf�[�^�L���t���O 1:�L�� [OUT] */
//int     *l_grmd;     /* �X�e�[�^�X�t���O 1:�X�e�[�^�X����
//					 �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ 2:������`���
//											0:�H�w�l [OUT] */
//double  *d_data;     /* �H�w�l�f�[�^ [OUT] */
//char    sz_sts_name[];/* ������f�[�^
//					  (MAX 64:sizeof(obsname_t.sz_sts_name)����) [OUT] */

int CEngValCnvDll::ud_montbl_data(char *shmadr, int l_dataid, unsigned char ca_data[], int l_offset, int *l_inflg, int *l_grmd, double *d_data, char sz_sts_name[])
{
	long double ld_data;
	long        l_bitinfo;

	return  ud_montbl_data64(shmadr, l_dataid, ca_data, l_offset
		, l_inflg, l_grmd, d_data, sz_sts_name
		, &ld_data, &l_bitinfo);
}




/*
---------------------------------------------------------------------
$name = { BCD�f�[�^���o�ϊ����� }
$outline = {
�w��̃f�[�^���a�b�c�ϊ����ăf�[�^�𔲂��o���B
}
$comment = {
}
---------------------------------------------------------------------
*/
//double *d_data;             /* �ϊ����� */
//char   ca_data[];           /* �Ď��f�[�^ */
//long   l_word;              /* ���[�h�ʒu */
//long   l_bit;               /* �r�b�g�ʒu */
//long   l_len;               /* �T�C�Y */

void CEngValCnvDll::ub_bcdext(double *d_data, unsigned char ca_data[], long l_word, long l_bit, long l_len)
{
	long   i;
	long   l;
	long   m;
	//	long   n;
	long   l_data;

	l = l_len / 4;
	m = l_len % 4;
	if (m == 0) {
		m = 4;
	}
	else {
		l++;
	}

	*d_data = 0.0;
	for (i = 0; i < l; i++) {
		ua_ldbitfield((unsigned long *)&l_data, ca_data, l_word, l_bit, m, 0);
		l_bit += m;
		if (l_bit >= 8) {
			l_bit -= 8;
			l_word += 1;
		}
		m = 4;
		(*d_data) *= 10.0;
		(*d_data) += l_data;
	}
	return;
}

/*<a name="ub_dtime_to_asc"></a>
@function : ub_dtime_to_asc()
@name     : unix ut time code to ascii strings
@���藚�� : 93-11-05 Ver1.0 1st coding
@���藚�� : 2001-05-08 Ver1.2 0.99995�b��1.00000�b�ɂ��肠����Ȃ�BUG-FIX
@�����T�v : ut ��������YYYY-MM-DD HH:MM:SS.ssss������ɕϊ�����B
@�߂�l   : �Ȃ�
@�⑫     :
*/
//	double           d_uttime;     /* input : ������� */
//char            *sz_time;      /* output: YYYY���i�[����̈� */
void CEngValCnvDll::ub_dtime_to_asc(double d_uttime, char *sz_time)
{
	long l_100usec;
	__time64_t long_time;
	struct  tm t;
	long    l_yyyy, l_mm;
	if (d_uttime <= -2147483648.0){ /* 0x80000000..0x7fffffff �͈̔̓`�F�b�N */
		d_uttime = -2147483648.0;    /* �����integer�ϊ�overflow-FPE����������*/
	}
	else if (d_uttime >= 2147483646.0){ /*���Ƃ�����邽�߂ł���B*/
		d_uttime = 2147483646.0;          /* 1998-03-18 */
	}
	long_time = (__time64_t)d_uttime;
	l_100usec = (long)((d_uttime - ((double)long_time) + 0.00005) * 10000); /* 100usec */
	if (l_100usec >= 10000){ /* 0.99995 �� 1.00000 �ɂȂ� */
		l_100usec -= 10000;
		long_time++;           /* �b�̌���␳ */
	}
	//t = localtime(&l_uttime); /*gmtime->localtime:1997-10-23*/
	_localtime64_s(&t, &long_time); /*gmtime->localtime:1997-10-23*/
	l_yyyy = t.tm_year + 1900;  /* 70 --> 1970 */
	l_mm = t.tm_mon + 1;      /* 0..11 -> 1..12 */

	//sprintf(sz_time, "%04d-%02d-%02d %02d:%02d:%02d.%04d", l_yyyy
	sprintf_s(sz_time, sizeof(sz_time), "%04d-%02d-%02d %02d:%02d:%02d.%04d", l_yyyy
		, l_mm, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, l_100usec);
	return;
}

/*<a name="ua_ddd_to_dtime"></a>
@function : ua_ddd_to_dtime()
@name     : ddd to dtime�����ϊ�����
@���藚�� : 95-12-27 Ver1.1 uk_ddd_to_dtime.c -> ua_ddd_to_dtime.c
@�����T�v : �e�A�[�M�������g�́A�ŌオNULL�̕�����ł���K�v�͂Ȃ��B
@�߂�l   : double time
@�⑫     :
���� char ddd : �ʎZ��
char hh  : ��
char mm  : ��     ---->    double dtime : 1970�N����̒ʎZ�b
char ss  : �b
[�O���Q�Ɗ֐�]
void      ub_bcd6_to_bcd();
double    ub_bcd_to_dtime();
*/
//char    *cap_ddd;      /* i: ����(ddd) */
//char    *cap_hh;       /* i: ����(hh)  */
//char    *cap_mm;       /* i: ����(mm)  */
//char    *cap_ss;       /* i: ����(ss)  */
//
double CEngValCnvDll::ua_ddd_to_dtime(char *cap_ddd, char *cap_hh, char *cap_mm, char *cap_ss)
{
	unsigned char   uca_bcd[7];         /* ����(BCD)   */
	unsigned char   uca_year[2];        /* yyy(BCD)    */
	unsigned char   uca_time[7];        /* hhmmss(BCD) */

	/****  6byte BCD �ϊ� ****/
	uca_bcd[0] = cap_ddd[0] & 0x0f;
	uca_bcd[1] = ((cap_ddd[1] & 0x0f) << 4) + (cap_ddd[2] & 0x0f);
	uca_bcd[2] = ((cap_hh[0] & 0x0f) << 4) + (cap_hh[1] & 0x0f);
	uca_bcd[3] = ((cap_mm[0] & 0x0f) << 4) + (cap_mm[1] & 0x0f);
	uca_bcd[4] = ((cap_ss[0] & 0x0f) << 4) + (cap_ss[1] & 0x0f);
	uca_bcd[5] = 0x00;

#ifdef true
	fprintf(stderr, "in(%c%c%c,%c%c:%c%c:%c%c) -> BCD(%02x%02x,%02x:%02x:%02x)\n",
		cap_ddd[0], cap_ddd[1], cap_ddd[2], cap_hh[0], cap_hh[1],
		cap_mm[0], cap_mm[1], cap_ss[0], cap_ss[1],
		uca_bcd[0], uca_bcd[1], uca_bcd[2], uca_bcd[3],
		uca_bcd[4], uca_bcd[5]);
	fflush(stderr);
#endif

	/****  6byte BCD to 9byte BCD ****/
	ub_bcd6_to_bcd(uca_bcd, uca_year, uca_time);
#ifdef true
	fprintf(stderr, "year(%02x%02x),time(%02x%02x,%02x:%02x:%02x.%02x%02x)\n",
		uca_year[0], uca_year[1], uca_time[0], uca_time[1], uca_time[2],
		uca_time[3], uca_time[4], uca_time[5], uca_time[6]);
	fflush(stderr);
#endif

	/****  9byte BCD to dtime ****/
	return(ub_bcd_to_dtime(uca_year, uca_time));

}

/*<a name="ub_bcd6_to_bcd"></a>
@function : ub_bcd6_to_bcd()
@name     : �U�޲�BCD�`���̎����̂X�޲Č`�������ւ̕ϊ�����
@���藚�� : 95-02-07 Ver1.0 1st coding
@�����T�v : 0dddHHMMSStt --> yyyy, 0dddHHMMSSttt0
@�߂�l   : �Ȃ�
@�⑫     :
@    unsigned char achBCD6[6]    �������(1970-01-01����̕b��)
@    unsigned char achYYYY[]     YYYY���i�[����̈� 2�o�C�g
@    unsigned char achTime[]     �������i�[����̈� �V�o�C�g
*/
//unsigned char    achBCD6[6];   /* input : ������� */
//unsigned char    achYYYY[];    /* output: YYYY���i�[����̈� */
//unsigned char    achTime[];    /* output: �������i�[����̈� */

void CEngValCnvDll::ub_bcd6_to_bcd(unsigned char achBCD6[], unsigned char achYYYY[], unsigned char achTime[])
{
	//long l_time;
	long long l_time;
	struct tm p_tm;
	static unsigned char uca_yyyy[2] = { 0, 0 };
	static unsigned char uc_old_dddhi = 0x7f;

	memcpy(achTime, achBCD6, 6);
	achTime[6] = 0;
	if (achTime[0] < uc_old_dddhi){
		l_time = time(NULL) + 1800; /* 1800sec = 30m : set date error limit */
		//p_tm = localtime(&l_time);          /*BCD̫�ϯĂ̔N(4��):1994..2001....*/
		_localtime64_s(&p_tm, &l_time);
		uca_yyyy[0] = (((p_tm.tm_year + 1900) / 1000) << 4) /* �N 1000�̈� */
			+ ((p_tm.tm_year + 1900) % 1000 / 100);    /* �N  100�̈� */
		uca_yyyy[1] = (((p_tm.tm_year + 1900) % 100 / 10) << 4) /* �N   10�̈� */
			+ ((p_tm.tm_year + 1900) % 10);    /* �N    1�̈� */
	};
	uc_old_dddhi = (achTime[0] & 0x03);
	achYYYY[0] = uca_yyyy[0];
	achYYYY[1] = uca_yyyy[1];
}

/*<a name="ub_bcd_to_dtime"></a>
@function : ub_bcd_to_dtime()
@name     : bcd time/ ascii time to unix ut time code
@���藚�� : 93-11-05 Ver0.0 1st coding
@�����T�v : 1970-01-01 00:00:00 ����̕b�����ߏ������W���[��
@�߂�l   : n:�ʎZ�b
@�⑫     :
*/
//unsigned char	uca_yyyy[];	/* input: YYYY */
///*         0 1 2 3 4 5 6 */
//unsigned char	uca_time[];	/* input: 0DDDHHMMSSmmm0 */


double CEngValCnvDll::ub_bcd_to_dtime(unsigned char uca_yyyy[], unsigned char uca_time[])  /*  0 1 */
{
	long  l_year, l_day, l_hour, l_min, l_sec, l_100usec, l_uru;
	long  l_byou;
	int   i_cnt;
	long  l_timez;
	double d_sec;
	static long la_yyyysec[100] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };/*1970..2069*/
	/*
	�N�������Z�b�g
	*/
	l_year = (uca_yyyy[0] >> 4) * 1000 + (uca_yyyy[0] & 0x0f) * 100
		+ (uca_yyyy[1] >> 4) * 10 + (uca_yyyy[1] & 0x0f);
	l_day = (uca_time[0] & 0x0f) * 100
		+ (uca_time[1] >> 4) * 10 + (uca_time[1] & 0x0f);
	l_hour = (uca_time[2] >> 4) * 10 + (uca_time[2] & 0x0f);
	l_min = (uca_time[3] >> 4) * 10 + (uca_time[3] & 0x0f);
	l_sec = (uca_time[4] >> 4) * 10 + (uca_time[4] & 0x0f);
	l_100usec = (uca_time[5] >> 4) * 1000 + (uca_time[5] & 0x0f) * 100
		+ (uca_time[6] >> 4) * 10 + (uca_time[6] & 0x0f);
	/*
	���߂�b���̌v�Z
	*/
	l_uru = 0;
	if (la_yyyysec[1] == 0){ /*1971�N==0�Ȃ�΃e�[�u�����쐬*/
		for (l_byou = 0, i_cnt = 1970; i_cnt < YYYY_MAX; i_cnt++){
			if ((i_cnt % 4) != 0){ /*�[�N�ł͖���*/
				l_uru = 0;
			}
			else if ((i_cnt % 400) == 0){ /*400�N�ɂP��[�N�ł���*/
				l_uru = 1;
			}
			else if ((i_cnt % 100) == 0){ /*100�N�ɂP��[�N�Ŗ���*/
				l_uru = 0;
			}
			else{
				l_uru = 1;                /*�[�N*/
			}
			l_byou += (365L + l_uru) * 60L * 60L * 24L;
			la_yyyysec[i_cnt - 1970 + 1] = l_byou;
		}
	}
	if ((l_year < 1970) || (l_year > YYYY_MAX)){
		l_byou = 0;
	}
	else{
		l_byou = la_yyyysec[l_year - 1970];
	}

	if (g_l_ub_dtime_timezoneflg == 0){ /*1:timezone�͗L��*/
		_tzset();
		g_l_ub_dtime_timezoneflg = 1; /*1:timezone�͗L��*/
	}
	_get_timezone(&l_timez);
	l_byou += l_timez; /*�����UT�̒ʎZ�b:1997-10-23*/
	l_byou += l_sec;

	d_sec = (double)l_byou
		+ (((double)l_100usec) / 10000.0)
		+ ((double)l_min      * 60.0)
		+ ((double)l_hour     * 3600.0)
		+ ((double)(l_day - 1)  * 86400.0);

	// PC�����擾
	SYSTEMTIME st_UTC;
	GetSystemTime(&st_UTC);
	CTime ctimePC = CTime(st_UTC);
	double d_pc_time = (double)ctimePC.GetTime();

	long year_incdec = 0;
	// PC�̔N����΍�B�V�����Ď����������߂̊Ď�����100���ȏ�̏ꍇ�́A1�N�����Čv�Z���Ȃ���
	if (d_sec - d_pc_time >= (double)PCTIME_SPAN100)
	{
		double d_span = d_sec - d_pc_time;
		while (d_span >= (double)PCTIME_SPAN100)
		{
			d_span = d_span - (double)PCTIME_SPAN365;
			year_incdec--;
		}
	}
	// PC�̔N����΍�B�V�����Ď����������߂̊Ď���������100���ȉ��̏ꍇ�́A1�N�����Čv�Z���Ȃ���
	else if (d_pc_time - d_sec >= (double)PCTIME_SPAN100)
	{
		double d_span = d_pc_time - d_sec;
		while (d_span >= (double)PCTIME_SPAN100)
		{
			d_span = d_span - (double)PCTIME_SPAN365;
			year_incdec++;
		}
	}

	if (year_incdec != 0)
	{
		// PC�������傫������Ă���ꍇ�A�N���C�����Ď����v�Z���Ȃ���
		l_year = l_year + year_incdec;

		if ((l_year < 1970) || (l_year > YYYY_MAX)){
			l_byou = 0;
		}
		else{
			l_byou = la_yyyysec[l_year - 1970];
		}

		if (g_l_ub_dtime_timezoneflg == 0){ /*1:timezone�͗L��*/
			_tzset();
			g_l_ub_dtime_timezoneflg = 1; /*1:timezone�͗L��*/
		}
		_get_timezone(&l_timez);
		l_byou += l_timez; /*�����UT�̒ʎZ�b:1997-10-23*/
		l_byou += l_sec;

		d_sec = (double)l_byou
			+ (((double)l_100usec) / 10000.0)
			+ ((double)l_min      * 60.0)
			+ ((double)l_hour     * 3600.0)
			+ ((double)(l_day - 1)  * 86400.0);
	}

	//	fprintf(stderr, "yyyy=%x:%x:date=%x:%x:", uca_yyyy[0], uca_yyyy[1], uca_time[0], uca_time[1]);
	//	fprintf(stderr, "year=%d,day=%d,hour=%d,min=%d,sec=%d,100usec=%d,uru=%d,byou=%d\n", l_year, l_day, l_hour, l_min, l_sec, l_100usec, l_uru, l_byou);

	return(d_sec);

}


/*<a name="ub_asc_to_dtime"></a>
@function : ub_asc_to_dtime()
@name     : �����������ʎZ�b�ɂ���B
@���藚�� : 93-11-05 Ver1.0 1st coding
@�����T�v :
@�߂�l   :  n :�ʎZ�b
@�⑫     :
*/
//char   *sz_time;   /* input: YYYY */
double CEngValCnvDll::ub_asc_to_dtime(char *sz_time)
{
	int i;
	struct tm t;
	double d_sec, d_0sec;
	long l_uttime;

	/*
	�N�������Z�b�g YYYY-MM-DD HH:MM:SS.mmmm
	*/
	if ((i = sscanf_s(sz_time, "%d-%d-%d %d:%d:%lf", &t.tm_year, &t.tm_mon
		, &t.tm_mday, &t.tm_hour, &t.tm_min, &d_sec)) < 6)
	{
		return (-1.0);
	};
	if ((d_sec < 0.0) || (d_sec >= 60.0)){ return (-1.0); }
	t.tm_sec = (int)floor(d_sec);
	d_0sec = d_sec - t.tm_sec;
	if ((t.tm_mon  < 1) || (t.tm_mon  > 12)){ return (-1.0); }
	if ((t.tm_mday < 1) || (t.tm_mday > 31)){ return (-1.0); }
	if ((t.tm_hour < 0) || (t.tm_hour > 23)){ return (-1.0); }
	if ((t.tm_min  < 0) || (t.tm_min  > 59)){ return (-1.0); }
	if (t.tm_year < 70){ t.tm_year += 2000; }; /* yyyy == 00 --> 2000 */
	if (t.tm_year > 1900){ t.tm_year -= 1900; }; /* yyyy == 1970 --> 70 */
	t.tm_mon -= 1;  /* mon == 1..12 --> 0..11 */
	t.tm_isdst = 0; /* daylight flg off */
	if ((l_uttime = (long)mktime(&t)) == -1){
		return (-1.0);
	};
	/* l_uttime -= timezone; /@ local-time to UTC:�폜:1997-10-23 */

#ifdef DEBUGTIMEZONE
	fprintf(stderr, "(uc_asc_to_dtime=%s:%lf)", asctime(&t), d_0sec);
#endif

	return((double)l_uttime + d_0sec);
}


/*
---------------------------------------------------------------------
$name = { �Ď����ڃ��[�h�ʒu�e�[�u���A�h���X���� }
$outline = {
�w��̊Ď����ڃ��[�h�ʒu�e�[�u���̃A�h���X����������
}
$comment = {
}
---------------------------------------------------------------------
*/
//char             sz_watch[]; /* �Ď����ږ��� [IN] */
//char             *shmadr[];  /* DB�e�[�u���A�h���X
//							 uc_get_shm_adr ()�őI��ꂽ�A�h���X�̔z��[IN] */
//struct ext_tbl_t *ext_adr[]; /* �e���o��`���[OUT] */

int CEngValCnvDll::ud_ext_adr(char sz_watch[], xobsname_t *obs_adr, xext_tbl_t *ext_adr[])
{
	char *mon_adr;
	/*----- ���L�������̃A�h���X�f�d�s -----*/

	mon_adr = (char *)obs_adr;
	while (1) {
		ext_adr[0] = (struct xext_tbl_t *)mon_adr;

		long ed_id = ds_htonl(ext_adr[0]->l_id);
		if (ed_id == EXT_ID ||
			ed_id == EXTS_ID ||
			ed_id == EXTAVG_ID ||
			ed_id == EXTAVGS_ID ||
			ed_id == EXTCHAR_ID ||
			ed_id == EXTCHARDEC_ID ||
			ed_id == EXTDBL_ID ||
			ed_id == EXTFLT_ID ||
			ed_id == EXTBCD_ID ||
			ed_id == EXTBCDS_ID) {
			return 0;
		}
		if (ed_id == ENDOFOBS_ID) {
			return -1;
		}

		long ed_len = ds_htonl(ext_adr[0]->l_len);
		mon_adr += ed_len;
	}
}


double CEngValCnvDll::dswap(double d)
{
#if IS_BIG_ENDIA
	double s = d;
	char *ps((char*)&s), *pd((char*)&d);

	ps[0] = pd[7];
	ps[1] = pd[6];
	ps[2] = pd[5];
	ps[3] = pd[4];
	ps[4] = pd[3];
	ps[5] = pd[2];
	ps[6] = pd[1];
	ps[7] = pd[0];

	return s;
#else
	return d;
#endif
}

double CEngValCnvDll::dswap2(double d)
{
	double s = d;
	char *ps((char*)&s), *pd((char*)&d);

	ps[0] = pd[7];
	ps[1] = pd[6];
	ps[2] = pd[5];
	ps[3] = pd[4];
	ps[4] = pd[3];
	ps[5] = pd[2];
	ps[6] = pd[1];
	ps[7] = pd[0];

	return s;
}

float CEngValCnvDll::fswap(float d)
{
	float s = d;
	char *ps((char*)&s), *pd((char*)&d);

	ps[0] = pd[3];
	ps[1] = pd[2];
	ps[2] = pd[1];
	ps[3] = pd[0];

	return s;
}


long CEngValCnvDll::ds_htonl(long val)
{
#if IS_BIG_ENDIA
	return htonl(val);
#else
	return val;
#endif


}

/*<a name="uc_valchk"></a>
@function : uc_valchk()
@name     : ���l������`�F�b�N���o�C�i���l�ϊ�����
@���藚�� : 92-05-29 Ver1.0 1st coding  mika
@���藚�� : 92-07-03 Ver1.1   mika
@���藚�� : 92-08-17 Ver1.2   mika
@���藚�� : 97-01-16 Ver2.0   T.S.
@���藚�� : 97-12-17 Ver2.1   T.S. (K/M�Ή�)
@���藚�� : 98-04-09 Ver2.2   T.S. (8086�Ή�)
@���藚�� : 2000-08-23 Ver2.4 �W�i��(0t777,0T777)�Ή�
@�����T�v : �ϊ����镶�����^�����l������`�F�b�N�ƃo�C�i���l�ϊ����s��
@            1) s        ---<  input : sz_ :  n-byte : >---
@                             �ϊ����镶����
@            2)i_p       ---<  output : long  :  1  :  >---
@                             �ϊ����ꂽ4�o�C�g�̃o�C�i���f�[�^
@                             (i_p=NULL�̏ꍇ�͌��ʂ͖߂��Ȃ�)
@            3)d_p       ---<  output  : double : 1  : >---
@                             �ϊ����ꂽ8�o�C�g��double�f�[�^
@                             (d_p=NULL�̏ꍇ�͌��ʂ͖߂��Ȃ�)

@                             ('0x'+16�i��, '0b'+2�i��, '0t'+8�i����
@                              �ꍇ�͕����Ȃ��f�[�^�Ƃ݂Ȃ��̂Œ���)

@                             ���̃|�C���^�l�� -1 �̏ꍇ�͑�S�p��
@                             ���[�^(l_opt)���L���ɂȂ�

@            4)l_opt     ---<  input  :  long  :  1  : >---
@                             1000 �܂��́A1024 ���w�肷��ƁA���l
@                             �̌��� K M ���t������Ă����ꍇ��
@                             �ϊ��l�Ƃ��āAK:1000�{�܂���1024�{
@                             M:1000*1000�{�܂���1024*1024�{�̒l
@                             ��Ԃ�
@
@�߂�l   :    (�`�F�b�N/�ϊ����ꂽ�f�[�^�̌^������)
@              -1 : ���l�ł͂Ȃ�
@                          (�ϊ����ʂ͂��ǂ��Ȃ�����)
@               0 : s��NULL�|�C���^�ł�����
@                          (�ϊ����ʂ͂��ǂ��Ȃ�����)
@               1 : long�^�̐��l�Ƃ��ĕϊ�����
@               (10�i��, '0x'+16�i��, '0b'+2�i���̏ꍇ)
@               (8�i��, '0t'+8�i���̏ꍇ)
@
@               2 : double�^�̐��l�Ƃ��ĕϊ�����
@                   (999.99, 99.9e99�`���̏ꍇ)
@                   (long�ł̓I�[�o�[�t���[����ꍇ)
@
@�⑫     :
*/
//char    *s;                             /* �ϊ����镶���� */
//INT32bit *i_p;                           /* �ϊ����ꂽ4�o�C�g�̃o�C�i���f�[�^ */
//double  *d_p;                           /* �ϊ����ꂽ8�o�C�g��double�f�[�^   */
//INT32bit l_opt;                          /* d_p==-1�̎��̂ݗL��*/

int CEngValCnvDll::uc_valchk(char *s, int *i_p, double *d_p, int l_opt)
{
	int     i = 2;                            /* ������J�E���^ */
	int     j = 0;                            /* �J�E���^ */
	char    sz_dmy[512];                    /* sscanf�_�~�[�o�b�t�@ */

	long    l_bitbuf = 0;                     /* �r�b�g�V�t�g�o�b�t�@ */
	double  d_bitbuf = 0.0;                   /* 32bit�ȏ�̏ꍇ */
	long    l_data;                         /* �v�Z���ʃf�[�^  */
	double  d_data;

	union   xumoji  mojibuf;                 /* ������擪2�����̃o�b�t�@ */



	/*********************************************************************/
	/*                             PROCEDURE                             */
	/*********************************************************************/

	/* s��NULL���H */
	if (s == NULL)  return(0);

	/* ������擪2�����̎擾 */
	//strncpy(mojibuf.c_moji, 2, s);
	//strncpy_s(mojibuf.c_moji, 2, s, 2);
	memcpy(mojibuf.c_moji, s, 2);

	/* ������擪2�����ɂ�菈����I�� */
	switch (mojibuf.s_moji) {

	case LX: /* 0X */
	case SX: /* 0x */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 4;    /* ����4�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 16;
			if (0x30 <= s[i] && 0x39 >= s[i]) {
				l_data = (long)(s[i] - 0x30);
				l_bitbuf = l_bitbuf | l_data; /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else  if (0x61 <= (s[i] = tolower(s[i])) && 0x66 >= (s[i] = tolower(s[i]))) {
				l_data = (long)(s[i] - 0x57);
				l_bitbuf = l_bitbuf | l_data;  /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else {
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LB: /* 0B */
	case SB: /* 0b */
		while (s[i] != iNULL) { /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 1;    /* ����1�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 2;
			switch (s[i]) {
			case '0': break;
			case '1': l_bitbuf = l_bitbuf + 1;
				d_bitbuf = d_bitbuf + 1;
				break;
			default:  return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LT: /* 0T */
	case ST: /* 0t */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 3;    /* ����3�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 8;
			if ((s[i] >= '0') && (s[i] <= '7')){
				l_bitbuf = l_bitbuf + (s[i] - '0'); /* 0..7 */
				d_bitbuf = d_bitbuf + (s[i] - '0'); /* 0..7 */
			}
			else{
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	default:
		if ((int)d_p == -1){
			//j = sscanf(s, "%lf%s", &d_data, sz_dmy);
			//j = sscanf_s(s, "%lf%s", &d_data, sz_dmy, sizeof(sz_dmy));
			j = sscanf_s(s, "%lf%s", &d_data, sz_dmy, (unsigned)_countof(sz_dmy));
			if (j == 1){
				; /*d_data�����̂܂ܕԂ�*/
			}
			else if (j == 2){
				if (!strcmp(sz_dmy, "K")){
					d_data = d_data * l_opt;
				}
				else if (!strcmp(sz_dmy, "M")){
					d_data = d_data * l_opt * l_opt;
				}
				else{
					return(-1); /*ERROR*/
				}
			}
			else{
				return(-1); /*ERROR*/
			}
			if (i_p != NULL){ *i_p = (int)d_data; }
			return(1); /*�����l*/
		}
		//if (sscanf(s, "%lf%s", &d_data, sz_dmy) != 1){ return(-1); }
		//if (sscanf_s(s, "%lf%s", &d_data, sz_dmy, sizeof(sz_dmy)) != 1){ return(-1); }
		if (sscanf_s(s, "%lf%s", &d_data, sz_dmy, (unsigned)_countof(sz_dmy)) != 1){ return(-1); }
		if ((d_data >  2147483647.0) || (d_data < -2147483648.0)){
			if (i_p != NULL){ *i_p = 0x80000000; }
			if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
			return(2); /*integer���I�[�o�[�t���[����̂Ŏ����l*/
		}
		else{
			/* �����l? */
			if ((strchr(s, '.') == NULL) && (strchr(s, 'e') == NULL) && (strchr(s, 'E') == NULL)){
				if (i_p != NULL){ *i_p = (int)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(1); /*�����l*/
			}
			else{
				if (i_p != NULL){ *i_p = (int)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(2);
			}
		}
	}/*end switch*/
}

/***********************************************************************************
* uc_valchk()��64�r�b�g(long long)�o�[�W����
************************************************************************************/

/*<a name="uc_valchkll"></a>
@function : uc_valchkll()
@name     : ���l������`�F�b�N���o�C�i���l�ϊ�������64bit(long long)�o�[�W����
@���藚�� : 2010-09-19 Ver0.0 1st coding
@�����T�v : uc_valchk()��64bit(long long)�o�[�W����
@�⑫     :
*/
//char        *s;     /*input:  �ϊ����镶���� */
//INT64bit    *i_p;   /*output: �ϊ����ꂽ8�o�C�g�̃o�C�i���f�[�^ */
//long double *d_p;   /*output: �ϊ����ꂽ10.or.16�o�C�g��long double�f�[�^   */
//long         l_opt; /*input:  �ʏ�́AiNULL���w�肷�邱��;d_p==-1�̎��̂ݗL�� */


int CEngValCnvDll::uc_valchkll(char *s, long *i_p, long double *d_p, long l_opt)
{
	int     i = 2;                            /* ������J�E���^ */
	int     j = 0;                            /* �J�E���^ */
	char    sz_dmy[512];                    /* sscanf�_�~�[�o�b�t�@ */

	unsigned long long l_bitbuf = 0;          /* �r�b�g�V�t�g�o�b�t�@ */
	long double        d_bitbuf = 0.0;        /* 32bit�ȏ�̏ꍇ */
	long long          l_data;              /* �v�Z���ʃf�[�^  */
	long double        d_data;

	union   xumoji  mojibuf;                 /* ������擪2�����̃o�b�t�@ */



	/*********************************************************************/
	/*                             PROCEDURE                             */
	/*********************************************************************/

	/* s��NULL���H */
	if (s == NULL)  return(0);

	/* ������擪2�����̎擾 */
	//strncpy(mojibuf.c_moji, s, 2);
	//	strcpy_s(mojibuf.c_moji, 2, s);
	memcpy(mojibuf.c_moji, s, 2);

	/* ������擪2�����ɂ�菈����I�� */
	switch (mojibuf.s_moji) {

	case LX: /* 0X */
	case SX: /* 0x */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 4;    /* ����4�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 16;
			if (0x30 <= s[i] && 0x39 >= s[i]) {
				l_data = (long long)(s[i] - 0x30);
				l_bitbuf = l_bitbuf | l_data; /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else  if (0x61 <= (s[i] = tolower(s[i])) && 0x66 >= (s[i] = tolower(s[i]))) {
				l_data = (long long)(s[i] - 0x57);
				l_bitbuf = l_bitbuf | l_data;  /* OR */
				d_bitbuf = d_bitbuf + l_data;
			}
			else {
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = (long)l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LB: /* 0B */
	case SB: /* 0b */
		while (s[i] != iNULL) { /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 1;    /* ����1�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 2;
			switch (s[i]) {
			case '0': break;
			case '1': l_bitbuf = l_bitbuf + 1;
				d_bitbuf = d_bitbuf + 1;
				break;
			default:  return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = (long)l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	case LT: /* 0T */
	case ST: /* 0t */
		while (s[i] != iNULL) {  /* 2005-1-7 */
			l_bitbuf = l_bitbuf << 3;    /* ����3�r�b�g�V�t�g */
			d_bitbuf = d_bitbuf * 8;
			if ((s[i] >= '0') && (s[i] <= '7')){
				l_bitbuf = l_bitbuf + (s[i] - '0'); /* 0..7 */
				d_bitbuf = d_bitbuf + (s[i] - '0'); /* 0..7 */
			}
			else{
				return(-1);
			}
			i++;
		}
		if (i_p != NULL){ *i_p = (long)l_bitbuf; }
		if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_bitbuf; }
		return(1);
		break;

	default:
		if ((int)d_p == -1){
			//j = sscanf(s, "%Lf%s", &d_data, sz_dmy); /* %Lf: long double */
			//j = sscanf_s(s, "%Lf%s", &d_data, sz_dmy, sizeof(sz_dmy)); /* %Lf: long double */
			j = sscanf_s(s, "%Lf%s", &d_data, sz_dmy, (unsigned)_countof(sz_dmy)); /* %Lf: long double */
			if (j == 1){
				; /*d_data�����̂܂ܕԂ�*/
			}
			else if (j == 2){
				if (!strcmp(sz_dmy, "K")){
					d_data = d_data * l_opt;
				}
				else if (!strcmp(sz_dmy, "M")){
					d_data = d_data * l_opt * l_opt;
				}
				else{
					return(-1); /*ERROR*/
				}
			}
			else{
				return(-1); /*ERROR*/
			}
			if (i_p != NULL){ *i_p = (long)d_data; }
			return(1); /*�����l*/
		}
		//if (sscanf(s, "%Lf%s", &d_data, sz_dmy) != 1){ return(-1); } /* %Lf: long double */
		//if (sscanf_s(s, "%Lf%s", &d_data, sz_dmy, sizeof(sz_dmy)) != 1){ return(-1); } /* %Lf: long double */
		if (sscanf_s(s, "%Lf%s", &d_data, sz_dmy, (unsigned)_countof(sz_dmy)) != 1){ return(-1); } /* %Lf: long double */
		if ((d_data > 9223372036854775807.0L) || (d_data < -9223372036854775808.0L)){
			if (i_p != NULL){ *((unsigned long long *)i_p) = 0x8000000000000000LL; }
			if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
			return(2); /*integer���I�[�o�[�t���[����̂Ŏ����l*/
		}
		else{
			/* �����l? */
			//if (sscanf(s, "%lld%s", &l_data, sz_dmy) == 1){ /* ���� */
			//if (sscanf_s(s, "%lld%s", &l_data, sz_dmy, sizeof(sz_dmy)) == 1){ /* ���� */
			if (sscanf_s(s, "%lld%s", &l_data, sz_dmy, (unsigned)_countof(sz_dmy)) == 1){ /* ���� */

				if (i_p != NULL){ *i_p = (long)l_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(1); /*�����l*/

			}
			else{ /* �����łȂ��Ȃ���� */

				if (i_p != NULL){ *i_p = (long)d_data; }
				if ((d_p != NULL) && ((int)d_p != -1)){ *d_p = d_data; }
				return(2);

			}
		}
	}/*end switch*/
}

/*
---------------------------------------------------------------------
$name = { �ݔ��Ď��c�a�t�@�C���Ď����ڃ`�F�b�N }
$outline = {
�ݔ��Ď��c�a�t�@�C�����Ď����ږ��̂����݂��邩�`�F�b�N���A
���̃f�[�^���X�e�[�^�X���ڂ����ׂ�B
}
$comment = {
}
---------------------------------------------------------------------
*/
//char             sz_watch[]; /* �Ď����ږ��� [IN] */
//char             *sz_path;   /* DB�e�[�u���t�@�C���p�X��[IN] */
//char             *shmadr[];  /* DB�e�[�u���A�h���X[IN] */
//INT32bit         l_n;        /* �X�e�[�^�e�X������̃e�[�u���� */
//INT32bit         *l_grmd;    /* �X�e�[�^�X�t���O[OUT] */
//INT32bit         *l_offset;  /* �Ď��f�[�^�̃I�t�Z�b�g�ʒu[OUT]*/
//char             *sz_sts_tbl[]; /* �X�e�[�^�X������e�[�u��[OUT]
//								sz_sts_tbl[0] = other�̕�����A�h���X�i�����ꍇ��NULL)
//								sz_sts_tbl[1] = �X�e�[�^�X������
//								sz_sts_tbl[2] = �X�e�[�^�X������
//								.
//								.
//								.
//								sz_sts_tbl[n] = NULL �Ō� */
int CEngValCnvDll::ud_sts_flg(char sz_watch[], char *sz_path, char *shmadr, long l_n, int *l_grmd, long *l_offset, char *sz_sts_tbl[])
{
	struct xobssts_tbl_t *obssts_adr;/* �X�e�[�^�X���e�[�u�� */
	struct xobsname_t *obs_adr; /* �Ď����ڒ�`��� */
	struct xext_tbl_t *ext_adr; /* �e���o��`��� */
	char             *shmadrs;
	char             *m_adr;
	char *mon_adr;
	long i;
	long j;


	/*----- ���L�������̃A�h���X�f�d�s -----*/

	m_adr = NULL;
	if (sz_path == NULL) {
		shmadrs = shmadr;
	}
	else {
		return -1;
	}

	if (ud_obsname_adr(sz_watch, shmadrs, &obs_adr) != 0) {
		return -1;
	}

	mon_adr = (char *)obs_adr;
	*l_offset = (long)(mon_adr - shmadrs);

	while (1) {
		ext_adr = (struct xext_tbl_t *)mon_adr;

		if ((sz_sts_tbl != (char **)NULL) && (ds_htonl(ext_adr->l_id) == OBSSTS_ID)) {
			*l_grmd = 1;
			if (m_adr != NULL) {
				free(m_adr);
			}
			if (l_n < 1) {
				return 0;
			}
			obssts_adr = (struct xobssts_tbl_t *)mon_adr;
			if (obssts_adr->sz_i_sts[0] == '\0') {
				sz_sts_tbl[0] = NULL;
				i = 0;
			}
			else {
				sz_sts_tbl[0] = obssts_adr->sz_i_sts;
				i = 1;
			}
			for (j = 0; j < obssts_adr->l_sts_cnt; j++, i++) {
				if (i >= l_n) {
					return 0;
				}
				sz_sts_tbl[i] = obssts_adr->chk_t[j].sz_sts;
			}
			if (i >= l_n) {
				return 0;
			}
			sz_sts_tbl[i] = NULL;
			return 0;
		}
		if (ds_htonl(ext_adr->l_id) == TIMECVT_ID) {
			*l_grmd = 2;
			if (m_adr != NULL) {
				free(m_adr);
			}
			return 0;
		}
		if (ds_htonl(ext_adr->l_id) == ENDOFOBS_ID) {
			*l_grmd = 0;
			if (m_adr != NULL) {
				free(m_adr);
			}
			return 0;
		}
		mon_adr += ds_htonl(ext_adr->l_len);
	}
}

/*
---------------------------------------------------------------------
$name = { �Ď����ږ��e�[�u���A�h���X���� }
$outline = {
�w��̊Ď����ږ��̃e�[�u���̃A�h���X����������
}
$comment = {
}
---------------------------------------------------------------------
*/
//char             sz_watch[]; /* �Ď����ږ��� [IN] */
//char             *shmadr[];  /* DB�e�[�u���A�h���X
//							 uc_get_shm_adr ()�őI��ꂽ�A�h���X�̔z��[IN] */
//struct obsname_t *obs_adr[]; /* �Ď����ڒ�`���[OUT] */
int CEngValCnvDll::ud_obsname_adr(char sz_watch[], char *shmadr, struct xobsname_t *obs_adr[])
{
	char *mon_adr;

	/*----- ���L�������̃A�h���X�f�d�s -----*/

	// �������A������DB�ɒ��ڒl����������ł���炵���̂ŁA���������Ȃ��B
	//mon_adr = db_data2;
	mon_adr = shmadr;

	mon_adr += (sizeof(long) * 2);

	int a = 0;

	while (1) {
		obs_adr[0] = (struct xobsname_t *)mon_adr;
		long id = ds_htonl(obs_adr[0]->l_id);

		a++;

		if (id == 0) {
			return -1;
		}
		if (id == OBSNAME_ID) {
			if (strcmp(sz_watch, obs_adr[0]->sz_obsname) == 0) {
				return 0;
			}
		}
		mon_adr += ds_htonl(obs_adr[0]->l_len);
	}
}