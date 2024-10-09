// DataCollection.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "RemoteGraph.h"
#include "DataCollection.h"
#include "MainFrm.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CDataCollection

CEngValCnv*	CDataCollection::m_pEngval[eStation_MAX] = { NULL, NULL, NULL, NULL };
CEngValCnv* CDataCollection::GetEngval(int pos)
{
	return m_pEngval[pos];
}
void CDataCollection::SetEngval(int pos, CEngValCnv* eval)
{
	m_pEngval[pos] = eval;
}
void CDataCollection::DeleteEngval(int pos)
{
	TRY
	{
		delete m_pEngval[pos];
		m_pEngval[pos] = NULL;
	} CATCH_ALL(e){
		m_pEngval[pos] = NULL;
	}
	END_CATCH_ALL
}

CDataCollection::CDataCollection()
{
	mpCollectThread = NULL;
	mpSaveRealTimeDataThread = NULL;
	for( int i = 0; i < MAX_VIEW; i++ )
		mpSaveAnalysisDataThread[i] = NULL;

	for( int i = 0; i < MAX_VIEW; i++ )
	{
		mAnalysisStart[i] = CTime(0);
		mAnalysisEnd[i] = CTime(0);

		mTrendAllTblNo[i] = 0;
		mTrendZoomTblNo[i] = 0;

		mTrendZoomStart[i] = 0;
		mTrendZoomEnd[i] = 0;

		mTrendCursor[i] = 0;

		for( int j = 0; j < MAX_TDATA; j++ )
		{
			mTrendAllData[i][j].clear();
			mTrendZoomData[i][j].clear();

			mTrendZoomMMPair[i][j].cnt = 1;
			mTrendZoomMMPair[i][j].max = -DBL_MAX;
			mTrendZoomMMPair[i][j].min = DBL_MAX;

			mTrendCursorData[i][j].clear();

			m_LimitState[i][j].clear();
		}
	}

	int count = MAX_VIEW * MAX_TDATA;
	for (int i = 0; i < count; i++){
		for (int j = 0; j < NUM_REAL_SCALE_INFO_TABLE; j++){
			m_CompData[i][j].clear();
			m_CompDataForCopy[i][j].clear();
		}
	}

	m_pMessageThread = NULL;

	for (int i = 0; i < eStation_MAX; i++){
		m_bCreatePipe[i] = false;
		m_pStationMessageThread[i] = NULL;
	}
	m_ThreadNotify = 0;

	m_DummySpaceDataDeque.clear();
	m_DummySpaceDataVector.clear();

	memset(&m_DummyDrawInfo, 0, sizeof(stDrawInfo));
	m_DummyDrawInfo.start_pos = 0;
	m_DummyDrawInfo.data_num = 0;
	m_DummyDrawInfo.update_pos = 0;
	m_DummyDrawInfo.change_flg = false;
}

CDataCollection::~CDataCollection()
{
	// ���b�Z�[�W���W�p�X���b�h�̏I��
	RemoveMessageThread();

	StopCollection();
	StopAllAnalysisDataThread();

	DeleteCollectionAll();

	//	GetSocketClient().Close();
}

/*============================================================================*/
/*! �f�[�^���W

-# �Ď��f�[�^�o�^�v��

@param
@retval

*/
/*============================================================================*/
void CDataCollection::RequestAddSignalList()
{
#ifndef _NOCOMM
	vector<stSelectData>& datalist = GetSelectDataList();
	vector<stSelectData>::iterator itr;
	UINT count = (UINT)datalist.size();

	// �Ď��f�[�^�o�^�v��
	stCommandHeader	header;
	header.command = eCommand_ReqSignal;
	header.size = sizeof(stCommandHeader) + (sizeof(stReqSignalName)*count);
	theApp.GetDataCollection().GetSocketClient().SendData((char*)&header, sizeof(stCommandHeader));
	// �Ď��f�[�^���ږ��𑗐M
	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		stReqSignalName data;
		CStringA name((*itr).name);
		sprintf_s((char*)data.name, _MAX_NAME, "%s", name);
		theApp.GetDataCollection().GetSocketClient().SendData((char*)&data, sizeof(stReqSignalName));
	}

	stAckNack ack;
	UINT size = sizeof(stAckNack);
	theApp.GetDataCollection().GetSocketClient().RecvData((char*)&ack, &size, CCommBase::mDefaultCommTimeout);
	if (size == 0 || ack.size == 0 || ack.status == 0x01){
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("RequestAddSignalList"), _T("Error"), _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return;
	}
#else
#endif
}
/*============================================================================*/
/*! �f�[�^���W

-# �Ď��f�[�^���M�J�n�v��

@param
@retval

*/
/*============================================================================*/
void CDataCollection::RequestStartSignal()
{
#ifdef _PIPE_COLLECTION

	CString strDebug;
	// �H�w�l�ϊ��p�R���g���[���̍쐬
	if (CDataCollection::GetEngval(0) == NULL){
		// �H�w�l�ϊ��p�̃N���X���쐬
		for (int i = 0; i < eStation_MAX; i++){
			CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, i);
			char szFilePath[_MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);

			CDataCollection::SetEngval(i, new CEngValCnv(szFilePath));
			if (CDataCollection::GetEngval(i) == NULL){
				strDebug.Format(_T("%s(%d) :: Fail new CEngValCnv, station(%d)"), __FILEW__, __LINE__, i);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("RequestStartSignal"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
	}

	// �e�H�w�l�ϊ��p�R���g���[���̊Ď����X�g���N���A����
	for (int i = 0; i < eStation_MAX; i++){
		if (CDataCollection::GetEngval(i) != NULL)
			CDataCollection::GetEngval(i)->ClearMonitorName();
	}

	// ���S�Ď������擾����ꍇ�͈ȉ����R�����g�ɂ���
#if 1
	// �e�H�w�l�ϊ��p�R���g���[���ɊĎ�����o�^����
	vector<stSelectData>& seldatalist = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itr;
	CString sttag, eqtag, name;
	TCHAR buf[_MAX_PATH];
	for (itr = seldatalist.begin(); itr != seldatalist.end(); itr++){
		// �f�[�^�擾�Ώۃt���O���m�F
		if ((*itr).valid == false)
			continue;
		name = (*itr).name;
		AfxExtractSubString(sttag, name, 0, '.');	// �ǖ����o��
		AfxExtractSubString(eqtag, name, 1, '.');	// �ݔ������o��
		// ���ۂ̊Ď�����o�^����
		for (int i = 0; i < eStation_MAX; i++){
			if (mStationSimpleString[i] == sttag){
				wsprintf(buf, _T("%s"), (LPCTSTR)name.Mid(sttag.GetLength() + 1));
				if (CDataCollection::GetEngval(i) != NULL){
					CDataCollection::GetEngval(i)->AddMonitorName(buf);
				}
			}
		}
	}
#endif

	// �H�w�l�ϊ��̏������ł����̂ŁA�p�C�v�ʐM���m������

	// �p�C�v�̍쐬
	createPipe();


#endif
}
/*============================================================================*/
/*! �f�[�^���W

-# �Ď��f�[�^���M�I���v��

@param
@retval

*/
/*============================================================================*/
void CDataCollection::RequestStopSignal()
{
#ifdef _PIPE_COLLECTION
//	TRACE("CDataCollection::RequestStopSignal deletePipe Start\n");
//	//=====================================================//
//	//������������������������ Log ������������������������//
//	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("RequestStopSignal"), _T("Start"), _T(""), nLogEx::debug);
//	//������������������������ Log ������������������������//
//	//=====================================================//
	deletePipe();
//	TRACE("CDataCollection::RequestStopSignal deletePipe End\n");
//	//=====================================================//
//	//������������������������ Log ������������������������//
//	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("RequestStopSignal"), _T("deletePipe end"), _T(""), nLogEx::debug);
//	//������������������������ Log ������������������������//
//	//=====================================================//
	if (m_ThreadNotify != 0){
		PostThreadMessage(m_ThreadNotify, WM_QUIT, 0, 0);
		m_ThreadNotify = 0;
	}
//	//=====================================================//
//	//������������������������ Log ������������������������//
//	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("RequestStopSignal"), _T("End"), _T(""), nLogEx::debug);
//	//������������������������ Log ������������������������//
//	//=====================================================//
#endif
}

/*============================================================================*/
/*! �f�[�^���W

-# �Ď��f�[�^���ꗗ�擾�v��

@param
@retval

*/
/*============================================================================*/
void CDataCollection::RequestDataList()
{
#ifdef _PIPE_COLLECTION
	// DB����S�ǂ̊Ď������X�g���擾
	mDBAccess.all_eqmon_db_read();
#else
	// DB����S�ǂ̊Ď������X�g���擾
	mDBAccess.all_eqmon_db_read();
#endif
}

/*============================================================================*/
/*! �f�[�^���W

-# �Ď��f�[�^�̎擾

@param
@retval

*/
/*============================================================================*/
DWORD CDataCollection::GetData(LPBYTE lpData, DWORD dwSize)
{
#ifdef _PIPE_COLLECTION
#endif
	return 0;
}

/*============================================================================*/
/*! �f�[�^���W

-# ��n�ǃ��X�g�̎擾

@param		list	��n�ǖ��̊i�[�o�b�t�@
@retval

*/
/*============================================================================*/
void CDataCollection::GetBaseStationList(vector<CString>& list)
{
	list.clear();
	vector<CString>::iterator itrl;

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
//	for (itr = mDataNameList.begin(); itr != mDataNameList.end(); itr++){
	for (itr = mDBAccess.m_monnamelist.begin(); itr != mDBAccess.m_monnamelist.end(); itr++){
		AfxExtractSubString(strs, (*itr).second, 0, '.');
		AfxExtractSubString(stre, (*itr).second, 1, '.');
		bool set = true;
		for (itrl = list.begin(); itrl != list.end(); itrl++){
			if (strs == (*itrl)){
				set = false;
				break;
			}
		}
		if (set == true)
			list.push_back(strs);
	}
}
/*============================================================================*/
/*! �f�[�^���W

-# �ݔ��i�@��j���X�g�̎擾

@param		baseName	��n�ǖ���
@param		list		��n�Ǔ��̐ݔ��i�@��j���̊i�[�o�b�t�@
@retval

*/
/*============================================================================*/
void CDataCollection::GetEquipmentList(CString baseName, vector<CString>& list)
{
	list.clear();
	vector<CString>::iterator itrl;

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
//	for (itr = mDataNameList.begin(); itr != mDataNameList.end(); itr++){
	for (itr = mDBAccess.m_monnamelist.begin(); itr != mDBAccess.m_monnamelist.end(); itr++){
		AfxExtractSubString(strs, (*itr).second, 0, '.');
		AfxExtractSubString(stre, (*itr).second, 1, '.');
		if (baseName.IsEmpty() == false && baseName != strs)
			continue;
		bool set = true;
		for (itrl=list.begin(); itrl != list.end(); itrl++){
			if (stre == (*itrl)){
				set = false;
				break;
			}
		}
		if(set == true)
			list.push_back(stre);
	}
}
/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���̃��X�g�̎擾

@param		baseName	��n�ǖ���
@param		equipment	��n�Ǔ��̐ݔ��i�@��j����
@param		list		�f�[�^���̊i�[�o�b�t�@
@retval

*/
/*============================================================================*/
void CDataCollection::GetDataNameList(CString baseName, CString equipment, vector<CString>& list)
{
	list.clear();

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
//	for (itr = mDataNameList.begin(); itr != mDataNameList.end(); itr++){
	for (itr = mDBAccess.m_monnamelist.begin(); itr != mDBAccess.m_monnamelist.end(); itr++){
		AfxExtractSubString(strs, (*itr).second, 0, '.');
		AfxExtractSubString(stre, (*itr).second, 1, '.');
		if (baseName.IsEmpty() == false && baseName != strs)
			continue;
		if (equipment.IsEmpty() == false && equipment != stre)
			continue;
		list.push_back((*itr).second);
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^ID�̎擾

@param
@retval

*/
/*============================================================================*/
DWORD CDataCollection::ConvNameToId(CString name)
{
	map<DWORD, CString>::iterator itr;
//	for (itr = mDataNameList.begin(); itr != mDataNameList.end(); itr++){
	for (itr = mDBAccess.m_monnamelist.begin(); itr != mDBAccess.m_monnamelist.end(); itr++){
		if ((*itr).second == name){
			return (*itr).first;
		}
	}
	return 0xffffffff;
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���̂̎擾

@param
@retval

*/
/*============================================================================*/
CString CDataCollection::ConvIdToName(DWORD id)
{
//	map<DWORD, CString>::iterator itr = mDataNameList.find(id);
	map<DWORD, CString>::iterator itr = mDBAccess.m_monnamelist.find(id);
//	if (itr == mDataNameList.end())
	if (itr == mDBAccess.m_monnamelist.end())
		return _T("");
	return (*itr).second;
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^�o�^ID���擾����

@param

@retval
*/
/*============================================================================*/
DWORD CDataCollection::GetSelectDataId(CString name)
{
	DWORD	id = 0xffffffff;

	map<CString, DWORD>::iterator itr = mSelName2Id.find(name);
	if( itr != mSelName2Id.end() )
		id = (*itr).second;

	return id;
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^�o�^�����擾����

@param

@retval
*/
/*============================================================================*/
CString CDataCollection::GetSelectDataName(DWORD id)
{
	CString	name = _T("");

	map<DWORD, CString>::iterator itr = mSelId2Name.find(id);
	if( itr != mSelId2Name.end() )
		name = (*itr).second;

	return name;
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^�o�^�ԍ����擾����

@param

@retval
*/
/*============================================================================*/
int CDataCollection::GetSelectDataNo(DWORD id)
{
	int	index = -1;

	map<DWORD, int>::iterator itr = mSelId2Index.find(id);
	if( itr != mSelId2Index.end() )
		index = (*itr).second;

	return index;
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���W�̍폜

@param
@retval

*/
/*============================================================================*/
void CDataCollection::DeleteCollection()
{
	deque<CSpaceData>::iterator itr;

	// �Ď��f�[�^��������
	for (itr = mSpaceData.begin(); itr != mSpaceData.end(); itr++){
		(*itr).value.clear();
		(*itr).value_for_copy.clear();
	}
	mSpaceData.clear();

	// �ꎞ�Ď��f�[�^��������
	for (itr = mSpaceTemp.begin(); itr != mSpaceTemp.end(); itr++){
		(*itr).value.clear();
		(*itr).value_for_copy.clear();
	}
	mSpaceTemp.clear();

	for( int i = 0; i < MAX_VIEW; i++ )
	{
		for( int j = 0; j < MAX_TDATA; j++ )
			m_LimitState[i][j].clear();
	}

	// ���k�f�[�^��������
	for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
	{
		for( int j = 0; j < NUM_REAL_SCALE_INFO_TABLE; j++ )
		{
			m_CompData[i][j].clear();
			m_CompDataForCopy[i][j].clear();
		}
	}

	// �`�����������
	for (int i = 0; i < (MAX_VIEW * MAX_TDATA); i++)
	{
		for (int j = 0; j < NUM_REAL_SCALE_INFO_TABLE; j++)
		{
			m_TrendDraw[i][j].start_pos = 0;
			m_TrendDraw[i][j].data_num = 0;
			m_TrendDraw[i][j].update_pos = 0;
			m_TrendDraw[i][j].change_flg = false;
		}
	}

	// AUTO�N���A���s�t���O���N���A
	m_bAutoClear = false;
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���W�̊J�n

@param
@retval

*/
/*============================================================================*/
void CDataCollection::StartCollection()
{
	DeleteCollection();

	createCollectionThread();
	createSaveRealTimeDataThread();
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���W�X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::createCollectionThread()
{
	stThreadParam param;

	if (mpCollectThread == NULL){
		mpCollectThread = (CDataCollectionThread*)AfxBeginThread(RUNTIME_CLASS(CDataCollectionThread), THREAD_PRIORITY_HIGHEST/*THREAD_PRIORITY_NORMAL*/, 0, CREATE_SUSPENDED);
		param.parent = this;
		param.callback = messageProc;
		mpCollectThread->SetThreadParam(param);
		mpCollectThread->ResumeThread();
	}
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# AP�w����̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void WINAPI CDataCollection::receiveServerCallBack01(PIPEMONITORACK* pbuf)
{
	if (pbuf->header.command == ePipeCommand_finish){
		PostThreadMessage(pbuf->header.station, WM_APP + ePipeCommand_finish, 0, 0);
		return;
	}
	// UDSC64
	CGraphMutex::Lock((eMutex)(eMessage + 1));
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(0);
	data.push_back(*pbuf);
	CGraphMutex::Unlock((eMutex)(eMessage + 1));
}
void WINAPI CDataCollection::receiveServerCallBack02(PIPEMONITORACK* pbuf)
{
	if (pbuf->header.command == ePipeCommand_finish){
		PostThreadMessage(pbuf->header.station, WM_APP + ePipeCommand_finish, 0, 0);
		return;
	}
	// USC34
	CGraphMutex::Lock((eMutex)(eMessage + 3));
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(2);
	data.push_back(*pbuf);
	CGraphMutex::Unlock((eMutex)(eMessage + 3));
}
void WINAPI CDataCollection::receiveServerCallBack03(PIPEMONITORACK* pbuf)
{
	if (pbuf->header.command == ePipeCommand_finish){
		PostThreadMessage(pbuf->header.station, WM_APP + ePipeCommand_finish, 0, 0);
		return;
	}
	// USC20
	CGraphMutex::Lock((eMutex)(eMessage + 4));
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(3);
	data.push_back(*pbuf);
	CGraphMutex::Unlock((eMutex)(eMessage + 4));
}
void WINAPI CDataCollection::receiveServerCallBack04(PIPEMONITORACK* pbuf)
{
	if (pbuf->header.command == ePipeCommand_finish){
		PostThreadMessage(pbuf->header.station, WM_APP + ePipeCommand_finish, 0, 0);
		return;
	}
	// UDSC54
	CGraphMutex::Lock((eMutex)(eMessage + 2));
	deque<PIPEMONITORACK>& data = theApp.GetMessageTemp(1);
	data.push_back(*pbuf);
	CGraphMutex::Unlock((eMutex)(eMessage + 2));
}

/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# AP�w����̃��b�Z�[�W��M

@param

@retval
*/
/*============================================================================*/
void WINAPI CDataCollection::receiveServerCallBack(PIPEMONITORACK* pbuf)
{
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("Start"), _T(""), nLogEx::debug);

	if (pbuf->header.command == ePipeCommand_finish){
		PostThreadMessage(pbuf->header.station, WM_APP + ePipeCommand_finish, 0, 0);
		return;
	}

	CString strDebug;

	if (CDataCollection::GetEngval(0) == NULL){

		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("Create Class"), _T(""), nLogEx::debug);

		// �H�w�l�ϊ��p�̃N���X���쐬
		for (int i = 0; i < eStation_MAX; i++){
			CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, i);
			char szFilePath[_MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);
			strDebug.Format(_T("%s(%d) :: DB file path(%s)"), __FILEW__, __LINE__, (LPCTSTR)strFilePath);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);

			CDataCollection::SetEngval(i, new CEngValCnv(szFilePath));
			if (CDataCollection::GetEngval(i) == NULL){
				strDebug.Format(_T("%s(%d) :: Fail new CEngValCnv, station(%d)"), __FILEW__, __LINE__, i);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);
			} else{
				CDataCollection::GetEngval(i)->SetCheckCheckOutDataList(true);
				strDebug.Format(_T("%s(%d) :: Success new CEngValCnv, station(%d)"), __FILEW__, __LINE__, i);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);
			}
		}

		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("End(Create Class)"), _T(""), nLogEx::debug);

		// �H�w�l�ϊ��N���X�̍\�z���s�����̂ŁA���̎�M�͖�������
		return;
	}

#if 1

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("Pipe Command Begin"), _T(""), nLogEx::debug);

	int err;
	size_t	size;
	BOOL	bAddTrend = FALSE;

	deque<CSpaceData>& tempData = theApp.GetDataCollection().GetSpaceTemp();

	switch (pbuf->header.command)
	{
	case ePipeCommand_monitorget:
	{
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("ePipeCommand_monitorget"), _T("Begin"), nLogEx::debug);

		DWORD	dwStart = timeGetTime();
		UINT nSelectStationMode = theApp.GetSelectStationMode();
		UINT nSelectStation = theApp.GetSelectStation();
		// �Ď����f�[�^
		// buf��App��deque�ɓo�^
		// 1�ǃ��[�h�̏ꍇ�͎w��ǁA4�ǃ��[�h�̏ꍇ�͋ǔԍ����͈͓��̏ꍇ
		if ((nSelectStationMode == eStation_1 &&
			((nSelectStation == 0 && pbuf->header.station == 1) ||
			(nSelectStation == 1 && pbuf->header.station == 4) ||
			(nSelectStation == 2 && pbuf->header.station == 2) ||
			(nSelectStation == 3 && pbuf->header.station == 3))) ||
			(nSelectStationMode == eStation_5 && pbuf->header.station >= eStation_Usuda64 + 1 && pbuf->header.station <= eStation_MAX))
		{
			UINT nStation = 0;
			if (pbuf->header.station == 1) nStation = 0;
			if (pbuf->header.station == 2) nStation = 2;
			if (pbuf->header.station == 3) nStation = 3;
			if (pbuf->header.station == 4) nStation = 1;
			if (CDataCollection::GetEngval(nStation) != NULL){
				err = CDataCollection::GetEngval(nStation)->CalcFromMsg((CEngValCnv::sdtp_msg_ex_t*)&(pbuf->buf[0]));
				if (err == 0){
					vector<CEngValCnv::obs_tbl_res_t>& data = CDataCollection::GetEngval(nStation)->GetAllVal();
					size = data.size();
					if (size > 0)
					{
						if (CDataCollection::GetEngval(nStation)->mOutDataList.size() != 0)
						{
							// �q���f�[�^�L���[�ɓo�^
							theApp.GetDataCollection().SetEqMonData(nStation, data);
							theApp.GetDataCollection().ConvertTrendData(nStation, data);
							bAddTrend = TRUE;
						}
						else
						{
							// ����Ă���Ď�����\��(�f�o�b�O�p)
							if( theApp.GetLogLevel() == nLogEx::debug )
							{
								if( GetPrivateProfileInt(SECTION_SYSTEM, KEY_DATANAMEOUT, 0, theApp.GetAppInifileName()) != 0 )
								{
									for( UINT i = 0; i < data.size(); i++ )
									{
										strDebug.Format(_T("%s.%s"), mStationSimpleString[nStation], (LPCTSTR)CString(data[i].obs_adr.sz_obsname));
										CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("data obsname"), strDebug, nLogEx::debug);
									}
								}
							}
						}
					} else{
						strDebug.Format(_T("%s(%d) :: CalcFromMsg no size(%d)"), __FILEW__, __LINE__, (int)size);
						CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);
					}
				} else{
					strDebug.Format(_T("%s(%d) :: CalcFromMsg err(%d)"), __FILEW__, __LINE__, err);
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);
				}
			}
			else
			{
				strDebug.Format(_T("%s(%d) :: mpTrendEng[%d] null"), __FILEW__, __LINE__, nStation);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("debug"), strDebug, nLogEx::debug);
			}
		}
//		DWORD dwElapsed = (timeGetTime() - dwStart);
//		TRACE("### ePipeCommand_monitorget Elapsed(%d)\n", dwElapsed);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("ePipeCommand_monitorget"), _T("End"), nLogEx::debug);
	}
	break;
	default:
		break;
	}
	//::PostMessage(mPipeMessageWnd->m_hWnd, ID_PIPE_SERVER, 9, (LPARAM)pbuf);

#endif // 0

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("receiveServerCallBack"), _T("End"), _T(""), nLogEx::debug);
}

