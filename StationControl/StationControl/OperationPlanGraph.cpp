// OperationPlanGraph.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "OperationPlanGraph.h"

#define DRAW_OPE_X		150
#define DRAW_OPE_Y		 10
#define DRAW_OPE_WIDTH  700
#define DRAW_OPE_HEIGHT  20


// COperationPlanGraph

IMPLEMENT_DYNAMIC(COperationPlanGraph, CStatic)

COperationPlanGraph::COperationPlanGraph()
{

}

COperationPlanGraph::~COperationPlanGraph()
{
}


BEGIN_MESSAGE_MAP(COperationPlanGraph, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// COperationPlanGraph メッセージ ハンドラー



#include "OperationPlan.h"

void COperationPlanGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CStatic::OnPaint() を呼び出さないでください。

	COperationPlan *pParent = (COperationPlan*)GetParent();

	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));

	CBrush lCBrush;
	lCBrush.CreateSolidBrush(RGB(0, 0, 0));

	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	CFont* font = GetParent()->GetFont();
	dc.SelectObject(font);

	//-----------------------------------------------------------------
	// 運用
	CRect dRectOpe(DRAW_OPE_X, DRAW_OPE_Y, DRAW_OPE_X + DRAW_OPE_WIDTH, DRAW_OPE_Y + DRAW_OPE_HEIGHT);
	dc.FillSolidRect(&dRectOpe, COLOR_OPE_OPERATION);
	dc.FrameRect(&dRectOpe, &lCBrush);


	int x = DRAW_OPE_X - 20;
	int y = DRAW_OPE_Y;

#if 1
	if ((pParent->m_bOpInit) && (pParent->m_bAntStandby))	// 初期設定(magenta) 且つ ANT待ち受け指向
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT/2);
		dc.FillSolidRect(&dRect, COLOR_OPE_INIT);
		dc.FrameRect(&dRect, &lCBrush);

		int y2 = y + DRAW_OPE_HEIGHT / 2;
		dRect = CRect(x, y2, x + 20, y2 + DRAW_OPE_HEIGHT / 2);
		dc.FillSolidRect(&dRect, COLOR_OPE_ANTSDBY);
		dc.FrameRect(&dRect, &lCBrush);

		x -= 20;
	}
	else if (pParent->m_bOpInit)	// 初期設定
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_INIT);
		dc.FrameRect(&dRect, &lCBrush);
		x -= 20;
	}
	else if (pParent->m_bAntStandby)	// ANT待ち受け指向
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_ANTSDBY);
		dc.FrameRect(&dRect, &lCBrush);
		x -= 20;
	}
#else
	if (pParent->m_bOpInit)	// 初期設定
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_INIT);
		dc.FrameRect(&dRect, &lCBrush);
		x -= 20;
	}
	if (pParent->m_bAntStandby) // ANT待ち受け指向
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_ORANGE);
		dc.FrameRect(&dRect, &lCBrush);
		x -= 20;
	}
#endif

	if (pParent->m_CMDChkValue) // レディネスチェック-CMD
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_RDCK);
		dc.FrameRect(&dRect, &lCBrush);
		dc.DrawText(_T("C"), &dRect, nFormat);
		x -= 20;
	}
	if (pParent->m_TLMChkValue) // レディネスチェック-TLM
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_RDCK);
		dc.FrameRect(&dRect, &lCBrush);
		dc.DrawText(_T("T"), &dRect, nFormat);
		x -= 20;
	}
	if (pParent->m_RNGChkValue) // レディネスチェック-RNG
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_RDCK);
		dc.FrameRect(&dRect, &lCBrush);
		dc.DrawText(_T("R"), &dRect, nFormat);
		x -= 20;
	}
	if (pParent->m_AGCChkValue) // レディネスチェック-AGC
	{
		CRect dRect(x, y, x + 20, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_RDCK);
		dc.FrameRect(&dRect, &lCBrush);
		dc.DrawText(_T("A"), &dRect, nFormat);
		x -= 20;
	}

	// ANT天頂志向有無
	int ax = DRAW_OPE_X + DRAW_OPE_WIDTH;
	CRect dRect(ax, y, ax + 20, y + DRAW_OPE_HEIGHT);
	if (pParent->m_bAntZenith)
	{
		dc.FillSolidRect(&dRect, COLOR_OPE_ANTZENITH);
		dc.FrameRect(&dRect, &lCBrush);
	}
	else
	{
		dc.FillSolidRect(&dRect, COLOR_WHITE);
	}

	//-----------------------------------------------------------------
	// TLM S
	if ( (pParent->m_bXTX == FALSE) && pParent->m_TLMSChkBox)
	{
#if 1
		int count = pParent->m_TlmSList.GetItemCount();

		if (count > 0)
		{
			y += 25;
		}

		int x, w;
		for (int i = 0; i < count; ++i)
		{
			CString strSTime = pParent->m_TlmSList.GetItemText(i, 0);
			CString strETime = pParent->m_TlmSList.GetItemText(i, 1);

			TimeToCoordinate(strSTime, strETime, x, w);

			if (x < 0)
			{
				x = 0;
			}

			if (x + w > rect.right)
			{
				w = rect.Width() - x;
			}

			CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
			dc.FillSolidRect(&dRect, COLOR_OPE_TLMS);
			dc.FrameRect(&dRect, &lCBrush);
			dc.DrawText(_T("TLM S"), &dRect, nFormat);
		}
#else
		y += 25;

		pParent->m_tTLMSS = pParent->m_tOpeS;
		pParent->m_tTLMSE = pParent->m_tOpeE;

		int x, w;

		TimeToCoordinate(pParent->m_tTLMSS, pParent->m_tTLMSE, x, w);

		CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_TLMS);
		dc.FrameRect(&dRect, &lCBrush);
		dc.DrawText(_T("TLM S"), &dRect, nFormat);
