#include "stdafx.h"
#include "EngValConvDLL.h"
#include "EngValConvApi.h"

static CEngValCnvDll mEng;

/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# EQMON_DB�t�@�C�����̓ǂݍ���

@param	val	EQMON_DB�t�@�C����

@retval
*/
/*============================================================================*/
void WINAPI Dll_SetEqMonDbFileName(TCHAR* val)
{
	mEng.SetEqMonDbFileName(val);
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �Ď��f�[�^�i�[�t�H���_���̂̐ݒ�

@param	val	�Ď��f�[�^�i�[�t�H���_��

@retval
*/
/*============================================================================*/
void WINAPI Dll_SetMonDataPath(TCHAR* val)
{
	mEng.SetMonDataPath(val);
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �Ď����̓o�^

@param	name	�Ď���

@retval
*/
/*============================================================================*/
void WINAPI Dll_AddMonitorName(TCHAR* name)
{
	mEng.AddMonitorName(name);
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �f�[�^�t�@�C�����X�g�̃N���A

@param

@retval
*/
/*============================================================================*/
void WINAPI Dll_ClearDataFileList()
{
	mEng.ClearDataFileList();
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �Ď��f�[�^���X�g�̃N���A

@param

@retval
*/
/*============================================================================*/
void WINAPI Dll_ClearMonitorName()
{
	mEng.ClearMonitorName();
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �f�[�^�t�@�C�����X�g�̓o�^

@param	val	�f�[�^�t�@�C����

@retval
*/
/*============================================================================*/
void WINAPI Dll_AddDataFileList(TCHAR* val)
{
	mEng.AddDataFileList(val);
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �H�w�l�ϊ�

@param	timeTarget	�H�w�l�ϊ����s�����ԃf�[�^�i���Ԕ͈́A�J�n�I�����ԁj

@retval
*/
/*============================================================================*/
bool WINAPI Dll_GetMonitorDataEx(xTL_TIME* timeTarget)
{
	return mEng.GetMonitorDataEx(timeTarget);
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �H�w�l�ϊ����s���Ď������X�g�̎擾

@param

@retval	�H�w�l�ϊ��f�[�^���X�g
*/
/*============================================================================*/
map< CString, vector<xobs_tbl_res_t> >& WINAPI Dll_GetOutDataList()
{
	return mEng.mOutDataList;
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �H�w�l�ϊ���f�[�^�̎擾

@param

@retval	�H�w�l�ϊ��f�[�^���X�g
*/
/*============================================================================*/
vector<xobs_tbl_res_t>& WINAPI Dll_GetAllVal()
{
	return mEng.data_list;
}
/*============================================================================*/
/*! �H�w�l�ϊ�DLL

-# �H�w�l�ϊ��I����ʒm����R�[���o�b�N�֐��̐ݒ�

@param	fCallBack	�R�[���o�b�N�֐�

@retval
*/
/*============================================================================*/
void WINAPI Dll_SetCallBack(NOTIFYDATACALLBACK fCallBack)
{
	return mEng.SetCallBack(fCallBack);
}
