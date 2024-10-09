#include "stdafx.h"
#include "EngValConvDLL.h"
#include "EngValConvApi.h"

static CEngValCnvDll mEng;

/*============================================================================*/
/*! 工学値変換DLL

-# EQMON_DBファイル名の読み込み

@param	val	EQMON_DBファイル名

@retval
*/
/*============================================================================*/
void WINAPI Dll_SetEqMonDbFileName(TCHAR* val)
{
	mEng.SetEqMonDbFileName(val);
}
/*============================================================================*/
/*! 工学値変換DLL

-# 監視データ格納フォルダ名称の設定

@param	val	監視データ格納フォルダ名

@retval
*/
/*============================================================================*/
void WINAPI Dll_SetMonDataPath(TCHAR* val)
{
	mEng.SetMonDataPath(val);
}
/*============================================================================*/
/*! 工学値変換DLL

-# 監視名の登録

@param	name	監視名

@retval
*/
/*============================================================================*/
void WINAPI Dll_AddMonitorName(TCHAR* name)
{
	mEng.AddMonitorName(name);
}
/*============================================================================*/
/*! 工学値変換DLL

-# データファイルリストのクリア

@param

@retval
*/
/*============================================================================*/
void WINAPI Dll_ClearDataFileList()
{
	mEng.ClearDataFileList();
}
/*============================================================================*/
/*! 工学値変換DLL

-# 監視データリストのクリア

@param

@retval
*/
/*============================================================================*/
void WINAPI Dll_ClearMonitorName()
{
	mEng.ClearMonitorName();
}
/*============================================================================*/
/*! 工学値変換DLL

-# データファイルリストの登録

@param	val	データファイル名

@retval
*/
/*============================================================================*/
void WINAPI Dll_AddDataFileList(TCHAR* val)
{
	mEng.AddDataFileList(val);
}
/*============================================================================*/
/*! 工学値変換DLL

-# 工学値変換

@param	timeTarget	工学値変換を行う時間データ（時間範囲、開始終了時間）

@retval
*/
/*============================================================================*/
bool WINAPI Dll_GetMonitorDataEx(xTL_TIME* timeTarget)
{
	return mEng.GetMonitorDataEx(timeTarget);
}
/*============================================================================*/
/*! 工学値変換DLL

-# 工学値変換を行う監視名リストの取得

@param

@retval	工学値変換データリスト
*/
/*============================================================================*/
map< CString, vector<xobs_tbl_res_t> >& WINAPI Dll_GetOutDataList()
{
	return mEng.mOutDataList;
}
/*============================================================================*/
/*! 工学値変換DLL

-# 工学値変換後データの取得

@param

@retval	工学値変換データリスト
*/
/*============================================================================*/
vector<xobs_tbl_res_t>& WINAPI Dll_GetAllVal()
{
	return mEng.data_list;
}
/*============================================================================*/
/*! 工学値変換DLL

-# 工学値変換終了を通知するコールバック関数の設定

@param	fCallBack	コールバック関数

@retval
*/
/*============================================================================*/
void WINAPI Dll_SetCallBack(NOTIFYDATACALLBACK fCallBack)
{
	return mEng.SetCallBack(fCallBack);
}
