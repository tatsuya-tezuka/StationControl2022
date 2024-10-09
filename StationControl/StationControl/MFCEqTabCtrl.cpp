// MFCEqTabCtrl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "MFCEqTabCtrl.h"


// CMFCEqTabCtrl

IMPLEMENT_DYNAMIC(CMFCEqTabCtrl, CMFCTabCtrl)

CMFCEqTabCtrl::CMFCEqTabCtrl()
: m_nTabType(eTabType_Normal)
, m_pProp(NULL)
{

}

CMFCEqTabCtrl::~CMFCEqTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMFCEqTabCtrl, CMFCTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMFCEqTabCtrl ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! �ݔ��󋵃r���[

-# �^�u�R���g���[���̏�����

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
void CMFCEqTabCtrl::Initialize()
{
	ModifyTabStyle(CMFCTabCtrl::STYLE_3D);
	//ModifyTabStyle(CMFCTabCtrl::STYLE_FLAT);
	SetLocation(CMFCTabCtrl::LOCATION_TOP);
	for (int i = 0; i < GetTabsNum(); i++){
		if (i == 0){
			GetTabWnd(i)->ShowWindow(SW_SHOW);
			//SetTabBkColor(i, RGB(121, 210, 231));
		}
		else{
			GetTabWnd(i)->ShowWindow(SW_HIDE);
			//SetTabBkColor(i, RGB(192, 192, 192));
		}
	}

	SetActiveTab(0);
	RecalcLayout();
	RedrawWindow();
}


void CMFCEqTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int pos = GetTabFromPoint(point);
	if (pos < 0){
		CMFCTabCtrl::OnLButtonDown(nFlags, point);
		return;
	}

	int posNext = pos;

	for (int i = 0; i < GetTabsNum(); i++){
		switch (m_nTabType)
		{
		case eTabType_SummaryGraph:
			if (i == 0)
			{
				GetTabWnd(i)->ShowWindow(SW_SHOW);
				posNext = i;
			}
			else
			{
				GetTabWnd(i)->ShowWindow(SW_HIDE);
				if (pos == 1)
				{
					CPropertiesData& prop = theApp.GetSatelliteData().GetSummaryPropertiesData(m_pProp->m_nStationID, m_pProp->m_nViewID);
					if (prop.nGraphIdx == eSummaryWndSetType_XSP && m_pProp->m_nStationID != eStation_Uchinoura20 ||
						prop.nGraphIdx == eSummaryWndSetType_KASP && m_pProp->m_nStationID == eStation_Usuda642)
					{
						// �T�}���X�y�N�g�����v���p�e�B�_�C�A���O���J���ʒm
						theApp.m_pMainWnd->PostMessage(eMessage_OpenSummarySpectrumSetDlg, (WPARAM)m_pProp, (LPARAM)this);
					}
					else
					{
						// �T�}���g�����h�v���p�e�B�_�C�A���O���J���ʒm
						theApp.m_pMainWnd->PostMessage(eMessage_OpenSummaryTrendSetDlg, (WPARAM)m_pProp, (LPARAM)this);
					}
				}
			}
			break;

		case eTabType_Normal:
		default:
			if (i == pos)
			{
				GetTabWnd(i)->ShowWindow(SW_SHOW);
				posNext = i;
				if (theApp.GetServiceMode() != eServiceMode_oldremote)
				{
					// �������[�g�����[�h�Ŗ����ꍇ�A�^�u���������E�B���h�E�Ƀt�H�[�J�X�����Ă�
					GetTabWnd(i)->SetFocus();
				}
				//SetTabBkColor(i, RGB(121, 210, 231));
			}
			else
			{
				GetTabWnd(i)->ShowWindow(SW_HIDE);
				//SetTabBkColor(i, RGB(192, 192, 192));
			}
			break;
		}
	}

	SetActiveTab(posNext);
	RecalcLayout();
	RedrawWindow();
}
