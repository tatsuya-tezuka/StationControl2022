// FileAccess.cpp : 実装ファイル
//

#include "stdafx.h"
#ifdef _CONTROLMAN
#include "..\ControlManagement\ControlManagement.h"
#else
#include "..\StationControl\StationControl.h"
#endif

#include "FileAccess.h"


// CFileAccess

CFileAccess::CFileAccess()
{
	mClientTerminal = eClientTksc;
}

CFileAccess::~CFileAccess()
{
}

/*============================================================================*/
/*! 制御権利管理

-# 対象端末の取得・設定

@param
@retval

*/
/*============================================================================*/
int CFileAccess::GetClientTerminal()
{
	char host_name[256]; // ホストの名前
	// ホスト名の取得
	if (gethostname(host_name, 256) == 0){
		// 取得できた
		CString hname = CString(host_name);
		for (int i = 0; i < sizeof(mHostNameString) / sizeof(mHostNameString[0]); i++){
			if (hname.Left(5).CompareNoCase(mHostNameString[i]) == 0){
				mClientTerminal = eClientTksc;
				if (i <= eHost_sttn)		mClientTerminal = eClientSttn;
				else if (i == eHost_ssoc)	mClientTerminal = eClientSsoc;
				else if (i == eHost_tksc)	mClientTerminal = eClientTksc;
				return mClientTerminal;
			}
		}
	}
	return mClientTerminal;
}
void CFileAccess::SetClientTerminal(int val)
{
	mClientTerminal = val;
}

