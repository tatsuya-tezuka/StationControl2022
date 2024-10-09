#pragma once

#include "DataCollectionThread.h"
#include "EngValCnv.h"
#include "..\utility\StationPipe.h"

using namespace std;


//=============================================================================
//	�\����
//=============================================================================
//=============================================================================
//	�T�}���f�[�^
//=============================================================================
#define MAX_SUMMARY_DATA	50
typedef struct {
	double	value;
} stSummaryData;
//=============================================================================
//	�����f�[�^
//=============================================================================
#define MAX_HISTORY_DATA 1000
#define MAX_HISTORYSEARCH_DATA 50000

//=============================================================================
//	��`
//=============================================================================

// ���M��
static const CString mHistEquipmentString[] = {
	_T("�ǉ^�p�ǐ����u"), _T("�ݔ����䑕�u"), _T("X��TLM���͑��u(2)"), _T("X��CMD�o�͑��u(2)"),
};
static const CString mHistEquipmentStringSimple[] = {
	_T("��"), _T("��"), _T("XTLM"), _T("XCMD"),
};
enum{
	eSenderItem1,
	eSenderItem2,
	eSenderItem3,
	eSenderItem4,

	eSenderItemMax,
};

// �X�e�[�^�X
static const CString mHistStatusString[] = {
	_T("���"), _T("�x��"), _T("�ُ�"), _T("STATUS"), _T("WARNING"), _T("FAULT"), _T("REMOTE/LOCAL"), _T("POWER ON/OFF"),
};
static const CString mHistImStatusString[] = {
	_T("����"), _T("�x��"), _T("�ُ�"),
};
static const CString mEqHistStatusString[] = {
	_T("STATUS"), _T("WARNING"), _T("FAULT"), _T("REMOTE/LOCAL"), _T("POWER ON/OFF"),
};

enum{
	eHistStatus_Normal,
	eHistStatus_Warning,
	eHistStatus_Fault,
};

enum{
	eEqHistStatus_Status,
	eEqHistStatus_Warning,
	eEqHistStatus_Fault,
	eEqHistStatus_RemoteLocal,
	eEqHistStatus_PowerOnOff,
	eEqHistStatus_Max,
};

//=============================================================================
//	�ݔ��^�p��
//=============================================================================
#define mTitleEquipment	_T("�ݔ����")
#define mTitleOperation	_T("�^�p���")
#ifdef _DEBUG
static const CString mEqString[4][5] = {
	{ _T("DSN\nMODEM"), _T(""), _T(""), _T(""), _T("") },
	{ _T("TRK-RX\n"), _T(""), _T(""), _T(""), _T("") },
	{ _T("ACU\n"), _T(""), _T(""), _T(""), _T("") },
	{ _T("SSPA\n"), _T(""), _T(""), _T(""), _T("") },
};
static const CString mOpeString[4][5] = {
	{ _T("AGC\nCAL"), _T("STLM\nSET"), _T("STLM\nON"), _T("STLM\nPASS"), _T("STLM\nOFF") },
	{ _T("TLM\nCHK"), _T("XTLM\nSET"), _T("XTLM\nON"), _T("XTLM\nPASS"), _T("XTLM\nOFF") },
	{ _T("CMD\nCHK"), _T("U/L\nSET"), _T("U/L\nON"), _T("U/L\nPASS"), _T("U/L\nOFF") },
	{ _T("RNG\nCAL"), _T("RNG\nSET"), _T("RNG\nON"), _T("RNG\nPASS"), _T("RNG\nOFF") },
};
#endif

// �O���t�Ď���
#define GRAPH_KANSHI_NUM	50


// CDataCollection �R�}���h �^�[�Q�b�g

class CDataCollection
{
public:
	CDataCollection();
	virtual ~CDataCollection();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	static const time_t		mKanshiDisposeTimeFromUTC = 60 * 60 * 24 * 360;		// �Ď���UTC��������̔j�����莞��(s)

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	int				m_angle[80];
	CSpaceData				mSpaceData[eStation_MAX][GRAPH_KANSHI_NUM];	// �Ď��f�[�^
	deque<CSpaceData>		mSpaceTemp[eStation_MAX];		// �T�[�o��M�p�̃��[�N
	deque<CSpaceData>		mSpaceEngValTemp[eStation_MAX];	// �T�[�o��M�p�̃��[�N�i�H�w�l�ϊ���f�[�^�j
	deque<CSpaceData>		mForecastData[eStation_MAX];	// �\��l�Ď��f�[�^
	deque<CSpaceData>		mForecastTemp[eStation_MAX];	// �\��l�̃��[�N

