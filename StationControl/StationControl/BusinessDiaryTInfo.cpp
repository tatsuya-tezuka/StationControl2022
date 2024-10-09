/*============================================================================*/
/*! BusinessDiaryTInfo.cpp

-# �����Ɩ����� ���M���o�͊��ԓ��͉��
*/
/*============================================================================*/
// BusinessDiaryTInfo.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "BusinessDiaryTInfo.h"
#include "afxdialogex.h"
#include "SatelliteData.h"


#define IDX_RFINFO_DATE	0
#define IDX_RFINFO_SATN_UDSC	13
#define IDX_RFINFO_SATN_USC		14
//#define IDX_RFINFO_CMDSTATE2_USC	9
//#define IDX_RFINFO_CMDSTATE1_USC	6

// CBusinessDiaryTInfo �_�C�A���O

IMPLEMENT_DYNAMIC(CBusinessDiaryTInfo, CDialogBase)

CBusinessDiaryTInfo::CBusinessDiaryTInfo(CWnd* pParent /*=NULL*/)
: CDialogBase(CBusinessDiaryTInfo::IDD, pParent)
, m_StartTime(0)
, m_EndTime(0)
{

}

CBusinessDiaryTInfo::~CBusinessDiaryTInfo()
{
}

void CBusinessDiaryTInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DIARY_TINFPERIOD_START_DATE, mStartDate);
	DDX_Control(pDX, IDC_DIARY_TINFPERIOD_END_DATE, mEndDate);
	DDX_Control(pDX, IDC_DIARY_TINFPERIOD_STLT_CMB, mCbStation);
	DDX_DateTimeCtrl(pDX, IDC_DIARY_TINFPERIOD_START_TIME, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DIARY_TINFPERIOD_END_TIME, m_EndTime);
	DDX_Control(pDX, IDC_DIARY_TINFPERIOD_START_TIME, mStartTime);
	DDX_Control(pDX, IDC_DIARY_TINFPERIOD_END_TIME, mEndTime);
}


BEGIN_MESSAGE_MAP(CBusinessDiaryTInfo, CDialogBase)
	ON_BN_CLICKED(IDC_DIARY_TINFPERIOD_OUT_BTN, &CBusinessDiaryTInfo::OnBnClickedDiaryTinfperiodOutBtn)
END_MESSAGE_MAP()


// CBusinessDiaryTInfo ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CBusinessDiaryTInfo

-# �_�C�A���O����������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
BOOL CBusinessDiaryTInfo::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	mStartDate.SetTime(&mStartOpeTime);
	mEndDate.SetTime(&mEndOpeTime);

	mCbStation.ResetContent();
	//vector<CString>::iterator itr;
	//for (itr = mSpaceList.begin(); itr != mSpaceList.end(); itr++){
	//	mCbStation.AddString((*itr));
	//}
	//mCbStation.SetCurSel(0);

	// �����t�H�[�}�b�g�ύX
	mStartDate.SetFormat(_T("yyyy-MM-dd"));
	mStartTime.SetFormat(_T("HH:mm:ss"));
	mEndDate.SetFormat(_T("yyyy-MM-dd"));
	mEndTime.SetFormat(_T("HH:mm:ss"));

	// RF���t�@�C�������������m�F
	if (CheckFormatRFInfoFile() == FALSE)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	GetStartEndTimeinFile();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CBusinessDiaryTInfo

