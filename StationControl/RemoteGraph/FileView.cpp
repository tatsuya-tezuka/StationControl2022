// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "RemoteGraph.h"
#include "WaitDialog.h"

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const CString mRootName = _T("監視データファイル");

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
	m_nActiveNo = 0;
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_OPEN, &CFileView::OnUpdateOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar メッセージ ハンドラー

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ビューの作成:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("ファイル ビューを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	// ビューのイメージの読み込み:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ロックされています*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// すべてのコマンドが、親フレーム経由ではなくこのコントロール経由で渡されます:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 静的ツリー ビュー データ (ダミー コード) を入力します
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	CWaitCursor wait;

	if (m_wndFileView.GetSafeHwnd() == NULL)
	{
		return;
	}
	m_wndFileView.DeleteAllItems();
	HTREEITEM hRoot = m_wndFileView.InsertItem(mRootName, 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	m_MonDataFileList.clear();

	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itr;
	UINT mask = 0;
	HTREEITEM hStation[eStation_MAX];
	CString str;
	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		AfxExtractSubString(str, (*itr).name, 0, '.');
		str.MakeUpper();
		int stno = GetStationNo(str);
		if (stno >= 0 && !(mask&(1 << stno))){
			mask |= (1<<stno);
			hStation[stno] = m_wndFileView.InsertItem(str, hRoot, TVI_LAST);
			m_wndFileView.SetItemData(hStation[stno], (((DWORD_PTR)stno+1)<<16));
			CString pathMon = theApp.GetShareFilePath(eFileType_Equipment_Folder, stno);
			CreateServerFileList(hStation[stno], pathMon, stno);
		}
	}

	ExpandAll(hRoot);
}

/*============================================================================*/
/*! データ収集

-# 全アイテムを展開する

@param		nItem	ツリーハンドル
@retval

*/
/*============================================================================*/
void CFileView::ExpandAll(HTREEITEM hItem)
{
	m_wndFileView.Expand(hItem, TVE_EXPAND);
	HTREEITEM hNextItem = m_wndFileView.GetChildItem(hItem);
	while (hNextItem != NULL){
		m_wndFileView.Expand(hNextItem, TVE_EXPAND);
		ExpandAll(hNextItem);
		hNextItem = m_wndFileView.GetNextItem(hNextItem, TVGN_NEXT);
	}
}

/*============================================================================*/
/*! データ収集

-# データ名称から局番号を取得する

@param		hRoot	親ツリーハンドル
@param		path	親ディレクトリ名
@param		flag	種別フラグ(0x01:年 0x02:月 0x04:日 0x08:時)
@retval

*/
/*============================================================================*/
int CFileView::GetStationNo(CString& name)
{
	for (int i = 0; i < sizeof(mStationSimpleString) / sizeof(mStationSimpleString[0]); i++){
		if (name == mStationSimpleString[i]){
			return i;
		}
	}
	return -1;
}

/*============================================================================*/
/*! データ収集

-# データ名称から局番号を取得する

@param		hRoot	親ツリーハンドル
@param		path	親ディレクトリ名
@param		flag	種別フラグ(0x01:年 0x02:月 0x04:日 0x08:時)
@retval

*/
/*============================================================================*/
HTREEITEM CFileView::FindItemText(HTREEITEM hRoot, CString& name)
{
	HTREEITEM hItem = m_wndFileView.GetChildItem(hRoot);

	while (hItem != NULL){
		if (m_wndFileView.GetItemText(hItem).CompareNoCase(name) == 0){
			return hItem;
		}
		hItem = m_wndFileView.GetNextSiblingItem(hItem);
	}
	return NULL;
}