/*============================================================================*/
/*! 制御権利管理

-# 制御権利管理用パスワードファイルの読み込み（専用）

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::ReadManagementPassword(CString& val)
{
	bool ret = false;
	CFile file;
	CString name;
	val.Empty();
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[eManPass]);
	if (file.Open(name, CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary) != NULL){
		try
		{
			ret = true;
			int len = 0;
			file.Read(&len, sizeof(int));
			char *pbuf = new char[len+1];
			memset(pbuf, 0, len + 1);
			file.Read(pbuf, len);
			val = CString(pbuf);
			delete[] pbuf;
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		file.Close();
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 制御権利管理用パスワードファイルの書き込み（専用）

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::WriteManagementPassword(CString val)
{
	bool ret = false;
	CFile file;
	CString name;
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[eManPass]);
	if (file.Open(name, CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) != NULL){
		try
		{
			ret = true;
			int len = val.GetLength();
			file.Write(&len, sizeof(int));
			file.Write(CStringA(val), len);
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		file.Flush();
		file.Close();
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 運用アカウントファイルの読み込み（専用）

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::ReadOperationAccount(vector<stAccount>& val)
{
	bool ret = false;
	CFile file;
	CString name;
	val.clear();
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[eOpeAccount]);
	if (file.Open(name, CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary) != NULL){
		try
		{
			ret = true;
			UINT size = 0;
			file.Read(&size, sizeof(UINT));
			for (UINT i = 0; i < size; i++){
				stAccount data;
				int len=0;
				file.Read(&len, sizeof(int));
				char *pbuf = new char[len + 1];
				memset(pbuf, 0, len + 1);
				file.Read(pbuf, len);
				data.user = CString(pbuf);
				delete[] pbuf;
				file.Read(&len, sizeof(int));
				pbuf = new char[len + 1];
				memset(pbuf, 0, len + 1);
				file.Read(pbuf, len);
				data.usersid = CString(pbuf);
				delete[] pbuf;
				file.Read(&data.usc20, sizeof(bool));
				file.Read(&data.usc34, sizeof(bool));
				file.Read(&data.udsc64, sizeof(bool));
				file.Read(&data.xdsc54, sizeof(bool));
				file.Read(&data.enable, sizeof(bool));
				file.Read(&data.mycomputer, sizeof(bool));
				file.Read(&data.dummy, sizeof(int));
				val.push_back(data);
			}
		}
		catch (CArchiveException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		file.Close();
	}

#if 0
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
		);
	// lpMsgBuf 内のすべての挿入シーケンスを処理する。
	// ...
	// 文字列を表示する。
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION);
	TRACE("%s\n", CStringA((LPCTSTR)lpMsgBuf));
	// バッファを解放する。
	LocalFree(lpMsgBuf);
#endif // 0


	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 運用アカウントファイルの書き込み（専用）

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::WriteOperationAccount(vector<stAccount> val)
{
	bool ret = false;
	CFile file;
	CString name;
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[eOpeAccount]);
	if (file.Open(name, CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) != NULL){
		try
		{
			ret = true;
			UINT size = (UINT)val.size();
			file.Write(&size, sizeof(UINT));
			vector<stAccount>::iterator itr;
			for (itr = val.begin(); itr != val.end(); itr++){
				int len = (*itr).user.GetLength();
				file.Write(&len, sizeof(int));
				file.Write(CStringA((*itr).user), len);
				len = (*itr).usersid.GetLength();
				file.Write(&len, sizeof(int));
				file.Write(CStringA((*itr).usersid), len);
				file.Write(&(*itr).usc20, sizeof(bool));
				file.Write(&(*itr).usc34, sizeof(bool));
				file.Write(&(*itr).udsc64, sizeof(bool));
				file.Write(&(*itr).xdsc54, sizeof(bool));
				file.Write(&(*itr).enable, sizeof(bool));
				file.Write(&(*itr).mycomputer, sizeof(bool));
				file.Write(&(*itr).dummy, sizeof(int));
			}
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		file.Flush();
		file.Close();
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 筑波局運用制御ファイルの読み込み(LOCAL)

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::ReadLocalControl(stLocalControl& val, int type)
{
	bool ret = false;
	FILE *fp;
	CString name;
	memset(&val, 0, sizeof(stLocalControl));
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[type]);
	if ((fopen_s(&fp, CStringA(name), "r") == 0) && (fp != NULL)){
		try
		{
			ret = true;
			char line[100];
			CString cbuf, str;
			while (fgets(line, 100, fp)){
				cbuf = CString(line);
				cbuf.Replace(_T("\n"), _T("\0"));
				if (cbuf.Find(mLocalHeader) >= 0){
					// ヘッダーなので何もしない
				}
				else if (cbuf.Find(mLocalItemOpflag) >= 0){
					// 制御権利取得有無フラグ
					AfxExtractSubString(str, cbuf, 2, ' ');
					val.flag = (str == _T("1")) ? true : false;
				}
				else if (cbuf.Find(mLocalItemLatestdate) >= 0){
					// 制御権利取得開始日時
					AfxExtractSubString(str, cbuf, 2, ' ');
					CString strTime;
					AfxExtractSubString(strTime, cbuf, 3, ' ');
					sprintf_s(val.latestdate, mMaxDate, "%s %s", (LPCSTR)CStringA(str), (LPCSTR)CStringA(strTime));
				}
				else if (cbuf.Find(mLocalItemLifetime) >= 0){
					// 制御権利取得時間
					AfxExtractSubString(str, cbuf, 2, ' ');
					sprintf_s(val.lifetime, mMaxLifetime, "%s", (LPCSTR)CStringA(str));
				}
			}
		}
		catch (CArchiveException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		fclose(fp);
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 筑波局運用制御ファイルの書き込み(LOCAL)

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::WriteLocalControl(stLocalControl val, int type)
{
	bool ret = false;
	FILE *fp;
	CString name;
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[type]);
	if ((fopen_s(&fp, CStringA(name), "w") == 0) && fp != NULL){
		try
		{
			// 現在時刻
			CTime time;
			SYSTEMTIME st_UTC;
			GetSystemTime(&st_UTC);
			time = CTime(st_UTC);
			CString strtime = time.Format("%Y-%m-%d %H:%M:%S");
			ret = true;
			CString str;
			str.Format(_T("%s %s%s\n"), (LPCTSTR)mLocalHeader, (LPCTSTR)mLocalHeaderUpdate, (LPCTSTR)strtime);
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mLocalItemOpflag, (val.flag == true) ? (LPCTSTR)_T("1") : (LPCTSTR)_T("0"));
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mLocalItemLatestdate, (LPCTSTR)strtime);
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mLocalItemLifetime, (LPCTSTR)CString(val.lifetime));
			fputs(CStringA(str), fp);
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		fclose(fp);
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 筑波局運用制御ファイルの読み込み

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::ReadTKSCControl(stControl& val, int type)
{
	bool ret = false;
	FILE *fp;
	CString name;
	memset(&val, 0, sizeof(stControl));
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[type]);
	if ((fopen_s(&fp, CStringA(name), "r") == 0) && (fp != NULL)){
		try
		{
			ret = true;
			char line[100];
			CString cbuf, str;
			while (fgets(line, 100, fp)){
				cbuf = CString(line);
				cbuf.Replace(_T("\n"), _T("\0"));
				if (cbuf.Find(mHeader) >= 0){
					// ヘッダーなので何もしない
				}
				else if (cbuf.Find(mItemOpflag) >= 0){
					// 制御権利取得有無フラグ
					AfxExtractSubString(str, cbuf, 2, ' ');
					val.flag = (str == _T("1")) ? true : false;
				}
				else if (cbuf.Find(mItemPassword) >= 0){
					// 制御権利取得パスワード
					AfxExtractSubString(str, cbuf, 2, ' ');
					memcpy(val.password, (char*)(const char*)CStringA(str), mMaxPassword);
				}
				else if (cbuf.Find(mItemLatestdate) >= 0){
					// 制御権利取得開始日時
					AfxExtractSubString(str, cbuf, 2, ' ');
					CString strTime;
					AfxExtractSubString(strTime, cbuf, 3, ' ');
					sprintf_s(val.latestdate, mMaxDate, "%s %s", (LPCSTR)CStringA(str), (LPCSTR)CStringA(strTime));
				}
				else if (cbuf.Find(mItemLifetime) >= 0){
					// 制御権利取得時間
					AfxExtractSubString(str, cbuf, 2, ' ');
					sprintf_s(val.lifetime, mMaxLifetime, "%s", (LPCSTR)CStringA(str));
				}
			}
		}
		catch (CArchiveException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		fclose(fp);
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 筑波局運用制御ファイルの書き込み

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::WriteTKSCControl(stControl val, int type)
{
	bool ret = false;
	FILE *fp;
	CString name;
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[type]);
	CString ftpname;
	ftpname.Format(_T("%s%s\\%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManagementTksc, (LPCTSTR)mManFileName[type]);
	if ((fopen_s(&fp, CStringA(name), "w") == 0) && (fp != NULL)){
		try
		{
			// 現在時刻
			CTime time;
			SYSTEMTIME st_UTC;
			GetSystemTime(&st_UTC);
			time = CTime(st_UTC);
			CString strtime = time.Format("%Y-%m-%d %H:%M:%S");
			ret = true;
			CString str;
			str.Format(_T("%s\n"), (LPCTSTR)mHeader);
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemOpflag, (val.flag == true) ? (LPCTSTR)_T("1") : (LPCTSTR)_T("0"));
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemPassword, (LPCTSTR)CString(val.password));
			fputs(CStringA(str), fp);
			str.Format(_T("%s1970-1-1 00:00:00\n"), (LPCTSTR)mItemLatestdate);
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemLifetime, (LPCTSTR)CString(val.lifetime));
			fputs(CStringA(str), fp);
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		fclose(fp);
		if (ret == true){
			// 対象ファイルを対象のフォルダにコピーする、コピーした後にサーバーへ通知
			//CopyFile(name, ftpname, FALSE);
			ExecShell(eClientTksc, type);
		}
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 局運用制御ファイルの読み込み

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::ReadSOSControl(stControl& val, int type)
{
	bool ret = false;
	FILE *fp;
	CString name;
	memset(&val, 0, sizeof(stControl));
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[type]);
	if ((fopen_s(&fp, CStringA(name), "r") == 0) && (fp != NULL)){
		try
		{
			ret = true;
			char line[100];
			CString cbuf, str;
			while (fgets(line, 100, fp)){
				cbuf = CString(line);
				cbuf.Replace(_T("\n"), _T("\0"));
				if (cbuf.Find(mHeader) >= 0){
					// ヘッダーなので何もしない
				}
				else if (cbuf.Find(mItemOpflag) >= 0){
					// 制御権利取得有無フラグ
					AfxExtractSubString(str, cbuf, 2, ' ');
					val.flag = (str == _T("1")) ? true : false;
				}
				else if (cbuf.Find(mItemPassword) >= 0){
					// 制御権利取得パスワード
					AfxExtractSubString(str, cbuf, 2, ' ');
					memcpy(val.password, (char*)(const char*)CStringA(str), mMaxPassword);
				}
				else if (cbuf.Find(mItemLatestdate) >= 0){
					// 制御権利取得開始日時
					AfxExtractSubString(str, cbuf, 2, ' ');
					CString strTime;
					AfxExtractSubString(strTime, cbuf, 3, ' ');
					sprintf_s(val.latestdate, mMaxDate, "%s %s", (LPCSTR)CStringA(str), (LPCSTR)CStringA(strTime));
				}
				else if (cbuf.Find(mItemLifetime) >= 0){
					// 制御権利取得時間
					AfxExtractSubString(str, cbuf, 2, ' ');
					sprintf_s(val.lifetime, mMaxLifetime, "%s", (LPCSTR)CStringA(str));
				}
			}
		}
		catch (CArchiveException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		fclose(fp);
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 局運用制御ファイルの書き込み

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::WriteSOSControl(stControl val, int type, bool bDefault/* = false*/)
{
	bool ret = false;
	FILE *fp;
	CString name;
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[type]);
	CString ftpname;
	ftpname.Format(_T("%s%s\\%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManagementSttn, (LPCTSTR)mManFileName[type]);
	if ((fopen_s(&fp, CStringA(name), "w") == 0) && (fp != NULL)){
		try
		{
			// 現在時刻
			CTime time;
			SYSTEMTIME st_UTC;
			GetSystemTime(&st_UTC);
			time = CTime(st_UTC);
			CString strtime = time.Format("%Y-%m-%d %H:%M:%S");
			if (bDefault == true){
				strtime = _T("1970-1-1 00:00:00");
			}
			ret = true;
			CString str;
			str.Format(_T("%s\n"), (LPCTSTR)mHeader);
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemOpflag, (val.flag == true) ? (LPCTSTR)_T("1") : (LPCTSTR)_T("0"));
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemPassword, (LPCTSTR)CString(val.password));
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemLatestdate, (LPCTSTR)strtime);
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemLifetime, (LPCTSTR)CString(val.lifetime));
			fputs(CStringA(str), fp);
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		fclose(fp);
		if (ret == true){
			// 対象ファイルを対象のフォルダにコピーする、コピーした後にサーバーへ通知
			//CopyFile(name, ftpname, FALSE);
			ExecShell(eClientSsoc, type);
		}
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 相模原局管制制御ファイルの読み込み

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::ReadSSOCControl(stControl& val, int type)
{
	bool ret = false;
	FILE *fp;
	CString name;
	memset(&val, 0, sizeof(stControl));
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[type]);
	if ((fopen_s(&fp, CStringA(name), "r") == 0) && (fp != NULL)){
		try
		{
			ret = true;
			char line[100];
			CString cbuf, str;
			while (fgets(line, 100, fp)){
				cbuf = CString(line);
				cbuf.Replace(_T("\n"), _T("\0"));
				if (cbuf.Find(mHeader) >= 0){
					// ヘッダーなので何もしない
				}
				else if (cbuf.Find(mItemOpflag) >= 0){
					// 制御権利取得有無フラグ
					AfxExtractSubString(str, cbuf, 2, ' ');
					val.flag = (str == _T("1")) ? true : false;
				}
				else if (cbuf.Find(mItemPassword) >= 0){
					// 制御権利取得パスワード
					AfxExtractSubString(str, cbuf, 2, ' ');
					memcpy(val.password, (char*)(const char*)CStringA(str), mMaxPassword);
				}
				else if (cbuf.Find(mItemLatestdate) >= 0){
					// 制御権利取得開始日時
					AfxExtractSubString(str, cbuf, 2, ' ');
					CString strTime;
					AfxExtractSubString(strTime, cbuf, 3, ' ');
					sprintf_s(val.latestdate, mMaxDate, "%s %s", (LPCSTR)CStringA(str), (LPCSTR)CStringA(strTime));
				}
				else if (cbuf.Find(mItemLifetime) >= 0){
					// 制御権利取得時間
					AfxExtractSubString(str, cbuf, 2, ' ');
					sprintf_s(val.lifetime, mMaxLifetime, "%s", (LPCSTR)CStringA(str));
				}
			}
		}
		catch (CArchiveException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		fclose(fp);
	}
	return ret;
}

