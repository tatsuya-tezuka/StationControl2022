// DataCollection.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "DataCollection.h"
#include <locale.h>
#include "EngValCnv.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// CDataCollection

CDataCollection::CDataCollection()
{
	mpCollectThread = NULL;

	for (int i = 0; i < sizeof(m_angle) / sizeof(m_angle[0]); ++i) {
		m_angle[i] = rand() * 360 / RAND_MAX;
	}
	for (int i = 0; i < eStation_MAX; i++)
	{
		mDataNameList[i].clear();
	}

	for (int i = 0; i < eStation_MAX; i++)
	{
		mGraphSigIndexNum[i] = 0;
	}

#ifdef _NOCOMM
	mOperationHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mControlHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mEquipmentHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mEventHistFile.m_hFile = INVALID_HANDLE_VALUE;
#endif

	m_nHistID54 = 0;
//#ifdef _DEBUG
//	m_shortCycleTime = 0;
//	m_integtime = 0;
//#endif
}

CDataCollection::~CDataCollection()
{
}


/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^ID�̎擾

@param
@retval

*/
/*============================================================================*/
void CDataCollection::SetKanshiName()
{
	// �[���ԍ��擾����
	CString strTermNo;
	UINT nTermNo = theApp.GetTermNo();
	if (nTermNo > 0)
		nTermNo = nTermNo - 1;
	strTermNo.Format(_T("%02u"), nTermNo);

	int i, j, k;
	int index[eStation_MAX];
	// �q���ߑ��p�̋�.�ݔ�.���ږ������X�g�ɒǉ�
	for (i = 0; i < eStation_MAX; i++)
	{
		index[i] = 1;
		for (j = 0; j < eTrendWndType_Num; j++)
		{
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (mListSatelliteCaptureSig[i][j][k].strSigName.IsEmpty())
					break;
				if (mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_AGC_XRX2_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(UDSC64_AGC_XXTCR_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(XDSC54_AGC_DSNMODEM_INTEG_SIG) == 0 ||
					mListSatelliteCaptureSig[i][j][k].strSigName.Compare(USC34_AGC_INTEG_SIG) == 0)
				{
					// �Ǔ��̏ꏊ�ɉ����ĊĎ����̖�����ς���
					mListSatelliteCaptureSig[i][j][k].strSigName = ReplaceAreaKanshiName(mListSatelliteCaptureSig[i][j][k].strSigName) + strTermNo;
				}
				mDataNameList[i].insert(map<DWORD, CString>::value_type((j << 16) | k, mListSatelliteCaptureSig[i][j][k].strSigName));

				// �O���t�̊Ď��}�b�v�C���f�b�N�X�ɊĎ��������Z�b�g�Ȃ�΃C���f�b�N�X�Z�b�g����
				if (mGraphSigIndexMap[i][mListSatelliteCaptureSig[i][j][k].strSigName] == 0)
				{
					mGraphSigIndexMap[i][mListSatelliteCaptureSig[i][j][k].strSigName] = index[i];
					index[i]++;
				}
			}
		}
	}

	int pre_j = j;
	int pre_k = k;

	// �T�}���p�̋�.�ݔ�.���ږ������X�g�ɒǉ�
	for (i = 0; i < eStation_MAX; i++)
	{
		for (j = 0; j < eSummaryWndSetType_Num; j++)
		{
			for (k = 0; k < MAX_TDATA; k++)
			{
				if (mListSummarySig[i][j][k].strSigName.IsEmpty())
					break;
				mDataNameList[i].insert(map<DWORD, CString>::value_type(((j + pre_j) << 16) | (k + pre_k), mListSummarySig[i][j][k].strSigName));

				// �O���t�̊Ď��}�b�v�C���f�b�N�X�ɊĎ��������Z�b�g�Ȃ�΃C���f�b�N�X�Z�b�g����
				if (mGraphSigIndexMap[i][mListSummarySig[i][j][k].strSigName] == 0)
				{
					mGraphSigIndexMap[i][mListSummarySig[i][j][k].strSigName] = index[i];
					index[i]++;
				}
			}
		}
	}

	for (i = 0; i < eStation_MAX; i++)
	{
		mGraphSigIndexNum[i] = (int)mGraphSigIndexMap[i].size();
	}
}


/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^ID�̎擾

@param
@retval

