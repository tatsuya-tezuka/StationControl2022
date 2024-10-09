/*============================================================================*/
/*! AntStanbyOriented.cpp

-# 衛星捕捉画面　ANT待ち受け指向(54m)
*/
/*============================================================================*/
// AntStanbyOriented.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "AntStanbyOriented.h"
#include "afxdialogex.h"

// CAntStanbyOriented ダイアログ

IMPLEMENT_DYNAMIC(CAntStanbyOriented, CDialogBase)

/*============================================================================*/
// コンストラクタ
/*============================================================================*/
CAntStanbyOriented::CAntStanbyOriented(CWnd* pParent /*=NULL*/)
	: CDialogBase(CAntStanbyOriented::IDD, pParent)
{

}

/*============================================================================*/
// デストラクタ
/*============================================================================*/
CAntStanbyOriented::~CAntStanbyOriented()
{
}

void CAntStanbyOriented::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SATELLITECAPTURE_ANTFORECAST_BUTTON, m_buttonAntForecast);
	DDX_Text(pDX, IDC_ANTSTANBYORIENTED_PASSSTARTTIME_EDIT, m_strPassStart);
	DDX_Text(pDX, IDC_ANTSTANBYORIENTED_PASSENDTIMET_EDIT, m_strPassEnd);
	DDX_Control(pDX, IDC_SATELLITECAPTURE_ANTFORECAST_CMB, m_SatelliteCmb);
}

BEGIN_MESSAGE_MAP(CAntStanbyOriented, CDialogBase)
	ON_BN_CLICKED(IDC_SATELLITECAPTURE_ANTFORECAST_BUTTON, &CAntStanbyOriented::OnBnClickedSatellitecaptureAntforecastButton)
	ON_BN_CLICKED(IDC_ANTSTANBYORIENTED_EXEC_BUTTON, &CAntStanbyOriented::OnBnClickedAntstanbyorientedExecButton)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_SATELLITECAPTURE_ANTFORECAST_CMB, &CAntStanbyOriented::OnCbnSelchangeSatelliteCmb)
END_MESSAGE_MAP()


// CAntStanbyOriented メッセージ ハンドラー

/*============================================================================*/
// 描画時に呼ばれ、指定コントロールの背景を塗りつぶすブラシのハンドルを返す
/*============================================================================*/
HBRUSH CAntStanbyOriented::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		// 静的テキスト背景色
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(DC_BRUSH);
		hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
		break;

	case CTLCOLOR_DLG:
		// ダイアログの背景色
		if (pWnd == this)
		{
			hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
		}
		break;

	default:
		break;
	}

	return hbr;
}

/*============================================================================*/
// ダイアログ初期化処理
/*============================================================================*/
BOOL CAntStanbyOriented::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	UpdateSatellite(FALSE);	//探査機リストの作成

	// パス開始時刻、パス終了時刻初期化 YYYY-MM-DD hh:mm:ss
	m_strPassStart = _T("");
	m_strPassEnd = _T("");

	// 監視データ取得
	UpdateDlg(_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
// 探査機の更新
// 予報値ファイルの更新
/*============================================================================*/
LRESULT CAntStanbyOriented::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == eMessage_UpdateShareFile)
	{
		UpdateSatellite(TRUE);
		return TRUE;
	}
	else if (message == eMessage_UpdateForecastFilePath)
	{
		CString fname = (*(CString*)lParam);
		// 監視データ取得更新
		UpdateDlg(fname);
		return TRUE;
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}

/*============================================================================*/
// 探査機コンボボックス選択変更
/*============================================================================*/
void CAntStanbyOriented::OnCbnSelchangeSatelliteCmb()
{
	UpdateData(TRUE);

	if (mSatelliteNameList.empty() == TRUE)
	{
		return;
	}

	int pos = m_SatelliteCmb.GetCurSel();
	if(pos < 0)
	{
		return;
	}

	CString satellite = mSatelliteNameList[pos];
	mPartPredList.clear();

	vector<stPredListData>::iterator itr;
	CGraphMutex::Lock(eFile);
	vector<stPredListData>& predlist = theApp.GetSatelliteData().GetPredList();		//予報値一覧
	for (itr = predlist.begin(); itr != predlist.end(); itr++){
		if (satellite.CompareNoCase((*itr).strSatellite) == 0){
			mPartPredList.push_back((*itr));
		}
	}
	CGraphMutex::Unlock(eFile);

	CString predfile = mPartPredList[0].strPredPath;
	SetFileName(predfile);
}