/*============================================================================*/
/*! 制御権利管理

-# 相模原局管制制御ファイルの書き込み

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::WriteSSOCControl(stControl val, int type)
{
	bool ret = false;
	FILE *fp;
	CString name;
	name.Format(_T("%s%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManFileName[type]);
	CString ftpname;
	ftpname.Format(_T("%s%s\\%s"), (LPCTSTR)theApp.GetManagementPath(), (LPCTSTR)mManagementSsoc, (LPCTSTR)mManFileName[type]);
	if ((fopen_s(&fp, CStringA(name), "w") == 0) && (fp != NULL)){
		try
		{
			// 現在時刻
			CTime time;
			SYSTEMTIME st_UTC;
			GetSystemTime(&st_UTC);
			time = CTime(st_UTC);
			CString strtime = time.Format("%Y-%m-%d %H:%M:%S");
			ret = true;
			CString str;
			str.Format(_T("%s\n"), (LPCTSTR)mHeader);
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemOpflag, (val.flag == true) ? (LPCTSTR)_T("1") : (LPCTSTR)_T("0"));
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemPassword, (LPCTSTR)CString(val.password));
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemLatestdate, (LPCTSTR)strtime);
			fputs(CStringA(str), fp);
			str.Format(_T("%s%s\n"), (LPCTSTR)mItemLifetime, (LPCTSTR)CString(val.lifetime));
			fputs(CStringA(str), fp);
		}
		catch (CFileException* e)
		{
			ret = false;
			e->Delete();
		}
		catch (CMemoryException* e)
		{
			ret = false;
			e->Delete();
		}
		fclose(fp);
		if (ret == true){
			// 対象ファイルを対象のフォルダにコピーする、コピーした後にサーバーへ通知
			//CopyFile(name, ftpname, FALSE);
			ExecShell(eClientSttn, type);
		}
	}
	return ret;
}
/*============================================================================*/
/*! 制御権利管理

-# シェルの実行

@param
@retval

*/
/*============================================================================*/
bool CFileAccess::ExecShell(int kind, int type)
{
#ifdef _CONTROLMAN
	CString strDest;
	if (type == eTKSCusc20 || type == eSOSusc20 || type == eSSOCusc20){
		// UDSC20
		strDest = _T("usc20");
	}
	else if (type == eTKSCusc34 || type == eSOSusc34 || type == eSSOCusc34){
		// UDSC34
		strDest = _T("usc34");
	}
	else if (type == eTKSCudsc64 || type == eSOSudsc64 || type == eSSOCudsc64){
		// UDSC64
		strDest = _T("udsc64");
	}
	else if (type == eTKSCxdsc54 || type == eSOSxdsc54 || type == eSSOCxdsc54){
		// UDSC54
		strDest = _T("udsc54");
	}
	else{
		return false;
	}

	CString name;
	name.Format(_T("%s"), (LPCTSTR)mManFileName[type]);

	TCHAR buf[MAX_PATH];
	CString		m_strSshServerName;					// SSHサーバー名称
	CString		m_strSshUserName;					// SSHユーザ名称
	CString		m_strSshKeyPath;					// SSHキーパス
	CString		m_strBatchPath;						// SSHバッチファイルパス 

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHSERVER_NAME, _T(""), buf, MAX_PATH, theApp.GetAppIniFileName());
	m_strSshServerName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHUSER_NAME, _T(""), buf, MAX_PATH, theApp.GetAppIniFileName());
	m_strSshUserName = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHKEY_PATH, _T(""), buf, MAX_PATH, theApp.GetAppIniFileName());
	m_strSshKeyPath = buf;

	GetPrivateProfileString(SECTION_SYSTEM, KEY_SSHBATCH_PATH, _T(""), buf, MAX_PATH, theApp.GetAppIniFileName());
	m_strBatchPath = buf;

	CSshShell::structSshParam stParam;
	stParam.strBatchPath = m_strBatchPath;
	stParam.strUserName = m_strSshUserName;
	stParam.strServerName = m_strSshServerName;
	stParam.strKeyFilePath = m_strSshKeyPath;
	stParam.strRemShellName = SEND_CRM_SH;
	CString arg;
	// 第一引数：対象の設備制御装置
	// 第二引数：対象ファイル
	arg.Format(_T("%s %s"), (LPCTSTR)strDest.MakeLower(), (LPCTSTR)name.MakeLower());
	stParam.strRemShellParam = arg;

	int err = CSshShell::ExecShell(stParam);

	if (err == 0)
		return true;

	return false;
#else
	return true;
#endif
}
