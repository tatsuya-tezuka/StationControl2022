/*============================================================================*/
/*! BusinessDiaryPrint.cpp

-# 無線業務日誌 印刷期間入力画面
*/
/*============================================================================*/
// BusinessDiaryPrint.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "BusinessDiaryPrint.h"
#include "afxdialogex.h"
#include "BusinessDiary.h"
#include "HookMsgBox.h"

#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

#define	PRINT_FORM_COVER_FILE	_T("REP_FORM_COVER.txt")
#define	PRINT_FORM_HEADER_FILE	_T("REP_FORM_HEADER.txt")
#define	PRINT_FORM_LIST_FILE	_T("REP_FORM_LIST.txt")
#define	PRINT_FORM_CSV_FILE		_T("REP_FORM_CSV.txt")
#define	PRINT_COVER_FILE		_T("REP_COVER.txt")
#define	PRINT_HEADER_FILE		_T("REP_HEADER.txt")
#define	PRINT_LIST_FILE			_T("REP_LIST.txt")
#define	PRINT_LIST_TMP_FILE		_T("REP_LIST.vrxd")


// CBusinessDiaryPrint ダイアログ

IMPLEMENT_DYNAMIC(CBusinessDiaryPrint, CDialogBase)

CBusinessDiaryPrint::CBusinessDiaryPrint(CWnd* pParent /*=NULL*/)
: CDialogBase(CBusinessDiaryPrint::IDD, pParent)
, mSendCount(FALSE)
{
	m_pParent = pParent;
}

CBusinessDiaryPrint::~CBusinessDiaryPrint()
{
}

void CBusinessDiaryPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DIARY_PRINTPERIOD_START_DATE, mStartDate);
	DDX_Control(pDX, IDC_DIARY_PRINTPERIOD_END_DATE, mEndDate);
	DDX_Control(pDX, IDC_DIARY_PRINTPERIOD_STLT_CMB, mCbStation);
	DDX_Check(pDX, IDC_DIARY_PRINTPERIOD_TCNT_CHECK, mSendCount);
}


BEGIN_MESSAGE_MAP(CBusinessDiaryPrint, CDialogBase)
	ON_BN_CLICKED(IDC_DIARY_PRINTPERIOD_PRNT_BTN, &CBusinessDiaryPrint::OnBnClickedDiaryPrintperiodPrntBtn)
END_MESSAGE_MAP()


// CBusinessDiaryPrint メッセージ ハンドラー


