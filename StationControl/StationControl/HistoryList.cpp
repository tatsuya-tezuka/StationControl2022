// HistoryList.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "HistoryList.h"

#include <locale.h>

// CHistoryList

IMPLEMENT_DYNAMIC(CHistoryList, CListCtrl)

CHistoryList::CHistoryList()
{
	mParentWnd = NULL;
}

CHistoryList::~CHistoryList()
{
}


BEGIN_MESSAGE_MAP(CHistoryList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CHistoryList::OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCCALCSIZE()
END_MESSAGE_MAP()

// CHistoryList ���b�Z�[�W �n���h���[

void CHistoryList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	int nR, nC;
	NMCUSTOMDRAW* pia = (NMCUSTOMDRAW*)pNMHDR;
	NMLVCUSTOMDRAW *plvcd = (NMLVCUSTOMDRAW*)pia;
	DWORD dw;
	bool bEquipment;
	switch (plvcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		nR = (int)plvcd->nmcd.dwItemSpec;
		nC = plvcd->iSubItem;
		dw = (DWORD)GetItemData(nR);
		bEquipment = (dw & 0x80000000)?true:false;
		dw &= 0x7fffffff;
		if (dw == eHistStatus_Normal){
			plvcd->clrTextBk = RGB(0, 0, 0);
			if(bEquipment)
				plvcd->clrText = RGB(0, 255, 0);
			else
				plvcd->clrText = RGB(255, 255, 255);
		}
		else if (dw == eHistStatus_Warning){
			plvcd->clrTextBk = RGB(0, 0, 0);
			plvcd->clrText = RGB(255, 255, 0);
		}
		else if (dw == eHistStatus_Fault){
			plvcd->clrTextBk = RGB(0, 0, 0);
			plvcd->clrText = RGB(255, 0, 0);
		}
		else{
			plvcd->clrTextBk = RGB(0, 0, 0);
			plvcd->clrText = RGB(255, 255, 255);
		}
		if (nC == eFilterItem_Station){
			CString str = GetItemText(nR, nC);
			plvcd->clrTextBk = RGB(255, 255, 255);
			for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); ++i) {
				if (str == mStationString[i]){
					plvcd->clrTextBk = mStationColor[i];
					break;
				}
			}
			plvcd->clrText = RGB(0, 0, 0);
		}
		*pResult = CDRF_DODEFAULT;
		break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}

void CHistoryList::OnLButtonDown(UINT nFlags, CPoint point)
{
	int item = HitTest(point);
	UINT val = GetItemState(item, LVIS_SELECTED);
	if (val == LVIS_SELECTED){
		SetItemState(item, 0, LVIS_SELECTED);
		if (mParentWnd)	mParentWnd->PostMessage(eMessage_History, 0, 0);
		return;
	}
	else{
		if (mParentWnd)	mParentWnd->PostMessage(eMessage_History, 0, 1);
	}

	CListCtrl::OnLButtonDown(nFlags, point);
}


void CHistoryList::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	//��ɃX�N���[���o�[��\��������
	EnableScrollBarCtrl(SB_VERT | SB_HORZ, TRUE);

	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

/*============================================================================*/
/*! �^�p����

-# ��������CSV�t�@�C���o��

@param	strTitle		CSV�o�͗p�^�C�g��
@retval

*/
/*============================================================================*/
void CHistoryList::CsvOutput(CString& strTitle)
{
	const TCHAR BASED_CODE szFilter[] = _T("CSV File(*.csv)|*.csv|All Files (*.*)|*.*||");
	CString defName;
	CTime ctime;
	theApp.GetCurrentUtcTime(ctime);
	CString temp = ctime.Format("%Y%m%d%H%M%S");
	defName.Format(_T("Hist%s.csv"), (LPCTSTR)temp);
	CFileDialog dlg(FALSE, _T("csv"), defName, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() != IDOK)
		return;
	
	TCHAR* p = _wsetlocale(LC_CTYPE, _T("japanese"));

	CStdioFile file;
	if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite) == FALSE){
		_wsetlocale(LC_ALL, p);
		return;
	}

	try
	{
		CString line, str;

		line = strTitle + _T("\n");
		file.WriteString(line);

		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int maxcol = pHeader->GetItemCount();
		TCHAR Buffer[1024] = { 0 };
		HDITEM  phi;
		phi.mask = HDI_TEXT;
		phi.cchTextMax = sizeof(Buffer);
		phi.pszText = Buffer;
		line.Empty();
		for (int col = 0; col < maxcol; col++){
			pHeader->GetItem(col, &phi);
			if (line.IsEmpty() == false)
				line += _T(",");
			str = CString(Buffer);
			str.TrimLeft();
			str.TrimRight();
			line += str;
		}
		line += _T("\n");
		file.WriteString(line);

		int count = GetItemCount();
		for (int i = 0; i < count; i++){
			line.Empty();
			for (int col = 0; col < maxcol; col++){
				str = GetItemText(i, col);
				if (line.IsEmpty() == false)
					line += _T(",");
				str.TrimLeft();
				str.TrimRight();
				line += _T("\"") + str + _T("\"");
			}
			line += _T("\n");
			file.WriteString(line);
		}
		file.Flush();
	}
	catch (CFileException* e)
	{
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}
	file.Close();
	_wsetlocale(LC_ALL, p);
}

