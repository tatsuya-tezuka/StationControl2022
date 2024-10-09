#include "EngValCnv.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	void WINAPI Dll_SetEqMonDbFileName(TCHAR* val);
	void WINAPI Dll_SetMonDataPath(TCHAR* val);
	void WINAPI Dll_AddMonitorName(TCHAR* name);
	void WINAPI Dll_ClearDataFileList();
	void WINAPI Dll_ClearMonitorName();
	void WINAPI Dll_AddDataFileList(TCHAR* val);
	bool WINAPI Dll_GetMonitorDataEx(xTL_TIME* timeTarget);
	map< CString, vector<xobs_tbl_res_t> >&	WINAPI Dll_GetOutDataList();
	vector<xobs_tbl_res_t>& WINAPI Dll_GetAllVal();
	void WINAPI Dll_SetCallBack(NOTIFYDATACALLBACK fCallBack);

#ifdef __cplusplus
}
#endif