/*============================================================================*/
/*! �f�[�^���W

-# �Ď��f�[�^�i�[

@param	nStation	�F�ǔԍ�
@param	data		�F�H�w�l�ϊ��f�[�^
@retval

*/
/*============================================================================*/
void CDataCollection::SetEqMonData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data)
{
	int cnt = (int)data.size();
	for (int i = 0; i < cnt; ++i)
	{
		CEngValCnv::obs_tbl_res_t mon_data = data[i];
		CString obsname = CString(mon_data.obs_adr.sz_obsname);
		m_mondatalist[nStation][obsname] = mon_data.obs_adr;
	}
}
/*============================================================================*/
/*! CDBAccess

-# �Ď��f�[�^�擾

@param	nStation	�F�ǔԍ�
@param	strWatch	�F�Ď�����
@retval strct obsname_t �|�C���^

*/
/*============================================================================*/
char* CDataCollection::GetObsname_t(UINT nStation, CString strWatch)
{
	char *obs = NULL;

	if (m_mondatalist.count(nStation) != 0)
	{
		map<CString, CEngValCnv::obsname_t> &mon = m_mondatalist[nStation];

		if (mon.count(strWatch) != 0)
		{
			obs = (char*)&mon[strWatch];
		}
	}

	return obs;
}

/*============================================================================*/
/*! �f�[�^���W

-# �p�C�v�ُ�ʒm�X���b�h

@param

@retval void
*/
/*============================================================================*/
DWORD threadAbnormalNotifyProc(LPVOID lParam)
{
	CDataCollection* ptr = (CDataCollection*)lParam;

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)){
		if (msg.message == WM_APP + ePipeCommand_finish){
			// �p�C�v�̍č쐬
			ptr->deletePipe();
			ptr->createPipe();
		}
		DispatchMessage(&msg);
	}
	return 0;
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���W�p�C�v�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::createPipe()
{
	TRACE("CDataCollection::createPipe Start\n");
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("createPipe"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	if (CDataCollection::GetEngval(0) == NULL){

		// �H�w�l�ϊ��p�̃N���X���쐬
		for (int i = 0; i < eStation_MAX; i++){
			CString strFilePath = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, i);
			char szFilePath[_MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, strFilePath, -1, szFilePath, sizeof(szFilePath), NULL, NULL);
			CDataCollection::SetEngval(i, new CEngValCnv(szFilePath));
			if (CDataCollection::GetEngval(i) == NULL){
			}
			else{
				CDataCollection::GetEngval(i)->SetCheckCheckOutDataList(true);
			}
		}
	}

	// ���b�Z�[�W���W�p�X���b�h�̍쐬
	CreateMessageThread();

	// �p�C�v�ُ팟�m�X���b�h�̍쐬
	if (m_ThreadNotify == 0){
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadAbnormalNotifyProc, this, 0, &m_ThreadNotify);
	}

	for (int i = 0; i < eStation_MAX; i++){
		m_bCreatePipe[i] = false;
		mPipeSend[i].SetPipeName(mPipeNameString[i]);
		mPipeRecv[i].SetPipeName(mPipeNameString[i]);
		bool ret;
		switch (i){
		case	0: ret = mPipeSend[i].Create(false, &receiveServerCallBack01, i, m_ThreadNotify); break;
		case	1: ret = mPipeSend[i].Create(false, &receiveServerCallBack02, i, m_ThreadNotify); break;
		case	2: ret = mPipeSend[i].Create(false, &receiveServerCallBack03, i, m_ThreadNotify); break;
		case	3: ret = mPipeSend[i].Create(false, &receiveServerCallBack04, i, m_ThreadNotify); break;
		}
		if (ret == true){
			m_bCreatePipe[i] = true;
			SendPipe(eSTATIONSTART, i);
		}
		else
		{
			//=====================================================//
			//������������������������ Log ������������������������//
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("createPipe"), _T("Error"), _T(""), nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
		}
	}

	TRACE("CDataCollection::createPipe End\n");
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("createPipe"), _T("End"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# ���b�Z�[�W���W�X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateMessageThread()
{
	if (m_pMessageThread == NULL){
		m_pMessageThread = (CMessageThread*)AfxBeginThread(RUNTIME_CLASS(CMessageThread), THREAD_PRIORITY_HIGHEST/*THREAD_PRIORITY_NORMAL*/, 0, CREATE_SUSPENDED);
		m_pMessageThread->SetStation(0);
		m_pMessageThread->ResumeThread();
	}
	for (int i = 0; i < eStation_MAX; i++){
		if (m_pStationMessageThread[i] == NULL){
			m_pStationMessageThread[i] = (CMessageThread*)AfxBeginThread(RUNTIME_CLASS(CMessageThread), THREAD_PRIORITY_HIGHEST/*THREAD_PRIORITY_NORMAL*/, 0, CREATE_SUSPENDED);
			m_pStationMessageThread[i]->SetStation(i + 1);
			m_pStationMessageThread[i]->ResumeThread();
		}
	}
}
/*============================================================================*/
/*! �A�v���P�[�V����

-# ���b�Z�[�W���W�X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::RemoveMessageThread()
{
	if (m_pMessageThread != NULL){
		m_pMessageThread->StopThread();
		WaitForSingleObject(m_pMessageThread->m_hThread, 10000);
		delete m_pMessageThread;
		m_pMessageThread = NULL;
	}
	for (int i = 0; i < eStation_MAX; i++){
		if (m_pStationMessageThread[i] != NULL){
			m_pStationMessageThread[i]->StopThread();
			WaitForSingleObject(m_pStationMessageThread[i]->m_hThread, 10000);
			delete m_pStationMessageThread[i];
			m_pStationMessageThread[i] = NULL;
		}
	}
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# �T�[�r�X���W���[���ւ̃p�C�v�ʐM

@param

@retval
*/
/*============================================================================*/
void CDataCollection::SendPipe(DWORD command, int nStation)
{
	switch (command)
	{
	case	eSTATIONSTART:
	{
		return;
		PIPESTART buf;
		memset(&buf, 0, sizeof(PIPESTART));
		buf.header.size = sizeof(PIPESTART) - sizeof(PIPEHEADER);
		buf.header.command = ePipeCommand_start;
		buf.header.station = 1;//�Œ�
		DWORD ip = theApp.GetHostIpInfo();
		buf.val[0] = ((BYTE)ip & 0xff);
		buf.val[1] = ((BYTE)(ip >> 8) & 0xff);
		buf.val[2] = ((BYTE)(ip >> 16) & 0xff);
		buf.val[3] = ((BYTE)(ip >> 24) & 0xff);
		mPipeSend[nStation].Send(&buf);
		break;
	}
	case	eSTATIONEND:
	{
		PIPESTOP buf;
		memset(&buf, 0, sizeof(PIPESTOP));
		buf.header.size = sizeof(PIPESTOP) - sizeof(PIPEHEADER);
		buf.header.command = ePipeCommand_stop;
		buf.header.station = 1;//�Œ�
		DWORD ip = theApp.GetHostIpInfo();
		buf.val[0] = ((BYTE)ip & 0xff);
		buf.val[1] = ((BYTE)(ip >> 8) & 0xff);
		buf.val[2] = ((BYTE)(ip >> 16) & 0xff);
		buf.val[3] = ((BYTE)(ip >> 24) & 0xff);
		mPipeSend[nStation].Send(&buf);
		break;
	}
	default:
		return;
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# ���A���^�C���f�[�^�ۑ��X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::createSaveRealTimeDataThread()
{
	stThreadParam param;

	if (mpSaveRealTimeDataThread == NULL){
		mpSaveRealTimeDataThread = (CSaveRealTimeDataThread*)AfxBeginThread(RUNTIME_CLASS(CSaveRealTimeDataThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		param.parent = this;
		param.callback = messageProc;
		mpSaveRealTimeDataThread->SetThreadParam(param);
		mpSaveRealTimeDataThread->ResumeThread();
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# �m�����A���^�C���f�[�^�ۑ��X���b�h�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::createSaveAnalysisDataThread(int nView)
{
	stThreadParam param;

	if (mpSaveAnalysisDataThread[nView] == NULL){
		mpSaveAnalysisDataThread[nView] = (CSaveAnalysisDataThread*)AfxBeginThread(RUNTIME_CLASS(CSaveAnalysisDataThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		param.parent = this;
		param.callback = messageProc;
		mpSaveAnalysisDataThread[nView]->SetThreadParam(param);
		mpSaveAnalysisDataThread[nView]->SetViewNo(nView);
		mpSaveAnalysisDataThread[nView]->ClrFinish();
		mpSaveAnalysisDataThread[nView]->ResumeThread();
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���W�̏I��

@param
@retval

*/
/*============================================================================*/
void CDataCollection::StopCollection()
{
	removeCollectionThread();
	removeSaveRealTimeDataThread();
}
/*============================================================================*/
/*! �v���Z�X�ԒʐM

-# �v���Z�X�ԒʐM���s�����߂̃p�C�v�폜

@param

@retval
*/
/*============================================================================*/
void CDataCollection::deletePipe()
{
	TRACE("CDataCollection::deletePipe Start\n");
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("deletePipe"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//

	// ���b�Z�[�W���W�p�X���b�h�̏I��
	RemoveMessageThread();

	for (int i = 0; i < eStation_MAX; i++){
		if (m_bCreatePipe[i] == true){
			SendPipe(eSTATIONEND, i);
			mPipeSend[i].Delete();
			mPipeRecv[i].Delete();
			if (CDataCollection::GetEngval(i) != NULL){
				TRACE("CDataCollection::deletePipe mpTrendEng Delete Start Index\n");
				CDataCollection::DeleteEngval(i);
			}
			m_bCreatePipe[i] = false;
		}
	}
	TRACE("CDataCollection::deletePipe End\n");
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("deletePipe"), _T("End"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
}

/*============================================================================*/
/*! �f�[�^���W

-# �m�����A���^�C���f�[�^�ۑ��X���b�h�̏I��

@param
@retval

*/
/*============================================================================*/
void CDataCollection::StopAllAnalysisDataThread()
{
	for( int i = 0; i < MAX_VIEW; i++ )
		removeSaveAnalysisDataThread(i);
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���W�X���b�h�̍폜

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::removeCollectionThread()
{
	if (mpCollectThread != NULL){
		mpCollectThread->StopCollection();
		WaitForSingleObject(mpCollectThread->m_hThread, 60000);
		delete mpCollectThread;
		mpCollectThread = NULL;
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# ���A���^�C���f�[�^�ۑ��X���b�h�̍폜

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::removeSaveRealTimeDataThread()
{
	if (mpSaveRealTimeDataThread != NULL){
		mpSaveRealTimeDataThread->Stop();
		WaitForSingleObject(mpSaveRealTimeDataThread->m_hThread, 60000);
		delete mpSaveRealTimeDataThread;
		mpSaveRealTimeDataThread = NULL;
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# �m�����A���^�C���f�[�^�ۑ��X���b�h�̍폜

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::removeSaveAnalysisDataThread(int nView)
{
	if (mpSaveAnalysisDataThread[nView] != NULL){
		mpSaveAnalysisDataThread[nView]->Stop();
		while( !mpSaveAnalysisDataThread[nView]->IsFinish() )
			Sleep(100);
		WaitForSingleObject(mpSaveAnalysisDataThread[nView]->m_hThread, 60000);
		delete mpSaveAnalysisDataThread[nView];
		mpSaveAnalysisDataThread[nView] = NULL;
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���W�X���b�h�̍쐬

@param		message	���b�Z�[�W
@param		wParam	���b�Z�[�W�̕t�����
@param		lParam	���b�Z�[�W�̕t�����

@retval void
*/
/*============================================================================*/
void CALLBACK CDataCollection::messageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
		break;
	case	eMessage_ThreadError:
		break;
	case	eMessage_ThreadFail:
		break;
	case	eMessage_ThreadException:
		break;
	default:
		return;
		break;
	}
	(theApp.m_pMainWnd)->PostMessage(message, wParam, lParam);
}

/*============================================================================*/
/*! �f�[�^���W

-# �T�[�o�f�[�^���A�v�����f�[�^�ɍč쐬

@param	lpData		�T�[�o�f�[�^
@param	dwSize		�o�b�t�@�T�C�Y

@retval void
*/
/*============================================================================*/
bool CDataCollection::ConvertTrendData(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& convdata)
{
	bool res = false;

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Start"), _T(""), nLogEx::debug);

	CGraphMutex::Lock(eRealTimeData);

	try
	{
		CSpaceData			data;
		stSpaceData			sData;

		deque<CSpaceData>	temp;
		CTime				t1(0), t2(0);

		CString strStation = mStationSimpleString[nStation];
		CString str;
		double fval;

		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Create Space Data"), _T("Begin"), nLogEx::debug);

		// �H�w�l�ϊ��f�[�^���̎擾
		int count = (int)convdata.size();
		// �H�w�l�ϊ��f�[�^�����ȉ����J��Ԃ�
		for (int i = 0; i < count; ++i){
			CEngValCnv::obs_tbl_res_t mondata = convdata[i];

			if( mondata.ld_time_val == 0.0 )
			{
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Create Space Data"), _T("Data Time Zero"), nLogEx::info);

				continue;
			}

			// �ϊ���f�[�^�̃N���A
			data.value.clear();

			// �f�[�^�̎��W
			str = strStation + _T(".");
			str += CString(mondata.obs_adr.sz_obsname);
			data.dataid = GetSelectDataId(str);
			if (mondata.obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr)){
				// ������
				fval = (double)mondata.obs_adr.l_data;
			}
			else if (mondata.obs_adr.l_cnvResType & (1 << eCnvResType_Long)){
				// long�l
				fval = (double)mondata.obs_adr.l_data;
			}
			else if (mondata.obs_adr.l_cnvResType & (1 << eCnvResType_Double)){
				// double�l
				fval = (double)mondata.obs_adr.d_data;
			}
			else if (mondata.obs_adr.l_cnvResType & (1 << eCnvResType_LongDouble)){
				// long double�l
				fval = (double)mondata.obs_adr.ld_data;
			}
			else{
				// long�l
				fval = (double)mondata.obs_adr.l_data;
			}
			data.kind = 0;
			data.totalSize = 1;

			memset(&sData, 0, sizeof(stSpaceData));
			ConvertTime(mondata.ld_time_val, &sData.systime);
#ifdef	DemoTimeStamp
			// �f���p�C�v�ʐM�p�Ɍ��ݎ������X�^���v
			GetSystemTime(&sData.systime);
#endif	// DemoTimeStamp
#ifdef  DemoStepData
			time_t demoTime = CTime(sData.systime).GetTime();
			static time_t demoTimeStart = demoTime;
			time_t nVal = (demoTime - demoTimeStart) / 10;
			double dVal;
			{
				if (nVal <= 5)
				{
					dVal = (double)nVal;
				}
				else if (nVal <= 15)
				{
					dVal = (double)(10 - nVal);
				}
				else
				{
					dVal = (double)(nVal - 20);
				}
			
				if (nVal >= 20)
				{
					demoTimeStart = demoTime;
					dVal = 0;
				}
			}
//			{
//				if (nVal <= 5)
//				{
//					dVal = (double)nVal;
//				}
//				else
//				{
//					dVal = (double)(10 - nVal);
//				}
//
//				if (nVal >= 10)
//				{
//					demoTimeStart = demoTime;
//					dVal = 0;
//				}
//			}

			fval = 10.0 * dVal;

			// �������f�[�^�e�X�g�p >>>
			//while (fval > 10000.0 || fval < -10000.0)
			//{
			//	fval /= 10.0;
			//}
			// �������f�[�^�e�X�g�p <<<

			// �傫���f�[�^�e�X�g�p >>>
			//while (fval != 0.0 && fval < 1000000000000.0 && fval > -1000000000000.0)
			//{
			//	fval *= 10.0;
			//}
			// �������f�[�^�e�X�g�p <<<
			TRACE(_T("ConvertTrendData: %s = %f\n"), (LPCTSTR)CString(mondata.obs_adr.sz_obsname), fval);
#endif	// DemoStepData
#ifdef  DemoSaigen
			if (i % 6 == 0)
			{
				//				fval = 42.0;
				fval = 50.0;
				
			}
			else if (i % 5 == 0)
			{
				//				fval = 5.0;
				fval = 40.0;
//				fval = 41.8;
			}
			else if (i % 4 == 0)
			{
				fval = 30.0;
//				fval = 30.6;
			}
			else if (i % 3 == 0)
			{
				fval = 20.0;
//				fval = 19.4;
			}
			else if (i % 2 == 0)
			{
				fval = 10.0;
//				fval = 8.2;
			}
			else
			{
				fval = 0.0;
			}

			//			fval = 20.0;
#endif	// DemoSaigen
#ifdef DemoSin
			time_t demoTime = CTime(sData.systime).GetTime();
			static time_t demoTimeStart = demoTime;
			time_t nVal = (demoTime - demoTimeStart);
			double dVal = (double)nVal / 10.0;
			fval = 25 * sin(dVal * 3.1415926538 / 180.0) + 25;

#endif
			sData.status = SDS_VALID | ((mondata.obs_adr.l_cnvResType & (1 << eCnvResType_TimeLeapSec)) ? SDS_LEAP : 0x0);
			sData.mmPair.max = (double)fval;
			sData.mmPair.min = (double)fval;
			sData.mmPair.cnt = 1;
			data.value.push_back(sData);
			temp.push_back(data);

			// �ł��ߋ��̎��ԂƖ����̎��Ԃ��擾
			if ((t1 == 0) && (t2 == 0))
			{
				t1 = sData.systime;
				t2 = sData.systime;
			}
			else
			{
				if (t1 > sData.systime)
					t1 = sData.systime;
				if (t2 < sData.systime)
					t2 = sData.systime;
			}
		}

		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Create Space Data"), _T("End"), nLogEx::debug);

		// �L���f�[�^�����݂���Ȃ�Ď��f�[�^���X�g���쐬
		if ((t1 != 0) && (t2 != 0))
		{
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Set Space Data"), _T("Begin"), nLogEx::debug);

			// ���A���^�C�����[�h�̎��������X�V
			if (theApp.GetRealTimeStart() == CTime(0))
			{
				theApp.SetRealTimeStart(t1);

				for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
					theApp.SetRealTimeWriteData(i, t1);
				theApp.SetRealTimeWriteMax(t1);
			}

			if ((theApp.GetRealTimeLast() == CTime(0)) || (theApp.GetRealTimeLast() < t2))
				theApp.SetRealTimeLast(t2);

			// �I�����X�g�����o
			for (int i = 0; i < mSelectDataList.size(); i++)
			{
				bool	bExist = false;

				// �擾�f�[�^�����o
				deque<CSpaceData>::iterator	itr;
				for (itr = temp.begin(); itr != temp.end(); itr++)
				{
					// ���o�o�����烊�X�g�ɒǉ�
					if (mSelectDataList[i].id == itr->dataid)
					{
						CTime	tmpTime = sData.systime;
						CString	str;
						str.Format(_T("%s %s"), (LPCTSTR)(tmpTime.Format("%Y/%m/%d")), (LPCTSTR)(tmpTime.Format("%H:%M:%S")));
						CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Set Space Data Time"), str, nLogEx::info);

						// �J�n�������ߋ��̃f�[�^���폜
						while (!itr->value.empty())
						{
							if (theApp.GetRealTimeStart() > CTime(itr->value.front().systime))
							{
								CTime	tmpTime = itr->value.front().systime;
								CString	str;
								str.Format(_T("%s %s"), (LPCTSTR)(tmpTime.Format("%Y/%m/%d")), (LPCTSTR)(tmpTime.Format("%H:%M:%S")));
								CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Delete Old Data"), str, nLogEx::info);

								itr->value.pop_front();
							}
							else
								break;
						}


						// �L���f�[�^�����݂���Ȃ烊�X�g�ɒǉ�
						if (!itr->value.empty())
						{
							CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Set Space Data"), _T("Valid Data -> SetSpaceTemp"), nLogEx::debug);

							//							mSpaceTemp.push_back(*itr);
							if( SetSpaceTemp(*itr) )
							{
								CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("SetSpaceTemp"), _T("Next"), nLogEx::info);

								res = true;
							}
							else
								CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("SetSpaceTemp"), _T("Keep"), nLogEx::info);

							bExist = true;
						}

						temp.erase(itr);

						break;
					}
				}

				// �擾�f�[�^�����݂����A�擾�Ώۃf�[�^�ŊJ�n�����łȂ��Ȃ烊�X�g�ɒǉ����Ȃ�
				if ((!bExist) && mSelectDataList[i].valid && (theApp.GetRealTimeStart() != t1))
					bExist = true;

				// �擾�f�[�^�����݂��Ȃ��̂Ŗ����f�[�^�����X�g�ɒǉ�
				if (!bExist)
				{
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Set Space Data"), _T("Invalid Data -> SetSpaceTemp"), nLogEx::debug);

					data.value.clear();

					data.dataid = mSelectDataList[i].id;
					data.kind = 0;
					data.totalSize = 1;

					memset(&sData, 0, sizeof(stSpaceData));
					t1.GetAsSystemTime(sData.systime);
					sData.status = SDS_INVALID;
					sData.mmPair.max = 0.0;
					sData.mmPair.min = 0.0;
					sData.mmPair.cnt = 1;

					data.value.push_back(sData);

					//					mSpaceTemp.push_back(data);
					if (SetSpaceTemp(data))
					{
						CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("SetSpaceTemp"), _T("Next"), nLogEx::info);

						res = true;
					}
					else
						CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("SetSpaceTemp"), _T("Keep"), nLogEx::info);
				}
			}

			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("Set Space Data"), _T("End"), nLogEx::debug);
		}
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eRealTimeData);

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvertTrendData"), _T("End"), _T(""), nLogEx::debug);

	return res;
}

/*============================================================================*/
/*! �f�[�^���W

-# �T�[�o�f�[�^���A�v�����f�[�^�ɍč쐬

@param	lpData		�T�[�o�f�[�^
@param	dwSize		�o�b�t�@�T�C�Y

@retval void
*/
/*============================================================================*/
bool CDataCollection::CreateSpaceData(LPBYTE lpData, DWORD dwSize)
#if 0
{
	CGraphMutex::Lock(eRealData);

	try
	{
		stServerData *pdata = (stServerData*)lpData;
		CSpaceData temp;
		temp.value.clear();
		for (DWORD i = 0; i < dwSize; i += sizeof(stServerData), pdata++){
			stSpaceData data;
			memset(&data, 0, sizeof(stSpaceData));
			data.systime = pdata->systime;
			data.kind = pdata->kind;
			data.value = pdata->value;
			data.status = pdata->status;
			data.dataid = ConvertNameToId(pdata->name);
			temp.value.push_back(data);
		}
		// �f�[�^���ꎞ�I�Ɋi�[
		mSpaceTemp.push_back(temp);
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eRealData);
}
#else
{
	bool	res = false;

	CGraphMutex::Lock(eRealTimeData);

	try
	{
		stServerItemData	*pdata;
		CSpaceData		data;
		stSpaceData		sData;

		pdata = (stServerItemData *)(lpData + sizeof(stCommandHeader));
		dwSize -= sizeof(stCommandHeader);

		deque<CSpaceData>	temp;

		CTime	t1(0), t2(0);

		// �T�[�o�[�f�[�^����A�v���f�[�^�̍\���ɕϊ�
		for (DWORD i = 0; i + 1 < dwSize; i += sizeof(stServerItemData), pdata++)
		{
			data.value.clear();

			data.dataid = GetSelectDataId(CString(pdata->name));
			data.kind = pdata->kind;
			data.totalSize = 1;

			memset(&sData, 0, sizeof(stSpaceData));
			ConvertTime(&pdata->systime, &sData.systime);
			sData.status = pdata->status;	// ���邤�b�t���O�͊��ɃZ�b�g����Ă�����̂Ƃ��ď���
			sData.mmPair.max = (double)pdata->value;
			sData.mmPair.min = (double)pdata->value;
			sData.mmPair.cnt = 1;

			data.value.push_back(sData);

			temp.push_back(data);

			// �ł��ߋ��̎��ԂƖ����̎��Ԃ��擾
			if( (t1 == 0) && (t2 == 0) )
			{
				t1 = sData.systime;
				t2 = sData.systime;
			}
			else
			{
				if( t1 > sData.systime )
					t1 = sData.systime;
				if( t2 < sData.systime )
					t2 = sData.systime;
			}
		}

		// �L���f�[�^�����݂���Ȃ�Ď��f�[�^���X�g���쐬
		if( (t1 != 0) && (t2 != 0) )
		{
			// ���A���^�C�����[�h�̎��������X�V
			if( theApp.GetRealTimeStart() == CTime(0) )
			{
				theApp.SetRealTimeStart(t1);

				for( int i = 0; i < (MAX_VIEW * MAX_TDATA); i++ )
					theApp.SetRealTimeWriteData(i, t1);
				theApp.SetRealTimeWriteMax(t1);
			}

			if( (theApp.GetRealTimeLast() == CTime(0)) || (theApp.GetRealTimeLast() < t2) )
				theApp.SetRealTimeLast(t2);

			// �I�����X�g�����o
			for( int i = 0; i < mSelectDataList.size(); i++ )
			{
				bool	bExist = false;

				// �擾�f�[�^�����o
				deque<CSpaceData>::iterator	itr;
				for( itr = temp.begin(); itr != temp.end(); itr++ )
				{
					// ���o�o�����烊�X�g�ɒǉ�
					if( mSelectDataList[i].id == itr->dataid )
					{
						// �J�n�������ߋ��̃f�[�^���폜
						while( !itr->value.empty() )
						{
							if( theApp.GetRealTimeStart() > CTime(itr->value.front().systime) )
								itr->value.pop_front();
							else
								break;
						}

#if 0	// �����I�ɂT�b��Ƀf�[�^�����𔭐������鏈��
						if( i == 0 )
						{
							if( (theApp.GetRealTimeStart() + 5) == CTime(itr->value.front().systime) )
								itr->value.pop_front();
							else if( (theApp.GetRealTimeStart() + 5) < CTime(itr->value.front().systime) )
							{
								CTimeSpan s(0, 0, 0, 1);
								CTime t = CTime(itr->value.front().systime) - s;
								t.GetAsSystemTime(itr->value.front().systime);
							}
						}
#endif

						// �L���f�[�^�����݂���Ȃ烊�X�g�ɒǉ�
						if( !itr->value.empty() )
						{
//							mSpaceTemp.push_back(*itr);
							if( SetSpaceTemp(*itr) )
								res = true;

							bExist = true;
						}

						temp.erase(itr);

						break;
					}
				}

				// �擾�f�[�^�����݂����A�擾�Ώۃf�[�^�ŊJ�n�����łȂ��Ȃ烊�X�g�ɒǉ����Ȃ�
				if( (!bExist) && mSelectDataList[i].valid && (theApp.GetRealTimeStart() != t1) )
					bExist = true;

				// �擾�f�[�^�����݂��Ȃ��̂Ŗ����f�[�^�����X�g�ɒǉ�
				if( !bExist )
				{
					data.value.clear();

					data.dataid = mSelectDataList[i].id;
					data.kind = 0;
					data.totalSize = 1;

					memset(&sData, 0, sizeof(stSpaceData));
					t1.GetAsSystemTime(sData.systime);
					sData.status = SDS_INVALID;
					sData.mmPair.max = 0.0;
					sData.mmPair.min = 0.0;
					sData.mmPair.cnt = 1;

					data.value.push_back(sData);

//					mSpaceTemp.push_back(data);
					if( SetSpaceTemp(data) )
						res = true;
				}
			}
		}
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eRealTimeData);

//	TRACE("mSpaceTemp size = %d\n", mSpaceTemp.size());

	return res;
}
#endif

/*============================================================================*/
/*! �f�[�^���W

-# mSpaceTemp���󂩊m�F

@param

@retval void
*/
/*============================================================================*/
bool CDataCollection::IsSpaceTempEmpty()
{
	bool	res = false;

	CGraphMutex::Lock(eRealTimeData);

	res = mSpaceTemp.empty();

	CGraphMutex::Unlock(eRealTimeData);

	return res;
}

/*============================================================================*/
/*! �f�[�^���W

-# mSpaceTemp����ɂȂ�܂ō폜

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::DeleteSpaceTemp()
{
	CGraphMutex::Lock(eRealTimeData);

	while( !mSpaceTemp.empty() )
	{
		mSpaceTemp.front().value.clear();
		mSpaceTemp.pop_front();
	}

	CGraphMutex::Unlock(eRealTimeData);
}

/*============================================================================*/
/*! �f�[�^���W

-# �T�����M�����f�[�^��o�^����

@param

@retval void
*/
/*============================================================================*/
bool CDataCollection::AddCollection(UINT message, WPARAM wParam, LPARAM lParam)
{
#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo

	try
	{
		// �󂫗e�ʃ`�F�b�N���L��
		if (theApp.IsCheckDiskFreeSpace())
		{
			// �󂫗e�ʂ��m�F
			if (!theApp.IsDiskFreeSpace())
			{
				//=====================================================//
				//������������������������ Log ������������������������//
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Disk free space none"), _T(""), nLogEx::error);
				//������������������������ Log ������������������������//
				//=====================================================//

				// �X���b�h�͋N�������H
				if (mpCollectThread != NULL)
				{
					// �擾�f�[�^���폜
					CGraphMutex::Lock(eRealTimeData);
					while (!mSpaceTemp.empty())
						mSpaceTemp.erase(mSpaceTemp.begin());
					CGraphMutex::Unlock(eRealTimeData);

					// ��̓f�[�^���폜
					for (int i = 0; i < MAX_VIEW; i++)
						DeleteAnalysisData(i);

					// ������~
					((CMainFrame *)theApp.m_pMainWnd)->OnButtonOnlineStop();

					AfxMessageBox(_T("�f�B�X�N�̋󂫗e�ʂ����Ȃ��̂ő�����~���܂����B\n�ēx���肵�����ꍇ�͏\���ȋ󂫗e�ʂ��m�ۂ��Ă��������B"));
				}

				return false;
			}
		}

#ifndef HighSpeedDemo
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("mSpaceTemp size = %I64u"), mSpaceTemp.size());
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Processing"), msg, nLogEx::debug);
		//������������������������ Log ������������������������//
		//=====================================================//
#endif // HighSpeedDemo

		// �ő�擾�f�[�^���ɒB������A�ł��Â��t�@�C��������͑Ώۂ���폜
#if 0
#ifdef	_TEST_MAXDATA
		if( theApp.GetRealTimeWriteMax() > (theApp.GetRealTimeStart() + CTimeSpan(0, 3, 0, 0) - CTimeSpan(0, 0, 0, 1)) )
#else	// 	_TEST_MAXDATA
		if( theApp.GetRealTimeWriteMax() > (theApp.GetRealTimeStart() + CTimeSpan(MAX_USEDATA_DAYS, 0, 0, 0) - CTimeSpan(0, 0, 0, 1)) )
#endif	// 	_TEST_MAXDATA
		{
			theApp.SetRealTimeStart(theApp.GetRealTimeStart() + MAX_SIZE_PER_MMDATA);
			theApp.SetStartFile(theApp.GetStartFile() + 1);
		}
#endif

		CGraphMutex::Lock(eRealTimeData);

		if( !mSpaceTemp.empty() )
		{
			// �`�����������
			for (int i = 0; i < (MAX_VIEW * MAX_TDATA); i++)
			{
				for (int j = 0; j < NUM_REAL_SCALE_INFO_TABLE; j++)
				{
					m_TrendDraw[i][j].update_pos = 0;
					m_TrendDraw[i][j].change_flg = false;
				}
			}

#ifndef HighSpeedDemo
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Set SpaceTemp"), _T("Begin"), nLogEx::debug);
#endif // HighSpeedDemo

			// �ꎞ�I�Ɋi�[���ꂽ�f�[�^���P�v�̈�Ɋi�[����
			while (!mSpaceTemp.empty())
			{
				CSpaceData newData = mSpaceTemp.front();

				BOOL	existId = FALSE;
				for (int i = 0; i < mSpaceData.size(); ++i)
				{
					// �����Ď��f�[�^ID������
					if (mSpaceData[i].dataid == newData.dataid)
					{
						// �����Ď��f�[�^ID����������A�Y��ID�����L���[�ɒǉ�
						while (!newData.value.empty())
						{
							stSpaceData spaceData = newData.value.front();
							newData.value.pop_front();

							int size = (int)mSpaceData[i].value.size();
							if (size > 0)
							{
								deque<stSpaceData>::iterator itr = mSpaceData[i].value.end();
								itr--;
								CTime lastTime = (*itr).systime;
								CTime curTime = spaceData.systime;

								// ���ɏ������������̃f�[�^�͔j��
								if( curTime <= lastTime )
								{
									existId = TRUE;
									continue;
								}

								// �f�[�^�������Ή�
								while (curTime > lastTime + 1)
								{
									stSpaceData		sEmptyData;
									memset(&sEmptyData, 0, sizeof(stSpaceData));
									lastTime += 1;
									lastTime.GetAsSystemTime(sEmptyData.systime);
									sEmptyData.status = SDS_INVALID;
									sEmptyData.mmPair.max = 0.0;
									sEmptyData.mmPair.min = 0.0;
									sEmptyData.mmPair.cnt = 1;

									// �񈳏k�X�P�[���̕`�����ύX
									for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
									{
										if( tblRealScaleInfo[t].cmp_rate <= 1 )
											ChangeDrawInfoEx(mSpaceData[i].dataid, t);
									}

									// �Ď��f�[�^�̃L���[�T�C�Y����
									while (mSpaceData[i].value.size() >= tblRealScaleInfo[0].hld_data)
									{
										mSpaceData[i].value.pop_front();
									}

									// �f�[�^�g�[�^���T�C�Y���C���N�������g
									mSpaceData[i].totalSize++;

									// ���~�b�g���m�F
									CheckLimitState(mSpaceData[i].dataid, mSpaceData[i].value.back(), sEmptyData);

#ifndef HighSpeedDemo
									CString	str;
									str.Format(_T("%s %s"), (LPCTSTR)(lastTime.Format("%Y/%m/%d")), (LPCTSTR)(lastTime.Format("%H:%M:%S")));
									CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Push Invalid Data"), str, nLogEx::debug);
#endif // HighSpeedDemo

									// �Ď��f�[�^�̃L���[�ɒǉ�
									mSpaceData[i].value.push_back(sEmptyData);

									// �R�s�[�p�f�[�^���R�s�[
									if( mSpaceData[i].value_for_copy.size() < tblRealScaleInfo[0].hld_data )
										mSpaceData[i].value_for_copy.push_back(sEmptyData);

									// �񈳏k�X�P�[���̕`������X�V
									for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
									{
										if( tblRealScaleInfo[t].cmp_rate <= 1 )
											UpdateDrawInfoEx(mSpaceData[i].dataid, t);
									}

									// ���k�f�[�^���쐬
									CompSpaceData(mSpaceData[i].dataid, (int)mSpaceData[i].value.size() - 1);
								}
							}

							// �񈳏k�X�P�[���̕`�����ύX
							for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
							{
								if( tblRealScaleInfo[t].cmp_rate <= 1 )
									ChangeDrawInfoEx(mSpaceData[i].dataid, t);
							}

							// �Ď��f�[�^�̃L���[�T�C�Y����
							while (mSpaceData[i].value.size() >= tblRealScaleInfo[0].hld_data)
							{
								mSpaceData[i].value.pop_front();
							}

							// �ő�l�^�ŏ��l���X�V
							if( m_bAutoClear || (mSpaceData[i].data_max < spaceData.mmPair.max) )
								mSpaceData[i].data_max = spaceData.mmPair.max;
							if( m_bAutoClear || (mSpaceData[i].data_min > spaceData.mmPair.min) )
								mSpaceData[i].data_min = spaceData.mmPair.min;

							// AUTO�N���A���s�t���O���N���A
							m_bAutoClear = false;

							// �f�[�^�g�[�^���T�C�Y���C���N�������g
							mSpaceData[i].totalSize++;

							// ���~�b�g���m�F
							CheckLimitState(mSpaceData[i].dataid, mSpaceData[i].value.back(), spaceData);

#ifndef HighSpeedDemo
							CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Push Valid Data"), _T(""), nLogEx::debug);
#endif // HighSpeedDemo

							// �Ď��f�[�^�̃L���[�ɒǉ�
							mSpaceData[i].value.push_back(spaceData);

							// �R�s�[�p�f�[�^���R�s�[
							if( mSpaceData[i].value_for_copy.size() < tblRealScaleInfo[0].hld_data )
								mSpaceData[i].value_for_copy.push_back(spaceData);

							// �񈳏k�X�P�[���̕`������X�V
							for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
							{
								if( tblRealScaleInfo[t].cmp_rate <= 1 )
									UpdateDrawInfoEx(mSpaceData[i].dataid, t);
							}

							// ���k�f�[�^���쐬
							CompSpaceData(mSpaceData[i].dataid, (int)mSpaceData[i].value.size() - 1);

							existId = TRUE;
	//						break;
						}
					}
				}

				// �����Ď��f�[�^ID�������ꍇ
				if (existId == FALSE)
				{
#ifndef HighSpeedDemo
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Push Valid Data(First)"), _T(""), nLogEx::debug);
#endif // HighSpeedDemo

					mSpaceData.push_back(newData);

					// �ő�l�^�ŏ��l��������
					for (UINT i = 0; i < newData.value.size(); i++)
					{
						// �񈳏k�X�P�[���̕`������X�V
						for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
						{
							if( tblRealScaleInfo[t].cmp_rate <= 1 )
								UpdateDrawInfoEx(newData.dataid, t);
						}

						if (i == 0)
						{
							newData.data_max = newData.value[i].mmPair.max;
							newData.data_min = newData.value[i].mmPair.min;
							continue;
						}

						if( newData.data_max < newData.value[i].mmPair.max )
							newData.data_max = newData.value[i].mmPair.max;
						if( newData.data_min > newData.value[i].mmPair.min )
							newData.data_min = newData.value[i].mmPair.min;

						// ���~�b�g���m�F
						CheckLimitState(newData.dataid, newData.value[i - 1], newData.value[i]);
					}

					// �ŏ��̃f�[�^���J�n�����ɂȂ�悤�ɒ���
					if( mSpaceData.back().value.size() )
					{
						if( theApp.GetRealTimeStart() < CTime(mSpaceData.back().value.back().systime) )
						{
							theApp.GetRealTimeStart().GetAsSystemTime(mSpaceData.back().value.back().systime);
							mSpaceData.back().value.back().status = SDS_INVALID;
							mSpaceData.back().value.back().mmPair.max = 0.0;
							mSpaceData.back().value.back().mmPair.min = 0.0;
							mSpaceData.back().value.back().mmPair.cnt = 1;
							mSpaceTemp.push_back(newData);
						}
					}

					// �R�s�[�p�f�[�^���R�s�[
					mSpaceData.back().value_for_copy.reserve(tblRealScaleInfo[0].hld_data);
					for( int i = 0; i < mSpaceData.back().value.size(); i++ )
						mSpaceData.back().value_for_copy.push_back(mSpaceData.back().value[i]);

					// ���k�f�[�^���쐬
					CompSpaceData(newData.dataid, (int)mSpaceData.back().value.size() - 1);
				}

				mSpaceTemp.erase(mSpaceTemp.begin());
			}

#ifndef HighSpeedDemo
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Set SpaceTemp"), _T("End"), nLogEx::debug);
#endif // HighSpeedDemo

			// ���A�����[�h����f�[�^��ۑ�
			WriteRealTimeData();
		}

		CGraphMutex::Unlock(eRealTimeData);

		// ��͈ڍs�t���O���m�F
		if( theApp.IsAnalysisMigrationAll() )
		{
			//=====================================================//
			//������������������������ Log ������������������������//
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Analysis"), _T("Start"), nLogEx::debug);
			//������������������������ Log ������������������������//
			//=====================================================//

			// �擾�����f�[�^���ۑ�����Ă��邩�m�F
			while( IsExistSaveRealTimeDataQue() )
				Sleep(5);

			CGraphMutex::Lock(eRealTimeData);

			// �m�����A�����[�h�ւ̐؂�ւ�������
			for( int i = 0; i < MAX_VIEW; i++ )
			{
				// ��͈ڍs�t���O���m�F
				if( theApp.IsAnalysisMigration(i) )
				{
					// ��͂Ŏg�p����f�[�^���쐬
					CreateAnalysisDataFromRealTime(i);

					// ��͈ڍs�t���O���N���A
					theApp.ClrAnalysisMigration(i);
				}
			}

			CGraphMutex::Unlock(eRealTimeData);

			//=====================================================//
			//������������������������ Log ������������������������//
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Analysis"), _T("End"), nLogEx::debug);
			//������������������������ Log ������������������������//
			//=====================================================//
		}
	}
	catch (CMemoryException* e)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Exception"), _T("Memory"), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//

		e->Delete();
	}
	catch (...)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("Exception"), _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
	}

#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("End"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo

	return true;
}

/*============================================================================*/
/*! �f�[�^���W

-# ���A�����[�h����f�[�^��ۑ�

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::WriteRealTimeData()
{
#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("WriteRealTimeData"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo

	while( 1 )
	{
		CTime	tmpRealTimeWrite = CTime(0);

		bool	bValidData = false;

		vector<stSpaceData>	tmpSpaceData;

		tmpSpaceData.reserve(mSelectDataList.size());

		// �f�[�^�o�^���X�g�����o
		for( int i = 0; i < mSelectDataList.size(); i++ )
		{
			deque<stSpaceData>& value = GetSameSpaceData(mSelectDataList[i].id);

			// �Ď��f�[�^���X�g�����o
			for( int j = (int)value.size() - 1; j >= 0; j-- )
			{
				// �ۑ��������m�F
				if( theApp.GetRealTimeWriteData(i) != CTime(value[j].systime) )
					continue;

				// �ꎞ�ۑ��������m�F
				if( (tmpRealTimeWrite != CTime(0)) && (tmpRealTimeWrite != CTime(value[j].systime)) )
					continue;

				// �ۑ��������X�V
				theApp.SetRealTimeWriteData(i, theApp.GetRealTimeWriteData(i) + CTimeSpan(0, 0, 0, 1));

				// �ꎞ�ۑ��������X�V
				tmpRealTimeWrite = CTime(value[j].systime);

				// �L���f�[�^��ۑ��p�Ď��f�[�^���X�g�ɒǉ�
				tmpSpaceData.push_back(value[j]);

				// �L���f�[�^�t���O���Z�b�g
				bValidData = true;

				break;
			}

			// ��f�[�^��ۑ��p�Ď��f�[�^���X�g�ɒǉ�
			if( i >= tmpSpaceData.size() )
			{
				stSpaceData		sEmptyData;
				memset(&sEmptyData, 0, sizeof(stSpaceData));
				CTime(0).GetAsSystemTime(sEmptyData.systime);
				sEmptyData.status = SDS_INVALID;
				sEmptyData.mmPair.max = 0.0;
				sEmptyData.mmPair.min = 0.0;
				sEmptyData.mmPair.cnt = 1;

				tmpSpaceData.push_back(sEmptyData);
			}
		}

		// �f�[�^�o�^����Ă���L���f�[�^���܂ޑS�Ď��f�[�^�����݂��Ă�����ۑ�
		if( bValidData && (tmpSpaceData.size() == mSelectDataList.size()) )
		{
#ifndef HighSpeedDemo
			//=====================================================//
			//������������������������ Log ������������������������//
			CString msg;
			msg.Format(_T("data size = %I64u"), tmpSpaceData.size());
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("WriteRealTimeData"), _T("PushSaveRealTimeDataQue"), msg, nLogEx::debug);
			//������������������������ Log ������������������������//
			//=====================================================//
#endif // HighSpeedDemo

			// �Ď��f�[�^��ۑ�
			PushSaveRealTimeDataQue(tmpSpaceData);

			// �ő�ۑ��������X�V
			if( theApp.GetRealTimeWriteMax() == tmpRealTimeWrite )
				theApp.SetRealTimeWriteMax(theApp.GetRealTimeWriteMax() + CTimeSpan(0, 0, 0, 1));

			tmpSpaceData.clear();
		}
		else
		{
			tmpSpaceData.clear();

			// �I��
			break;
		}
	}

#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("WriteRealTimeData"), _T("End"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo
}

/*============================================================================*/
/*! �f�[�^���W

-# ���A�����[�h����f�[�^�̃��~�b�g���m�F

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CheckLimitState(DWORD dataid, stSpaceData& data1, stSpaceData& data2)
{
#if 0
#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CheckLimitState"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo
#endif

	for( int i = 0; i < MAX_VIEW; i++ )
	{
		CPropertiesData&	prop = theApp.GetPropertiesData(i);

		for( int j = 0; j < MAX_TDATA; j++ )
		{
			stLimitState	tmpLimitState;

			// �Y���̊Ď��f�[�^������
			if( dataid != GetSelectDataId(prop.item[j].dataName) )
				continue;

			// ���[�j���O����𔻒�
			if( prop.item[j].isLimitWarningUpper )
			{
				// ���������H
				if( (data1.mmPair.max < prop.item[j].limitWarningUpper) && (prop.item[j].limitWarningUpper <= data2.mmPair.max) )
				{
					tmpLimitState.time = CTime(data2.systime);
					tmpLimitState.state = MAKE_LIMIT_STATE(LimitWarning, LimitUpper, LimitAbove);

					CGraphMutex::Lock(eLimitState);

					if( m_LimitState[i][j].size() >= MAX_LIMIT_HISTORY )
						m_LimitState[i][j].pop_front();

					m_LimitState[i][j].push_back(tmpLimitState);

					CGraphMutex::Unlock(eLimitState);
				}
				// ���������?
				if( (prop.item[j].limitWarningUpper <= data1.mmPair.max) && (data2.mmPair.max < prop.item[j].limitWarningUpper) )
				{
					tmpLimitState.time = CTime(data2.systime);
					tmpLimitState.state = MAKE_LIMIT_STATE(LimitWarning, LimitUpper, LimitBelow);

					CGraphMutex::Lock(eLimitState);

					if( m_LimitState[i][j].size() >= MAX_LIMIT_HISTORY )
						m_LimitState[i][j].pop_front();

					m_LimitState[i][j].push_back(tmpLimitState);

					CGraphMutex::Unlock(eLimitState);
				}
			}
			// ���[�j���O�����𔻒�
			if( prop.item[j].isLimitWarningLower )
			{
				// ���������?
				if( (prop.item[j].limitWarningLower < data1.mmPair.min) && (data2.mmPair.min <= prop.item[j].limitWarningLower) )
				{
					tmpLimitState.time = CTime(data2.systime);
					tmpLimitState.state = MAKE_LIMIT_STATE(LimitWarning, LimitLower, LimitBelow);

					CGraphMutex::Lock(eLimitState);

					if( m_LimitState[i][j].size() >= MAX_LIMIT_HISTORY )
						m_LimitState[i][j].pop_front();

					m_LimitState[i][j].push_back(tmpLimitState);

					CGraphMutex::Unlock(eLimitState);
				}
				// ���������H
				if( (data1.mmPair.min <= prop.item[j].limitWarningLower) && (prop.item[j].limitWarningLower < data2.mmPair.min) )
				{
					tmpLimitState.time = CTime(data2.systime);
					tmpLimitState.state = MAKE_LIMIT_STATE(LimitWarning, LimitLower, LimitAbove);

					CGraphMutex::Lock(eLimitState);

					if( m_LimitState[i][j].size() >= MAX_LIMIT_HISTORY )
						m_LimitState[i][j].pop_front();

					m_LimitState[i][j].push_back(tmpLimitState);

					CGraphMutex::Unlock(eLimitState);
				}
			}
			// �A���[������𔻒�
			if( prop.item[j].isLimitAlarmUpper )
			{
				// ���������H
				if( (data1.mmPair.max < prop.item[j].limitAlarmUpper) && (prop.item[j].limitAlarmUpper <= data2.mmPair.max) )
				{
					tmpLimitState.time = CTime(data2.systime);
					tmpLimitState.state = MAKE_LIMIT_STATE(LimitAlarm, LimitUpper, LimitAbove);

					CGraphMutex::Lock(eLimitState);

					if( m_LimitState[i][j].size() >= MAX_LIMIT_HISTORY )
						m_LimitState[i][j].pop_front();

					m_LimitState[i][j].push_back(tmpLimitState);

					CGraphMutex::Unlock(eLimitState);
				}
				// ���������?
				if( (prop.item[j].limitAlarmUpper <= data1.mmPair.max) && (data2.mmPair.max < prop.item[j].limitAlarmUpper) )
				{
					tmpLimitState.time = CTime(data2.systime);
					tmpLimitState.state = MAKE_LIMIT_STATE(LimitAlarm, LimitUpper, LimitBelow);

					CGraphMutex::Lock(eLimitState);

					if( m_LimitState[i][j].size() >= MAX_LIMIT_HISTORY )
						m_LimitState[i][j].pop_front();

					m_LimitState[i][j].push_back(tmpLimitState);

					CGraphMutex::Unlock(eLimitState);
				}
			}
			// �A���[�������𔻒�
			if( prop.item[j].isLimitAlarmLower )
			{
				// ���������?
				if( (prop.item[j].limitAlarmLower < data1.mmPair.min) && (data2.mmPair.min <= prop.item[j].limitAlarmLower) )
				{
					tmpLimitState.time = CTime(data2.systime);
					tmpLimitState.state = MAKE_LIMIT_STATE(LimitAlarm, LimitLower, LimitBelow);

					CGraphMutex::Lock(eLimitState);

					if( m_LimitState[i][j].size() >= MAX_LIMIT_HISTORY )
						m_LimitState[i][j].pop_front();

					m_LimitState[i][j].push_back(tmpLimitState);

					CGraphMutex::Unlock(eLimitState);
				}
				// ���������H
				if( (data1.mmPair.min <= prop.item[j].limitAlarmLower) && (prop.item[j].limitAlarmLower < data2.mmPair.min) )
				{
					tmpLimitState.time = CTime(data2.systime);
					tmpLimitState.state = MAKE_LIMIT_STATE(LimitAlarm, LimitLower, LimitAbove);

					CGraphMutex::Lock(eLimitState);

					if( m_LimitState[i][j].size() >= MAX_LIMIT_HISTORY )
						m_LimitState[i][j].pop_front();

					m_LimitState[i][j].push_back(tmpLimitState);

					CGraphMutex::Unlock(eLimitState);
				}
			}
		}
	}

#if 0
#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CheckLimitState"), _T("End"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo
#endif
}

/*============================================================================*/
/*! �f�[�^���W

-# ��͂Ŏg�p����f�[�^�����A���^�C������f�[�^����쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateAnalysisDataFromRealTime(int nView)
{
#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateAnalysisDataFromRealTime"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo

//	DWORD	t = timeGetTime();

	CGraphMutex::Lock_Analysis(nView);

	CGraphMutex::Lock(eRealTimeSave);

	// ����̊J�n��������͂̊J�n�����Ƃ��Đݒ�
	mAnalysisStart[nView] = theApp.GetRealTimeStart();

	// �f�[�^�o�^�̊Ď��f�[�^���ۑ�����Ă���ő厞������͂̏I�������Ƃ��Đݒ�
	mAnalysisEnd[nView] = theApp.GetRealTimeWriteMax() - CTimeSpan(0, 0, 0, 1);

	// ��͂̎������m�F
	if( mAnalysisStart[nView] > mAnalysisEnd[nView] )
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateAnalysisDataFromRealTime"), _T("Error"), _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//

		CGraphMutex::Unlock(eRealTimeSave);

		CGraphMutex::Unlock_Analysis(nView);

		return;
	}

	// �g�����hALL�ɕ\������X�P�[���ԍ���ݒ�
	for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
	{
		unsigned __int64	data_size = GetAnalysisCount(nView) / tblRealScaleInfo[t].cmp_rate;

		if( tblRealScaleInfo[t].dsp_data > data_size )
		{
			mTrendAllTblNo[nView] = t;
			break;
		}
	}

	// �g�����hALL�^Zoom�ɕ\������Ď��f�[�^���擾
	CPropertiesData&	prop = theApp.GetPropertiesData(nView);
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		DWORD	id = theApp.GetDataCollection().GetSelectDataId(prop.item[i].dataName);
		unsigned __int64	size, count;

		if( id == 0xFFFFFFFF )
			continue;

		// �f�[�^���擾
		vector<stSpaceData>&	data_all_list = (tblRealScaleInfo[mTrendAllTblNo[nView]].cmp_rate > 1) ? GetCompDataForCopy(id, mTrendAllTblNo[nView]) : GetSameSpaceDataForCopy(id);
		size = data_all_list.size();
		if( size == 0 )
			continue;

		// �f�[�^���R�s�[
		mTrendAllData[nView][i].resize(tblRealScaleInfo[mTrendAllTblNo[nView]].dsp_data);
		memcpy((stSpaceData *)&(mTrendAllData[nView][i][0]), (stSpaceData *)&(data_all_list[0]), sizeof(stSpaceData) * size);

		if( (count = GetAnalysisCount(nView)) == 0 )
			continue;

		mTrendZoomMemMap[nView][i].ReadInitialization(-1, GetSelectDataNo(id), theApp.GetStartFile(), count);
	}

	// ZOOM�̃X�P�[���e�[�u���ԍ��^�`��ʒu�^�f�[�^���X�V
	UpdateTrendZoomTblNo(nView, m1, false);
	UpdateTrendZoomPos(nView, 0/* �����l(0�F�ŏ��̃f�[�^�ԍ�) */, false);
	UpdateTrendZoomData(nView, false);

	// �J�[�\���̃f�[�^�ԍ��^�f�[�^���X�V
	UpdateTrendCursorPos(nView, 0/* �����l(0�F�ŏ��̃f�[�^�ԍ�) */, false);
	UpdateTrendCursorData(nView, false);

	// �r���[�ɃO���t�X�V��ʒm
	CWnd	*pWnd = ((CMainFrame *)theApp.m_pMainWnd)->FindChildListWnd(nView);
	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 1, (LPARAM)nView);
	
	CGraphMutex::Unlock(eRealTimeSave);

	CGraphMutex::Unlock_Analysis(nView);

//	TRACE("CreateAnalysisDataFromRealTime end time = %d\n", timeGetTime() - t);

#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateAnalysisDataFromRealTime"), _T("End"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo
}

/*============================================================================*/
/*! �f�[�^���W

-# ��͂Ŏg�p����f�[�^���Ď��f�[�^�t�@�C������쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateAnalysisDataFromFile(int nView, CTime nStart, CTime nEnd)
{
#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateAnalysisDataFromFile"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo

//	DWORD	t = timeGetTime();

	// �m�����A���^�C���f�[�^�ۑ��X���b�h���I��
	removeSaveAnalysisDataThread(nView);

	// ��͂Ŏg�p����f�[�^���폜
	DeleteAnalysisData(nView);

	CGraphMutex::Lock_Analysis(nView);

	// �t�@�C���w��̊J�n��������͂̊J�n�����Ƃ��Đݒ�
	mAnalysisStart[nView] = nStart;

	// �t�@�C���w��̏I����������͂̏I�������Ƃ��Đݒ�
	mAnalysisEnd[nView] = nEnd;

	// ��͂̎������m�F
	if( mAnalysisStart[nView] > mAnalysisEnd[nView] )
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateAnalysisDataFromFile"), _T("Error"), _T(""), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//

		CGraphMutex::Unlock_Analysis(nView);

		return;
	}

	// �g�����hALL�ɕ\������X�P�[���ԍ���ݒ�
	for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
	{
		unsigned __int64	data_size = GetAnalysisCount(nView) / tblRealScaleInfo[t].cmp_rate;

		if( tblRealScaleInfo[t].dsp_data > data_size )
		{
			mTrendAllTblNo[nView] = t;
			break;
		}
	}

	// �g�����hALL�^Zoom�ɕ\������Ď��f�[�^���擾
	CPropertiesData&	prop = theApp.GetPropertiesData(nView);
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		DWORD	id = theApp.GetDataCollection().GetSelectDataId(prop.item[i].dataName);
		unsigned __int64	count;

		if( id == 0xFFFFFFFF )
			continue;

		// ��f�[�^���쐬
#if 0
		mTrendAllData[nView][i].resize(tblRealScaleInfo[mTrendAllTblNo[nView]].dsp_data);
		memset((stSpaceData *)&(mTrendAllData[nView][i][0]), 0, sizeof(stSpaceData) * mTrendAllData[nView][i].size());
#else
		mTrendAllData[nView][i].resize(tblRealScaleInfo[mTrendAllTblNo[nView]].dsp_data);
#endif

		if( (count = GetAnalysisCount(nView)) == 0 )
			continue;

		mTrendZoomMemMap[nView][i].ReadInitialization(nView, i, 0, count);
	}

#if 0
	// ZOOM�̃X�P�[���e�[�u���ԍ��^�`��ʒu�^�f�[�^���X�V
	UpdateTrendZoomTblNo(nView, m1, false);
	UpdateTrendZoomPos(nView, 0/* �����l(0�F�ŏ��̃f�[�^�ԍ�) */, false);
	UpdateTrendZoomData(nView, false);

	// �J�[�\���̃f�[�^�ԍ��^�f�[�^���X�V
	UpdateTrendCursorPos(nView, 0/* �����l(0�F�ŏ��̃f�[�^�ԍ�) */, false);
	UpdateTrendCursorData(nView, false);

	// �r���[�ɃO���t�X�V��ʒm
	CWnd	*pWnd = ((CMainFrame *)theApp.m_pMainWnd)->FindChildListWnd(nView);
	if( pWnd != NULL )
		theApp.m_pMainWnd->PostMessage(eMessage_PropetyChange, 1, (LPARAM)nView);
#endif

	CGraphMutex::Unlock_Analysis(nView);

	// �m�����A���^�C���f�[�^�ۑ��X���b�h���J�n
	createSaveAnalysisDataThread(nView);

//	TRACE("CreateAnalysisDataFromRealTime end time = %d\n", timeGetTime() - t);

#ifndef HighSpeedDemo
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateAnalysisDataFromFile"), _T("End"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif // HighSpeedDemo
}

/*============================================================================*/
/*! �f�[�^���W

-# ��͂Ŏg�p����f�[�^���폜

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::DeleteAnalysisData(int nView)
{
	CGraphMutex::Lock_Analysis(nView);

	mAnalysisStart[nView] = CTime(0);
	mAnalysisEnd[nView] = CTime(0);

	mTrendAllTblNo[nView] = 0;
	mTrendZoomTblNo[nView] = 0;

	mTrendZoomStart[nView] = 0;
	mTrendZoomEnd[nView] = 0;

	mTrendCursor[nView] = 0;

	for( int i = 0; i < MAX_TDATA; i++ )
	{
		mTrendAllData[nView][i].clear();

		if( mTrendZoomMemMap[nView][i].GetMemMapListStatus() != eMMListStatus_None )
			mTrendZoomMemMap[nView][i].ReadFinalization();

		mTrendZoomData[nView][i].clear();

		mTrendZoomMMPair[nView][i].cnt = 1;
		mTrendZoomMMPair[nView][i].max = -DBL_MAX;
		mTrendZoomMMPair[nView][i].min = DBL_MAX;

		mTrendCursorData[nView][i].clear();
	}

	CGraphMutex::Unlock_Analysis(nView);
}

/*============================================================================*/
/*! �f�[�^���W

-# ��͂Ŏg�p����ZOOM�̕`��f�[�^���X�V

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::UpdateTrendZoomData(int nView, bool bLock/* = true*/)
{
	if( bLock )
		CGraphMutex::Lock_Analysis(nView);

	// �Ď��f�[�^�����݂��郁�����}�b�v�����o
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		unsigned __int64	nCount = mTrendZoomMemMap[nView][i].GetDataCount();
		if( nCount == 0 )
			continue;

		// �ǂݍ��ݓ_�����m�F
		if( (mTrendZoomEnd[nView] - mTrendZoomStart[nView] + 1) > mTrendZoomData[nView][i].size() )
			continue;

		// �������}�b�v����w��͈͂̊Ď��f�[�^��ǂݍ���
		mTrendZoomMemMap[nView][i].ReadData(mTrendZoomStart[nView], mTrendZoomEnd[nView], mTrendZoomData[nView][i]);

		// �Y�[���g���̊e�f�[�^�̍ő�l�^�ŏ��l���擾
		mTrendZoomMMPair[nView][i].max = -DBL_MAX;
		mTrendZoomMMPair[nView][i].min = DBL_MAX;
		for( int j = 0; j < (mTrendZoomEnd[nView] - mTrendZoomStart[nView] + 1); j++ )
		{
			if( mTrendZoomData[nView][i][j].status == SDS_INVALID )
				continue;

			if( j >= mTrendZoomData[nView][i].size() )
				continue;

			if( mTrendZoomMMPair[nView][i].max < mTrendZoomData[nView][i][j].mmPair.max )
				mTrendZoomMMPair[nView][i].max = mTrendZoomData[nView][i][j].mmPair.max;
			if( mTrendZoomMMPair[nView][i].min > mTrendZoomData[nView][i][j].mmPair.min )
				mTrendZoomMMPair[nView][i].min = mTrendZoomData[nView][i][j].mmPair.min;
		}
	}

	if( bLock )
		CGraphMutex::Unlock_Analysis(nView);
}

/*============================================================================*/
/*! �f�[�^���W

-# ��͂Ŏg�p����J�[�\���ʒu�̃f�[�^���X�V

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::UpdateTrendCursorData(int nView, bool bLock/* = true*/)
{
	if( bLock )
		CGraphMutex::Lock_Analysis(nView);

	// �Ď��f�[�^�����݂��郁�����}�b�v�����o
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		mTrendCursorData[nView][i].clear();

		unsigned __int64	nCount = mTrendZoomMemMap[nView][i].GetDataCount();
		if( nCount == 0 )
			continue;

		// �J�[�\���ʒu�̊Ď��f�[�^��ǂݍ���
		mTrendCursorData[nView][i].resize(1);
		mTrendZoomMemMap[nView][i].ReadData(mTrendCursor[nView], mTrendCursor[nView], mTrendCursorData[nView][i]);
	}

	if( bLock )
		CGraphMutex::Unlock_Analysis(nView);
}

/*============================================================================*/
/*! �f�[�^���W

-# ��͂Ŏg�p����ZOOM�̃X�P�[���e�[�u���ԍ����X�V

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::UpdateTrendZoomTblNo(int nView, int nTbl, bool bLock/* = true*/)
{
	if( bLock )
		CGraphMutex::Lock_Analysis(nView);

	// �w��̃X�P�[���e�[�u���ԍ����m�F
	if( (nTbl < 0) || (GetTrendAllTblNo(nView) == m1) )
		nTbl = m1;
	else
	{
		if( (GetTrendAllTblNo(nView) == nTbl) || (tblRealScaleInfo[nTbl].cmp_rate > 1) )
			nTbl = mTrendZoomTblNo[nView];
	}

	// �X�P�[���e�[�u���ԍ���ݒ�
	mTrendZoomTblNo[nView] = nTbl;

	// �w��X�P�[���e�[�u���ԍ��̕\���_�����擾
	unsigned long	display = tblRealScaleInfo[mTrendZoomTblNo[nView]].dsp_data;

	for( int i = 0; i < MAX_TDATA; i++ )
	{
		vector<stSpaceData>	tmpTrendZoomData(0);

		// �m�ۍς݂̗̈�T�C�Y���m�F
		if( mTrendZoomData[nView][i].size() == display )
			continue;

		// �Ď��f�[�^�i�[�̈���ēx�m��
		mTrendZoomData[nView][i].swap(tmpTrendZoomData);
		mTrendZoomData[nView][i].clear();
		mTrendZoomData[nView][i].resize(display);
	}

	if( bLock )
		CGraphMutex::Unlock_Analysis(nView);
}

/*============================================================================*/
/*! �f�[�^���W

-# ��͂Ŏg�p����ZOOM�̕`��ʒu���X�V

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::UpdateTrendZoomPos(int nView, unsigned __int64 nStart, bool bLock/* = true*/)
{
	if( bLock )
		CGraphMutex::Lock_Analysis(nView);

	// �w��X�P�[���e�[�u���ԍ��̕\���_�����擾
	unsigned long	display = tblRealScaleInfo[mTrendZoomTblNo[nView]].dsp_data;

	unsigned __int64	nMaxCount = 0;
	int	index = -1;
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		unsigned __int64	nCount = mTrendZoomMemMap[nView][i].GetDataCount();
		if( nCount == 0 )
			continue;

		if( nMaxCount < nCount )
		{
			nMaxCount = nCount;

			index = i;
		}
	}

	// �Ď��f�[�^�����݂��郁�����}�b�v�����o
	if( index != -1 )
	{
		unsigned __int64	nCount = mTrendZoomMemMap[nView][index].GetDataCount();

		// �J�n�ʒu���S�_�����z���Ă��邩�H
		if( nStart >= nCount )
			nStart = nCount - 1;

		// �S�_�����\���_���ȉ����H
		if( nCount <= display )
		{
			mTrendZoomStart[nView] = 0;
			mTrendZoomEnd[nView] = nCount - 1;
		}
		else
		{
			unsigned __int64	nEnd = nStart + display - 1;

			// �I���ʒu���S�_�����z���Ă��邩�H
			if( nEnd >= nCount )
			{
				mTrendZoomStart[nView] = nCount - display;
				mTrendZoomEnd[nView] = nCount - 1;
			}
			else
			{
				mTrendZoomStart[nView] = nStart;
				mTrendZoomEnd[nView] = nEnd;
			}
		}
	}

	if( bLock )
		CGraphMutex::Unlock_Analysis(nView);
}