-# ����{�^�� ��������

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
#define UDSC_RFINFO_TMP_EXT _T(".vrxr")
#define USC_RFINFO_TMP_EXT _T(".vrxr2")
void CBusinessDiaryTInfo::OnBnClickedDiaryTinfperiodOutBtn()
{
	UpdateData(TRUE);

	// �J�n�����̎擾
	CTime sTime, eTime, tmpTime;
	mStartDate.GetTime(tmpTime);
	sTime = CTime(tmpTime.GetYear(), tmpTime.GetMonth(), tmpTime.GetDay(), m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());
	//	CString dbStr = sTime.Format("%Y-%m-%d %H:%M:%S");

	// �I�������̎擾
	mEndDate.GetTime(tmpTime);
	eTime = CTime(tmpTime.GetYear(), tmpTime.GetMonth(), tmpTime.GetDay(), m_EndTime.GetHour(), m_EndTime.GetMinute(), m_EndTime.GetSecond());
	//	dbStr = eTime.Format("%Y-%m-%d %H:%M:%S");

	if (eTime.GetTime() < sTime.GetTime())
	{
		AfxMessageBox(_T("�o�͏I���������o�͊J�n�������ߋ��ł��B\n�������������Ă��������B"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// �q���I���擾
	int i = mCbStation.GetCurSel();
	CString mStation;
	mCbStation.GetLBText(i, mStation);

	// ���M���t�@�C�����擾
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString filenamein = theApp.GetShareFilePath(eFileType_Diary_RFInfo_File, nStation);
	CString filenameout;

	switch (nStation)
	{
	case eStation_Usuda64:
	case eStation_Usuda642:
		filenameout = theApp.GetShareFilePath(eFileType_Diary_RFInfo_File, nStation) + UDSC_RFINFO_TMP_EXT;
		break;

	case eStation_Uchinoura34:
	case eStation_Uchinoura20:
		filenameout = theApp.GetShareFilePath(eFileType_Diary_RFInfo_File, nStation) + USC_RFINFO_TMP_EXT;
		break;

	default:
		filenameout = theApp.GetShareFilePath(eFileType_Diary_RFInfo_File, nStation) + UDSC_RFINFO_TMP_EXT;
		break;
	}

	FILE *fpin = NULL;
	FILE *fpout = NULL;

	char szRead[2048];
	TCHAR szWrite[2048];

	UpdateData();

	bool bWrite = false;

	TRY
	{
		// ���M���t�@�C���̃I�[�v��
		if ((_wfopen_s(&fpin, filenamein, _T("r")) != 0) || (fpin == NULL))
		{
			return;
		}
		if ((_wfopen_s(&fpout, filenameout, _T("w")) != 0) || (fpout == NULL))
		{
			fclose(fpin);
			return;
		}

		//		// �w�b�_�[�ǂݔ�΂�
		//		fgets(szRead, static_cast<unsigned>(_countof(szRead)), fpin);
		CString str, cbuf;
		BOOL bRead = TRUE;
		COleDateTime oleTime;
		CTime cTime;
		CString strSat;

		BOOL bFirst = TRUE;
		CString strSTime, strETime;
		char szBuf[2046];

		//		int nRfFormatType = GetRfFormatType();

		while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fpin) != NULL)
		{
			cbuf = CServerFile::ConvertUnicode(szRead);
			cbuf = cbuf.Mid(0, cbuf.GetLength() - 1);	// ���s�R�[�h�폜

			// �^�p��
			AfxExtractSubString(str, cbuf, IDX_RFINFO_DATE, ',');
			if (str == _T(""))
			{
				continue;
			}
			oleTime.ParseDateTime(str);
			cTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

			// �q����
			strSat = GetSatelliteName(cbuf);

			// �o�͏����m�F
			if (mStation == mAllText || mStation == strSat)
			{
				if (cTime >= sTime && cTime <= eTime)
				{
					if (bFirst == TRUE)
					{
						// �o�͊���
						strSTime = sTime.Format("%Y-%m-%d %H:%M:%S");
						strETime = eTime.Format("%Y-%m-%d %H:%M:%S");
						sprintf_s(szBuf, "PERIOD=%s�`%s\n", (LPCSTR)CStringA(strSTime), (LPCSTR)CStringA(strETime));
						fputs(szBuf, fpout);

						// �q����
						sprintf_s(szBuf, "SATELLITE=%s\n", (LPCSTR)CStringA(mStation));
						fputs(szBuf, fpout);

						bWrite = true;

						bFirst = FALSE;
					}

					//					// ���V�Y�̏ꍇ�́ACMD�ϒ��̏�Ԃ�10�Z����(9)�ɂ����邽�߁A7�Z����(6)�̉E�ɕt������
					//					if (nRfFormatType == eRfFormatType_USC)
					//					{
					//						CString strCMDSTATE2;
					//						AfxExtractSubString(strCMDSTATE2, cbuf, IDX_RFINFO_CMDSTATE2_USC, ',');
					//						CString cbufTmp1 = _T("");
					//						CString cbufTmp2 = _T("");
					//
					//						for (int i = 0; i <= (int)IDX_RFINFO_SATN_USC; i++)
					//						{ 
					//							CString cbufCol;
					//							AfxExtractSubString(cbufCol, cbuf, i, ',');
					//							if (i == IDX_RFINFO_CMDSTATE1_USC)
					//							{
					//								// ���V�Y�̏ꍇ��CMD�ϒ����
					//								CString cbufTmp3 = cbufCol + _T("-") + strCMDSTATE2;
					//								cbufCol = cbufTmp3;
					//							}
					//
					//							if (i == 0)
					//							{
					//								// 1�J������
					//								cbufTmp1 = cbufCol;
					//							}
					//							else if (i == IDX_RFINFO_CMDSTATE2_USC)
					//							{
					//								// 10�J�����ڂ�7�J�����ڂŃR�s�[�ς݂̂��߃R�s�[���Ȃ�
					//							}
					//							else
					//							{
					//								// ����ȊO
					//								cbufTmp1 = cbufTmp2 + _T(",") + cbufCol;
					//							}
					//							cbufTmp2 = cbufTmp1;
					//						}
					//
					//						cbuf = cbufTmp2;
					//					}

					// �o�͑Ώۂ����������̂ŏo�͂���
					wsprintf(szWrite, _T("%s\n"), (LPCTSTR)cbuf);
					CStringA abuf = CServerFile::ConvertEuc(szWrite);
					fputs(abuf, fpout);

					bWrite = true;
				}
			}
		}
	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fpin != NULL)
		fclose(fpin);
	if (fpout != NULL)
		fclose(fpout);

#if 1
	// ���[
	if(bWrite == true){
		CSatelliteData::ExcelOutPut(m_hWnd, filenameout);
	}
	else
	{
		AfxMessageBox(_T("�o�͑Ώۂ�������܂���ł����B"), MB_OK | MB_ICONEXCLAMATION);
	}

#else
	// ���
	CString title = _T("");
	title.Format(_T("���M��� : %s - %s �q����=%s"), (LPCTSTR)sTime.Format("%Y-%m-%d %H:%M:%S"), (LPCTSTR)eTime.Format("%Y-%m-%d %H:%M:%S"), (LPCTSTR)mStation);
	CPrintUtility::PrintEucFile(filenameout, title);
#endif
}

/*============================================================================*/
/*! CBusinessDiaryTInfo

-# �t�@�C����莞���擾

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CBusinessDiaryTInfo::GetStartEndTimeinFile()
{
	FILE *fp;

	CString filenamein = theApp.GetShareFilePath(eFileType_Diary_RFInfo_File, theApp.GetSelectStation());
	if ((_wfopen_s(&fp, filenamein, _T("r")) != 0) || (fp == NULL))
	{
		return;
	}

	char szRead[2048];

	CString str, cbuf;
	BOOL bRead = TRUE;
	BOOL bFirst = TRUE;
	COleDateTime oleTime;

	mSpaceList.clear();
	mSpaceList.push_back(mAllText);

	while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fp) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(szRead);
		cbuf = cbuf.Mid(0, cbuf.GetLength() - 1);

		if (bFirst == TRUE)
		{
			// �^�p��
			AfxExtractSubString(str, cbuf, IDX_RFINFO_DATE, ',');
			// �t�@�C���̊J�n����
			oleTime.ParseDateTime(str);
			mStartOpeTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

			bFirst = FALSE;
		}

		// �q�����擾�����X�g�ɒǉ�
		str = GetSatelliteName(cbuf);
		AddSpaceName(str);
	}

	if (fp != NULL)
		fclose(fp);

	// �t�@�C���̏I������
	AfxExtractSubString(str, cbuf, 0, ',');
	oleTime.ParseDateTime(str);
	mEndOpeTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

	str = mStartOpeTime.Format("%Y-%m-%d %H:%M:%S");
	str = mEndOpeTime.Format("%Y-%m-%d %H:%M:%S");

	// ��ʍX�V
	mStartDate.SetTime(&mStartOpeTime);
	m_StartTime = mStartOpeTime;

	mEndDate.SetTime(&mEndOpeTime);
	m_EndTime = mEndOpeTime;


	int cnt = (int)mSpaceList.size();
	for (int i = 0; i < cnt; ++i)
	{
		mCbStation.AddString(mSpaceList[i]);
	}
	mCbStation.SetCurSel(0);

	UpdateData(FALSE);
}

/*============================================================================*/
/*! CBusinessDiary

-# RF�t�@�C���̃t�H�[�}�b�g�`�F�b�N

@param  �Ȃ�
@retval TRUE�F���� / FALSE�F�ُ�
*/
/*============================================================================*/
BOOL CBusinessDiaryTInfo::CheckFormatRFInfoFile()
{
	CString filename = theApp.GetShareFilePath(eFileType_Diary_RFInfo_File, theApp.GetSelectStation());

	BOOL ret = TRUE;
	FILE *fp = NULL;

	CString title = _T("");
	GetWindowText(title);

	TRY
	{
		// RF���t�@�C���̃I�[�v��
		if ((_wfopen_s(&fp, filename, _T("r")) == 0) && (fp != NULL))
		{
			char szline[2048];
			CString cbuf = _T("");
			CString ccanma = _T("");
			CString cbufchk = _T("");
			int canmanum = 0;
			while (fgets(szline, static_cast<unsigned>(_countof(szline)), fp) != NULL && ret == TRUE)
			{
				cbuf = CServerFile::ConvertUnicode(szline);
				// �J���}�̐��`�F�b�N
				ccanma = cbuf;
				canmanum = ccanma.Remove(_T(','));
				if (canmanum != 13 && canmanum != 14)
				{
					ret = FALSE;
				}
				// �������`�F�b�N
				for (int i = 0; i < canmanum + 1 && ret == TRUE; i++)
				{
					AfxExtractSubString(cbufchk, cbuf, i, ',');
					if (cbufchk.GetLength() > 64)
					{
						ret = FALSE;
					}
				}
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format(_T("RF���t�@�C�����J���Ȃ����߁A���M���o�͊��ԓ��͉�ʂ��I�����܂��B\n%s"), filename);
			MessageBox(strMsg, title, MB_OK | MB_ICONWARNING);
			return FALSE;
		}

	}CATCH_ALL(e){
	}
	END_CATCH_ALL

	if (fp != NULL)
		fclose(fp);

	if (ret == FALSE)
	{
		CString strMsg;
		strMsg.Format(_T("RF���t�@�C���t�H�[�}�b�g���قȂ邽�߁A���M���o�͊��ԓ��͉�ʂ��I�����܂��B\n%s"), filename);
		MessageBox(strMsg, title, MB_OK | MB_ICONWARNING);
	}

	return ret;
}


void CBusinessDiaryTInfo::AddSpaceName(CString name)
{
	if (name == _T("-"))
		return;
	vector<CString>::iterator itr;
	for (itr = mSpaceList.begin(); itr != mSpaceList.end(); itr++){
		if ((*itr) == name)
			return;
	}
	mSpaceList.push_back(name);
}


/*============================================================================*/
/*! CBusinessDiaryTInfo

-# �Ǖʂ�RF�t�H�[�}�b�g�^�C�v�擾

@param  �Ȃ�
@retval RF�t�H�[�}�b�g�^�C�v
*/
/*============================================================================*/
int CBusinessDiaryTInfo::GetRfFormatType()
{
	int nRet = eRfFormatType_UDSC;
	UINT nStation = theApp.GetSelectStation();

	switch (nStation)
	{
	case eStation_Usuda64:
	case eStation_Usuda642:
		nRet = eRfFormatType_UDSC;
		break;

	case eStation_Uchinoura34:
	case eStation_Uchinoura20:
		nRet = eRfFormatType_USC;
		break;

	default:
		break;
	}

	return nRet;
}

/*============================================================================*/
/*! CBusinessDiaryTInfo

-# RF�t�H�[�}�b�g�^�C�v�ʂ̉q�����ʒu�擾

@param  �Ȃ�
@retval �q�����ʒu
*/
/*============================================================================*/
int CBusinessDiaryTInfo::GetRfSatellitePos()
{
	int nRfFormatType = GetRfFormatType();

	if (nRfFormatType == eRfFormatType_UDSC)
	{
		return IDX_RFINFO_SATN_UDSC;
	}
	if (nRfFormatType == eRfFormatType_USC)
	{
		return IDX_RFINFO_SATN_USC;
	}

	return IDX_RFINFO_SATN_UDSC;
}


/*============================================================================*/
/*! CBusinessDiaryTInfo

-# RF��񕶎���̈�ԉE�[����q�����擾

@param  RF��񕶎���
@retval �q�����ʒu
*/
/*============================================================================*/
CString CBusinessDiaryTInfo::GetSatelliteName(CString& cbuf)
{
	// �q�����͈�ԉE�̃J��������擾
	CString str;
	for (int i = 0;; i++)
	{
		CString cbufCol;
		if (AfxExtractSubString(cbufCol, cbuf, i, ',') == FALSE)
		{
			break;
		}
		str = cbufCol;
	}

	return str;
}