/*============================================================================*/
/*! データ収集

-# サーバーファイル一覧の作成

@param		hRoot	親ツリーハンドル
@param		path	親ディレクトリ名
@param		flag	種別フラグ(0x01:年 0x02:月 0x04:日 0x08:時)
@retval

*/
/*============================================================================*/
void CFileView::CreateServerFileList(HTREEITEM hRoot, CString path, int nStationNo)
{
	CString tempPath = path;

	tempPath += _T("*.*");
	// 対象フォルダ内のファイル情報を取得する
	// 指定されたファイル名に一致するファイルを、ディレクトリ内で検索する
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(tempPath, &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return;
	}

	// フォルダが見つかった
	UINT item = 0;
	do{
		TRY
		{
			if (ffdata.cFileName[0] == '\0')
				AfxThrowUserException();
			if (ffdata.cFileName[0] == '.')
				AfxThrowUserException();
			if (ffdata.cFileName[0] == '[')
				AfxThrowUserException();

			// ディレクトリ名取得
			if (!(ffdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				AfxThrowUserException();

			CString name, str;
			name = CString(ffdata.cFileName);

			// 日付フォルダが存在したので、フォルダ内に対象監視名が存在するかチェックを行う
			// 存在した場合は、outFileListにファイルフルパスが格納される
			if (FindMonitorData(name, nStationNo, m_MonDataFileList) == false){
				AfxThrowUserException();
			}

			HTREEITEM hYear, hMonth, hDay;
			// 年のアイテム作成
			AfxExtractSubString(str, name, 0, '-');
			hYear = FindItemText(hRoot, str);
			if (hYear == NULL){
				hYear = m_wndFileView.InsertItem(str, hRoot, TVI_LAST);
				m_wndFileView.SetItemData(hYear, 1);
			}
			// 月のアイテム作成
			AfxExtractSubString(str, name, 1, '-');
			hMonth = FindItemText(hYear, str);
			if (hMonth == NULL){
				hMonth = m_wndFileView.InsertItem(str, hYear, TVI_LAST);
				m_wndFileView.SetItemData(hMonth, 2);
			}
			// 日のアイテム作成
			AfxExtractSubString(str, name, 2, '-');
			hDay = FindItemText(hMonth, str);
			if (hDay == NULL){
				hDay = m_wndFileView.InsertItem(str, hMonth, TVI_LAST);
				m_wndFileView.SetItemData(hDay, 3);
			}
		}
		CATCH(CUserException, e)
		{
		}
		AND_CATCH(CException, e)
		{
		}
		END_CATCH
	} while (FindNextFile(hFindFile, &ffdata)); // ファイルがなくなるまで続ける
}

/*============================================================================*/
/*! データ収集

-# 指定ファイルにデータ登録された監視名があるかの存在チェック

@param		hRoot		親ツリーハンドル
@param		path		親ディレクトリ名
@param		outFileList	対象ファイル
@retval

*/
/*============================================================================*/
bool CFileView::FindMonitorData(CString fname, int nStationNo, vector<CString>& outFileList)
{
	CString strPath = theApp.GetShareFilePath(eFileType_Equipment_Folder, nStationNo);

	strPath += fname;
	strPath += _T("\\");
	int size = (int)outFileList.size();
	GetDataFileList(strPath, outFileList);

	if ((int)outFileList.size() == size)
		return false;

	return true;
}
/*============================================================================*/
/*! 監視データアクセス

-# 指定フォルダのファイル一覧を取得する

@param	path	フォルダ名
@param	outFileList	監視ファイルリスト(出力)

@retval

*/
/*============================================================================*/
void CFileView::GetDataFileList(CString& path, vector<CString>& outFileList)
{
	// 対象フォルダ内のファイル情報を取得する
	// 指定されたファイル名に一致するファイルを、ディレクトリ内で検索する
	WIN32_FIND_DATA ffdata;
	HANDLE hFindFile = FindFirstFile(path + _T("*"), &ffdata);
	if (hFindFile == INVALID_HANDLE_VALUE){
		return;
	}

	// 監視データ一覧
	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();

	UINT item = 0;
	do{
		TRY
		{
			CString fname;
			if (ffdata.cFileName[0] == '\0')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '.')
			{
				AfxThrowUserException();
			}
			if (ffdata.cFileName[0] == '[')
			{
				AfxThrowUserException();
			}

			// ファイル名取得
			if (ffdata.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				CString name = CString(ffdata.cFileName);
				if (name.Find(_T(".")) >= 0){
					continue;
				}
				if (name.Left(3).CompareNoCase(_T("NON")) == 0){
					continue;
				}
				// ファイル名から監視名を抜き出す
				CString tag;
				AfxExtractSubString(tag, name, 0, '_');

				vector<stSelectData>::iterator itr;
				CString str;
				for (itr = datalist.begin(); itr != datalist.end(); itr++){
					AfxExtractSubString(str, (*itr).name, 1, '.');
					AfxExtractSubString(str, str, 0, '_');
					if (tag.CompareNoCase(str) == 0)
						break;
				}
				if(itr != datalist.end())
					outFileList.push_back(path + name);
			}
		}
			CATCH(CUserException, e)
		{
		}
		AND_CATCH(CException, e)
		{
		}
		END_CATCH
	} while (FindNextFile(hFindFile, &ffdata));

	FindClose(hFindFile);
}
/*============================================================================*/
/*! プロセス間通信

-# AP層からのメッセージ受信

@param

@retval
*/
/*============================================================================*/
void CALLBACK CFileView::receiveCallBack(TCHAR* monName, time_t monTime, xobs_tbl_res_t* monData, bool bLeapSec)
{
//	TRACE("### monName:%s monTime:%I64d monData:%d leapSec=%d\n", CStringA(monName), monTime, monData->obs_adr.l_id, bLeapSec);

	if( monTime == 0 )
	{
		CLogTraceEx::Write(_T(""), _T("CFileView"), _T("receiveCallBack"), _T("monTime"), _T("Data Time Zero"), nLogEx::info);

		return;
	}

	CTime	t = monTime;

	// 年数をファイルビューのものに置き換え
	CTime	s_time = theApp.GetDataCollection().GetAnalysisStart(((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetActiveNo());
	if( t.GetYear() != s_time.GetYear() )
		t = (t.GetTime() - CTime(t.GetYear(), 1, 1, 0, 0, 0).GetTime()) + CTime(s_time.GetYear(), 1, 1, 0, 0, 0).GetTime();

#ifdef	_TEST_LEAP
	if( t.GetSecond() == 0 )
	{
		stQueData	que_data;
		que_data.name.Format(_T("%s.%s"), mStationSimpleString[((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetHitItemPath().stno], monName);
		memset(&que_data.data, 0, sizeof(stSpaceData));
		t.GetAsSystemTime(que_data.data.systime);
		que_data.data.status = SDS_VALID | (1 ? SDS_LEAP : 0x0);
		que_data.data.mmPair.cnt = 1;

		// データ値を取得
		if( (monData->obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr)) || (monData->obs_adr.l_cnvResType & (1 << eCnvResType_Long)) )
			que_data.data.mmPair.max = que_data.data.mmPair.min = 10.0;
		else
			que_data.data.mmPair.max = que_data.data.mmPair.min = 10.0;

		// 読み込み監視データをキューにプッシュ
		theApp.GetDataCollection().PushAnalysisData(((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetActiveNo(), que_data);
	}
#endif	// _TEST_LEAP

	stQueData	que_data;
	que_data.name.Format(_T("%s.%s"), mStationSimpleString[((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetHitItemPath().stno], monName);
	memset(&que_data.data, 0, sizeof(stSpaceData));
	t.GetAsSystemTime(que_data.data.systime);
	que_data.data.status = SDS_VALID | (bLeapSec ? SDS_LEAP : 0x0);
	que_data.data.mmPair.cnt = 1;

	// データ値を取得
#if 0
	if( (monData->obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr)) || (monData->obs_adr.l_cnvResType & (1 << eCnvResType_Long)) )
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.l_data;
	else
		que_data.data.mmPair.max = que_data.data.mmPair.min = monData->obs_adr.d_data;
#else
	if( monData->obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr) )
	{
		// 文字列
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.l_data;
	}
	else if( monData->obs_adr.l_cnvResType & (1 << eCnvResType_Long) )
	{
		// long値
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.l_data;
	}
	else if( monData->obs_adr.l_cnvResType & (1 << eCnvResType_Double) )
	{
		// double値
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.d_data;
	}
	else if( monData->obs_adr.l_cnvResType & (1 << eCnvResType_LongDouble) )
	{
		// long double値
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.ld_data;
	}
	else
	{
		// long値
		que_data.data.mmPair.max = que_data.data.mmPair.min = (double)monData->obs_adr.l_data;
	}
#endif

	// 読み込み監視データをキューにプッシュ
	theApp.GetDataCollection().PushAnalysisData(((CMainFrame *)theApp.m_pMainWnd)->GetFileView().GetActiveNo(), que_data);
}

/*============================================================================*/
/*! データ収集

-# サーバーファイル一覧の作成

@param		hRoot	親ツリーハンドル
@param		path	親ディレクトリ名
@retval

*/
/*============================================================================*/
bool CFileView::GetRangeTimeMonitorData(SYSTEMTIME& stdate, SYSTEMTIME& spdate, int nStationNo)
{
	CDlgWaitCmd waitDlg;

	waitDlg.Open(_T("データ読み込み"), _T("ファイルからデータを読み込んでいます。\n\nしばらくお待ちください。"), false);

#ifdef _ENGDLL
	CString strDbName = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, nStationNo);
	CString strPath = theApp.GetShareFilePath(eFileType_Equipment_Folder, nStationNo);
	CString str, tag;
	CEngValCnv engcnv;
	// DB名称の設定
	TCHAR buf[_MAX_PATH];
	wsprintf(buf, _T("%s"), strDbName);
	engcnv.SetEqMonDbFileName(buf);
	// 監視データ格納フォルダ名称の設定
	wsprintf(buf, _T("%s"), strPath);
	engcnv.SetMonDataPath(buf);

	CTime stime, ptime;
	stime = CTime(stdate);
	ptime = CTime(spdate);
	CEngValCnv::TL_TIME timeTarget;
	timeTarget.bMode = (BYTE)1;
	timeTarget.sttime = stime.GetTime();
	timeTarget.sptime = ptime.GetTime();

	// 監視名を取り出す
#ifdef _ALLMON
	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itr;
	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		AfxExtractSubString(str, (*itr).name, 0, '.');
		wsprintf(buf, _T("%s"), ((*itr).name).Mid(str.GetLength() + 1));
		engcnv.AddMonitorName(buf);
	}
#else
	CPropertiesData& prop = theApp.GetPropertiesData(m_nActiveNo);
	for (int i = 0; i < MAX_TDATA; i++){
		tag = prop.item[i].dataName;
		AfxExtractSubString(tag, prop.item[i].dataName, 0, '.');
		CString name = CString(prop.item[i].dataName);
		wsprintf(buf, _T("%s"), name.Mid(tag.GetLength() + 1));
		engcnv.AddMonitorName(buf);
	}
#endif

	// 工学値変換対象のファイル一覧をコピーする
	vector<CString>::iterator fitr;
	engcnv.ClearDataFileList();
	for (fitr = m_MonDataFileList.begin(); fitr != m_MonDataFileList.end(); fitr++){
		// パス名から対象日付を決定する
		CString temp, str = (*fitr);
		str = str.Mid(strPath.GetLength());
		SYSTEMTIME st;
		memset(&st, 0, sizeof(SYSTEMTIME));
		AfxExtractSubString(temp, str, 0, '-');
		st.wYear = _wtoi(temp);
		AfxExtractSubString(temp, str, 1, '-');
		st.wMonth = _wtoi(temp);
		AfxExtractSubString(temp, str, 2, '-');
		st.wDay = _wtoi(temp);
		CTime tm;
		tm = CTime(st);
		if (tm.GetTime() < timeTarget.sttime || tm.GetTime() > timeTarget.sptime)
			continue;

		wsprintf(buf, _T("%s"), (*fitr));
		engcnv.AddDataFileList(buf);
	}

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	// 時間範囲での工学値変換
	engcnv.GetMonitorDataEx(&timeTarget);

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### GetRangeTimeMonitorData = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
	timeEndPeriod(1);

	map< CString, vector<CEngValCnv::obs_tbl_res_t> >& list = engcnv.GetOutDataList();

	vector<CEngValCnv::obs_tbl_res_t>& data = engcnv.GetAllVal();

	map< CString, vector<CEngValCnv::obs_tbl_res_t> >::iterator itrd;
	for (itrd = engcnv.mOutDataList.begin(); itrd != engcnv.mOutDataList.end(); itrd++){
		if ((*itrd).second.size() != 0)
			return true;
	}
	return false;
#else
	CString strDbName = theApp.GetShareFilePath(eFileType_DB_Eqmon_File, nStationNo);
	CString strPath = theApp.GetShareFilePath(eFileType_Equipment_Folder, nStationNo);
	CString str, sttag, eqtag, name;
	// DB名称の設定
	TCHAR buf[_MAX_PATH];
	wsprintf(buf, _T("%s"), (LPCTSTR)strDbName);
	Dll_SetEqMonDbFileName(buf);

	// 監視データ格納フォルダ名称の設定
	wsprintf(buf, _T("%s"), (LPCTSTR)strPath);
	Dll_SetMonDataPath(buf);

	CTime stime, ptime;
	stime = CTime(stdate);
	ptime = CTime(spdate);
	xTL_TIME timeTarget;
	timeTarget.bMode = (BYTE)1;
	timeTarget.sttime = stime.GetTime();
	timeTarget.sptime = ptime.GetTime();

	// 監視名を取り出す
#ifdef _ALLMON
	vector<stSelectData>& datalist = theApp.GetDataCollection().GetSelectDataList();
	vector<stSelectData>::iterator itr;
	for (itr = datalist.begin(); itr != datalist.end(); itr++){
		AfxExtractSubString(str, (*itr).name, 0, '.');
		wsprintf(buf, _T("%s"), ((*itr).name).Mid(str.GetLength() + 1));
		engcnv.AddMonitorName(buf);
	}
#else
	Dll_ClearMonitorName();
	CPropertiesData& prop = theApp.GetPropertiesData(m_nActiveNo);
	for (int i = 0; i < MAX_TDATA; i++){
		name = prop.item[i].dataName;
		AfxExtractSubString(sttag, name, 0, '.');	// 局名取り出し
		AfxExtractSubString(eqtag, name, 1, '.');	// 設備名取り出し

		// 実際の監視名を登録する
		if( mStationSimpleString[nStationNo] == sttag )
		{
			wsprintf(buf, _T("%s"), (LPCTSTR)name.Mid(sttag.GetLength() + 1));
			Dll_AddMonitorName(buf);
		}
		else
			Dll_AddMonitorName(_T(""));
	}
#endif

	// 工学値変換対象のファイル一覧をコピーする
	vector<CString>::iterator fitr; 
	Dll_ClearDataFileList();
	for (fitr = m_MonDataFileList.begin(); fitr != m_MonDataFileList.end(); fitr++){
		// パス名から対象日付を決定する
		CString temp, str = (*fitr);
		str = str.Mid(strPath.GetLength());
		SYSTEMTIME st;
		memset(&st, 0, sizeof(SYSTEMTIME));
		AfxExtractSubString(temp, str, 0, '-');
		st.wYear = _wtoi(temp);
		AfxExtractSubString(temp, str, 1, '-');
		st.wMonth = _wtoi(temp);
		AfxExtractSubString(temp, str, 2, '-');
		st.wDay = _wtoi(temp);
		CTime tm;
		tm = CTime(st);
		if (tm.GetTime() < timeTarget.sttime || tm.GetTime() > timeTarget.sptime)
			continue;

		wsprintf(buf, _T("%s"), (LPCTSTR)(*fitr));
		Dll_AddDataFileList(buf);
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("Dll_SetCallBack"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// DLLにコールバック関数と通知する
	Dll_SetCallBack(receiveCallBack);

	// 最小タイマ分解能を設定(1ms)
	timeBeginPeriod(1);
	DWORD	dwStart = timeGetTime();

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("DeleteSpaceRead"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 読み込み監視データを初期化
	theApp.GetDataCollection().DeleteSpaceRead();

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("CreateAnalysisDataFromFile"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 解析で使用するデータを監視データファイルから作成を開始
	theApp.GetDataCollection().CreateAnalysisDataFromFile(m_nActiveNo, stime, ptime);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("Dll_GetMonitorDataEx"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 時間範囲での工学値変換
	Dll_GetMonitorDataEx(&timeTarget);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("PushAnalysisDataFinish"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 残りの読み込み監視データをキューにプッシュ
	theApp.GetDataCollection().PushAnalysisDataFinish(m_nActiveNo);

	// 取得したデータが保存されているか確認
	while( theApp.GetDataCollection().IsExistSaveAnalysisDataQue(m_nActiveNo) )
		Sleep(5);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("GetRangeTimeMonitorData"), _T("FinishAnalysisDataFromFile"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 解析で使用するデータを監視データファイルから作成を終了
	theApp.GetDataCollection().FinishAnalysisDataFromFile(m_nActiveNo);

	DWORD dwElapsed = (timeGetTime() - dwStart);
	TRACE("### GetRangeTimeMonitorData = %.3lf(sec)\n", (double)((double)dwElapsed / 1000.0));
	timeEndPeriod(1);

	waitDlg.Close();

#if 0
	map< CString, vector<xobs_tbl_res_t> >& list = Dll_GetOutDataList();
	vector<xobs_tbl_res_t>& data = Dll_GetAllVal();
	map< CString, vector<xobs_tbl_res_t> >::iterator itrd;
	for (itrd = list.begin(); itrd != list.end(); itrd++){
		if ((*itrd).second.size() != 0)
			return true;
	}
	return false;
#else
	return true;
#endif
#endif
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// クリックされた項目の選択:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			// 選択されているアイテモ文字列を取得する
			pWndTree->SelectItem(hTreeItem);
			SetHitItemName(pWndTree->GetItemText(hTreeItem));
			// 選択されているアイテムからフォルダ名を求める
			m_stHitItemPath.stno = -1;
			m_stHitItemPath.year.Empty();
			m_stHitItemPath.month.Empty();
			m_stHitItemPath.day.Empty();
			DWORD dw = (DWORD)pWndTree->GetItemData(hTreeItem);
			switch (dw){
			case	3:/*Day*/
				m_stHitItemPath.day = pWndTree->GetItemText(hTreeItem);
				hTreeItem = pWndTree->GetParentItem(hTreeItem);
			case	2:/*Month*/
				m_stHitItemPath.month = pWndTree->GetItemText(hTreeItem);
				hTreeItem = pWndTree->GetParentItem(hTreeItem);
			case	1:/*Year*/
				m_stHitItemPath.year = pWndTree->GetItemText(hTreeItem);
				hTreeItem = pWndTree->GetParentItem(hTreeItem);
				dw = (DWORD)pWndTree->GetItemData(hTreeItem);
				if ((dw >> 16) != 0){
					// 局名
					m_stHitItemPath.stno = (dw >> 16) - 1;
				}
			default:
				break;
			}
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
//	AfxMessageBox(_T("プロパティ..."));
}

void CFileView::OnFileOpen()
{
	CWaitCursor wait;

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("OnFileOpen"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

//	vector<CSpaceData> spacedataList;
	CString name = GetHitItemName();
	// 選択されたアイテムの年月日を取得する
	stSelectPath& stpath = GetHitItemPath();

	if (m_stHitItemPath.stno < 0)
		return;

	SYSTEMTIME stdate, spdate;

	// 工学値変換を行う開始終了日時を求める
	stdate.wHour = 0;
	stdate.wMinute = 0;
	stdate.wSecond = 0;
	stdate.wMilliseconds = 0;
	spdate.wHour = 23;
	spdate.wMinute = 59;
	spdate.wSecond = 59;
	spdate.wMilliseconds = 0;

	stdate.wYear = _wtoi(m_stHitItemPath.year);
	spdate.wYear = stdate.wYear;
	if (m_stHitItemPath.month.IsEmpty() == true){
		// 指定年の１年分
		stdate.wMonth = 1;
		spdate.wMonth = 12;
	}
	else{
		stdate.wMonth = _wtoi(m_stHitItemPath.month);
		spdate.wMonth = stdate.wMonth;
	}

	if (m_stHitItemPath.day.IsEmpty() == true){
		// 指定月の１月分
		stdate.wDay = 1;
		if (spdate.wMonth == 12){
			spdate.wYear++;
			spdate.wMonth = 1;
		}
		else{
			spdate.wMonth = stdate.wMonth + 1;
		}
		spdate.wDay = 1;
		CTime tm = CTime(spdate);
		tm -= CTimeSpan(1, 0, 0, 0);
		tm.GetAsSystemTime(spdate);
	}
	else{
		stdate.wDay = _wtoi(m_stHitItemPath.day);
		spdate.wDay = stdate.wDay;
	}

	// アプリが扱い可能な最大データ数を確認
	CTime	t1 = stdate;
	CTime	t2 = spdate;
	if( ((t2 - t1).GetTotalSeconds() + 1) > MAX_ALLDATA_SIZE )
	{
		t2 = t1 + CTimeSpan(MAX_USEDATA_DAYS, 0, 0, 0) - CTimeSpan(0, 0, 0, 1);
		t2.GetAsSystemTime(spdate);
	}

	CString	msg;
	msg.Format(_T("Start Time = %s %s, End Time = %s %s, Data Count = %I64d"), (LPCTSTR)(t1.Format("%Y/%m/%d")), (LPCTSTR)(t1.Format("%H:%M:%S")), (LPCTSTR)(t2.Format("%Y/%m/%d")), (LPCTSTR)(t2.Format("%H:%M:%S")), (t2 - t1).GetTotalSeconds() + 1);

	TRACE("### %s\n", CStringA(msg));

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("OnFileOpen"), _T("Get Data"), msg, nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 指定期間のデータを取得する
	GetRangeTimeMonitorData(stdate, spdate, m_stHitItemPath.stno);

#if 0
	deque<CSpaceData>& spdata = theApp.GetDataCollection().GetSpaceData();
	spdata.clear();

	deque<CSpaceData>& data = theApp.GetDataCollection().GetSpaceTemp();
	data.clear();
	vector<CSpaceData>::iterator itr;
	for (itr = spacedataList.begin(); itr != spacedataList.end(); itr++){
		data.push_back((*itr));
	}
#endif

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T(""), _T("CFileView"), _T("OnFileOpen"), _T("End"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	return;
}

void CFileView::OnUpdateOpen(CCmdUI *pCmdUI)
{
//	// リアルタイム停止中に有効
//	pCmdUI->Enable(!((CMainFrame*)theApp.GetMainWnd())->GetOnline());

	CPropertiesData& prop = theApp.GetPropertiesData(m_nActiveNo);
	pCmdUI->Enable(((m_stHitItemPath.stno < 0) || prop.isRealTime) ? FALSE : TRUE);
}

void CFileView::OnFileOpenWith()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnDummyCompile()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditCut()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditCopy()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditClear()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ロックされました*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("ビットマップを読み込めませんでした: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}




