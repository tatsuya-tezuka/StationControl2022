/*============================================================================*/
/*! AntStanbyOriented.cpp

-# �q���ߑ���ʁ@ANT�҂��󂯎w��(54m)
*/
/*============================================================================*/
// AntStanbyOriented.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "AntStanbyOriented.h"
#include "afxdialogex.h"

// CAntStanbyOriented �_�C�A���O

IMPLEMENT_DYNAMIC(CAntStanbyOriented, CDialogBase)

/*============================================================================*/
// �R���X�g���N�^
/*============================================================================*/
CAntStanbyOriented::CAntStanbyOriented(CWnd* pParent /*=NULL*/)
	: CDialogBase(CAntStanbyOriented::IDD, pParent)
{

}

/*============================================================================*/
// �f�X�g���N�^
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


// CAntStanbyOriented ���b�Z�[�W �n���h���[

/*============================================================================*/
// �`�掞�ɌĂ΂�A�w��R���g���[���̔w�i��h��Ԃ��u���V�̃n���h����Ԃ�
/*============================================================================*/
HBRUSH CAntStanbyOriented::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		// �ÓI�e�L�X�g�w�i�F
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(DC_BRUSH);
		hbr = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
		break;

	case CTLCOLOR_DLG:
		// �_�C�A���O�̔w�i�F
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
// �_�C�A���O����������
/*============================================================================*/
BOOL CAntStanbyOriented::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	UpdateSatellite(FALSE);	//�T���@���X�g�̍쐬

	// �p�X�J�n�����A�p�X�I������������ YYYY-MM-DD hh:mm:ss
	m_strPassStart = _T("");
	m_strPassEnd = _T("");

	// �Ď��f�[�^�擾
	UpdateDlg(_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
// �T���@�̍X�V
// �\��l�t�@�C���̍X�V
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
		// �Ď��f�[�^�擾�X�V
		UpdateDlg(fname);
		return TRUE;
	}

	return CDialogBase::WindowProc(message, wParam, lParam);
}

/*============================================================================*/
// �T���@�R���{�{�b�N�X�I��ύX
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
	vector<stPredListData>& predlist = theApp.GetSatelliteData().GetPredList();		//�\��l�ꗗ
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
// �A���e�i�\��l�{�^���N���b�N
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
	// �\��l�ꗗ��ʌĂяo��
	theApp.m_pMainWnd->PostMessage(eMessage_OpenForecastListDlg, (WPARAM)this, (LPARAM)&m_satellilteName);
}

/*============================================================================*/
// ���s�{�^���N���b�N
/*============================================================================*/
void CAntStanbyOriented::OnBnClickedAntstanbyorientedExecButton()
{
	// �ݒ肵���A���e�i�\��l�̓��e�ɉ����āA�A���e�i�ݔ����w�����鐧������s����B
	// SEND_CONTROL_UI.sh �ǖ� CTRL.PRED_CHANGE_FREQ �q����@�\��l�t�@�C����
	// �����g������i�\��l�؂�ւ��j�̗\��l�ݒ�Ɠ���

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

	/* ������ */
	CString ctrl = _T("");
	ctrl.Format(_T("CTRL.PRED_CHANGE_FREQ %s@%s"), (LPCTSTR)satellite, (LPCTSTR)predfile);
	TRACE("[%s]\n", CStringA(ctrl));
	theApp.SendControl(ctrl, _T(""), _T(""), _T(""));
}

/*============================================================================*/
// �T���@���X�g�̍X�V
/*============================================================================*/
void CAntStanbyOriented::UpdateSatellite(BOOL update)
{
	// �T���@���X�g�̍쐬
	vector<CString>::iterator itr;
	CString satellite = _T("");
	int pos;
	BOOL find;

	if(update == TRUE)
	{
		pos = m_SatelliteCmb.GetCurSel();
		if(pos >= 0)
		{
			satellite = mSatelliteNameList[pos];	//���X�g�X�V�O�̒T���@
		}
	}

	m_SatelliteCmb.ResetContent();

	GetSatelliteNameList();										//�q�������X�g
	pos = (-1);
	find = FALSE;
	for(itr = mSatelliteNameList.begin(); itr != mSatelliteNameList.end(); itr++)
	{
		m_SatelliteCmb.AddString((*itr));
		pos++;
		if(update == TRUE)
		{
			//�X�V�O�̒T���@��\��
			if(satellite.CompareNoCase((*itr)) == 0)
			{
				m_SatelliteCmb.SetCurSel(pos);
				find = TRUE;
			}
		}
	}

	// �擪�̒T���@���A�N�e�B�u�ɂ���
	if( (update != TRUE) || 						//�N����
		(mSatelliteNameList.empty() == TRUE) ||		//�q�������X�g����
		(pos < 0) )									//�X�V�O�̒T���@���Ȃ�
	{
		m_SatelliteCmb.SetCurSel(0);
	}

	if( (satellite.GetLength() != 0) && (find != TRUE) )
	{
		// �T���@���ύX�̏ꍇ�́A�\��l�ꗗ����̎擾���ʂ͎g���Ȃ�
		TRACE("satellite old:[%s] \n", satellite);
		MessageBox(_T("�T���@���ύX�ɂȂ��Ă��܂��B\n�A���e�i�\��l���擾���Ȃ����Ă��������B"), _T("�m�F"), MB_OK);
	}

	return;
}