	map<DWORD, CString>		mDataNameList[eStation_MAX];			// �f�[�^���̃��X�g
	map<CString, UINT>		mGraphSigIndexMap[eStation_MAX];		// �q���ߑ��O���t�p�Ď��̃}�b�v�C���f�b�N�X
	int						mGraphSigIndexNum[eStation_MAX];		// �q���ߑ��O���t�p�Ď��̃C���f�b�N�X��
	CDataCollectionThread*	mpCollectThread;		// �f�[�^���W�X���b�h
	deque<stHistoryData>	mOperationHistoryData;	// �^�p�����f�[�^
	deque<stHistoryData>	mOperationHistoryTemp;	// �^�p�����f�[�^�T�[�o��M�p�̃��[�N
	deque<stHistoryData>	mControlHistoryData;	// ���䗚���f�[�^
	deque<stHistoryData>	mControlHistoryTemp;	// ���䗚���f�[�^�T�[�o��M�p�̃��[�N
	deque<stHistoryData>	mEquipmentHistoryData;	// �ݔ��󋵗����f�[�^
	deque<stHistoryData>	mEquipmentHistoryTemp;	// �ݔ��󋵗����f�[�^�T�[�o��M�p�̃��[�N
	deque<stHistoryData>	mEventHistoryData;		// �C�x���g�����f�[�^
	deque<stHistoryData>	mEventHistoryTemp;		// �C�x���g�����f�[�^�T�[�o��M�p�̃��[�N
	deque<stHistoryData>	mSubEquipmentHistoryData;	// �ݔ��󋵗����f�[�^�i�T�u��ʁj
	deque<stHistoryData>	mSubEventHistoryData;		// �C�x���g�����f�[�^�i�T�u��ʁj

	deque<PIPEMONITORACK>	mMessageTemp[eStation_MAX];

//	map<time_t, double>		mELPredInterpolateMap[eStation_MAX];	// EL�\��l��ԃf�[�^�}�b�v
	vector<stPredData>		mELPredInterpolateList[eStation_MAX];	// EL�\��l��ԃf�[�^���X�g

	DWORD					m_nHistID54;			// 54m �������ʔԍ�

#ifdef _NOCOMM
	CStdioFile	mOperationHistFile;
	CStdioFile	mControlHistFile;
	CStdioFile	mEquipmentHistFile;
	CStdioFile	mEventHistFile;
#endif

//#ifdef _DEBUG
//	time_t	m_shortCycleTime;
//	time_t	m_integtime;
//#endif

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

	CSpaceData*				GetSpaceData(int nStation = eStation_MAX);
	deque<CSpaceData>&		GetSpaceTemp(int nStation = eStation_MAX);
	deque<CSpaceData>&		GetSpaceEngValTemp(int nStation = eStation_MAX);
//	map<time_t, double>&	GetELPredInterpolateMap(UINT nStation)	{ return mELPredInterpolateMap[nStation]; }
	vector<stPredData>&		GetELPredInterpolateList(UINT nStation)	{ return mELPredInterpolateList[nStation]; }

	int					GetGraphSigIndex(int nStation, CString& strSigName) {
		if (nStation < 0 || nStation >= eStation_MAX) {
			if (mGraphSigIndexMap[0][strSigName] == 0)
				return -1;		// �O���t�̃C���f�b�N�X�}�b�v�ɊĎ������݂��Ȃ��ꍇ
			return (int)mGraphSigIndexMap[0][strSigName] - 1;
		}

		if (mGraphSigIndexMap[nStation][strSigName] == 0)
			return -1;		// �O���t�̃C���f�b�N�X�}�b�v�ɊĎ������݂��Ȃ��ꍇ

		return (int)mGraphSigIndexMap[nStation][strSigName] - 1;
	}
	
