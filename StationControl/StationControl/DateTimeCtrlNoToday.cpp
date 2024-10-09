// DateTimeCtrlNoToday.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "DateTimeCtrlNoToday.h"


// CDateTimeCtrlNoToday

IMPLEMENT_DYNAMIC(CDateTimeCtrlNoToday, CDateTimeCtrl)

CDateTimeCtrlNoToday::CDateTimeCtrlNoToday()
{

}

CDateTimeCtrlNoToday::~CDateTimeCtrlNoToday()
{
}


BEGIN_MESSAGE_MAP(CDateTimeCtrlNoToday, CDateTimeCtrl)
	ON_NOTIFY_REFLECT(DTN_DROPDOWN, &CDateTimeCtrlNoToday::OnDtnDropdown)
END_MESSAGE_MAP()



// CDateTimeCtrlNoToday メッセージ ハンドラー



// 今日の日付削除
void CDateTimeCtrlNoToday::OnDtnDropdown(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMonthCalCtrl *calendar = GetMonthCalCtrl();
	calendar->ModifyStyle(0, MCS_NOTODAY);
	*pResult = 0;
}