/*============================================================================*/
// �\��l�ꗗ����q�������X�g���擾����
/*============================================================================*/
void CAntStanbyOriented::GetSatelliteNameList()
{
	mSatelliteNameList.clear();

	vector<stPredListData>::iterator itr;
	vector<CString>::iterator itrName;

	CGraphMutex::Lock(eFile);
	vector<stPredListData>& predlist = theApp.GetSatelliteData().GetPredList();		//�\��l�ꗗ
	for (itr = predlist.begin(); itr != predlist.end(); itr++)
	{
		// �q���������ɉq�������X�g�ɂ��邩���ׂ�
		itrName = find(mSatelliteNameList.begin(), mSatelliteNameList.end(), (*itr).strSatellite);
		if (itrName == mSatelliteNameList.end())
		{
			// �����ꍇ�͋ǖ����X�g�ɒǉ�
			mSatelliteNameList.push_back((*itr).strSatellite);
		}
	}
	CGraphMutex::Unlock(eFile);

	return;
}

/*============================================================================*/
// �Ď��f�[�^���擾���ĉ�ʕ\������
/*============================================================================*/
void CAntStanbyOriented::UpdateDlg(CString fname)
{
	// �Ď��f�[�^�擾
	GetKanshiData(fname);

	UpdateData(FALSE);

	return;
}

/*============================================================================*/
// �Ď�����T���@�A�\��l�t�@�C���A�p�X�J�n�����A�p�X�I���������擾���X�V����
/*============================================================================*/
void CAntStanbyOriented::GetKanshiData(CString fname)
{
	// �T���@�擾
	CString	strNewSatellite = _T("");
	CSatelliteData& sd = theApp.GetSatelliteData();
	UINT sta = theApp.GetSelectStation();
	CEngValCnv::obsname_t *obs_adr;	// �Ď����ڃA�h���X
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T(EISEI_NAME));
	if( (obs_adr != NULL) && (obs_adr->l_inflg == INFLG_ON) )
	{
		strNewSatellite = CString(obs_adr->sz_sts_name);
	}
	else
	{
		TRACE("�Ď��擾�G���[:[%s] \n", EISEI_NAME);
	}

	CString strFilePath = fname;	//eMessage_UpdateForecastFilePath��lParam���b��ŃZ�b�g(�Ď��f�[�^�̎擾���s�p)
	// �A���e�i�\��l�t�@�C��
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T(PREDFILE_NAME));
	if( (obs_adr != NULL) && (obs_adr->l_inflg == INFLG_ON) )
	{
		TCHAR	ca_fpath[1024];				// �\��l�t�@�C����(�t���p�X)
		if (MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, obs_adr->sz_sts_name, -1, ca_fpath, _MAX_PATH) != 0)
		{
			strFilePath = CString(ca_fpath);
		}
	}
	else
	{
		TRACE("�Ď��擾�G���[:[%s] \n", PREDFILE_NAME);
	}
	SetFileName(strFilePath);

	// �p�X�J�n����
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T(START_TIME));
	if( (obs_adr != NULL) && (obs_adr->l_inflg == INFLG_ON) )
	{
		char	ca_stime[40];				// �p�X�^�p�J�n����(�t��)
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
		TRACE("�Ď��擾�G���[:[%s] \n", START_TIME);
	}

	// �p�X�I������
	obs_adr = (CEngValCnv::obsname_t *)sd.GetDBAccessCls().GetObsname_t(sta, _T(STOP_TIME));
	if( (obs_adr != NULL) && (obs_adr->l_inflg == INFLG_ON) )
	{
		char	ca_etime[40];				// �p�X�^�p�I������(�t��)
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
		TRACE("�Ď��擾�G���[:[%s] \n", STOP_TIME);
	}

	if(strNewSatellite.GetLength() == 0)
	{
		return;
	}

	//���s�̒T���@
	CString strOldSatellite = _T("");
	int pos = m_SatelliteCmb.GetCurSel();
	if(pos >= 0)
	{
		strOldSatellite = mSatelliteNameList[pos];	//�T���@���X�g
	}

	//�Ď��f�[�^����擾�����T���@
	pos = 0;
	vector<CString>::iterator itr;
	for(itr = mSatelliteNameList.begin(); itr != mSatelliteNameList.end(); itr++)
	{
		//�Ď��f�[�^����擾�����T���@��\��
		if(strNewSatellite.CompareNoCase((*itr)) == 0)
		{
			m_SatelliteCmb.SetCurSel(pos);
			break;
		}
		pos++;
	}

	//�T���@���ύX�ɂȂ�����?
	if( (strOldSatellite.GetLength() != 0) ||
		(strOldSatellite.CompareNoCase(strNewSatellite) != 0) )
	{
		// �T���@���ύX�̏ꍇ�́A�\��l�ꗗ����̎擾���ʂ͎g���Ȃ�
		MessageBox(_T("�T���@���ύX�ɂȂ��Ă��܂��B\n�A���e�i�\��l���擾���Ȃ����Ă��������B"), _T("�m�F"), MB_OK);
	}

	return;
}

/*============================================================================*/
// �\��l�t�@�C�������{�^���ɃZ�b�g����
/*============================================================================*/
void CAntStanbyOriented::SetFileName(CString filepath)
{
	//�t�@�C�����̂ݎ�o��
	CString strFileName = filepath.Right(filepath.GetLength() - filepath.ReverseFind(_T('/')) - 1);

	//�g���q�̒u��(->.anp)
	int idx = strFileName.Find(_T("."));
	if(idx > 0)
	{
		strFileName = strFileName.Mid(0, idx) + _T(".anp");
	}

	m_buttonAntForecast.SetWindowTextW(strFileName);

	return;
}