/*============================================================================*/
/*! �^�p����

-# �������̈��

@param	strTitle		����o�͗p�^�C�g��
@retval

*/
/*============================================================================*/
#define HISOUT_OPR_EXT		_T("vrxo")	// �^�p���𒠕[�p�e���|�����g���q
#define HISOUT_CTRL_EXT		_T("vrxc")	// ���䗚�𒠕[�p�e���|�����g���q
#define HISOUT_MONI_EXT		_T("vrxm")	// �����ԗ��𒠕[�p�e���|�����g���q
#define HISOUT_EVENT_EXT	_T("vrxe")	// �C�x���g���𒠕[�p�e���|�����g���q
void CHistoryList::PrintOutput(CString& strTitle, CString& strFilter, int nFilterType, stFilterData& data)
{
#if 1
	CString path, fname;

	UINT nStation = theApp.GetSelectStation();

	CTime stime, ptime, time;
	CTime start, stop;
	stime = CTime(data.stime);
	ptime = CTime(data.ptime);
	start = __min(stime, ptime);
	stop = __max(stime, ptime);
//	stime = CTime(start.GetYear(), start.GetMonth(), start.GetDay(), 0, 0, 0);
//	ptime = CTime(stop.GetYear(), stop.GetMonth(), stop.GetDay(), 0, 0, 0);

	FILE *fpout;
	char	szWrite[2048];

	// ���[�p�e���|�����t�@�C�����擾
	switch (nFilterType){
	case eHistory_SearchOperation:	// �^�p����
		{
			if (theApp.GetSelectStationMode() == eStation_1)
			{
				// 1��
				path = theApp.GetShareFilePath(eFileType_OpHistory_Folder, nStation);
				fname.Format(_T("%sopr.%s"), (LPCTSTR)path, HISOUT_OPR_EXT);
			}
			else
			{
				// 4��
				CString strFilePath;
				strFilePath.Format(_T("\\\\%sshare\\log\\opr\\"), (LPCTSTR)theApp.GetFileServerName());
				if (theApp.GetFileServerName().IsEmpty() == true){
					strFilePath.Format(_T("C:\\share\\log\\opr\\"));
				}
				path = strFilePath;
				fname.Format(_T("%sopr.%s"), (LPCTSTR)path, HISOUT_OPR_EXT);
			}
		}

		break;
	case eHistory_SearchControl:	// ���䗚��
		{
			if (theApp.GetSelectStationMode() == eStation_1)
			{
				// 1��
				path = theApp.GetShareFilePath(eFileType_CtrlHistory_Folder, nStation);
				fname.Format(_T("%sctrl.%s"), (LPCTSTR)path, HISOUT_CTRL_EXT);
			}
			else
			{
				// 4��
				CString strFilePath;
				strFilePath.Format(_T("\\\\%sshare\\log\\ctrl\\"), (LPCTSTR)theApp.GetFileServerName());
				if (theApp.GetFileServerName().IsEmpty() == true)
				{
					strFilePath.Format(_T("C:\\share\\log\\ctrl\\"));
				}
				path = strFilePath;
				fname.Format(_T("%sctrl.%s"), (LPCTSTR)path, HISOUT_CTRL_EXT);
			}
		}
		break;

	case eHistory_SearchEquipment:	// �ݔ���ԗ���
		{
			if (theApp.GetSelectStationMode() == eStation_1)
			{
				// 1��
				path = theApp.GetShareFilePath(eFileType_EquipHistory_Folder, nStation);
				fname.Format(_T("%smoni.%s"), (LPCTSTR)path, HISOUT_MONI_EXT);
			}
			else
			{
				// 4��
				CString strFilePath;
				strFilePath.Format(_T("\\\\%sshare\\log\\moni\\"), (LPCTSTR)theApp.GetFileServerName());
				if (theApp.GetFileServerName().IsEmpty() == true)
				{
					strFilePath.Format(_T("C:\\share\\log\\moni\\"));
				}
				path = strFilePath;
				fname.Format(_T("%smoni.%s"), (LPCTSTR)path, HISOUT_MONI_EXT);
			}
		}
		break;

	case eHistory_SearchEvent:		// �C�x���g����
		{
			if (theApp.GetSelectStationMode() == eStation_1)
			{
				// 1��
				path = theApp.GetShareFilePath(eFileType_EventHistory_Folder, nStation);
				fname.Format(_T("%sevent.%s"), (LPCTSTR)path, HISOUT_EVENT_EXT);
			}
			else
			{
				// 4��
				CString strFilePath;
				strFilePath.Format(_T("\\\\%sshare\\log\\event\\"), (LPCTSTR)theApp.GetFileServerName());
				if (theApp.GetFileServerName().IsEmpty() == true)
				{
					strFilePath.Format(_T("C:\\share\\log\\event\\"));
				}
				path = strFilePath;
				fname.Format(_T("%sevent.%s"), (LPCTSTR)path, HISOUT_EVENT_EXT);
			}
		}
		break;

	case eHistory_SearchAlarm:
	default:
		return;
	}

	if ((_wfopen_s(&fpout, fname, _T("w")) != 0) || (fpout == NULL))
	{
		return;
	}

	// �o�͊J�n����
	CString strSTime;
	if (start == 0)
	{
		strSTime = _T("");
	}
	else
	{
		strSTime = start.Format("%Y-%m-%d %H:%M:%S");
	}
	sprintf_s(szWrite, "�o�͊J�n����=\"%s\"\n", (LPCSTR)CStringA(strSTime));
	fputs(szWrite, fpout);

	// �o�͏I������
	CString strETime;
	if (stop == 0)
	{
		strETime = _T("");
	}
	else
	{
		strETime = stop.Format("%Y-%m-%d %H:%M:%S");
	}
	sprintf_s(szWrite, "�o�͏I������=\"%s\"\n", (LPCSTR)CStringA(strETime));
	fputs(szWrite, fpout);

	// �t�B���^�[�F��
	CString strStaion = _T("");
	UINT    nSender1 = 0;
	UINT    nSender2 = 0;
	UINT    nSender3 = 0;
	UINT    nSender4 = 0;

	if (data.station & 0x08)	// ���V�Y20m
	{
		strStaion += _T("���V�Y20m��");

		if (data.sender[2] & 0x01)	// ���M���F�ǉ^�p�ǐ����u
		{
			nSender1 |= 0x08;
		}
		if (data.sender[2] & 0x02)	// ���M���F�ݔ����䑕�u
		{
			nSender2 |= 0x08;
		}
		if (data.sender[2] & 0x04)	// ���M���FX��TLM���͑��u(2)
		{
			nSender3 |= 0x08;
		}
		if (data.sender[2] & 0x08)	// ���M���FX��CMD���͑��u(2)
		{
			nSender4 |= 0x08;
		}
	}
	if (data.station & 0x04)	// ���V�Y34m
	{
		if (strStaion.IsEmpty() == FALSE)
			strStaion += _T(",");
		strStaion += _T("���V�Y34m��");

		if (data.sender[1] & 0x01)	// ���M���F�ǉ^�p�ǐ����u
		{
			nSender1 |= 0x04;
		}
		if (data.sender[1] & 0x02)	// ���M���F�ݔ����䑕�u
		{
			nSender2 |= 0x04;
		}
		if (data.sender[1] & 0x04)	// ���M���FX��TLM���͑��u(2)
		{
			nSender3 |= 0x04;
		}
		if (data.sender[1] & 0x08)	// ���M���FX��CMD���͑��u(2)
		{
			nSender4 |= 0x04;
		}
	}
	if (data.station & 0x01)	// �P�c64m
	{
		if (strStaion.IsEmpty() == FALSE)
			strStaion += _T(",");
		strStaion += _T("�P�c64m��");

		if (data.sender[0] & 0x01)	// ���M���F�ǉ^�p�ǐ����u
		{
			nSender1 |= 0x01;
		}
		if (data.sender[0] & 0x02)	// ���M���F�ݔ����䑕�u
		{
			nSender2 |= 0x01;
		}
		if (data.sender[0] & 0x04)	// ���M���FX��TLM���͑��u(2)
		{
			nSender3 |= 0x01;
		}
		if (data.sender[0] & 0x08)	// ���M���FX��CMD���͑��u(2)
		{
			nSender4 |= 0x01;
		}
	}
	if (data.station & 0x02)	// �P�c54m
	{
		if (strStaion.IsEmpty() == FALSE)
			strStaion += _T(",");
		strStaion += _T("�P�c54m��");

		if (data.sender[3] & 0x01)	// ���M���F�ǉ^�p�ǐ����u
		{
			nSender1 |= 0x02;
		}
		if (data.sender[3] & 0x02)	// ���M���F�ݔ����䑕�u
		{
			nSender2 |= 0x02;
		}
		if (data.sender[3] & 0x04)	// ���M���FX��TLM���͑��u(2)
		{
			nSender3 |= 0x02;
		}
		if (data.sender[3] & 0x08)	// ���M���FX��CMD���͑��u(2)
		{
			nSender4 |= 0x02;
		}
	}
	if (data.station & 0x10)	// �}�g�ǉ^�p�ǐ�
	{
		if (strStaion.IsEmpty() == FALSE)
			strStaion += _T(",");
		strStaion += _T("�}�g�ǉ^�p�ǐ�");
	}

	sprintf_s(szWrite, "�t�B���^�[�i�ǁj=\"%s\"\n", (LPCSTR)CStringA(strStaion));
	fputs(szWrite, fpout);

	// �t�B���^�[�F���M�����u
	CString strSender1 = _T("�ǉ^�p�ǐ����u");		// �ǉ^�p�ǐ����u
	CString strSender2 = _T("�ݔ����䑕�u");		// �ݔ����䑕�u
	CString strSender3 = _T("X��TLM���͑��u(2)");	// X��TLM���͑��u(2)
	CString strSender4 = _T("X��CMD���͑��u(2)");	// X��CMD���͑��u(2)

	if (theApp.GetSelectStationMode() == eStation_5 && data.sender[0] == 0x15)
	{
		// 4�ǃ��[�h�őS�Ă̑��M�����u������ꍇ
		sprintf_s(szWrite, "�t�B���^�[�i���M�����u�j=%s\n", "ALL");
		fputs(szWrite, fpout);
	}
	else
	{
		// ��L�ȊO�́A�X�ɕ\��
		CString strSenderTmp1;
		CString strSenderTmp2;
		CString strSenderTmp3;
		CString strSenderTmp4;
		CString strSenderTmp;
		strSenderTmp1 = _T("");
		strSenderTmp2 = _T("");
		strSenderTmp3 = _T("");
		strSenderTmp4 = _T("");
		strSenderTmp = _T("");

		//-------------------------------------------------------------
		// ���M���F�ǉ^�p�ǐ����u
		if ((data.station & 0x08) && (nSender1 & 0x08))	// ���V�Y20m
		{
			if (strSenderTmp1.IsEmpty() == FALSE)
				strSenderTmp1 += _T(",");
			strSenderTmp1 += _T("���V�Y20m��");
		}
		if ((data.station & 0x04) && (nSender1 & 0x04))	// ���V�Y34m
		{
			if (strSenderTmp1.IsEmpty() == FALSE)
				strSenderTmp1 += _T(",");
			strSenderTmp1 += _T("���V�Y34m��");
		}
		if ((data.station & 0x01) && (nSender1 & 0x01))	// �P�c64m
		{
			if (strSenderTmp1.IsEmpty() == FALSE)
				strSenderTmp1 += _T(",");
			strSenderTmp1 += _T("�P�c64m��");
		}
		if ((data.station & 0x02) && (nSender1 & 0x02))	// �P�c54m
		{
			if (strSenderTmp1.IsEmpty() == FALSE)
				strSenderTmp1 += _T(",");
			strSenderTmp1 += _T("�P�c54m��");
		}

		if (strSenderTmp1.IsEmpty() == FALSE)
		{
			if (strSenderTmp.IsEmpty() == FALSE)
			{
				strSenderTmp += _T(",");
			}
			strSenderTmp += _T("�ǉ^�p�ǐ����u(") + strSenderTmp1 + _T(")");
		}

		//-------------------------------------------------------------
		// ���M���F�ݔ����䑕�u
		if ((data.station & 0x08) && (nSender2 & 0x08))	// ���V�Y20m
		{
			if (strSenderTmp2.IsEmpty() == FALSE)
				strSenderTmp2 += _T(",");
			strSenderTmp2 += _T("���V�Y20m��");
		}
		if ((data.station & 0x04) && (nSender2 & 0x04))	// ���V�Y34m
		{
			if (strSenderTmp2.IsEmpty() == FALSE)
				strSenderTmp2 += _T(",");
			strSenderTmp2 += _T("���V�Y34m��");
		}
		if ((data.station & 0x01) && (nSender2 & 0x01))	// �P�c64m
		{
			if (strSenderTmp2.IsEmpty() == FALSE)
				strSenderTmp2 += _T(",");
			strSenderTmp2 += _T("�P�c64m��");
		}
		if ((data.station & 0x02) && (nSender2 & 0x02))	// �P�c54m
		{
			if (strSenderTmp2.IsEmpty() == FALSE)
				strSenderTmp2 += _T(",");
			strSenderTmp2 += _T("�P�c54m��");
		}

		if (strSenderTmp2.IsEmpty() == FALSE)
		{
			if (strSenderTmp.IsEmpty() == FALSE)
			{
				strSenderTmp += _T(",");
			}
			strSenderTmp += _T("�ݔ����䑕�u(") + strSenderTmp2 + _T(")");
		}

		//-------------------------------------------------------------
		// ���M���FX��TLM���͑��u(2)
		if ((data.station & 0x08) && (nSender3 & 0x08))	// ���V�Y20m
		{
			if (strSenderTmp3.IsEmpty() == FALSE)
				strSenderTmp3 += _T(",");
			strSenderTmp3 += _T("���V�Y20m��");
		}
		if ((data.station & 0x04) && (nSender3 & 0x04))	// ���V�Y34m
		{
			if (strSenderTmp3.IsEmpty() == FALSE)
				strSenderTmp3 += _T(",");
			strSenderTmp3 += _T("���V�Y34m��");
		}
		if ((data.station & 0x01) && (nSender3 & 0x01))	// �P�c64m
		{
			if (strSenderTmp3.IsEmpty() == FALSE)
				strSenderTmp3 += _T(",");
			strSenderTmp3 += _T("�P�c64m��");
		}
		if ((data.station & 0x02) && (nSender3 & 0x02))	// �P�c54m
		{
			if (strSenderTmp3.IsEmpty() == FALSE)
				strSenderTmp3 += _T(",");
			strSenderTmp3 += _T("�P�c54m��");
		}

		if (strSenderTmp3.IsEmpty() == FALSE)
		{
			if (strSenderTmp.IsEmpty() == FALSE)
			{
				strSenderTmp += _T(",");
			}
			strSenderTmp += _T("X��TLM���͑��u(2)(") + strSenderTmp3 + _T(")");
		}

		//-------------------------------------------------------------
		// ���M���FX��CMD���͑��u(2)
		if ((data.station & 0x08) && (nSender4 & 0x08))	// ���V�Y20m
		{
			if (strSenderTmp4.IsEmpty() == FALSE)
				strSenderTmp4 += _T(",");
			strSenderTmp4 += _T("���V�Y20m��");
		}
		if ((data.station & 0x04) && (nSender4 & 0x04))	// ���V�Y34m
		{
			if (strSenderTmp4.IsEmpty() == FALSE)
				strSenderTmp4 += _T(",");
			strSenderTmp4 += _T("���V�Y34m��");
		}
		if ((data.station & 0x01) && (nSender4 & 0x01))	// �P�c64m
		{
			if (strSenderTmp4.IsEmpty() == FALSE)
				strSenderTmp4 += _T(",");
			strSenderTmp4 += _T("�P�c64m��");
		}
		if ((data.station & 0x02) && (nSender4 & 0x02))	// �P�c54m
		{
			if (strSenderTmp4.IsEmpty() == FALSE)
				strSenderTmp4 += _T(",");
			strSenderTmp4 += _T("�P�c54m��");
		}

		if (strSenderTmp4.IsEmpty() == FALSE)
		{
			if (strSenderTmp.IsEmpty() == FALSE)
			{
				strSenderTmp += _T(",");
			}
			strSenderTmp += _T("X��CMD���͑��u(2)(") + strSenderTmp4 + _T(")");
		}

		sprintf_s(szWrite, "�t�B���^�[�i���M�����u�j=\"%s\"\n", (LPCSTR)CStringA(strSenderTmp));
		fputs(szWrite, fpout);
	}


	// �t�B���^�[�F�d�v�x
	CString strImstatus = _T("");
	if (data.imstatus == 0x07)
	{
		strImstatus = _T("ALL");
	}
	else
	{
		if (data.imstatus & 0x01)
		{
			if (strImstatus.IsEmpty() == FALSE)
				strImstatus += _T(",");
			strImstatus += _T("���");
		}
		if (data.imstatus & 0x02)
		{
			if (strImstatus.IsEmpty() == FALSE)
				strImstatus += _T(",");
			strImstatus += _T("�x��");
		}
		if (data.imstatus & 0x04)
		{
			if (strImstatus.IsEmpty() == FALSE)
				strImstatus += _T(",");
			strImstatus += _T("�ُ�");
		}
	}
	sprintf_s(szWrite, "�t�B���^�[�i�d�v�x�j=\"%s\"\n", (LPCSTR)CStringA(strImstatus));
	fputs(szWrite, fpout);

	// �t�B���^�[�F���b�Z�[�W�ԍ�
	if (data.message != 0)
	{
		sprintf_s(szWrite, "�t�B���^�[�i���b�Z�[�W�ԍ��j=\"%d\"\n", data.message);
	}
	else
	{
		sprintf_s(szWrite, "�t�B���^�[�i���b�Z�[�W�ԍ��j=\"ALL\"\n");
	}
	fputs(szWrite, fpout);

	// ����
	int count = GetItemCount();
	CString line, str;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	for (int i = 0; i < count; i++){
		line.Empty();
		for (int col = 0; col < maxcol; col++){
			str = GetItemText(i, col);
			if (line.IsEmpty() == false)
				line += _T(",");
			str.TrimLeft();
			str.TrimRight();
			line += _T("\"") + str + _T("\"");
		}
		line += _T("\n");

		sprintf_s(szWrite, "%s", (LPCSTR)CStringA(line));
		fputs(szWrite, fpout);
	}

	fclose(fpout);

	// ���[
	CSatelliteData::ExcelOutPut(m_hWnd, fname);

#else
	CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_HIDEPRINTTOFILE | PD_NOPAGENUMS | PD_RETURNDC | PD_USEDEVMODECOPIES);
	//dlg.m_pd.Flags |= PD_SELECTION;
	dlg.m_pd.Flags |= PD_NOSELECTION;

	switch (dlg.DoModal())
	{
	case 0:
	case IDCANCEL:
		return;
	case IDOK:
		break;
	default:
		ASSERT(FALSE);
		return;
	}

	CDC dc;
	dc.Attach(dlg.m_pd.hDC);
	CHistoryPrint *printer;
	printer = new CHistoryPrint(&dc);

	printer->SetTitle(strTitle, strFilter);

	printer->SetTitle(strTitle, strFilter);
	if (printer->StartPrinting(this))
	{
		CString line, str;
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int maxcol = pHeader->GetItemCount();
		TCHAR Buffer[1024];
		HDITEM  phi;
		phi.mask = HDI_TEXT;
		phi.cchTextMax = sizeof(Buffer);
		phi.pszText = Buffer;
		//line.Empty();
		//for (int col = 0; col < maxcol; col++){
		//	pHeader->GetItem(col, &phi);
		//	str = CString(Buffer);
		//	line += str;
		//}
		//printer->PrintLine(line);
		printer->PrintLine(-1);

		int count = GetItemCount();
		for (int i = 0; i < count; i++){
			printer->PrintLine(i);
			//line.Empty();
			//for (int col = 0; col < maxcol; col++){
			//	str = GetItemText(i, col);
			//	line += str;
			//}
			//printer->PrintLine(line);
		}

		printer->EndPrinting();
	}
	delete printer;
	::DeleteDC(dc.Detach());