/*============================================================================*/
// アンテナ予報値ボタンクリック
/*============================================================================*/
void CAntStanbyOriented::OnBnClickedSatellitecaptureAntforecastButton()
{
	if (mSatelliteNameList.size() == 0)
	{
		return;
	}

	int pos = m_SatelliteCmb.GetCurSel();
	if(pos < 0)
	{
		return;
	}

	m_satellilteName = mSatelliteNameList[pos];
	// 予報値一覧画面呼び出し
	theApp.m_pMainWnd->PostMessage(eMessage_OpenForecastListDlg, (WPARAM)this, (LPARAM)&m_satellilteName);
}

/*============================================================================*/
// 実行ボタンクリック
/*============================================================================*/
void CAntStanbyOriented::OnBnClickedAntstanbyorientedExecButton()
{
	// 設定したアンテナ予報値の内容に沿って、アンテナ設備を指向する制御を実行する。
	// SEND_CONTROL_UI.sh 局名 CTRL.PRED_CHANGE_FREQ 衛星名@予報値ファイル名
	// ※周波数制御（予報値切り替え）の予報値設定と同じ

	if (mSatelliteNameList.empty() == TRUE)
	{
		return;
	}

	int pos = m_SatelliteCmb.GetCurSel();
	if(pos < 0)
	{
		return;
	}

	CString satellite = mSatelliteNameList[pos];
	CString predfile;
	m_buttonAntForecast.GetWindowText(predfile);

	/* 制御情報 */
	CString ctrl = _T("");
	ctrl.Format(_T("CTRL.PRED_CHANGE_FREQ %s@%s"), (LPCTSTR)satellite, (LPCTSTR)predfile);
	TRACE("[%s]\n", CStringA(ctrl));
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));
}

/*============================================================================*/
// 探査機リストの更新
/*============================================================================*/
void CAntStanbyOriented::UpdateSatellite(BOOL update)
{
	// 探査機リストの作成
	vector<CString>::iterator itr;
	CString satellite = _T("");
	int pos;
	BOOL find;

	if(update == TRUE)
	{
		pos = m_SatelliteCmb.GetCurSel();
		if(pos >= 0)
		{
			satellite = mSatelliteNameList[pos];	//リスト更新前の探査機
		}
	}

	m_SatelliteCmb.ResetContent();

	GetSatelliteNameList();										//衛星名リスト
	pos = (-1);
	find = FALSE;
	for(itr = mSatelliteNameList.begin(); itr != mSatelliteNameList.end(); itr++)
	{
		m_SatelliteCmb.AddString((*itr));
		pos++;
		if(update == TRUE)
		{
			//更新前の探査機を表示
			if(satellite.CompareNoCase((*itr)) == 0)
			{
				m_SatelliteCmb.SetCurSel(pos);
				find = TRUE;
			}
		}
	}

	// 先頭の探査機をアクティブにする
	if( (update != TRUE) || 						//起動時
		(mSatelliteNameList.empty() == TRUE) ||		//衛星名リストが空
		(pos < 0) )									//更新前の探査機がない
	{
		m_SatelliteCmb.SetCurSel(0);
	}

	if( (satellite.GetLength() != 0) && (find != TRUE) )
	{
		// 探査機が変更の場合は、予報値一覧からの取得結果は使えない
		TRACE("satellite old:[%s] \n", satellite);
		MessageBox(_T("探査機が変更になっています。\nアンテナ予報値を取得しなおしてください。"), _T("確認"), MB_OK);
	}

	return;
}

/*============================================================================*/
// 予報値一覧から衛星名リストを取得する
/*============================================================================*/
void CAntStanbyOriented::GetSatelliteNameList()
{
	mSatelliteNameList.clear();

	vector<stPredListData>::iterator itr;
	vector<CString>::iterator itrName;

	CGraphMutex::Lock(eFile);
	vector<stPredListData>& predlist = theApp.GetSatelliteData().GetPredList();		//予報値一覧
	for (itr = predlist.begin(); itr != predlist.end(); itr++)
	{
		// 衛星名が既に衛星名リストにあるか調べる
		itrName = find(mSatelliteNameList.begin(), mSatelliteNameList.end(), (*itr).strSatellite);
		if (itrName == mSatelliteNameList.end())
		{
			// 無い場合は局名リストに追加
			mSatelliteNameList.push_back((*itr).strSatellite);
		}
	}
	CGraphMutex::Unlock(eFile);

	return;
}

/*============================================================================*/
// 監視データを取得して画面表示する
/*============================================================================*/
void CAntStanbyOriented::UpdateDlg(CString fname)
{
	// 監視データ取得
	GetKanshiData(fname);

	UpdateData(FALSE);

	return;
}

