/*============================================================================*/
/*! SessionStatus.cpp

-# �Z�b�V������ԉ��
*/
/*============================================================================*/
// SessionStatus.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SessionStatus.h"
#include "afxdialogex.h"


// CSessionStatus �_�C�A���O

IMPLEMENT_DYNAMIC(CSessionStatus, CDialogBase)

/*============================================================================*/
/*! CSessionStatus

-# �R���X�g���N�^

@param  pParent�F�e�E�B���h�E
@retval �Ȃ�
*/
/*============================================================================*/
CSessionStatus::CSessionStatus(CWnd* pParent /*=NULL*/)
: CDialogBase(CSessionStatus::IDD, pParent)
{
}

/*============================================================================*/
/*! CSessionStatus

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CSessionStatus::~CSessionStatus()
{
}

void CSessionStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SESSIONSTATUS_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CSessionStatus, CDialogBase)
	ON_COMMAND(ID_MENU_CLOSE_RC2, &CSessionStatus::OnMenuClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SESSIONSTATUS_TAB1, &CSessionStatus::OnTcnSelchangeSessionstatusTab1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


/*============================================================================*/
/*! CSessionStatus

-# �ǖ��ǉ�

@param	nIndex�F�C���f�b�N�X
@param	stationName�F�ǖ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatus::AddStation(UINT nIndex, CString stationName)
{
	m_station[nIndex].SetStationName(stationName);
	m_station[nIndex].SetStationID(nIndex);
}


// CSessionStatus ���b�Z�[�W �n���h���[


/*============================================================================*/
/*! CSessionStatus

-# ���j���[����̃t�@�C���N���[�Y���ɌĂ΂��

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CSessionStatus::OnMenuClose()
{
	//	OnOK();
	ShowWindow(SW_HIDE);
}

/*============================================================================*/
/*! CSessionStatus

-# �_�C�A���O����������

@param  �Ȃ�
@retval TRUE�F���������� / FALSE�F���������s
*/
/*============================================================================*/
BOOL CSessionStatus::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CRect rect;
	m_tabCtrl.GetClientRect(&rect);
	rect.top += m_nTabTopOffset;
	rect.left += m_nTabLeftOffset;
	// ���X�g�ɓo�^���ꂽ�ǂ̃^�u������
	for (int i = 0, nTabIndex = 0; i < eSessionType_Num; i++)
	{
#if 0
		CString stationName = m_station[i].GetStationName();
		if (stationName.CompareNoCase(_T("")) == 0)
			continue;
		if (!m_station[i].Create(CSessionStatusTab::IDD, this))
			return FALSE;
		m_station[i].MoveWindow(&rect, FALSE);
		m_tabCtrl.InsertItem(nTabIndex, m_station[i].GetStationName());
		nTabIndex++;
#else
		// ���\���C���f�b�N�X����V�Y20m, ���V�Y34m, �P�c64m, �P�c54m�̏��ɂ���
		UINT nViewIdx = mStationViewOrder[i];
		CString stationName = m_station[nViewIdx].GetStationName();
		if (stationName.CompareNoCase(_T("")) == 0)
			continue;
		if (!m_station[nViewIdx].Create(CSessionStatusTab::IDD, this))
			return FALSE;
		m_station[nViewIdx].MoveWindow(&rect, FALSE);
		m_tabCtrl.InsertItem(nTabIndex, m_station[nViewIdx].GetStationName());
		nTabIndex++;
#endif
	}

//	m_tabCtrl.SetCurSel(m_nStation);
	m_tabCtrl.SetCurSel(mStationViewCnv[m_nStation]);

	if (theApp.GetSelectStationMode() == eStation_1)
	{
		SetWindowSubCode(1);
	}
	else
	{
		SetWindowSubCode(0);
	}

	OnTcnSelchangeSessionstatusTab1(0, 0);

#if 1
	// �Ď��^�C�}�Z�b�g
	SetTimer(IDD_DLG_SESSIONSTATUS, m_nIntervalTime, NULL);
#endif
	UpdateData(FALSE);

	CenterWindowEx();

	// �Z�b�V������Ԃ��擾����
	GetSessionStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