/*============================================================================*/
/*! �f�[�^���W

-# ��͂Ŏg�p����J�[�\���̈ʒu���X�V

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::UpdateTrendCursorPos(int nView, unsigned __int64 nPos, bool bLock/* = true*/)
{
	if( bLock )
		CGraphMutex::Lock_Analysis(nView);

	unsigned __int64	nMaxCount = 0;
	int	index = -1;
	for( int i = 0; i < MAX_TDATA; i++ )
	{
		unsigned __int64	nCount = mTrendZoomMemMap[nView][i].GetDataCount();
		if( nCount == 0 )
			continue;

		if( nMaxCount < nCount )
		{
			nMaxCount = nCount;

			index = i;
		}
	}

	// �Ď��f�[�^�����݂��郁�����}�b�v�����o
	if( index != -1 )
	{
		unsigned __int64	nCount = mTrendZoomMemMap[nView][index].GetDataCount();

		// �J�n�ʒu���S�_�����z���Ă��邩�H
		if( nPos >= nCount )
			nPos = nCount - 1;

		// �J�[�\���ʒu�̃f�[�^�ԍ���ݒ�
		mTrendCursor[nView] = nPos;
	}

	if( bLock )
		CGraphMutex::Unlock_Analysis(nView);
}

/*============================================================================*/
/*! �f�[�^���W

-# �擾�������̊Ď��f�[�^���爳�k�f�[�^���쐬����

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CompSpaceData(DWORD id, int pos, bool clr/* = false*/)
{
	int	index, i;

	// �����Ď��f�[�^ID�̈��k���Ď��f�[�^�����o
	for( index = -1, i = 0; i < mSpaceData.size(); i++ )
	{
		if( mSpaceData[i].dataid == id )
		{
			index = i;
			break;
		}
	}
	if( index == -1 )
		return;
	CSpaceData&	src_data = mSpaceData[index];

	// �����Ď��f�[�^ID�̃f�[�^�o�^�ԍ������o
	if( (index = GetSelectDataNo(id)) == -1 )
		return;

	try
	{
		// �N���A�t���OON�Ȃ爳�k�f�[�^���폜
		if( clr )
		{
			for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
			{
				m_CompData[index][t].clear();
				m_CompDataForCopy[index][t].clear();
			}
		}

		// �w�肳�ꂽ�Ď��f�[�^�̈��k�f�[�^���쐬
		for( ; pos < src_data.value.size(); pos++ )
		{
			for( int t = 0; t < NUM_REAL_SCALE_INFO_TABLE; t++ )
			{
				// �񈳏k�X�P�[���Ȃ�X���[
				if( tblRealScaleInfo[t].cmp_rate <= 1 )
					continue;

				// ���k�X�P�[���̕`�����ύX
				ChangeDrawInfoEx(id, t);

				// ���k�f�[�^�����݂��Ȃ��Ȃ珉����
				if( m_CompData[index][t].empty() )
				{
					m_CompData[index][t].push_back(src_data.value[pos]);

					// �R�s�[�p�f�[�^���R�s�[
					m_CompDataForCopy[index][t].reserve(tblRealScaleInfo[t].hld_data);
					m_CompDataForCopy[index][t].push_back(src_data.value[pos]);

//					TRACE(_T("[comp new] index = %d, table = %d, pos = %d, count = %d\n"), index, t, m_CompData[index][t].size() - 1, m_CompData[index][t].back().mmPair.count);

					continue;
				}

				// �ŏI�ʒu�̃f�[�^�������k���ɒB���Ă��邩�m�F
				if( tblRealScaleInfo[t].cmp_rate <= m_CompData[index][t].back().mmPair.cnt )
				{
					// �����_���ɒB���Ă��邩�m�F
					if( tblRealScaleInfo[t].hld_data <= m_CompData[index][t].size() )
					{
						// �ŏI�X�P�[���Ȃ�A�P�t�@�C��������̃f�[�^�������폜���Ċi�[���p��
						if( t == (NUM_REAL_SCALE_INFO_TABLE - 1) )
						{
							unsigned __int64	delete_size = MAX_SIZE_PER_MMDATA / tblRealScaleInfo[t].cmp_rate;

							vector<stSpaceData>	tmpCompDataForCopy(0);
							m_CompDataForCopy[index][t].swap(tmpCompDataForCopy);
							m_CompDataForCopy[index][t].reserve(tblRealScaleInfo[t].hld_data);

							for( i = 0; i < delete_size; i++ )
								m_CompData[index][t].pop_front();

							for( i = 0; i < (tblRealScaleInfo[t].hld_data - delete_size); i++ )
								m_CompDataForCopy[index][t].push_back(tmpCompDataForCopy[i + delete_size]);

							theApp.SetRealTimeStart(theApp.GetRealTimeStart() + MAX_SIZE_PER_MMDATA);
							theApp.SetStartFile(theApp.GetStartFile() + 1);
						}
						else
							m_CompData[index][t].pop_front();
					}

					m_CompData[index][t].push_back(src_data.value[pos]);

					// �R�s�[�p�f�[�^���R�s�[
					if( m_CompDataForCopy[index][t].size() < tblRealScaleInfo[t].hld_data )
						m_CompDataForCopy[index][t].push_back(src_data.value[pos]);

//					TRACE(_T("[comp over] index = %d, table = %d, pos = %d, count = %d\n"), index, t, m_CompData[index][t].size() - 1, m_CompData[index][t].back().mmPair.count);

					continue;
				}

				// ���k�f�[�^���X�V
				stSpaceData&	cmp_value = m_CompData[index][t].back();
				stSpaceData&	src_value = src_data.value[pos];
				cmp_value.mmPair.cnt += src_value.mmPair.cnt;

#if 0
				cmp_value.mmPair.max = __max(cmp_value.mmPair.max, src_value.mmPair.max);
				cmp_value.mmPair.min = __min(cmp_value.mmPair.min, src_value.mmPair.min);
				if( (cmp_value.status == SDS_INVALID) && (src_value.status & SDS_VALID) )
					cmp_value.status = src_value.status;	// �L���f�[�^�ɕύX
#else
				if( (cmp_value.status & SDS_VALID) && (src_value.status & SDS_VALID) )	// �L���̗L��
				{
					cmp_value.mmPair.max = __max(cmp_value.mmPair.max, src_value.mmPair.max);
					cmp_value.mmPair.min = __min(cmp_value.mmPair.min, src_value.mmPair.min);
				}
				else if( (cmp_value.status == SDS_INVALID) && (src_value.status & SDS_VALID) )	// �����̗L��
				{
					cmp_value.mmPair.max = src_value.mmPair.max;
					cmp_value.mmPair.min = src_value.mmPair.min;
					cmp_value.status = src_value.status;	// �L���f�[�^�ɕύX
				}

				if( cmp_value.status == SDS_INVALID )
				{
					cmp_value.mmPair.max = 0.0;
					cmp_value.mmPair.min = 0.0;
				}
#endif

				// �R�s�[�p�f�[�^���R�s�[
				m_CompDataForCopy[index][t].back() = cmp_value;

//				TRACE(_T("[comp add] index = %d, table = %d, pos = %d, count = %d\n"), index, t, m_CompData[index][t].size() - 1, m_CompData[index][t].back().mmPair.count);

				// ���k�X�P�[���̕`������X�V
				UpdateDrawInfoEx(id, t);
			}
		}
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# �\������ύX

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::ChangeDrawInfoEx(DWORD id, int tbl)
{
	// �������m�F
	if( (UINT)tbl >= NUM_REAL_SCALE_INFO_TABLE )
		return;

	int	index, i;

	// �����Ď��f�[�^ID�̈��k���Ď��f�[�^�����o
	for( index = -1, i = 0; i < mSpaceData.size(); i++ )
	{
		if( mSpaceData[i].dataid == id )
		{
			index = i;
			break;
		}
	}
	if( index == -1 )
		return;
	CSpaceData&	src_data = mSpaceData[index];

	// �����Ď��f�[�^ID�̃f�[�^�o�^�ԍ������o
	if( (index = GetSelectDataNo(id)) == -1 )
		return;

	// �񈳏k�X�P�[���^���k�X�P�[���𔻒�
	if( tblRealScaleInfo[tbl].cmp_rate <= 1 )
	{
		if( !src_data.value.empty() )
		{
			// �`��_�����ő�\���_���ɒB���Ă���H
			if( m_TrendDraw[index][tbl].data_num >= tblRealScaleInfo[tbl].dsp_data )
			{
				m_TrendDraw[index][tbl].data_num = tblRealScaleInfo[tbl].dsp_data - (unsigned __int64)((tblRealScaleInfo[tbl].dsp_data - 1) * 0.1);
				m_TrendDraw[index][tbl].start_pos = (unsigned __int64)src_data.value.size() - m_TrendDraw[index][tbl].data_num;
				m_TrendDraw[index][tbl].change_flg = true;
			}

			// �X�V�J�n�ʒu���X�V
			if( m_TrendDraw[index][tbl].update_pos == 0 )
				m_TrendDraw[index][tbl].update_pos = (unsigned __int64)src_data.value.size();

			// �����_�����ő�����_���ɒB���Ă���H
			if( src_data.value.size() >= tblRealScaleInfo[0].hld_data )
			{
				// �ŏI�X�P�[���Ȃ�A�P�t�@�C��������̃f�[�^�������폜���Ċi�[���p��
				if( tbl == (NUM_REAL_SCALE_INFO_TABLE - 1) )
				{
					// �񈳏k�e�[�u�����ŏI�X�P�[���ɂȂ邱�Ƃ͖����̂ł����ɂ͗��Ȃ��B
				}
				else
				{
					m_TrendDraw[index][tbl].start_pos--;
					m_TrendDraw[index][tbl].update_pos--;
				}
			}
		}
	}
	else
	{
		if( !m_CompData[index][tbl].empty() )
		{
			// �ŏI�_�̈��k�������k���ɒB���Ă���H
			if( m_CompData[index][tbl].back().mmPair.cnt >= tblRealScaleInfo[tbl].cmp_rate )
			{
				// �`��_�����ő�\���_���ɒB���Ă���H
				if( m_TrendDraw[index][tbl].data_num >= tblRealScaleInfo[tbl].dsp_data )
				{
					m_TrendDraw[index][tbl].data_num = tblRealScaleInfo[tbl].dsp_data - (unsigned __int64)((tblRealScaleInfo[tbl].dsp_data - 1) * 0.1);
					m_TrendDraw[index][tbl].start_pos = (unsigned __int64)m_CompData[index][tbl].size() - m_TrendDraw[index][tbl].data_num;
					m_TrendDraw[index][tbl].change_flg = true;
				}

				// �X�V�J�n�ʒu���X�V
				if( m_TrendDraw[index][tbl].update_pos == 0 )
					m_TrendDraw[index][tbl].update_pos = (unsigned __int64)m_CompData[index][tbl].size();

				// �����_�����ő�����_���ɒB���Ă���H
				if( m_CompData[index][tbl].size() >= tblRealScaleInfo[tbl].hld_data )
				{
					// �ŏI�X�P�[���Ȃ�A�P�t�@�C��������̃f�[�^�������폜���Ċi�[���p��
					if( tbl == (NUM_REAL_SCALE_INFO_TABLE - 1) )
					{
						unsigned __int64	delete_size = MAX_SIZE_PER_MMDATA / tblRealScaleInfo[tbl].cmp_rate;

						m_TrendDraw[index][tbl].update_pos -= delete_size;
						m_TrendDraw[index][tbl].data_num -= delete_size;
					}
					else
					{
						m_TrendDraw[index][tbl].start_pos--;
						m_TrendDraw[index][tbl].update_pos--;
					}
				}
			}
			else
			{
				// �X�V�J�n�ʒu���X�V
				if( m_TrendDraw[index][tbl].update_pos == 0 )
					m_TrendDraw[index][tbl].update_pos = (int)m_CompData[index][tbl].size() - 1;
			}
		}
	}

//	TRACE(_T("[draw info change] index = %d, table = %d, pos = %d, num = %d\n"), index, tbl, m_TrendDraw[index][tbl].start_pos, m_TrendDraw[index][tbl].data_num);
}

/*============================================================================*/
/*! �f�[�^���W

-# �\�������X�V

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::UpdateDrawInfoEx(DWORD id, int tbl)
{
	// �������m�F
	if( (UINT)tbl >= NUM_REAL_SCALE_INFO_TABLE )
		return;

	int	index;

	// �����Ď��f�[�^ID�̃f�[�^�o�^�ԍ������o
	if( (index = GetSelectDataNo(id)) == -1 )
		return;

	// �񈳏k�X�P�[���^���k�X�P�[���𔻒�
	if( tblRealScaleInfo[tbl].cmp_rate <= 1 )
		m_TrendDraw[index][tbl].data_num++;
	else
	{
		// �ŏI�_�̈��k�������k���ɒB���Ă���H
		if( (!m_CompData[index][tbl].empty()) && (m_CompData[index][tbl].back().mmPair.cnt >= tblRealScaleInfo[tbl].cmp_rate) )
			m_TrendDraw[index][tbl].data_num++;
	}

//	TRACE(_T("[draw info update] index = %d, table = %d, pos = %d, num = %d\n"), index, tbl, m_TrendDraw[index][tbl].start_pos, m_TrendDraw[index][tbl].data_num);
}

/*============================================================================*/
/*! �f�[�^���W

-# �Ď��f�[�^���擾����

@param

@retval deque<stSpaceData>&
*/
/*============================================================================*/
deque<stSpaceData>& CDataCollection::GetSameSpaceData(DWORD id)
{
	int	index, i;

	for( index = -1, i = 0; i < mSpaceData.size(); i++ )
	{
		if( mSpaceData[i].dataid == id )
		{
			index = i;
			break;
		}
	}
	if( index == -1 )
		return m_DummySpaceDataDeque;

	return mSpaceData[index].value;
}

/*============================================================================*/
/*! �f�[�^���W

-# �R�s�[�p�Ď��f�[�^���擾����

@param

@retval vector<stSpaceData>&
*/
/*============================================================================*/
vector<stSpaceData>& CDataCollection::GetSameSpaceDataForCopy(DWORD id)
{
	int	index, i;

	for( index = -1, i = 0; i < mSpaceData.size(); i++ )
	{
		if( mSpaceData[i].dataid == id )
		{
			index = i;
			break;
		}
	}
	if( index == -1 )
		return m_DummySpaceDataVector;

	return mSpaceData[index].value_for_copy;
}

/*============================================================================*/
/*! �f�[�^���W

-# ���k�f�[�^���擾����

@param

@retval deque<stSpaceData>&
*/
/*============================================================================*/
deque<stSpaceData>& CDataCollection::GetCompData(DWORD id, int tbl)
{
	int	index;

	// �������m�F
	if( (UINT)tbl >= NUM_REAL_SCALE_INFO_TABLE )
		tbl = 0;

	// �����Ď��f�[�^ID�̃f�[�^�o�^�ԍ������o
	if( (index = GetSelectDataNo(id)) == -1 )
		return m_DummySpaceDataDeque;

	return m_CompData[index][tbl];
}

/*============================================================================*/
/*! �f�[�^���W

-# �R�s�[�p���k�f�[�^���擾����

@param

@retval vector<stSpaceData>&
*/
/*============================================================================*/
vector<stSpaceData>& CDataCollection::GetCompDataForCopy(DWORD id, int tbl)
{
	int	index;

	// �������m�F
	if( (UINT)tbl >= NUM_REAL_SCALE_INFO_TABLE )
		tbl = 0;

	// �����Ď��f�[�^ID�̃f�[�^�o�^�ԍ������o
	if( (index = GetSelectDataNo(id)) == -1 )
		return m_DummySpaceDataVector;

	return m_CompDataForCopy[index][tbl];
}

/*============================================================================*/
/*! �f�[�^���W

-# �`����擾����

@param

@retval stDrawInfo&
*/
/*============================================================================*/
stDrawInfo& CDataCollection::GetDrawInfo(DWORD id, int tbl)
{
	int	index;

	// �������m�F
	if( (UINT)tbl >= NUM_REAL_SCALE_INFO_TABLE )
		tbl = 0;

	// �����Ď��f�[�^ID�̃f�[�^�o�^�ԍ������o
	if( (index = GetSelectDataNo(id)) == -1 )
		return m_DummyDrawInfo;

	return m_TrendDraw[index][tbl];
}

/*============================================================================*/
/*! �f�[�^���W

-# �Ď������X�g��ۑ�

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::WriteDataNameList(CString szFilePath)
{
	CString	str;

	// �Ď����̓o�^����ۑ�
	str.Format(_T("%d"), (UINT)mSelectDataList.size());
	WritePrivateProfileString(SECTION_DATALIST, KEY_DATANAMENUM, str, szFilePath);

	for( int i = 0; i < mSelectDataList.size(); i++ )
	{
		CString	key;

		// �Ď�����ۑ�
		key.Format(KEY_DATANAME, i + 1);
		WritePrivateProfileString(SECTION_DATALIST, key, mSelectDataList[i].name, szFilePath);
	}
}

/*============================================================================*/
/*! �f�[�^���W

-# �Ď������X�g�𕜌�

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::ReadDataNameList(CString szFilePath)
{
	// �Ď����̓o�^�����擾
	int	n = GetPrivateProfileInt(SECTION_DATALIST, KEY_DATANAMENUM, 0, szFilePath);
	n = (n > MAX_DATASELECT) ? MAX_DATASELECT : n;

	mSelectDataList.clear();
	mSelName2Id.clear();
	mSelId2Name.clear();
	mSelId2Index.clear();

	for( int i = 0; i < n; i++ )
	{
		CString	key;
		TCHAR	name[1024];
		stSelectData	data;

		// �Ď������擾
		key.Format(KEY_DATANAME, i + 1);
		GetPrivateProfileString(SECTION_DATALIST, key, _T(""), name, _countof(name), szFilePath);

		// �Ď������X�g�ɒǉ�
		data.name = name;
		if( data.name.IsEmpty() )
			continue;
		data.id = ConvNameToId(name);
		mSelectDataList.push_back(data);
		mSelName2Id.insert(map<CString, DWORD>::value_type(data.name, data.id));
		mSelId2Name.insert(map<DWORD, CString>::value_type(data.id, data.name));
		mSelId2Index.insert(map<DWORD, int>::value_type(data.id, i));
	}

	// �X�e�[�^�X�����񃊃X�g���쐬
	CreateStatusList();
}

/*============================================================================*/
/*! �f�[�^���W

-# �I���f�[�^�̃f�[�^�擾�Ώۃt���O��ݒ肷��

@param

@retval
*/
/*============================================================================*/
void CDataCollection::SetSelectDataValid()
{
	int	i, j, k;

	// �f�[�^�o�^���X�g���猟�o
	for( i = 0; i < mSelectDataList.size(); i++ )
	{
		bool	valid = false;

		// �r���[�\�����X�g���猟�o
		for( j = 0; j < MAX_VIEW; j++ )
		{
			// �r���[��\���Ȃ�X���[
			if( !((CMainFrame *)theApp.m_pMainWnd)->IsExistChildList(j) )
				continue;

			CPropertiesData& prop = theApp.GetPropertiesData(j);

			for( k = 0; k < MAX_TDATA; k++ )
			{
				// �f�[�^ID���m�F
//				if (mSelectDataList[i].id == theApp.GetDataCollection().ConvNameToId(prop.item[k].dataName))
				if (mSelectDataList[i].name == prop.item[k].dataName)
				{
					valid = true;
					break;
				}
			}

			if( valid )
				break;
		}

		// �f�[�^�擾�Ώۃt���O��ݒ�
		mSelectDataList[i].valid = valid;
	}
}

/*============================================================================*/
/*! �T�[�o�[/�N���C�A���g

-# Unix���t����Windows���t�ւ̕ϊ�

@param		unixTime	Unix����
@param		winTime		Windows����

@retval

*/
/*============================================================================*/
void CDataCollection::ConvertTime(const stServerTime* unixTime, SYSTEMTIME* winTime)
{
	CTime time(unixTime->wYear, 1, 1, 0, 0, 0);
	CTimeSpan ts(unixTime->wDay - 1/*0=1��1���Ȃ̂�*/, 0, 0, 0);
	CTime at = time + ts;
	//CString s = at.Format(_T("%Y/%m/%d"));
	at.GetAsSystemTime(*winTime);
	winTime->wHour = unixTime->wHour;
	winTime->wMinute = unixTime->wMinute;
	winTime->wSecond = unixTime->wSecond;
	winTime->wMilliseconds = unixTime->wMilliseconds;
}
/*============================================================================*/
/*! �T�[�o�[/�N���C�A���g

-# Windows���t����Unix���t�ւ̕ϊ�

@param		winTime		Windows����
@param		unixTime	Unix����

@retval

*/
/*============================================================================*/
void CDataCollection::ConvertTime(const SYSTEMTIME* winTime, stServerTime* unixTime)
{
	CTime time1(winTime->wYear, 1, 1, 0, 0, 0);
	CTime time2(*winTime);
	CTimeSpan ts = time2 - time1;
	LONGLONG days = ts.GetDays() + 1;
	unixTime->wYear = winTime->wYear;
	unixTime->wDay = (WORD)days;
	unixTime->wHour = winTime->wHour;
	unixTime->wMinute = winTime->wMinute;
	unixTime->wSecond = winTime->wSecond;
	unixTime->wMilliseconds = winTime->wMilliseconds;
}
/*============================================================================*/
/*! �T�[�o�[/�N���C�A���g

-# �H�w�l���Ԃ���Unix���t�ւ̕ϊ�

@param		montime		�H�w�l����
@param		unixTime	Unix����

@retval

*/
/*============================================================================*/
void CDataCollection::ConvertTime(const long double montime, SYSTEMTIME* winTime)
{
	time_t tm = (time_t)montime;
	CTime time(tm);
	time.GetAsSystemTime(*winTime);

	//SYSTEMTIME st;
	//time.GetAsSystemTime(st);

	//CTime time1(st.wYear, 1, 1, 0, 0, 0);
	//CTimeSpan ts = time - time1;
	//LONGLONG days = ts.GetDays() + 1;

	//unixTime->wYear = st.wYear;
	//unixTime->wDay = (WORD)days;
	//unixTime->wHour = st.wHour;
	//unixTime->wMinute = st.wMinute;
	//unixTime->wSecond = st.wSecond;
	//unixTime->wMilliseconds = st.wMilliseconds;
}

/*============================================================================*/
/*! �T�[�o�[/�N���C�A���g

-# �H�w�l���Ԃ���Unix���t�ւ̕ϊ�

@param		montime		�H�w�l����
@param		unixTime	Unix����

@retval

*/
/*============================================================================*/
void CDataCollection::OutputCsvFile(int nView, CString sFilePath)
{
	CStdioFile	file;
	UINT	max_count;
	CString	sCsvDelimiter = _T(",");
	CString	sInvalid = _T("");

	// �v���p�e�B�f�[�^
	CPropertiesData& prop = theApp.GetPropertiesData(nView);

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("OutputCsvFile"), _T("Start"), _T("All"), nLogEx::debug);

	try
	{
		// �t�@�C���I�[�v��
		if( file.Open(sFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite) )
		{
			CString	sCommon;
			CString	sRecord;

			// All�̃w�b�_����CSV�ɏo��
			{
				// ���ʕ�����ݒ�
				sCommon = _T("No");
				sCommon += sCsvDelimiter;
				sCommon += _T("Date");
				sCommon += sCsvDelimiter;
				sCommon += _T("Time");

				sRecord.Empty();
				max_count = 0;
				for( int i = 0; i < MAX_TDATA; i++ )
				{
					CString	str;

					// ���R�[�h������ݒ�
					str.Format(_T("%s(%s) Min[%s]"), prop.item[i].dataGName, prop.item[i].dataName, (LPCTSTR)prop.item[i].dataUnit);
					sRecord += sCsvDelimiter;
					sRecord += str;
					str.Format(_T("%s(%s) Max[%s]"), prop.item[i].dataGName, prop.item[i].dataName, (LPCTSTR)prop.item[i].dataUnit);
					sRecord += sCsvDelimiter;
					sRecord += str;

					CGraphMutex::Lock_Analysis(nView);

					vector<stSpaceData>&	data = GetTrendAllData(nView, i);

					// �ő�f�[�^�����擾
					if( max_count < (UINT)data.size() )
						max_count = (UINT)data.size();

					CGraphMutex::Unlock_Analysis(nView);
				}

				file.WriteString(sCommon + sRecord + _T("\n"));
			}

			// All�̃f�[�^����CSV�ɏo��
			{
				for( UINT index = 0; index < max_count; index++ )
				{
					sCommon.Empty();
					sRecord.Empty();
					for( int i = 0; i < MAX_TDATA; i++ )
					{
						CString	str;

						CGraphMutex::Lock_Analysis(nView);

						vector<stSpaceData>&	data = GetTrendAllData(nView, i);

						if( (index < (UINT)data.size()) )
						{
							// ���ʕ�����ݒ�
							if( (data[index].systime.wYear != 0) && sCommon.IsEmpty() )
							{
								str.Format(_T("%d"), index + 1);
								sCommon = str;
								str.Format(_T("%04d/%02d/%02d"), data[index].systime.wYear, data[index].systime.wMonth, data[index].systime.wDay);
								sCommon += sCsvDelimiter;
								sCommon += str;
								str.Format(_T("%02d:%02d:%02d"), data[index].systime.wHour, data[index].systime.wMinute, data[index].systime.wSecond);
								sCommon += sCsvDelimiter;
								sCommon += str;
							}

							if( data[index].status & SDS_VALID )
							{
								// ���R�[�h������ݒ�
								str.Format(_T("%.6f"), data[index].mmPair.min);
								sRecord += sCsvDelimiter;
								sRecord += str;
								str.Format(_T("%.6f"), data[index].mmPair.max);
								sRecord += sCsvDelimiter;
								sRecord += str;
							}
							else
							{
								// ���R�[�h������ݒ�
								sRecord += sCsvDelimiter;
								sRecord += sInvalid;
								sRecord += sCsvDelimiter;
								sRecord += sInvalid;
							}
						}
						else
						{
							// ���R�[�h������ݒ�
							sRecord += sCsvDelimiter;
							sRecord += sInvalid;
							sRecord += sCsvDelimiter;
							sRecord += sInvalid;
						}

						CGraphMutex::Unlock_Analysis(nView);
					}

					if( !sCommon.IsEmpty() )
						file.WriteString(sCommon + sRecord + _T("\n"));
				}
			}

			// �t�@�C���N���[�Y
			file.Close();
		}
	}
	catch (CFileException* e)
	{
		CString	sError;
		sError.Format(_T("GetLastError = %d"), ::GetLastError());
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("OutputCsvFile"), _T("File Exception"), sError, nLogEx::error);

		e->Delete();
	}
	catch (...)
	{
		CString	sError;
		sError.Format(_T("GetLastError = %d"), ::GetLastError());
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("OutputCsvFile"), _T("Exception"), sError, nLogEx::error);
	}

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("OutputCsvFile"), _T("End"), _T("All"), nLogEx::debug);

	// Zoom��CSV�t�@�C�������쐬
	CString	sFilePath_Zoom = theApp.AddFileName(sFilePath, _T("_Zoom"));

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("OutputCsvFile"), _T("Start"), _T("Zoom"), nLogEx::debug);

	try
	{
		// �t�@�C���I�[�v��
		if( file.Open(sFilePath_Zoom, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite) )
		{
			CString	sCommon;
			CString	sRecord;

			// Zoom�̃w�b�_����CSV�ɏo��
			{
				// ���ʕ�����ݒ�
				sCommon = _T("No");
				sCommon += sCsvDelimiter;
				sCommon += _T("Date");
				sCommon += sCsvDelimiter;
				sCommon += _T("Time");

				sRecord.Empty();
				max_count = 0;
				for( int i = 0; i < MAX_TDATA; i++ )
				{
					CString	str;

					// ���R�[�h������ݒ�
					str.Format(_T("%s(%s) Min[%s]"), prop.item[i].dataGName, prop.item[i].dataName, (LPCTSTR)prop.item[i].dataUnit);
					sRecord += sCsvDelimiter;
					sRecord += str;
					str.Format(_T("%s(%s) Max[%s]"), prop.item[i].dataGName, prop.item[i].dataName, (LPCTSTR)prop.item[i].dataUnit);
					sRecord += sCsvDelimiter;
					sRecord += str;

					CGraphMutex::Lock_Analysis(nView);

					vector<stSpaceData>&	data = GetTrendZoomData(nView, i);

					// �ő�f�[�^�����擾
					if( max_count < (UINT)data.size() )
						max_count = (UINT)data.size();

					CGraphMutex::Unlock_Analysis(nView);
				}

				file.WriteString(sCommon + sRecord + _T("\n"));
			}

			// Zoom�̃f�[�^����CSV�ɏo��
			{
				for( UINT index = 0; index < max_count; index++ )
				{
					sCommon.Empty();
					sRecord.Empty();
					for( int i = 0; i < MAX_TDATA; i++ )
					{
						CString	str;

						CGraphMutex::Lock_Analysis(nView);

						vector<stSpaceData>&	data = GetTrendZoomData(nView, i);

						if( (index < (UINT)data.size()) )
						{
							// ���ʕ�����ݒ�
							if( (data[index].systime.wYear != 0) && sCommon.IsEmpty() )
							{
								str.Format(_T("%d"), index + 1);
								sCommon = str;
								str.Format(_T("%04d/%02d/%02d"), data[index].systime.wYear, data[index].systime.wMonth, data[index].systime.wDay);
								sCommon += sCsvDelimiter;
								sCommon += str;
								str.Format(_T("%02d:%02d:%02d"), data[index].systime.wHour, data[index].systime.wMinute, data[index].systime.wSecond);
								sCommon += sCsvDelimiter;
								sCommon += str;
							}

							if( data[index].status & SDS_VALID )
							{
								// ���R�[�h������ݒ�
								str.Format(_T("%.6f"), data[index].mmPair.min);
								sRecord += sCsvDelimiter;
								sRecord += str;
								str.Format(_T("%.6f"), data[index].mmPair.max);
								sRecord += sCsvDelimiter;
								sRecord += str;
							}
							else
							{
								// ���R�[�h������ݒ�
								sRecord += sCsvDelimiter;
								sRecord += sInvalid;
								sRecord += sCsvDelimiter;
								sRecord += sInvalid;
							}
						}
						else
						{
							// ���R�[�h������ݒ�
							sRecord += sCsvDelimiter;
							sRecord += sInvalid;
							sRecord += sCsvDelimiter;
							sRecord += sInvalid;
						}

						CGraphMutex::Unlock_Analysis(nView);
					}

					if( !sCommon.IsEmpty() )
						file.WriteString(sCommon + sRecord + _T("\n"));
				}
			}

			// �t�@�C���N���[�Y
			file.Close();
		}
	}
	catch (CFileException* e)
	{
		CString	sError;
		sError.Format(_T("GetLastError = %d"), ::GetLastError());
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("OutputCsvFile"), _T("File Exception Zoom"), sError, nLogEx::error);

		e->Delete();
	}
	catch (...)
	{
		CString	sError;
		sError.Format(_T("GetLastError = %d"), ::GetLastError());
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("OutputCsvFile"), _T("Exception Zoom"), sError, nLogEx::error);
	}

	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("OutputCsvFile"), _T("End"), _T("Zoom"), nLogEx::debug);
}