#endif
	}

	//-----------------------------------------------------------------
	// TLM X
	if (pParent->m_TLMXChkBox)
	{
#if 1
		int count = pParent->m_TlmXList.GetItemCount();

		if (count > 0)
		{
			y += 25;
		}

		int x, w;
		for (int i = 0; i < count; ++i)
		{
			CString strSTime = pParent->m_TlmXList.GetItemText(i, 0);
			CString strETime = pParent->m_TlmXList.GetItemText(i, 1);

			TimeToCoordinate(strSTime, strETime, x, w);

			if (x < 0)
			{
				x = 0;
			}

			if (x + w > rect.right)
			{
				w = rect.Width() - x;
			}

			CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
			dc.FillSolidRect(&dRect, COLOR_OPE_TLMX);
			dc.FrameRect(&dRect, &lCBrush);
			dc.DrawText(_T("TLM X"), &dRect, nFormat);
		}
#else
		y += 25;

		int x, w;
		TimeToCoordinate(pParent->m_tTLMXS, pParent->m_tTLMXE, x, w);

		CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_TLMX);
		dc.FrameRect(&dRect, &lCBrush);
		dc.DrawText(_T("TLM X"), &dRect, nFormat);
#endif
	}

	//-----------------------------------------------------------------
	// TLM Ka
	if(pParent->m_TLMKaChkBox)
	{
#if 1
		int count = pParent->m_TlmKaList.GetItemCount();

		if (count > 0)
		{
			y += 25;
		}

		int x, w;
		for (int i = 0; i < count; ++i)
		{
			CString strSTime = pParent->m_TlmKaList.GetItemText(i, 0);
			CString strETime = pParent->m_TlmKaList.GetItemText(i, 1);

			TimeToCoordinate(strSTime, strETime, x, w);

			if (x < 0)
			{
				x = 0;
			}

			if (x + w > rect.right)
			{
				w = rect.Width() - x;
			}

			CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
			dc.FillSolidRect(&dRect, COLOR_OPE_TLMKa);
			dc.FrameRect(&dRect, &lCBrush);
			dc.DrawText(_T("TLM Ka"), &dRect, nFormat);
	}
#else
		y += 25;

		int x, w;
		TimeToCoordinate(pParent->m_tTLMKaS, pParent->m_tTLMKaE, x, w);

		CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
		dc.FillSolidRect(&dRect, COLOR_OPE_TLMKa);
		dc.FrameRect(&dRect, &lCBrush);
		dc.DrawText(_T("TLM Ka"), &dRect, nFormat);
#endif
	}

	//-----------------------------------------------------------------
	// アップリンク
	if (pParent->m_UpLinkChkBox)
	{
		int count = pParent->m_UpLinkList.GetItemCount();

		if (count > 0)
		{
			y += 25;
		}

		int x, w;
		for (int i = 0; i < count; ++i)
		{
			CString strSTime = pParent->m_UpLinkList.GetItemText(i, 0);
			CString strETime = pParent->m_UpLinkList.GetItemText(i, 1);

			TimeToCoordinate(strSTime, strETime, x, w);

			if (x < 0)
			{
				x = 0;
			}

			if (x + w > rect.right)
			{
				w = rect.Width() - x;
			}

			CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
			dc.FillSolidRect(&dRect, COLOR_OPE_UPLINK);
			dc.FrameRect(&dRect, &lCBrush);
			dc.DrawText(_T("UL"), &dRect, nFormat);
		}
	}

	//-----------------------------------------------------------------
	// RNG
	if (pParent->m_RngChkBox)
	{
		int count = pParent->m_RngList.GetItemCount();

		if (count > 0)
		{
			y += 25;
		}

		int x, w;
		for (int i = 0; i < count; ++i)
		{
			CString strSTime = pParent->m_RngList.GetItemText(i, 0);
			CString strETime = pParent->m_RngList.GetItemText(i, 1);
			CString strRngType = pParent->m_RngList.GetItemText(i, 2);

			TimeToCoordinate(strSTime, strETime, x, w);

			if (x < 0)
			{
				x = 0;
			}

			if (x + w > rect.right)
			{
				w = rect.Width() - x;
			}

			CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
			dc.FillSolidRect(&dRect, COLOR_OPE_RNG);
			dc.FrameRect(&dRect, &lCBrush);
			dc.DrawText(strRngType, &dRect, nFormat);
		}
	}

	//-----------------------------------------------------------------
	// CMD
	if (pParent->m_CmdChkBox)
	{
		int count = pParent->m_CmdList.GetItemCount();

		if (count > 0)
		{
			y += 25;
		}

		int x, w;
		for (int i = 0; i < count; ++i)
		{
			CString strSTime = pParent->m_CmdList.GetItemText(i, 0);
			CString strETime = pParent->m_CmdList.GetItemText(i, 1);

			TimeToCoordinate(strSTime, strETime, x, w);

			if (x < 0)
			{
				x = 0;
			}

			if (x + w > rect.right)
			{
				w = rect.Width() - x;
			}

			CRect dRect(x, y, x + w, y + DRAW_OPE_HEIGHT);
			dc.FillSolidRect(&dRect, COLOR_OPE_CMD);
			dc.FrameRect(&dRect, &lCBrush);
			dc.DrawText(_T("CMD"), &dRect, nFormat);
		}
	}
}


