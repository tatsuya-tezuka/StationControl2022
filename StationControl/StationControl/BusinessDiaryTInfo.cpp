/*============================================================================*/
/*! BusinessDiaryTInfo.cpp

-# 無線業務日誌 送信情報出力期間入力画面
*/
/*============================================================================*/
// BusinessDiaryTInfo.cpp : 実装ファイル
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

// CBusinessDiaryTInfo ダイアログ

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


// CBusinessDiaryTInfo メッセージ ハンドラー


/*============================================================================*/
/*! CBusinessDiaryTInfo

-# ダイアログ初期化処理

@param  なし
@retval なし
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

	// 日時フォーマット変更
	mStartDate.SetFormat(_T("yyyy-MM-dd"));
	mStartTime.SetFormat(_T("HH:mm:ss"));
	mEndDate.SetFormat(_T("yyyy-MM-dd"));
	mEndTime.SetFormat(_T("HH:mm:ss"));

	// RF情報ファイルが正しいか確認
	if (CheckFormatRFInfoFile() == FALSE)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

	GetStartEndTimeinFile();

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CBusinessDiaryTInfo

-# 印刷ボタン 押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
#define UDSC_RFINFO_TMP_EXT _T(".vrxr")
#define USC_RFINFO_TMP_EXT _T(".vrxr2")
void CBusinessDiaryTInfo::OnBnClickedDiaryTinfperiodOutBtn()
{
	UpdateData(TRUE);

	// 開始時刻の取得
	CTime sTime, eTime, tmpTime;
	mStartDate.GetTime(tmpTime);
	sTime = CTime(tmpTime.GetYear(), tmpTime.GetMonth(), tmpTime.GetDay(), m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());
	//	CString dbStr = sTime.Format("%Y-%m-%d %H:%M:%S");

	// 終了時刻の取得
	mEndDate.GetTime(tmpTime);
	eTime = CTime(tmpTime.GetYear(), tmpTime.GetMonth(), tmpTime.GetDay(), m_EndTime.GetHour(), m_EndTime.GetMinute(), m_EndTime.GetSecond());
	//	dbStr = eTime.Format("%Y-%m-%d %H:%M:%S");

	if (eTime.GetTime() < sTime.GetTime())
	{
		AfxMessageBox(_T("出力終了時刻が出力開始時刻より過去です。\n時刻を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// 衛星選択取得
	int i = mCbStation.GetCurSel();
	CString mStation;
	mCbStation.GetLBText(i, mStation);

	// 送信情報ファイル名取得
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
		// 送信情報ファイルのオープン
		if ((_wfopen_s(&fpin, filenamein, _T("r")) != 0) || (fpin == NULL))
		{
			return;
		}
		if ((_wfopen_s(&fpout, filenameout, _T("w")) != 0) || (fpout == NULL))
		{
			fclose(fpin);
			return;
		}

		//		// ヘッダー読み飛ばし
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
			cbuf = cbuf.Mid(0, cbuf.GetLength() - 1);	// 改行コード削除

			// 運用日
			AfxExtractSubString(str, cbuf, IDX_RFINFO_DATE, ',');
			if (str == _T(""))
			{
				continue;
			}
			oleTime.ParseDateTime(str);
			cTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

			// 衛星名
			strSat = GetSatelliteName(cbuf);

			// 出力条件確認
			if (mStation == mAllText || mStation == strSat)
			{
				if (cTime >= sTime && cTime <= eTime)
				{
					if (bFirst == TRUE)
					{
						// 出力期間
						strSTime = sTime.Format("%Y-%m-%d %H:%M:%S");
						strETime = eTime.Format("%Y-%m-%d %H:%M:%S");
						sprintf_s(szBuf, "PERIOD=%s～%s\n", (LPCSTR)CStringA(strSTime), (LPCSTR)CStringA(strETime));
						fputs(szBuf, fpout);

						// 衛星名
						sprintf_s(szBuf, "SATELLITE=%s\n", (LPCSTR)CStringA(mStation));
						fputs(szBuf, fpout);

						bWrite = true;

						bFirst = FALSE;
					}

					//					// 内之浦の場合は、CMD変調の状態が10セル目(9)にもあるため、7セル目(6)の右に付加する
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
					//								// 内之浦の場合のCMD変調状態
					//								CString cbufTmp3 = cbufCol + _T("-") + strCMDSTATE2;
					//								cbufCol = cbufTmp3;
					//							}
					//
					//							if (i == 0)
					//							{
					//								// 1カラム目
					//								cbufTmp1 = cbufCol;
					//							}
					//							else if (i == IDX_RFINFO_CMDSTATE2_USC)
					//							{
					//								// 10カラム目は7カラム目でコピー済みのためコピーしない
					//							}
					//							else
					//							{
					//								// それ以外
					//								cbufTmp1 = cbufTmp2 + _T(",") + cbufCol;
					//							}
					//							cbufTmp2 = cbufTmp1;
					//						}
					//
					//						cbuf = cbufTmp2;
					//					}

					// 出力対象が見つかったので出力する
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
	// 帳票
	if(bWrite == true){
		CSatelliteData::ExcelOutPut(m_hWnd, filenameout);
	}
	else
	{
		AfxMessageBox(_T("出力対象が見つかりませんでした。"), MB_OK | MB_ICONEXCLAMATION);
	}

#else
	// 印刷
	CString title = _T("");
	title.Format(_T("送信情報 : %s - %s 衛星名=%s"), (LPCTSTR)sTime.Format("%Y-%m-%d %H:%M:%S"), (LPCTSTR)eTime.Format("%Y-%m-%d %H:%M:%S"), (LPCTSTR)mStation);
	CPrintUtility::PrintEucFile(filenameout, title);
#endif
}

/*============================================================================*/
/*! CBusinessDiaryTInfo

-# ファイルより時刻取得

@param  なし
@retval なし
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
			// 運用日
			AfxExtractSubString(str, cbuf, IDX_RFINFO_DATE, ',');
			// ファイルの開始時間
			oleTime.ParseDateTime(str);
			mStartOpeTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

			bFirst = FALSE;
		}

		// 衛星名取得しリストに追加
		str = GetSatelliteName(cbuf);
		AddSpaceName(str);
	}

	if (fp != NULL)
		fclose(fp);

	// ファイルの終了時間
	AfxExtractSubString(str, cbuf, 0, ',');
	oleTime.ParseDateTime(str);
	mEndOpeTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

	str = mStartOpeTime.Format("%Y-%m-%d %H:%M:%S");
	str = mEndOpeTime.Format("%Y-%m-%d %H:%M:%S");

	// 画面更新
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

-# RFファイルのフォーマットチェック

@param  なし
@retval TRUE：正常 / FALSE：異常
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
		// RF情報ファイルのオープン
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
				// カンマの数チェック
				ccanma = cbuf;
				canmanum = ccanma.Remove(_T(','));
				if (canmanum != 13 && canmanum != 14)
				{
					ret = FALSE;
				}
				// 文字数チェック
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
			strMsg.Format(_T("RF情報ファイルが開けないため、送信情報出力期間入力画面を終了します。\n%s"), filename);
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
		strMsg.Format(_T("RF情報ファイルフォーマットが異なるため、送信情報出力期間入力画面を終了します。\n%s"), filename);
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

-# 局別のRFフォーマットタイプ取得

@param  なし
@retval RFフォーマットタイプ
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

-# RFフォーマットタイプ別の衛星名位置取得

@param  なし
@retval 衛星名位置
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

-# RF情報文字列の一番右端から衛星名取得

@param  RF情報文字列
@retval 衛星名位置
*/
/*============================================================================*/
CString CBusinessDiaryTInfo::GetSatelliteName(CString& cbuf)
{
	// 衛星名は一番右のカラムから取得
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