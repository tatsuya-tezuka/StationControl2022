// EqStatusGridView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "EqStatusGridView.h"
#include "MainFrm.h"


// CEqStatusGridView

IMPLEMENT_DYNCREATE(CEqStatusGridView, CScrollView)

CEqStatusGridView::CEqStatusGridView()
{
	mButtonList.clear();
}

CEqStatusGridView::~CEqStatusGridView()
{
	vector<CGridButton*>::iterator itr;
	for (itr = mButtonList.begin(); itr != mButtonList.end(); itr++){
		//(*itr)->DestroyWindow();
		//delete (*itr);
	}
	vector<CGridButton*> temp(0);
	mButtonList.swap(temp);
}


BEGIN_MESSAGE_MAP(CEqStatusGridView, CScrollView)
	ON_COMMAND_RANGE(eMessage_EqStatusControl, eMessage_EqStatusControl+100, OnPushButton)
END_MESSAGE_MAP()


// CEqStatusGridView 描画

void CEqStatusGridView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	if (!mButtonList.empty())
		return;

	UINT nWidth, nHeight;
	if (theApp.GetSelectStationMode() == eStation_1){
		nWidth = mWidth+24;
		nHeight = mHeight;

		SetScrollSizes(MM_TEXT, CSize(nWidth*mHCount, nHeight*mVCount));
	}
	else{
		nWidth = mWidth-5;
		nHeight = mHeight-5;
		SetScrollSizes(MM_TEXT, CSize(nWidth*mHCount-1, nHeight*mVCount));
	}

	LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	style = style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	SetWindowLong(m_hWnd, GWL_STYLE, style);

	mStation = theApp.GetOpenViewType();

	mButtonList.clear();
	mStatusTagList.clear();

	CString strEqGreen(MONIEQUIPMENT_GREEN);

	int pos = 0;
	int cx = 0, cy = 0;
	for (UINT x = 0; x < nWidth*mHCount; x += nWidth)
	{
		cy = 0;
		for (UINT y = 0; y < nHeight*mVCount; y += nHeight)
		{
			CRect rc(x, y, x + nWidth, y + nHeight);
			CGridButton *btn = new CGridButton();
			btn->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_FLAT, rc, this, eMessage_EqStatusControl + pos);
			btn->m_bDrawFocus = false;
			mButtonList.push_back(btn);

			CString str;
			if (theApp.GetSelectStationMode() == eStation_5){
				str = mEquipmentStatusString[mStation - eViewType5_EqOperation1][cy][cx];
				mStatusTagList.push_back(mEquipmentStatusStatusTag[mStation - eViewType5_EqOperation1][cy][cx]);
			}
			else{
				str = mEquipmentStatusString[theApp.GetSelectStation()][cy][cx];
				mStatusTagList.push_back(mEquipmentStatusStatusTag[theApp.GetSelectStation()][cy][cx]);
			}

			btn->SetText(str);
			if (str.IsEmpty() == true)
				btn->SetStatus(eButtonStatus_None);
			else if (mEquipmentStatusStatusTag[theApp.GetSelectStation()][cy][cx].Compare(strEqGreen) != 0)
				btn->SetStatus(eButtonStatus_Close);
			else
				btn->SetStatus(eButtonStatus_Remote);

			pos++;
			cy++;
		}
		cx++;
	}
}

void CEqStatusGridView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 描画コードをここに追加してください。
}


// CEqStatusGridView 診断

#ifdef _DEBUG
void CEqStatusGridView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CEqStatusGridView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEqStatusGridView メッセージ ハンドラー
/*============================================================================*/
/*! 設備状況

-# 各設備状況ボタン押下処理

@param
@retval

*/
/*============================================================================*/
void CEqStatusGridView::OnPushButton(UINT nID)
{
	CString str = mButtonList[nID - eMessage_EqStatusControl]->GetText();
	if (str.IsEmpty() == true)
		return;
	str += _T(".xml");
	((CMainFrame*)(theApp.m_pMainWnd))->OnFaBrowser(mStation, str);
}