#endif
}









/*============================================================================*/
/*! �^�p����

-# ��������R���X�g���N�^

@param	dc			����p�f�o�C�X�R���e�L�X�g
@retval

*/
/*============================================================================*/
CHistoryPrint::CHistoryPrint(CDC* dc)
{
	mDc = dc;

	mPageVMargin = 0;
	mPageHMargin = 0;
	mPageHeight = 0;
	mSaveVMargin = 0;
	mLineHeight = 0;
	mPageStarted = FALSE;
	mDocStarted = FALSE;
	mPageNumber = 0;

	mFontHeader = NULL;
	mFontSubHeader = NULL;
	mFontBody = NULL;
}

/*============================================================================*/
/*! �^�p����

-# ��������f�X�g���N�^

@param
@retval

*/
/*============================================================================*/
CHistoryPrint::~CHistoryPrint()
{
	if (mDocStarted)
		EndPrinting();

	if (mFontHeader != NULL)
		delete mFontHeader;
	if (mFontSubHeader != NULL)
		delete mFontSubHeader;
	if (mFontBody != NULL)
		delete mFontBody;
}

/*============================================================================*/
/*! �^�p����

-# ��������J�n

@param
@retval	BOOL

*/
/*============================================================================*/
void CHistoryPrint::CalcRect(CListCtrl* plist)
{
	// 1�����̃T�C�Y�����߂�
	CSize charSize = mDc->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"), 52);
	charSize.cx /= 52;

	CHeaderCtrl* pHeader = (CHeaderCtrl*)plist->GetDlgItem(0);
	int maxcol = pHeader->GetItemCount();
	TCHAR Buffer[1024] = { 0 };
	HDITEM  phi;
	phi.mask = HDI_TEXT;
	phi.cchTextMax = sizeof(Buffer);
	phi.pszText = Buffer;
	CString str;
	vector<int> lenlist;
	int len, max;
	// �w�b�_�[�R���g���[���̊e���ڂ̕������߂�
	for (int col = 0; col < maxcol; col++){
		pHeader->GetItem(col, &phi);
		str = CString(Buffer);
		str.TrimLeft();
		str.TrimRight();
		len = (int)strlen((char*)(const char*)CStringA(str));
		lenlist.push_back(len+1);
	}
	// �S���X�g�̍ő啝�����߂�
	int count = plist->GetItemCount();
	for (int i = 0; i < count; i++){
		for (int col = 0; col < maxcol; col++){
			str = plist->GetItemText(i, col);
			str.TrimLeft();
			str.TrimRight();
			len = (int)strlen((char*)(const char*)CStringA(str));
			max = lenlist[col];
			lenlist[col] = __max(len+1, max);
		}
	}
	mWidthList.clear();
	for (int col = 0; col < maxcol; col++){
		mWidthList.push_back(lenlist[col] * charSize.cx);
	}
	mMaxCol = maxcol;
}