/*============================================================================*/
/*! COperationPlanGraph

-# 時刻より座標を求める

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlanGraph::TimeToCoordinate(CString strSTime, CString strETime, int &x, int &w)
{
	// 運用時刻の間隔
	COperationPlan *pParent = (COperationPlan*)GetParent();
	CTimeSpan antts = pParent->m_tOpeE - pParent->m_tOpeS;
	__int64 as = antts.GetTotalSeconds();
	double wps = (double)(DRAW_OPE_WIDTH / (double)as);

	COleDateTime oleSTime, oleETime;
	oleSTime.ParseDateTime(strSTime);
	oleETime.ParseDateTime(strETime);

	CTime cSTime = 0;
	if (strSTime.CompareNoCase(_T("1970-01-01 00:00:00")) != 0)
	{
		cSTime = CTime(oleSTime.GetYear(), oleSTime.GetMonth(), oleSTime.GetDay(), oleSTime.GetHour(), oleSTime.GetMinute(), oleSTime.GetSecond());
	}

	CTime cETime = 0;
	if (strETime.CompareNoCase(_T("1970-01-01 00:00:00")) != 0)
	{
		cETime = CTime(oleETime.GetYear(), oleETime.GetMonth(), oleETime.GetDay(), oleETime.GetHour(), oleETime.GetMinute(), oleETime.GetSecond());
	}

	CTimeSpan ts = cETime - cSTime;
	__int64 s = ts.GetTotalSeconds();

	w = (int)((DRAW_OPE_WIDTH * s) / (double)as);

	ts = cSTime - pParent->m_tOpeS;

	x = DRAW_OPE_X + (int)(ts.GetTotalSeconds() * wps);
}

/*============================================================================*/
/*! COperationPlanGraph

-# 時刻より座標を求める

@param  なし
@retval なし
*/
/*============================================================================*/
void COperationPlanGraph::TimeToCoordinate(CTime tS, CTime tE, int &x, int &w)
{
	// 運用時刻の間隔
	COperationPlan *pParent = (COperationPlan*)GetParent();
	CTimeSpan antts = pParent->m_tOpeE - pParent->m_tOpeS;
	__int64 as = antts.GetTotalSeconds();
	double wps = (double)(DRAW_OPE_WIDTH / (double)as);

	CTimeSpan ts = tE - tS;
	__int64 s = ts.GetTotalSeconds();

	w = (int)((DRAW_OPE_WIDTH * s) / (double)as);

	ts = tS - pParent->m_tOpeS;

	x = DRAW_OPE_X + (int)(ts.GetTotalSeconds() * wps);
}