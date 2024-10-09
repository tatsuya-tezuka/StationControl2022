// TabOperationView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "TabOperationView.h"
#include "afxdialogex.h"


// CTabOperationView �_�C�A���O

IMPLEMENT_DYNCREATE(CTabOperationView, CDialogEx)

CTabOperationView::CTabOperationView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabOperationView::IDD, pParent)
	, mSatelliteVal(_T(""))
	, mPlanVal(_T(""))
	, mForecastVal(_T(""))
	, mOpeStatusVal(_T(""))
	, mOpeStartVal(_T(""))
	, mOpeStopVal(_T(""))
{

}

CTabOperationView::~CTabOperationView()
{
}

void CTabOperationView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SATELLITEVAL, m_stSatelliteVal);
	DDX_Control(pDX, IDC_STATIC_PLANVAL, m_stPlanVal);
	DDX_Control(pDX, IDC_STATIC_FORECASTVAL, m_stForecastVal);
	DDX_Control(pDX, IDC_STATIC_OPESTATUSVAL, m_stOpeStatusVal);
	DDX_Control(pDX, IDC_STATIC_OPESTARTVAL, m_stOpeStartVal);
	DDX_Control(pDX, IDC_STATIC_OPESTOPVAL, m_stOpeStopVal);
	DDX_Control(pDX, IDC_STATIC_SENDSTATUSVAL, m_stSendStatusVal);
	DDX_Control(pDX, IDC_STATIC_SENDSTATUS_RF, m_stSendStatuRf);
	DDX_Control(pDX, IDC_STATIC_SENDSTATUS_HOLDTONE, m_stSendStatusHoldTone);
	DDX_Control(pDX, IDC_STATIC_SENDSTATUS_RNGCONT, m_stSendStatusRngCont);
	DDX_Control(pDX, IDC_STATIC_RECVNEW_CARR, m_stRecvNewCarr);
	DDX_Control(pDX, IDC_STATIC_RECVNEW_SUBCARR, m_stRecvNewSubCarr);
	DDX_Control(pDX, IDC_STATIC_RECVNEW_VITERBI, m_stRecvNewViteRbi);
	DDX_Control(pDX, IDC_STATIC_RECVNEW_BIT, m_stRecvNewBit);
	DDX_Control(pDX, IDC_STATIC_RECVNEW_FRAME, m_stRecvNewFrame);
	DDX_Control(pDX, IDC_STATIC_RECVOLD_CARR, m_stRecvOldCarr);
	DDX_Control(pDX, IDC_STATIC_RECVOLD_SUBCARR, m_stRecvOldSubCarr);
	DDX_Control(pDX, IDC_STATIC_RECVOLD_VITERBI, m_stRecvOldViteRbi);
	DDX_Control(pDX, IDC_STATIC_RECVOLD_BIT, m_stRecvOldBit);
	DDX_Control(pDX, IDC_STATIC_RECVOLD_FRAME, m_stRecvOldFrame);
	DDX_Text(pDX, IDC_STATIC_SATELLITEVAL, mSatelliteVal);
	DDX_Text(pDX, IDC_STATIC_PLANVAL, mPlanVal);
	DDX_Text(pDX, IDC_STATIC_FORECASTVAL, mForecastVal);
	DDX_Text(pDX, IDC_STATIC_OPESTATUSVAL, mOpeStatusVal);
	DDX_Text(pDX, IDC_STATIC_OPESTARTVAL, mOpeStartVal);
	DDX_Text(pDX, IDC_STATIC_OPESTOPVAL, mOpeStopVal);
}