*/
/*============================================================================*/
DWORD CDataCollection::ConvertNameToId(UINT nStation, CString name)
{
	map<DWORD, CString>::iterator itr;
	for (itr = mDataNameList[nStation].begin(); itr != mDataNameList[nStation].end(); itr++){
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
CString CDataCollection::ConvertIdToName(UINT nStation, DWORD id)
{
	map<DWORD, CString>::iterator itr = mDataNameList[nStation].find(id);
	if (itr == mDataNameList[nStation].end())
		return _T("");
	return (*itr).second;
}


/*============================================================================*/
/*! �f�[�^���W

-# �T�[�o�f�[�^���A�v�����f�[�^�ɍč쐬

@param	lpData		�T�[�o�f�[�^
@param	dwSize		�o�b�t�@�T�C�Y
@param	nStation	�ǔԍ�
@retval void
*/
/*============================================================================*/
void CDataCollection::CreateSpaceData(LPBYTE lpData, DWORD dwSize, UINT nStation)
{
	CGraphMutex::Lock(eData);

	try
	{
		stServerItemData *pdata = (stServerItemData*)(lpData + sizeof(stCommandHeader));
		dwSize -= sizeof(stCommandHeader);
		CSpaceData		data;
		stSpaceData		sData;
		int cnt = 0;
		for (DWORD i = 0; i + 1 < dwSize; i += sizeof(stServerItemData), pdata++)
		{
			cnt++;
			data.value.clear();
			memset(&sData, 0, sizeof(stSpaceData));
			CString strDataName = CString(pdata->name);
//			data.dataid = ConvertNameToId(nStation, strDataName);
			data.dataid = mGraphSigIndexMap[nStation][strDataName];
			if (data.dataid == 0)
				continue;
			data.dataname = strDataName;
			data.kind = pdata->kind;
			data.srcType = eSigSrcType_EngExchange;
			data.totalSize = 1;
			CCommBase::ConvertTime(&pdata->systime, &sData.systime);
			sData.idx = 0;
			sData.value = pdata->value;
			memcpy(sData.status, pdata->status, 8);
			sData.ppPair.max = sData.value;
			sData.ppPair.min = sData.value;
			sData.ppPair.count = 1;
			data.value.push_back(sData);
			// �f�[�^���ꎞ�I�Ɋi�[
			if (data.value.empty() == FALSE)
			{
				mSpaceTemp[nStation].push_back(data);
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

	CGraphMutex::Unlock(eData);
}


/*============================================================================*/
/*! �f�[�^���W

-# �T�[�o�f�[�^���A�v�����f�[�^�ɍč쐬

@param	data		�T�[�o�f�[�^
@param	nStation	�ǔԍ�
@retval void
*/
/*============================================================================*/
void CDataCollection::CreateForecastData(stServerItemData* pdata, UINT nStation)
//#ifndef _ENGVALCNV_TREND
{
	CGraphMutex::Lock(eData);

	try
	{
		CSpaceData		data;
		stSpaceData		sData;
		data.value.clear();
		memset(&sData, 0, sizeof(stSpaceData));
		CString strDataName = CString(pdata->name);
//		data.dataid = ConvertNameToId(nStation, strDataName);
//		if (data.dataid != 0xffffffff)
		data.dataid = mGraphSigIndexMap[nStation][strDataName];
		if (data.dataid != 0)
		{
			data.dataname = strDataName;
			data.kind = pdata->kind;
			data.srcType = eSigSrcType_File;
			data.totalSize = 1;
			CCommBase::ConvertTime(&pdata->systime, &sData.systime);
			sData.idx = 0;
			sData.value = pdata->value;
			memcpy(sData.status, pdata->status, 8);
			sData.status[0] = 0;
			sData.ppPair.max = sData.value;
			sData.ppPair.min = sData.value;
			//			sData.ppPair.count = 0;
			sData.ppPair.count = 1;
			data.value.push_back(sData);
			// �f�[�^���ꎞ�I�Ɋi�[
			if (data.value.empty() == FALSE)
			{
				mSpaceTemp[nStation].push_back(data);
#ifdef _DEBUG
				{
					CTime ctime(sData.systime);
					CString msg;
					msg.Format(_T("add dataid : %s(%u) value(%f) at time(%I64d)"), (LPCTSTR)CString(pdata->name), data.dataid, sData.value, ctime.GetTime());
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateForecastData"), msg, _T("debug"), nLogEx::debug);
				}
#endif
			}
		}
#if _DEBUG
		else
		{
			CString msg;
			msg.Format(_T("dataid error : %s"), (LPCTSTR)CString(pdata->name));
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateForecastData"), msg, _T("debug"), nLogEx::debug);
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}


/*============================================================================*/
/*! �f�[�^���W

-# �������̍쐬

@param	pUtf8Str			UTF-8

@retval	CString
*/
/*============================================================================*/
CString CDataCollection::UTF8_3CharsToSJis(char *pUtf8Str)
{
	wchar_t pUtfMsg[4];

	if (pUtf8Str[0] >= 0x00 && pUtf8Str[0] <= 0x7f){
		// 1�o�C�g����
		return _T("");
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
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("Code Error : err(%d), 0x%02X 0x%02X 0x%02X"), err, pUtf8Str[0] & 0xFF, pUtf8Str[1] & 0xFF, pUtf8Str[2] & 0xFF);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("UTF8_3CharsToSJis"), msg, _T("detail"), nLogEx::detail);
		//������������������������ Log ������������������������//
		//=====================================================//
		return _T("");
	}

	CString str(sjis);
	return str;
}
#ifdef _DEBUG
//#define _DEBUG_HISTORY 1
#endif
/*============================================================================*/
/*! �f�[�^���W

-# �������̍쐬

@param	pbuf		�T�[�o�[�����f�[�^

@retval
*/
/*============================================================================*/
void CDataCollection::CreateDataToHistory(PIPEMONITORACK* pbuf)
{
	DWORD	dwStart = timeGetTime();

	stHistoryData data;
	memset(&data, 0, sizeof(stHistoryData));

	// �Y���ǂ𒲂ׂ�
	char* ptr = (char*)pbuf->buf;
	ptr += 0x0C;	// ���M������
	DWORD nStation = *((DWORD*)ptr);
	if (nStation == 0x010a0102) // UDSC64
		data.station = 0;
	else if (nStation == m_nHistID54) // XDSC54
		data.station = 1;
	else if (nStation == 0x010a0203) // USC34
		data.station = 2;
	else if (nStation == 0x010a0103) // USC20
		data.station = 3;
	else{
		if (nStation == 0) // �}�g�Ǔ�
			data.station = (BYTE)theApp.GetSelectStation();
	}

	ptr = (char*)pbuf->buf;
	CEngValCnv::sdtp_msg_ex_t* pmsg = (CEngValCnv::sdtp_msg_ex_t*)pbuf->buf;

#ifdef _DEBUG_HISTORY
	const char sdtp[109] = {
		/*0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x89, 0x00, 0x00, 0x00, 0xB0, 0x30, 0x00, 0x01,
		0x01, 0x00, 0x00, 0x00, 0x09, 0x80, 0x05, 0x00,
		0x04, 0x00, 0x02, 0x00,*/ 0x41, 0x00, 0x00, 0x00,
		0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B,
		0x02, 0x01, 0x0A, 0x01, 0x00, 0x00, 0x02, 0x01,
		0x04, 0x01, 0x00, 0x00, 0x10, 0x03, 0x6D, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
		0x32, 0x30, 0x31, 0x37, 0x2D, 0x30, 0x33, 0x2D,
		0x31, 0x30, 0x20, 0x30, 0x35, 0x3A, 0x34, 0x31,
		0x3A, 0x33, 0x32, 0x20, 0x43, 0x30, 0x20, 0x28,
		0x30, 0x30, 0x30, 0x30, 0x39, 0x29, 0x20, 0x64,
		0x69, 0x20, 0xE3, 0x83, 0x86, 0xE3, 0x82, 0xB9,
		0xE3, 0x83, 0x88, 0xE3, 0x83, 0xA1, 0xE3, 0x83,
		0x83, 0xE3, 0x82, 0xBB, 0xE3, 0x83, 0xBC, 0xE3,
		0x82, 0xB8, 0xE3, 0x81, 0xA7, 0xE3, 0x81, 0x99,
		0x00,
	};
	memcpy(ptr, (char*)&sdtp[0], 109);
#endif

	CString strDate, strTime, strSend, strMsgid, strLevel, strMsg;

	ptr += 0x2C;

//	// 54m�ǈȊO�̏ꍇ�i�����[�g���j
//	if (data.station != eStation_Usuda642)
//	{
//		CString buf;
//		int item = 0;
//		CString temp;
//		buf.Empty();
//		while (1){
//			if (*ptr == 0x00){
//				strMsg = buf;
//				break;
//			}
//			if (*ptr == 0x20 && item < 5){
//				// ��̃A�C�e��������
//				switch (item){
//				case	0:
//					strDate = buf;
//					break;
//				case	1:
//					strTime = buf;
//					break;
//				case	2:
//					strSend = buf;
//					break;
//				case	3:
//					strMsgid = buf;
//					break;
//				case	4:
//					strLevel = buf;
//					break;
//				case	5:
//					strMsg = buf;
//					break;
//				}
//				ptr++;
//				item++;
//				buf.Empty();
//				continue;
//			}
//			if (*ptr >= 0x00 && *ptr <= 0x7F){
//				// ASCII�R�[�h
//				buf += CString(char(*ptr));
//				ptr++;
//			}
//			else if ((*ptr & 0xE0) == 0xE0){
//				// ����
//				buf += UTF8_3CharsToSJis(ptr);
//				ptr += 3;
//			}
//			else{
//				// ���̑�
//				//=====================================================//
//				//������������������������ Log ������������������������//
//				CString msg;
//				msg.Format(_T("Code Error : 0x%02X"), *ptr & 0xFF);
//				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("detail"), nLogEx::detail);
//				//������������������������ Log ������������������������//
//				//=====================================================//
//				return;
//			}
//		}
//	}
//	// 54m�ǂ̏ꍇ�iGREAT�j
//	else
	{
		//�d����胁�b�Z�[�W�����擾����(4ByteLSB) �o�b�t�@��16384BYTE�������̂ŃJ�b�g
		unsigned int msglen = ((BYTE)ptr[0x28] + ((BYTE)ptr[0x29] << 8) + ((BYTE)ptr[0x2A] << 16) + ((BYTE)ptr[0x2B] << 24)) % 0x4000;

		std::string stdbuf;
		int item = 0;
		CString temp;
		// ���b�Z�[�W���������b�Z�[�W���ɂȂ�܂ŌJ��Ԃ�
		// ���b�Z�[�W�������F2015-08-23 00:25:41 u6opr1 (5052) di CTRL.PRIV_STTN,ON,,ANS:OK
		// �@���t�@�A���ԁ@�B�ݔ��@�C���b�Z�[�WID�@�D��ԁ@�E���b�Z�[�W������
		while (1){
			if (msglen < 1 || *ptr == 0x00){
				// �f�[�^�t�B�[���h���I������̂ŕϊ���������Z�b�g���ďI��
				// ���b�Z�[�W������̃R�[�h�ϊ�
				strMsg = ConvUtf8Unicode(stdbuf);
				break;
			}
			// ��؂蕶���u0x20�v����������A�ϊ�������������A�C�e�����ɃZ�b�g����
			if (*ptr == 0x20 && item < 5){
				// ��̃A�C�e��������
				switch (item){
				case	0:	strDate = stdbuf.c_str();		break;
				case	1:	strTime = stdbuf.c_str();		break;
				case	2:	strSend = stdbuf.c_str();		break;
				case	3:	strMsgid = stdbuf.c_str();		break;
				case	4:	strLevel = stdbuf.c_str();		break;
				}
				ptr++;
				msglen--;
				item++;
				stdbuf = "";
				continue;
			}
			// ������A������
			stdbuf += char(*ptr);
			ptr++;
			msglen--;
		}
	}

	CString str;
	int i = 0;
	// ���t
	i = 0;
	AfxExtractSubString(str, strDate, i++, '-');
	data.year = _wtoi(str);
	AfxExtractSubString(str, strDate, i++, '-');
	data.month = _wtoi(str);
	AfxExtractSubString(str, strDate, i++, '-');
	data.day = _wtoi(str);
	// ����
	i = 0;
	AfxExtractSubString(str, strTime, i++, ':');
	data.hour = _wtoi(str);
	AfxExtractSubString(str, strTime, i++, ':');
	data.minute = _wtoi(str);
	AfxExtractSubString(str, strTime, i++, ':');
	data.second = _wtoi(str);

	// ���M��
	typedef struct {
		CString		name;
		UINT		code;
	} stSender;
	const stSender mSender[] = {
		{ _T("e1"), eSenderItem1 },	// �o��Ǒ��ǉ^�p�ǐ����u
		{ _T("e2"), eSenderItem1 },	// ���͌����ǉ^�p�ǐ����u
		{ _T("e3"), eSenderItemMax },	// �}�g�ǉ^�p�ǐ����u
		{ _T("c0"), eSenderItem2 },	// �ݔ����䑕�u
		{ _T("85"), eSenderItem3 },	// S��TLM���͑��u
		{ _T("86"), eSenderItem3 },	// X��TLM���͑��u
		{ _T("87"), eSenderItem4 },	// S�уR�}���h�o�͑��u
	};
#ifdef _DEBUG_HISTORY
	data.equipment = (BYTE)(rand() % (eSenderItemMax+1));
#else
	strSend.MakeLower();
	data.equipment = eSenderItemMax+1;
	for (int i = 0; i < sizeof(mSender) / sizeof(mSender[0]); i++){
		if (strSend == mSender[i].name){
			data.equipment = mSender[i].code;
			break;
		}
	}
	if (data.equipment == (eSenderItemMax+1)){
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return;
	}
#endif
	if (theApp.GetPipeIgnore() & 0x04){
		if (data.equipment == eSenderItemMax){
			return;
		}
	}

	// ���b�Z�[�WID
#ifdef _DEBUG_HISTORY
	data.message = (DWORD)((rand() % 7) * 10000 + (rand() % 10000));
//	data.message = (DWORD)40000;
#else
	data.message = _wtoi(strMsgid.Mid(1, strMsgid.GetLength() - 2));
#endif

	// ���b�Z�[�W������
	wsprintf(data.msgstring, _T("%s"), (LPCTSTR)strMsg);
#ifdef _DEBUG_HISTORY
	if ((rand() % 5) == 0){
		wsprintf(data.msgstring, _T("[�C�x���g���s]%s"), (LPCTSTR)strMsg);
	}

#endif

	bool bLimit = false;
	if (data.message >= 30000 && data.message <= 39999 ||
		data.message >= 40000 && data.message <= 49999){
		bLimit = IsMessageLimit(data.message);
	}

	// �X�e�[�^�X
	if (data.message >= 30000 && data.message <= 39999 ||
		data.message >= 40000 && data.message <= 49999)
	{
		bool bStatus = false;
		// ��ԑJ�ڗ���
		// ���~�b�g�G���[����
		switch (data.message%10){
		case	1:/*POWER ON/OFF*/
			data.status = 4;
			break;
		case	2:/*REMOTE/LOCAL*/
			data.status = 3;
			break;
		case	3:/*MAINT/NORMAL*/		
			data.status = 3;		// REMOTE/LOCAL�Ɋ܂߂�
			break;
		case	4:/*WARNING*/
			data.status = 1;
			break;
		case	5:/*FAULT*/
			data.status = 2;
			break;
		case	6:/*FAULT*/
			data.status = 2;
			break;
		case	7:/*STATUS*/
			data.status = 0;
			break;
		case	8:/*����*/
			data.status = 0;
			data.imstatus = 0;
			bStatus = true;
			break;
		default:
			if (bLimit == true){
				data.status = 0;
				data.imstatus = 0;
				bStatus = true;
				break;
			}
			//=====================================================//
			//������������������������ Log ������������������������//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
			return;
		}
		if (bStatus == false)
		{
#ifdef _DEBUG_HISTORY
			data.imstatus = (BYTE)(rand() % 3);
#else
			if (strLevel == _T("di"))
				data.imstatus = 0;
			else if (strLevel == _T("dw"))
				data.imstatus = 1;
			else if (strLevel == _T("ds"))
				data.imstatus = 2;
			else if (strLevel == _T("dg"))
				data.imstatus = 0;
			else{// if (strLevel == _T("lo")){
				//=====================================================//
				//������������������������ Log ������������������������//
				CString msg;
				msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
				//������������������������ Log ������������������������//
				//=====================================================//
				return;
			}
#endif
		}
	}
	else
	{
		// ��L�ȊO
		data.imstatus = 0;
#ifdef _DEBUG_HISTORY
		data.status = (BYTE)(rand() % 3);
#else
		if (strLevel == _T("di"))
			data.status = 0;
		else if (strLevel == _T("dw"))
			data.status = 1;
		else if (strLevel == _T("ds"))
			data.status = 2;
		else if (strLevel == _T("dg"))
			data.status = 0;
		else{// if (strLevel == _T("lo")){
			//=====================================================//
			//������������������������ Log ������������������������//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
			return;
		}
#endif
	}

	bool bOpeMsg = false;
	if (data.message >= 0 && data.message <= 29999){
		// �^�p����
		bOpeMsg = true;
	}

#ifdef _DEBUG_HISTORY
	data.station = (BYTE)(rand() % eStation_MAX);
	if (data.equipment == eSenderItemMax){
		data.station = (BYTE)0;
	}
#else
	if (nStation == 0x010a0102) {// UDSC64
		// �֐��`���ŋǔԃZ�b�g�ς�
	}
	else if (nStation == m_nHistID54) {// XDSC54
		// �֐��`���ŋǔԃZ�b�g�ς�
	}
	else if (nStation == 0x010a0203) {// USC34
		// �֐��`���ŋǔԃZ�b�g�ς�
	}
	else if (nStation == 0x010a0103) {// USC20
		// �֐��`���ŋǔԃZ�b�g�ς�
	}
	else{
		if (bOpeMsg == true){
			// �^�p�����̏ꍇ�͒}�g�Ǔ��̃��b�Z�[�W�����݂��A�ǎ��ʎq���O
			// �֐��`���ŋǔԃZ�b�g�ς�
		}
		else{
			//=====================================================//
			//������������������������ Log ������������������������//
			CString msg;
			msg.Format(_T("%s %s %s %s %s 0x%08x %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, nStation, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
			return;
		}
	}
	if (data.equipment == eSenderItemMax){
//		data.station = (BYTE)0;
		data.station = (BYTE)theApp.GetSelectStation();
	}
#endif
	if (bOpeMsg == false){
		// �^�p�����ȊO�́A���M�����}�g�ǉ^�p�ǐ����u�͂��肦�Ȃ�
		if (data.equipment >= eSenderItemMax){
			//=====================================================//
			//������������������������ Log ������������������������//
			CString msg;
			msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
			CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
			//������������������������ Log ������������������������//
			//=====================================================//
			return;
		}
	}

	if (theApp.GetSelectStationMode() == eStation_1 && theApp.GetSelectStation() != data.station){
		// �Ώۂ̋ǂł͂Ȃ�
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("Station(%d) is not support"), (int)data.station);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T("Error"), nLogEx::error);
		//������������������������ Log ������������������������//
		//=====================================================//
		return;
	}


	bool bEvent = IsMessageEvent(data.msgstring);

	CGraphMutex::Lock(eData);
	if (bEvent == true){
		// �C�x���g����
		mEventHistoryTemp.push_back(data);
		mSubEventHistoryData.push_back(data);
	}
	else{
		if (data.message >= 0 && data.message <= 29999){
			// �^�p����
			mOperationHistoryTemp.push_back(data);
		}
		if (data.message >= 30000 && data.message <= 39999){
			if (bLimit == true){
				// ���~�b�g�G���[����
				//mEquipmentHistoryTemp.push_back(data);
				mSubEquipmentHistoryData.push_back(data);
			}
			else{
				// ��ԑJ�ڗ���
				mEquipmentHistoryTemp.push_back(data);
				//mSubEquipmentHistoryData.push_back(data);
			}
		}
		if (data.message >= 40000 && data.message <= 49999){
			// ���~�b�g�G���[����
			//mEquipmentHistoryTemp.push_back(data);
			mSubEquipmentHistoryData.push_back(data);
		}
		if (data.message >= 50000 && data.message <= 59999){
			// ���䗚��
			mControlHistoryTemp.push_back(data);
		}
		if (data.message >= 60000 && data.message <= 69999){
			// �C�x���g����
			mEventHistoryTemp.push_back(data);
			mSubEventHistoryData.push_back(data);
		}
	}
#ifdef _DEBUG
	//=====================================================//
	//������������������������ Log ������������������������//
	CString msg;
	msg.Format(_T("%s %s %s %s %s %s"), (LPCTSTR)strDate, (LPCTSTR)strTime, (LPCTSTR)strSend, (LPCTSTR)strMsgid, (LPCTSTR)strLevel, (LPCTSTR)strMsg);
	CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("CreateDataToHistory"), msg, _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
#endif

	DWORD dwElapsed = (timeGetTime() - dwStart);
//	TRACE("### CreateDataToHistory Elapsed(%d)\n", dwElapsed);
	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! �f�[�^���W

-# �����R�[�h�ϊ����s��

@param	src		�@�@UTF-8�e�L�X�g

@retval	Cstring		ShiftJIS(CP932)�e�L�X�g
*/
/*============================================================================*/
CString CDataCollection::ConvUtf8SjisEx(std::string const& src)
{
	//UTF-16�ϊ���̃T�C�Y���擾
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0U, src.data(), int(src.size()) + 1, nullptr, 0U);
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	//UTF-8����UTF-16�ֈ�U�ϊ�
	MultiByteToWideChar(CP_UTF8, 0, src.data(), int(src.size()) + 1, bufUnicode, lenghtUnicode);
	int lengthSJis = WideCharToMultiByte(932, 0U, bufUnicode, -1, nullptr, 0U, nullptr, nullptr);
	char* bufShiftJis = new char[lengthSJis];
	//UTF-16����Shift-JIS(CP932)�֕ϊ�
	WideCharToMultiByte(932, 0, bufUnicode, lenghtUnicode, bufShiftJis, lengthSJis, nullptr, nullptr);
	std::string strSJis(bufShiftJis);

	delete[] bufUnicode;
	delete[] bufShiftJis;

	return CString(&strSJis[0]);
}

/*============================================================================*/
/*! �f�[�^���W

-# �����R�[�h�ϊ����s��

@param	src		�@�@UTF-8�e�L�X�g

@retval	Cstring		UNICODE�e�L�X�g
*/
/*============================================================================*/
CString CDataCollection::ConvUtf8Unicode(std::string const& src)
{
	//UTF-16�ϊ���̃T�C�Y���擾
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0U, src.data(), int(src.size()) + 1, nullptr, 0U);
	if (lenghtUnicode <= 0)
	{
		//=====================================================//
		//������������������������ Log ������������������������//
		CString msg;
		msg.Format(_T("Code Error : MultiByteToWideChar Length(%d)"), lenghtUnicode);
		CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("ConvUtf8Unicode"), msg, _T("detail"), nLogEx::detail);
		//������������������������ Log ������������������������//
		//=====================================================//
		return _T("");
	}

	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	//UTF-8����UTF-16�֕ϊ�
	MultiByteToWideChar(CP_UTF8, 0, src.data(), int(src.size()) + 1, bufUnicode, lenghtUnicode);

	CString strUnicode(bufUnicode);
	delete[] bufUnicode;

	return strUnicode;
}

/*============================================================================*/
/*! �f�[�^���W

-# ���b�Z�[�W���e����C�x���g�������̃`�F�b�N���s��

@param	pmsg	���b�Z�[�W������

@retval bool
*/
/*============================================================================*/
bool CDataCollection::IsMessageEvent(TCHAR* pmsg)
{
	const CString mMessageEvent[] = {
		{ _T("[�C�x���g���s]") },
		{ _T("[�V�[�P���X�Ǘ�]") },
		{ _T("[TLM�^�p]") },
		{ _T("[CMD�^�p]") },
		{ _T("[RNG�^�p]") },
		{ _T("[�ϒ�����Ǘ�]") },
		{ _T("[UPLINK�^�p]") },
		{ _T("[����o�̓T�u]") },
		{ _T("[S-DL�ߑ�]") },
		{ _T("[X-DL�ߑ�]") },
		{ _T("[Ka-DL�ߑ�]") },
	};
	CString msg = CString(pmsg);
	for (int i = 0; i < sizeof(mMessageEvent) / sizeof(mMessageEvent[0]); i++){
		if (msg.Find(mMessageEvent[i]) >= 0){
			// �C�x���g�����̕����񂪌�������
			return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! �f�[�^���W

-# ���b�Z�[�W���e����C�x���g�������̃`�F�b�N���s��

@param	msgno	���b�Z�[�W�ԍ�

@retval	bool
*/
/*============================================================================*/
bool CDataCollection::IsMessageLimit(DWORD msgno)
{
	CString msg;
	msg.Format(_T("%d"), msgno);
	if (msg.Right(1) == _T("9")){
		return true;
	}
	return false;
}

/*============================================================================*/
/*! �f�[�^���W

-# �^�p�����f�[�^�ɍ쐬

���b�Z�[�WID�̋敪�͈ȉ��̂悤�ɂȂ�܂��B
0 �` 29999 �^�p����
30000 �` 39999 ��ԑJ�ڗ���
40000 �` 49999 ���~�b�g�G���[����
50000 �` 59999 ���䗚��
60000 �` 69999 �C�x���g����

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateOperationHistory()
{
	CGraphMutex::Lock(eData);

	try
	{
		stHistoryData data;
		SYSTEMTIME time;
		GetLocalTime(&time);
		data.year = time.wYear;
		data.month = (BYTE)time.wMonth;
		data.day = (BYTE)time.wDay;
		data.hour = (BYTE)time.wHour;
		data.minute = (BYTE)time.wMinute;
		data.second = (BYTE)time.wSecond;
		UINT val;
		val = sizeof(mStationString) / sizeof(mStationString[0]);
		data.station = (BYTE)(rand() % val);
		val = sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]);
		data.equipment = (BYTE)(rand() % val);
		BYTE sb = (BYTE)(rand() % 23);
		if (sb == 13)
			data.status = 1;
		else if (sb == 17)
			data.status = 2;
		else
			data.status = 0;
		data.message = (BYTE)(rand() % 30000);
		mOperationHistoryTemp.push_back(data);
#ifdef _NOCOMM
		CString str, cbuf, temp;
		cbuf.Empty();
		str.Format(_T("%04d-%02d-%02d"), data.year, data.month, data.day);
		cbuf += str;
		str.Format(_T(",%02d:%02d:%02d"), data.hour, data.minute, data.second);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mStationString[data.station]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistStatusString[data.status]);
		cbuf += str;
		str.Format(_T(",%d"), data.message);
		cbuf += str;
		str.Format(_T(",[### TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST ###"));
		cbuf += str;
		cbuf += _T("\n");
		if (mOperationHistFile.m_hFile != INVALID_HANDLE_VALUE){
			TCHAR* p = _wsetlocale(LC_ALL, _T(""));
			mOperationHistFile.WriteString(cbuf);
			_wsetlocale(LC_ALL, p);
			mOperationHistFile.Flush();
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! �f�[�^���W

-# ���䗚���f�[�^�ɍ쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateControlHistory()
{
	CGraphMutex::Lock(eData);

	try
	{
		stHistoryData data;
		SYSTEMTIME time;
		GetLocalTime(&time);
		data.year = time.wYear;
		data.month = (BYTE)time.wMonth;
		data.day = (BYTE)time.wDay;
		data.hour = (BYTE)time.wHour;
		data.minute = (BYTE)time.wMinute;
		data.second = (BYTE)time.wSecond;
		UINT val;
		val = sizeof(mStationString) / sizeof(mStationString[0]);
		data.station = (BYTE)(rand() % val);
		val = sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]);
		data.equipment = (BYTE)(rand() % val);
		BYTE sb = (BYTE)(rand() % 23);
		if (sb == 13)
			data.status = 1;
		else if (sb == 17)
			data.status = 2;
		else
			data.status = 0;
		data.message = (BYTE)(rand() % 10000)+50000;
		mControlHistoryTemp.push_back(data);
#ifdef _NOCOMM
		CString str, cbuf, temp;
		cbuf.Empty();
		str.Format(_T("%04d-%02d-%02d"), data.year, data.month, data.day);
		cbuf += str;
		str.Format(_T(",%02d:%02d:%02d"), data.hour, data.minute, data.second);
		cbuf += str;
		str.Format(_T(",%s"), mStationString[data.station]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistStatusString[data.status]);
		cbuf += str;
		str.Format(_T(",%d"), data.message);
		cbuf += str;
		str.Format(_T(",[### TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST ###"));
		cbuf += str;
		cbuf += _T("\n");
		if (mControlHistFile.m_hFile != INVALID_HANDLE_VALUE){
			TCHAR* p = _wsetlocale(LC_ALL, _T(""));
			mControlHistFile.WriteString(cbuf);
			_wsetlocale(LC_ALL, p);
			mControlHistFile.Flush();
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! �f�[�^���W

-# �ݔ��󋵗����f�[�^�ɍ쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateEquipmentHistory()
{
	CGraphMutex::Lock(eData);

	try
	{
		stHistoryData data;
		SYSTEMTIME time;
		GetLocalTime(&time);
		data.year = time.wYear;
		data.month = (BYTE)time.wMonth;
		data.day = (BYTE)time.wDay;
		data.hour = (BYTE)time.wHour;
		data.minute = (BYTE)time.wMinute;
		data.second = (BYTE)time.wSecond;
		UINT val;
		val = sizeof(mStationString) / sizeof(mStationString[0]);
		data.station = (BYTE)(rand() % val);
		val = sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]);
		data.equipment = (BYTE)(rand() % val);
		BYTE sb = (BYTE)(rand() % 93);
		if (sb == 13)
			data.imstatus = 1;
		else if (sb == 17)
			data.imstatus = 2;
		else
			data.imstatus = 0;
		data.status = (BYTE)(rand() % 5);
		data.message = (BYTE)(rand() % 10000) + 40000;
		mEquipmentHistoryTemp.push_back(data);
#ifdef _NOCOMM
		CString str, cbuf, temp;
		cbuf.Empty();
		str.Format(_T("%04d-%02d-%02d"), data.year, data.month, data.day);
		cbuf += str;
		str.Format(_T(",%02d:%02d:%02d"), data.hour, data.minute, data.second);
		cbuf += str;
		str.Format(_T(",%s"), mStationString[data.station]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistImStatusString[data.imstatus]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistStatusString[data.status]);
		cbuf += str;
		str.Format(_T(",%d"), data.message);
		cbuf += str;
		str.Format(_T(",[### TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST ###"));
		cbuf += str;
		cbuf += _T("\n");
		if (mEquipmentHistFile.m_hFile != INVALID_HANDLE_VALUE){
			TCHAR* p = _wsetlocale(LC_ALL, _T(""));
			mEquipmentHistFile.WriteString(cbuf);
			_wsetlocale(LC_ALL, p);
			mEquipmentHistFile.Flush();
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}

/*============================================================================*/
/*! �f�[�^���W

-# �C�x���g�����f�[�^�ɍ쐬

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::CreateEventHistory()
{
	CGraphMutex::Lock(eData);

	try
	{
		stHistoryData data;
		SYSTEMTIME time;
		GetLocalTime(&time);
		data.year = time.wYear;
		data.month = (BYTE)time.wMonth;
		data.day = (BYTE)time.wDay;
		data.hour = (BYTE)time.wHour;
		data.minute = (BYTE)time.wMinute;
		data.second = (BYTE)time.wSecond;
		UINT val;
		val = sizeof(mStationString) / sizeof(mStationString[0]);
		data.station = (BYTE)(rand() % val);
		val = sizeof(mHistEquipmentString) / sizeof(mHistEquipmentString[0]);
		data.equipment = (BYTE)(rand() % val);
		BYTE sb = (BYTE)(rand() % 93);
		if (sb == 13)
			data.imstatus = 1;
		else if (sb == 17)
			data.imstatus = 2;
		else
			data.imstatus = 0;
		data.status = (BYTE)(rand() % 3);
		data.message = (BYTE)(rand() % 10000) + 60000;
		mEventHistoryTemp.push_back(data);
#ifdef _NOCOMM
		CString str, cbuf, temp;
		cbuf.Empty();
		str.Format(_T("%04d-%02d-%02d"), data.year, data.month, data.day);
		cbuf += str;
		str.Format(_T(",%02d:%02d:%02d"), data.hour, data.minute, data.second);
		cbuf += str;
		str.Format(_T(",%s"), mStationString[data.station]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistEquipmentString[data.equipment]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistImStatusString[data.imstatus]);
		cbuf += str;
		str.Format(_T(",%s"), (LPCTSTR)mHistStatusString[data.status]);
		cbuf += str;
		str.Format(_T(",%d"), data.message);
		cbuf += str;
		str.Format(_T(",[### TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST ###"));
		cbuf += str;
		cbuf += _T("\n");
		if (mEventHistFile.m_hFile != INVALID_HANDLE_VALUE){
			TCHAR* p = _wsetlocale(LC_ALL, _T(""));
			mEventHistFile.WriteString(cbuf);
			_wsetlocale(LC_ALL, p);
			mEventHistFile.Flush();
		}
#endif
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
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
#ifdef _CUTNOCOMM
	SYSTEMTIME time;
	GetLocalTime(&time);
	if (mOperationHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mOperationHistFile.Close();
	}
	if (mControlHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mControlHistFile.Close();
	}
	if (mEquipmentHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mEquipmentHistFile.Close();
	}
	if (mEventHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mEventHistFile.Close();
	}

	bool ret;
	CString str, fname;
	str.Format(_T("%4d%02d%02d"), time.wYear, time.wMonth, time.wDay);
	fname = theApp.GetAppDataPath();
	fname += _T("\\");
	fname += _T("Operation_");
	fname += str;
	fname += _T(".txt");
	ret = false;
	mOperationHistFile.Open(fname, CFile::modeRead);
	if (mOperationHistFile.m_hFile != INVALID_HANDLE_VALUE){
		// ���ɂ���
		ret = true;
		mOperationHistFile.Close();
	}
	mOperationHistFile.Open(fname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
	if (ret == true)
		mOperationHistFile.SeekToEnd();

	fname = theApp.GetAppDataPath();
	fname += _T("\\");
	fname += _T("Control_");
	fname += str;
	fname += _T(".txt");
	ret = false;
	mControlHistFile.Open(fname, CFile::modeRead);
	if (mControlHistFile.m_hFile != INVALID_HANDLE_VALUE){
		// ���ɂ���
		ret = true;
		mControlHistFile.Close();
	}
	mControlHistFile.Open(fname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
	if (ret == true)
		mControlHistFile.SeekToEnd();

	fname = theApp.GetAppDataPath();
	fname += _T("\\");
	fname += _T("Equipment_");
	fname += str;
	fname += _T(".txt");
	ret = false;
	mEquipmentHistFile.Open(fname, CFile::modeRead);
	if (mEquipmentHistFile.m_hFile != INVALID_HANDLE_VALUE){
		// ���ɂ���
		ret = true;
		mEquipmentHistFile.Close();
	}
	mEquipmentHistFile.Open(fname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
	if (ret == true)
		mEquipmentHistFile.SeekToEnd();

	fname = theApp.GetAppDataPath();
	fname += _T("\\");
	fname += _T("Event_");
	fname += str;
	fname += _T(".txt");
	ret = false;
	mEventHistFile.Open(fname, CFile::modeRead);
	if (mEventHistFile.m_hFile != INVALID_HANDLE_VALUE){
		// ���ɂ���
		ret = true;
		mEventHistFile.Close();
	}
	mEventHistFile.Open(fname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
	if (ret == true)
		mEventHistFile.SeekToEnd();
#endif
	createCollectionThread();
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
	case	eMessage_UpdateForecastData:
		break;
	default:
		return;
		break;
	}
	if ((theApp.m_pMainWnd)->GetSafeHwnd())
		(theApp.m_pMainWnd)->PostMessage(message, wParam, lParam);
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
	removeMeasureThread();
#ifdef _NOCOMM
	if (mOperationHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mOperationHistFile.Close();
	}
	if (mControlHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mControlHistFile.Close();
	}
	if (mEquipmentHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mEquipmentHistFile.Close();
	}
	if (mEventHistFile.m_hFile != INVALID_HANDLE_VALUE){
		mEventHistFile.Close();
	}
	mOperationHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mControlHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mEquipmentHistFile.m_hFile = INVALID_HANDLE_VALUE;
	mEventHistFile.m_hFile = INVALID_HANDLE_VALUE;
#endif
}
/*============================================================================*/
/*! �f�[�^���W

-# �f�[�^���W�X���b�h�̍폜

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::removeMeasureThread()
{
	if (mpCollectThread != NULL){
		mpCollectThread->StopCollection();
		WaitForSingleObject(mpCollectThread->m_hThread, 10000);
		delete mpCollectThread;
		mpCollectThread = NULL;
	}
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
	CGraphMutex::Lock(eData);

	try
	{
		for (int nStation = 0; nStation < eStation_MAX; nStation++)
		{
			// �ꎞ�I�Ɋi�[���ꂽ�f�[�^���P�v�̈�Ɋi�[����
			while (!mSpaceTemp[nStation].empty())
			{
				CSpaceData newData = mSpaceTemp[nStation].front();
				mSpaceTemp[nStation].erase(mSpaceTemp[nStation].begin());

				// �C���f�b�N�X�}�b�v����C���f�b�N�X�擾
				UINT nIndex = mGraphSigIndexMap[nStation][newData.dataname];
				if (nIndex == 0)
				{
					// �}�b�v�ɂȂ����߁A�O���t�ΏۊO
					continue;
				}
				
				nIndex--;

				if (mSpaceData[nStation][nIndex].dataname.IsEmpty())
				{
					// ����͊Ď�������ʂ�R�s�[
					mSpaceData[nStation][nIndex].dataid = newData.dataid;
					mSpaceData[nStation][nIndex].dataname = newData.dataname;
					mSpaceData[nStation][nIndex].kind = newData.kind;
					mSpaceData[nStation][nIndex].srcType = newData.srcType;
					mSpaceData[nStation][nIndex].totalSize = newData.totalSize;
					mSpaceData[nStation][nIndex].value = newData.value;
				}
				else
				{
					// �����Ď��f�[�^���L���[��������A�l��ǉ�
					while (!newData.value.empty())
					{
						stSpaceData spaceData = newData.value.front();
						newData.value.pop_front();
#ifdef _DEBUG
						{
							CString strDebug;
							strDebug.Format(_T("%s(%d) :: Add Data to mSpaceData: Time(%I64d), SigId(%u), Val(%f)"), __FILEW__, __LINE__, CTime(spaceData.systime).GetTime(), newData.dataid, spaceData.value);
							CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection_Dummy"), _T("debug"), strDebug, nLogEx::debug);
						}
#endif
						// �f�[�^�������Ή�
						int size = (int)mSpaceData[nStation][nIndex].value.size();
						if (size > 0)
						{
							deque<stSpaceData>::iterator itr = mSpaceData[nStation][nIndex].value.end();
							itr--;
							CTime lastTime = (*itr).systime;
							CTime curTime = spaceData.systime;

							// �V�����f�[�^�̎������A���X�g�i�[�ς݃f�[�^�����Â��ꍇ�͐V�����f�[�^��j������
							if (curTime < lastTime)
							{
								break;
							}

							while (curTime > lastTime + 1)
							{
								stSpaceData		sEmptyData;
								memset(&sEmptyData, 0, sizeof(stSpaceData));
								lastTime += 1;
								lastTime.GetAsSystemTime(sEmptyData.systime);
								sEmptyData.status[0] = -1;
								sEmptyData.ppPair.max = 0.0;
								sEmptyData.ppPair.min = 0.0;
								sEmptyData.ppPair.count = 1;
								// �Ď��f�[�^�̃L���[�T�C�Y����
//								while (mSpaceData[nStation][nIndex].value.size() >= MAX_SPACEDATA_SIZE)
//								{
//									mSpaceData[nStation][nIndex].value.pop_front();
//								}
								LimitQueueSize(mSpaceData[nStation][nIndex].value);

								// �f�[�^�g�[�^���T�C�Y���C���N�������g
								mSpaceData[nStation][nIndex].totalSize++;
								// �Ď��f�[�^�̃L���[�ɒǉ�
								mSpaceData[nStation][nIndex].value.push_back(sEmptyData);
							}
						}

						// �Ď��f�[�^�̃L���[�T�C�Y����
//						while (mSpaceData[nStation][nIndex].value.size() >= MAX_SPACEDATA_SIZE)
//						{
//							mSpaceData[nStation][nIndex].value.pop_front();
//						}
						LimitQueueSize(mSpaceData[nStation][nIndex].value);

						// �f�[�^�g�[�^���T�C�Y���C���N�������g
						mSpaceData[nStation][nIndex].totalSize++;
						// �Ď��f�[�^�̃L���[�ɒǉ�
						mSpaceData[nStation][nIndex].value.push_back(spaceData);

						break;
					}
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

	CGraphMutex::Unlock(eData);

	// �����łP�b�����Ŏ擾�ł��Ȃ������f�[�^�̑΍���s���B
	// CSpaceData���́uvalue�v�ɂ�stSpaceData�\���̂������i�[����Ă���
	/*
	�@[stSpaceData(A)][stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]�E�E�E
	�A[stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]�E�E�E
	�P�b�������Ɏ擾�����f�[�^���̓T�[�o�[�R���g���[���̐��\�ɂ��قȂ�
	�Ⴆ�΁A�@�Ŏ擾�����f�[�^�Ɂu(A)�v�f�[�^�����݂��Ȃ��ꍇ��
	�u(A)�v�́ustatus�v���uNone�v�ɂ��ēo�^����K�v������
	�A[stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]�E�E�E[stSpaceData(A)]
	�O���t�`�悷��ۂɁustatus�v���uNone�v�̃f�[�^�͕`�悵�Ȃ�
	*/

	return true;
}


/*============================================================================*/
/*! �f�[�^���W

-# �H�w�l�ϊ������M�����f�[�^��o�^����

@param

@retval void
*/
/*============================================================================*/
bool CDataCollection::AddCollection(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data)
{
	CGraphMutex::Lock(eData);

	try
	{

		for (vector<CEngValCnv::obs_tbl_res_t>::iterator itrEng = data.begin(); itrEng != data.end(); itrEng++)
		{
			CSpaceData		newData;
			stSpaceData		sData;
			newData.value.clear();
			memset(&sData, 0, sizeof(stSpaceData));
			CString strDataName = CString((*itrEng).obs_adr.sz_obsname);
			newData.dataid = mGraphSigIndexMap[nStation][strDataName];
			if (newData.dataid == 0)
				continue;

			newData.dataname = strDataName;
			newData.kind = 0;		// �\���Ώۃf�[�^���(0:�f�[�^�l / 1:�X�e�[�g�l)
			newData.srcType = eSigSrcType_EngExchange;
			newData.totalSize = 1;

			CTime	cTime((time_t)(*itrEng).ld_time_val);
			cTime.GetAsSystemTime(sData.systime);

#ifdef	DemoTimeStamp
			// �f���p�C�v�ʐM�p�Ɍ��ݎ������X�^���v
			GetSystemTime(&sData.systime);
#endif	// DemoTimeStamp

			time_t	data_t = cTime.GetTime();
			CTime	curTime;
			theApp.GetCurrentUtcTime(curTime);
			time_t  utc_t = curTime.GetTime();

			// �Ď��f�[�^���������݂�UTC�V�X�e���������K�莞�Ԉȏ�Â��ꍇ�́A�O���t�q���Ď��f�[�^�L���[�Ɋi�[�����ɔj������
			if (data_t + mKanshiDisposeTimeFromUTC < utc_t)
			{
#ifdef _DEBUG
				{
					TCHAR szSigName[_MAX_PATH];
					MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: Recieved kanshi data time(%I64d) + Kanshi dispase time(%I64d) is older System UTC Time(%I64d), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, data_t, mKanshiDisposeTimeFromUTC, utc_t, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
				}
#endif
				continue;
			}

			sData.idx = 0;
			sData.value = (*itrEng).obs_adr.d_data;
			memcpy(sData.status, (*itrEng).obs_adr.sz_sts_name, 8);

			sData.status[0] = 0;

			sData.ppPair.max = sData.value;
			sData.ppPair.min = sData.value;
			//			sData.ppPair.count = 0;
			sData.ppPair.count = 1;
			newData.value.push_back(sData);

#ifdef _DEBUG
			{
				TCHAR szSigName[_MAX_PATH];
				//								MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, (int)strlen((*itrEng).obs_adr.sz_obsname), szSigName, (int)sizeof(szSigName) / 2);
				MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
				CString strDebug;
				strDebug.Format(_T("%s(%d) :: Recv Eng Data: Time(%lf), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, (*itrEng).ld_time_val, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
				CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
			}
#endif

			// �C���f�b�N�X�}�b�v����C���f�b�N�X�擾
			UINT nIndex = mGraphSigIndexMap[nStation][newData.dataname];
			if (nIndex == 0)
			{
				// �}�b�v�ɂȂ����߁A�O���t�ΏۊO
				continue;
			}

			nIndex--;

			if (mSpaceData[nStation][nIndex].dataname.IsEmpty())
			{
				BOOL bNewEngValData = TRUE;
				int size = GetGraphSigSize(nStation);
				for (int i = 0; i < size; i++)
				{
					if (mSpaceData[nStation][i].value.size() > 0)
					{
						bNewEngValData = FALSE;
						break;
					}
				}

				// ����͊Ď�������ʂ�R�s�[
				mSpaceData[nStation][nIndex].dataid = newData.dataid;
				mSpaceData[nStation][nIndex].dataname = newData.dataname;
				mSpaceData[nStation][nIndex].kind = newData.kind;
				mSpaceData[nStation][nIndex].srcType = newData.srcType;
				mSpaceData[nStation][nIndex].totalSize = newData.totalSize;
				mSpaceData[nStation][nIndex].value = newData.value;

#ifdef _DEBUG
				{
					TCHAR szSigName[_MAX_PATH];
					MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: Add Data to mSpaceData(New ID): Time(%lf), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, (*itrEng).ld_time_val, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
				}
#endif
				// ��̏ꍇ�́A�V�����H�w�l�ϊ����󂯂����Ƃ�ʒm
				if (bNewEngValData)
					theApp.m_pMainWnd->PostMessage(eMessage_NewEngValData, 0, 0);
			}
			else
			{

#ifdef _DEBUG
				{
					TCHAR szSigName[_MAX_PATH];
					MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
					CString strDebug;
					strDebug.Format(_T("%s(%d) :: Detect Same Data in mSpaceData: Time(%lf), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, (*itrEng).ld_time_val, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
					CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
				}
#endif

				// �����Ď��f�[�^���L���[��������A�l��ǉ�
				while (!newData.value.empty())
				{
					stSpaceData spaceData = newData.value.front();
					newData.value.pop_front();

					// �f�[�^�������Ή�
					int size = (int)mSpaceData[nStation][nIndex].value.size();
					if (size > 0)
					{
						deque<stSpaceData>::iterator itrSpace = mSpaceData[nStation][nIndex].value.end();
						itrSpace--;
						CTime lastTime = (*itrSpace).systime;
						CTime curTime = spaceData.systime;

						// �V�����f�[�^�̎������A���X�g�i�[�ς݃f�[�^�����Â��ꍇ�͐V�����f�[�^��j������
						if (curTime < lastTime)
						{
//#ifdef _DEBUG
//							//---------------------- ���Ȃ��Ď��f�[�^�Œ����ԓ���m�F�������Ƃ��̂ݎg�p ------------------------------//
//							time_t inc_t = 1;
//							if (newData.dataname.Compare(USC34_AGC_SHXX_TCR_SIG) == 0)
//							{
//								time_t cur_t = curTime.GetTime();
//								// �V�����f�[�^�����ƒ��O�̒Z�����������r���A��v���Ă���Ȃ�Ύ����C���N�������g�͖���
//								if (cur_t == m_shortCycleTime)
//								{
//									inc_t = 0;
//								}
//								// ��v���Ă��Ȃ��ꍇ�́A�V�����f�[�^����������
//								else
//								{
//									m_shortCycleTime = cur_t;
//								}
//
//								// �ϕ��̃f�[�^������1�b��藣��Ă��鎞�͎����C���N�������g�͖���
//								if (lastTime.GetTime() > 0 && m_integtime > 0 && lastTime.GetTime() > m_integtime + 1)
//								{
//									inc_t = 0;
//								}
//							}
//
//							if (newData.dataname.Compare(USC34_EL_EL_SIG) == 0)
//							{
//								// �ϕ��̃f�[�^������1�b��藣��Ă��鎞�͎����C���N�������g�͖���
//								if (lastTime.GetTime() > 0 && m_integtime > 0 && lastTime.GetTime() > m_integtime + 1)
//								{
//									inc_t = 0;
//								}
//							}
//
//							curTime = lastTime + inc_t;
//							curTime.GetAsSystemTime(spaceData.systime);
//
//							if (newData.dataname.Find(USC34_AGC_INTEG_SIG) >= 0)
//							{
//								// �ϕ��̃f�[�^�������T���Ă���
//								m_integtime = curTime.GetTime();
//							}
//
//							//--------------------------------------------------------------------------------------------------------//
//#else
							break;
//#endif
						}

						while (curTime > lastTime + 1)
						{
							stSpaceData		sEmptyData;
							memset(&sEmptyData, 0, sizeof(stSpaceData));
							lastTime += 1;
							lastTime.GetAsSystemTime(sEmptyData.systime);
							sEmptyData.status[0] = -1;
							sEmptyData.ppPair.max = 0.0;
							sEmptyData.ppPair.min = 0.0;
							sEmptyData.ppPair.count = 1;
							// �Ď��f�[�^�̃L���[�T�C�Y����
//							while (mSpaceData[nStation][nIndex].value.size() >= MAX_SPACEDATA_SIZE)
//							{
//								mSpaceData[nStation][nIndex].value.pop_front();
//							}
							LimitQueueSize(mSpaceData[nStation][nIndex].value);

							// �f�[�^�g�[�^���T�C�Y���C���N�������g
							mSpaceData[nStation][nIndex].totalSize++;
							// �Ď��f�[�^�̃L���[�ɒǉ�
							mSpaceData[nStation][nIndex].value.push_back(sEmptyData);
						}
					}

					// �Ď��f�[�^�̃L���[�T�C�Y����
//					while (mSpaceData[nStation][nIndex].value.size() >= MAX_SPACEDATA_SIZE)
//					{
//						mSpaceData[nStation][nIndex].value.pop_front();
//					}
					LimitQueueSize(mSpaceData[nStation][nIndex].value);

#ifdef _DEBUG
						{
							TCHAR szSigName[_MAX_PATH];
							MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (*itrEng).obs_adr.sz_obsname, -1, szSigName, _MAX_PATH);
							CString strDebug;
							strDebug.Format(_T("%s(%d) :: Add Data to mSpaceData(Same ID): Time(%lf), SigName(%s), SigId(%u), Val(%f)"), __FILEW__, __LINE__, (*itrEng).ld_time_val, szSigName, newData.dataid, (*itrEng).obs_adr.d_data);
							CLogTraceEx::Write(_T(""), _T("CDataCollection"), _T("AddCollection"), _T("debug"), strDebug, nLogEx::debug);
						}
#endif

					// �f�[�^�g�[�^���T�C�Y���C���N�������g
					mSpaceData[nStation][nIndex].totalSize++;
					// �Ď��f�[�^�̃L���[�ɒǉ�
					mSpaceData[nStation][nIndex].value.push_back(spaceData);
					break;
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

	CGraphMutex::Unlock(eData);

	// �����łP�b�����Ŏ擾�ł��Ȃ������f�[�^�̑΍���s���B
	// CSpaceData���́uvalue�v�ɂ�stSpaceData�\���̂������i�[����Ă���
	/*
	�@[stSpaceData(A)][stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]�E�E�E
	�A[stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]�E�E�E
	�P�b�������Ɏ擾�����f�[�^���̓T�[�o�[�R���g���[���̐��\�ɂ��قȂ�
	�Ⴆ�΁A�@�Ŏ擾�����f�[�^�Ɂu(A)�v�f�[�^�����݂��Ȃ��ꍇ��
	�u(A)�v�́ustatus�v���uNone�v�ɂ��ēo�^����K�v������
	�A[stSpaceData(B)][stSpaceData(C)][stSpaceData(D)]�E�E�E[stSpaceData(A)]
	�O���t�`�悷��ۂɁustatus�v���uNone�v�̃f�[�^�͕`�悵�Ȃ�
	*/

	return true;
}


/*============================================================================*/
/*! �f�[�^���W

-# �L���[�̃O���t�f�[�^���폜����

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::ClearCollection(UINT nStation, CString strGName)
{
	CGraphMutex::Lock(eData);

	try
	{
		// �C���f�b�N�X�}�b�v����C���f�b�N�X�擾
		UINT nIndex = mGraphSigIndexMap[nStation][strGName];
		if (nIndex == 0)
		{
			// �}�b�v�ɂȂ����߁A�O���t�ΏۊO
			CGraphMutex::Unlock(eData);
			return;
		}

		nIndex--;

		mSpaceData[nStation][nIndex].value.clear();
		mSpaceData[nStation][nIndex].totalSize = 0;
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);
}


/*============================================================================*/
/*! �f�[�^���W

-# �L���[�̃O���t�f�[�^���폜����

@param

@retval void
*/
/*============================================================================*/
void CDataCollection::ClearCollection(UINT nStation)
{
	for (int i = 0; i < GRAPH_YOHOSIGNUM; i++)
	{
		ClearCollection(nStation, mListYohoSig[nStation][i]);
	}
}


/*============================================================================*/
/*! �f�[�^���W

-# �T�����M�����f�[�^��o�^����

@param

@retval bool
*/
/*============================================================================*/
UINT CDataCollection::AddHistory(UINT message, WPARAM wParam, LPARAM lParam)
{
	CGraphMutex::Lock(eData);
	UINT	status = eHistStatus_Normal;

	try
	{
		// �ꎞ�I�Ɋi�[���ꂽ�f�[�^���P�v�̈�Ɋi�[����
		while (!mOperationHistoryTemp.empty())
		{
			stHistoryData newData = mOperationHistoryTemp.front();
			mOperationHistoryTemp.pop_front();

			if (mOperationHistoryData.size() == MAX_HISTORY_DATA){
				mOperationHistoryData.pop_front();
			}
			mOperationHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.status);
		}

		while (!mControlHistoryTemp.empty())
		{
			stHistoryData newData = mControlHistoryTemp.front();
			mControlHistoryTemp.pop_front();

			if (mControlHistoryData.size() == MAX_HISTORY_DATA){
				mControlHistoryData.pop_front();
			}
			mControlHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.status);
		}

		while (!mEquipmentHistoryTemp.empty())
		{
			stHistoryData newData = mEquipmentHistoryTemp.front();
			mEquipmentHistoryTemp.pop_front();

			if (mEquipmentHistoryData.size() == MAX_HISTORY_DATA){
				mEquipmentHistoryData.pop_front();
			}
			mEquipmentHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.imstatus & 0x7fffffff);
		}

		if (mSubEquipmentHistoryData.size() > MAX_HISTORY_DATA){
			mSubEquipmentHistoryData.pop_front();
		}

		while (!mEventHistoryTemp.empty())
		{
			stHistoryData newData = mEventHistoryTemp.front();
			mEventHistoryTemp.pop_front();

			if (mEventHistoryData.size() == MAX_HISTORY_DATA){
				mEventHistoryData.pop_front();
			}
			if (mSubEventHistoryData.size() == MAX_HISTORY_DATA){
				mSubEventHistoryData.pop_front();
			}
			mEventHistoryData.push_back(newData);
			mSubEventHistoryData.push_back(newData);
			status = __max((BYTE)status, newData.imstatus & 0x7fffffff);
		}

	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	CGraphMutex::Unlock(eData);

	return status;
}


/*============================================================================*/
/*! �f�[�^���W

-# ��n�ǃ��X�g�̎擾

@param		list	��n�ǖ��̊i�[�o�b�t�@
@retval

*/
/*============================================================================*/
void CDataCollection::GetBaseStationList(UINT nStation, vector<CString>& list)
{
	list.clear();
	vector<CString>::iterator itrl;

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
	for (itr = mDataNameList[nStation].begin(); itr != mDataNameList[nStation].end(); itr++){
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
void CDataCollection::GetEquipmentList(UINT nStation, CString baseName, vector<CString>& list)
{
	list.clear();
	vector<CString>::iterator itrl;

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
	for (itr = mDataNameList[nStation].begin(); itr != mDataNameList[nStation].end(); itr++){
		AfxExtractSubString(strs, (*itr).second, 0, '.');
		AfxExtractSubString(stre, (*itr).second, 1, '.');
		if (baseName.IsEmpty() == false && baseName != strs)
			continue;
		bool set = true;
		for (itrl = list.begin(); itrl != list.end(); itrl++){
			if (stre == (*itrl)){
				set = false;
				break;
			}
		}
		if (set == true)
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
void CDataCollection::GetDataNameList(UINT nStation, CString baseName, CString equipment, vector<CString>& list)
{
	list.clear();

	map<DWORD, CString>::iterator itr;
	CString strs, stre;
	for (itr = mDataNameList[nStation].begin(); itr != mDataNameList[nStation].end(); itr++){
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

-# �q���ߑ��f�[�^�擾

@param		nStation	�ǔԍ�
@retval		�q���ߑ��f�[�^�L���[

*/
/*============================================================================*/
CSpaceData* CDataCollection::GetSpaceData(int nStation /*= eStation_MAX*/)
{
	if (nStation < 0 || nStation > eStation_MAX)
		return &mSpaceData[0][0];
	if (nStation == eStation_MAX)
		return &mSpaceData[theApp.GetSelectStation()][0];
	return &mSpaceData[nStation][0];
}

/*============================================================================*/
/*! �f�[�^���W

-# �q���ߑ��e���|�����f�[�^�擾�i�_�~�[�f�[�^�j

@param		nStation	�ǔԍ�
@retval		�q���ߑ��e���|�����f�[�^�L���[

*/
/*============================================================================*/
deque<CSpaceData>& CDataCollection::GetSpaceTemp(int nStation /*= eStation_MAX*/)
{
	if (nStation < 0 || nStation > eStation_MAX)
		return mSpaceTemp[0];
	if (nStation == eStation_MAX)
		return mSpaceTemp[theApp.GetSelectStation()];
	return mSpaceTemp[nStation];
}

/*============================================================================*/
/*! �f�[�^���W

-# �q���ߑ��e���|�����f�[�^�擾�i�H�w�l�ϊ���f�[�^�j

@param		nStation	�ǔԍ�
@retval		�q���ߑ��e���|�����f�[�^�L���[

*/
/*============================================================================*/
deque<CSpaceData>& CDataCollection::GetSpaceEngValTemp(int nStation /*= eStation_MAX*/)
{
	if (nStation < 0 || nStation > eStation_MAX)
		return mSpaceTemp[0];
	if (nStation == eStation_MAX)
		return mSpaceTemp[theApp.GetSelectStation()];
	return mSpaceTemp[nStation];
}


/*============================================================================*/
/*! �f�[�^���W

-# �q�����f�[�^�̃T�C�Y���ő�l�܂łɐ�������

@param		value	�q���f�[�^�L���[
@retval		�Ȃ�

*/
/*============================================================================*/
void CDataCollection::LimitQueueSize(deque<stSpaceData>& value)
{
	// �폜����ꍇ
	if (value.size() > MAX_SPACEDATA_SIZE)
	{
		// �L���[�̐擪�ƏI���̎����𒲂ׂ�
		deque<stSpaceData>::iterator itrSpace;
		itrSpace = value.begin();
		CTime firstTime = (*itrSpace).systime;

		itrSpace = value.end();
		itrSpace--;
		CTime lastTime = (*itrSpace).systime;

		time_t first_t = firstTime.GetTime();
		time_t last_t = lastTime.GetTime();

		// �L���[�T�C�Y��������V�����擪�������o��
		time_t new_first_t = last_t - ((time_t)MAX_SPACEDATA_SIZE - 1);
		if (new_first_t < 0)
		{
			new_first_t = 0;
		}

		while (first_t < new_first_t)
		{
			// �L���[����V�����擪���O�̎����̃f�[�^���폜����
			value.pop_front();
			itrSpace = value.begin();
			firstTime = (*itrSpace).systime;
			first_t = firstTime.GetTime();
		}
	}
}

/*============================================================================*/
/*! �A�v���P�[�V����

-# �Ǔ��ʂ̊Ď����u������

@param str:�Ď���

@retval void
*/
/*============================================================================*/
CString CDataCollection::ReplaceAreaKanshiName(CString& str)
{
	CString strKanshi = str;
	int nAreaNo = theApp.GetClientTerminal();
	switch (nAreaNo)
	{
	case eClientSttn:
		strKanshi.Replace(mAreaNameString[eClientTksc], mAreaNameString[eClientSttn]);
		break;
	
	case eClientSsoc:
		strKanshi.Replace(mAreaNameString[eClientTksc], mAreaNameString[eClientSsoc]);
		break;
	
	case eClientTksc:
	default:
		// �������Ȃ�
		break;
	}

	return strKanshi;
}