/*============================================================================*/
/*! CSessionStatus

-# �^�u�I��ύX���ɌĂ΂��

@param  pNMHDR�F�ʒm���b�Z�[�W�|�C���^
@param	pResult�F���ʃ|�C���^
@retval �Ȃ�
*/
/*============================================================================*/
void CSessionStatus::OnTcnSelchangeSessionstatusTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = m_tabCtrl.GetCurSel();
	int nStation = theApp.GetSelectStation();

	// 1�ǃ��[�h�ȊO����сA1�ǃ��[�h�ł��ǃ^�u��v���̂݁A�I�������^�u��\������
	// ���\���C���f�b�N�X����V�Y20m, ���V�Y34m, �P�c64m, �P�c54m�̏��ɂ���
	if (theApp.GetSelectStationMode() != eStation_1 ||
		theApp.GetSelectStationMode() == eStation_1 && mStationViewOrder[sel] == nStation)
	{
		// �I������Ă���^�u��\������
		for (int i = 0, nTabIndex = 0; i < eSessionType_Num; i++)
		{
			UINT nViewIdx = mStationViewOrder[i];
			CString stationName = m_station[nViewIdx].GetStationName();
			if (stationName.CompareNoCase(_T("")) == 0)
				continue;

			if (sel == nTabIndex)
			{
				m_station[nViewIdx].ShowWindow(SW_SHOW);
				if (theApp.GetSelectStationMode() != eStation_1)
				{
					m_nStation = nViewIdx;
				}
			}
			else
			{
				m_station[nViewIdx].ShowWindow(SW_HIDE);
			}
			nTabIndex++;
		}
	}
	else
	{
//		m_tabCtrl.SetCurSel(m_nStation);
		m_tabCtrl.SetCurSel(mStationViewCnv[m_nStation]);
	}

	if (pResult)
		*pResult = 0;
}


/*============================================================================*/
/*! CSessionStatus

-# �^�C�}�C�x���g�ŌĂяo�����

@param	nIDEvent�F�C�x���gID
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatus::OnTimer(UINT_PTR nIDEvent)
{
	// ��ʕ\�����̂݁A�Z�b�V������Ԃ��擾����
	GetSessionStatus(FALSE, FALSE);

	CDialogBase::OnTimer(nIDEvent);
}


/*============================================================================*/
/*! CSessionStatus

-# ��ʕ\�����̂݁A�Z�b�V������Ԃ��擾����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatus::GetSessionStatus(BOOL bWaitComplete /*= TRUE*/, BOOL bForce /*= TRUE*/)
{
	if (this->IsWindowVisible() || bForce)
	{
		int sel = m_tabCtrl.GetCurSel();
		// ���\���C���f�b�N�X����V�Y20m, ���V�Y34m, �P�c64m, �P�c54m�̏��ɂ���
		if (mStationViewOrder[sel] == m_nStation)
		{
			m_station[m_nStation].GetSessionStatus(bWaitComplete);
		}
	}
}


/*============================================================================*/
/*! CSessionStatus

-# �E�B���h�E�j�����ɌĂяo�����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CSessionStatus::OnDestroy()
{
	CDialogBase::OnDestroy();

	KillTimer(1);
}


/*============================================================================*/
/*! CSessionStatus

-# �Z�b�V������ԕ\���X�V���ɏ�������

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
LRESULT CSessionStatus::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	eMessage_UpdateSessionStatus:
	{
		// �Z�b�V������ԕ\���X�V
		// 1�ǃ��[�h
		int nStation = theApp.GetSelectStation();

		// 1�ǃ��[�h��
		if (theApp.GetSelectStationMode() == eStation_1)
		{
			m_station[nStation].UpdateSessionDisplay();
		}
		// 1�ǃ��[�h�ȊO
		else
		{
			for (int i = 0, nTabIndex = 0; i < eSessionType_Num; i++)
			{
				CString stationName = m_station[i].GetStationName();
				if (stationName.CompareNoCase(_T("")) == 0)
					continue;

				m_station[i].UpdateSessionDisplay();
				nTabIndex++;
			}
		}
	}
		break;

	default:
		return CDialogBase::WindowProc(message, wParam, lParam);
	}

	return TRUE;
}

