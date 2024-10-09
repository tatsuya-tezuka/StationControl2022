// DateTimeCtrlNoToday.cpp : �����t�@�C��
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



// CDateTimeCtrlNoToday ���b�Z�[�W �n���h���[



// �����̓��t�폜
void CDateTimeCtrlNoToday::OnDtnDropdown(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMonthCalCtrl *calendar = GetMonthCalCtrl();
	calendar->ModifyStyle(0, MCS_NOTODAY);
	*pResult = 0;
}