/*============================================================================*/
/*! 設備状況

-# 各設備状況の更新

@param
@retval

*/
/*============================================================================*/
void CEqStatusGridView::UpdateStatus()
{
	vector<CGridButton*>::iterator itrBtn;
	vector<CString>::iterator itrStatusTag;
	CString strEqGreen(MONIEQUIPMENT_GREEN);

	UINT sta;

	if (theApp.GetSelectStationMode() == eStation_1)
	{
		sta = theApp.GetSelectStation();
	}
	else
	{
		sta = mStation - eViewType5_EqOperation1;
	}

	for (itrBtn = mButtonList.begin(), itrStatusTag = mStatusTagList.begin(); itrBtn != mButtonList.end() && itrStatusTag != mStatusTagList.end(); itrBtn++, itrStatusTag++){

#ifdef _ENGVALCNV_TREND
		char	sz_status[256];
		CEngValCnv::obsname_t *obs_adr;	// 監視項目アドレス
		CSatelliteData& sd = theApp.GetSatelliteData();
		//if (sd.stn_tbl_access() == 1)
		//{
		//	return;
		//}

		//{
		//	CString strDebug;
		//	strDebug.Format(_T("%s(%d) :: Btn(%s), Equipment(%s)"), __FILEW__, __LINE__, (*itrBtn)->GetText(), (*itrStatusTag));
		//	CLogTraceEx::Write(_T(""), _T("CEqStatusGridView"), _T("UpdateStatus"), _T("debug"), strDebug, nLogEx::debug);
		//}


		// 該当設備が空白の場合
		if ((*itrStatusTag).IsEmpty())
		{
			if ((*itrBtn)->GetText().IsEmpty() == false){
				if ((*itrBtn)->GetStatus() != eButtonStatus_None){
					(*itrBtn)->SetStatus(eButtonStatus_None);
				}
			}
		}
		// 設備が緑色固定の場合
		else if ((*itrStatusTag).Compare(strEqGreen) == 0)
		{
			if ((*itrBtn)->GetText().IsEmpty() == false){
				if ((*itrBtn)->GetStatus() != eButtonStatus_Remote){
					(*itrBtn)->SetStatus(eButtonStatus_Remote);
				}
			}
		}
		// 該当設備がある場合
		else
		{
			obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, (*itrStatusTag));
	
			if (obs_adr == NULL)
			{
				continue;
			}
	
			strcpy_s(sz_status, sizeof(sz_status), obs_adr->sz_sts_name);
	
			BOOL bFoundStr = FALSE;
	
			for (int i = 0; i < eButtonStatus_Max; i++)
			{
				// 文字列定義番号を調べる
				if (strcmp(sz_status, mEquipmentStatusRcvString[i]) == 0)
				{
					bFoundStr = TRUE;
					// 文字列定義番号が変更になっていたらセットしなおす
					if ((*itrBtn)->GetText().IsEmpty() == false){
						if ((*itrBtn)->GetStatus() != i){
							(*itrBtn)->SetStatus(i);
						}
					}
				}
			}

			// その他の文字列の場合
			if (!bFoundStr)
			{
				if ((*itrBtn)->GetText().IsEmpty() == false){
					if ((*itrBtn)->GetStatus() != eButtonStatus_Remote){
						(*itrBtn)->SetStatus(eButtonStatus_Remote);
						bFoundStr = TRUE;
					}
				}
			}
		}

#else
#ifdef _NOCOMM
			UINT val = (rand() % 25);
			if (val >= eButtonStatus_None)
				val = eButtonStatus_Remote;
			if ((*itrBtn)->GetText().IsEmpty() == false){
				if ((*itrBtn)->GetStatus() != val){
					(*itrBtn)->SetStatus(val);
				}
			}
#endif
#endif /* _ENGVALCNV_TREND */

	}
}

LRESULT CEqStatusGridView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
		UpdateStatus();
		break;
	default:
		return CScrollView::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}