/*============================================================================*/
// 監視から探査機、予報値ファイル、パス開始時刻、パス終了時刻を取得し更新する
/*============================================================================*/
void CAntStanbyOriented::GetKanshiData(CString fname)
{
	// 探査機取得
	CString	strNewSatellite = _T("");
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();
	CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T(EISEI_NAME));
	if( (obs_adr != NULL) && (obs_adr->l_inflg == INFLG_ON) )
	{
		strNewSatellite = CString(obs_adr->sz_sts_name);
	}
	else
	{
		TRACE("監視取得エラー:[%s] \n", EISEI_NAME);
	}

	CString strFilePath = fname;	//eMessage_UpdateForecastFilePathのlParamを暫定でセット(監視データの取得失敗用)
	// アンテナ予報値ファイル
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T(PREDFILE_NAME));
	if( (obs_adr != NULL) && (obs_adr->l_inflg == INFLG_ON) )
	{
		TCHAR	ca_fpath[1024];				// 予報値ファイル名(フルパス)
		if (MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, obs_adr->sz_sts_name, -1, ca_fpath, _MAX_PATH) != 0)
		{
			strFilePath = CString(ca_fpath);
		}
	}
	else
	{
		TRACE("監視取得エラー:[%s] \n", PREDFILE_NAME);
	}
	SetFileName(strFilePath);

	// パス開始時刻
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T(START_TIME));
	if( (obs_adr != NULL) && (obs_adr->l_inflg == INFLG_ON) )
	{
		char	ca_stime[40];				// パス運用開始時刻(フル)
		CSatelliteData::ub_asc_to_asctime(obs_adr->sz_sts_name, ca_stime);
		double d_Stime = CSatelliteData::ub_asc_to_dtime(ca_stime);
		if (d_Stime > 0)
		{
			CTime timePassStart = CTime((time_t)d_Stime);
			m_strPassStart = timePassStart.Format(L"%Y-%m-%d %H:%M:%S");
		}
	}
	else
	{
		TRACE("監視取得エラー:[%s] \n", START_TIME);
	}

	// パス終了時刻
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T(STOP_TIME));
	if( (obs_adr != NULL) && (obs_adr->l_inflg == INFLG_ON) )
	{
		char	ca_etime[40];				// パス運用終了時刻(フル)
		CSatelliteData::ub_asc_to_asctime(obs_adr->sz_sts_name, ca_etime);
		double d_Etime = CSatelliteData::ub_asc_to_dtime(ca_etime);
		if (d_Etime > 0)
		{
			CTime timePassEnd = CTime((time_t)d_Etime);
			m_strPassEnd = timePassEnd.Format(L"%Y-%m-%d %H:%M:%S");
		}
	}
	else
	{
		TRACE("監視取得エラー:[%s] \n", STOP_TIME);
	}

	if(strNewSatellite.GetLength() == 0)
	{
		return;
	}

	//現行の探査機
	CString strOldSatellite = _T("");
	int pos = m_SatelliteCmb.GetCurSel();
	if(pos >= 0)
	{
		strOldSatellite = mSatelliteNameList[pos];	//探査機リスト
	}

	//監視データから取得した探査機
	pos = 0;
	vector<CString>::iterator itr;
	for(itr = mSatelliteNameList.begin(); itr != mSatelliteNameList.end(); itr++)
	{
		//監視データから取得した探査機を表示
		if(strNewSatellite.CompareNoCase((*itr)) == 0)
		{
			m_SatelliteCmb.SetCurSel(pos);
			break;
		}
		pos++;
	}

	//探査機が変更になったか?
	if( (strOldSatellite.GetLength() != 0) ||
		(strOldSatellite.CompareNoCase(strNewSatellite) != 0) )
	{
		// 探査機が変更の場合は、予報値一覧からの取得結果は使えない
		MessageBox(_T("探査機が変更になっています。\nアンテナ予報値を取得しなおしてください。"), _T("確認"), MB_OK);
	}

	return;
}

/*============================================================================*/
// 予報値ファイル名をボタンにセットする
/*============================================================================*/
void CAntStanbyOriented::SetFileName(CString filepath)
{
	//ファイル名のみ取出し
	CString strFileName = filepath.Right(filepath.GetLength() - filepath.ReverseFind(_T('/')) - 1);

	//拡張子の置換(->.anp)
	int idx = strFileName.Find(_T("."));
	if(idx > 0)
	{
		strFileName = strFileName.Mid(0, idx) + _T(".anp");
	}

	m_buttonAntForecast.SetWindowTextW(strFileName);

	return;
}