BEGIN_MESSAGE_MAP(CTabOperationView, CDialogEx)
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTabOperationView ���b�Z�[�W �n���h���[

BOOL CTabOperationView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_stSatelliteVal.SetGradient(false);
	m_stSatelliteVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stSatelliteVal.SetBackColorEx(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE));
	m_stSatelliteVal.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_stPlanVal.SetGradient(false);
	m_stPlanVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stPlanVal.SetBackColorEx(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE));
	m_stPlanVal.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_stForecastVal.SetGradient(false);
	m_stForecastVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stForecastVal.SetBackColorEx(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE));
	m_stForecastVal.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_stOpeStatusVal.SetGradient(false);
	m_stOpeStatusVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stOpeStatusVal.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stOpeStatusVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stOpeStartVal.SetGradient(false);
	m_stOpeStartVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stOpeStartVal.SetBackColorEx(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE));
	m_stOpeStartVal.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_stOpeStopVal.SetGradient(false);
	m_stOpeStopVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stOpeStopVal.SetBackColorEx(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE));
	m_stOpeStopVal.SetTextStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_stSendStatusVal.SetGradient(false);
	m_stSendStatusVal.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stSendStatusVal.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stSendStatusVal.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stSendStatuRf.SetGradient(false);
	m_stSendStatuRf.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stSendStatuRf.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stSendStatuRf.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stSendStatusHoldTone.SetGradient(false);
	m_stSendStatusHoldTone.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stSendStatusHoldTone.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stSendStatusHoldTone.SetTextStyle(DT_CENTER | DT_WORDBREAK | DT_VCENTER);

	m_stSendStatusRngCont.SetGradient(false);
	m_stSendStatusRngCont.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stSendStatusRngCont.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stSendStatusRngCont.SetTextStyle(DT_CENTER | DT_WORDBREAK | DT_VCENTER);

	m_stRecvNewCarr.SetGradient(false);
	m_stRecvNewCarr.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvNewCarr.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvNewCarr.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stRecvNewSubCarr.SetGradient(false);
	m_stRecvNewSubCarr.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvNewSubCarr.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvNewSubCarr.SetTextStyle(DT_CENTER | DT_WORDBREAK | DT_VCENTER);

	m_stRecvNewViteRbi.SetGradient(false);
	m_stRecvNewViteRbi.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvNewViteRbi.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvNewViteRbi.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stRecvNewBit.SetGradient(false);
	m_stRecvNewBit.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvNewBit.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvNewBit.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stRecvNewFrame.SetGradient(false);
	m_stRecvNewFrame.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvNewFrame.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvNewFrame.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stRecvOldCarr.SetGradient(false);
	m_stRecvOldCarr.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvOldCarr.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvOldCarr.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stRecvOldSubCarr.SetGradient(false);
	m_stRecvOldSubCarr.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvOldSubCarr.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvOldSubCarr.SetTextStyle(DT_CENTER | DT_WORDBREAK | DT_VCENTER);

	m_stRecvOldViteRbi.SetGradient(false);
	m_stRecvOldViteRbi.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvOldViteRbi.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvOldViteRbi.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stRecvOldBit.SetGradient(false);
	m_stRecvOldBit.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvOldBit.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvOldBit.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	m_stRecvOldFrame.SetGradient(false);
	m_stRecvOldFrame.SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_stRecvOldFrame.SetBackColorEx(GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
	m_stRecvOldFrame.SetTextStyle(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	mStation = theApp.GetOpenViewType();

	UINT nStationID = mStation - eViewType5_EqOperation1;

	switch (nStationID)
	{
	case eStation_Usuda64:
		(GetDlgItem(IDC_STATIC_RECVNEW))->SetWindowText(_T("X�ю�M�i�V�n�j"));
		(GetDlgItem(IDC_STATIC_RECVOLD))->SetWindowText(_T("X�ю�M�i���n�j"));
		break;
	case eStation_Usuda642:
		(GetDlgItem(IDC_STATIC_RECVSTATUS))->SetWindowText(_T(""));			//"��M��"        -> ""
		(GetDlgItem(IDC_STATIC_RECVNEW))->SetWindowText(_T("��M��"));	//"X�ю�M�i�V�n�j" -> "��M��"
		(GetDlgItem(IDC_STATIC_RECVOLD))->SetWindowText(_T(""));			//"X�ю�M�i���n�j" -> ""
		m_stRecvOldCarr.ShowWindow(SW_HIDE);
		m_stRecvOldSubCarr.ShowWindow(SW_HIDE);
		m_stRecvOldViteRbi.ShowWindow(SW_HIDE);
		m_stRecvOldBit.ShowWindow(SW_HIDE);
		m_stRecvOldFrame.ShowWindow(SW_HIDE);
		break;
	case eStation_Uchinoura34:
		(GetDlgItem(IDC_STATIC_RECVNEW))->SetWindowText(_T("S�ю�M"));
		(GetDlgItem(IDC_STATIC_RECVOLD))->SetWindowText(_T("X�ю�M"));
		break;
	case eStation_Uchinoura20:
		(GetDlgItem(IDC_STATIC_RECVNEW))->SetWindowText(_T("S�ю�M"));
		(GetDlgItem(IDC_STATIC_RECVOLD))->SetWindowText(_T("X�ю�M"));
		break;
	default:
		break;
	}

//	mTimeDataList.clear();
//	SetTimer(1, 10, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


//void CTabOperationView::OnTimer(UINT_PTR nIDEvent)
//{
//	static bool bFirst = true;
//	if (nIDEvent == 1){
//		if (bFirst == true)
//			KillTimer(1);
//		UpdateData(TRUE);
//		// ���ݎ����̎擾
//		theApp.GetCurrentUtcTime(mCurrentTime);
//		UpdateOperationPlanInfo();
//		if (bFirst == true)
//			SetTimer(1, 1000, NULL);
//		bFirst = false;
//		return;
//	}
//
//	CDialogEx::OnTimer(nIDEvent);
//}

///*============================================================================*/
///*! �^�p�󋵉��
//
//-# �^�p�v��A���e�i�\��l�擾
//
//@param
//@retval �Ȃ�
//*/
///*============================================================================*/
//void CTabOperationView::UpdateOperationPlanInfo()
//{
//	UpdateData(TRUE);
//#ifdef _NOCOMM
//	if (theApp.GetSelectStationMode() != eStation_5)
//		return;
//
//	UINT nStationID = mStation - eViewType5_EqOperation1;
//
//	CGraphMutex::Lock(eFile);
//	vector<stPlanListData>& planlist = theApp.GetSatelliteData().GetPlanList();
//	vector<stPlanListData>::iterator itr;
//
//	// ����ǂŌ��݉^�p���̉^�p�v���T���B
//	// ���݂��Ȃ��ꍇ�͖����ň�Ԍ��ݎ��Ԃɋ߂��^�p�v���T��
//	vector<stPlanListData>::iterator itrp = planlist.end();
//	__int64 curTime = mCurrentTime.GetTime();
//	__int64 nNearTime = 0;
//	bool bStatus = false;
//	for (itr = planlist.begin(); itr != planlist.end(); itr++){
//		UINT id = theApp.GetSatelliteData().GetStationID((*itr).strStationName);
//		if (id == nStationID){
//			if (curTime >= (*itr).tPassStart && curTime <= (*itr).tPassEnd){
//				itrp = itr;
//				bStatus = true;
//				break;
//			}
//			if (curTime < (*itr).tPassStart){
//				// ��ԋ߂��^�p�v���T��
//				if ((curTime - nNearTime) >((*itr).tPassStart - curTime)){
//					itrp = itr;
//					nNearTime = (*itr).tPassStart;
//				}
//			}
//		}
//	}
//
//	if (itrp == planlist.end()){
//		CGraphMutex::Unlock(eFile);
//		return;
//	}
//
//	CString str;
//
//	mSatelliteVal = (*itrp).strSatelliteName;
//	mPlanVal = (*itrp).strPassId;
//	mForecastVal = ConvertPredfileToId((*itrp).strPredName);
//	if (bStatus == true){
//		mOpeStatusVal = _T("�p�X�^�p��");
//		m_stOpeStatusVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
//	}
//	else{
//		mOpeStatusVal = _T("��^�p��");
//		m_stOpeStatusVal.SetBackColor(RGB(192, 192, 192), RGB(192, 192, 192));
//	}
//	m_stOpeStatusVal.RedrawWindow();
//	CTime ctime;
//	ctime = CTime((*itrp).tPassStart);
//	str.Format(_T("%s %s"), ctime.Format(_T("%Y-%m-%d")), ctime.Format(_T("%H:%M:%S")));
//	mOpeStartVal = str;
//	ctime = CTime((*itrp).tPassEnd);
//	str.Format(_T("%s %s"), ctime.Format(_T("%Y-%m-%d")), ctime.Format(_T("%H:%M:%S")));
//	mOpeStopVal = str;
//
//	CGraphMutex::Unlock(eFile);
//
//#endif
//	UpdateData(FALSE);
//}

/*============================================================================*/
/*! �^�p�󋵉��

-# �\��l�t�@�C��������\��lID�ւ̕ϊ�

@param		name	�\��l�t�@�C����
@retval �\��lID������
*/
/*============================================================================*/
CString CTabOperationView::ConvertPredfileToId(CString name)
{
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];
	TCHAR	szFname[_MAX_FNAME];
	_wsplitpath_s(name, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, NULL, 0);
	CString str, temp;
	temp = szFname;
	str.Format(_T("%s-%s"), (LPCTSTR)(temp.Left(6)), (LPCTSTR)(temp.Mid(6)));
	return str;
}


/*============================================================================*/
/*! S�ю�M

-# �ߑ��󋵃X�e�[�^�X�̍X�V

@param
@retval

*/
/*============================================================================*/
void CTabOperationView::UpdateStatus(UINT nStationID)
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mOperationParam[nStationID]) / sizeof(mOperationParam[nStationID][0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mOperationParam[nStationID][i].name;
		CString str;
//		GetDlgItem(mOperationParam[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mOperationParam[nStationID][i].type;

//		{
//			CString strDebug;
//			strDebug.Format(_T("%s(%d) :: mControlString(%s)"), __FILEW__, __LINE__, mControlString);
//			CLogTraceEx::Write(_T(""), _T("CTabScaptView20"), _T("UpdateStatus"), _T("debug"), strDebug, nLogEx::debug);
//		}

		if (mControlString.IsEmpty())
		{
			continue;
		}

		// �Ď��f�[�^����擾
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStationID, mControlString);

		if (obs_adr == NULL)
		{
			//if(nStationID == eStation_Usuda642)
			//{
			//	TRACE("54m GetObsname_t()�G���[ �Ď���:%s\n", CStringA(mControlString));
			//}
			continue;
		}

		switch (val.type){
		case	eControlStatus:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		case	eControlNumeric:
			str.Format(_T("%d"), obs_adr->l_data);
			break;
		case	eControlDouble:
			str.Format(_T("%0.3f"), obs_adr->d_data);
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		// �����E�\���F�Z�b�g
		switch (mOperationParam[nStationID][i].id)
		{
		// �q����
		// �v��ID
		case IDC_STATIC_SATELLITEVAL:
		case IDC_STATIC_PLANVAL:
			((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(str);
			break;

		// �\��l
		case IDC_STATIC_FORECASTVAL:
			str.Trim(_T(" "));
			if (!str.IsEmpty())
			{
//				// �Ď��̃t�@�C���p�X����t�@�C������؂�o��
//				CString strFileName = str.Right(str.GetLength() - str.ReverseFind(_T('/')) - 1);
//				// �t�@�C��������g���q���O��
//				CString strYohoID = str.Right(str.GetLength() - str.ReverseFind(_T('.')) - 1);
//				// �t�@�C�����̍�����6�������"-"������
//				strYohoID.Insert(6, _T('-'));
//				((CColorStatic*)GetDlgItem(mOperationParam[i].id))->SetText(strYohoID);
				CString strYohoID = ConvertPredfileToId(str);
				((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(strYohoID);
			}
			break;

		// �^�p���
		case IDC_STATIC_OPESTATUSVAL:
			if (str.Compare(_T("�p�X��")) == 0)
			{
				str.Format(_T("�p�X�^�p��"));
				m_stOpeStatusVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else if (str.Compare(_T("�p�X�O")) == 0)
			{
				str.Format(_T("��^�p��"));
				m_stOpeStatusVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(str);
			break;

		// �^�p�J�n����
		case IDC_STATIC_OPESTARTVAL:
		// �^�p�I������
		case IDC_STATIC_OPESTOPVAL:
			str.Trim(_T(" "));
			if (!str.IsEmpty())
			{
				char	  ca_stime[40];				/* �p�X�^�p�J�n�����i�t���j   */
				CSatelliteData::ub_asc_to_asctime(obs_adr->sz_sts_name, ca_stime);

				CString strTime = _T("");
				CString tmp = CString(ca_stime);
				int idx = 0;
				if ((idx = tmp.Find(_T("."))) != -1)
				{
					strTime = tmp.Mid(0, idx);
				}
				else
				{
					strTime = tmp;
				}

				((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(strTime);
			}
			break;

		// ���M��
		case IDC_STATIC_SENDSTATUSVAL:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatusVal.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusVal.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// RF
		case IDC_STATIC_SENDSTATUS_RF:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatuRf.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatuRf.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// HOLD TONE
		case IDC_STATIC_SENDSTATUS_HOLDTONE:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatusHoldTone.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusHoldTone.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// RNG CONT
		case IDC_STATIC_SENDSTATUS_RNGCONT:
			if( (str.CompareNoCase(_T("LOCK")) == 0) || (str.CompareNoCase(_T("ON")) == 0) ||
				(str.CompareNoCase(_T("START")) == 0) )
			{
				m_stSendStatusRngCont.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusRngCont.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jCARR
		case IDC_STATIC_RECVNEW_CARR:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jSUB CARR
		case IDC_STATIC_RECVNEW_SUBCARR:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewSubCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewSubCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jVITERBI
		case IDC_STATIC_RECVNEW_VITERBI:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewViteRbi.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewViteRbi.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jBIT
		case IDC_STATIC_RECVNEW_BIT:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewBit.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewBit.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jFRAME
		case IDC_STATIC_RECVNEW_FRAME:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewFrame.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewFrame.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i���n�jCARR
		case IDC_STATIC_RECVOLD_CARR:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i���n�jSUB CARR
		case IDC_STATIC_RECVOLD_SUBCARR:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldSubCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldSubCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i���n�jVITERBI
		case IDC_STATIC_RECVOLD_VITERBI:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldViteRbi.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldViteRbi.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i���n�jBIT
		case IDC_STATIC_RECVOLD_BIT:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldBit.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldBit.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i���n�jFRAME
		case IDC_STATIC_RECVOLD_FRAME:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldFrame.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldFrame.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		default:
			break;
		}
	}

	UpdateData(FALSE);
}


/*============================================================================*/
// �ߑ��󋵃X�e�[�^�X�̍X�V(�P�c54m)
/*============================================================================*/
void CTabOperationView::UpdateStatus54m(UINT nStationID)
{
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mOperationParam[nStationID]) / sizeof(mOperationParam[nStationID][0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mOperationParam[nStationID][i].name;
		CString str;
		stControlValue val;
		val.ret = 0;
		val.type = mOperationParam[nStationID][i].type;

		if (mControlString.IsEmpty())
		{
			continue;
		}

		// �Ď��f�[�^����擾
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStationID, mControlString);
		if (obs_adr == NULL)
		{
			//TRACE("54m GetObsname_t()�G���[ �Ď���:%s\n", CStringA(mControlString));
			continue;
		}

		switch (val.type){
		case	eControlStatus:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		case	eControlNumeric:
			str.Format(_T("%d"), obs_adr->l_data);
			break;
		case	eControlDouble:
			str.Format(_T("%0.3f"), obs_adr->d_data);
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		// �����E�\���F�Z�b�g
		switch (mOperationParam[nStationID][i].id)
		{
		// �q����
		// �v��ID
		case IDC_STATIC_SATELLITEVAL:
		case IDC_STATIC_PLANVAL:
			((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(str);
			break;

		// �\��l
		case IDC_STATIC_FORECASTVAL:
			str.Trim(_T(" "));
			if (!str.IsEmpty())
			{
//				// �Ď��̃t�@�C���p�X����t�@�C������؂�o��
//				CString strFileName = str.Right(str.GetLength() - str.ReverseFind(_T('/')) - 1);
//				// �t�@�C��������g���q���O��
//				CString strYohoID = str.Right(str.GetLength() - str.ReverseFind(_T('.')) - 1);
//				// �t�@�C�����̍�����6�������"-"������
//				strYohoID.Insert(6, _T('-'));
//				((CColorStatic*)GetDlgItem(mOperationParam[i].id))->SetText(strYohoID);
				CString strYohoID = ConvertPredfileToId(str);
				((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(strYohoID);
			}
			break;

		// �^�p���
		case IDC_STATIC_OPESTATUSVAL:
			if (str.Compare(_T("�p�X��")) == 0)
			{
				str.Format(_T("�p�X�^�p��"));
				m_stOpeStatusVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else if (str.Compare(_T("�p�X�O")) == 0)
			{
				str.Format(_T("��^�p��"));
				m_stOpeStatusVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(str);
			break;

		// �^�p�J�n����
		case IDC_STATIC_OPESTARTVAL:
		// �^�p�I������
		case IDC_STATIC_OPESTOPVAL:
			str.Trim(_T(" "));
			if (!str.IsEmpty())
			{
				char	  ca_stime[40];				/* �p�X�^�p�J�n�����i�t���j   */
				CSatelliteData::ub_asc_to_asctime(obs_adr->sz_sts_name, ca_stime);

				CString strTime = _T("");
				CString tmp = CString(ca_stime);
				int idx = 0;
				if ((idx = tmp.Find(_T("."))) != -1)
				{
					strTime = tmp.Mid(0, idx);
				}
				else
				{
					strTime = tmp;
				}

				((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(strTime);
			}
			break;

		// ���M��
		case IDC_STATIC_SENDSTATUSVAL:
			if(str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatusVal.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusVal.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// RF
		case IDC_STATIC_SENDSTATUS_RF:
			if(str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatuRf.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatuRf.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// HOLD TONE
		case IDC_STATIC_SENDSTATUS_HOLDTONE:
			if(str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatusHoldTone.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusHoldTone.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// RNG CONT
		case IDC_STATIC_SENDSTATUS_RNGCONT:
			if(str.CompareNoCase(_T("START")) == 0)
			{
				m_stSendStatusRngCont.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusRngCont.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jCARR
		case IDC_STATIC_RECVNEW_CARR:
			if(str.CompareNoCase(_T("LOCK")) == 0)
			{
				m_stRecvNewCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jSUB CARR
		case IDC_STATIC_RECVNEW_SUBCARR:
			if(str.CompareNoCase(_T("LOCK")) == 0)
			{
				m_stRecvNewSubCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewSubCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jVITERBI
		case IDC_STATIC_RECVNEW_VITERBI:
			if(str.CompareNoCase(_T("LOCK")) == 0)
			{
				m_stRecvNewViteRbi.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewViteRbi.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jBIT
		case IDC_STATIC_RECVNEW_BIT:
			if(str.CompareNoCase(_T("LOCK")) == 0)
			{
				m_stRecvNewBit.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewBit.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		// X�ю�M�i�V�n�jFRAME
		case IDC_STATIC_RECVNEW_FRAME:
			if(str.CompareNoCase(_T("LOCK")) == 0)
			{
				m_stRecvNewFrame.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewFrame.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		default:
			break;
		}
	}

	UpdateData(FALSE);
}

/*============================================================================*/
/*! S�ю�M

-# �ߑ��󋵃X�e�[�^�X�̍X�V�i���V�Y34m�j

@param
@retval

*/
/*============================================================================*/
void CTabOperationView::UpdateStatus34m(UINT nStationID)
{
	int xtx = 0;

	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	CSatelliteData& sd = theApp.GetSatelliteData();

	int szControl = sizeof(mOperationParam[nStationID]) / sizeof(mOperationParam[nStationID][0]);

	for (int i = 0; i < szControl; i++)
	{
		CString mControlString = mOperationParam[nStationID][i].name;
		CString str;
		//		GetDlgItem(mOperationParam[i].id)->GetWindowText(str);
		stControlValue val;
		val.ret = 0;
		val.type = mOperationParam[nStationID][i].type;

		//		{
		//			CString strDebug;
		//			strDebug.Format(_T("%s(%d) :: mControlString(%s)"), __FILEW__, __LINE__, mControlString);
		//			CLogTraceEx::Write(_T(""), _T("CTabScaptView20"), _T("UpdateStatus"), _T("debug"), strDebug, nLogEx::debug);
		//		}

		if (mControlString.IsEmpty())
		{
			continue;
		}

		// S��/X�тɂ���ĊĎ�����؂�ւ���
		switch (mOperationParam[nStationID][i].id)
		{
		// ���M��
		case IDC_STATIC_SENDSTATUSVAL:
			// X�т̏ꍇ
			if (xtx == XTX_Y_ID)
			{
				((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(_T("X�ё��M"));
			}
			// S�т̏ꍇ
			else
			{
				((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(_T("S�ё��M"));
				mControlString.Format(_T("S-TX.RF_ON_DET_ANS"));
			}
			break;
		// RF
		case IDC_STATIC_SENDSTATUS_RF:
			// X�т̏ꍇ
			if (xtx == XTX_Y_ID)
			{
			}
			// S�т̏ꍇ
			else
			{
				mControlString.Format(_T("S-TX.RF_ON_DET_ANS"));
			}
			break;
		// HOLD TONE
		case IDC_STATIC_SENDSTATUS_HOLDTONE:
			// X�т̏ꍇ
			if (xtx == XTX_Y_ID)
			{
			}
			// S�т̏ꍇ
			else
			{
				mControlString.Format(_T("CMDOUT.HOLDTONE_STAT"));
			}
			break;
		// RNG CONT
		case IDC_STATIC_SENDSTATUS_RNGCONT:
			// X�т̏ꍇ
			if (xtx == XTX_Y_ID)
			{
			}
			// S�т̏ꍇ
			else
			{
				mControlString.Format(_T("SS-TCRDEM.RNG_CONT"));
			}
			break;
		default:
			break;
		}

		// �Ď��f�[�^����擾
		obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(nStationID, mControlString);

		if (obs_adr == NULL)
		{
			continue;
		}

		switch (val.type){
		case	eControlStatus:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		case	eControlNumeric:
			str.Format(_T("%d"), obs_adr->l_data);
			break;
		case	eControlDouble:
			str.Format(_T("%0.3f"), obs_adr->d_data);
			break;
		case	eControlTime:
			str.Format(_T("%s"), (LPCTSTR)CString(obs_adr->sz_sts_name));
			break;
		}

		// �����E�\���F�Z�b�g
		switch (mOperationParam[nStationID][i].id)
		{
			// �q����
			// �v��ID
		case IDC_STATIC_SATELLITEVAL:
			xtx = GetEqSatDB(str);

		case IDC_STATIC_PLANVAL:
			((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(str);
			break;

			// �\��l
		case IDC_STATIC_FORECASTVAL:
			str.Trim(_T(" "));
			if (!str.IsEmpty())
			{
				//				// �Ď��̃t�@�C���p�X����t�@�C������؂�o��
				//				CString strFileName = str.Right(str.GetLength() - str.ReverseFind(_T('/')) - 1);
				//				// �t�@�C��������g���q���O��
				//				CString strYohoID = str.Right(str.GetLength() - str.ReverseFind(_T('.')) - 1);
				//				// �t�@�C�����̍�����6�������"-"������
				//				strYohoID.Insert(6, _T('-'));
				//				((CColorStatic*)GetDlgItem(mOperationParam[i].id))->SetText(strYohoID);
				CString strYohoID = ConvertPredfileToId(str);
				((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(strYohoID);
			}
			break;

			// �^�p���
		case IDC_STATIC_OPESTATUSVAL:
			if (str.Compare(_T("�p�X��")) == 0)
			{
				str.Format(_T("�p�X�^�p��"));
				m_stOpeStatusVal.SetBackColor(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else if (str.Compare(_T("�p�X�O")) == 0)
			{
				str.Format(_T("��^�p��"));
				m_stOpeStatusVal.SetBackColor(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(str);
			break;

			// �^�p�J�n����
		case IDC_STATIC_OPESTARTVAL:
			// �^�p�I������
		case IDC_STATIC_OPESTOPVAL:
			str.Trim(_T(" "));
			if (!str.IsEmpty())
			{
				char	  ca_stime[40];				/* �p�X�^�p�J�n�����i�t���j   */
				CSatelliteData::ub_asc_to_asctime(obs_adr->sz_sts_name, ca_stime);

				CString strTime = _T("");
				CString tmp = CString(ca_stime);
				int idx = 0;
				if ((idx = tmp.Find(_T("."))) != -1)
				{
					strTime = tmp.Mid(0, idx);
				}
				else
				{
					strTime = tmp;
				}

				((CColorStatic*)GetDlgItem(mOperationParam[nStationID][i].id))->SetText(strTime);
			}
			break;

			// ���M��
		case IDC_STATIC_SENDSTATUSVAL:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatusVal.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusVal.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// RF
		case IDC_STATIC_SENDSTATUS_RF:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatuRf.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatuRf.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// HOLD TONE
		case IDC_STATIC_SENDSTATUS_HOLDTONE:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stSendStatusHoldTone.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusHoldTone.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// RNG CONT
		case IDC_STATIC_SENDSTATUS_RNGCONT:
			if( (str.CompareNoCase(_T("LOCK")) == 0) || (str.CompareNoCase(_T("ON")) == 0) ||
				(str.CompareNoCase(_T("START")) == 0) )
			{
				m_stSendStatusRngCont.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stSendStatusRngCont.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i�V�n�jCARR
		case IDC_STATIC_RECVNEW_CARR:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i�V�n�jSUB CARR
		case IDC_STATIC_RECVNEW_SUBCARR:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewSubCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewSubCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i�V�n�jVITERBI
		case IDC_STATIC_RECVNEW_VITERBI:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewViteRbi.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewViteRbi.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i�V�n�jBIT
		case IDC_STATIC_RECVNEW_BIT:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewBit.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewBit.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i�V�n�jFRAME
		case IDC_STATIC_RECVNEW_FRAME:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvNewFrame.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvNewFrame.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i���n�jCARR
		case IDC_STATIC_RECVOLD_CARR:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i���n�jSUB CARR
		case IDC_STATIC_RECVOLD_SUBCARR:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldSubCarr.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldSubCarr.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i���n�jVITERBI
		case IDC_STATIC_RECVOLD_VITERBI:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldViteRbi.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldViteRbi.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i���n�jBIT
		case IDC_STATIC_RECVOLD_BIT:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldBit.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldBit.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

			// X�ю�M�i���n�jFRAME
		case IDC_STATIC_RECVOLD_FRAME:
			if (str.CompareNoCase(_T("LOCK")) == 0 || str.CompareNoCase(_T("ON")) == 0)
			{
				m_stRecvOldFrame.SetBackColorEx(RGB(0, 255, 0), RGB(0, 255, 0));
			}
			else
			{
				m_stRecvOldFrame.SetBackColorEx(RGB(255, 255, 0), RGB(255, 255, 0));
			}
			break;

		default:
			break;
		}
	}

	UpdateData(FALSE);
}


/*============================================================================*/
/*! �^�p�󋵉��

-# �^�p�󋵂̍X�V

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CTabOperationView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_ThreadNormal:
	{
		UINT nStationID = mStation - eViewType5_EqOperation1;
		switch (nStationID)
		{
		case eStation_Usuda64:
		case eStation_Uchinoura20:
			UpdateStatus(nStationID);
			break;

		case eStation_Usuda642:
			UpdateStatus54m(nStationID);
			break;

		case eStation_Uchinoura34:
			UpdateStatus34m(nStationID);
			break;

		default:
			break;
		}

		break;
	}

	default:
		return CDialogEx::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}


/*============================================================================*/
/*! �^�p�󋵉��

-# �q���ŗLDB���f�[�^�擾���AX��/S�ё��M�Z�b�g

@param  �q����
@retval 0:�Y���q�����Ȃ�, 1:X�ё��M����, 2:X�ё��M�Ȃ�
*/
/*============================================================================*/
int CTabOperationView::GetEqSatDB(CString satname)
{
	struct satinf_t *satinf_adr;
	struct satinf_t *original_adr;
	char *sattbl_adr;

	struct xtx_t		*xtx_t_adr;		/* �w�ё��M�ݔ��g�p�L�����e�[�u���A�h���X */

	if (satname == _T(""))
	{
		return 0;
	}

	// DB�A�N�Z�X�N���X
	CDBAccess& dba = theApp.GetSatelliteData().GetDBAccessCls();

	vector<CString>			&saNameList = dba.GetSatNameList();
	map<CString, __int64>	&satAdrList = dba.GetSatAdrList();

	satinf_adr = (struct satinf_t *)((__int64*)satAdrList[satname]);
	original_adr = satinf_adr;
	sattbl_adr = (char *)satinf_adr;

	/*--------------------------------------*/
	/* �w�ё��M�ݔ��g�p�L�����擾 */
	/*--------------------------------------*/
	while (sattbl_adr != NULL)
	{
		satinf_adr = (struct satinf_t *)sattbl_adr;    /*satinf_t�ɕϊ�*/
		if (satinf_adr->l_id == ENDOFSAT_ID)
		{
			/*���ʂh�c���I���̏ꍇ*/
			break;
		}
		if (satinf_adr->l_id == XTX_ID)
		{
			/*���ʂh�c���u�w�ё��M�ݔ��g�p�L�����v�̏ꍇ*/
			xtx_t_adr = (struct xtx_t *)satinf_adr;
			return xtx_t_adr->l_x_tx;
			break;
		}
		sattbl_adr += satinf_adr->l_len;
	}

	return 0;
}