	int					GetGraphSigSize(int nStation = eStation_MAX) {
		if (nStation < 0 || nStation >= eStation_MAX) {
//			return (int)mGraphSigIndexMap[0].size();
			return mGraphSigIndexNum[0];
		}

//		return (int)mGraphSigIndexMap[nStation].size();
		return mGraphSigIndexNum[nStation];
	}


	BOOL DbRead();

	deque<stHistoryData>&	GetOperationHistoryData()	{ return mOperationHistoryData; }
	deque<stHistoryData>&	GetOperationHistoryTemp()	{ return mOperationHistoryTemp; }
	deque<stHistoryData>&	GetControlHistoryData()		{ return mControlHistoryData; }
	deque<stHistoryData>&	GetControlHistoryTemp()		{ return mControlHistoryTemp; }
	deque<stHistoryData>&	GetEquipmentHistoryData()		{ return mEquipmentHistoryData; }
	deque<stHistoryData>&	GetEquipmentHistoryTemp()		{ return mEquipmentHistoryTemp; }
	deque<stHistoryData>&	GetEventHistoryData()		{ return mEventHistoryData; }
	deque<stHistoryData>&	GetEventHistoryTemp()		{ return mEventHistoryTemp; }
	deque<stHistoryData>&	GetSubEquipmentHistoryData()		{ return mSubEquipmentHistoryData; }
	deque<stHistoryData>&	GetSubEventHistoryData()		{ return mSubEventHistoryData; }

	deque<PIPEMONITORACK>&	GetMessageTemp(int nDispStation)		{ return mMessageTemp[nDispStation]; }

	vector<stSelectData>	mSelectDataList;		// �I���f�[�^���X�g

	map<DWORD, CString>&	GetDataNameList(UINT nStation)	{ return mDataNameList[nStation]; }
	vector<stSelectData>&	GetSelectDataList()	{ return mSelectDataList; }
	void	GetBaseStationList(UINT nStation, vector<CString>& list);
	void	GetEquipmentList(UINT nStation, CString baseName, vector<CString>& list);
	void	GetDataNameList(UINT nStation, CString baseName, CString equipment, vector<CString>& list);

	DWORD	ConvertNameToId(UINT nStation, CString name);
	CString	ConvertIdToName(UINT nStation, DWORD id);
	void	CreateSpaceData(LPBYTE lpData, DWORD dwSize, UINT nStation);
	void	CreateForecastData(stServerItemData* pdata, UINT nStation);

	CString ConvUtf8SjisEx(std::string const& src);
	CString ConvUtf8Unicode(std::string const& src);

	bool	IsMessageEvent(TCHAR* pmsg);
	bool	IsMessageLimit(DWORD msgno);

	CString	UTF8_3CharsToSJis(char *pUtf8Str);
	void	CreateDataToHistory(PIPEMONITORACK* pbuf);
	void	CreateOperationHistory();
	void	CreateControlHistory();
	void	CreateEquipmentHistory();
	void	CreateEventHistory();

	void	StartCollection();
	void	StopCollection();
	bool	AddCollection(UINT message, WPARAM wParam, LPARAM lParam);				// �_�~�[�p�f�[�^���q�����X�g�ɒǉ�
	bool	AddCollection(UINT nStation, vector<CEngValCnv::obs_tbl_res_t>& data);		// �Ď����f�[�^���q�����X�g�ɒǉ�
	void	ClearCollection(UINT nStation, CString strGName);							// �O���t�M�����̃f�[�^���L���[����폜����
	void	ClearCollection(UINT nStation);												// �O���t�M�����̃f�[�^���L���[����폜����
	UINT	AddHistory(UINT message, WPARAM wParam, LPARAM lParam);

	void	SetKanshiName();

	CString ReplaceAreaKanshiName(CString& str);

	void	SetHistID54(DWORD val)			{ m_nHistID54 = val; }

protected:
	void	createCollectionThread();
	void	removeMeasureThread();
	static void CALLBACK messageProc(UINT message, WPARAM wParam, LPARAM lParam);

	void	LimitQueueSize(deque<stSpaceData>& value);

	/* ------------------------------------------------------------------------------------ */
};