/*============================================================================*/
/*! �^�p����

-# ��������J�n

@param
@retval	BOOL

*/
/*============================================================================*/
BOOL CHistoryPrint::StartPrinting(CListCtrl* plist)
{
	DOCINFO info;
	::ZeroMemory(&info, sizeof(info));
	info.lpszDocName = AfxGetAppName();

	SetPrinterFont();
	CalcRect(plist);
	mpList = plist;

	mDc->StartDoc(&info);
	mDocStarted = TRUE;

	TEXTMETRIC tm;
	mDc->GetTextMetrics(&tm);
	mLineHeight = tm.tmHeight + tm.tmInternalLeading;
	mPageVMargin = mDc->GetDeviceCaps(LOGPIXELSY) / 2;
	mPageHMargin = mDc->GetDeviceCaps(LOGPIXELSX) / 2;
	mPageHeight = mDc->GetDeviceCaps(VERTRES);
	mPageWidth = mDc->GetDeviceCaps(HORZRES);
	mSaveVMargin = mPageVMargin;
	return TRUE;
}

/*============================================================================*/
/*! �^�p����

-# ������� 1�s���

@param		str		1�s������
@retval

*/
/*============================================================================*/
void CHistoryPrint::PrintLine(const CString& str)
{
	if (!mPageStarted || mSaveVMargin > mPageHeight - mPageVMargin)
	{
		// ���y�[�W
		if (mPageStarted){
			mDc->EndPage();
		}
		mDc->StartPage();
		mPageNumber++;
		mSaveVMargin = mPageVMargin;
		mPageStarted = TRUE;
		CString s(AfxGetAppName());
		PrintHeader(mTitle);
	}
	mDc->TextOut(mPageHMargin, mSaveVMargin, str);
	mSaveVMargin += mLineHeight;
}

