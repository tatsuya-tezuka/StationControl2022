// ControlManager.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "ControlManager.h"
#include "afxdialogex.h"


// CControlManager �_�C�A���O

IMPLEMENT_DYNAMIC(CControlManager, CDialogEx)

CControlManager::CControlManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlManager::IDD, pParent)
{

}

CControlManager::~CControlManager()
{
}

void CControlManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlManager, CDialogEx)
END_MESSAGE_MAP()


// CControlManager ���b�Z�[�W �n���h���[