/*============================================================================*/
/*! CBusinessDiaryPrint

-# ダイアログ初期化処理

@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CBusinessDiaryPrint::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	mStartDate.SetTime(&mStartOpeTime);
	mEndDate.SetTime(&mEndOpeTime);

	mCbStation.ResetContent();
	vector<CString>::iterator itr;
	for (itr = mSpaceList.begin(); itr != mSpaceList.end(); itr++){
//		mCbStation.AddString((*itr));
		mCbStation.InsertString(-1, (*itr));
	}
	mCbStation.SetCurSel(0);

	// 日時フォーマット変更
	mStartDate.SetFormat(_T("yyyy-MM-dd"));;
	mEndDate.SetFormat(_T("yyyy-MM-dd"));;

	CenterWindowEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


/*============================================================================*/
/*! CBusinessDiaryPrint

-# 印刷ボタン押下処理

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryPrint::OnBnClickedDiaryPrintperiodPrntBtn()
{
#if 1	// 帳票
	UpdateData();

	// 印刷開始/終了日取得
	CTime sDate, eDate;
	mStartDate.GetTime(sDate);
	mEndDate.GetTime(eDate);

	if (eDate.GetTime() < sDate.GetTime())
	{
		AfxMessageBox(_T("印刷終了日が印刷開始日より過去です。\n印刷日を見直してください。"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// 印刷対象クリア
	mPrintList.clear();

	// 衛星名取得
	CString strSatName;
	int idx = mCbStation.GetCurSel();
	if (idx == CB_ERR)
	{
		return;
	}
	mCbStation.GetLBText(idx, strSatName);

	// フォームディレクトリ取得
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString formDir = theApp.GetShareFilePath(eFileType_Diary_Form_Folder, nStation);


	// 日誌データ取得
	vector<stDiary> &DiaryData = ((CBusinessDiary*)m_pParent)->GetDialyDataList();

	int satCnt = 2;
	if (strSatName == mAllText)
	{
		satCnt = mCbStation.GetCount();
	}

	CString satname;
	//	TCHAR szWrite[2048];
	char szWriteCsv[2048];
	CString cbuf;
	CStringA wbuf;

	stDiary st;

	CString FormCoverFile, FormHeaderFile, FormListDir, FormListDirEn;
	CString CoverFile, HeaderFile, ListFile;
	CString ListCsvFile;

	vector<stPrintFile> printlist;
	map<CString, bool> prisatlist;

	for (int i = 1; i < satCnt; ++i)
	{
		// 衛星名取得
		if (strSatName == mAllText)
		{
			mCbStation.GetLBText(i, satname);
		}
		else
		{
			mCbStation.GetLBText(mCbStation.GetCurSel(), satname);
		}

		CString strSatDir = _T("");

		// 衛星番号取得
		int satidx = ((CBusinessDiary*)m_pParent)->GetIndexEqSatDBData(satname);
		if (satidx == -1)
		{
			// 対象外
			continue;
		}

		int PrintYear = 0;
		int oldPrintYear = 0;
		int PrintMonth = 0;
		int oldPrintMonth = 0;
		int PrintDay = 0;
		int oldPrintDay = 0;
		COleDateTime oleTime;
		CTime cTime;

		FILE *fpFCover = NULL;
		FILE *fpFHeader = NULL;
		FILE *fpFList = NULL;
		FILE *fpList = NULL;
		FILE *fpListCsv = NULL;

		BOOL bOpenListFile = FALSE;

		int allTime = 0;
		CTime printSTime = 0;
		CTime printETime = 0;

		// 各ファイルフォーマット取得
		vector<CStringA> FormListList;
		FormListDir.Format(_T("%s%s"), (LPCTSTR)formDir, (LPCTSTR)satname);
		FormListDirEn.Format(_T("%s%s\\"), (LPCTSTR)formDir, (LPCTSTR)satname.MakeLower());
//		FormListFile.Format(_T("%s%s\\%s"), (LPCTSTR)formDir, (LPCTSTR)satname, (LPCTSTR)PRINT_FORM_LIST_FILE);
		if (PathIsDirectory(FormListDirEn) == FALSE)
		{
			MakeSureDirectoryPathExists((PCSTR)CStringA(FormListDirEn));
		}

		stPrintFile stPrint;
		stPrint.dir = FormListDir;

		// 日誌データ走査
		CString oldSatname = _T("");
		int dcnt = (int)DiaryData.size();
		for (int d = 0; d < dcnt; ++d)
		{
			st = DiaryData[d];
			// 相手局が衛星名以外の場合は、印刷対象外とする。
			if ((((CBusinessDiary*)m_pParent)->GetIndexEqSatDBData(st.data[0].station) == -1)
				&& (((CBusinessDiary*)m_pParent)->GetIndexEqSatDBData(st.data[1].station) == -1)
				&& (((CBusinessDiary*)m_pParent)->GetIndexEqSatDBData(st.data[2].station) == -1) )
			{
				continue;
			}

			// 送信回数が全て0の場合は印刷対象外とする。
			if ( (st.data[0].count == _T("0"))
				&& (st.data[1].count == _T("0"))
				&& (st.data[2].count == _T("0")) )
			{
				continue;
			}

			// 日時確認
			oleTime.ParseDateTime(st.date);
			cTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), 0, 0, 0);

			if ( !(cTime >= sDate && cTime <= eDate) )
			{
				continue;
			}

			// 衛星名確認
			if ( !((satname.CompareNoCase(st.data[0].station) == 0)
				|| (satname.CompareNoCase(st.data[1].station) == 0)
				|| (satname.CompareNoCase(st.data[2].station) == 0)) )
			{
				continue;
			}

			// 年月日
			PrintYear = cTime.GetYear();
			PrintMonth = cTime.GetMonth();
			PrintDay = cTime.GetDay();

			// 曜日
			CString strDayOfWeek = DefDayOfWeek[cTime.GetDayOfWeek() - 1];

			// 月替わり
			if (!((PrintYear == oldPrintYear) && (PrintMonth == oldPrintMonth)))
			{
				// List ファイルクローズ
				if (fpListCsv != NULL)
				{
					WriteCoverCsv(fpListCsv, formDir, satname, oldPrintYear, oldPrintMonth, printSTime.GetDay(), printETime.GetDay(), allTime);
					fclose(fpListCsv);
					bOpenListFile = FALSE;

					printlist.push_back(stPrint);
					prisatlist[satname] = true;
				}

				// Listファイルオープン
				ListCsvFile.Format(_T("%s%s\\%04d_%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, PrintYear, PrintMonth, (LPCTSTR)PRINT_LIST_TMP_FILE);
				if (_wfopen_s(&fpListCsv, ListCsvFile, _T("w")) == 0)
				{
					bOpenListFile = TRUE;

					printSTime = 0;
					printETime = 0;
					allTime = 0;

					stPrint.file = ListCsvFile;
				}
			}

			// List 出力
			if (bOpenListFile == TRUE)
			{
				for (int phaseT = 0; phaseT < 3; phaseT++)
				{
					// 各項のデータ無しまたは'-'文字を'-'連続文字列に変換する
					((CBusinessDiary*)m_pParent)->ExchgNoneData2Long(st.data[phaseT], st.data[phaseT]);
				}

				// 1
				CString ref,strCnt;
				AfxExtractSubString(ref, st.info3, 0, '\\');

				CString strTime = cTime.Format("%Y-%m-%d");
				sprintf_s(szWriteCsv, "%s,(%s),%s,%-3s:%s,%-3s:%s,%s,%02d時%02d分,%s,%s,%s\n", (LPCSTR)CStringA(strTime), (LPCSTR)CStringA(strDayOfWeek), (LPCSTR)CStringA(st.data[0].weather), (LPCSTR)CStringA(st.data[0].capa1), (LPCSTR)CStringA(st.data[0].name1),
					(LPCSTR)CStringA(st.data[0].capa2), (LPCSTR)CStringA(st.data[0].name2), (LPCSTR)CStringA(st.data[0].station), _wtoi(st.data[0].time) / 60, _wtoi(st.data[0].time) % 60,
					(LPCSTR)CStringA(st.info1), (LPCSTR)CStringA(st.info2), (LPCSTR)CStringA(ref));
				fputs(szWriteCsv, fpListCsv);

				AfxExtractSubString(ref, st.info3, 1, '\\');
				strCnt = _T("");
				if (mSendCount == TRUE)
				{
					strCnt.Format(_T("(%s回)"), (LPCTSTR)st.data[0].count);
				}
				sprintf_s(szWriteCsv, ",,,,%-3s:%s,,%s,,,%s\n", (LPCSTR)CStringA(st.data[0].capa3), (LPCSTR)CStringA(st.data[0].name3), (LPCSTR)CStringA(strCnt), (LPCSTR)CStringA(ref));
				fputs(szWriteCsv, fpListCsv);

				// 2
				AfxExtractSubString(ref, st.info3, 2, '\\');
				sprintf_s(szWriteCsv, ",,%s,%-3s:%s,%-3s:%s,%s,%02d時%02d分,%s,,\n", (LPCSTR)CStringA(st.data[1].weather), (LPCSTR)CStringA(st.data[1].capa1), (LPCSTR)CStringA(st.data[1].name1),
					(LPCSTR)CStringA(st.data[1].capa2), (LPCSTR)CStringA(st.data[1].name2), (LPCSTR)CStringA(st.data[1].station), _wtoi(st.data[1].time) / 60, _wtoi(st.data[1].time) % 60, (LPCSTR)CStringA(ref));
				fputs(szWriteCsv, fpListCsv);

				AfxExtractSubString(ref, st.info3, 3, '\\');
				strCnt = _T("");
				if (mSendCount == TRUE)
				{
					strCnt.Format(_T("(%s回)"), (LPCTSTR)st.data[1].count);
				}
				sprintf_s(szWriteCsv, ",,,,%-3s:%s,,%s,,,%s\n", (LPCSTR)CStringA(st.data[1].capa3), (LPCSTR)CStringA(st.data[1].name3), (LPCSTR)CStringA(strCnt), (LPCSTR)CStringA(ref));
				fputs(szWriteCsv, fpListCsv);

				// 3
				AfxExtractSubString(ref, st.info3, 4, '\\');
				sprintf_s(szWriteCsv, ",,%s,%-3s:%s,%-3s:%s,%s,%02d時%02d分,%s,,\n", (LPCSTR)CStringA(st.data[2].weather), (LPCSTR)CStringA(st.data[2].capa1), (LPCSTR)CStringA(st.data[2].name1),
					(LPCSTR)CStringA(st.data[2].capa2), (LPCSTR)CStringA(st.data[2].name2), (LPCSTR)CStringA(st.data[2].station), _wtoi(st.data[2].time) / 60, _wtoi(st.data[2].time) % 60, (LPCSTR)CStringA(ref));
				fputs(szWriteCsv, fpListCsv);

				AfxExtractSubString(ref, st.info3, 5, '\\');
				strCnt = _T("");
				if (mSendCount == TRUE)
				{
					strCnt.Format(_T("(%s回)"), (LPCTSTR)st.data[2].count);
				}
				sprintf_s(szWriteCsv, ",,,,%-3s:%s,,%s,,,%s\n", (LPCSTR)CStringA(st.data[2].capa3), (LPCSTR)CStringA(st.data[2].name3), (LPCSTR)CStringA(strCnt), (LPCSTR)CStringA(ref));
				fputs(szWriteCsv, fpListCsv);

				// 延通信時間
				allTime += _wtoi(st.data[0].time) + _wtoi(st.data[1].time) + _wtoi(st.data[2].time);

				// 印刷開始日　月毎
				if (printSTime == 0)
				{
					printSTime = cTime;
				}

				// 印刷終了日　月毎
				printETime = cTime;

			}

			oldPrintYear = PrintYear;
			oldPrintMonth = PrintMonth;
			oldPrintDay = PrintDay;
			oldSatname = satname;
		}

		// List ファイルクローズ
		if (fpListCsv != NULL)
		{
			WriteCoverCsv(fpListCsv, formDir, satname, PrintYear, PrintMonth, printSTime.GetDay(), printETime.GetDay(), allTime);
			fclose(fpListCsv);

			printlist.push_back(stPrint);
			prisatlist[satname] = true;
		}
	}

	// 帳票出力確認ダイアログ
	CString strOutMsg;
	strOutMsg.Format(_T("無線業務日誌帳票を%I64uファイル、Excel出力します。\n\n出力対象期間:\n%04d-%02d-%02d ～ %04d-%02d-%02d\n\n"), printlist.size(), 
		sDate.GetYear(), sDate.GetMonth(), sDate.GetDay(), eDate.GetYear(), eDate.GetMonth(), eDate.GetDay());
	strOutMsg += _T("出力対象衛星:\n");

#if 0
	CString strSatTest[] = { _T("aaaaa"), _T("bbbbb"), _T("ccccc"), _T("ddddd"), _T("eeeee"),
		_T("fffff"), _T("ggggg"), _T("hhhhh"), _T("iiiii"), _T("jjjjj"),
		_T("kkkkk"), _T("lllll"), _T("mmmmm"), _T("nnnnn"), _T("ooooo"),
		_T("ppppp"), _T("qqqqq"), _T("rrrrr"), _T("sssss"), _T("ttttt"),
		_T("uuuuu"), _T("vvvvv"), _T("wwwww"), _T("xxxxx"), _T("yyyyy"),
		_T("zzzzz"), _T("12345678910"), _T("irohanihoheto"), _T("aabbccddeeff"), _T("abcdefg")
	};
	for (int i = 0; i < 30; i++)
	{
		prisatlist[strSatTest[i]] = true;
	}
#endif

	int nSatCnt = 0;
	for (auto itr = prisatlist.begin(); itr != prisatlist.end(); itr++, nSatCnt++)
	{
		strOutMsg += itr->first;
		if (nSatCnt < prisatlist.size() - 1)
		{
			strOutMsg += _T(", ");
		}
	}

	strOutMsg += _T("\n\n出力した帳票ファイルは、以下のフォルダ直下にある衛星別フォルダを参照してください。\n") + formDir + _T("\n\n帳票出力してよろしいですか？");

	if (IDCANCEL == AfxMessageBoxHooked(strOutMsg, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		return;
	}

	// 帳票出力
	CString	strParam;
	for (int i = 0; i<printlist.size(); ++i)
	{
		strParam.Format(_T("%s /O%s"), (LPCTSTR)printlist[i].file, (LPCTSTR)printlist[i].dir);
//		CSatelliteData::ExcelOutPut(m_hWnd, strParam);
//		Sleep(1);
		CSatelliteData::ExcelOutPutProc(m_hWnd, strParam);
	}

#else
	UpdateData();

	// 印刷対象クリア
	mPrintList.clear();

	// 衛星名取得
	CString strSatName;
	int idx = mCbStation.GetCurSel();
	if (idx == CB_ERR)
	{
		return;
	}
	mCbStation.GetLBText(idx, strSatName);

	// 印刷開始/終了日取得
	CTime sDate,eDate;
	mStartDate.GetTime(sDate);
	mEndDate.GetTime(eDate);

	// フォームディレクトリ取得
	UINT nStation = theApp.GetSelectStation();
	if (theApp.GetSelectStationMode() == eStation_5)
		nStation = 0;

	CString formDir = theApp.GetShareFilePath(eFileType_Diary_Form_Folder, nStation);


	// 日誌データ取得
	vector<stDiary> &DiaryData = ((CBusinessDiary*)m_pParent)->GetDialyDataList();

	int satCnt = 2;
	if (strSatName == mAllText)
	{
		satCnt = mCbStation.GetCount();
	}

	CString satname;
	char szRead[2048];
//	TCHAR szWrite[2048];
	char szWrite[2048];
	char szWriteCsv[2048];
	CString cbuf;
	CStringA wbuf;

	stDiary st, stNext;

	for (int i = 1; i < satCnt; ++i)
	{
		// 衛星名取得
		if (strSatName == mAllText)
		{
			mCbStation.GetLBText(i, satname);
		}
		else
		{
			mCbStation.GetLBText(mCbStation.GetCurSel(), satname);
		}

		CString strSatDir = _T("");

		// 衛星番号取得
		int satidx = ((CBusinessDiary*)m_pParent)->GetIndexEqSatDBData(satname);
		if (satidx == -1)
		{
			// 対象外
			continue;
		}

		int PrintYear = 0;
		int oldPrintYear = 0;
		int PrintMonth = 0;
		int oldPrintMonth = 0;
		int PrintDay = 0;
		int oldPrintDay = 0;
		COleDateTime oleTime;
		CTime cTime;
		CString FormCoverFile, FormHeaderFile, FormListFile;
		CString CoverFile, HeaderFile, ListFile;
		CString ListCsvFile;

		FILE *fpFCover = NULL;
		FILE *fpFHeader = NULL;
		FILE *fpFList = NULL;
		FILE *fpList = NULL;
		FILE *fpListCsv = NULL;

		BOOL bOpenListFile = FALSE;

		int allTime = 0;
		CTime printSTime = 0;
		CTime printETime = 0;

		// 各ファイルフォーマット取得
		vector<CStringA> FormListList;
		FormListFile.Format(_T("%s%s\\%s"), (LPCTSTR)formDir, (LPCTSTR)satname, (LPCTSTR)PRINT_FORM_LIST_FILE);


		// 日誌データ走査
		CString oldSatname = _T("");
		int dcnt = (int)DiaryData.size();
		for (int d = 0; d < dcnt; ++d)
		{
			st = DiaryData[d];

			// 相手局が衛星名以外の場合は、印刷対象外とする。
			if ((((CBusinessDiary*)m_pParent)->GetIndexEqSatDBData(st.data[0].station) == -1)
				&& (((CBusinessDiary*)m_pParent)->GetIndexEqSatDBData(st.data[1].station) == -1)
				&& (((CBusinessDiary*)m_pParent)->GetIndexEqSatDBData(st.data[2].station) == -1) )
			{
				continue;
			}

			// 送信回数が全て0の場合は印刷対象外とする。
			if ( (st.data[0].count == _T("0"))
				&& (st.data[1].count == _T("0"))
				&& (st.data[2].count == _T("0")) )
			{
				continue;
			}

			// 日時確認
			oleTime.ParseDateTime(st.date);
			cTime = CTime(oleTime.GetYear(), oleTime.GetMonth(), oleTime.GetDay(), 0, 0, 0);

			if ( !(cTime >= sDate && cTime <= eDate) )
			{
				continue;
			}

			// 衛星名確認
			if ( !((satname.CompareNoCase(st.data[0].station) == 0)
				|| (satname.CompareNoCase(st.data[1].station) == 0)
				|| (satname.CompareNoCase(st.data[2].station) == 0)) )
			{
				continue;
			}

			// フォーマット取得
			if (oldSatname != satname)
			{
				// List Format
				if (_wfopen_s(&fpFList, FormListFile, _T("r")) != 0)
				{
					continue;
				}

				while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fpFList) != NULL)
				{
					cbuf = CServerFile::ConvertUnicode(szRead);
					FormListList.push_back(CStringA(cbuf));

//					CStringA strTmp = CStringA(szRead);
//					FormListList.push_back(strTmp);
				}
				fclose(fpFList);
			}

			// 年月日
			PrintYear = cTime.GetYear();
			PrintMonth = cTime.GetMonth();
			PrintDay = cTime.GetDay();

			// 曜日
			CString strDayOfWeek = DefDayOfWeek[cTime.GetDayOfWeek() - 1];

			// 月替わり
			if (!((PrintYear == oldPrintYear) && (PrintMonth == oldPrintMonth)))
			{
				// List ファイルクローズ
				if (fpList != NULL)
				{
					fclose(fpList);
					bOpenListFile = FALSE;
				}

				if (fpListCsv != NULL)
				{
					WriteCoverCsv(fpListCsv, formDir, satname, oldPrintYear, oldPrintMonth, printSTime.GetDay(), printETime.GetDay(), allTime);
					fclose(fpListCsv);
				}

				// Coverファイル、Headerファイル作成
				if (printSTime != 0 && printETime != 0)
				{
					WriteCoverAndHeader(formDir, satname, oldPrintYear, oldPrintMonth, printSTime.GetDay(), printETime.GetDay(), allTime);
				}

				// Listファイルオープン
				ListFile.Format(_T("%s%s\\%04d_%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, PrintYear, PrintMonth, (LPCTSTR)PRINT_LIST_FILE);
				if (_wfopen_s(&fpList, ListFile, _T("w")) == 0)
				{
					bOpenListFile = TRUE;

					printSTime = 0;
					printETime = 0;
					allTime = 0;
				}
			
				ListCsvFile.Format(_T("%s%s\\%04d_%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, PrintYear, PrintMonth, (LPCTSTR)PRINT_LIST_CSV_FILE);
				if (_wfopen_s(&fpListCsv, ListCsvFile, _T("w")) == 0)
				{
					//
				}
			}

			// List 出力
			if (bOpenListFile == TRUE)
			{
				// 1
				CString ref,strCnt;
				AfxExtractSubString(ref, st.info3, 0, '\\');
				sprintf_s(szWrite, FormListList[0], PrintDay, CStringA(strDayOfWeek), CStringA(st.data[0].weather), CStringA(st.data[0].capa1), CStringA(st.data[0].name1),
					CStringA(st.data[0].capa2), CStringA(st.data[0].name2), CStringA(st.data[0].station), _wtoi(st.data[0].time) / 60, _wtoi(st.data[0].time) % 60,
					CStringA(st.info1), CStringA(st.info2), CStringA(ref));
				fputs(szWrite, fpList);

#if 1
				CString strTime = cTime.Format("%Y-%m-%d");
				sprintf_s(szWriteCsv, "%s,(%s),%s,%-3s:%s,%-3s:%s,%s,%02d時%02d分,%s,%s,%s\n", CStringA(strTime), CStringA(strDayOfWeek), CStringA(st.data[0].weather), CStringA(st.data[0].capa1), CStringA(st.data[0].name1),
					CStringA(st.data[0].capa2), CStringA(st.data[0].name2), CStringA(st.data[0].station), _wtoi(st.data[0].time) / 60, _wtoi(st.data[0].time) % 60,
					CStringA(st.info1), CStringA(st.info2), CStringA(ref));
				fputs(szWriteCsv, fpListCsv);
#endif

				AfxExtractSubString(ref, st.info3, 1, '\\');
				strCnt = _T("");
				if (mSendCount == TRUE)
				{
					strCnt.Format(_T("(%s回)"), (LPCTSTR)st.data[0].count);
				}
				sprintf_s(szWrite, FormListList[1], CStringA(st.data[0].capa3), CStringA(st.data[0].name3), CStringA(strCnt), CStringA(ref));
				fputs(szWrite, fpList);

#if 1
				sprintf_s(szWriteCsv, ",,,,%-3s:%s,,%s,,,%s\n", CStringA(st.data[0].capa3), CStringA(st.data[0].name3), CStringA(strCnt), CStringA(ref));
				fputs(szWriteCsv, fpListCsv);
#endif

				// 2
				AfxExtractSubString(ref, st.info3, 2, '\\');
				sprintf_s(szWrite, FormListList[2], CStringA(st.data[1].weather), CStringA(st.data[1].capa1), CStringA(st.data[1].name1),
					CStringA(st.data[1].capa2), CStringA(st.data[1].name2), CStringA(st.data[1].station), _wtoi(st.data[1].time) / 60, _wtoi(st.data[1].time) % 60, CStringA(ref));
				fputs(szWrite, fpList);

#if 1
				sprintf_s(szWriteCsv, ",,%s,%-3s:%s,%-3s:%s,%s,%02d時%02d分,%s,,\n", CStringA(st.data[1].weather), CStringA(st.data[1].capa1), CStringA(st.data[1].name1),
					CStringA(st.data[1].capa2), CStringA(st.data[1].name2), CStringA(st.data[1].station), _wtoi(st.data[1].time) / 60, _wtoi(st.data[1].time) % 60, CStringA(ref));
				fputs(szWriteCsv, fpListCsv);
#endif

				AfxExtractSubString(ref, st.info3, 3, '\\');
				strCnt = _T("");
				if (mSendCount == TRUE)
				{
					strCnt.Format(_T("(%s回)"), (LPCTSTR)st.data[1].count);
				}
				sprintf_s(szWrite, FormListList[3], CStringA(st.data[1].capa3), CStringA(st.data[1].name3), CStringA(strCnt), CStringA(ref));
				fputs(szWrite, fpList);

#if 1
				sprintf_s(szWriteCsv, ",,,,%-3s:%s,,%s,,,%s\n", CStringA(st.data[1].capa3), CStringA(st.data[1].name3), CStringA(strCnt), CStringA(ref));
				fputs(szWriteCsv, fpListCsv);
#endif

				// 3
				AfxExtractSubString(ref, st.info3, 4, '\\');
				sprintf_s(szWrite, FormListList[4], CStringA(st.data[2].weather), CStringA(st.data[2].capa1), CStringA(st.data[2].name1),
					CStringA(st.data[2].capa2), CStringA(st.data[2].name2), CStringA(st.data[2].station), _wtoi(st.data[2].time) / 60, _wtoi(st.data[2].time) % 60, CStringA(ref));
				fputs(szWrite, fpList);

#if 1
				sprintf_s(szWriteCsv, ",,%s,%-3s:%s,%-3s:%s,%s,%02d時%d02分,%s,,\n", CStringA(st.data[2].weather), CStringA(st.data[2].capa1), CStringA(st.data[2].name1),
					CStringA(st.data[2].capa2), CStringA(st.data[2].name2), CStringA(st.data[2].station), _wtoi(st.data[2].time) / 60, _wtoi(st.data[2].time) % 60, CStringA(ref));
				fputs(szWriteCsv, fpListCsv);
#endif

				AfxExtractSubString(ref, st.info3, 5, '\\');
				strCnt = _T("");
				if (mSendCount == TRUE)
				{
					strCnt.Format(_T("(%s回)"), (LPCTSTR)st.data[2].count);
				}
				sprintf_s(szWrite, FormListList[5], CStringA(st.data[2].capa3), CStringA(st.data[2].name3), CStringA(strCnt), CStringA(ref));
				fputs(szWrite, fpList);

#if 1
				sprintf_s(szWriteCsv, ",,,,%-3s:%s,,%s,,,%s\n", CStringA(st.data[2].capa3), CStringA(st.data[2].name3), CStringA(strCnt), CStringA(ref));
				fputs(szWriteCsv, fpListCsv);
#endif

				sprintf_s(szWrite, "%s", (LPCSTR)FormListList[6]);
				fputs(szWrite, fpList);

				// 延通信時間
				allTime += _wtoi(st.data[0].time) + _wtoi(st.data[1].time) + _wtoi(st.data[2].time);

				// 印刷開始日　月毎
				if (printSTime == 0)
				{
					printSTime = cTime;
				}

				// 印刷終了日　月毎
				printETime = cTime;

			}

			oldPrintYear = PrintYear;
			oldPrintMonth = PrintMonth;
			oldPrintDay = PrintDay;
			oldSatname = satname;
		}

		// List ファイルクローズ
		if (fpList != NULL)
		{
			fclose(fpList);
			bOpenListFile = FALSE;
		}

		if (fpListCsv != NULL)
		{
			WriteCoverCsv(fpListCsv, formDir, satname, PrintYear, PrintMonth, printSTime.GetDay(), printETime.GetDay(), allTime);
			fclose(fpListCsv);
		}

		if (printSTime != 0 && printETime != 0)
		{
			WriteCoverAndHeader(formDir, satname, PrintYear, PrintMonth, printSTime.GetDay(), printETime.GetDay(), allTime);
		}
	}

	// 印刷
	int pcnt = (int)mPrintList.size();
	if (pcnt > 0)
	{
		CString title = _T("OFF");
		CPrintUtility::PrintSjisFileList(mPrintList, title);
	}
	else
	{
		CString title = _T("無線業務日誌");
		MessageBox(_T("印刷対象はありません。"), title, MB_OK | MB_ICONINFORMATION);
	}
#endif
}

/*============================================================================*/
/*! CBusinessDiaryPrint

-# Cover、Header部ファイル出力

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryPrint::WriteCoverAndHeader(CString formDir, CString satname, int iYear, int iMonth, int iSDay, int iEDay, int iAllTime)
{
	FILE *fpCvrIn;
	FILE *fpCvrOut;
	FILE *fpHdrIn;
	FILE *fpHdrOut;

	CString FormCoverFile, CoverFile;
	CString FormHeaderFile, HeaderFile;

	char	szRead[2048];
	char	szWrite[2048];
	CString	cbuf;

	vector<CStringA> FormCoverList;
	vector<CStringA> FormHeaderList;

	//-------------------------------------------------------
	// Cover Format
	FormCoverFile.Format(_T("%s%s\\%s"), (LPCTSTR)formDir, (LPCTSTR)satname, (LPCTSTR)PRINT_FORM_COVER_FILE);
	if ((_wfopen_s(&fpCvrIn, FormCoverFile, _T("r")) != 0) || (fpCvrIn == NULL))
	{
		return;
	}
	while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fpCvrIn) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(szRead);
		FormCoverList.push_back(CStringA(cbuf));
	}
	fclose(fpCvrIn);

	// Cover 出力
	CoverFile.Format(_T("%s%s\\%04d_%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, iYear, iMonth, (LPCTSTR)PRINT_COVER_FILE);
	if ((_wfopen_s(&fpCvrOut, CoverFile, _T("w")) == 0) && (fpCvrOut != NULL))
	{
		int ccnt = (int)FormCoverList.size();
		for (int c = 0; c < ccnt; ++c)
		{
			if (c == 3)	// 自
			{
				sprintf_s(szWrite, FormCoverList[c], (iYear - 1988), iMonth, iSDay);
				fputs(szWrite, fpCvrOut);
			}
			else if (c == 4)	// 至
			{
				sprintf_s(szWrite, FormCoverList[c], (iYear - 1988), iMonth, iEDay);
				fputs(szWrite, fpCvrOut);
			}
			else if (c == 11)	// 延通信時間
			{
				sprintf_s(szWrite, FormCoverList[c], iAllTime / 60, iAllTime % 60);
				fputs(szWrite, fpCvrOut);
			}
			else
			{
				sprintf_s(szWrite, "%s", (LPCSTR)FormCoverList[c]);
				fputs(szWrite, fpCvrOut);
			}
		}
		fclose(fpCvrOut);
	}

	//-------------------------------------------------------
	// Header Format
	FormHeaderFile.Format(_T("%s%s\\%s"), (LPCTSTR)formDir, (LPCTSTR)satname, (LPCTSTR)PRINT_FORM_HEADER_FILE);
	if ((_wfopen_s(&fpHdrIn, FormHeaderFile, _T("r")) != 0) || (fpHdrIn == NULL))
	{
		return;
	}
	while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fpHdrIn) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(szRead);
		FormHeaderList.push_back(CStringA(cbuf));
	}
	fclose(fpHdrIn);

	// Header 出力
	HeaderFile.Format(_T("%s%s\\%04d_%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, iYear, iMonth, (LPCTSTR)PRINT_HEADER_FILE);
	if ((_wfopen_s(&fpHdrOut, HeaderFile, _T("w")) == 0) && (fpHdrOut != NULL))
	{
		int hcnt = (int)FormHeaderList.size();
		for (int h = 0; h < hcnt; ++h)
		{
			if (h == 5)
			{
				sprintf_s(szWrite, FormHeaderList[h], iMonth);
				fputs(szWrite, fpHdrOut);
			}
			else
			{
				sprintf_s(szWrite, "%s", (LPCSTR)FormHeaderList[h]);
				fputs(szWrite, fpHdrOut);
			}
		}
		fclose(fpHdrOut);
	}


	// 印刷対象取得
	CreatePrintList(formDir, satname, iYear, iMonth);
}

void CBusinessDiaryPrint::WriteCoverCsv(FILE *fp, CString formDir, CString satname, int iYear, int iMonth, int iSDay, int iEDay, int iAllTime)
{
	char szWrite[2046];

	// #HEADER
	strcpy_s(szWrite, sizeof(szWrite), "\n#HEADER\n");
	fputs(szWrite, fp);

	// 出力期間
	CString strStartDay, strEndDay;
	strStartDay.Format(_T("%04d-%02d-%02d"), iYear, iMonth, iSDay);
	strEndDay.Format(_T("%04d-%02d-%02d"), iYear, iMonth, iEDay);

	sprintf_s(szWrite, "PERIOD=%s～%s\n", (LPCSTR)CStringA(strStartDay), (LPCSTR)CStringA(strEndDay));
	fputs(szWrite, fp);

	// 局名
	sprintf_s(szWrite, "STATION=%s\n", (LPCSTR)CStringA(CString(mStationString[theApp.GetSelectStation()])));
	fputs(szWrite, fp);

	// 衛星名
	sprintf_s(szWrite, "SATELLITE=%s\n", (LPCSTR)CStringA(satname));
	fputs(szWrite, fp);

	// 延通信時間
	sprintf_s(szWrite, "TIME=%d時間%d分\n", iAllTime / 60, iAllTime % 60);
	fputs(szWrite, fp);

	// 雛形
//CHG 2018/12/11 >>>
//	sprintf_s(szWrite, "FORMAT\n", iAllTime / 60, iAllTime % 60);
//CHG 2018/12/11 <<<
	sprintf_s(szWrite, "FORMAT\n");
	fputs(szWrite, fp);

	CString FormCoverFile;
	FILE *fpCvrIn = NULL;
	char szRead[2046];
	CString	cbuf;

	FormCoverFile.Format(_T("%s%s\\%s"), (LPCTSTR)formDir, (LPCTSTR)satname, (LPCTSTR)PRINT_FORM_COVER_FILE);
	if ((_wfopen_s(&fpCvrIn, FormCoverFile, _T("r")) != 0) || (fpCvrIn == NULL))
	{
		return;
	}

	int cnt = 0;
	while (fgets(szRead, static_cast<unsigned>(_countof(szRead)), fpCvrIn) != NULL)
	{
		cbuf = CServerFile::ConvertUnicode(szRead);
		cbuf.TrimLeft();

		if (cnt >= 8)
		{
			sprintf_s(szWrite, "%s", (LPCSTR)CStringA(cbuf));
			fputs(szWrite, fp);
		}

		cnt++;
	}

	if (fpCvrIn != NULL)
	{
		fclose(fpCvrIn);
	}
}

/*============================================================================*/
/*! CBusinessDiaryPrint

-# Cover、Header部ファイル出力

@param  なし
@retval なし
*/
/*============================================================================*/
void CBusinessDiaryPrint::CreatePrintList(CString formDir, CString satname, int iYear, int iMonth)
{
	CString CoverFile, HeaderFile, ListFile;

#if 1
	// Cover 印刷
	CoverFile.Format(_T("%s%s\\%04d_%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, iYear, iMonth, (LPCTSTR)PRINT_COVER_FILE);
	mPrintList.push_back(CoverFile);

	// Header 印刷
	HeaderFile.Format(_T("%s%s\\%04d_%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, iYear, iMonth, (LPCTSTR)PRINT_HEADER_FILE);
	mPrintList.push_back(HeaderFile);

	// List 印刷
	ListFile.Format(_T("%s%s\\%04d_%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, iYear, iMonth, (LPCTSTR)PRINT_LIST_FILE);
	mPrintList.push_back(ListFile);
#else
	// Cover 印刷
	CoverFile.Format(_T("%s%s\\%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, iMonth, (LPCTSTR)PRINT_COVER_FILE);
	mPrintList.push_back(CoverFile);

	// Header 印刷
	HeaderFile.Format(_T("%s%s\\%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, iMonth, (LPCTSTR)PRINT_HEADER_FILE);
	mPrintList.push_back(HeaderFile);

	// List 印刷
	ListFile.Format(_T("%s%s\\%02d_%s"), (LPCTSTR)formDir, (LPCTSTR)satname, iMonth, (LPCTSTR)PRINT_LIST_FILE);
	mPrintList.push_back(ListFile);
#endif
}