void CHistoryPrint::PrintLine(const int line)
{
	if (!mPageStarted || mSaveVMargin > mPageHeight - mPageVMargin)
	{
		// ���y�[�W
		if (mPageStarted){
			mDc->EndPage();
		}
		mDc->StartPage();
		mPageNumber++;
		mSaveVMargin = mPageVMargin;
		mPageStarted = TRUE;
		CString s(AfxGetAppName());
		PrintHeader(mTitle);
	}

	if (line < 0){
		CHeaderCtrl* pHeader = (CHeaderCtrl*)mpList->GetDlgItem(0);
		int maxcol = pHeader->GetItemCount();
		TCHAR Buffer[1024] = { 0 };
		HDITEM  phi;
		phi.mask = HDI_TEXT;
		phi.cchTextMax = sizeof(Buffer);
		phi.pszText = Buffer;
		CString str;
		CRect rect = CRect(mPageHMargin, mSaveVMargin, mPageHMargin, mSaveVMargin + mLineHeight);
		for (int col = 0; col < maxcol; col++){
			pHeader->GetItem(col, &phi);
			str = CString(Buffer);
			str.TrimLeft();
			str.TrimRight();
			rect.right = rect.left + mWidthList[col];
			mDc->DrawText(str, -1, rect, DT_LEFT | DT_VCENTER);
			rect.left = rect.right;
		}
	}
	else{
		int count = mpList->GetItemCount();
		CString str;
		CRect rect = CRect(mPageHMargin, mSaveVMargin, mPageHMargin, mSaveVMargin + mLineHeight);
		for (int col = 0; col < mMaxCol; col++){
			str = mpList->GetItemText(line, col);
			str.TrimLeft();
			str.TrimRight();
			rect.right = rect.left + mWidthList[col];
			mDc->DrawText(str, -1, rect, DT_LEFT | DT_VCENTER);
			rect.left = rect.right;
		}
	}
	mSaveVMargin += mLineHeight;
}

/*============================================================================*/
/*! �^�p����

-# ��������I��

@param
@retval

*/
/*============================================================================*/
void CHistoryPrint::EndPrinting()
{
	mDc->EndPage();
	mDc->EndDoc();
	mDocStarted = FALSE;
}

/*============================================================================*/
/*! �^�p����

-# ��������I��

@param
@retval

*/
/*============================================================================*/
void CHistoryPrint::SetPrinterFont()
{
	mFontHeader = CreateFont(12, FW_BOLD);
	mFontSubHeader = CreateFont(10, FW_NORMAL);
	mFontBody = CreateFont(10, FW_NORMAL);

	//CFont f;
	//f.CreateStockObject(DEVICE_DEFAULT_FONT);
	mDc->SelectObject(mFontBody);
}

CFont* CHistoryPrint::CreateFont(int nPoints, int lfWeight)
{
	CFont defFont;
	CFont* oldFont = NULL;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	defFont.GetLogFont(&lf);

	TEXTMETRIC tm;
	mDc->GetTextMetrics(&tm);
	CFont* pFont = new CFont();
	CSize PaperPixelsPerInch(mDc->GetDeviceCaps(LOGPIXELSX), mDc->GetDeviceCaps(LOGPIXELSY));
	lf.lfWeight = lfWeight;
	lf.lfHeight = -MulDiv(nPoints, PaperPixelsPerInch.cy, 72);
	//_tcscpy(lf.lfFaceName, (TCHAR*)(const TCHAR*)strFaceName);
	BOOL bResult = pFont->CreateFontIndirect(&lf);
	return pFont;
}

/*============================================================================*/
/*! �^�p����

-# ������� �w�b�_�[���

@param
@retval

*/
/*============================================================================*/
void CHistoryPrint::PrintHeader(const CString& str)
{
	//CString s(AfxGetAppName());
	int headingY = mPageVMargin - (3 * mLineHeight) / 2;
	int lineY = mPageVMargin - mLineHeight / 2;
	int saved = mDc->SaveDC();
	mDc->SetTextColor(RGB(0, 0, 0));
	CPen pen(PS_SOLID, mDc->GetDeviceCaps(LOGPIXELSX) / 100, RGB(0, 0, 0));
	mDc->SelectObject(pen);
	mDc->TextOut(mPageHMargin, headingY, str);
	CString page;
	page.Format(_T("%d"), mPageNumber);
	int w = mDc->GetTextExtent(page).cx;
	mDc->TextOut(mPageWidth - mPageHMargin - w, headingY, page);
	mDc->MoveTo(mPageHMargin, lineY);
	mDc->LineTo(mPageWidth - mPageHMargin, lineY);
	mDc->RestoreDC(saved);
}
